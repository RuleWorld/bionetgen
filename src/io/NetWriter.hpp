#pragma once

#include <filesystem>
#include <string>
#include <unordered_map>

#include "ast/Model.hpp"
#include "ast/Expression.hpp"
#include "engine/NetworkGenerator.hpp"

namespace bng::io {

struct DerivedRateInfo {
    std::string paramName;
    std::string expression;
    ast::Expression exprTree;
    bool reverseDirection = false;
    bool asFunction = false;  // true if should be written as function (references observables)
};

class NetWriter {
public:
    static void write(const std::filesystem::path& outputPath, ast::Model& model, const engine::GeneratedNetwork& network);
    static std::unordered_map<std::string, DerivedRateInfo> buildDerivedRateParams(
        const ast::Model& model,
        const engine::GeneratedNetwork& network);
    static std::optional<double> computeUnitConversionFactor(
        const ast::Rxn& reaction,
        const ast::Model& model,
        const engine::GeneratedNetwork& network);
};

} // namespace bng::io
