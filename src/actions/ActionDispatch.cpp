#include "actions/ActionDispatch.hpp"

#include <filesystem>
#include <optional>
#include <string>

#include "engine/NetworkGenerator.hpp"
#include "io/NetWriter.hpp"

namespace bng::actions {

void ActionDispatch::execute(ast::Model& model, const std::filesystem::path& sourcePath) {
    engine::NetworkGenerator generator(model);
    std::optional<engine::GeneratedNetwork> network;

    for (const auto& action : model.getActions()) {
        if (action.name == "generate_network") {
            network = generator.generate(sourcePath);
            const auto outputPath = sourcePath.parent_path() / (sourcePath.stem().string() + ".net");
            io::NetWriter::write(outputPath, model, *network);
            continue;
        }

        if (action.name.rfind("write", 0) == 0) {
            if (!network.has_value()) {
                network = generator.generate(sourcePath);
            }
            const auto outputPath = sourcePath.parent_path() / (sourcePath.stem().string() + ".net");
            io::NetWriter::write(outputPath, model, *network);
        }
    }
}

} // namespace bng::actions
