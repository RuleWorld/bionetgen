#include "Compartment.hpp"
#include <utility>

namespace bng {
namespace ast {

Compartment::Compartment(std::string name, double volume, int dimension, std::string parent)
    : name_(std::move(name)), volume_(volume), dimension_(dimension), parent_(std::move(parent)) {}

std::string Compartment::getName() const {
    return name_;
}

double Compartment::getVolume() const {
    return volume_;
}

void Compartment::setVolume(double volume) {
    volume_ = volume;
}

int Compartment::getDimension() const {
    return dimension_;
}

const std::string& Compartment::getParent() const {
    return parent_;
}

} // namespace ast
} // namespace bng
