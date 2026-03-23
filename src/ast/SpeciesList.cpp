#include "SpeciesList.hpp"

#include <stdexcept>
#include <utility>

namespace bng::ast {

std::pair<std::size_t, bool> SpeciesList::add(Species species) {
    const std::string label = species.getSpeciesGraph().canonicalLabel();
    const auto existing = indexByLabel_.find(label);
    if (existing != indexByLabel_.end()) {
        auto& existingSpecies = species_[existing->second];
        if (existingSpecies.getCompartment().empty() && !species.getCompartment().empty()) {
            existingSpecies.setCompartment(species.getCompartment());
        }
        return {existing->second, false};
    }

    const std::size_t index = species_.size();
    species.setIndex(index);
    species_.push_back(std::move(species));
    indexByLabel_[label] = index;
    return {index, true};
}

const Species& SpeciesList::get(std::size_t index) const {
    return species_.at(index);
}

Species& SpeciesList::get(std::size_t index) {
    return species_.at(index);
}

bool SpeciesList::containsLabel(const std::string& canonicalLabel) const {
    return indexByLabel_.find(canonicalLabel) != indexByLabel_.end();
}

std::size_t SpeciesList::indexOfLabel(const std::string& canonicalLabel) const {
    const auto found = indexByLabel_.find(canonicalLabel);
    if (found == indexByLabel_.end()) {
        throw std::runtime_error("Unknown species label: " + canonicalLabel);
    }
    return found->second;
}

std::size_t SpeciesList::size() const {
    return species_.size();
}

const std::vector<Species>& SpeciesList::all() const {
    return species_;
}

} // namespace bng::ast
