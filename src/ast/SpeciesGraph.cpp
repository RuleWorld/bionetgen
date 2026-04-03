#include "SpeciesGraph.hpp"

#include <utility>

namespace bng::ast {

SpeciesGraph::SpeciesGraph(BNGcore::PatternGraph graph, std::string compartment)
    : graph_(std::move(graph)), compartment_(std::move(compartment)) {}

const BNGcore::PatternGraph& SpeciesGraph::getGraph() const {
    return graph_;
}

BNGcore::PatternGraph& SpeciesGraph::getGraph() {
    return graph_;
}

std::string SpeciesGraph::canonicalLabel() const {
    return graph_.get_label();
}

std::string SpeciesGraph::fingerprint() const {
    return graph_.computeFingerprint();
}

std::string SpeciesGraph::toString() const {
    return graph_.get_BNG2_string();
}

std::string SpeciesGraph::toStringForDedup() const {
    // Perl convention: the dedup key includes the species compartment prefix
    // and per-molecule compartments only when they differ from species-level.
    // This matches Perl's SpeciesGraph::toString(1,0) which passes $sg->Compartment
    // to Molecule::toString, which conditionally appends @comp.
    std::vector<std::string> molComps;
    std::string base = graph_.get_BNG2_string(molComps);

    std::string result;
    if (!compartment_.empty()) {
        result += "@" + compartment_ + "::";
    }

    // Annotate molecules with compartments that differ from species-level
    if (!compartment_.empty() && !molComps.empty()) {
        std::size_t molIdx = 0, pos = 0;
        while (pos < base.size()) {
            int parenDepth = 0;
            std::size_t molEnd = pos;
            while (molEnd < base.size()) {
                if (base[molEnd] == '(') parenDepth++;
                else if (base[molEnd] == ')') parenDepth--;
                else if (base[molEnd] == '.' && parenDepth == 0) break;
                molEnd++;
            }
            result += base.substr(pos, molEnd - pos);
            if (molIdx < molComps.size() && !molComps[molIdx].empty() &&
                molComps[molIdx] != compartment_) {
                result += "@" + molComps[molIdx];
            }
            if (molEnd < base.size() && base[molEnd] == '.') {
                result += '.';
                molEnd++;
            }
            pos = molEnd;
            molIdx++;
        }
    } else {
        result += base;
    }

    return result;
}

const std::string& SpeciesGraph::getCompartment() const {
    return compartment_;
}

void SpeciesGraph::setCompartment(std::string compartment) {
    compartment_ = std::move(compartment);
}

} // namespace bng::ast
