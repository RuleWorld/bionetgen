#include "SbmlWriter.hpp"

#include <sstream>
#include <iomanip>
#include <algorithm>
#include <cctype>
#include <cmath>
#include <unordered_set>

#include <antlr4-runtime.h>
#include "generated/BNGLexer.h"
#include "generated/BNGParser.h"
#include "parser/PatternGraphBuilder.hpp"
#include "core/Ullmann.hpp"

namespace bng::io {

std::string SbmlWriter::escapeXml(const std::string& text) {
    std::string result;
    result.reserve(text.size());
    for (char c : text) {
        switch (c) {
            case '&': result += "&amp;"; break;
            case '<': result += "&lt;"; break;
            case '>': result += "&gt;"; break;
            case '"': result += "&quot;"; break;
            case '\'': result += "&apos;"; break;
            default: result += c; break;
        }
    }
    return result;
}

std::string SbmlWriter::makeValidSBMLId(const std::string& text) {
    std::string result;
    result.reserve(text.size());

    bool firstChar = true;
    for (char c : text) {
        if (std::isalnum(static_cast<unsigned char>(c))) {
            result += c;
            firstChar = false;
        } else if (c == '_') {
            result += '_';
            firstChar = false;
        } else if (!firstChar) {
            result += '_';
        }
    }

    if (!result.empty() && std::isdigit(static_cast<unsigned char>(result[0]))) {
        result = "_" + result;
    }

    return result.empty() ? "species" : result;
}

std::string SbmlWriter::write(const ast::Model& model, const engine::GeneratedNetwork* network) {
    Options options;
    return write(model, network, options);
}

std::string SbmlWriter::write(const ast::Model& model, const engine::GeneratedNetwork* network, const Options& options) {
    std::ostringstream sbml;

    // Compute observable groups (needs network for species matching)
    std::vector<ObservableGroup> groups;
    if (network && options.networksExport) {
        groups = computeObservableGroups(model, *network);
    }

    // SBML header (matching Perl: L2V3 default)
    sbml << "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n";
    sbml << "<!-- Created by BioNetGen C++ -->\n";
    sbml << "<sbml xmlns=\"http://www.sbml.org/sbml/level" << options.level
         << "/version" << options.version << "\" "
         << "level=\"" << options.level << "\" version=\"" << options.version << "\">\n";

    sbml << "  <model id=\"" << escapeXml(makeValidSBMLId(model.getModelName())) << "\" name=\""
         << escapeXml(model.getModelName()) << "\">\n";

    // Unit definitions (Perl: substance = item)
    sbml << writeUnitDefinitions();

    // Compartments
    sbml << writeCompartments(model);

    // Parameters (constants + observables as non-constant params)
    sbml << writeParameters(model, groups);

    // Species
    sbml << writeSpecies(model, network);

    // Reactions
    if (network && options.networksExport) {
        sbml << writeReactions(*network, model);
    }

    // Assignment rules (observables + global functions)
    if (network && options.networksExport) {
        sbml << writeAssignmentRules(model, groups);
    }

    sbml << "  </model>\n";
    sbml << "</sbml>\n";

    return sbml.str();
}

std::string SbmlWriter::writeUnitDefinitions() {
    std::ostringstream sbml;
    // Perl BNG2 convention: substance unit = item (molecule count)
    sbml << "    <listOfUnitDefinitions>\n";
    sbml << "      <unitDefinition id=\"substance\" name=\"substance\">\n";
    sbml << "        <listOfUnits>\n";
    sbml << "          <unit kind=\"item\" exponent=\"1\" multiplier=\"1\"/>\n";
    sbml << "        </listOfUnits>\n";
    sbml << "      </unitDefinition>\n";
    sbml << "    </listOfUnitDefinitions>\n";
    return sbml.str();
}

std::string SbmlWriter::writeCompartments(const ast::Model& model) {
    std::ostringstream sbml;

    if (model.getCompartments().empty()) {
        // Perl convention: default compartment named "default" with volume 1
        sbml << "    <listOfCompartments>\n";
        sbml << "      <compartment id=\"default\" spatialDimensions=\"3\" size=\"1\" constant=\"true\"/>\n";
        sbml << "    </listOfCompartments>\n";
        return sbml.str();
    }

    sbml << "    <listOfCompartments>\n";
    for (const auto& comp : model.getCompartments()) {
        sbml << "      <compartment id=\"" << makeValidSBMLId(comp.getName())
             << "\" name=\"" << escapeXml(comp.getName())
             << "\" spatialDimensions=\"" << comp.getDimension()
             << "\" size=\"" << comp.getVolume()
             << "\" constant=\"true\"";

        if (!comp.getParent().empty()) {
            sbml << " outside=\"" << makeValidSBMLId(comp.getParent()) << "\"";
        }

        sbml << "/>\n";
    }
    sbml << "    </listOfCompartments>\n";
    return sbml.str();
}

std::string SbmlWriter::writeParameters(const ast::Model& model, const std::vector<ObservableGroup>& groups) {
    std::ostringstream sbml;
    sbml << "    <listOfParameters>\n";

    // Model parameters (constant=true)
    for (const auto& param : model.getParameters().all()) {
        sbml << "      <parameter id=\"" << makeValidSBMLId(param.getName())
             << "\" name=\"" << escapeXml(param.getName())
             << "\" value=\"" << param.getValue()
             << "\" constant=\"true\"/>\n";
    }

    // Observables as non-constant parameters (Perl: constant=false)
    for (const auto& group : groups) {
        sbml << "      <parameter id=\"" << makeValidSBMLId(group.name)
             << "\" name=\"" << escapeXml(group.name)
             << "\" value=\"0\" constant=\"false\"/>\n";
    }

    // Global functions (no args) as non-constant parameters
    for (const auto& func : model.getFunctions()) {
        if (func.getArgs().empty()) {
            sbml << "      <parameter id=\"" << makeValidSBMLId(func.getName())
                 << "\" name=\"" << escapeXml(func.getName())
                 << "\" value=\"0\" constant=\"false\"/>\n";
        }
    }

    sbml << "    </listOfParameters>\n";
    return sbml.str();
}

std::string SbmlWriter::writeSpecies(const ast::Model& model, const engine::GeneratedNetwork* network) {
    std::ostringstream sbml;
    sbml << "    <listOfSpecies>\n";

    if (network) {
        for (std::size_t i = 0; i < network->species.size(); ++i) {
            const auto& species = network->species.get(i);
            std::string speciesId = "S" + std::to_string(i + 1);
            std::string compartmentId = species.getCompartment().empty()
                ? "default" : makeValidSBMLId(species.getCompartment());

            sbml << "      <species id=\"" << speciesId
                 << "\" name=\"" << escapeXml(species.getSpeciesGraph().toString())
                 << "\" compartment=\"" << compartmentId
                 << "\" initialAmount=\"" << species.getAmount()
                 << "\" hasOnlySubstanceUnits=\"true\"";

            if (species.isConstant()) {
                sbml << " constant=\"true\" boundaryCondition=\"true\"";
            } else {
                sbml << " constant=\"false\" boundaryCondition=\"false\"";
            }

            sbml << "/>\n";
        }
    } else {
        for (std::size_t i = 0; i < model.getSeedSpecies().size(); ++i) {
            const auto& seed = model.getSeedSpecies()[i];
            std::string speciesId = "S" + std::to_string(i + 1);
            std::string compartmentId = seed.getCompartment().empty()
                ? "default" : makeValidSBMLId(seed.getCompartment());

            auto amountValue = seed.getAmount().evaluate([&](const std::string& name) {
                return model.getParameters().evaluate(name);
            }, 0.0);

            sbml << "      <species id=\"" << speciesId
                 << "\" name=\"" << escapeXml(seed.getPattern())
                 << "\" compartment=\"" << compartmentId
                 << "\" initialAmount=\"" << amountValue
                 << "\" hasOnlySubstanceUnits=\"true\"";

            if (seed.isConstant()) {
                sbml << " constant=\"true\" boundaryCondition=\"true\"";
            } else {
                sbml << " constant=\"false\" boundaryCondition=\"false\"";
            }

            sbml << "/>\n";
        }
    }

    sbml << "    </listOfSpecies>\n";
    return sbml.str();
}

std::string SbmlWriter::writeAssignmentRules(const ast::Model& model, const std::vector<ObservableGroup>& groups) {
    std::ostringstream sbml;

    bool hasRules = !groups.empty();
    for (const auto& func : model.getFunctions()) {
        if (func.getArgs().empty()) { hasRules = true; break; }
    }
    if (!hasRules) return {};

    sbml << "    <listOfRules>\n";

    // Observable assignment rules: each observable = weighted sum of species
    for (const auto& group : groups) {
        sbml << "      <assignmentRule variable=\"" << makeValidSBMLId(group.name) << "\">\n";
        sbml << "        <math xmlns=\"http://www.w3.org/1998/Math/MathML\">\n";

        if (group.entries.empty()) {
            // No matching species - observable = 0
            sbml << "          <cn> 0 </cn>\n";
        } else if (group.entries.size() == 1) {
            // Single species
            const auto& [idx, weight] = group.entries[0];
            if (weight == 1) {
                sbml << "          <ci> S" << (idx + 1) << " </ci>\n";
            } else {
                sbml << "          <apply>\n";
                sbml << "            <times/>\n";
                sbml << "            <cn> " << weight << " </cn>\n";
                sbml << "            <ci> S" << (idx + 1) << " </ci>\n";
                sbml << "          </apply>\n";
            }
        } else {
            // Multiple species - sum them
            sbml << "          <apply>\n";
            sbml << "            <plus/>\n";
            for (const auto& [idx, weight] : group.entries) {
                if (weight == 1) {
                    sbml << "            <ci> S" << (idx + 1) << " </ci>\n";
                } else {
                    sbml << "            <apply>\n";
                    sbml << "              <times/>\n";
                    sbml << "              <cn> " << weight << " </cn>\n";
                    sbml << "              <ci> S" << (idx + 1) << " </ci>\n";
                    sbml << "            </apply>\n";
                }
            }
            sbml << "          </apply>\n";
        }

        sbml << "        </math>\n";
        sbml << "      </assignmentRule>\n";
    }

    // Global function assignment rules (functions with no arguments)
    for (const auto& func : model.getFunctions()) {
        if (!func.getArgs().empty()) continue;

        sbml << "      <assignmentRule variable=\"" << makeValidSBMLId(func.getName()) << "\">\n";
        sbml << "        <math xmlns=\"http://www.w3.org/1998/Math/MathML\">\n";
        sbml << exprToMathML(func.getExpression(), "          ");
        sbml << "        </math>\n";
        sbml << "      </assignmentRule>\n";
    }

    sbml << "    </listOfRules>\n";
    return sbml.str();
}

std::string SbmlWriter::writeReactions(const engine::GeneratedNetwork& network, const ast::Model& model) {
    std::ostringstream sbml;
    sbml << "    <listOfReactions>\n";

    const auto& reactions = network.reactions.all();
    for (std::size_t r = 0; r < reactions.size(); ++r) {
        const auto& rxn = reactions[r];
        std::string reactionId = "R" + std::to_string(r + 1);

        sbml << "      <reaction id=\"" << reactionId << "\" reversible=\"false\">\n";

        // Reactants
        if (!rxn.getReactants().empty()) {
            sbml << "        <listOfReactants>\n";
            for (const auto idx : rxn.getReactants()) {
                sbml << "          <speciesReference species=\"S" << (idx + 1) << "\"/>\n";
            }
            sbml << "        </listOfReactants>\n";
        }

        // Products
        if (!rxn.getProducts().empty()) {
            sbml << "        <listOfProducts>\n";
            for (const auto idx : rxn.getProducts()) {
                sbml << "          <speciesReference species=\"S" << (idx + 1) << "\"/>\n";
            }
            sbml << "        </listOfProducts>\n";
        }

        // Kinetic law with MathML
        sbml << "        <kineticLaw>\n";
        sbml << "          <math xmlns=\"http://www.w3.org/1998/Math/MathML\">\n";
        sbml << rateLawToMathML(rxn, model, "            ");
        sbml << "          </math>\n";
        sbml << "        </kineticLaw>\n";

        sbml << "      </reaction>\n";
    }

    sbml << "    </listOfReactions>\n";
    return sbml.str();
}

std::vector<SbmlWriter::ObservableGroup> SbmlWriter::computeObservableGroups(
    const ast::Model& model, const engine::GeneratedNetwork& network) {
    std::vector<ObservableGroup> groups;
    auto& mutableModel = const_cast<ast::Model&>(model);

    for (const auto& observable : model.getObservables()) {
        ObservableGroup group;
        group.name = observable.getName();
        group.type = observable.getType();

        for (std::size_t speciesIndex = 0; speciesIndex < network.species.size(); ++speciesIndex) {
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
                            network.species.get(speciesIndex).getSpeciesGraph().getGraph());
                        BNGcore::List<BNGcore::Map> maps;
                        weight += matcher.find_maps(maps);
                    }
                } catch (...) {
                    // Skip patterns that fail to parse
                }
            }

            if (observable.getType() == "Species" && weight > 0) {
                weight = 1;
            }

            if (weight > 0) {
                group.entries.push_back({speciesIndex, weight});
            }
        }

        groups.push_back(std::move(group));
    }

    return groups;
}

