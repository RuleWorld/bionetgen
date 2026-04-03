
// Generated from src/parser/BNGParser.g4 by ANTLR 4.13.1

#pragma once


#include "antlr4-runtime.h"




class  BNGParser : public antlr4::Parser {
public:
  enum {
    LINE_COMMENT = 1, LB = 2, WS = 3, BEGIN = 4, END = 5, MODEL = 6, PARAMETERS = 7, 
    COMPARTMENTS = 8, MOLECULE = 9, MOLECULES = 10, COUNTER = 11, TYPES = 12, 
    SEED = 13, SPECIES = 14, OBSERVABLES = 15, FUNCTIONS = 16, REACTION = 17, 
    REACTIONS = 18, RULES = 19, REACTION_RULES = 20, MOLECULE_TYPES = 21, 
    GROUPS = 22, ACTIONS = 23, POPULATION = 24, MAPS = 25, ENERGY = 26, 
    PATTERNS = 27, MOLECULAR = 28, MATCHONCE = 29, DELETEMOLECULES = 30, 
    MOVECONNECTED = 31, INCLUDE_REACTANTS = 32, INCLUDE_PRODUCTS = 33, EXCLUDE_REACTANTS = 34, 
    EXCLUDE_PRODUCTS = 35, TOTALRATE = 36, VERSION = 37, SET_OPTION = 38, 
    SET_MODEL_NAME = 39, SUBSTANCEUNITS = 40, PREFIX = 41, SUFFIX = 42, 
    GENERATENETWORK = 43, OVERWRITE = 44, MAX_AGG = 45, MAX_ITER = 46, MAX_STOICH = 47, 
    PRINT_ITER = 48, CHECK_ISO = 49, GENERATEHYBRIDMODEL = 50, SAFE = 51, 
    EXECUTE = 52, SIMULATE = 53, METHOD = 54, ODE = 55, SSA = 56, PLA = 57, 
    NF = 58, VERBOSE = 59, NETFILE = 60, ARGFILE = 61, CONTINUE = 62, T_START = 63, 
    T_END = 64, N_STEPS = 65, N_OUTPUT_STEPS = 66, MAX_SIM_STEPS = 67, OUTPUT_STEP_INTERVAL = 68, 
    SAMPLE_TIMES = 69, SAVE_PROGRESS = 70, PRINT_CDAT = 71, PRINT_FUNCTIONS = 72, 
    PRINT_NET = 73, PRINT_END = 74, STOP_IF = 75, PRINT_ON_STOP = 76, SIMULATE_ODE = 77, 
    ATOL = 78, RTOL = 79, STEADY_STATE = 80, SPARSE = 81, SIMULATE_SSA = 82, 
    SIMULATE_PLA = 83, PLA_CONFIG = 84, PLA_OUTPUT = 85, SIMULATE_NF = 86, 
    SIMULATE_RM = 87, PARAM = 88, COMPLEX = 89, GET_FINAL_STATE = 90, GML = 91, 
    NOCSLF = 92, NOTF = 93, BINARY_OUTPUT = 94, UTL = 95, EQUIL = 96, PARAMETER_SCAN = 97, 
    BIFURCATE = 98, PARAMETER = 99, PAR_MIN = 100, PAR_MAX = 101, N_SCAN_PTS = 102, 
    LOG_SCALE = 103, RESET_CONC = 104, READFILE = 105, FILE = 106, ATOMIZE = 107, 
    BLOCKS = 108, SKIPACTIONS = 109, VISUALIZE = 110, TYPE = 111, BACKGROUND = 112, 
    COLLAPSE = 113, OPTS = 114, WRITESSC = 115, WRITESSCCFG = 116, FORMAT = 117, 
    WRITEFILE = 118, WRITEMODEL = 119, WRITEXML = 120, WRITENETWORK = 121, 
    WRITESBML = 122, WRITEMDL = 123, WRITELATEX = 124, INCLUDE_MODEL = 125, 
    INCLUDE_NETWORK = 126, PRETTY_FORMATTING = 127, EVALUATE_EXPRESSIONS = 128, 
    TEXTREACTION = 129, TEXTSPECIES = 130, WRITEMFILE = 131, WRITEMEXFILE = 132, 
    BDF = 133, MAX_STEP = 134, MAXORDER = 135, STATS = 136, MAX_NUM_STEPS = 137, 
    MAX_ERR_TEST_FAILS = 138, MAX_CONV_FAILS = 139, STIFF = 140, SETCONCENTRATION = 141, 
    ADDCONCENTRATION = 142, SAVECONCENTRATIONS = 143, RESETCONCENTRATIONS = 144, 
    SETPARAMETER = 145, SAVEPARAMETERS = 146, RESETPARAMETERS = 147, QUIT = 148, 
    TRUE = 149, FALSE = 150, SAT = 151, MM = 152, HILL = 153, ARRHENIUS = 154, 
    MRATIO = 155, TFUN = 156, FUNCTIONPRODUCT = 157, PRIORITY = 158, IF = 159, 
    EXP = 160, LN = 161, LOG10 = 162, LOG2 = 163, SQRT = 164, RINT = 165, 
    ABS = 166, SIN = 167, COS = 168, TAN = 169, ASIN = 170, ACOS = 171, 
    ATAN = 172, SINH = 173, COSH = 174, TANH = 175, ASINH = 176, ACOSH = 177, 
    ATANH = 178, PI = 179, EULERIAN = 180, MIN = 181, MAX = 182, SUM = 183, 
    AVG = 184, TIME = 185, FLOAT = 186, INT = 187, STRING = 188, QUOTED_STRING = 189, 
    SINGLE_QUOTED_STRING = 190, SEMI = 191, COLON = 192, LSBRACKET = 193, 
    RSBRACKET = 194, LBRACKET = 195, RBRACKET = 196, COMMA = 197, DOT = 198, 
    LPAREN = 199, RPAREN = 200, UNI_REACTION_SIGN = 201, BI_REACTION_SIGN = 202, 
    DOLLAR = 203, TILDE = 204, AT = 205, GTE = 206, GT = 207, LTE = 208, 
    LT = 209, ASSIGNS = 210, EQUALS = 211, NOT_EQUALS = 212, BECOMES = 213, 
    LOGICAL_AND = 214, LOGICAL_OR = 215, DIV = 216, TIMES = 217, MINUS = 218, 
    PLUS = 219, POWER = 220, MOD = 221, PIPE = 222, QMARK = 223, EMARK = 224, 
    AMPERSAND = 225, VERSION_NUMBER = 226, ULB = 227
  };

  enum {
    RuleProg = 0, RuleHeader_block = 1, RuleVersion_def = 2, RuleSubstance_def = 3, 
    RuleSet_option = 4, RuleSet_model_name = 5, RuleProgram_block = 6, RuleParameters_block = 7, 
    RuleParameter_def = 8, RuleParam_name = 9, RuleMolecule_types_block = 10, 
    RuleMolecule_type_def = 11, RuleMolecule_def = 12, RuleMolecule_attributes = 13, 
    RuleComponent_def_list = 14, RuleComponent_def = 15, RuleKeyword_as_component_name = 16, 
    RuleKeyword_as_mol_name = 17, RuleState_list = 18, RuleState_name = 19, 
    RuleSeed_species_block = 20, RuleSeed_species_def = 21, RuleSpecies_def = 22, 
    RuleMolecule_compartment = 23, RuleMolecule_pattern = 24, RuleScope_prefix = 25, 
    RulePattern_bond_wildcard = 26, RuleMolecule_tag = 27, RuleComponent_pattern_list = 28, 
    RuleComponent_pattern = 29, RuleState_value = 30, RuleBond_spec = 31, 
    RuleComponent_label = 32, RuleBond_id = 33, RuleObservables_block = 34, 
    RuleObservable_def = 35, RuleObservable_type = 36, RuleObservable_pattern_list = 37, 
    RuleObservable_pattern = 38, RuleReaction_rules_block = 39, RuleReaction_rule_def = 40, 
    RuleLabel_def = 41, RuleReactant_patterns = 42, RuleProduct_patterns = 43, 
    RuleReaction_sign = 44, RuleRate_law = 45, RuleRule_modifiers = 46, 
    RulePattern_list = 47, RuleFunctions_block = 48, RuleFunction_def = 49, 
    RuleParam_list = 50, RuleCompartments_block = 51, RuleCompartment_def = 52, 
    RuleEnergy_patterns_block = 53, RuleEnergy_pattern_def = 54, RulePopulation_maps_block = 55, 
    RulePopulation_map_def = 56, RuleActions_block = 57, RuleWrapped_actions_block = 58, 
    RuleBegin_actions_block = 59, RuleAction_command = 60, RuleGenerate_network_cmd = 61, 
    RuleSimulate_cmd = 62, RuleWrite_cmd = 63, RuleSet_cmd = 64, RuleOther_action_cmd = 65, 
    RuleAction_args = 66, RuleAction_arg_list = 67, RuleAction_arg = 68, 
    RuleAction_arg_value = 69, RuleQuoted_string = 70, RuleKeyword_as_value = 71, 
    RuleNested_hash_list = 72, RuleNested_hash_item = 73, RuleArg_name = 74, 
    RuleExpression_list = 75, RuleExpression = 76, RuleConditional_expr = 77, 
    RuleOr_expr = 78, RuleAnd_expr = 79, RuleEquality_expr = 80, RuleRelational_expr = 81, 
    RuleAdditive_expr = 82, RuleMultiplicative_expr = 83, RulePower_expr = 84, 
    RuleUnary_expr = 85, RulePrimary_expr = 86, RuleFunction_call = 87, 
    RuleObservable_ref = 88, RuleLiteral = 89
  };

