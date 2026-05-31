#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_string.hpp>
#include <sstream>

#include "../src/io/LatexWriter.hpp"
#include "../src/ast/Model.hpp"
#include "../src/engine/NetworkGenerator.hpp"
#include "../src/parser/PatternGraphBuilder.hpp"
#include "BNGLexer.h"
#include "BNGParser.h"

using namespace bng::io;
using namespace bng::ast;
using namespace bng::engine;

static SpeciesGraph makeSpeciesGraph(const std::string& patternText, Model& model) {
    antlr4::ANTLRInputStream input(patternText);
    BNGLexer lexer(&input);
    antlr4::CommonTokenStream tokens(&lexer);
    BNGParser parser(&tokens);
    auto* species = parser.species_def();
    auto graph = bng::parser::buildPatternGraph(species, model, false);
    return SpeciesGraph(std::move(graph));
}

TEST_CASE("LatexWriter tests", "[LatexWriter]") {
    Model model;
    model.addMoleculeType(MoleculeType("A", {}));

    Parameter k1("k1", Expression::number(1.5));
    k1.setValue(1.5);
    model.addParameter(k1);

    model.addObservable(Observable("O1", "Species", {"A()"}));

    GeneratedNetwork network;
    network.species.add(Species(makeSpeciesGraph("A()", model), 10.0));

    SECTION("Numeric rate law") {
        network.reactions.add(Rxn("R1", {0}, {0}, "1.5", 1.0, "Rule1"));
        std::string result = LatexWriter::write(model, network);
        REQUIRE(result.find("1.5") != std::string::npos);
        REQUIRE(result.find("\\text{1.5}") == std::string::npos); // Should not fall back to non-numeric formatting
    }

    SECTION("Non-numeric rate law") {
        network.reactions.add(Rxn("R2", {0}, {0}, "k1*A", 1.0, "Rule2"));
        std::string result = LatexWriter::write(model, network);
        REQUIRE(result.find("\\text{k1*A}") != std::string::npos); // Should fall back to non-numeric formatting
    }

    SECTION("Malformed non-numeric rate law that starts with a number") {
        network.reactions.add(Rxn("R3", {0}, {0}, "1.5k1", 1.0, "Rule3"));
        std::string result = LatexWriter::write(model, network);
        REQUIRE(result.find("\\text{1.5k1}") != std::string::npos); // Should fall back to non-numeric formatting
    }
}
