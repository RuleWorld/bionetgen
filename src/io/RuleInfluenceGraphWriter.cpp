#include "RuleInfluenceGraphWriter.hpp"

#include <sstream>

namespace bng::io {

RuleInfluenceGraphWriter::RuleInfluenceGraph RuleInfluenceGraphWriter::build(
    const ast::Model& /*model*/, const engine::GeneratedNetwork& network) {

    RuleInfluenceGraph graph;

    // Collect unique rule names, mapping rule name -> index
    std::map<std::string, std::size_t> ruleNameToIndex;
    for (const auto& rxn : network.reactions.all()) {
        const std::string& ruleName = rxn.getOriginRuleName();
        std::string name = ruleName.empty() ? rxn.getLabel() : ruleName;
        if (ruleNameToIndex.find(name) == ruleNameToIndex.end()) {
            ruleNameToIndex[name] = graph.ruleNames.size();
            graph.ruleNames.push_back(name);
        }
    }

    // For each rule, compute which species it produces and which it consumes.
    // A rule "consumes" a species if that species appears as a reactant.
    // A rule "produces" a species if that species appears as a product.
    std::map<std::size_t, std::set<std::size_t>> ruleProduces;  // ruleIdx -> set of species indices
    std::map<std::size_t, std::set<std::size_t>> ruleConsumes;  // ruleIdx -> set of species indices

    for (const auto& rxn : network.reactions.all()) {
        const std::string& ruleName = rxn.getOriginRuleName();
        std::string name = ruleName.empty() ? rxn.getLabel() : ruleName;
        std::size_t ruleIdx = ruleNameToIndex[name];

        for (const auto idx : rxn.getReactants()) {
            ruleConsumes[ruleIdx].insert(idx);
        }
        for (const auto idx : rxn.getProducts()) {
            ruleProduces[ruleIdx].insert(idx);
        }
    }

    // Build influence edges: rule A -> rule B if A produces a species that B consumes.
    // Track which species mediates the influence (pick the first one for labeling).
    std::set<std::pair<std::size_t, std::size_t>> seenEdges;

    for (const auto& [ruleA, producedSpecies] : ruleProduces) {
        for (const auto& [ruleB, consumedSpecies] : ruleConsumes) {
            if (ruleA == ruleB) continue;

            for (const auto specIdx : producedSpecies) {
                if (consumedSpecies.count(specIdx)) {
                    auto key = std::make_pair(ruleA, ruleB);
                    if (seenEdges.insert(key).second) {
                        RuleInfluenceGraph::InfluenceEdge edge;
                        edge.sourceRule = ruleA;
                        edge.targetRule = ruleB;
                        edge.mediatingSpecies =
                            network.species.get(specIdx).getSpeciesGraph().toString();
                        graph.edges.push_back(std::move(edge));
                    }
                    break;  // one mediating species is enough for the edge
                }
            }
        }
    }

    return graph;
}

std::string RuleInfluenceGraphWriter::toGML(const RuleInfluenceGraph& graph) {
    std::ostringstream gml;

    gml << "graph [\n";
    gml << "  directed 1\n";

    // Rule nodes
    for (std::size_t i = 0; i < graph.ruleNames.size(); ++i) {
        gml << "  node [\n";
        gml << "    id " << i << "\n";
        gml << "    label \"" << graph.ruleNames[i] << "\"\n";
        gml << "    type \"rule\"\n";
        gml << "    graphics [\n";
        gml << "      type \"roundrectangle\"\n";
        gml << "      fill \"#FFCC00\"\n";
        gml << "    ]\n";
        gml << "  ]\n";
    }

    // Influence edges
    for (const auto& edge : graph.edges) {
        gml << "  edge [\n";
        gml << "    source " << edge.sourceRule << "\n";
        gml << "    target " << edge.targetRule << "\n";
        gml << "    label \"" << edge.mediatingSpecies << "\"\n";
        gml << "    graphics [\n";
        gml << "      fill \"#6666CC\"\n";
        gml << "      targetArrow \"standard\"\n";
        gml << "    ]\n";
        gml << "  ]\n";
    }

    gml << "]\n";
    return gml.str();
}

} // namespace bng::io
