#pragma once

#include <string>

#include "core/BNGcore.hpp"

namespace bng::ast {

class SpeciesGraph {
public:
    SpeciesGraph() = default;
    explicit SpeciesGraph(BNGcore::PatternGraph graph);

    const BNGcore::PatternGraph& getGraph() const;
    BNGcore::PatternGraph& getGraph();
    std::string canonicalLabel() const;
    std::string toString() const;

private:
    BNGcore::PatternGraph graph_;
};

} // namespace bng::ast
