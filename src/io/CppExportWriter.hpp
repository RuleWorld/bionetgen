#pragma once

#include <string>
#include <vector>
#include "ast/Model.hpp"
#include "engine/NetworkGenerator.hpp"

namespace bng::io {

/**
 * CppExportWriter - Export to standalone C++ CVODE header file
 *
 * Generates a self-contained C++ header that uses SUNDIALS/CVODE to
 * integrate the ODE system. Matches the Perl BNG2 writeCPPfile()
 * implementation in BNGOutput.pm.
 *
 * The generated file includes:
 * - SUNDIALS CVODE headers
 * - Parameter, expression, observable, ratelaw, and derivative functions
 * - A simulate() function returning species and observable time series
 * - A Result struct for output
 *
 * Reference: BNG2/bng2/Perl2/BNGOutput.pm::writeCPPfile()
 */
class CppExportWriter {
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
        bool stiff = true;       // true = CV_BDF/CV_NEWTON, false = CV_ADAMS/CV_FUNCTIONAL
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
    // Reuse structures from MatlabWriter pattern
    struct ObservableGroup {
        std::string name;
        std::string type;
        std::vector<std::pair<std::size_t, std::size_t>> entries; // (speciesIndex, weight)
    };

    struct StoichEntry {
        std::size_t speciesIndex;
        std::vector<std::pair<std::size_t, int>> rxnCoeffs; // (rxnIndex, coefficient)
    };

    static std::string convertRateToCVode(const std::string& rate, const std::vector<std::string>& paramNames);

    static std::vector<ObservableGroup> computeObservableGroups(
        const ast::Model& model, const engine::GeneratedNetwork& network);
    static std::vector<StoichEntry> computeStoichiometry(
        const engine::GeneratedNetwork& network, std::size_t nSpecies);
};

} // namespace bng::io
