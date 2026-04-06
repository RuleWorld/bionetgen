#pragma once

#include <string>
#include <vector>
#include "ast/Model.hpp"
#include "engine/NetworkGenerator.hpp"

namespace bng::io {

/**
 * PythonExportWriter - Export to standalone Python ODE simulation script
 *
 * Generates a self-contained Python script that uses numpy and
 * scipy.integrate.odeint to integrate the ODE system and output
 * .cdat/.gdat style files. Inspired by the Perl BNG2 writeCPYfile()
 * implementation in BNGOutput.pm but targets pure Python rather than
 * C-for-ctypes.
 *
 * The generated file includes:
 * - numpy/scipy imports
 * - Parameter array, initial species concentrations
 * - ODE RHS function (dydt)
 * - Observable computation function
 * - Solver invocation and file output
 *
 * Reference: BNG2/bng2/Perl2/BNGOutput.pm::writeCPYfile()
 */
class PythonExportWriter {
public:
    struct Options {
        double tStart = 0.0;
        double tEnd = 10.0;
        std::size_t nSteps = 20;
        double atol = 1e-6;
        double rtol = 1e-8;
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

    static std::string convertRateToPython(const std::string& rate, const std::vector<std::string>& paramNames);

    static std::vector<ObservableGroup> computeObservableGroups(
        const ast::Model& model, const engine::GeneratedNetwork& network);
    static std::vector<StoichEntry> computeStoichiometry(
        const engine::GeneratedNetwork& network, std::size_t nSpecies);
};

} // namespace bng::io
