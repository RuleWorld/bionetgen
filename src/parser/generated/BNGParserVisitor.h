
// Generated from src/parser/BNGParser.g4 by ANTLR 4.13.1

#pragma once


#include "antlr4-runtime.h"
#include "BNGParser.h"



/**
 * This class defines an abstract visitor for a parse tree
 * produced by BNGParser.
 */
class  BNGParserVisitor : public antlr4::tree::AbstractParseTreeVisitor {
public:

  /**
   * Visit parse trees produced by BNGParser.
   */
    virtual std::any visitProg(BNGParser::ProgContext *context) = 0;

    virtual std::any visitHeader_block(BNGParser::Header_blockContext *context) = 0;

    virtual std::any visitVersion_def(BNGParser::Version_defContext *context) = 0;

    virtual std::any visitSubstance_def(BNGParser::Substance_defContext *context) = 0;

    virtual std::any visitSet_option(BNGParser::Set_optionContext *context) = 0;

    virtual std::any visitSet_model_name(BNGParser::Set_model_nameContext *context) = 0;

    virtual std::any visitProgram_block(BNGParser::Program_blockContext *context) = 0;

    virtual std::any visitParameters_block(BNGParser::Parameters_blockContext *context) = 0;

    virtual std::any visitParameter_def(BNGParser::Parameter_defContext *context) = 0;

    virtual std::any visitParam_name(BNGParser::Param_nameContext *context) = 0;

    virtual std::any visitMolecule_types_block(BNGParser::Molecule_types_blockContext *context) = 0;

    virtual std::any visitMolecule_type_def(BNGParser::Molecule_type_defContext *context) = 0;

    virtual std::any visitMolecule_def(BNGParser::Molecule_defContext *context) = 0;

    virtual std::any visitMolecule_attributes(BNGParser::Molecule_attributesContext *context) = 0;

    virtual std::any visitComponent_def_list(BNGParser::Component_def_listContext *context) = 0;

    virtual std::any visitComponent_def(BNGParser::Component_defContext *context) = 0;

    virtual std::any visitKeyword_as_component_name(BNGParser::Keyword_as_component_nameContext *context) = 0;

    virtual std::any visitKeyword_as_mol_name(BNGParser::Keyword_as_mol_nameContext *context) = 0;

    virtual std::any visitState_list(BNGParser::State_listContext *context) = 0;

    virtual std::any visitState_name(BNGParser::State_nameContext *context) = 0;

    virtual std::any visitSeed_species_block(BNGParser::Seed_species_blockContext *context) = 0;

    virtual std::any visitSeed_species_def(BNGParser::Seed_species_defContext *context) = 0;

    virtual std::any visitSpecies_def(BNGParser::Species_defContext *context) = 0;

    virtual std::any visitMolecule_compartment(BNGParser::Molecule_compartmentContext *context) = 0;

    virtual std::any visitMolecule_pattern(BNGParser::Molecule_patternContext *context) = 0;

    virtual std::any visitScope_prefix(BNGParser::Scope_prefixContext *context) = 0;

    virtual std::any visitPattern_bond_wildcard(BNGParser::Pattern_bond_wildcardContext *context) = 0;

    virtual std::any visitMolecule_tag(BNGParser::Molecule_tagContext *context) = 0;

    virtual std::any visitComponent_pattern_list(BNGParser::Component_pattern_listContext *context) = 0;

    virtual std::any visitComponent_pattern(BNGParser::Component_patternContext *context) = 0;

    virtual std::any visitState_value(BNGParser::State_valueContext *context) = 0;

    virtual std::any visitBond_spec(BNGParser::Bond_specContext *context) = 0;

    virtual std::any visitComponent_label(BNGParser::Component_labelContext *context) = 0;

    virtual std::any visitBond_id(BNGParser::Bond_idContext *context) = 0;

    virtual std::any visitObservables_block(BNGParser::Observables_blockContext *context) = 0;

    virtual std::any visitObservable_def(BNGParser::Observable_defContext *context) = 0;

    virtual std::any visitObservable_type(BNGParser::Observable_typeContext *context) = 0;

    virtual std::any visitObservable_pattern_list(BNGParser::Observable_pattern_listContext *context) = 0;

    virtual std::any visitObservable_pattern(BNGParser::Observable_patternContext *context) = 0;

    virtual std::any visitReaction_rules_block(BNGParser::Reaction_rules_blockContext *context) = 0;

    virtual std::any visitReaction_rule_def(BNGParser::Reaction_rule_defContext *context) = 0;

    virtual std::any visitLabel_def(BNGParser::Label_defContext *context) = 0;

