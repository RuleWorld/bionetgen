#pragma once

#include <cstddef>
#include <functional>
#include <limits>
#include <memory>
#include <string>
#include <unordered_set>
#include <vector>

#include "SpeciesGraph.hpp"
#include <unordered_map>
#include "SpeciesList.hpp"
#include "RxnList.hpp"
#include "Expression.hpp"

namespace bng::ast {

class Model;  // forward declaration

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
        const std::function<bool(const SpeciesGraph&)>& productFilter = {},
        std::size_t speciesBoundary = std::numeric_limits<std::size_t>::max(),
        const Model* model = nullptr) const;

private:
    std::vector<EmbeddingResult> findEmbeddingsForSpecies(
        std::size_t patternIndex,
        const SpeciesList& speciesList,
        const std::vector<std::size_t>& candidateSpecies,
        const Model* model = nullptr) const;

    bool buildReaction(
        const std::vector<EmbeddingResult>& matchSet,
        SpeciesList& speciesList,
        RxnList& rxnList,
        const std::function<bool(const SpeciesGraph&)>& productFilter,
        const Model* model = nullptr) const;

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
    mutable std::vector<std::size_t> lastProcessedInIteration_;
    bool hasScopePrefix_ = false;  // true if rule uses %x:: scope prefix syntax

    // Tag-based component mapping: product ComponentRef → reactant ComponentRef
    // Used when %tag labels map components across reactant/product boundaries
    std::map<ComponentRef, ComponentRef> tagComponentMap_;

    // Per-molecule compartment transport: for each product molecule (identified by
    // pattern index + molecule index), stores the compartment from the product pattern
    // and the corresponding reactant molecule ref.  Used in buildReaction to apply
    // compartment changes for bond-forming rules where product molecules originate
    // from different reactant patterns.
    struct MoleculeCompartmentTransport {
        std::size_t productPatternIndex;
        std::size_t productMoleculeIndex;
        std::string productCompartment;       // compartment on the product pattern molecule
        std::size_t reactantPatternIndex;
        std::size_t reactantMoleculeIndex;
    };
    std::vector<MoleculeCompartmentTransport> moleculeCompartmentTransports_;

    // Cross-boundary bonds: bonds in product patterns that connect a component in a
    // matched reactant molecule to a component in a newly-added product molecule.
    // These bonds are NOT captured as AddBond operations (which require both sides to
    // be reactant-mapped) and must be established separately in buildReaction after
    // AddMolecule operations clone the new molecules into the aggregate graph.
    struct CrossBondRef {
        ComponentRef reactantComponent;  // reactant-side component (has match mapping)
        ComponentRef productComponent;   // product-side component (in new molecule)
    };
    std::vector<CrossBondRef> crossBonds_;

    // Bonds between two newly-added product molecules (neither side mapped to reactant).
    // Both ComponentRefs reference product-side components.
    struct NewMoleculeBondRef {
        ComponentRef productComponent1;
        ComponentRef productComponent2;
    };
    std::vector<NewMoleculeBondRef> newMoleculeBonds_;

    // Product-only component state changes: product pattern specifies a component
    // with an explicit state (e.g. s~U) that was NOT mentioned in the reactant
    // pattern.  During buildReaction, we resolve by finding the named component on
    // the matched species molecule and setting its state / breaking bonds.
    struct ProductOnlyStateChange {
        std::size_t reactantPatternIndex;   // pattern index of the mapped reactant molecule
        std::size_t reactantMoleculeIndex;  // molecule index within that pattern
        std::string componentName;          // component name to find on the species molecule
        std::string newState;               // state to set (empty = don't change state)
        bool breakBond = false;             // if true, remove any existing bond on the component
    };
    std::vector<ProductOnlyStateChange> productOnlyStateChanges_;

    // True when a product molecule has the same type name as a reactant molecule
    // but a different set of components (different count or names).  This indicates
    // a molecule "replacement" (delete old + add new) where bonded partners of the
    // old molecule become orphans that should NOT survive as products.
    bool hasMoleculeTypeMismatch_ = false;

public:
    void setHasScopePrefix(bool v) { hasScopePrefix_ = v; }
    bool hasScopePrefix() const { return hasScopePrefix_; }
    const std::map<ComponentRef, ComponentRef>& getTagComponentMap() const { return tagComponentMap_; }
private:

    // Parsed include/exclude reactant and product filters
    struct ReactantFilter {
        enum class Type { Include, Exclude };
        enum class Side { Reactant, Product };
        Type type;
        Side side = Side::Reactant;
        std::size_t patternIndex; // 0-based
        std::string moleculeName;
    };
    std::vector<ReactantFilter> reactantFilters_;
    std::vector<ReactantFilter> productFilters_;
    void parseReactantFilters();
    bool passesReactantFilters(std::size_t patternIndex, const SpeciesGraph& species) const;
    bool passesProductFilters(const std::vector<SpeciesGraph>& products) const;
};

// Set compartment dimension map for cross-compartment species assignment
void setCompartmentDimensions(const std::unordered_map<std::string, int>& dims);
// Set compartment parent map for transport (endocytosis/exocytosis)
void setCompartmentParents(const std::unordered_map<std::string, std::string>& parents);

} // namespace bng::ast
