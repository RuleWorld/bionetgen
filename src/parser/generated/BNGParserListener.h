
// Generated from src/parser/BNGParser.g4 by ANTLR 4.13.1

#pragma once


#include "antlr4-runtime.h"
#include "BNGParser.h"


/**
 * This interface defines an abstract listener for a parse tree produced by BNGParser.
 */
class  BNGParserListener : public antlr4::tree::ParseTreeListener {
public:

  virtual void enterProg(BNGParser::ProgContext *ctx) = 0;
  virtual void exitProg(BNGParser::ProgContext *ctx) = 0;

  virtual void enterHeader_block(BNGParser::Header_blockContext *ctx) = 0;
  virtual void exitHeader_block(BNGParser::Header_blockContext *ctx) = 0;

  virtual void enterVersion_def(BNGParser::Version_defContext *ctx) = 0;
  virtual void exitVersion_def(BNGParser::Version_defContext *ctx) = 0;

  virtual void enterSubstance_def(BNGParser::Substance_defContext *ctx) = 0;
  virtual void exitSubstance_def(BNGParser::Substance_defContext *ctx) = 0;

  virtual void enterSet_option(BNGParser::Set_optionContext *ctx) = 0;
  virtual void exitSet_option(BNGParser::Set_optionContext *ctx) = 0;

  virtual void enterSet_model_name(BNGParser::Set_model_nameContext *ctx) = 0;
  virtual void exitSet_model_name(BNGParser::Set_model_nameContext *ctx) = 0;

  virtual void enterProgram_block(BNGParser::Program_blockContext *ctx) = 0;
  virtual void exitProgram_block(BNGParser::Program_blockContext *ctx) = 0;

  virtual void enterParameters_block(BNGParser::Parameters_blockContext *ctx) = 0;
  virtual void exitParameters_block(BNGParser::Parameters_blockContext *ctx) = 0;

  virtual void enterParameter_def(BNGParser::Parameter_defContext *ctx) = 0;
  virtual void exitParameter_def(BNGParser::Parameter_defContext *ctx) = 0;

  virtual void enterParam_name(BNGParser::Param_nameContext *ctx) = 0;
  virtual void exitParam_name(BNGParser::Param_nameContext *ctx) = 0;

  virtual void enterMolecule_types_block(BNGParser::Molecule_types_blockContext *ctx) = 0;
  virtual void exitMolecule_types_block(BNGParser::Molecule_types_blockContext *ctx) = 0;

  virtual void enterMolecule_type_def(BNGParser::Molecule_type_defContext *ctx) = 0;
  virtual void exitMolecule_type_def(BNGParser::Molecule_type_defContext *ctx) = 0;

  virtual void enterMolecule_def(BNGParser::Molecule_defContext *ctx) = 0;
  virtual void exitMolecule_def(BNGParser::Molecule_defContext *ctx) = 0;

  virtual void enterMolecule_attributes(BNGParser::Molecule_attributesContext *ctx) = 0;
  virtual void exitMolecule_attributes(BNGParser::Molecule_attributesContext *ctx) = 0;

  virtual void enterComponent_def_list(BNGParser::Component_def_listContext *ctx) = 0;
  virtual void exitComponent_def_list(BNGParser::Component_def_listContext *ctx) = 0;

  virtual void enterComponent_def(BNGParser::Component_defContext *ctx) = 0;
  virtual void exitComponent_def(BNGParser::Component_defContext *ctx) = 0;

  virtual void enterKeyword_as_component_name(BNGParser::Keyword_as_component_nameContext *ctx) = 0;
  virtual void exitKeyword_as_component_name(BNGParser::Keyword_as_component_nameContext *ctx) = 0;

  virtual void enterKeyword_as_mol_name(BNGParser::Keyword_as_mol_nameContext *ctx) = 0;
  virtual void exitKeyword_as_mol_name(BNGParser::Keyword_as_mol_nameContext *ctx) = 0;

  virtual void enterState_list(BNGParser::State_listContext *ctx) = 0;
  virtual void exitState_list(BNGParser::State_listContext *ctx) = 0;

  virtual void enterState_name(BNGParser::State_nameContext *ctx) = 0;
  virtual void exitState_name(BNGParser::State_nameContext *ctx) = 0;

  virtual void enterSeed_species_block(BNGParser::Seed_species_blockContext *ctx) = 0;
  virtual void exitSeed_species_block(BNGParser::Seed_species_blockContext *ctx) = 0;

