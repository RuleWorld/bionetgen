#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_string.hpp>

#include "../src/io/CppExportWriter.hpp"
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

TEST_CASE("CppExportWriter basic export", "[CppExportWriter]") {
    Model model;
    model.setModelName("test_model");
    model.addMoleculeType(MoleculeType("A", {}));
    model.addMoleculeType(MoleculeType("B", {}));

    Parameter k1("k1", Expression::number(1.5));
    k1.setValue(1.5);
    model.addParameter(k1);

    GeneratedNetwork network;
    network.species.add(Species(makeSpeciesGraph("A()", model), 10.0));
    network.species.add(Species(makeSpeciesGraph("B()", model), 0.0));

    network.reactions.add(Rxn("R1", {0}, {1}, "k1", 1.0, "Rule1"));

    std::string result = CppExportWriter::write(model, network);

    // Verify it includes expected C++ CVode code
    REQUIRE_THAT(result, Catch::Matchers::ContainsSubstring("#include <cvode/cvode.h>"));
    REQUIRE_THAT(result, Catch::Matchers::ContainsSubstring("void\ncalc_observables ( N_Vector observables, N_Vector species, N_Vector expressions )"));
    REQUIRE_THAT(result, Catch::Matchers::ContainsSubstring("void\ncalc_ratelaws ( N_Vector ratelaws, N_Vector species, N_Vector expressions, N_Vector observables )"));
    REQUIRE_THAT(result, Catch::Matchers::ContainsSubstring("NV_Ith_S(ratelaws,0) = NV_Ith_S(expressions,0)*NV_Ith_S(species,0);"));
}
