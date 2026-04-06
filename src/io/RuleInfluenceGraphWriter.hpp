#pragma once

#include <map>
#include <set>
#include <string>
#include <vector>

#include "ast/Model.hpp"
#include "engine/NetworkGenerator.hpp"

namespace bng::io {

/**
 * RuleInfluenceGraphWriter - Generate rule influence graphs
 *
 * A directed graph showing how rules influence each other:
 * - Nodes: rules (from the generated network)
 * - Edges: rule A -> rule B when A produces a species that B consumes
 *
 * Requires a generated network (ensureNetwork()).
 *
 * Reference: BNG2/bng2/Perl2/Visualization/Viz.pm  type => "rinf"
 */
class RuleInfluenceGraphWriter {
public:
    struct RuleInfluenceGraph {
        std::vector<std::string> ruleNames;

        struct InfluenceEdge {
            std::size_t sourceRule;
            std::size_t targetRule;
            std::string mediatingSpecies;  // species that mediates the influence
        };

        std::vector<InfluenceEdge> edges;
    };

    /**
     * Build rule influence graph from generated network
     */
    static RuleInfluenceGraph build(const ast::Model& model,
                                    const engine::GeneratedNetwork& network);

    /**
     * Export to GML format
     */
    static std::string toGML(const RuleInfluenceGraph& graph);
};

} // namespace bng::io
