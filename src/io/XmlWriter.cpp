#include "XmlWriter.hpp"

#include <sstream>
#include <iomanip>
#include <algorithm>
#include <cctype>
#include <map>
#include <regex>
#include <vector>

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

// Parse a BNGL pattern string into structured molecule/component/bond data
// for XML serialization. This is a lightweight parser that handles:
//   Mol(comp1~state!bond,comp2~state)
//   Mol1(c!1).Mol2(c!1)
//   @Compartment::Mol(c)
namespace {

struct ParsedComponent {
    std::string name;
    std::string state;         // empty if no state
    std::string label;         // empty if no label
    std::vector<int> bonds;    // bond indices, -1 for ?, -2 for +
    std::string compartment;
};

struct ParsedMolecule {
    std::string name;
    std::string compartment;   // per-molecule compartment
    std::string label;
    std::vector<ParsedComponent> components;
};

struct ParsedBond {
    int id;
    int mol1, comp1;  // indices into molecule/component arrays
    int mol2, comp2;
};

struct ParsedPattern {
    std::string compartment;  // species-level compartment
    std::vector<ParsedMolecule> molecules;
    std::vector<ParsedBond> bonds;
};

// Minimal BNGL pattern parser
ParsedPattern parsePattern(const std::string& text) {
    ParsedPattern pattern;
    std::string input = text;

    // Strip leading @Compartment:: prefix
    if (!input.empty() && input[0] == '@') {
        auto colonPos = input.find("::");
        if (colonPos != std::string::npos) {
            pattern.compartment = input.substr(1, colonPos - 1);
            input = input.substr(colonPos + 2);
        }
    }

    // Track bond endpoints: bondId -> list of (molIdx, compIdx)
    std::map<int, std::vector<std::pair<int, int>>> bondEndpoints;

    // Split by '.' at top level (not inside parentheses)
    std::vector<std::string> molStrings;
    int depth = 0;
    std::string current;
    for (char c : input) {
        if (c == '(') depth++;
        else if (c == ')') depth--;

        if (c == '.' && depth == 0) {
            if (!current.empty()) molStrings.push_back(current);
            current.clear();
        } else {
            current += c;
        }
    }
    if (!current.empty()) molStrings.push_back(current);

    for (const auto& molStr : molStrings) {
        ParsedMolecule mol;

        // Check for per-molecule compartment: @Comp before the name
        std::string remaining = molStr;

        // Parse molecule name and optional compartment
        auto parenPos = remaining.find('(');
        std::string namepart = (parenPos != std::string::npos)
            ? remaining.substr(0, parenPos) : remaining;

        // Check for @compartment suffix on molecule name
        auto atPos = namepart.find('@');
        if (atPos != std::string::npos) {
            mol.name = namepart.substr(0, atPos);
            mol.compartment = namepart.substr(atPos + 1);
        } else {
            mol.name = namepart;
        }

        // Strip $ prefix for constant species
        if (!mol.name.empty() && mol.name[0] == '$') {
            mol.name = mol.name.substr(1);
        }

        // Check for %label
        auto pctPos = mol.name.find('%');
        if (pctPos != std::string::npos) {
            mol.label = mol.name.substr(pctPos + 1);
            mol.name = mol.name.substr(0, pctPos);
        }

        // Parse components inside parentheses
        if (parenPos != std::string::npos) {
            auto closePos = remaining.rfind(')');
            if (closePos != std::string::npos && closePos > parenPos) {
                std::string compStr = remaining.substr(parenPos + 1, closePos - parenPos - 1);

                // Split components by ','
                std::vector<std::string> compParts;
                std::string part;
                for (char c : compStr) {
                    if (c == ',') {
                        if (!part.empty()) compParts.push_back(part);
                        part.clear();
                    } else {
                        part += c;
                    }
                }
                if (!part.empty()) compParts.push_back(part);

                for (const auto& cp : compParts) {
                    ParsedComponent comp;

                    std::string rem = cp;
                    // Parse bonds (!) - can have multiple
                    while (true) {
                        auto bangPos = rem.find('!');
                        if (bangPos == std::string::npos) break;

                        std::string beforeBang = rem.substr(0, bangPos);
                        std::string afterBang = rem.substr(bangPos + 1);

                        // Get bond part
                        std::string bondStr;
                        std::size_t bondEnd = 0;
                        for (std::size_t i = 0; i < afterBang.size(); ++i) {
                            if (afterBang[i] == '!' || afterBang[i] == '~' || afterBang[i] == '%') {
                                bondEnd = i;
                                break;
                            }
                            bondEnd = i + 1;
                        }
                        bondStr = afterBang.substr(0, bondEnd);
                        rem = beforeBang + afterBang.substr(bondEnd);

                        if (bondStr == "?") {
                            comp.bonds.push_back(-1);
                        } else if (bondStr == "+") {
                            comp.bonds.push_back(-2);
                        } else {
                            try {
                                comp.bonds.push_back(std::stoi(bondStr));
                            } catch (...) {}
                        }
                    }

                    // Parse state (~)
                    auto tildePos = rem.find('~');
                    if (tildePos != std::string::npos) {
                        comp.name = rem.substr(0, tildePos);

                        // Find end of state (before % if present)
                        std::string stateRem = rem.substr(tildePos + 1);
                        auto pctPos2 = stateRem.find('%');
                        if (pctPos2 != std::string::npos) {
                            comp.state = stateRem.substr(0, pctPos2);
                            comp.label = stateRem.substr(pctPos2 + 1);
                        } else {
                            comp.state = stateRem;
                        }
                    } else {
                        auto pctPos2 = rem.find('%');
                        if (pctPos2 != std::string::npos) {
                            comp.name = rem.substr(0, pctPos2);
                            comp.label = rem.substr(pctPos2 + 1);
                        } else {
                            comp.name = rem;
                        }
                    }

                    // Record bond endpoints
                    int molIdx = static_cast<int>(pattern.molecules.size());
                    int compIdx = static_cast<int>(mol.components.size());
                    for (int bondId : comp.bonds) {
                        if (bondId > 0) {
                            bondEndpoints[bondId].push_back({molIdx, compIdx});
                        }
                    }

                    mol.components.push_back(std::move(comp));
                }
            }
        }

        pattern.molecules.push_back(std::move(mol));
    }

    // Build bond list from endpoints
    for (auto& [bondId, endpoints] : bondEndpoints) {
        if (endpoints.size() == 2) {
            ParsedBond bond;
            bond.id = bondId;
            bond.mol1 = endpoints[0].first;
            bond.comp1 = endpoints[0].second;
            bond.mol2 = endpoints[1].first;
            bond.comp2 = endpoints[1].second;
            pattern.bonds.push_back(bond);
        }
    }

    return pattern;
}

std::string patternToXml(const ParsedPattern& pattern, const std::string& idPrefix,
                         const std::string& indent) {
    std::ostringstream xml;

    // ListOfMolecules
    xml << indent << "<ListOfMolecules>\n";
    for (std::size_t m = 0; m < pattern.molecules.size(); ++m) {
        const auto& mol = pattern.molecules[m];
        std::string molId = idPrefix + "_M" + std::to_string(m + 1);
        xml << indent << "  <Molecule id=\"" << molId << "\" name=\"" << mol.name << "\"";
        if (!mol.compartment.empty()) {
            xml << " compartment=\"" << mol.compartment << "\"";
        }
        if (!mol.label.empty()) {
            xml << " label=\"" << mol.label << "\"";
        }
        xml << ">\n";

        if (!mol.components.empty()) {
            xml << indent << "    <ListOfComponents>\n";
            for (std::size_t c = 0; c < mol.components.size(); ++c) {
                const auto& comp = mol.components[c];
                std::string compId = molId + "_C" + std::to_string(c + 1);
                xml << indent << "      <Component id=\"" << compId
                    << "\" name=\"" << comp.name << "\"";
                if (!comp.state.empty()) {
                    xml << " state=\"" << comp.state << "\"";
                }
                if (!comp.label.empty()) {
                    xml << " label=\"" << comp.label << "\"";
                }
                // numberOfBonds
                int nBonds = 0;
                std::string bondStr;
                for (int b : comp.bonds) {
                    if (b == -1) { bondStr = "?"; break; }
                    else if (b == -2) { bondStr = "+"; break; }
                    else nBonds++;
                }
                if (bondStr.empty()) bondStr = std::to_string(nBonds);
                xml << " numberOfBonds=\"" << bondStr << "\"";
                xml << "/>\n";
            }
            xml << indent << "    </ListOfComponents>\n";
        }

        xml << indent << "  </Molecule>\n";
    }
    xml << indent << "</ListOfMolecules>\n";

    // ListOfBonds
    if (!pattern.bonds.empty()) {
        xml << indent << "<ListOfBonds>\n";
        for (std::size_t b = 0; b < pattern.bonds.size(); ++b) {
            const auto& bond = pattern.bonds[b];
            std::string bondId = idPrefix + "_B" + std::to_string(b + 1);
            std::string site1 = idPrefix + "_M" + std::to_string(bond.mol1 + 1) +
                               "_C" + std::to_string(bond.comp1 + 1);
            std::string site2 = idPrefix + "_M" + std::to_string(bond.mol2 + 1) +
                               "_C" + std::to_string(bond.comp2 + 1);
            xml << indent << "  <Bond id=\"" << bondId
                << "\" site1=\"" << site1 << "\" site2=\"" << site2 << "\"/>\n";
        }
        xml << indent << "</ListOfBonds>\n";
    }

    return xml.str();
}

} // anonymous namespace

