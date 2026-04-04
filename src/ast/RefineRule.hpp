#pragma once

#include <cstddef>
#include <functional>
#include <limits>
#include <string>
#include <unordered_map>
#include <vector>

#include "ReactionRule.hpp"
#include "SpeciesGraph.hpp"
#include "Model.hpp"

namespace bng::ast {

/**
 * RefineRule -- Hybrid particle/population rule refinement.
 *
 * Faithful C++ port of RefineRule.pm (lines 295-746).
 *
 * This module implements two key functions used in hybrid ODE/SSA/NFsim
 * simulations where certain species are represented as populations:
 *
 *   expandRule   -- expands a ReactionRule with respect to a list of
 *                   SpeciesGraphs, producing a set of "child rules" that
 *                   act on specific species rather than abstract patterns.
 *
 *   restrictRule -- given a particular set of reactant embeddings (a
 *                   "rule instance"), constructs a single "child rule"
 *                   by applying the rule's operations to the matched
 *                   reactants and performing all validation checks.
 */

/// Parameters for expandRule / restrictRule (mirrors Perl %params hash).
struct RefineRuleParams {
    bool verbose = false;
    bool safe = false;
    double maxAgg = 1e99;
    std::unordered_map<std::string, double> maxStoich;
    std::string indent = "        ";  // 8 spaces
};

/// Result of expandRule: a list of child rules.
struct ExpandRuleResult {
    std::string error;  // empty on success
    std::vector<ReactionRule> childRules;
};

/// Expand a reaction rule with respect to a list of species graphs.
///
/// Faithfully ports RefineRule.pm::expandRule (lines 295-425).
///
/// For each reactant pattern, finds all embeddings in the species graph list.
/// In safe mode: adds self-embedding for every pattern.
/// Otherwise: checks isomorphism and adds self-embedding only if the pattern
/// is not isomorphic to anything in sgList.
/// Generates a Cartesian product of all match combinations, calls restrictRule
/// for each, deduplicates child rules by StringID (accumulating statFactors),
/// and adjusts rate parameters for accumulated statFactors.
///
/// @param rule        The reaction rule to expand.
/// @param sgList      List of species graphs to match against.
/// @param model       The original model (for PopulationList, ParamList, etc.).
/// @param hybridModel The new hybrid model being constructed.
/// @param params      Optional parameters (verbose, safe, etc.).
/// @return            ExpandRuleResult with child rules or error.
ExpandRuleResult expandRule(
    const ReactionRule& rule,
    const std::vector<SpeciesGraph>& sgList,
    const Model& model,
    Model& hybridModel,
    const RefineRuleParams& params = {});

/// Restrict a reaction rule to a specific set of reactant embeddings.
///
/// Faithfully ports RefineRule.pm::restrict_rule (lines 436-746).
///
/// Given a rule and a specific set of embeddings (one per reactant pattern
/// into a target species graph), this function:
/// 1. Copies the target reactants and transfers labels from patterns
/// 2. Assigns temporary labels to unlabeled elements
/// 3. Applies rule operations (transformations) to get products
/// 4. Validates products against max_agg, max_stoich, exclude/include patterns
/// 5. Replaces matching products/reactants with population graph equivalents
/// 6. Removes redundant labels
/// 7. Constructs a child rule with new reactants/products
/// 8. Finds the mapping for the child rule
/// 9. Sets DeleteMolecules flag if needed
///
/// @param rule         The parent reaction rule.
/// @param ruleInstance The set of embeddings (one Map per reactant pattern).
/// @param model        The original model.
/// @param hybridModel  The hybrid model being constructed.
/// @param params       Optional parameters (max_agg, max_stoich, verbose).
/// @return             A new child ReactionRule, or nullptr if the rule
///                     instance is invalid (fails validation).
std::unique_ptr<ReactionRule> restrictRule(
    const ReactionRule& rule,
    const std::vector<ReactionRule::EmbeddingResult>& ruleInstance,
    const Model& model,
    Model& hybridModel,
    const RefineRuleParams& params = {});

/// Helper: compute factorial (used for stat factors).
/// Ports RefineRule.pm::factorial (lines 756-761).
double factorial(int n);

} // namespace bng::ast
