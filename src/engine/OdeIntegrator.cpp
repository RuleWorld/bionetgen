#include "OdeIntegrator.hpp"

#include <algorithm>
#include <cmath>
#include <fstream>
#include <iomanip>
#include <random>
#include <sstream>
#include <stdexcept>

#include "antlr4-runtime.h"
#include "BNGLexer.h"
#include "BNGParser.h"
#include "parser/PatternGraphBuilder.hpp"
#include "parser/BNGAstVisitor.hpp"
#include "core/Ullmann.hpp"
#include "io/NetWriter.hpp"
#include "ast/ReactionRule.hpp"
#include "ast/Function.hpp"

// SUNDIALS/CVODE includes
extern "C" {
#include "sundials/sundials_types.h"
#include "cvode/cvode.h"
#include "nvector/nvector_serial.h"
#include "cvode/cvode_dense.h"
#include "cvode/cvode_spgmr.h"
#include "sundials/sundials_dense.h"
}

namespace bng::engine {

namespace {

// Recursive expression evaluator for rate strings.
// Handles: numbers, parameters, +, -, *, /, (), and nested expressions.
double evaluateRateString(const std::string& rateStr,
                          const std::function<double(const std::string&)>& resolve) {
    std::string s = rateStr;
    // Trim whitespace
    s.erase(0, s.find_first_not_of(" \t"));
    if (s.empty()) return 0.0;
    s.erase(s.find_last_not_of(" \t") + 1);

    // Strip outermost balanced parentheses
    if (s.size() >= 2 && s.front() == '(' && s.back() == ')') {
        int depth = 0;
        bool outermost = true;
        for (std::size_t i = 0; i < s.size() - 1; ++i) {
            if (s[i] == '(') ++depth;
            else if (s[i] == ')') { --depth; if (depth == 0) { outermost = false; break; } }
        }
        if (outermost) return evaluateRateString(s.substr(1, s.size() - 2), resolve);
    }

    // Find lowest-precedence operator at depth 0 (right-to-left for left-assoc)
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

    // Handle unary minus
    if (s.front() == '-') return -evaluateRateString(s.substr(1), resolve);

    // Handle exp(...)
    if (s.rfind("exp(", 0) == 0 && s.back() == ')') {
        return std::exp(evaluateRateString(s.substr(4, s.size() - 5), resolve));
    }

    // Try as number
    try {
        std::size_t pos = 0;
        double val = std::stod(s, &pos);
        if (pos == s.size()) return val;
    } catch (...) {}

    // Must be a parameter name
    return resolve(s);
}

} // anonymous namespace


OdeIntegrator::OdeIntegrator(const ast::Model& model, const GeneratedNetwork& network)
    : model_(model), network_(network) {
    compile();
}

void OdeIntegrator::compile() {
    nSpecies_ = network_.species.size();
    fixedSpecies_.resize(nSpecies_, false);

    for (std::size_t i = 0; i < nSpecies_; ++i) {
        fixedSpecies_[i] = network_.species.get(i).isConstant();
    }

    // Compile reactions
    auto paramResolver = [&](const std::string& name) -> double {
        return model_.getParameters().evaluate(name);
    };

    for (const auto& rxn : network_.reactions.all()) {
        CompiledReaction crxn;
        crxn.reactantIndices = rxn.getReactants();
        crxn.productIndices = rxn.getProducts();
        crxn.statFactor = rxn.getFactor();

        // Check if the origin rule has TotalRate modifier
        const auto& originRuleName = rxn.getOriginRuleName();
        if (!originRuleName.empty()) {
            // Strip _reverse__ prefix to find the base rule name
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

        // Build the complete rate string that NetWriter writes to .net file
        // Format: [unitConvFactor*][statFactor*]derivedParam OR [statFactor*]rawRate
        std::ostringstream rateStrBuilder;
        bool foundDerived = false;

        if (!originRuleName.empty()) {
            // Check if this is an Arrhenius rate with a derived parameter
            const bool isReverse = originRuleName.rfind("_reverse__", 0) == 0;
            std::string ruleBase = isReverse
                ? originRuleName.substr(std::string("_reverse__").size())
                : originRuleName;

            // Remove leading underscore if present
            if (!ruleBase.empty() && ruleBase[0] == '_' && ruleBase != "_reverse") {
                ruleBase = ruleBase.substr(1);
            }

            const std::string derivedParamName = isReverse
                ? ("__reverse__" + ruleBase + "_local1")
                : ("__" + ruleBase + "_local1");

            // Check if this derived parameter exists
            try {
                paramResolver(derivedParamName);
                foundDerived = true;

                // Build rate string with unit conversion and derived param
                // This matches what NetWriter writes: unitFactor*statFactor*derivedParam
                const auto unitFactor = bng::io::NetWriter::computeUnitConversionFactor(rxn, model_, network_);
                if (unitFactor.has_value()) {
                    rateStrBuilder << *unitFactor << "*";
                }
                if (std::abs(rxn.getFactor() - 1.0) >= 1e-9) {
                    rateStrBuilder << rxn.getFactor() << "*";
                }
                rateStrBuilder << derivedParamName;
            } catch (const std::exception& e) {
                foundDerived = false;
            }
        }

        if (!foundDerived) {
            // No derived parameter — apply volume scaling factor (Bug 3 fix)
            const auto unitFactor = bng::io::NetWriter::computeUnitConversionFactor(rxn, model_, network_);
            if (unitFactor.has_value()) {
                rateStrBuilder << *unitFactor << "*";
            }
            if (std::abs(rxn.getFactor() - 1.0) >= 1e-9) {
                rateStrBuilder << rxn.getFactor() << "*";
            }
            rateStrBuilder << rxn.getRateLaw();
        }

        std::string rateStr = rateStrBuilder.str();
        const std::string rawRateLaw = rxn.getRateLaw();

        // --- Bug 1 fix: Detect Sat/MM/Hill rate law types ---
        // Format in .net: "Sat kcat Km" or "MM kcat Km" or "Hill Vmax Kh n"
        bool isSatMMHill = false;
        {
            std::string rlLower = rawRateLaw;
            std::transform(rlLower.begin(), rlLower.end(), rlLower.begin(), ::tolower);
            // Trim leading whitespace
            auto start = rlLower.find_first_not_of(" \t");
            if (start != std::string::npos) rlLower = rlLower.substr(start);

            if (rlLower.rfind("sat(", 0) == 0 || rlLower.rfind("sat ", 0) == 0 || rlLower.rfind("sat\t", 0) == 0 ||
                rlLower.rfind("mm(", 0) == 0 || rlLower.rfind("mm ", 0) == 0 || rlLower.rfind("mm\t", 0) == 0 ||
                rlLower.rfind("hill(", 0) == 0 || rlLower.rfind("hill ", 0) == 0 || rlLower.rfind("hill\t", 0) == 0) {
                isSatMMHill = true;
            }
        }

        if (isSatMMHill && !crxn.reactantIndices.empty()) {
            // Expand Sat/MM/Hill to functional rate with implicit substrate
            // Sat kcat Km → rate = kcat * [S] / (Km + [S])  (TotalRate, no mass-action multiply)
            // Hill Vmax Kh n → rate = Vmax * [S]^n / (Kh^n + [S]^n)
            crxn.isFunctional = true;
            crxn.isTotalRate = true;  // Don't multiply by reactant conc again
            hasFunctionalRates_ = true;

            // Parse parameters from the rate law string
            // Handle both "Sat kcat Km" and "Sat(kcat, Km)" formats
            std::vector<std::string> paramNames;
            {
                std::string cleaned = rawRateLaw;
                // Strip function call syntax: Sat(kcat,Km) → kcat Km
                auto paren = cleaned.find('(');
                if (paren != std::string::npos) {
                    cleaned = cleaned.substr(paren + 1);
                    auto cparen = cleaned.rfind(')');
                    if (cparen != std::string::npos) cleaned = cleaned.substr(0, cparen);
                    // Split by comma
                    std::istringstream css(cleaned);
                    std::string tok;
                    while (std::getline(css, tok, ',')) {
                        // Trim whitespace
                        tok.erase(0, tok.find_first_not_of(" \t"));
                        tok.erase(tok.find_last_not_of(" \t") + 1);
                        if (!tok.empty()) paramNames.push_back(tok);
                    }
                } else {
                    // Space-separated: "Sat kcat Km"
                    std::istringstream rlStream(cleaned);
                    std::string typeName;
                    rlStream >> typeName;  // skip "Sat"/"MM"/"Hill"
                    std::string tok;
                    while (rlStream >> tok) paramNames.push_back(tok);
                }
            }

            // Store a lambda-like expression that will be evaluated in derivs()
            // We build the expression text for the resolver
            std::size_t substrateIdx = crxn.reactantIndices[0];

            if (paramNames.size() >= 2) {
                // Sat/MM(kcat, Km): rate = kcat * [S] / (Km + [S]) * [other reactants]
                // Hill(Vmax, Kh, n): rate = Vmax * [S]^n / (Kh^n + [S]^n) * [other reactants]
                // Substrate S = first reactant; other reactants multiplied via mass-action
                std::string kcat = paramNames[0];
                std::string Km = paramNames[1];

                // Build base Sat/MM/Hill expression with substrate
                ast::Expression baseExpr = (paramNames.size() >= 3)
                    ? ast::Expression::function("Hill",
                        {ast::Expression::identifier(kcat), ast::Expression::identifier(Km),
                         ast::Expression::identifier(paramNames[2]),
                         ast::Expression::identifier("__substrate_" + std::to_string(substrateIdx))})
                    : ast::Expression::function("Sat",
                        {ast::Expression::identifier(kcat), ast::Expression::identifier(Km),
                         ast::Expression::identifier("__substrate_" + std::to_string(substrateIdx))});

                // Multiply by non-substrate reactant concentrations (e.g., enzyme [E])
                for (std::size_t ri = 1; ri < crxn.reactantIndices.size(); ++ri) {
                    baseExpr = ast::Expression::binary("*", std::move(baseExpr),
                        ast::Expression::identifier("__substrate_" + std::to_string(crxn.reactantIndices[ri])));
                }

                // Apply unit and stat factors
                double combinedFactor = 1.0;
                const auto uf = bng::io::NetWriter::computeUnitConversionFactor(rxn, model_, network_);
                if (uf.has_value()) combinedFactor *= *uf;
                if (std::abs(rxn.getFactor() - 1.0) >= 1e-9) combinedFactor *= rxn.getFactor();
                if (std::abs(combinedFactor - 1.0) >= 1e-9) {
                    baseExpr = ast::Expression::binary("*",
                        ast::Expression::number(combinedFactor), std::move(baseExpr));
                }

                crxn.functionalRateExpr = std::move(baseExpr);
            }
        }

        // Check if this reaction has a functional rate (depends on time or observables)
        bool isFunctional = crxn.isFunctional;  // May already be set by Sat/MM/Hill
        const auto& rateExpr = rxn.getRateExpression();

        if (!isFunctional && rateExpr.has_value()) {
            std::string rateLawLower = rawRateLaw;
            std::transform(rateLawLower.begin(), rateLawLower.end(), rateLawLower.begin(), ::tolower);

            if (rateLawLower.find("time") != std::string::npos) {
                isFunctional = true;
            } else {
                // Check for observable dependencies
                auto deps = rateExpr->getDependencies();
                for (const auto& dep : deps) {
                    if (!model_.getParameters().contains(dep)) {
                        isFunctional = true;
                        break;
                    }
                }
            }

            // --- Bug 2 fix: Check for user-defined function references ---
            std::string matchedFuncName;
            if (!isFunctional) {
                const std::string rawRL = rxn.getRateLaw();
                for (const auto& func : model_.getFunctions()) {
                    const auto& fname = func.getName();
                    std::string fnameLower = fname;
                    std::transform(fnameLower.begin(), fnameLower.end(), fnameLower.begin(), ::tolower);
                    if (rateLawLower.find(fnameLower) != std::string::npos ||
                        rawRL.find(fname) != std::string::npos) {
                        isFunctional = true;
                        matchedFuncName = fname;
                        break;
                    }
                }
            }

            if (isFunctional) {
                crxn.isFunctional = true;
                if (!matchedFuncName.empty()) {
                    // Use identifier so the resolver evaluates the function body
                    crxn.functionalRateExpr = ast::Expression::identifier(matchedFuncName);
                } else {
                    crxn.functionalRateExpr = rateExpr;
                }
                crxn.rateConstant = 0.0;
                hasFunctionalRates_ = true;
            }
        }

        // Bug 2 fix: Also check for user-defined function references OUTSIDE the rateExpr block
        // This catches cases where rateExpr is nullopt but the rate law string references a function
        if (!crxn.isFunctional) {
            const std::string rawRL = rxn.getRateLaw();
            std::string rlLow = rawRL;
            std::transform(rlLow.begin(), rlLow.end(), rlLow.begin(), ::tolower);
            for (const auto& func : model_.getFunctions()) {
                std::string fnameLow = func.getName();
                std::transform(fnameLow.begin(), fnameLow.end(), fnameLow.begin(), ::tolower);
                if (rlLow.find(fnameLow) != std::string::npos) {
                    crxn.isFunctional = true;
                    // Build an identifier expression — the resolver in derivs() will
                    // find the function by name and evaluate its body
                    crxn.functionalRateExpr = ast::Expression::identifier(func.getName());
                    crxn.rateConstant = 0.0;
                    hasFunctionalRates_ = true;
                    break;
                }
            }
        }

        // Evaluate the rate string (only for non-functional, non-Sat/MM rates)
        if (!crxn.isFunctional) {
            try {
                crxn.rateConstant = evaluateRateString(rateStr, paramResolver);
            } catch (const std::exception&) {
                crxn.isFunctional = true;
                crxn.rateConstant = 0.0;
                hasFunctionalRates_ = true;
            }
        }

        compiledRxns_.push_back(crxn);
    }

    // Compile observable groups
    compileGroups();

    // Build performance optimization structures
    // 1. Observable name→index map for O(1) lookup in derivs()
    observableIndex_.clear();
    for (std::size_t i = 0; i < compiledGroups_.size(); ++i) {
        observableIndex_[compiledGroups_[i].name] = i;
    }

    // 2. Pre-allocate groupValues for reuse in derivs()
    groupValues_.resize(compiledGroups_.size(), 0.0);

    // 3. Separate constant-rate vs functional-rate reaction indices
    constantRxnIndices_.clear();
    functionalRxnIndices_.clear();
    for (std::size_t i = 0; i < compiledRxns_.size(); ++i) {
        if (compiledRxns_[i].isFunctional) {
            functionalRxnIndices_.push_back(i);
        } else {
            constantRxnIndices_.push_back(i);
        }
    }
}

void OdeIntegrator::compileGroups() {
    compiledGroups_.clear();

    // Note: const_cast needed because buildPatternGraph may infer molecule types
    auto& mutableModel = const_cast<ast::Model&>(model_);

    for (const auto& observable : model_.getObservables()) {
        CompiledGroup group;
        group.name = observable.getName();

        // Match each pattern against all species
        for (std::size_t speciesIndex = 0; speciesIndex < network_.species.size(); ++speciesIndex) {
            std::size_t weight = 0;

            for (const auto& patternText : observable.getPatterns()) {
                // Parse the observable pattern
                try {
                    // Extract optional quantifier (e.g., "> 2", "== 1", "<= 3")
                    std::string cleanPattern = patternText;
                    std::string quantOp;
                    int quantThreshold = 0;
                    bool hasQuantifier = false;

                    // Look for trailing quantifier: operators >=, <=, ==, !=, >, <
                    // Pattern: species_def WS* (OP WS* INT)
                    static const std::vector<std::string> ops = {">=", "<=", "==", "!=", ">", "<"};
                    for (const auto& op : ops) {
                        auto pos = cleanPattern.rfind(op);
                        if (pos != std::string::npos && pos > 0) {
                            std::string after = cleanPattern.substr(pos + op.size());
                            // Trim whitespace
                            after.erase(0, after.find_first_not_of(" \t"));
                            after.erase(after.find_last_not_of(" \t") + 1);
                            // Check if remainder is an integer
                            bool isInt = !after.empty();
                            for (char c : after) {
                                if (!std::isdigit(static_cast<unsigned char>(c))) { isInt = false; break; }
                            }
                            if (isInt) {
                                // Verify the character before the operator is whitespace or close-paren
                                // to avoid matching bond syntax like "!1"
                                std::string before = cleanPattern.substr(0, pos);
                                before.erase(before.find_last_not_of(" \t") + 1);
                                if (!before.empty() && (before.back() == ')' || std::isalnum(static_cast<unsigned char>(before.back())) || before.back() == '_')) {
                                    quantOp = op;
                                    quantThreshold = std::stoi(after);
                                    hasQuantifier = true;
                                    cleanPattern = before;
                                    break;
                                }
                            }
                        }
                    }

                    antlr4::ANTLRInputStream input(cleanPattern);
                    BNGLexer lexer(&input);
                    antlr4::CommonTokenStream tokens(&lexer);
                    BNGParser parser(&tokens);
                    auto* species = parser.species_def();

                    if (parser.getNumberOfSyntaxErrors() != 0) {
                        throw std::runtime_error("Could not parse observable pattern: " + patternText);
                    }

                    const auto pattern = bng::parser::buildPatternGraph(species, mutableModel, true);

                    // Check compartment qualifier: if pattern specifies a compartment,
                    // only match species in that same compartment
                    const auto patternCompartment = bng::parser::extractSpeciesCompartment(species);
                    if (!patternCompartment.empty()) {
                        const auto& speciesCompartment = network_.species.get(speciesIndex).getCompartment();
                        if (speciesCompartment != patternCompartment) {
                            continue;  // Skip — compartment mismatch
                        }
                    }

                    // Count matches
                    BNGcore::UllmannSGIso matcher(pattern, network_.species.get(speciesIndex).getSpeciesGraph().getGraph());
                    BNGcore::List<BNGcore::Map> maps;
                    std::size_t matchCount = matcher.find_maps(maps);

                    // Apply quantifier filter if present
                    if (hasQuantifier) {
                        bool passes = false;
                        int mc = static_cast<int>(matchCount);
                        if (quantOp == ">") passes = mc > quantThreshold;
                        else if (quantOp == "<") passes = mc < quantThreshold;
                        else if (quantOp == ">=") passes = mc >= quantThreshold;
                        else if (quantOp == "<=") passes = mc <= quantThreshold;
                        else if (quantOp == "==") passes = mc == quantThreshold;
                        else if (quantOp == "!=") passes = mc != quantThreshold;
                        matchCount = passes ? matchCount : 0;
                    }

                    weight += matchCount;
                } catch (const std::exception& e) {
                    throw std::runtime_error("Failed to compile observable " + observable.getName() +
                                           " pattern '" + patternText + "': " + e.what());
                }
            }

            // For "Species" observables, weight is 0 or 1 (presence/absence)
            if (observable.getType() == "Species" && weight > 0) {
                weight = 1;
            }

            if (weight > 0) {
                group.entries.push_back({speciesIndex, static_cast<double>(weight)});
            }
        }

        compiledGroups_.push_back(group);
    }
}

void OdeIntegrator::derivs(double t, const double* y, double* dydt) const {
    // Zero derivatives
    std::fill(dydt, dydt + nSpecies_, 0.0);

    // Update observables for functional rates (using pre-allocated buffer)
    if (hasFunctionalRates_) {
        updateGroups(y, groupValues_);
    }

    // Process constant-rate reactions first (no expression evaluation needed)
    for (const auto idx : constantRxnIndices_) {
        const auto& rxn = compiledRxns_[idx];
        double rate = rxn.rateConstant;

        if (!rxn.isTotalRate) {
            for (const auto ri : rxn.reactantIndices) {
                rate *= y[ri];
            }
        }

        for (const auto ri : rxn.reactantIndices) { dydt[ri] -= rate; }
        for (const auto pi : rxn.productIndices) { dydt[pi] += rate; }
    }

    // Process functional-rate reactions (require expression evaluation)
    if (hasFunctionalRates_) {
        // Build resolver with O(1) observable lookup
        // Use std::function to allow recursive self-reference for function evaluation
        std::function<double(const std::string&)> resolver;
        resolver = [&](const std::string& name) -> double {
            if (name == "time") return t;

            // Sat/MM/Hill substrate references: __substrate_N → y[N]
            if (name.rfind("__substrate_", 0) == 0) {
                std::size_t idx = std::stoul(name.substr(12));
                return (idx < nSpecies_) ? y[idx] : 0.0;
            }

            // O(1) observable lookup via precomputed map
            auto it = observableIndex_.find(name);
            if (it != observableIndex_.end()) {
                return groupValues_[it->second];
            }

            // Check user-defined functions (Bug 2 fix)
            for (const auto& func : model_.getFunctions()) {
                if (func.getName() == name) {
                    return func.getExpression().evaluate(resolver, t);
                }
            }

            // Otherwise try as parameter
            return model_.getParameters().evaluate(name);
        };

        for (const auto idx : functionalRxnIndices_) {
            const auto& rxn = compiledRxns_[idx];
            double rate;

            if (rxn.functionalRateExpr.has_value()) {
                try {
                    rate = rxn.functionalRateExpr->evaluate(resolver, t);
                } catch (const std::exception&) {
                    rate = rxn.rateConstant;
                }
            } else {
                rate = rxn.rateConstant;
            }

            if (!rxn.isTotalRate) {
                for (const auto ri : rxn.reactantIndices) {
                    rate *= y[ri];
                }
            }

            for (const auto ri : rxn.reactantIndices) { dydt[ri] -= rate; }
            for (const auto pi : rxn.productIndices) { dydt[pi] += rate; }
        }
    }

    // Zero derivatives for fixed (constant) species
    for (std::size_t i = 0; i < nSpecies_; ++i) {
        if (fixedSpecies_[i]) {
            dydt[i] = 0.0;
        }
    }
}

void OdeIntegrator::updateGroups(const double* y, std::vector<double>& groupValues) const {
    groupValues.resize(compiledGroups_.size(), 0.0);
    for (std::size_t i = 0; i < compiledGroups_.size(); ++i) {
        double value = 0.0;
        for (const auto& [speciesIdx, weight] : compiledGroups_[i].entries) {
            value += weight * y[speciesIdx];
        }
        groupValues[i] = value;
    }
}

OdeResult OdeIntegrator::integrate(const OdeOptions& options) {
    if (options.method == "euler") {
        return integrateEuler(options);
    } else if (options.method == "rk4") {
        return integrateRK4(options);
    } else if (options.method == "cvode") {
        return integrateCvode(options);
    } else if (options.method == "ssa") {
        return integrateSSA(options);
    } else {
        throw std::runtime_error("Unknown ODE method: " + options.method);
    }
}

OdeResult OdeIntegrator::integrateEuler(const OdeOptions& opts) {
    const double dt = opts.tEnd / static_cast<double>(opts.nSteps);
    std::vector<double> y(nSpecies_);

    // Parse stop_if expression at compile time
    std::optional<ast::Expression> stopIfExpr;
    if (!opts.stopIf.empty()) {
        try {
            stopIfExpr = parser::parseExpression(opts.stopIf);
        } catch (const std::exception& e) {
            std::cerr << "[bng_cpp] Warning: failed to parse stop_if expression: " << e.what() << "\n";
        }
    }

    // Initialize from species amounts
    for (std::size_t i = 0; i < nSpecies_; ++i) {
        y[i] = network_.species.get(i).getAmount();
    }

    OdeResult result;
    result.timePoints.reserve(opts.nSteps + 1);
    result.concentrations.reserve(opts.nSteps + 1);

    for (std::size_t step = 0; step <= opts.nSteps; ++step) {
        double t = step * dt;
        result.timePoints.push_back(t);
        result.concentrations.push_back(y);

        if (step < opts.nSteps) {
            std::vector<double> dydt(nSpecies_);
            derivs(t, y.data(), dydt.data());

            // Check steady-state condition (BNG2 parity)
            if (opts.steadyState && step > 0) {
                double sumSq = 0.0;
                for (std::size_t i = 0; i < nSpecies_; ++i) {
                    sumSq += dydt[i] * dydt[i];
                }
                double dx = std::sqrt(sumSq) / static_cast<double>(nSpecies_);
                if (dx < opts.steadyStateTol) {
                    std::cerr << "[bng_cpp] Steady state reached at step " << step
                              << ", t=" << t << ", dx=" << dx << "\n";
                    break;  // Stop early
                }
            }

            // Check stop_if condition (BNG2 parity)
            if (stopIfExpr.has_value() && step > 0) {
                std::vector<double> groupValues;
                updateGroups(y.data(), groupValues);

                auto resolver = [&](const std::string& name) -> double {
                    if (name == "time") return t;
                    for (std::size_t g = 0; g < compiledGroups_.size(); ++g) {
                        if (compiledGroups_[g].name == name) {
                            return groupValues[g];
                        }
                    }
                    return model_.getParameters().evaluate(name);
                };

                try {
                    double stopVal = stopIfExpr->evaluate(resolver, t);
                    if (stopVal != 0.0) {
                        std::cerr << "[bng_cpp] stop_if condition met at step " << step
                                  << ", t=" << t << ": " << opts.stopIf << "\n";
                        break;
                    }
                } catch (const std::exception& e) {
                    // Ignore evaluation errors in stop_if
                }
            }

            for (std::size_t i = 0; i < nSpecies_; ++i) {
                y[i] += dt * dydt[i];
                if (y[i] < 0.0) y[i] = 0.0;  // Clamp negative concentrations
            }
        }
    }

    // Compute observables for each time point
    result.observables.resize(result.timePoints.size());
    for (std::size_t step = 0; step <= opts.nSteps; ++step) {
        updateGroups(result.concentrations[step].data(), result.observables[step]);
    }

    return result;
}

OdeResult OdeIntegrator::integrateRK4(const OdeOptions& opts) {
    // Use internal sub-stepping for stability
    // For stiff systems, we need many more steps than the output points
    const std::size_t internalStepsPerOutput = 1000;  // Subdivide each output interval
    const double outputDt = opts.tEnd / static_cast<double>(opts.nSteps);
    const double dt = outputDt / static_cast<double>(internalStepsPerOutput);

    std::vector<double> y(nSpecies_);

    // Parse stop_if expression at compile time
    std::optional<ast::Expression> stopIfExpr;
    if (!opts.stopIf.empty()) {
        try {
            stopIfExpr = parser::parseExpression(opts.stopIf);
        } catch (const std::exception& e) {
            std::cerr << "[bng_cpp] Warning: failed to parse stop_if expression: " << e.what() << "\n";
        }
    }

    // Initialize from species amounts
    for (std::size_t i = 0; i < nSpecies_; ++i) {
        y[i] = network_.species.get(i).getAmount();
    }

    OdeResult result;
    result.timePoints.reserve(opts.nSteps + 1);
    result.concentrations.reserve(opts.nSteps + 1);

    std::vector<double> k1(nSpecies_), k2(nSpecies_), k3(nSpecies_), k4(nSpecies_);
    std::vector<double> yTemp(nSpecies_);

    double t = 0.0;
    bool stopEarly = false;

    for (std::size_t outputStep = 0; outputStep <= opts.nSteps; ++outputStep) {
        double targetT = outputStep * outputDt;

        // Save output at this time point
        result.timePoints.push_back(targetT);
        result.concentrations.push_back(y);

        // Check steady-state and stop_if at each output point
        if (outputStep > 0 && outputStep < opts.nSteps) {
            std::vector<double> dydt(nSpecies_);
            derivs(t, y.data(), dydt.data());

            // Steady-state detection (BNG2 parity)
            if (opts.steadyState) {
                double sumSq = 0.0;
                for (std::size_t i = 0; i < nSpecies_; ++i) {
                    sumSq += dydt[i] * dydt[i];
                }
                double dx = std::sqrt(sumSq) / static_cast<double>(nSpecies_);
                if (dx < opts.steadyStateTol) {
                    std::cerr << "[bng_cpp] Steady state reached at step " << outputStep
                              << ", t=" << t << ", dx=" << dx << "\n";
                    stopEarly = true;
                }
            }

            // stop_if condition (BNG2 parity)
            if (stopIfExpr.has_value()) {
                std::vector<double> groupValues;
                updateGroups(y.data(), groupValues);

                auto resolver = [&](const std::string& name) -> double {
                    if (name == "time") return t;
                    for (std::size_t g = 0; g < compiledGroups_.size(); ++g) {
                        if (compiledGroups_[g].name == name) {
                            return groupValues[g];
                        }
                    }
                    return model_.getParameters().evaluate(name);
                };

                try {
                    double stopVal = stopIfExpr->evaluate(resolver, t);
                    if (stopVal != 0.0) {
                        std::cerr << "[bng_cpp] stop_if condition met at step " << outputStep
                                  << ", t=" << t << ": " << opts.stopIf << "\n";
                        stopEarly = true;
                    }
                } catch (const std::exception& e) {
                    // Ignore evaluation errors
                }
            }
        }

        if (stopEarly || outputStep >= opts.nSteps) {
            break;
        }

        if (outputStep < opts.nSteps) {
            // Take internal steps to reach next output point
            for (std::size_t internalStep = 0; internalStep < internalStepsPerOutput; ++internalStep) {
                // k1 = f(t, y)
                derivs(t, y.data(), k1.data());

                // k2 = f(t + dt/2, y + dt*k1/2)
                for (std::size_t i = 0; i < nSpecies_; ++i) {
                    yTemp[i] = y[i] + 0.5 * dt * k1[i];
                    if (yTemp[i] < 0.0) yTemp[i] = 0.0;
                }
                derivs(t + 0.5 * dt, yTemp.data(), k2.data());

                // k3 = f(t + dt/2, y + dt*k2/2)
                for (std::size_t i = 0; i < nSpecies_; ++i) {
                    yTemp[i] = y[i] + 0.5 * dt * k2[i];
                    if (yTemp[i] < 0.0) yTemp[i] = 0.0;
                }
                derivs(t + 0.5 * dt, yTemp.data(), k3.data());

                // k4 = f(t + dt, y + dt*k3)
                for (std::size_t i = 0; i < nSpecies_; ++i) {
                    yTemp[i] = y[i] + dt * k3[i];
                    if (yTemp[i] < 0.0) yTemp[i] = 0.0;
                }
                derivs(t + dt, yTemp.data(), k4.data());

                // y_next = y + dt/6 * (k1 + 2*k2 + 2*k3 + k4)
                for (std::size_t i = 0; i < nSpecies_; ++i) {
                    y[i] += (dt / 6.0) * (k1[i] + 2.0 * k2[i] + 2.0 * k3[i] + k4[i]);
                    if (y[i] < 0.0) y[i] = 0.0;  // Clamp negative concentrations
                    if (!std::isfinite(y[i])) {
                        y[i] = 0.0;
                    }
                }

                t += dt;
            }
        }
    }

    // Compute observables for each time point
    result.observables.resize(result.timePoints.size());
    for (std::size_t step = 0; step <= opts.nSteps; ++step) {
        updateGroups(result.concentrations[step].data(), result.observables[step]);
    }

    return result;
}

void OdeIntegrator::writeOutputFiles(const std::string& prefix, const OdeResult& result, bool printCDAT, bool printFunctions) const {
    // Write .cdat (concentrations) - only if printCDAT is true
    if (printCDAT) {
        std::ofstream cdat(prefix + ".cdat");
        if (!cdat) {
            throw std::runtime_error("Failed to open " + prefix + ".cdat for writing");
        }

        for (std::size_t step = 0; step < result.timePoints.size(); ++step) {
            cdat << std::setw(18) << std::setprecision(12) << std::scientific
                 << result.timePoints[step];
            for (const auto& c : result.concentrations[step]) {
                cdat << " " << std::setw(18) << c;
            }
            cdat << "\n";
        }
    }

    // Write .gdat (observables)
    std::ofstream gdat(prefix + ".gdat");
    if (!gdat) {
        throw std::runtime_error("Failed to open " + prefix + ".gdat for writing");
    }

    // Header line
    gdat << "#";
    gdat << std::setw(17) << "time";
    for (const auto& group : compiledGroups_) {
        gdat << " " << std::setw(18) << group.name;
    }
    gdat << "\n";

    for (std::size_t step = 0; step < result.timePoints.size(); ++step) {
        gdat << std::setw(18) << std::setprecision(12) << std::scientific
             << result.timePoints[step];
        for (const auto& obs : result.observables[step]) {
            gdat << " " << std::setw(18) << obs;
        }
        gdat << "\n";
    }
}

// Static C-style callback for CVODE
static int cvodeCallbackWrapper(double t, N_Vector y, N_Vector ydot, void* user_data) {
    auto* self = static_cast<OdeIntegrator*>(user_data);
    self->derivs(t, NV_DATA_S(y), NV_DATA_S(ydot));
    return 0;
}

OdeResult OdeIntegrator::integrateCvode(const OdeOptions& opts) {
    // Parse stop_if expression at compile time
    std::optional<ast::Expression> stopIfExpr;
    if (!opts.stopIf.empty()) {
        try {
            stopIfExpr = parser::parseExpression(opts.stopIf);
        } catch (const std::exception& e) {
            std::cerr << "[bng_cpp] Warning: failed to parse stop_if expression: " << e.what() << "\n";
        }
    }

    // Initialize state vector
    N_Vector y = N_VNew_Serial(static_cast<long int>(nSpecies_));
    if (y == nullptr) {
        throw std::runtime_error("Failed to allocate CVODE state vector");
    }

    for (std::size_t i = 0; i < nSpecies_; ++i) {
        NV_Ith_S(y, i) = network_.species.get(i).getAmount();
    }

    // Create CVODE solver (BDF for stiff systems, Newton iteration)
    void* cvode_mem = CVodeCreate(CV_BDF, CV_NEWTON);
    if (cvode_mem == nullptr) {
        N_VDestroy_Serial(y);
        throw std::runtime_error("Failed to create CVODE solver");
    }

    // Initialize CVODE
    int flag = CVodeInit(cvode_mem, cvodeCallbackWrapper, 0.0, y);
    if (flag != CV_SUCCESS) {
        CVodeFree(&cvode_mem);
        N_VDestroy_Serial(y);
        throw std::runtime_error("CVodeInit failed with flag " + std::to_string(flag));
    }

    // Set tolerances (matching BNG2 defaults: rtol=1e-8, atol=1e-8)
    flag = CVodeSStolerances(cvode_mem, opts.rtol, opts.atol);
    if (flag != CV_SUCCESS) {
        CVodeFree(&cvode_mem);
        N_VDestroy_Serial(y);
        throw std::runtime_error("CVodeSStolerances failed");
    }

    // Set user data
    CVodeSetUserData(cvode_mem, this);

    // Set max number of steps (BNG2 default: 2000, auto-increases if needed)
    CVodeSetMaxNumSteps(cvode_mem, 2000);

    // Enable stability limit detection for stiff systems (reduces unnecessary small steps)
    CVodeSetStabLimDet(cvode_mem, 1);

    // Choose solver based on network size (matches BNG2 behavior)
    // Dense for small networks (<200 species), GMRES for large networks
    if (nSpecies_ < 200) {
        // Dense direct solver
        flag = CVDense(cvode_mem, static_cast<long int>(nSpecies_));
        if (flag != CV_SUCCESS) {
            CVodeFree(&cvode_mem);
            N_VDestroy_Serial(y);
            throw std::runtime_error("CVDense failed");
        }
    } else {
        // GMRES iterative solver (better for large sparse systems)
        flag = CVSpgmr(cvode_mem, PREC_NONE, 0);  // No preconditioner, auto Krylov dimension
        if (flag != CV_SUCCESS) {
            CVodeFree(&cvode_mem);
            N_VDestroy_Serial(y);
            throw std::runtime_error("CVSpgmr failed");
        }
    }

    // Integration loop
    OdeResult result;
    result.timePoints.reserve(opts.nSteps + 1);
    result.concentrations.reserve(opts.nSteps + 1);

    const double dt = opts.tEnd / static_cast<double>(opts.nSteps);
    double t = 0.0;

    for (std::size_t step = 0; step <= opts.nSteps; ++step) {
        double tOut = step * dt;

        // Save current state
        result.timePoints.push_back(tOut);
        std::vector<double> conc(nSpecies_);
        for (std::size_t i = 0; i < nSpecies_; ++i) {
            conc[i] = NV_Ith_S(y, i);
        }
        result.concentrations.push_back(conc);

        // Check stop_if condition after each output point (BNG2 parity)
        if (stopIfExpr.has_value() && step > 0 && step < opts.nSteps) {
            std::vector<double> groupValues;
            updateGroups(conc.data(), groupValues);

            auto resolver = [&](const std::string& name) -> double {
                if (name == "time") return tOut;
                for (std::size_t g = 0; g < compiledGroups_.size(); ++g) {
                    if (compiledGroups_[g].name == name) {
                        return groupValues[g];
                    }
                }
                return model_.getParameters().evaluate(name);
            };

            try {
                double stopVal = stopIfExpr->evaluate(resolver, tOut);
                if (stopVal != 0.0) {
                    std::cerr << "[bng_cpp] stop_if condition met at step " << step
                              << ", t=" << tOut << ": " << opts.stopIf << "\n";
                    break;
                }
            } catch (const std::exception& e) {
                // Ignore evaluation errors
            }
        }

        // Integrate to next output point
        if (step < opts.nSteps) {
            long int maxSteps = 2000;
            while (true) {
                flag = CVode(cvode_mem, tOut + dt, y, &t, CV_NORMAL);

                if (flag == CV_SUCCESS || flag == CV_TSTOP_RETURN) {
                    break;
                } else if (flag == CV_TOO_MUCH_WORK) {
                    // Auto-increase max steps (matches BNG2 behavior)
                    maxSteps *= 2;
                    CVodeSetMaxNumSteps(cvode_mem, maxSteps);
                    continue;
                } else {
                    CVodeFree(&cvode_mem);
                    N_VDestroy_Serial(y);
                    throw std::runtime_error("CVODE failed with flag " + std::to_string(flag));
                }
            }
        }
    }

    // Compute observables for each time point
    result.observables.resize(result.timePoints.size());
    for (std::size_t step = 0; step < result.timePoints.size(); ++step) {
        updateGroups(result.concentrations[step].data(), result.observables[step]);
    }

    // Cleanup
    CVodeFree(&cvode_mem);
    N_VDestroy_Serial(y);

    return result;
}

double OdeIntegrator::computePropensity(const CompiledReaction& rxn, const std::vector<double>& y) const {
    // Compute discrete propensity for SSA
    // For identical reactants A+A: propensity = k * n * (n-1)
    // The stat_factor is already baked into rateConstant

    double rateConstant = rxn.rateConstant;

    // If this is a functional rate, we need to evaluate it with current observables/time
    // For SSA, this is called at each propensity update, so we need the time
    // But we don't have time here - need to pass it
    // For now, use the cached rateConstant (functional rates in SSA need more work)

    double propensity = rateConstant;

    // For TotalRate, the propensity IS the rate constant (no species multiplication)
    if (rxn.isTotalRate) {
        return propensity;
    }

    // Group identical reactants and apply discrete formula
    // BNG2 assumes reactant indices are sorted, so identical species are adjacent
    double n_offset = 0.0;
    for (size_t i = 0; i < rxn.reactantIndices.size(); ++i) {
        std::size_t idx = rxn.reactantIndices[i];

        // Check if this is a repeat of the previous reactant
        if (i > 0 && rxn.reactantIndices[i] == rxn.reactantIndices[i-1]) {
            n_offset += 1.0;
        } else {
            n_offset = 0.0;
        }

        // Discrete formula: multiply by (population - offset)
        double population = y[idx];
        propensity *= std::max(0.0, population - n_offset);
    }

    return propensity;
}

OdeResult OdeIntegrator::integrateSSA(const OdeOptions& opts) {
    // Direct Gillespie algorithm (matches BNG2 implementation)
    std::mt19937_64 rng;
    if (opts.seed > 0) {
        rng.seed(opts.seed);
    } else {
        std::random_device rd;
        rng.seed(rd());
    }
    std::uniform_real_distribution<double> uniform(0.0, 1.0);

    // Initialize state (round to nearest integer)
    std::vector<double> y(nSpecies_);
    for (std::size_t i = 0; i < nSpecies_; ++i) {
        y[i] = std::round(network_.species.get(i).getAmount());
    }

    OdeResult result;
    result.timePoints.reserve(opts.nSteps + 1);
    result.concentrations.reserve(opts.nSteps + 1);

    double t = 0.0;
    const double dt = opts.tEnd / static_cast<double>(opts.nSteps);
    std::size_t nextOutputStep = 0;
    std::size_t ssaStepCount = 0;  // track internal SSA steps for output_step_interval

    // Compute initial propensities
    std::vector<double> propensities(compiledRxns_.size());
    double totalPropensity = 0.0;

    auto recomputePropensities = [&]() {
        totalPropensity = 0.0;
        for (std::size_t r = 0; r < compiledRxns_.size(); ++r) {
            propensities[r] = computePropensity(compiledRxns_[r], y);
            totalPropensity += propensities[r];
        }
    };

    recomputePropensities();

    // Record initial state
    if (opts.outputStepInterval > 0) {
        result.timePoints.push_back(t);
        result.concentrations.push_back(y);
    }

    // Main SSA loop
    while (t < opts.tEnd) {
        // Record output points as we pass them (only when not using output_step_interval)
        if (opts.outputStepInterval == 0) {
            while (nextOutputStep * dt <= t && nextOutputStep <= opts.nSteps) {
                result.timePoints.push_back(nextOutputStep * dt);
                result.concentrations.push_back(y);
                ++nextOutputStep;
            }
        }

        // Check if we've passed the end time
        if (t >= opts.tEnd || totalPropensity <= 0.0) {
            break;
        }

        // Compute time to next reaction
        double r1 = uniform(rng);
        while (r1 == 0.0 || r1 == 1.0) {
            r1 = uniform(rng);
        }
        double tau = -std::log(r1) / totalPropensity;

        // Check if next reaction would occur after end time
        if (t + tau > opts.tEnd) {
            t = opts.tEnd;
            break;
        }

        t += tau;

        // Select next reaction
        double r2 = uniform(rng);
        double cumulative = 0.0;
        double target = r2 * totalPropensity;
        std::size_t selectedRxn = compiledRxns_.size();

        for (std::size_t r = 0; r < compiledRxns_.size(); ++r) {
            cumulative += propensities[r];
            if (cumulative >= target) {
                selectedRxn = r;
                break;
            }
        }

        if (selectedRxn >= compiledRxns_.size()) {
            // Shouldn't happen unless roundoff error
            break;
        }

        // Fire the selected reaction
        const auto& rxn = compiledRxns_[selectedRxn];
        for (const auto idx : rxn.reactantIndices) {
            y[idx] -= 1.0;
            if (y[idx] < 0.0) y[idx] = 0.0;  // Safety clamp
        }
        for (const auto idx : rxn.productIndices) {
            y[idx] += 1.0;
        }

        // Recompute propensities
        // Optimization: could track which reactions are affected by this species change
        recomputePropensities();

        // output_step_interval: record output every N internal SSA steps
        ++ssaStepCount;
        if (opts.outputStepInterval > 0 && (ssaStepCount % opts.outputStepInterval) == 0) {
            result.timePoints.push_back(t);
            result.concentrations.push_back(y);
        }
    }

    if (opts.outputStepInterval > 0) {
        // Record final state at end time
        result.timePoints.push_back(t);
        result.concentrations.push_back(y);
    } else {
        // Record final state if we haven't already (uniform time-interval mode)
        while (nextOutputStep <= opts.nSteps) {
            result.timePoints.push_back(nextOutputStep * dt);
            result.concentrations.push_back(y);
            ++nextOutputStep;
        }
    }

    // Compute observables for each time point
    result.observables.resize(result.timePoints.size());
    for (std::size_t step = 0; step < result.timePoints.size(); ++step) {
        updateGroups(result.concentrations[step].data(), result.observables[step]);
    }

    return result;
}

} // namespace bng::engine