  virtual void enterSeed_species_def(BNGParser::Seed_species_defContext *ctx) = 0;
  virtual void exitSeed_species_def(BNGParser::Seed_species_defContext *ctx) = 0;

  virtual void enterSpecies_def(BNGParser::Species_defContext *ctx) = 0;
  virtual void exitSpecies_def(BNGParser::Species_defContext *ctx) = 0;

  virtual void enterMolecule_compartment(BNGParser::Molecule_compartmentContext *ctx) = 0;
  virtual void exitMolecule_compartment(BNGParser::Molecule_compartmentContext *ctx) = 0;

  virtual void enterMolecule_pattern(BNGParser::Molecule_patternContext *ctx) = 0;
  virtual void exitMolecule_pattern(BNGParser::Molecule_patternContext *ctx) = 0;

  virtual void enterScope_prefix(BNGParser::Scope_prefixContext *ctx) = 0;
  virtual void exitScope_prefix(BNGParser::Scope_prefixContext *ctx) = 0;

  virtual void enterPattern_bond_wildcard(BNGParser::Pattern_bond_wildcardContext *ctx) = 0;
  virtual void exitPattern_bond_wildcard(BNGParser::Pattern_bond_wildcardContext *ctx) = 0;

  virtual void enterMolecule_tag(BNGParser::Molecule_tagContext *ctx) = 0;
  virtual void exitMolecule_tag(BNGParser::Molecule_tagContext *ctx) = 0;

  virtual void enterComponent_pattern_list(BNGParser::Component_pattern_listContext *ctx) = 0;
  virtual void exitComponent_pattern_list(BNGParser::Component_pattern_listContext *ctx) = 0;

  virtual void enterComponent_pattern(BNGParser::Component_patternContext *ctx) = 0;
  virtual void exitComponent_pattern(BNGParser::Component_patternContext *ctx) = 0;

  virtual void enterState_value(BNGParser::State_valueContext *ctx) = 0;
  virtual void exitState_value(BNGParser::State_valueContext *ctx) = 0;

  virtual void enterBond_spec(BNGParser::Bond_specContext *ctx) = 0;
  virtual void exitBond_spec(BNGParser::Bond_specContext *ctx) = 0;

  virtual void enterComponent_label(BNGParser::Component_labelContext *ctx) = 0;
  virtual void exitComponent_label(BNGParser::Component_labelContext *ctx) = 0;

  virtual void enterBond_id(BNGParser::Bond_idContext *ctx) = 0;
  virtual void exitBond_id(BNGParser::Bond_idContext *ctx) = 0;

  virtual void enterObservables_block(BNGParser::Observables_blockContext *ctx) = 0;
  virtual void exitObservables_block(BNGParser::Observables_blockContext *ctx) = 0;

  virtual void enterObservable_def(BNGParser::Observable_defContext *ctx) = 0;
  virtual void exitObservable_def(BNGParser::Observable_defContext *ctx) = 0;

  virtual void enterObservable_type(BNGParser::Observable_typeContext *ctx) = 0;
  virtual void exitObservable_type(BNGParser::Observable_typeContext *ctx) = 0;

  virtual void enterObservable_pattern_list(BNGParser::Observable_pattern_listContext *ctx) = 0;
  virtual void exitObservable_pattern_list(BNGParser::Observable_pattern_listContext *ctx) = 0;

  virtual void enterObservable_pattern(BNGParser::Observable_patternContext *ctx) = 0;
  virtual void exitObservable_pattern(BNGParser::Observable_patternContext *ctx) = 0;

  virtual void enterReaction_rules_block(BNGParser::Reaction_rules_blockContext *ctx) = 0;
  virtual void exitReaction_rules_block(BNGParser::Reaction_rules_blockContext *ctx) = 0;

  virtual void enterReaction_rule_def(BNGParser::Reaction_rule_defContext *ctx) = 0;
  virtual void exitReaction_rule_def(BNGParser::Reaction_rule_defContext *ctx) = 0;

  virtual void enterLabel_def(BNGParser::Label_defContext *ctx) = 0;
  virtual void exitLabel_def(BNGParser::Label_defContext *ctx) = 0;

  virtual void enterReactant_patterns(BNGParser::Reactant_patternsContext *ctx) = 0;
  virtual void exitReactant_patterns(BNGParser::Reactant_patternsContext *ctx) = 0;

  virtual void enterProduct_patterns(BNGParser::Product_patternsContext *ctx) = 0;
  virtual void exitProduct_patterns(BNGParser::Product_patternsContext *ctx) = 0;

