#pragma once
#include <string>

namespace bng {
namespace ast {

class Molecule {
public:
    Molecule(std::string name);
    std::string getName() const;

private:
    std::string name;
};

} // namespace ast
} // namespace bng
