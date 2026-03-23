#include "SeedSpecies.hpp"

#include <utility>

namespace bng::ast {

SeedSpecies::SeedSpecies(
    std::string pattern,
    Expression amount,
    bool constant,
    std::string compartment,
    BNGcore::PatternGraph graph)
    : pattern_(std::move(pattern)),
      amount_(std::move(amount)),
      constant_(constant),
      compartment_(std::move(compartment)),
      graph_(std::move(graph)) {}

const std::string& SeedSpecies::getPattern() const {
    return pattern_;
}

const Expression& SeedSpecies::getAmount() const {
    return amount_;
}

bool SeedSpecies::isConstant() const {
    return constant_;
}

const std::string& SeedSpecies::getCompartment() const {
    return compartment_;
}

const BNGcore::PatternGraph& SeedSpecies::getGraph() const {
    return graph_;
}

std::string SeedSpecies::getCanonicalLabel() const {
    return graph_.get_label();
}

} // namespace bng::ast
