#pragma once

#include <string>

#include "core/BNGcore.hpp"
#include "Expression.hpp"

namespace bng::ast {

class SeedSpecies {
public:
    SeedSpecies(
        std::string pattern,
        Expression amount,
        bool constant = false,
        std::string compartment = {},
        BNGcore::PatternGraph graph = BNGcore::PatternGraph());

    const std::string& getPattern() const;
    const Expression& getAmount() const;
    bool isConstant() const;
    const std::string& getCompartment() const;
    const BNGcore::PatternGraph& getGraph() const;
    std::string getCanonicalLabel() const;

private:
    std::string pattern_;
    Expression amount_;
    bool constant_;
    std::string compartment_;
    BNGcore::PatternGraph graph_;
};

} // namespace bng::ast
