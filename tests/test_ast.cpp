#include <catch2/catch_test_macros.hpp>
#include <antlr4-runtime.h>
#include "ast/Model.hpp"
#include "BNGLexer.h"
#include "BNGParser.h"

TEST_CASE("Model can add components", "[ast]") {
    bng::ast::Model model;

    SECTION("Adding a compartment") {
        model.addCompartment(bng::ast::Compartment("Cell", 1.0));
        REQUIRE(model.getCompartments().size() == 1);
        REQUIRE(model.getCompartments()[0].getName() == "Cell");
        REQUIRE(model.getCompartments()[0].getVolume() == 1.0);
    }

    SECTION("Adding a molecule") {
        model.addMolecule(bng::ast::Molecule("EGFR"));
        REQUIRE(model.getMolecules().size() == 1);
        REQUIRE(model.getMolecules()[0].getName() == "EGFR");
    }
}

TEST_CASE("Parser accepts a minimal parameters block", "[parser]") {
    std::string bngl = "begin parameters\n  k1 1.0\nend parameters\n";
    antlr4::ANTLRInputStream input(bngl);
    BNGLexer lexer(&input);
    antlr4::CommonTokenStream tokens(&lexer);
    BNGParser parser(&tokens);

    auto* tree = parser.prog();

    REQUIRE(tree != nullptr);
    REQUIRE(parser.getNumberOfSyntaxErrors() == 0);
}
