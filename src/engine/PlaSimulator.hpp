#pragma once

#include <random>
#include <string>
#include <vector>

#include "ast/Model.hpp"
#include "NetworkGenerator.hpp"
#include "OdeIntegrator.hpp"

namespace bng::engine {

/**
 * PLA (Partitioned Leaping Algorithm) Configuration
 *
 * Parsed from config string format: "METHOD|PREPOST|PARAMS"
 * Example: "fEuler|pre-neg:sb|eps=0.03"
 *
 * Method: fEuler, midpt, rk4
 * Checking: pre-neg:sb, pre-neg:rb, post-eps:sb, etc.
 * Params: eps=0.03, p=0.5, q=1.5, w=0.75, tau=fixed_value
 */
struct PlaConfig {
    enum class Method { fEuler, Midpoint, RK4 };
    enum class CheckType { PreNeg, PreEps, PostNeg, PostEps, Fixed };
    enum class Basis { SpeciesBased, ReactionBased };

    Method method = Method::fEuler;
    CheckType checkType = CheckType::PreNeg;
    Basis basis = Basis::SpeciesBased;

    double eps = 0.03;        // Error tolerance
    double p = 0.5;           // Acceptance probability threshold
    double q = 1.5;           // Barely acceptable tau multiplier
    double w = 0.75;          // Tau scaling factor
    double fixedTau = 0.0;    // Fixed timestep (0 = adaptive)
    double apx1 = 3.0;        // Approximation threshold 1
    double gg1 = 100.0;       // Gg1 parameter

    static PlaConfig parse(const std::string& configStr);
};

/**
 * Reaction classification for PLA
 */
enum class RxnClass {
    ExactStochastic = 0,  // Gillespie-style exact
    Poisson = 1,          // Poisson tau-leaping
    Langevin = 2,         // Langevin CLE
    Deterministic = 3     // Deterministic (ODE)
};

/**
 * PlaSimulator - Partitioned Leaping Algorithm
 *
 * Implements adaptive tau-leaping with automatic reaction classification.
 * Each reaction is classified as Exact Stochastic, Poisson, Langevin, or
 * Deterministic based on propensity magnitudes, then the appropriate
 * integration method is applied.
 *
 * Reference: bng2/Network3/src/pla/PLA.hh, PLA.cpp
 */
class PlaSimulator {
public:
    PlaSimulator(const ast::Model& model, const GeneratedNetwork& network);

    OdeResult simulate(const OdeOptions& opts, const PlaConfig& config);

private:
    struct CompiledReaction {
        std::vector<std::size_t> reactantIndices;
        std::vector<std::size_t> productIndices;
        double rateConstant;
        double statFactor;
        // Stoichiometry: species_index -> coefficient
        std::vector<std::pair<std::size_t, int>> stoich;
    };

    struct CompiledGroup {
        std::string name;
        std::vector<std::pair<std::size_t, double>> entries;
    };

    void compile();
    void compileGroups();

    double computePropensity(const CompiledReaction& rxn, const std::vector<double>& state) const;
    void computeAllPropensities(const std::vector<double>& state, std::vector<double>& a) const;
    double computeTotalPropensity(const std::vector<double>& a) const;

    // Tau calculation
    double computeTau(const std::vector<double>& state, const std::vector<double>& a,
                      const PlaConfig& config) const;

    // Reaction classification
    std::vector<RxnClass> classifyReactions(const std::vector<double>& a,
                                             const PlaConfig& config) const;

    // Firing generators
    void fireReactionsEuler(std::vector<double>& state, const std::vector<double>& a,
                            const std::vector<RxnClass>& classes, double tau,
                            std::mt19937& rng) const;

    // Postleap checking
    bool checkNegativePopulations(const std::vector<double>& state) const;
    void clampNegatives(std::vector<double>& state) const;

    // Observable computation
    void updateObservables(const std::vector<double>& state,
                          std::vector<double>& obsValues) const;

    const ast::Model& model_;
    const GeneratedNetwork& network_;
    std::vector<CompiledReaction> compiledRxns_;
    std::vector<CompiledGroup> compiledGroups_;
    std::vector<bool> fixedSpecies_;
    std::size_t nSpecies_ = 0;
};

} // namespace bng::engine
