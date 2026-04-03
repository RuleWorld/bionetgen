#pragma once

#include <string>
#include <vector>
#include "ast/Model.hpp"
#include "engine/NetworkGenerator.hpp"

namespace bng::io {

/**
 * SbmlWriter - Export to SBML (Systems Biology Markup Language)
 *
 * Generates SBML Level 2 Version 3 output from BioNetGen models,
 * matching the Perl BNG2 writeSBML() implementation in BNGOutput.pm.
 *
 * Features:
 * - Parameters (constants + expressions + observables as non-constant)
 * - Compartments (2D/3D, nesting via 'outside')
 * - Species with compartment references
 * - Reactions with full MathML kinetic laws
 * - Observable assignment rules (weighted species sums)
 * - Global function assignment rules
 * - Unit definitions (substance=item)
 * - Volume correction for compartmental rate laws
 *
 * Reference:
 * - BNG2/bng2/Perl2/BNGOutput.pm::writeSBML()
 * - BNG2/bng2/Perl2/RateLaw.pm::toMathMLString()
 * - BNG2/bng2/Perl2/Observable.pm::toMathMLString()
 * - SBML spec: http://sbml.org/
 */
class SbmlWriter {
public:
    struct Options {
        int level = 2;               // SBML level
        int version = 3;             // SBML version (Perl default: L2V3)
        bool networksExport = true;  // Export generated network (vs rules)
    };

    static std::string write(
        const ast::Model& model,
        const engine::GeneratedNetwork* network
    );

    static std::string write(
        const ast::Model& model,
        const engine::GeneratedNetwork* network,
        const Options& options
    );

private:
    // Observable-to-species weight mapping
    struct ObservableGroup {
        std::string name;
        std::string type;
        std::vector<std::pair<std::size_t, std::size_t>> entries; // (speciesIndex, weight)
    };

    static std::string escapeXml(const std::string& text);
    static std::string makeValidSBMLId(const std::string& text);

    static std::string writeUnitDefinitions();
    static std::string writeCompartments(const ast::Model& model);
    static std::string writeParameters(const ast::Model& model, const std::vector<ObservableGroup>& groups);
    static std::string writeSpecies(const ast::Model& model, const engine::GeneratedNetwork* network);
    static std::string writeInitialAssignments(const ast::Model& model);
    static std::string writeAssignmentRules(const ast::Model& model, const std::vector<ObservableGroup>& groups);
    static std::string writeReactions(const engine::GeneratedNetwork& network, const ast::Model& model);

    static std::vector<ObservableGroup> computeObservableGroups(
        const ast::Model& model, const engine::GeneratedNetwork& network);

    // MathML generation helpers
    static std::string exprToMathML(const ast::Expression& expr, const std::string& indent);
    static std::string rateLawToMathML(
        const ast::Rxn& rxn, const ast::Model& model, const std::string& indent);
};

} // namespace bng::io
