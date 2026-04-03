#pragma once

#include <functional>
#include <string>
#include <vector>

#include "ast/Model.hpp"
#include "NetworkGenerator.hpp"

namespace bng::engine {

struct OdeOptions {
    double tEnd = 1.0;
    std::size_t nSteps = 100;
    double rtol = 1e-8;
    double atol = 1e-8;
    std::string method = "euler";  // "euler", "rk4", "cvode" (cvode not yet implemented)
    double maxStep = 0.0;          // 0 = no limit
    std::string stopCondition;     // muParser expression (empty = none)
    unsigned int seed = 0;         // for SSA (0 = random)
    bool steadyState = false;      // Enable steady-state detection
    double steadyStateTol = 1e-8;  // Tolerance for steady-state (|dydt| < tol)
    std::string stopIf;            // Boolean expression to evaluate at each step
    bool printCDAT = true;         // Whether to write .cdat output file
};

struct OdeResult {
    std::vector<double> timePoints;                    // length = nSteps + 1
    std::vector<std::vector<double>> concentrations;   // [timeIndex][speciesIndex]
    std::vector<std::vector<double>> observables;      // [timeIndex][groupIndex]
};

class OdeIntegrator {
public:
    OdeIntegrator(const ast::Model& model, const GeneratedNetwork& network);

    OdeResult integrate(const OdeOptions& options);
    void writeOutputFiles(const std::string& prefix, const OdeResult& result, bool printCDAT = true) const;
    void derivs(double t, const double* y, double* dydt) const;

private:
    const ast::Model& model_;
    const GeneratedNetwork& network_;

    // Compiled reaction network for fast derivative evaluation
    struct CompiledReaction {
        std::vector<std::size_t> reactantIndices;  // 0-based species indices
        std::vector<std::size_t> productIndices;
        double rateConstant;          // evaluated rate (for elementary)
        double statFactor;            // statistical factor
        bool isFunctional = false;    // true if rate depends on time/observables
        std::optional<ast::Expression> functionalRateExpr;  // for runtime evaluation
    };

    struct CompiledGroup {
        std::string name;
        std::vector<std::pair<std::size_t, double>> entries;  // (speciesIndex, weight)
    };

    std::vector<CompiledReaction> compiledRxns_;
    std::vector<CompiledGroup> compiledGroups_;
    std::vector<bool> fixedSpecies_;    // true for $ (constant) species
    std::size_t nSpecies_ = 0;
    bool hasFunctionalRates_ = false;

    void compile();
    void compileGroups();
    void updateGroups(const double* y, std::vector<double>& groupValues) const;

    OdeResult integrateEuler(const OdeOptions& opts);
    OdeResult integrateRK4(const OdeOptions& opts);
    OdeResult integrateCvode(const OdeOptions& opts);
    OdeResult integrateSSA(const OdeOptions& opts);

    double computePropensity(const CompiledReaction& rxn, const std::vector<double>& y) const;
};

} // namespace bng::engine
