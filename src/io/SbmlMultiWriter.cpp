#include "SbmlMultiWriter.hpp"

#include <sstream>
#include <algorithm>
#include <cctype>
#include <set>

namespace bng::io {

std::string SbmlMultiWriter::escapeXml(const std::string& text) {
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

std::string SbmlMultiWriter::makeValidSBMLId(const std::string& text) {
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

std::string SbmlMultiWriter::write(const ast::Model& model) {
    Options options;
    return write(model, options);
}

std::string SbmlMultiWriter::write(const ast::Model& model, const Options& options) {
    std::ostringstream sbml;

    // SBML L3V1 header with Multi package namespace
    sbml << "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n";
    sbml << "<!-- Created by BioNetGen C++ (writeSBMLMulti) -->\n";
    sbml << "<sbml xmlns=\"http://www.sbml.org/sbml/level3/version1/core\"\n";
    sbml << "      xmlns:multi=\"http://www.sbml.org/sbml/level3/version1/multi/version1\"\n";
    sbml << "      level=\"" << options.level << "\" version=\"" << options.version
         << "\" multi:required=\"true\">\n";

    sbml << "  <model id=\"" << escapeXml(makeValidSBMLId(model.getModelName())) << "\" name=\""
         << escapeXml(model.getModelName()) << "\">\n";

    // Species types (molecule types with components and states)
    sbml << writeSpeciesTypes(model);

    // Compartments
    sbml << writeCompartments(model);

    // Parameters
    sbml << writeParameters(model);

    // Seed species with multi:speciesType references
    sbml << writeSeedSpecies(model);

    // Reaction rules with pattern matching
    sbml << writeReactionRules(model);

    sbml << "  </model>\n";
    sbml << "</sbml>\n";

    return sbml.str();
}

std::string SbmlMultiWriter::writeCompartments(const ast::Model& model) {
    std::ostringstream sbml;

    if (model.getCompartments().empty()) {
        sbml << "    <listOfCompartments>\n";
        sbml << "      <compartment id=\"default\" spatialDimensions=\"3\" size=\"1\" constant=\"true\">\n";
        sbml << "        <multi:listOfCompartmentReferences/>\n";
        sbml << "      </compartment>\n";
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

        sbml << ">\n";
        sbml << "        <multi:listOfCompartmentReferences/>\n";
        sbml << "      </compartment>\n";
    }
    sbml << "    </listOfCompartments>\n";
    return sbml.str();
}

std::string SbmlMultiWriter::writeSpeciesTypes(const ast::Model& model) {
    std::ostringstream sbml;

    if (model.getMoleculeTypes().empty()) {
        return {};
    }

    sbml << "    <multi:listOfSpeciesTypes>\n";

    for (const auto& molType : model.getMoleculeTypes()) {
        std::string stId = "st_" + makeValidSBMLId(molType.getName());
        sbml << "      <multi:speciesType multi:id=\"" << stId
             << "\" multi:name=\"" << escapeXml(molType.getName()) << "\">\n";

        // Components as speciesFeatureTypes and binding sites
        if (!molType.getComponents().empty()) {
            sbml << "        <multi:listOfSpeciesFeatureTypes>\n";

            for (const auto& comp : molType.getComponents()) {
                std::string compId = stId + "_" + makeValidSBMLId(comp.name);

                if (!comp.allowedStates.empty()) {
                    // Component with states -- speciesFeatureType
                    sbml << "          <multi:speciesFeatureType multi:id=\"" << compId
                         << "\" multi:name=\"" << escapeXml(comp.name)
                         << "\" multi:occur=\"1\">\n";
                    sbml << "            <multi:listOfPossibleSpeciesFeatureValues>\n";

                    for (const auto& state : comp.allowedStates) {
                        std::string stateId = compId + "_" + makeValidSBMLId(state);
                        sbml << "              <multi:possibleSpeciesFeatureValue multi:id=\""
                             << stateId << "\" multi:name=\"" << escapeXml(state) << "\"/>\n";
                    }

                    sbml << "            </multi:listOfPossibleSpeciesFeatureValues>\n";
                    sbml << "          </multi:speciesFeatureType>\n";
                } else {
                    // Component without states -- binding site
                    sbml << "          <multi:speciesFeatureType multi:id=\"" << compId
                         << "\" multi:name=\"" << escapeXml(comp.name)
                         << "\" multi:occur=\"1\">\n";
                    sbml << "            <multi:listOfPossibleSpeciesFeatureValues>\n";
                    sbml << "              <multi:possibleSpeciesFeatureValue multi:id=\""
                         << compId << "_unbound\" multi:name=\"unbound\"/>\n";
                    sbml << "              <multi:possibleSpeciesFeatureValue multi:id=\""
                         << compId << "_bound\" multi:name=\"bound\"/>\n";
                    sbml << "            </multi:listOfPossibleSpeciesFeatureValues>\n";
                    sbml << "          </multi:speciesFeatureType>\n";
                }
            }

            sbml << "        </multi:listOfSpeciesFeatureTypes>\n";
        }

        // Intra-species-type bonds (for binding sites)
        {
            std::vector<std::string> bindingSites;
            for (const auto& comp : molType.getComponents()) {
                if (comp.allowedStates.empty()) {
                    bindingSites.push_back(stId + "_" + makeValidSBMLId(comp.name));
                }
            }
            if (!bindingSites.empty()) {
                sbml << "        <multi:listOfIntraSpeciesTypeBonds>\n";
                for (std::size_t i = 0; i < bindingSites.size(); ++i) {
                    for (std::size_t j = i + 1; j < bindingSites.size(); ++j) {
                        sbml << "          <multi:intraSpeciesTypeBond multi:bindingSite1=\""
                             << bindingSites[i] << "\" multi:bindingSite2=\""
                             << bindingSites[j] << "\"/>\n";
                    }
                }
                sbml << "        </multi:listOfIntraSpeciesTypeBonds>\n";
            }
        }

        sbml << "      </multi:speciesType>\n";
    }

    sbml << "    </multi:listOfSpeciesTypes>\n";
    return sbml.str();
}

std::string SbmlMultiWriter::writeParameters(const ast::Model& model) {
    std::ostringstream sbml;
    sbml << "    <listOfParameters>\n";

    for (const auto& param : model.getParameters().all()) {
        sbml << "      <parameter id=\"" << makeValidSBMLId(param.getName())
             << "\" name=\"" << escapeXml(param.getName())
             << "\" value=\"" << param.getValue()
             << "\" constant=\"true\"/>\n";
    }

    sbml << "    </listOfParameters>\n";
    return sbml.str();
}

std::string SbmlMultiWriter::writeSeedSpecies(const ast::Model& model) {
    std::ostringstream sbml;

    if (model.getSeedSpecies().empty()) {
        return {};
    }

    sbml << "    <listOfSpecies>\n";

    for (std::size_t i = 0; i < model.getSeedSpecies().size(); ++i) {
        const auto& seed = model.getSeedSpecies()[i];
        std::string speciesId = "S" + std::to_string(i + 1);
        std::string compartmentId = seed.getCompartment().empty()
            ? "default" : makeValidSBMLId(seed.getCompartment());

        auto amountValue = seed.getAmount().evaluate([&](const std::string& name) {
            return model.getParameters().evaluate(name);
        }, 0.0);

        // Try to match species to a molecule type for multi:speciesType reference
        std::string speciesTypeRef;
        const std::string& pattern = seed.getPattern();
        for (const auto& mt : model.getMoleculeTypes()) {
            if (pattern.find(mt.getName()) != std::string::npos) {
                speciesTypeRef = "st_" + makeValidSBMLId(mt.getName());
                break;
            }
        }

        sbml << "      <species id=\"" << speciesId
             << "\" name=\"" << escapeXml(pattern)
             << "\" compartment=\"" << compartmentId
             << "\" initialAmount=\"" << amountValue
             << "\" hasOnlySubstanceUnits=\"true\"";

        if (seed.isConstant()) {
            sbml << " constant=\"true\" boundaryCondition=\"true\"";
        } else {
            sbml << " constant=\"false\" boundaryCondition=\"false\"";
        }

        if (!speciesTypeRef.empty()) {
            sbml << " multi:speciesType=\"" << speciesTypeRef << "\"";
        }

        sbml << "/>\n";
    }

    sbml << "    </listOfSpecies>\n";
    return sbml.str();
}

std::string SbmlMultiWriter::writeReactionRules(const ast::Model& model) {
    std::ostringstream sbml;

    if (model.getReactionRules().empty()) {
        return {};
    }

    sbml << "    <multi:listOfReactionRules>\n";

    for (std::size_t r = 0; r < model.getReactionRules().size(); ++r) {
        const auto& rule = model.getReactionRules()[r];
        std::string ruleId = "RR" + std::to_string(r + 1);

        // Use rule name/label if available, fall back to generated id
        std::string ruleName = rule.getRuleName();
        if (ruleName.empty()) {
            ruleName = rule.getLabel();
        }
        if (ruleName.empty()) {
            ruleName = ruleId;
        }

        sbml << "      <multi:reactionRule multi:id=\"" << makeValidSBMLId(ruleId)
             << "\" multi:name=\"" << escapeXml(ruleName)
             << "\" multi:reversible=\"" << (rule.isBidirectional() ? "true" : "false")
             << "\">\n";

        // Reactant patterns
        if (!rule.getReactants().empty()) {
            sbml << "        <multi:listOfReactantPatterns>\n";
            for (std::size_t i = 0; i < rule.getReactants().size(); ++i) {
                std::string patternId = ruleId + "_rp" + std::to_string(i + 1);
                sbml << "          <multi:reactantPattern multi:id=\"" << patternId
                     << "\" multi:compartment=\"default\">\n";
                sbml << "            <multi:listOfSpeciesTypeInstances>\n";
                sbml << "              <!-- Pattern: " << escapeXml(rule.getReactants()[i]) << " -->\n";

                // Try to resolve molecule type reference
                for (const auto& mt : model.getMoleculeTypes()) {
                    if (rule.getReactants()[i].find(mt.getName()) != std::string::npos) {
                        sbml << "              <multi:speciesTypeInstance multi:id=\""
                             << patternId << "_sti1\" multi:speciesType=\"st_"
                             << makeValidSBMLId(mt.getName()) << "\"/>\n";
                        break;
                    }
                }

                sbml << "            </multi:listOfSpeciesTypeInstances>\n";
                sbml << "          </multi:reactantPattern>\n";
            }
            sbml << "        </multi:listOfReactantPatterns>\n";
        }

        // Product patterns
        if (!rule.getProducts().empty()) {
            sbml << "        <multi:listOfProductPatterns>\n";
            for (std::size_t i = 0; i < rule.getProducts().size(); ++i) {
                std::string patternId = ruleId + "_pp" + std::to_string(i + 1);
                sbml << "          <multi:productPattern multi:id=\"" << patternId
                     << "\" multi:compartment=\"default\">\n";
                sbml << "            <multi:listOfSpeciesTypeInstances>\n";
                sbml << "              <!-- Pattern: " << escapeXml(rule.getProducts()[i]) << " -->\n";

                // Try to resolve molecule type reference
                for (const auto& mt : model.getMoleculeTypes()) {
                    if (rule.getProducts()[i].find(mt.getName()) != std::string::npos) {
                        sbml << "              <multi:speciesTypeInstance multi:id=\""
                             << patternId << "_sti1\" multi:speciesType=\"st_"
                             << makeValidSBMLId(mt.getName()) << "\"/>\n";
                        break;
                    }
                }

                sbml << "            </multi:listOfSpeciesTypeInstances>\n";
                sbml << "          </multi:productPattern>\n";
            }
            sbml << "        </multi:listOfProductPatterns>\n";
        }

        // Rate law
        const auto& rates = rule.getRates();
        if (!rates.empty()) {
            sbml << "        <multi:kineticLaw>\n";
            sbml << "          <math xmlns=\"http://www.w3.org/1998/Math/MathML\">\n";

            const auto& rateExpr = rates[0];
            if (rateExpr.kind() == ast::ExpressionKind::Number) {
                sbml << "            <cn> " << rateExpr.name() << " </cn>\n";
            } else if (rateExpr.kind() == ast::ExpressionKind::Identifier) {
                sbml << "            <ci> " << escapeXml(rateExpr.name()) << " </ci>\n";
            } else {
                // For complex expressions, emit the top-level name/value
                sbml << "            <ci> " << escapeXml(rateExpr.name()) << " </ci>\n";
            }

            sbml << "          </math>\n";

            // If bidirectional, also emit reverse rate
            if (rule.isBidirectional() && rates.size() > 1) {
                sbml << "          <!-- Reverse rate -->\n";
                sbml << "          <!-- ";
                if (rates[1].kind() == ast::ExpressionKind::Number) {
                    sbml << rates[1].name();
                } else {
                    sbml << escapeXml(rates[1].name());
                }
                sbml << " -->\n";
            }

            sbml << "        </multi:kineticLaw>\n";
        }

        sbml << "      </multi:reactionRule>\n";
    }

    sbml << "    </multi:listOfReactionRules>\n";
    return sbml.str();
}

} // namespace bng::io
