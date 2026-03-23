#pragma once

#include <fstream>
#include <sstream>
#include <stdexcept>
#include <string>

#include "parser/BNGAstVisitor.hpp"

namespace test {

inline std::string sourcePath(const std::string& relativePath) {
    return std::string(BNG_CPP_SOURCE_DIR) + "/" + relativePath;
}

inline std::string readTextFile(const std::string& path) {
    std::ifstream input(path);
    if (!input) {
        throw std::runtime_error("Could not open file: " + path);
    }

    std::ostringstream buffer;
    buffer << input.rdbuf();
    return buffer.str();
}

inline std::unique_ptr<bng::ast::Model> parseModelFile(const std::string& path) {
    return bng::parser::parseModel(readTextFile(path));
}

} // namespace test
