#pragma once

#include <functional>
#include <set>
#include <string>
#include <vector>

namespace bng::ast {

enum class ExpressionKind {
    Number,
    Identifier,
    Unary,
    Binary,
    Function,
    ObservableRef,
};

class Expression {
public:
    Expression();

    static Expression number(double value);
    static Expression identifier(std::string name);
    static Expression unary(std::string op, Expression operand);
    static Expression binary(std::string op, Expression lhs, Expression rhs);
    static Expression function(std::string name, std::vector<Expression> args);
    static Expression observableRef(std::string name, std::vector<Expression> args);

    double evaluate(const std::function<double(const std::string&)>& resolveIdentifier, double t = 0.0) const;
    std::set<std::string> getDependencies() const;
    std::string toString() const;

    ExpressionKind kind() const { return kind_; }
    const std::string& name() const { return text_; }
    const std::vector<Expression>& args() const { return children_; }

private:
    ExpressionKind kind_;
    double numberValue_;
    std::string text_;
    std::vector<Expression> children_;
};

} // namespace bng::ast
