
// Generated from BNGParser.g4 by ANTLR 4.13.1

#pragma once


#include "antlr4-runtime.h"
#include "BNGParserListener.h"


/**
 * This class provides an empty implementation of BNGParserListener,
 * which can be extended to create a listener which only needs to handle a subset
 * of the available methods.
 */
class  BNGParserBaseListener : public BNGParserListener {
public:

  virtual void enterProg(BNGParser::ProgContext * /*ctx*/) override { }
  virtual void exitProg(BNGParser::ProgContext * /*ctx*/) override { }

  virtual void enterHeader_block(BNGParser::Header_blockContext * /*ctx*/) override { }
  virtual void exitHeader_block(BNGParser::Header_blockContext * /*ctx*/) override { }

  virtual void enterVersion_def(BNGParser::Version_defContext * /*ctx*/) override { }
  virtual void exitVersion_def(BNGParser::Version_defContext * /*ctx*/) override { }

  virtual void enterSubstance_def(BNGParser::Substance_defContext * /*ctx*/) override { }
  virtual void exitSubstance_def(BNGParser::Substance_defContext * /*ctx*/) override { }

  virtual void enterSet_option(BNGParser::Set_optionContext * /*ctx*/) override { }
  virtual void exitSet_option(BNGParser::Set_optionContext * /*ctx*/) override { }

  virtual void enterSet_model_name(BNGParser::Set_model_nameContext * /*ctx*/) override { }
  virtual void exitSet_model_name(BNGParser::Set_model_nameContext * /*ctx*/) override { }

  virtual void enterProgram_block(BNGParser::Program_blockContext * /*ctx*/) override { }
  virtual void exitProgram_block(BNGParser::Program_blockContext * /*ctx*/) override { }

  virtual void enterParameters_block(BNGParser::Parameters_blockContext * /*ctx*/) override { }
  virtual void exitParameters_block(BNGParser::Parameters_blockContext * /*ctx*/) override { }

  virtual void enterParameter_def(BNGParser::Parameter_defContext * /*ctx*/) override { }
  virtual void exitParameter_def(BNGParser::Parameter_defContext * /*ctx*/) override { }

  virtual void enterParam_name(BNGParser::Param_nameContext * /*ctx*/) override { }
  virtual void exitParam_name(BNGParser::Param_nameContext * /*ctx*/) override { }

  virtual void enterMolecule_types_block(BNGParser::Molecule_types_blockContext * /*ctx*/) override { }
  virtual void exitMolecule_types_block(BNGParser::Molecule_types_blockContext * /*ctx*/) override { }

  virtual void enterMolecule_type_def(BNGParser::Molecule_type_defContext * /*ctx*/) override { }
  virtual void exitMolecule_type_def(BNGParser::Molecule_type_defContext * /*ctx*/) override { }

  virtual void enterMolecule_def(BNGParser::Molecule_defContext * /*ctx*/) override { }
  virtual void exitMolecule_def(BNGParser::Molecule_defContext * /*ctx*/) override { }

  virtual void enterMolecule_attributes(BNGParser::Molecule_attributesContext * /*ctx*/) override { }
  virtual void exitMolecule_attributes(BNGParser::Molecule_attributesContext * /*ctx*/) override { }

  virtual void enterComponent_def_list(BNGParser::Component_def_listContext * /*ctx*/) override { }
  virtual void exitComponent_def_list(BNGParser::Component_def_listContext * /*ctx*/) override { }

  virtual void enterComponent_def(BNGParser::Component_defContext * /*ctx*/) override { }
  virtual void exitComponent_def(BNGParser::Component_defContext * /*ctx*/) override { }

  virtual void enterKeyword_as_component_name(BNGParser::Keyword_as_component_nameContext * /*ctx*/) override { }
  virtual void exitKeyword_as_component_name(BNGParser::Keyword_as_component_nameContext * /*ctx*/) override { }

  virtual void enterKeyword_as_mol_name(BNGParser::Keyword_as_mol_nameContext * /*ctx*/) override { }
  virtual void exitKeyword_as_mol_name(BNGParser::Keyword_as_mol_nameContext * /*ctx*/) override { }

  virtual void enterState_list(BNGParser::State_listContext * /*ctx*/) override { }
  virtual void exitState_list(BNGParser::State_listContext * /*ctx*/) override { }

  virtual void enterState_name(BNGParser::State_nameContext * /*ctx*/) override { }
  virtual void exitState_name(BNGParser::State_nameContext * /*ctx*/) override { }

  virtual void enterSeed_species_block(BNGParser::Seed_species_blockContext * /*ctx*/) override { }
  virtual void exitSeed_species_block(BNGParser::Seed_species_blockContext * /*ctx*/) override { }

