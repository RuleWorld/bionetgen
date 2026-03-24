#include "PatternGraphBuilder.hpp"

#include <string>
#include <unordered_map>
#include <vector>

namespace bng::parser {

namespace {

std::string moleculePatternName(BNGParser::Molecule_patternContext* ctx) {
    if (ctx->STRING() != nullptr) {
        return ctx->STRING()->getText();
    }
    if (ctx->keyword_as_mol_name() != nullptr) {
        return ctx->keyword_as_mol_name()->getText();
    }
    return ctx->getText();
}

std::string componentPatternName(BNGParser::Component_patternContext* ctx) {
    if (ctx->STRING() != nullptr) {
        return ctx->STRING()->getText();
    }
    if (ctx->INT() != nullptr) {
        return ctx->INT()->getText();
    }
    return ctx->keyword_as_component_name()->getText();
}

const bng::ast::MoleculeType* findMoleculeType(const bng::ast::Model& model, const std::string& name) {
    for (const auto& moleculeType : model.getMoleculeTypes()) {
        if (moleculeType.getName() == name) {
            return &moleculeType;
        }
    }
    return nullptr;
}

const bng::ast::ComponentType* findComponentType(const bng::ast::MoleculeType& moleculeType, const std::string& componentName) {
    for (const auto& componentType : moleculeType.getComponents()) {
        if (componentType.name == componentName) {
            return &componentType;
        }
    }
    return nullptr;
}

const bng::ast::MoleculeType& ensureInferredMoleculeType(BNGParser::Molecule_patternContext* ctx, bng::ast::Model& model) {
    const auto name = moleculePatternName(ctx);
    if (const auto* existing = findMoleculeType(model, name)) {
        return *existing;
    }

    std::vector<bng::ast::ComponentType> components;
    if (auto* componentList = ctx->component_pattern_list()) {
        for (auto* componentPattern : componentList->component_pattern()) {
            std::vector<std::string> states;
            for (auto* state : componentPattern->state_value()) {
                states.push_back(state->getText());
            }
            components.push_back({componentPatternName(componentPattern), std::move(states)});
        }
    }

    model.addMoleculeType(bng::ast::MoleculeType(name, std::move(components), false));
    return model.getMoleculeTypes().back();
}

} // namespace

BNGcore::PatternGraph buildPatternGraph(
    BNGParser::Species_defContext* ctx,
    ast::Model& model,
    bool treatUnspecifiedBondAsWildcard) {
    BNGcore::PatternGraph graph;
    std::unordered_map<std::string, BNGcore::Node*> bondNodesById;

    for (auto* moleculePattern : ctx->molecule_pattern()) {
        const auto& moleculeType = ensureInferredMoleculeType(moleculePattern, model);
        BNGcore::Node moleculeNodeValue(model.getGraphTypeRegistry().ensureMoleculeType(moleculeType));
        auto* moleculeNode = graph.add_node(moleculeNodeValue);

        if (auto* componentList = moleculePattern->component_pattern_list()) {
            for (auto* componentPattern : componentList->component_pattern()) {
                const auto componentName = componentPatternName(componentPattern);
                const auto* componentType = findComponentType(moleculeType, componentName);
                if (componentType == nullptr) {
                    continue;
                }

                BNGcore::Node componentNodeValue(model.getGraphTypeRegistry().ensureComponentType(moleculeType, *componentType));
                auto* componentNode = graph.add_node(componentNodeValue);
                graph.add_edge(moleculeNode, componentNode);

                if (!componentPattern->state_value().empty()) {
                    const auto& runtimeStateType = componentNode->get_type().get_state_type();
                    if (const auto* labelStateType = dynamic_cast<const BNGcore::LabelStateType*>(&runtimeStateType)) {
                        BNGcore::LabelState state(*labelStateType, componentPattern->state_value().front()->getText());
                        componentNode->set_state(state);
                    }
                }

                bool handledBond = false;
                for (auto* bondSpec : componentPattern->bond_spec()) {
                    if (bondSpec->bond_id() != nullptr) {
                        const auto bondId = bondSpec->bond_id()->getText();
                        auto* bondNode = bondNodesById[bondId];
                        if (bondNode == nullptr) {
                            BNGcore::Node bondNodeValue(BNGcore::BOND_NODE_TYPE);
                            bondNode = graph.add_node(bondNodeValue);
                            bondNode->set_state(BNGcore::BOUND_STATE);
                            bondNodesById[bondId] = bondNode;
                        }
                        graph.add_edge(componentNode, bondNode);
                        handledBond = true;
                    } else if (bondSpec->PLUS() != nullptr) {
                        BNGcore::Node bondNodeValue(BNGcore::BOND_NODE_TYPE);
                        auto* bondNode = graph.add_node(bondNodeValue);
                        bondNode->set_state(BNGcore::BOUND_STATE);
                        graph.add_edge(componentNode, bondNode);
                        handledBond = true;
                    } else if (bondSpec->QMARK() != nullptr) {
                        BNGcore::Node bondNodeValue(BNGcore::BOND_NODE_TYPE);
                        auto* bondNode = graph.add_node(bondNodeValue);
                        BNGcore::BondState wildcardState(BNGcore::BOND_STATE_TYPE, BNGcore::WILDCARD_STRING);
                        bondNode->set_state(wildcardState);
                        graph.add_edge(componentNode, bondNode);
                        handledBond = true;
                    } else if (bondSpec->DOT() != nullptr) {
                        BNGcore::Node bondNodeValue(BNGcore::BOND_NODE_TYPE);
                        auto* bondNode = graph.add_node(bondNodeValue);
                        bondNode->set_state(BNGcore::UNBOUND_STATE);
                        graph.add_edge(componentNode, bondNode);
                        handledBond = true;
                    }
                }

                if (!handledBond) {
                    BNGcore::Node bondNodeValue(BNGcore::BOND_NODE_TYPE);
                    auto* bondNode = graph.add_node(bondNodeValue);
                    if (treatUnspecifiedBondAsWildcard) {
                        BNGcore::BondState wildcardState(BNGcore::BOND_STATE_TYPE, BNGcore::WILDCARD_STRING);
                        bondNode->set_state(wildcardState);
                    } else {
                        bondNode->set_state(BNGcore::UNBOUND_STATE);
                    }
                    graph.add_edge(componentNode, bondNode);
                }
            }
        }
    }

    return graph;
}

} // namespace bng::parser