  explicit BNGParser(antlr4::TokenStream *input);

  BNGParser(antlr4::TokenStream *input, const antlr4::atn::ParserATNSimulatorOptions &options);

  ~BNGParser() override;

  std::string getGrammarFileName() const override;

  const antlr4::atn::ATN& getATN() const override;

  const std::vector<std::string>& getRuleNames() const override;

  const antlr4::dfa::Vocabulary& getVocabulary() const override;

  antlr4::atn::SerializedATNView getSerializedATN() const override;


  class ProgContext;
  class Header_blockContext;
  class Version_defContext;
  class Substance_defContext;
  class Set_optionContext;
  class Set_model_nameContext;
  class Program_blockContext;
  class Parameters_blockContext;
  class Parameter_defContext;
  class Param_nameContext;
  class Molecule_types_blockContext;
  class Molecule_type_defContext;
  class Molecule_defContext;
  class Molecule_attributesContext;
  class Component_def_listContext;
  class Component_defContext;
  class Keyword_as_component_nameContext;
  class Keyword_as_mol_nameContext;
  class State_listContext;
  class State_nameContext;
  class Seed_species_blockContext;
  class Seed_species_defContext;
  class Species_defContext;
  class Molecule_compartmentContext;
  class Molecule_patternContext;
  class Scope_prefixContext;
  class Pattern_bond_wildcardContext;
  class Molecule_tagContext;
  class Component_pattern_listContext;
  class Component_patternContext;
  class State_valueContext;
  class Bond_specContext;
  class Component_labelContext;
  class Bond_idContext;
  class Observables_blockContext;
  class Observable_defContext;
  class Observable_typeContext;
  class Observable_pattern_listContext;
  class Observable_patternContext;
  class Reaction_rules_blockContext;
  class Reaction_rule_defContext;
  class Label_defContext;
  class Reactant_patternsContext;
  class Product_patternsContext;
  class Reaction_signContext;
  class Rate_lawContext;
  class Rule_modifiersContext;
  class Pattern_listContext;
  class Functions_blockContext;
  class Function_defContext;
  class Param_listContext;
  class Compartments_blockContext;
  class Compartment_defContext;
  class Energy_patterns_blockContext;
  class Energy_pattern_defContext;
  class Population_maps_blockContext;
  class Population_map_defContext;
  class Actions_blockContext;
  class Wrapped_actions_blockContext;
  class Begin_actions_blockContext;
  class Action_commandContext;
  class Generate_network_cmdContext;
  class Simulate_cmdContext;
  class Write_cmdContext;
  class Set_cmdContext;
  class Other_action_cmdContext;
  class Action_argsContext;
  class Action_arg_listContext;
  class Action_argContext;
  class Action_arg_valueContext;
  class Quoted_stringContext;
  class Keyword_as_valueContext;
  class Nested_hash_listContext;
  class Nested_hash_itemContext;
  class Arg_nameContext;
  class Expression_listContext;
  class ExpressionContext;
  class Conditional_exprContext;
  class Or_exprContext;
  class And_exprContext;
  class Equality_exprContext;
  class Relational_exprContext;
  class Additive_exprContext;
  class Multiplicative_exprContext;
  class Power_exprContext;
  class Unary_exprContext;
  class Primary_exprContext;
  class Function_callContext;
  class Observable_refContext;
  class LiteralContext; 

  class  ProgContext : public antlr4::ParserRuleContext {
  public:
    ProgContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *EOF();
    std::vector<antlr4::tree::TerminalNode *> LB();
    antlr4::tree::TerminalNode* LB(size_t i);
    std::vector<Header_blockContext *> header_block();
    Header_blockContext* header_block(size_t i);
    std::vector<Action_commandContext *> action_command();
    Action_commandContext* action_command(size_t i);
    Wrapped_actions_blockContext *wrapped_actions_block();
    Actions_blockContext *actions_block();
    antlr4::tree::TerminalNode *BEGIN();
    std::vector<antlr4::tree::TerminalNode *> MODEL();
    antlr4::tree::TerminalNode* MODEL(size_t i);
    antlr4::tree::TerminalNode *END();
    std::vector<Program_blockContext *> program_block();
    Program_blockContext* program_block(size_t i);


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  ProgContext* prog();

  class  Header_blockContext : public antlr4::ParserRuleContext {
  public:
    Header_blockContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    Version_defContext *version_def();
    Substance_defContext *substance_def();
    Set_optionContext *set_option();
    Set_model_nameContext *set_model_name();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  Header_blockContext* header_block();

  class  Version_defContext : public antlr4::ParserRuleContext {
  public:
    Version_defContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *VERSION();
    antlr4::tree::TerminalNode *LPAREN();
    Quoted_stringContext *quoted_string();
    antlr4::tree::TerminalNode *RPAREN();
    antlr4::tree::TerminalNode *SEMI();
    std::vector<antlr4::tree::TerminalNode *> LB();
    antlr4::tree::TerminalNode* LB(size_t i);


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  Version_defContext* version_def();

  class  Substance_defContext : public antlr4::ParserRuleContext {
  public:
    Substance_defContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *SUBSTANCEUNITS();
    antlr4::tree::TerminalNode *LPAREN();
    Quoted_stringContext *quoted_string();
    antlr4::tree::TerminalNode *RPAREN();
    antlr4::tree::TerminalNode *SEMI();
    std::vector<antlr4::tree::TerminalNode *> LB();
    antlr4::tree::TerminalNode* LB(size_t i);


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  Substance_defContext* substance_def();

  class  Set_optionContext : public antlr4::ParserRuleContext {
  public:
    Set_optionContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *SET_OPTION();
    antlr4::tree::TerminalNode *LPAREN();
    std::vector<Quoted_stringContext *> quoted_string();
    Quoted_stringContext* quoted_string(size_t i);
    std::vector<antlr4::tree::TerminalNode *> COMMA();
    antlr4::tree::TerminalNode* COMMA(size_t i);
    std::vector<Action_arg_valueContext *> action_arg_value();
    Action_arg_valueContext* action_arg_value(size_t i);
    antlr4::tree::TerminalNode *RPAREN();
    antlr4::tree::TerminalNode *SEMI();
    std::vector<antlr4::tree::TerminalNode *> LB();
    antlr4::tree::TerminalNode* LB(size_t i);


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  Set_optionContext* set_option();

  class  Set_model_nameContext : public antlr4::ParserRuleContext {
  public:
    Set_model_nameContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *SET_MODEL_NAME();
    antlr4::tree::TerminalNode *LPAREN();
    Quoted_stringContext *quoted_string();
    antlr4::tree::TerminalNode *RPAREN();
    antlr4::tree::TerminalNode *SEMI();
    std::vector<antlr4::tree::TerminalNode *> LB();
    antlr4::tree::TerminalNode* LB(size_t i);


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  Set_model_nameContext* set_model_name();

