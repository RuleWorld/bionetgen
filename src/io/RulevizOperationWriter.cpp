#include "RulevizOperationWriter.hpp"

#include <sstream>

namespace bng::io {

RulevizOperationWriter::OperationGraph RulevizOperationWriter::build(const ast::Model& model) {
    OperationGraph graph;
    int nextId = 0;

    for (const auto& rule : model.getReactionRules()) {
        // Create a rule node
        OperationGraph::RuleNode ruleNode;
        ruleNode.id = nextId++;
        ruleNode.label = rule.getRuleName().empty() ? rule.getLabel() : rule.getRuleName();
        const int ruleNodeId = ruleNode.id;
        graph.ruleNodes.push_back(std::move(ruleNode));

        // Create operation nodes from the rule's TransformOps
        for (const auto& op : rule.getOperations()) {
            OperationGraph::OperationNode opNode;
            opNode.id = nextId++;

            switch (op.type) {
                case ast::ReactionRule::TransformOp::Type::AddBond:
                    opNode.opType = "AddBond";
                    opNode.label = "AddBond";
                    break;
                case ast::ReactionRule::TransformOp::Type::DeleteBond:
                    opNode.opType = "DeleteBond";
                    opNode.label = "DeleteBond";
                    break;
                case ast::ReactionRule::TransformOp::Type::ChangeState:
                    opNode.opType = "ChangeState";
                    opNode.label = "ChangeState(" + op.newState + ")";
                    break;
                case ast::ReactionRule::TransformOp::Type::AddMolecule:
                    opNode.opType = "AddMolecule";
                    opNode.label = "AddMolecule";
                    break;
                case ast::ReactionRule::TransformOp::Type::DeleteMolecule:
                    opNode.opType = "DeleteMolecule";
                    opNode.label = "DeleteMolecule";
                    break;
            }

            graph.operationNodes.push_back(std::move(opNode));

            // Edge from rule to operation
            OperationGraph::OpEdge edge;
            edge.ruleNodeId = ruleNodeId;
            edge.operationNodeId = graph.operationNodes.back().id;
            graph.edges.push_back(std::move(edge));
        }

        // If the rule has no detected operations (e.g., synthesis from zero pattern),
        // still show the rule node with a generic "Transform" operation.
        if (rule.getOperations().empty()) {
            OperationGraph::OperationNode opNode;
            opNode.id = nextId++;
            opNode.opType = "Transform";
            opNode.label = "Transform";
            graph.operationNodes.push_back(std::move(opNode));

            OperationGraph::OpEdge edge;
            edge.ruleNodeId = ruleNodeId;
            edge.operationNodeId = graph.operationNodes.back().id;
            graph.edges.push_back(std::move(edge));
        }
    }

    return graph;
}

std::string RulevizOperationWriter::toGML(const OperationGraph& graph) {
    std::ostringstream gml;

    gml << "graph [\n";
    gml << "  directed 1\n";

    // Rule nodes
    for (const auto& rn : graph.ruleNodes) {
        gml << "  node [\n";
        gml << "    id " << rn.id << "\n";
        gml << "    label \"" << rn.label << "\"\n";
        gml << "    type \"rule\"\n";
        gml << "    graphics [\n";
        gml << "      type \"roundrectangle\"\n";
        gml << "      fill \"#FFCC00\"\n";
        gml << "    ]\n";
        gml << "  ]\n";
    }

    // Operation nodes
    for (const auto& on : graph.operationNodes) {
        gml << "  node [\n";
        gml << "    id " << on.id << "\n";
        gml << "    label \"" << on.label << "\"\n";
        gml << "    type \"operation\"\n";
        gml << "    opType \"" << on.opType << "\"\n";
        gml << "    graphics [\n";

        // Color-code by operation type
        if (on.opType == "AddBond") {
            gml << "      type \"diamond\"\n";
            gml << "      fill \"#66CC66\"\n";
        } else if (on.opType == "DeleteBond") {
            gml << "      type \"diamond\"\n";
            gml << "      fill \"#CC6666\"\n";
        } else if (on.opType == "ChangeState") {
            gml << "      type \"ellipse\"\n";
            gml << "      fill \"#6699CC\"\n";
        } else if (on.opType == "AddMolecule") {
            gml << "      type \"hexagon\"\n";
            gml << "      fill \"#99FF99\"\n";
        } else if (on.opType == "DeleteMolecule") {
            gml << "      type \"hexagon\"\n";
            gml << "      fill \"#FF9999\"\n";
        } else {
            gml << "      type \"ellipse\"\n";
            gml << "      fill \"#CCCCCC\"\n";
        }

        gml << "    ]\n";
        gml << "  ]\n";
    }

    // Edges: rule -> operation
    for (const auto& edge : graph.edges) {
        gml << "  edge [\n";
        gml << "    source " << edge.ruleNodeId << "\n";
        gml << "    target " << edge.operationNodeId << "\n";
        gml << "    graphics [\n";
        gml << "      targetArrow \"standard\"\n";
        gml << "    ]\n";
        gml << "  ]\n";
    }

    gml << "]\n";
    return gml.str();
}

} // namespace bng::io
