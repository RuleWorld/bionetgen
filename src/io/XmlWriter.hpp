#pragma once

#include <string>
#include "ast/Model.hpp"
#include "engine/NetworkGenerator.hpp"

namespace bng::io {

/**
 * XmlWriter - Export models to BNG-XML format for NFsim
 *
 * Reference: BNG2/bng2/Perl2/BNGOutput.pm::writeXML()
 */
class XmlWriter {
public:
    /**
     * Write BNG-XML format for NFsim simulation
     * @param model The parsed BNGL model
     * @param network Optional generated network (if nullptr, writes rule-based XML)
     * @return XML string in BNG-XML format
     */
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
};

} // namespace bng::io