std::string XmlWriter::write(const ast::Model& model, const engine::GeneratedNetwork* network) {
    std::ostringstream xml;

    xml << "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n";
    xml << "<!-- Created by BioNetGen C++ -->\n";
    xml << "<sbml xmlns=\"http://www.sbml.org/sbml/level3\" level=\"3\" version=\"1\">\n";
    xml << "  <model id=\"" << escapeXml(model.getModelName()) << "\">\n";

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
            << "\" expr=\"" << escapeXml(param.getExpression().toString()) << "\"/>\n";
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
            xml << "          <ComponentType id=\"" << escapeXml(comp.name) << "\"";
            if (!comp.allowedStates.empty()) {
                xml << ">\n";
                xml << "            <ListOfAllowedStates>\n";
                for (const auto& state : comp.allowedStates) {
                    xml << "              <AllowedState id=\"" << escapeXml(state) << "\"/>\n";
                }
                xml << "            </ListOfAllowedStates>\n";
                xml << "          </ComponentType>\n";
            } else {
                xml << "/>\n";
            }
        }

        xml << "        </ListOfComponentTypes>\n";
        xml << "      </MoleculeType>\n";
    }

    xml << "    </ListOfMoleculeTypes>\n";
    return xml.str();
}

std::string XmlWriter::writeCompartments(const ast::Model& model) {
    std::ostringstream xml;
    if (model.getCompartments().empty()) return {};

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
        for (std::size_t i = 0; i < network->species.size(); ++i) {
            const auto& species = network->species.get(i);
            std::string spId = "S" + std::to_string(i + 1);
            std::string patternStr = species.getSpeciesGraph().toString();

            xml << "      <Species id=\"" << spId
                << "\" concentration=\"" << species.getAmount()
                << "\" name=\"" << escapeXml(patternStr) << "\">\n";

            // Serialize species pattern
            auto parsed = parsePattern(patternStr);
            xml << patternToXml(parsed, spId, "        ");

            xml << "      </Species>\n";
        }
    } else {
        for (std::size_t i = 0; i < model.getSeedSpecies().size(); ++i) {
            const auto& seed = model.getSeedSpecies()[i];
            std::string spId = "S" + std::to_string(i + 1);

            auto amountValue = seed.getAmount().evaluate([&](const std::string& name) {
                return model.getParameters().evaluate(name);
            }, 0.0);

            xml << "      <Species id=\"" << spId
                << "\" concentration=\"" << amountValue
                << "\" name=\"" << escapeXml(seed.getPattern()) << "\">\n";

            auto parsed = parsePattern(seed.getPattern());
            xml << patternToXml(parsed, spId, "        ");

            xml << "      </Species>\n";
        }
    }

    xml << "    </ListOfSpecies>\n";
    return xml.str();
}

