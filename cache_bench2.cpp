#include <iostream>
#include <chrono>
#include <sstream>

#include "io/NetWriter.hpp"
#include "ast/Model.hpp"
#include "engine/NetworkGenerator.hpp"
#include "parser/PatternGraphBuilder.hpp"
#include "generated/BNGLexer.h"
#include "generated/BNGParser.h"

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

    // Function that references observables, making it use localFunc rates
    Function f("myFunc", {"m"}, Expression::parse("10 * Obs1(m) + 5 * Obs2(m)"));
    model.addFunction(f);

    for (int i = 0; i < 2; ++i) {
        model.addObservable(Observable("Obs" + std::to_string(i+1), "Molecules", {"A()", "A(b~1)"}));
    }

    GeneratedNetwork network;
    for (int i = 0; i < 50; ++i) {
        network.species.add(Species(makeSpeciesGraph("A(b~" + std::to_string(i%2) + ")", model), 1.0));
    }

    for (int i = 0; i < 50; ++i) {
        ReactionRule r("Rule" + std::to_string(i), {}, {}, {"myFunc(a)"}, false);
        model.addReactionRule(r);
        network.reactions.add(Rxn("Rule" + std::to_string(i), {static_cast<std::size_t>(i)}, {static_cast<std::size_t>((i+1)%50)}, "10", 1.0, "Rule" + std::to_string(i)));
    }

    auto start = std::chrono::high_resolution_clock::now();

    for (int i = 0; i < 10; ++i) {
        std::stringstream ss;
        NetWriter::write(ss, model, network);
    }

    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> diff = end - start;
    std::cout << "Time: " << diff.count() << " s\n";

    return 0;
}