  virtual void enterReaction_sign(BNGParser::Reaction_signContext *ctx) = 0;
  virtual void exitReaction_sign(BNGParser::Reaction_signContext *ctx) = 0;

  virtual void enterRate_law(BNGParser::Rate_lawContext *ctx) = 0;
  virtual void exitRate_law(BNGParser::Rate_lawContext *ctx) = 0;

  virtual void enterRule_modifiers(BNGParser::Rule_modifiersContext *ctx) = 0;
  virtual void exitRule_modifiers(BNGParser::Rule_modifiersContext *ctx) = 0;

  virtual void enterPattern_list(BNGParser::Pattern_listContext *ctx) = 0;
  virtual void exitPattern_list(BNGParser::Pattern_listContext *ctx) = 0;

  virtual void enterFunctions_block(BNGParser::Functions_blockContext *ctx) = 0;
  virtual void exitFunctions_block(BNGParser::Functions_blockContext *ctx) = 0;

  virtual void enterFunction_def(BNGParser::Function_defContext *ctx) = 0;
  virtual void exitFunction_def(BNGParser::Function_defContext *ctx) = 0;

  virtual void enterParam_list(BNGParser::Param_listContext *ctx) = 0;
  virtual void exitParam_list(BNGParser::Param_listContext *ctx) = 0;

  virtual void enterCompartments_block(BNGParser::Compartments_blockContext *ctx) = 0;
  virtual void exitCompartments_block(BNGParser::Compartments_blockContext *ctx) = 0;

  virtual void enterCompartment_def(BNGParser::Compartment_defContext *ctx) = 0;
  virtual void exitCompartment_def(BNGParser::Compartment_defContext *ctx) = 0;

  virtual void enterEnergy_patterns_block(BNGParser::Energy_patterns_blockContext *ctx) = 0;
  virtual void exitEnergy_patterns_block(BNGParser::Energy_patterns_blockContext *ctx) = 0;

  virtual void enterEnergy_pattern_def(BNGParser::Energy_pattern_defContext *ctx) = 0;
  virtual void exitEnergy_pattern_def(BNGParser::Energy_pattern_defContext *ctx) = 0;

  virtual void enterPopulation_maps_block(BNGParser::Population_maps_blockContext *ctx) = 0;
  virtual void exitPopulation_maps_block(BNGParser::Population_maps_blockContext *ctx) = 0;

  virtual void enterPopulation_map_def(BNGParser::Population_map_defContext *ctx) = 0;
  virtual void exitPopulation_map_def(BNGParser::Population_map_defContext *ctx) = 0;

  virtual void enterActions_block(BNGParser::Actions_blockContext *ctx) = 0;
  virtual void exitActions_block(BNGParser::Actions_blockContext *ctx) = 0;

  virtual void enterWrapped_actions_block(BNGParser::Wrapped_actions_blockContext *ctx) = 0;
  virtual void exitWrapped_actions_block(BNGParser::Wrapped_actions_blockContext *ctx) = 0;

  virtual void enterBegin_actions_block(BNGParser::Begin_actions_blockContext *ctx) = 0;
  virtual void exitBegin_actions_block(BNGParser::Begin_actions_blockContext *ctx) = 0;

  virtual void enterAction_command(BNGParser::Action_commandContext *ctx) = 0;
  virtual void exitAction_command(BNGParser::Action_commandContext *ctx) = 0;

  virtual void enterGenerate_network_cmd(BNGParser::Generate_network_cmdContext *ctx) = 0;
  virtual void exitGenerate_network_cmd(BNGParser::Generate_network_cmdContext *ctx) = 0;

  virtual void enterSimulate_cmd(BNGParser::Simulate_cmdContext *ctx) = 0;
  virtual void exitSimulate_cmd(BNGParser::Simulate_cmdContext *ctx) = 0;

  virtual void enterWrite_cmd(BNGParser::Write_cmdContext *ctx) = 0;
  virtual void exitWrite_cmd(BNGParser::Write_cmdContext *ctx) = 0;

  virtual void enterSet_cmd(BNGParser::Set_cmdContext *ctx) = 0;
  virtual void exitSet_cmd(BNGParser::Set_cmdContext *ctx) = 0;

  virtual void enterOther_action_cmd(BNGParser::Other_action_cmdContext *ctx) = 0;
  virtual void exitOther_action_cmd(BNGParser::Other_action_cmdContext *ctx) = 0;

  virtual void enterAction_args(BNGParser::Action_argsContext *ctx) = 0;
  virtual void exitAction_args(BNGParser::Action_argsContext *ctx) = 0;

