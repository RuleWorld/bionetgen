#include <catch2/catch_test_macros.hpp>
#include "ast/Species.hpp"
#include "ast/SpeciesGraph.hpp"
#include "core/BNGcore.hpp"

using namespace bng::ast;

TEST_CASE("Species functionality", "[ast][Species]") {
    BNGcore::PatternGraph pg;
    SpeciesGraph sg(pg);

    SECTION("Default construction and initial values") {
        Species s(sg);

        REQUIRE(s.getAmount() == 0.0);
        REQUIRE(s.isConstant() == false);
        REQUIRE(s.getCompartment().empty());
        REQUIRE(s.getIndex() == 0);
        REQUIRE(s.rulesApplied() == false);
    }

    SECTION("Constructor with parameters") {
        Species s(sg, 10.5, true, "cytosol");

        REQUIRE(s.getAmount() == 10.5);
        REQUIRE(s.isConstant() == true);
        REQUIRE(s.getCompartment() == "cytosol");
        REQUIRE(s.getSpeciesGraph().getCompartment() == "cytosol");
    }

    SECTION("Constructor with non-empty SpeciesGraph compartment") {
        SpeciesGraph sg_comp(pg, "existing");
        Species s(sg_comp, 5.0, false, "new_comp");

        REQUIRE(s.getCompartment() == "new_comp");
        REQUIRE(s.getSpeciesGraph().getCompartment() == "existing");
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

    SECTION("SpeciesGraph references") {
        Species s(sg);

        const Species& cs = s;
        REQUIRE(cs.getSpeciesGraph().getCompartment().empty());

        s.getSpeciesGraph().setCompartment("membrane");
        REQUIRE(cs.getSpeciesGraph().getCompartment() == "membrane");
    }
}
