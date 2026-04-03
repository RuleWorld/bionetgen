#include "PlaSimulator.hpp"

#include <algorithm>
#include <cmath>
#include <limits>
#include <numeric>
#include <sstream>
#include <stdexcept>

#include "antlr4-runtime.h"
#include "BNGLexer.h"
#include "BNGParser.h"
#include "parser/PatternGraphBuilder.hpp"
#include "core/Ullmann.hpp"

namespace bng::engine {

namespace {

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
    if (s.rfind("exp(", 0) == 0 && s.back() == ')')
        return std::exp(evaluateRateString(s.substr(4, s.size() - 5), resolve));

    try {
        std::size_t pos = 0;
        double val = std::stod(s, &pos);
        if (pos == s.size()) return val;
    } catch (...) {}

    return resolve(s);
}

} // anonymous namespace

// ===== PlaConfig =====

PlaConfig PlaConfig::parse(const std::string& configStr) {
    PlaConfig config;

    // Split by '|'
    std::vector<std::string> parts;
    std::string current;
    for (char c : configStr) {
        if (c == '|') {
            parts.push_back(current);
            current.clear();
        } else {
            current += c;
        }
    }
    if (!current.empty()) parts.push_back(current);

    // Part 1: Method
    if (!parts.empty()) {
        const auto& m = parts[0];
        if (m == "fEuler" || m == "feuler") config.method = Method::fEuler;
        else if (m == "midpt" || m == "midpoint") config.method = Method::Midpoint;
        else if (m == "rk4" || m == "RK4") config.method = Method::RK4;
        else config.method = Method::fEuler;
    }

    // Part 2: Check type + basis
    if (parts.size() > 1) {
        const auto& check = parts[1];
        if (check.find("pre-neg") != std::string::npos) config.checkType = CheckType::PreNeg;
        else if (check.find("pre-eps") != std::string::npos) config.checkType = CheckType::PreEps;
        else if (check.find("post-neg") != std::string::npos) config.checkType = CheckType::PostNeg;
        else if (check.find("post-eps") != std::string::npos) config.checkType = CheckType::PostEps;
        else if (check.find("fixed") != std::string::npos) config.checkType = CheckType::Fixed;

        if (check.find(":rb") != std::string::npos) config.basis = Basis::ReactionBased;
        else config.basis = Basis::SpeciesBased;
    }

    // Part 3: Parameters
    if (parts.size() > 2) {
        // Split by ','
        std::istringstream paramStream(parts[2]);
        std::string param;
        while (std::getline(paramStream, param, ',')) {
            auto eqPos = param.find('=');
            if (eqPos == std::string::npos) continue;
            std::string key = param.substr(0, eqPos);
            std::string val = param.substr(eqPos + 1);
            try {
                if (key == "eps") config.eps = std::stod(val);
                else if (key == "p") config.p = std::stod(val);
                else if (key == "q") config.q = std::stod(val);
                else if (key == "w") config.w = std::stod(val);
                else if (key == "tau") config.fixedTau = std::stod(val);
                else if (key == "apx1") config.apx1 = std::stod(val);
                else if (key == "gg1") config.gg1 = std::stod(val);
            } catch (...) {}
        }
    }

    return config;
}

// ===== PlaSimulator =====

PlaSimulator::PlaSimulator(const ast::Model& model, const GeneratedNetwork& network)
    : model_(model), network_(network) {
    compile();
    compileGroups();
}

