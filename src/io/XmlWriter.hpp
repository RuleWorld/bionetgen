#pragma once

#include <string>
#include <vector>
#include "ast/Model.hpp"
#include "engine/NetworkGenerator.hpp"

namespace bng::io {

/**
 * XmlWriter - Export models to BNG-XML format for NFsim
 *
 * Generates full BNG-XML with molecule types, species patterns,
 * reaction rules (with reactant/product pattern serialization,
 * bond lists, and component maps), observables with patterns,
 * and function definitions.
 *
 * Reference: BNG2/bng2/Perl2/BNGOutput.pm::writeXML()
 *            BNG2/bng2/Perl2/RxnRule.pm::toXML()
 *            BNG2/bng2/Perl2/SpeciesGraph.pm::toXML()
 */
class XmlWriter {
public:
    static std::string write(
        const ast::Model& model,
        const engine::GeneratedNetwork* network = nullptr
    );

private:
    static std::string escapeXml(const std::string& text);
    static std::string writeParameters(const ast::Model& model);
    static std::string writeMoleculeTypes(const ast::Model& model);
    static std::string writeCompartments(const ast::Model& model);
    static std::string writeSpecies(const ast::Model& model, const engine::GeneratedNetwork* network);
    static std::string writeReactionRules(const ast::Model& model);
    static std::string writeObservables(const ast::Model& model);
    static std::string writeFunctions(const ast::Model& model);

    // Pattern serialization helpers
    static std::string writePatternXml(
        const std::string& patternText,
        const std::string& idPrefix,
        const ast::Model& model,
        const std::string& indent);
    static std::string writeSpeciesPatternXml(
        const std::string& patternText,
        const std::string& idPrefix,
        const ast::Model& model,
        const std::string& indent);
};

} // namespace bng::io
