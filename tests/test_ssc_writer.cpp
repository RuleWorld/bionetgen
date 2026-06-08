#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_string.hpp>

#include "../src/io/SscWriter.hpp"
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

TEST_CASE("SscWriter tests", "[SscWriter]") {
    Model model;
    model.addMoleculeType(MoleculeType("A", {}));
    model.addMoleculeType(MoleculeType("B", {}));
    model.addMoleculeType(MoleculeType("C", {}));

    // Add parameters
    Parameter k1("k1", Expression::number(1.5));
    k1.setValue(1.5);
    model.addParameter(k1);

    Parameter k2("k_local", Expression::number(2.0));
    k2.setValue(2.0);
    model.addParameter(k2);

    GeneratedNetwork network;
    // Note: To skip iso checks in tests when using dummy graphs, we normally set setCheckIso(false), but let's just make valid graphs.
    network.species.setCheckIso(false);

    network.species.add(Species(makeSpeciesGraph("A()", model), 10.4));
    network.species.add(Species(makeSpeciesGraph("B()", model), 20.0)); // name starting with number
    network.species.add(Species(makeSpeciesGraph("C()", model), 0.0));

    SECTION("Basic parameters and species output") {
        std::string result = SscWriter::write(model, network);

        // Parameters
        REQUIRE(result.find("const k1 = 1.5;") != std::string::npos);
        REQUIRE(result.find("const k_local = 2;") != std::string::npos);

        // Species sanitization and initial counts (rounded)
        REQUIRE(result.find("new A__(10);") != std::string::npos);
        REQUIRE(result.find("new B__(20);") != std::string::npos);
        REQUIRE(result.find("new C__(0);") != std::string::npos);
    }

    SECTION("Reactions with non-numeric rate laws") {
        network.reactions.add(Rxn("R1", {0}, {1}, "k1", 1.0, "Rule1"));
        std::string result = SscWriter::write(model, network);
        REQUIRE(result.find("A__ -> B__, k1;") != std::string::npos);
    }

    SECTION("Reactions with numeric rate laws") {
        network.reactions.add(Rxn("R2", {0}, {2}, "1.5", 2.0, "Rule2")); // statFactor = 2.0
        std::string result = SscWriter::write(model, network);
        // combined = 1.5 * 2.0 = 3
        REQUIRE(result.find("A__ -> C__, 3;") != std::string::npos);
    }

    SECTION("Reactions with |local: directive") {
        network.reactions.add(Rxn("R3", {0, 1}, {2}, "k_local|local:1", 1.0, "Rule3"));
        std::string result = SscWriter::write(model, network);
        // |local:1 should be stripped
        REQUIRE(result.find("A__ + B__ -> C__, k_local;") != std::string::npos);
    }

    SECTION("Empty reactants and products") {
        network.reactions.add(Rxn("R4", {}, {0}, "k1", 1.0, "Rule4"));
        network.reactions.add(Rxn("R5", {0}, {}, "k1", 1.0, "Rule5"));
        std::string result = SscWriter::write(model, network);
        REQUIRE(result.find("0 -> A__, k1;") != std::string::npos);
        REQUIRE(result.find("A__ -> 0, k1;") != std::string::npos);
    }

    SECTION("Non-numeric rate laws with statFactor") {
        network.reactions.add(Rxn("R6", {0}, {1}, "k1", 0.5, "Rule6"));
        std::string result = SscWriter::write(model, network);
        REQUIRE(result.find("A__ -> B__, 0.5*k1;") != std::string::npos);
    }
}