  virtual void enterSeed_species_def(BNGParser::Seed_species_defContext * /*ctx*/) override { }
  virtual void exitSeed_species_def(BNGParser::Seed_species_defContext * /*ctx*/) override { }

  virtual void enterSpecies_def(BNGParser::Species_defContext * /*ctx*/) override { }
  virtual void exitSpecies_def(BNGParser::Species_defContext * /*ctx*/) override { }

  virtual void enterMolecule_compartment(BNGParser::Molecule_compartmentContext * /*ctx*/) override { }
  virtual void exitMolecule_compartment(BNGParser::Molecule_compartmentContext * /*ctx*/) override { }

  virtual void enterMolecule_pattern(BNGParser::Molecule_patternContext * /*ctx*/) override { }
  virtual void exitMolecule_pattern(BNGParser::Molecule_patternContext * /*ctx*/) override { }

  virtual void enterPattern_bond_wildcard(BNGParser::Pattern_bond_wildcardContext * /*ctx*/) override { }
  virtual void exitPattern_bond_wildcard(BNGParser::Pattern_bond_wildcardContext * /*ctx*/) override { }

  virtual void enterMolecule_tag(BNGParser::Molecule_tagContext * /*ctx*/) override { }
  virtual void exitMolecule_tag(BNGParser::Molecule_tagContext * /*ctx*/) override { }

  virtual void enterComponent_pattern_list(BNGParser::Component_pattern_listContext * /*ctx*/) override { }
  virtual void exitComponent_pattern_list(BNGParser::Component_pattern_listContext * /*ctx*/) override { }

  virtual void enterComponent_pattern(BNGParser::Component_patternContext * /*ctx*/) override { }
  virtual void exitComponent_pattern(BNGParser::Component_patternContext * /*ctx*/) override { }

  virtual void enterState_value(BNGParser::State_valueContext * /*ctx*/) override { }
  virtual void exitState_value(BNGParser::State_valueContext * /*ctx*/) override { }

  virtual void enterBond_spec(BNGParser::Bond_specContext * /*ctx*/) override { }
  virtual void exitBond_spec(BNGParser::Bond_specContext * /*ctx*/) override { }

  virtual void enterBond_id(BNGParser::Bond_idContext * /*ctx*/) override { }
  virtual void exitBond_id(BNGParser::Bond_idContext * /*ctx*/) override { }

  virtual void enterObservables_block(BNGParser::Observables_blockContext * /*ctx*/) override { }
  virtual void exitObservables_block(BNGParser::Observables_blockContext * /*ctx*/) override { }

  virtual void enterObservable_def(BNGParser::Observable_defContext * /*ctx*/) override { }
  virtual void exitObservable_def(BNGParser::Observable_defContext * /*ctx*/) override { }

  virtual void enterObservable_type(BNGParser::Observable_typeContext * /*ctx*/) override { }
  virtual void exitObservable_type(BNGParser::Observable_typeContext * /*ctx*/) override { }

  virtual void enterObservable_pattern_list(BNGParser::Observable_pattern_listContext * /*ctx*/) override { }
  virtual void exitObservable_pattern_list(BNGParser::Observable_pattern_listContext * /*ctx*/) override { }

  virtual void enterObservable_pattern(BNGParser::Observable_patternContext * /*ctx*/) override { }
  virtual void exitObservable_pattern(BNGParser::Observable_patternContext * /*ctx*/) override { }

  virtual void enterReaction_rules_block(BNGParser::Reaction_rules_blockContext * /*ctx*/) override { }
  virtual void exitReaction_rules_block(BNGParser::Reaction_rules_blockContext * /*ctx*/) override { }

  virtual void enterReaction_rule_def(BNGParser::Reaction_rule_defContext * /*ctx*/) override { }
  virtual void exitReaction_rule_def(BNGParser::Reaction_rule_defContext * /*ctx*/) override { }

  virtual void enterLabel_def(BNGParser::Label_defContext * /*ctx*/) override { }
  virtual void exitLabel_def(BNGParser::Label_defContext * /*ctx*/) override { }

  virtual void enterReactant_patterns(BNGParser::Reactant_patternsContext * /*ctx*/) override { }
  virtual void exitReactant_patterns(BNGParser::Reactant_patternsContext * /*ctx*/) override { }

  virtual void enterProduct_patterns(BNGParser::Product_patternsContext * /*ctx*/) override { }
  virtual void exitProduct_patterns(BNGParser::Product_patternsContext * /*ctx*/) override { }

  virtual void enterReaction_sign(BNGParser::Reaction_signContext * /*ctx*/) override { }
  virtual void exitReaction_sign(BNGParser::Reaction_signContext * /*ctx*/) override { }

  virtual void enterRate_law(BNGParser::Rate_lawContext * /*ctx*/) override { }
  virtual void exitRate_law(BNGParser::Rate_lawContext * /*ctx*/) override { }

