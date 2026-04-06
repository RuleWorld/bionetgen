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

bool Compartment::isOutside(const std::string& other, const std::vector<Compartment>& all) const {
    for (const auto& c : all) {
        if (c.getName() == other && c.getParent() == name_) return true;
    }
    return false;
}

bool Compartment::isAdjacent(const std::string& other, const std::vector<Compartment>& all) const {
    // Adjacent if one is the parent of the other, or they share a parent surface
    if (parent_ == other) return true;
    for (const auto& c : all) {
        if (c.getName() == other && c.getParent() == name_) return true;
    }
    return false;
}

int Compartment::adjacency(const std::string& other, const std::vector<Compartment>& all) const {
    if (parent_ == other) return 1;   // this is inside other
    for (const auto& c : all) {
        if (c.getName() == other && c.getParent() == name_) return -1;  // other is inside this
    }
    return 0;  // no containment relationship
}

} // namespace ast
} // namespace bng
