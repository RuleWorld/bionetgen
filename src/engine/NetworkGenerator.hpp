#pragma once

#include <filesystem>
#include <memory>
#include <string>

#include "ast/Model.hpp"
#include "ast/RxnList.hpp"
#include "ast/SpeciesList.hpp"

namespace bng::engine {

struct GeneratedNetwork {
    ast::SpeciesList species;
    ast::RxnList reactions;
};

class NetworkGenerator {
public:
    explicit NetworkGenerator(ast::Model& model);

    GeneratedNetwork generate(const std::filesystem::path& sourcePath);
    GeneratedNetwork generateNative(std::size_t maxIter = 32);

private:
    ast::Model& model_;
};

} // namespace bng::engine
