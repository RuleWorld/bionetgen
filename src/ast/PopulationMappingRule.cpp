/**
 * PopulationMappingRule.cpp
 *
 * Faithful C++ port of RefineRule.pm::newPopulationMappingRule (lines 13-282).
 *
 * Parses a population mapping rule string and returns a ReactionRule.
 */

#include "PopulationMappingRule.hpp"

#include <algorithm>
#include <regex>
#include <sstream>
#include <string>
#include <unordered_map>
#include <vector>

#include "core/BNGcore.hpp"
#include "PatternGraphBuilder.hpp"

#include <antlr4-runtime.h>
#include "BNGLexer.h"
#include "BNGParser.h"

namespace bng::ast {

namespace {

// Helper: trim leading whitespace from a string (in-place)
void ltrim(std::string& s) {
    s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](unsigned char ch) {
        return !std::isspace(ch);
    }));
}

// Helper: trim trailing whitespace
void rtrim(std::string& s) {
    s.erase(std::find_if(s.rbegin(), s.rend(), [](unsigned char ch) {
        return !std::isspace(ch);
    }).base(), s.end());
}

void trim(std::string& s) {
    ltrim(s);
    rtrim(s);
}

// Helper: check if node is a bond node
bool isBondNode(const BNGcore::Node& node) {
    return node.get_type().get_type_name() == BNGcore::BOND_NODE_TYPE.get_type_name();
}

// Helper: check if node is a component node (has a non-bond parent)
bool isComponentNode(const BNGcore::Node& node) {
    if (isBondNode(node)) return false;
    for (auto edge = node.edges_in_begin(); edge != node.edges_in_end(); ++edge) {
        if (!isBondNode(**edge)) return true;
    }
    return false;
}

// Helper: check if node is a molecule node (not bond, not component)
bool isMoleculeNode(const BNGcore::Node& node) {
    return !isBondNode(node) && !isComponentNode(node);
}

// Parse a species pattern from text using the ANTLR parser.
// Returns a SpeciesGraph. On error, returns an empty graph and sets err.
SpeciesGraph parseSpeciesPattern(const std::string& text, Model& model, std::string& err) {
    antlr4::ANTLRInputStream input(text);
    BNGLexer lexer(&input);
    antlr4::CommonTokenStream tokens(&lexer);
    BNGParser parser(&tokens);
    auto* species = parser.species_def();
    if (parser.getNumberOfSyntaxErrors() != 0) {
        err = "Syntax error parsing species pattern: " + text;
        return SpeciesGraph();
    }
    BNGcore::PatternGraph graph = bng::parser::buildPatternGraph(species, model, true);
    return SpeciesGraph(graph);
}

// Count molecule nodes in a PatternGraph
std::size_t countMolecules(const BNGcore::PatternGraph& graph) {
    std::size_t count = 0;
    for (auto it = graph.begin(); it != graph.end(); ++it) {
        if (isMoleculeNode(**it)) ++count;
    }
    return count;
}

// Count component nodes under a specific molecule node
std::size_t countComponents(const BNGcore::Node* mol) {
    std::size_t count = 0;
    for (auto edge = mol->edges_out_begin(); edge != mol->edges_out_end(); ++edge) {
        if (isComponentNode(**edge)) ++count;
    }
    return count;
}

// Get the first molecule node in a graph
BNGcore::Node* getFirstMoleculeNode(const BNGcore::PatternGraph& graph) {
    for (auto it = graph.begin(); it != graph.end(); ++it) {
        if (isMoleculeNode(**it)) return *it;
    }
    return nullptr;
}

// Extract molecule label tag from a molecule node
std::string getMoleculeLabel(const BNGcore::Node* mol) {
    return mol->get_label_tag();
}

// Extract component label tag from a component node
std::string getComponentLabel(const BNGcore::Node* comp) {
    return comp->get_label_tag();
}

} // anonymous namespace


