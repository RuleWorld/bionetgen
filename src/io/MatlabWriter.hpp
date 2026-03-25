#pragma once

#include <string>
#include "ast/Model.hpp"
#include "engine/NetworkGenerator.hpp"

namespace bng::io {

/**
 * MatlabWriter - Export to MATLAB .m files
 *
 * Generates MATLAB scripts for ODE simulation using ode15s or ode45.
 * The generated .m file includes:
 * - Parameter definitions
 * - Initial conditions
 * - ODE function (derivatives)
 * - Observable calculation
 * - Plotting code
 *
 * Reference: BNG2/bng2/Perl2/BNGOutput.pm::writeMfile()
 */
class MatlabWriter {
public:
    struct Options {
        double tStart = 0.0;
        double tEnd = 10.0;
        std::size_t nSteps = 100;
        double atol = 1e-4;
        double rtol = 1e-8;
        int maxOrder = 5;
        bool stats = false;
        bool bdf = false;
    };

    /**
     * Export model to MATLAB .m format
     * @param model The BioNetGen model
     * @param network Generated network
     * @return MATLAB .m script content
     */
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
    static std::string makeMatlabFunctionName(const std::string& modelName);
    static std::string convertRateToMatlab(const std::string& rate, const std::vector<std::string>& paramNames);
};

} // namespace bng::io
