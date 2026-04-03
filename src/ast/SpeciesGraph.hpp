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
    std::string toStringForDedup() const;
    const std::string& getCompartment() const;
    void setCompartment(std::string compartment);
    bool isCompartmentPrefix() const { return compartmentIsPrefix_; }
    void setCompartmentIsPrefix(bool v) { compartmentIsPrefix_ = v; }

private:
    BNGcore::PatternGraph graph_;
    std::string compartment_;
    bool compartmentIsPrefix_ = false;
};

} // namespace bng::ast
