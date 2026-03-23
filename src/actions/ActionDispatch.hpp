#pragma once

#include <filesystem>

#include "ast/Model.hpp"

namespace bng::actions {

class ActionDispatch {
public:
    static void execute(ast::Model& model, const std::filesystem::path& sourcePath);
};

} // namespace bng::actions