  class  Program_blockContext : public antlr4::ParserRuleContext {
  public:
    Program_blockContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    Parameters_blockContext *parameters_block();
    Molecule_types_blockContext *molecule_types_block();
    Seed_species_blockContext *seed_species_block();
    Observables_blockContext *observables_block();
    Reaction_rules_blockContext *reaction_rules_block();
    Functions_blockContext *functions_block();
    Compartments_blockContext *compartments_block();
    Energy_patterns_blockContext *energy_patterns_block();
    Population_maps_blockContext *population_maps_block();
    Wrapped_actions_blockContext *wrapped_actions_block();
    Begin_actions_blockContext *begin_actions_block();
    Set_optionContext *set_option();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  Program_blockContext* program_block();

  class  Parameters_blockContext : public antlr4::ParserRuleContext {
  public:
    Parameters_blockContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *BEGIN();
    std::vector<antlr4::tree::TerminalNode *> PARAMETERS();
    antlr4::tree::TerminalNode* PARAMETERS(size_t i);
    antlr4::tree::TerminalNode *END();
    std::vector<antlr4::tree::TerminalNode *> LB();
    antlr4::tree::TerminalNode* LB(size_t i);
    std::vector<Parameter_defContext *> parameter_def();
    Parameter_defContext* parameter_def(size_t i);


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  Parameters_blockContext* parameters_block();

  class  Parameter_defContext : public antlr4::ParserRuleContext {
  public:
    Parameter_defContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    std::vector<Param_nameContext *> param_name();
    Param_nameContext* param_name(size_t i);
    antlr4::tree::TerminalNode *INT();
    antlr4::tree::TerminalNode *COLON();
    antlr4::tree::TerminalNode *BECOMES();
    ExpressionContext *expression();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  Parameter_defContext* parameter_def();

  class  Param_nameContext : public antlr4::ParserRuleContext {
  public:
    Param_nameContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *STRING();
    Arg_nameContext *arg_name();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  Param_nameContext* param_name();

  class  Molecule_types_blockContext : public antlr4::ParserRuleContext {
  public:
    Molecule_types_blockContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *BEGIN();
    std::vector<antlr4::tree::TerminalNode *> MOLECULE();
    antlr4::tree::TerminalNode* MOLECULE(size_t i);
    std::vector<antlr4::tree::TerminalNode *> TYPES();
    antlr4::tree::TerminalNode* TYPES(size_t i);
    antlr4::tree::TerminalNode *END();
    std::vector<antlr4::tree::TerminalNode *> LB();
    antlr4::tree::TerminalNode* LB(size_t i);
    std::vector<Molecule_type_defContext *> molecule_type_def();
    Molecule_type_defContext* molecule_type_def(size_t i);
    std::vector<antlr4::tree::TerminalNode *> MOLECULE_TYPES();
    antlr4::tree::TerminalNode* MOLECULE_TYPES(size_t i);


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  Molecule_types_blockContext* molecule_types_block();

  class  Molecule_type_defContext : public antlr4::ParserRuleContext {
  public:
    Molecule_type_defContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    Molecule_defContext *molecule_def();
    antlr4::tree::TerminalNode *STRING();
    antlr4::tree::TerminalNode *COLON();
    antlr4::tree::TerminalNode *POPULATION();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  Molecule_type_defContext* molecule_type_def();

  class  Molecule_defContext : public antlr4::ParserRuleContext {
  public:
    Molecule_defContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *STRING();
    Keyword_as_mol_nameContext *keyword_as_mol_name();
    antlr4::tree::TerminalNode *LPAREN();
    antlr4::tree::TerminalNode *RPAREN();
    Molecule_attributesContext *molecule_attributes();
    Component_def_listContext *component_def_list();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  Molecule_defContext* molecule_def();

  class  Molecule_attributesContext : public antlr4::ParserRuleContext {
  public:
    Molecule_attributesContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *LBRACKET();
    antlr4::tree::TerminalNode *RBRACKET();
    Action_arg_listContext *action_arg_list();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  Molecule_attributesContext* molecule_attributes();

  class  Component_def_listContext : public antlr4::ParserRuleContext {
  public:
    Component_def_listContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    std::vector<Component_defContext *> component_def();
    Component_defContext* component_def(size_t i);
    std::vector<antlr4::tree::TerminalNode *> COMMA();
    antlr4::tree::TerminalNode* COMMA(size_t i);


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  Component_def_listContext* component_def_list();

  class  Component_defContext : public antlr4::ParserRuleContext {
  public:
    Component_defContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *STRING();
    antlr4::tree::TerminalNode *INT();
    Keyword_as_component_nameContext *keyword_as_component_name();
    antlr4::tree::TerminalNode *TILDE();
    State_listContext *state_list();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  Component_defContext* component_def();

  class  Keyword_as_component_nameContext : public antlr4::ParserRuleContext {
  public:
    Keyword_as_component_nameContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *SIN();
    antlr4::tree::TerminalNode *COS();
    antlr4::tree::TerminalNode *TAN();
    antlr4::tree::TerminalNode *ASIN();
    antlr4::tree::TerminalNode *ACOS();
    antlr4::tree::TerminalNode *ATAN();
    antlr4::tree::TerminalNode *SINH();
    antlr4::tree::TerminalNode *COSH();
    antlr4::tree::TerminalNode *TANH();
    antlr4::tree::TerminalNode *ASINH();
    antlr4::tree::TerminalNode *ACOSH();
    antlr4::tree::TerminalNode *ATANH();
    antlr4::tree::TerminalNode *EXP();
    antlr4::tree::TerminalNode *LN();
    antlr4::tree::TerminalNode *LOG10();
    antlr4::tree::TerminalNode *LOG2();
    antlr4::tree::TerminalNode *SQRT();
    antlr4::tree::TerminalNode *ABS();
    antlr4::tree::TerminalNode *MIN();
    antlr4::tree::TerminalNode *MAX();
    antlr4::tree::TerminalNode *SUM();
    antlr4::tree::TerminalNode *AVG();
    antlr4::tree::TerminalNode *IF();
    antlr4::tree::TerminalNode *TIME();
    antlr4::tree::TerminalNode *SAT();
    antlr4::tree::TerminalNode *MM();
    antlr4::tree::TerminalNode *HILL();
    antlr4::tree::TerminalNode *ARRHENIUS();
    antlr4::tree::TerminalNode *MRATIO();
    antlr4::tree::TerminalNode *TFUN();
    antlr4::tree::TerminalNode *FUNCTIONPRODUCT();
    antlr4::tree::TerminalNode *TYPE();
    antlr4::tree::TerminalNode *METHOD();
    antlr4::tree::TerminalNode *PARAMETER();
    antlr4::tree::TerminalNode *FILE();
    antlr4::tree::TerminalNode *FORMAT();
    antlr4::tree::TerminalNode *PREFIX();
    antlr4::tree::TerminalNode *SUFFIX();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  Keyword_as_component_nameContext* keyword_as_component_name();

  class  Keyword_as_mol_nameContext : public antlr4::ParserRuleContext {
  public:
    Keyword_as_mol_nameContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *SPECIES();
    antlr4::tree::TerminalNode *MOLECULE();
    antlr4::tree::TerminalNode *MOLECULES();
    antlr4::tree::TerminalNode *REACTION();
    antlr4::tree::TerminalNode *REACTIONS();
    antlr4::tree::TerminalNode *RULES();
    antlr4::tree::TerminalNode *PARAMETERS();
    antlr4::tree::TerminalNode *OBSERVABLES();
    antlr4::tree::TerminalNode *FUNCTIONS();
    antlr4::tree::TerminalNode *COMPARTMENTS();
    antlr4::tree::TerminalNode *ENERGY();
    antlr4::tree::TerminalNode *PATTERNS();
    antlr4::tree::TerminalNode *MODEL();
    antlr4::tree::TerminalNode *SEED();
    antlr4::tree::TerminalNode *GROUPS();
    antlr4::tree::TerminalNode *POPULATION();
    antlr4::tree::TerminalNode *COUNTER();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  Keyword_as_mol_nameContext* keyword_as_mol_name();

  class  State_listContext : public antlr4::ParserRuleContext {
  public:
    State_listContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    std::vector<State_nameContext *> state_name();
    State_nameContext* state_name(size_t i);
    std::vector<antlr4::tree::TerminalNode *> TILDE();
    antlr4::tree::TerminalNode* TILDE(size_t i);


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  State_listContext* state_list();

  class  State_nameContext : public antlr4::ParserRuleContext {
  public:
    State_nameContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *STRING();
    antlr4::tree::TerminalNode *INT();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  State_nameContext* state_name();

