#include <catch2/catch_test_macros.hpp>
#include "MoleculeType.hpp"

using namespace bng::ast;

TEST_CASE("MoleculeType basic functionality", "[ast]") {
    SECTION("Empty components and no population") {
        MoleculeType m("SimpleMol", {});
        REQUIRE(m.getName() == "SimpleMol");
        REQUIRE(m.getComponents().empty());
        REQUIRE(m.isPopulation() == false);
    }

    SECTION("With components and no population") {
        std::vector<ComponentType> components = {
            {"comp1", {"stateA", "stateB"}},
            {"comp2", {}}
        };
        MoleculeType m("ComplexMol", components);

        REQUIRE(m.getName() == "ComplexMol");
        REQUIRE(m.getComponents().size() == 2);

        REQUIRE(m.getComponents()[0].name == "comp1");
        REQUIRE(m.getComponents()[0].allowedStates.size() == 2);
        REQUIRE(m.getComponents()[0].allowedStates[0] == "stateA");
        REQUIRE(m.getComponents()[0].allowedStates[1] == "stateB");

        REQUIRE(m.getComponents()[1].name == "comp2");
        REQUIRE(m.getComponents()[1].allowedStates.empty());

        REQUIRE(m.isPopulation() == false);
    }

    SECTION("With population flag true") {
        MoleculeType m("PopMol", {}, true);
        REQUIRE(m.getName() == "PopMol");
        REQUIRE(m.getComponents().empty());
        REQUIRE(m.isPopulation() == true);
    }
}
