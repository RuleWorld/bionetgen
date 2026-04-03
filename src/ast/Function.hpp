#pragma once

#include <string>
#include <vector>

#include "Expression.hpp"

namespace bng::ast {

class Function {
public:
    Function(std::string name, std::vector<std::string> args, Expression expression);

    const std::string& getName() const;
    const std::vector<std::string>& getArgs() const;
    const Expression& getExpression() const;

private:
    std::string name_;
    std::vector<std::string> args_;
    Expression expression_;
};

} // namespace bng::ast
