#include <catch2/catch_test_macros.hpp>
#include <cstdlib>
#include "../src/engine/NetworkGenerator.hpp"
// Include implementation to exercise evaluateRateString in anonymous namespace.
#include "../src/engine/NetworkGenerator.cpp"

using namespace bng::engine;

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
