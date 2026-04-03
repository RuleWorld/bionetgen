#pragma once
#include <map>
#include <string>
#include <vector>
#include "Compartment.hpp"
#include "EnergyPattern.hpp"
#include "Function.hpp"
#include "GraphTypeRegistry.hpp"
#include "Molecule.hpp"
#include "MoleculeType.hpp"
#include "Observable.hpp"
#include "ParameterList.hpp"
#include "ReactionRule.hpp"
#include "SeedSpecies.hpp"

namespace bng {
namespace ast {

struct Action {
    std::string name;
    std::map<std::string, std::string> arguments;
};

class Model {
public:
    Model();
    ~Model();

    void addCompartment(const Compartment& comp);
    void addMolecule(const Molecule& mol);
    void addParameter(Parameter parameter);
    void addAction(Action action);
    void addFunction(Function function);
    void addEnergyPattern(EnergyPattern energyPattern);
    void addObservable(Observable observable);
    void addMoleculeType(MoleculeType moleculeType);
    void addSeedSpecies(SeedSpecies seedSpecies);
    void addReactionRule(ReactionRule reactionRule);
    void setVersion(std::string version);
    void setSubstanceUnits(std::string units);
    void setModelName(std::string modelName);
    void setOption(std::string key, std::string value);
    
    const std::vector<Compartment>& getCompartments() const;
    std::vector<Compartment>& getCompartments();
    const std::vector<Molecule>& getMolecules() const;
    const ParameterList& getParameters() const;
    ParameterList& getParameters();
    const std::vector<Action>& getActions() const;
    std::vector<Action>& getActions();
    const std::vector<Function>& getFunctions() const;
    const std::vector<EnergyPattern>& getEnergyPatterns() const;
    const std::vector<Observable>& getObservables() const;
    const std::vector<MoleculeType>& getMoleculeTypes() const;
    const std::vector<SeedSpecies>& getSeedSpecies() const;
    const std::vector<ReactionRule>& getReactionRules() const;
    std::vector<ReactionRule>& getReactionRules();
    const std::string& getVersion() const;
    const std::string& getSubstanceUnits() const;
    const std::string& getModelName() const;
    const std::map<std::string, std::string>& getOptions() const;
    GraphTypeRegistry& getGraphTypeRegistry();

private:
    std::vector<Compartment> compartments;
    std::vector<Molecule> molecules;
    ParameterList parameters_;
    std::vector<Action> actions_;
    std::vector<Function> functions_;
    std::vector<EnergyPattern> energyPatterns_;
    std::vector<Observable> observables_;
    std::vector<MoleculeType> moleculeTypes_;
    std::vector<SeedSpecies> seedSpecies_;
    std::vector<ReactionRule> reactionRules_;
    GraphTypeRegistry graphTypeRegistry_;
    std::string version_;
    std::string substanceUnits_;
    std::string modelName_;
    std::map<std::string, std::string> options_;
};

} // namespace ast
} // namespace bng
