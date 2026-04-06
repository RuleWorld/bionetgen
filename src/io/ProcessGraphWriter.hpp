#pragma once

#include <string>
#include <vector>

#include "ast/Model.hpp"
#include "engine/NetworkGenerator.hpp"

namespace bng::io {

/**
 * ProcessGraphWriter - Generate bipartite process graphs
 *
 * A bipartite graph with species nodes (circles) and reaction nodes (squares).
 * Edges connect species to the reactions they participate in:
 *   species -> reaction (reactant)
 *   reaction -> species (product)
 *
 * Requires a generated network (ensureNetwork()).
 *
 * Reference: BNG2/bng2/Perl2/Visualization/Viz.pm  type => "process"
 */
class ProcessGraphWriter {
public:
    struct ProcessGraph {
        struct SpeciesNode {
            int id;
            std::string label;
        };

        struct ReactionNode {
            int id;
            std::string label;
        };

        struct ProcessEdge {
            int sourceId;
            int targetId;
            std::string role;  // "reactant" or "product"
        };

        std::vector<SpeciesNode> speciesNodes;
        std::vector<ReactionNode> reactionNodes;
        std::vector<ProcessEdge> edges;
    };

    /**
     * Build process graph from generated network
     */
    static ProcessGraph build(const ast::Model& model,
                              const engine::GeneratedNetwork& network);

    /**
     * Export to GML format
     */
    static std::string toGML(const ProcessGraph& graph);
};

} // namespace bng::io
