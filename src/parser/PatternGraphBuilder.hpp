#pragma once

#include "ast/Model.hpp"
#include "generated/BNGParser.h"

namespace bng::parser {

BNGcore::PatternGraph buildPatternGraph(
	BNGParser::Species_defContext* ctx,
	ast::Model& model,
	bool treatUnspecifiedBondAsWildcard = false);

std::string extractSpeciesCompartment(BNGParser::Species_defContext* ctx);
bool isSpeciesCompartmentPrefix(BNGParser::Species_defContext* ctx);

} // namespace bng::parser
