#include "Expression.hpp"

#include <algorithm>
#include <cmath>
#include <sstream>
#include <stdexcept>

namespace bng::ast {

namespace {

double requireArity(const std::string& name, const std::vector<Expression>& args, std::size_t arity) {
    if (args.size() != arity) {
        throw std::runtime_error("Function '" + name + "' expects " + std::to_string(arity) + " argument(s)");
    }
    return 0.0;
}

std::string joinArgs(const std::vector<Expression>& args) {
    std::ostringstream out;
    for (std::size_t i = 0; i < args.size(); ++i) {
        if (i > 0) {
            out << ", ";
        }
        out << args[i].toString();
    }
    return out.str();
}

} // namespace

Expression::Expression()
    : kind_(ExpressionKind::Number), numberValue_(0.0) {}

Expression Expression::number(double value) {
    Expression expr;
    expr.kind_ = ExpressionKind::Number;
    expr.numberValue_ = value;
    return expr;
}

Expression Expression::identifier(std::string name) {
    Expression expr;
    expr.kind_ = ExpressionKind::Identifier;
    expr.text_ = std::move(name);
    return expr;
}

Expression Expression::unary(std::string op, Expression operand) {
    Expression expr;
    expr.kind_ = ExpressionKind::Unary;
    expr.text_ = std::move(op);
    expr.children_.push_back(std::move(operand));
    return expr;
}

Expression Expression::binary(std::string op, Expression lhs, Expression rhs) {
    Expression expr;
    expr.kind_ = ExpressionKind::Binary;
    expr.text_ = std::move(op);
    expr.children_.push_back(std::move(lhs));
    expr.children_.push_back(std::move(rhs));
    return expr;
}

Expression Expression::function(std::string name, std::vector<Expression> args) {
    Expression expr;
    expr.kind_ = ExpressionKind::Function;
    expr.text_ = std::move(name);
    expr.children_ = std::move(args);
    return expr;
}

Expression Expression::observableRef(std::string name, std::vector<Expression> args) {
    Expression expr;
    expr.kind_ = ExpressionKind::ObservableRef;
    expr.text_ = std::move(name);
    expr.children_ = std::move(args);
    return expr;
}

double Expression::evaluate(const std::function<double(const std::string&)>& resolveIdentifier, double t) const {
    switch (kind_) {
    case ExpressionKind::Number:
        return numberValue_;
    case ExpressionKind::Identifier:
        if (text_ == "time") {
            return t;
        }
        return resolveIdentifier(text_);
    case ExpressionKind::Unary: {
        const double value = children_.front().evaluate(resolveIdentifier, t);
        if (text_ == "+") {
            return value;
        }
        if (text_ == "-") {
            return -value;
        }
        if (text_ == "!" || text_ == "~") {
            return value == 0.0 ? 1.0 : 0.0;
        }
        throw std::runtime_error("Unsupported unary operator '" + text_ + "'");
    }
    case ExpressionKind::Binary: {
        const double lhs = children_[0].evaluate(resolveIdentifier, t);
        const double rhs = children_[1].evaluate(resolveIdentifier, t);

        if (text_ == "+") return lhs + rhs;
        if (text_ == "-") return lhs - rhs;
        if (text_ == "*") return lhs * rhs;
        if (text_ == "/") return lhs / rhs;
        if (text_ == "%") return std::fmod(lhs, rhs);
        if (text_ == "^" || text_ == "**") return std::pow(lhs, rhs);
        if (text_ == "==") return lhs == rhs ? 1.0 : 0.0;
        if (text_ == "!=" || text_ == "~=") return lhs != rhs ? 1.0 : 0.0;
        if (text_ == ">") return lhs > rhs ? 1.0 : 0.0;
        if (text_ == ">=") return lhs >= rhs ? 1.0 : 0.0;
        if (text_ == "<") return lhs < rhs ? 1.0 : 0.0;
        if (text_ == "<=") return lhs <= rhs ? 1.0 : 0.0;
        if (text_ == "&&") return (lhs != 0.0 && rhs != 0.0) ? 1.0 : 0.0;
        if (text_ == "||") return (lhs != 0.0 || rhs != 0.0) ? 1.0 : 0.0;
        throw std::runtime_error("Unsupported binary operator '" + text_ + "'");
    }
    case ExpressionKind::Function: {
        const auto evalArg = [&](std::size_t index) { return children_[index].evaluate(resolveIdentifier, t); };

        if (text_ == "time") {
            requireArity(text_, children_, 0);
            return t;
        }
        if (text_ == "exp") { requireArity(text_, children_, 1); return std::exp(evalArg(0)); }
        if (text_ == "ln") { requireArity(text_, children_, 1); return std::log(evalArg(0)); }
        if (text_ == "log10") { requireArity(text_, children_, 1); return std::log10(evalArg(0)); }
        if (text_ == "log2") { requireArity(text_, children_, 1); return std::log2(evalArg(0)); }
        if (text_ == "sqrt") { requireArity(text_, children_, 1); return std::sqrt(evalArg(0)); }
        if (text_ == "abs") { requireArity(text_, children_, 1); return std::abs(evalArg(0)); }
        if (text_ == "sin") { requireArity(text_, children_, 1); return std::sin(evalArg(0)); }
        if (text_ == "cos") { requireArity(text_, children_, 1); return std::cos(evalArg(0)); }
        if (text_ == "tan") { requireArity(text_, children_, 1); return std::tan(evalArg(0)); }
        if (text_ == "asin") { requireArity(text_, children_, 1); return std::asin(evalArg(0)); }
        if (text_ == "acos") { requireArity(text_, children_, 1); return std::acos(evalArg(0)); }
        if (text_ == "atan") { requireArity(text_, children_, 1); return std::atan(evalArg(0)); }
        if (text_ == "sinh") { requireArity(text_, children_, 1); return std::sinh(evalArg(0)); }
        if (text_ == "cosh") { requireArity(text_, children_, 1); return std::cosh(evalArg(0)); }
        if (text_ == "tanh") { requireArity(text_, children_, 1); return std::tanh(evalArg(0)); }
        if (text_ == "asinh") { requireArity(text_, children_, 1); return std::asinh(evalArg(0)); }
        if (text_ == "acosh") { requireArity(text_, children_, 1); return std::acosh(evalArg(0)); }
        if (text_ == "atanh") { requireArity(text_, children_, 1); return std::atanh(evalArg(0)); }
        if (text_ == "rint") { requireArity(text_, children_, 1); return std::rint(evalArg(0)); }
        if (text_ == "min") {
            if (children_.empty()) throw std::runtime_error("Function 'min' expects at least one argument");
            double value = evalArg(0);
            for (std::size_t i = 1; i < children_.size(); ++i) value = std::min(value, evalArg(i));
            return value;
        }
        if (text_ == "max") {
            if (children_.empty()) throw std::runtime_error("Function 'max' expects at least one argument");
            double value = evalArg(0);
            for (std::size_t i = 1; i < children_.size(); ++i) value = std::max(value, evalArg(i));
            return value;
        }
        if (text_ == "sum") {
            double total = 0.0;
            for (std::size_t i = 0; i < children_.size(); ++i) total += evalArg(i);
            return total;
        }
        if (text_ == "avg") {
            if (children_.empty()) throw std::runtime_error("Function 'avg' expects at least one argument");
            double total = 0.0;
            for (std::size_t i = 0; i < children_.size(); ++i) total += evalArg(i);
            return total / static_cast<double>(children_.size());
        }
        if (text_ == "if") {
            requireArity(text_, children_, 3);
            return evalArg(0) != 0.0 ? evalArg(1) : evalArg(2);
        }
        throw std::runtime_error("Unsupported function '" + text_ + "'");
    }
    case ExpressionKind::ObservableRef:
        throw std::runtime_error("Observable references are not evaluable yet: " + text_);
    }

    throw std::runtime_error("Unsupported expression kind");
}

std::set<std::string> Expression::getDependencies() const {
    std::set<std::string> deps;
    if (kind_ == ExpressionKind::Identifier && text_ != "time") {
        deps.insert(text_);
    }
    for (const auto& child : children_) {
        const auto childDeps = child.getDependencies();
        deps.insert(childDeps.begin(), childDeps.end());
    }
    return deps;
}

std::string Expression::toString() const {
    switch (kind_) {
    case ExpressionKind::Number: {
        std::ostringstream out;
        out << numberValue_;
        return out.str();
    }
    case ExpressionKind::Identifier:
        return text_;
    case ExpressionKind::Unary:
        return text_ + children_.front().toString();
    case ExpressionKind::Binary:
        return "(" + children_[0].toString() + " " + text_ + " " + children_[1].toString() + ")";
    case ExpressionKind::Function:
    case ExpressionKind::ObservableRef:
        return text_ + "(" + joinArgs(children_) + ")";
    }

    return {};
}

} // namespace bng::ast