void PlaSimulator::compile() {
    nSpecies_ = network_.species.size();
    fixedSpecies_.resize(nSpecies_, false);

    for (std::size_t i = 0; i < nSpecies_; ++i) {
        fixedSpecies_[i] = network_.species.get(i).isConstant();
    }

    const auto resolver = [&](const std::string& name) -> double {
        return model_.getParameters().evaluate(name);
    };

    compiledRxns_.clear();
    for (const auto& rxn : network_.reactions.all()) {
        CompiledReaction crxn;
        crxn.reactantIndices = rxn.getReactants();
        crxn.productIndices = rxn.getProducts();
        crxn.statFactor = rxn.getFactor();

        // Parse rate constant
        std::string rateLaw = rxn.getRateLaw();
        auto localPos = rateLaw.find("|local:");
        if (localPos != std::string::npos) rateLaw = rateLaw.substr(0, localPos);

        try {
            crxn.rateConstant = evaluateRateString(rateLaw, resolver);
        } catch (...) {
            crxn.rateConstant = 0.0;
        }

        // Compute stoichiometry
        std::map<std::size_t, int> stoichMap;
        for (auto idx : crxn.reactantIndices) stoichMap[idx] -= 1;
        for (auto idx : crxn.productIndices) stoichMap[idx] += 1;
        for (auto& [idx, coeff] : stoichMap) {
            if (coeff != 0) crxn.stoich.push_back({idx, coeff});
        }

        compiledRxns_.push_back(std::move(crxn));
    }
}

void PlaSimulator::compileGroups() {
    compiledGroups_.clear();
    auto& mutableModel = const_cast<ast::Model&>(model_);

    for (const auto& observable : model_.getObservables()) {
        CompiledGroup group;
        group.name = observable.getName();

        for (std::size_t speciesIndex = 0; speciesIndex < network_.species.size(); ++speciesIndex) {
            std::size_t weight = 0;
            for (const auto& patternText : observable.getPatterns()) {
                try {
                    antlr4::ANTLRInputStream input(patternText);
                    BNGLexer lexer(&input);
                    antlr4::CommonTokenStream tokens(&lexer);
                    BNGParser parser(&tokens);
                    auto* species = parser.species_def();
                    if (parser.getNumberOfSyntaxErrors() == 0) {
                        const auto pattern = bng::parser::buildPatternGraph(species, mutableModel);
                        BNGcore::UllmannSGIso matcher(pattern,
                            network_.species.get(speciesIndex).getSpeciesGraph().getGraph());
                        BNGcore::List<BNGcore::Map> maps;
                        weight += matcher.find_maps(maps);
                    }
                } catch (...) {}
            }
            if (observable.getType() == "Species" && weight > 0) weight = 1;
            if (weight > 0) group.entries.push_back({speciesIndex, static_cast<double>(weight)});
        }
        compiledGroups_.push_back(std::move(group));
    }
}

double PlaSimulator::computePropensity(const CompiledReaction& rxn, const std::vector<double>& state) const {
    double prop = rxn.statFactor * rxn.rateConstant;

    for (auto idx : rxn.reactantIndices) {
        if (idx < state.size()) {
            prop *= state[idx];
        } else {
            return 0.0;
        }
    }

    // Handle dimerization: if same reactant appears twice, use n*(n-1)/2
    if (rxn.reactantIndices.size() == 2 && rxn.reactantIndices[0] == rxn.reactantIndices[1]) {
        double n = state[rxn.reactantIndices[0]];
        prop = rxn.statFactor * rxn.rateConstant * n * (n - 1.0);
    }

    return std::max(0.0, prop);
}

void PlaSimulator::computeAllPropensities(const std::vector<double>& state, std::vector<double>& a) const {
    a.resize(compiledRxns_.size());
    for (std::size_t r = 0; r < compiledRxns_.size(); ++r) {
        a[r] = computePropensity(compiledRxns_[r], state);
    }
}

double PlaSimulator::computeTotalPropensity(const std::vector<double>& a) const {
    return std::accumulate(a.begin(), a.end(), 0.0);
}