std::string XmlWriter::writeReactionRules(const ast::Model& model) {
    std::ostringstream xml;
    xml << "    <ListOfReactionRules>\n";

    for (std::size_t i = 0; i < model.getReactionRules().size(); ++i) {
        const auto& rule = model.getReactionRules()[i];
        std::string rrId = "RR" + std::to_string(i + 1);

        xml << "      <ReactionRule id=\"" << rrId
            << "\" name=\"" << escapeXml(rule.getRuleName()) << "\">\n";

        // Reactant patterns
        xml << "        <ListOfReactantPatterns>\n";
        for (std::size_t r = 0; r < rule.getReactants().size(); ++r) {
            std::string rpId = rrId + "_RP" + std::to_string(r + 1);
            auto parsed = parsePattern(rule.getReactants()[r]);

            xml << "          <ReactantPattern id=\"" << rpId << "\"";
            if (!parsed.compartment.empty()) {
                xml << " compartment=\"" << parsed.compartment << "\"";
            }
            xml << ">\n";
            xml << patternToXml(parsed, rpId, "            ");
            xml << "          </ReactantPattern>\n";
        }
        xml << "        </ListOfReactantPatterns>\n";

        // Product patterns
        xml << "        <ListOfProductPatterns>\n";
        for (std::size_t p = 0; p < rule.getProducts().size(); ++p) {
            std::string ppId = rrId + "_PP" + std::to_string(p + 1);
            auto parsed = parsePattern(rule.getProducts()[p]);

            xml << "          <ProductPattern id=\"" << ppId << "\"";
            if (!parsed.compartment.empty()) {
                xml << " compartment=\"" << parsed.compartment << "\"";
            }
            xml << ">\n";
            xml << patternToXml(parsed, ppId, "            ");
            xml << "          </ProductPattern>\n";
        }
        xml << "        </ListOfProductPatterns>\n";

        // Rate law
        if (!rule.getRates().empty()) {
            xml << "        <RateLaw id=\"" << rrId << "_RateLaw\" type=\"Ele\" totalrate=\"0\">\n";
            xml << "          <ListOfRateConstants>\n";
            xml << "            <RateConstant value=\"" << escapeXml(rule.getRates()[0].toString()) << "\"/>\n";
            xml << "          </ListOfRateConstants>\n";
            xml << "        </RateLaw>\n";
        }

        xml << "      </ReactionRule>\n";

        // If bidirectional, emit reverse rule
        if (rule.isBidirectional() && rule.getRates().size() >= 2) {
            std::string revId = rrId + "r";
            xml << "      <ReactionRule id=\"" << revId
                << "\" name=\"" << escapeXml(rule.getRuleName()) << "(reverse)\">\n";

            // Reverse: products become reactants
            xml << "        <ListOfReactantPatterns>\n";
            for (std::size_t p = 0; p < rule.getProducts().size(); ++p) {
                std::string rpId = revId + "_RP" + std::to_string(p + 1);
                auto parsed = parsePattern(rule.getProducts()[p]);
                xml << "          <ReactantPattern id=\"" << rpId << "\"";
                if (!parsed.compartment.empty()) {
                    xml << " compartment=\"" << parsed.compartment << "\"";
                }
                xml << ">\n";
                xml << patternToXml(parsed, rpId, "            ");
                xml << "          </ReactantPattern>\n";
            }
            xml << "        </ListOfReactantPatterns>\n";

            xml << "        <ListOfProductPatterns>\n";
            for (std::size_t r = 0; r < rule.getReactants().size(); ++r) {
                std::string ppId = revId + "_PP" + std::to_string(r + 1);
                auto parsed = parsePattern(rule.getReactants()[r]);
                xml << "          <ProductPattern id=\"" << ppId << "\"";
                if (!parsed.compartment.empty()) {
                    xml << " compartment=\"" << parsed.compartment << "\"";
                }
                xml << ">\n";
                xml << patternToXml(parsed, ppId, "            ");
                xml << "          </ProductPattern>\n";
            }
            xml << "        </ListOfProductPatterns>\n";

            xml << "        <RateLaw id=\"" << revId << "_RateLaw\" type=\"Ele\" totalrate=\"0\">\n";
            xml << "          <ListOfRateConstants>\n";
            xml << "            <RateConstant value=\"" << escapeXml(rule.getRates()[1].toString()) << "\"/>\n";
            xml << "          </ListOfRateConstants>\n";
            xml << "        </RateLaw>\n";

            xml << "      </ReactionRule>\n";
        }
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

        for (std::size_t p = 0; p < obs.getPatterns().size(); ++p) {
            std::string patId = "O" + std::to_string(i + 1) + "_P" + std::to_string(p + 1);
            auto parsed = parsePattern(obs.getPatterns()[p]);
            xml << "          <Pattern id=\"" << patId << "\"";
            if (!parsed.compartment.empty()) {
                xml << " compartment=\"" << parsed.compartment << "\"";
            }
            xml << ">\n";
            xml << patternToXml(parsed, patId, "            ");
            xml << "          </Pattern>\n";
        }

        xml << "        </ListOfPatterns>\n";
        xml << "      </Observable>\n";
    }

    xml << "    </ListOfObservables>\n";
    return xml.str();
}

std::string XmlWriter::writeFunctions(const ast::Model& model) {
    std::ostringstream xml;
    if (model.getFunctions().empty()) return {};

    xml << "    <ListOfFunctions>\n";

    for (const auto& func : model.getFunctions()) {
        xml << "      <Function id=\"" << escapeXml(func.getName()) << "\"";
        if (!func.getArgs().empty()) {
            xml << " args=\"";
            for (std::size_t a = 0; a < func.getArgs().size(); ++a) {
                if (a > 0) xml << ",";
                xml << escapeXml(func.getArgs()[a]);
            }
            xml << "\"";
        }
        xml << ">\n";
        xml << "        <Expression>" << escapeXml(func.getExpression().toString()) << "</Expression>\n";
        xml << "      </Function>\n";
    }

    xml << "    </ListOfFunctions>\n";
    return xml.str();
}

} // namespace bng::io
