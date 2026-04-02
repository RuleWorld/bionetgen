#include "io/NetWriter.hpp"

#include <algorithm>
#include <cmath>
#include <fstream>
#include <iomanip>
#include <map>
#include <optional>
#include <regex>
#include <stdexcept>
#include <string>
#include <unordered_map>
#include <unordered_set>

#include <antlr4-runtime.h>

#include "core/List.hpp"
#include "core/Ullmann.hpp"
#include "generated/BNGLexer.h"
#include "generated/BNGParser.h"
#include "parser/PatternGraphBuilder.hpp"

namespace bng::io {

namespace {

// BNG built-in rate law functions — written directly in reactions, not as _rateLaw params
const std::unordered_set<std::string> builtinRateLawFunctions = {
    "Sat", "sat", "MM", "mm", "Hill", "hill", "Arrhenius", "arrhenius",
    "Hybrid", "hybrid"
};

// Canonical (Perl-compatible) casing for built-in rate law functions
const std::unordered_map<std::string, std::string> builtinCanonicalName = {
    {"sat", "Sat"}, {"Sat", "Sat"},
    {"mm", "MM"}, {"MM", "MM"},
    {"hill", "Hill"}, {"Hill", "Hill"},
    {"arrhenius", "Arrhenius"}, {"Arrhenius", "Arrhenius"},
    {"hybrid", "Hybrid"}, {"Hybrid", "Hybrid"},
};

// Simple recursive evaluator for mathematical expressions
// Handles: numbers, parameters, +, -, *, /, (), exp(), and nested expressions
double evaluateExpressionString(const std::string& expr,
                                const std::function<double(const std::string&)>& resolve);

double evaluateExpressionString(const std::string& expr,
                                const std::function<double(const std::string&)>& resolve) {
    std::string trimmed = expr;
    // Trim whitespace
    trimmed.erase(0, trimmed.find_first_not_of(" \t"));
    trimmed.erase(trimmed.find_last_not_of(" \t") + 1);

    // Handle exp(...)
    if (trimmed.rfind("exp(", 0) == 0) {
        // Find matching close paren
        int depth = 0;
        size_t start = 4;  // After "exp("
        size_t end = start;
        for (size_t i = start - 1; i < trimmed.size(); ++i) {
            if (trimmed[i] == '(') ++depth;
            else if (trimmed[i] == ')') {
                --depth;
                if (depth == 0) {
                    end = i;
                    break;
                }
            }
        }
        if (end > start) {
            std::string inner = trimmed.substr(start, end - start);
            return std::exp(evaluateExpressionString(inner, resolve));
        }
    }

    // Handle parentheses - only strip if they're balanced and outermost
    if (trimmed.front() == '(' && trimmed.back() == ')') {
        // Check if these parens match (not just any parens)
        int depth = 0;
        bool outermost = true;
        for (size_t i = 0; i < trimmed.size() - 1; ++i) {
            if (trimmed[i] == '(') ++depth;
            else if (trimmed[i] == ')') {
                --depth;
                if (depth == 0 && i < trimmed.size() - 1) {
                    // Closing paren before end - not outermost
                    outermost = false;
                    break;
                }
            }
        }
        if (outermost) {
            std::string inner = trimmed.substr(1, trimmed.size() - 2);
            return evaluateExpressionString(inner, resolve);
        }
    }

    // Try as number (must consume entire string)
    try {
        std::size_t pos = 0;
        double val = std::stod(trimmed, &pos);
        if (pos == trimmed.size()) return val;
    } catch (...) {}

    // Try as parameter name (no operators)
    if (trimmed.find_first_of("+-*/()^") == std::string::npos) {
        return resolve(trimmed);
    }

    // Check for unary minus at start
    if (trimmed.front() == '-') {
        // Count to see if there are any binary operators at depth 0 after position 0
        int parenDepth = 0;
        bool hasBinaryOpAtZero = false;
        for (size_t i = 1; i < trimmed.size(); ++i) {
            if (trimmed[i] == '(') ++parenDepth;
            else if (trimmed[i] == ')') --parenDepth;
            else if (parenDepth == 0 && (trimmed[i] == '+' || trimmed[i] == '-' || trimmed[i] == '*' || trimmed[i] == '/' || trimmed[i] == '^')) {
                hasBinaryOpAtZero = true;
                break;
            }
        }
        if (!hasBinaryOpAtZero) {
            // It's unary minus
            std::string inner = trimmed.substr(1);
            return -evaluateExpressionString(inner, resolve);
        }
    }

    // Find operators at lowest precedence level (outside parentheses)
    // Process +/- (lowest), then */, then ^ (highest), with left-to-right associativity
    int parenDepth = 0;
    int lastAddSub = -1;
    char lastAddSubOp = 0;
    int lastMulDiv = -1;
    char lastMulDivOp = 0;
    int lastPower = -1;

    for (size_t i = 0; i < trimmed.size(); ++i) {
        if (trimmed[i] == '(') ++parenDepth;
        else if (trimmed[i] == ')') --parenDepth;
        else if (parenDepth == 0 && i > 0) {
            if (trimmed[i] == '+' || trimmed[i] == '-') {
                lastAddSub = i;
                lastAddSubOp = trimmed[i];
            }
            if (trimmed[i] == '*' || trimmed[i] == '/') {
                lastMulDiv = i;
                lastMulDivOp = trimmed[i];
            }
            if (trimmed[i] == '^') {
                lastPower = i;
            }
        }
    }

    // Process +/- first (lowest precedence)
    if (lastAddSub >= 0) {
        std::string left = trimmed.substr(0, lastAddSub);
        std::string right = trimmed.substr(lastAddSub + 1);
        double leftVal = evaluateExpressionString(left, resolve);
        double rightVal = evaluateExpressionString(right, resolve);
        return lastAddSubOp == '+' ? leftVal + rightVal : leftVal - rightVal;
    }

    // Then */ (higher precedence)
    if (lastMulDiv >= 0) {
        std::string left = trimmed.substr(0, lastMulDiv);
        std::string right = trimmed.substr(lastMulDiv + 1);
        double leftVal = evaluateExpressionString(left, resolve);
        double rightVal = evaluateExpressionString(right, resolve);
        if (lastMulDivOp == '*') {
            return leftVal * rightVal;
        } else {
            if (std::abs(rightVal) < 1e-300) throw std::runtime_error("Division by zero");
            return leftVal / rightVal;
        }
    }

    // Then ^ (power, highest binary precedence)
    if (lastPower >= 0) {
        std::string left = trimmed.substr(0, lastPower);
        std::string right = trimmed.substr(lastPower + 1);
        double leftVal = evaluateExpressionString(left, resolve);
        double rightVal = evaluateExpressionString(right, resolve);
        return std::pow(leftVal, rightVal);
    }

    throw std::runtime_error("Cannot evaluate expression: " + trimmed);
}

BNGcore::PatternGraph parseObservablePattern(const std::string& patternText, ast::Model& model) {
    antlr4::ANTLRInputStream input(patternText);
    BNGLexer lexer(&input);
    antlr4::CommonTokenStream tokens(&lexer);
    BNGParser parser(&tokens);
    auto* species = parser.species_def();
    if (parser.getNumberOfSyntaxErrors() != 0) {
        throw std::runtime_error("Could not parse observable pattern: " + patternText);
    }
    return bng::parser::buildPatternGraph(species, model);
}

std::string formatSpecies(const ast::Species& species) {
    std::string result;
    const auto& speciesCompartment = species.getCompartment();
    if (!speciesCompartment.empty()) {
        result += "@" + speciesCompartment + "::";
    }
    if (species.isConstant()) {
        result += "$";
    }

    // Get species string and per-molecule compartments in output order
    std::vector<std::string> molCompartments;
    std::string speciesStr = species.getSpeciesGraph().getGraph().get_BNG2_string(molCompartments);

    // Add per-molecule compartment suffixes for molecules whose compartment
    // differs from the species-level compartment (Perl convention:
    // @COMP suffix only when molecule compartment != species compartment)
    if (!speciesCompartment.empty() && !molCompartments.empty()) {
        std::string annotated;
        std::size_t molIdx = 0;
        std::size_t pos = 0;
        while (pos < speciesStr.size()) {
            // Find the end of this molecule (next '.' at top level, outside parens)
            int parenDepth = 0;
            std::size_t molEnd = pos;
            while (molEnd < speciesStr.size()) {
                if (speciesStr[molEnd] == '(') parenDepth++;
                else if (speciesStr[molEnd] == ')') parenDepth--;
                else if (speciesStr[molEnd] == '.' && parenDepth == 0) break;
                molEnd++;
            }

            annotated += speciesStr.substr(pos, molEnd - pos);

            // Add compartment suffix if molecule is in a different compartment
            if (molIdx < molCompartments.size() &&
                !molCompartments[molIdx].empty() &&
                molCompartments[molIdx] != speciesCompartment) {
                annotated += "@" + molCompartments[molIdx];
            }

            if (molEnd < speciesStr.size() && speciesStr[molEnd] == '.') {
                annotated += '.';
                molEnd++;
            }

            pos = molEnd;
            molIdx++;
        }
        speciesStr = annotated;
    }

    result += speciesStr;
    return result;
}

std::string formatFactor(double factor) {
    if (std::abs(factor - std::round(factor)) < 1e-9) {
        return std::to_string(static_cast<long long>(std::llround(factor)));
    }
    std::ostringstream out;
    out << std::setprecision(12) << factor;
    return out.str();
}

std::string compactExpression(std::string text) {
    // Remove all whitespace
    text.erase(std::remove_if(text.begin(), text.end(), [](unsigned char c) {
        return std::isspace(c) != 0;
    }), text.end());
    // Strip redundant outer parentheses: "(expr)" → "expr" if the parens are matched
    while (text.size() >= 2 && text.front() == '(' && text.back() == ')') {
        int depth = 0;
        bool outermost = true;
        for (std::size_t i = 0; i < text.size() - 1; ++i) {
            if (text[i] == '(') ++depth;
            else if (text[i] == ')') {
                --depth;
                if (depth == 0 && i < text.size() - 1) {
                    outermost = false;
                    break;
                }
            }
        }
        if (outermost) {
            text = text.substr(1, text.size() - 2);
        } else {
            break;
        }
    }
    return text;
}

std::string formatInitialAmountName(std::size_t index) {
    return "_InitialConc" + std::to_string(index + 1);
}

std::string formatSpeciesAmount(const ast::Species& species, const ast::Model& model) {
    const auto speciesLabel = species.getSpeciesGraph().canonicalLabel();
    for (std::size_t i = 0; i < model.getSeedSpecies().size(); ++i) {
        const auto& seed = model.getSeedSpecies()[i];
        if (seed.getCanonicalLabel() == speciesLabel && std::abs(species.getAmount()) > 1e-12) {
            // Use the original seed species amount expression (parameter name or value)
            return seed.getAmount().toString();
        }
    }

    std::ostringstream out;
    out << std::setprecision(12) << species.getAmount();
    return out.str();
}

std::size_t countPatternMatches(const ast::SpeciesGraph& pattern, const ast::Species& species) {
    BNGcore::UllmannSGIso matcher(pattern.getGraph(), species.getSpeciesGraph().getGraph());
    BNGcore::List<BNGcore::Map> maps;
    return matcher.find_maps(maps);
}

std::string parenthesize(const std::string& text) {
    return "(" + text + ")";
}

std::string energyDeltaExpression(
    const ast::Rxn& reaction,
    const ast::Model& model,
    const engine::GeneratedNetwork& network) {
    std::vector<std::string> terms;
    for (const auto& energyPattern : model.getEnergyPatterns()) {
        long long delta = 0;
        for (const auto reactantIndex : reaction.getReactants()) {
            delta -= static_cast<long long>(countPatternMatches(energyPattern.getGraph(), network.species.get(reactantIndex)));
        }
        for (const auto productIndex : reaction.getProducts()) {
            delta += static_cast<long long>(countPatternMatches(energyPattern.getGraph(), network.species.get(productIndex)));
        }
        if (delta == 0) {
            continue;
        }

        const auto expr = compactExpression(energyPattern.getExpression().toString());
        const auto addTerm = [&](const std::string& term) {
            terms.push_back(term);
        };

        for (long long i = 0; i < std::llabs(delta); ++i) {
            if (delta > 0) {
                addTerm(parenthesize(expr));
            } else {
                addTerm("(-" + parenthesize(expr) + ")");
            }
        }
    }

    if (terms.empty()) {
        return {};
    }

    std::string joined;
    for (std::size_t i = 0; i < terms.size(); ++i) {
        if (i != 0) {
            joined += "+";
        }
        joined += terms[i];
    }
    return joined;
}

struct ArrheniusInfo {
    std::string phiArg;
    std::string eaArg;
};

std::optional<ArrheniusInfo> parseArrhenius(const std::string& rateLaw) {
    static const std::regex pattern(R"(^\s*arrhenius\s*\(\s*([^,]+)\s*,\s*([^)]+)\s*\)\s*$)", std::regex::icase);
    std::smatch match;
    if (!std::regex_match(rateLaw, match, pattern)) {
        return std::nullopt;
    }
    return ArrheniusInfo {compactExpression(match[1].str()), compactExpression(match[2].str())};
}

std::optional<std::string> unitConversionExpression(const ast::Rxn& reaction, const ast::Model& model, const engine::GeneratedNetwork& network) {
    if (reaction.getReactants().size() <= 1) {
        return std::nullopt;
    }

    const auto npqFound = model.getOptions().find("NumberPerQuantityUnit");
    if (npqFound == model.getOptions().end()) {
        return std::nullopt;
    }

    std::string compartmentName;
    for (const auto reactantIndex : reaction.getReactants()) {
        const auto& compartment = network.species.get(reactantIndex).getCompartment();
        if (!compartment.empty()) {
            compartmentName = compartment;
            break;
        }
    }
    if (compartmentName.empty()) {
        return std::nullopt;
    }

    for (const auto& compartment : model.getCompartments()) {
        if (compartment.getName() == compartmentName) {
            std::string sizeText;
            for (const auto& parameter : model.getParameters().all()) {
                if (std::abs(parameter.getValue() - compartment.getVolume()) < 1e-18) {
                    sizeText = parameter.getName();
                    break;
                }
            }
            if (sizeText.empty()) {
                std::ostringstream out;
                out << std::setprecision(12) << compartment.getVolume();
                sizeText = compactExpression(out.str());
            }
            return "1/(" + npqFound->second + "*" + sizeText + ")";
        }
    }
    return std::nullopt;
}

std::optional<double> unitConversionFactor(const ast::Rxn& reaction, const ast::Model& model, const engine::GeneratedNetwork& network) {
    if (reaction.getReactants().size() <= 1) {
        return std::nullopt;
    }

    const auto npqFound = model.getOptions().find("NumberPerQuantityUnit");
    if (npqFound == model.getOptions().end()) {
        return std::nullopt;
    }

    const double npq = std::stod(npqFound->second);
    std::string compartmentName;
    for (const auto reactantIndex : reaction.getReactants()) {
        const auto& compartment = network.species.get(reactantIndex).getCompartment();
        if (!compartment.empty()) {
            compartmentName = compartment;
            break;
        }
    }
    if (compartmentName.empty()) {
        return std::nullopt;
    }

    for (const auto& compartment : model.getCompartments()) {
        if (compartment.getName() == compartmentName) {
            return 1.0 / (npq * compartment.getVolume());
        }
    }
    return std::nullopt;
}

std::string formatDoubleScientific(double value) {
    std::ostringstream out;
    out << std::setprecision(8) << std::scientific << value;
    auto text = out.str();
    std::replace(text.begin(), text.end(), 'E', 'e');
    return text;
}

} // namespace

std::optional<double> NetWriter::computeUnitConversionFactor(
    const ast::Rxn& reaction,
    const ast::Model& model,
    const engine::GeneratedNetwork& network) {
    return unitConversionFactor(reaction, model, network);
}

std::unordered_map<std::string, DerivedRateInfo> NetWriter::buildDerivedRateParams(
    const ast::Model& model,
    const engine::GeneratedNetwork& network) {
    std::unordered_map<std::string, DerivedRateInfo> derived;
    int rateLawCounter = 1;

    // Helper: Check if expression is simple (just a parameter/constant name)
    auto isSimpleExpression = [&](const std::string& expr) -> bool {
        std::string trimmed = expr;
        trimmed.erase(0, trimmed.find_first_not_of(" \t"));
        trimmed.erase(trimmed.find_last_not_of(" \t") + 1);

        // Simple if it's just alphanumeric + underscore (no operators or parens)
        return std::all_of(trimmed.begin(), trimmed.end(),
            [](char c) { return std::isalnum(c) || c == '_' || c == '.'; });
    };

    // Helper: Check if expression references observables
    auto referencesObservables = [&](const std::string& expr) -> bool {
        for (const auto& obs : model.getObservables()) {
            if (expr.find(obs.getName()) != std::string::npos) {
                return true;
            }
        }
        return false;
    };

    // Process rules in BNGL order to match Perl output
    for (const auto& rule : model.getReactionRules()) {
        const std::string ruleName = rule.getRuleName();

        // Skip if already processed
        if (derived.find(ruleName) != derived.end()) {
            continue;
        }

        // Get forward rate expression
        if (rule.getRates().empty()) {
            continue;
        }

        const auto& rateExprObj = rule.getRates()[0];
        const std::string rateExpr = rateExprObj.toString();

        // Skip built-in BNG rate law functions (Sat, MM, Hill, etc.)
        // These are written directly in the reaction rate field
        if (rateExprObj.kind() == ast::ExpressionKind::Function &&
            builtinRateLawFunctions.count(rateExprObj.name()) > 0) {
            continue;
        }

        // Handle model-defined functions (kPlus, kMinus, func, etc.)
        // Parser may store them as Function or ObservableRef kind
        if (rateExprObj.kind() == ast::ExpressionKind::Function ||
            rateExprObj.kind() == ast::ExpressionKind::ObservableRef) {
            bool isModelFunction = false;
            bool hasLocalArgs = !rateExprObj.args().empty();
            const auto& exprName = rateExprObj.name();
            const ast::Function* matchedFunc = nullptr;
            for (const auto& func : model.getFunctions()) {
                if (func.getName() == exprName) {
                    isModelFunction = true;
                    matchedFunc = &func;
                    break;
                }
            }
            if (isModelFunction && hasLocalArgs && (rule.hasScopePrefix() || !model.getEnergyPatterns().empty())) {
                // Per-species numeric evaluation for:
                // 1. Scope prefix (%x::) models like localfunc
                // 2. Models with energy patterns (like isingspin_localfcn) where
                //    local functions reference per-species observables
                // Perl creates per-species derived parameters: rateLaw{N}_{M} or Rule1_local{M}

                // Extract rule index (1-based)
                int ruleIndex = 0;
                for (const auto& r : model.getReactionRules()) {
                    ++ruleIndex;
                    if (r.getRuleName() == ruleName) break;
                }
                std::string baseParamName = rule.hasScopePrefix()
                    ? "rateLaw" + std::to_string(ruleIndex)
                    : ruleName + "_local";

                // Get function body and formal args
                const auto& formalArgs = matchedFunc->getArgs();
                const auto& funcExpr = matchedFunc->getExpression();

                // Find ALL observables referenced in function body
                std::vector<std::string> localObsNames;
                for (const auto& obs : model.getObservables()) {
                    if (funcExpr.toString().find(obs.getName()) != std::string::npos) {
                        localObsNames.push_back(obs.getName());
                    }
                }

                // Compute per-species observable values and create rate params
                DerivedRateInfo info;
                info.paramName = baseParamName;
                info.isLocalFunction = true;

                int seqNum = 1;
                // Collect reactions from this rule and their reactant species
                for (const auto& rxn : network.reactions.all()) {
                    if (rxn.getOriginRuleName() != ruleName) continue;

                    // Get the first reactant species (the scoped species)
                    if (rxn.getReactants().empty()) continue;
                    std::size_t speciesIdx = rxn.getReactants()[0];

                    // Skip if already computed for this species
                    if (info.perSpeciesRates.find(speciesIdx) != info.perSpeciesRates.end()) continue;

                    // Build expression with ALL observable values substituted
                    std::string funcBody = funcExpr.toString();
                    for (const auto& localObsName : localObsNames) {
                        // Compute observable count for this specific species
                        std::size_t obsCount = 0;
                        for (const auto& obs : model.getObservables()) {
                            if (obs.getName() != localObsName) continue;
                            for (const auto& patternText : obs.getPatterns()) {
                                const auto& speciesGraph = network.species.get(speciesIdx).getSpeciesGraph().getGraph();
                                if (speciesGraph.empty()) continue;
                                auto pattern = parseObservablePattern(patternText, const_cast<ast::Model&>(model));
                                BNGcore::UllmannSGIso matcher(pattern, speciesGraph);
                                BNGcore::List<BNGcore::Map> maps;
                                obsCount += matcher.find_maps(maps);
                            }
                            break;
                        }
                        // Substitute this observable's value in the function body
                        for (const auto& farg : formalArgs) {
                            std::string pat = localObsName + "(" + farg + ")";
                            std::string::size_type pos = 0;
                            while ((pos = funcBody.find(pat, pos)) != std::string::npos) {
                                funcBody.replace(pos, pat.length(), std::to_string(obsCount));
                                pos += std::to_string(obsCount).length();
                            }
                        }
                    }

                    // Evaluate the expression
                    double rateValue = 0.0;
                    try {
                        rateValue = evaluateExpressionString(funcBody,
                            [&](const std::string& name) -> double {
                                if (model.getParameters().contains(name))
                                    return model.getParameters().get(name).getValue();
                                return 0.0;
                            });
                    } catch (...) {}

                    std::string paramName = baseParamName + std::to_string(seqNum);
                    info.perSpeciesRates[speciesIdx] = {paramName, rateValue};
                    ++seqNum;
                }

                // Use the first rate as the default expression
                info.expression = "0";
                info.exprTree = ast::Expression::number(0);

                derived.emplace(ruleName, std::move(info));
                continue;
            }
            // Also process reverse rule if bidirectional with local function rates
            if (rule.getRates().size() > 1) {
                const std::string reverseRuleName = "_reverse__" + ruleName;
                if (derived.find(reverseRuleName) == derived.end()) {
                    const auto& reverseRateExprObj = rule.getRates()[1];
                    const std::string reverseRateExpr = reverseRateExprObj.toString();
                    const ast::Function* reverseMatchedFunc = nullptr;
                    for (const auto& func : model.getFunctions()) {
                        if (reverseRateExpr.find(func.getName()) != std::string::npos) {
                            reverseMatchedFunc = &func;
                            break;
                        }
                    }
                    if (reverseMatchedFunc != nullptr) {
                        const auto& revFormalArgs = reverseMatchedFunc->getArgs();
                        const auto& revFuncExpr = reverseMatchedFunc->getExpression();
                        std::vector<std::string> revObsNames;
                        for (const auto& obs : model.getObservables()) {
                            if (revFuncExpr.toString().find(obs.getName()) != std::string::npos) {
                                revObsNames.push_back(obs.getName());
                            }
                        }
                        DerivedRateInfo revInfo;
                        std::string revBaseParam = reverseRuleName.substr(std::string("_reverse__").size()) + "r_local";
                        revInfo.paramName = revBaseParam;
                        revInfo.isLocalFunction = true;
                        int revSeqNum = 1;
                        for (const auto& rxn : network.reactions.all()) {
                            if (rxn.getOriginRuleName() != reverseRuleName) continue;
                            if (rxn.getReactants().empty()) continue;
                            std::size_t speciesIdx = rxn.getReactants()[0];
                            if (revInfo.perSpeciesRates.find(speciesIdx) != revInfo.perSpeciesRates.end()) continue;
                            std::string funcBody = revFuncExpr.toString();
                            for (const auto& obsName : revObsNames) {
                                std::size_t obsCount = 0;
                                for (const auto& obs : model.getObservables()) {
                                    if (obs.getName() != obsName) continue;
                                    for (const auto& patternText : obs.getPatterns()) {
                                        const auto& speciesGraph = network.species.get(speciesIdx).getSpeciesGraph().getGraph();
                                        if (speciesGraph.empty()) continue;
                                        auto pattern = parseObservablePattern(patternText, const_cast<ast::Model&>(model));
                                        BNGcore::UllmannSGIso matcher(pattern, speciesGraph);
                                        BNGcore::List<BNGcore::Map> maps;
                                        obsCount += matcher.find_maps(maps);
                                    }
                                    break;
                                }
                                for (const auto& farg : revFormalArgs) {
                                    std::string pat = obsName + "(" + farg + ")";
                                    std::string::size_type pos = 0;
                                    while ((pos = funcBody.find(pat, pos)) != std::string::npos) {
                                        funcBody.replace(pos, pat.length(), std::to_string(obsCount));
                                        pos += std::to_string(obsCount).length();
                                    }
                                }
                            }
                            double rateValue = 0.0;
                            try {
                                rateValue = evaluateExpressionString(funcBody,
                                    [&](const std::string& name) -> double {
                                        if (model.getParameters().contains(name))
                                            return model.getParameters().get(name).getValue();
                                        return 0.0;
                                    });
                            } catch (...) {}
                            std::string paramName = revBaseParam + std::to_string(revSeqNum);
                            revInfo.perSpeciesRates[speciesIdx] = {paramName, rateValue};
                            ++revSeqNum;
                        }
                        revInfo.expression = "0";
                        revInfo.exprTree = ast::Expression::number(0);
                        derived.emplace(reverseRuleName, std::move(revInfo));
                    }
                }
            }
            // Local function with %x molecule tag but no scope prefix/energy patterns:
            // write symbolic expression (Perl convention __R{N}_local{M})
            if (isModelFunction && hasLocalArgs) {
                int ruleIndex = 0;
                for (const auto& r : model.getReactionRules()) {
                    ++ruleIndex;
                    if (r.getRuleName() == ruleName) break;
                }
                std::string localParamName = "__R" + std::to_string(ruleIndex) + "_local1";
                std::string funcBody;
                if (matchedFunc != nullptr) {
                    funcBody = matchedFunc->getExpression().toString();
                    const auto& fArgs = matchedFunc->getArgs();
                    for (const auto& obs : model.getObservables()) {
                        for (const auto& farg : fArgs) {
                            std::string pat = obs.getName() + "(" + farg + ")";
                            std::string::size_type pos = 0;
                            while ((pos = funcBody.find(pat, pos)) != std::string::npos) {
                                funcBody.replace(pos, pat.length(), "0");
                            }
                        }
                    }
                } else {
                    funcBody = "0";
                }
                derived.emplace(ruleName, DerivedRateInfo {
                    localParamName, funcBody, ast::Expression::number(0), false, false
                });
                continue;
            }
            if (isModelFunction) {
                // No-arg model function — write directly as function name
                continue;
            }
        }

        // Skip if rate expression is simple (just a parameter name)
        if (isSimpleExpression(rateExpr)) {
            // Check if it's a known parameter or function
            bool isKnownParam = false;
            for (const auto& param : model.getParameters().all()) {
                if (param.getName() == rateExpr) {
                    isKnownParam = true;
                    break;
                }
            }
            if (isKnownParam) {
                continue;  // Use parameter directly
            }
            // TODO: Check if it's a function name
            // For now, assume simple names are parameters
            continue;
        }

        // Complex expression - create rateLaw entry
        // Perl convention: _rateLaw (underscore) for functions referencing observables,
        // rateLaw (no underscore) for pure constant expressions.
        bool asFunction = referencesObservables(rateExpr);
        std::string paramName = (asFunction ? "_rateLaw" : "rateLaw") + std::to_string(rateLawCounter++);

        derived.emplace(
            ruleName,
            DerivedRateInfo {
                paramName,
                rateExpr,
                rateExprObj,  // Store actual expression for evaluation
                false,  // reverseDirection
                asFunction
            });

        // Handle bidirectional rules (reverse rate)
        if (rule.isBidirectional() && rule.getRates().size() > 1) {
            const std::string reverseRuleName = "_reverse__" + ruleName;
            const std::string reverseRateExpr = rule.getRates()[1].toString();

            if (!isSimpleExpression(reverseRateExpr)) {
                bool reverseAsFunction = referencesObservables(reverseRateExpr);
                std::string reverseParamName = (reverseAsFunction ? "_rateLaw" : "rateLaw") + std::to_string(rateLawCounter++);

                derived.emplace(
                    reverseRuleName,
                    DerivedRateInfo {
                        reverseParamName,
                        reverseRateExpr,
                        rule.getRates()[1],  // Store actual expression
                        true,  // reverseDirection
                        reverseAsFunction
                    });
            }
        }
    }

    // Handle Arrhenius expressions with per-reaction energy-dependent rates.
    // Each reaction gets a unique rate parameter based on its specific energy delta
    // (determined by counting energy pattern matches in reactant vs product species).
    {
        // Group rules that use Arrhenius rate laws
        std::unordered_set<std::string> arrheniusRules;
        for (const auto& reaction : network.reactions.all()) {
            if (reaction.getOriginRuleName().empty()) continue;
            if (derived.find(reaction.getOriginRuleName()) != derived.end()) continue;
            if (parseArrhenius(reaction.getRateLaw()).has_value()) {
                arrheniusRules.insert(reaction.getOriginRuleName());
            }
        }

        auto paramResolver = [&](const std::string& name) -> double {
            return model.getParameters().evaluate(name);
        };

        for (const auto& ruleName : arrheniusRules) {
            const bool reverseDirection = ruleName.rfind("_reverse__", 0) == 0;
            const std::string ruleBase = reverseDirection
                ? ruleName.substr(std::string("_reverse__").size())
                : ruleName;
            // Perl naming: Rule1Rate_N for forward, Rule1rRate_N for reverse
            // Our rule names are like R1 or _reverse__R1
            const std::string paramPrefix = reverseDirection
                ? ruleBase + "rRate_"
                : ruleBase + "Rate_";

            // Compute per-reaction rates, caching by energy delta fingerprint
            std::unordered_map<std::string, std::pair<std::string, double>> fingerprintToParam;
            std::size_t nextParamIndex = 1;

            DerivedRateInfo info;
            info.paramName = "__" + ruleName + "_local1"; // fallback name (unused with per-reaction)
            info.expression = {};
            info.exprTree = ast::Expression::number(0.0);
            info.reverseDirection = reverseDirection;
            info.isPerReactionArrhenius = true;

            for (std::size_t rxnIdx = 0; rxnIdx < network.reactions.all().size(); ++rxnIdx) {
                const auto& reaction = network.reactions.all()[rxnIdx];
                if (reaction.getOriginRuleName() != ruleName) continue;

                const auto arrhenius = parseArrhenius(reaction.getRateLaw());
                if (!arrhenius.has_value()) continue;

                // Compute energy delta fingerprint for this specific reaction
                std::string deltaFingerprint;
                double deltaG = 0.0;
                for (const auto& energyPattern : model.getEnergyPatterns()) {
                    long long delta = 0;
                    for (const auto reactantIndex : reaction.getReactants()) {
                        delta -= static_cast<long long>(countPatternMatches(energyPattern.getGraph(), network.species.get(reactantIndex)));
                    }
                    for (const auto productIndex : reaction.getProducts()) {
                        delta += static_cast<long long>(countPatternMatches(energyPattern.getGraph(), network.species.get(productIndex)));
                    }
                    deltaFingerprint += std::to_string(delta) + ",";
                    if (delta != 0) {
                        double gf = 0.0;
                        try {
                            gf = evaluateExpressionString(energyPattern.getExpression().toString(), paramResolver);
                        } catch (...) {}
                        deltaG += delta * gf;
                    }
                }

                // Check if this fingerprint already has a parameter
                auto fpIt = fingerprintToParam.find(deltaFingerprint);
                if (fpIt != fingerprintToParam.end()) {
                    // Reuse existing parameter for same energy fingerprint
                    info.perReactionRates[rxnIdx] = fpIt->second;
                } else {
                    // Compute the numeric rate value
                    double phi = 0.0;
                    double eact0 = 0.0;
                    try {
                        const std::string phiExpr = reverseDirection
                            ? ("1-" + arrhenius->phiArg)
                            : arrhenius->phiArg;
                        phi = evaluateExpressionString(phiExpr, paramResolver);
                        eact0 = evaluateExpressionString(arrhenius->eaArg, paramResolver);
                    } catch (...) {}
                    double rate = std::exp(-(eact0 + phi * deltaG));

                    std::string paramName = paramPrefix + std::to_string(nextParamIndex++);
                    auto paramPair = std::make_pair(paramName, rate);
                    fingerprintToParam[deltaFingerprint] = paramPair;
                    info.perReactionRates[rxnIdx] = paramPair;
                }
            }

            derived.emplace(ruleName, std::move(info));
        }
    }

    return derived;
}

void NetWriter::write(const std::filesystem::path& outputPath, ast::Model& model, const engine::GeneratedNetwork& network) {
    std::ofstream out(outputPath);
    if (!out) {
        throw std::runtime_error("Could not open output file: " + outputPath.string());
    }

    out << "# Created by bng_cpp\n";
    if (!model.getVersion().empty()) {
        out << "version(\"" << model.getVersion() << "\")\n";
    }
    for (const auto& [key, value] : model.getOptions()) {
        out << "setOption(\"" << key << "\",\"" << value << "\")\n";
    }

    const auto derivedRateParams = NetWriter::buildDerivedRateParams(model, network);

    // Add derived rate parameters to the model so they can be used by OdeIntegrator
    // We evaluate the expression strings numerically using a simple evaluator
    // Format: exp(-(Ea0_S_kinase+(phi*(Gf_S_kinase/RT))))
    // We manually parse and evaluate these
    auto paramResolver = [&](const std::string& name) -> double {
        return model.getParameters().evaluate(name);
    };

    for (const auto& [ruleName, info] : derivedRateParams) {
        if (info.isLocalFunction) {
            // Register per-species rate params
            for (const auto& [specIdx, paramPair] : info.perSpeciesRates) {
                model.getParameters().add(ast::Parameter(paramPair.first, ast::Expression::number(paramPair.second)));
            }
        } else if (info.isPerReactionArrhenius) {
            for (const auto& [rxnIdx, paramPair] : info.perReactionRates) {
                model.getParameters().add(ast::Parameter(paramPair.first, ast::Expression::number(paramPair.second)));
            }
        } else {
            try {
                double value = evaluateExpressionString(info.expression, paramResolver);
                model.getParameters().add(ast::Parameter(info.paramName, ast::Expression::number(value)));
            } catch (const std::exception& e) {
                // If evaluation fails, skip - will error later if used
            }
        }
    }

    // Build set of derived parameter names to skip when writing base parameters
    std::unordered_set<std::string> derivedParamNames;
    for (const auto& [ruleName, info] : derivedRateParams) {
        if (info.isLocalFunction) {
            for (const auto& [specIdx, paramPair] : info.perSpeciesRates) {
                derivedParamNames.insert(paramPair.first);
            }
        } else if (info.isPerReactionArrhenius) {
            for (const auto& [rxnIdx, paramPair] : info.perReactionRates) {
                derivedParamNames.insert(paramPair.first);
            }
        } else {
            derivedParamNames.insert(info.paramName);
        }
    }

    out << "begin parameters\n";
    std::size_t parameterIndex = 1;
    for (const auto& parameter : model.getParameters().all()) {
        // Skip derived parameters - they'll be written later with symbolic expressions
        if (derivedParamNames.count(parameter.getName()) > 0) {
            continue;
        }
        // Write parameter value with enough precision for %.8g comparison (Perl compatibility)
        {
            std::ostringstream valStr;
            valStr << std::setprecision(15) << parameter.getValue();
            out << "    " << parameterIndex++ << " " << parameter.getName() << " " << valStr.str() << '\n';
        }
    }
    // Seed species initial amounts are written directly in species section
    // using parameter names (e.g., Ga_0) - no _InitialConc parameters needed
    // Write derived rate parameters (only non-function ones) with evaluated numeric values
    // Perl writes the evaluated numeric value for ConstantExpression derived params.
    auto formatDerivedParam = [&](const DerivedRateInfo& info) -> std::string {
        // Local function params (__R{N}_local{M}): write expression string (Perl convention)
        if (info.paramName.find("__R") == 0 && info.paramName.find("_local") != std::string::npos) {
            return compactExpression(info.expression);
        }
        // Regular derived params: write evaluated numeric value
        try {
            double val = info.exprTree.evaluate(
                [&](const std::string& name) -> double {
                    if (model.getParameters().contains(name)) {
                        return model.getParameters().get(name).getValue();
                    }
                    return 0.0;
                });
            std::ostringstream voss;
            voss << std::setprecision(15) << val;
            return voss.str();
        } catch (...) {
            return compactExpression(info.expression);
        }
    };
    for (const auto& rule : model.getReactionRules()) {
        // Write forward rule derived params
        const auto found = derivedRateParams.find(rule.getRuleName());
        if (found != derivedRateParams.end() && !found->second.asFunction) {
            if (found->second.isPerReactionArrhenius) {
                // Write per-reaction Arrhenius params interleaved: forward param, then
                // matching reverse param (Perl writes them in order of first encounter)
            } else if (found->second.isLocalFunction) {
                std::vector<std::pair<std::size_t, std::pair<std::string, double>>> sorted;
                for (const auto& [specIdx, paramPair] : found->second.perSpeciesRates) {
                    sorted.emplace_back(specIdx, paramPair);
                }
                std::sort(sorted.begin(), sorted.end());
                for (const auto& [specIdx, paramPair] : sorted) {
                    std::ostringstream voss;
                    voss << std::setprecision(15) << paramPair.second;
                    out << "    " << parameterIndex++ << " " << paramPair.first << " "
                        << voss.str() << '\n';
                }
            } else {
                out << "    " << parameterIndex++ << " " << found->second.paramName << " "
                    << formatDerivedParam(found->second) << '\n';
            }
        }
        // Write reverse rule derived params
        const auto reverseFound = derivedRateParams.find("_reverse__" + rule.getRuleName());
        if (reverseFound != derivedRateParams.end() && !reverseFound->second.asFunction) {
            if (reverseFound->second.isPerReactionArrhenius) {
                // Handled below with forward params
            } else {
                out << "    " << parameterIndex++ << " " << reverseFound->second.paramName << " "
                    << formatDerivedParam(reverseFound->second) << '\n';
            }
        }
        // Write per-reaction Arrhenius params (forward + reverse interleaved by reaction order)
        if ((found != derivedRateParams.end() && found->second.isPerReactionArrhenius) ||
            (reverseFound != derivedRateParams.end() && reverseFound->second.isPerReactionArrhenius)) {
            // Collect all unique params from both forward and reverse, ordered by reaction index
            std::set<std::string> writtenParams;
            std::vector<std::pair<std::size_t, std::pair<std::string, double>>> allParams;
            if (found != derivedRateParams.end()) {
                for (const auto& [rxnIdx, pp] : found->second.perReactionRates) {
                    allParams.emplace_back(rxnIdx, pp);
                }
            }
            if (reverseFound != derivedRateParams.end()) {
                for (const auto& [rxnIdx, pp] : reverseFound->second.perReactionRates) {
                    allParams.emplace_back(rxnIdx, pp);
                }
            }
            std::sort(allParams.begin(), allParams.end());
            for (const auto& [rxnIdx, pp] : allParams) {
                if (writtenParams.insert(pp.first).second) {
                    std::ostringstream voss;
                    voss << std::setprecision(15) << pp.second;
                    out << "    " << parameterIndex++ << " " << pp.first << " "
                        << voss.str() << "  # ConstantExpression\n";
                }
            }
        }
    }
    out << "end parameters\n";

    // Write functions section for derived rate laws that reference observables
    bool hasFunctions = false;
    for (const auto& [ruleName, info] : derivedRateParams) {
        if (info.asFunction) {
            hasFunctions = true;
            break;
        }
    }
    if (hasFunctions) {
        out << "begin functions\n";
        std::size_t functionIndex = 1;
        for (const auto& rule : model.getReactionRules()) {
            const auto found = derivedRateParams.find(rule.getRuleName());
            if (found != derivedRateParams.end() && found->second.asFunction) {
                out << "    " << functionIndex++ << " " << found->second.paramName << "() " << compactExpression(found->second.expression) << '\n';
            }
            const auto reverseFound = derivedRateParams.find("_reverse__" + rule.getRuleName());
            if (reverseFound != derivedRateParams.end() && reverseFound->second.asFunction) {
                out << "    " << functionIndex++ << " " << reverseFound->second.paramName << "() " << compactExpression(reverseFound->second.expression) << '\n';
            }
        }
        out << "end functions\n";
    }

    out << "begin species\n";
    for (const auto& species : network.species.all()) {
        out << "    " << (species.getIndex() + 1) << " " << formatSpecies(species) << " "
            << formatSpeciesAmount(species, model) << '\n';
    }
    out << "end species\n";

    out << "begin reactions\n";
    std::size_t reactionIndex = 1;
    for (const auto& reaction : network.reactions.all()) {
        // Perl BNG2 sorts reactant/product indices for elementary rate laws (simple
        // parameter names), but preserves original order for MM/Sat/Hill/Function rates.
        const bool isElementary = [&]() {
            const auto derivedFound = derivedRateParams.find(reaction.getOriginRuleName());
            if (derivedFound != derivedRateParams.end()) {
                // Derived rate params that are ConstantExpressions (not functions)
                // are treated as "Ele" type in Perl — sort reactants/products
                return !derivedFound->second.asFunction;
            }
            const auto& rateExpr = reaction.getRateExpression();
            if (rateExpr.has_value()) {
                const auto kind = rateExpr->kind();
                if (kind == ast::ExpressionKind::Function ||
                    kind == ast::ExpressionKind::ObservableRef)
                    return false;
            }
            // Check if rate law string is a simple identifier (no operators)
            const auto& rl = reaction.getRateLaw();
            return std::all_of(rl.begin(), rl.end(),
                [](char c) { return std::isalnum(c) || c == '_' || c == '.'; });
        }();

        auto reactants = reaction.getReactants();
        auto products = reaction.getProducts();
        if (isElementary) {
            std::sort(reactants.begin(), reactants.end());
            std::sort(products.begin(), products.end());
        }

        out << "    " << reactionIndex++ << " ";
        if (reactants.empty()) {
            out << "0";  // Null reactant (synthesis rule)
        } else {
            for (std::size_t i = 0; i < reactants.size(); ++i) {
                if (i != 0) {
                    out << ",";
                }
                out << (reactants[i] + 1);
            }
        }
        out << " ";
        if (products.empty()) {
            out << "0";
        } else {
            for (std::size_t i = 0; i < products.size(); ++i) {
                if (i != 0) {
                    out << ",";
                }
                out << (products[i] + 1);
            }
        }
        out << " ";
        const std::size_t rxnIdx = reactionIndex - 2; // reactionIndex was incremented above
        const auto derivedFound = derivedRateParams.find(reaction.getOriginRuleName());
        if (derivedFound != derivedRateParams.end() && derivedFound->second.isPerReactionArrhenius) {
            // Per-reaction energy-pattern Arrhenius rate
            const auto rxnIt = derivedFound->second.perReactionRates.find(rxnIdx);
            std::string rateParamName = rxnIt != derivedFound->second.perReactionRates.end()
                ? rxnIt->second.first : derivedFound->second.paramName;
            if (std::abs(reaction.getFactor() - 1.0) >= 1e-9) {
                out << formatFactor(reaction.getFactor()) << "*";
            }
            out << rateParamName;
            // Perl uses #Rule1 / #Rule1r for forward/reverse
            const bool isReverse = reaction.getOriginRuleName().rfind("_reverse__", 0) == 0;
            std::string ruleComment = isReverse
                ? reaction.getOriginRuleName().substr(std::string("_reverse__").size()) + "r"
                : reaction.getOriginRuleName();
            out << " #" << ruleComment;
        } else if (derivedFound != derivedRateParams.end() && derivedFound->second.isLocalFunction) {
            // Per-species local function rate: look up by reactant species
            std::string rateParamName = derivedFound->second.paramName + "_1"; // fallback
            if (!reactants.empty()) {
                const auto specIt = derivedFound->second.perSpeciesRates.find(reactants[0]);
                if (specIt != derivedFound->second.perSpeciesRates.end()) {
                    rateParamName = specIt->second.first;
                }
            }
            if (std::abs(reaction.getFactor() - 1.0) >= 1e-9) {
                out << formatFactor(reaction.getFactor()) << "*";
            }
            out << rateParamName;
            // Perl writes rule name comment like #Rule1
            out << " #" << reaction.getOriginRuleName();
        } else if (derivedFound != derivedRateParams.end()) {
            const auto unitFactor = unitConversionFactor(reaction, model, network);
            const auto unitExpr = unitConversionExpression(reaction, model, network);
            bool wrotePrefix = false;
            if (unitFactor.has_value()) {
                out << formatDoubleScientific(*unitFactor) << "*";
                wrotePrefix = true;
            }
            if (std::abs(reaction.getFactor() - 1.0) >= 1e-9) {
                out << formatFactor(reaction.getFactor()) << "*";
                wrotePrefix = true;
            }
            out << derivedFound->second.paramName;
            out << " #" << (reaction.getOriginRuleName().rfind("_", 0) == 0 ? reaction.getOriginRuleName() : "_" + reaction.getOriginRuleName());
            if (unitExpr.has_value()) {
                out << " unit_conversion=" << *unitExpr;
            }
        } else {
            // Zero-order synthesis in compartmental models: multiply rate by compartment volume
            // (Perl BNG2 convention: rate_molecules = k * volume for 0th order reactions)
            if (reactants.empty() && !products.empty() && !model.getCompartments().empty()) {
                std::string productCompartment;
                for (const auto prodIdx : products) {
                    const auto& comp = network.species.get(prodIdx).getCompartment();
                    if (!comp.empty()) {
                        productCompartment = comp;
                        break;
                    }
                }
                if (!productCompartment.empty()) {
                    for (const auto& compartment : model.getCompartments()) {
                        if (compartment.getName() == productCompartment) {
                            // Find parameter name for compartment volume
                            std::string volName;
                            for (const auto& parameter : model.getParameters().all()) {
                                if (std::abs(parameter.getValue() - compartment.getVolume()) < 1e-18) {
                                    volName = parameter.getName();
                                    break;
                                }
                            }
                            if (!volName.empty()) {
                                out << volName << "*";
                            } else {
                                out << compartment.getVolume() << "*";
                            }
                            break;
                        }
                    }
                }
            }
            if (std::abs(reaction.getFactor() - 1.0) >= 1e-9) {
                out << formatFactor(reaction.getFactor()) << "*";
            }
            // Check if the rate is a built-in BNG function call (Sat, MM, Hill, etc.)
            // If so, write as "FunctionName arg1 arg2" (space-separated, Perl format)
            const auto& rateExpr = reaction.getRateExpression();
            if (rateExpr.has_value() &&
                rateExpr->kind() == ast::ExpressionKind::Function &&
                builtinRateLawFunctions.count(rateExpr->name()) > 0) {
                const auto canonIt = builtinCanonicalName.find(rateExpr->name());
                out << (canonIt != builtinCanonicalName.end() ? canonIt->second : rateExpr->name());
                for (const auto& arg : rateExpr->args()) {
                    out << " " << arg.toString();
                }
            } else if (rateExpr.has_value() &&
                       (rateExpr->kind() == ast::ExpressionKind::Function ||
                        rateExpr->kind() == ast::ExpressionKind::ObservableRef)) {
                // Model-defined or built-in function — write with original casing
                const auto& exprName = rateExpr->name();
                std::string canonName = exprName;
                // Check model functions first
                for (const auto& func : model.getFunctions()) {
                    if (func.getName() == exprName) {
                        canonName = func.getName();
                        break;
                    }
                }
                out << canonName;
                // Write arguments if any (not empty-arg functions like kPlus())
                if (!rateExpr->args().empty()) {
                    for (const auto& arg : rateExpr->args()) {
                        out << " " << arg.toString();
                    }
                }
            } else {
                out << reaction.getRateLaw();
            }
            // Append rule name comment for non-derived reactions
            if (!reaction.getOriginRuleName().empty()) {
                out << " #" << reaction.getOriginRuleName();
            }
        }
        out << '\n';
    }
    out << "end reactions\n";

    out << "begin groups\n";
    std::size_t groupIndex = 1;
    for (const auto& observable : model.getObservables()) {
        out << "    " << groupIndex++ << " " << observable.getName() << " ";
        bool firstEntry = true;
        for (std::size_t speciesIndex = 0; speciesIndex < network.species.size(); ++speciesIndex) {
            std::size_t weight = 0;
            for (const auto& patternText : observable.getPatterns()) {
                const auto pattern = parseObservablePattern(patternText, model);
                BNGcore::UllmannSGIso matcher(pattern, network.species.get(speciesIndex).getSpeciesGraph().getGraph());
                BNGcore::List<BNGcore::Map> maps;
                weight += matcher.find_maps(maps);
            }
            if (observable.getType() == "Species" && weight > 0) {
                weight = 1;
            }
            if (weight == 0) {
                continue;
            }
            if (!firstEntry) {
                out << ",";
            }
            if (weight > 1) {
                out << weight << "*";
            }
            out << (speciesIndex + 1);
            firstEntry = false;
        }
        out << '\n';
    }
    out << "end groups\n";
}

} // namespace bng::io
