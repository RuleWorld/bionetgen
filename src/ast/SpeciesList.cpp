#include "SpeciesList.hpp"

#include <stdexcept>
#include <utility>

#include "core/List.hpp"
#include "core/Ullmann.hpp"

namespace bng::ast {

namespace {

bool isIsomorphic(const SpeciesGraph& lhs, const SpeciesGraph& rhs) {
    // For raw-string species (loaded from .net files), skip graph isomorphism
    if (lhs.getGraph().empty() || rhs.getGraph().empty()) {
        return lhs.toString() == rhs.toString();
    }
    // Quick size check: graphs of different sizes can't be isomorphic
    if (lhs.getGraph().size() != rhs.getGraph().size()) {
        return false;
    }
    BNGcore::UllmannSGIso matcher(lhs.getGraph(), rhs.getGraph());
    BNGcore::List<BNGcore::Map> maps;
    matcher.find_maps(maps);
    for (auto mapIter = maps.begin(); mapIter != maps.end(); ++mapIter) {
        bool compatible = true;
        for (auto nodeIter = lhs.getGraph().begin(); nodeIter != lhs.getGraph().end(); ++nodeIter) {
            auto* mapped = mapIter->mapf(*nodeIter);
            if (mapped == nullptr || !((*nodeIter)->get_type() == mapped->get_type()) ||
                !((*nodeIter)->get_state() == mapped->get_state())) {
                compatible = false;
                break;
            }
        }
        if (compatible) {
            return true;
        }
    }
    return false;
}

} // namespace

void SpeciesList::setCheckIso(bool enabled) {
    checkIso_ = enabled;
}

bool SpeciesList::getCheckIso() const {
    return checkIso_;
}

std::pair<std::size_t, bool> SpeciesList::add(Species species) {
    // When check_iso is disabled, skip all dedup and add unconditionally
    if (!checkIso_) {
        const std::size_t index = species_.size();
        species.setIndex(index);
        species_.push_back(std::move(species));
        return {index, true};
    }

    const std::string label = species.getSpeciesGraph().canonicalLabel();
    // Use compartment-aware string for dedup to distinguish species that differ
    // only by per-molecule compartments (e.g., Im@CP.NP vs Im@NU.NP).
    const std::string exact = species.getSpeciesGraph().toStringForDedup();

    // Fast path 1: exact string match (O(1))
    const auto exactBucket = indicesByExactString_.find(exact);
    if (exactBucket != indicesByExactString_.end()) {
        for (const auto index : exactBucket->second) {
            auto& existingSpecies = species_[index];
            if (existingSpecies.getCompartment() != species.getCompartment()) {
                continue;
            }
            if (existingSpecies.getCompartment().empty() && !species.getCompartment().empty()) {
                existingSpecies.setCompartment(species.getCompartment());
            }
            return {index, false};
        }
    }

    // Fast path 2: canonical label match (O(1))
    // Note: canonical label is compartment-blind. Species with same graph structure
    // and same species-level compartment but different per-molecule compartments
    // (e.g., Im@CP.NP vs Im@NU.NP, both at species @NM) must NOT be merged.
    // Check dedup string to distinguish them.
    const auto existing = indicesByLabel_.find(label);
    if (existing != indicesByLabel_.end()) {
        for (const auto index : existing->second) {
            auto& existingSpecies = species_[index];
            if (existingSpecies.getCompartment() != species.getCompartment()) {
                continue;
            }
            // Per-molecule compartment check: when species have compartments,
            // dedup strings must match to avoid merging species that differ
            // only in per-molecule compartments (e.g., Im@CP.NP vs Im@NU.NP)
            if (!species.getCompartment().empty() &&
                existingSpecies.getSpeciesGraph().toStringForDedup() != exact) {
                continue;
            }
            if (existingSpecies.getCompartment().empty() && !species.getCompartment().empty()) {
                existingSpecies.setCompartment(species.getCompartment());
            }
            return {index, false};
        }
    }

    // Fast path 3: structural fingerprint match (O(1) lookup + targeted Ullmann).
    // The fingerprint is a graph-structure invariant: isomorphic species always produce
    // the same fingerprint. This replaces the old O(n) scan over all species with an
    // O(1) hash lookup. Only species in the same fingerprint bucket need Ullmann
    // confirmation (which now correctly handles in-edges for full isomorphism).
    const std::string fp = species.getSpeciesGraph().fingerprint();
    const auto fpBucket = indicesByFingerprint_.find(fp);
    if (fpBucket != indicesByFingerprint_.end()) {
        for (const auto index : fpBucket->second) {
            auto& existingSpecies = species_[index];
            if (existingSpecies.getCompartment() != species.getCompartment()) {
                continue;
            }
            if (!isIsomorphic(existingSpecies.getSpeciesGraph(), species.getSpeciesGraph())) {
                continue;
            }
            if (!species.getCompartment().empty() &&
                existingSpecies.getSpeciesGraph().toStringForDedup() != exact) {
                continue;
            }
            if (existingSpecies.getCompartment().empty() && !species.getCompartment().empty()) {
                existingSpecies.setCompartment(species.getCompartment());
            }
            indicesByLabel_[label].push_back(index);
            return {index, false};
        }
    }

    const std::size_t index = species_.size();
    species.setIndex(index);
    species_.push_back(std::move(species));
    indicesByLabel_[label].push_back(index);
    indicesByExactString_[exact].push_back(index);
    indicesByFingerprint_[fp].push_back(index);
    return {index, true};
}

const Species& SpeciesList::get(std::size_t index) const {
    return species_.at(index);
}

Species& SpeciesList::get(std::size_t index) {
    return species_.at(index);
}

bool SpeciesList::containsLabel(const std::string& canonicalLabel) const {
    const auto found = indicesByLabel_.find(canonicalLabel);
    return found != indicesByLabel_.end() && !found->second.empty();
}

std::size_t SpeciesList::indexOfLabel(const std::string& canonicalLabel) const {
    const auto found = indicesByLabel_.find(canonicalLabel);
    if (found == indicesByLabel_.end() || found->second.empty()) {
        throw std::runtime_error("Unknown species label: " + canonicalLabel);
    }
    return found->second.front();
}

std::size_t SpeciesList::size() const {
    return species_.size();
}

std::size_t SpeciesList::capacity() const {
    return species_.capacity();
}

const std::vector<Species>& SpeciesList::all() const {
    return species_;
}

} // namespace bng::ast
