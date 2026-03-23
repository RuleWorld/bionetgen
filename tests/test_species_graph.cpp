#include <string>

#include <catch2/catch_test_macros.hpp>

#include "TestUtils.hpp"

TEST_CASE("Seed species build non-empty pattern graphs with stable canonical labels", "[species-graph]") {
    const auto model = test::parseModelFile(test::sourcePath("tests/models/blbr.bngl"));
    REQUIRE(model);
    REQUIRE(model->getSeedSpecies().size() >= 2);

    const auto& receptor = model->getSeedSpecies()[0];
    const auto& ligand = model->getSeedSpecies()[1];

    CHECK(!receptor.getGraph().get_BNG2_string().empty());
    CHECK(!ligand.getGraph().get_BNG2_string().empty());
    CHECK(!receptor.getCanonicalLabel().empty());
    CHECK(!ligand.getCanonicalLabel().empty());
    CHECK(receptor.getCanonicalLabel() != ligand.getCanonicalLabel());
}

TEST_CASE("Identical species strings canonicalize the same way", "[species-graph]") {
    const auto first = test::parseModelFile(test::sourcePath("tests/models/species_label_a.bngl"));
    const auto second = test::parseModelFile(test::sourcePath("tests/models/species_label_b.bngl"));

    REQUIRE(first);
    REQUIRE(second);
    REQUIRE(first->getSeedSpecies().size() == 1);
    REQUIRE(second->getSeedSpecies().size() == 1);

    CHECK(first->getSeedSpecies()[0].getCanonicalLabel() == second->getSeedSpecies()[0].getCanonicalLabel());
}
