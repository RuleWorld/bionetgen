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
        if (text_ == "^^") return (lhs != 0.0) != (rhs != 0.0) ? 1.0 : 0.0;
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
        if (text_ == "floor") { requireArity(text_, children_, 1); return std::floor(evalArg(0)); }
        if (text_ == "ceil") { requireArity(text_, children_, 1); return std::ceil(evalArg(0)); }
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
        if (text_ == "mratio") {
            requireArity(text_, children_, 3);
            const double a = evalArg(0);
            const double b = evalArg(1);
            const double z = evalArg(2);
            // Kummer's confluent hypergeometric function 1F1(a,b,z) via series expansion
            auto hypergeom1F1 = [](double aa, double bb, double zz) -> double {
                double sum = 1.0, term = 1.0;
                for (int k = 1; k <= 1000; ++k) {
                    term *= (aa + k - 1) * zz / ((bb + k - 1) * k);
                    sum += term;
                    if (std::abs(term) < 1e-15 * std::abs(sum)) break;
                }
                return sum;
            };
            const double M_ab = hypergeom1F1(a, b, z);
            const double M_a1b1 = hypergeom1F1(a + 1, b + 1, z);
            return (std::abs(M_ab) < 1e-300) ? 0.0 : M_a1b1 / M_ab;
        }
        // Rate law functions: Sat/MM/Hill/Arrhenius/FunctionProduct
        // Sat(Vmax, Km, S) -> Vmax * S / (Km + S)
        if (text_ == "Sat" || text_ == "sat") {
            requireArity(text_, children_, 3);
            const double Vmax = evalArg(0);
            const double Km = evalArg(1);
            const double S = evalArg(2);
            return Vmax * S / (Km + S);
        }
        // MM(kcat, Km, St, Et) -> self-consistent Michaelis-Menten (BNG2 parity)
        // Computes free substrate via quadratic: S = 0.5*(b + sqrt(b^2 + 4*St*Km))
        // where b = St - Km - Et, then rate = kcat * Et * S / (Km + S)
        if (text_ == "MM" || text_ == "mm") {
            if (children_.size() == 4) {
                const double kcat = evalArg(0);
                const double Km   = evalArg(1);
                const double St   = evalArg(2);  // total substrate
                const double Et   = evalArg(3);  // total enzyme
                const double b = St - Km - Et;
                const double S = 0.5 * (b + std::sqrt(b * b + 4.0 * St * Km));
                return kcat * Et * S / (Km + S);
            }
            // Fallback for 3-arg form (legacy): Vmax * S / (Km + S)
            requireArity(text_, children_, 3);
            const double Vmax = evalArg(0);
            const double Km = evalArg(1);
            const double S = evalArg(2);
            return Vmax * S / (Km + S);
        }
        // Hill(Vmax, Kh, n, S) -> Vmax * S^n / (Kh^n + S^n)
        if (text_ == "Hill" || text_ == "hill") {
            requireArity(text_, children_, 4);
            const double Vmax = evalArg(0);
            const double Kh = evalArg(1);
            const double n = evalArg(2);
            const double S = evalArg(3);
            const double Sn = std::pow(S, n);
            return Vmax * Sn / (std::pow(Kh, n) + Sn);
        }
        // Arrhenius(A, Ea) -> A * exp(-Ea / (kB * T))
        // kB = 1.3806488e-23 J/K, T = 298.15 K  =>  kB*T = 4.11577e-21 J
        // Perl BNG2 uses kB*T ~ 2478.956 J/mol (i.e. R*T with R=8.31446 J/(mol*K))
        // but at single-molecule level: kB*T = 1.3806488e-23 * 298.15 = 4.11577e-21
        if (text_ == "Arrhenius" || text_ == "arrhenius") {
            requireArity(text_, children_, 2);
            const double A = evalArg(0);
            const double Ea = evalArg(1);
            constexpr double kBT = 1.3806488e-23 * 298.15; // kB * T at 298.15 K
            return A * std::exp(-Ea / kBT);
        }
        // FunctionProduct(v1, v2) -> v1 * v2
        if (text_ == "FunctionProduct" || text_ == "functionproduct") {
            requireArity(text_, children_, 2);
            return evalArg(0) * evalArg(1);
        }
        // TFUN(obsValue, ...) -> returns first argument (stub for ODE mode)
        if (text_ == "TFUN" || text_ == "tfun") {
            if (children_.empty()) throw std::runtime_error("Function 'TFUN' expects at least 1 argument");
            return evalArg(0);
        }
        // Constants
        if (text_ == "_pi") { requireArity(text_, children_, 0); return M_PI; }
        if (text_ == "_e") { requireArity(text_, children_, 0); return M_E; }

        // Try resolving as user-defined function (zero-arg call like myFunc())
        if (children_.empty()) {
            return resolveIdentifier(text_);
        }
        throw std::runtime_error("Unsupported function '" + text_ + "'");
    }
    case ExpressionKind::ObservableRef:
        // Resolve observable references via the resolver (which knows about
        // observable values at the current simulation state).  If children
        // are present they are ignored — BNG2 observable refs are always
        // resolved by name alone.
        return resolveIdentifier(text_);
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
