#include "XmlWriter.hpp"

#include <sstream>
#include <iomanip>

namespace bng::io {

std::string XmlWriter::escapeXml(const std::string& text) {
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

std::string XmlWriter::write(const ast::Model& model, const engine::GeneratedNetwork* network) {
    std::ostringstream xml;

    xml << "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n";
    xml << "<sbml xmlns=\"http://www.sbml.org/sbml/level2\" level=\"2\" version=\"1\">\n";
    xml << "  <model id=\"" << escapeXml(model.getModelName()) << "\">\n";

    // Write sections
    xml << writeParameters(model);
    xml << writeMoleculeTypes(model);
    xml << writeCompartments(model);
    xml << writeSpecies(model, network);
    xml << writeReactionRules(model);
    xml << writeObservables(model);
    xml << writeFunctions(model);

    xml << "  </model>\n";
    xml << "</sbml>\n";

    return xml.str();
}

std::string XmlWriter::writeParameters(const ast::Model& model) {
    std::ostringstream xml;
    xml << "    <ListOfParameters>\n";

    for (const auto& param : model.getParameters().all()) {
        xml << "      <Parameter id=\"" << escapeXml(param.getName())
            << "\" type=\"Constant\""
            << " value=\"" << param.getValue()
            << "\" expr=\"" << param.getValue() << "\"/>\n";
    }

    xml << "    </ListOfParameters>\n";
    return xml.str();
}

std::string XmlWriter::writeMoleculeTypes(const ast::Model& model) {
    std::ostringstream xml;
    xml << "    <ListOfMoleculeTypes>\n";

    for (const auto& mt : model.getMoleculeTypes()) {
        xml << "      <MoleculeType id=\"" << escapeXml(mt.getName()) << "\">\n";
        xml << "        <ListOfComponentTypes>\n";

        for (const auto& comp : mt.getComponents()) {
            xml << "          <ComponentType id=\"" << escapeXml(comp.name) << "\">\n";

            // Write states if any
            if (!comp.allowedStates.empty()) {
                xml << "            <ListOfAllowedStates>\n";
                for (const auto& state : comp.allowedStates) {
                    xml << "              <AllowedState id=\"" << escapeXml(state) << "\"/>\n";
                }
                xml << "            </ListOfAllowedStates>\n";
            }

            xml << "          </ComponentType>\n";
        }

        xml << "        </ListOfComponentTypes>\n";
        xml << "      </MoleculeType>\n";
    }

    xml << "    </ListOfMoleculeTypes>\n";
    return xml.str();
}

std::string XmlWriter::writeCompartments(const ast::Model& model) {
    std::ostringstream xml;
    xml << "    <ListOfCompartments>\n";

    for (const auto& comp : model.getCompartments()) {
        xml << "      <Compartment id=\"" << escapeXml(comp.getName())
            << "\" spatialDimensions=\"" << comp.getDimension()
            << "\" size=\"" << comp.getVolume() << "\"";

        if (!comp.getParent().empty()) {
            xml << " outside=\"" << escapeXml(comp.getParent()) << "\"";
        }

        xml << "/>\n";
    }

    xml << "    </ListOfCompartments>\n";
    return xml.str();
}

std::string XmlWriter::writeSpecies(const ast::Model& model, const engine::GeneratedNetwork* network) {
    std::ostringstream xml;
    xml << "    <ListOfSpecies>\n";

    if (network) {
        // Write from generated network
        for (std::size_t i = 0; i < network->species.size(); ++i) {
            const auto& species = network->species.get(i);
            xml << "      <Species id=\"S" << (i + 1)
                << "\" concentration=\"" << species.getAmount()
                << "\" name=\"" << escapeXml(species.getSpeciesGraph().toString()) << "\"/>\n";
        }
    } else {
        // Write seed species
        for (std::size_t i = 0; i < model.getSeedSpecies().size(); ++i) {
            const auto& seed = model.getSeedSpecies()[i];
            // Evaluate amount expression
            auto amountValue = seed.getAmount().evaluate([&](const std::string& name) {
                return model.getParameters().evaluate(name);
            }, 0.0);

            xml << "      <Species id=\"S" << (i + 1)
                << "\" concentration=\"" << amountValue
                << "\" name=\"" << escapeXml(seed.getPattern()) << "\"/>\n";
        }
    }

    xml << "    </ListOfSpecies>\n";
    return xml.str();
}

std::string XmlWriter::writeReactionRules(const ast::Model& model) {
    std::ostringstream xml;
    xml << "    <ListOfReactionRules>\n";

    // Simplified - full implementation would need pattern serialization
    for (std::size_t i = 0; i < model.getReactionRules().size(); ++i) {
        const auto& rule = model.getReactionRules()[i];
        xml << "      <ReactionRule id=\"RR" << (i + 1)
            << "\" name=\"" << escapeXml(rule.getRuleName())
            << "\" symmetry_factor=\"1\">\n";
        xml << "        <!-- Full pattern serialization pending -->\n";
        xml << "      </ReactionRule>\n";
    }

    xml << "    </ListOfReactionRules>\n";
    return xml.str();
}

std::string XmlWriter::writeObservables(const ast::Model& model) {
    std::ostringstream xml;
    xml << "    <ListOfObservables>\n";

    for (std::size_t i = 0; i < model.getObservables().size(); ++i) {
        const auto& obs = model.getObservables()[i];
        xml << "      <Observable id=\"O" << (i + 1)
            << "\" name=\"" << escapeXml(obs.getName())
            << "\" type=\"" << obs.getType() << "\">\n";
        xml << "        <ListOfPatterns>\n";
        // Join patterns
        for (const auto& pattern : obs.getPatterns()) {
            xml << "          <!-- Pattern: " << escapeXml(pattern) << " -->\n";
        }
        xml << "        </ListOfPatterns>\n";
        xml << "      </Observable>\n";
    }

    xml << "    </ListOfObservables>\n";
    return xml.str();
}

std::string XmlWriter::writeFunctions(const ast::Model& model) {
    std::ostringstream xml;
    xml << "    <ListOfFunctions>\n";

    for (const auto& func : model.getFunctions()) {
        xml << "      <Function id=\"" << escapeXml(func.getName()) << "\">\n";
        xml << "        <Expression>" << escapeXml(func.getExpression().toString()) << "</Expression>\n";
        xml << "      </Function>\n";
    }

    xml << "    </ListOfFunctions>\n";
    return xml.str();
}

} // namespace bng::io
