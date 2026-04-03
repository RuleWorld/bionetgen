#include <cmath>
#include <stdexcept>
#include <string>

#include <catch2/catch_test_macros.hpp>
#include <catch2/catch_approx.hpp>
#include <catch2/matchers/catch_matchers_string.hpp>
#include <catch2/matchers/catch_matchers.hpp>

#include "ast/Expression.hpp"
#include "ast/Parameter.hpp"
#include "ast/ParameterList.hpp"

namespace {

constexpr double kTolerance = 1e-12;

}

TEST_CASE("Expression evaluates numeric literals and math helpers", "[expression]") {
    using bng::ast::Expression;

    const auto numeric = Expression::number(1.0e-5);
    CHECK(numeric.evaluate([](const std::string&) { return 0.0; }) == Catch::Approx(1.0e-5).margin(kTolerance));

    const auto exponent = Expression::binary("^", Expression::number(10.0), Expression::number(3.0));
    CHECK(exponent.evaluate([](const std::string&) { return 0.0; }) == Catch::Approx(1000.0).margin(kTolerance));

    const auto conditional = Expression::function(
        "if",
        {Expression::number(1.0), Expression::number(5.0), Expression::number(7.0)});
    CHECK(conditional.evaluate([](const std::string&) { return 0.0; }) == Catch::Approx(5.0).margin(kTolerance));

    const auto average = Expression::function(
        "avg",
        {Expression::number(2.0), Expression::number(4.0), Expression::number(6.0)});
    CHECK(average.evaluate([](const std::string&) { return 0.0; }) == Catch::Approx(4.0).margin(kTolerance));

    const auto timed = Expression::binary("+", Expression::identifier("time"), Expression::number(2.0));
    CHECK(timed.evaluate([](const std::string&) { return 0.0; }, 3.5) == Catch::Approx(5.5).margin(kTolerance));
}

TEST_CASE("ParameterList resolves dependency chains and functions", "[expression][parameters]") {
    using bng::ast::Expression;
    using bng::ast::Parameter;
    using bng::ast::ParameterList;

    ParameterList parameters;
    parameters.add(Parameter("a", Expression::number(1.0)));
    parameters.add(Parameter("b", Expression::binary("*", Expression::number(2.0), Expression::identifier("a"))));
    parameters.add(Parameter(
        "c",
        Expression::function("exp", {Expression::identifier("b")})));
    parameters.add(Parameter(
        "d",
        Expression::function(
            "if",
            {Expression::binary(">", Expression::identifier("b"), Expression::number(1.5)),
             Expression::identifier("c"),
             Expression::number(0.0)})));

    parameters.evaluateAll();

    CHECK(parameters.get("a").getValue() == Catch::Approx(1.0).margin(kTolerance));
    CHECK(parameters.get("b").getValue() == Catch::Approx(2.0).margin(kTolerance));
    CHECK(parameters.get("c").getValue() == Catch::Approx(std::exp(2.0)).margin(kTolerance));
    CHECK(parameters.get("d").getValue() == Catch::Approx(std::exp(2.0)).margin(kTolerance));
}

TEST_CASE("ParameterList detects circular dependencies", "[expression][parameters]") {
    using bng::ast::Expression;
    using bng::ast::Parameter;
    using bng::ast::ParameterList;

    ParameterList parameters;
    parameters.add(Parameter("a", Expression::identifier("b")));
    parameters.add(Parameter("b", Expression::identifier("a")));

    CHECK_THROWS_WITH(parameters.evaluateAll(), Catch::Matchers::ContainsSubstring("Circular parameter dependency"));
}
