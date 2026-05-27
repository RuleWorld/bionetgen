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
