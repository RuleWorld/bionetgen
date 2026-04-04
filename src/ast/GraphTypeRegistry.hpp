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

    /// Move all type entries from another registry into this one.
    /// Entries already present in this registry are skipped (the other's pointer
    /// is kept alive but our existing pointer wins).
    /// This keeps PatternGraph node pointers valid when the source Model is destroyed.
    void mergeFrom(GraphTypeRegistry& other);

private:
    struct ComponentRuntimeType {
        std::unique_ptr<BNGcore::StateType> stateType;
        std::unique_ptr<BNGcore::EntityType> nodeType;
    };

    std::unordered_map<std::string, std::unique_ptr<BNGcore::EntityType>> moleculeTypes_;
    std::unordered_map<std::string, ComponentRuntimeType> componentTypes_;
};

} // namespace bng::ast
