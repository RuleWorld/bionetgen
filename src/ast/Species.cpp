#include "Species.hpp"

#include <utility>

namespace bng::ast {

Species::Species(SpeciesGraph graph, double amount, bool constant, std::string compartment)
    : graph_(std::move(graph)),
      amount_(amount),
      constant_(constant),
      compartment_(std::move(compartment)) {}

const SpeciesGraph& Species::getSpeciesGraph() const {
    return graph_;
}

SpeciesGraph& Species::getSpeciesGraph() {
    return graph_;
}

double Species::getAmount() const {
    return amount_;
}

void Species::setAmount(double amount) {
    amount_ = amount;
}

bool Species::isConstant() const {
    return constant_;
}

const std::string& Species::getCompartment() const {
    return compartment_;
}

void Species::setCompartment(std::string compartment) {
    compartment_ = std::move(compartment);
}

std::size_t Species::getIndex() const {
    return index_;
}

void Species::setIndex(std::size_t index) {
    index_ = index;
}

bool Species::rulesApplied() const {
    return rulesApplied_;
}

void Species::setRulesApplied(bool value) {
    rulesApplied_ = value;
}

} // namespace bng::ast
