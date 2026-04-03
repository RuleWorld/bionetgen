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

    // Collect per-molecule compartments from the species_def level
    // Grammar: (AT STRING COLON)? mol mol_comp? (DOT mol mol_comp?)* (AT STRING)?
    // molecule_compartment children of species_def are the suffix compartments between molecules
    const auto& speciesMolComps = ctx->molecule_compartment();
    const auto& moleculePatterns = ctx->molecule_pattern();

    for (std::size_t mi = 0; mi < moleculePatterns.size(); ++mi) {
        auto* moleculePattern = moleculePatterns[mi];
        const auto& moleculeType = ensureInferredMoleculeType(moleculePattern, model);
        BNGcore::Node moleculeNodeValue(model.getGraphTypeRegistry().ensureMoleculeType(moleculeType));
        auto* moleculeNode = graph.add_node(moleculeNodeValue);

        // Extract per-molecule compartment from two sources:
        // 1. Inside molecule_pattern: A@CYT(x) — compartment before parentheses
        // 2. After molecule_pattern at species_def level: A(x)@CYT — suffix compartment
        std::string molComp;
        if (moleculePattern->molecule_compartment() != nullptr) {
            // Compartment inside molecule_pattern (e.g., A@CYT(...))
            molComp = moleculePattern->molecule_compartment()->STRING()->getText();
        } else if (mi < speciesMolComps.size() && speciesMolComps[mi] != nullptr) {
            // Compartment at species_def level after this molecule (e.g., A(...)@CYT)
            molComp = speciesMolComps[mi]->STRING()->getText();
        }
        if (!molComp.empty()) {
            moleculeNode->set_compartment(molComp);
        }

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

                const auto& runtimeStateType = componentNode->get_type().get_state_type();
                if (const auto* labelStateType = dynamic_cast<const BNGcore::LabelStateType*>(&runtimeStateType)) {
                    if (!componentPattern->state_value().empty()) {
                        BNGcore::LabelState state(*labelStateType, componentPattern->state_value().front()->getText());
                        componentNode->set_state(state);
                    } else {
                        // No state specified in pattern → wildcard (matches any state)
                        // Use "?" which LabelState::operator== treats as wildcard
                        BNGcore::LabelState wildcardState(*labelStateType, "?");
                        componentNode->set_state(wildcardState);
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

                // Extract component label/tag (e.g., %1, %2)
                if (!componentPattern->component_label().empty()) {
                    auto* labelCtx = componentPattern->component_label().front();
                    std::string tag = labelCtx->getText();
                    // Strip leading % sign
                    if (!tag.empty() && tag[0] == '%') {
                        tag = tag.substr(1);
                    }
                    componentNode->set_label_tag(tag);
                }
            }
        }
    }

    return graph;
}

std::string extractSpeciesCompartment(BNGParser::Species_defContext* ctx) {
    if (ctx == nullptr) {
        return {};
    }

    // First, check for explicit species-level prefix compartment: @COMP:
    if (ctx->COLON() != nullptr && !ctx->AT().empty()) {
        // There's a prefix compartment annotation (@COMP:)
        const auto& strings = ctx->STRING();
        if (!strings.empty()) {
            return strings[0]->getText();
        }
    }

    // Next, check for explicit species-level suffix compartment: @COMP at the end
    auto text = ctx->getText();
    const auto atPos = text.rfind('@');
    if (atPos != std::string::npos && atPos > 0) {
        // Make sure it's after all the parentheses and molecule patterns
        const auto lastCloseParen = text.rfind(')');
        if (lastCloseParen != std::string::npos && atPos > lastCloseParen) {
            // This is a suffix compartment
            return text.substr(atPos + 1);
        }
    }

    // Finally, try to infer from molecule-level compartments
    // If all molecules have the same compartment, use that
    std::string inferredCompartment;
    bool allSame = true;
    for (auto* molPattern : ctx->molecule_pattern()) {
        if (molPattern == nullptr) continue;

        std::string molCompartment;
        if (molPattern->molecule_compartment() != nullptr) {
            auto* molComp = molPattern->molecule_compartment();
            if (molComp->STRING() != nullptr) {
                molCompartment = molComp->STRING()->getText();
            }
        }

        if (inferredCompartment.empty() && !molCompartment.empty()) {
            inferredCompartment = molCompartment;
        } else if (!molCompartment.empty() && molCompartment != inferredCompartment) {
            allSame = false;
            break;
        }
    }

    return allSame ? inferredCompartment : std::string();
}

bool isSpeciesCompartmentPrefix(BNGParser::Species_defContext* ctx) {
    if (ctx == nullptr) return false;
    return (ctx->COLON() != nullptr && !ctx->AT().empty());
}

} // namespace bng::parser

