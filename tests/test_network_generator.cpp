#include <catch2/catch_test_macros.hpp>
#include <filesystem>
#include <cstdlib>
#include "../src/engine/NetworkGenerator.hpp"
#include "../src/ast/Model.hpp"
#include "../src/parser/PatternGraphBuilder.hpp"
#include "BNGLexer.h"
#include "BNGParser.h"
// Include implementation to exercise evaluateRateString in anonymous namespace.
#include "../src/engine/NetworkGenerator.cpp"

using namespace bng::engine;
using namespace bng::ast;

TEST_CASE("parsePrintIter behaves correctly", "[NetworkGenerator]") {
    // default case: empty model
    bng::ast::Model model;

    // ensure env var is unset
    unsetenv("BNG_CPP_PROGRESS");
    REQUIRE(parsePrintIter(model) == false);

    // action without print_iter
    bng::ast::Action generateNetAction;
    generateNetAction.name = "generate_network";
    model.addAction(generateNetAction);
    REQUIRE(parsePrintIter(model) == false);

    // non-matching action with print_iter
    bng::ast::Model modelDiffAction;
    bng::ast::Action otherAction;
    otherAction.name = "simulate";
    otherAction.arguments["print_iter"] = "1";
    modelDiffAction.addAction(otherAction);
    REQUIRE(parsePrintIter(modelDiffAction) == false);

    // matching action with print_iter=1
    bng::ast::Model modelWithPrintIter;
    bng::ast::Action generateNetPrintAction;
    generateNetPrintAction.name = "generate_network";
    generateNetPrintAction.arguments["print_iter"] = "1";
    modelWithPrintIter.addAction(generateNetPrintAction);
    REQUIRE(parsePrintIter(modelWithPrintIter) == true);

    // matching action with print_iter=0
    bng::ast::Model modelWithPrintIterFalse;
    bng::ast::Action generateNetPrintActionFalse;
    generateNetPrintActionFalse.name = "generate_network";
    generateNetPrintActionFalse.arguments["print_iter"] = "0";
    modelWithPrintIterFalse.addAction(generateNetPrintActionFalse);
    REQUIRE(parsePrintIter(modelWithPrintIterFalse) == false);

    // env var overrides model false -> true
    setenv("BNG_CPP_PROGRESS", "1", 1);
    REQUIRE(parsePrintIter(model) == true);
    REQUIRE(parsePrintIter(modelDiffAction) == true);
    REQUIRE(parsePrintIter(modelWithPrintIterFalse) == true);

    // env var overrides model true -> false
    setenv("BNG_CPP_PROGRESS", "0", 1);
    REQUIRE(parsePrintIter(modelWithPrintIter) == false);

    // clean up
    unsetenv("BNG_CPP_PROGRESS");
}


TEST_CASE("parsePrintRules behaves correctly", "[NetworkGenerator]") {
    // default case: empty model
    bng::ast::Model model;

    // ensure env var is unset
    unsetenv("BNG_CPP_PROGRESS_RULES");
    REQUIRE(parsePrintRules(model) == false);

    // action without print_rule_progress
    bng::ast::Action generateNetAction;
    generateNetAction.name = "generate_network";
    model.addAction(generateNetAction);
    REQUIRE(parsePrintRules(model) == false);

    // non-matching action with print_rule_progress
    bng::ast::Model modelDiffAction;
    bng::ast::Action otherAction;
    otherAction.name = "simulate";
    otherAction.arguments["print_rule_progress"] = "1";
    modelDiffAction.addAction(otherAction);
    REQUIRE(parsePrintRules(modelDiffAction) == false);

    // matching action with print_rule_progress=1
    bng::ast::Model modelWithPrintRules;
    bng::ast::Action generateNetPrintAction;
    generateNetPrintAction.name = "generate_network";
    generateNetPrintAction.arguments["print_rule_progress"] = "1";
    modelWithPrintRules.addAction(generateNetPrintAction);
    REQUIRE(parsePrintRules(modelWithPrintRules) == true);

    // matching action with print_rule_progress=0
    bng::ast::Model modelWithPrintRulesFalse;
    bng::ast::Action generateNetPrintActionFalse;
    generateNetPrintActionFalse.name = "generate_network";
    generateNetPrintActionFalse.arguments["print_rule_progress"] = "0";
    modelWithPrintRulesFalse.addAction(generateNetPrintActionFalse);
    REQUIRE(parsePrintRules(modelWithPrintRulesFalse) == false);

    // env var overrides model false -> true
    setenv("BNG_CPP_PROGRESS_RULES", "1", 1);
    REQUIRE(parsePrintRules(model) == true);
    REQUIRE(parsePrintRules(modelDiffAction) == true);
    REQUIRE(parsePrintRules(modelWithPrintRulesFalse) == true);

    // env var overrides model true -> false
    setenv("BNG_CPP_PROGRESS_RULES", "0", 1);
    REQUIRE(parsePrintRules(modelWithPrintRules) == false);

    // clean up
    unsetenv("BNG_CPP_PROGRESS_RULES");
}

