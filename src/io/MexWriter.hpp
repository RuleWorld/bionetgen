#pragma once

#include <string>
#include <vector>
#include "ast/Model.hpp"
#include "engine/NetworkGenerator.hpp"

namespace bng::io {

/**
 * MexWriter - Export to MATLAB MEX (C interface) file
 *
 * Generates a standalone C file that can be compiled with MATLAB's mex
 * compiler for fast ODE simulation. Uses a built-in RK4 solver so
 * users do not need SUNDIALS/CVODE installed.
 *
 * The generated file includes:
 * - mex.h gateway function (mexFunction)
 * - ODE RHS function (calc_species_deriv)
 * - Observable computation
 * - Built-in RK4 integrator
 *
 * Reference: BNG2/bng2/Perl2/BNGOutput.pm::writeMEXfile()
 */
class MexWriter {
public:
    struct Options {
        double tStart = 0.0;
        double tEnd = 10.0;
        std::size_t nSteps = 20;
        double atol = 1e-6;
        double rtol = 1e-8;
        int maxNumSteps = 2000;
        int maxErrTestFails = 7;
        int maxConvFails = 10;
        double maxStep = 0.0;
        bool stiff = true;
        bool sparse = false;
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
    struct ObservableGroup {
        std::string name;
        std::string type;
        std::vector<std::pair<std::size_t, std::size_t>> entries; // (speciesIndex, weight)
    };

    struct StoichEntry {
        std::size_t speciesIndex;
        std::vector<std::pair<std::size_t, int>> rxnCoeffs; // (rxnIndex, coefficient)
    };

    static std::string convertRateToC(const std::string& rate, const std::vector<std::string>& paramNames);

    static std::vector<ObservableGroup> computeObservableGroups(
        const ast::Model& model, const engine::GeneratedNetwork& network);
    static std::vector<StoichEntry> computeStoichiometry(
        const engine::GeneratedNetwork& network, std::size_t nSpecies);
};

} // namespace bng::io
