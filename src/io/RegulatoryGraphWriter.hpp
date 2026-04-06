#pragma once

#include <map>
#include <set>
#include <string>
#include <vector>

#include "ast/Model.hpp"
#include "engine/NetworkGenerator.hpp"

namespace bng::io {

/**
 * RegulatoryGraphWriter - Generate regulatory graphs
 *
 * A directed graph showing which rules affect which observables/species:
 * - Nodes: rules and observables
 * - Edges: rule -> observable (activating or inhibiting based on whether
 *          the rule increases or decreases the observable)
 * - Output in GML format
 *
 * Requires a generated network (ensureNetwork()).
 *
 * Reference: BNG2/bng2/Perl2/BNGOutput.pm::visualize() type=>"regulatory"
 */
class RegulatoryGraphWriter {
public:
    struct RegulatoryGraph {
        std::vector<std::string> ruleNames;
        std::vector<std::string> observableNames;

        struct Edge {
            std::size_t ruleIndex;
            std::size_t observableIndex;
            int effect;  // +1 = activating, -1 = inhibiting, 0 = ambiguous
        };
        std::vector<Edge> edges;
    };

    /**
     * Build regulatory graph from the generated network
     */
    static RegulatoryGraph build(const ast::Model& model,
                                 const engine::GeneratedNetwork& network);

    /**
     * Export regulatory graph to GML format
     */
    static std::string toGML(const RegulatoryGraph& graph);
};

} // namespace bng::io
