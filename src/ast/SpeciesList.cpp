#include "SpeciesList.hpp"

#include <stdexcept>
#include <utility>

#include "core/List.hpp"
#include "core/Ullmann.hpp"

namespace bng::ast {

namespace {

bool isIsomorphic(const SpeciesGraph& lhs, const SpeciesGraph& rhs) {
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

std::pair<std::size_t, bool> SpeciesList::add(Species species) {
    const std::string label = species.getSpeciesGraph().canonicalLabel();
    const std::string exact = species.getSpeciesGraph().toString();

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

    const auto existing = indicesByLabel_.find(label);
    if (existing != indicesByLabel_.end()) {
        for (const auto index : existing->second) {
            auto& existingSpecies = species_[index];
            if (existingSpecies.getCompartment() != species.getCompartment()) {
                continue;
            }
            if (!isIsomorphic(existingSpecies.getSpeciesGraph(), species.getSpeciesGraph())) {
                continue;
            }
            if (existingSpecies.getCompartment().empty() && !species.getCompartment().empty()) {
                existingSpecies.setCompartment(species.getCompartment());
            }
            return {index, false};
        }
    }

    for (std::size_t index = 0; index < species_.size(); ++index) {
        auto& existingSpecies = species_[index];
        if (existingSpecies.getCompartment() != species.getCompartment()) {
            continue;
        }
        if (!isIsomorphic(existingSpecies.getSpeciesGraph(), species.getSpeciesGraph())) {
            continue;
        }
        if (existingSpecies.getCompartment().empty() && !species.getCompartment().empty()) {
            existingSpecies.setCompartment(species.getCompartment());
        }
        indicesByLabel_[label].push_back(index);
        return {index, false};
    }

    const std::size_t index = species_.size();
    species.setIndex(index);
    species_.push_back(std::move(species));
    indicesByLabel_[label].push_back(index);
    indicesByExactString_[exact].push_back(index);
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
