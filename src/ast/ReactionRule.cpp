#include "ReactionRule.hpp"

#include <algorithm>
#include <cstdint>
#include <iterator>
#include <map>
#include <memory>
#include <optional>
#include <set>
#include <sstream>
#include <stdexcept>
#include <tuple>
#include <unordered_map>
#include <unordered_set>
#include <utility>

#include "core/BNGcore.hpp"
#include "core/Ullmann.hpp"

namespace bng::ast {

namespace {

struct ComponentInfo {
    BNGcore::Node* node = nullptr;
    BNGcore::Node* bondNode = nullptr;
    std::string name;
    std::string state;
    std::optional<ReactionRule::ComponentRef> partner;
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
            for (auto bondEdge = component.node->edges_out_begin(); bondEdge != component.node->edges_out_end(); ++bondEdge) {
                if (isBondNode(**bondEdge)) {
                    component.bondNode = *bondEdge;
                    break;
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
            if (component.bondNode == nullptr || component.bondNode->get_state().get_BNG2_string() != "!+") {
                continue;
            }
            for (auto inEdge = component.bondNode->edges_in_begin(); inEdge != component.bondNode->edges_in_end(); ++inEdge) {
                if (*inEdge == component.node) {
                    continue;
                }
                const auto found = componentRefs.find(*inEdge);
                if (found != componentRefs.end()) {
                    component.partner = found->second;
                    break;
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
                if (!component.partner.has_value()) {
                    continue;
                }
                bonds.insert(canonicalBondPair(
                    ReactionRule::ComponentRef {patternIndex, moleculeIndex, componentIndex},
                    *component.partner));
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

void cloneProductMolecule(
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

        if (component.bondNode == nullptr) {
            continue;
        }

        auto bondFound = clones.find(component.bondNode);
        BNGcore::Node* bondClone = nullptr;
        if (bondFound == clones.end()) {
            bondClone = component.bondNode->clone();
            target.add_node(bondClone);
            clones[component.bondNode] = bondClone;
        } else {
            bondClone = bondFound->second;
        }
        target.add_edge(componentClone, bondClone);
    }
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
    }
    return true;
}

bool patternHasAnyBoundSite(const PatternInfo& info) {
    for (const auto& molecule : info.molecules) {
        for (const auto& component : molecule.components) {
            if (component.bondNode != nullptr &&
                component.bondNode->get_state().get_BNG2_string() == "!+") {
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
                               const BNGcore::PatternGraph& /*targetGraph*/) {
    // Use canonical node IDs for target nodes to collapse automorphic embeddings.
    // This deduplicates mappings that differ only in how pattern nodes are assigned
    // to equivalent target nodes (e.g., swapping two identical x components).
    std::vector<std::string> parts;
    for (auto nodeIter = pattern.begin(); nodeIter != pattern.end(); ++nodeIter) {
        auto* target = map.mapf(*nodeIter);
        if (target == nullptr) {
            continue;
        }
        std::ostringstream out;
        out << (*nodeIter)->get_type().get_type_name()
            << "|" << (*nodeIter)->get_state().get_BNG2_string()
            << "->" << canonicalNodeId(target);
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

    for (std::size_t i = 0; i < productMolecules.size(); ++i) {
        auto& bucket = unmatchedReactantsByKey[moleculeKey(productMolecules[i])];
        if (!bucket.empty()) {
            const std::size_t matchedIndex = bucket.front();
            bucket.erase(bucket.begin());
            productToReactant[i] = matchedIndex;
            reactantMatched[matchedIndex] = true;
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
            auto& bucket = reactantComponentsByName[productMol.components[i].name];
            if (bucket.empty()) {
                continue;
            }
            const std::size_t reactantComponentIndex = bucket.front();
            bucket.erase(bucket.begin());
            ComponentRef productRef {productMolRef.base.patternIndex, productMolRef.base.moleculeIndex, i};
            ComponentRef reactantRef {reactantMolRef.base.patternIndex, reactantMolRef.base.moleculeIndex, reactantComponentIndex};
            productToReactantComponent[productRef] = reactantRef;

            const auto& productComponent = productMol.components[i];
            const auto& reactantComponent = reactantMol.components[reactantComponentIndex];
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
    for (const auto& productBond : collectBonds(productInfo)) {
        const auto lhs = productToReactantComponent.find(productBond.first);
        const auto rhs = productToReactantComponent.find(productBond.second);
        if (lhs == productToReactantComponent.end() || rhs == productToReactantComponent.end()) {
            continue;
        }
        remappedProductBonds.insert(canonicalBondPair(lhs->second, rhs->second));
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
    std::unordered_set<std::size_t> allSpecies;
    for (std::size_t speciesIndex = 0; speciesIndex < speciesList.size(); ++speciesIndex) {
        allSpecies.insert(speciesIndex);
    }
    return findEmbeddingsForSpecies(patternIndex, speciesList, allSpecies);
}

std::vector<ReactionRule::EmbeddingResult> ReactionRule::findEmbeddingsForSpecies(
    std::size_t patternIndex,
    const SpeciesList& speciesList,
    const std::unordered_set<std::size_t>& candidateSpecies) const {
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

    for (std::size_t speciesIndex = 0; speciesIndex < speciesList.size(); ++speciesIndex) {
        if (candidateSpecies.find(speciesIndex) == candidateSpecies.end()) {
            continue;
        }
        // Skip species that have already been searched in previous iterations
        if (alreadySearchedSpecies.count(speciesIndex) > 0) {
            continue;
        }
        // Apply include/exclude reactant filters
        if (!passesReactantFilters(patternIndex, speciesList.get(speciesIndex).getSpeciesGraph())) {
            continue;
        }
        BNGcore::UllmannSGIso matcher(pattern, speciesList.get(speciesIndex).getSpeciesGraph().getGraph());
        BNGcore::List<BNGcore::Map> maps;
        matcher.find_maps(maps);
        std::size_t mapsBeforeDedup = 0;
        for (auto mapIter = maps.begin(); mapIter != maps.end(); ++mapIter) {
            ++mapsBeforeDedup;
        }

        // Track signature → index in results for multiplicity counting
        std::unordered_map<std::string, std::size_t> signatureToResultIndex;
        for (auto mapIter = maps.begin(); mapIter != maps.end(); ++mapIter) {
            if (!embeddingRespectsNodeStates(pattern, *mapIter)) {
                continue;
            }
            const auto& targetGraph = speciesList.get(speciesIndex).getSpeciesGraph().getGraph();
            const auto signature = std::to_string(speciesIndex) + "|" +
                (reactionCenter_.at(patternIndex).empty()
                    ? embeddingSignature(pattern, *mapIter, targetGraph)
                    : reactionCenterSignature(reactantInfo, reactionCenter_.at(patternIndex), *mapIter, targetGraph));
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
}

std::size_t ReactionRule::expandRule(
    SpeciesList& speciesList,
    RxnList& rxnList,
    std::size_t currentIteration,
    const std::function<bool(const SpeciesGraph&)>& productFilter) const {

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
        }
        reverseRule_->expandRule(speciesList, rxnList, currentIteration, productFilter);
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
    bool hasTrulyNewSpecies = false;
    for (std::size_t i = 0; i < speciesList.size(); ++i) {
        if (!speciesList.get(i).rulesApplied()) {
            hasTrulyNewSpecies = true;
            break;
        }
    }

    std::unordered_set<std::size_t> newSpecies;
    for (std::size_t i = 0; i < speciesList.size(); ++i) {
        const auto iter = lastProcessedInIteration_.find(i);
        const bool notYetProcessed = !speciesList.get(i).rulesApplied();
        const bool notProcessedByThisRuleInCurrentIteration =
            (iter == lastProcessedInIteration_.end() || iter->second < currentIteration);
        const bool neverProcessedByThisRule = (iter == lastProcessedInIteration_.end());

        // Unimolecular: only process species never seen by this rule before.
        // Using neverProcessedByThisRule (not just notProcessedInCurrentIteration)
        // prevents re-processing species across iterations, which would cause
        // duplicate reactions and doubled stat factors.
        // Bimolecular: process new species OR (if truly new species exist) old
        // species for cross-matching (old × new combinations).
        if (!isBimolecular) {
            // Unimolecular: process only never-before-seen species
            if (neverProcessedByThisRule && notYetProcessed) {
                newSpecies.insert(i);
            }
        } else {
            // Bimolecular: process new species or old species for cross-matching
            if (notYetProcessed && notProcessedByThisRuleInCurrentIteration) {
                newSpecies.insert(i);
            } else if (hasTrulyNewSpecies && speciesList.get(i).rulesApplied() &&
                       notProcessedByThisRuleInCurrentIteration) {
                newSpecies.insert(i);
            }
        }
    }
    if (debug) {
        std::cerr << "[DEBUG] Rule " << ruleName_ << " iter=" << currentIteration
                  << ": newSpecies={";
        for (auto idx : newSpecies) std::cerr << idx << ",";
        std::cerr << "} trackedCount=" << lastProcessedInIteration_.size() << "\n";
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
        std::unordered_set<std::size_t> searchSet;
        if (cacheNeedsRebuild) {
            for (std::size_t i = 0; i < speciesList.size(); ++i) {
                searchSet.insert(i);
            }
        } else {
            searchSet = newSpecies;
        }

        auto newMatches = findEmbeddingsForSpecies(patternIndex, speciesList, searchSet);

        firstNewPerPattern[patternIndex] = patternMatches_[patternIndex].size();

        if (cacheNeedsRebuild && !newMatches.empty()) {
            // Partition: "old" species (processed by this rule in a previous iteration)
            // go first; "new" species (never processed by this rule) go after.
            // This ensures the enumeration's trigger/non-trigger split correctly
            // avoids re-enumerating already-counted combinations.
            std::stable_partition(newMatches.begin(), newMatches.end(),
                [this](const EmbeddingResult& m) {
                    return lastProcessedInIteration_.count(m.speciesIndex) > 0;
                });

            std::size_t oldCount = 0;
            for (const auto& m : newMatches) {
                if (lastProcessedInIteration_.count(m.speciesIndex) > 0) {
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
                if (buildReaction(matchSet, speciesList, rxnList, productFilter)) {
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
        lastProcessedInIteration_[idx] = currentIteration;
    }

    return created;
}

bool ReactionRule::buildReaction(
    const std::vector<EmbeddingResult>& matchSet,
    SpeciesList& speciesList,
    RxnList& rxnList,
    const std::function<bool(const SpeciesGraph&)>& productFilter) const {
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

    for (const auto& operation : operations_) {
        switch (operation.type) {
        case TransformOp::Type::AddBond: {
            const auto& lhsInfo = getComponentInfo(reactantInfo, operation.source);
            const auto& rhsInfo = getComponentInfo(reactantInfo, operation.partner);
            if (lhsInfo.bondNode == nullptr || rhsInfo.bondNode == nullptr) {
                break;
            }
            auto* lhsComponent = mapMatchedNodeToAggregate(operation.source, lhsInfo.node);
            auto* rhsComponent = mapMatchedNodeToAggregate(operation.partner, rhsInfo.node);
            auto* lhsBond = findAttachedBondNode(lhsComponent);
            auto* rhsBond = findAttachedBondNode(rhsComponent);
            if (lhsComponent == nullptr || rhsComponent == nullptr || lhsBond == nullptr || rhsBond == nullptr) {
                return false;
            }
            safeDeleteNode(*graph, lhsBond);
            safeDeleteNode(*graph, rhsBond);
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
            if (lhsInfo.bondNode == nullptr) {
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
        case TransformOp::Type::AddMolecule:
            cloneProductMolecule(productInfo[operation.patternIndex], operation.moleculeIndex, aggregateGraph);
            break;
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

    if (productPatterns_.empty() && !reactantPatterns_.empty()) {
        // Degradation rule (e.g., A() -> 0): no products
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

        if (productGraphs.size() > productPatterns_.size() && !deleteMolecules && !isPureBondRule) {
            // More product components than product patterns: this happens when a rule
            // transforms part of a complex (e.g., D() -> Trash() matching in a dimer).
            // In BNG2, orphaned components (not corresponding to any product pattern) are
            // destroyed. Match product patterns to product graphs and keep only matched ones.
            std::vector<SpeciesGraph> matchedProducts;
            std::vector<bool> used(productGraphs.size(), false);
            for (std::size_t pi = 0; pi < productPatterns_.size(); ++pi) {
                const auto& prodPat = productPatterns_[pi];
                bool found = false;
                // Try to match each product pattern to a product graph by subgraph isomorphism
                for (std::size_t gi = 0; gi < productGraphs.size(); ++gi) {
                    if (used[gi]) continue;
                    BNGcore::UllmannSGIso matcher(prodPat.getGraph(), productGraphs[gi].getGraph());
                    BNGcore::List<BNGcore::Map> maps;
                    matcher.find_maps(maps);
                    if (maps.begin() != maps.end()) {
                        matchedProducts.push_back(std::move(productGraphs[gi]));
                        productPatternIndices.push_back(pi);
                        used[gi] = true;
                        found = true;
                        break;
                    }
                }
                if (!found) {
                    return false;  // Product pattern has no matching graph
                }
            }
            productGraphs = std::move(matchedProducts);
        } else if (productGraphs.size() != productPatterns_.size() && !deleteMolecules) {
            if (debug) {
                std::cerr << "[BUILD_RXN] FAIL: productGraphs.size()=" << productGraphs.size()
                          << " != productPatterns_.size()=" << productPatterns_.size() << "\n";
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
            // Use product pattern's compartment if available, otherwise use inferred compartment
            const auto& patternCompartment = productPatternIndices[i] < productPatterns_.size()
                ? productPatterns_[productPatternIndices[i]].getCompartment()
                : std::string();
            const auto& compartmentToUse = !patternCompartment.empty()
                ? patternCompartment
                : (sameCompartment ? inferredCompartment : std::string());
            const auto [index, _] = speciesList.add(Species(
                productGraph,
                0.0,
                false,
                compartmentToUse));
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
    // Note: we do NOT multiply by per-pattern multiplicity (automorphic Ullmann
    // map count).  Embeddings that represent distinct reaction pathways already
    // have distinct reaction-center signatures and are kept as separate
    // embeddings.  Collapsed automorphisms (multiplicity > 1) are redundant
    // mappings that should not inflate the rate.
    double factor = 1.0;
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

    const std::string label = reactionLabel(*this, reactantIndices, productLabels);
    const bool added = rxnList.add(Rxn(
        label,
        reactantIndices,
        productIndices,
        rates_.empty() ? "0" : rates_.front().toString(),
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