  class  Seed_species_blockContext : public antlr4::ParserRuleContext {
  public:
    Seed_species_blockContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *BEGIN();
    antlr4::tree::TerminalNode *END();
    std::vector<antlr4::tree::TerminalNode *> SEED();
    antlr4::tree::TerminalNode* SEED(size_t i);
    std::vector<antlr4::tree::TerminalNode *> SPECIES();
    antlr4::tree::TerminalNode* SPECIES(size_t i);
    std::vector<antlr4::tree::TerminalNode *> LB();
    antlr4::tree::TerminalNode* LB(size_t i);
    std::vector<Seed_species_defContext *> seed_species_def();
    Seed_species_defContext* seed_species_def(size_t i);


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  Seed_species_blockContext* seed_species_block();

  class  Seed_species_defContext : public antlr4::ParserRuleContext {
  public:
    Seed_species_defContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    Species_defContext *species_def();
    antlr4::tree::TerminalNode *INT();
    std::vector<antlr4::tree::TerminalNode *> STRING();
    antlr4::tree::TerminalNode* STRING(size_t i);
    std::vector<antlr4::tree::TerminalNode *> COLON();
    antlr4::tree::TerminalNode* COLON(size_t i);
    antlr4::tree::TerminalNode *DOLLAR();
    antlr4::tree::TerminalNode *AT();
    ExpressionContext *expression();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  Seed_species_defContext* seed_species_def();

  class  Species_defContext : public antlr4::ParserRuleContext {
  public:
    Species_defContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    std::vector<Molecule_patternContext *> molecule_pattern();
    Molecule_patternContext* molecule_pattern(size_t i);
    std::vector<antlr4::tree::TerminalNode *> AT();
    antlr4::tree::TerminalNode* AT(size_t i);
    std::vector<antlr4::tree::TerminalNode *> STRING();
    antlr4::tree::TerminalNode* STRING(size_t i);
    antlr4::tree::TerminalNode *COLON();
    std::vector<Molecule_compartmentContext *> molecule_compartment();
    Molecule_compartmentContext* molecule_compartment(size_t i);
    std::vector<antlr4::tree::TerminalNode *> DOT();
    antlr4::tree::TerminalNode* DOT(size_t i);


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  Species_defContext* species_def();

  class  Molecule_compartmentContext : public antlr4::ParserRuleContext {
  public:
    Molecule_compartmentContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *AT();
    antlr4::tree::TerminalNode *STRING();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  Molecule_compartmentContext* molecule_compartment();

  class  Molecule_patternContext : public antlr4::ParserRuleContext {
  public:
    Molecule_patternContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *STRING();
    Keyword_as_mol_nameContext *keyword_as_mol_name();
    Scope_prefixContext *scope_prefix();
    Molecule_compartmentContext *molecule_compartment();
    std::vector<Molecule_tagContext *> molecule_tag();
    Molecule_tagContext* molecule_tag(size_t i);
    antlr4::tree::TerminalNode *LPAREN();
    antlr4::tree::TerminalNode *RPAREN();
    Pattern_bond_wildcardContext *pattern_bond_wildcard();
    Molecule_attributesContext *molecule_attributes();
    Component_pattern_listContext *component_pattern_list();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  Molecule_patternContext* molecule_pattern();

  class  Scope_prefixContext : public antlr4::ParserRuleContext {
  public:
    Scope_prefixContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *MOD();
    antlr4::tree::TerminalNode *STRING();
    std::vector<antlr4::tree::TerminalNode *> COLON();
    antlr4::tree::TerminalNode* COLON(size_t i);


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  Scope_prefixContext* scope_prefix();

  class  Pattern_bond_wildcardContext : public antlr4::ParserRuleContext {
  public:
    Pattern_bond_wildcardContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *EMARK();
    antlr4::tree::TerminalNode *PLUS();
    antlr4::tree::TerminalNode *QMARK();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  Pattern_bond_wildcardContext* pattern_bond_wildcard();

  class  Molecule_tagContext : public antlr4::ParserRuleContext {
  public:
    Molecule_tagContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *MOD();
    antlr4::tree::TerminalNode *INT();
    antlr4::tree::TerminalNode *STRING();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  Molecule_tagContext* molecule_tag();

  class  Component_pattern_listContext : public antlr4::ParserRuleContext {
  public:
    Component_pattern_listContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    std::vector<Component_patternContext *> component_pattern();
    Component_patternContext* component_pattern(size_t i);
    std::vector<antlr4::tree::TerminalNode *> COMMA();
    antlr4::tree::TerminalNode* COMMA(size_t i);


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  Component_pattern_listContext* component_pattern_list();

  class  Component_patternContext : public antlr4::ParserRuleContext {
  public:
    Component_patternContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *STRING();
    antlr4::tree::TerminalNode *INT();
    Keyword_as_component_nameContext *keyword_as_component_name();
    std::vector<Bond_specContext *> bond_spec();
    Bond_specContext* bond_spec(size_t i);
    std::vector<Component_labelContext *> component_label();
    Component_labelContext* component_label(size_t i);
    std::vector<antlr4::tree::TerminalNode *> DOT();
    antlr4::tree::TerminalNode* DOT(size_t i);
    std::vector<antlr4::tree::TerminalNode *> TILDE();
    antlr4::tree::TerminalNode* TILDE(size_t i);
    std::vector<State_valueContext *> state_value();
    State_valueContext* state_value(size_t i);


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  Component_patternContext* component_pattern();

  class  State_valueContext : public antlr4::ParserRuleContext {
  public:
    State_valueContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *STRING();
    antlr4::tree::TerminalNode *INT();
    antlr4::tree::TerminalNode *QMARK();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  State_valueContext* state_value();

  class  Bond_specContext : public antlr4::ParserRuleContext {
  public:
    Bond_specContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *DOT();
    antlr4::tree::TerminalNode *EMARK();
    Bond_idContext *bond_id();
    antlr4::tree::TerminalNode *PLUS();
    antlr4::tree::TerminalNode *QMARK();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  Bond_specContext* bond_spec();

  class  Component_labelContext : public antlr4::ParserRuleContext {
  public:
    Component_labelContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *MOD();
    antlr4::tree::TerminalNode *INT();
    antlr4::tree::TerminalNode *STRING();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  Component_labelContext* component_label();

  class  Bond_idContext : public antlr4::ParserRuleContext {
  public:
    Bond_idContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *INT();
    antlr4::tree::TerminalNode *STRING();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  Bond_idContext* bond_id();

  class  Observables_blockContext : public antlr4::ParserRuleContext {
  public:
    Observables_blockContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *BEGIN();
    std::vector<antlr4::tree::TerminalNode *> OBSERVABLES();
    antlr4::tree::TerminalNode* OBSERVABLES(size_t i);
    antlr4::tree::TerminalNode *END();
    std::vector<antlr4::tree::TerminalNode *> LB();
    antlr4::tree::TerminalNode* LB(size_t i);
    std::vector<Observable_defContext *> observable_def();
    Observable_defContext* observable_def(size_t i);


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  Observables_blockContext* observables_block();

  class  Observable_defContext : public antlr4::ParserRuleContext {
  public:
    Observable_defContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    std::vector<antlr4::tree::TerminalNode *> STRING();
    antlr4::tree::TerminalNode* STRING(size_t i);
    Observable_pattern_listContext *observable_pattern_list();
    antlr4::tree::TerminalNode *COLON();
    Observable_typeContext *observable_type();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  Observable_defContext* observable_def();

  class  Observable_typeContext : public antlr4::ParserRuleContext {
  public:
    Observable_typeContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *MOLECULES();
    antlr4::tree::TerminalNode *SPECIES();
    antlr4::tree::TerminalNode *COUNTER();
    antlr4::tree::TerminalNode *STRING();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  Observable_typeContext* observable_type();

  class  Observable_pattern_listContext : public antlr4::ParserRuleContext {
  public:
    Observable_pattern_listContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    std::vector<Observable_patternContext *> observable_pattern();
    Observable_patternContext* observable_pattern(size_t i);
    std::vector<antlr4::tree::TerminalNode *> COMMA();
    antlr4::tree::TerminalNode* COMMA(size_t i);


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  Observable_pattern_listContext* observable_pattern_list();

