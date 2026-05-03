#include "ReactionRule.hpp"

#include <algorithm>
#include <chrono>
#include <cstdint>
#include <iterator>
#include <limits>
#include <map>
#include <memory>
#include <optional>
#include <queue>
#include <set>
#include <sstream>
#include <stdexcept>
#include <tuple>
#include <unordered_map>
#include <unordered_set>
#include <utility>

#include "core/BNGcore.hpp"
#include "core/Ullmann.hpp"
#include "Model.hpp"

#include <antlr4-runtime.h>
#include "BNGLexer.h"
#include "BNGParser.h"
#include "PatternGraphBuilder.hpp"

namespace bng::ast {

// Global compartment dimension map for cross-compartment species assignment.
// Populated by NetworkGenerator before rule expansion.
static std::unordered_map<std::string, int> g_compartmentDimensions;
// Global compartment parent map: child → parent (e.g., PM→EC, CP→PM)
static std::unordered_map<std::string, std::string> g_compartmentParents;

void setCompartmentDimensions(const std::unordered_map<std::string, int>& dims) {
    g_compartmentDimensions = dims;
}

void setCompartmentParents(const std::unordered_map<std::string, std::string>& parents) {
    g_compartmentParents = parents;
}

// Get the "Outside" compartment for a given compartment.
// For 2D surfaces, Outside is the parent (e.g., PM.Outside = EC).
// For 3D volumes, Outside is the parent (e.g., CP.Outside = PM).
static std::string getOutside(const std::string& comp) {
    auto it = g_compartmentParents.find(comp);
    return it != g_compartmentParents.end() ? it->second : std::string();
}

// Get the "Inside" compartment: the child volume/surface that has this compartment as parent.
// For 2D surfaces, Inside is a 3D volume (e.g., PM.Inside = CP).
// Returns the first such child found.
static std::string getInside(const std::string& comp) {
    for (const auto& [child, parent] : g_compartmentParents) {
        if (parent == comp) return child;
    }
    return {};
}

// Determine transport type between two surface compartments.
// Returns: 1 = endocytosis (flip Outside/Inside), -1 = exocytosis (preserve), 0 = not connected.
// Perl: Compartment::separated_by_volume()
static int separatedByVolume(const std::string& comp1, const std::string& comp2) {
    auto dim1 = g_compartmentDimensions.find(comp1);
    auto dim2 = g_compartmentDimensions.find(comp2);
    if (dim1 == g_compartmentDimensions.end() || dim2 == g_compartmentDimensions.end()) return 0;
    if (dim1->second != 2 || dim2->second != 2) return 0;

    std::string out1 = getOutside(comp1);
    std::string out2 = getOutside(comp2);

    // Exocytosis: both surfaces share the same Outside (adjacent surfaces)
    if (!out1.empty() && !out2.empty() && out1 == out2 &&
        !getInside(comp1).empty() && !getInside(comp2).empty()) {
        return -1;
    }

    // Endocytosis: comp2.Outside.Outside == comp1 (nested)
    if (!out2.empty()) {
        std::string out2_out = getOutside(out2);
        if (!out2_out.empty() && out2_out == comp1 &&
            !getInside(comp2).empty() && !out1.empty()) {
            return 1;
        }
    }

    // Endocytosis reversed: comp1.Outside.Outside == comp2
    if (!out1.empty()) {
        std::string out1_out = getOutside(out1);
        if (!out1_out.empty() && out1_out == comp2 &&
            !getInside(comp1).empty() && !out2.empty()) {
            return 1;
        }
    }

    return 0;
}

// Check if a volume compartment is adjacent to a surface compartment.
// Adjacent means the volume is the immediate Inside or Outside of the surface.
// Perl: Compartment::adjacent()
static bool isAdjacentToSurface(const std::string& volume, const std::string& surface) {
    if (volume.empty() || surface.empty()) return false;
    auto dimV = g_compartmentDimensions.find(volume);
    auto dimS = g_compartmentDimensions.find(surface);
    if (dimV == g_compartmentDimensions.end() || dimS == g_compartmentDimensions.end()) return false;
    if (dimV->second != 3 || dimS->second != 2) return false;
    // Volume is Outside of surface (parent of surface == volume's parent? No, simpler:
    // surface.Outside == volume means volume is the outer compartment)
    std::string outside = getOutside(surface);
    if (outside == volume) return true;
    // Volume is Inside of surface
    std::string inside = getInside(surface);
    if (inside == volume) return true;
    return false;
}

namespace {

struct ComponentInfo {
    BNGcore::Node* node = nullptr;
    std::vector<BNGcore::Node*> bondNodes;  // supports polyvalent (multi-bond) components
    std::string name;
    std::string state;
    std::vector<ReactionRule::ComponentRef> partners;  // one per bond partner
    std::string label;  // component tag label (e.g., "1", "2" from %1, %2)
    // Convenience accessors for single-bond backward compatibility
    BNGcore::Node* bondNode() const { return bondNodes.empty() ? nullptr : bondNodes.front(); }
    bool hasBond() const { return !bondNodes.empty(); }
    bool hasPartner() const { return !partners.empty(); }
};

struct MoleculeInfo {
    BNGcore::Node* node = nullptr;
    std::string name;
    std::vector<ComponentInfo> components;
};

struct PatternInfo {
    std::vector<MoleculeInfo> molecules;
};

struct FlattenedMoleculeRef {
    ReactionRule::ComponentRef base;
    std::string name;
    std::vector<std::string> componentNames;
};

using BondPair = std::pair<ReactionRule::ComponentRef, ReactionRule::ComponentRef>;

bool isBondNode(const BNGcore::Node& node) {
    return node.get_type().get_type_name() == BNGcore::BOND_NODE_TYPE.get_type_name();
}

bool isComponentNode(const BNGcore::Node& node) {
    if (isBondNode(node)) {
        return false;
    }
    for (auto edge = node.edges_in_begin(); edge != node.edges_in_end(); ++edge) {
        if (!isBondNode(**edge)) {
            return true;
        }
    }
    return false;
}

bool isMoleculeNode(const BNGcore::Node& node) {
    return !isBondNode(node) && !isComponentNode(node);
}

bool nodeCompatible(const BNGcore::Node& patternNode, const BNGcore::Node& targetNode) {
    if (!(patternNode.get_type() == targetNode.get_type())) {
        return false;
    }
    return patternNode.get_state() == targetNode.get_state();
}

std::string stateToken(const BNGcore::Node& node) {
    const std::string text = node.get_state().get_BNG2_string();
    return text.empty() ? text : text.substr(1);
}

PatternInfo describePattern(const SpeciesGraph& patternGraph, std::size_t patternIndex) {
    PatternInfo info;
    std::unordered_map<BNGcore::Node*, ReactionRule::ComponentRef> componentRefs;

    for (auto nodeIter = patternGraph.getGraph().begin(); nodeIter != patternGraph.getGraph().end(); ++nodeIter) {
        if (!isMoleculeNode(**nodeIter)) {
            continue;
        }

        MoleculeInfo molecule;
        molecule.node = *nodeIter;
        molecule.name = molecule.node->get_type().get_type_name();

        std::size_t componentIndex = 0;
        for (auto edge = molecule.node->edges_out_begin(); edge != molecule.node->edges_out_end(); ++edge) {
            if (!isComponentNode(**edge)) {
                continue;
            }
            ComponentInfo component;
            component.node = *edge;
            component.name = component.node->get_type().get_type_name();
            component.state = stateToken(*component.node);
            component.label = component.node->get_label_tag();
            for (auto bondEdge = component.node->edges_out_begin(); bondEdge != component.node->edges_out_end(); ++bondEdge) {
                if (isBondNode(**bondEdge)) {
                    component.bondNodes.push_back(*bondEdge);
                }
            }
            molecule.components.push_back(component);
            componentRefs[component.node] = ReactionRule::ComponentRef {patternIndex, info.molecules.size(), componentIndex++};
        }

        info.molecules.push_back(std::move(molecule));
    }

    for (std::size_t moleculeIndex = 0; moleculeIndex < info.molecules.size(); ++moleculeIndex) {
        auto& molecule = info.molecules[moleculeIndex];
        for (std::size_t componentIndex = 0; componentIndex < molecule.components.size(); ++componentIndex) {
            auto& component = molecule.components[componentIndex];
            for (auto* bn : component.bondNodes) {
                if (bn->get_state().get_BNG2_string() != "!+") {
                    continue;
                }
                for (auto inEdge = bn->edges_in_begin(); inEdge != bn->edges_in_end(); ++inEdge) {
                    if (*inEdge == component.node) {
                        continue;
                    }
                    const auto found = componentRefs.find(*inEdge);
                    if (found != componentRefs.end()) {
                        component.partners.push_back(found->second);
                        break;
                    }
                }
            }
        }
    }

    return info;
}

std::vector<PatternInfo> describePatterns(const std::vector<SpeciesGraph>& patterns) {
    std::vector<PatternInfo> infos;
    infos.reserve(patterns.size());
    for (std::size_t i = 0; i < patterns.size(); ++i) {
        infos.push_back(describePattern(patterns[i], i));
    }
    return infos;
}

std::vector<FlattenedMoleculeRef> flattenMolecules(const std::vector<PatternInfo>& infos) {
    std::vector<FlattenedMoleculeRef> refs;
    for (std::size_t patternIndex = 0; patternIndex < infos.size(); ++patternIndex) {
        for (std::size_t moleculeIndex = 0; moleculeIndex < infos[patternIndex].molecules.size(); ++moleculeIndex) {
            FlattenedMoleculeRef ref;
            ref.base = ReactionRule::ComponentRef {patternIndex, moleculeIndex, 0};
            ref.name = infos[patternIndex].molecules[moleculeIndex].name;
            for (const auto& component : infos[patternIndex].molecules[moleculeIndex].components) {
                ref.componentNames.push_back(component.name);
            }
            refs.push_back(std::move(ref));
        }
    }
    return refs;
}

std::string moleculeKey(const FlattenedMoleculeRef& ref) {
    auto names = ref.componentNames;
    std::sort(names.begin(), names.end());
    std::ostringstream out;
    out << ref.name;
    for (const auto& name : names) {
        out << "|" << name;
    }
    return out.str();
}

BondPair canonicalBondPair(ReactionRule::ComponentRef lhs, ReactionRule::ComponentRef rhs) {
    if (rhs < lhs) {
        std::swap(lhs, rhs);
    }
    return {lhs, rhs};
}

std::set<BondPair> collectBonds(const std::vector<PatternInfo>& infos) {
    std::set<BondPair> bonds;
    for (std::size_t patternIndex = 0; patternIndex < infos.size(); ++patternIndex) {
        for (std::size_t moleculeIndex = 0; moleculeIndex < infos[patternIndex].molecules.size(); ++moleculeIndex) {
            const auto& molecule = infos[patternIndex].molecules[moleculeIndex];
            for (std::size_t componentIndex = 0; componentIndex < molecule.components.size(); ++componentIndex) {
                const auto& component = molecule.components[componentIndex];
                if (component.partners.empty()) {
                    continue;
                }
                for (const auto& ptnr : component.partners) {
                    bonds.insert(canonicalBondPair(
                        ReactionRule::ComponentRef {patternIndex, moleculeIndex, componentIndex},
                        ptnr));
                }
            }
        }
    }
    return bonds;
}

const ComponentInfo& getComponentInfo(
    const std::vector<PatternInfo>& infos,
    const ReactionRule::ComponentRef& ref) {
    return infos.at(ref.patternIndex).molecules.at(ref.moleculeIndex).components.at(ref.componentIndex);
}

BNGcore::Node* getComponentNode(const std::vector<PatternInfo>& infos, const ReactionRule::ComponentRef& ref) {
    return getComponentInfo(infos, ref).node;
}

BNGcore::Node* getMoleculeNode(const std::vector<PatternInfo>& infos, std::size_t patternIndex, std::size_t moleculeIndex) {
    return infos.at(patternIndex).molecules.at(moleculeIndex).node;
}

std::string reactionLabel(
    const ReactionRule& rule,
    const std::vector<std::size_t>& reactants,
    const std::vector<std::string>& products) {
    // Sort reactants for label/dedup (actual Rxn preserves BNGL pattern order)
    auto sortedReactants = reactants;
    std::sort(sortedReactants.begin(), sortedReactants.end());
    std::ostringstream out;
    out << rule.getLabel() << "|";
    for (std::size_t i = 0; i < sortedReactants.size(); ++i) {
        if (i != 0) {
            out << ",";
        }
        out << sortedReactants[i];
    }
    out << "->";
    for (std::size_t i = 0; i < products.size(); ++i) {
        if (i != 0) {
            out << ",";
        }
        out << products[i];
    }
    return out.str();
}

// Find an unbound ("!-") marker bond node attached to a component.
// For polyvalent components, this finds the free site marker without touching real bonds.
BNGcore::Node* findAttachedUnboundBondNode(BNGcore::Node* componentNode) {
    if (componentNode == nullptr) return nullptr;
    for (auto edge = componentNode->edges_out_begin(); edge != componentNode->edges_out_end(); ++edge) {
        if (isBondNode(**edge) && (*edge)->get_state().get_BNG2_string() == "!-") {
            return *edge;
        }
    }
    return nullptr;
}

BNGcore::Node* findAttachedBondNode(BNGcore::Node* componentNode) {
    if (componentNode == nullptr) {
        return nullptr;
    }
    for (auto edge = componentNode->edges_out_begin(); edge != componentNode->edges_out_end(); ++edge) {
        if (isBondNode(**edge)) {
            return *edge;
        }
    }
    return nullptr;
}

BNGcore::Node* findSharedBondNode(BNGcore::Node* lhsComponent, BNGcore::Node* rhsComponent) {
    if (lhsComponent == nullptr || rhsComponent == nullptr) {
        return nullptr;
    }
    for (auto lhsEdge = lhsComponent->edges_out_begin(); lhsEdge != lhsComponent->edges_out_end(); ++lhsEdge) {
        if (!isBondNode(**lhsEdge)) {
            continue;
        }
        for (auto rhsEdge = rhsComponent->edges_out_begin(); rhsEdge != rhsComponent->edges_out_end(); ++rhsEdge) {
            if (*lhsEdge == *rhsEdge) {
                return *lhsEdge;
            }
        }
    }
    return nullptr;
}

std::unordered_map<BNGcore::Node*, BNGcore::Node*> cloneProductMolecule(
    const PatternInfo& sourceInfo,
    std::size_t moleculeIndex,
    BNGcore::PatternGraph& target) {
    const auto& sourceMolecule = sourceInfo.molecules.at(moleculeIndex);
    std::unordered_map<BNGcore::Node*, BNGcore::Node*> clones;

    auto* moleculeClone = sourceMolecule.node->clone();
    target.add_node(moleculeClone);
    clones[sourceMolecule.node] = moleculeClone;

    for (const auto& component : sourceMolecule.components) {
        auto* componentClone = component.node->clone();
        target.add_node(componentClone);
        target.add_edge(moleculeClone, componentClone);
        clones[component.node] = componentClone;

        for (auto* bn : component.bondNodes) {
            auto bondFound = clones.find(bn);
            BNGcore::Node* bondClone = nullptr;
            if (bondFound == clones.end()) {
                bondClone = bn->clone();
                target.add_node(bondClone);
                clones[bn] = bondClone;
            } else {
                bondClone = bondFound->second;
            }
            target.add_edge(componentClone, bondClone);
        }
    }
    return clones;
}

std::unordered_map<BNGcore::Node*, BNGcore::Node*> cloneGraphIntoTarget(
    const BNGcore::PatternGraph& source,
    BNGcore::PatternGraph& target) {
    std::unordered_map<BNGcore::Node*, BNGcore::Node*> clones;
    for (auto nodeIter = source.begin(); nodeIter != source.end(); ++nodeIter) {
        auto* clone = (*nodeIter)->clone();
        target.add_node(clone);
        clones[*nodeIter] = clone;
    }
    for (auto nodeIter = source.begin(); nodeIter != source.end(); ++nodeIter) {
        for (auto edge = (*nodeIter)->edges_out_begin(); edge != (*nodeIter)->edges_out_end(); ++edge) {
            const auto srcFound = clones.find(*nodeIter);
            const auto dstFound = clones.find(*edge);
            if (srcFound != clones.end() && dstFound != clones.end()) {
                target.add_edge(srcFound->second, dstFound->second);
            }
        }
    }
    return clones;
}

std::vector<SpeciesGraph> splitIntoSpeciesGraphs(const BNGcore::PatternGraph& graph) {
    std::vector<SpeciesGraph> products;
    std::set<BNGcore::Node*> visited;

    for (auto nodeIter = graph.begin(); nodeIter != graph.end(); ++nodeIter) {
        if (visited.find(*nodeIter) != visited.end()) {
            continue;
        }

        std::vector<BNGcore::Node*> stack {*nodeIter};
        std::vector<BNGcore::Node*> component;
        while (!stack.empty()) {
            BNGcore::Node* node = stack.back();
            stack.pop_back();
            if (!visited.insert(node).second) {
                continue;
            }
            component.push_back(node);
            for (auto edge = node->edges_in_begin(); edge != node->edges_in_end(); ++edge) {
                if (visited.find(*edge) == visited.end()) {
                    stack.push_back(*edge);
                }
            }
            for (auto edge = node->edges_out_begin(); edge != node->edges_out_end(); ++edge) {
                if (visited.find(*edge) == visited.end()) {
                    stack.push_back(*edge);
                }
            }
        }

        BNGcore::PatternGraph componentGraph;
        std::unordered_map<BNGcore::Node*, BNGcore::Node*> clones;
        for (auto* node : component) {
            auto* clone = node->clone();
            componentGraph.add_node(clone);
            clones[node] = clone;
        }
        for (auto* node : component) {
            for (auto edge = node->edges_out_begin(); edge != node->edges_out_end(); ++edge) {
                if (clones.find(*edge) != clones.end()) {
                    componentGraph.add_edge(clones[node], clones[*edge]);
                }
            }
        }
        products.emplace_back(componentGraph);
    }

    return products;
}

void safeDeleteNode(BNGcore::PatternGraph& graph, BNGcore::Node* node) {
    if (node == nullptr) {
        return;
    }

    std::vector<BNGcore::Node*> incoming;
    std::vector<BNGcore::Node*> outgoing;
    for (auto edge = node->edges_in_begin(); edge != node->edges_in_end(); ++edge) {
        incoming.push_back(*edge);
    }
    for (auto edge = node->edges_out_begin(); edge != node->edges_out_end(); ++edge) {
        outgoing.push_back(*edge);
    }

    for (auto* src : incoming) {
        graph.delete_edge(src, node);
    }
    for (auto* dst : outgoing) {
        graph.delete_edge(node, dst);
    }
    graph.delete_node(node);
}

void deleteMappedMolecule(BNGcore::Map& aggregateMap, BNGcore::Node* sourceMoleculeNode) {
    auto* targetMolecule = aggregateMap.mapf(sourceMoleculeNode);
    if (targetMolecule == nullptr) {
        return;
    }

    std::vector<BNGcore::Node*> components;
    for (auto edge = targetMolecule->edges_out_begin(); edge != targetMolecule->edges_out_end(); ++edge) {
        components.push_back(*edge);
    }

    auto* graph = aggregateMap.get_target_graph();
    for (auto* component : components) {
        std::vector<BNGcore::Node*> bondNodes;
        for (auto edge = component->edges_out_begin(); edge != component->edges_out_end(); ++edge) {
            bondNodes.push_back(*edge);
        }
        for (auto* bondNode : bondNodes) {
            safeDeleteNode(*graph, bondNode);
        }
        safeDeleteNode(*graph, component);
    }
    safeDeleteNode(*graph, targetMolecule);
    aggregateMap.erase(sourceMoleculeNode);
}

bool embeddingRespectsNodeStates(const BNGcore::PatternGraph& pattern, const BNGcore::Map& map) {
    for (auto nodeIter = pattern.begin(); nodeIter != pattern.end(); ++nodeIter) {
        auto* target = map.mapf(*nodeIter);
        if (target == nullptr) {
            return false;
        }
        if (!nodeCompatible(**nodeIter, *target)) {
            return false;
        }
        // Per-molecule compartment constraint: if the pattern molecule specifies a
        // compartment (e.g., TF(dna)@NU), the target molecule must be in that
        // compartment. Prevents cross-compartment reactions.
        if (isMoleculeNode(**nodeIter)) {
            const auto& patternComp = (*nodeIter)->get_compartment();
            if (!patternComp.empty()) {
                const auto& targetComp = target->get_compartment();
                if (!targetComp.empty() && patternComp != targetComp) {
                    return false;
                }
            }
        }
    }
    return true;
}

bool patternHasAnyBoundSite(const PatternInfo& info) {
    for (const auto& molecule : info.molecules) {
        for (const auto& component : molecule.components) {
            if (component.hasBond() &&
                component.bondNode()->get_state().get_BNG2_string() == "!+") {
                return true;
            }
        }
    }
    return false;
}

bool hasModifier(const std::vector<std::string>& modifiers, const std::string& name) {
    std::string needle = name;
    std::transform(needle.begin(), needle.end(), needle.begin(), [](unsigned char c) {
        return static_cast<char>(std::tolower(c));
    });
    for (auto modifier : modifiers) {
        std::transform(modifier.begin(), modifier.end(), modifier.begin(), [](unsigned char c) {
            return static_cast<char>(std::tolower(c));
        });
        if (modifier == needle) {
            return true;
        }
    }
    return false;
}

BNGcore::PatternGraph parseObsPattern(const std::string& patternText, Model& model) {
    antlr4::ANTLRInputStream input(patternText);
    BNGLexer lexer(&input);
    antlr4::CommonTokenStream tokens(&lexer);
    BNGParser parser(&tokens);
    auto* species = parser.species_def();
    if (parser.getNumberOfSyntaxErrors() != 0) {
        return BNGcore::PatternGraph();
    }
    return bng::parser::buildPatternGraph(species, model, true);
}

// Compute per-observable constrained match counts for a specific scoped molecule.
// Returns sorted "obsName=count;..." fingerprint that uniquely identifies the
// local function evaluation context. Two molecules with the same fingerprint
// produce the same local function rate value.
std::string computeLocalObsFingerprint(
    const BNGcore::Node* scopedMolecule,
    const BNGcore::PatternGraph& speciesGraph,
    const Model& model,
    const std::string& rateExpr) {
    // Extract function name from rate expression like "rateFlipUp(x)"
    std::string funcName = rateExpr;
    auto parenPos = funcName.find('(');
    if (parenPos != std::string::npos) {
        funcName = funcName.substr(0, parenPos);
    }
    // Find the function definition
    const Function* func = nullptr;
    for (const auto& f : model.getFunctions()) {
        if (f.getName() == funcName) { func = &f; break; }
    }
    if (!func || func->getArgs().empty()) return {};

    // Find observable names referenced in the function body
    std::vector<std::string> obsNames;
    const std::string funcBody = func->getExpression().toString();
    for (const auto& obs : model.getObservables()) {
        if (funcBody.find(obs.getName()) != std::string::npos) {
            obsNames.push_back(obs.getName());
        }
    }
    if (obsNames.empty()) return {};
    std::sort(obsNames.begin(), obsNames.end());

    // For each observable, compute constrained match count:
    // count Ullmann maps where the first molecule node maps to scopedMolecule
    std::ostringstream fp;
    for (const auto& obsName : obsNames) {
        std::size_t count = 0;
        for (const auto& obs : model.getObservables()) {
            if (obs.getName() != obsName) continue;
            for (const auto& patternText : obs.getPatterns()) {
                auto pattern = parseObsPattern(patternText, const_cast<Model&>(model));
                if (pattern.empty()) continue;
                // Find the first molecule node in the observable pattern
                BNGcore::Node* firstPatternMol = nullptr;
                for (auto it = pattern.begin(); it != pattern.end(); ++it) {
                    if (isMoleculeNode(**it)) {
                        firstPatternMol = *it;
                        break;
                    }
                }
                if (!firstPatternMol) continue;

                BNGcore::UllmannSGIso matcher(pattern, speciesGraph);
                BNGcore::List<BNGcore::Map> maps;
                matcher.find_maps(maps);
                for (auto mapIt = maps.begin(); mapIt != maps.end(); ++mapIt) {
                    if (mapIt->mapf(firstPatternMol) == scopedMolecule) {
                        ++count;
                    }
                }
            }
            break;
        }
        fp << obsName << "=" << count << ";";
    }
    return fp.str();
}

std::string canonicalNodeId(const BNGcore::Node* node) {
    // Create a canonical identifier based on node type, state, and adjacency.
    // This collapses symmetric nodes (same type/state/neighbors).
    std::ostringstream out;
    out << node->get_type().get_type_name() << "|" << node->get_state().get_BNG2_string();

    // Add sorted adjacency info for uniqueness
    std::vector<std::string> neighbors;
    for (auto edge = node->edges_in_begin(); edge != node->edges_in_end(); ++edge) {
        neighbors.push_back((*edge)->get_type().get_type_name() + ":" + (*edge)->get_state().get_BNG2_string());
    }
    for (auto edge = node->edges_out_begin(); edge != node->edges_out_end(); ++edge) {
        neighbors.push_back((*edge)->get_type().get_type_name() + ":" + (*edge)->get_state().get_BNG2_string());
    }
    std::sort(neighbors.begin(), neighbors.end());
    for (const auto& n : neighbors) {
        out << "~" << n;
    }
    return out.str();
}

std::string embeddingSignature(const BNGcore::PatternGraph& pattern, const BNGcore::Map& map,
                               const BNGcore::PatternGraph& targetGraph,
                               bool distinguishMolecules = false) {
    // Build a signature that identifies distinct embeddings.
    //
    // When distinguishMolecules is false (default — used for context-only
    // patterns that are not modified), canonicalNodeId collapses automorphic
    // target molecules so all automorphic embeddings collapse into one
    // EmbeddingResult.  This is the correct Perl behavior.
    //
    // When distinguishMolecules is true (used for -> 0 DeleteMolecules where
    // every pattern is reactive), use the target node's pointer address so
    // embeddings mapping to DIFFERENT molecules produce distinct signatures.
    // Embeddings mapping to the SAME set of molecules (symmetric pattern on
    // a complex) still collapse because the sorted pointer set is identical.
    std::vector<std::string> parts;
    for (auto nodeIter = pattern.begin(); nodeIter != pattern.end(); ++nodeIter) {
        auto* target = map.mapf(*nodeIter);
        if (target == nullptr) {
            continue;
        }
        std::ostringstream out;
        out << (*nodeIter)->get_type().get_type_name()
            << "|" << (*nodeIter)->get_state().get_BNG2_string()
            << "->";
        if (distinguishMolecules && isMoleculeNode(**nodeIter)) {
            out << reinterpret_cast<std::uintptr_t>(target);
        } else {
            out << canonicalNodeId(target);
        }
        parts.push_back(out.str());
    }
    std::sort(parts.begin(), parts.end());
    std::ostringstream joined;
    for (const auto& part : parts) {
        joined << part << ";";
    }
    return joined.str();
}

template <typename T>
void sortAndUnique(std::vector<T>& values) {
    std::sort(values.begin(), values.end());
    values.erase(std::unique(values.begin(), values.end()), values.end());
}

std::string reactionCenterSignature(
    const std::vector<PatternInfo>& infos,
    const std::vector<ReactionRule::ReactionCenterRef>& reactionCenter,
    const BNGcore::Map& map,
    const BNGcore::PatternGraph& /*targetGraph*/) {
    // Perl BNG2 convention (filter_identical_by_rxn_center):
    // - Molecule-level reaction center refs (e.g., MolDel) use only the pattern
    //   index as identity — all embeddings of a molecule-level delete look identical,
    //   so only one embedding survives per rule application to a species.
    // - Component-level reaction center refs (e.g., AddBond, DeleteBond) use the
    //   target node pointer to distinguish different physical bonds/sites.
    // Sort the parts so automorphic embeddings (swapping equivalent target
    // molecules/components) collapse to the same signature.
    std::vector<std::string> parts;
    for (const auto& ref : reactionCenter) {
        std::ostringstream out;
        if (ref.isComponent) {
            // Component-level: use target pointer to distinguish different bonds
            BNGcore::Node* sourceNode =
                infos.at(ref.patternIndex).molecules.at(ref.moleculeIndex).components.at(ref.componentIndex).node;
            auto* target = map.mapf(sourceNode);
            if (target == nullptr) continue;
            out << reinterpret_cast<std::uintptr_t>(target);
        } else {
            // Molecule-level: use pattern index only (Perl convention —
            // all embeddings of molecule-level operations look the same)
            out << "P" << ref.patternIndex;
        }
        parts.push_back(out.str());
    }
    std::sort(parts.begin(), parts.end());
    std::ostringstream joined;
    for (const auto& part : parts) {
        joined << part << ";";
    }
    return joined.str();
}

// Check if two compartments are compatible for a bond between them.
// Same compartment, or one is a surface adjacent to the other's volume.
bool bondCompatibleCompartments(const std::string& comp1, const std::string& comp2) {
    if (comp1 == comp2) return true;
    if (comp1.empty() || comp2.empty()) return true;
    auto dim1 = g_compartmentDimensions.find(comp1);
    auto dim2 = g_compartmentDimensions.find(comp2);
    if (dim1 == g_compartmentDimensions.end() || dim2 == g_compartmentDimensions.end()) return true;
    if (dim1->second == 2 && dim2->second == 3) return isAdjacentToSurface(comp2, comp1);
    if (dim1->second == 3 && dim2->second == 2) return isAdjacentToSurface(comp1, comp2);
    return false; // same dimension but different compartment
}

// Verify bond topology of a product graph (Perl's verifyTopology).
bool verifyBondTopology(const BNGcore::PatternGraph& graph) {
    if (g_compartmentDimensions.empty()) return true;
    for (auto nodeIter = graph.begin(); nodeIter != graph.end(); ++nodeIter) {
        if (!isMoleculeNode(**nodeIter)) continue;
        const auto& comp1 = (*nodeIter)->get_compartment();
        if (comp1.empty()) continue;
        for (auto ce = (*nodeIter)->edges_out_begin(); ce != (*nodeIter)->edges_out_end(); ++ce) {
            if (!isComponentNode(**ce)) continue;
            for (auto be = (*ce)->edges_out_begin(); be != (*ce)->edges_out_end(); ++be) {
                if (!isBondNode(**be)) continue;
                if (!((*be)->get_state() == BNGcore::BOUND_STATE)) continue;
                for (auto pce = (*be)->edges_in_begin(); pce != (*be)->edges_in_end(); ++pce) {
                    if (*pce == *ce) continue;
                    for (auto pme = (*pce)->edges_in_begin(); pme != (*pce)->edges_in_end(); ++pme) {
                        if (!isMoleculeNode(**pme)) continue;
                        const auto& comp2 = (*pme)->get_compartment();
                        if (comp2.empty()) continue;
                        if (!bondCompatibleCompartments(comp1, comp2)) return false;
                    }
                }
            }
        }
    }
    return true;
}

} // namespace

bool ReactionRule::ComponentRef::operator==(const ComponentRef& other) const {
    return std::tie(patternIndex, moleculeIndex, componentIndex) ==
           std::tie(other.patternIndex, other.moleculeIndex, other.componentIndex);
}

bool ReactionRule::ComponentRef::operator<(const ComponentRef& other) const {
    return std::tie(patternIndex, moleculeIndex, componentIndex) <
           std::tie(other.patternIndex, other.moleculeIndex, other.componentIndex);
}

ReactionRule::ReactionRule(
    std::string ruleName,
    std::string label,
    std::vector<std::string> reactants,
    std::vector<std::string> products,
    std::vector<Expression> rates,
    std::vector<std::string> modifiers,
    bool bidirectional,
    std::vector<SpeciesGraph> reactantPatterns,
    std::vector<SpeciesGraph> productPatterns)
    : ruleName_(std::move(ruleName)),
      label_(std::move(label)),
      reactants_(std::move(reactants)),
      products_(std::move(products)),
      rates_(std::move(rates)),
      modifiers_(std::move(modifiers)),
      bidirectional_(bidirectional),
      reactantPatterns_(std::move(reactantPatterns)),
      productPatterns_(std::move(productPatterns)) {
    parseReactantFilters();
    initialize();
}

const std::string& ReactionRule::getRuleName() const {
    return ruleName_;
}

const std::string& ReactionRule::getLabel() const {
    return label_;
}

const std::vector<std::string>& ReactionRule::getReactants() const {
    return reactants_;
}

const std::vector<std::string>& ReactionRule::getProducts() const {
    return products_;
}

const std::vector<Expression>& ReactionRule::getRates() const {
    return rates_;
}

const std::vector<std::string>& ReactionRule::getModifiers() const {
    return modifiers_;
}

bool ReactionRule::isBidirectional() const {
    return bidirectional_;
}

const std::vector<SpeciesGraph>& ReactionRule::getReactantPatterns() const {
    return reactantPatterns_;
}

const std::vector<SpeciesGraph>& ReactionRule::getProductPatterns() const {
    return productPatterns_;
}

const std::vector<ReactionRule::TransformOp>& ReactionRule::getOperations() const {
    return operations_;
}

void ReactionRule::initialize() {
    operations_.clear();
    productOnlyStateChanges_.clear();
    hasMoleculeTypeMismatch_ = false;
    reactionCenter_.assign(reactantPatterns_.size(), {});
    patternMatches_.assign(reactantPatterns_.size(), {});
    matchesInitialized_ = true;
    if (reactantPatterns_.empty() || productPatterns_.empty()) {
        return;
    }

    const auto reactantInfo = describePatterns(reactantPatterns_);
    const auto productInfo = describePatterns(productPatterns_);
    const auto reactantMolecules = flattenMolecules(reactantInfo);
    const auto productMolecules = flattenMolecules(productInfo);

    std::map<std::string, std::vector<std::size_t>> unmatchedReactantsByKey;
    for (std::size_t i = 0; i < reactantMolecules.size(); ++i) {
        unmatchedReactantsByKey[moleculeKey(reactantMolecules[i])].push_back(i);
    }

    std::vector<std::optional<std::size_t>> productToReactant(productMolecules.size());
    std::vector<bool> reactantMatched(reactantMolecules.size(), false);

    // First pass: match by exact key (molecule name + sorted component names)
    for (std::size_t i = 0; i < productMolecules.size(); ++i) {
        auto& bucket = unmatchedReactantsByKey[moleculeKey(productMolecules[i])];
        if (!bucket.empty()) {
            const std::size_t matchedIndex = bucket.front();
            bucket.erase(bucket.begin());
            productToReactant[i] = matchedIndex;
            reactantMatched[matchedIndex] = true;
        }
    }

    // NOTE: Perl BNG's findMaps uses labels that include all component names.
    // If reactant and product molecules of the same type have different component
    // sets, they do NOT match. The reactant is deleted and the product is added
    // fresh. We do NOT do a second pass matching by type name alone, as that
    // would incorrectly map molecules with differing component sets.

    // Detect molecule replacement: when there are BOTH unmatched reactant molecules
    // (deleted) AND unmatched product molecules (added), the rule effectively
    // replaces one molecule type with another.  Bonded partners of the deleted
    // molecule become orphan fragments that should NOT survive as products (the
    // new molecule does not inherit the old one's bonds).  This covers:
    //  - Same type name but different component sets (e.g., GPCR(b,loc) -> GPCR(l,b,loc,s))
    //  - Different type names entirely (e.g., FGFR(s~P) -> Internalized_Rec())
    //
    // When there are only unmatched reactant molecules and no unmatched products
    // (e.g., RevErb(b) + Bmal1() -> RevErb(b) + 0), the deletion is explicit
    // and orphan fragments should be KEPT as products (Perl behavior).
    hasMoleculeTypeMismatch_ = false;
    if (!hasModifier(modifiers_, "deletemolecules")) {
        bool hasUnmatchedReactant = false;
        bool hasUnmatchedProduct = false;
        for (std::size_t i = 0; i < reactantMolecules.size(); ++i) {
            if (!reactantMatched[i]) {
                hasUnmatchedReactant = true;
                break;
            }
        }
        for (std::size_t i = 0; i < productMolecules.size(); ++i) {
            if (!productToReactant[i].has_value()) {
                hasUnmatchedProduct = true;
                break;
            }
        }
        hasMoleculeTypeMismatch_ = hasUnmatchedReactant && hasUnmatchedProduct;
    }

    // Build per-molecule compartment transport table: for each product molecule that
    // has an explicit compartment annotation AND maps to a reactant molecule, record
    // the mapping so buildReaction can apply it.  This handles bond-forming rules like
    // R(l)@PM + L(r)@EC <-> R(l!1)@PM.L(r!1)@PM where L moves from EC to PM.
    moleculeCompartmentTransports_.clear();
    for (std::size_t i = 0; i < productMolecules.size(); ++i) {
        if (!productToReactant[i].has_value()) continue;
        const auto& pRef = productMolecules[i].base;
        const auto& pMol = productInfo[pRef.patternIndex].molecules[pRef.moleculeIndex];
        const std::string pComp = pMol.node->get_compartment();
        if (pComp.empty()) continue;
        const auto& rRef = reactantMolecules[*productToReactant[i]].base;
        const auto& rMol = reactantInfo[rRef.patternIndex].molecules[rRef.moleculeIndex];
        const std::string rComp = rMol.node->get_compartment();
        // Only record if compartments differ (actual transport) or if the reactant
        // molecule has no compartment but the product does (assignment).
        if (rComp != pComp) {
            moleculeCompartmentTransports_.push_back(MoleculeCompartmentTransport{
                pRef.patternIndex, pRef.moleculeIndex,
                pComp,
                rRef.patternIndex, rRef.moleculeIndex});
        }
    }

    // Build global tag-to-reactant-component mapping across ALL reactant patterns
    std::unordered_map<std::string, ComponentRef> globalReactantComponentsByTag;
    for (std::size_t pi = 0; pi < reactantInfo.size(); ++pi) {
        for (std::size_t mi = 0; mi < reactantInfo[pi].molecules.size(); ++mi) {
            for (std::size_t ci = 0; ci < reactantInfo[pi].molecules[mi].components.size(); ++ci) {
                const auto& comp = reactantInfo[pi].molecules[mi].components[ci];
                if (!comp.label.empty()) {
                    globalReactantComponentsByTag[comp.label] = ComponentRef{pi, mi, ci};
                }
            }
        }
    }

    // Build tag mapping for ALL product components (including in new molecules)
    for (std::size_t pi = 0; pi < productInfo.size(); ++pi) {
        for (std::size_t mi = 0; mi < productInfo[pi].molecules.size(); ++mi) {
            for (std::size_t ci = 0; ci < productInfo[pi].molecules[mi].components.size(); ++ci) {
                const auto& comp = productInfo[pi].molecules[mi].components[ci];
                if (!comp.label.empty()) {
                    auto tagIt = globalReactantComponentsByTag.find(comp.label);
                    if (tagIt != globalReactantComponentsByTag.end()) {
                        tagComponentMap_[ComponentRef{pi, mi, ci}] = tagIt->second;
                    }
                }
            }
        }
    }

    std::map<ComponentRef, ComponentRef> productToReactantComponent;
    for (std::size_t productFlatIndex = 0; productFlatIndex < productMolecules.size(); ++productFlatIndex) {
        if (!productToReactant[productFlatIndex].has_value()) {
            continue;
        }

        const auto& productMolRef = productMolecules[productFlatIndex];
        const auto& reactantMolRef = reactantMolecules[*productToReactant[productFlatIndex]];
        const auto& productMol = productInfo[productMolRef.base.patternIndex].molecules[productMolRef.base.moleculeIndex];
        const auto& reactantMol = reactantInfo[reactantMolRef.base.patternIndex].molecules[reactantMolRef.base.moleculeIndex];

        std::map<std::string, std::vector<std::size_t>> reactantComponentsByName;
        for (std::size_t i = 0; i < reactantMol.components.size(); ++i) {
            reactantComponentsByName[reactantMol.components[i].name].push_back(i);
        }

        for (std::size_t i = 0; i < productMol.components.size(); ++i) {
            const auto& productComponent = productMol.components[i];
            ComponentRef productRef {productMolRef.base.patternIndex, productMolRef.base.moleculeIndex, i};

            // Check if product component has a tag label
            bool matchedByTag = false;
            ComponentRef reactantRef;
            if (!productComponent.label.empty()) {
                auto tagIt = globalReactantComponentsByTag.find(productComponent.label);
                if (tagIt != globalReactantComponentsByTag.end()) {
                    reactantRef = tagIt->second;
                    matchedByTag = true;
                    // Remove from name bucket to avoid double-matching
                    auto& bucket = reactantComponentsByName[productComponent.name];
                    // Find and remove the matched component from bucket if in same molecule
                    if (reactantRef.patternIndex == reactantMolRef.base.patternIndex &&
                        reactantRef.moleculeIndex == reactantMolRef.base.moleculeIndex) {
                        auto it = std::find(bucket.begin(), bucket.end(), reactantRef.componentIndex);
                        if (it != bucket.end()) bucket.erase(it);
                    }
                }
            }

            if (!matchedByTag) {
                auto& bucket = reactantComponentsByName[productComponent.name];
                if (bucket.empty()) {
                    // Product introduces a component not in the reactant pattern.
                    // Record a product-only state change to apply during buildReaction.
                    // Check if the product component has an actual bound bond (not just
                    // an unbound "!-" marker).  An unbound marker means "free" — any
                    // existing bond on the species component should be broken.
                    bool hasRealBound = false;
                    for (auto* bn : productComponent.bondNodes) {
                        const std::string bst = bn->get_state().get_BNG2_string();
                        if (bst != "!-") {
                            hasRealBound = true;
                            break;
                        }
                    }
                    if (!productComponent.state.empty() || !hasRealBound) {
                        ProductOnlyStateChange posc;
                        posc.reactantPatternIndex = reactantMolRef.base.patternIndex;
                        posc.reactantMoleculeIndex = reactantMolRef.base.moleculeIndex;
                        posc.componentName = productComponent.name;
                        posc.newState = productComponent.state;  // may be empty
                        posc.breakBond = !hasRealBound;  // product has no real bond → break existing
                        productOnlyStateChanges_.push_back(posc);
                    }
                    continue;
                }
                const std::size_t reactantComponentIndex = bucket.front();
                bucket.erase(bucket.begin());
                reactantRef = ComponentRef{reactantMolRef.base.patternIndex, reactantMolRef.base.moleculeIndex, reactantComponentIndex};
            }

            productToReactantComponent[productRef] = reactantRef;

            // Determine reactant component state
            const auto& reactantComponent = reactantInfo[reactantRef.patternIndex].molecules[reactantRef.moleculeIndex].components[reactantRef.componentIndex];

            if (!productComponent.state.empty() && !reactantComponent.state.empty() &&
                productComponent.state != reactantComponent.state) {
                operations_.push_back(TransformOp {
                    TransformOp::Type::ChangeState,
                    reactantRef,
                    {},
                    0,
                    0,
                    productComponent.state,
                });
                reactionCenter_[reactantRef.patternIndex].push_back(
                    ReactionCenterRef {reactantRef.patternIndex, reactantRef.moleculeIndex, true, reactantRef.componentIndex});
            }
        }
    }

    const auto reactantBonds = collectBonds(reactantInfo);
    std::set<BondPair> remappedProductBonds;
    crossBonds_.clear();
    newMoleculeBonds_.clear();
    for (const auto& productBond : collectBonds(productInfo)) {
        const auto lhs = productToReactantComponent.find(productBond.first);
        const auto rhs = productToReactantComponent.find(productBond.second);
        if (lhs != productToReactantComponent.end() && rhs != productToReactantComponent.end()) {
            remappedProductBonds.insert(canonicalBondPair(lhs->second, rhs->second));
        } else if (lhs != productToReactantComponent.end() && rhs == productToReactantComponent.end()) {
            // Bond from matched reactant component to new product molecule component
            crossBonds_.push_back(CrossBondRef{lhs->second, productBond.second});
        } else if (lhs == productToReactantComponent.end() && rhs != productToReactantComponent.end()) {
            // Bond from new product molecule component to matched reactant component
            crossBonds_.push_back(CrossBondRef{rhs->second, productBond.first});
        } else {
            // Both sides unmapped: bond between two newly-added molecules.
            // Only record inter-molecule bonds (different molecules); intra-molecule
            // bonds within the same new molecule are handled by cloneProductMolecule.
            if (productBond.first.patternIndex != productBond.second.patternIndex ||
                productBond.first.moleculeIndex != productBond.second.moleculeIndex) {
                newMoleculeBonds_.push_back(NewMoleculeBondRef{productBond.first, productBond.second});
            }
        }
    }

    for (const auto& productBond : remappedProductBonds) {
        if (reactantBonds.find(productBond) == reactantBonds.end()) {
            operations_.push_back(TransformOp {
                TransformOp::Type::AddBond,
                productBond.first,
                productBond.second,
                0,
                0,
                {},
            });
            reactionCenter_[productBond.first.patternIndex].push_back(
                ReactionCenterRef {productBond.first.patternIndex, productBond.first.moleculeIndex, true, productBond.first.componentIndex});
            reactionCenter_[productBond.second.patternIndex].push_back(
                ReactionCenterRef {productBond.second.patternIndex, productBond.second.moleculeIndex, true, productBond.second.componentIndex});
        }
    }
    for (const auto& reactantBond : reactantBonds) {
        if (remappedProductBonds.find(reactantBond) == remappedProductBonds.end()) {
            operations_.push_back(TransformOp {
                TransformOp::Type::DeleteBond,
                reactantBond.first,
                reactantBond.second,
                0,
                0,
                {},
            });
            reactionCenter_[reactantBond.first.patternIndex].push_back(
                ReactionCenterRef {reactantBond.first.patternIndex, reactantBond.first.moleculeIndex, true, reactantBond.first.componentIndex});
            reactionCenter_[reactantBond.second.patternIndex].push_back(
                ReactionCenterRef {reactantBond.second.patternIndex, reactantBond.second.moleculeIndex, true, reactantBond.second.componentIndex});
        }
    }

    for (std::size_t i = 0; i < reactantMolecules.size(); ++i) {
        if (!reactantMatched[i]) {
            operations_.push_back(TransformOp {
                TransformOp::Type::DeleteMolecule,
                {},
                {},
                reactantMolecules[i].base.moleculeIndex,
                reactantMolecules[i].base.patternIndex,
                {},
            });
            reactionCenter_[reactantMolecules[i].base.patternIndex].push_back(
                ReactionCenterRef {reactantMolecules[i].base.patternIndex, reactantMolecules[i].base.moleculeIndex, false, 0});
        }
    }
    for (std::size_t i = 0; i < productMolecules.size(); ++i) {
        if (!productToReactant[i].has_value()) {
            operations_.push_back(TransformOp {
                TransformOp::Type::AddMolecule,
                {},
                {},
                productMolecules[i].base.moleculeIndex,
                productMolecules[i].base.patternIndex,
                {},
            });
        }
    }

    for (auto& refs : reactionCenter_) {
        std::sort(refs.begin(), refs.end(), [](const ReactionCenterRef& lhs, const ReactionCenterRef& rhs) {
            return std::tie(lhs.patternIndex, lhs.moleculeIndex, lhs.isComponent, lhs.componentIndex) <
                   std::tie(rhs.patternIndex, rhs.moleculeIndex, rhs.isComponent, rhs.componentIndex);
        });
        refs.erase(std::unique(refs.begin(), refs.end(), [](const ReactionCenterRef& lhs, const ReactionCenterRef& rhs) {
            return std::tie(lhs.patternIndex, lhs.moleculeIndex, lhs.isComponent, lhs.componentIndex) ==
                   std::tie(rhs.patternIndex, rhs.moleculeIndex, rhs.isComponent, rhs.componentIndex);
        }), refs.end());
    }
}

std::vector<ReactionRule::EmbeddingResult> ReactionRule::findEmbeddings(
    std::size_t patternIndex,
    const SpeciesList& speciesList) const {
    std::vector<std::size_t> allSpecies;
    for (std::size_t speciesIndex = 0; speciesIndex < speciesList.size(); ++speciesIndex) {
        allSpecies.push_back(speciesIndex);
    }
    return findEmbeddingsForSpecies(patternIndex, speciesList, allSpecies);
}

std::vector<ReactionRule::EmbeddingResult> ReactionRule::findEmbeddingsForSpecies(
    std::size_t patternIndex,
    const SpeciesList& speciesList,
    const std::vector<std::size_t>& candidateSpecies,
    const Model* model) const {
    std::vector<EmbeddingResult> results;
    const auto& pattern = reactantPatterns_.at(patternIndex).getGraph();
    const auto reactantInfo = describePatterns(reactantPatterns_);
    std::unordered_set<std::string> seen;
    const bool debug = std::getenv("BNG_DEBUG_EMBEDDINGS") != nullptr;

    // Build set of species already in cache to avoid re-searching them
    std::unordered_set<std::size_t> alreadySearchedSpecies;
    if (patternIndex < patternMatches_.size()) {
        for (const auto& existing : patternMatches_[patternIndex]) {
            alreadySearchedSpecies.insert(existing.speciesIndex);
        }
    }

    // Pre-compute pattern molecule type requirements for fast pre-filtering
    std::unordered_map<std::string, std::size_t> patternMoleculeTypes;
    for (auto nodeIter = pattern.begin(); nodeIter != pattern.end(); ++nodeIter) {
        if (isMoleculeNode(**nodeIter)) {
            patternMoleculeTypes[(*nodeIter)->get_type().get_type_name()]++;
        }
    }

    for (std::size_t speciesIndex : candidateSpecies) {
        // Skip species that have already been searched in previous iterations
        if (alreadySearchedSpecies.count(speciesIndex) > 0) {
            continue;
        }
        // Apply include/exclude reactant filters
        if (!passesReactantFilters(patternIndex, speciesList.get(speciesIndex).getSpeciesGraph())) {
            continue;
        }
        // Species-level compartment check: if reactant pattern has a species-level
        // PREFIX compartment (e.g., @PM:R.R or @EM:R with colon syntax), only match
        // species with the same species-level compartment. Suffix annotations like
        // TF(d~pY)@CP are per-molecule and handled by embeddingRespectsNodeStates.
        if (reactantPatterns_.at(patternIndex).isCompartmentPrefix()) {
            const auto& patternComp = reactantPatterns_.at(patternIndex).getCompartment();
            if (!patternComp.empty() &&
                speciesList.get(speciesIndex).getCompartment() != patternComp) {
                continue;
            }
        }

        // Quick molecule composition pre-filter: skip species that don't contain
        // enough molecules of the required types (avoids expensive Ullmann call)
        const auto& targetGraph = speciesList.get(speciesIndex).getSpeciesGraph().getGraph();
        if (!patternMoleculeTypes.empty() && !targetGraph.empty()) {
            std::unordered_map<std::string, std::size_t> targetMolTypes;
            for (auto tIter = targetGraph.begin(); tIter != targetGraph.end(); ++tIter) {
                if (isMoleculeNode(**tIter)) {
                    targetMolTypes[(*tIter)->get_type().get_type_name()]++;
                }
            }
            bool compatible = true;
            for (const auto& [molType, needed] : patternMoleculeTypes) {
                auto it = targetMolTypes.find(molType);
                if (it == targetMolTypes.end() || it->second < needed) {
                    compatible = false;
                    break;
                }
            }
            if (!compatible) continue;
        }

        BNGcore::UllmannSGIso matcher(pattern, targetGraph);
        BNGcore::List<BNGcore::Map> maps;
        matcher.find_maps(maps);
        std::size_t mapsBeforeDedup = 0;
        {
            auto countIter = maps.begin();
            while (countIter != maps.end()) {
                ++mapsBeforeDedup;
                ++countIter;
            }
        }

        // For species-level transport rules (prefix compartment like @EM:R → @PM:R),
        // Perl treats the rule as applying to the entire species, not individual molecules.
        // Only keep one embedding per species (first valid one).
        // For pure degradation rules (-> 0 without DeleteMolecules), all embeddings of
        // a pattern into the same species produce the same reaction (species → trash),
        // so only one embedding per species is needed. Perl generates one reaction.
        const bool speciesLevelTransport = reactantPatterns_.at(patternIndex).isCompartmentPrefix();
        const bool pureDegradation = operations_.empty() && productPatterns_.empty()
            && !reactantPatterns_.empty() && !hasModifier(modifiers_, "deletemolecules");
        bool foundEmbeddingForSpecies = false;

        // Track signature → index in results for multiplicity counting
        std::unordered_map<std::string, std::size_t> signatureToResultIndex;
        for (auto mapIter = maps.begin(); mapIter != maps.end(); ++mapIter) {
            if ((speciesLevelTransport || pureDegradation) && foundEmbeddingForSpecies) break;
            if (!embeddingRespectsNodeStates(pattern, *mapIter)) {
                continue;
            }
            // When the reaction center for this pattern is empty (no transformations
            // on the matched molecule), all embeddings into the same species produce
            // the same reaction — use only the species index as signature.
            // Exception: -> 0 DeleteMolecules rules have empty reaction centers
            // (initialize() returned early) but different embeddings may delete
            // different molecules, producing distinct reactions. Use pointer-based
            // embeddingSignature to distinguish them.
            // When the reaction center is non-empty, use the reaction center signature
            // to distinguish embeddings with different reactive sites.
            const bool isDeleteMoleculesDegradation = operations_.empty()
                && productPatterns_.empty() && !reactantPatterns_.empty()
                && hasModifier(modifiers_, "deletemolecules");
            auto sigBase = std::to_string(speciesIndex) + "|" +
                (reactionCenter_.at(patternIndex).empty()
                    ? (isDeleteMoleculesDegradation
                        ? embeddingSignature(pattern, *mapIter, targetGraph, /*distinguishMolecules=*/true)
                        : std::string())
                    : reactionCenterSignature(reactantInfo, reactionCenter_.at(patternIndex), *mapIter, targetGraph));
            // For rules with local function rates (scope-prefix %x:: or tag-label %x
            // with energy patterns), distinguish embeddings by observable counts for
            // the scoped molecule. Different observable counts → different rate values.
            const bool hasLocalFuncRate = model != nullptr && !rates_.empty() &&
                rates_.front().toString().find('(') != std::string::npos;
            if (hasLocalFuncRate) {
                const std::string rateName = rates_.front().toString();
                for (auto pn = pattern.begin(); pn != pattern.end(); ++pn) {
                    if (isMoleculeNode(**pn)) {
                        auto* target = mapIter->mapf(*pn);
                        if (target) {
                            std::string fp = computeLocalObsFingerprint(
                                target, targetGraph, *model, rateName);
                            if (!fp.empty()) {
                                sigBase += "|obs:" + fp;
                            }
                        }
                        break;
                    }
                }
            }
            const auto signature = sigBase;
            if (!seen.insert(signature).second) {
                // Duplicate signature — increment multiplicity of the existing embedding
                auto it = signatureToResultIndex.find(signature);
                if (it != signatureToResultIndex.end() && it->second < results.size()) {
                    results[it->second].multiplicity++;
                }
                if (debug) {
                    std::cerr << "[DEBUG] Rule " << ruleName_ << " pattern " << patternIndex
                              << " species " << speciesIndex << ": DUPLICATE signature " << signature << "\n";
                }
                continue;
            }
            signatureToResultIndex[signature] = results.size();
            results.push_back(EmbeddingResult {speciesIndex, *mapIter, 1});
            if (speciesLevelTransport || pureDegradation) foundEmbeddingForSpecies = true;
        }
        if (debug && mapsBeforeDedup > 0) {
            std::cerr << "[DEBUG] Rule " << ruleName_ << " pattern " << patternIndex
                      << " species " << speciesIndex << ": " << mapsBeforeDedup << " Ullmann maps → "
                      << results.size() << " unique embeddings\n";
        }
    }

    return results;
}

void ReactionRule::clearPatternMatchCache() const {
    patternMatches_.assign(reactantPatterns_.size(), {});
    matchesInitialized_ = false;
    lastSpeciesListCapacity_ = 0;
    // Clear iteration tracking only on full cache reset (between generate_network calls),
    // not on vector reallocation within a single generation.
    lastProcessedInIteration_.clear();
    // Reset synthesis flag so zero-order rules fire again on re-generation
    synthesisApplied_ = false;
    // Recursively clear the reverse rule's cache so that bidirectional rules
    // re-discover embeddings on the next generate_network call.
    if (reverseRule_) {
        reverseRule_->clearPatternMatchCache();
    }
}

std::size_t ReactionRule::expandRule(
    SpeciesList& speciesList,
    RxnList& rxnList,
    std::size_t currentIteration,
    const std::function<bool(const SpeciesGraph&)>& productFilter,
    std::size_t speciesBoundary,
    const Model* model) const {

    // --- Reverse rule delegation ---
    if (bidirectional_ && !rates_.empty()) {
        if (!reverseRule_) {
            // Transform modifiers for reverse rule (Perl convention):
            // Forward's exclude_reactants → Reverse's exclude_products
            // Forward's include_reactants → Reverse's include_products
            // Forward's exclude_products → Reverse's exclude_reactants
            // Forward's include_products → Reverse's include_reactants
            std::vector<std::string> reverseModifiers;
            for (const auto& mod : modifiers_) {
                std::string transformed = mod;
                if (mod.find("exclude_reactants") == 0) {
                    transformed.replace(0, 17, "exclude_products");
                } else if (mod.find("include_reactants") == 0) {
                    transformed.replace(0, 17, "include_products");
                } else if (mod.find("exclude_products") == 0) {
                    transformed.replace(0, 16, "exclude_reactants");
                } else if (mod.find("include_products") == 0) {
                    transformed.replace(0, 16, "include_reactants");
                }
                reverseModifiers.push_back(std::move(transformed));
            }
            reverseRule_ = std::make_unique<ReactionRule>(
                std::string("_reverse__") + ruleName_,
                label_.empty() ? std::string("_reverse") : std::string("_reverse__") + label_,
                products_,
                reactants_,
                std::vector<Expression>{rates_.size() > 1 ? rates_[1] : rates_.front()},
                std::move(reverseModifiers),
                false,
                productPatterns_,
                reactantPatterns_);
            reverseRule_->setHasScopePrefix(hasScopePrefix_);
        }
        reverseRule_->expandRule(speciesList, rxnList, currentIteration, productFilter, speciesBoundary, model);
    }

    if (reactantPatterns_.empty()) {
        // Synthesis rule (0th order): no reactants, just create products.
        // Fire once to add product species and the synthesis reaction.
        if (!synthesisApplied_) {
            synthesisApplied_ = true;
            // Build product species from product patterns
            std::vector<std::size_t> productIndices;
            std::vector<std::string> productLabels;
            for (const auto& productPattern : productPatterns_) {
                BNGcore::PatternGraph productGraph;
                auto clones = cloneGraphIntoTarget(productPattern.getGraph(), productGraph);
                SpeciesGraph sg(productGraph, productPattern.getCompartment());
                if (productFilter && !productFilter(sg)) {
                    return 0;
                }
                productLabels.push_back(sg.canonicalLabel());
                const auto [index, isNew] = speciesList.add(Species(sg, 0.0, false, productPattern.getCompartment()));
                productIndices.push_back(index);
            }
            std::sort(productLabels.begin(), productLabels.end());
            std::sort(productIndices.begin(), productIndices.end());
            const std::string label = ruleName_ + "|synth->" + [&]{
                std::string s;
                for (const auto& l : productLabels) s += l + ";";
                return s;
            }();
            rxnList.add(Rxn(
                label,
                {},  // empty reactants
                productIndices,
                rates_.empty() ? "0" : rates_.front().toString(),
                1.0,
                ruleName_,
                rates_.empty() ? std::nullopt : std::optional<Expression>(rates_.front())));
            return 1;
        }
        return 0;
    }

    // --- Cache invalidation on vector reallocation ---
    if (speciesList.capacity() != lastSpeciesListCapacity_) {
        patternMatches_.assign(reactantPatterns_.size(), {});
        matchesInitialized_ = false;
        lastSpeciesListCapacity_ = speciesList.capacity();
        // NOTE: Do NOT clear lastProcessedInIteration_ here.
        // Pattern match cache uses pointers (invalidated by reallocation),
        // but iteration tracking uses species indices (still valid).
    }

    if (!matchesInitialized_ || patternMatches_.size() != reactantPatterns_.size()) {
        patternMatches_.assign(reactantPatterns_.size(), {});
        matchesInitialized_ = false;
    }

    // --- Collect new species (not processed by THIS RULE in current iteration) ---
    const bool debug = std::getenv("BNG_DEBUG_RULES") != nullptr;
    const bool isBimolecular = reactantPatterns_.size() >= 2;

    // First pass: identify truly new species (never processed by ANY rule yet)
    // Only consider species that existed at the start of this iteration (index < speciesBoundary).
    // Species created by earlier rules in the same iteration are deferred to the next iteration.
    bool hasTrulyNewSpecies = false;
    for (std::size_t i = 0; i < std::min(speciesList.size(), speciesBoundary); ++i) {
        if (!speciesList.get(i).rulesApplied()) {
            hasTrulyNewSpecies = true;
            break;
        }
    }

    std::vector<std::size_t> newSpecies;

    // Ensure the vector is sized appropriately
    if (lastProcessedInIteration_.size() < speciesBoundary) {
        lastProcessedInIteration_.resize(speciesBoundary, static_cast<std::size_t>(-1));
    }

    for (std::size_t i = 0; i < std::min(speciesList.size(), speciesBoundary); ++i) {
        const bool hasIter = (i < lastProcessedInIteration_.size() && lastProcessedInIteration_[i] != static_cast<std::size_t>(-1));
        const std::size_t lastIter = hasIter ? lastProcessedInIteration_[i] : 0;

        const bool notYetProcessed = !speciesList.get(i).rulesApplied();
        const bool notProcessedByThisRuleInCurrentIteration = (!hasIter || lastIter < currentIteration);
        const bool neverProcessedByThisRule = !hasIter;

        // Unimolecular: only process species never seen by this rule before.
        // Using neverProcessedByThisRule (not just notProcessedInCurrentIteration)
        // prevents re-processing species across iterations, which would cause
        // duplicate reactions and doubled stat factors.
        // Bimolecular: process new species OR (if truly new species exist) old
        // species for cross-matching (old × new combinations).
        if (!isBimolecular) {
            // Unimolecular: process only never-before-seen species
            if (neverProcessedByThisRule && notYetProcessed) {
                newSpecies.push_back(i);
            }
        } else {
            // Bimolecular: process new species or old species for cross-matching
            if (notYetProcessed && notProcessedByThisRuleInCurrentIteration) {
                newSpecies.push_back(i);
            } else if (hasTrulyNewSpecies && speciesList.get(i).rulesApplied() &&
                       notProcessedByThisRuleInCurrentIteration) {
                newSpecies.push_back(i);
            }
        }
    }
    if (debug) {
        std::cerr << "[DEBUG] Rule " << ruleName_ << " iter=" << currentIteration
                  << ": newSpecies={";
        for (auto idx : newSpecies) std::cerr << idx << ",";
        std::cerr << "} trackedCapacity=" << lastProcessedInIteration_.size() << "\n";
    }
    if (newSpecies.empty()) {
        return 0;
    }

    const bool cacheNeedsRebuild = !matchesInitialized_;
    const std::size_t nPatterns = reactantPatterns_.size();
    std::size_t created = 0;

    if (debug) {
        std::cerr << "[DEBUG] Rule " << ruleName_ << ": nPatterns=" << nPatterns
                  << " cacheNeedsRebuild=" << cacheNeedsRebuild << "\n";
    }

    // =============================================
    // Two-phase: update all caches, then enumerate
    // =============================================
    // Phase 1: Find new embeddings for ALL patterns and update caches.
    // Phase 2: For each pattern with new matches, enumerate combinations
    //          with that pattern as trigger.

    std::vector<std::size_t> firstNewPerPattern(nPatterns, 0);
    bool anyNewMatches = false;

    // Phase 1: Update all pattern caches
    for (std::size_t patternIndex = nPatterns; patternIndex-- > 0;) {
        std::vector<std::size_t> searchSet;
        if (cacheNeedsRebuild) {
            for (std::size_t i = 0; i < std::min(speciesList.size(), speciesBoundary); ++i) {
                searchSet.push_back(i);
            }
        } else {
            searchSet = newSpecies;
        }

        auto newMatches = findEmbeddingsForSpecies(patternIndex, speciesList, searchSet, model);

        firstNewPerPattern[patternIndex] = patternMatches_[patternIndex].size();

        if (cacheNeedsRebuild && !newMatches.empty()) {
            // Partition: "old" species (processed by this rule in a previous iteration)
            // go first; "new" species (never processed by this rule) go after.
            // This ensures the enumeration's trigger/non-trigger split correctly
            // avoids re-enumerating already-counted combinations.
            std::stable_partition(newMatches.begin(), newMatches.end(),
                [this](const EmbeddingResult& m) {
                    return m.speciesIndex < lastProcessedInIteration_.size() &&
                           lastProcessedInIteration_[m.speciesIndex] != static_cast<std::size_t>(-1);
                });

            std::size_t oldCount = 0;
            for (const auto& m : newMatches) {
                if (m.speciesIndex < lastProcessedInIteration_.size() &&
                    lastProcessedInIteration_[m.speciesIndex] != static_cast<std::size_t>(-1)) {
                    ++oldCount;
                }
            }
            firstNewPerPattern[patternIndex] = patternMatches_[patternIndex].size() + oldCount;
        }

        if (!newMatches.empty()) {
            anyNewMatches = true;
            patternMatches_[patternIndex].insert(
                patternMatches_[patternIndex].end(),
                std::make_move_iterator(newMatches.begin()),
                std::make_move_iterator(newMatches.end()));
        }
    }

    if (debug) {
        std::cerr << "[DEBUG] Rule " << ruleName_ << " after Phase 1: anyNewMatches=" << anyNewMatches << "\n";
        for (std::size_t i = 0; i < nPatterns; ++i) {
            std::cerr << "[DEBUG]   pattern[" << i << "]: " << patternMatches_[i].size() << " total matches\n";
        }
    }

    if (!anyNewMatches) {
        if (debug) std::cerr << "[DEBUG] Rule " << ruleName_ << ": no new matches, returning 0\n";
        matchesInitialized_ = true;
        lastSpeciesListCapacity_ = speciesList.capacity();
        // Mark processed even if no matches
        for (std::size_t idx : newSpecies) {
            if (idx >= lastProcessedInIteration_.size()) {
                lastProcessedInIteration_.resize(idx + 1, static_cast<std::size_t>(-1));
            }
            lastProcessedInIteration_[idx] = currentIteration;
        }
        return 0;
    }

    // Check if all patterns have at least one match
    bool allNonEmpty = true;
    for (const auto& matches : patternMatches_) {
        if (matches.empty()) {
            allNonEmpty = false;
            break;
        }
    }
    if (!allNonEmpty) {
        if (debug) std::cerr << "[DEBUG] Rule " << ruleName_ << ": not all patterns have matches, returning 0\n";
        matchesInitialized_ = true;
        lastSpeciesListCapacity_ = speciesList.capacity();
        // Mark processed even if not all patterns match
        for (std::size_t idx : newSpecies) {
            if (idx >= lastProcessedInIteration_.size()) {
                lastProcessedInIteration_.resize(idx + 1, static_cast<std::size_t>(-1));
            }
            lastProcessedInIteration_[idx] = currentIteration;
        }
        return 0;
    }

    // Phase 2: Enumerate for each pattern that has new matches.
    // Iterate triggers in reverse order (matching Perl's expand_rule).
    // To avoid double-counting (new×new) combinations:
    //   - The trigger pattern iterates from firstNew to end (new matches only).
    //   - Non-trigger patterns that were ALREADY processed as triggers
    //     iterate from 0 to firstNew (old matches only, since new×new was
    //     already counted when that pattern was the trigger).
    //   - Non-trigger patterns NOT YET processed iterate from 0 to end (all).
    std::unordered_set<std::size_t> alreadyTriggered;
    for (std::size_t patternIndex = nPatterns; patternIndex-- > 0;) {
        const std::size_t firstNew = firstNewPerPattern[patternIndex];
        if (firstNew >= patternMatches_[patternIndex].size()) {
            if (debug) std::cerr << "[DEBUG] Rule " << ruleName_ << ": pattern " << patternIndex << " has no new matches, skipping\n";
            continue;  // No new matches for this pattern
        }

        if (debug) std::cerr << "[DEBUG] Rule " << ruleName_ << ": enumerating with pattern " << patternIndex << " as trigger\n";

        std::vector<EmbeddingResult> matchSet(nPatterns);
        std::size_t enumerateCalls = 0;
        std::function<void(std::size_t)> enumerate = [&](std::size_t idx) {
            if (idx == nPatterns) {
                ++enumerateCalls;
                if (buildReaction(matchSet, speciesList, rxnList, productFilter, model)) {
                    ++created;
                    if (debug) std::cerr << "[DEBUG] Rule " << ruleName_ << ": created reaction #" << created << "\n";
                }
                return;
            }

            std::size_t begin = 0;
            std::size_t end = patternMatches_[idx].size();

            if (idx == patternIndex) {
                // Trigger: new matches only
                begin = firstNew;
            } else if (alreadyTriggered.count(idx)) {
                // Already triggered: use old matches only (new×new already counted)
                end = firstNewPerPattern[idx];
            }

            for (std::size_t i = begin; i < end; ++i) {
                matchSet[idx] = patternMatches_[idx][i];
                enumerate(idx + 1);
            }
        };

        enumerate(0);
        if (debug) std::cerr << "[DEBUG] Rule " << ruleName_ << ": enumerate called " << enumerateCalls << " times\n";
        alreadyTriggered.insert(patternIndex);
    }

    matchesInitialized_ = true;
    lastSpeciesListCapacity_ = speciesList.capacity();

    // Mark these species as processed by this rule in this iteration
    for (std::size_t idx : newSpecies) {
        if (idx >= lastProcessedInIteration_.size()) {
            lastProcessedInIteration_.resize(idx + 1, static_cast<std::size_t>(-1));
        }
        lastProcessedInIteration_[idx] = currentIteration;
    }

    return created;
}

bool ReactionRule::buildReaction(
    const std::vector<EmbeddingResult>& matchSet,
    SpeciesList& speciesList,
    RxnList& rxnList,
    const std::function<bool(const SpeciesGraph&)>& productFilter,
    const Model* model) const {
    const bool debug = std::getenv("BNG_DEBUG_BUILD_RXN") != nullptr;
    const auto reactantInfo = describePatterns(reactantPatterns_);
    const auto productInfo = describePatterns(productPatterns_);
    std::vector<std::size_t> reactantIndices;
    reactantIndices.reserve(matchSet.size());
    std::string inferredCompartment;
    bool sameCompartment = true;

    BNGcore::PatternGraph aggregateGraph;
    std::vector<std::unordered_map<BNGcore::Node*, BNGcore::Node*>> cloneMaps;
    cloneMaps.reserve(matchSet.size());

    for (const auto& match : matchSet) {
        const auto& matchedGraph = speciesList.get(match.speciesIndex).getSpeciesGraph().getGraph();
        cloneMaps.push_back(cloneGraphIntoTarget(matchedGraph, aggregateGraph));
        reactantIndices.push_back(match.speciesIndex);
        const auto& compartment = speciesList.get(match.speciesIndex).getCompartment();
        if (reactantIndices.size() == 1) {
            inferredCompartment = compartment;
        } else if (compartment != inferredCompartment) {
            sameCompartment = false;
        }
    }

    // Perl's interactingSet check (SpeciesGraph.pm:955-1009):
    // Reactant species must be compartment-compatible:
    // - All surfaces must be the same, all volumes must be the same
    // - If surface + volume, they must be adjacent
    // - No mix of compartment/no-compartment species
    if (matchSet.size() > 1 && !g_compartmentDimensions.empty()) {
        std::string uniqSurface, uniqVolume;
        int hasMissing = 0, hasComp = 0;
        bool valid = true;
        for (const auto& match : matchSet) {
            const auto& comp = speciesList.get(match.speciesIndex).getCompartment();
            if (comp.empty()) { hasMissing++; continue; }
            hasComp++;
            auto dimIt = g_compartmentDimensions.find(comp);
            if (dimIt == g_compartmentDimensions.end()) continue;
            if (dimIt->second == 2) {
                if (uniqSurface.empty()) uniqSurface = comp;
                else if (uniqSurface != comp) { valid = false; break; }
            } else if (dimIt->second == 3) {
                if (uniqVolume.empty()) uniqVolume = comp;
                else if (uniqVolume != comp) { valid = false; break; }
            }
        }
        if (valid && hasMissing > 0 && hasComp > 0) valid = false;
        if (valid && !uniqSurface.empty() && !uniqVolume.empty()) {
            valid = isAdjacentToSurface(uniqVolume, uniqSurface);
        }
        if (!valid) return false;
    }

    auto* graph = &aggregateGraph;

    const auto mapMatchedNodeToAggregate = [&](const ComponentRef& ref, BNGcore::Node* patternNode) -> BNGcore::Node* {
        if (ref.patternIndex >= matchSet.size() || ref.patternIndex >= cloneMaps.size()) {
            return nullptr;
        }
        auto* matchedNode = matchSet[ref.patternIndex].map.mapf(patternNode);
        if (matchedNode == nullptr) {
            return nullptr;
        }
        const auto cloneFound = cloneMaps[ref.patternIndex].find(matchedNode);
        return cloneFound == cloneMaps[ref.patternIndex].end() ? nullptr : cloneFound->second;
    };

    // Capture reactant component states BEFORE operations modify/delete them
    // (needed for tag-based state propagation after operations)
    std::map<ComponentRef, std::string> capturedReactantStates;
    if (!tagComponentMap_.empty()) {
        for (const auto& [productRef, reactantRef] : tagComponentMap_) {
            const auto& compInfo = getComponentInfo(reactantInfo, reactantRef);
            auto* node = mapMatchedNodeToAggregate(reactantRef, compInfo.node);
            if (node != nullptr) {
                std::string st = node->get_state().get_BNG2_string();
                if (!st.empty() && st[0] == '~') st = st.substr(1);
                capturedReactantStates[reactantRef] = st;
            }
        }
    }

    // Map from product pattern nodes to aggregate graph clones (populated by AddMolecule)
    std::unordered_map<BNGcore::Node*, BNGcore::Node*> productCloneMaps;

    for (const auto& operation : operations_) {
        switch (operation.type) {
        case TransformOp::Type::AddBond: {
            const auto& lhsInfo = getComponentInfo(reactantInfo, operation.source);
            const auto& rhsInfo = getComponentInfo(reactantInfo, operation.partner);
            if (!lhsInfo.hasBond() || !rhsInfo.hasBond()) {
                break;
            }
            auto* lhsComponent = mapMatchedNodeToAggregate(operation.source, lhsInfo.node);
            auto* rhsComponent = mapMatchedNodeToAggregate(operation.partner, rhsInfo.node);
            auto* lhsBond = findAttachedUnboundBondNode(lhsComponent);
            auto* rhsBond = findAttachedUnboundBondNode(rhsComponent);
            if (lhsComponent == nullptr || rhsComponent == nullptr) {
                return false;
            }
            // Remove unbound marker bonds if present (for components gaining a new bond)
            if (lhsBond) safeDeleteNode(*graph, lhsBond);
            if (rhsBond) safeDeleteNode(*graph, rhsBond);
            auto* bondNode = new BNGcore::Node(BNGcore::BOND_NODE_TYPE);
            bondNode->set_state(BNGcore::BOUND_STATE);
            graph->add_node(bondNode);
            graph->add_edge(lhsComponent, bondNode);
            graph->add_edge(rhsComponent, bondNode);
            break;
        }
        case TransformOp::Type::DeleteBond: {
            const auto& lhsInfo = getComponentInfo(reactantInfo, operation.source);
            const auto& rhsInfo = getComponentInfo(reactantInfo, operation.partner);
            if (!lhsInfo.hasBond()) {
                break;
            }
            auto* lhsComponent = mapMatchedNodeToAggregate(operation.source, lhsInfo.node);
            auto* rhsComponent = mapMatchedNodeToAggregate(operation.partner, rhsInfo.node);
            auto* bondNode = findSharedBondNode(lhsComponent, rhsComponent);
            if (lhsComponent == nullptr || rhsComponent == nullptr || bondNode == nullptr) {
                return false;
            }
            safeDeleteNode(*graph, bondNode);
            auto* lhsBond = new BNGcore::Node(BNGcore::BOND_NODE_TYPE);
            lhsBond->set_state(BNGcore::UNBOUND_STATE);
            graph->add_node(lhsBond);
            graph->add_edge(lhsComponent, lhsBond);
            auto* rhsBond = new BNGcore::Node(BNGcore::BOND_NODE_TYPE);
            rhsBond->set_state(BNGcore::UNBOUND_STATE);
            graph->add_node(rhsBond);
            graph->add_edge(rhsComponent, rhsBond);
            break;
        }
        case TransformOp::Type::ChangeState: {
            const auto& componentInfo = getComponentInfo(reactantInfo, operation.source);
            auto* componentNode = mapMatchedNodeToAggregate(operation.source, componentInfo.node);
            if (componentNode == nullptr) {
                return false;
            }
            const auto& stateType = componentNode->get_type().get_state_type();
            const auto& labelType = static_cast<const BNGcore::LabelStateType&>(stateType);
            BNGcore::LabelState state(labelType, operation.newState);
            graph->set_node_state(componentNode, state);
            break;
        }
        case TransformOp::Type::AddMolecule: {
            auto addMolClones = cloneProductMolecule(productInfo[operation.patternIndex], operation.moleculeIndex, aggregateGraph);
            // Merge into a global product→aggregate map for cross-bond application later
            for (auto& [src, dst] : addMolClones) {
                productCloneMaps[src] = dst;
            }
            break;
        }
        case TransformOp::Type::DeleteMolecule:
            {
                ComponentRef moleculeRef {operation.patternIndex, operation.moleculeIndex, 0};
                auto* mappedMolecule = mapMatchedNodeToAggregate(
                    moleculeRef,
                    getMoleculeNode(reactantInfo, operation.patternIndex, operation.moleculeIndex));
                if (mappedMolecule == nullptr) {
                    return false;
                }
                std::vector<BNGcore::Node*> components;
                for (auto edge = mappedMolecule->edges_out_begin(); edge != mappedMolecule->edges_out_end(); ++edge) {
                    components.push_back(*edge);
                }
                for (auto* component : components) {
                    std::vector<BNGcore::Node*> bondNodes;
                    for (auto edge = component->edges_out_begin(); edge != component->edges_out_end(); ++edge) {
                        bondNodes.push_back(*edge);
                    }
                    for (auto* bondNode : bondNodes) {
                        safeDeleteNode(*graph, bondNode);
                    }
                    safeDeleteNode(*graph, component);
                }
                safeDeleteNode(*graph, mappedMolecule);
            }
            break;
        }
    }

    // Apply product-only state changes: when the product pattern specifies a
    // component with an explicit state (e.g. s~U) or no bond that was NOT
    // mentioned in the reactant pattern, we must find that component on the
    // matched species molecule and apply the state change / break its bond.
    // Example: BetaR(l,g,loc~cyt) -> BetaR(l,g,loc~mem,s~U) — the 's' component
    // is absent from the reactant pattern but the product requires s~U and no bond.
    if (!productOnlyStateChanges_.empty()) {
        for (const auto& posc : productOnlyStateChanges_) {
            // Find the molecule node in the aggregate graph
            auto* moleculePatternNode = getMoleculeNode(reactantInfo, posc.reactantPatternIndex, posc.reactantMoleculeIndex);
            ComponentRef molRef{posc.reactantPatternIndex, posc.reactantMoleculeIndex, 0};
            auto* moleculeAggNode = mapMatchedNodeToAggregate(molRef, moleculePatternNode);
            if (moleculeAggNode == nullptr) continue;

            // Walk the component children of this molecule to find the named component
            for (auto edge = moleculeAggNode->edges_out_begin(); edge != moleculeAggNode->edges_out_end(); ++edge) {
                if (!isComponentNode(**edge)) continue;
                if ((*edge)->get_type().get_type_name() != posc.componentName) continue;

                // Apply state change if requested
                if (!posc.newState.empty()) {
                    const auto& stateType = (*edge)->get_type().get_state_type();
                    if (const auto* labelType = dynamic_cast<const BNGcore::LabelStateType*>(&stateType)) {
                        BNGcore::LabelState state(*labelType, posc.newState);
                        graph->set_node_state(*edge, state);
                    }
                }

                // Break bond if requested (product has no bond on this component)
                if (posc.breakBond) {
                    std::vector<BNGcore::Node*> boundBondNodes;
                    for (auto be = (*edge)->edges_out_begin(); be != (*edge)->edges_out_end(); ++be) {
                        if (isBondNode(**be)) {
                            // Only break actual bound bonds, not unbound "!-" markers
                            if ((*be)->get_state().get_BNG2_string() != "!-") {
                                boundBondNodes.push_back(*be);
                            }
                        }
                    }
                    for (auto* bn : boundBondNodes) {
                        for (auto pe = bn->edges_in_begin(); pe != bn->edges_in_end(); ++pe) {
                            if (*pe != *edge) {
                                auto* unbondNode = new BNGcore::Node(BNGcore::BOND_NODE_TYPE);
                                unbondNode->set_state(BNGcore::UNBOUND_STATE);
                                graph->add_node(unbondNode);
                                graph->add_edge(*pe, unbondNode);
                            }
                        }
                        safeDeleteNode(*graph, bn);
                    }
                }
                break; // Only apply to the first matching component
            }
        }
    }

    // Apply cross-boundary bonds: connect existing (reactant-matched) components
    // to newly-added product molecule components. This handles rules like
    // A(x) -> A(x!1).B(y!1) where B is new and the bond between A.x and B.y
    // must be established after AddMolecule clones B into the aggregate graph.
    if (!crossBonds_.empty() && !productCloneMaps.empty()) {
        for (const auto& crossBond : crossBonds_) {
            // Find the reactant-side component in the aggregate graph
            const auto& reactantCompInfo = getComponentInfo(reactantInfo, crossBond.reactantComponent);
            auto* existingComp = mapMatchedNodeToAggregate(crossBond.reactantComponent, reactantCompInfo.node);
            if (existingComp == nullptr) continue;

            // Find the product-side component in the aggregate graph (cloned by AddMolecule)
            const auto& productCompInfo = getComponentInfo(productInfo, crossBond.productComponent);
            auto productCloneIt = productCloneMaps.find(productCompInfo.node);
            if (productCloneIt == productCloneMaps.end()) continue;
            auto* newComp = productCloneIt->second;

            // Remove any dangling unbound bond nodes on the existing component
            auto* existingBond = findAttachedUnboundBondNode(existingComp);
            if (existingBond) safeDeleteNode(*graph, existingBond);

            // Remove any dangling bond nodes on the new component (cloned from product pattern)
            // These are bond nodes that were cloned but not connected to the other side.
            {
                std::vector<BNGcore::Node*> danglingBonds;
                for (auto edge = newComp->edges_out_begin(); edge != newComp->edges_out_end(); ++edge) {
                    if (isBondNode(**edge)) {
                        danglingBonds.push_back(*edge);
                    }
                }
                for (auto* bn : danglingBonds) {
                    safeDeleteNode(*graph, bn);
                }
            }

            // Create a new bond connecting the two components
            auto* bondNode = new BNGcore::Node(BNGcore::BOND_NODE_TYPE);
            bondNode->set_state(BNGcore::BOUND_STATE);
            graph->add_node(bondNode);
            graph->add_edge(existingComp, bondNode);
            graph->add_edge(newComp, bondNode);
        }
    }

    // Apply bonds between two newly-added product molecules.
    // Both sides were cloned by separate AddMolecule operations.
    if (debug) {
        std::cerr << "[BUILD_RXN] newMoleculeBonds=" << newMoleculeBonds_.size()
                  << " productCloneMaps=" << productCloneMaps.size() << "\n";
    }
    if (!newMoleculeBonds_.empty() && !productCloneMaps.empty()) {
        for (const auto& nmBond : newMoleculeBonds_) {
            if (debug) {
                const auto& comp1Info = getComponentInfo(productInfo, nmBond.productComponent1);
                const auto& comp2Info = getComponentInfo(productInfo, nmBond.productComponent2);
                auto it1 = productCloneMaps.find(comp1Info.node);
                auto it2 = productCloneMaps.find(comp2Info.node);
                std::cerr << "[BUILD_RXN] nmBond: comp1.found=" << (it1 != productCloneMaps.end())
                          << " comp2.found=" << (it2 != productCloneMaps.end())
                          << " p1=" << nmBond.productComponent1.patternIndex << "." << nmBond.productComponent1.moleculeIndex << "." << nmBond.productComponent1.componentIndex
                          << " p2=" << nmBond.productComponent2.patternIndex << "." << nmBond.productComponent2.moleculeIndex << "." << nmBond.productComponent2.componentIndex << "\n";
            }
            const auto& comp1Info = getComponentInfo(productInfo, nmBond.productComponent1);
            const auto& comp2Info = getComponentInfo(productInfo, nmBond.productComponent2);
            auto it1 = productCloneMaps.find(comp1Info.node);
            auto it2 = productCloneMaps.find(comp2Info.node);
            if (it1 == productCloneMaps.end() || it2 == productCloneMaps.end()) continue;
            auto* comp1 = it1->second;
            auto* comp2 = it2->second;

            // Remove dangling bond nodes on both sides
            {
                std::vector<BNGcore::Node*> dangling;
                for (auto edge = comp1->edges_out_begin(); edge != comp1->edges_out_end(); ++edge) {
                    if (isBondNode(**edge)) dangling.push_back(*edge);
                }
                for (auto* bn : dangling) safeDeleteNode(*graph, bn);
            }
            {
                std::vector<BNGcore::Node*> dangling;
                for (auto edge = comp2->edges_out_begin(); edge != comp2->edges_out_end(); ++edge) {
                    if (isBondNode(**edge)) dangling.push_back(*edge);
                }
                for (auto* bn : dangling) safeDeleteNode(*graph, bn);
            }

            auto* bondNode = new BNGcore::Node(BNGcore::BOND_NODE_TYPE);
            bondNode->set_state(BNGcore::BOUND_STATE);
            graph->add_node(bondNode);
            graph->add_edge(comp1, bondNode);
            graph->add_edge(comp2, bondNode);
        }
    }

    // Apply tag-based state propagation for newly added product molecules
    if (!tagComponentMap_.empty() && !capturedReactantStates.empty()) {
        for (const auto& [productRef, reactantRef] : tagComponentMap_) {
            auto stateIt = capturedReactantStates.find(reactantRef);
            if (stateIt == capturedReactantStates.end()) continue;
            const std::string& reactantState = stateIt->second;
            if (reactantState.empty() || reactantState == "?") continue;

            // Find the product molecule in the aggregate graph by type
            const auto& productMolInfo = productInfo[productRef.patternIndex].molecules[productRef.moleculeIndex];
            for (auto nodeIter = graph->begin(); nodeIter != graph->end(); ++nodeIter) {
                if (!isMoleculeNode(**nodeIter)) continue;
                if ((*nodeIter)->get_type().get_type_name() != productMolInfo.name) continue;

                std::size_t compIdx = 0;
                for (auto edge = (*nodeIter)->edges_out_begin(); edge != (*nodeIter)->edges_out_end(); ++edge) {
                    if (!isComponentNode(**edge)) continue;
                    if (compIdx == productRef.componentIndex) {
                        const auto& stateType = (*edge)->get_type().get_state_type();
                        if (const auto* labelType = dynamic_cast<const BNGcore::LabelStateType*>(&stateType)) {
                            std::string currentState = (*edge)->get_state().get_BNG2_string();
                            if (currentState == "~?" || currentState == "?") {
                                BNGcore::LabelState state(*labelType, reactantState);
                                graph->set_node_state(*edge, state);
                            }
                        }
                        break;
                    }
                    ++compIdx;
                }
            }
        }
    }

    std::vector<std::size_t> productIndices;
    std::vector<std::string> productLabels;

    if (debug) {
        std::cerr << "[BUILD_RXN] rule=" << ruleName_ << " operations=" << operations_.size() << "\n";
        for (std::size_t i = 0; i < operations_.size(); ++i) {
            const auto& op = operations_[i];
            std::string opType = "Unknown";
            if (op.type == TransformOp::Type::AddBond) opType = "AddBond";
            else if (op.type == TransformOp::Type::DeleteBond) opType = "DeleteBond";
            else if (op.type == TransformOp::Type::ChangeState) opType = "ChangeState";
            else if (op.type == TransformOp::Type::AddMolecule) opType = "AddMolecule";
            else if (op.type == TransformOp::Type::DeleteMolecule) opType = "DeleteMolecule";
            std::cerr << "[BUILD_RXN]   op[" << i << "]=" << opType << "\n";
        }
    }

    // Check if rule has AddMolecule operations (synthesis-like: Source() -> NewComplex())
    bool hasAddMolecule = false;
    for (const auto& op : operations_) {
        if (op.type == TransformOp::Type::AddMolecule) {
            hasAddMolecule = true;
            break;
        }
    }

    // --- Compartment transport: update per-molecule compartments ---
    // When reactant and product patterns have different species-level compartments
    // (e.g., @PM:R.R -> @EM:R.R), update per-molecule compartments in the aggregate
    // graph before splitting. Uses Perl's separated_by_volume logic for surface transport.
    // Also handles peer-level transport (e.g., @Cell1 <-> @Cell2 with no parent/child).
    if (!g_compartmentDimensions.empty()) {
        auto endIt = g_compartmentDimensions.end();
        std::string lastCompR;
        std::string lastCompP;
        std::unordered_map<std::string, int>::const_iterator lastDimRIt = endIt;
        std::unordered_map<std::string, int>::const_iterator lastDimPIt = endIt;

        for (std::size_t pi = 0; pi < reactantPatterns_.size() && pi < productPatterns_.size(); ++pi) {
            const auto& compR = reactantPatterns_[pi].getCompartment();
            const auto& compP = (pi < productPatterns_.size()) ? productPatterns_[pi].getCompartment() : std::string();
            if (compR.empty() || compP.empty() || compR == compP) continue;

            auto dimRIt = lastDimRIt;
            if (lastCompR != compR) {
                dimRIt = g_compartmentDimensions.find(compR);
                lastCompR = compR;
                lastDimRIt = dimRIt;
            }

            auto dimPIt = lastDimPIt;
            if (lastCompP != compP) {
                dimPIt = g_compartmentDimensions.find(compP);
                lastCompP = compP;
                lastDimPIt = dimPIt;
            }

            if (dimRIt == endIt || dimPIt == endIt) continue;
            if (dimRIt->second != dimPIt->second) continue; // must be same dimension

            // Build compartment mapping based on transport type
            std::unordered_map<std::string, std::string> compMap;
            compMap[compR] = compP;

            if (dimRIt->second == 2) {
                // Surface-to-surface: determine endocytosis vs exocytosis
                int cytosis = separatedByVolume(compR, compP);
                std::string outsideR = getOutside(compR);
                std::string insideR = getInside(compR);
                std::string outsideP = getOutside(compP);
                std::string insideP = getInside(compP);
                if (cytosis == 1) {
                    // Endocytosis: flip outside/inside
                    std::swap(insideP, outsideP);
                }
                if (!outsideR.empty() && !outsideP.empty()) compMap[outsideR] = outsideP;
                if (!insideR.empty() && !insideP.empty()) compMap[insideR] = insideP;
            } else {
                // Volume-to-volume: simple mapping (all molecules → new compartment)
                // Already handled by compMap[compR] = compP above
            }

            // Apply compartment mapping to all molecule nodes in the aggregate graph
            // that came from reactant pattern pi
            if (pi < cloneMaps.size()) {
                const auto& speciesGraph = speciesList.get(matchSet[pi].speciesIndex).getSpeciesGraph().getGraph();
                for (auto nodeIter = speciesGraph.begin(); nodeIter != speciesGraph.end(); ++nodeIter) {
                    if (!isMoleculeNode(**nodeIter)) continue;
                    auto cloneIt = cloneMaps[pi].find(*nodeIter);
                    if (cloneIt == cloneMaps[pi].end()) continue;
                    auto* clonedMol = cloneIt->second;
                    const auto& molComp = clonedMol->get_compartment();
                    auto mapIt = compMap.find(molComp);
                    if (mapIt != compMap.end()) {
                        clonedMol->set_compartment(mapIt->second);
                    }
                }
            }
        }

        // --- Per-molecule compartment transport ---
        // Apply compartment changes from the pre-computed moleculeCompartmentTransports_
        // table.  This handles both same-pattern and cross-pattern molecule mappings
        // (e.g., R(l)@PM + L(r)@EC <-> R(l!1)@PM.L(r!1)@PM where L moves EC→PM).
        std::unordered_map<BNGcore::Node*, std::string> moleculeTransported; // clone → new compartment
        for (const auto& mct : moleculeCompartmentTransports_) {
            const std::size_t rpi = mct.reactantPatternIndex;
            const std::size_t rmi = mct.reactantMoleculeIndex;
            if (rpi >= cloneMaps.size() || rpi >= reactantInfo.size()) continue;
            if (rmi >= reactantInfo[rpi].molecules.size()) continue;
            auto* patternMolNode = reactantInfo[rpi].molecules[rmi].node;
            auto* targetMolNode = matchSet[rpi].map.mapf(patternMolNode);
            if (!targetMolNode) continue;
            auto cloneIt = cloneMaps[rpi].find(targetMolNode);
            if (cloneIt == cloneMaps[rpi].end()) continue;
            cloneIt->second->set_compartment(mct.productCompartment);
            moleculeTransported[cloneIt->second] = mct.productCompartment;
        }

        // --- MoveConnected: transport connected cargo molecules ---
        // Perl's MoveConnected: when a molecule changes compartment, all molecules
        // connected to it through bonds in the SAME original compartment also move,
        // UNLESS they are explicitly named in the reactant pattern.
        if (hasModifier(modifiers_, "MoveConnected")) {
            // Build set of PATTERN molecule clones (to exclude from BFS).
            // Only include molecules explicitly in the reactant PATTERN, not all
            // molecules in the matched species.
            std::unordered_set<BNGcore::Node*> patternMols;
            for (std::size_t pi = 0; pi < reactantInfo.size() && pi < cloneMaps.size(); ++pi) {
                for (const auto& molInfo : reactantInfo[pi].molecules) {
                    // Map pattern molecule → target molecule → aggregate clone
                    auto* targetMol = matchSet[pi].map.mapf(molInfo.node);
                    if (targetMol) {
                        auto cloneIt = cloneMaps[pi].find(targetMol);
                        if (cloneIt != cloneMaps[pi].end()) {
                            patternMols.insert(cloneIt->second);
                        }
                    }
                }
            }

            // For each molecule that was ACTUALLY transported, BFS to connected cargo
            for (auto& [seed, targetComp] : moleculeTransported) {
                // BFS through bonds in the aggregate graph
                std::queue<BNGcore::Node*> q;
                std::unordered_set<BNGcore::Node*> visited;
                q.push(seed);
                visited.insert(seed);

                while (!q.empty()) {
                    auto* mol = q.front();
                    q.pop();
                    // mol → out_edges → components → out_edges → bonds → in_edges → partner components → in_edges → partner molecules
                    for (auto ce = mol->edges_out_begin(); ce != mol->edges_out_end(); ++ce) {
                        if (!isComponentNode(**ce)) continue;
                        for (auto be = (*ce)->edges_out_begin(); be != (*ce)->edges_out_end(); ++be) {
                            if (!isBondNode(**be)) continue;
                            // Only follow real bonds (bound state "+"), not unbound markers
                            const auto& bs = (*be)->get_state().get_BNG2_string();
                            if (bs != "!+" && bs != "+") continue;
                            for (auto pce = (*be)->edges_in_begin(); pce != (*be)->edges_in_end(); ++pce) {
                                if (*pce == *ce) continue;
                                for (auto pme = (*pce)->edges_in_begin(); pme != (*pce)->edges_in_end(); ++pme) {
                                    if (!isMoleculeNode(**pme)) continue;
                                    if (visited.count(*pme)) continue;
                                    visited.insert(*pme);
                                    // Skip pattern molecules (they have their own compartment handling)
                                    if (patternMols.count(*pme)) continue;
                                    // Transport this cargo molecule
                                    (*pme)->set_compartment(targetComp);
                                    q.push(*pme);
                                }
                            }
                        }
                    }
                }
            }
        }
    }

    const bool deleteMoleculesModifier = hasModifier(modifiers_, "deletemolecules");
    if (productPatterns_.empty() && !reactantPatterns_.empty()) {
        if (deleteMoleculesModifier) {
            // DeleteMolecules rule applied to a complex: delete the matched
            // molecules from the aggregate graph, then any remaining connected
            // components become product species.
            // E.g., VEGF(r!1).VEGFR2(l!1) -> 0 DeleteMolecules applied to a dimer
            // should leave the other half of the complex as a product.

            // Delete matched molecules from aggregate graph
            const auto reactantInfoLocal = describePatterns(reactantPatterns_);
            for (std::size_t pi = 0; pi < reactantPatterns_.size(); ++pi) {
                for (const auto& molInfo : reactantInfoLocal[pi].molecules) {
                    auto* targetMol = matchSet[pi].map.mapf(molInfo.node);
                    if (!targetMol) continue;
                    // Find cloned node in aggregate graph
                    auto cloneIt = cloneMaps[pi].find(targetMol);
                    if (cloneIt == cloneMaps[pi].end()) continue;
                    auto* clonedMol = cloneIt->second;
                    // Delete molecule and all its components/bonds
                    std::vector<BNGcore::Node*> components;
                    for (auto edge = clonedMol->edges_out_begin(); edge != clonedMol->edges_out_end(); ++edge) {
                        components.push_back(*edge);
                    }
                    for (auto* component : components) {
                        std::vector<BNGcore::Node*> bondNodes;
                        for (auto edge = component->edges_out_begin(); edge != component->edges_out_end(); ++edge) {
                            bondNodes.push_back(*edge);
                        }
                        for (auto* bondNode : bondNodes) {
                            safeDeleteNode(aggregateGraph, bondNode);
                        }
                        safeDeleteNode(aggregateGraph, component);
                    }
                    safeDeleteNode(aggregateGraph, clonedMol);
                }
            }

            auto remainingGraphs = splitIntoSpeciesGraphs(aggregateGraph);
            for (auto& productGraph : remainingGraphs) {
                if (productFilter && !productFilter(productGraph)) {
                    return false;
                }
                productLabels.push_back(productGraph.canonicalLabel());
                std::string compartmentToUse;
                if (!g_compartmentDimensions.empty()) {
                    std::set<std::string> surfaces, volumes;
                    for (auto nodeIter = productGraph.getGraph().begin(); nodeIter != productGraph.getGraph().end(); ++nodeIter) {
                        if (!isMoleculeNode(**nodeIter)) continue;
                        auto mc = (*nodeIter)->get_compartment();
                        if (mc.empty()) continue;
                        auto dimIt = g_compartmentDimensions.find(mc);
                        if (dimIt != g_compartmentDimensions.end()) {
                            if (dimIt->second == 2) surfaces.insert(mc);
                            else if (dimIt->second == 3) volumes.insert(mc);
                        }
                    }
                    if (surfaces.empty() && volumes.size() == 1) {
                        compartmentToUse = *volumes.begin();
                    } else if (surfaces.size() == 1) {
                        compartmentToUse = *surfaces.begin();
                    }
                }
                if (compartmentToUse.empty() && sameCompartment) {
                    compartmentToUse = inferredCompartment;
                }
                if (!compartmentToUse.empty()) {
                    for (auto nodeIter = productGraph.getGraph().begin();
                         nodeIter != productGraph.getGraph().end(); ++nodeIter) {
                        if (isMoleculeNode(**nodeIter) && (*nodeIter)->get_compartment().empty()) {
                            (*nodeIter)->set_compartment(compartmentToUse);
                        }
                    }
                }
                auto prodSp = Species(productGraph, 0.0, false, compartmentToUse);
                const auto [index, wasNew] = speciesList.add(std::move(prodSp));
                productIndices.push_back(index);
            }
        }
        // else: Pure degradation rule (e.g., A() -> 0): no products
        // productIndices and productLabels remain empty
    } else {
        auto productGraphs = splitIntoSpeciesGraphs(aggregateGraph);

        // If the operation-based product construction produced the wrong number of
        // graphs AND the rule has AddMolecule operations, try building the "new" product
        // patterns directly (like synthesis). This handles rules like
        // Source() -> B(t!1).Receptor(Y!1) where bonds between new molecules are lost.
        // Only do this when the product graphs can't be matched to patterns via the
        // normal orphan handling (which handles DeleteMolecule cases correctly).
        if (hasAddMolecule && productGraphs.size() != productPatterns_.size()) {
            // First, try normal orphan handling (match patterns to graphs)
            bool orphanHandlingWorked = true;
            for (std::size_t pi = 0; pi < productPatterns_.size(); ++pi) {
                const auto& prodPat = productPatterns_[pi];
                bool found = false;
                for (std::size_t gi = 0; gi < productGraphs.size(); ++gi) {
                    BNGcore::UllmannSGIso matcher(prodPat.getGraph(), productGraphs[gi].getGraph());
                    BNGcore::List<BNGcore::Map> maps;
                    matcher.find_maps(maps);
                    if (maps.begin() != maps.end()) {
                        found = true;
                        break;
                    }
                }
                if (!found) {
                    orphanHandlingWorked = false;
                    break;
                }
            }
            if (!orphanHandlingWorked) {
                // Orphan handling can't match all product patterns to graphs.
                // This means bonds between newly-added molecules were lost.
                // Rebuild products directly from product patterns.
                productGraphs.clear();
                for (const auto& productPattern : productPatterns_) {
                    BNGcore::PatternGraph productGraph;
                    cloneGraphIntoTarget(productPattern.getGraph(), productGraph);
                    productGraphs.emplace_back(productGraph);
                }
            }
        }

        if (debug) {
            std::cerr << "[BUILD_RXN] after splitIntoSpeciesGraphs: " << productGraphs.size() << " graphs\n";
        }
        const bool deleteMolecules = hasModifier(modifiers_, "deletemolecules");

        // Track which product pattern each product graph corresponds to
        std::vector<std::size_t> productPatternIndices;

        // Check if the rule only has bond-related operations (DeleteBond, no DeleteMolecule/AddMolecule).
        // For pure bond-breaking rules (e.g., reverse of intramolecular rules), if breaking
        // the bond disconnects the complex into more fragments than product patterns, the
        // reaction is invalid (bridge bond in non-cyclic complex — not a ring opening).
        // Rules with DeleteMolecule operations (degradation) may legitimately produce orphan
        // fragments that should be discarded.
        bool isPureBondRule = true;
        for (const auto& op : operations_) {
            if (op.type == TransformOp::Type::DeleteMolecule ||
                op.type == TransformOp::Type::AddMolecule) {
                isPureBondRule = false;
                break;
            }
        }

        if (productGraphs.size() > productPatterns_.size()) {
            // More product components than product patterns.  Match each product
            // pattern to a product graph; unmatched graphs are orphan fragments.
            //
            // Three sub-cases determine what happens to orphans:
            //  (a) deleteMolecules modifier is set:
            //      The user explicitly requested DeleteMolecules — only the matched
            //      molecule is deleted, orphans are valid released molecules.
            //  (b) hasMoleculeTypeMismatch_ (and !deleteMolecules):
            //      The rule replaces one molecule with a different type (e.g.,
            //      FGFR(s~P)->Internalized_Rec()) or the product has extra
            //      components (e.g., BetaR(l,g,loc~cyt)->BetaR(l,g,loc~mem,s~U)).
            //      Bonded partners of the replaced/transformed molecule become
            //      orphans that should be DISCARDED (Perl behavior: they do not
            //      appear as products).
            //  (c) Rule has DeleteMolecule operations (unmatched reactant molecules
            //      mapped to null/0 product, e.g., RevErb(b)+Bmal1()->RevErb(b)+0)
            //      WITHOUT the DeleteMolecules modifier.  In Perl BNG2, -> 0 means
            //      the entire connected component containing the deleted molecule is
            //      destroyed.  Orphans should be DISCARDED (not kept as products).
            //      Only bond-breaking (no molecule deletion) should keep orphans.
            bool hasDeleteMoleculeOps = false;
            for (const auto& op : operations_) {
                if (op.type == TransformOp::Type::DeleteMolecule) {
                    hasDeleteMoleculeOps = true;
                    break;
                }
            }
            // MoveConnected rules: when a compartment-transport rule with
            // MoveConnected produces orphan fragments (from product-only bond
            // breaking), Perl BNG2 does NOT generate the reaction.  The rule
            // simply doesn't match species where applying it would disconnect
            // connected molecules.  Reject such reactions here.
            if (hasModifier(modifiers_, "MoveConnected") && !deleteMolecules) {
                return false;
            }
            const bool keepOrphans = deleteMolecules
                || (!hasMoleculeTypeMismatch_ && !hasDeleteMoleculeOps);
            std::vector<SpeciesGraph> orderedProducts;
            std::vector<bool> used(productGraphs.size(), false);
            for (std::size_t pi = 0; pi < productPatterns_.size(); ++pi) {
                const auto& prodPat = productPatterns_[pi];
                bool found = false;
                for (std::size_t gi = 0; gi < productGraphs.size(); ++gi) {
                    if (used[gi]) continue;
                    BNGcore::UllmannSGIso matcher(prodPat.getGraph(), productGraphs[gi].getGraph());
                    BNGcore::List<BNGcore::Map> maps;
                    matcher.find_maps(maps);
                    if (maps.begin() != maps.end()) {
                        orderedProducts.push_back(std::move(productGraphs[gi]));
                        productPatternIndices.push_back(pi);
                        used[gi] = true;
                        found = true;
                        break;
                    }
                }
                if (!found) {
                    return false;
                }
            }
            if (keepOrphans) {
                // Append orphan fragments (released molecules) after the matched products
                for (std::size_t gi = 0; gi < productGraphs.size(); ++gi) {
                    if (!used[gi]) {
                        orderedProducts.push_back(std::move(productGraphs[gi]));
                        // Orphans don't correspond to any product pattern; use
                        // productPatterns_.size() as sentinel so compartment
                        // inference below falls through to molecule-level logic.
                        productPatternIndices.push_back(productPatterns_.size());
                    }
                }
            }
            // else: hasMoleculeTypeMismatch_ — orphans are silently discarded
            productGraphs = std::move(orderedProducts);
        } else if (productGraphs.size() < productPatterns_.size() && !deleteMolecules) {
            // Reject: fewer products than patterns (unexpected).
            if (debug) {
                std::cerr << "[BUILD_RXN] FAIL: rule=" << ruleName_
                          << " productGraphs.size()=" << productGraphs.size()
                          << " < productPatterns_.size()=" << productPatterns_.size()
                          << " hasMoleculeTypeMismatch=" << hasMoleculeTypeMismatch_ << "\n";
            }
            return false;
        } else {
            // All product patterns correspond to product graphs in order
            for (std::size_t i = 0; i < productPatterns_.size(); ++i) {
                productPatternIndices.push_back(i);
            }
        }

        // Apply product-side filters (from reverse rule modifier transformation)
        if (!productFilters_.empty() && !passesProductFilters(productGraphs)) {
            return false;
        }

        for (std::size_t i = 0; i < productGraphs.size(); ++i) {
            auto& productGraph = productGraphs[i];
            if (productFilter && !productFilter(productGraph)) {
                return false;
            }
            productLabels.push_back(productGraph.canonicalLabel());
            // Perl-faithful inferSpeciesCompartment (SpeciesGraph.pm:795-893):
            // Collect unique 2D surfaces and 3D volumes from molecule compartments.
            // 0 surfaces: 1 volume → that volume; 0 volumes → undefined; >1 → first alphabetically
            // 1 surface → that surface
            // >1 surfaces → first alphabetically (Perl errors, we tolerate)
            std::string compartmentToUse;
            const auto& patternCompartment = productPatternIndices[i] < productPatterns_.size()
                ? productPatterns_[productPatternIndices[i]].getCompartment()
                : std::string();
            if (!g_compartmentDimensions.empty()) {
                // Perl's inferSpeciesCompartment transfers the species-level compartment
                // to molecules without explicit compartment BEFORE gathering. This is
                // necessary for synthesis rules like `Source -> @PM:Mol1@CP.Mol2` where
                // Mol2 inherits @PM from the species-level prefix.
                std::set<std::string> surfaces, volumes;
                for (auto nodeIter = productGraph.getGraph().begin(); nodeIter != productGraph.getGraph().end(); ++nodeIter) {
                    if (!isMoleculeNode(**nodeIter)) continue;
                    auto mc = (*nodeIter)->get_compartment();
                    if (mc.empty() && !patternCompartment.empty()) {
                        mc = patternCompartment;
                    }
                    if (mc.empty()) continue;
                    auto dimIt = g_compartmentDimensions.find(mc);
                    if (dimIt != g_compartmentDimensions.end()) {
                        if (dimIt->second == 2) surfaces.insert(mc);
                        else if (dimIt->second == 3) volumes.insert(mc);
                    }
                }
                if (surfaces.empty()) {
                    if (volumes.size() == 1) {
                        compartmentToUse = *volumes.begin();
                    } else if (volumes.size() > 1) {
                        return false;
                    }
                    // volumes.empty() → no inference from molecules
                } else if (surfaces.size() == 1) {
                    const auto& surface = *surfaces.begin();
                    // Perl checks all volumes are adjacent to the surface.
                    // Reject if any volume is not adjacent (invalid compartment topology).
                    bool valid = true;
                    for (const auto& vol : volumes) {
                        if (!isAdjacentToSurface(vol, surface)) {
                            valid = false;
                            break;
                        }
                    }
                    if (!valid) {
                        return false; // Invalid species — reject like Perl
                    }
                    compartmentToUse = surface;
                } else {
                    return false;
                }
            }
            // Fallback: use product pattern compartment or inferred reactant compartment
            if (compartmentToUse.empty()) {
                if (!patternCompartment.empty()) {
                    compartmentToUse = patternCompartment;
                } else if (sameCompartment) {
                    compartmentToUse = inferredCompartment;
                }
            }
            // Assign inferred compartment to molecules with empty compartments
            // (mirrors Perl's assignCompartment in SpeciesGraph.pm:775-783)
            if (!compartmentToUse.empty()) {
                for (auto nodeIter = productGraph.getGraph().begin();
                     nodeIter != productGraph.getGraph().end(); ++nodeIter) {
                    if (isMoleculeNode(**nodeIter) && (*nodeIter)->get_compartment().empty()) {
                        (*nodeIter)->set_compartment(compartmentToUse);
                    }
                }
            }
            // Perl's verifyTopology: reject products with cross-compartment bonds
            if (!verifyBondTopology(productGraph.getGraph())) {
                return false;
            }
            // Product pattern bond constraint check (Perl parity):
            // When a product pattern mentions a component as explicitly UNBOUND
            // and that component is NOT a target of any operation (AddBond,
            // DeleteBond, ChangeState), it acts as a constraint: the corresponding
            // component in the actual product species must also be unbound.
            // If the reactant species has that component bonded and no operation
            // breaks it, the product will still be bonded, violating the constraint.
            // This prevents rules like GPCR(b!1,loc~mem).Arrestin(b!1) ->
            // GPCR(l,b!1,loc~cyt,s~P).Arrestin(b!1) from applying to species where
            // GPCR has 'l' bonded: the product pattern says 'l' should be unbound.
            // (Product pattern bond constraint check would go here in a future version)
            auto prodSp = Species(productGraph, 0.0, false, compartmentToUse);
            const auto [index, wasNew] = speciesList.add(std::move(prodSp));
            productIndices.push_back(index);
        }
    }
    std::sort(productLabels.begin(), productLabels.end());

    // Do NOT sort reactantIndices or productIndices — preserve BNGL pattern order (Perl compatibility)

    // Compute statistical factor using Perl-compatible MultScale convention:
    // For each group of n identical reactant patterns (same canonical label),
    // multiply the stat factor by 1/n!.  This accounts for:
    //  - Identical reactive patterns (e.g., A(b)+A(b): 1/2!)
    //  - Identical context patterns (e.g., B()+B(): 1/2!)
    //  - Multiple groups (e.g., A(b)+A(b)+B()+B(): 1/2! * 1/2! = 0.25)
    // Duplicate reaction instances from permuted pattern-to-species assignments
    // are merged by rxnList dedup (stat factors sum), which combined with the
    // 1/n! correction produces the correct combinatorial rate.
    //
    // For degradation rules with DeleteMolecules modifier (-> 0 DeleteMolecules),
    // the initialize() function returns early, leaving operations_ and reactionCenter_
    // empty.  In this case, automorphic Ullmann maps that collapse to the same
    // embedding signature genuinely represent distinct reaction pathways (e.g.,
    // DeleteMolecules on a symmetric dimer can remove either half).  Perl
    // generates a separate reaction for each, which get merged with additive
    // factors.  We replicate this by using the embedding multiplicity.
    //
    // For pure degradation (-> 0 without DeleteMolecules), the entire species is
    // consumed regardless of how many embeddings exist.  All embeddings collapse
    // to the same reaction (species → trash), so multiplicity must NOT be applied.
    //
    // For all other rules, the reaction center signature and/or the normal
    // RxnList dedup already handle symmetry correctly.
    double factor = 1.0;
    // NOTE: For -> 0 DeleteMolecules rules (operations_ empty, productPatterns_
    // empty), we intentionally do NOT apply the embedding multiplicity as a stat
    // factor.  The multiplicity counts automorphic embeddings that map the pattern
    // to the same set of species molecules in different orientations.  When
    // embeddings map to DIFFERENT molecules (e.g., A() on a symmetric dimer picks
    // one A or the other), they produce separate EmbeddingResults (distinct
    // signatures), each calling buildReaction independently; the reactions get
    // merged by RxnList dedup with additive factors.  When embeddings map to the
    // SAME set of molecules (e.g., EGFR symmetric dimer pattern on a non-symmetric
    // complex), they collapse into one EmbeddingResult with multiplicity>1.  These
    // represent the same physical reaction and should have factor 1, not
    // multiplicity.  In Perl each automorphic embedding carries a 1/auto_pattern
    // correction that exactly cancels the multiplicity, yielding factor 1.
    if (reactantPatterns_.size() > 1) {
        // Group reactant patterns by canonical label, compute 1/n! per group
        std::unordered_map<std::string, int> patternGroupCounts;
        for (const auto& rp : reactantPatterns_) {
            patternGroupCounts[rp.canonicalLabel()]++;
        }
        for (const auto& [label, count] : patternGroupCounts) {
            if (count > 1) {
                // 1/n! for this group
                int factorial = 1;
                for (int i = 2; i <= count; ++i) factorial *= i;
                factor /= static_cast<double>(factorial);
            }
        }
    }

    // For scope-prefix rules, compute observable-count fingerprint for the scoped molecule
    // and include it in the rate law string. This prevents RxnList from merging
    // reactions with different local function contexts.
    std::string rateLawStr = rates_.empty() ? "0" : rates_.front().toString();
    const bool hasLocalFuncRate = model != nullptr && !rates_.empty() &&
        rates_.front().toString().find('(') != std::string::npos;
    if (hasLocalFuncRate && !matchSet.empty()) {
        const std::string rateName = rates_.front().toString();
        const auto& speciesGraph = speciesList.get(matchSet[0].speciesIndex).getSpeciesGraph().getGraph();
        for (auto pn = reactantPatterns_[0].getGraph().begin();
             pn != reactantPatterns_[0].getGraph().end(); ++pn) {
            if (isMoleculeNode(**pn)) {
                auto* target = matchSet[0].map.mapf(*pn);
                if (target) {
                    std::string fp = computeLocalObsFingerprint(
                        target, speciesGraph, *model, rateName);
                    if (!fp.empty()) {
                        rateLawStr += "|local:" + fp;
                    }
                }
                break;
            }
        }
    }

    const std::string label = reactionLabel(*this, reactantIndices, productLabels);
    const bool added = rxnList.add(Rxn(
        label,
        reactantIndices,
        productIndices,
        rateLawStr,
        factor,
        ruleName_,
        rates_.empty() ? std::nullopt : std::optional<Expression>(rates_.front())));
    return added;
}

void ReactionRule::parseReactantFilters() {
    reactantFilters_.clear();
    productFilters_.clear();
    for (const auto& mod : modifiers_) {
        ReactantFilter::Type type;
        ReactantFilter::Side side;
        if (mod.find("include_reactants(") == 0 || mod.find("include_reactants (") == 0) {
            type = ReactantFilter::Type::Include;
            side = ReactantFilter::Side::Reactant;
        } else if (mod.find("exclude_reactants(") == 0 || mod.find("exclude_reactants (") == 0) {
            type = ReactantFilter::Type::Exclude;
            side = ReactantFilter::Side::Reactant;
        } else if (mod.find("include_products(") == 0 || mod.find("include_products (") == 0) {
            type = ReactantFilter::Type::Include;
            side = ReactantFilter::Side::Product;
        } else if (mod.find("exclude_products(") == 0 || mod.find("exclude_products (") == 0) {
            type = ReactantFilter::Type::Exclude;
            side = ReactantFilter::Side::Product;
        } else {
            continue;
        }
        // Parse: prefix(INT,PATTERN)
        const auto lparen = mod.find('(');
        const auto rparen = mod.rfind(')');
        if (lparen == std::string::npos || rparen == std::string::npos) continue;
        const auto inner = mod.substr(lparen + 1, rparen - lparen - 1);
        const auto comma = inner.find(',');
        if (comma == std::string::npos) continue;
        const auto indexStr = inner.substr(0, comma);
        auto molName = inner.substr(comma + 1);
        // Trim whitespace
        molName.erase(0, molName.find_first_not_of(" \t"));
        molName.erase(molName.find_last_not_of(" \t") + 1);
        // Remove any trailing component list like "()" for simple molecule name matching
        const auto parenPos = molName.find('(');
        if (parenPos != std::string::npos) {
            molName = molName.substr(0, parenPos);
        }
        try {
            const std::size_t idx = std::stoul(indexStr) - 1; // Convert 1-based to 0-based
            ReactantFilter filter{type, side, idx, molName};
            if (side == ReactantFilter::Side::Reactant) {
                reactantFilters_.push_back(std::move(filter));
            } else {
                productFilters_.push_back(std::move(filter));
            }
        } catch (...) {}
    }
}

bool ReactionRule::passesReactantFilters(std::size_t patternIndex, const SpeciesGraph& species) const {
    for (const auto& filter : reactantFilters_) {
        if (filter.patternIndex != patternIndex) continue;

        // Check if species contains a molecule with the given name
        bool containsMolecule = false;
        const auto& graph = species.getGraph();
        for (auto nodeIter = graph.begin(); nodeIter != graph.end(); ++nodeIter) {
            if (isMoleculeNode(**nodeIter) &&
                (*nodeIter)->get_type().get_type_name() == filter.moleculeName) {
                containsMolecule = true;
                break;
            }
        }

        if (filter.type == ReactantFilter::Type::Include && !containsMolecule) {
            return false; // Must contain molecule but doesn't
        }
        if (filter.type == ReactantFilter::Type::Exclude && containsMolecule) {
            return false; // Must not contain molecule but does
        }
    }
    return true;
}

bool ReactionRule::passesProductFilters(const std::vector<SpeciesGraph>& products) const {
    for (const auto& filter : productFilters_) {
        if (filter.patternIndex >= products.size()) continue;
        const auto& product = products[filter.patternIndex];

        // Check if product contains a molecule with the given name
        bool containsMolecule = false;
        const auto& graph = product.getGraph();
        for (auto nodeIter = graph.begin(); nodeIter != graph.end(); ++nodeIter) {
            if (isMoleculeNode(**nodeIter) &&
                (*nodeIter)->get_type().get_type_name() == filter.moleculeName) {
                containsMolecule = true;
                break;
            }
        }

        if (filter.type == ReactantFilter::Type::Include && !containsMolecule) {
            return false; // Product must contain molecule but doesn't
        }
        if (filter.type == ReactantFilter::Type::Exclude && containsMolecule) {
            return false; // Product must not contain molecule but does
        }
    }
    return true;
}

} // namespace bng::ast



