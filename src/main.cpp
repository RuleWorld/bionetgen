#include <fstream>
#include <iostream>
#include <memory>
#include <string>
#include <vector>

#include <antlr4-runtime.h>

#include "BNGLexer.h"
#include "BNGParser.h"
#include "actions/ActionDispatch.hpp"
#include "parser/BNGAstVisitor.hpp"

namespace {

struct ParseResult {
    int syntaxErrors = 0;
    bool opened = false;
    std::unique_ptr<bng::ast::Model> model;
    std::string error;
};

ParseResult parseFile(const std::string& path) {
    std::ifstream inputStream(path);
    if (!inputStream) {
        return {.syntaxErrors = -1, .opened = false, .model = nullptr, .error = {}};
    }

    antlr4::ANTLRInputStream input(inputStream);
    BNGLexer lexer(&input);
    antlr4::CommonTokenStream tokens(&lexer);
    BNGParser parser(&tokens);
    auto* tree = parser.prog();

    ParseResult result {
        .syntaxErrors = static_cast<int>(parser.getNumberOfSyntaxErrors()),
        .opened = true,
        .model = nullptr,
        .error = {},
    };

    if (result.syntaxErrors != 0) {
        return result;
    }

    try {
        bng::parser::BNGAstVisitor visitor;
        visitor.visit(tree);
        result.model = visitor.takeModel();
    } catch (const std::exception& ex) {
        result.error = ex.what();
    }

    return result;
}

void printUsage() {
    std::cerr << "Usage: bng_cpp [--check] [--verbose] [--version] <model1.bngl> [model2.bngl ...]\n";
}

} // namespace

int main(int argc, char** argv) {
    bool checkOnly = false;
    bool verbose = false;
    std::vector<std::string> inputs;

    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];
        if (arg == "--check") {
            checkOnly = true;
            continue;
        }
        if (arg == "--verbose") {
            verbose = true;
            continue;
        }
        if (arg == "--version") {
            std::cout << "bng_cpp version 4.0\n";
            return 0;
        }
        inputs.push_back(arg);
    }

    if (inputs.empty()) {
        printUsage();
        return 1;
    }

    int failures = 0;
    for (const auto& input : inputs) {
        const auto result = parseFile(input);
        if (!result.opened) {
            std::cerr << "error: could not open " << input << '\n';
            ++failures;
            continue;
        }

        if (result.syntaxErrors != 0) {
            std::cerr << input << ": parse failed with " << result.syntaxErrors << " syntax error(s)\n";
            ++failures;
            continue;
        }

        if (!result.error.empty()) {
            std::cerr << input << ": semantic parse failed: " << result.error << '\n';
            ++failures;
            continue;
        }

        std::cout << input << ": parse ok"
                  << " (parameters=" << result.model->getParameters().size()
                  << ", functions=" << result.model->getFunctions().size()
                  << ", molecule_types=" << result.model->getMoleculeTypes().size()
                  << ", seed_species=" << result.model->getSeedSpecies().size()
                  << ", observables=" << result.model->getObservables().size()
                  << ", reaction_rules=" << result.model->getReactionRules().size()
                  << ", actions=" << result.model->getActions().size()
                  << ")\n";
    }

    if (!checkOnly && failures == 0) {
        for (const auto& input : inputs) {
            const auto result = parseFile(input);
            if (result.opened && result.syntaxErrors == 0 && result.error.empty()) {
                if (verbose) {
                    std::cerr << "[bng_cpp] executing actions for " << input << '\n';
                }
                bng::actions::ActionDispatch::execute(*result.model, input, verbose);
            }
        }
    }

    return failures == 0 ? 0 : 1;
}
