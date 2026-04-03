#include "actions/ActionDispatch.hpp"

#include <algorithm>
#include <cctype>
#include <cmath>
#include <cstdlib>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <optional>
#include <iomanip>
#include <sstream>
#include <string>
#include <unordered_map>
#include <vector>

#include "ast/Parameter.hpp"
#include "engine/NetworkGenerator.hpp"
#include "engine/OdeIntegrator.hpp"
#include "engine/PlaSimulator.hpp"
#include "io/NetWriter.hpp"
#include "io/XmlWriter.hpp"
#include "io/BnglWriter.hpp"
#include "io/NetReader.hpp"
#include "io/SbmlWriter.hpp"
#include "io/MatlabWriter.hpp"
#include "io/CppExportWriter.hpp"
#include "io/PythonExportWriter.hpp"
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
    if (method == "pla") {
        return "pla";
    }
    if (method == "nf") {
        return "nf";
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
    const ast::Action& actionOrig,
    const std::filesystem::path& sourcePath,
    engine::GeneratedNetwork& network,
    bool verbose,
    std::vector<double>& lastSimulationState) {

    // Handle argfile: read arguments from file (file args have lower priority than inline args)
    ast::Action action = actionOrig;
    {
        const auto argfileText = stripQuotes(readArgument(action, "argfile", ""));
        if (!argfileText.empty()) {
            std::filesystem::path argfilePath(argfileText);
            if (!argfilePath.is_absolute()) {
                argfilePath = sourcePath.parent_path() / argfileText;
            }
            std::ifstream argfile(argfilePath);
            if (!argfile) {
                throw std::runtime_error("Could not open argfile: " + argfilePath.string());
            }
            std::string line;
            while (std::getline(argfile, line)) {
                line = trim(line);
                if (line.empty() || line[0] == '#') continue; // skip comments/blanks
                // Parse "key value" or "key=value"
                std::string key, value;
                auto eqPos = line.find('=');
                if (eqPos != std::string::npos) {
                    key = trim(line.substr(0, eqPos));
                    value = trim(line.substr(eqPos + 1));
                } else {
                    std::istringstream iss(line);
                    iss >> key;
                    std::getline(iss, value);
                    value = trim(value);
                }
                // File args have lower priority: only insert if not already present
                if (!key.empty() && action.arguments.find(key) == action.arguments.end()) {
                    action.arguments[key] = value;
                }
            }
            if (verbose) {
                std::cerr << "[bng_cpp] Loaded arguments from argfile: " << argfilePath.string() << "\n";
            }
        }
    }

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
    } else if (method == "pla") {
        opts.method = "pla";
    } else {
        opts.method = "cvode";  // Default to CVODE (matches BNG2)
    }

    // Parse seed for SSA/PLA
    if (opts.method == "ssa" || opts.method == "pla") {
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

    // Parse print_CDAT flag (BNG2 parity)
    const auto printCDATText = lowercase(stripQuotes(readArgument(action, "print_CDAT", readArgument(action, "print_cdat", "1"))));
    opts.printCDAT = (printCDATText != "0" && printCDATText != "false");

    // Parse continue flag (BNG2 parity)
    const auto continueText = lowercase(stripQuotes(readArgument(action, "continue", "0")));
    bool continueSimulation = (continueText == "1" || continueText == "true");

    // Parse save_progress flag (BNG2 parity: write .net checkpoint at each output step)
    const auto saveProgressText = lowercase(stripQuotes(readArgument(action, "save_progress", "0")));
    opts.saveProgress = (saveProgressText == "1" || saveProgressText == "true");

    // Parse print_net flag (BNG2 parity: write .net file after simulation with final concentrations)
    const auto printNetText = lowercase(stripQuotes(readArgument(action, "print_net", "0")));
    opts.printNet = (printNetText == "1" || printNetText == "true");

    // Parse output_step_interval (BNG2 parity: output every N internal steps, mainly for SSA/PLA)
    const auto outputStepIntervalText = readArgument(action, "output_step_interval", "");
    if (!outputStepIntervalText.empty()) {
        opts.outputStepInterval = static_cast<std::size_t>(parseScalarValue(outputStepIntervalText, model));
    }

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
        if (opts.saveProgress) {
            std::cerr << " save_progress=1";
        }
        if (opts.printNet) {
            std::cerr << " print_net=1";
        }
        if (opts.outputStepInterval > 0) {
            std::cerr << " output_step_interval=" << opts.outputStepInterval;
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

    // Run simulation
    engine::OdeResult result;
    if (opts.method == "pla") {
        // PLA simulation
        const auto plaConfigStr = stripQuotes(readArgument(action, "pla_config", "fEuler|pre-neg:sb|eps=0.03"));
        auto plaConfig = engine::PlaConfig::parse(plaConfigStr);
        engine::PlaSimulator simulator(model, network);
        result = simulator.simulate(opts, plaConfig);
    } else {
        // ODE/SSA integration
        engine::OdeIntegrator integrator(model, network);
        result = integrator.integrate(opts);
    }

    // Save final state for potential continue
    if (!result.concentrations.empty()) {
        lastSimulationState = result.concentrations.back();
    }

    // Write output files
    const auto prefix = simulationPrefix(action, sourcePath);
    const auto outputPrefix = sourcePath.parent_path() / prefix;
    engine::OdeIntegrator integrator(model, network);
    integrator.writeOutputFiles(outputPrefix.string(), result, opts.printCDAT);

    if (verbose) {
        if (opts.printCDAT) {
            std::cerr << "[bng_cpp] Wrote " << outputPrefix.string() << ".cdat and "
                      << outputPrefix.string() << ".gdat\n";
        } else {
            std::cerr << "[bng_cpp] Wrote " << outputPrefix.string() << ".gdat (print_CDAT=0)\n";
        }
    }

    // save_progress: write .net checkpoint files at each output step with species concentrations
    if (opts.saveProgress && !result.concentrations.empty()) {
        for (std::size_t step = 0; step < result.timePoints.size(); ++step) {
            // Update network species with concentrations at this time step
            for (std::size_t i = 0; i < network.species.size() && i < result.concentrations[step].size(); ++i) {
                network.species.get(i).setAmount(result.concentrations[step][i]);
            }
            // Write checkpoint .net file: prefix_t{time}.net
            std::ostringstream timeTag;
            timeTag << std::setprecision(15) << result.timePoints[step];
            const auto checkpointPath = sourcePath.parent_path() / (prefix + "_t" + timeTag.str() + ".net");
            io::NetWriter::write(checkpointPath, model, network);
            if (verbose) {
                std::cerr << "[bng_cpp] save_progress: wrote " << checkpointPath.string() << "\n";
            }
        }
        // Restore final state concentrations
        if (!result.concentrations.empty()) {
            for (std::size_t i = 0; i < network.species.size() && i < result.concentrations.back().size(); ++i) {
                network.species.get(i).setAmount(result.concentrations.back()[i]);
            }
        }
    }

    // print_net: write .net file after simulation with final concentrations
    if (opts.printNet && !result.concentrations.empty()) {
        for (std::size_t i = 0; i < network.species.size() && i < result.concentrations.back().size(); ++i) {
            network.species.get(i).setAmount(result.concentrations.back()[i]);
        }
        const auto netPath = sourcePath.parent_path() / (sourcePath.stem().string() + ".net");
        io::NetWriter::write(netPath, model, network);
        if (verbose) {
            std::cerr << "[bng_cpp] print_net: wrote " << netPath.string() << "\n";
        }
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

    // Store loaded .net data for passthrough writing (readFile)
    std::optional<io::NetReader::ParseResult> loadedNetData;

    const auto ensureNetwork = [&]() {
        if (!network.has_value()) {
            network = generator.generate(sourcePath);
        }
    };

    const auto writeCurrentNetwork = [&]() {
        const auto outputPath = sourcePath.parent_path() / (sourcePath.stem().string() + ".net");
        if (loadedNetData.has_value()) {
            // Write loaded .net data with updated species concentrations
            std::ofstream out(outputPath);
            if (!out) throw std::runtime_error("Could not open output file: " + outputPath.string());
            out << "# Created by bng_cpp\n";
            // Parameters
            out << "begin parameters\n";
            std::size_t pidx = 1;
            for (const auto& param : model.getParameters().all()) {
                std::ostringstream valStr;
                valStr << std::setprecision(15) << param.getValue();
                out << "    " << pidx++ << " " << param.getName() << " " << valStr.str() << '\n';
            }
            out << "end parameters\n";
            // Functions (passthrough from loaded .net)
            if (!loadedNetData->rawFunctionLines.empty()) {
                out << "begin functions\n";
                for (const auto& line : loadedNetData->rawFunctionLines) {
                    out << "    " << line << '\n';
                }
                out << "end functions\n";
            }
            // Species (with updated concentrations from network)
            out << "begin species\n";
            for (std::size_t i = 0; i < network->species.size(); ++i) {
                const auto& sp = network->species.get(i);
                std::string prefix;
                if (sp.isConstant()) prefix = "$";
                out << "    " << (i + 1) << " " << prefix << sp.getSpeciesGraph().toString() << " ";
                // Write concentration - use scientific notation for consistency
                std::ostringstream concStr;
                concStr << std::setprecision(15) << sp.getAmount();
                out << concStr.str() << '\n';
            }
            out << "end species\n";
            // Reactions (passthrough from loaded .net)
            out << "begin reactions\n";
            for (const auto& line : loadedNetData->reactions) {
                out << "    " << line << '\n';
            }
            out << "end reactions\n";
            // Groups (passthrough from loaded .net)
            if (!loadedNetData->rawGroupLines.empty()) {
                out << "begin groups\n";
                for (const auto& line : loadedNetData->rawGroupLines) {
                    out << "    " << line << '\n';
                }
                out << "end groups\n";
            }
            return outputPath;
        }
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

                // Merge parsed parameters into current model
                for (const auto& [name, value] : parseResult.parameters) {
                    model.getParameters().add(ast::Parameter(name, ast::Expression::number(value)));
                }
                model.getParameters().evaluateAll();

                // Reconstruct network from parsed species and reactions
                engine::GeneratedNetwork loadedNetwork;
                for (const auto& [pattern, concStr] : parseResult.species) {
                    // Check for constant species ($ prefix)
                    bool isConstant = false;
                    std::string cleanPattern = pattern;
                    if (!cleanPattern.empty() && cleanPattern[0] == '$') {
                        isConstant = true;
                        cleanPattern = cleanPattern.substr(1);
                    }
                    // Create a PatternGraph with raw string (no full graph construction needed)
                    BNGcore::PatternGraph pg;
                    pg.set_raw_string(cleanPattern);
                    ast::SpeciesGraph sg(std::move(pg));
                    // Resolve concentration: can be a number or a parameter name
                    double concentration = 0.0;
                    try {
                        concentration = std::stod(concStr);
                    } catch (...) {
                        // Try resolving as parameter name
                        try {
                            concentration = model.getParameters().evaluate(concStr);
                        } catch (...) {
                            concentration = 0.0;
                        }
                    }
                    ast::Species sp(std::move(sg), concentration, isConstant);
                    loadedNetwork.species.add(std::move(sp));
                }

                // Store reactions as-is (raw lines for passthrough writing)
                // Also create Rxn objects for ODE integration
                for (const auto& rxnLine : parseResult.reactions) {
                    // Parse reaction line: <index> <reactants> <products> <rate>
                    std::istringstream iss(rxnLine);
                    std::string idxStr, reactantsStr, productsStr, rateStr;
                    iss >> idxStr >> reactantsStr >> productsStr;
                    std::getline(iss, rateStr);
                    // Trim leading whitespace from rate
                    auto rateStart = rateStr.find_first_not_of(" \t");
                    if (rateStart != std::string::npos) rateStr = rateStr.substr(rateStart);
                    // Strip #comment from rate
                    auto commentPos = rateStr.find('#');
                    if (commentPos != std::string::npos) rateStr = rateStr.substr(0, commentPos);
                    // Trim trailing whitespace
                    while (!rateStr.empty() && std::isspace(rateStr.back())) rateStr.pop_back();

                    // Parse reactant indices (comma-separated, 1-based)
                    std::vector<std::size_t> reactants;
                    if (reactantsStr != "0") {
                        std::istringstream rss(reactantsStr);
                        std::string tok;
                        while (std::getline(rss, tok, ',')) {
                            reactants.push_back(std::stoul(tok) - 1);
                        }
                    }
                    // Parse product indices
                    std::vector<std::size_t> products;
                    if (productsStr != "0") {
                        std::istringstream pss(productsStr);
                        std::string tok;
                        while (std::getline(pss, tok, ',')) {
                            products.push_back(std::stoul(tok) - 1);
                        }
                    }

                    loadedNetwork.reactions.add(ast::Rxn(
                        idxStr, reactants, products, rateStr));
                }

                network = std::move(loadedNetwork);
                loadedNetData = std::move(parseResult);

                if (verbose) {
                    std::cerr << "[bng_cpp] Read .net file: " << path << "\n";
                    std::cerr << "[bng_cpp]   Parameters: " << loadedNetData->parameters.size() << "\n";
                    std::cerr << "[bng_cpp]   Species: " << network->species.size() << "\n";
                    std::cerr << "[bng_cpp]   Reactions: " << network->reactions.size() << "\n";
                }
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

        if (actionName == "setmodelname") {
            const auto value = stripQuotes(readArgument(action, "value", ""));
            if (value.empty()) {
                throw std::runtime_error("setModelName requires a 'value' argument");
            }
            model.setModelName(value);
            if (verbose) {
                std::cerr << "[bng_cpp] Set model name to '" << value << "'\n";
            }
            continue;
        }

        if (actionName == "setvolume") {
            const auto target = stripQuotes(readArgument(action, "target", ""));
            const auto valueText = readArgument(action, "value", "");
            if (target.empty()) {
                throw std::runtime_error("setVolume requires a 'target' compartment name");
            }
            const double value = parseScalarValue(valueText, model);
            bool found = false;
            for (auto& comp : model.getCompartments()) {
                if (comp.getName() == target) {
                    comp.setVolume(value);
                    found = true;
                    break;
                }
            }
            if (!found) {
                throw std::runtime_error("setVolume: compartment not found: " + target);
            }
            // Regenerate network if one exists, since volume affects rate constants
            if (network.has_value()) {
                network = generator.generate(sourcePath);
            }
            if (verbose) {
                std::cerr << "[bng_cpp] Set volume of compartment '" << target << "' to " << value << "\n";
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

        if (actionName == "simulate_pla") {
            ensureNetwork();
            const auto tEnd = stripQuotes(readArgument(action, "t_end", ""));
            const auto nSteps = stripQuotes(readArgument(action, "n_steps", stripQuotes(readArgument(action, "n_output_steps", ""))));
            if (tEnd.empty() || nSteps.empty()) {
                throw std::runtime_error("simulate_pla requires t_end and n_steps");
            }

            engine::OdeOptions opts;
            opts.tEnd = parseScalarValue(tEnd, model);
            opts.nSteps = static_cast<std::size_t>(parseScalarValue(nSteps, model));
            opts.method = "pla";

            const auto seedText = readArgument(action, "seed", "");
            if (!seedText.empty()) {
                opts.seed = static_cast<unsigned int>(parseScalarValue(seedText, model));
            }

            // Parse PLA config
            const auto plaConfigStr = stripQuotes(readArgument(action, "pla_config", "fEuler|pre-neg:sb|eps=0.03"));
            auto plaConfig = engine::PlaConfig::parse(plaConfigStr);

            if (verbose) {
                std::cerr << "[bng_cpp] Simulating PLA with config: " << plaConfigStr
                          << " t_end=" << opts.tEnd << " n_steps=" << opts.nSteps << "\n";
            }

            // Run PLA simulation
            engine::PlaSimulator simulator(model, *network);
            auto result = simulator.simulate(opts, plaConfig);

            // Write output files using OdeIntegrator's writer
            const auto prefix = simulationPrefix(action, sourcePath);
            const auto outputPrefix = sourcePath.parent_path() / prefix;
            engine::OdeIntegrator integrator(model, *network);
            integrator.writeOutputFiles(outputPrefix.string(), result);

            if (verbose) {
                std::cerr << "[bng_cpp] Wrote PLA output: " << outputPrefix.string() << ".cdat, .gdat\n";
            }
            continue;
        }

        if (actionName == "simulate_nf") {
            // NFSim simulation: write XML, invoke NFsim binary, read results
            // Write XML model
            const auto xmlContent = io::XmlWriter::write(model, network.has_value() ? &(*network) : nullptr);
            const auto prefix = simulationPrefix(action, sourcePath);
            const auto xmlPath = sourcePath.parent_path() / (prefix + ".xml");
            {
                std::ofstream xmlOut(xmlPath);
                if (!xmlOut) throw std::runtime_error("Failed to write XML for NFSim: " + xmlPath.string());
                xmlOut << xmlContent;
            }

            // Find NFSim executable
            std::string nfsimExec = stripQuotes(readArgument(action, "nfsim_exec", ""));
            if (nfsimExec.empty()) {
                // Search standard locations
                std::vector<std::filesystem::path> searchPaths;
                // Check BNG_PATH environment variable
                const char* bngPath = std::getenv("BNG_PATH");
                if (bngPath) {
                    searchPaths.push_back(std::filesystem::path(bngPath) / "bin");
                }
                // Check relative to source
                searchPaths.push_back(sourcePath.parent_path() / "bin");
                // Check BNG_CPP_SOURCE_DIR macro
#ifdef BNG_CPP_SOURCE_DIR
                searchPaths.push_back(std::filesystem::path(BNG_CPP_SOURCE_DIR) / "bng2" / "bin");
#endif

                std::vector<std::string> nfsimNames = {"NFsim", "NFsim.exe"};
                for (const auto& dir : searchPaths) {
                    for (const auto& name : nfsimNames) {
                        auto candidate = dir / name;
                        if (std::filesystem::exists(candidate)) {
                            nfsimExec = candidate.string();
                            break;
                        }
                    }
                    if (!nfsimExec.empty()) break;
                }

                if (nfsimExec.empty()) {
                    throw std::runtime_error(
                        "NFsim executable not found. Set nfsim_exec parameter or BNG_PATH environment variable.");
                }
            }

            // Build NFSim command
            const auto tEnd = stripQuotes(readArgument(action, "t_end", "10"));
            const auto nSteps = stripQuotes(readArgument(action, "n_steps", "20"));
            const auto gdatPath = sourcePath.parent_path() / (prefix + ".gdat");

            std::string cmd = "\"" + nfsimExec + "\""
                + " -xml \"" + xmlPath.string() + "\""
                + " -o \"" + gdatPath.string() + "\""
                + " -sim " + tEnd
                + " -oSteps " + nSteps;

            // Map BNG parameters to NFSim flags
            const auto seedText = readArgument(action, "seed", "");
            if (!seedText.empty()) cmd += " -seed " + stripQuotes(seedText);

            const auto utlText = readArgument(action, "utl", "");
            if (!utlText.empty()) cmd += " -utl " + stripQuotes(utlText);

            const auto verboseFlag = lowercase(stripQuotes(readArgument(action, "verbose", "0")));
            if (verboseFlag == "1" || verboseFlag == "true") cmd += " -v";

            const auto complexFlag = lowercase(stripQuotes(readArgument(action, "complex", "1")));
            if (complexFlag == "1" || complexFlag == "true") cmd += " -cb";

            const auto getFinalState = lowercase(stripQuotes(readArgument(action, "get_final_state", "1")));
            if (getFinalState == "1" || getFinalState == "true") {
                const auto speciesPath = sourcePath.parent_path() / (prefix + ".species");
                cmd += " -ss \"" + speciesPath.string() + "\"";
            }

            if (verbose) {
                std::cerr << "[bng_cpp] Running NFSim: " << cmd << "\n";
            }

            // Execute NFSim
            int exitCode = std::system(cmd.c_str());
            if (exitCode != 0) {
                std::cerr << "[bng_cpp] Warning: NFSim returned exit code " << exitCode << "\n";
            }

            if (verbose) {
                std::cerr << "[bng_cpp] NFSim completed. Output: " << gdatPath << "\n";
            }
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
            const auto logScale = lowercase(stripQuotes(readArgument(action, "log_scale", "0"))) == "1"
                || lowercase(stripQuotes(readArgument(action, "log_scale", "0"))) == "true";

            // Build a simulate action from the bifurcate arguments
            ast::Action simulateAction = action;
            simulateAction.name = "simulate";
            simulateAction.arguments["method"] = readArgument(action, "method", "ode");

            const auto baseSuffix = stripQuotes(readArgument(action, "suffix", ""));
            const auto prefix = simulationPrefix(action, sourcePath);
            const auto outputDir = sourcePath.parent_path();

            // Helper: compute parameter value at scan point i
            auto computeParamValue = [&](std::size_t i, double pMin, double pMax) -> double {
                const double alpha = points == 1 ? 0.0 : static_cast<double>(i) / static_cast<double>(points - 1);
                if (logScale) {
                    if (pMin <= 0.0 || pMax <= 0.0) {
                        throw std::runtime_error("bifurcate with log_scale requires positive par_min and par_max");
                    }
                    return std::exp(std::log(pMin) + alpha * (std::log(pMax) - std::log(pMin)));
                }
                return pMin + alpha * (pMax - pMin);
            };

            // Save initial concentrations
            const auto initialConc = snapshotConcentrations(*network);

            // === Forward scan (min -> max) ===
            if (verbose) {
                std::cerr << "[bng_cpp] Bifurcate: forward scan (" << points << " points)\n";
            }

            std::vector<std::string> forwardGdatFiles;
            for (std::size_t i = 0; i < points; ++i) {
                const double value = computeParamValue(i, minValue, maxValue);

                model.getParameters().add(ast::Parameter(parameterName, ast::Expression::number(value)));
                model.getParameters().evaluateAll();
                network = generator.generate(sourcePath);
                writeCurrentNetwork();

                std::string scanPrefix = prefix + "_forward_scan" + std::to_string(i + 1);
                simulateAction.arguments["prefix"] = scanPrefix;
                runSimulation(model, simulateAction, sourcePath, *network, verbose, lastSimulationState);
                forwardGdatFiles.push_back((outputDir / (scanPrefix + ".gdat")).string());
            }

            // === Backward scan (max -> min) ===
            // Restore initial concentrations before backward scan
            restoreConcentrations(*network, initialConc);

            if (verbose) {
                std::cerr << "[bng_cpp] Bifurcate: backward scan (" << points << " points)\n";
            }

            std::vector<std::string> backwardGdatFiles;
            for (std::size_t i = 0; i < points; ++i) {
                // Backward: scan from max to min
                const double value = computeParamValue(i, maxValue, minValue);

                model.getParameters().add(ast::Parameter(parameterName, ast::Expression::number(value)));
                model.getParameters().evaluateAll();
                network = generator.generate(sourcePath);
                writeCurrentNetwork();

                std::string scanPrefix = prefix + "_backward_scan" + std::to_string(i + 1);
                simulateAction.arguments["prefix"] = scanPrefix;
                runSimulation(model, simulateAction, sourcePath, *network, verbose, lastSimulationState);
                backwardGdatFiles.push_back((outputDir / (scanPrefix + ".gdat")).string());
            }

            // === Merge forward and backward .gdat files into bifurcation diagram ===
            // For each scan point, extract the final time point from the .gdat file
            // and write them into a combined bifurcation .gdat file
            const auto bifurcationPath = outputDir / (prefix + "_bifurcation.gdat");
            std::ofstream bifOut(bifurcationPath);
            if (!bifOut) {
                throw std::runtime_error("Failed to open " + bifurcationPath.string() + " for writing");
            }

            // Read header from first forward .gdat file (if available)
            std::string headerLine;
            if (!forwardGdatFiles.empty()) {
                std::ifstream firstFile(forwardGdatFiles[0]);
                if (firstFile && std::getline(firstFile, headerLine)) {
                    // Replace "time" with parameter name in the header
                    bifOut << "#" << std::setw(17) << parameterName;
                    // Copy observable column headers from the original header
                    std::istringstream hss(headerLine);
                    std::string tok;
                    bool first = true;
                    while (hss >> tok) {
                        if (first) { first = false; continue; }  // Skip "#time" or first token
                        bifOut << " " << std::setw(18) << tok;
                    }
                    bifOut << " " << std::setw(18) << "direction";
                    bifOut << "\n";
                }
            }

            // Helper: read the last data line from a .gdat file
            auto readLastLine = [](const std::string& path) -> std::string {
                std::ifstream in(path);
                std::string line, lastLine;
                while (std::getline(in, line)) {
                    if (!line.empty() && line[0] != '#') {
                        lastLine = line;
                    }
                }
                return lastLine;
            };

            // Write forward scan final states
            for (std::size_t i = 0; i < points; ++i) {
                const double value = computeParamValue(i, minValue, maxValue);
                const auto lastLine = readLastLine(forwardGdatFiles[i]);
                if (!lastLine.empty()) {
                    // Replace time column with parameter value, keep observable columns
                    std::istringstream lss(lastLine);
                    std::string timeTok;
                    lss >> timeTok;  // Skip time value
                    bifOut << std::setw(18) << std::setprecision(12) << std::scientific << value;
                    std::string tok;
                    while (lss >> tok) {
                        bifOut << " " << std::setw(18) << tok;
                    }
                    bifOut << " " << std::setw(18) << 1;  // direction = 1 (forward)
                    bifOut << "\n";
                }
            }

            // Write backward scan final states
            for (std::size_t i = 0; i < points; ++i) {
                const double value = computeParamValue(i, maxValue, minValue);
                const auto lastLine = readLastLine(backwardGdatFiles[i]);
                if (!lastLine.empty()) {
                    std::istringstream lss(lastLine);
                    std::string timeTok;
                    lss >> timeTok;  // Skip time value
                    bifOut << std::setw(18) << std::setprecision(12) << std::scientific << value;
                    std::string tok;
                    while (lss >> tok) {
                        bifOut << " " << std::setw(18) << tok;
                    }
                    bifOut << " " << std::setw(18) << -1;  // direction = -1 (backward)
                    bifOut << "\n";
                }
            }

            if (verbose) {
                std::cerr << "[bng_cpp] Wrote bifurcation diagram to " << bifurcationPath << "\n";
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
            sbmlOpts.version = 3;  // Perl default: L2V3
            sbmlOpts.networksExport = true;

            const auto suffix = stripQuotes(readArgument(action, "suffix", "sbml"));
            const auto sbmlContent = io::SbmlWriter::write(model, &(*network), sbmlOpts);
            const auto outputPath = sourcePath.parent_path() / (sourcePath.stem().string() + "_" + suffix + ".xml");
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
            io::MatlabWriter::Options mOpts;
            // Parse action arguments for writeMfile options
            const auto tStartText = readArgument(action, "t_start", "");
            if (!tStartText.empty()) mOpts.tStart = parseScalarValue(tStartText, model);
            const auto tEndText = readArgument(action, "t_end", "");
            if (!tEndText.empty()) mOpts.tEnd = parseScalarValue(tEndText, model);
            const auto nStepsText = readArgument(action, "n_steps", "");
            if (!nStepsText.empty()) mOpts.nSteps = static_cast<std::size_t>(parseScalarValue(nStepsText, model));
            const auto atolText2 = readArgument(action, "atol", "");
            if (!atolText2.empty()) mOpts.atol = parseScalarValue(atolText2, model);
            const auto rtolText2 = readArgument(action, "rtol", "");
            if (!rtolText2.empty()) mOpts.rtol = parseScalarValue(rtolText2, model);
            const auto maxOrderText = readArgument(action, "max_order", readArgument(action, "maxOrder", ""));
            if (!maxOrderText.empty()) mOpts.maxOrder = static_cast<int>(parseScalarValue(maxOrderText, model));
            const auto statsText = lowercase(stripQuotes(readArgument(action, "stats", "0")));
            mOpts.stats = (statsText == "1" || statsText == "on" || statsText == "true");
            const auto bdfText = lowercase(stripQuotes(readArgument(action, "bdf", "0")));
            mOpts.bdf = (bdfText == "1" || bdfText == "on" || bdfText == "true");
            const auto maxStepText = readArgument(action, "max_step", "");
            if (!maxStepText.empty()) mOpts.maxStep = parseScalarValue(maxStepText, model);

            const auto suffix = stripQuotes(readArgument(action, "suffix", ""));
            const auto mContent = io::MatlabWriter::write(model, *network, mOpts);
            std::string outName = sourcePath.stem().string();
            if (!suffix.empty()) outName += "_" + suffix;
            const auto outputPath = sourcePath.parent_path() / (outName + ".m");
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

        if (actionName == "writecppfile") {
            ensureNetwork();
            io::CppExportWriter::Options cppOpts;
            const auto tStartText = readArgument(action, "t_start", "");
            if (!tStartText.empty()) cppOpts.tStart = parseScalarValue(tStartText, model);
            const auto tEndText = readArgument(action, "t_end", "");
            if (!tEndText.empty()) cppOpts.tEnd = parseScalarValue(tEndText, model);
            const auto nStepsText = readArgument(action, "n_steps", "");
            if (!nStepsText.empty()) cppOpts.nSteps = static_cast<std::size_t>(parseScalarValue(nStepsText, model));
            const auto atolText3 = readArgument(action, "atol", "");
            if (!atolText3.empty()) cppOpts.atol = parseScalarValue(atolText3, model);
            const auto rtolText3 = readArgument(action, "rtol", "");
            if (!rtolText3.empty()) cppOpts.rtol = parseScalarValue(rtolText3, model);
            const auto maxNumStepsText = readArgument(action, "max_num_steps", "");
            if (!maxNumStepsText.empty()) cppOpts.maxNumSteps = static_cast<int>(parseScalarValue(maxNumStepsText, model));
            const auto maxErrText = readArgument(action, "max_err_test_fails", "");
            if (!maxErrText.empty()) cppOpts.maxErrTestFails = static_cast<int>(parseScalarValue(maxErrText, model));
            const auto maxConvText = readArgument(action, "max_conv_fails", "");
            if (!maxConvText.empty()) cppOpts.maxConvFails = static_cast<int>(parseScalarValue(maxConvText, model));
            const auto maxStepText3 = readArgument(action, "max_step", "");
            if (!maxStepText3.empty()) cppOpts.maxStep = parseScalarValue(maxStepText3, model);
            const auto stiffText = lowercase(stripQuotes(readArgument(action, "stiff", "1")));
            cppOpts.stiff = (stiffText != "0" && stiffText != "false" && stiffText != "off");
            const auto sparseText = lowercase(stripQuotes(readArgument(action, "sparse", "0")));
            cppOpts.sparse = (sparseText == "1" || sparseText == "on" || sparseText == "true");

            const auto suffix = stripQuotes(readArgument(action, "suffix", ""));
            const auto cppContent = io::CppExportWriter::write(model, *network, cppOpts);
            std::string outName = sourcePath.stem().string();
            if (!suffix.empty()) outName += "_" + suffix;
            outName += "_cvode";
            const auto outputPath = sourcePath.parent_path() / (outName + ".h");
            std::ofstream outFile(outputPath);
            if (!outFile) {
                throw std::runtime_error("Failed to open " + outputPath.string() + " for writing");
            }
            outFile << cppContent;
            if (verbose) {
                std::cerr << "[bng_cpp] Wrote C++ CVODE header to " << outputPath << "\n";
            }
            continue;
        }

        if (actionName == "writecpyfile") {
            ensureNetwork();
            io::PythonExportWriter::Options pyOpts;
            const auto tStartText = readArgument(action, "t_start", "");
            if (!tStartText.empty()) pyOpts.tStart = parseScalarValue(tStartText, model);
            const auto tEndText = readArgument(action, "t_end", "");
            if (!tEndText.empty()) pyOpts.tEnd = parseScalarValue(tEndText, model);
            const auto nStepsText = readArgument(action, "n_steps", "");
            if (!nStepsText.empty()) pyOpts.nSteps = static_cast<std::size_t>(parseScalarValue(nStepsText, model));
            const auto atolText4 = readArgument(action, "atol", "");
            if (!atolText4.empty()) pyOpts.atol = parseScalarValue(atolText4, model);
            const auto rtolText4 = readArgument(action, "rtol", "");
            if (!rtolText4.empty()) pyOpts.rtol = parseScalarValue(rtolText4, model);

            const auto suffix = stripQuotes(readArgument(action, "suffix", ""));
            const auto pyContent = io::PythonExportWriter::write(model, *network, pyOpts);
            std::string outName = sourcePath.stem().string();
            if (!suffix.empty()) outName += "_" + suffix;
            outName += "_pyode";
            const auto outputPath = sourcePath.parent_path() / (outName + ".py");
            std::ofstream outFile(outputPath);
            if (!outFile) {
                throw std::runtime_error("Failed to open " + outputPath.string() + " for writing");
            }
            outFile << pyContent;
            if (verbose) {
                std::cerr << "[bng_cpp] Wrote Python ODE to " << outputPath << "\n";
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