std::string SbmlWriter::exprToMathML(const ast::Expression& expr, const std::string& indent) {
    std::ostringstream out;

    switch (expr.kind()) {
        case ast::ExpressionKind::Number:
            out << indent << "<cn> " << expr.name() << " </cn>\n";
            break;

        case ast::ExpressionKind::Identifier:
            out << indent << "<ci> " << escapeXml(expr.name()) << " </ci>\n";
            break;

        case ast::ExpressionKind::Unary:
            if (expr.name() == "-") {
                out << indent << "<apply>\n";
                out << indent << "  <minus/>\n";
                out << exprToMathML(expr.args()[0], indent + "  ");
                out << indent << "</apply>\n";
            } else {
                out << exprToMathML(expr.args()[0], indent);
            }
            break;

        case ast::ExpressionKind::Binary: {
            out << indent << "<apply>\n";
            const auto& op = expr.name();
            if (op == "+") out << indent << "  <plus/>\n";
            else if (op == "-") out << indent << "  <minus/>\n";
            else if (op == "*") out << indent << "  <times/>\n";
            else if (op == "/") out << indent << "  <divide/>\n";
            else if (op == "^" || op == "**") out << indent << "  <power/>\n";
            else out << indent << "  <times/>\n";

            out << exprToMathML(expr.args()[0], indent + "  ");
            out << exprToMathML(expr.args()[1], indent + "  ");
            out << indent << "</apply>\n";
            break;
        }

        case ast::ExpressionKind::Function:
        case ast::ExpressionKind::ObservableRef: {
            const auto& funcName = expr.name();
            // Built-in math functions
            if (funcName == "exp" || funcName == "log" || funcName == "ln" ||
                funcName == "sin" || funcName == "cos" || funcName == "tan" ||
                funcName == "abs" || funcName == "sqrt" || funcName == "floor" ||
                funcName == "ceiling") {
                out << indent << "<apply>\n";
                if (funcName == "ln" || funcName == "log") {
                    out << indent << "  <ln/>\n";
                } else if (funcName == "exp") {
                    out << indent << "  <exp/>\n";
                } else if (funcName == "sqrt") {
                    out << indent << "  <root/>\n";
                } else if (funcName == "abs") {
                    out << indent << "  <abs/>\n";
                } else if (funcName == "floor") {
                    out << indent << "  <floor/>\n";
                } else if (funcName == "ceiling") {
                    out << indent << "  <ceiling/>\n";
                } else {
                    out << indent << "  <" << funcName << "/>\n";
                }
                for (const auto& arg : expr.args()) {
                    out << exprToMathML(arg, indent + "  ");
                }
                out << indent << "</apply>\n";
            } else if (funcName == "if" && expr.args().size() == 3) {
                // Piecewise for if(cond, then, else)
                out << indent << "<piecewise>\n";
                out << indent << "  <piece>\n";
                out << exprToMathML(expr.args()[1], indent + "    ");
                out << exprToMathML(expr.args()[0], indent + "    ");
                out << indent << "  </piece>\n";
                out << indent << "  <otherwise>\n";
                out << exprToMathML(expr.args()[2], indent + "    ");
                out << indent << "  </otherwise>\n";
                out << indent << "</piecewise>\n";
            } else {
                // Reference to model-defined function or observable
                out << indent << "<ci> " << escapeXml(funcName) << " </ci>\n";
            }
            break;
        }
    }

    return out.str();
}

