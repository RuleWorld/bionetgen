#pragma once

#include <string>
#include <vector>
#include "ast/Model.hpp"
#include "engine/NetworkGenerator.hpp"

namespace bng::io {

/**
 * MatlabWriter - Export to MATLAB .m files
 *
 * Generates MATLAB scripts for ODE simulation matching the Perl BNG2
 * writeMfile() implementation in BNGOutput.pm.
 *
 * Features (Perl parity):
 * - Function signature: [err, timepoints, species_out, observables_out] = model(timepoints, species_init, parameters, suppress_plot)
 * - Modular nested functions: calc_expressions, calc_observables, calc_ratelaws, calc_species_deriv
 * - Parameter expressions with dependency tracking
 * - Observable computation (weighted species sums)
 * - User-defined global functions
 * - Sparse stoichiometry matrix
 * - ode15s with configurable options (atol, rtol, stats, bdf, maxOrder, max_step)
 * - Proper plotting with labels and formatting
 *
 * Reference: BNG2/bng2/Perl2/BNGOutput.pm::writeMfile()
 */
class MatlabWriter {
public:
    struct Options {
        double tStart = 0.0;
        double tEnd = 10.0;
        std::size_t nSteps = 20;     // Perl default: 20 (not 100)
        double atol = 1e-4;
        double rtol = 1e-8;
        int maxOrder = 5;
        bool stats = false;
        bool bdf = false;
        double maxStep = 0.0;        // 0 = not set
    };

    static std::string write(
        const ast::Model& model,
        const engine::GeneratedNetwork& network
    );

    static std::string write(
        const ast::Model& model,
        const engine::GeneratedNetwork& network,
        const Options& options
    );

private:
    // Observable-to-species weight mapping
    struct ObservableGroup {
        std::string name;
        std::string type;
        std::vector<std::pair<std::size_t, std::size_t>> entries; // (speciesIndex, weight)
    };

    // Stoichiometry entry: species_index -> map of rxn_index -> coefficient
    struct StoichEntry {
        std::size_t speciesIndex;
        std::vector<std::pair<std::size_t, int>> rxnCoeffs; // (rxnIndex, coefficient)
    };

    static std::string makeMatlabFunctionName(const std::string& modelName);
    static std::string convertRateToMatlab(const std::string& rate, const std::vector<std::string>& paramNames);

    static std::vector<ObservableGroup> computeObservableGroups(
        const ast::Model& model, const engine::GeneratedNetwork& network);
    static std::vector<StoichEntry> computeStoichiometry(
        const engine::GeneratedNetwork& network, std::size_t nSpecies);
};

} // namespace bng::io
