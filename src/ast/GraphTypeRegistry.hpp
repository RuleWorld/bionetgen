#pragma once

#include <memory>
#include <string>
#include <unordered_map>

#include "MoleculeType.hpp"
#include "core/BNGcore.hpp"

namespace bng::ast {

class GraphTypeRegistry {
public:
    const BNGcore::EntityType& ensureMoleculeType(const MoleculeType& moleculeType);
    const BNGcore::EntityType& ensureComponentType(const MoleculeType& moleculeType, const ComponentType& componentType);

private:
    struct ComponentRuntimeType {
        std::unique_ptr<BNGcore::StateType> stateType;
        std::unique_ptr<BNGcore::EntityType> nodeType;
    };

    std::unordered_map<std::string, std::unique_ptr<BNGcore::EntityType>> moleculeTypes_;
    std::unordered_map<std::string, ComponentRuntimeType> componentTypes_;
};

} // namespace bng::ast
