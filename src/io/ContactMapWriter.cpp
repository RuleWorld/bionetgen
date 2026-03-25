#include "ContactMapWriter.hpp"

#include <sstream>
#include <algorithm>

namespace bng::io {

ContactMapWriter::ContactMap ContactMapWriter::buildContactMap(const ast::Model& model) {
    ContactMap map;

    // Add all molecule types
    for (const auto& mt : model.getMoleculeTypes()) {
        map.moleculeTypes.insert(mt.getName());
    }

    // Scan reaction rules for bonds between different molecules
    for (const auto& rule : model.getReactionRules()) {
        // Parse reactants and products to find bonds
        for (const auto& pattern : rule.getReactantPatterns()) {
            // Check for inter-molecular bonds in the pattern
            // This is simplified - full implementation would parse the graph structure
            // For now, detect patterns like "A(b!1).B(a!1)"

            std::string patternStr = pattern.toString();

            // Simple heuristic: if pattern contains !1, !2 etc. and multiple molecules
            // then there are bonds between molecules
            if (patternStr.find('!') != std::string::npos && patternStr.find('.') != std::string::npos) {
                // Extract molecule names (simplified)
                // This is a placeholder - full implementation needs proper parsing
            }
        }
    }

    return map;
}

std::string ContactMapWriter::toGML(const ContactMap& contactMap) {
    std::ostringstream gml;

    gml << "graph [\n";
    gml << "  directed 0\n";

    // Write nodes
    int nodeId = 0;
    std::map<std::string, int> nodeIds;
    for (const auto& molType : contactMap.moleculeTypes) {
        gml << "  node [\n";
        gml << "    id " << nodeId << "\n";
        gml << "    label \"" << molType << "\"\n";
        gml << "  ]\n";
        nodeIds[molType] = nodeId++;
    }

    // Write edges
    for (const auto& edge : contactMap.edges) {
        gml << "  edge [\n";
        gml << "    source " << nodeIds[edge.mol1] << "\n";
        gml << "    target " << nodeIds[edge.mol2] << "\n";
        gml << "    label \"" << edge.comp1 << "--" << edge.comp2 << "\"\n";
        gml << "  ]\n";
    }

    gml << "]\n";
    return gml.str();
}

std::string ContactMapWriter::toDOT(const ContactMap& contactMap) {
    std::ostringstream dot;

    dot << "graph ContactMap {\n";
    dot << "  node [shape=box];\n\n";

    // Write nodes
    for (const auto& molType : contactMap.moleculeTypes) {
        dot << "  \"" << molType << "\";\n";
    }

    dot << "\n";

    // Write edges
    for (const auto& edge : contactMap.edges) {
        dot << "  \"" << edge.mol1 << "\" -- \"" << edge.mol2
            << "\" [label=\"" << edge.comp1 << "--" << edge.comp2 << "\"];\n";
    }

    dot << "}\n";
    return dot.str();
}

} // namespace bng::io
