#include "BatchRunner.hpp"

#include <algorithm>
#include <atomic>
#include <chrono>
#include <cctype>
#include <cstring>
#include <exception>
#include <iomanip>
#include <iostream>
#include <mutex>
#include <sstream>
#include <stdexcept>
#include <string>
#include <thread>
#include <vector>

#if defined(_WIN32) || defined(__WIN32__) || defined(__CYGWIN__)
#include <process.h>
#else
#include <cerrno>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#endif

namespace bng::cli {

namespace {

namespace fs = std::filesystem;

struct Job {
    fs::path sourcePath;
    fs::path workingDirectory;
    fs::path localModelPath;
};

struct ChildResult {
    int exitCode = 1;
    std::string error;
};

std::string safeStem(const fs::path& path) {
    std::string stem = path.stem().string();
    for (char& character : stem) {
        const auto value = static_cast<unsigned char>(character);
        if (!std::isalnum(value) && character != '-' && character != '_') {
            character = '_';
        }
    }
    return stem.empty() ? "model" : stem;
}

fs::path makeOutputRoot(const fs::path& requested) {
    if (!requested.empty()) {
        const auto root = fs::absolute(requested).lexically_normal();
        std::error_code error;
        if (fs::exists(root, error) && !fs::is_directory(root, error)) {
            throw std::runtime_error("parallel output root is not a directory: " + root.string());
        }
        fs::create_directories(root, error);
        if (error) {
            throw std::runtime_error("could not create parallel output root " + root.string() + ": " + error.message());
        }
        if (fs::directory_iterator(root, error) != fs::directory_iterator()) {
            throw std::runtime_error("parallel output root must be empty: " + root.string());
        }
        if (error) {
            throw std::runtime_error("could not inspect parallel output root " + root.string() + ": " + error.message());
        }
        return root;
    }

    const auto base = fs::temp_directory_path();
    const auto timestamp = std::chrono::high_resolution_clock::now().time_since_epoch().count();
#if defined(_WIN32) || defined(__WIN32__) || defined(__CYGWIN__)
    const auto processId = static_cast<unsigned long long>(_getpid());
#else
    const auto processId = static_cast<unsigned long long>(getpid());
#endif
    for (unsigned int attempt = 0; attempt < 100; ++attempt) {
        const auto name = "bng-cpp-parallel-" + std::to_string(processId) + "-"
            + std::to_string(timestamp) + "-" + std::to_string(attempt);
        const auto root = base / name;
        std::error_code error;
        if (fs::create_directory(root, error)) {
            return root;
        }
        if (error && error != std::errc::file_exists) {
            throw std::runtime_error("could not create parallel output root " + root.string() + ": " + error.message());
        }
    }
    throw std::runtime_error("could not create a unique parallel output root");
}

fs::path resolveExecutable(const fs::path& executable) {
    if (executable.empty()) {
        throw std::runtime_error("parallel executable path is empty");
    }
    if (executable.has_parent_path() || fs::exists(executable)) {
        return fs::absolute(executable).lexically_normal();
    }
    // A bare executable name is intentionally left for PATH lookup by execvp
    // or _spawnvp after the child changes to its job directory.
    return executable;
}

std::vector<Job> stageJobs(const ParallelBatchOptions& options, const fs::path& outputRoot) {
    std::vector<Job> jobs;
    jobs.reserve(options.inputs.size());
    for (std::size_t index = 0; index < options.inputs.size(); ++index) {
        const auto sourcePath = fs::absolute(options.inputs[index]).lexically_normal();
        std::error_code error;
        if (!fs::is_regular_file(sourcePath, error)) {
            throw std::runtime_error("parallel input is not a regular file: " + sourcePath.string());
        }

        std::ostringstream directoryName;
        directoryName << "job_" << std::setw(3) << std::setfill('0') << index
                      << "-" << safeStem(sourcePath);
        const auto workingDirectory = outputRoot / directoryName.str();
        fs::create_directory(workingDirectory, error);
        if (error) {
            throw std::runtime_error("could not create parallel job directory "
                + workingDirectory.string() + ": " + error.message());
        }

        const auto localModelPath = workingDirectory / sourcePath.filename();
        fs::copy_file(sourcePath, localModelPath, fs::copy_options::none, error);
        if (error) {
            throw std::runtime_error("could not stage " + sourcePath.string() + ": " + error.message());
        }
        jobs.push_back({sourcePath, workingDirectory, localModelPath});
    }
    return jobs;
}

std::vector<std::string> childArguments(
    const fs::path& executable,
    const fs::path& localModelPath,
    bool checkOnly,
    bool verbose) {

    std::vector<std::string> arguments;
    arguments.reserve(4);
    arguments.push_back(executable.string());
    if (checkOnly) {
        arguments.emplace_back("--check");
    }
    if (verbose) {
        arguments.emplace_back("--verbose");
    }
    arguments.push_back(localModelPath.string());
    return arguments;
}

int runChild(
    const fs::path& executable,
    const Job& job,
    bool checkOnly,
    bool verbose,
    std::string& errorMessage) {

    auto arguments = childArguments(executable, job.localModelPath, checkOnly, verbose);
    std::vector<char*> nativeArguments;
    nativeArguments.reserve(arguments.size() + 1);
    for (auto& argument : arguments) {
        nativeArguments.push_back(argument.data());
    }
    nativeArguments.push_back(nullptr);

#if defined(_WIN32) || defined(__WIN32__) || defined(__CYGWIN__)
    // Windows has no fork/chdir-for-the-child primitive. The short critical
    // section only covers changing the parent's cwd and spawning; waiting for
    // the child happens after restoring it, so jobs still run concurrently.
    static std::mutex spawnMutex;
    intptr_t process = -1;
    {
        std::lock_guard<std::mutex> lock(spawnMutex);
        std::error_code error;
        const auto previousDirectory = fs::current_path(error);
        if (error) {
            errorMessage = "could not read current directory: " + error.message();
            return 1;
        }
        fs::current_path(job.workingDirectory, error);
        if (error) {
            errorMessage = "could not enter " + job.workingDirectory.string() + ": " + error.message();
            return 1;
        }

        const auto executableText = executable.string();
        if (executable.has_parent_path()) {
            process = _spawnv(_P_NOWAIT, executableText.c_str(), nativeArguments.data());
        } else {
            process = _spawnvp(_P_NOWAIT, executableText.c_str(), nativeArguments.data());
        }
        const auto restoreError = [&]() {
            std::error_code restoreStatus;
            fs::current_path(previousDirectory, restoreStatus);
            return restoreStatus;
        }();
        if (restoreError) {
            errorMessage = "could not restore current directory: " + restoreError.message();
            return 1;
        }
        if (process == -1) {
            errorMessage = "could not start child process";
            return 1;
        }
    }

    int status = 1;
    if (_cwait(&status, process, 0) == -1) {
        errorMessage = "could not wait for child process";
        return 1;
    }
    return status;
#else
    const pid_t process = fork();
    if (process == -1) {
        errorMessage = std::strerror(errno);
        return 1;
    }
    if (process == 0) {
        if (chdir(job.workingDirectory.c_str()) != 0) {
            _exit(126);
        }
        if (executable.has_parent_path()) {
            execv(executable.c_str(), nativeArguments.data());
        } else {
            execvp(executable.c_str(), nativeArguments.data());
        }
        _exit(127);
    }

    int status = 0;
    while (waitpid(process, &status, 0) == -1) {
        if (errno != EINTR) {
            errorMessage = std::strerror(errno);
            return 1;
        }
    }
    if (WIFEXITED(status)) {
        return WEXITSTATUS(status);
    }
    if (WIFSIGNALED(status)) {
        errorMessage = "child terminated by signal " + std::to_string(WTERMSIG(status));
        return 128 + WTERMSIG(status);
    }
    errorMessage = "child ended in an unknown state";
    return 1;
#endif
}

} // namespace

int runParallelBatch(const ParallelBatchOptions& options) {
    if (options.inputs.empty()) {
        std::cerr << "error: parallel mode requires at least one model\n";
        return 1;
    }
    if (options.maxJobs == 0) {
        std::cerr << "error: parallel job count must be greater than zero\n";
        return 1;
    }

    try {
        const auto executable = resolveExecutable(options.executable);
        const auto outputRoot = makeOutputRoot(options.outputRoot);
        const auto jobs = stageJobs(options, outputRoot);
        const auto workerCount = std::min(options.maxJobs, jobs.size());
        std::vector<ChildResult> results(jobs.size());
        std::atomic<std::size_t> nextJob{0};
        std::vector<std::thread> workers;
        workers.reserve(workerCount);

        for (std::size_t worker = 0; worker < workerCount; ++worker) {
            workers.emplace_back([&]() {
                while (true) {
                    const auto index = nextJob.fetch_add(1);
                    if (index >= jobs.size()) {
                        return;
                    }
                    try {
                        results[index].exitCode = runChild(
                            executable, jobs[index], options.checkOnly, options.verbose, results[index].error);
                    } catch (const std::exception& exception) {
                        results[index].error = exception.what();
                        results[index].exitCode = 1;
                    }
                }
            });
        }
        for (auto& worker : workers) {
            worker.join();
        }

        std::cout << "[bng_cpp] parallel batch outputs: " << outputRoot.string() << '\n';
        std::size_t failures = 0;
        for (std::size_t index = 0; index < results.size(); ++index) {
            const auto& result = results[index];
            std::cout << "[bng_cpp] parallel batch job " << index
                      << " output: " << jobs[index].workingDirectory.string()
                      << " exit=" << result.exitCode << '\n';
            if (result.exitCode != 0) {
                ++failures;
                std::cerr << "[bng_cpp] parallel batch job " << index << " failed";
                if (!result.error.empty()) {
                    std::cerr << ": " << result.error;
                }
                std::cerr << '\n';
            }
        }
        if (failures != 0) {
            std::cerr << "[bng_cpp] parallel batch failed: " << failures
                      << " of " << results.size() << " job(s)\n";
            return 1;
        }
        return 0;
    } catch (const std::exception& exception) {
        std::cerr << "error: " << exception.what() << '\n';
        return 1;
    }
}

} // namespace bng::cli
