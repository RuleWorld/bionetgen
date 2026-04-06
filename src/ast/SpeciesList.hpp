#pragma once

#include <string>
#include <unordered_map>
#include <vector>

#include "Species.hpp"

namespace bng::ast {

class SpeciesList {
public:
    std::pair<std::size_t, bool> add(Species species);
    const Species& get(std::size_t index) const;
    Species& get(std::size_t index);
    bool containsLabel(const std::string& canonicalLabel) const;
    std::size_t indexOfLabel(const std::string& canonicalLabel) const;
    std::size_t size() const;
    std::size_t capacity() const;
    const std::vector<Species>& all() const;

    /// When false, skip isomorphism/dedup checks during add().
    /// Species are added unconditionally (useful for debugging/speed).
    void setCheckIso(bool enabled);
    bool getCheckIso() const;

private:
    std::vector<Species> species_;
    std::unordered_map<std::string, std::vector<std::size_t>> indicesByLabel_;
    std::unordered_map<std::string, std::vector<std::size_t>> indicesByExactString_;
    std::unordered_map<std::string, std::vector<std::size_t>> indicesByFingerprint_;
    bool checkIso_ = true;
};

} // namespace bng::ast
