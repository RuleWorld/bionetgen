#include "io/NetWriter.hpp"

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
    text.erase(std::remove_if(text.begin(), text.end(), [](unsigned char c) {
        return std::isspace(c) != 0;
    }), text.end());
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
            return formatInitialAmountName(i);
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

        // Build string expression for .net file
        std::string expr = "exp((-( " + arrhenius->eaArg;
        if (!deltaExpr.empty()) {
            expr = "exp((-( " + arrhenius->eaArg + "+(" + phiExpr + "*(" + deltaExpr + "))))))";
        } else {
            expr = "exp((-( " + arrhenius->eaArg + ")))";
        }
        expr = compactExpression(expr);

        // Placeholder Expression tree (won't be used for evaluation)
        ast::Expression exprTree = ast::Expression::number(0.0);

        derived.emplace(
            reaction.getOriginRuleName(),
            DerivedRateInfo {
                reverseDirection ? "__reverse__" + ruleBase + "_local1" : "__" + ruleBase + "_local1",
                expr,
                exprTree,
                reverseDirection,
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
        out << "    " << parameterIndex++ << " " << parameter.getName() << " " << parameter.getValue() << '\n';
    }
    for (std::size_t i = 0; i < model.getSeedSpecies().size(); ++i) {
        const auto& seed = model.getSeedSpecies()[i];
        out << "    " << parameterIndex++ << " " << formatInitialAmountName(i) << " " << seed.getAmount().toString() << '\n';
    }
    // Write derived rate parameters with symbolic expressions (for Perl compatibility)
    for (const auto& rule : model.getReactionRules()) {
        const auto found = derivedRateParams.find(rule.getRuleName());
        if (found != derivedRateParams.end()) {
            out << "    " << parameterIndex++ << " " << found->second.paramName << " " << found->second.expression << '\n';
        }
        const auto reverseFound = derivedRateParams.find("_reverse__" + rule.getRuleName());
        if (reverseFound != derivedRateParams.end()) {
            out << "    " << parameterIndex++ << " " << reverseFound->second.paramName << " " << reverseFound->second.expression << '\n';
        }
    }
    out << "end parameters\n";

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
        for (std::size_t i = 0; i < reaction.getReactants().size(); ++i) {
            if (i != 0) {
                out << ",";
            }
            out << (reaction.getReactants()[i] + 1);
        }
        out << " ";
        for (std::size_t i = 0; i < reaction.getProducts().size(); ++i) {
            if (i != 0) {
                out << ",";
            }
            out << (reaction.getProducts()[i] + 1);
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
            out << reaction.getRateLaw();
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
