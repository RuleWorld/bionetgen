#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_floating_point.hpp>

#include "../src/engine/OdeIntegrator.hpp"
#include "../src/ast/Model.hpp"
#include "../src/engine/NetworkGenerator.hpp"
#include "../src/ast/Function.hpp"
#include "../src/ast/Expression.hpp"
#include "../src/ast/Rxn.hpp"
#include "../src/ast/Species.hpp"
#include "../src/ast/SpeciesGraph.hpp"

using namespace bng::engine;

TEST_CASE("OdeIntegrator handles expression parsing fallback", "[OdeIntegrator]") {
    bng::ast::Model model;

    // Add a function to trigger the expression parser
    std::vector<std::string> args;
    model.addFunction(bng::ast::Function("myFunc", args, bng::ast::Expression::number(1.0)));

    // Create a generated network with a reaction using that function in its rate law
    GeneratedNetwork network;

    // In order for PatternGraph (used by SpeciesGraph) to not segfault on label generation,
    // we bypass isolation checking entirely. But even setting checkIso(false) might not
    // be enough if OdeIntegrator accesses properties internally.
    // Instead, let's just make the species map mock simple strings by creating
    // a small mock via NetworkGenerator or bypass it by putting dummy nodes.

    network.species.setCheckIso(false);

    // Set up dummy species: S starting at concentration 1.0
    // If graph empty, SpeciesList::add will call canonicalLabel() which might crash if not handled correctly.
    // However, when checkIso is false, SpeciesList::add just skips dedup! So it shouldn't call canonicalLabel.
    bng::ast::SpeciesGraph sg;
    network.species.add(bng::ast::Species(sg, 1.0)); // S is index 0

    // Add a dummy species to receive the product
    bng::ast::SpeciesGraph sg2;
    network.species.add(bng::ast::Species(sg2, 0.0)); // P is index 1

    // Create a reaction that uses the function, but with invalid syntax
    // to trigger the exception in parser::parseExpression
    // Reaction: S -> P
    std::vector<std::size_t> reactants = {0};
    std::vector<std::size_t> products = {1};
    bng::ast::Rxn rxn("R1", reactants, products, "myFunc * (1 + )", 1.0, "dummy_rule");

    network.reactions.add(rxn);

    // Test that the OdeIntegrator compiles successfully despite the parser exception
    OdeIntegrator integrator(model, network);

    // Test that we can actually run the integrator.
    OdeOptions options;
    options.method = "euler";
    options.tStart = 0.0;
    options.tEnd = 0.1;
    options.nSteps = 1;

    OdeResult result;
    REQUIRE_NOTHROW(result = integrator.integrate(options));

    // Since rate is 1.0 * [S], at t=0, [S]=1.0. Derivative is -1.0 for S, +1.0 for P.
    // Euler step size is 0.1, so at t=0.1, [S] should be 1.0 - 0.1 = 0.9
    // and [P] should be 0.0 + 0.1 = 0.1
    REQUIRE(result.concentrations.size() == 2); // 2 time points (t=0.0 and t=0.1)
    REQUIRE(result.concentrations.back().size() == 2); // 2 species

    REQUIRE_THAT(result.concentrations.back()[0], Catch::Matchers::WithinAbs(0.9, 1e-6));
    REQUIRE_THAT(result.concentrations.back()[1], Catch::Matchers::WithinAbs(0.1, 1e-6));
}

TEST_CASE("OdeIntegrator handles destruction", "[OdeIntegrator]") {
    bng::ast::Model model;
    GeneratedNetwork network;
    network.species.setCheckIso(false);

    // We just want to test if it can be constructed and destroyed without leaks or issues.
    auto integrator = std::make_unique<OdeIntegrator>(model, network);

    // Resetting it will call the destructor.
    REQUIRE_NOTHROW(integrator.reset());
}

