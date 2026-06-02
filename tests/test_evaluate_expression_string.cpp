#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_floating_point.hpp>
#include <string>
#include <functional>
#include <cmath>

// Directly include NetWriter.cpp to test the internal evaluateExpressionString
#define CATCH_CONFIG_MAIN
#include "io/NetWriter.cpp"

using namespace bng::io;

TEST_CASE("evaluateExpressionString", "[netwriter]") {
    auto resolver = [](const std::string& name) {
        if (name == "x") return 1.0;
        if (name == "y") return 2.0;
        if (name == "pi") return M_PI;
        return 0.0;
    };

    SECTION("Numbers and whitespace") {
        REQUIRE(evaluateExpressionString("42", resolver) == 42.0);
        REQUIRE(evaluateExpressionString("  3.14  ", resolver) == 3.14);
        REQUIRE(evaluateExpressionString("-1.5", resolver) == -1.5);
    }

    SECTION("Parameters") {
        REQUIRE(evaluateExpressionString("x", resolver) == 1.0);
        REQUIRE(evaluateExpressionString("y", resolver) == 2.0);
        REQUIRE(evaluateExpressionString("pi", resolver) == M_PI);
        REQUIRE(evaluateExpressionString("unknown", resolver) == 0.0);
    }

    SECTION("Basic arithmetic") {
        REQUIRE(evaluateExpressionString("2 + 3", resolver) == 5.0);
        REQUIRE(evaluateExpressionString("5 - 2", resolver) == 3.0);
        REQUIRE(evaluateExpressionString("4 * 3", resolver) == 12.0);
        REQUIRE(evaluateExpressionString("10 / 2", resolver) == 5.0);
        REQUIRE(evaluateExpressionString("2 ^ 3", resolver) == 8.0);
    }

    SECTION("Operator precedence") {
        REQUIRE(evaluateExpressionString("2 + 3 * 4", resolver) == 14.0);
        REQUIRE(evaluateExpressionString("2 * 3 + 4", resolver) == 10.0);
        REQUIRE(evaluateExpressionString("10 - 4 / 2", resolver) == 8.0);
        REQUIRE(evaluateExpressionString("2 ^ 3 * 2", resolver) == 16.0); // 8 * 2
        REQUIRE(evaluateExpressionString("2 * 2 ^ 3", resolver) == 16.0); // 2 * 8
    }

    SECTION("Parentheses") {
        REQUIRE(evaluateExpressionString("(2 + 3) * 4", resolver) == 20.0);
        REQUIRE(evaluateExpressionString("2 * (3 + 4)", resolver) == 14.0);
        REQUIRE(evaluateExpressionString("(10 - 4) / 2", resolver) == 3.0);
        REQUIRE(evaluateExpressionString("(2)", resolver) == 2.0);
        REQUIRE(evaluateExpressionString("((2 + 3))", resolver) == 5.0);
    }

    SECTION("Unary minus") {
        REQUIRE(evaluateExpressionString("-5", resolver) == -5.0);
        REQUIRE(evaluateExpressionString("-x", resolver) == -1.0);
        REQUIRE(evaluateExpressionString("-(2 + 3)", resolver) == -5.0);
        REQUIRE(evaluateExpressionString("2 * -3", resolver) == -6.0);
        REQUIRE(evaluateExpressionString("-exp(1)", resolver) == -std::exp(1.0));
    }

    SECTION("Math functions") {
        REQUIRE(evaluateExpressionString("exp(0)", resolver) == 1.0);
        REQUIRE_THAT(evaluateExpressionString("exp(1)", resolver), Catch::Matchers::WithinRel(std::exp(1.0), 1e-5));
        REQUIRE(evaluateExpressionString("ln(1)", resolver) == 0.0);
        REQUIRE_THAT(evaluateExpressionString("log(1)", resolver), Catch::Matchers::WithinRel(0.0, 1e-5));
        REQUIRE(evaluateExpressionString("log2(2)", resolver) == 1.0);
        REQUIRE(evaluateExpressionString("log10(10)", resolver) == 1.0);
        REQUIRE(evaluateExpressionString("sqrt(16)", resolver) == 4.0);
        REQUIRE(evaluateExpressionString("abs(-5.5)", resolver) == 5.5);
        REQUIRE(evaluateExpressionString("sin(0)", resolver) == 0.0);
        REQUIRE(evaluateExpressionString("cos(0)", resolver) == 1.0);
        REQUIRE(evaluateExpressionString("tan(0)", resolver) == 0.0);
    }

    SECTION("Complex expressions") {
        REQUIRE(evaluateExpressionString("x + y * 3", resolver) == 7.0);
        REQUIRE(evaluateExpressionString("2 * x + y / 2", resolver) == 3.0);
        REQUIRE(evaluateExpressionString("exp(0) + sqrt(16) * 2", resolver) == 9.0);
    }

    SECTION("Non-numeric and edge cases") {
        // Throws invalid_argument in stod, falls back to parameter resolution
        REQUIRE(evaluateExpressionString("not_a_number", resolver) == 0.0);
        // Throws out_of_range in stod, falls back to parameter resolution
        REQUIRE(evaluateExpressionString("1e10000", resolver) == 0.0);
        // Partially parses as number but fails pos == size check, falls back to parameter
        REQUIRE(evaluateExpressionString("123foo", resolver) == 0.0);
    }

    SECTION("Error conditions") {
        REQUIRE_THROWS_AS(evaluateExpressionString("10 / 0", resolver), std::runtime_error);
        REQUIRE_THROWS_AS(evaluateExpressionString("10 / (2 - 2)", resolver), std::runtime_error);
        REQUIRE_THROWS_AS(evaluateExpressionString("foo(", resolver), std::runtime_error);
        REQUIRE_THROWS_AS(evaluateExpressionString("2 + * 3", resolver), std::runtime_error);
        REQUIRE_THROWS_AS(evaluateExpressionString("(2 + 3", resolver), std::runtime_error);
    }

    SECTION("Scientific notation parsing") {
        REQUIRE(evaluateExpressionString("1e-5", resolver) == 1e-5);
        REQUIRE(evaluateExpressionString("2.5E2", resolver) == 2.5e2);
        REQUIRE(evaluateExpressionString("1e-5 + 2", resolver) == 1e-5 + 2);
        REQUIRE(evaluateExpressionString("2 + 1e-5", resolver) == 2 + 1e-5);
        REQUIRE(evaluateExpressionString("1e+5 * 2", resolver) == 1e5 * 2);
        REQUIRE(evaluateExpressionString("1e-5 * 1e5", resolver) == 1e-5 * 1e5);
        REQUIRE_THAT(evaluateExpressionString("3e-2 ^ 2", resolver), Catch::Matchers::WithinRel(std::pow(3e-2, 2.0), 1e-5));
    }
}
