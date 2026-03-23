#include "Model.hpp"

#include <utility>

namespace bng {
namespace ast {

Model::Model() = default;
Model::~Model() = default;

void Model::addCompartment(const Compartment& comp) {
    compartments.push_back(comp);
}

void Model::addMolecule(const Molecule& mol) {
    molecules.push_back(mol);
}

void Model::addParameter(Parameter parameter) {
    parameters_.add(std::move(parameter));
}

void Model::addAction(Action action) {
    actions_.push_back(std::move(action));
}

void Model::addFunction(Function function) {
    functions_.push_back(std::move(function));
}

void Model::addEnergyPattern(EnergyPattern energyPattern) {
    energyPatterns_.push_back(std::move(energyPattern));
}

void Model::addObservable(Observable observable) {
    observables_.push_back(std::move(observable));
}

void Model::addMoleculeType(MoleculeType moleculeType) {
    moleculeTypes_.push_back(std::move(moleculeType));
}

void Model::addSeedSpecies(SeedSpecies seedSpecies) {
    seedSpecies_.push_back(std::move(seedSpecies));
}

void Model::addReactionRule(ReactionRule reactionRule) {
    reactionRules_.push_back(std::move(reactionRule));
}

void Model::setVersion(std::string version) {
    version_ = std::move(version);
}

void Model::setSubstanceUnits(std::string units) {
    substanceUnits_ = std::move(units);
}

void Model::setModelName(std::string modelName) {
    modelName_ = std::move(modelName);
}

void Model::setOption(std::string key, std::string value) {
    options_[std::move(key)] = std::move(value);
}

const std::vector<Compartment>& Model::getCompartments() const {
    return compartments;
}

const std::vector<Molecule>& Model::getMolecules() const {
    return molecules;
}

const ParameterList& Model::getParameters() const {
    return parameters_;
}

ParameterList& Model::getParameters() {
    return parameters_;
}

const std::vector<Action>& Model::getActions() const {
    return actions_;
}

std::vector<Action>& Model::getActions() {
    return actions_;
}

const std::vector<Function>& Model::getFunctions() const {
    return functions_;
}

const std::vector<EnergyPattern>& Model::getEnergyPatterns() const {
    return energyPatterns_;
}

const std::vector<Observable>& Model::getObservables() const {
    return observables_;
}

const std::vector<MoleculeType>& Model::getMoleculeTypes() const {
    return moleculeTypes_;
}

const std::vector<SeedSpecies>& Model::getSeedSpecies() const {
    return seedSpecies_;
}

const std::vector<ReactionRule>& Model::getReactionRules() const {
    return reactionRules_;
}

const std::string& Model::getVersion() const {
    return version_;
}

const std::string& Model::getSubstanceUnits() const {
    return substanceUnits_;
}

const std::string& Model::getModelName() const {
    return modelName_;
}

const std::map<std::string, std::string>& Model::getOptions() const {
    return options_;
}

GraphTypeRegistry& Model::getGraphTypeRegistry() {
    return graphTypeRegistry_;
}

} // namespace ast
} // namespace bng
