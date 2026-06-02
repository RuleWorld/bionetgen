#include <iostream>
#include <chrono>
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

int main() {
    Model model;
    model.addMoleculeType(MoleculeType("A", {}));

    // Add many parameters and observables to stress the NetWriter
    for (int i = 0; i < 10; ++i) {
        Parameter p("p" + std::to_string(i), Expression::number(1.0));
        p.setValue(1.0);
        model.addParameter(p);
    }

    // A large number of observables, each with a few patterns
    for (int i = 0; i < 100; ++i) {
        model.addObservable(Observable("Obs" + std::to_string(i), "Molecules", {"A()", "A(b~1)"}));
    }

    // Add some species
    GeneratedNetwork network;
    for (int i = 0; i < 100; ++i) {
        network.species.add(Species(makeSpeciesGraph("A(b~" + std::to_string(i%2) + ")", model), 1.0));
    }

    // And some reactions
    for (int i = 0; i < 100; ++i) {
        network.reactions.add(Rxn("R" + std::to_string(i), {static_cast<std::size_t>(i)}, {static_cast<std::size_t>((i+1)%100)}, "p1", 1.0, "Rule1"));
    }

    auto start = std::chrono::high_resolution_clock::now();

    // Call the writer multiple times
    for (int i = 0; i < 10; ++i) {
        std::stringstream ss;
        NetWriter::write(ss, model, network);
    }

    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> diff = end - start;
    std::cout << "Time: " << diff.count() << " s\n";

    return 0;
}
