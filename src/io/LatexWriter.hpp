#pragma once

#include <string>
#include <vector>
#include "ast/Model.hpp"
#include "engine/NetworkGenerator.hpp"

namespace bng::io {

/**
 * LatexWriter - Export model to a standalone compilable LaTeX document
 *
 * Generates a .tex file containing:
 * - Document preamble with amsmath, booktabs, mhchem packages
 * - Parameters table
 * - Species table with initial concentrations
 * - Reactions in equation environments with rate laws
 * - Observable definitions
 *
 * Reference: BNG2/bng2/Perl2/BNGOutput.pm::writeLatex()
 */
class LatexWriter {
public:
    static std::string write(
        const ast::Model& model,
        const engine::GeneratedNetwork& network
    );

private:
    static std::string escapeLatex(const std::string& text);
};

} // namespace bng::io
