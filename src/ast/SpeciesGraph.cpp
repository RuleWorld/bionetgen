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
    return graph_.get_BNG2_string_with_compartments();
}

const std::string& SpeciesGraph::getCompartment() const {
    return compartment_;
}

void SpeciesGraph::setCompartment(std::string compartment) {
    compartment_ = std::move(compartment);
}

} // namespace bng::ast
