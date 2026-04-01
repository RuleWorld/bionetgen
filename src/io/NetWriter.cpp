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

    // Try as number
    try {
        return std::stod(trimmed);
    } catch (...) {}

    // Try as parameter name (no operators)
    if (trimmed.find_first_of("+-*/()") == std::string::npos) {
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
            else if (parenDepth == 0 && (trimmed[i] == '+' || trimmed[i] == '-' || trimmed[i] == '*' || trimmed[i] == '/')) {
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
    // Process +/- (equal precedence), then */, with left-to-right associativity
    int parenDepth = 0;
    int lastAddSub = -1;
    char lastAddSubOp = 0;
    int lastMulDiv = -1;
    char lastMulDivOp = 0;

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
    if (!species.getCompartment().empty()) {
        result += "@" + species.getCompartment() + "::";
    }
    if (species.isConstant()) {
        result += "$";
    }
    result += species.getSpeciesGraph().toString();
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

        // Skip model-defined functions (kPlus, kMinus, etc.)
        // These are written directly as function name in the reaction rate field
        // Parser may store them as Function or ObservableRef kind
        if (rateExprObj.kind() == ast::ExpressionKind::Function ||
            rateExprObj.kind() == ast::ExpressionKind::ObservableRef) {
            bool isModelFunction = false;
            const auto& exprName = rateExprObj.name();
            for (const auto& func : model.getFunctions()) {
                if (func.getName() == exprName) {
                    isModelFunction = true;
                    break;
                }
            }
            if (isModelFunction) {
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
        // Perl convention: _rateLaw for functions (references observables), rateLaw for parameters
        bool asFunction = referencesObservables(rateExpr);
        std::string paramName = (asFunction ? "_rateLaw" : "rateLaw") + std::to_string(rateLawCounter++);

        derived.emplace(
            ruleName,
            DerivedRateInfo {
                paramName,
                rateExpr,
                ast::Expression::number(0.0),  // Placeholder
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
                        ast::Expression::number(0.0),
                        true,  // reverseDirection
                        reverseAsFunction
                    });
            }
        }
    }

    // Also handle Arrhenius expressions (keep existing logic for compatibility)
    for (const auto& reaction : network.reactions.all()) {
        if (reaction.getOriginRuleName().empty() || derived.find(reaction.getOriginRuleName()) != derived.end()) {
            continue;
        }
        const auto arrhenius = parseArrhenius(reaction.getRateLaw());
        if (!arrhenius.has_value()) {
            continue;
        }

        const bool reverseDirection = reaction.getOriginRuleName().rfind("_reverse__", 0) == 0;
        const std::string ruleBase = reverseDirection
            ? reaction.getOriginRuleName().substr(std::string("_reverse__").size())
            : reaction.getOriginRuleName();
        const std::string phiExpr = reverseDirection
            ? parenthesize("1-" + arrhenius->phiArg)
            : arrhenius->phiArg;
        const std::string deltaExpr = energyDeltaExpression(reaction, model, network);

        std::string expr = "exp((-( " + arrhenius->eaArg;
        if (!deltaExpr.empty()) {
            expr = "exp((-( " + arrhenius->eaArg + "+(" + phiExpr + "*(" + deltaExpr + "))))))";
        } else {
            expr = "exp((-( " + arrhenius->eaArg + ")))";
        }
        expr = compactExpression(expr);

        derived.emplace(
            reaction.getOriginRuleName(),
            DerivedRateInfo {
                reverseDirection ? "__reverse__" + ruleBase + "_local1" : "__" + ruleBase + "_local1",
                expr,
                ast::Expression::number(0.0),
                reverseDirection,
                false  // Arrhenius are parameters, not functions
            });
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
        try {
            // Simple evaluator for expressions of the form: exp(-(A+(B*(C))))
            // We recursively evaluate parameters and apply operations
            double value = evaluateExpressionString(info.expression, paramResolver);
            model.getParameters().add(ast::Parameter(info.paramName, ast::Expression::number(value)));
        } catch (const std::exception& e) {
            // If evaluation fails, skip - will error later if used
        }
    }

    // Build set of derived parameter names to skip when writing base parameters
    std::unordered_set<std::string> derivedParamNames;
    for (const auto& [ruleName, info] : derivedRateParams) {
        derivedParamNames.insert(info.paramName);
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
    for (const auto& rule : model.getReactionRules()) {
        const auto found = derivedRateParams.find(rule.getRuleName());
        if (found != derivedRateParams.end() && !found->second.asFunction) {
            // Try to use evaluated value; fall back to expression if not available
            std::string valueStr;
            if (model.getParameters().contains(found->second.paramName)) {
                std::ostringstream voss;
                voss << std::setprecision(15) << model.getParameters().get(found->second.paramName).getValue();
                valueStr = voss.str();
            } else {
                valueStr = compactExpression(found->second.expression);
            }
            out << "    " << parameterIndex++ << " " << found->second.paramName << " " << valueStr << '\n';
        }
        const auto reverseFound = derivedRateParams.find("_reverse__" + rule.getRuleName());
        if (reverseFound != derivedRateParams.end() && !reverseFound->second.asFunction) {
            std::string valueStr;
            if (model.getParameters().contains(reverseFound->second.paramName)) {
                std::ostringstream voss;
                voss << std::setprecision(15) << model.getParameters().get(reverseFound->second.paramName).getValue();
                valueStr = voss.str();
            } else {
                valueStr = compactExpression(reverseFound->second.expression);
            }
            out << "    " << parameterIndex++ << " " << reverseFound->second.paramName << " " << valueStr << '\n';
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
        out << "    " << reactionIndex++ << " ";
        if (reaction.getReactants().empty()) {
            out << "0";  // Null reactant (synthesis rule)
        } else {
            for (std::size_t i = 0; i < reaction.getReactants().size(); ++i) {
                if (i != 0) {
                    out << ",";
                }
                out << (reaction.getReactants()[i] + 1);
            }
        }
        out << " ";
        if (reaction.getProducts().empty()) {
            out << "0";
        } else {
            for (std::size_t i = 0; i < reaction.getProducts().size(); ++i) {
                if (i != 0) {
                    out << ",";
                }
                out << (reaction.getProducts()[i] + 1);
            }
        }
        out << " ";
        const auto derivedFound = derivedRateParams.find(reaction.getOriginRuleName());
        if (derivedFound != derivedRateParams.end()) {
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
