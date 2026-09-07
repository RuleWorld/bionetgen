#pragma once

#include <cstddef>
#include <filesystem>
#include <vector>

namespace bng::cli {

struct ParallelBatchOptions {
    std::filesystem::path executable;
    std::vector<std::filesystem::path> inputs;
    std::size_t maxJobs = 1;
    std::filesystem::path outputRoot;
    bool checkOnly = false;
    bool verbose = false;
};

/**
 * Run independent model files in isolated child processes.
 *
 * Each child receives a private working directory and a private copy of its
 * input model. This keeps generated files and native mutable graph state
 * separate while allowing independent models to overlap.
 */
int runParallelBatch(const ParallelBatchOptions& options);

} // namespace bng::cli