std::string SbmlWriter::rateLawToMathML(const ast::Rxn& rxn, const ast::Model& model, const std::string& indent) {
    std::ostringstream out;

    // Build rate expression: statFactor * rateConstant * reactant1 * ... * reactantN
    // If the rate law has a functional expression, use that; otherwise parse the rate string

    std::vector<std::string> terms;
    double statFactor = rxn.getFactor();

    // Handle the rate expression
    const auto& rateExpr = rxn.getRateExpression();
    bool hasFunctionalRate = rateExpr.has_value() &&
        (rateExpr->kind() == ast::ExpressionKind::Function ||
         rateExpr->kind() == ast::ExpressionKind::ObservableRef);

    if (hasFunctionalRate) {
        // Functional rate law - emit function reference with reactant multiplication
        out << indent << "<apply>\n";
        out << indent << "  <times/>\n";

        if (std::abs(statFactor - 1.0) > 1e-12) {
            out << indent << "  <cn> " << statFactor << " </cn>\n";
        }

        // Function call in MathML - just reference the function name
        out << exprToMathML(*rateExpr, indent + "  ");

        // Multiply by reactant concentrations
        for (const auto idx : rxn.getReactants()) {
            out << indent << "  <ci> S" << (idx + 1) << " </ci>\n";
        }

        out << indent << "</apply>\n";
    } else {
        // Elementary mass-action rate law
        // Try to parse rate as number, or reference parameter
        const std::string& rateLaw = rxn.getRateLaw();

        // Strip any |local: suffix from rate law
        std::string cleanRate = rateLaw;
        auto localPos = cleanRate.find("|local:");
        if (localPos != std::string::npos) {
            cleanRate = cleanRate.substr(0, localPos);
        }

        bool isNumeric = false;
        double rateVal = 0.0;
        try {
            std::size_t pos = 0;
            rateVal = std::stod(cleanRate, &pos);
            if (pos == cleanRate.size()) isNumeric = true;
        } catch (...) {}

        // Count total multiplication terms
        int termCount = 0;
        if (std::abs(statFactor - 1.0) > 1e-12) termCount++;
        termCount++; // rate constant
        termCount += static_cast<int>(rxn.getReactants().size());

        if (termCount <= 1) {
            // Just the rate constant
            if (isNumeric) {
                double combined = statFactor * rateVal;
                out << indent << "<cn> " << combined << " </cn>\n";
            } else {
                out << indent << "<ci> " << escapeXml(makeValidSBMLId(cleanRate)) << " </ci>\n";
            }
        } else {
            out << indent << "<apply>\n";
            out << indent << "  <times/>\n";

            if (isNumeric) {
                double combined = statFactor * rateVal;
                out << indent << "  <cn> " << combined << " </cn>\n";
            } else {
                if (std::abs(statFactor - 1.0) > 1e-12) {
                    out << indent << "  <cn> " << statFactor << " </cn>\n";
                }
                out << indent << "  <ci> " << escapeXml(makeValidSBMLId(cleanRate)) << " </ci>\n";
            }

            // Reactant concentrations
            for (const auto idx : rxn.getReactants()) {
                out << indent << "  <ci> S" << (idx + 1) << " </ci>\n";
            }

            out << indent << "</apply>\n";
        }
    }

    return out.str();
}

} // namespace bng::io
