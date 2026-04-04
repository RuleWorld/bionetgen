#include "console/Console.hpp"

#include <algorithm>
#include <cctype>
#include <iostream>
#include <sstream>
#include <string>

#include "actions/ActionDispatch.hpp"
#include "parser/BNGAstVisitor.hpp"

namespace bng::console {

namespace {

std::string trim(const std::string& s) {
    auto first = std::find_if_not(s.begin(), s.end(),
                                  [](unsigned char c) { return std::isspace(c); });
    auto last = std::find_if_not(s.rbegin(), s.rend(),
                                 [](unsigned char c) { return std::isspace(c); })
                    .base();
    if (first >= last) return {};
    return std::string(first, last);
}

std::string lowercase(std::string s) {
    std::transform(s.begin(), s.end(), s.begin(),
                   [](unsigned char c) { return static_cast<char>(std::tolower(c)); });
    return s;
}

} // namespace

void Console::printHelp() {
    std::cout << "BioNetGen Interactive Console\n"
              << "Commands:\n"
              << "  help                          - Show this help\n"
              << "  show parameters               - List all parameters\n"
              << "  show species                  - List seed species\n"
              << "  show rules                    - List reaction rules\n"
              << "  show observables              - List observables\n"
              << "  setParameter(\"name\", value)   - Set a parameter value\n"
              << "  setConcentration(\"spec\", val) - Set species concentration\n"
              << "  action({args})                - Execute a BNG action (e.g., generate_network, simulate)\n"
              << "  load <file.bngl>              - Load a BNGL model file\n"
              << "  quit / exit                   - Exit the console\n"
              << "\n"
              << "Any other input is attempted as a BNGL action command.\n";
}

void Console::showParameters(const ast::Model& model) {
    const auto& params = model.getParameters().all();
    if (params.empty()) {
        std::cout << "  (no parameters)\n";
        return;
    }
    for (const auto& p : params) {
        std::cout << "  " << p.getName() << " = " << p.getExpression().toString()
                  << " [= " << p.getValue() << "]"
                  << "\n";
    }
}

void Console::showSpecies(const ast::Model& model) {
    const auto& seeds = model.getSeedSpecies();
    if (seeds.empty()) {
        std::cout << "  (no seed species)\n";
        return;
    }
    for (std::size_t i = 0; i < seeds.size(); ++i) {
        std::cout << "  " << (i + 1) << ": " << seeds[i].getPattern()
                  << " " << seeds[i].getAmount().toString() << "\n";
    }
}

void Console::showRules(const ast::Model& model) {
    const auto& rules = model.getReactionRules();
    if (rules.empty()) {
        std::cout << "  (no reaction rules)\n";
        return;
    }
    for (std::size_t i = 0; i < rules.size(); ++i) {
        const auto& r = rules[i];
        std::string name = r.getRuleName().empty() ? r.getLabel() : r.getRuleName();
        std::cout << "  " << (i + 1) << ": " << name;

        // Show reactants -> products
        std::cout << "  ";
        for (std::size_t j = 0; j < r.getReactants().size(); ++j) {
            if (j > 0) std::cout << " + ";
            std::cout << r.getReactants()[j];
        }
        std::cout << (r.isBidirectional() ? " <-> " : " -> ");
        for (std::size_t j = 0; j < r.getProducts().size(); ++j) {
            if (j > 0) std::cout << " + ";
            std::cout << r.getProducts()[j];
        }

        // Show rate(s)
        for (const auto& rate : r.getRates()) {
            std::cout << " " << rate.toString();
        }
        std::cout << "\n";
    }
}

void Console::showObservables(const ast::Model& model) {
    const auto& obs = model.getObservables();
    if (obs.empty()) {
        std::cout << "  (no observables)\n";
        return;
    }
    for (std::size_t i = 0; i < obs.size(); ++i) {
        std::cout << "  " << obs[i].getType() << " " << obs[i].getName();
        for (const auto& pat : obs[i].getPatterns()) {
            std::cout << " " << pat;
        }
        std::cout << "\n";
    }
}

bool Console::handleCommand(const std::string& line,
                            std::unique_ptr<ast::Model>& model,
                            const std::filesystem::path& sourcePath, bool verbose) {
    const std::string trimmed = trim(line);
    if (trimmed.empty()) return true;

    const std::string lower = lowercase(trimmed);

    // Quit/exit
    if (lower == "quit" || lower == "exit") {
        return false;
    }

    // Help
    if (lower == "help" || lower == "?") {
        printHelp();
        return true;
    }

    // Show commands
    if (lower == "show parameters" || lower == "show params") {
        showParameters(*model);
        return true;
    }
    if (lower == "show species" || lower == "show seed_species") {
        showSpecies(*model);
        return true;
    }
    if (lower == "show rules" || lower == "show reaction_rules") {
        showRules(*model);
        return true;
    }
    if (lower == "show observables") {
        showObservables(*model);
        return true;
    }

    // Load command -- replaces the model entirely via unique_ptr swap
    if (lower.substr(0, 5) == "load ") {
        const std::string filePath = trim(trimmed.substr(5));
        try {
            auto loaded = parser::parseModelFromFile(filePath);
            if (loaded) {
                model = std::move(loaded);
                std::cout << "Model loaded from " << filePath << "\n";
                std::cout << "  parameters=" << model->getParameters().size()
                          << ", molecule_types=" << model->getMoleculeTypes().size()
                          << ", seed_species=" << model->getSeedSpecies().size()
                          << ", reaction_rules=" << model->getReactionRules().size()
                          << "\n";
            } else {
                std::cerr << "ERROR: Failed to parse " << filePath << "\n";
            }
        } catch (const std::exception& e) {
            std::cerr << "ERROR: " << e.what() << "\n";
        }
        return true;
    }

    // Try to interpret as a BNG action command.
    // Wrap it as an action and dispatch.
    try {
        // Parse the command as if it were in an actions block
        const std::string wrappedBngl = "begin actions\n" + trimmed + "\n" + "end actions\n";
        auto tempModel = parser::parseModel(wrappedBngl);
        if (tempModel && !tempModel->getActions().empty()) {
            // Transfer actions to the real model temporarily
            for (auto& act : tempModel->getActions()) {
                model->addAction(std::move(act));
            }
            actions::ActionDispatch::execute(*model, sourcePath, verbose);
            // Actions are consumed during execute, so just clear them
            model->getActions().clear();
        } else {
            std::cerr << "ERROR: Could not parse command: " << trimmed << "\n";
        }
    } catch (const std::exception& e) {
        std::cerr << "ERROR: " << e.what() << "\n";
    }

    return true;
}

void Console::run(std::unique_ptr<ast::Model> model,
                  const std::filesystem::path& sourcePath, bool verbose) {
    std::cout << "BioNetGen C++ Interactive Console\n";
    std::cout << "Type 'help' for available commands, 'quit' to exit.\n\n";

    if (!model->getModelName().empty()) {
        std::cout << "Model: " << model->getModelName() << "\n";
    }
    std::cout << "  parameters=" << model->getParameters().size()
              << ", molecule_types=" << model->getMoleculeTypes().size()
              << ", seed_species=" << model->getSeedSpecies().size()
              << ", reaction_rules=" << model->getReactionRules().size()
              << "\n\n";

    std::string line;
    while (true) {
        std::cout << "BNG> " << std::flush;
        if (!std::getline(std::cin, line)) {
            // EOF
            std::cout << "\n";
            break;
        }
        if (!handleCommand(line, model, sourcePath, verbose)) {
            break;
        }
    }

    std::cout << "Goodbye.\n";
}

} // namespace bng::console
