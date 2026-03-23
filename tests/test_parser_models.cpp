#include <catch2/catch_test_macros.hpp>

#include <fstream>
#include <filesystem>
#include <string>

#include <antlr4-runtime.h>

#include "BNGLexer.h"
#include "BNGParser.h"

namespace {

std::filesystem::path sourceRoot() {
    return std::filesystem::path(BNG_CPP_SOURCE_DIR);
}

std::string readFile(const std::string& path) {
    std::ifstream input(sourceRoot() / path);
    REQUIRE(input.is_open());
    return std::string(
        std::istreambuf_iterator<char>(input),
        std::istreambuf_iterator<char>());
}

void requireParsesWithoutSyntaxErrors(const std::string& path) {
    const std::string bngl = readFile(path);
    antlr4::ANTLRInputStream input(bngl);
    BNGLexer lexer(&input);
    antlr4::CommonTokenStream tokens(&lexer);
    BNGParser parser(&tokens);

    auto* tree = parser.prog();

    REQUIRE(tree != nullptr);
    REQUIRE(parser.getNumberOfSyntaxErrors() == 0);
}

} // namespace

TEST_CASE("Reference models parse without syntax errors", "[parser][models]") {
    requireParsesWithoutSyntaxErrors("tests/models/blbr.bngl");
    requireParsesWithoutSyntaxErrors("tests/models/catalysis.bngl");
    requireParsesWithoutSyntaxErrors("tests/models/egfr_net.bngl");
}
