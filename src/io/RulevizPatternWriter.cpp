#include "RulevizPatternWriter.hpp"

#include <sstream>

namespace bng::io {

RulevizPatternWriter::PatternGraph RulevizPatternWriter::build(const ast::Model& model) {
    PatternGraph graph;
    int nextId = 0;

    for (const auto& rule : model.getReactionRules()) {
        PatternGraph::RuleGroup group;
        group.ruleName = rule.getRuleName().empty() ? rule.getLabel() : rule.getRuleName();

        // Add reactant pattern molecules
        for (std::size_t pi = 0; pi < rule.getReactantPatterns().size(); ++pi) {
            const auto& pattern = rule.getReactantPatterns()[pi];
            const std::string label = pattern.toString();

            PatternGraph::MoleculeNode node;
            node.id = nextId++;
            node.label = label;
            node.ruleLabel = group.ruleName;
            node.side = "reactant";
            node.patternIdx = pi;
            group.reactantNodeIds.push_back(node.id);
            graph.nodes.push_back(std::move(node));
        }

        // Add product pattern molecules
        for (std::size_t pi = 0; pi < rule.getProductPatterns().size(); ++pi) {
            const auto& pattern = rule.getProductPatterns()[pi];
            const std::string label = pattern.toString();

            PatternGraph::MoleculeNode node;
            node.id = nextId++;
            node.label = label;
            node.ruleLabel = group.ruleName;
            node.side = "product";
            node.patternIdx = pi;
            group.productNodeIds.push_back(node.id);
            graph.nodes.push_back(std::move(node));
        }

        // Add edges from each reactant to each product (showing the rule transformation)
        for (int rId : group.reactantNodeIds) {
            for (int pId : group.productNodeIds) {
                PatternGraph::BondEdge edge;
                edge.sourceId = rId;
                edge.targetId = pId;
                edge.label = group.ruleName;
                graph.edges.push_back(std::move(edge));
            }
        }

        graph.ruleGroups.push_back(std::move(group));
    }

    return graph;
}

std::string RulevizPatternWriter::toGML(const PatternGraph& graph) {
    std::ostringstream gml;

    gml << "graph [\n";
    gml << "  directed 1\n";

    // Emit nodes
    for (const auto& node : graph.nodes) {
        gml << "  node [\n";
        gml << "    id " << node.id << "\n";
        gml << "    label \"" << node.label << "\"\n";
        gml << "    rule \"" << node.ruleLabel << "\"\n";
        gml << "    side \"" << node.side << "\"\n";
        gml << "    graphics [\n";
        if (node.side == "reactant") {
            gml << "      type \"roundrectangle\"\n";
            gml << "      fill \"#FFCC99\"\n";
        } else {
            gml << "      type \"roundrectangle\"\n";
            gml << "      fill \"#99CCFF\"\n";
        }
        gml << "    ]\n";
        gml << "  ]\n";
    }

    // Emit edges
    for (const auto& edge : graph.edges) {
        gml << "  edge [\n";
        gml << "    source " << edge.sourceId << "\n";
        gml << "    target " << edge.targetId << "\n";
        gml << "    label \"" << edge.label << "\"\n";
        gml << "    graphics [\n";
        gml << "      targetArrow \"standard\"\n";
        gml << "    ]\n";
        gml << "  ]\n";
    }

    gml << "]\n";
    return gml.str();
}

} // namespace bng::io
