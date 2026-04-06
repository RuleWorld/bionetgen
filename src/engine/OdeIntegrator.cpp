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

// SUNDIALS/CVODE includes (v7.x API)
#include "sundials/sundials_context.h"
#include "sundials/sundials_types.h"
#include "cvode/cvode.h"
#include "nvector/nvector_serial.h"
#include "sunlinsol/sunlinsol_dense.h"
#include "sunlinsol/sunlinsol_spgmr.h"
#include "sunmatrix/sunmatrix_dense.h"

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

    // Build per-reaction derived rate parameter map from NetWriter's naming convention.
    // This handles local functions (%x:: scope prefix), energy patterns, and Arrhenius
    // rates where the NetWriter creates per-reaction derived parameters.
    std::unordered_map<std::size_t, std::pair<std::string, double>> perRxnDerivedRates;
    {
        auto derived = bng::io::NetWriter::buildDerivedRateParams(model_, network_);
        for (const auto& [ruleName, info] : derived) {
            if (info.isPerReactionArrhenius || info.isLocalFunction) {
                for (const auto& [rxnIdx, paramPair] : info.perReactionRates) {
                    perRxnDerivedRates[rxnIdx] = paramPair;
                }
                // Also build from perSpeciesRates by walking the reaction list
                if (!info.perReactionRates.empty()) continue;
                std::size_t rxnIdx = 0;
                for (const auto& rxn2 : network_.reactions.all()) {
                    if (rxn2.getOriginRuleName() == ruleName) {
                        if (!rxn2.getReactants().empty()) {
                            auto specIdx = rxn2.getReactants()[0];
                            auto it = info.perSpeciesRates.find(specIdx);
                            if (it != info.perSpeciesRates.end()) {
                                perRxnDerivedRates[rxnIdx] = it->second;
                            }
                        }
                    }
                    ++rxnIdx;
                }
            }
        }
    }

    std::size_t rxnIndex = 0;
    for (const auto& rxn : network_.reactions.all()) {
        CompiledReaction crxn;
        crxn.reactantIndices = rxn.getReactants();
        crxn.productIndices = rxn.getProducts();
        crxn.statFactor = rxn.getFactor();

        // NOTE: The rule-level TotalRate modifier affects how rate constants
        // are computed at the RULE level (dividing by number of matching sites),
        // but the generated per-reaction rate constant already accounts for this.
        // The ODE solver should ALWAYS multiply rate * [reactants] (mass action).
        // isTotalRate is only set for Sat/MM/Hill rate laws where the rate
        // function already includes the reactant concentration dependence.
        const auto& originRuleName = rxn.getOriginRuleName();

        // Fast path: If NetWriter pre-computed a per-reaction derived rate parameter
        // (local functions with %x:: scope prefix or energy patterns), use it directly.
        // The derived value is the raw rate (e.g. exp(-(Ea0 + phi*DG/RT))), so we
        // still need to apply unit conversion (for bimolecular reactions in
        // compartments) and the statistical factor, matching what Perl writes in
        // the .net reactions section.
        {
            auto drIt = perRxnDerivedRates.find(rxnIndex);
            if (drIt != perRxnDerivedRates.end()) {
                double rate = drIt->second.second;  // numeric value
                const auto unitFactor = bng::io::NetWriter::computeUnitConversionFactor(rxn, model_, network_);
                if (unitFactor.has_value()) {
                    rate *= *unitFactor;
                }
                if (std::abs(rxn.getFactor() - 1.0) >= 1e-9) {
                    rate *= rxn.getFactor();
                }
                crxn.rateConstant = rate;
                crxn.isFunctional = false;
                compiledRxns_.push_back(crxn);
                ++rxnIndex;
                continue;
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

            // Also try alternative naming: {ruleName}Rate_{rxnIndex} (energy pattern format)
            const std::string rxnLabel = rxn.getLabel();
            // Extract reaction index from label (e.g., "R1|..." -> look for "R1Rate_1")
            std::string altDerivedName;
            {
                // Try: ruleBase + "Rate_" + "1" (per-species index, usually 1 for single-species)
                std::string altBase = ruleBase;
                if (altBase.front() == '_') altBase = altBase.substr(1);
                altDerivedName = altBase + "Rate_1";
                // Also try reverse: "R1rRate_1" for reverse rules
                if (isReverse) {
                    altDerivedName = altBase + "rRate_1";
                }
            }

            // Check if any derived parameter exists
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
                // Try alternative name format (energy pattern derived params)
                try {
                    paramResolver(altDerivedName);
                    foundDerived = true;
                    const auto unitFactor2 = bng::io::NetWriter::computeUnitConversionFactor(rxn, model_, network_);
                    // Reset and rebuild with alternative name
                    rateStrBuilder.str("");
                    rateStrBuilder.clear();
                    if (unitFactor2.has_value()) {
                        rateStrBuilder << *unitFactor2 << "*";
                    }
                    if (std::abs(rxn.getFactor() - 1.0) >= 1e-9) {
                        rateStrBuilder << rxn.getFactor() << "*";
                    }
                    rateStrBuilder << altDerivedName;
                } catch (const std::exception&) {
                    // Also try the rate string from .net directly (it may already be the param name)
                    // The .net writes "R1Rate_1" but the Rxn has "Arrhenius(phi,Ea_AB)"
                    // Check all model parameters for one matching the reaction's origin rule
                    for (const auto& param : model_.getParameters().all()) {
                        const auto& pname = param.getName();
                        if (pname.find(ruleBase) != std::string::npos &&
                            (pname.find("Rate") != std::string::npos || pname.find("rate") != std::string::npos)) {
                            try {
                                paramResolver(pname);
                                foundDerived = true;
                                rateStrBuilder.str("");
                                rateStrBuilder.clear();
                                const auto uf = bng::io::NetWriter::computeUnitConversionFactor(rxn, model_, network_);
                                if (uf.has_value()) rateStrBuilder << *uf << "*";
                                if (std::abs(rxn.getFactor() - 1.0) >= 1e-9) rateStrBuilder << rxn.getFactor() << "*";
                                rateStrBuilder << pname;
                                break;
                            } catch (...) {}
                        }
                    }
                    if (!foundDerived) foundDerived = false;
                }
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
        bool isMM = false;  // True when rate law is MM (Michaelis-Menten), not Sat
        {
            std::string rlLower = rawRateLaw;
            std::transform(rlLower.begin(), rlLower.end(), rlLower.begin(), ::tolower);
            // Trim leading whitespace
            auto start = rlLower.find_first_not_of(" \t");
            if (start != std::string::npos) rlLower = rlLower.substr(start);

            if (rlLower.rfind("sat(", 0) == 0 || rlLower.rfind("sat ", 0) == 0 || rlLower.rfind("sat\t", 0) == 0) {
                isSatMMHill = true;
            }
            if (rlLower.rfind("mm(", 0) == 0 || rlLower.rfind("mm ", 0) == 0 || rlLower.rfind("mm\t", 0) == 0) {
                isSatMMHill = true;
                isMM = true;
            }
            if (rlLower.rfind("hill(", 0) == 0 || rlLower.rfind("hill ", 0) == 0 || rlLower.rfind("hill\t", 0) == 0) {
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
                // Sat(kcat, Km): rate = kcat * [S] / (Km + [S]) * [other reactants]
                // MM(kcat, Km):  rate = self-consistent MM with quadratic for free substrate
                //   St=reactant[0], Et=reactant[1], b=St-Et-Km, S=0.5*(b+sqrt(b^2+4*St*Km))
                //   rate = kcat * Et * S / (Km + S)   (total rate, no further mass-action multiply)
                // Hill(Vmax, Kh, n): rate = Vmax * [S]^n / (Kh^n + [S]^n) * [other reactants]
                std::string kcat = paramNames[0];
                std::string Km = paramNames[1];

                ast::Expression baseExpr = ast::Expression::number(0.0); // placeholder

                if (paramNames.size() >= 3) {
                    // Hill(Vmax, Kh, n): substrate is first reactant
                    baseExpr = ast::Expression::function("Hill",
                        {ast::Expression::identifier(kcat), ast::Expression::identifier(Km),
                         ast::Expression::identifier(paramNames[2]),
                         ast::Expression::identifier("__substrate_" + std::to_string(substrateIdx))});
                    // Multiply by non-substrate reactant concentrations
                    for (std::size_t ri = 1; ri < crxn.reactantIndices.size(); ++ri) {
                        baseExpr = ast::Expression::binary("*", std::move(baseExpr),
                            ast::Expression::identifier("__substrate_" + std::to_string(crxn.reactantIndices[ri])));
                    }
                } else if (isMM && crxn.reactantIndices.size() >= 2) {
                    // MM(kcat, Km) with 2 reactants: self-consistent Michaelis-Menten
                    // Use 4-arg MM(kcat, Km, St, Et) which applies the quadratic formula
                    std::size_t enzymeIdx = crxn.reactantIndices[1];
                    baseExpr = ast::Expression::function("MM",
                        {ast::Expression::identifier(kcat), ast::Expression::identifier(Km),
                         ast::Expression::identifier("__substrate_" + std::to_string(substrateIdx)),
                         ast::Expression::identifier("__substrate_" + std::to_string(enzymeIdx))});
                    // No further mass-action multiply -- MM already includes enzyme
                } else {
                    // Sat(kcat, Km) or MM with single reactant: simple saturation
                    baseExpr = ast::Expression::function("Sat",
                        {ast::Expression::identifier(kcat), ast::Expression::identifier(Km),
                         ast::Expression::identifier("__substrate_" + std::to_string(substrateIdx))});
                    // Multiply by non-substrate reactant concentrations (e.g., enzyme [E])
                    for (std::size_t ri = 1; ri < crxn.reactantIndices.size(); ++ri) {
                        baseExpr = ast::Expression::binary("*", std::move(baseExpr),
                            ast::Expression::identifier("__substrate_" + std::to_string(crxn.reactantIndices[ri])));
                    }
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
                // Use the original parsed rate expression, but wrap with unit
                // conversion factor and stat factor so that compartment-volume
                // scaling is applied even for functional rates.
                ast::Expression funcExpr = *rateExpr;

                double combinedFactor = 1.0;
                const auto uf = bng::io::NetWriter::computeUnitConversionFactor(rxn, model_, network_);
                if (uf.has_value()) combinedFactor *= *uf;
                if (std::abs(rxn.getFactor() - 1.0) >= 1e-9) combinedFactor *= rxn.getFactor();
                if (std::abs(combinedFactor - 1.0) >= 1e-9) {
                    funcExpr = ast::Expression::binary("*",
                        ast::Expression::number(combinedFactor), std::move(funcExpr));
                }

                crxn.functionalRateExpr = std::move(funcExpr);
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
                    // Parse the full rate law string into an expression so that
                    // compound expressions like "k * funcName()" are preserved.
                    ast::Expression funcExpr2 = ast::Expression::number(0.0);
                    try {
                        funcExpr2 = parser::parseExpression(rawRL);
                    } catch (...) {
                        // Fallback: use bare identifier if parsing fails
                        funcExpr2 = ast::Expression::identifier(func.getName());
                    }

                    // Apply unit conversion and stat factor
                    double combinedFactor2 = 1.0;
                    const auto uf2 = bng::io::NetWriter::computeUnitConversionFactor(rxn, model_, network_);
                    if (uf2.has_value()) combinedFactor2 *= *uf2;
                    if (std::abs(rxn.getFactor() - 1.0) >= 1e-9) combinedFactor2 *= rxn.getFactor();
                    if (std::abs(combinedFactor2 - 1.0) >= 1e-9) {
                        funcExpr2 = ast::Expression::binary("*",
                            ast::Expression::number(combinedFactor2), std::move(funcExpr2));
                    }

                    crxn.functionalRateExpr = std::move(funcExpr2);
                    crxn.rateConstant = 0.0;
                    hasFunctionalRates_ = true;
                    break;
                }
            }
        }

        // Evaluate the rate string (only for non-functional, non-Sat/MM rates).
        // Use the full ANTLR parser so that complex expressions like
        // "k1*kPlus()" or "v_translat()/vol" are handled correctly.
        if (!crxn.isFunctional) {
            bool evaluated = false;

            // Try the ANTLR expression parser first — it handles function
            // calls, power operators, and all arithmetic correctly.
            auto tryParseAndEval = [&](const std::string& str) -> bool {
                try {
                    auto parsed = parser::parseExpression(str);

                    // Check whether the expression depends on anything that
                    // isn't a plain parameter (observables, user-defined
                    // functions, time, etc.).  If so, it must be evaluated
                    // at every time-step (functional rate).
                    bool needsRuntime = false;

                    auto deps = parsed.getDependencies();
                    for (const auto& dep : deps) {
                        if (dep == "time") {
                            needsRuntime = true;
                            break;
                        }
                        if (!model_.getParameters().contains(dep)) {
                            needsRuntime = true;
                            break;
                        }
                    }

                    // Also check for user-defined function calls (zero-arg
                    // functions like kPlus() appear as Function nodes whose
                    // name is not a built-in).
                    if (!needsRuntime && str.find('(') != std::string::npos) {
                        for (const auto& func : model_.getFunctions()) {
                            if (str.find(func.getName()) != std::string::npos) {
                                needsRuntime = true;
                                break;
                            }
                        }
                    }

                    if (needsRuntime) {
                        crxn.isFunctional = true;
                        crxn.functionalRateExpr = std::move(parsed);
                        crxn.rateConstant = 0.0;
                        hasFunctionalRates_ = true;
                    } else {
                        crxn.rateConstant = parsed.evaluate(paramResolver);
                    }
                    return true;
                } catch (...) {
                    return false;
                }
            };

            // Prefer the full rateStr (includes unit/stat factor prefixes).
            evaluated = tryParseAndEval(rateStr);

            // If that failed, try the raw rate law without prefixes.
            if (!evaluated) {
                evaluated = tryParseAndEval(rawRateLaw);
            }

            // Last resort: the simple recursive descent evaluator for
            // trivial numeric expressions.
            if (!evaluated) {
                try {
                    crxn.rateConstant = evaluateRateString(rateStr, paramResolver);
                } catch (const std::exception&) {
                    crxn.isFunctional = true;
                    crxn.rateConstant = 0.0;
                    hasFunctionalRates_ = true;
                }
            }
        }

        compiledRxns_.push_back(crxn);
        ++rxnIndex;
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

                    const auto pattern = bng::parser::buildPatternGraph(species, mutableModel, false);

                    // Compartment matching strategy:
                    // 1. Molecule-level compartment (e.g., SARM()@Cyt) -- handled
                    //    by Ullmann matcher's Node::operator== compartment check.
                    // 2. Species-level prefix compartment (e.g., @PM:L) -- no
                    //    compartment on pattern nodes; filter at species level.
                    const auto patternCompartment = bng::parser::extractSpeciesCompartment(species);
                    if (!patternCompartment.empty()) {
                        bool patternHasMoleculeCompartment = false;
                        for (auto it = pattern.begin(); it != pattern.end(); ++it) {
                            if (!(*it)->get_compartment().empty()) {
                                patternHasMoleculeCompartment = true;
                                break;
                            }
                        }
                        if (!patternHasMoleculeCompartment) {
                            const auto& speciesCompartment = network_.species.get(speciesIndex).getCompartment();
                            if (speciesCompartment != patternCompartment) {
                                continue;  // Skip -- species-level compartment mismatch
                            }
                        }
                    }

                    // Count matches (with node-level state and structure validation)
                    const auto& targetGraph = network_.species.get(speciesIndex).getSpeciesGraph().getGraph();
                    BNGcore::UllmannSGIso matcher(pattern, targetGraph);
                    BNGcore::List<BNGcore::Map> maps;
                    matcher.find_maps(maps);

                    // Post-filter: verify each map respects node states, compartments,
                    // and structural roles (molecule vs component vs bond nodes).
                    // The Ullmann SGIso matches by type_name which may conflate molecule
                    // types and component types that share the same name (e.g., CDKN1A).
                    std::size_t matchCount = 0;
                    for (auto mapIt = maps.begin(); mapIt != maps.end(); ++mapIt) {
                        bool valid = true;
                        for (auto pnIt = pattern.begin(); pnIt != pattern.end(); ++pnIt) {
                            auto* target = mapIt->mapf(*pnIt);
                            if (!target) { valid = false; break; }
                            // Check state compatibility
                            if (!((*pnIt)->get_state() == target->get_state())) { valid = false; break; }
                            // Check structural role: molecule nodes must map to molecule nodes
                            bool patternIsMol = ((*pnIt)->in_degree() == 0);
                            bool targetIsMol = (target->in_degree() == 0);
                            if (patternIsMol != targetIsMol) { valid = false; break; }
                            // Check per-molecule compartment
                            if (patternIsMol) {
                                const auto& pc = (*pnIt)->get_compartment();
                                if (!pc.empty() && target->get_compartment() != pc) { valid = false; break; }
                            }
                        }
                        if (valid) ++matchCount;
                    }

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

                    // For "Species" observables, each pattern contributes 0 or 1
                    // (presence/absence). The total weight is the number of
                    // patterns that match, NOT clamped to 1 across all patterns.
                    if (observable.getType() == "Species" && matchCount > 0) {
                        matchCount = 1;
                    }
                    weight += matchCount;
                } catch (const std::exception& e) {
                    throw std::runtime_error("Failed to compile observable " + observable.getName() +
                                           " pattern '" + patternText + "': " + e.what());
                }
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
    const double dt = (opts.tEnd - opts.tStart) / static_cast<double>(opts.nSteps);
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
        double t = opts.tStart + step * dt;
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
    const double outputDt = (opts.tEnd - opts.tStart) / static_cast<double>(opts.nSteps);
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

    double t = opts.tStart;
    bool stopEarly = false;

    for (std::size_t outputStep = 0; outputStep <= opts.nSteps; ++outputStep) {
        double targetT = opts.tStart + outputStep * outputDt;

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

void OdeIntegrator::writeOutputFiles(const std::string& prefix, const OdeResult& result, bool printCDAT, bool printFunctions, bool append) const {
    // When appending (continue=1), skip the first output row since it
    // duplicates the last row of the previous phase — matches Perl/run_network
    // behavior with the -c flag.
    const std::size_t startStep = append ? 1 : 0;

    // Write .cdat (concentrations) - only if printCDAT is true
    if (printCDAT) {
        std::ofstream cdat(prefix + ".cdat", append ? std::ios::app : std::ios::trunc);
        if (!cdat) {
            throw std::runtime_error("Failed to open " + prefix + ".cdat for writing");
        }

        for (std::size_t step = startStep; step < result.timePoints.size(); ++step) {
            cdat << std::setw(18) << std::setprecision(12) << std::scientific
                 << result.timePoints[step];
            for (const auto& c : result.concentrations[step]) {
                cdat << " " << std::setw(18) << c;
            }
            cdat << "\n";
        }
    }

    // Write .gdat (observables)
    std::ofstream gdat(prefix + ".gdat", append ? std::ios::app : std::ios::trunc);
    if (!gdat) {
        throw std::runtime_error("Failed to open " + prefix + ".gdat for writing");
    }

    // Collect function names and expressions for print_functions output
    std::vector<std::string> funcNames;
    std::vector<ast::Expression> funcExprs;
    if (printFunctions) {
        for (const auto& fn : model_.getFunctions()) {
            funcNames.push_back(fn.getName());
            funcExprs.push_back(fn.getExpression());
        }
    }

    // Header line - only write if not appending
    if (!append) {
        gdat << "#";
        gdat << std::setw(17) << "time";
        for (const auto& group : compiledGroups_) {
            gdat << " " << std::setw(18) << group.name;
        }
        for (const auto& fname : funcNames) {
            gdat << " " << std::setw(18) << fname;
        }
        gdat << "\n";
    }

    for (std::size_t step = startStep; step < result.timePoints.size(); ++step) {
        gdat << std::setw(18) << std::setprecision(12) << std::scientific
             << result.timePoints[step];
        for (const auto& obs : result.observables[step]) {
            gdat << " " << std::setw(18) << obs;
        }
        // Evaluate and print function values
        if (printFunctions && !funcExprs.empty()) {
            // Build observable value map for this time step
            std::unordered_map<std::string, double> obsMap;
            for (std::size_t gi = 0; gi < compiledGroups_.size() && gi < result.observables[step].size(); ++gi) {
                obsMap[compiledGroups_[gi].name] = result.observables[step][gi];
            }
            // Also include parameters
            auto resolver = [&](const std::string& name) -> double {
                auto obsIt = obsMap.find(name);
                if (obsIt != obsMap.end()) return obsIt->second;
                return model_.getParameters().evaluate(name);
            };
            for (auto& fexpr : funcExprs) {
                double val = fexpr.evaluate(resolver);
                gdat << " " << std::setw(18) << val;
            }
        }
        gdat << "\n";
    }
}

// Static C-style callback for CVODE (v7: sunrealtype instead of realtype)
static int cvodeCallbackWrapper(sunrealtype t, N_Vector y, N_Vector ydot, void* user_data) {
    auto* self = static_cast<OdeIntegrator*>(user_data);
    self->derivs(static_cast<double>(t), NV_DATA_S(y), NV_DATA_S(ydot));
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

    // SUNDIALS v7: Create context (required for all SUNDIALS objects)
    SUNContext sunctx = nullptr;
    int flag = SUNContext_Create(SUN_COMM_NULL, &sunctx);
    if (flag != 0) {
        throw std::runtime_error("SUNContext_Create failed");
    }

    // Initialize state vector (v7: takes sunctx)
    N_Vector y = N_VNew_Serial(static_cast<sunindextype>(nSpecies_), sunctx);
    if (y == nullptr) {
        SUNContext_Free(&sunctx);
        throw std::runtime_error("Failed to allocate CVODE state vector");
    }

    for (std::size_t i = 0; i < nSpecies_; ++i) {
        NV_Ith_S(y, i) = network_.species.get(i).getAmount();
    }

    // Create CVODE solver (v7: BDF with context, Newton is default)
    void* cvode_mem = CVodeCreate(CV_BDF, sunctx);
    if (cvode_mem == nullptr) {
        N_VDestroy(y);
        SUNContext_Free(&sunctx);
        throw std::runtime_error("Failed to create CVODE solver");
    }

    // Initialize CVODE at the requested start time (critical for continue=1 phases)
    flag = CVodeInit(cvode_mem, cvodeCallbackWrapper, opts.tStart, y);
    if (flag != CV_SUCCESS) {
        CVodeFree(&cvode_mem);
        N_VDestroy(y);
        SUNContext_Free(&sunctx);
        throw std::runtime_error("CVodeInit failed with flag " + std::to_string(flag));
    }

    // Set tolerances (matching BNG2 defaults: rtol=1e-8, atol=1e-8)
    flag = CVodeSStolerances(cvode_mem, opts.rtol, opts.atol);
    if (flag != CV_SUCCESS) {
        CVodeFree(&cvode_mem);
        N_VDestroy(y);
        SUNContext_Free(&sunctx);
        throw std::runtime_error("CVodeSStolerances failed");
    }

    // Set user data
    CVodeSetUserData(cvode_mem, this);

    // Set max number of steps (BNG2 default: 2000, auto-increases if needed)
    CVodeSetMaxNumSteps(cvode_mem, 2000);

    // Note: Perl's run_network does NOT enable stability limit detection.
    // Disabled for exact parity with Perl CVODE stepping behavior.
    // CVodeSetStabLimDet(cvode_mem, 1);

    // Linear solver setup (v7 API: SUNMatrix + SUNLinearSolver + CVodeSetLinearSolver)
    SUNMatrix A = nullptr;
    SUNLinearSolver LS = nullptr;

    // Choose solver based on network size (matches BNG2 behavior)
    // Dense for small networks (<200 species), GMRES for large networks
    if (nSpecies_ < 200) {
        // Dense direct solver
        A = SUNDenseMatrix(static_cast<sunindextype>(nSpecies_),
                           static_cast<sunindextype>(nSpecies_), sunctx);
        if (A == nullptr) {
            CVodeFree(&cvode_mem);
            N_VDestroy(y);
            SUNContext_Free(&sunctx);
            throw std::runtime_error("SUNDenseMatrix failed");
        }
        LS = SUNLinSol_Dense(y, A, sunctx);
        if (LS == nullptr) {
            SUNMatDestroy(A);
            CVodeFree(&cvode_mem);
            N_VDestroy(y);
            SUNContext_Free(&sunctx);
            throw std::runtime_error("SUNLinSol_Dense failed");
        }
        flag = CVodeSetLinearSolver(cvode_mem, LS, A);
        if (flag != CV_SUCCESS) {
            SUNLinSolFree(LS);
            SUNMatDestroy(A);
            CVodeFree(&cvode_mem);
            N_VDestroy(y);
            SUNContext_Free(&sunctx);
            throw std::runtime_error("CVodeSetLinearSolver (dense) failed");
        }
    } else {
        // GMRES iterative solver (better for large sparse systems)
        LS = SUNLinSol_SPGMR(y, SUN_PREC_NONE, 0, sunctx);
        if (LS == nullptr) {
            CVodeFree(&cvode_mem);
            N_VDestroy(y);
            SUNContext_Free(&sunctx);
            throw std::runtime_error("SUNLinSol_SPGMR failed");
        }
        flag = CVodeSetLinearSolver(cvode_mem, LS, nullptr);
        if (flag != CV_SUCCESS) {
            SUNLinSolFree(LS);
            CVodeFree(&cvode_mem);
            N_VDestroy(y);
            SUNContext_Free(&sunctx);
            throw std::runtime_error("CVodeSetLinearSolver (GMRES) failed");
        }
    }

    // Integration loop
    OdeResult result;
    result.timePoints.reserve(opts.nSteps + 1);
    result.concentrations.reserve(opts.nSteps + 1);

    const double dt = (opts.tEnd - opts.tStart) / static_cast<double>(opts.nSteps);
    double t = opts.tStart;

    for (std::size_t step = 0; step <= opts.nSteps; ++step) {
        double tOut = opts.tStart + step * dt;

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
                    SUNLinSolFree(LS);
                    if (A) SUNMatDestroy(A);
                    CVodeFree(&cvode_mem);
                    N_VDestroy(y);
                    SUNContext_Free(&sunctx);
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

    // Cleanup (v7: also free linear solver, matrix, and context)
    SUNLinSolFree(LS);
    if (A) SUNMatDestroy(A);
    CVodeFree(&cvode_mem);
    N_VDestroy(y);
    SUNContext_Free(&sunctx);

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

    double t = opts.tStart;
    const double dt = (opts.tEnd - opts.tStart) / static_cast<double>(opts.nSteps);
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
            while (opts.tStart + nextOutputStep * dt <= t && nextOutputStep <= opts.nSteps) {
                result.timePoints.push_back(opts.tStart + nextOutputStep * dt);
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
            result.timePoints.push_back(opts.tStart + nextOutputStep * dt);
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
