#pragma once

#include <filesystem>
#include <string>
#include <vector>

#include "ast/Model.hpp"
#include "NetworkGenerator.hpp"

namespace bng::engine {

/**
 * HybridModelGenerator - HPP (Hybrid Particle Population) Model Generator
 *
 * Faithful port of BNG2 Perl generate_hybrid_model action from
 * bng2/Perl2/BNGAction.pm lines 1109-1515.
 *
 * Steps (matching Perl implementation exactly):
 *   0. Validate preconditions (PopulationMaps must exist)
 *   1. Create new hybrid model, copy metadata (name, version, units, options)
 *   2. Copy constant parameters from ParamList
 *   3. Copy compartments
 *   4. Copy molecule types + add population types (check for name clashes)
 *   5. Copy seed species, replacing isomorphic population equivalents
 *   6. Add empty populations (concentration=0) for populations not in seed species
 *   7. Copy observables, add population pattern matches
 *   8. Copy functions
 *   9. Expand rules w.r.t. populations (copy rules; expandRule stub for now)
 *  10. Add population mapping rules
 *  11. Create empty reaction list
 *  12. Write hybrid BNGL file ({prefix}_hpp.bngl)
 *  13. Optionally execute actions on the hybrid model
 *
 * Output: A BNGL file containing the hybrid model, written using BnglWriter.
 *
 * Reference: BNG2 Perl generate_hybrid_model / BNGAction.pm
 */
class HybridModelGenerator {
public:
    struct Options {
        std::string prefix;          // Output prefix (empty = use model name)
        std::string suffix = "hpp";  // Suffix for hybrid model name
        bool overwrite = false;      // Overwrite existing output file
        bool verbose = false;        // Print detailed progress
        bool safe = false;           // Conservative mode (no rule expansion)
        bool execute = false;        // Run simulation after generating submodels
        std::vector<std::string> actions = {"writeXML()"};  // Actions to write in output
        double tEnd = 10.0;          // Simulation end time (if execute=true)
        std::size_t nSteps = 100;    // Number of output steps (if execute=true)
    };

    struct GenerateResult {
        std::string hybridBnglPath;   // Path to generated {prefix}_hpp.bngl file
        std::string hybridModelName;   // Name of the hybrid model
        std::size_t nParameters = 0;   // Number of copied parameters
        std::size_t nCompartments = 0; // Number of copied compartments
        std::size_t nMoleculeTypes = 0;// Number of molecule types (including population types)
        std::size_t nPopulationTypes = 0; // Number of added population types
        std::size_t nSeedSpecies = 0;  // Number of seed species
        std::size_t nZeroPopulations = 0; // Number of zero-concentration populations added
        std::size_t nObservables = 0;  // Number of observables
        std::size_t nFunctions = 0;    // Number of functions
        std::size_t nRules = 0;        // Number of reaction rules after expansion
        std::size_t nPopulationMaps = 0; // Number of population mapping rules added
    };

    HybridModelGenerator(ast::Model& model, const GeneratedNetwork& network);

    /**
     * Generate the hybrid model.
     * Follows the exact 13-step process from Perl BNGAction.pm.
     * @param sourcePath  Path to the original .bngl source file
     * @param opts        Generation options
     * @return            Result with statistics and output path
     */
    GenerateResult generate(const std::filesystem::path& sourcePath, const Options& opts);

    friend class HybridModelGeneratorTest;

private:
    /**
     * Check if two species patterns are isomorphic.
     * Used in Step 5 to check if a seed species matches a population.
     */
    bool isIsomorphic(const std::string& pattern1, const std::string& pattern2) const;

    /**
     * Count match occurrences of an observable pattern against a species.
     * Used in Step 7 to find population matches for observables.
     */
    std::size_t countMatches(const std::string& obsPattern, const std::string& speciesPattern) const;

    ast::Model& model_;
    const GeneratedNetwork& network_;
};

} // namespace bng::engine
