#include "EnergyPattern.hpp"

#include <utility>

namespace bng::ast {

EnergyPattern::EnergyPattern(std::string label, std::string pattern, Expression expression, SpeciesGraph graph)
    : label_(std::move(label)),
      pattern_(std::move(pattern)),
      expression_(std::move(expression)),
      graph_(std::move(graph)) {}

const std::string& EnergyPattern::getLabel() const {
    return label_;
}

const std::string& EnergyPattern::getPattern() const {
    return pattern_;
}

const Expression& EnergyPattern::getExpression() const {
    return expression_;
}

const SpeciesGraph& EnergyPattern::getGraph() const {
    return graph_;
}

} // namespace bng::ast
