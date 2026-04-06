#pragma once

#include <string>
#include <vector>
#include "ast/Model.hpp"
#include "engine/NetworkGenerator.hpp"

namespace bng::io {

/**
 * SscWriter - Export model to SSC (Stochastic Simulation Compiler) format
 *
 * Generates a simple text file with:
 * - Parameter definitions as const declarations
 * - Species declarations with initial counts
 * - Reaction definitions with rate law references
 *
 * Reference: BNG2/bng2/Perl2/BNGOutput.pm::writeSSCcfg()
 */
class SscWriter {
public:
    static std::string write(
        const ast::Model& model,
        const engine::GeneratedNetwork& network
    );
};

} // namespace bng::io
