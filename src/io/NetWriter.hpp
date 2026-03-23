#pragma once

#include <filesystem>

#include "ast/Model.hpp"
#include "engine/NetworkGenerator.hpp"

namespace bng::io {

class NetWriter {
public:
    static void write(const std::filesystem::path& outputPath, const ast::Model& model, const engine::GeneratedNetwork& network);
};

} // namespace bng::io
