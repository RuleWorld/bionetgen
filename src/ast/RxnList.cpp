#include "RxnList.hpp"

#include <algorithm>
#include <sstream>
#include <utility>

namespace bng::ast {

namespace {

std::string reactionSignature(const Rxn& reaction) {
    std::ostringstream out;
    // Sort reactants for deduplication (order in Rxn is preserved for output)
    auto sortedReactants = reaction.getReactants();
    std::sort(sortedReactants.begin(), sortedReactants.end());
    for (std::size_t i = 0; i < sortedReactants.size(); ++i) {
        if (i != 0) {
            out << ",";
        }
        out << sortedReactants[i];
    }
    out << "->";
    for (std::size_t i = 0; i < reaction.getProducts().size(); ++i) {
        if (i != 0) {
            out << ",";
        }
        out << reaction.getProducts()[i];
    }
    out << "@" << reaction.getRateLaw();
    return out.str();
}

} // namespace

bool RxnList::add(Rxn reaction) {
    const auto signature = reactionSignature(reaction);
    const auto existing = indexBySignature_.find(signature);
    if (existing != indexBySignature_.end()) {
        reactions_[existing->second].addFactor(reaction.getFactor());
        return false;
    }
    indexBySignature_[signature] = reactions_.size();
    reactions_.push_back(std::move(reaction));
    return true;
}

bool RxnList::containsLabel(const std::string& label) const {
    for (const auto& reaction : reactions_) {
        if (reaction.getLabel() == label) {
            return true;
        }
    }
    return false;
}

std::size_t RxnList::size() const {
    return reactions_.size();
}

const std::vector<Rxn>& RxnList::all() const {
    return reactions_;
}

} // namespace bng::ast
