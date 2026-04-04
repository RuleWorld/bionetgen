#pragma once

#include <string>
#include <vector>

#include "ast/Model.hpp"

namespace bng::io {

/**
 * RulevizPatternWriter - Generate rule-based pattern visualizations
 *
 * For each rule, shows reactant and product patterns as subgraphs
 * with nodes for each molecule and edges for bonds.
 * Output in GML format.
 *
 * Does NOT require a generated network -- works directly from rules.
 *
 * Reference: BNG2/bng2/Perl2/Visualization/Viz.pm  type => "ruleviz_pattern"
 */
class RulevizPatternWriter {
public:
    struct PatternGraph {
        struct MoleculeNode {
            int id;
            std::string label;
            std::string ruleLabel;   // which rule this belongs to
            std::string side;        // "reactant" or "product"
            std::size_t patternIdx;  // which pattern within the side
        };

        struct BondEdge {
            int sourceId;
            int targetId;
            std::string label;
        };

        struct RuleGroup {
            std::string ruleName;
            std::vector<int> reactantNodeIds;
            std::vector<int> productNodeIds;
        };

        std::vector<MoleculeNode> nodes;
        std::vector<BondEdge> edges;
        std::vector<RuleGroup> ruleGroups;
    };

    /**
     * Build pattern graph from reaction rules
     */
    static PatternGraph build(const ast::Model& model);

    /**
     * Export to GML format
     */
    static std::string toGML(const PatternGraph& graph);
};

} // namespace bng::io
