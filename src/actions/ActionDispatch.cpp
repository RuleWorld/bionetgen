#include "actions/ActionDispatch.hpp"

#include <algorithm>
#include <cctype>
#include <cmath>
#include <cstdlib>
#include <filesystem>
#include <iostream>
#include <optional>
#include <sstream>
#include <string>
#include <unordered_map>
#include <vector>

#include "ast/Parameter.hpp"
#include "engine/NetworkGenerator.hpp"
#include "io/NetWriter.hpp"

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

    throw std::runtime_error("Unsupported scalar action value: '" + text + "'");
}

std::string readArgument(const ast::Action& action, const std::string& key, const std::string& fallback = {}) {
    const auto found = action.arguments.find(key);
    if (found == action.arguments.end()) {
        return fallback;
    }
    return found->second;
}

std::filesystem::path findRunNetworkBinary(const std::filesystem::path& sourcePath) {
    for (auto cursor = sourcePath.parent_path(); !cursor.empty(); cursor = cursor.parent_path()) {
        const auto binPath = cursor / "bng2" / "bin";
        if (!std::filesystem::exists(binPath) || !std::filesystem::is_directory(binPath)) {
            if (cursor == cursor.root_path()) {
                break;
            }
            continue;
        }

        std::filesystem::path fallback;
        for (const auto& entry : std::filesystem::directory_iterator(binPath)) {
            if (!entry.is_regular_file()) {
                continue;
            }
            const auto name = lowercase(entry.path().filename().string());
            if (name == "run_network" || name == "run_network.exe") {
                return entry.path();
            }
            if (name.rfind("run_network", 0) == 0) {
                fallback = entry.path();
            }
        }
        if (!fallback.empty()) {
            return fallback;
        }
    }

    throw std::runtime_error("Could not find run_network binary under bng2/bin");
}

std::string quoted(const std::filesystem::path& path) {
    return "\"" + path.string() + "\"";
}

std::string quoted(const std::string& text) {
    return "\"" + text + "\"";
}

std::string resolveSimulationMethod(const ast::Action& action) {
    if (action.name == "simulate_ode") {
        return "cvode";
    }
    if (action.name == "simulate_ssa") {
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
    const std::filesystem::path& netPath,
    bool verbose) {
    const auto method = resolveSimulationMethod(action);
    const auto tEnd = stripQuotes(readArgument(action, "t_end", ""));
    const auto nSteps = stripQuotes(readArgument(action, "n_steps", stripQuotes(readArgument(action, "n_output_steps", ""))));
    if (tEnd.empty() || nSteps.empty()) {
        throw std::runtime_error("simulate requires t_end and n_steps (or n_output_steps)");
    }

    const auto binary = findRunNetworkBinary(sourcePath);
    const auto prefix = simulationPrefix(action, sourcePath);
    const auto outputPrefixPath = sourcePath.parent_path() / prefix;

    std::ostringstream command;
    command << quoted(binary)
            << " -o " << quoted(outputPrefixPath)
            << " -p " << method
            << " " << tEnd
            << " " << nSteps
            << " < " << quoted(netPath);

    if (verbose) {
        std::cerr << "[bng_cpp] running: " << command.str() << '\n';
    }

    const int code = std::system(command.str().c_str());
    if (code != 0) {
        throw std::runtime_error("run_network failed with exit code " + std::to_string(code));
    }

    (void)model;
}

} // namespace

void ActionDispatch::execute(ast::Model& model, const std::filesystem::path& sourcePath, bool verbose) {
    engine::NetworkGenerator generator(model);
    std::optional<engine::GeneratedNetwork> network;
    std::unordered_map<std::string, std::vector<double>> savedConcentrations;

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
        if (action.name == "generate_network") {
            network = generator.generate(sourcePath);
            continue;
        }

        if (action.name == "setparameter") {
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

        if (action.name == "setconcentration") {
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

        if (action.name == "saveconcentrations") {
            ensureNetwork();
            const auto label = stripQuotes(readArgument(action, "value", "default"));
            savedConcentrations[label] = snapshotConcentrations(*network);
            continue;
        }

        if (action.name == "resetconcentrations") {
            ensureNetwork();
            const auto label = stripQuotes(readArgument(action, "value", "default"));
            const auto found = savedConcentrations.find(label);
            if (found == savedConcentrations.end()) {
                throw std::runtime_error("resetConcentrations label not found: " + label);
            }
            restoreConcentrations(*network, found->second);
            writeCurrentNetwork();
            continue;
        }

        if (action.name == "simulate" || action.name == "simulate_ode" || action.name == "simulate_ssa") {
            ensureNetwork();
            const auto netPath = writeCurrentNetwork();
            runSimulation(model, action, sourcePath, netPath, verbose);
            continue;
        }

        if (action.name == "parameter_scan") {
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
                const auto netPath = writeCurrentNetwork();

                simulateAction.arguments["prefix"] = simulationPrefix(action, sourcePath, i);
                runSimulation(model, simulateAction, sourcePath, netPath, verbose);
            }
            continue;
        }

        if (action.name.rfind("write", 0) == 0) {
            ensureNetwork();
            writeCurrentNetwork();
        }
    }
}

} // namespace bng::actions
