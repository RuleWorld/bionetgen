#include "GraphTypeRegistry.hpp"

#include <unordered_map>

namespace bng::ast {

namespace {

std::string componentKey(const MoleculeType& moleculeType, const ComponentType& componentType) {
    return moleculeType.getName() + "::" + componentType.name;
}

} // namespace

const BNGcore::EntityType& GraphTypeRegistry::ensureMoleculeType(const MoleculeType& moleculeType) {
    const auto existing = moleculeTypes_.find(moleculeType.getName());
    if (existing != moleculeTypes_.end()) {
        return *existing->second;
    }

    auto moleculeNodeType = std::make_unique<BNGcore::EntityType>(
        moleculeType.getName(),
        BNGcore::ENTITY_NODE_TYPE,
        BNGcore::NULL_STATE_TYPE);

    BNGcore::EntityType* rawNodeType = moleculeNodeType.get();
    moleculeTypes_[moleculeType.getName()] = std::move(moleculeNodeType);

    std::unordered_map<std::string, int> multiplicityByComponent;
    for (const auto& component : moleculeType.getComponents()) {
        ++multiplicityByComponent[component.name];
    }
    for (const auto& component : moleculeType.getComponents()) {
        const auto& componentNodeType = ensureComponentType(moleculeType, component);
        rawNodeType->add_edges_out(const_cast<BNGcore::EntityType&>(componentNodeType), multiplicityByComponent[component.name]);
    }

    return *rawNodeType;
}

const BNGcore::EntityType& GraphTypeRegistry::ensureComponentType(const MoleculeType& moleculeType, const ComponentType& componentType) {
    const auto key = componentKey(moleculeType, componentType);
    const auto existing = componentTypes_.find(key);
    if (existing != componentTypes_.end()) {
        return *existing->second.nodeType;
    }

    ComponentRuntimeType runtimeType;
    const BNGcore::StateType* stateType = &BNGcore::NULL_STATE_TYPE;
    if (!componentType.allowedStates.empty()) {
        auto labelType = std::make_unique<BNGcore::LabelStateType>(key, componentType.allowedStates.front());
        for (std::size_t i = 1; i < componentType.allowedStates.size(); ++i) {
            labelType->add_state(componentType.allowedStates[i]);
        }
        stateType = labelType.get();
        runtimeType.stateType = std::move(labelType);
    }

    runtimeType.nodeType = std::make_unique<BNGcore::EntityType>(componentType.name, BNGcore::COMPONENT_NODE_TYPE, *stateType);
    runtimeType.nodeType->add_edges_out(BNGcore::BOND_NODE_TYPE, 1);
    auto moleculeIter = moleculeTypes_.find(moleculeType.getName());
    if (moleculeIter != moleculeTypes_.end()) {
        runtimeType.nodeType->add_edges_in(*moleculeIter->second, 1);
    }

    BNGcore::EntityType* rawNodeType = runtimeType.nodeType.get();
    componentTypes_[key] = std::move(runtimeType);
    return *rawNodeType;
}

void GraphTypeRegistry::mergeFrom(GraphTypeRegistry& other) {
    // Transfer molecule types (skip duplicates — keep ours)
    for (auto& [name, ptr] : other.moleculeTypes_) {
        if (moleculeTypes_.find(name) == moleculeTypes_.end()) {
            moleculeTypes_[name] = std::move(ptr);
        }
    }
    // Transfer component types
    for (auto& [key, rt] : other.componentTypes_) {
        if (componentTypes_.find(key) == componentTypes_.end()) {
            componentTypes_[key] = std::move(rt);
        }
    }
}

} // namespace bng::ast
