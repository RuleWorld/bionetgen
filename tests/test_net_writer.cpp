#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_string.hpp>
#include <sstream>

#include "../src/io/NetWriter.hpp"
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

TEST_CASE("NetWriter tests via ostream", "[NetWriter]") {
    Model model;
    model.addMoleculeType(MoleculeType("A", {}));

    Parameter k1("k1", Expression::number(1.5));
    k1.setValue(1.5);
    model.addParameter(k1);

    model.addObservable(Observable("O1", "Species", {"A()"}));

    GeneratedNetwork network;
    network.species.add(Species(makeSpeciesGraph("A()", model), 10.0));
    network.reactions.add(Rxn("R1", {0}, {0}, "k1", 1.0, "Rule1"));

    std::stringstream ss;
    NetWriter::write(ss, model, network);
    std::string result = ss.str();

    REQUIRE(result.find("begin parameters") != std::string::npos);
    REQUIRE(result.find("1 k1 1.5") != std::string::npos);
    REQUIRE(result.find("begin species") != std::string::npos);
    REQUIRE(result.find("1 A() 10") != std::string::npos);
    REQUIRE(result.find("begin reactions") != std::string::npos);
    REQUIRE(result.find("1 1 1 k1 #Rule1") != std::string::npos);
    REQUIRE(result.find("begin groups") != std::string::npos);
    REQUIRE(result.find("1 O1 1") != std::string::npos);
}
