#pragma once

#include <filesystem>
#include <memory>
#include <string>

#include "ast/Model.hpp"

namespace bng::console {

/**
 * Console - Interactive BNGL shell (REPL).
 *
 * Provides an interactive command-line interface for BioNetGen, similar
 * to Perl BNG2's Console.pm. Supports:
 * - Parameter setting: setParameter("name", value)
 * - Concentration changes: setConcentration("species", value)
 * - Network generation: generate_network({})
 * - Simulation: simulate({method=>"ode", t_end=>100})
 * - Model info: show parameters / show species / show rules
 * - quit / exit
 *
 * Invoked when the binary is run with --console or -i flag.
 *
 * Reference: BNG2/bng2/Perl2/Console.pm
 */
class Console {
public:
    /**
     * Start the interactive console.
     * @param model Unique pointer to model (may be empty model if no file loaded).
     *              Ownership is taken; the pointer may be replaced on "load".
     * @param sourcePath Optional source path for output file naming
     * @param verbose Enable verbose output
     */
    static void run(std::unique_ptr<ast::Model> model,
                    const std::filesystem::path& sourcePath,
                    bool verbose = false);

private:
    static void printHelp();
    static void showParameters(const ast::Model& model);
    static void showSpecies(const ast::Model& model);
    static void showRules(const ast::Model& model);
    static void showObservables(const ast::Model& model);

    static bool handleCommand(const std::string& line,
                              std::unique_ptr<ast::Model>& model,
                              const std::filesystem::path& sourcePath, bool verbose);
};

} // namespace bng::console
