#include <catch2/catch_test_macros.hpp>
#include "Compartment.hpp"

using namespace bng::ast;

TEST_CASE("Compartment getters and setters", "[ast][compartment]") {
    Compartment c("cytoplasm", 1.5, 3, "membrane");

    REQUIRE(c.getName() == "cytoplasm");
    REQUIRE(c.getVolume() == 1.5);
    REQUIRE(c.getDimension() == 3);
    REQUIRE(c.getParent() == "membrane");

    c.setVolume(2.0);
    REQUIRE(c.getVolume() == 2.0);
}

TEST_CASE("Compartment topology types", "[ast][compartment]") {
    Compartment vol("cytoplasm", 1.0, 3);
    Compartment surf("membrane", 0.1, 2);

    REQUIRE(vol.isVolume());
    REQUIRE_FALSE(vol.isSurface());

    REQUIRE(surf.isSurface());
    REQUIRE_FALSE(surf.isVolume());
}

TEST_CASE("Compartment topology relationships", "[ast][compartment]") {
    Compartment cell("cell", 10.0, 3);
    Compartment membrane("membrane", 1.0, 2, "cell");
    Compartment cytoplasm("cytoplasm", 5.0, 3, "membrane");
    Compartment nucleus_membrane("nucleus_membrane", 0.5, 2, "cytoplasm");
    Compartment nucleus("nucleus", 1.0, 3, "nucleus_membrane");

    std::vector<Compartment> all_compartments = {
        cell, membrane, cytoplasm, nucleus_membrane, nucleus
    };

    SECTION("isInside") {
        REQUIRE(cytoplasm.isInside("membrane"));
        REQUIRE_FALSE(cytoplasm.isInside("cell")); // isInside checks direct parent
        REQUIRE_FALSE(cell.isInside("membrane"));
    }

    SECTION("isOutside") {
        // cytoplasm parent is membrane.
        // membrane.isOutside("cytoplasm") should check if any compartment c has name "cytoplasm" and parent "membrane".
        REQUIRE(membrane.isOutside("cytoplasm", all_compartments));
        REQUIRE_FALSE(membrane.isOutside("cell", all_compartments));
        REQUIRE_FALSE(cytoplasm.isOutside("membrane", all_compartments));
    }

    SECTION("isAdjacent") {
        // Direct parent relationship
        REQUIRE(cytoplasm.isAdjacent("membrane", all_compartments));
        REQUIRE(membrane.isAdjacent("cytoplasm", all_compartments));

        // Shared parent surface (not directly supported by current AST logic unless one is parent of another)
        // Wait, looking at the code:
        // if (parent_ == other) return true;
        // for (const auto& c : all) { if (c.getName() == other && c.getParent() == name_) return true; }
        // The implementation implies they are adjacent if one is the direct parent of the other.
        REQUIRE_FALSE(cytoplasm.isAdjacent("cell", all_compartments));
        REQUIRE_FALSE(cytoplasm.isAdjacent("nucleus", all_compartments));
    }

    SECTION("adjacency") {
        // cytoplasm parent is membrane
        REQUIRE(cytoplasm.adjacency("membrane", all_compartments) == 1); // this is inside other
        REQUIRE(membrane.adjacency("cytoplasm", all_compartments) == -1); // other is inside this
        REQUIRE(cytoplasm.adjacency("cell", all_compartments) == 0); // no direct containment
    }
}
