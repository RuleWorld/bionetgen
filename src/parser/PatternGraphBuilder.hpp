#pragma once

#include "ast/Model.hpp"
#include "generated/BNGParser.h"

namespace bng::parser {

BNGcore::PatternGraph buildPatternGraph(BNGParser::Species_defContext* ctx, ast::Model& model);

} // namespace bng::parser
