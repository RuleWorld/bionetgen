#pragma once

#include <cstddef>
#include <functional>
#include <memory>
#include <string>
#include <unordered_set>
#include <vector>

#include "SpeciesGraph.hpp"
#include "SpeciesList.hpp"
#include "RxnList.hpp"
#include "Expression.hpp"

namespace bng::ast {

class ReactionRule {
public:
    struct ComponentRef {
        std::size_t patternIndex = 0;
        std::size_t moleculeIndex = 0;
        std::size_t componentIndex = 0;

        bool operator==(const ComponentRef& other) const;
        bool operator<(const ComponentRef& other) const;
    };

    struct TransformOp {
        enum class Type {
            AddBond,
            DeleteBond,
            ChangeState,
            AddMolecule,
            DeleteMolecule,
        };

        Type type;
        ComponentRef source;
        ComponentRef partner;
        std::size_t moleculeIndex = 0;
        std::size_t patternIndex = 0;
        std::string newState;
    };

    struct ReactionCenterRef {
        std::size_t patternIndex = 0;
        std::size_t moleculeIndex = 0;
        bool isComponent = false;
        std::size_t componentIndex = 0;
    };

    struct EmbeddingResult {
        std::size_t speciesIndex = 0;
        BNGcore::Map map;
        std::size_t multiplicity = 1;  // Number of Ullmann maps that collapse to this embedding
    };

    ReactionRule(
        std::string ruleName,
        std::string label,
        std::vector<std::string> reactants,
        std::vector<std::string> products,
        std::vector<Expression> rates,
        std::vector<std::string> modifiers,
        bool bidirectional,
        std::vector<SpeciesGraph> reactantPatterns = {},
        std::vector<SpeciesGraph> productPatterns = {});

    const std::string& getRuleName() const;
    const std::string& getLabel() const;
    const std::vector<std::string>& getReactants() const;
    const std::vector<std::string>& getProducts() const;
    const std::vector<Expression>& getRates() const;
    const std::vector<std::string>& getModifiers() const;
    bool isBidirectional() const;
    const std::vector<SpeciesGraph>& getReactantPatterns() const;
    const std::vector<SpeciesGraph>& getProductPatterns() const;
    const std::vector<TransformOp>& getOperations() const;

    void initialize();
    void clearPatternMatchCache() const;
    std::vector<EmbeddingResult> findEmbeddings(std::size_t patternIndex, const SpeciesList& speciesList) const;
    std::size_t expandRule(
        SpeciesList& speciesList,
        RxnList& rxnList,
        std::size_t currentIteration,
        const std::function<bool(const SpeciesGraph&)>& productFilter = {}) const;

private:
    std::vector<EmbeddingResult> findEmbeddingsForSpecies(
        std::size_t patternIndex,
        const SpeciesList& speciesList,
        const std::unordered_set<std::size_t>& candidateSpecies) const;

    bool buildReaction(
        const std::vector<EmbeddingResult>& matchSet,
        SpeciesList& speciesList,
        RxnList& rxnList,
        const std::function<bool(const SpeciesGraph&)>& productFilter) const;

    std::string ruleName_;
    std::string label_;
    std::vector<std::string> reactants_;
    std::vector<std::string> products_;
    std::vector<Expression> rates_;
    std::vector<std::string> modifiers_;
    bool bidirectional_;
    std::vector<SpeciesGraph> reactantPatterns_;
    std::vector<SpeciesGraph> productPatterns_;
    std::vector<TransformOp> operations_;
    std::vector<std::vector<ReactionCenterRef>> reactionCenter_;
    mutable std::vector<std::vector<EmbeddingResult>> patternMatches_;
    mutable bool matchesInitialized_ = false;
    mutable bool synthesisApplied_ = false;
    mutable std::size_t lastSpeciesListCapacity_ = 0;
    mutable std::unique_ptr<ReactionRule> reverseRule_;
    mutable std::map<std::size_t, std::size_t> lastProcessedInIteration_;

    // Parsed include/exclude reactant filters
    struct ReactantFilter {
        enum class Type { Include, Exclude };
        Type type;
        std::size_t patternIndex; // 0-based
        std::string moleculeName;
    };
    std::vector<ReactantFilter> reactantFilters_;
    void parseReactantFilters();
    bool passesReactantFilters(std::size_t patternIndex, const SpeciesGraph& species) const;
};

} // namespace bng::ast
