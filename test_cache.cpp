#include "io/NetWriter.hpp"
#include "ast/Model.hpp"
#include "engine/NetworkGenerator.hpp"
#include "parser/PatternGraphBuilder.hpp"
#include "generated/BNGLexer.h"
#include "generated/BNGParser.h"
#include "core/Ullmann.hpp"
#include <vector>
#include <string>

using namespace bng;

struct ParsedPattern {
    BNGcore::PatternGraph graph;
    std::string compartment;
};

int main() {
    std::vector<ParsedPattern> patterns;
    return 0;
}