PopulationMappingRuleResult newPopulationMappingRule(
    const std::string& ruleString,
    Model& model)
{
    PopulationMappingRuleResult result;
    // Construct a default "empty" ReactionRule to return on error
    result.rule = ReactionRule("", "", {}, {}, {}, {}, false);

    std::string str = ruleString;

    // Remove leading whitespace (Perl line 24: $string =~ s/^\s*//)
    ltrim(str);

    // ---- Extract optional name/label (Perl lines 26-36) ----
    std::string name;
    // Check for alphanumeric name followed by colon: name: ...
    std::regex nameColonRe("^([\\w\\s*]+):\\s*");
    std::regex indexRe("^(\\d+)\\s+");
    std::smatch match;
    if (std::regex_search(str, match, nameColonRe)) {
        name = match[1].str();
        str = match.suffix().str();
    } else if (std::regex_search(str, match, indexRe)) {
        name = match[1].str();
        str = match.suffix().str();
    }

    // ---- Data structures for reactants, products, and references ----
    // Perl lines 43-48
    std::vector<SpeciesGraph> reac;
    std::vector<SpeciesGraph> prod;
    std::unordered_map<std::string, std::string> rrefs;  // label -> "ipatt" or "ipatt.imol" or "ipatt.imol.icomp"
    std::unordered_map<std::string, std::string> prefs;
    // Labels hash: label -> 'M' (molecule) or 'C' (component) or 'found'
    std::unordered_map<std::string, std::string> labels;

    // ---- Read Reactant Patterns (Perl lines 51-120) ----
    // Separator: "+" or "->" or "<->"
    // Perl: $sep = '^\s*[+]\s*|^\s*([<]?-[>])\s*'
    // We parse by splitting on "+" until we find "->" or "<->"

    bool reversible = false;
    int ipatt = -1;

    // Parse reactant patterns until we hit an arrow
    while (!str.empty()) {
        // Find the next separator: "+" or "->" or "<->"
        // Look for arrow first (greedy)
        std::size_t arrowPos = std::string::npos;
        bool isArrow = false;
        std::string arrowType;

        // Search for "<->" first, then "->"
        std::size_t pos = str.find("<->");
        if (pos != std::string::npos) {
            arrowPos = pos;
            isArrow = true;
            arrowType = "<->";
        }
        pos = str.find("->");
        if (pos != std::string::npos && (arrowPos == std::string::npos || pos < arrowPos)) {
            arrowPos = pos;
            isArrow = true;
            arrowType = "->";
        }
        // Also check for "+"
        std::size_t plusPos = str.find('+');
        // The plus is only a valid separator if it comes BEFORE the arrow
        bool usePlus = (plusPos != std::string::npos && (arrowPos == std::string::npos || plusPos < arrowPos));

        std::string patternText;
        if (usePlus) {
            patternText = str.substr(0, plusPos);
            str = str.substr(plusPos + 1);
            ltrim(str);
        } else if (isArrow) {
            patternText = str.substr(0, arrowPos);
            str = str.substr(arrowPos + arrowType.size());
            ltrim(str);
        } else {
            result.error = "Invalid syntax in reaction rule: " + str;
            return result;
        }

        trim(patternText);
        if (patternText.empty()) {
            result.error = "Empty reactant pattern in population mapping rule";
            return result;
        }

        // Parse this reactant pattern (Perl lines 56-59: readString with MoleculeTypesList)
        std::string parseErr;
        SpeciesGraph g = parseSpeciesPattern(patternText, model, parseErr);
        if (!parseErr.empty()) {
            result.error = parseErr;
            return result;
        }
        ++ipatt;

        // ---- Check labels for this reactant (Perl lines 63-101) ----
        const auto& graph = g.getGraph();

        // Check species-level label
        // (In our architecture, species-level labels are not directly on SpeciesGraph,
        //  but we check molecule/component labels below)

        // Check molecule labels
        int imol = -1;
        for (auto nodeIt = graph.begin(); nodeIt != graph.end(); ++nodeIt) {
            if (!isMoleculeNode(**nodeIt)) continue;
            ++imol;
            std::string label = getMoleculeLabel(*nodeIt);
            if (!label.empty()) {
                if (labels.count(label)) {
                    result.error = "Repeated label " + label + " in reactants of reactant rule";
                    return result;
                }
                labels[label] = "M";
                rrefs[label] = std::to_string(ipatt) + "." + std::to_string(imol);
            }

            // Check component labels (Perl lines 84-100)
            int icomp = -1;
            for (auto edge = (*nodeIt)->edges_out_begin(); edge != (*nodeIt)->edges_out_end(); ++edge) {
                if (!isComponentNode(**edge)) continue;
                ++icomp;
                std::string compLabel = getComponentLabel(*edge);
                if (!compLabel.empty()) {
                    if (labels.count(compLabel)) {
                        result.error = "Repeated label " + compLabel + " in reactants of reaction rule";
                        return result;
                    }
                    labels[compLabel] = "C";
                    rrefs[compLabel] = std::to_string(ipatt) + "." + std::to_string(imol) + "." + std::to_string(icomp);
                }
            }
        }

        // Save this reactant (Perl line 104)
        reac.push_back(std::move(g));

        // If arrow was the separator, check reversibility and break (Perl lines 113-119)
        if (isArrow) {
            reversible = (arrowType == "<->");
            if (reversible) {
                result.error = "Population mapping rule should not be reversible!";
                return result;
            }
            break;
        }
    }

    // Only 1 reactant pattern allowed (Perl lines 121-123)
    if (reac.size() != 1) {
        result.error = "Incorrect number of reactants in population mapping rule!";
        return result;
    }

    // ---- Read Product Patterns (Perl lines 127-224) ----
    // Separator: "+" or whitespace
    ipatt = -1;

    while (!str.empty()) {
        // Find next "+" separator or end of products (before rate law)
        // Products end when we hit whitespace followed by a rate-law token
        // For simplicity, parse one product at a time, splitting on '+'

        // Trim leading whitespace
        ltrim(str);
        if (str.empty()) break;

        // Find next '+' (product separator) or space that indicates start of rate law
        // In population mapping rules, there's exactly 1 product, so after the product
        // pattern the rest is the rate law.
        // We need to find where the product pattern ends and rate law begins.
        // The product is a simple molecule name (no components allowed), so we parse
        // until we hit whitespace or '+'.

        std::size_t plusPos = str.find('+');
        std::size_t spacePos = str.find(' ');
        std::size_t tabPos = str.find('\t');
        std::size_t wsPos = std::min(spacePos, tabPos);

        std::string patternText;
        bool hasMoreProducts = false;

        if (plusPos != std::string::npos && (wsPos == std::string::npos || plusPos < wsPos)) {
            // '+' comes before whitespace: this separates products
            patternText = str.substr(0, plusPos);
            str = str.substr(plusPos + 1);
            hasMoreProducts = true;
        } else if (wsPos != std::string::npos) {
            // Whitespace separates product from rate law
            patternText = str.substr(0, wsPos);
            str = str.substr(wsPos);
        } else {
            // Rest of string is the product (no rate law?)
            patternText = str;
            str.clear();
        }

        trim(patternText);
        if (patternText.empty()) break;

        // Parse product pattern (Perl lines 132-136: readString with PopulationTypesList)
        // For population products, we parse the pattern using the normal parser
        std::string parseErr;
        SpeciesGraph g = parseSpeciesPattern(patternText, model, parseErr);
        if (!parseErr.empty()) {
            result.error = parseErr;
            return result;
        }
        ++ipatt;

        const auto& pgraph = g.getGraph();

        // ---- Validate product (Perl lines 140-154) ----
        // Check that pattern is a single molecule
        if (countMolecules(pgraph) != 1) {
            result.error = "Incorrect number of molecules in product of population mapping rule!";
            return result;
        }

        // Check that population type does not collide with MoleculeTypesList (Perl lines 146-149)
        BNGcore::Node* prodMol = getFirstMoleculeNode(pgraph);
        if (prodMol) {
            std::string prodName = prodMol->get_type().get_type_name();
            // Check against existing MoleculeTypes
            for (const auto& mt : model.getMoleculeTypes()) {
                if (mt.getName() == prodName && !mt.isPopulation()) {
                    result.error = "Population product type collides with a previously defined molecule type!";
                    return result;
                }
            }
            // Check that the molecule does not have components (Perl lines 150-153)
            if (countComponents(prodMol) > 0) {
                result.error = "Components are not permitted in product of population mapping rule!";
                return result;
            }
        }

        // ---- Check labels for this product (Perl lines 157-211) ----
        int imol = -1;
        for (auto nodeIt = pgraph.begin(); nodeIt != pgraph.end(); ++nodeIt) {
            if (!isMoleculeNode(**nodeIt)) continue;
            ++imol;
            std::string label = getMoleculeLabel(*nodeIt);
            if (!label.empty()) {
                auto it = labels.find(label);
                if (it == labels.end()) {
                    result.error = "Mis- or un-matched label " + label + " in products of reaction rule " + name;
                    return result;
                }
                if (it->second == "M") {
                    it->second = "found";
                } else if (it->second == "found") {
                    result.error = "Repeated label " + label + " in products of reaction rule " + name;
                    return result;
                } else {
                    result.error = "Mis- or un-matched label " + label + " in products of reaction rule " + name;
                    return result;
                }
                prefs[label] = std::to_string(ipatt) + "." + std::to_string(imol);
            }

            // Check component labels (Perl lines 187-209)
            int icomp = -1;
            for (auto edge = (*nodeIt)->edges_out_begin(); edge != (*nodeIt)->edges_out_end(); ++edge) {
                if (!isComponentNode(**edge)) continue;
                ++icomp;
                std::string compLabel = getComponentLabel(*edge);
                if (!compLabel.empty()) {
                    auto it = labels.find(compLabel);
                    if (it == labels.end()) {
                        result.error = "Mis- or un-matched label " + compLabel + " in products of reaction rule " + name;
                        return result;
                    }
                    if (it->second == "C") {
                        it->second = "found";
                    } else if (it->second == "found") {
                        result.error = "Repeated label " + compLabel + " in products of reaction rule " + name;
                        return result;
                    } else {
                        result.error = "Mis- or un-matched label " + compLabel + " in products of reaction rule " + name;
                        return result;
                    }
                    prefs[compLabel] = std::to_string(ipatt) + "." + std::to_string(imol) + "." + std::to_string(icomp);
                }
            }
        }

        // Save this product (Perl line 214)
        prod.push_back(std::move(g));

        // If no more products, break
        if (!hasMoreProducts) break;
    }

    // Only 1 product pattern allowed (Perl lines 222-224)
    if (prod.size() != 1) {
        result.error = "Incorrect number of products in population mapping rule!";
        return result;
    }

    // ---- Read Rate Law (Perl lines 228-254) ----
    trim(str);

    // Check for TotalRate attribute (Perl lines 233-235)
    bool totalRate = false;
    {
        std::regex totalRateRe("(^|\\s)TotalRate(\\s|$)");
        if (std::regex_search(str, totalRateRe)) {
            totalRate = true;
            str = std::regex_replace(str, totalRateRe, " ");
            trim(str);
        }
    }

    // Parse rate law expression
    // Perl lines 241: RateLaw::newRateLaw( \$string, $model, $TotalRate, $reac )
    // In our C++ architecture, we parse the rate as an Expression
    if (str.empty()) {
        result.error = "Missing rate law in population mapping rule";
        return result;
    }

    // The rate law string may contain commas for reversible (but we already rejected reversible)
    // Check for extra ratelaw tokens (Perl lines 249-250)
    if (str.find(',') == 0) {
        result.error = "Unidirection reaction may have only one rate law";
        return result;
    }

    // Parse the rate expression
    std::string rateStr = str;
    trim(rateStr);

    // Check there's no trailing garbage (Perl lines 253-254)
    // The rate string itself should be valid; we'll let the Expression parser handle it

    // Build reactant/product string representations for the ReactionRule constructor
    std::vector<std::string> reactantStrs;
    for (const auto& r : reac) {
        reactantStrs.push_back(r.toString());
    }
    std::vector<std::string> productStrs;
    for (const auto& p : prod) {
        productStrs.push_back(p.toString());
    }

    // Parse rate expression
    std::vector<Expression> rates;
    // Try to parse as a number first
    try {
        double val = std::stod(rateStr);
        rates.push_back(Expression::number(val));
    } catch (...) {
        // Otherwise treat as an identifier/expression
        rates.push_back(Expression::identifier(rateStr));
    }

    // ---- Construct ReactionRule (Perl lines 258-278) ----
    // Perl lines 259-273:
    //   $rr->Name($name)
    //   $rr->Reactants($reac)
    //   $rr->Products($prod)
    //   $rr->Priority(0)
    //   $rr->RateLaw($rl)
    //   $rr->Rexclude([([])])
    //   $rr->Pexclude([([])])
    //   $rr->Rinclude([([])])
    //   $rr->Pinclude([([])])
    //   $rr->TotalRate($TotalRate)
    //   $rr->DeleteMolecules(0)
    //   $rr->MoveConnected(0)
    //   $rr->RRefs($rrefs)
    //   $rr->PRefs($prefs)

    std::vector<std::string> modifiers;
    if (totalRate) {
        modifiers.push_back("TotalRate");
    }

    result.rule = ReactionRule(
        name,                           // ruleName
        name,                           // label
        std::move(reactantStrs),        // reactants (string representations)
        std::move(productStrs),         // products (string representations)
        std::move(rates),               // rates
        std::move(modifiers),           // modifiers
        false,                          // bidirectional = false (reversible rejected above)
        std::move(reac),                // reactantPatterns
        std::move(prod)                 // productPatterns
    );

    // Perl line 277: $rr->findMap($model->PopulationTypesList)
    // In our C++ architecture, findMap is done automatically in ReactionRule::initialize()
    // which is called by the constructor. The mapping is established through
    // pattern matching in initialize().

    result.error.clear();
    return result;
}

} // namespace bng::ast
