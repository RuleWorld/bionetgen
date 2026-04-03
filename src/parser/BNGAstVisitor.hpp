#pragma once

#include <memory>

#include "ast/Model.hpp"
#include "generated/BNGParserBaseVisitor.h"

namespace bng::parser {

class BNGAstVisitor : public BNGParserBaseVisitor {
public:
    BNGAstVisitor();

    std::unique_ptr<ast::Model> takeModel();

    std::any visitProg(BNGParser::ProgContext* ctx) override;
    std::any visitVersion_def(BNGParser::Version_defContext* ctx) override;
    std::any visitSubstance_def(BNGParser::Substance_defContext* ctx) override;
    std::any visitSet_option(BNGParser::Set_optionContext* ctx) override;
    std::any visitSet_model_name(BNGParser::Set_model_nameContext* ctx) override;
    std::any visitParameter_def(BNGParser::Parameter_defContext* ctx) override;
    std::any visitFunction_def(BNGParser::Function_defContext* ctx) override;
    std::any visitCompartment_def(BNGParser::Compartment_defContext* ctx) override;
    std::any visitMolecule_type_def(BNGParser::Molecule_type_defContext* ctx) override;
    std::any visitSeed_species_def(BNGParser::Seed_species_defContext* ctx) override;
    std::any visitEnergy_pattern_def(BNGParser::Energy_pattern_defContext* ctx) override;
    std::any visitObservable_def(BNGParser::Observable_defContext* ctx) override;
    std::any visitReaction_rule_def(BNGParser::Reaction_rule_defContext* ctx) override;
    std::any visitGenerate_network_cmd(BNGParser::Generate_network_cmdContext* ctx) override;
    std::any visitSimulate_cmd(BNGParser::Simulate_cmdContext* ctx) override;
    std::any visitWrite_cmd(BNGParser::Write_cmdContext* ctx) override;
    std::any visitSet_cmd(BNGParser::Set_cmdContext* ctx) override;
    std::any visitOther_action_cmd(BNGParser::Other_action_cmdContext* ctx) override;

private:
    void addAction(const std::string& name, BNGParser::Action_argsContext* args);

    std::unique_ptr<ast::Model> currentModel_;
};

std::unique_ptr<ast::Model> parseModel(const std::string& sourceText);

} // namespace bng::parser
