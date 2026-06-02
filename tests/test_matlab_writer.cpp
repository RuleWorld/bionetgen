#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_string.hpp>

#include "../src/io/MatlabWriter.hpp"
#include "../src/ast/Model.hpp"
#include "../src/engine/NetworkGenerator.hpp"

using namespace bng::io;
using namespace bng::ast;
using namespace bng::engine;

TEST_CASE("MatlabWriter tests", "[MatlabWriter]") {
    Model model;
    model.setModelName("test_model");

    SECTION("numeric rate law") {
        GeneratedNetwork network;

        // Add a reaction with a numeric rate law
        network.reactions.add(Rxn("R1", {0}, {0}, "1.5", 1.0, "Rule1"));

        // Need at least one species
        network.species.add(Species(SpeciesGraph(), 10.0));

        // Act
        std::string result = MatlabWriter::write(model, network);

        // Assert that the numeric rate was written out correctly
        REQUIRE_THAT(result, Catch::Matchers::ContainsSubstring("1.5"));
    }

    SECTION("non-numeric rate law (error path test)") {
        // We need to define some parameters, molecule types, and a reaction.
        Parameter k1("k1", Expression::number(1.5));
        model.addParameter(k1);

        // Create a network with a non-numeric rate law reaction to hit the else branch
        GeneratedNetwork network;

        // Add a reaction with a rate law that is not numeric, for example "k1"
        network.reactions.add(Rxn("R1", {0}, {0}, "k1", 1.0, "Rule1"));

        // Need at least one species
        network.species.add(Species(SpeciesGraph(), 10.0));

        // Act
        std::string result = MatlabWriter::write(model, network);

        // Assert that the non-numeric parameter was written out correctly
        REQUIRE_THAT(result, Catch::Matchers::ContainsSubstring("k1"));
    }
}