double PlaSimulator::computeTau(const std::vector<double>& state, const std::vector<double>& a,
                                 const PlaConfig& config) const {
    if (config.fixedTau > 0.0) return config.fixedTau;

    double a0 = computeTotalPropensity(a);
    if (a0 <= 0.0) return std::numeric_limits<double>::infinity();

    // Adaptive tau calculation (Cao et al. 2006)
    // For each species, compute bounds on acceptable tau
    double tauMin = std::numeric_limits<double>::infinity();
    const double eps = config.eps;

    for (std::size_t s = 0; s < nSpecies_; ++s) {
        if (fixedSpecies_[s] || state[s] <= 0.0) continue;

        // Compute mu_s (expected change) and sigma2_s (variance of change)
        double mu = 0.0;
        double sigma2 = 0.0;

        for (std::size_t r = 0; r < compiledRxns_.size(); ++r) {
            if (a[r] <= 0.0) continue;
            for (const auto& [idx, coeff] : compiledRxns_[r].stoich) {
                if (idx == s) {
                    mu += coeff * a[r];
                    sigma2 += coeff * coeff * a[r];
                    break;
                }
            }
        }

        if (std::abs(mu) < 1e-30 && sigma2 < 1e-30) continue;

        // Tau bounds
        double bound_mu = std::max(eps * state[s], 1.0) / std::abs(mu + 1e-30);
        double bound_sigma = std::max(eps * state[s], 1.0) * std::max(eps * state[s], 1.0)
                            / (sigma2 + 1e-30);

        double tau_s = std::min(bound_mu, bound_sigma);
        tauMin = std::min(tauMin, tau_s);
    }

    // Don't let tau be too small
    double exactTau = (a0 > 0.0) ? (1.0 / a0) : std::numeric_limits<double>::infinity();
    if (tauMin < config.apx1 * exactTau) {
        // Fall back to exact stochastic for this step
        tauMin = exactTau;
    }

    return tauMin;
}

std::vector<RxnClass> PlaSimulator::classifyReactions(const std::vector<double>& a,
                                                       const PlaConfig& config) const {
    std::vector<RxnClass> classes(compiledRxns_.size(), RxnClass::Poisson);
    double a0 = computeTotalPropensity(a);
    if (a0 <= 0.0) return classes;

    for (std::size_t r = 0; r < compiledRxns_.size(); ++r) {
        double frac = a[r] / a0;

        if (a[r] <= 0.0) {
            classes[r] = RxnClass::ExactStochastic;
        } else if (a[r] < config.apx1) {
            // Low propensity: exact stochastic
            classes[r] = RxnClass::ExactStochastic;
        } else if (a[r] < config.gg1) {
            // Medium propensity: Poisson tau-leaping
            classes[r] = RxnClass::Poisson;
        } else {
            // High propensity: deterministic
            classes[r] = RxnClass::Deterministic;
        }
    }

    return classes;
}

void PlaSimulator::fireReactionsEuler(std::vector<double>& state, const std::vector<double>& a,
                                       const std::vector<RxnClass>& classes, double tau,
                                       std::mt19937& rng) const {
    for (std::size_t r = 0; r < compiledRxns_.size(); ++r) {
        if (a[r] <= 0.0) continue;

        int firings = 0;

        switch (classes[r]) {
            case RxnClass::ExactStochastic: {
                // Exact: Poisson with mean = a[r] * tau (for small tau this is ~Bernoulli)
                double mean = a[r] * tau;
                if (mean < 20.0) {
                    std::poisson_distribution<int> poisson(mean);
                    firings = poisson(rng);
                } else {
                    // Normal approximation for large lambda
                    std::normal_distribution<double> normal(mean, std::sqrt(mean));
                    firings = std::max(0, static_cast<int>(std::round(normal(rng))));
                }
                break;
            }
            case RxnClass::Poisson: {
                double mean = a[r] * tau;
                if (mean < 20.0) {
                    std::poisson_distribution<int> poisson(mean);
                    firings = poisson(rng);
                } else {
                    std::normal_distribution<double> normal(mean, std::sqrt(mean));
                    firings = std::max(0, static_cast<int>(std::round(normal(rng))));
                }
                break;
            }
            case RxnClass::Langevin: {
                // CLE: deterministic + noise
                double mean = a[r] * tau;
                double noise = std::sqrt(a[r] * tau);
                std::normal_distribution<double> normal(0.0, 1.0);
                firings = std::max(0, static_cast<int>(std::round(mean + noise * normal(rng))));
                break;
            }
            case RxnClass::Deterministic: {
                firings = static_cast<int>(std::round(a[r] * tau));
                break;
            }
        }

        // Apply stoichiometry changes
        if (firings > 0) {
            for (const auto& [idx, coeff] : compiledRxns_[r].stoich) {
                if (!fixedSpecies_[idx]) {
                    state[idx] += coeff * firings;
                }
            }
        }
    }
}

