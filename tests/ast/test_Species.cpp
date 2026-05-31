#include <catch2/catch_test_macros.hpp>
#include "ast/Species.hpp"
#include "ast/SpeciesGraph.hpp"

using namespace bng::ast;

TEST_CASE("Species: Basic properties", "[ast][Species]") {
    SpeciesGraph sg;

    SECTION("Default construction") {
        Species s(sg);

        REQUIRE(s.getAmount() == 0.0);
        REQUIRE(s.isConstant() == false);
        REQUIRE(s.getCompartment().empty());
        REQUIRE(s.getIndex() == 0);
        REQUIRE(s.rulesApplied() == false);
    }

    SECTION("Custom initialization") {
        Species s(sg, 10.5, true, "cytosol");

        REQUIRE(s.getAmount() == 10.5);
        REQUIRE(s.isConstant() == true);
        REQUIRE(s.getCompartment() == "cytosol");

        // Compartment propagation to SpeciesGraph
        REQUIRE(s.getSpeciesGraph().getCompartment() == "cytosol");
    }

    SECTION("Constructor compartment logic") {
        // If SpeciesGraph already has a compartment, Species doesn't overwrite it
        SpeciesGraph sg_comp;
        sg_comp.setCompartment("nucleus");
        Species s(sg_comp, 5.0, false, "cytosol");

        REQUIRE(s.getCompartment() == "cytosol");
        REQUIRE(s.getSpeciesGraph().getCompartment() == "nucleus");
    }

    SECTION("Getters and Setters") {
        Species s(sg);

        s.setAmount(42.0);
        REQUIRE(s.getAmount() == 42.0);

        s.setCompartment("nucleus");
        REQUIRE(s.getCompartment() == "nucleus");

        s.setIndex(5);
        REQUIRE(s.getIndex() == 5);

        s.setRulesApplied(true);
        REQUIRE(s.rulesApplied() == true);
    }

    SECTION("Non-const getSpeciesGraph") {
        Species s(sg);
        s.getSpeciesGraph().setCompartment("membrane");
        REQUIRE(s.getSpeciesGraph().getCompartment() == "membrane");
    }
}
