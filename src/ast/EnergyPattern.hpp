#pragma once

#include <string>

#include "Expression.hpp"
#include "SpeciesGraph.hpp"

namespace bng::ast {

class EnergyPattern {
public:
    EnergyPattern(std::string label, std::string pattern, Expression expression, SpeciesGraph graph);

    const std::string& getLabel() const;
    const std::string& getPattern() const;
    const Expression& getExpression() const;
    const SpeciesGraph& getGraph() const;

private:
    std::string label_;
    std::string pattern_;
    Expression expression_;
    SpeciesGraph graph_;
};

} // namespace bng::ast
