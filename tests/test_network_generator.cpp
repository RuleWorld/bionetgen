#include <catch2/catch_test_macros.hpp>
#include "../src/engine/NetworkGenerator.hpp"
// Include implementation to exercise evaluateRateString in anonymous namespace.
#include "../src/engine/NetworkGenerator.cpp"

using namespace bng::engine;

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