  class  Observable_patternContext : public antlr4::ParserRuleContext {
  public:
    Observable_patternContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    Species_defContext *species_def();
    antlr4::tree::TerminalNode *GT();
    antlr4::tree::TerminalNode *INT();
    antlr4::tree::TerminalNode *STRING();
    antlr4::tree::TerminalNode *EQUALS();
    antlr4::tree::TerminalNode *GTE();
    antlr4::tree::TerminalNode *LT();
    antlr4::tree::TerminalNode *LTE();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  Observable_patternContext* observable_pattern();

  class  Reaction_rules_blockContext : public antlr4::ParserRuleContext {
  public:
    Reaction_rules_blockContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *BEGIN();
    std::vector<antlr4::tree::TerminalNode *> REACTION();
    antlr4::tree::TerminalNode* REACTION(size_t i);
    std::vector<antlr4::tree::TerminalNode *> RULES();
    antlr4::tree::TerminalNode* RULES(size_t i);
    antlr4::tree::TerminalNode *END();
    std::vector<antlr4::tree::TerminalNode *> LB();
    antlr4::tree::TerminalNode* LB(size_t i);
    std::vector<Reaction_rule_defContext *> reaction_rule_def();
    Reaction_rule_defContext* reaction_rule_def(size_t i);
    std::vector<antlr4::tree::TerminalNode *> REACTION_RULES();
    antlr4::tree::TerminalNode* REACTION_RULES(size_t i);
    std::vector<antlr4::tree::TerminalNode *> REACTIONS();
    antlr4::tree::TerminalNode* REACTIONS(size_t i);


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  Reaction_rules_blockContext* reaction_rules_block();

  class  Reaction_rule_defContext : public antlr4::ParserRuleContext {
  public:
    Reaction_rule_defContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    Reactant_patternsContext *reactant_patterns();
    Reaction_signContext *reaction_sign();
    Product_patternsContext *product_patterns();
    Rate_lawContext *rate_law();
    Label_defContext *label_def();
    antlr4::tree::TerminalNode *LBRACKET();
    std::vector<Rule_modifiersContext *> rule_modifiers();
    Rule_modifiersContext* rule_modifiers(size_t i);
    antlr4::tree::TerminalNode *RBRACKET();
    std::vector<antlr4::tree::TerminalNode *> COMMA();
    antlr4::tree::TerminalNode* COMMA(size_t i);


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  Reaction_rule_defContext* reaction_rule_def();

  class  Label_defContext : public antlr4::ParserRuleContext {
  public:
    Label_defContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *COLON();
    std::vector<antlr4::tree::TerminalNode *> INT();
    antlr4::tree::TerminalNode* INT(size_t i);
    std::vector<antlr4::tree::TerminalNode *> STRING();
    antlr4::tree::TerminalNode* STRING(size_t i);
    std::vector<antlr4::tree::TerminalNode *> LPAREN();
    antlr4::tree::TerminalNode* LPAREN(size_t i);
    std::vector<antlr4::tree::TerminalNode *> RPAREN();
    antlr4::tree::TerminalNode* RPAREN(size_t i);


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  Label_defContext* label_def();

  class  Reactant_patternsContext : public antlr4::ParserRuleContext {
  public:
    Reactant_patternsContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    std::vector<Species_defContext *> species_def();
    Species_defContext* species_def(size_t i);
    std::vector<antlr4::tree::TerminalNode *> INT();
    antlr4::tree::TerminalNode* INT(size_t i);
    std::vector<antlr4::tree::TerminalNode *> PLUS();
    antlr4::tree::TerminalNode* PLUS(size_t i);


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  Reactant_patternsContext* reactant_patterns();

  class  Product_patternsContext : public antlr4::ParserRuleContext {
  public:
    Product_patternsContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    std::vector<Species_defContext *> species_def();
    Species_defContext* species_def(size_t i);
    std::vector<antlr4::tree::TerminalNode *> INT();
    antlr4::tree::TerminalNode* INT(size_t i);
    std::vector<antlr4::tree::TerminalNode *> PLUS();
    antlr4::tree::TerminalNode* PLUS(size_t i);


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  Product_patternsContext* product_patterns();

  class  Reaction_signContext : public antlr4::ParserRuleContext {
  public:
    Reaction_signContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *UNI_REACTION_SIGN();
    antlr4::tree::TerminalNode *BI_REACTION_SIGN();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  Reaction_signContext* reaction_sign();

  class  Rate_lawContext : public antlr4::ParserRuleContext {
  public:
    Rate_lawContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    std::vector<ExpressionContext *> expression();
    ExpressionContext* expression(size_t i);
    antlr4::tree::TerminalNode *COMMA();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  Rate_lawContext* rate_law();

  class  Rule_modifiersContext : public antlr4::ParserRuleContext {
  public:
    Rule_modifiersContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *DELETEMOLECULES();
    antlr4::tree::TerminalNode *MOVECONNECTED();
    antlr4::tree::TerminalNode *MATCHONCE();
    antlr4::tree::TerminalNode *TOTALRATE();
    antlr4::tree::TerminalNode *PRIORITY();
    antlr4::tree::TerminalNode *BECOMES();
    ExpressionContext *expression();
    antlr4::tree::TerminalNode *INCLUDE_REACTANTS();
    antlr4::tree::TerminalNode *LPAREN();
    antlr4::tree::TerminalNode *INT();
    antlr4::tree::TerminalNode *COMMA();
    Pattern_listContext *pattern_list();
    antlr4::tree::TerminalNode *RPAREN();
    antlr4::tree::TerminalNode *EXCLUDE_REACTANTS();
    antlr4::tree::TerminalNode *INCLUDE_PRODUCTS();
    antlr4::tree::TerminalNode *EXCLUDE_PRODUCTS();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  Rule_modifiersContext* rule_modifiers();

  class  Pattern_listContext : public antlr4::ParserRuleContext {
  public:
    Pattern_listContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    std::vector<Species_defContext *> species_def();
    Species_defContext* species_def(size_t i);
    std::vector<antlr4::tree::TerminalNode *> COMMA();
    antlr4::tree::TerminalNode* COMMA(size_t i);


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  Pattern_listContext* pattern_list();

  class  Functions_blockContext : public antlr4::ParserRuleContext {
  public:
    Functions_blockContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *BEGIN();
    std::vector<antlr4::tree::TerminalNode *> FUNCTIONS();
    antlr4::tree::TerminalNode* FUNCTIONS(size_t i);
    antlr4::tree::TerminalNode *END();
    std::vector<antlr4::tree::TerminalNode *> LB();
    antlr4::tree::TerminalNode* LB(size_t i);
    std::vector<Function_defContext *> function_def();
    Function_defContext* function_def(size_t i);


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  Functions_blockContext* functions_block();

  class  Function_defContext : public antlr4::ParserRuleContext {
  public:
    Function_defContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    std::vector<antlr4::tree::TerminalNode *> STRING();
    antlr4::tree::TerminalNode* STRING(size_t i);
    ExpressionContext *expression();
    antlr4::tree::TerminalNode *COLON();
    antlr4::tree::TerminalNode *LPAREN();
    antlr4::tree::TerminalNode *RPAREN();
    antlr4::tree::TerminalNode *BECOMES();
    Param_listContext *param_list();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  Function_defContext* function_def();

  class  Param_listContext : public antlr4::ParserRuleContext {
  public:
    Param_listContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    std::vector<antlr4::tree::TerminalNode *> STRING();
    antlr4::tree::TerminalNode* STRING(size_t i);
    std::vector<antlr4::tree::TerminalNode *> COMMA();
    antlr4::tree::TerminalNode* COMMA(size_t i);


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  Param_listContext* param_list();

  class  Compartments_blockContext : public antlr4::ParserRuleContext {
  public:
    Compartments_blockContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *BEGIN();
    std::vector<antlr4::tree::TerminalNode *> COMPARTMENTS();
    antlr4::tree::TerminalNode* COMPARTMENTS(size_t i);
    antlr4::tree::TerminalNode *END();
    std::vector<antlr4::tree::TerminalNode *> LB();
    antlr4::tree::TerminalNode* LB(size_t i);
    std::vector<Compartment_defContext *> compartment_def();
    Compartment_defContext* compartment_def(size_t i);


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  Compartments_blockContext* compartments_block();

  class  Compartment_defContext : public antlr4::ParserRuleContext {
  public:
    Compartment_defContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    std::vector<antlr4::tree::TerminalNode *> STRING();
    antlr4::tree::TerminalNode* STRING(size_t i);
    antlr4::tree::TerminalNode *INT();
    ExpressionContext *expression();
    antlr4::tree::TerminalNode *COLON();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  Compartment_defContext* compartment_def();