bool PlaSimulator::checkNegativePopulations(const std::vector<double>& state) const {
    for (std::size_t s = 0; s < nSpecies_; ++s) {
        if (!fixedSpecies_[s] && state[s] < -0.5) return true;
    }
    return false;
}

void PlaSimulator::clampNegatives(std::vector<double>& state) const {
    for (std::size_t s = 0; s < nSpecies_; ++s) {
        if (state[s] < 0.0) state[s] = 0.0;
    }
}

void PlaSimulator::updateObservables(const std::vector<double>& state,
                                      std::vector<double>& obsValues) const {
    obsValues.resize(compiledGroups_.size(), 0.0);
    for (std::size_t g = 0; g < compiledGroups_.size(); ++g) {
        double val = 0.0;
        for (const auto& [idx, weight] : compiledGroups_[g].entries) {
            val += weight * state[idx];
        }
        obsValues[g] = val;
    }
}

OdeResult PlaSimulator::simulate(const OdeOptions& opts, const PlaConfig& config) {
    OdeResult result;
    const std::size_t nSteps = opts.nSteps;
    const double tEnd = opts.tEnd;
    const double dtOutput = tEnd / static_cast<double>(nSteps);

    // Initialize state
    std::vector<double> state(nSpecies_);
    for (std::size_t i = 0; i < nSpecies_; ++i) {
        state[i] = network_.species.get(i).getAmount();
    }

    // Random number generator
    std::mt19937 rng(opts.seed > 0 ? opts.seed : std::random_device{}());

    // Record initial state
    result.timePoints.push_back(0.0);
    result.concentrations.push_back(state);
    std::vector<double> obsValues;
    updateObservables(state, obsValues);
    result.observables.push_back(obsValues);

    double t = 0.0;
    std::size_t outputStep = 1;

    // Main simulation loop
    while (t < tEnd && outputStep <= nSteps) {
        double nextOutputTime = outputStep * dtOutput;

        while (t < nextOutputTime) {
            // Compute propensities
            std::vector<double> a;
            computeAllPropensities(state, a);

            double a0 = computeTotalPropensity(a);
            if (a0 <= 0.0) {
                // No reactions possible - jump to end
                t = tEnd;
                break;
            }

            // Compute tau
            double tau = computeTau(state, a, config);
            if (tau == std::numeric_limits<double>::infinity()) {
                t = tEnd;
                break;
            }

            // Cap tau to not overshoot output time
            if (t + tau > nextOutputTime) {
                tau = nextOutputTime - t;
            }

            // Classify reactions
            auto classes = classifyReactions(a, config);

            // Save state for potential rollback
            std::vector<double> savedState = state;

            // Fire reactions
            fireReactionsEuler(state, a, classes, tau, rng);

            // Post-leap check
            if (config.checkType == PlaConfig::CheckType::PreNeg ||
                config.checkType == PlaConfig::CheckType::PostNeg) {
                if (checkNegativePopulations(state)) {
                    // Rollback and try with smaller tau
                    state = savedState;
                    tau *= config.w;
                    if (tau < 1e-15) {
                        // Give up and clamp
                        fireReactionsEuler(state, a, classes, tau, rng);
                        clampNegatives(state);
                    }
                    continue;
                }
            }

            // Clamp any small negatives from floating point
            clampNegatives(state);

            t += tau;
        }

        // Record output
        result.timePoints.push_back(std::min(t, tEnd));
        result.concentrations.push_back(state);
        updateObservables(state, obsValues);
        result.observables.push_back(obsValues);
        outputStep++;
    }

    return result;
}

} // namespace bng::engine
