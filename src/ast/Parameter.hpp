#pragma once

#include <string>

#include "Expression.hpp"

namespace bng::ast {

class Parameter {
public:
    Parameter(std::string name, Expression expression);

    const std::string& getName() const;
    const Expression& getExpression() const;
    double getValue() const;
    bool hasValue() const;
    void setValue(double value);
    void clearValue();

private:
    std::string name_;
    Expression expression_;
    double value_;
    bool hasValue_;
};

} // namespace bng::ast
