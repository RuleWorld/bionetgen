#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_string.hpp>

#include "../src/io/SbmlWriter.hpp"
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

TEST_CASE("SbmlWriter numeric vs non-numeric rate law", "[SbmlWriter]") {
    Model model;
    model.addMoleculeType(MoleculeType("A", {}));

    // Parameter
    Parameter k1("k1", Expression::number(1.5));
    k1.setValue(1.5);
    model.addParameter(k1);

    // Another Parameter
    Parameter non_numeric_param("k_complex", Expression::number(2.0));
    non_numeric_param.setValue(2.0);
    model.addParameter(non_numeric_param);

    GeneratedNetwork network;
    network.species.add(Species(makeSpeciesGraph("A()", model), 10.0));

    SECTION("Numeric rate law") {
        network.reactions.add(Rxn("R1", {0}, {0}, "1.5", 1.0, "Rule1"));
        std::string result = SbmlWriter::write(model, &network);
        REQUIRE(result.find("<cn> 1.5 </cn>") != std::string::npos);
    }

    SECTION("Non-numeric rate law") {
        network.reactions.add(Rxn("R2", {0}, {0}, "k_complex", 1.0, "Rule2"));
        std::string result = SbmlWriter::write(model, &network);
        REQUIRE(result.find("<ci> k_complex </ci>") != std::string::npos);
    }

    SECTION("Invalid numeric rate law is treated as non-numeric") {
        network.reactions.add(Rxn("R3", {0}, {0}, "1.5abc", 1.0, "Rule3"));
        std::string result = SbmlWriter::write(model, &network);
        REQUIRE(result.find("<ci> _1_5abc </ci>") != std::string::npos);
    }
}