TEST_CASE("OdeIntegrator preserves case-insensitive rate classification", "[OdeIntegrator]") {
    auto makeNetwork = [](const std::string& rateLaw) {
        GeneratedNetwork network;
        network.species.setCheckIso(false);

        bng::ast::SpeciesGraph reactantGraph;
        network.species.add(bng::ast::Species(reactantGraph, 1.0));

        bng::ast::SpeciesGraph productGraph;
        network.species.add(bng::ast::Species(productGraph, 0.0));

        network.reactions.add(bng::ast::Rxn(
            "R1", {0}, {1}, rateLaw, 1.0, "dummy_rule",
            bng::ast::Expression::number(2.0)));
        return network;
    };

    SECTION("time keyword casing remains functional") {
        for (const auto& rateLaw : {std::string("time"), std::string("TIME"), std::string("Time")}) {
            bng::ast::Model model;
            auto network = makeNetwork(rateLaw);
            OdeIntegrator integrator(model, network);

            double state[] = {1.0, 0.0};
            double derivatives[] = {0.0, 0.0};
            integrator.derivs(0.0, state, derivatives);

            REQUIRE_THAT(derivatives[0], Catch::Matchers::WithinAbs(-2.0, 1e-12));
            REQUIRE_THAT(derivatives[1], Catch::Matchers::WithinAbs(2.0, 1e-12));
        }
    }

    SECTION("mixed-case function names are matched without lowercasing") {
        bng::ast::Model model;
        model.addFunction(bng::ast::Function(
            "rateFn", {}, bng::ast::Expression::number(1.0)));
        auto network = makeNetwork("RATEFN");
        OdeIntegrator integrator(model, network);

        double state[] = {1.0, 0.0};
        double derivatives[] = {0.0, 0.0};
        integrator.derivs(0.0, state, derivatives);

        REQUIRE_THAT(derivatives[0], Catch::Matchers::WithinAbs(-2.0, 1e-12));
        REQUIRE_THAT(derivatives[1], Catch::Matchers::WithinAbs(2.0, 1e-12));
    }
}

TEST_CASE("OdeIntegrator handles cvode simulation", "[OdeIntegrator]") {
    bng::ast::Model model;
    GeneratedNetwork network;
    network.species.setCheckIso(false);

    bng::ast::SpeciesGraph sg;
    network.species.add(bng::ast::Species(sg, 1.0)); // S is index 0

    bng::ast::SpeciesGraph sg2;
    network.species.add(bng::ast::Species(sg2, 0.0)); // P is index 1

    std::vector<std::size_t> reactants = {0};
    std::vector<std::size_t> products = {1};
    bng::ast::Rxn rxn("R1", reactants, products, "1.0", 1.0, "dummy_rule");

    network.reactions.add(rxn);

    OdeIntegrator integrator(model, network);

    OdeOptions options;
    options.method = "cvode";
    options.tStart = 0.0;
    options.tEnd = 0.1;
    options.nSteps = 1;

    OdeResult result;
    REQUIRE_NOTHROW(result = integrator.integrate(options));

    REQUIRE(result.concentrations.size() == 2);
    REQUIRE(result.concentrations.back().size() == 2);

    REQUIRE_THAT(result.concentrations.back()[0], Catch::Matchers::WithinAbs(0.904837, 1e-5)); // e^-0.1
    REQUIRE_THAT(result.concentrations.back()[1], Catch::Matchers::WithinAbs(0.095162, 1e-5)); // 1 - e^-0.1
}

TEST_CASE("OdeIntegrator preserves multi-species derivative updates", "[OdeIntegrator]") {
    bng::ast::Model model;
    GeneratedNetwork network;
    network.species.setCheckIso(false);

    for (double amount : {3.0, 4.0, 0.0, 0.0}) {
        bng::ast::SpeciesGraph graph;
        network.species.add(bng::ast::Species(graph, amount));
    }

    // Cross the compact-reaction threshold while retaining a two-reactant,
    // two-product update shape representative of generated networks.
    for (std::size_t i = 0; i < 512; ++i) {
        network.reactions.add(bng::ast::Rxn(
            "R" + std::to_string(i), {0, 1}, {2, 3}, "2.0", 1.0,
            "rule" + std::to_string(i)));
    }

    OdeIntegrator integrator(model, network);
    double state[] = {3.0, 4.0, 0.0, 0.0};
    double derivatives[] = {0.0, 0.0, 0.0, 0.0};
    integrator.derivs(0.0, state, derivatives);

    constexpr double expectedRate = 2.0 * 3.0 * 4.0 * 512.0;
    REQUIRE_THAT(derivatives[0], Catch::Matchers::WithinAbs(-expectedRate, 1e-12));
    REQUIRE_THAT(derivatives[1], Catch::Matchers::WithinAbs(-expectedRate, 1e-12));
    REQUIRE_THAT(derivatives[2], Catch::Matchers::WithinAbs(expectedRate, 1e-12));
    REQUIRE_THAT(derivatives[3], Catch::Matchers::WithinAbs(expectedRate, 1e-12));
}
