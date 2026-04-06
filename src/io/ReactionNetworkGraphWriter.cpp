#include "ReactionNetworkGraphWriter.hpp"

#include <map>
#include <set>
#include <sstream>

namespace bng::io {

ReactionNetworkGraphWriter::ReactionNetworkGraph ReactionNetworkGraphWriter::build(
    const ast::Model& /*model*/, const engine::GeneratedNetwork& network) {

    ReactionNetworkGraph graph;

    // Add species nodes
    for (std::size_t i = 0; i < network.species.size(); ++i) {
        ReactionNetworkGraph::SpeciesNode node;
        node.index = i;
        node.label = network.species.get(i).getSpeciesGraph().toString();
        graph.species.push_back(std::move(node));
    }

    // Add edges from reactions: for each reaction, connect every reactant
    // to every product
    for (const auto& rxn : network.reactions.all()) {
        const std::string& label = rxn.getLabel();

        for (const auto srcIdx : rxn.getReactants()) {
            for (const auto tgtIdx : rxn.getProducts()) {
                ReactionNetworkGraph::ReactionEdge edge;
                edge.sourceSpecies = srcIdx;
                edge.targetSpecies = tgtIdx;
                edge.reactionLabel = label;
                graph.edges.push_back(std::move(edge));
            }
        }
    }

    return graph;
}

std::string ReactionNetworkGraphWriter::toGML(const ReactionNetworkGraph& graph) {
    std::ostringstream gml;

    gml << "graph [\n";
    gml << "  directed 1\n";

    // Species nodes
    for (const auto& sp : graph.species) {
        gml << "  node [\n";
        gml << "    id " << sp.index << "\n";
        gml << "    label \"" << sp.label << "\"\n";
        gml << "    graphics [\n";
        gml << "      type \"ellipse\"\n";
        gml << "      fill \"#CCFFCC\"\n";
        gml << "    ]\n";
        gml << "  ]\n";
    }

    // Reaction edges
    for (const auto& edge : graph.edges) {
        gml << "  edge [\n";
        gml << "    source " << edge.sourceSpecies << "\n";
        gml << "    target " << edge.targetSpecies << "\n";
        gml << "    label \"" << edge.reactionLabel << "\"\n";
        gml << "    graphics [\n";
        gml << "      targetArrow \"standard\"\n";
        gml << "    ]\n";
        gml << "  ]\n";
    }

    gml << "]\n";
    return gml.str();
}

} // namespace bng::io
