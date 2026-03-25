#include "SbmlWriter.hpp"

#include <sstream>
#include <iomanip>
#include <algorithm>
#include <cctype>

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
    // SBML IDs must start with letter or underscore, contain only alphanumeric + underscore
    std::string result;
    result.reserve(text.size());

    bool firstChar = true;
    for (char c : text) {
        if (std::isalnum(c)) {
            result += c;
            firstChar = false;
        } else if (c == '_') {
            result += '_';
            firstChar = false;
        } else if (!firstChar) {
            result += '_';
        }
    }

    // Ensure starts with letter or underscore
    if (!result.empty() && std::isdigit(result[0])) {
        result = "_" + result;
    }

    return result.empty() ? "species" : result;
}

std::string SbmlWriter::write(const ast::Model& model, const engine::GeneratedNetwork* network) {
    Options options;
    options.level = 2;
    options.version = 4;
    options.networksExport = true;
    return write(model, network, options);
}

std::string SbmlWriter::write(const ast::Model& model, const engine::GeneratedNetwork* network, const Options& options) {
    std::ostringstream sbml;

    // SBML header
    sbml << "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n";
    sbml << "<sbml xmlns=\"http://www.sbml.org/sbml/level" << options.level << "/version" << options.version << "\" "
         << "level=\"" << options.level << "\" version=\"" << options.version << "\">\n";

    sbml << "  <model id=\"" << escapeXml(makeValidSBMLId(model.getModelName())) << "\" name=\""
         << escapeXml(model.getModelName()) << "\">\n";

    // Write sections
    sbml << writeParameters(model);
    sbml << writeCompartments(model);
    sbml << writeSpecies(model, network);

    if (network && options.networksExport) {
        sbml << writeReactions(*network, model);
    }

    sbml << "  </model>\n";
    sbml << "</sbml>\n";

    return sbml.str();
}

std::string SbmlWriter::writeParameters(const ast::Model& model) {
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

std::string SbmlWriter::writeCompartments(const ast::Model& model) {
    std::ostringstream sbml;

    // Default compartment if none specified
    if (model.getCompartments().empty()) {
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

std::string SbmlWriter::writeSpecies(const ast::Model& model, const engine::GeneratedNetwork* network) {
    std::ostringstream sbml;
    sbml << "    <listOfSpecies>\n";

    if (network) {
        // Export generated network species
        for (std::size_t i = 0; i < network->species.size(); ++i) {
            const auto& species = network->species.get(i);
            std::string speciesId = "S" + std::to_string(i + 1);
            std::string compartmentId = species.getCompartment().empty() ? "default" : makeValidSBMLId(species.getCompartment());

            sbml << "      <species id=\"" << speciesId
                 << "\" name=\"" << escapeXml(species.getSpeciesGraph().toString())
                 << "\" compartment=\"" << compartmentId
                 << "\" initialAmount=\"" << species.getAmount()
                 << "\" hasOnlySubstanceUnits=\"false\"";

            if (species.isConstant()) {
                sbml << " constant=\"true\" boundaryCondition=\"true\"";
            } else {
                sbml << " constant=\"false\" boundaryCondition=\"false\"";
            }

            sbml << "/>\n";
        }
    } else {
        // Export seed species
        for (std::size_t i = 0; i < model.getSeedSpecies().size(); ++i) {
            const auto& seed = model.getSeedSpecies()[i];
            std::string speciesId = "S" + std::to_string(i + 1);
            std::string compartmentId = seed.getCompartment().empty() ? "default" : makeValidSBMLId(seed.getCompartment());

            // Evaluate amount
            auto amountValue = seed.getAmount().evaluate([&](const std::string& name) {
                return model.getParameters().evaluate(name);
            }, 0.0);

            sbml << "      <species id=\"" << speciesId
                 << "\" name=\"" << escapeXml(seed.getPattern())
                 << "\" compartment=\"" << compartmentId
                 << "\" initialAmount=\"" << amountValue
                 << "\" hasOnlySubstanceUnits=\"false\"";

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

std::string SbmlWriter::writeReactions(const engine::GeneratedNetwork& network, const ast::Model& model) {
    std::ostringstream sbml;
    sbml << "    <listOfReactions>\n";

    const auto& reactions = network.reactions.all();
    for (std::size_t r = 0; r < reactions.size(); ++r) {
        const auto& rxn = reactions[r];
        std::string reactionId = "R" + std::to_string(r + 1);

        sbml << "      <reaction id=\"" << reactionId << "\" reversible=\"false\">\n";

        // Reactants
        sbml << "        <listOfReactants>\n";
        for (const auto idx : rxn.getReactants()) {
            sbml << "          <speciesReference species=\"S" << (idx + 1) << "\" stoichiometry=\"1\"/>\n";
        }
        sbml << "        </listOfReactants>\n";

        // Products
        sbml << "        <listOfProducts>\n";
        for (const auto idx : rxn.getProducts()) {
            sbml << "          <speciesReference species=\"S" << (idx + 1) << "\" stoichiometry=\"1\"/>\n";
        }
        sbml << "        </listOfProducts>\n";

        // Rate law
        sbml << "        <kineticLaw>\n";
        sbml << "          <math xmlns=\"http://www.w3.org/1998/Math/MathML\">\n";

        // Build rate expression
        // For mass-action: k * S1 * S2 * ... (with stat factor)
        sbml << "            <apply>\n";
        sbml << "              <times/>\n";

        // Rate constant (including stat factor)
        double totalRate = rxn.getFactor();  // stat factor

        // Parse rate law
        try {
            double rateVal = std::stod(rxn.getRateLaw());
            totalRate *= rateVal;
        } catch (...) {
            // If not a number, it's a parameter name
            sbml << "              <ci>" << escapeXml(rxn.getRateLaw()) << "</ci>\n";
        }

        if (totalRate != 1.0) {
            sbml << "              <cn>" << totalRate << "</cn>\n";
        }

        // Reactant concentrations
        for (const auto idx : rxn.getReactants()) {
            sbml << "              <ci>S" << (idx + 1) << "</ci>\n";
        }

        sbml << "            </apply>\n";
        sbml << "          </math>\n";
        sbml << "        </kineticLaw>\n";

        sbml << "      </reaction>\n";
    }

    sbml << "    </listOfReactions>\n";
    return sbml.str();
}

} // namespace bng::io
