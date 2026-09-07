#include <catch2/catch_test_macros.hpp>

#include <chrono>
#include <cstdlib>
#include <filesystem>
#include <fstream>
#include <string>

namespace {

namespace fs = std::filesystem;

fs::path testRoot() {
    const auto stamp = std::chrono::high_resolution_clock::now().time_since_epoch().count();
    return fs::temp_directory_path() / ("bng-cpp-batch-test-" + std::to_string(stamp));
}

std::string shellQuote(const fs::path& path) {
#if defined(_WIN32) || defined(__WIN32__) || defined(__CYGWIN__)
    return "\"" + path.string() + "\"";
#else
    std::string quoted = "'";
    for (const char character : path.string()) {
        if (character == '\'') {
            quoted += "'\\''";
        } else {
            quoted += character;
        }
    }
    quoted += "'";
    return quoted;
#endif
}

int runCommand(const std::string& command) {
    return std::system(command.c_str());
}

} // namespace

TEST_CASE("bng_cpp parallel mode isolates model outputs", "[batch]") {
    const auto root = testRoot();
    const auto outputRoot = root / "outputs";
    fs::create_directories(root);

    const auto modelOne = fs::path(BNG_SOURCE_DIR) / "bng2" / "Models2" / "LV.bngl";
    const auto modelTwo = fs::path(BNG_SOURCE_DIR) / "bng2" / "Validate" / "blbr.bngl";
    const auto command = shellQuote(fs::path(BNG_CPP_PATH))
        + " --parallel 2 --parallel-dir " + shellQuote(outputRoot)
        + " " + shellQuote(modelOne) + " " + shellQuote(modelTwo);

    REQUIRE(runCommand(command) == 0);
    REQUIRE(fs::exists(outputRoot / "job_000-LV" / "LV.net"));
    REQUIRE(fs::exists(outputRoot / "job_001-blbr" / "blbr.net"));
    REQUIRE(fs::exists(outputRoot / "job_000-LV" / "LV.bngl"));
    REQUIRE(fs::exists(outputRoot / "job_001-blbr" / "blbr.bngl"));

    fs::remove_all(root);
}

TEST_CASE("bng_cpp parallel mode propagates child failures", "[batch]") {
    const auto root = testRoot();
    const auto outputRoot = root / "outputs";
    const auto invalidModel = root / "invalid.bngl";
    fs::create_directories(root);
    {
        std::ofstream output(invalidModel);
        output << "this is not a BioNetGen model\n";
    }

    const auto validModel = fs::path(BNG_SOURCE_DIR) / "bng2" / "Models2" / "LV.bngl";
    const auto command = shellQuote(fs::path(BNG_CPP_PATH))
        + " --parallel 2 --parallel-dir " + shellQuote(outputRoot)
        + " " + shellQuote(validModel) + " " + shellQuote(invalidModel);

    REQUIRE(runCommand(command) != 0);
    REQUIRE(fs::exists(outputRoot / "job_000-LV" / "LV.net"));

    fs::remove_all(root);
}
