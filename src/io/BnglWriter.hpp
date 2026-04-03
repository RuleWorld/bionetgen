#pragma once

#include <string>
#include "ast/Model.hpp"
#include "engine/NetworkGenerator.hpp"

namespace bng::io {

/**
 * BnglWriter - Serialize Model back to BNGL format
 *
 * Reference: BNG2/bng2/Perl2/BNGOutput.pm::writeModel(), writeBNGL()
 */
class BnglWriter {
public:
    struct Options {
        bool includeComments = true;
        bool includeActions = false;
        bool evaluateExpressions = false;
    };

    /**
     * Serialize model to BNGL format
     * @param model The AST model to serialize
     * @param network Optional generated network
     * @param options Writer options
     * @return BNGL-formatted string
     */
    static std::string write(
        const ast::Model& model,
        const engine::GeneratedNetwork* network = nullptr
    );

    static std::string write(
        const ast::Model& model,
        const engine::GeneratedNetwork* network,
        const Options& options
    );

private:
    static std::string formatNumber(double value);
    static std::string writeParameters(const ast::Model& model, const Options& opts);
    static std::string writeCompartments(const ast::Model& model);
    static std::string writeMoleculeTypes(const ast::Model& model);
    static std::string writeSeedSpecies(const ast::Model& model);
    static std::string writeObservables(const ast::Model& model);
    static std::string writeFunctions(const ast::Model& model);
    static std::string writeReactionRules(const ast::Model& model);
    static std::string writeActions(const ast::Model& model);
};

} // namespace bng::io