TEST_CASE("parseBooleanLike behaves correctly", "[NetworkGenerator]") {
    // positive cases
    REQUIRE(parseBooleanLike("1") == true);
    REQUIRE(parseBooleanLike("true") == true);
    REQUIRE(parseBooleanLike("yes") == true);
    REQUIRE(parseBooleanLike("on") == true);

    // case insensitivity
    REQUIRE(parseBooleanLike("TRUE") == true);
    REQUIRE(parseBooleanLike("YeS") == true);
    REQUIRE(parseBooleanLike("ON") == true);

    // negative cases
    REQUIRE(parseBooleanLike("0") == false);
    REQUIRE(parseBooleanLike("false") == false);
    REQUIRE(parseBooleanLike("no") == false);
    REQUIRE(parseBooleanLike("off") == false);

    // empty string and invalid
    REQUIRE(parseBooleanLike("") == false);
    REQUIRE(parseBooleanLike("asdf") == false);
    REQUIRE(parseBooleanLike("2") == false);
}

static SpeciesGraph makeSpeciesGraph(const std::string& patternText, Model& model) {
    antlr4::ANTLRInputStream input(patternText);
    BNGLexer lexer(&input);
    antlr4::CommonTokenStream tokens(&lexer);
    BNGParser parser(&tokens);
    auto* species = parser.species_def();
    auto graph = bng::parser::buildPatternGraph(species, model, false);
    return SpeciesGraph(std::move(graph));
}

TEST_CASE("withinStoichLimits handles empty limits", "[NetworkGenerator]") {
    Model model;
    model.addMoleculeType(MoleculeType("A", {}));
    SpeciesGraph graph = makeSpeciesGraph("A()", model);
    std::map<std::string, std::size_t> limits; // empty limits
    REQUIRE(withinStoichLimits(graph, limits) == true);
}

TEST_CASE("withinStoichLimits enforces molecule counts", "[NetworkGenerator]") {
    Model model;
    model.addMoleculeType(MoleculeType("A", {}));
    model.addMoleculeType(MoleculeType("B", {}));

    // A().B().A() implies 2 of A, 1 of B
    SpeciesGraph graph = makeSpeciesGraph("A().B().A()", model);

    SECTION("No limits applied") {
        std::map<std::string, std::size_t> limits;
        REQUIRE(withinStoichLimits(graph, limits) == true);
    }

    SECTION("Within limits for both") {
        std::map<std::string, std::size_t> limits = {{"A", 2}, {"B", 1}};
        REQUIRE(withinStoichLimits(graph, limits) == true);
    }

    SECTION("Above limit for A") {
        std::map<std::string, std::size_t> limits = {{"A", 1}, {"B", 1}};
        REQUIRE(withinStoichLimits(graph, limits) == false);
    }

    SECTION("Above limit for B") {
        std::map<std::string, std::size_t> limits = {{"A", 2}, {"B", 0}};
        REQUIRE(withinStoichLimits(graph, limits) == false);
    }

    SECTION("Limits have unrelated entries") {
        std::map<std::string, std::size_t> limits = {{"A", 2}, {"C", 0}};
        REQUIRE(withinStoichLimits(graph, limits) == true);
    }
}

