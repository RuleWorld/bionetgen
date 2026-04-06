#include "HybridModelGenerator.hpp"

#include <fstream>
#include <iostream>
#include <set>
#include <sstream>
#include <stdexcept>

#include "antlr4-runtime.h"
#include "BNGLexer.h"
#include "BNGParser.h"
#include "parser/BNGAstVisitor.hpp"
#include "parser/PatternGraphBuilder.hpp"
#include "core/Ullmann.hpp"
#include "io/BnglWriter.hpp"

namespace bng::engine {

HybridModelGenerator::HybridModelGenerator(ast::Model& model, const GeneratedNetwork& network)
    : model_(model), network_(network) {
}

bool HybridModelGenerator::isIsomorphic(const std::string& pattern1,
                                          const std::string& pattern2) const {
    // Use Ullmann subgraph isomorphism to check if two patterns are isomorphic.
    // Parse both patterns and check for bidirectional matching.
    try {
        auto& mutableModel = const_cast<ast::Model&>(model_);

        antlr4::ANTLRInputStream input1(pattern1);
        BNGLexer lexer1(&input1);
        antlr4::CommonTokenStream tokens1(&lexer1);
        BNGParser parser1(&tokens1);
        auto* species1 = parser1.species_def();
        if (parser1.getNumberOfSyntaxErrors() != 0) return false;
        const auto graph1 = bng::parser::buildPatternGraph(species1, mutableModel, true);

        antlr4::ANTLRInputStream input2(pattern2);
        BNGLexer lexer2(&input2);
        antlr4::CommonTokenStream tokens2(&lexer2);
        BNGParser parser2(&tokens2);
        auto* species2 = parser2.species_def();
        if (parser2.getNumberOfSyntaxErrors() != 0) return false;
        const auto graph2 = bng::parser::buildPatternGraph(species2, mutableModel, true);

        // Check both directions for isomorphism
        BNGcore::UllmannSGIso matcher1(graph1, graph2);
        BNGcore::List<BNGcore::Map> maps1;
        if (matcher1.find_maps(maps1) == 0) return false;

        BNGcore::UllmannSGIso matcher2(graph2, graph1);
        BNGcore::List<BNGcore::Map> maps2;
        if (matcher2.find_maps(maps2) == 0) return false;

        return true;
    } catch (...) {
        return false;
    }
}

std::size_t HybridModelGenerator::countMatches(const std::string& obsPattern,
                                                 const std::string& speciesPattern) const {
    try {
        auto& mutableModel = const_cast<ast::Model&>(model_);

        antlr4::ANTLRInputStream input1(obsPattern);
        BNGLexer lexer1(&input1);
        antlr4::CommonTokenStream tokens1(&lexer1);
        BNGParser parser1(&tokens1);
        auto* species1 = parser1.species_def();
        if (parser1.getNumberOfSyntaxErrors() != 0) return 0;
        const auto pattern = bng::parser::buildPatternGraph(species1, mutableModel, true);

        antlr4::ANTLRInputStream input2(speciesPattern);
        BNGLexer lexer2(&input2);
        antlr4::CommonTokenStream tokens2(&lexer2);
        BNGParser parser2(&tokens2);
        auto* species2 = parser2.species_def();
        if (parser2.getNumberOfSyntaxErrors() != 0) return 0;
        const auto target = bng::parser::buildPatternGraph(species2, mutableModel, true);

        BNGcore::UllmannSGIso matcher(pattern, target);
        BNGcore::List<BNGcore::Map> maps;
        return matcher.find_maps(maps);
    } catch (...) {
        return 0;
    }
}

HybridModelGenerator::GenerateResult
HybridModelGenerator::generate(const std::filesystem::path& sourcePath, const Options& opts) {
    GenerateResult result;

    const std::string indent = "    ";

    std::cerr << "ACTION: generate_hybrid_model( " << model_.getModelName() << " )\n";

    // Print options
    std::cerr << "options = \n";
    if (!opts.prefix.empty())
        std::cerr << "      prefix => " << opts.prefix << "\n";
    std::cerr << "      suffix => " << opts.suffix << "\n";
    std::cerr << "   overwrite => " << opts.overwrite << "\n";
    std::cerr << "     verbose => " << opts.verbose << "\n";
    std::cerr << "        safe => " << opts.safe << "\n";
    std::cerr << "     execute => " << opts.execute << "\n";

    // === Step 0: Validate preconditions ===
    // Check if PopulationMaps exist (equivalent to PopulationTypesList/PopulationList)
    if (model_.getPopulationMaps().empty()) {
        throw std::runtime_error(
            "Nothing to do! Model " + model_.getModelName() +
            " has zero population map definitions.");
    }

    if (model_.getMoleculeTypes().empty()) {
        throw std::runtime_error(
            "Nothing to do! Model " + model_.getModelName() +
            " has zero molecule type definitions.");
    }

    if (model_.getSeedSpecies().empty()) {
        throw std::runtime_error(
            "Nothing to do! Model " + model_.getModelName() +
            " has zero seed species definitions.");
    }

    if (model_.getReactionRules().empty()) {
        throw std::runtime_error(
            "Nothing to do! Model " + model_.getModelName() +
            " has zero reaction rule definitions.");
    }

    // Determine output paths
    // prefix = defined prefix ? model.getOutputPrefix(prefix) : model.getOutputPrefix()
    // prefix += "_" + suffix
    std::string prefix;
    if (!opts.prefix.empty()) {
        prefix = opts.prefix;
    } else {
        prefix = (sourcePath.parent_path() / sourcePath.stem()).string();
    }
    prefix += "_" + opts.suffix;

    const std::string modelname = std::filesystem::path(prefix).filename().string();
    const auto outdir = std::filesystem::path(prefix).parent_path();
    const std::string modelfile = prefix + ".bngl";

    result.hybridBnglPath = modelfile;
    result.hybridModelName = modelname;

    // Check if file exists
    if (std::filesystem::exists(modelfile)) {
        if (opts.overwrite) {
            std::cerr << "WARNING: Overwriting older model file: " << modelfile << "\n";
            std::filesystem::remove(modelfile);
        } else {
            throw std::runtime_error(
                "Model file " + modelfile +
                " already exists. Set overwrite=>1 option to force overwrite.");
        }
    }

    // === Create new hybrid model ===
    ast::Model hybridModel;

    // Step 1: Copy metadata
    hybridModel.setModelName(modelname);
    hybridModel.setVersion(model_.getVersion());
    hybridModel.setSubstanceUnits(model_.getSubstanceUnits());

    // Copy options
    for (const auto& [key, value] : model_.getOptions()) {
        hybridModel.setOption(key, value);
    }

    // === Step 2: Copy constant parameters ===
    std::size_t stepIndex = 0;
    std::cerr << indent << stepIndex << ":Fetching model parameters.. ";
    ++stepIndex;

    std::size_t nConstants = 0;
    for (const auto& param : model_.getParameters().all()) {
        // Copy all parameters (constants and expressions, but not observables/functions)
        // In Perl, copyConstant() copies parameters that are Constants or Expressions
        hybridModel.addParameter(ast::Parameter(param.getName(), param.getExpression()));
        ++nConstants;
    }
    result.nParameters = nConstants;
    std::cerr << "found " << nConstants << " constants and expressions.\n";

    // === Step 3: Copy compartments ===
    if (!model_.getCompartments().empty()) {
        std::cerr << indent << stepIndex << ":Fetching compartments.. ";
        ++stepIndex;
        for (const auto& comp : model_.getCompartments()) {
            hybridModel.addCompartment(comp);
        }
        result.nCompartments = model_.getCompartments().size();
        std::cerr << indent << "found " << result.nCompartments << " compartments.\n";
        if (result.nCompartments > 0) {
            std::cerr << "WARNING: generate_hybrid_model() does not support compartments at this time.\n";
        }
    }

    // === Step 4: Copy molecule types + add population types ===
    std::cerr << indent << stepIndex << ":Fetching molecule types..   ";
    ++stepIndex;
    for (const auto& mt : model_.getMoleculeTypes()) {
        hybridModel.addMoleculeType(mt);
    }
    result.nMoleculeTypes = model_.getMoleculeTypes().size();
    std::cerr << "found " << result.nMoleculeTypes << " molecule types.\n";

    // Add population types (derived from population maps)
    // In Perl, population types come from PopulationTypesList. In our C++ model,
    // population types are derived from the population map entries.
    // Each population map has a pattern and a population function.
    // We need to extract unique molecule type names that serve as population types.
    std::cerr << indent << stepIndex << ":Adding population types..   ";
    ++stepIndex;

    std::set<std::string> existingMtNames;
    for (const auto& mt : model_.getMoleculeTypes()) {
        existingMtNames.insert(mt.getName());
    }

    // Extract population type names from population maps
    // The population function typically creates a new molecule type for the population
    std::set<std::string> popTypeNames;
    for (const auto& pm : model_.getPopulationMaps()) {
        // The population function name is used as a molecule type for the population
        // In Perl: PopulationTypesList->MolTypes contains the population molecule types
        if (!pm.populationFunction.empty()) {
            popTypeNames.insert(pm.populationFunction);
        }
    }

    std::size_t nPopTypes = 0;
    for (const auto& name : popTypeNames) {
        if (existingMtNames.count(name) > 0) {
            throw std::runtime_error(
                "PopulationType " + name + " clashes with MoleculeType of the same name");
        }
        // Add a simple population molecule type (no components)
        ast::MoleculeType popMt(name, {});
        hybridModel.addMoleculeType(popMt);
        ++nPopTypes;
    }
    result.nPopulationTypes = nPopTypes;
    result.nMoleculeTypes += nPopTypes;
    std::cerr << "found " << nPopTypes << " population types.\n";

    // === Step 5: Copy seed species, replacing with populations if isomorphic ===
    std::cerr << indent << stepIndex << ":Fetching seed species..\n";
    ++stepIndex;

    // Build a list of population patterns for isomorphism checking
    // Each population map has a patternText (the species) and a populationFunction (the pop variable)
    struct PopInfo {
        std::string speciesPattern;   // The species graph pattern
        std::string populationName;   // The population molecule type name
    };
    std::vector<PopInfo> populations;
    for (const auto& pm : model_.getPopulationMaps()) {
        PopInfo pi;
        pi.speciesPattern = pm.patternText;
        pi.populationName = pm.populationFunction;
        // Build the population species string: just the population function name with args
        // In Perl: $pop->Population is the population molecule SpeciesGraph
        populations.push_back(pi);
    }

    std::set<std::string> addedPopulations;  // Track which populations are in seed species

    for (const auto& species : model_.getSeedSpecies()) {
        const std::string& pattern = species.getPattern();
        bool isPop = false;

        // Check if this species is isomorphic to any population
        for (const auto& pop : populations) {
            if (isIsomorphic(pattern, pop.speciesPattern)) {
                // Replace with population species
                // In Perl: $slist_new->add($pop->Population->copy(), $species->Concentration)
                hybridModel.addSeedSpecies(ast::SeedSpecies(
                    pop.populationName + "()",
                    species.getAmount(),
                    species.isConstant(),
                    species.getCompartment()));
                addedPopulations.insert(pop.populationName);
                isPop = true;

                if (opts.verbose) {
                    std::cerr << indent << indent
                              << "replaced species " << pattern
                              << " with population " << pop.populationName << "().\n";
                }
                break;
            }
        }

        if (!isPop) {
            // Not a population, add directly
            hybridModel.addSeedSpecies(species);
        }
    }
    result.nSeedSpecies = hybridModel.getSeedSpecies().size();
    std::cerr << indent << "  ..found " << result.nSeedSpecies << " seed species.\n";

    // === Step 6: Add empty populations for populations not yet in seed species ===
    std::cerr << indent << stepIndex
              << ":Adding populations with zero counts to seed species..\n";
    ++stepIndex;

    std::size_t zeroPops = 0;
    for (const auto& pop : populations) {
        if (addedPopulations.count(pop.populationName) == 0) {
            hybridModel.addSeedSpecies(ast::SeedSpecies(
                pop.populationName + "()",
                ast::Expression::number(0),
                false));
            addedPopulations.insert(pop.populationName);
            ++zeroPops;
        }
    }
    result.nZeroPopulations = zeroPops;
    std::cerr << indent << "  ..added " << zeroPops << " populations to seed species list.\n";

    // === Step 7: Copy observables and add population pattern matches ===
    std::cerr << indent << stepIndex
              << ":Fetching observables and adding population matches..\n";
    ++stepIndex;

    for (const auto& obs : model_.getObservables()) {
        // Copy the observable
        std::vector<std::string> patterns = obs.getPatterns();

        // For each population, check if the observable pattern matches the
        // population's species graph. If so, add the population pattern.
        for (const auto& pop : populations) {
            for (const auto& obsPattern : obs.getPatterns()) {
                std::size_t matches = countMatches(obsPattern, pop.speciesPattern);
                if (matches > 0) {
                    // Add population pattern for each match
                    for (std::size_t m = 0; m < matches; ++m) {
                        patterns.push_back(pop.populationName + "()");
                    }
                    if (opts.verbose) {
                        std::cerr << indent << indent
                                  << "observable '" << obs.getName() << "':  +"
                                  << matches << " match"
                                  << (matches > 1 ? "es" : "")
                                  << " to " << pop.populationName << "().\n";
                    }
                }
            }
        }

        hybridModel.addObservable(ast::Observable(
            obs.getName(), obs.getType(), patterns));
    }
    result.nObservables = model_.getObservables().size();
    std::cerr << indent << "  ..found " << result.nObservables << " observables.\n";

    // === Step 8: Copy functions ===
    std::cerr << indent << stepIndex << ":Fetching functions.. ";
    ++stepIndex;

    for (const auto& func : model_.getFunctions()) {
        hybridModel.addFunction(func);
    }
    result.nFunctions = model_.getFunctions().size();
    std::cerr << "found " << result.nFunctions << " functions.\n";

    // === Step 9: Expand rules w.r.t. populations ===
    std::cerr << indent << stepIndex
              << ":Expanding rules with respect to population objects..\n";
    ++stepIndex;

    // Get the species patterns for each population
    std::vector<std::string> popSpeciesPatterns;
    for (const auto& pop : populations) {
        popSpeciesPatterns.push_back(pop.speciesPattern);
    }

    std::size_t ruleCount = 0;
    for (const auto& rule : model_.getReactionRules()) {
        // In Perl: each element of @RxnRules is an array [forward, reverse].
        // For bidirectional rules, we handle both directions.
        // For now, copy the rule. In the full implementation, expandRule()
        // would specialize the rule for population species.

        // Check if any reactant matches a population species
        bool matchesPopulation = false;
        if (!opts.safe) {
            for (const auto& reactant : rule.getReactants()) {
                for (const auto& popPattern : popSpeciesPatterns) {
                    if (isIsomorphic(reactant, popPattern)) {
                        matchesPopulation = true;
                        break;
                    }
                }
                if (matchesPopulation) break;
            }
        }

        if (matchesPopulation) {
            // Create expanded copies that replace population-matching reactants
            // with population variables. For now, we add the original rule
            // plus a version with population reactants.
            // This is the expandRule() stub -- in full implementation,
            // RefineRule.pm logic would go here.

            // First, add the original rule
            hybridModel.addReactionRule(ast::ReactionRule(
                rule.getRuleName(),
                rule.getLabel(),
                rule.getReactants(),
                rule.getProducts(),
                rule.getRates(),
                rule.getModifiers(),
                false,  // Each direction is separate in hybrid model
                rule.getReactantPatterns(),
                rule.getProductPatterns()));

            // Create population-specialized version(s)
            // Replace matching reactants with their population equivalents
            auto reactantsCopy = rule.getReactants();
            auto productsCopy = rule.getProducts();
            bool modified = false;

            for (auto& reactant : reactantsCopy) {
                for (std::size_t p = 0; p < populations.size(); ++p) {
                    if (isIsomorphic(reactant, populations[p].speciesPattern)) {
                        reactant = populations[p].populationName + "()";
                        modified = true;
                        break;
                    }
                }
            }
            for (auto& product : productsCopy) {
                for (std::size_t p = 0; p < populations.size(); ++p) {
                    if (isIsomorphic(product, populations[p].speciesPattern)) {
                        product = populations[p].populationName + "()";
                        modified = true;
                        break;
                    }
                }
            }

            if (modified) {
                std::string popRuleName = rule.getRuleName() + "_pop";
                hybridModel.addReactionRule(ast::ReactionRule(
                    popRuleName,
                    rule.getLabel(),
                    reactantsCopy,
                    productsCopy,
                    rule.getRates(),
                    rule.getModifiers(),
                    false));
            }

            if (opts.verbose) {
                std::cerr << indent << indent
                          << "Rule '" << rule.getRuleName()
                          << "': expanded to " << (modified ? 2 : 1) << " child rule"
                          << (modified ? "s" : "") << ".\n";
            }
        } else {
            // No population match: copy rule as-is
            hybridModel.addReactionRule(ast::ReactionRule(
                rule.getRuleName(),
                rule.getLabel(),
                rule.getReactants(),
                rule.getProducts(),
                rule.getRates(),
                rule.getModifiers(),
                false,
                rule.getReactantPatterns(),
                rule.getProductPatterns()));
        }
        ++ruleCount;
    }
    result.nRules = ruleCount;
    std::cerr << indent << "  ..finished processing " << ruleCount << " reaction rules.\n";

    // === Step 10: Add population mapping rules ===
    std::cerr << indent << stepIndex << ":Fetching population maps.. ";
    ++stepIndex;

    for (const auto& pm : model_.getPopulationMaps()) {
        // In Perl: write rule as string from $pop->MappingRule->toString(),
        // then parse it back. Here we construct mapping rules directly.
        // A population mapping rule maps: species_pattern -> population()
        // Rate is typically 0 (the mapping is applied by the simulator, not kinetics)

        // Build the mapping rule: pattern -> popName()  rate
        // These are typically: species_pattern -> popName()  kmap
        // where kmap is effectively handled by the hybrid simulator
        std::string ruleStr = pm.patternText + " -> " + pm.populationFunction + "()";

        // Add as a reaction rule with a special name
        std::string mapRuleName = "_map_" + pm.populationFunction;
        hybridModel.addReactionRule(ast::ReactionRule(
            mapRuleName,
            pm.label,
            {pm.patternText},
            {pm.populationFunction + "()"},
            {ast::Expression::number(0)},  // Mapping rules have zero rate by default
            {},
            false));
    }
    result.nPopulationMaps = model_.getPopulationMaps().size();
    std::cerr << "found " << result.nPopulationMaps << " maps.\n";

    // === Step 11: Create empty reaction list ===
    std::cerr << indent << stepIndex << ":Creating empty reaction list.\n";
    ++stepIndex;
    // No explicit action needed; the hybrid model starts with no generated reactions

    // === Step 12: Write hybrid BNGL file ===
    std::cerr << indent << stepIndex << ":Attempting to write hybrid BNGL.. ";
    ++stepIndex;

    {
        std::ofstream out(modelfile);
        if (!out) {
            throw std::runtime_error("Couldn't write to " + modelfile);
        }

        // Use BnglWriter to serialize the hybrid model
        io::BnglWriter::Options writerOpts;
        writerOpts.includeComments = true;
        writerOpts.includeActions = false;
        writerOpts.evaluateExpressions = false;

        std::string bnglContent = io::BnglWriter::write(hybridModel, nullptr, writerOpts);
        out << bnglContent;

        // Write actions (matching Perl: write each action on its own line)
        if (!opts.actions.empty()) {
            out << "\n\n## model actions ##\n";
            for (const auto& action : opts.actions) {
                out << action << "\n";
            }
            out << "\n";
        }

        out.close();
    }

    std::cerr << "done.\n";
    std::cerr << "Wrote hybrid model to file " << modelfile << ".\n";

    // === Step 13: Optionally execute actions ===
    // In Perl, this calls $hybrid_model->$action for each action.
    // In C++, we flag that execution was requested; the caller in ActionDispatch
    // handles the actual execution.
    if (opts.execute) {
        std::cerr << indent << "Executing actions on hybrid model...\n";
        // Execution is handled by the caller (ActionDispatch.cpp) since it requires
        // parsing the hybrid BNGL and running the simulation pipeline.
    }

    return result;
}

} // namespace bng::engine