    virtual std::any visitReactant_patterns(BNGParser::Reactant_patternsContext *context) = 0;

    virtual std::any visitProduct_patterns(BNGParser::Product_patternsContext *context) = 0;

    virtual std::any visitReaction_sign(BNGParser::Reaction_signContext *context) = 0;

    virtual std::any visitRate_law(BNGParser::Rate_lawContext *context) = 0;

    virtual std::any visitRule_modifiers(BNGParser::Rule_modifiersContext *context) = 0;

    virtual std::any visitPattern_list(BNGParser::Pattern_listContext *context) = 0;

    virtual std::any visitFunctions_block(BNGParser::Functions_blockContext *context) = 0;

    virtual std::any visitFunction_def(BNGParser::Function_defContext *context) = 0;

    virtual std::any visitParam_list(BNGParser::Param_listContext *context) = 0;

    virtual std::any visitCompartments_block(BNGParser::Compartments_blockContext *context) = 0;

    virtual std::any visitCompartment_def(BNGParser::Compartment_defContext *context) = 0;

    virtual std::any visitEnergy_patterns_block(BNGParser::Energy_patterns_blockContext *context) = 0;

    virtual std::any visitEnergy_pattern_def(BNGParser::Energy_pattern_defContext *context) = 0;

    virtual std::any visitPopulation_maps_block(BNGParser::Population_maps_blockContext *context) = 0;

    virtual std::any visitPopulation_map_def(BNGParser::Population_map_defContext *context) = 0;

    virtual std::any visitActions_block(BNGParser::Actions_blockContext *context) = 0;

    virtual std::any visitWrapped_actions_block(BNGParser::Wrapped_actions_blockContext *context) = 0;

    virtual std::any visitBegin_actions_block(BNGParser::Begin_actions_blockContext *context) = 0;

    virtual std::any visitAction_command(BNGParser::Action_commandContext *context) = 0;

    virtual std::any visitGenerate_network_cmd(BNGParser::Generate_network_cmdContext *context) = 0;

    virtual std::any visitSimulate_cmd(BNGParser::Simulate_cmdContext *context) = 0;

    virtual std::any visitWrite_cmd(BNGParser::Write_cmdContext *context) = 0;

    virtual std::any visitSet_cmd(BNGParser::Set_cmdContext *context) = 0;

    virtual std::any visitOther_action_cmd(BNGParser::Other_action_cmdContext *context) = 0;

    virtual std::any visitAction_args(BNGParser::Action_argsContext *context) = 0;

    virtual std::any visitAction_arg_list(BNGParser::Action_arg_listContext *context) = 0;

    virtual std::any visitAction_arg(BNGParser::Action_argContext *context) = 0;

    virtual std::any visitAction_arg_value(BNGParser::Action_arg_valueContext *context) = 0;

    virtual std::any visitQuoted_string(BNGParser::Quoted_stringContext *context) = 0;

    virtual std::any visitKeyword_as_value(BNGParser::Keyword_as_valueContext *context) = 0;

    virtual std::any visitNested_hash_list(BNGParser::Nested_hash_listContext *context) = 0;

    virtual std::any visitNested_hash_item(BNGParser::Nested_hash_itemContext *context) = 0;

    virtual std::any visitArg_name(BNGParser::Arg_nameContext *context) = 0;

    virtual std::any visitExpression_list(BNGParser::Expression_listContext *context) = 0;

    virtual std::any visitExpression(BNGParser::ExpressionContext *context) = 0;

    virtual std::any visitConditional_expr(BNGParser::Conditional_exprContext *context) = 0;

    virtual std::any visitOr_expr(BNGParser::Or_exprContext *context) = 0;

    virtual std::any visitAnd_expr(BNGParser::And_exprContext *context) = 0;

    virtual std::any visitEquality_expr(BNGParser::Equality_exprContext *context) = 0;

    virtual std::any visitRelational_expr(BNGParser::Relational_exprContext *context) = 0;

    virtual std::any visitAdditive_expr(BNGParser::Additive_exprContext *context) = 0;

    virtual std::any visitMultiplicative_expr(BNGParser::Multiplicative_exprContext *context) = 0;

    virtual std::any visitPower_expr(BNGParser::Power_exprContext *context) = 0;

    virtual std::any visitUnary_expr(BNGParser::Unary_exprContext *context) = 0;

    virtual std::any visitPrimary_expr(BNGParser::Primary_exprContext *context) = 0;

    virtual std::any visitFunction_call(BNGParser::Function_callContext *context) = 0;

    virtual std::any visitObservable_ref(BNGParser::Observable_refContext *context) = 0;

    virtual std::any visitLiteral(BNGParser::LiteralContext *context) = 0;


};

