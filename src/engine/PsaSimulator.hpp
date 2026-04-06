#pragma once

#include <random>
#include <string>
#include <vector>

#include "ast/Model.hpp"
#include "NetworkGenerator.hpp"
#include "OdeIntegrator.hpp"

namespace bng::engine {

/**
 * PsaSimulator - Partial Scaling Algorithm (PSA) / Partial Scaling Approximation Simulation
 *
 * Faithfully implements the PSA algorithm from BNG2 Network3/src/network.cpp:
 *   - init_adaptive_scaling_network()
 *   - adaptive_scaling_network()
 *   - rxn_rate_scaled()
 *   - update_concentrations_psa()
 *   - update_rxn_rates_psa()
 *
 * The PSA algorithm is a Gillespie-type SSA with adaptive propensity scaling.
 * For each reaction, a scaling factor s[i] is computed based on the minimum
 * population of reactants/products relative to poplevel. Propensities are
 * computed using scaled concentrations: rate = k * prod((X[r]/s - n_offset))
 * * s^(n_reactants-1), and concentrations are updated by +/- s[i] per firing
 * (instead of +/- 1 in standard SSA).
 *
 * When poplevel=0, scaling factors are all 1 and this degenerates to pure SSA.
 *
 * Reference: BNG2 Network3/src/network.cpp, lines ~2656-3053, 5282-5351,
 *            5909-5975, 6121-6182, 6279-6372
 */
class PsaSimulator {
public:
    PsaSimulator(const ast::Model& model, const GeneratedNetwork& network);

    /**
     * Run PSA simulation.
     * @param opts      Standard simulation options (t_end, n_steps, seed, etc.)
     * @param poplevel  Population scaling target. Scaling factor for each reaction
     *                  is floor(minPop / poplevel) where minPop is the minimum
     *                  population among reactants (and products if pScaleChecker).
     *                  When poplevel <= 0, scaling = 1 (pure SSA).
     * @param pScaleChecker  If true, also check product populations when
     *                       computing scaling factors. Default true (matches Perl).
     */
    OdeResult simulate(const OdeOptions& opts, double poplevel,
                       bool pScaleChecker = true);

private:
    struct CompiledReaction {
        std::vector<std::size_t> reactantIndices;  // 0-based species indices
        std::vector<std::size_t> productIndices;
        double rateConstant;    // evaluated base rate (k)
        double statFactor;      // statistical factor
        bool isTotalRate = false;
    };

    struct CompiledGroup {
        std::string name;
        std::vector<std::pair<std::size_t, double>> entries;  // (speciesIndex, weight)
    };

    void compile();
    void compileGroups();

    /**
     * Compute the scaled propensity for a reaction (ELEMENTARY rate law).
     * Faithful port of rxn_rate_scaled() from network.cpp.
     *
     * Computes: rate = stat_factor * k * prod((X[r]/s - n_offset)) * s^(n-1)
     * where s = floor(minPop / poplevel), minPop = min population among
     * reactants (and optionally products), and n_offset handles repeated
     * reactant species.
     *
     * @param rxnIdx      Index of the reaction
     * @param state       Current concentrations
     * @param poplevel    Population level threshold
     * @param pScaleChecker  Whether to check products too
     * @param[out] scaling  The computed scaling factor for this reaction
     * @return The scaled propensity
     */
    double rxnRateScaled(std::size_t rxnIdx, const std::vector<double>& state,
                         double poplevel, bool pScaleChecker,
                         double& scaling) const;

    /**
     * Update concentrations after firing reaction irxn (PSA version).
     * Faithful port of update_concentrations_psa() from network.cpp.
     *
     * Reactant concentrations decrease by s[irxn], product concentrations
     * increase by s[irxn]. Returns true if a force_update of rates is needed
     * (when any affected species drops below threshold).
     */
    bool updateConcentrationsPsa(std::size_t irxn, std::vector<double>& state,
                                 const std::vector<double>& scaling) const;

    /**
     * Update reaction rates after firing reaction irxn (PSA version).
     * Faithful port of update_rxn_rates_psa() from network.cpp.
     *
     * Recomputes propensities for all reactions that depend on the species
     * affected by reaction irxn.
     */
    void updateRxnRatesPsa(std::size_t irxn, std::vector<double>& propensities,
                           std::vector<double>& scaling, double& aTot,
                           const std::vector<double>& state,
                           double poplevel, bool pScaleChecker) const;

    /**
     * Select next reaction to fire using sorted linear search.
     * Faithful port of select_next_rxn() from network.cpp.
     *
     * Uses a propensity ordering vector that is dynamically sorted
     * to speed up selection (most likely reactions bubble to front).
     */
    std::size_t selectNextRxn(const std::vector<double>& propensities,
                              double aTot,
                              std::vector<std::size_t>& propOrder,
                              std::mt19937_64& rng) const;

    /**
     * Build dependency lists: for each reaction, which other reactions
     * need their propensities recomputed when this reaction fires.
     * Faithful port of create_update_lists() from network.cpp.
     */
    void createUpdateLists();

    // Observable computation
    void updateObservables(const std::vector<double>& state,
                           std::vector<double>& obsValues) const;

    const ast::Model& model_;
    const GeneratedNetwork& network_;
    std::vector<CompiledReaction> compiledRxns_;
    std::vector<CompiledGroup> compiledGroups_;
    std::vector<bool> fixedSpecies_;
    std::size_t nSpecies_ = 0;
    std::size_t nReactions_ = 0;

    // Dependency lists: rxnUpdateRxn_[i] = list of reaction indices whose
    // propensities must be recomputed when reaction i fires.
    std::vector<std::vector<std::size_t>> rxnUpdateRxn_;
};

} // namespace bng::engine
