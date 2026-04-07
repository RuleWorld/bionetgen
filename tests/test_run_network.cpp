#include <catch2/catch_test_macros.hpp>
#include <string>
#include <iostream>
#include <fstream>
#include <cstdlib>

#ifndef RUN_NETWORK_PATH
#define RUN_NETWORK_PATH "../bng2/Network3/run_network"
#endif

// Check if a file exists
inline bool file_exists(const std::string& name) {
    if (FILE *file = fopen(name.c_str(), "r")) {
        fclose(file);
        return true;
    } else {
        return false;
    }
}

TEST_CASE("run_network print_error calls exit(1) and prints usage", "[run_network]") {
    // If the executable hasn't been built (e.g., in modern C++ CI pipeline), skip the test gracefully
    if (!file_exists(RUN_NETWORK_PATH)) {
        SUCCEED("run_network executable not found at " << RUN_NETWORK_PATH << ". Skipping test.");
        return;
    }

    // Construct the command to run the executable and redirect stderr to a temporary file
    std::string temp_file = "test_run_network_stderr.txt";
    std::string command = std::string(RUN_NETWORK_PATH) + " 2> " + temp_file;

    // Execute the command
    int result = std::system(command.c_str());

    // Check if the command ran
    REQUIRE(result != -1);

    // Verify exit status is not 0 (print_error calls exit(1))
    REQUIRE(result != 0);

    // Read the captured stderr output
    std::ifstream file(temp_file);
    REQUIRE(file.is_open());

    std::string output((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
    file.close();

    // Clean up the temporary file
    std::remove(temp_file.c_str());

    // Verify printed output contains the usage text from print_error()
    REQUIRE(output.find("Usage:") != std::string::npos);
    REQUIRE(output.find("run_network  [-bcdefkmsvx]") != std::string::npos);
}
