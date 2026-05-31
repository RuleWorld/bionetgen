#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_floating_point.hpp>
#include "../src/actions/ActionDispatch.hpp"
#include "../src/ast/Model.hpp"
// Include implementation to exercise internal functions in anonymous namespace.
#include "../src/actions/ActionDispatch.cpp"

using namespace bng::actions;
using namespace bng::ast;

TEST_CASE("ActionDispatch string utilities", "[ActionDispatch]") {
    SECTION("trim") {
        REQUIRE(trim("  hello  ") == "hello");
        REQUIRE(trim("\thello\n") == "hello");
        REQUIRE(trim("hello") == "hello");
        REQUIRE(trim("  ") == "");
        REQUIRE(trim("") == "");
    }

    SECTION("stripQuotes") {
        REQUIRE(stripQuotes("\"hello\"") == "hello");
        REQUIRE(stripQuotes("'hello'") == "hello");
        REQUIRE(stripQuotes("hello") == "hello");
        REQUIRE(stripQuotes("\"hello") == "\"hello");
        REQUIRE(stripQuotes("hello\"") == "hello\"");
    }

    SECTION("lowercase") {
        REQUIRE(lowercase("Hello") == "hello");
        REQUIRE(lowercase("HELLO") == "hello");
        REQUIRE(lowercase("hello") == "hello");
        REQUIRE(lowercase("123") == "123");
    }
}

TEST_CASE("ActionDispatch parseScalarValue", "[ActionDispatch]") {
    Model model;
    model.addParameter(Parameter("A", Expression::number(10.0)));
    model.getParameters().evaluateAll();

    SECTION("Parsing simple numbers") {
        REQUIRE_THAT(parseScalarValue("42", model), Catch::Matchers::WithinAbs(42.0, 1e-6));
        REQUIRE_THAT(parseScalarValue("-3.14", model), Catch::Matchers::WithinAbs(-3.14, 1e-6));
        REQUIRE_THAT(parseScalarValue(" 10.5 ", model), Catch::Matchers::WithinAbs(10.5, 1e-6));
        REQUIRE_THAT(parseScalarValue("1e-3", model), Catch::Matchers::WithinAbs(1e-3, 1e-6));
        REQUIRE_THAT(parseScalarValue("1E-3", model), Catch::Matchers::WithinAbs(1e-3, 1e-6));
        REQUIRE_THAT(parseScalarValue("1d-3", model), Catch::Matchers::WithinAbs(1e-3, 1e-6));
        REQUIRE_THAT(parseScalarValue("1D-3", model), Catch::Matchers::WithinAbs(1e-3, 1e-6));
    }

    SECTION("Parsing parameter names") {
        REQUIRE_THAT(parseScalarValue("A", model), Catch::Matchers::WithinAbs(10.0, 1e-6));
        REQUIRE_THAT(parseScalarValue("\"A\"", model), Catch::Matchers::WithinAbs(10.0, 1e-6));
        REQUIRE_THAT(parseScalarValue(" A ", model), Catch::Matchers::WithinAbs(10.0, 1e-6));
    }

    SECTION("Parsing expressions") {
        REQUIRE_THAT(parseScalarValue("2*A", model), Catch::Matchers::WithinAbs(20.0, 1e-6));
        REQUIRE_THAT(parseScalarValue("A*A", model), Catch::Matchers::WithinAbs(100.0, 1e-6));
        REQUIRE_THAT(parseScalarValue("A/2", model), Catch::Matchers::WithinAbs(5.0, 1e-6));
        REQUIRE_THAT(parseScalarValue("A+5", model), Catch::Matchers::WithinAbs(15.0, 1e-6));
    }

    SECTION("Error handling") {
        REQUIRE_THROWS_AS(parseScalarValue("NonExistent", model), std::runtime_error);
        REQUIRE_THROWS_AS(parseScalarValue("1+NonExistent", model), std::runtime_error);
        REQUIRE_THROWS_AS(parseScalarValue("(A+NonExistent)/2", model), std::runtime_error);
        REQUIRE_THROWS_AS(parseScalarValue("A / / 2", model), std::runtime_error);
        REQUIRE_THROWS_AS(parseScalarValue("2 * (A + 1", model), std::runtime_error);
    }
}

