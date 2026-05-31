#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_floating_point.hpp>

#include "../src/engine/PsaSimulator.hpp"
#include "../src/ast/Model.hpp"
#include "../src/engine/NetworkGenerator.hpp"
#include "../src/ast/Function.hpp"
#include "../src/ast/Expression.hpp"
#include "../src/ast/Rxn.hpp"
#include "../src/ast/Species.hpp"
#include "../src/ast/SpeciesGraph.hpp"

// Forward declaration is needed to access anonymous namespace evaluation function inside tests.
// Since PsaSimulator.cpp defines it inside an anonymous namespace, we can directly include it.
// To avoid ODR violations when linking against bng_engine, we shouldn't link test_psa_simulator with bng_engine
// if we include .cpp, or we can just skip testing this anonymous namespace helper and rely on evaluateRateString inside PsaSimulator.
// The easiest fix is to just let PsaSimulator class call it implicitly when we add reactions.

using namespace bng::engine;
using namespace bng::ast;

TEST_CASE("PsaSimulator initialization and simulation", "[PsaSimulator]") {
    bng::ast::Model model;

    std::vector<std::string> args;
    model.addFunction(bng::ast::Function("myFunc", args, bng::ast::Expression::number(1.0)));

    GeneratedNetwork network;
    network.species.setCheckIso(false);

    bng::ast::SpeciesGraph sg;
    network.species.add(bng::ast::Species(sg, 100.0)); // S1 is index 0

    bng::ast::SpeciesGraph sg2;
    network.species.add(bng::ast::Species(sg2, 50.0)); // S2 is index 1

    std::vector<std::size_t> reactants = {0};
    std::vector<std::size_t> products = {1};
    // Higher rate constant so it fires
    bng::ast::Rxn rxn("R1", reactants, products, "10.0", 1.0, "dummy_rule");

    network.reactions.add(rxn);

    PsaSimulator simulator(model, network);

    OdeOptions opts;
    opts.tEnd = 1.0;
    opts.nSteps = 10;
    opts.seed = 42; // Make deterministic

    // poplevel = 1.0, meaning scaling factor will be used.
    OdeResult result = simulator.simulate(opts, 1.0, true);

    REQUIRE(result.timePoints.size() > 0);
    REQUIRE_THAT(result.timePoints.back(), Catch::Matchers::WithinAbs(1.0, 1e-6));
    REQUIRE(result.concentrations.size() == result.timePoints.size());
    REQUIRE(result.concentrations.back()[0] < 100.0); // S1 should decrease
    REQUIRE(result.concentrations.back()[1] > 50.0);  // S2 should increase
}

TEST_CASE("PsaSimulator simulation with invalid rate", "[PsaSimulator]") {
    bng::ast::Model model;

    GeneratedNetwork network;
    network.species.setCheckIso(false);

    bng::ast::SpeciesGraph sg;
    network.species.add(bng::ast::Species(sg, 100.0)); // S1 is index 0

    std::vector<std::size_t> reactants = {0};
    std::vector<std::size_t> products = {};

    // Invalid rate law to ensure fallback parsing gracefully returns 0.0 in PsaSimulator compilation.
    bng::ast::Rxn rxn("R1", reactants, products, "invalid_float", 1.0, "");

    network.reactions.add(rxn);

    PsaSimulator simulator(model, network);

    OdeOptions opts;
    opts.tEnd = 1.0;
    opts.nSteps = 10;
    opts.seed = 42;

    OdeResult result = simulator.simulate(opts, 1.0, true);

    // Ensure state does not change because rate is 0.0
    REQUIRE(result.concentrations.back()[0] == 100.0);
}
