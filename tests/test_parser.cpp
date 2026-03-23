#include <string>

#include <catch2/catch_test_macros.hpp>

#include "TestUtils.hpp"

namespace {

struct ModelExpectations {
    std::size_t parameters;
    std::size_t functions;
    std::size_t moleculeTypes;
    std::size_t seedSpecies;
    std::size_t observables;
    std::size_t reactionRules;
    std::size_t actions;
};

void checkCounts(const bng::ast::Model& model, const ModelExpectations& expected) {
    CHECK(model.getParameters().size() == expected.parameters);
    CHECK(model.getFunctions().size() == expected.functions);
    CHECK(model.getMoleculeTypes().size() == expected.moleculeTypes);
    CHECK(model.getSeedSpecies().size() == expected.seedSpecies);
    CHECK(model.getObservables().size() == expected.observables);
    CHECK(model.getReactionRules().size() == expected.reactionRules);
    CHECK(model.getActions().size() == expected.actions);
}

} // namespace

TEST_CASE("Parser handles a minimal BNGL model", "[parser]") {
    const auto model = bng::parser::parseModel(R"(
begin model
begin parameters
  k 1.0
end parameters
begin molecule types
  A(b)
end molecule types
begin seed species
  A(b) 10
end seed species
begin reaction rules
  A(b) -> A(b) k
end reaction rules
end model
generate_network({overwrite=>1})
)");

    REQUIRE(model);
    checkCounts(*model, {.parameters = 1, .functions = 0, .moleculeTypes = 1, .seedSpecies = 1, .observables = 0, .reactionRules = 1, .actions = 1});
}

TEST_CASE("Parser captures counts for representative BNGL models", "[parser][models]") {
    const auto blbr = test::parseModelFile(test::sourcePath("tests/models/blbr.bngl"));
    const auto catalysis = test::parseModelFile(test::sourcePath("tests/models/catalysis.bngl"));
    const auto egfr = test::parseModelFile(test::sourcePath("tests/models/egfr_net.bngl"));
    const auto fceri = test::parseModelFile(test::sourcePath("tests/models/fceri_ji.bngl"));

    REQUIRE(blbr);
    REQUIRE(catalysis);
    REQUIRE(egfr);
    REQUIRE(fceri);

    checkCounts(*blbr, {.parameters = 8, .functions = 0, .moleculeTypes = 2, .seedSpecies = 2, .observables = 0, .reactionRules = 3, .actions = 1});
    checkCounts(*catalysis, {.parameters = 17, .functions = 0, .moleculeTypes = 5, .seedSpecies = 5, .observables = 6, .reactionRules = 4, .actions = 2});
    checkCounts(*egfr, {.parameters = 48, .functions = 0, .moleculeTypes = 5, .seedSpecies = 6, .observables = 15, .reactionRules = 23, .actions = 2});
    checkCounts(*fceri, {.parameters = 26, .functions = 0, .moleculeTypes = 4, .seedSpecies = 4, .observables = 10, .reactionRules = 19, .actions = 2});
}

TEST_CASE("Parser captures string-level reaction rules", "[parser][rules]") {
    const auto model = test::parseModelFile(test::sourcePath("tests/models/blbr.bngl"));
    REQUIRE(model);
    REQUIRE(model->getReactionRules().size() == 3);

    const auto& ligandAddition = model->getReactionRules().front();
    CHECK(ligandAddition.getLabel().empty());
    REQUIRE(ligandAddition.getReactants().size() == 2);
    REQUIRE(ligandAddition.getProducts().size() == 1);
    REQUIRE(ligandAddition.getRates().size() == 2);
    CHECK(ligandAddition.getReactants()[0] == "R(r)");
    CHECK(ligandAddition.getReactants()[1] == "L(l,l)");
    CHECK(ligandAddition.getProducts()[0] == "R(r!1).L(l!1,l)");
    CHECK(ligandAddition.getRates()[0].toString() == "kp1");
    CHECK(ligandAddition.getRates()[1].toString() == "km1");
    CHECK(ligandAddition.isBidirectional());
}
