#include <catch2/catch_test_macros.hpp>

#include <cmath>
#include <chrono>
#include <filesystem>
#include <fstream>
#include <sstream>
#include <string>

#include "actions/ActionDispatch.hpp"
#include "engine/NetworkGenerator.hpp"
#include "parser/BNGAstVisitor.hpp"

namespace {

class ScopedTempDir {
public:
    ScopedTempDir()
        : path_(std::filesystem::temp_directory_path() / ("bng-cpp-" + std::to_string(
              std::chrono::steady_clock::now().time_since_epoch().count()))) {
        std::filesystem::create_directories(path_);
    }

    ~ScopedTempDir() {
        std::error_code ignored;
        std::filesystem::remove_all(path_, ignored);
    }

    const std::filesystem::path& path() const {
        return path_;
    }

private:
    std::filesystem::path path_;
};

std::filesystem::path writeTestModel(const std::filesystem::path& directory) {
    const std::filesystem::path modelPath = directory / "simple.bngl";
    std::ofstream out(modelPath);
    REQUIRE(out.is_open());
    out << R"(begin model
begin parameters
  kf 1
  kr 2
end parameters
begin molecule types
  A(b)
  B(a)
end molecule types
begin seed species
  A(b) 10
  B(a) 20
end seed species
begin observables
  Molecules Atot A()
end observables
begin reaction rules
  A(b) + B(a) <-> A(b!1).B(a!1) kf,kr
end reaction rules
end model

generate_network({overwrite=>1,max_iter=>10})
)";
    return modelPath;
}

std::string readTextFile(const std::filesystem::path& path) {
    std::ifstream input(path);
    REQUIRE(input.is_open());
    std::ostringstream buffer;
    buffer << input.rdbuf();
    return buffer.str();
}

} // namespace

TEST_CASE("Compatibility network generator reads generated .net output", "[network]") {
    ScopedTempDir tempDir;
    const auto modelPath = writeTestModel(tempDir.path());
    auto model = bng::parser::parseModel(readTextFile(modelPath));

    bng::engine::NetworkGenerator generator(*model);
    const auto network = generator.generate(modelPath);

    REQUIRE(std::filesystem::exists(tempDir.path() / "simple.net"));
    REQUIRE(network.species.size() == 3);
    REQUIRE(network.reactions.size() == 2);
    REQUIRE(network.species.get(0).getSpeciesGraph().toString() == "A(b)");
    REQUIRE(network.species.get(2).getSpeciesGraph().toString() == "A(b!0).B(a!0)");
}

TEST_CASE("Action dispatch executes model actions through compatibility backend", "[actions]") {
    ScopedTempDir tempDir;
    const auto modelPath = writeTestModel(tempDir.path());
    auto model = bng::parser::parseModel(readTextFile(modelPath));

    REQUIRE_NOTHROW(bng::actions::ActionDispatch::execute(*model, modelPath));
    REQUIRE(std::filesystem::exists(tempDir.path() / "simple.net"));
}

TEST_CASE("Symmetric binding keeps multiplicity for duplicate site matches", "[network]") {
    auto model = bng::parser::parseModel(R"(begin model
begin parameters
  kf 1
end parameters
begin molecule types
  R(r,r)
  L(l,l)
end molecule types
begin seed species
  R(r,r) 1
  L(l,l) 1
end seed species
begin reaction rules
  R(r) + L(l,l) -> R(r!1).L(l!1,l) kf
end reaction rules
end model

generate_network({overwrite=>1,max_iter=>5})
)");

    bng::engine::NetworkGenerator generator(*model);
    const auto network = generator.generateNative(5);

    REQUIRE(network.reactions.size() >= 2);
    REQUIRE(network.reactions.all()[0].getRateLaw() == "kf");
    REQUIRE(std::abs(network.reactions.all()[1].getFactor() - 2.0) < 1e-9);
}