TEST_CASE("parseOverwrite behaves correctly", "[NetworkGenerator]") {
    bng::ast::Model model;

    SECTION("returns true by default (no actions)") {
        REQUIRE(parseOverwrite(model) == true);
    }

    SECTION("returns true if no generate_network action exists") {
        bng::ast::Action action;
        action.name = "simulate";
        action.arguments["method"] = "ode";
        model.addAction(action);
        REQUIRE(parseOverwrite(model) == true);
    }

    SECTION("returns true if generate_network exists but no overwrite argument") {
        bng::ast::Action action;
        action.name = "generate_network";
        action.arguments["print_iter"] = "1";
        model.addAction(action);
        REQUIRE(parseOverwrite(model) == true);
    }

    SECTION("returns true if overwrite is set to true") {
        bng::ast::Action action;
        action.name = "generate_network";
        action.arguments["overwrite"] = "1";
        model.addAction(action);
        REQUIRE(parseOverwrite(model) == true);
    }

    SECTION("returns false if overwrite is set to false") {
        bng::ast::Action action;
        action.name = "generate_network";
        action.arguments["overwrite"] = "0";
        model.addAction(action);
        REQUIRE(parseOverwrite(model) == false);
    }

    SECTION("ignores generate_network without overwrite and finds the one with overwrite") {
        bng::ast::Action action1;
        action1.name = "generate_network";
        action1.arguments["print_iter"] = "1";
        model.addAction(action1);

        bng::ast::Action action2;
        action2.name = "generate_network";
        action2.arguments["overwrite"] = "0";
        model.addAction(action2);

        REQUIRE(parseOverwrite(model) == false);
    }

    SECTION("handles other boolean-like value 'yes' for overwrite") {
        bng::ast::Action action;
        action.name = "generate_network";
        action.arguments["overwrite"] = "yes";
        model.addAction(action);
        REQUIRE(parseOverwrite(model) == true);
    }

    SECTION("handles other boolean-like value 'no' for overwrite") {
        bng::ast::Action action;
        action.name = "generate_network";
        action.arguments["overwrite"] = "no";
        model.addAction(action);
        REQUIRE(parseOverwrite(model) == false);
    }
}

TEST_CASE("NetworkGenerator::generate behaves correctly", "[NetworkGenerator]") {
    bng::ast::Model model;
    bng::engine::NetworkGenerator generator(model);

    SECTION("writes to file when path is provided") {
        auto tempDir = std::filesystem::temp_directory_path();
        auto bnglPath = tempDir / "test_model_for_generate.bngl";
        auto expectedNetPath = tempDir / "test_model_for_generate.net";

        if (std::filesystem::exists(expectedNetPath)) {
            std::filesystem::remove(expectedNetPath);
        }

        generator.generate(bnglPath);

        REQUIRE(std::filesystem::exists(expectedNetPath));

        std::filesystem::remove(expectedNetPath);
    }

    SECTION("does not write to file when path is empty") {
        REQUIRE_NOTHROW(generator.generate(std::filesystem::path("")));
    }
}

TEST_CASE("generateNative drives network generation", "[NetworkGenerator]") {
    Model model;

    model.addMoleculeType(MoleculeType("A", {}));
    model.addMoleculeType(MoleculeType("B", {}));

    SpeciesGraph seedGraph = makeSpeciesGraph("A()", model);
    model.addSeedSpecies(SeedSpecies(seedGraph.getGraph().get_raw_string(), Expression::number(100.0), false, "", makeSpeciesGraph("A()", model).getGraph()));

    SECTION("Rule expansion with convergence") {
        std::vector<SpeciesGraph> reactantPatterns;
        reactantPatterns.push_back(makeSpeciesGraph("A()", model));

        std::vector<SpeciesGraph> productPatterns;
        productPatterns.push_back(makeSpeciesGraph("B()", model));

        ReactionRule rule("R1", "A() -> B()", {"A()"}, {"B()"}, {Expression::number(1.0)}, {}, false, std::move(reactantPatterns), std::move(productPatterns));
        rule.initialize();
        model.addReactionRule(std::move(rule));

        NetworkGenerator generator(model);

        auto network = generator.generateNative(5);

        REQUIRE(network.species.size() == 2);
        REQUIRE(network.reactions.size() == 1);
    }
}