TEST_CASE("ActionDispatch readArgument", "[ActionDispatch]") {
    Action action;
    action.arguments["method"] = "\"ode\"";
    action.arguments["t_end"] = "10";

    SECTION("Existing arguments") {
        REQUIRE(readArgument(action, "method") == "\"ode\"");
        REQUIRE(readArgument(action, "t_end") == "10");
    }

    SECTION("Fallback arguments") {
        REQUIRE(readArgument(action, "nonexistent", "fallback") == "fallback");
        REQUIRE(readArgument(action, "n_steps", "100") == "100");
    }
}

TEST_CASE("ActionDispatch resolveSimulationMethod", "[ActionDispatch]") {
    SECTION("From action name") {
        Action a1; a1.name = "simulate_ode";
        REQUIRE(resolveSimulationMethod(a1) == "cvode");

        Action a2; a2.name = "simulate_ssa";
        REQUIRE(resolveSimulationMethod(a2) == "ssa");
    }

    SECTION("From method argument") {
        Action action;
        action.name = "simulate";

        action.arguments["method"] = "\"ode\"";
        REQUIRE(resolveSimulationMethod(action) == "cvode");

        action.arguments["method"] = "\"pla\"";
        REQUIRE(resolveSimulationMethod(action) == "pla");

        action.arguments["method"] = "\"nf\"";
        REQUIRE(resolveSimulationMethod(action) == "nf");

        action.arguments["method"] = "ssa";
        REQUIRE(resolveSimulationMethod(action) == "ssa");
    }

    SECTION("Default method") {
        Action action;
        action.name = "simulate";
        REQUIRE(resolveSimulationMethod(action) == "cvode");
    }
}

TEST_CASE("ActionDispatch simulationPrefix", "[ActionDispatch]") {
    std::filesystem::path sourcePath("my_model.bngl");
    Action action;

    SECTION("Default prefix") {
        REQUIRE(simulationPrefix(action, sourcePath) == "my_model");
    }

    SECTION("With prefix argument") {
        action.arguments["prefix"] = "\"custom_prefix\"";
        REQUIRE(simulationPrefix(action, sourcePath) == "custom_prefix");
    }

    SECTION("With suffix argument") {
        action.arguments["suffix"] = "\"v2\"";
        REQUIRE(simulationPrefix(action, sourcePath) == "my_model_v2");
    }

    SECTION("With both prefix and suffix") {
        action.arguments["prefix"] = "\"custom\"";
        action.arguments["suffix"] = "\"run1\"";
        REQUIRE(simulationPrefix(action, sourcePath) == "custom_run1");
    }

    SECTION("With scan index") {
        REQUIRE(simulationPrefix(action, sourcePath, 0) == "my_model_scan1");
        REQUIRE(simulationPrefix(action, sourcePath, 5) == "my_model_scan6");
    }

    SECTION("With prefix, suffix, and scan index") {
        action.arguments["prefix"] = "\"custom\"";
        action.arguments["suffix"] = "\"run\"";
        REQUIRE(simulationPrefix(action, sourcePath, 2) == "custom_run_scan3");
    }
}

