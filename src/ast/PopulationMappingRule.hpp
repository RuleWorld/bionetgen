#pragma once

#include <string>
#include <utility>

#include "ReactionRule.hpp"
#include "Model.hpp"

namespace bng::ast {

/**
 * PopulationMappingRule
 *
 * Faithful port of RefineRule.pm::newPopulationMappingRule (lines 13-282).
 *
 * Parses a population mapping rule string of the form:
 *     [name:] reactant_pattern -> population_product  rate_law
 *
 * - Exactly 1 reactant pattern (validated against MoleculeTypesList)
 * - Exactly 1 product pattern  (validated against PopulationTypesList)
 *   - Product must be a single molecule with no components
 *   - Product molecule name must NOT collide with existing MoleculeTypes
 * - Labels (%labels) are tracked: 'M' for molecule, 'C' for component
 *   in reactants, then matched to 'found' in products
 * - Rate law is parsed (supports TotalRate modifier)
 * - A ReactionRule object is constructed and findMap is called
 *
 * Returns (error_string, ReactionRule).  error is empty on success.
 */
struct PopulationMappingRuleResult {
    std::string error;
    ReactionRule rule;  // only valid when error.empty()
};

/// Parse a population mapping rule string and construct a ReactionRule.
/// Faithfully ports Perl RefineRule.pm::newPopulationMappingRule.
PopulationMappingRuleResult newPopulationMappingRule(
    const std::string& ruleString,
    Model& model);

} // namespace bng::ast