  virtual void enterRule_modifiers(BNGParser::Rule_modifiersContext * /*ctx*/) override { }
  virtual void exitRule_modifiers(BNGParser::Rule_modifiersContext * /*ctx*/) override { }

  virtual void enterPattern_list(BNGParser::Pattern_listContext * /*ctx*/) override { }
  virtual void exitPattern_list(BNGParser::Pattern_listContext * /*ctx*/) override { }

  virtual void enterFunctions_block(BNGParser::Functions_blockContext * /*ctx*/) override { }
  virtual void exitFunctions_block(BNGParser::Functions_blockContext * /*ctx*/) override { }

  virtual void enterFunction_def(BNGParser::Function_defContext * /*ctx*/) override { }
  virtual void exitFunction_def(BNGParser::Function_defContext * /*ctx*/) override { }

  virtual void enterParam_list(BNGParser::Param_listContext * /*ctx*/) override { }
  virtual void exitParam_list(BNGParser::Param_listContext * /*ctx*/) override { }

  virtual void enterCompartments_block(BNGParser::Compartments_blockContext * /*ctx*/) override { }
  virtual void exitCompartments_block(BNGParser::Compartments_blockContext * /*ctx*/) override { }

  virtual void enterCompartment_def(BNGParser::Compartment_defContext * /*ctx*/) override { }
  virtual void exitCompartment_def(BNGParser::Compartment_defContext * /*ctx*/) override { }

  virtual void enterEnergy_patterns_block(BNGParser::Energy_patterns_blockContext * /*ctx*/) override { }
  virtual void exitEnergy_patterns_block(BNGParser::Energy_patterns_blockContext * /*ctx*/) override { }

  virtual void enterEnergy_pattern_def(BNGParser::Energy_pattern_defContext * /*ctx*/) override { }
  virtual void exitEnergy_pattern_def(BNGParser::Energy_pattern_defContext * /*ctx*/) override { }

  virtual void enterPopulation_maps_block(BNGParser::Population_maps_blockContext * /*ctx*/) override { }
  virtual void exitPopulation_maps_block(BNGParser::Population_maps_blockContext * /*ctx*/) override { }

  virtual void enterPopulation_map_def(BNGParser::Population_map_defContext * /*ctx*/) override { }
  virtual void exitPopulation_map_def(BNGParser::Population_map_defContext * /*ctx*/) override { }

  virtual void enterActions_block(BNGParser::Actions_blockContext * /*ctx*/) override { }
  virtual void exitActions_block(BNGParser::Actions_blockContext * /*ctx*/) override { }

  virtual void enterWrapped_actions_block(BNGParser::Wrapped_actions_blockContext * /*ctx*/) override { }
  virtual void exitWrapped_actions_block(BNGParser::Wrapped_actions_blockContext * /*ctx*/) override { }

  virtual void enterBegin_actions_block(BNGParser::Begin_actions_blockContext * /*ctx*/) override { }
  virtual void exitBegin_actions_block(BNGParser::Begin_actions_blockContext * /*ctx*/) override { }

  virtual void enterAction_command(BNGParser::Action_commandContext * /*ctx*/) override { }
  virtual void exitAction_command(BNGParser::Action_commandContext * /*ctx*/) override { }

  virtual void enterGenerate_network_cmd(BNGParser::Generate_network_cmdContext * /*ctx*/) override { }
  virtual void exitGenerate_network_cmd(BNGParser::Generate_network_cmdContext * /*ctx*/) override { }

  virtual void enterSimulate_cmd(BNGParser::Simulate_cmdContext * /*ctx*/) override { }
  virtual void exitSimulate_cmd(BNGParser::Simulate_cmdContext * /*ctx*/) override { }

  virtual void enterWrite_cmd(BNGParser::Write_cmdContext * /*ctx*/) override { }
  virtual void exitWrite_cmd(BNGParser::Write_cmdContext * /*ctx*/) override { }

  virtual void enterSet_cmd(BNGParser::Set_cmdContext * /*ctx*/) override { }
  virtual void exitSet_cmd(BNGParser::Set_cmdContext * /*ctx*/) override { }

  virtual void enterOther_action_cmd(BNGParser::Other_action_cmdContext * /*ctx*/) override { }
  virtual void exitOther_action_cmd(BNGParser::Other_action_cmdContext * /*ctx*/) override { }

  virtual void enterAction_args(BNGParser::Action_argsContext * /*ctx*/) override { }
  virtual void exitAction_args(BNGParser::Action_argsContext * /*ctx*/) override { }

  virtual void enterAction_arg_list(BNGParser::Action_arg_listContext * /*ctx*/) override { }
  virtual void exitAction_arg_list(BNGParser::Action_arg_listContext * /*ctx*/) override { }

