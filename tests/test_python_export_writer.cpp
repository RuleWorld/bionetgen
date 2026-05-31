#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_string.hpp>
#include <sstream>

#include "../src/io/PythonExportWriter.hpp"
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

TEST_CASE("PythonExportWriter numerical vs expression rate laws", "[PythonExportWriter]") {
    Model model;
    model.addMoleculeType(MoleculeType("A", {}));
    model.addMoleculeType(MoleculeType("B", {}));

    Parameter k1("k1", Expression::number(1.5));
    k1.setValue(1.5);
    model.addParameter(k1);

    Parameter k2("k2", Expression::number(2.5));
    k2.setValue(2.5);
    model.addParameter(k2);

    GeneratedNetwork network;
    network.species.add(Species(makeSpeciesGraph("A()", model), 10.0));
    network.species.add(Species(makeSpeciesGraph("B()", model), 0.0));

    // Rxn with literal numerical rate
    network.reactions.add(Rxn("R1", {0}, {1}, "1.5", 1.0, "Rule1"));

    // Rxn with parameter expression rate
    network.reactions.add(Rxn("R2", {0}, {1}, "k2", 1.0, "Rule2"));

    std::string result = PythonExportWriter::write(model, network);

    // Verify parameter expressions calculation mapping
    REQUIRE(result.find("expressions[0] = parameters[0]  # k1") != std::string::npos);
    REQUIRE(result.find("expressions[1] = parameters[1]  # k2") != std::string::npos);

    // Verify rate expressions calculation function
    REQUIRE(result.find("ratelaws[0] = 1.5") != std::string::npos);
    REQUIRE(result.find("ratelaws[1] = expressions[1]") != std::string::npos);
}
