#include "Molecule.hpp"
#include <utility>

namespace bng {
namespace ast {

Molecule::Molecule(std::string name)
    : name(std::move(name)) {}

std::string Molecule::getName() const {
    return name;
}

} // namespace ast
} // namespace bng
