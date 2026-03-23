#pragma once

#include <string>
#include <vector>

namespace bng::ast {

struct ComponentType {
    std::string name;
    std::vector<std::string> allowedStates;
};

class MoleculeType {
public:
    MoleculeType(std::string name, std::vector<ComponentType> components, bool population = false);

    const std::string& getName() const;
    const std::vector<ComponentType>& getComponents() const;
    bool isPopulation() const;

private:
    std::string name_;
    std::vector<ComponentType> components_;
    bool population_;
};

} // namespace bng::ast