  virtual void enterAction_arg_list(BNGParser::Action_arg_listContext *ctx) = 0;
  virtual void exitAction_arg_list(BNGParser::Action_arg_listContext *ctx) = 0;

  virtual void enterAction_arg(BNGParser::Action_argContext *ctx) = 0;
  virtual void exitAction_arg(BNGParser::Action_argContext *ctx) = 0;

  virtual void enterAction_arg_value(BNGParser::Action_arg_valueContext *ctx) = 0;
  virtual void exitAction_arg_value(BNGParser::Action_arg_valueContext *ctx) = 0;

  virtual void enterQuoted_string(BNGParser::Quoted_stringContext *ctx) = 0;
  virtual void exitQuoted_string(BNGParser::Quoted_stringContext *ctx) = 0;

  virtual void enterKeyword_as_value(BNGParser::Keyword_as_valueContext *ctx) = 0;
  virtual void exitKeyword_as_value(BNGParser::Keyword_as_valueContext *ctx) = 0;

  virtual void enterNested_hash_list(BNGParser::Nested_hash_listContext *ctx) = 0;
  virtual void exitNested_hash_list(BNGParser::Nested_hash_listContext *ctx) = 0;

  virtual void enterNested_hash_item(BNGParser::Nested_hash_itemContext *ctx) = 0;
  virtual void exitNested_hash_item(BNGParser::Nested_hash_itemContext *ctx) = 0;

  virtual void enterArg_name(BNGParser::Arg_nameContext *ctx) = 0;
  virtual void exitArg_name(BNGParser::Arg_nameContext *ctx) = 0;

  virtual void enterExpression_list(BNGParser::Expression_listContext *ctx) = 0;
  virtual void exitExpression_list(BNGParser::Expression_listContext *ctx) = 0;

  virtual void enterExpression(BNGParser::ExpressionContext *ctx) = 0;
  virtual void exitExpression(BNGParser::ExpressionContext *ctx) = 0;

  virtual void enterConditional_expr(BNGParser::Conditional_exprContext *ctx) = 0;
  virtual void exitConditional_expr(BNGParser::Conditional_exprContext *ctx) = 0;

  virtual void enterOr_expr(BNGParser::Or_exprContext *ctx) = 0;
  virtual void exitOr_expr(BNGParser::Or_exprContext *ctx) = 0;

  virtual void enterAnd_expr(BNGParser::And_exprContext *ctx) = 0;
  virtual void exitAnd_expr(BNGParser::And_exprContext *ctx) = 0;

  virtual void enterEquality_expr(BNGParser::Equality_exprContext *ctx) = 0;
  virtual void exitEquality_expr(BNGParser::Equality_exprContext *ctx) = 0;

  virtual void enterRelational_expr(BNGParser::Relational_exprContext *ctx) = 0;
  virtual void exitRelational_expr(BNGParser::Relational_exprContext *ctx) = 0;

  virtual void enterAdditive_expr(BNGParser::Additive_exprContext *ctx) = 0;
  virtual void exitAdditive_expr(BNGParser::Additive_exprContext *ctx) = 0;

  virtual void enterMultiplicative_expr(BNGParser::Multiplicative_exprContext *ctx) = 0;
  virtual void exitMultiplicative_expr(BNGParser::Multiplicative_exprContext *ctx) = 0;

  virtual void enterPower_expr(BNGParser::Power_exprContext *ctx) = 0;
  virtual void exitPower_expr(BNGParser::Power_exprContext *ctx) = 0;

  virtual void enterUnary_expr(BNGParser::Unary_exprContext *ctx) = 0;
  virtual void exitUnary_expr(BNGParser::Unary_exprContext *ctx) = 0;

  virtual void enterPrimary_expr(BNGParser::Primary_exprContext *ctx) = 0;
  virtual void exitPrimary_expr(BNGParser::Primary_exprContext *ctx) = 0;

  virtual void enterFunction_call(BNGParser::Function_callContext *ctx) = 0;
  virtual void exitFunction_call(BNGParser::Function_callContext *ctx) = 0;

  virtual void enterObservable_ref(BNGParser::Observable_refContext *ctx) = 0;
  virtual void exitObservable_ref(BNGParser::Observable_refContext *ctx) = 0;

  virtual void enterLiteral(BNGParser::LiteralContext *ctx) = 0;
  virtual void exitLiteral(BNGParser::LiteralContext *ctx) = 0;


};