  class  Energy_patterns_blockContext : public antlr4::ParserRuleContext {
  public:
    Energy_patterns_blockContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *BEGIN();
    std::vector<antlr4::tree::TerminalNode *> ENERGY();
    antlr4::tree::TerminalNode* ENERGY(size_t i);
    std::vector<antlr4::tree::TerminalNode *> PATTERNS();
    antlr4::tree::TerminalNode* PATTERNS(size_t i);
    antlr4::tree::TerminalNode *END();
    std::vector<antlr4::tree::TerminalNode *> LB();
    antlr4::tree::TerminalNode* LB(size_t i);
    std::vector<Energy_pattern_defContext *> energy_pattern_def();
    Energy_pattern_defContext* energy_pattern_def(size_t i);


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  Energy_patterns_blockContext* energy_patterns_block();

  class  Energy_pattern_defContext : public antlr4::ParserRuleContext {
  public:
    Energy_pattern_defContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    Species_defContext *species_def();
    ExpressionContext *expression();
    antlr4::tree::TerminalNode *STRING();
    antlr4::tree::TerminalNode *COLON();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  Energy_pattern_defContext* energy_pattern_def();

  class  Population_maps_blockContext : public antlr4::ParserRuleContext {
  public:
    Population_maps_blockContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *BEGIN();
    std::vector<antlr4::tree::TerminalNode *> POPULATION();
    antlr4::tree::TerminalNode* POPULATION(size_t i);
    std::vector<antlr4::tree::TerminalNode *> MAPS();
    antlr4::tree::TerminalNode* MAPS(size_t i);
    antlr4::tree::TerminalNode *END();
    std::vector<antlr4::tree::TerminalNode *> LB();
    antlr4::tree::TerminalNode* LB(size_t i);
    std::vector<Population_map_defContext *> population_map_def();
    Population_map_defContext* population_map_def(size_t i);


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  Population_maps_blockContext* population_maps_block();

  class  Population_map_defContext : public antlr4::ParserRuleContext {
  public:
    Population_map_defContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    Species_defContext *species_def();
    antlr4::tree::TerminalNode *UNI_REACTION_SIGN();
    std::vector<antlr4::tree::TerminalNode *> STRING();
    antlr4::tree::TerminalNode* STRING(size_t i);
    antlr4::tree::TerminalNode *LPAREN();
    antlr4::tree::TerminalNode *RPAREN();
    antlr4::tree::TerminalNode *COLON();
    Param_listContext *param_list();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  Population_map_defContext* population_map_def();

  class  Actions_blockContext : public antlr4::ParserRuleContext {
  public:
    Actions_blockContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    std::vector<Action_commandContext *> action_command();
    Action_commandContext* action_command(size_t i);


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  Actions_blockContext* actions_block();

  class  Wrapped_actions_blockContext : public antlr4::ParserRuleContext {
  public:
    Wrapped_actions_blockContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *BEGIN();
    std::vector<antlr4::tree::TerminalNode *> ACTIONS();
    antlr4::tree::TerminalNode* ACTIONS(size_t i);
    antlr4::tree::TerminalNode *END();
    std::vector<antlr4::tree::TerminalNode *> LB();
    antlr4::tree::TerminalNode* LB(size_t i);
    std::vector<Action_commandContext *> action_command();
    Action_commandContext* action_command(size_t i);


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  Wrapped_actions_blockContext* wrapped_actions_block();

  class  Begin_actions_blockContext : public antlr4::ParserRuleContext {
  public:
    Begin_actions_blockContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *BEGIN();
    std::vector<antlr4::tree::TerminalNode *> ACTIONS();
    antlr4::tree::TerminalNode* ACTIONS(size_t i);
    antlr4::tree::TerminalNode *END();
    std::vector<antlr4::tree::TerminalNode *> LB();
    antlr4::tree::TerminalNode* LB(size_t i);
    std::vector<Action_commandContext *> action_command();
    Action_commandContext* action_command(size_t i);


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  Begin_actions_blockContext* begin_actions_block();

  class  Action_commandContext : public antlr4::ParserRuleContext {
  public:
    Action_commandContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    Generate_network_cmdContext *generate_network_cmd();
    Simulate_cmdContext *simulate_cmd();
    Write_cmdContext *write_cmd();
    Set_cmdContext *set_cmd();
    Other_action_cmdContext *other_action_cmd();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  Action_commandContext* action_command();

  class  Generate_network_cmdContext : public antlr4::ParserRuleContext {
  public:
    Generate_network_cmdContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *GENERATENETWORK();
    antlr4::tree::TerminalNode *LPAREN();
    antlr4::tree::TerminalNode *RPAREN();
    Action_argsContext *action_args();
    antlr4::tree::TerminalNode *SEMI();
    std::vector<antlr4::tree::TerminalNode *> LB();
    antlr4::tree::TerminalNode* LB(size_t i);


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  Generate_network_cmdContext* generate_network_cmd();

  class  Simulate_cmdContext : public antlr4::ParserRuleContext {
  public:
    Simulate_cmdContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *LPAREN();
    antlr4::tree::TerminalNode *RPAREN();
    antlr4::tree::TerminalNode *SIMULATE();
    antlr4::tree::TerminalNode *SIMULATE_ODE();
    antlr4::tree::TerminalNode *SIMULATE_SSA();
    antlr4::tree::TerminalNode *SIMULATE_PLA();
    antlr4::tree::TerminalNode *SIMULATE_NF();
    antlr4::tree::TerminalNode *SIMULATE_RM();
    Action_argsContext *action_args();
    antlr4::tree::TerminalNode *SEMI();
    std::vector<antlr4::tree::TerminalNode *> LB();
    antlr4::tree::TerminalNode* LB(size_t i);


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  Simulate_cmdContext* simulate_cmd();

  class  Write_cmdContext : public antlr4::ParserRuleContext {
  public:
    Write_cmdContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *LPAREN();
    antlr4::tree::TerminalNode *RPAREN();
    antlr4::tree::TerminalNode *WRITEFILE();
    antlr4::tree::TerminalNode *WRITEXML();
    antlr4::tree::TerminalNode *WRITESBML();
    antlr4::tree::TerminalNode *WRITENETWORK();
    antlr4::tree::TerminalNode *WRITEMODEL();
    antlr4::tree::TerminalNode *WRITEMFILE();
    antlr4::tree::TerminalNode *WRITEMEXFILE();
    antlr4::tree::TerminalNode *WRITELATEX();
    Action_argsContext *action_args();
    antlr4::tree::TerminalNode *SEMI();
    std::vector<antlr4::tree::TerminalNode *> LB();
    antlr4::tree::TerminalNode* LB(size_t i);


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  Write_cmdContext* write_cmd();

  class  Set_cmdContext : public antlr4::ParserRuleContext {
  public:
    Set_cmdContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *LPAREN();
    Quoted_stringContext *quoted_string();
    antlr4::tree::TerminalNode *COMMA();
    Action_arg_valueContext *action_arg_value();
    antlr4::tree::TerminalNode *RPAREN();
    antlr4::tree::TerminalNode *SETCONCENTRATION();
    antlr4::tree::TerminalNode *ADDCONCENTRATION();
    antlr4::tree::TerminalNode *SETPARAMETER();
    antlr4::tree::TerminalNode *SEMI();
    std::vector<antlr4::tree::TerminalNode *> LB();
    antlr4::tree::TerminalNode* LB(size_t i);


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  Set_cmdContext* set_cmd();

  class  Other_action_cmdContext : public antlr4::ParserRuleContext {
  public:
    Other_action_cmdContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *LPAREN();
    antlr4::tree::TerminalNode *RPAREN();
    antlr4::tree::TerminalNode *SAVECONCENTRATIONS();
    antlr4::tree::TerminalNode *RESETCONCENTRATIONS();
    antlr4::tree::TerminalNode *SAVEPARAMETERS();
    antlr4::tree::TerminalNode *RESETPARAMETERS();
    antlr4::tree::TerminalNode *QUIT();
    antlr4::tree::TerminalNode *PARAMETER_SCAN();
    antlr4::tree::TerminalNode *BIFURCATE();
    antlr4::tree::TerminalNode *VISUALIZE();
    antlr4::tree::TerminalNode *GENERATEHYBRIDMODEL();
    antlr4::tree::TerminalNode *READFILE();
    Action_argsContext *action_args();
    Action_arg_valueContext *action_arg_value();
    antlr4::tree::TerminalNode *SEMI();
    std::vector<antlr4::tree::TerminalNode *> LB();
    antlr4::tree::TerminalNode* LB(size_t i);


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  Other_action_cmdContext* other_action_cmd();

