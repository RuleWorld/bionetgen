#include "actions/ActionDispatch.hpp"

#include <algorithm>
#include <cctype>
#include <cmath>
#include <cstdlib>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <optional>
#include <sstream>
#include <string>
#include <unordered_map>
#include <vector>

#include "ast/Parameter.hpp"
#include "engine/NetworkGenerator.hpp"
#include "engine/OdeIntegrator.hpp"
#include "io/NetWriter.hpp"
#include "io/XmlWriter.hpp"
#include "io/BnglWriter.hpp"
#include "io/NetReader.hpp"
#include "io/SbmlWriter.hpp"
#include "io/MatlabWriter.hpp"
#include "io/ContactMapWriter.hpp"

namespace bng::actions {

namespace {

std::string trim(std::string value) {
    auto first = std::find_if_not(value.begin(), value.end(), [](unsigned char c) {
        return std::isspace(c) != 0;
    });
    auto last = std::find_if_not(value.rbegin(), value.rend(), [](unsigned char c) {
        return std::isspace(c) != 0;
    }).base();
    if (first >= last) {
        return {};
    }
    return std::string(first, last);
}

std::string stripQuotes(const std::string& text) {
    if (text.size() >= 2 &&
        ((text.front() == '"' && text.back() == '"') || (text.front() == '\'' && text.back() == '\''))) {
        return text.substr(1, text.size() - 2);
    }
    return text;
}

std::string lowercase(std::string value) {
    std::transform(value.begin(), value.end(), value.begin(), [](unsigned char c) {
        return static_cast<char>(std::tolower(c));
    });
    return value;
}

double parseScalarValue(const std::string& text, ast::Model& model) {
    std::string value = stripQuotes(trim(text));
    std::replace(value.begin(), value.end(), 'D', 'E');
    std::replace(value.begin(), value.end(), 'd', 'e');

    std::size_t consumed = 0;
    try {
        const double parsed = std::stod(value, &consumed);
        if (consumed == value.size()) {
            return parsed;
        }
    } catch (const std::exception&) {
        // Try parameter name fallback below.
    }

    if (model.getParameters().contains(value)) {
        return model.getParameters().evaluate(value);
    }

    // Try evaluating as expression with parameter resolution
    try {
        // Simple recursive evaluator for expressions like "2e-9*NA*V"
        double result = 1.0;
        std::string token;
        std::istringstream stream(value);
        bool allResolved = true;
        while (std::getline(stream, token, '*')) {
            std::string t = trim(token);
            if (t.empty()) continue;
            std::size_t pos = 0;
            try {
                double val = std::stod(t, &pos);
                if (pos == t.size()) { result *= val; continue; }
            } catch (...) {}
            if (model.getParameters().contains(t)) {
                result *= model.getParameters().evaluate(t);
            } else {
                allResolved = false;
                break;
            }
        }
        if (allResolved) return result;
    } catch (...) {}

    throw std::runtime_error("Unsupported scalar action value: '" + text + "'");
}

std::string readArgument(const ast::Action& action, const std::string& key, const std::string& fallback = {}) {
    const auto found = action.arguments.find(key);
    if (found == action.arguments.end()) {
        return fallback;
    }
    return found->second;
}

std::string resolveSimulationMethod(const ast::Action& action) {
    std::string name = lowercase(action.name);
    if (name == "simulate_ode") {
        return "cvode";
    }
    if (name == "simulate_ssa") {
        return "ssa";
    }

    std::string method = lowercase(stripQuotes(readArgument(action, "method", "ode")));
    if (method == "ode") {
        return "cvode";
    }
    return method;
}

std::optional<std::size_t> findSpeciesIndex(const engine::GeneratedNetwork& network, const std::string& target) {
    const std::string needle = stripQuotes(trim(target));
    for (std::size_t i = 0; i < network.species.size(); ++i) {
        const auto& species = network.species.get(i);
        const auto canonical = species.getSpeciesGraph().toString();
        std::string full;
        if (!species.getCompartment().empty()) {
            full += "@" + species.getCompartment() + "::";
        }
        if (species.isConstant()) {
            full += "$";
        }
        full += canonical;

        if (needle == canonical || needle == full) {
            return i;
        }
    }
    return std::nullopt;
}

std::vector<double> snapshotConcentrations(const engine::GeneratedNetwork& network) {
    std::vector<double> values;
    values.reserve(network.species.size());
    for (const auto& species : network.species.all()) {
        values.push_back(species.getAmount());
    }
    return values;
}

void restoreConcentrations(engine::GeneratedNetwork& network, const std::vector<double>& values) {
    const auto count = std::min(network.species.size(), values.size());
    for (std::size_t i = 0; i < count; ++i) {
        network.species.get(i).setAmount(values[i]);
    }
}

std::string simulationPrefix(const ast::Action& action, const std::filesystem::path& sourcePath, std::optional<std::size_t> scanIndex = std::nullopt) {
    std::string prefix = stripQuotes(readArgument(action, "prefix", sourcePath.stem().string()));
    const auto suffix = stripQuotes(readArgument(action, "suffix", ""));
    if (!suffix.empty()) {
        prefix += "_" + suffix;
    }
    if (scanIndex.has_value()) {
        prefix += "_scan" + std::to_string(*scanIndex + 1);
    }
    return prefix;
}

void runSimulation(
    ast::Model& model,
    const ast::Action& action,
    const std::filesystem::path& sourcePath,
    engine::GeneratedNetwork& network,
    bool verbose,
    std::vector<double>& lastSimulationState) {
    const auto method = resolveSimulationMethod(action);
    const auto tEnd = stripQuotes(readArgument(action, "t_end", ""));
    const auto nSteps = stripQuotes(readArgument(action, "n_steps", stripQuotes(readArgument(action, "n_output_steps", ""))));
    if (tEnd.empty() || nSteps.empty()) {
        throw std::runtime_error("simulate requires t_end and n_steps (or n_output_steps)");
    }

    // Parse simulation options
    engine::OdeOptions opts;
    opts.tEnd = parseScalarValue(tEnd, model);
    opts.nSteps = static_cast<std::size_t>(parseScalarValue(nSteps, model));

    // Parse method (match BNG2 defaults)
    if (method == "cvode" || method == "ode") {
        opts.method = "cvode";
    } else if (method == "ssa") {
        opts.method = "ssa";
    } else if (method == "euler") {
        opts.method = "euler";
    } else if (method == "rk4") {
        opts.method = "rk4";
    } else {
        opts.method = "cvode";  // Default to CVODE (matches BNG2)
    }

    // Parse seed for SSA
    if (opts.method == "ssa") {
        const auto seedText = readArgument(action, "seed", "");
        if (!seedText.empty()) {
            opts.seed = static_cast<unsigned int>(parseScalarValue(seedText, model));
        }
    }

    // Parse tolerances if provided
    const auto atolText = readArgument(action, "atol", "");
    if (!atolText.empty()) {
        opts.atol = parseScalarValue(atolText, model);
    }
    const auto rtolText = readArgument(action, "rtol", "");
    if (!rtolText.empty()) {
        opts.rtol = parseScalarValue(rtolText, model);
    }

    // Parse steady_state option (BNG2 parity)
    const auto steadyStateText = lowercase(stripQuotes(readArgument(action, "steady_state", "0")));
    opts.steadyState = (steadyStateText == "1" || steadyStateText == "true");
    if (opts.steadyState) {
        opts.steadyStateTol = opts.atol;  // Use atol for steady-state check
    }

    // Parse stop_if expression (BNG2 parity)
    opts.stopIf = stripQuotes(readArgument(action, "stop_if", ""));

    // Parse continue flag (BNG2 parity)
    const auto continueText = lowercase(stripQuotes(readArgument(action, "continue", "0")));
    bool continueSimulation = (continueText == "1" || continueText == "true");

    if (verbose) {
        std::cerr << "[bng_cpp] Simulating with method=" << opts.method
                  << " t_end=" << opts.tEnd
                  << " n_steps=" << opts.nSteps
                  << " atol=" << opts.atol
                  << " rtol=" << opts.rtol;
        if (opts.steadyState) {
            std::cerr << " steady_state=1";
        }
        if (!opts.stopIf.empty()) {
            std::cerr << " stop_if=\"" << opts.stopIf << "\"";
        }
        if (continueSimulation) {
            std::cerr << " continue=1";
        }
        std::cerr << "\n";
    }

    // If continuing from previous simulation, restore state
    if (continueSimulation && !lastSimulationState.empty()) {
        if (lastSimulationState.size() == network.species.size()) {
            for (std::size_t i = 0; i < network.species.size(); ++i) {
                network.species.get(i).setAmount(lastSimulationState[i]);
            }
            if (verbose) {
                std::cerr << "[bng_cpp] Continuing from previous simulation state\n";
            }
        }
    }

    // Run native ODE integration
    engine::OdeIntegrator integrator(model, network);
    auto result = integrator.integrate(opts);

    // Save final state for potential continue
    if (!result.concentrations.empty()) {
        lastSimulationState = result.concentrations.back();
    }

    // Write output files
    const auto prefix = simulationPrefix(action, sourcePath);
    const auto outputPrefix = sourcePath.parent_path() / prefix;
    integrator.writeOutputFiles(outputPrefix.string(), result);

    if (verbose) {
        std::cerr << "[bng_cpp] Wrote " << outputPrefix.string() << ".cdat and "
                  << outputPrefix.string() << ".gdat\n";
    }
}

} // namespace

void ActionDispatch::execute(ast::Model& model, const std::filesystem::path& sourcePath, bool verbose) {
    engine::NetworkGenerator generator(model);
    std::optional<engine::GeneratedNetwork> network;
    std::unordered_map<std::string, std::vector<double>> savedConcentrations;
    std::unordered_map<std::string, std::unordered_map<std::string, double>> savedParameters;

    // For continue simulation: track last simulation state
    std::vector<double> lastSimulationState;

    const auto ensureNetwork = [&]() {
        if (!network.has_value()) {
            network = generator.generate(sourcePath);
        }
    };

    const auto writeCurrentNetwork = [&]() {
        const auto outputPath = sourcePath.parent_path() / (sourcePath.stem().string() + ".net");
        io::NetWriter::write(outputPath, model, *network);
        return outputPath;
    };

    for (const auto& action : model.getActions()) {
        // Normalize action name to lowercase for case-insensitive matching
        std::string actionName = lowercase(action.name);

        if (actionName == "readfile") {
            const auto filepath = stripQuotes(readArgument(action, "file", ""));
            if (filepath.empty()) {
                throw std::runtime_error("readFile requires 'file' argument");
            }

            // Determine file type
            std::filesystem::path path(filepath);
            if (!path.is_absolute()) {
                path = sourcePath.parent_path() / filepath;
            }

            if (path.extension() == ".net") {
                // Parse .net file
                auto parseResult = io::NetReader::parse(path);
                if (!parseResult.success) {
                    throw std::runtime_error("Failed to read .net file: " + parseResult.error);
                }

                // Merge parsed data into current model
                for (const auto& [name, value] : parseResult.parameters) {
                    model.getParameters().add(ast::Parameter(name, ast::Expression::number(value)));
                }
                model.getParameters().evaluateAll();

                if (verbose) {
                    std::cerr << "[bng_cpp] Read .net file: " << path << "\n";
                    std::cerr << "[bng_cpp]   Parameters: " << parseResult.parameters.size() << "\n";
                    std::cerr << "[bng_cpp]   Species: " << parseResult.species.size() << "\n";
                    std::cerr << "[bng_cpp]   Reactions: " << parseResult.reactions.size() << "\n";
                }

                // NOTE: Full integration of species and reactions from .net into the Model
                // would require reconstructing the network. For now, readFile primarily
                // updates parameters, which is the main use case.
            } else if (path.extension() == ".bngl") {
                // Parse .bngl file - would need to use the parser
                throw std::runtime_error("readFile for .bngl not yet implemented - parse separately");
            } else {
                throw std::runtime_error("readFile: unsupported file type: " + path.extension().string());
            }

            continue;
        }

        if (actionName == "generate_network") {
            network = generator.generate(sourcePath);
            continue;
        }

        if (actionName == "setparameter") {
            const auto target = stripQuotes(readArgument(action, "target", ""));
            const auto valueText = readArgument(action, "value", "");
            if (target.empty()) {
                throw std::runtime_error("setParameter requires a target parameter name");
            }

            const double value = parseScalarValue(valueText, model);
            model.getParameters().add(ast::Parameter(target, ast::Expression::number(value)));
            model.getParameters().evaluateAll();

            if (network.has_value()) {
                network = generator.generate(sourcePath);
            }
            continue;
        }

        if (actionName == "saveparameters") {
            const auto label = stripQuotes(readArgument(action, "value", "default"));
            std::unordered_map<std::string, double> snapshot;
            for (const auto& param : model.getParameters().all()) {
                snapshot[param.getName()] = param.getValue();
            }
            savedParameters[label] = snapshot;
            if (verbose) {
                std::cerr << "[bng_cpp] Saved parameters with label '" << label << "'\n";
            }
            continue;
        }

        if (actionName == "resetparameters") {
            const auto label = stripQuotes(readArgument(action, "value", "default"));
            const auto found = savedParameters.find(label);
            if (found == savedParameters.end()) {
                throw std::runtime_error("resetParameters label not found: " + label);
            }
            for (const auto& [name, value] : found->second) {
                model.getParameters().add(ast::Parameter(name, ast::Expression::number(value)));
            }
            model.getParameters().evaluateAll();
            if (network.has_value()) {
                network = generator.generate(sourcePath);
            }
            if (verbose) {
                std::cerr << "[bng_cpp] Reset parameters from label '" << label << "'\n";
            }
            continue;
        }

        if (actionName == "setconcentration") {
            ensureNetwork();
            const auto target = readArgument(action, "target", "");
            const auto valueText = readArgument(action, "value", "");
            const double value = parseScalarValue(valueText, model);
            const auto found = findSpeciesIndex(*network, target);
            if (!found.has_value()) {
                throw std::runtime_error("setConcentration target species not found: " + stripQuotes(target));
            }
            network->species.get(*found).setAmount(value);
            writeCurrentNetwork();
            continue;
        }

        if (actionName == "saveconcentrations") {
            ensureNetwork();
            const auto label = stripQuotes(readArgument(action, "value", "default"));
            savedConcentrations[label] = snapshotConcentrations(*network);
            continue;
        }

        if (actionName == "resetconcentrations") {
            ensureNetwork();
            const auto label = stripQuotes(readArgument(action, "value", "default"));
            const auto found = savedConcentrations.find(label);
            if (found != savedConcentrations.end()) {
                restoreConcentrations(*network, found->second);
            } else {
                // Perl BNG2 behavior: reset to initial seed species concentrations
                const auto& seeds = model.getSeedSpecies();
                for (std::size_t i = 0; i < network->species.size(); ++i) {
                    if (i < seeds.size()) {
                        try {
                            const double amount = seeds[i].getAmount().evaluate(
                                [&](const std::string& name) { return model.getParameters().evaluate(name); });
                            network->species.get(i).setAmount(amount);
                        } catch (...) {
                            network->species.get(i).setAmount(0.0);
                        }
                    } else {
                        network->species.get(i).setAmount(0.0);
                    }
                }
            }
            writeCurrentNetwork();
            continue;
        }

        if (actionName == "addconcentration") {
            ensureNetwork();
            const auto target = readArgument(action, "target", "");
            const auto valueText = readArgument(action, "value", "");
            const double value = parseScalarValue(valueText, model);
            const auto found = findSpeciesIndex(*network, target);
            if (!found.has_value()) {
                throw std::runtime_error("addConcentration target species not found: " + stripQuotes(target));
            }
            network->species.get(*found).setAmount(network->species.get(*found).getAmount() + value);
            writeCurrentNetwork();
            continue;
        }

        if (actionName == "simulate" || action.name == "simulate_ode" || action.name == "simulate_ssa") {
            ensureNetwork();
            // Don't re-write .net file - already written by generate_network
            // Re-writing would cause duplicate parameters and corrupt stat factors
            runSimulation(model, action, sourcePath, *network, verbose, lastSimulationState);
            continue;
        }

        if (actionName == "parameter_scan") {
            const auto parameterName = stripQuotes(readArgument(action, "parameter", ""));
            if (parameterName.empty()) {
                throw std::runtime_error("parameter_scan requires parameter argument");
            }

            const auto minValue = parseScalarValue(readArgument(action, "par_min", ""), model);
            const auto maxValue = parseScalarValue(readArgument(action, "par_max", ""), model);
            const auto points = static_cast<std::size_t>(std::max(1.0, parseScalarValue(readArgument(action, "n_scan_pts", "1"), model)));
            const auto logScale = lowercase(stripQuotes(readArgument(action, "log_scale", "0"))) == "1"
                || lowercase(stripQuotes(readArgument(action, "log_scale", "0"))) == "true";

            ast::Action simulateAction = action;
            simulateAction.name = "simulate";
            simulateAction.arguments["method"] = readArgument(action, "method", "ode");

            for (std::size_t i = 0; i < points; ++i) {
                const double alpha = points == 1 ? 0.0 : static_cast<double>(i) / static_cast<double>(points - 1);
                double value = 0.0;
                if (logScale) {
                    if (minValue <= 0.0 || maxValue <= 0.0) {
                        throw std::runtime_error("parameter_scan with log_scale requires positive par_min and par_max");
                    }
                    value = std::exp(std::log(minValue) + alpha * (std::log(maxValue) - std::log(minValue)));
                } else {
                    value = minValue + alpha * (maxValue - minValue);
                }

                model.getParameters().add(ast::Parameter(parameterName, ast::Expression::number(value)));
                model.getParameters().evaluateAll();
                network = generator.generate(sourcePath);
                writeCurrentNetwork();  // Still write .net file for compatibility

                simulateAction.arguments["prefix"] = simulationPrefix(action, sourcePath, i);
                runSimulation(model, simulateAction, sourcePath, *network, verbose, lastSimulationState);
            }
            continue;
        }

        if (actionName == "bifurcate") {
            ensureNetwork();
            const auto parameterName = stripQuotes(readArgument(action, "parameter", ""));
            if (parameterName.empty()) {
                throw std::runtime_error("bifurcate requires parameter argument");
            }

            const auto minValue = parseScalarValue(readArgument(action, "par_min", ""), model);
            const auto maxValue = parseScalarValue(readArgument(action, "par_max", ""), model);
            const auto points = static_cast<std::size_t>(std::max(1.0, parseScalarValue(readArgument(action, "n_scan_pts", "1"), model)));
            const auto logScale = lowercase(stripQuotes(readArgument(action, "log_scale", "0"))) == "1";

            // Save initial concentrations
            const auto initialConc = snapshotConcentrations(*network);

            // Forward scan
            ast::Action forwardScan = action;
            forwardScan.name = "parameter_scan";
            forwardScan.arguments["reset_conc"] = "0";  // Don't reset between points
            forwardScan.arguments["suffix"] = readArgument(action, "suffix", "") + "_forward";

            if (verbose) {
                std::cerr << "[bng_cpp] Bifurcate: forward scan\n";
            }
            // Execute forward parameter_scan (handled above)
            // For now, just document - full implementation needs scan result collection

            // Backward scan
            restoreConcentrations(*network, initialConc);
            forwardScan.arguments["suffix"] = readArgument(action, "suffix", "") + "_backward";
            forwardScan.arguments["par_min"] = readArgument(action, "par_max", "");
            forwardScan.arguments["par_max"] = readArgument(action, "par_min", "");

            if (verbose) {
                std::cerr << "[bng_cpp] Bifurcate: backward scan\n";
                std::cerr << "[bng_cpp] Note: bifurcate full implementation pending (result file merging)\n";
            }

            continue;
        }

        if (actionName == "writexml") {
            // Write XML without network (NFsim XML format doesn't require generated network)
            const auto xmlContent = io::XmlWriter::write(model, network.has_value() ? &(*network) : nullptr);
            const auto outputPath = sourcePath.parent_path() / (sourcePath.stem().string() + ".xml");
            std::ofstream outFile(outputPath);
            if (!outFile) {
                throw std::runtime_error("Failed to open " + outputPath.string() + " for writing");
            }
            outFile << xmlContent;
            if (verbose) {
                std::cerr << "[bng_cpp] Wrote XML to " << outputPath << "\n";
            }
            continue;
        }

        if (actionName == "writebngl" || action.name == "writemodel") {
            ensureNetwork();
            io::BnglWriter::Options opts;
            opts.includeComments = true;
            opts.includeActions = false;
            const auto bnglContent = io::BnglWriter::write(model, &(*network), opts);
            const auto outputPath = sourcePath.parent_path() / (sourcePath.stem().string() + "_out.bngl");
            std::ofstream outFile(outputPath);
            if (!outFile) {
                throw std::runtime_error("Failed to open " + outputPath.string() + " for writing");
            }
            outFile << bnglContent;
            if (verbose) {
                std::cerr << "[bng_cpp] Wrote BNGL to " << outputPath << "\n";
            }
            continue;
        }

        if (actionName == "writesbml") {
            ensureNetwork();
            io::SbmlWriter::Options sbmlOpts;
            sbmlOpts.level = 2;
            sbmlOpts.version = 4;
            sbmlOpts.networksExport = true;

            const auto sbmlContent = io::SbmlWriter::write(model, &(*network), sbmlOpts);
            const auto outputPath = sourcePath.parent_path() / (sourcePath.stem().string() + ".xml");
            std::ofstream outFile(outputPath);
            if (!outFile) {
                throw std::runtime_error("Failed to open " + outputPath.string() + " for writing");
            }
            outFile << sbmlContent;
            if (verbose) {
                std::cerr << "[bng_cpp] Wrote SBML to " << outputPath << "\n";
            }
            continue;
        }

        if (actionName == "writemfile") {
            ensureNetwork();
            const auto mContent = io::MatlabWriter::write(model, *network);
            const auto outputPath = sourcePath.parent_path() / (sourcePath.stem().string() + ".m");
            std::ofstream outFile(outputPath);
            if (!outFile) {
                throw std::runtime_error("Failed to open " + outputPath.string() + " for writing");
            }
            outFile << mContent;
            if (verbose) {
                std::cerr << "[bng_cpp] Wrote MATLAB to " << outputPath << "\n";
            }
            continue;
        }

        if (actionName == "visualize") {
            // Generate contact map
            auto contactMap = io::ContactMapWriter::buildContactMap(model);

            // Default to GML format
            const auto format = lowercase(stripQuotes(readArgument(action, "type", "contactmap")));
            const auto outputFormat = lowercase(stripQuotes(readArgument(action, "format", "gml")));

            std::string content;
            std::string extension;

            if (outputFormat == "gml") {
                content = io::ContactMapWriter::toGML(contactMap);
                extension = ".gml";
            } else if (outputFormat == "dot") {
                content = io::ContactMapWriter::toDOT(contactMap);
                extension = ".dot";
            } else {
                content = io::ContactMapWriter::toGML(contactMap);
                extension = ".gml";
            }

            const auto outputPath = sourcePath.parent_path() / (sourcePath.stem().string() + "_contact" + extension);
            std::ofstream outFile(outputPath);
            if (!outFile) {
                throw std::runtime_error("Failed to open " + outputPath.string() + " for writing");
            }
            outFile << content;

            if (verbose) {
                std::cerr << "[bng_cpp] Wrote contact map to " << outputPath << "\n";
            }
            continue;
        }

        if (actionName.rfind("write", 0) == 0) {
            ensureNetwork();
            writeCurrentNetwork();
        }
    }
}

} // namespace bng::actions
