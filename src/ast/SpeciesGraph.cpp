#include "SpeciesGraph.hpp"

#include <utility>

namespace bng::ast {

SpeciesGraph::SpeciesGraph(BNGcore::PatternGraph graph)
    : graph_(std::move(graph)) {}

const BNGcore::PatternGraph& SpeciesGraph::getGraph() const {
    return graph_;
}

BNGcore::PatternGraph& SpeciesGraph::getGraph() {
    return graph_;
}

std::string SpeciesGraph::canonicalLabel() const {
    return graph_.get_label();
}

std::string SpeciesGraph::toString() const {
    return graph_.get_BNG2_string();
}

} // namespace bng::ast