  class  Action_argsContext : public antlr4::ParserRuleContext {
  public:
    Action_argsContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *LBRACKET();
    antlr4::tree::TerminalNode *RBRACKET();
    Action_arg_listContext *action_arg_list();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  Action_argsContext* action_args();

  class  Action_arg_listContext : public antlr4::ParserRuleContext {
  public:
    Action_arg_listContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    std::vector<Action_argContext *> action_arg();
    Action_argContext* action_arg(size_t i);
    std::vector<antlr4::tree::TerminalNode *> COMMA();
    antlr4::tree::TerminalNode* COMMA(size_t i);


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  Action_arg_listContext* action_arg_list();

  class  Action_argContext : public antlr4::ParserRuleContext {
  public:
    Action_argContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    Arg_nameContext *arg_name();
    antlr4::tree::TerminalNode *ASSIGNS();
    Action_arg_valueContext *action_arg_value();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  Action_argContext* action_arg();

  class  Action_arg_valueContext : public antlr4::ParserRuleContext {
  public:
    Action_arg_valueContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    ExpressionContext *expression();
    Keyword_as_valueContext *keyword_as_value();
    Quoted_stringContext *quoted_string();
    antlr4::tree::TerminalNode *LSBRACKET();
    Expression_listContext *expression_list();
    antlr4::tree::TerminalNode *RSBRACKET();
    antlr4::tree::TerminalNode *LBRACKET();
    antlr4::tree::TerminalNode *RBRACKET();
    Nested_hash_listContext *nested_hash_list();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  Action_arg_valueContext* action_arg_value();

  class  Quoted_stringContext : public antlr4::ParserRuleContext {
  public:
    Quoted_stringContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *QUOTED_STRING();
    antlr4::tree::TerminalNode *SINGLE_QUOTED_STRING();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  Quoted_stringContext* quoted_string();

  class  Keyword_as_valueContext : public antlr4::ParserRuleContext {
  public:
    Keyword_as_valueContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *ODE();
    antlr4::tree::TerminalNode *SSA();
    antlr4::tree::TerminalNode *NF();
    antlr4::tree::TerminalNode *PLA();
    antlr4::tree::TerminalNode *SPARSE();
    antlr4::tree::TerminalNode *VERBOSE();
    antlr4::tree::TerminalNode *OVERWRITE();
    antlr4::tree::TerminalNode *CONTINUE();
    antlr4::tree::TerminalNode *SAFE();
    antlr4::tree::TerminalNode *EXECUTE();
    antlr4::tree::TerminalNode *BINARY_OUTPUT();
    antlr4::tree::TerminalNode *STEADY_STATE();
    antlr4::tree::TerminalNode *BDF();
    antlr4::tree::TerminalNode *STIFF();
    antlr4::tree::TerminalNode *METHOD();
    antlr4::tree::TerminalNode *TRUE();
    antlr4::tree::TerminalNode *FALSE();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  Keyword_as_valueContext* keyword_as_value();

  class  Nested_hash_listContext : public antlr4::ParserRuleContext {
  public:
    Nested_hash_listContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    std::vector<Nested_hash_itemContext *> nested_hash_item();
    Nested_hash_itemContext* nested_hash_item(size_t i);
    std::vector<antlr4::tree::TerminalNode *> COMMA();
    antlr4::tree::TerminalNode* COMMA(size_t i);


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  Nested_hash_listContext* nested_hash_list();

  class  Nested_hash_itemContext : public antlr4::ParserRuleContext {
  public:
    Nested_hash_itemContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *ASSIGNS();
    Action_arg_valueContext *action_arg_value();
    antlr4::tree::TerminalNode *STRING();
    Quoted_stringContext *quoted_string();
    Arg_nameContext *arg_name();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  Nested_hash_itemContext* nested_hash_item();

  class  Arg_nameContext : public antlr4::ParserRuleContext {
  public:
    Arg_nameContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *STRING();
    antlr4::tree::TerminalNode *TIME();
    antlr4::tree::TerminalNode *OVERWRITE();
    antlr4::tree::TerminalNode *MAX_AGG();
    antlr4::tree::TerminalNode *MAX_ITER();
    antlr4::tree::TerminalNode *MAX_STOICH();
    antlr4::tree::TerminalNode *PRINT_ITER();
    antlr4::tree::TerminalNode *CHECK_ISO();
    antlr4::tree::TerminalNode *METHOD();
    antlr4::tree::TerminalNode *T_START();
    antlr4::tree::TerminalNode *T_END();
    antlr4::tree::TerminalNode *N_STEPS();
    antlr4::tree::TerminalNode *N_OUTPUT_STEPS();
    antlr4::tree::TerminalNode *ATOL();
    antlr4::tree::TerminalNode *RTOL();
    antlr4::tree::TerminalNode *STEADY_STATE();
    antlr4::tree::TerminalNode *SPARSE();
    antlr4::tree::TerminalNode *VERBOSE();
    antlr4::tree::TerminalNode *NETFILE();
    antlr4::tree::TerminalNode *CONTINUE();
    antlr4::tree::TerminalNode *PREFIX();
    antlr4::tree::TerminalNode *SUFFIX();
    antlr4::tree::TerminalNode *FORMAT();
    antlr4::tree::TerminalNode *FILE();
    antlr4::tree::TerminalNode *PRINT_CDAT();
    antlr4::tree::TerminalNode *PRINT_FUNCTIONS();
    antlr4::tree::TerminalNode *PRINT_NET();
    antlr4::tree::TerminalNode *PRINT_END();
    antlr4::tree::TerminalNode *STOP_IF();
    antlr4::tree::TerminalNode *PRINT_ON_STOP();
    antlr4::tree::TerminalNode *SAVE_PROGRESS();
    antlr4::tree::TerminalNode *MAX_SIM_STEPS();
    antlr4::tree::TerminalNode *OUTPUT_STEP_INTERVAL();
    antlr4::tree::TerminalNode *SAMPLE_TIMES();
    antlr4::tree::TerminalNode *PLA_CONFIG();
    antlr4::tree::TerminalNode *PLA_OUTPUT();
    antlr4::tree::TerminalNode *PARAM();
    antlr4::tree::TerminalNode *COMPLEX();
    antlr4::tree::TerminalNode *GET_FINAL_STATE();
    antlr4::tree::TerminalNode *GML();
    antlr4::tree::TerminalNode *NOCSLF();
    antlr4::tree::TerminalNode *NOTF();
    antlr4::tree::TerminalNode *BINARY_OUTPUT();
    antlr4::tree::TerminalNode *UTL();
    antlr4::tree::TerminalNode *EQUIL();
    antlr4::tree::TerminalNode *PARAMETER();
    antlr4::tree::TerminalNode *PAR_MIN();
    antlr4::tree::TerminalNode *PAR_MAX();
    antlr4::tree::TerminalNode *N_SCAN_PTS();
    antlr4::tree::TerminalNode *LOG_SCALE();
    antlr4::tree::TerminalNode *RESET_CONC();
    antlr4::tree::TerminalNode *BDF();
    antlr4::tree::TerminalNode *MAX_STEP();
    antlr4::tree::TerminalNode *MAXORDER();
    antlr4::tree::TerminalNode *STATS();
    antlr4::tree::TerminalNode *MAX_NUM_STEPS();
    antlr4::tree::TerminalNode *MAX_ERR_TEST_FAILS();
    antlr4::tree::TerminalNode *MAX_CONV_FAILS();
    antlr4::tree::TerminalNode *STIFF();
    antlr4::tree::TerminalNode *ATOMIZE();
    antlr4::tree::TerminalNode *BLOCKS();
    antlr4::tree::TerminalNode *SKIPACTIONS();
    antlr4::tree::TerminalNode *INCLUDE_MODEL();
    antlr4::tree::TerminalNode *INCLUDE_NETWORK();
    antlr4::tree::TerminalNode *PRETTY_FORMATTING();
    antlr4::tree::TerminalNode *EVALUATE_EXPRESSIONS();
    antlr4::tree::TerminalNode *TYPE();
    antlr4::tree::TerminalNode *BACKGROUND();
    antlr4::tree::TerminalNode *COLLAPSE();
    antlr4::tree::TerminalNode *OPTS();
    antlr4::tree::TerminalNode *SAFE();
    antlr4::tree::TerminalNode *EXECUTE();
    antlr4::tree::TerminalNode *TEXTREACTION();
    antlr4::tree::TerminalNode *TEXTSPECIES();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  Arg_nameContext* arg_name();

