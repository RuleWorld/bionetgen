#pragma once

#include <string>
#include <set>
#include <map>
#include "ast/Model.hpp"

namespace bng::io {

/**
 * ContactMapWriter - Generate contact maps showing molecule type interactions
 *
 * Contact maps show which molecule types can bind to each other and through
 * which components. Output formats:
 * - GML (Graph Modeling Language) - for yEd, Cytoscape
 * - DOT (Graphviz) - for automatic layout
 *
 * Reference: BNG2/bng2/Perl2/BNGOutput.pm::visualize()
 */
class ContactMapWriter {
public:
    struct Edge {
        std::string mol1;
        std::string mol2;
        std::string comp1;
        std::string comp2;
    };

    struct ContactMap {
        std::set<std::string> moleculeTypes;
        std::vector<Edge> edges;
    };

    /**
     * Build contact map from reaction rules
     * @param model The BioNetGen model
     * @return ContactMap structure
     */
    static ContactMap buildContactMap(const ast::Model& model);

    /**
     * Export contact map to GML format
     * @param contactMap The contact map to export
     * @return GML-formatted string
     */
    static std::string toGML(const ContactMap& contactMap);

    /**
     * Export contact map to DOT (Graphviz) format
     * @param contactMap The contact map to export
     * @return DOT-formatted string
     */
    static std::string toDOT(const ContactMap& contactMap);
};

} // namespace bng::io
