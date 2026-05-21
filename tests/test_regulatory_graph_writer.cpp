#include <catch2/catch_test_macros.hpp>
#include "../src/io/RegulatoryGraphWriter.hpp"
#include "../src/ast/Model.hpp"
#include "../src/engine/NetworkGenerator.hpp"

using namespace bng::io;
using namespace bng::ast;
using namespace bng::engine;

TEST_CASE("RegulatoryGraphWriter error path", "[RegulatoryGraphWriter]") {
    Model model;
    // We create an observable with an invalid BNGL pattern to trigger the syntax error
    model.addObservable(Observable("O1", "Species", {"A(invalid_syntax=="}));

    GeneratedNetwork network;
    // Add a dummy species so the loop inside RegulatoryGraphWriter::build iterates
    network.species.add(Species(SpeciesGraph(), 10.0));

    // Build the graph, it should safely handle the syntax error
    auto graph = RegulatoryGraphWriter::build(model, network);

    // Verify the error did not crash the program and weight calculation handled it
    REQUIRE(graph.observableNames.size() == 1);
    REQUIRE(graph.observableNames[0] == "O1");
    // Since there are no species or rules, edges should be empty
    REQUIRE(graph.edges.empty());
}