  class  Expression_listContext : public antlr4::ParserRuleContext {
  public:
    Expression_listContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    std::vector<ExpressionContext *> expression();
    ExpressionContext* expression(size_t i);
    std::vector<antlr4::tree::TerminalNode *> COMMA();
    antlr4::tree::TerminalNode* COMMA(size_t i);


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  Expression_listContext* expression_list();

  class  ExpressionContext : public antlr4::ParserRuleContext {
  public:
    ExpressionContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    Conditional_exprContext *conditional_expr();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  ExpressionContext* expression();

  class  Conditional_exprContext : public antlr4::ParserRuleContext {
  public:
    Conditional_exprContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    Or_exprContext *or_expr();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  Conditional_exprContext* conditional_expr();

  class  Or_exprContext : public antlr4::ParserRuleContext {
  public:
    Or_exprContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    std::vector<And_exprContext *> and_expr();
    And_exprContext* and_expr(size_t i);
    std::vector<antlr4::tree::TerminalNode *> LOGICAL_OR();
    antlr4::tree::TerminalNode* LOGICAL_OR(size_t i);


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  Or_exprContext* or_expr();

  class  And_exprContext : public antlr4::ParserRuleContext {
  public:
    And_exprContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    std::vector<Equality_exprContext *> equality_expr();
    Equality_exprContext* equality_expr(size_t i);
    std::vector<antlr4::tree::TerminalNode *> LOGICAL_AND();
    antlr4::tree::TerminalNode* LOGICAL_AND(size_t i);


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  And_exprContext* and_expr();

  class  Equality_exprContext : public antlr4::ParserRuleContext {
  public:
    Equality_exprContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    std::vector<Relational_exprContext *> relational_expr();
    Relational_exprContext* relational_expr(size_t i);
    std::vector<antlr4::tree::TerminalNode *> EQUALS();
    antlr4::tree::TerminalNode* EQUALS(size_t i);
    std::vector<antlr4::tree::TerminalNode *> NOT_EQUALS();
    antlr4::tree::TerminalNode* NOT_EQUALS(size_t i);
    std::vector<antlr4::tree::TerminalNode *> GTE();
    antlr4::tree::TerminalNode* GTE(size_t i);
    std::vector<antlr4::tree::TerminalNode *> GT();
    antlr4::tree::TerminalNode* GT(size_t i);
    std::vector<antlr4::tree::TerminalNode *> LTE();
    antlr4::tree::TerminalNode* LTE(size_t i);
    std::vector<antlr4::tree::TerminalNode *> LT();
    antlr4::tree::TerminalNode* LT(size_t i);


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  Equality_exprContext* equality_expr();

  class  Relational_exprContext : public antlr4::ParserRuleContext {
  public:
    Relational_exprContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    Additive_exprContext *additive_expr();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  Relational_exprContext* relational_expr();

  class  Additive_exprContext : public antlr4::ParserRuleContext {
  public:
    Additive_exprContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    std::vector<Multiplicative_exprContext *> multiplicative_expr();
    Multiplicative_exprContext* multiplicative_expr(size_t i);
    std::vector<antlr4::tree::TerminalNode *> PLUS();
    antlr4::tree::TerminalNode* PLUS(size_t i);
    std::vector<antlr4::tree::TerminalNode *> MINUS();
    antlr4::tree::TerminalNode* MINUS(size_t i);


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  Additive_exprContext* additive_expr();

  class  Multiplicative_exprContext : public antlr4::ParserRuleContext {
  public:
    Multiplicative_exprContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    std::vector<Power_exprContext *> power_expr();
    Power_exprContext* power_expr(size_t i);
    std::vector<antlr4::tree::TerminalNode *> TIMES();
    antlr4::tree::TerminalNode* TIMES(size_t i);
    std::vector<antlr4::tree::TerminalNode *> DIV();
    antlr4::tree::TerminalNode* DIV(size_t i);
    std::vector<antlr4::tree::TerminalNode *> MOD();
    antlr4::tree::TerminalNode* MOD(size_t i);


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  Multiplicative_exprContext* multiplicative_expr();

  class  Power_exprContext : public antlr4::ParserRuleContext {
  public:
    Power_exprContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    std::vector<Unary_exprContext *> unary_expr();
    Unary_exprContext* unary_expr(size_t i);
    std::vector<antlr4::tree::TerminalNode *> POWER();
    antlr4::tree::TerminalNode* POWER(size_t i);


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  Power_exprContext* power_expr();

  class  Unary_exprContext : public antlr4::ParserRuleContext {
  public:
    Unary_exprContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    Primary_exprContext *primary_expr();
    antlr4::tree::TerminalNode *PLUS();
    antlr4::tree::TerminalNode *MINUS();
    antlr4::tree::TerminalNode *EMARK();
    antlr4::tree::TerminalNode *TILDE();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  Unary_exprContext* unary_expr();

  class  Primary_exprContext : public antlr4::ParserRuleContext {
  public:
    Primary_exprContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *LPAREN();
    ExpressionContext *expression();
    antlr4::tree::TerminalNode *RPAREN();
    Function_callContext *function_call();
    Observable_refContext *observable_ref();
    LiteralContext *literal();
    Arg_nameContext *arg_name();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  Primary_exprContext* primary_expr();

  class  Function_callContext : public antlr4::ParserRuleContext {
  public:
    Function_callContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *LPAREN();
    antlr4::tree::TerminalNode *RPAREN();
    antlr4::tree::TerminalNode *EXP();
    antlr4::tree::TerminalNode *LN();
    antlr4::tree::TerminalNode *LOG10();
    antlr4::tree::TerminalNode *LOG2();
    antlr4::tree::TerminalNode *SQRT();
    antlr4::tree::TerminalNode *ABS();
    antlr4::tree::TerminalNode *SIN();
    antlr4::tree::TerminalNode *COS();
    antlr4::tree::TerminalNode *TAN();
    antlr4::tree::TerminalNode *ASIN();
    antlr4::tree::TerminalNode *ACOS();
    antlr4::tree::TerminalNode *ATAN();
    antlr4::tree::TerminalNode *SINH();
    antlr4::tree::TerminalNode *COSH();
    antlr4::tree::TerminalNode *TANH();
    antlr4::tree::TerminalNode *ASINH();
    antlr4::tree::TerminalNode *ACOSH();
    antlr4::tree::TerminalNode *ATANH();
    antlr4::tree::TerminalNode *RINT();
    antlr4::tree::TerminalNode *MIN();
    antlr4::tree::TerminalNode *MAX();
    antlr4::tree::TerminalNode *SUM();
    antlr4::tree::TerminalNode *AVG();
    antlr4::tree::TerminalNode *IF();
    antlr4::tree::TerminalNode *SAT();
    antlr4::tree::TerminalNode *MM();
    antlr4::tree::TerminalNode *HILL();
    antlr4::tree::TerminalNode *ARRHENIUS();
    antlr4::tree::TerminalNode *TIME();
    antlr4::tree::TerminalNode *MRATIO();
    antlr4::tree::TerminalNode *TFUN();
    antlr4::tree::TerminalNode *FUNCTIONPRODUCT();
    Expression_listContext *expression_list();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  Function_callContext* function_call();

  class  Observable_refContext : public antlr4::ParserRuleContext {
  public:
    Observable_refContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *STRING();
    antlr4::tree::TerminalNode *LPAREN();
    antlr4::tree::TerminalNode *RPAREN();
    Expression_listContext *expression_list();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  Observable_refContext* observable_ref();

  class  LiteralContext : public antlr4::ParserRuleContext {
  public:
    LiteralContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *INT();
    antlr4::tree::TerminalNode *FLOAT();
    antlr4::tree::TerminalNode *PI();
    antlr4::tree::TerminalNode *EULERIAN();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  LiteralContext* literal();


  // By default the static state used to implement the parser is lazily initialized during the first
  // call to the constructor. You can call this function if you wish to initialize the static state
  // ahead of time.
  static void initialize();

private:
};

