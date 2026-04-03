#include "Function.hpp"

#include <utility>

namespace bng::ast {

Function::Function(std::string name, std::vector<std::string> args, Expression expression)
    : name_(std::move(name)), args_(std::move(args)), expression_(std::move(expression)) {}

const std::string& Function::getName() const {
    return name_;
}

const std::vector<std::string>& Function::getArgs() const {
    return args_;
}

const Expression& Function::getExpression() const {
    return expression_;
}

} // namespace bng::ast
