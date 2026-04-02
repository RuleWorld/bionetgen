#pragma once

#include <string>

#include "core/BNGcore.hpp"

namespace bng::ast {

class SpeciesGraph {
public:
    SpeciesGraph() = default;
    explicit SpeciesGraph(BNGcore::PatternGraph graph, std::string compartment = {});

    const BNGcore::PatternGraph& getGraph() const;
    BNGcore::PatternGraph& getGraph();
    std::string canonicalLabel() const;
    std::string fingerprint() const;
    std::string toString() const;
    const std::string& getCompartment() const;
    void setCompartment(std::string compartment);

private:
    BNGcore::PatternGraph graph_;
    std::string compartment_;
};

} // namespace bng::ast