  virtual void enterAction_arg(BNGParser::Action_argContext * /*ctx*/) override { }
  virtual void exitAction_arg(BNGParser::Action_argContext * /*ctx*/) override { }

  virtual void enterAction_arg_value(BNGParser::Action_arg_valueContext * /*ctx*/) override { }
  virtual void exitAction_arg_value(BNGParser::Action_arg_valueContext * /*ctx*/) override { }

  virtual void enterKeyword_as_value(BNGParser::Keyword_as_valueContext * /*ctx*/) override { }
  virtual void exitKeyword_as_value(BNGParser::Keyword_as_valueContext * /*ctx*/) override { }

  virtual void enterNested_hash_list(BNGParser::Nested_hash_listContext * /*ctx*/) override { }
  virtual void exitNested_hash_list(BNGParser::Nested_hash_listContext * /*ctx*/) override { }

  virtual void enterNested_hash_item(BNGParser::Nested_hash_itemContext * /*ctx*/) override { }
  virtual void exitNested_hash_item(BNGParser::Nested_hash_itemContext * /*ctx*/) override { }

  virtual void enterArg_name(BNGParser::Arg_nameContext * /*ctx*/) override { }
  virtual void exitArg_name(BNGParser::Arg_nameContext * /*ctx*/) override { }

  virtual void enterExpression_list(BNGParser::Expression_listContext * /*ctx*/) override { }
  virtual void exitExpression_list(BNGParser::Expression_listContext * /*ctx*/) override { }

  virtual void enterExpression(BNGParser::ExpressionContext * /*ctx*/) override { }
  virtual void exitExpression(BNGParser::ExpressionContext * /*ctx*/) override { }

  virtual void enterConditional_expr(BNGParser::Conditional_exprContext * /*ctx*/) override { }
  virtual void exitConditional_expr(BNGParser::Conditional_exprContext * /*ctx*/) override { }

  virtual void enterOr_expr(BNGParser::Or_exprContext * /*ctx*/) override { }
  virtual void exitOr_expr(BNGParser::Or_exprContext * /*ctx*/) override { }

  virtual void enterAnd_expr(BNGParser::And_exprContext * /*ctx*/) override { }
  virtual void exitAnd_expr(BNGParser::And_exprContext * /*ctx*/) override { }

  virtual void enterEquality_expr(BNGParser::Equality_exprContext * /*ctx*/) override { }
  virtual void exitEquality_expr(BNGParser::Equality_exprContext * /*ctx*/) override { }

  virtual void enterRelational_expr(BNGParser::Relational_exprContext * /*ctx*/) override { }
  virtual void exitRelational_expr(BNGParser::Relational_exprContext * /*ctx*/) override { }

  virtual void enterAdditive_expr(BNGParser::Additive_exprContext * /*ctx*/) override { }
  virtual void exitAdditive_expr(BNGParser::Additive_exprContext * /*ctx*/) override { }

  virtual void enterMultiplicative_expr(BNGParser::Multiplicative_exprContext * /*ctx*/) override { }
  virtual void exitMultiplicative_expr(BNGParser::Multiplicative_exprContext * /*ctx*/) override { }

  virtual void enterPower_expr(BNGParser::Power_exprContext * /*ctx*/) override { }
  virtual void exitPower_expr(BNGParser::Power_exprContext * /*ctx*/) override { }

  virtual void enterUnary_expr(BNGParser::Unary_exprContext * /*ctx*/) override { }
  virtual void exitUnary_expr(BNGParser::Unary_exprContext * /*ctx*/) override { }

  virtual void enterPrimary_expr(BNGParser::Primary_exprContext * /*ctx*/) override { }
  virtual void exitPrimary_expr(BNGParser::Primary_exprContext * /*ctx*/) override { }

  virtual void enterFunction_call(BNGParser::Function_callContext * /*ctx*/) override { }
  virtual void exitFunction_call(BNGParser::Function_callContext * /*ctx*/) override { }

  virtual void enterObservable_ref(BNGParser::Observable_refContext * /*ctx*/) override { }
  virtual void exitObservable_ref(BNGParser::Observable_refContext * /*ctx*/) override { }

  virtual void enterLiteral(BNGParser::LiteralContext * /*ctx*/) override { }
  virtual void exitLiteral(BNGParser::LiteralContext * /*ctx*/) override { }


  virtual void enterEveryRule(antlr4::ParserRuleContext * /*ctx*/) override { }
  virtual void exitEveryRule(antlr4::ParserRuleContext * /*ctx*/) override { }
  virtual void visitTerminal(antlr4::tree::TerminalNode * /*node*/) override { }
  virtual void visitErrorNode(antlr4::tree::ErrorNode * /*node*/) override { }

};

