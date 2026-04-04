#pragma once

#include <string>
#include <vector>

#include "ast/Model.hpp"

namespace bng::io {

/**
 * RulevizOperationWriter - Generate rule operation visualizations
 *
 * For each rule, shows the transformation operations (AddBond, DeleteBond,
 * ChangeState, AddMolecule, DeleteMolecule) as nodes with edges showing
 * which components are affected.
 * Output in GML format.
 *
 * Does NOT require a generated network -- works directly from rules.
 *
 * Reference: BNG2/bng2/Perl2/Visualization/Viz.pm  type => "ruleviz_operation"
 */
class RulevizOperationWriter {
public:
    struct OperationGraph {
        struct RuleNode {
            int id;
            std::string label;
        };

        struct OperationNode {
            int id;
            std::string label;     // e.g. "AddBond", "ChangeState(new_state)"
            std::string opType;    // "AddBond", "DeleteBond", "ChangeState", etc.
        };

        struct OpEdge {
            int ruleNodeId;
            int operationNodeId;
        };

        std::vector<RuleNode> ruleNodes;
        std::vector<OperationNode> operationNodes;
        std::vector<OpEdge> edges;
    };

    /**
     * Build operation graph from reaction rules
     */
    static OperationGraph build(const ast::Model& model);

    /**
     * Export to GML format
     */
    static std::string toGML(const OperationGraph& graph);
};

} // namespace bng::io
