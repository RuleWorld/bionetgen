#pragma once

#include <string>
#include <vector>

namespace bng::ast {

/**
 * PopulationMap - Represents a mapping from a species pattern to a
 * population-level variable/function.
 *
 * In BNG2, the "begin population maps" block defines how individual
 * species patterns are aggregated into population-level variables for
 * hybrid population/particle simulations.
 *
 * Grammar: [label:] species_pattern -> functionName(args...)
 *
 * Note: Parsing and storage is supported. Actual population-based
 * simulation refinement (RefineRule) is not yet executed.
 */
struct PopulationMap {
    std::string label;
    std::string patternText;         // raw species pattern text
    std::string populationFunction;  // function name (e.g., "lumped")
    std::vector<std::string> functionArgs;
};

} // namespace bng::ast
