#pragma once

#include <string>
#include <vector>
#include "ast/Model.hpp"

namespace bng::io {

/**
 * SbmlMultiWriter - Export to SBML Level 3 with Multi package extension
 *
 * Generates SBML Level 3 Version 1 with the Multi package (version 1)
 * for rule-based models. Unlike regular writeSBML which exports the
 * flattened reaction network, writeSBMLMulti exports molecule types,
 * components, states, and reaction rules natively in the SBML Multi format.
 *
 * Key SBML Multi elements:
 * - <multi:listOfSpeciesTypes> -- molecule types with components and states
 * - <multi:listOfIntraSpeciesTypeBonds> -- allowed bonds within species types
 * - <multi:listOfSpeciesFeatures> -- component features (states)
 * - <multi:bindingSiteSpeciesFeatureType> -- binding site definitions
 * - Species defined as <multi:speciesType> references
 * - Reactions reference rule patterns instead of concrete species
 *
 * Reference:
 * - BNG2/bng2/Perl2/BNGOutput.pm::writeSBMLMulti()
 * - SBML Multi spec: http://sbml.org/Documents/Specifications/SBML_Level_3/Packages/multi
 */
class SbmlMultiWriter {
public:
    struct Options {
        int level = 3;
        int version = 1;
        int multiVersion = 1;
    };

    static std::string write(const ast::Model& model);
    static std::string write(const ast::Model& model, const Options& options);

private:
    static std::string escapeXml(const std::string& text);
    static std::string makeValidSBMLId(const std::string& text);

    static std::string writeCompartments(const ast::Model& model);
    static std::string writeSpeciesTypes(const ast::Model& model);
    static std::string writeSeedSpecies(const ast::Model& model);
    static std::string writeReactionRules(const ast::Model& model);
    static std::string writeParameters(const ast::Model& model);
};

} // namespace bng::io