TEST_CASE("ActionDispatch findSpeciesIndex", "[ActionDispatch]") {
    bng::engine::GeneratedNetwork network;

    // Build some species
    // Note: in a full test we'd parse properly, but here we can just mock the string representation
    BNGcore::PatternGraph pg1, pg2, pg3;
    pg1.set_raw_string("A(b)");
    pg2.set_raw_string("B(a~p)");
    pg3.set_raw_string("C()");

    bng::ast::Species sp1(bng::ast::SpeciesGraph(std::move(pg1)), 10.0, false);
    bng::ast::Species sp2(bng::ast::SpeciesGraph(std::move(pg2)), 20.0, true);
    bng::ast::Species sp3(bng::ast::SpeciesGraph(std::move(pg3)), 30.0, false);

    // Add compartment to sp3
    sp3.setCompartment("comp1");

    network.species.add(std::move(sp1));
    network.species.add(std::move(sp2));
    network.species.add(std::move(sp3));

    SECTION("Exact match canonical") {
        REQUIRE(findSpeciesIndex(network, "A(b)").value() == 0);
        REQUIRE(findSpeciesIndex(network, "B(a~p)").value() == 1);
        REQUIRE(findSpeciesIndex(network, "C()").value() == 2);
    }

    SECTION("Prefix match (BNG2 compatibility)") {
        REQUIRE(findSpeciesIndex(network, "A").value() == 0);
        REQUIRE(findSpeciesIndex(network, "B").value() == 1);
        REQUIRE(findSpeciesIndex(network, "C").value() == 2);
    }

    SECTION("Match with constant/compartment formatting") {
        // Double colon format
        REQUIRE(findSpeciesIndex(network, "$B(a~p)").value() == 1);
        REQUIRE(findSpeciesIndex(network, "@comp1::C()").value() == 2);

        // Single colon format
        REQUIRE(findSpeciesIndex(network, "@comp1:C()").value() == 2);
    }

    SECTION("Not found") {
        REQUIRE_FALSE(findSpeciesIndex(network, "D()").has_value());
        REQUIRE_FALSE(findSpeciesIndex(network, "A(c)").has_value());
    }
}

TEST_CASE("ActionDispatch snapshot and restore concentrations", "[ActionDispatch]") {
    bng::engine::GeneratedNetwork network;

    BNGcore::PatternGraph pg1, pg2;
    pg1.set_raw_string("A()");
    pg2.set_raw_string("B()");

    bng::ast::Species sp1(bng::ast::SpeciesGraph(std::move(pg1)), 1.5, false);
    bng::ast::Species sp2(bng::ast::SpeciesGraph(std::move(pg2)), 2.5, false);

    network.species.add(std::move(sp1));
    network.species.add(std::move(sp2));

    SECTION("snapshotConcentrations") {
        auto concs = snapshotConcentrations(network);
        REQUIRE(concs.size() == 2);
        REQUIRE_THAT(concs[0], Catch::Matchers::WithinAbs(1.5, 1e-6));
        REQUIRE_THAT(concs[1], Catch::Matchers::WithinAbs(2.5, 1e-6));
    }

    SECTION("restoreConcentrations") {
        std::vector<double> newConcs = {10.0, 20.0};
        restoreConcentrations(network, newConcs);

        REQUIRE_THAT(network.species.get(0).getAmount(), Catch::Matchers::WithinAbs(10.0, 1e-6));
        REQUIRE_THAT(network.species.get(1).getAmount(), Catch::Matchers::WithinAbs(20.0, 1e-6));
    }

    SECTION("restoreConcentrations mismatched sizes") {
        // Less elements
        std::vector<double> lessConcs = {10.0};
        restoreConcentrations(network, lessConcs);
        REQUIRE_THAT(network.species.get(0).getAmount(), Catch::Matchers::WithinAbs(10.0, 1e-6));
        REQUIRE_THAT(network.species.get(1).getAmount(), Catch::Matchers::WithinAbs(2.5, 1e-6)); // Unchanged

        // More elements
        std::vector<double> moreConcs = {30.0, 40.0, 50.0};
        restoreConcentrations(network, moreConcs);
        REQUIRE_THAT(network.species.get(0).getAmount(), Catch::Matchers::WithinAbs(30.0, 1e-6));
        REQUIRE_THAT(network.species.get(1).getAmount(), Catch::Matchers::WithinAbs(40.0, 1e-6));
    }
}

TEST_CASE("ActionDispatch execute basic", "[ActionDispatch]") {
    bng::ast::Model model;

    bng::ast::Action a;
    a.name = "generate_network";
    a.arguments["overwrite"] = "1";
    model.addAction(a);

    std::filesystem::path sourcePath("test_execute.bngl");

    REQUIRE_NOTHROW(bng::actions::ActionDispatch::execute(model, sourcePath, false));

    // Check that test_execute.net is generated
    REQUIRE(std::filesystem::exists("test_execute.net"));

    // Cleanup
    std::filesystem::remove("test_execute.net");
}
