#include "ReactionRule.hpp"

#include <algorithm>
#include <cstdint>
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
    std::ostringstream out;
    out << rule.getLabel() << "|";
    for (std::size_t i = 0; i < reactants.size(); ++i) {
        if (i != 0) {
            out << ",";
        }
        out << reactants[i];
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
std::string embeddingSignature(const BNGcore::PatternGraph& pattern, const BNGcore::Map& map) {
    std::vector<std::string> parts;
    for (auto nodeIter = pattern.begin(); nodeIter != pattern.end(); ++nodeIter) {
        auto* target = map.mapf(*nodeIter);
        if (target == nullptr) {
            continue;
        }
        std::ostringstream out;
        out << (*nodeIter)->get_type().get_type_name()
            << "|" << (*nodeIter)->get_state().get_BNG2_string()
            << "->" << reinterpret_cast<std::uintptr_t>(target);
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
    const BNGcore::Map& map) {
    std::vector<std::string> parts;
    for (const auto& ref : reactionCenter) {
        BNGcore::Node* sourceNode = ref.isComponent
            ? infos.at(ref.patternIndex).molecules.at(ref.moleculeIndex).components.at(ref.componentIndex).node
            : infos.at(ref.patternIndex).molecules.at(ref.moleculeIndex).node;
        auto* target = map.mapf(sourceNode);
        if (target == nullptr) {
            continue;
        }
        std::ostringstream out;
        out << (ref.isComponent ? "c" : "m")
            << "|" << ref.moleculeIndex
            << "|" << (ref.isComponent ? std::to_string(ref.componentIndex) : std::string("-"))
            << "->" << reinterpret_cast<std::uintptr_t>(target);
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
    std::vector<EmbeddingResult> results;
    const auto& pattern = reactantPatterns_.at(patternIndex).getGraph();
    const auto reactantInfo = describePatterns(reactantPatterns_);
    std::unordered_set<std::string> seen;
    for (std::size_t speciesIndex = 0; speciesIndex < speciesList.size(); ++speciesIndex) {
        BNGcore::UllmannSGIso matcher(pattern, speciesList.get(speciesIndex).getSpeciesGraph().getGraph());
        BNGcore::List<BNGcore::Map> maps;
        matcher.find_maps(maps);
        for (auto mapIter = maps.begin(); mapIter != maps.end(); ++mapIter) {
            if (!embeddingRespectsNodeStates(pattern, *mapIter)) {
                continue;
            }
            const auto signature = std::to_string(speciesIndex) + "|" +
                (reactionCenter_.at(patternIndex).empty()
                    ? embeddingSignature(pattern, *mapIter)
                    : reactionCenterSignature(reactantInfo, reactionCenter_.at(patternIndex), *mapIter));
            if (!seen.insert(signature).second) {
                continue;
            }
            results.push_back(EmbeddingResult {speciesIndex, *mapIter});
        }
    }
    return results;
}

std::size_t ReactionRule::expandRule(
    SpeciesList& speciesList,
    RxnList& rxnList,
    const std::function<bool(const SpeciesGraph&)>& productFilter) const {
    if (bidirectional_ && !rates_.empty()) {
        ReactionRule reverseRule(
            std::string("_reverse__") + ruleName_,
            label_.empty() ? std::string("_reverse") : std::string("_reverse__") + label_,
            products_,
            reactants_,
            {rates_.size() > 1 ? rates_[1] : rates_.front()},
            modifiers_,
            false,
            productPatterns_,
            reactantPatterns_);
        const auto reverseCreated = reverseRule.expandRule(speciesList, rxnList, productFilter);
        (void)reverseCreated;
    }

    if (reactantPatterns_.empty()) {
        return 0;
    }

    std::vector<std::vector<EmbeddingResult>> allMatches;
    allMatches.reserve(reactantPatterns_.size());
    for (std::size_t i = 0; i < reactantPatterns_.size(); ++i) {
        allMatches.push_back(findEmbeddings(i, speciesList));
        if (allMatches.back().empty()) {
            return 0;
        }
    }

    std::size_t created = 0;
    std::vector<std::size_t> indices(allMatches.size(), 0);
    while (true) {
        std::vector<EmbeddingResult> matchSet;
        std::set<std::size_t> usedSpecies;
        bool validMatchSet = true;
        bool hasNewSpecies = reactantPatterns_.empty();
        for (std::size_t i = 0; i < indices.size(); ++i) {
            const auto& match = allMatches[i][indices[i]];
            if (!usedSpecies.insert(match.speciesIndex).second) {
                validMatchSet = false;
                break;
            }
            if (!speciesList.get(match.speciesIndex).rulesApplied()) {
                hasNewSpecies = true;
            }
            matchSet.push_back(match);
        }
        if (validMatchSet && hasNewSpecies) {
            const bool added = buildReaction(matchSet, speciesList, rxnList, productFilter);
            if (added) {
                ++created;
            }
        }

        std::size_t dim = 0;
        for (; dim < indices.size(); ++dim) {
            ++indices[dim];
            if (indices[dim] < allMatches[dim].size()) {
                break;
            }
            indices[dim] = 0;
        }
        if (dim == indices.size()) {
            break;
        }
    }
    return created;
}

bool ReactionRule::buildReaction(
    const std::vector<EmbeddingResult>& matchSet,
    SpeciesList& speciesList,
    RxnList& rxnList,
    const std::function<bool(const SpeciesGraph&)>& productFilter) const {
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
    for (auto& productGraph : splitIntoSpeciesGraphs(aggregateGraph)) {
        if (productFilter && !productFilter(productGraph)) {
            return false;
        }
        productLabels.push_back(productGraph.canonicalLabel());
        const auto [index, _] = speciesList.add(Species(
            productGraph,
            0.0,
            false,
            sameCompartment ? inferredCompartment : std::string()));
        productIndices.push_back(index);
    }
    std::sort(productLabels.begin(), productLabels.end());

    std::sort(reactantIndices.begin(), reactantIndices.end());
    std::sort(productIndices.begin(), productIndices.end());
    const std::string label = reactionLabel(*this, reactantIndices, productLabels);
    const bool added = rxnList.add(Rxn(
        label,
        reactantIndices,
        productIndices,
        rates_.empty() ? "0" : rates_.front().toString(),
        1.0,
        ruleName_));
    return added;
}

} // namespace bng::ast
