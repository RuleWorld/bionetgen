#include "NetReader.hpp"

#include <fstream>
#include <sstream>
#include <algorithm>

namespace bng::io {

namespace {

std::string trim(const std::string& str) {
    auto start = std::find_if_not(str.begin(), str.end(), [](unsigned char c) {
        return std::isspace(c);
    });
    auto end = std::find_if_not(str.rbegin(), str.rend(), [](unsigned char c) {
        return std::isspace(c);
    }).base();
    return (start < end) ? std::string(start, end) : std::string();
}

std::vector<std::string> split(const std::string& str, char delim) {
    std::vector<std::string> tokens;
    std::stringstream ss(str);
    std::string token;
    while (std::getline(ss, token, delim)) {
        tokens.push_back(token);
    }
    return tokens;
}

} // anonymous namespace

NetReader::ParseResult NetReader::parse(const std::filesystem::path& filepath) {
    ParseResult result;

    std::ifstream infile(filepath);
    if (!infile) {
        result.error = "Failed to open file: " + filepath.string();
        return result;
    }

    std::string line;
    std::string currentSection;
    int lineNum = 0;

    try {
        while (std::getline(infile, line)) {
            lineNum++;

            // Remove comments (everything after #)
            auto commentPos = line.find('#');
            if (commentPos != std::string::npos) {
                line = line.substr(0, commentPos);
            }

            line = trim(line);
            if (line.empty()) continue;

            // Check for section headers
            if (line.find("begin") == 0) {
                auto tokens = split(line, ' ');
                if (tokens.size() >= 2) {
                    currentSection = tokens[1];
                }
                continue;
            }

            if (line.find("end") == 0) {
                currentSection.clear();
                continue;
            }

            // Parse section content
            if (currentSection == "parameters") {
                parseParameterLine(line, result);
            } else if (currentSection == "compartments") {
                parseCompartmentLine(line, result);
            } else if (currentSection == "species") {
                parseSpeciesLine(line, result.species);
            } else if (currentSection == "reactions") {
                parseReactionLine(line, result.reactions);
            } else if (currentSection == "groups") {
                parseGroupLine(line, result);
            }
        }

        result.success = true;

    } catch (const std::exception& e) {
        result.success = false;
        result.error = "Parse error at line " + std::to_string(lineNum) + ": " + e.what();
    }

    return result;
}

void NetReader::parseParameterLine(const std::string& line, ParseResult& result) {
    // Format: <index> <name> <value>
    // Example: "1 NA 6.02e+23"
    auto tokens = split(line, ' ');
    std::vector<std::string> filtered;
    for (const auto& t : tokens) {
        if (!trim(t).empty()) filtered.push_back(trim(t));
    }

    if (filtered.size() < 3) {
        throw std::runtime_error("Invalid parameter format: " + line);
    }

    std::string name = filtered[1];
    double value = std::stod(filtered[2]);

    result.parameters[name] = value;
}

void NetReader::parseCompartmentLine(const std::string& line, ParseResult& result) {
    // Format: <index> <name> <dimension> <size> [<parent>]
    // Example: "1 EC 3 1.0e-10"
    // Just store the line for now
    result.compartments.push_back(line);
}

void NetReader::parseSpeciesLine(const std::string& line, std::vector<std::pair<std::string, std::string>>& species) {
    // Format: <index> <pattern> <concentration>
    // Example: "1 EGFR(L,CR1,Y1068~U) 1.8e5" or "1 S(a~0,c~R) S0"
    // Note: concentration can be a numeric literal or a parameter reference

    // Find first space (after index)
    auto firstSpace = line.find(' ');
    if (firstSpace == std::string::npos) {
        throw std::runtime_error("Invalid species format: " + line);
    }

    // Find last space (before concentration)
    auto lastSpace = line.rfind(' ');
    if (lastSpace == firstSpace) {
        throw std::runtime_error("Invalid species format: " + line);
    }

    std::string pattern = trim(line.substr(firstSpace + 1, lastSpace - firstSpace - 1));
    std::string concentration = trim(line.substr(lastSpace + 1));

    species.emplace_back(pattern, concentration);
}

void NetReader::parseReactionLine(const std::string& line, std::vector<std::string>& reactions) {
    // Format: <index> <reactants> <products> <rate> [<label>]
    // Example: "1 S1,S2 S3 k1*S1*S2"

    // Just store the raw line for now - full parsing needs more complex handling
    reactions.push_back(line);
}

void NetReader::parseGroupLine(const std::string& line, ParseResult& result) {
    // Two formats are supported:
    // Format A (from BNGL): <index> <name> <type> <patterns...>
    //   Example: "1 Dimers Molecules EGFR(CR1!+)"
    // Format B (from .net):  <index> <name> <weights>
    //   Example: "1 Sa0  1,4"  (weight-based species references)

    auto tokens = split(line, ' ');
    std::vector<std::string> filtered;
    for (const auto& t : tokens) {
        if (!trim(t).empty()) filtered.push_back(trim(t));
    }

    if (filtered.size() < 3) {
        throw std::runtime_error("Invalid observable format: " + line);
    }

    std::string name = filtered[1];

    if (filtered.size() >= 4) {
        // Format A: <index> <name> <type> <patterns...>
        std::vector<std::string> patterns;
        for (std::size_t i = 3; i < filtered.size(); ++i) {
            patterns.push_back(filtered[i]);
        }
        result.observables.emplace_back(name, patterns);
    } else {
        // Format B: <index> <name> <weights>
        // Third token is comma-separated species indices (possibly with weight multipliers)
        // e.g., "1,4" or "2*1,3*2"
        std::vector<std::string> weights;
        weights.push_back(filtered[2]);
        result.observables.emplace_back(name, weights);
    }
}

} // namespace bng::io
