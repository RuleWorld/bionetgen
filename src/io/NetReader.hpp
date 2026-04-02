#pragma once

#include <string>
#include <filesystem>
#include "ast/Model.hpp"
#include "engine/NetworkGenerator.hpp"

namespace bng::io {

/**
 * NetReader - Parse BioNetGen .net format files
 *
 * Reads post-network-generation .net files produced by writeNetwork or BNG2.pl
 * Enables readFile action for .net files and continue workflows.
 *
 * .net format structure:
 * - begin parameters ... end parameters
 * - begin compartments ... end compartments
 * - begin molecule types ... end molecule types
 * - begin species ... end species
 * - begin reactions ... end reactions
 * - begin groups ... end groups (observables)
 *
 * Reference: BNG2/bng2/Perl2/BNGModel.pm::readNetwork()
 */
class NetReader {
public:
    struct ParseResult {
        bool success = false;
        std::string error;
        // Parsed network data (if successful)
        std::vector<std::pair<std::string, std::string>> species;  // pattern, concentration (as string - can be number or parameter ref)
        std::vector<std::string> reactions;
        std::unordered_map<std::string, double> parameters;
        std::vector<std::string> compartments;
        std::vector<std::pair<std::string, std::vector<std::string>>> observables; // name, patterns
        std::vector<std::pair<std::string, std::string>> functions;  // name, expression
        // Raw sections for passthrough writing
        std::vector<std::string> rawFunctionLines;
        std::vector<std::string> rawReactionRuleLines;
        std::vector<std::string> rawMoleculeTypeLines;
        std::vector<std::string> rawGroupLines;
    };

    /**
     * Parse a .net file into a Model and network data
     * @param filepath Path to .net file
     * @return Parse result with model and network or error
     */
    static ParseResult parse(const std::filesystem::path& filepath);

private:
    static void parseParameterLine(const std::string& line, ParseResult& result);
    static void parseCompartmentLine(const std::string& line, ParseResult& result);
    static void parseSpeciesLine(const std::string& line, std::vector<std::pair<std::string, std::string>>& species);
    static void parseReactionLine(const std::string& line, std::vector<std::string>& reactions);
    static void parseGroupLine(const std::string& line, ParseResult& result);
};

} // namespace bng::io
