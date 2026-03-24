#pragma once

#include <optional>
#include <string>
#include <vector>

#include "Expression.hpp"

namespace bng::ast {

class Rxn {
public:
    Rxn(
        std::string label,
        std::vector<std::size_t> reactants,
        std::vector<std::size_t> products,
        std::string rateLaw,
        double factor = 1.0,
        std::string originRuleName = {},
        std::optional<Expression> rateExpression = std::nullopt);

    const std::string& getLabel() const;
    const std::vector<std::size_t>& getReactants() const;
    const std::vector<std::size_t>& getProducts() const;
    const std::string& getRateLaw() const;
    const std::string& getOriginRuleName() const;
    double getFactor() const;
    void addFactor(double factor);
    const std::optional<Expression>& getRateExpression() const;

private:
    std::string label_;
    std::vector<std::size_t> reactants_;
    std::vector<std::size_t> products_;
    std::string rateLaw_;
    double factor_;
    std::string originRuleName_;
    std::optional<Expression> rateExpression_;
};

} // namespace bng::ast
