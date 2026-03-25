#pragma once

#include <string>
#include "ast/Model.hpp"
#include "engine/NetworkGenerator.hpp"

namespace bng::io {

/**
 * SbmlWriter - Export to SBML (Systems Biology Markup Language)
 *
 * Generates SBML Level 2 or 3 output from BioNetGen models.
 * SBML is the standard format for systems biology models.
 *
 * Reference:
 * - BNG2/bng2/Perl2/BNGOutput.pm::writeSBML()
 * - SBML spec: http://sbml.org/
 */
class SbmlWriter {
public:
    struct Options {
        int level = 2;               // SBML level (2 or 3)
        int version = 4;             // SBML version
        bool networksExport = true;  // Export generated network (vs rules)
    };

    /**
     * Export model to SBML format
     * @param model The BioNetGen model
     * @param network Optional generated network (if nullptr, exports rules)
     * @return SBML XML string
     */
    static std::string write(
        const ast::Model& model,
        const engine::GeneratedNetwork* network
    );

    /**
     * Export model to SBML format with options
     */
    static std::string write(
        const ast::Model& model,
        const engine::GeneratedNetwork* network,
        const Options& options
    );

private:
    static std::string escapeXml(const std::string& text);
    static std::string makeValidSBMLId(const std::string& text);
    static std::string writeParameters(const ast::Model& model);
    static std::string writeCompartments(const ast::Model& model);
    static std::string writeSpecies(const ast::Model& model, const engine::GeneratedNetwork* network);
    static std::string writeReactions(const engine::GeneratedNetwork& network, const ast::Model& model);
};

} // namespace bng::io
