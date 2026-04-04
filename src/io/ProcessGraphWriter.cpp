#include "ProcessGraphWriter.hpp"

#include <sstream>

namespace bng::io {

ProcessGraphWriter::ProcessGraph ProcessGraphWriter::build(
    const ast::Model& /*model*/, const engine::GeneratedNetwork& network) {

    ProcessGraph graph;
    int nextId = 0;

    // Species nodes (IDs 0 .. nSpecies-1)
    for (std::size_t i = 0; i < network.species.size(); ++i) {
        ProcessGraph::SpeciesNode sn;
        sn.id = nextId++;
        sn.label = network.species.get(i).getSpeciesGraph().toString();
        graph.speciesNodes.push_back(std::move(sn));
    }

    // Reaction nodes
    const auto& reactions = network.reactions.all();
    for (std::size_t ri = 0; ri < reactions.size(); ++ri) {
        ProcessGraph::ReactionNode rn;
        rn.id = nextId++;
        rn.label = reactions[ri].getLabel();
        graph.reactionNodes.push_back(std::move(rn));

        const int rxnNodeId = rn.id;

        // Edges: species -> reaction (reactant role)
        for (const auto srcIdx : reactions[ri].getReactants()) {
            ProcessGraph::ProcessEdge edge;
            edge.sourceId = static_cast<int>(srcIdx);  // species node ID == species index
            edge.targetId = rxnNodeId;
            edge.role = "reactant";
            graph.edges.push_back(std::move(edge));
        }

        // Edges: reaction -> species (product role)
        for (const auto tgtIdx : reactions[ri].getProducts()) {
            ProcessGraph::ProcessEdge edge;
            edge.sourceId = rxnNodeId;
            edge.targetId = static_cast<int>(tgtIdx);  // species node ID == species index
            edge.role = "product";
            graph.edges.push_back(std::move(edge));
        }
    }

    return graph;
}

std::string ProcessGraphWriter::toGML(const ProcessGraph& graph) {
    std::ostringstream gml;

    gml << "graph [\n";
    gml << "  directed 1\n";

    // Species nodes (circles)
    for (const auto& sn : graph.speciesNodes) {
        gml << "  node [\n";
        gml << "    id " << sn.id << "\n";
        gml << "    label \"" << sn.label << "\"\n";
        gml << "    type \"species\"\n";
        gml << "    graphics [\n";
        gml << "      type \"ellipse\"\n";
        gml << "      fill \"#CCFFCC\"\n";
        gml << "    ]\n";
        gml << "  ]\n";
    }

    // Reaction nodes (squares)
    for (const auto& rn : graph.reactionNodes) {
        gml << "  node [\n";
        gml << "    id " << rn.id << "\n";
        gml << "    label \"" << rn.label << "\"\n";
        gml << "    type \"reaction\"\n";
        gml << "    graphics [\n";
        gml << "      type \"rectangle\"\n";
        gml << "      fill \"#FFCC99\"\n";
        gml << "    ]\n";
        gml << "  ]\n";
    }

    // Edges
    for (const auto& edge : graph.edges) {
        gml << "  edge [\n";
        gml << "    source " << edge.sourceId << "\n";
        gml << "    target " << edge.targetId << "\n";
        gml << "    label \"" << edge.role << "\"\n";
        gml << "    graphics [\n";
        if (edge.role == "reactant") {
            gml << "      fill \"#CC0000\"\n";
        } else {
            gml << "      fill \"#00CC00\"\n";
        }
        gml << "      targetArrow \"standard\"\n";
        gml << "    ]\n";
        gml << "  ]\n";
    }

    gml << "]\n";
    return gml.str();
}

} // namespace bng::io
