#include "PsaSimulator.hpp"

#include <algorithm>
#include <cmath>
#include <iostream>
#include <set>
#include <sstream>
#include <stdexcept>

#include "antlr4-runtime.h"
#include "BNGLexer.h"
#include "BNGParser.h"
#include "parser/BNGAstVisitor.hpp"
#include "parser/PatternGraphBuilder.hpp"
#include "core/Ullmann.hpp"
#include "io/NetWriter.hpp"
#include "ast/ReactionRule.hpp"

namespace bng::engine {

namespace {

// Recursive expression evaluator (same as OdeIntegrator's internal helper)
double evaluateRateString(const std::string& rateStr,
                          const std::function<double(const std::string&)>& resolve) {
    std::string s = rateStr;
    s.erase(0, s.find_first_not_of(" \t"));
    if (s.empty()) return 0.0;
    s.erase(s.find_last_not_of(" \t") + 1);

    if (s.size() >= 2 && s.front() == '(' && s.back() == ')') {
        int depth = 0;
        bool outermost = true;
        for (std::size_t i = 0; i < s.size() - 1; ++i) {
            if (s[i] == '(') ++depth;
            else if (s[i] == ')') { --depth; if (depth == 0) { outermost = false; break; } }
        }
        if (outermost) return evaluateRateString(s.substr(1, s.size() - 2), resolve);
    }

    int depth = 0;
    int lastAddSub = -1;
    char lastAddSubOp = 0;
    int lastMulDiv = -1;
    char lastMulDivOp = 0;
    for (std::size_t i = 0; i < s.size(); ++i) {
        if (s[i] == '(') ++depth;
        else if (s[i] == ')') --depth;
        else if (depth == 0 && i > 0) {
            if (s[i] == '+' || s[i] == '-') { lastAddSub = static_cast<int>(i); lastAddSubOp = s[i]; }
            if (s[i] == '*' || s[i] == '/') { lastMulDiv = static_cast<int>(i); lastMulDivOp = s[i]; }
        }
    }

    if (lastAddSub >= 0) {
        double l = evaluateRateString(s.substr(0, lastAddSub), resolve);
        double r = evaluateRateString(s.substr(lastAddSub + 1), resolve);
        return lastAddSubOp == '+' ? l + r : l - r;
    }
    if (lastMulDiv >= 0) {
        double l = evaluateRateString(s.substr(0, lastMulDiv), resolve);
        double r = evaluateRateString(s.substr(lastMulDiv + 1), resolve);
        return lastMulDivOp == '*' ? l * r : l / r;
    }

    if (s.front() == '-') return -evaluateRateString(s.substr(1), resolve);

    if (s.rfind("exp(", 0) == 0 && s.back() == ')') {
        return std::exp(evaluateRateString(s.substr(4, s.size() - 5), resolve));
    }

    try {
        std::size_t pos = 0;
        double val = std::stod(s, &pos);
        if (pos == s.size()) return val;
    } catch (...) {}

    return resolve(s);
}

} // anonymous namespace


PsaSimulator::PsaSimulator(const ast::Model& model, const GeneratedNetwork& network)
    : model_(model), network_(network) {
    compile();
}

void PsaSimulator::compile() {
    nSpecies_ = network_.species.size();
    fixedSpecies_.resize(nSpecies_, false);

    for (std::size_t i = 0; i < nSpecies_; ++i) {
        fixedSpecies_[i] = network_.species.get(i).isConstant();
    }

    auto paramResolver = [&](const std::string& name) -> double {
        return model_.getParameters().evaluate(name);
    };

    for (const auto& rxn : network_.reactions.all()) {
        CompiledReaction crxn;
        crxn.reactantIndices = rxn.getReactants();
        crxn.productIndices = rxn.getProducts();
        crxn.statFactor = rxn.getFactor();

        // Check for TotalRate modifier
        const auto& originRuleName = rxn.getOriginRuleName();
        if (!originRuleName.empty()) {
            std::string lookupName = originRuleName;
            if (lookupName.rfind("_reverse__", 0) == 0) {
                lookupName = lookupName.substr(std::string("_reverse__").size());
            }
            for (const auto& rule : model_.getReactionRules()) {
                if (rule.getRuleName() == lookupName) {
                    for (const auto& mod : rule.getModifiers()) {
                        if (mod == "TotalRate") {
                            crxn.isTotalRate = true;
                            break;
                        }
                    }
                    break;
                }
            }
        }

        // Build rate string (same logic as OdeIntegrator::compile)
        std::ostringstream rateStrBuilder;
        bool foundDerived = false;

        if (!originRuleName.empty()) {
            const bool isReverse = originRuleName.rfind("_reverse__", 0) == 0;
            std::string ruleBase = isReverse
                ? originRuleName.substr(std::string("_reverse__").size())
                : originRuleName;
            if (!ruleBase.empty() && ruleBase[0] == '_' && ruleBase != "_reverse") {
                ruleBase = ruleBase.substr(1);
            }
            const std::string derivedParamName = isReverse
                ? ("__reverse__" + ruleBase + "_local1")
                : ("__" + ruleBase + "_local1");

            try {
                paramResolver(derivedParamName);
                foundDerived = true;
                const auto unitFactor = bng::io::NetWriter::computeUnitConversionFactor(rxn, model_, network_);
                if (unitFactor.has_value()) {
                    rateStrBuilder << *unitFactor << "*";
                }
                if (std::abs(rxn.getFactor() - 1.0) >= 1e-9) {
                    rateStrBuilder << rxn.getFactor() << "*";
                }
                rateStrBuilder << derivedParamName;
            } catch (const std::exception&) {
                foundDerived = false;
            }
        }

        if (!foundDerived) {
            if (std::abs(rxn.getFactor() - 1.0) >= 1e-9) {
                rateStrBuilder << rxn.getFactor() << "*";
            }
            rateStrBuilder << rxn.getRateLaw();
        }

        std::string rateStr = rateStrBuilder.str();
        try {
            crxn.rateConstant = evaluateRateString(rateStr, paramResolver);
        } catch (const std::exception&) {
            crxn.rateConstant = 0.0;
        }

        compiledRxns_.push_back(crxn);
    }

    nReactions_ = compiledRxns_.size();
    compileGroups();
    createUpdateLists();
}

void PsaSimulator::compileGroups() {
    compiledGroups_.clear();

    auto& mutableModel = const_cast<ast::Model&>(model_);

    for (const auto& observable : model_.getObservables()) {
        CompiledGroup group;
        group.name = observable.getName();

        for (std::size_t speciesIndex = 0; speciesIndex < network_.species.size(); ++speciesIndex) {
            std::size_t weight = 0;

            for (const auto& patternText : observable.getPatterns()) {
                try {
                    std::string cleanPattern = patternText;

                    antlr4::ANTLRInputStream input(cleanPattern);
                    BNGLexer lexer(&input);
                    antlr4::CommonTokenStream tokens(&lexer);
                    BNGParser parser(&tokens);
                    auto* species = parser.species_def();

                    if (parser.getNumberOfSyntaxErrors() != 0) {
                        continue;
                    }

                    const auto pattern = bng::parser::buildPatternGraph(species, mutableModel, true);

                    BNGcore::UllmannSGIso matcher(pattern, network_.species.get(speciesIndex).getSpeciesGraph().getGraph());
                    BNGcore::List<BNGcore::Map> maps;
                    std::size_t matchCount = matcher.find_maps(maps);

                    weight += matchCount;
                } catch (...) {
                    // Skip patterns that fail to parse
                }
            }

            if (weight > 0) {
                group.entries.emplace_back(speciesIndex, static_cast<double>(weight));
            }
        }

        compiledGroups_.push_back(group);
    }
}

void PsaSimulator::createUpdateLists() {
    // Build dependency lists: for each reaction, determine which reactions
    // need propensity updates when it fires.
    // This mirrors create_update_lists() from network.cpp.

    // Step 1: Build species-to-reaction maps.
    // asReactant[species] = set of reactions that have this species as a reactant.
    std::vector<std::set<std::size_t>> asReactant(nSpecies_);
    for (std::size_t r = 0; r < nReactions_; ++r) {
        for (const auto idx : compiledRxns_[r].reactantIndices) {
            asReactant[idx].insert(r);
        }
    }

    // Step 2: For each reaction, find all species it touches (reactants + products),
    // then collect all reactions that depend on those species.
    rxnUpdateRxn_.resize(nReactions_);
    for (std::size_t r = 0; r < nReactions_; ++r) {
        std::set<std::size_t> depRxns;

        // All species affected by firing reaction r
        for (const auto idx : compiledRxns_[r].reactantIndices) {
            for (const auto depR : asReactant[idx]) {
                depRxns.insert(depR);
            }
        }
        for (const auto idx : compiledRxns_[r].productIndices) {
            for (const auto depR : asReactant[idx]) {
                depRxns.insert(depR);
            }
        }

        rxnUpdateRxn_[r].assign(depRxns.begin(), depRxns.end());
    }
}

double PsaSimulator::rxnRateScaled(std::size_t rxnIdx,
                                     const std::vector<double>& state,
                                     double poplevel, bool pScaleChecker,
                                     double& scaling) const {
    // Faithful port of rxn_rate_scaled() from network.cpp (ELEMENTARY case).
    // For our C++ port, all compiled reactions are ELEMENTARY-like since
    // rate constants are pre-evaluated. The scaling logic is the key HAS feature.

    const auto& rxn = compiledRxns_[rxnIdx];
    const auto& reactants = rxn.reactantIndices;
    const auto& products = rxn.productIndices;
    const std::size_t nReactants = reactants.size();

    // TotalRate reactions: propensity = rate constant, no scaling
    if (rxn.isTotalRate) {
        scaling = 1.0;
        return rxn.rateConstant;
    }

    double rate = rxn.statFactor * rxn.rateConstant;

    // If poplevel <= 0 or no reactants, use standard SSA propensity
    if (poplevel <= 0.0 || nReactants == 0) {
        scaling = 1.0;
        // Standard discrete propensity: rate * prod(X[r] - n_offset)
        double n = 0.0;
        for (std::size_t i = 0; i < nReactants; ++i) {
            if (i > 0 && reactants[i] == reactants[i - 1]) {
                n += 1.0;
            } else {
                n = 0.0;
            }
            rate *= std::max(0.0, state[reactants[i]] - n);
        }
        return rate;
    }

    // HAS scaling computation (discrete case with poplevel > 0)
    // Mirrors network.cpp lines 2686-2737
    const double upperBound = 2.0 * poplevel;
    double tempPop = 1.0;
    double scalingExp = 0.0;

    // Determine tempPop = minimum "effective" population among reactants
    if (nReactants > 0) {
        if (state[reactants[0]] < upperBound) {
            tempPop = poplevel;
        } else {
            tempPop = state[reactants[0]];
            for (std::size_t i = 1; i < nReactants; ++i) {
                if (state[reactants[i]] < upperBound) {
                    tempPop = poplevel;
                    break;
                } else {
                    if (state[reactants[i]] < tempPop) {
                        tempPop = state[reactants[i]];
                    }
                }
            }
            // Check products too if pScaleChecker enabled
            if (pScaleChecker && tempPop >= upperBound) {
                for (std::size_t i = 0; i < products.size(); ++i) {
                    if (state[products[i]] < upperBound) {
                        tempPop = poplevel;
                        break;
                    } else {
                        if (state[products[i]] < tempPop) {
                            tempPop = state[products[i]];
                        }
                    }
                }
            }
        }

        scaling = std::floor(tempPop / poplevel);
        if (scaling < 1.0) {
            scaling = 1.0;
        }
    } else {
        // No reactants (zeroth-order): scaling = poplevel
        // Matches: iScaling = poplevel in the else branch of network.cpp
        scaling = poplevel;
    }

    // Compute scaled propensity: rate * prod((X[r]/s - n)) * s^(nReactants-1)
    // Mirrors network.cpp lines 2726-2737
    double n = 0.0;
    for (std::size_t i = 0; i < nReactants; ++i) {
        scalingExp += 1.0;
        if (i > 0) {
            if (reactants[i] == reactants[i - 1]) {
                n += 1.0;
            } else {
                n = 0.0;
            }
        }
        rate *= (state[reactants[i]] / scaling - n);
    }
    rate = rate * std::pow(scaling, scalingExp - 1.0);

    // Guard against negative rates (shouldn't happen but safety check)
    if (rate < 0.0) {
        rate = 0.0;
    }

    return rate;
}

bool PsaSimulator::updateConcentrationsHas(std::size_t irxn,
                                             std::vector<double>& state,
                                             const std::vector<double>& scaling) const {
    // Faithful port of update_concentrations_has() from network.cpp.
    // Concentration changes by +/- s[irxn] instead of +/- 1.
    const int threshOcc = 10;
    bool forceUpdate = false;
    const auto& rxn = compiledRxns_[irxn];
    const double s = scaling[irxn];

    // Loop over reactants: decrease by s
    for (const auto ri : rxn.reactantIndices) {
        if (!fixedSpecies_[ri]) {
            state[ri] -= s;
            if (state[ri] < 1.0) {
                state[ri] = 0.0;  // Prevent negative concentrations
            }
            if (state[ri] < threshOcc) {
                forceUpdate = true;
            }
        }
    }

    // Loop over products: increase by s
    for (const auto pi : rxn.productIndices) {
        if (!fixedSpecies_[pi]) {
            state[pi] += s;
            if (state[pi] <= threshOcc) {
                forceUpdate = true;
            }
        }
    }

    return forceUpdate;
}

void PsaSimulator::updateRxnRatesHas(std::size_t irxn,
                                       std::vector<double>& propensities,
                                       std::vector<double>& scaling,
                                       double& aTot,
                                       const std::vector<double>& state,
                                       double poplevel,
                                       bool pScaleChecker) const {
    // Faithful port of update_rxn_rates_has() from network.cpp.
    // Recompute propensities only for reactions in the dependency list of irxn.

    for (const auto jrxn : rxnUpdateRxn_[irxn]) {
        double anew = rxnRateScaled(jrxn, state, poplevel, pScaleChecker, scaling[jrxn]);
        aTot += anew - propensities[jrxn];
        propensities[jrxn] = anew;
    }

    // Error check: recalculate if a_tot goes negative (floating point drift)
    if (aTot < 0.0) {
        aTot = 0.0;
        for (std::size_t i = 0; i < nReactions_; ++i) {
            aTot += propensities[i];
        }
        if (aTot < 0.0) {
            // This should never happen; treat as zero
            aTot = 0.0;
        }
    }
}

std::size_t PsaSimulator::selectNextRxn(const std::vector<double>& propensities,
                                          double aTot,
                                          std::vector<std::size_t>& propOrder,
                                          std::mt19937_64& rng) const {
    // Faithful port of select_next_rxn() from network.cpp.
    // Uses sorted linear search with propensity ordering for speedup.

    std::uniform_real_distribution<double> dist(0.0, aTot);
    const std::size_t na = propOrder.size();

    while (true) {
        // Generate random number between 0 and aTot
        double f = dist(rng);
        while (f == 0.0) {
            f = dist(rng);
        }

        // Find reaction corresponding to random sample (sorted linear search)
        double aSum = 0.0;
        std::size_t irxn = 0;
        for (irxn = 0; irxn < na; ++irxn) {
            aSum += propensities[propOrder[irxn]];
            if (f <= aSum) break;
            // Speed up: if neighboring propensities are in descending order, swap
            if (irxn > 0 && propensities[propOrder[irxn]] > propensities[propOrder[irxn - 1]]) {
                std::swap(propOrder[irxn], propOrder[irxn - 1]);
            }
        }

        if (irxn == na) {
            // Picked a reaction that doesn't exist - recalculate aTot
            double newATot = aSum;
            if (newATot == 0.0) {
                return na;  // No reactions have positive propensity
            }
            // Update distribution bounds and retry
            dist = std::uniform_real_distribution<double>(0.0, newATot);
        } else {
            return propOrder[irxn];
        }
    }
}

void PsaSimulator::updateObservables(const std::vector<double>& state,
                                      std::vector<double>& obsValues) const {
    obsValues.resize(compiledGroups_.size());
    for (std::size_t g = 0; g < compiledGroups_.size(); ++g) {
        double val = 0.0;
        for (const auto& [idx, weight] : compiledGroups_[g].entries) {
            val += weight * state[idx];
        }
        obsValues[g] = val;
    }
}

OdeResult PsaSimulator::simulate(const OdeOptions& opts, double poplevel,
                                  bool pScaleChecker) {
    // Faithful port of init_adaptive_scaling_network() + adaptive_scaling_network()
    // from network.cpp.

    // === INITIALIZATION (init_adaptive_scaling_network) ===

    // Seed RNG
    std::mt19937_64 rng;
    if (opts.seed > 0) {
        rng.seed(opts.seed);
    } else {
        std::random_device rd;
        rng.seed(rd());
    }
    std::uniform_real_distribution<double> uniform(0.0, 1.0);

    // Initialize concentration array and round to nearest integer
    std::vector<double> c(nSpecies_);
    for (std::size_t i = 0; i < nSpecies_; ++i) {
        c[i] = std::round(network_.species.get(i).getAmount());
    }

    // Initialize scaling factor array
    std::vector<double> s(nReactions_, 1.0);

    // Initialize propensity array using rxn_rate_scaled
    std::vector<double> a(nReactions_, 0.0);
    double aTot = 0.0;
    for (std::size_t i = 0; i < nReactions_; ++i) {
        a[i] = rxnRateScaled(i, c, poplevel, pScaleChecker, s[i]);
        aTot += a[i];
    }

    // Initialize propensity ordering (for sorted linear search in select_next_rxn)
    std::vector<std::size_t> propOrder(nReactions_);
    for (std::size_t i = 0; i < nReactions_; ++i) {
        propOrder[i] = i;
    }

    // rxn_rate_update_interval = 1 (always update, matching network.cpp)
    const int rxnRateUpdateInterval = 1;

    // === PREPARE OUTPUT ===

    OdeResult result;
    result.timePoints.reserve(opts.nSteps + 1);
    result.concentrations.reserve(opts.nSteps + 1);

    const double dt = opts.tEnd / static_cast<double>(opts.nSteps);
    double t = 0.0;

    // Determine max simulation steps
    const double maxStep = (opts.maxSimSteps > 0)
        ? static_cast<double>(opts.maxSimSteps)
        : std::numeric_limits<double>::max();

    // Record initial state
    result.timePoints.push_back(0.0);
    result.concentrations.push_back(c);

    double nSteps = 0.0;
    bool hitMaxSteps = false;
    bool hitStopCondition = false;

    // === MAIN SIMULATION LOOP (adaptive_scaling_network) ===
    // Process each output interval [t, t + dt]
    for (std::size_t step = 1; step <= opts.nSteps; ++step) {
        const double tEnd = step * dt;
        double tRemain = tEnd - t;

        while (tRemain > 0.0) {
            // Check maxStep limit
            if (nSteps >= maxStep) {
                hitMaxSteps = true;
                break;
            }

            // Determine time to next reaction
            double rnd = uniform(rng);
            while (rnd == 0.0 || rnd == 1.0) {
                rnd = uniform(rng);
            }
            double tau = -std::log(rnd) / aTot;

            tRemain -= tau;

            // Don't fire the next reaction if it occurs past the current
            // integration endpoint
            if (tRemain < 0.0) break;

            // Select next reaction to fire
            std::size_t irxn = selectNextRxn(a, aTot, propOrder, rng);
            if (irxn == nReactions_) break;  // a_tot = 0.0

            // Fire reaction by updating concentrations (HAS version)
            bool rxnRateUpdate = updateConcentrationsHas(irxn, c, s);
            nSteps += 1.0;

            // Update reaction rates
            // Matches: if (rxn_rate_update || fmod <= 1e-12)
            double gspInterval = static_cast<double>(rxnRateUpdateInterval);
            double fmod = nSteps - static_cast<double>(static_cast<long>(nSteps / gspInterval)) * gspInterval;
            if (rxnRateUpdate || fmod <= 1e-12) {
                updateRxnRatesHas(irxn, a, s, aTot, c, poplevel, pScaleChecker);
            }
        }

        if (hitMaxSteps || hitStopCondition) {
            // Adjust time to where we actually got
            t = tEnd - std::max(0.0, tRemain);
        } else {
            // Back up to return time (t_remain < 0 means no rxn fired past endpoint)
            t = tEnd;
        }

        // Record state at this output time
        result.timePoints.push_back(step * dt);
        result.concentrations.push_back(c);

        if (hitMaxSteps || hitStopCondition) {
            break;
        }
    }

    // Fill remaining output points if we stopped early
    while (result.timePoints.size() <= opts.nSteps) {
        result.timePoints.push_back(result.timePoints.back());
        result.concentrations.push_back(c);
    }

    // Compute observables for each time point
    result.observables.resize(result.timePoints.size());
    for (std::size_t step = 0; step < result.timePoints.size(); ++step) {
        updateObservables(result.concentrations[step], result.observables[step]);
    }

    return result;
}

} // namespace bng::engine
