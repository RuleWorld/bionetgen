#include "MoleculeType.hpp"

#include <utility>

namespace bng::ast {

MoleculeType::MoleculeType(std::string name, std::vector<ComponentType> components, bool population)
    : name_(std::move(name)), components_(std::move(components)), population_(population) {}

const std::string& MoleculeType::getName() const {
    return name_;
}

const std::vector<ComponentType>& MoleculeType::getComponents() const {
    return components_;
}

bool MoleculeType::isPopulation() const {
    return population_;
}

} // namespace bng::ast
