#pragma once

#include <set>
#include <string>
#include <vector>

#include "ast/Model.hpp"
#include "engine/NetworkGenerator.hpp"

namespace bng::io {

/**
 * ReactionNetworkGraphWriter - Generate reaction network graphs
 *
 * A graph of the generated reaction network:
 * - Nodes: species (from generated network)
 * - Edges: reactions connecting reactant species to product species
 * - Output in GML format
 *
 * Requires a generated network (ensureNetwork()).
 *
 * Reference: BNG2/bng2/Perl2/BNGOutput.pm::visualize() type=>"reaction_network"
 */
class ReactionNetworkGraphWriter {
public:
    struct ReactionNetworkGraph {
        struct SpeciesNode {
            std::size_t index;
            std::string label;
        };

        struct ReactionEdge {
            std::size_t sourceSpecies;
            std::size_t targetSpecies;
            std::string reactionLabel;
        };

        std::vector<SpeciesNode> species;
        std::vector<ReactionEdge> edges;
    };

    /**
     * Build reaction network graph from the generated network
     */
    static ReactionNetworkGraph build(const ast::Model& model,
                                      const engine::GeneratedNetwork& network);

    /**
     * Export reaction network graph to GML format
     */
    static std::string toGML(const ReactionNetworkGraph& graph);
};

} // namespace bng::io
