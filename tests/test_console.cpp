#include <catch2/catch_test_macros.hpp>
#include <sstream>
#include <iostream>
#include "console/Console.hpp"
#include "ast/Model.hpp"

using namespace bng::console;
using namespace bng::ast;

// RAII helper to safely restore stream buffers
class StreamRedirector {
public:
    StreamRedirector(std::istream& in, std::streambuf* new_in_buf,
                     std::ostream& out, std::streambuf* new_out_buf,
                     std::ostream* err = nullptr, std::streambuf* new_err_buf = nullptr)
        : m_in(in), m_original_in(in.rdbuf()),
          m_out(out), m_original_out(out.rdbuf()),
          m_err(err), m_original_err(err ? err->rdbuf() : nullptr) {

        m_in.rdbuf(new_in_buf);
        m_out.rdbuf(new_out_buf);
        if (m_err && new_err_buf) {
            m_err->rdbuf(new_err_buf);
        }
    }

    ~StreamRedirector() {
        m_in.rdbuf(m_original_in);
        m_out.rdbuf(m_original_out);
        if (m_err && m_original_err) {
            m_err->rdbuf(m_original_err);
        }
    }

private:
    std::istream& m_in;
    std::streambuf* m_original_in;
    std::ostream& m_out;
    std::streambuf* m_original_out;
    std::ostream* m_err;
    std::streambuf* m_original_err;
};

// RAII helper to delete temp file
class TempFileGuard {
public:
    explicit TempFileGuard(const std::string& path) : m_path(path) {}
    ~TempFileGuard() { std::remove(m_path.c_str()); }
private:
    std::string m_path;
};

TEST_CASE("Console::run handles basic flow", "[console]") {
    std::istringstream input("help\nquit\n");
    std::ostringstream output;

    StreamRedirector redirector(std::cin, input.rdbuf(), std::cout, output.rdbuf());

    auto model = std::make_unique<Model>();
    model->setModelName("TestModel123");
    std::filesystem::path sourcePath;

    Console::run(std::move(model), sourcePath, false);

    std::string out_str = output.str();
    REQUIRE(out_str.find("BioNetGen C++ Interactive Console") != std::string::npos);
    REQUIRE(out_str.find("Model: TestModel123") != std::string::npos);
    REQUIRE(out_str.find("help") != std::string::npos);
    REQUIRE(out_str.find("Goodbye") != std::string::npos);
}

TEST_CASE("Console::run ends on EOF", "[console]") {
    std::istringstream input("help\n");
    std::ostringstream output;

    StreamRedirector redirector(std::cin, input.rdbuf(), std::cout, output.rdbuf());

    auto model = std::make_unique<Model>();
    std::filesystem::path sourcePath;

    Console::run(std::move(model), sourcePath, false);

    std::string out_str = output.str();
    REQUIRE(out_str.find("BioNetGen C++ Interactive Console") != std::string::npos);
    REQUIRE(out_str.find("Goodbye") != std::string::npos);
}

TEST_CASE("Console::run handles show commands and unknown commands", "[console]") {
    std::istringstream input(
        "show parameters\n"
        "show species\n"
        "show rules\n"
        "show observables\n"
        "some_unknown_command\n"
        "quit\n"
    );
    std::ostringstream output;
    std::ostringstream error_output;

    StreamRedirector redirector(std::cin, input.rdbuf(), std::cout, output.rdbuf(), &std::cerr, error_output.rdbuf());

    auto model = std::make_unique<Model>();
    std::filesystem::path sourcePath;

    Console::run(std::move(model), sourcePath, false);

    std::string out_str = output.str();
    std::string err_str = error_output.str();

    REQUIRE(out_str.find("(no parameters)") != std::string::npos);
    REQUIRE(out_str.find("(no seed species)") != std::string::npos);
    REQUIRE(out_str.find("(no reaction rules)") != std::string::npos);
    REQUIRE(out_str.find("(no observables)") != std::string::npos);

    REQUIRE(err_str.find("some_unknown_command") != std::string::npos);
    REQUIRE(err_str.find("ERROR") != std::string::npos);
}

TEST_CASE("Console::run handles load command", "[console]") {
    std::string tempBngl = "temp_test_load.bngl";
    FILE* f = fopen(tempBngl.c_str(), "w");
    fprintf(f, "begin model\nend model\n");
    fclose(f);

    TempFileGuard fileGuard(tempBngl);

    std::istringstream input("load temp_test_load.bngl\nquit\n");
    std::ostringstream output;

    StreamRedirector redirector(std::cin, input.rdbuf(), std::cout, output.rdbuf());

    auto model = std::make_unique<Model>();
    std::filesystem::path sourcePath;

    Console::run(std::move(model), sourcePath, false);

    std::string out_str = output.str();
    REQUIRE(out_str.find("Model loaded from temp_test_load.bngl") != std::string::npos);
}
