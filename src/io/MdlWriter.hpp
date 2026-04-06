#pragma once

#include <string>
#include <vector>
#include "ast/Model.hpp"
#include "engine/NetworkGenerator.hpp"

namespace bng::io {

/**
 * MdlWriter - Export model to MCell MDL (Model Description Language)
 *
 * Generates a basic MDL file with:
 * - DEFINE_MOLECULES block with diffusion constants
 * - DEFINE_REACTIONS block with rate constants
 * - INSTANTIATE block with initial molecule counts
 *
 * Users will need to customize geometry and spatial parameters.
 *
 * Reference: BNG2/bng2/Perl2/BNGOutput.pm::writeMDL()
 */
class MdlWriter {
public:
    static std::string write(
        const ast::Model& model,
        const engine::GeneratedNetwork& network
    );
};

} // namespace bng::io
