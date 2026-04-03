#include <catch2/catch_test_macros.hpp>

#include <filesystem>
#include <fstream>
#include <string>

#include "parser/BNGAstVisitor.hpp"

namespace {

std::filesystem::path sourceRoot() {
    return std::filesystem::path(BNG_CPP_SOURCE_DIR);
}

std::string readModel(const std::string& relativePath) {
    std::ifstream input(sourceRoot() / relativePath);
    REQUIRE(input.is_open());
    return std::string(
        std::istreambuf_iterator<char>(input),
        std::istreambuf_iterator<char>());
}

} // namespace

TEST_CASE("Semantic parser captures model headers and compartments", "[visitor][models]") {
    auto catalysis = bng::parser::parseModel(readModel("bng2/Models2/catalysis.bngl"));

    REQUIRE(catalysis->getVersion() == "2.2.4");
    REQUIRE(catalysis->getOptions().at("NumberPerQuantityUnit") == "6.0221e23");
    REQUIRE(catalysis->getCompartments().size() == 1);
    REQUIRE(catalysis->getCompartments()[0].getName() == "C");
    REQUIRE(catalysis->getCompartments()[0].getDimension() == 3);
}

TEST_CASE("Semantic parser captures richer counts from reference models", "[visitor][models]") {
    auto geneExpr = bng::parser::parseModel(readModel("bng2/Models2/gene_expr_func.bngl"));
    REQUIRE(geneExpr->getParameters().size() == 11);
    REQUIRE(geneExpr->getFunctions().size() == 2);
    REQUIRE(geneExpr->getObservables().size() == 4);
    REQUIRE(geneExpr->getActions().size() == 6);

    auto blbr = bng::parser::parseModel(readModel("bng2/Models2/blbr.bngl"));
    REQUIRE(blbr->getSeedSpecies().size() == 2);
    REQUIRE(blbr->getReactionRules().size() == 3);

    auto egfr = bng::parser::parseModel(readModel("bng2/Models2/egfr_net.bngl"));
    REQUIRE(egfr->getMoleculeTypes().size() >= 2);
}
