#include "engine/NetworkGenerator.hpp"

#include <algorithm>
#include <cctype>
#include <cstdlib>
#include <iostream>
#include <map>
#include <optional>
#include <string>

#include "ast/ReactionRule.hpp"
#include "io/NetWriter.hpp"

namespace bng::engine {

namespace {

std::optional<std::size_t> parseMaxIter(const ast::Model& model) {
    for (const auto& action : model.getActions()) {
        if (action.name != "generate_network") {
            continue;
        }
        const auto found = action.arguments.find("max_iter");
        if (found == action.arguments.end()) {
            continue;
        }
        return static_cast<std::size_t>(std::stoul(found->second));
    }
    return std::nullopt;
}

std::map<std::string, std::size_t> parseMaxStoich(const ast::Model& model) {
    for (const auto& action : model.getActions()) {
        if (action.name != "generate_network") {
            continue;
        }
        const auto found = action.arguments.find("max_stoich");
        if (found == action.arguments.end()) {
            continue;
        }

        std::map<std::string, std::size_t> limits;
        std::string text = found->second;
        text.erase(std::remove_if(text.begin(), text.end(), [](unsigned char c) {
            return std::isspace(c) != 0;
        }), text.end());
        if (text.size() >= 2 && text.front() == '{' && text.back() == '}') {
            text = text.substr(1, text.size() - 2);
        }

        std::size_t start = 0;
        while (start < text.size()) {
            const auto comma = text.find(',', start);
            const auto entry = text.substr(start, comma == std::string::npos ? std::string::npos : comma - start);
            const auto arrow = entry.find("=>");
            if (arrow != std::string::npos) {
                std::string key = entry.substr(0, arrow);
                // Strip surrounding quotes from key (e.g., 'R' -> R, "R" -> R)
                if (key.size() >= 2 && ((key.front() == '\'' && key.back() == '\'') ||
                                         (key.front() == '"' && key.back() == '"'))) {
                    key = key.substr(1, key.size() - 2);
                }
                limits.emplace(key, static_cast<std::size_t>(std::stoull(entry.substr(arrow + 2))));
            }
            if (comma == std::string::npos) {
                break;
            }
            start = comma + 1;
        }
        return limits;
    }
    return {};
}

bool parseBooleanLike(std::string text) {
    text.erase(std::remove_if(text.begin(), text.end(), [](unsigned char c) {
        return std::isspace(c) != 0;
    }), text.end());
    if (text.size() >= 2 && ((text.front() == '"' && text.back() == '"') || (text.front() == '\'' && text.back() == '\''))) {
        text = text.substr(1, text.size() - 2);
    }
    std::transform(text.begin(), text.end(), text.begin(), [](unsigned char c) {
        return static_cast<char>(std::tolower(c));
    });
    return text == "1" || text == "true" || text == "yes" || text == "on";
}

std::optional<std::size_t> parseMaxAgg(const ast::Model& model) {
    for (const auto& action : model.getActions()) {
        if (action.name != "generate_network") {
            continue;
        }
        const auto found = action.arguments.find("max_agg");
        if (found == action.arguments.end()) {
            continue;
        }
        return static_cast<std::size_t>(std::stoul(found->second));
    }
    return std::nullopt;
}

bool parsePrintIter(const ast::Model& model) {
    const char* envFlag = std::getenv("BNG_CPP_PROGRESS");
    if (envFlag != nullptr && *envFlag != '\0') {
        return parseBooleanLike(envFlag);
    }

    for (const auto& action : model.getActions()) {
        if (action.name != "generate_network") {
            continue;
        }
        const auto found = action.arguments.find("print_iter");
        if (found != action.arguments.end()) {
            return parseBooleanLike(found->second);
        }
    }
    return false;
}

bool parseOverwrite(const ast::Model& model) {
    for (const auto& action : model.getActions()) {
        if (action.name != "generate_network") {
            continue;
        }
        const auto found = action.arguments.find("overwrite");
        if (found == action.arguments.end()) {
            continue;
        }
        return parseBooleanLike(found->second);
    }
    return true;  // default: always regenerate (overwrite=1)
}

bool parseCheckIso(const ast::Model& model) {
    for (const auto& action : model.getActions()) {
        if (action.name != "generate_network") {
            continue;
        }
        const auto found = action.arguments.find("check_iso");
        if (found == action.arguments.end()) {
            continue;
        }
        // Default is true (enabled). Only disable if explicitly set to 0/false.
        return parseBooleanLike(found->second);
    }
    return true;  // default: isomorphism checking enabled
}

bool parsePrintRules(const ast::Model& model) {
    const char* envFlag = std::getenv("BNG_CPP_PROGRESS_RULES");
    if (envFlag != nullptr && *envFlag != '\0') {
        return parseBooleanLike(envFlag);
    }

    for (const auto& action : model.getActions()) {
        if (action.name != "generate_network") {
            continue;
        }
        const auto found = action.arguments.find("print_rule_progress");
        if (found != action.arguments.end()) {
            return parseBooleanLike(found->second);
        }
    }
    return false;
}

bool isBondNode(const BNGcore::Node& node) {
    return node.get_type().get_type_name() == BNGcore::BOND_NODE_TYPE.get_type_name();
}

bool isComponentNode(const BNGcore::Node& node) {
    if (isBondNode(node)) {
        return false;
    }
    for (auto edge = node.edges_in_begin(); edge != node.edges_in_end(); ++edge) {
        if (!isBondNode(**edge)) {
            return true;
        }
    }
    return false;
}

bool isMoleculeNode(const BNGcore::Node& node) {
    return !isBondNode(node) && !isComponentNode(node);
}

std::map<std::string, std::size_t> countMolecules(const ast::SpeciesGraph& graph) {
    std::map<std::string, std::size_t> counts;
    for (auto nodeIter = graph.getGraph().begin(); nodeIter != graph.getGraph().end(); ++nodeIter) {
        if (isMoleculeNode(**nodeIter)) {
            ++counts[(*nodeIter)->get_type().get_type_name()];
        }
    }
    return counts;
}

bool withinStoichLimits(const ast::SpeciesGraph& graph, const std::map<std::string, std::size_t>& limits) {
    if (limits.empty()) {
        return true;
    }
    const auto counts = countMolecules(graph);
    for (const auto& [name, count] : counts) {
        const auto found = limits.find(name);
        if (found != limits.end() && count > found->second) {
            return false;
        }
    }
    return true;
}

bool withinAggLimit(const ast::SpeciesGraph& graph, std::size_t maxAgg) {
    // Count total number of molecules in the species (regardless of type)
    std::size_t totalMolecules = 0;
    for (auto nodeIter = graph.getGraph().begin(); nodeIter != graph.getGraph().end(); ++nodeIter) {
        if (isMoleculeNode(**nodeIter)) {
            ++totalMolecules;
        }
    }
    return totalMolecules <= maxAgg;
}

double evaluateAmount(const ast::Expression& expr, ast::Model& model) {
    return expr.evaluate([&](const std::string& dependency) {
        return model.getParameters().evaluate(dependency);
    });
}

} // namespace

NetworkGenerator::NetworkGenerator(ast::Model& model)
    : model_(model) {}

GeneratedNetwork NetworkGenerator::generateNative(std::size_t maxIter) {
    model_.getParameters().evaluateAll();
    const auto maxStoich = parseMaxStoich(model_);
    const auto maxAgg = parseMaxAgg(model_);
    const bool logProgress = parsePrintIter(model_);
    const bool logRules = parsePrintRules(model_);
    const bool checkIso = parseCheckIso(model_);

    // Set compartment dimension and parent maps for cross-compartment species assignment
    // and compartment transport (endocytosis/exocytosis)
    {
        std::unordered_map<std::string, int> compDims;
        std::unordered_map<std::string, std::string> compParents;
        for (const auto& comp : model_.getCompartments()) {
            compDims[comp.getName()] = comp.getDimension();
            if (!comp.getParent().empty()) {
                compParents[comp.getName()] = comp.getParent();
            }
        }
        ast::setCompartmentDimensions(compDims);
        ast::setCompartmentParents(compParents);
    }

    // Clear per-rule state from previous generate_network calls
    for (auto& rule : model_.getReactionRules()) {
        rule.clearPatternMatchCache();
    }

    GeneratedNetwork network;
    network.species.setCheckIso(checkIso);
    for (const auto& seed : model_.getSeedSpecies()) {
        // Perl's assignCompartment: propagate the species-level compartment to
        // molecule nodes that lack a per-molecule compartment.  Without this,
        // rule expansion cannot infer compartments for products because the
        // molecule nodes in the aggregate graph would have empty compartments.
        auto seedGraph = seed.getGraph();  // mutable copy
        const auto& seedComp = seed.getCompartment();
        if (!seedComp.empty()) {
            for (auto it = seedGraph.begin(); it != seedGraph.end(); ++it) {
                // Molecule nodes have in_degree == 0 in species graphs
                if ((*it)->in_degree() == 0 && (*it)->get_compartment().empty()) {
                    (*it)->set_compartment(seedComp);
                }
            }
        }
        network.species.add(ast::Species(
            ast::SpeciesGraph(seedGraph),
            evaluateAmount(seed.getAmount(), model_),
            seed.isConstant(),
            seed.getCompartment()));
    }

    if (logProgress) {
        std::cerr << "[generate_network] start species=" << network.species.size()
                  << " reactions=" << network.reactions.size()
                  << " max_iter=" << maxIter << '\n';
    }

    for (std::size_t iter = 0; iter < maxIter; ++iter) {
        const std::size_t previousSpecies = network.species.size();
        const std::size_t previousReactions = network.reactions.size();
        const std::size_t speciesAtIterStart = network.species.size();

        for (const auto& rule : model_.getReactionRules()) {
            const std::size_t beforeSpecies = network.species.size();
            const std::size_t beforeReactions = network.reactions.size();
            const auto created = rule.expandRule(network.species, network.reactions, iter, [&](const ast::SpeciesGraph& graph) {
                if (!withinStoichLimits(graph, maxStoich)) return false;
                if (maxAgg.has_value() && !withinAggLimit(graph, *maxAgg)) return false;
                return true;
            }, speciesAtIterStart, &model_);
            const bool debugRules = std::getenv("BNG_DEBUG_RULES") != nullptr;
            if (debugRules) {
                std::cerr << "[generate_network] iter=" << (iter + 1)
                          << " rule=" << rule.getRuleName()
                          << " created=" << created
                          << " species=" << network.species.size()
                          << " reactions=" << network.reactions.size()
                          << " d_species=" << (network.species.size() - beforeSpecies)
                          << " d_rxns=" << (network.reactions.size() - beforeReactions) << '\n';
            } else if (logRules && (created > 0 || network.species.size() != beforeSpecies || network.reactions.size() != beforeReactions)) {
                std::cerr << "[generate_network] iter=" << (iter + 1)
                          << " rule=" << rule.getRuleName()
                          << " created=" << created
                          << " d_species=" << (network.species.size() - beforeSpecies)
                          << " d_rxns=" << (network.reactions.size() - beforeReactions) << '\n';
            }
        }

        // Mark only species that existed at the START of this iteration as processed.
        // Species created during this iteration remain with rulesApplied=false for next iteration.
        for (std::size_t i = 0; i < speciesAtIterStart; ++i) {
            network.species.get(i).setRulesApplied(true);
        }

        if (logProgress) {
            std::cerr << "[generate_network] iter=" << (iter + 1)
                      << " species=" << network.species.size()
                      << " reactions=" << network.reactions.size()
                      << " d_species=" << (network.species.size() - previousSpecies)
                      << " d_rxns=" << (network.reactions.size() - previousReactions) << '\n';
        }

        if (network.species.size() == previousSpecies && network.reactions.size() == previousReactions) {
            if (logProgress) {
                std::cerr << "[generate_network] converged at iter=" << (iter + 1) << '\n';
            }
            break;
        }
    }

    return network;
}

GeneratedNetwork NetworkGenerator::generate(const std::filesystem::path& sourcePath) {
    auto network = generateNative(parseMaxIter(model_).value_or(100));
    if (!sourcePath.empty()) {
        const auto outputPath = sourcePath.parent_path() / (sourcePath.stem().string() + ".net");
        io::NetWriter::write(outputPath, model_, network);
    }
    return network;
}

} // namespace bng::engine
