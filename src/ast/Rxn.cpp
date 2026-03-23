#include "Rxn.hpp"

#include <utility>

namespace bng::ast {

Rxn::Rxn(
    std::string label,
    std::vector<std::size_t> reactants,
    std::vector<std::size_t> products,
    std::string rateLaw,
    double factor,
    std::string originRuleName)
    : label_(std::move(label)),
      reactants_(std::move(reactants)),
      products_(std::move(products)),
      rateLaw_(std::move(rateLaw)),
      factor_(factor),
      originRuleName_(std::move(originRuleName)) {}

const std::string& Rxn::getLabel() const {
    return label_;
}

const std::vector<std::size_t>& Rxn::getReactants() const {
    return reactants_;
}

const std::vector<std::size_t>& Rxn::getProducts() const {
    return products_;
}

const std::string& Rxn::getRateLaw() const {
    return rateLaw_;
}

const std::string& Rxn::getOriginRuleName() const {
    return originRuleName_;
}

double Rxn::getFactor() const {
    return factor_;
}

void Rxn::addFactor(double factor) {
    factor_ += factor;
}

} // namespace bng::ast
