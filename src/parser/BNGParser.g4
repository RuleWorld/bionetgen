/**
 * ANTLR4 Parser Grammar for BNGL
 * Inspired by RuleWorld/BNGParser
 * Simplified unified grammar for web simulator
 */
parser grammar BNGParser;

options {
    tokenVocab = BNGLexer;
}

// Entry point - support both "begin actions...end actions" and loose action commands after model
prog
    : LB* (header_block | action_command)* ((BEGIN MODEL LB+ program_block* END MODEL LB*) | program_block*) (wrapped_actions_block | actions_block)? EOF
    ;

header_block
    : version_def
    | substance_def
    | set_option
    | set_model_name
    ;

version_def
    : VERSION LPAREN quoted_string RPAREN SEMI? LB+
    ;

substance_def
    : SUBSTANCEUNITS LPAREN quoted_string RPAREN SEMI? LB+
    ;

set_option
    : SET_OPTION LPAREN quoted_string COMMA action_arg_value
      (COMMA quoted_string COMMA action_arg_value)* RPAREN SEMI? LB+
    ;

set_model_name
    : SET_MODEL_NAME LPAREN quoted_string RPAREN SEMI? LB+
    ;

// Program blocks
program_block
    : parameters_block
    | molecule_types_block
    | seed_species_block
    | observables_block
    | reaction_rules_block
    | functions_block
    | compartments_block
    | energy_patterns_block
    | population_maps_block
    | wrapped_actions_block
    | begin_actions_block  // NEW: Support "begin actions ... end actions"
    | set_option           // setOption() can appear inside model block
    ;

// Parameters block
parameters_block
    : BEGIN PARAMETERS LB+ (parameter_def LB+)* END PARAMETERS LB*
    ;

// Parameter names can be STRING or arg_name keywords (t_end, n_steps, etc.)
// Support numbered parameters like: 1 L0 1.0
parameter_def
    : INT? (param_name COLON)? param_name BECOMES? expression?
    ;

param_name
    : STRING | arg_name
    ;

// Molecule types block - supports "molecule types", "molecules", "molecule_types", and "molecular types"
molecule_types_block
    : BEGIN MOLECULE TYPES LB+ (molecule_type_def LB+)* END MOLECULE TYPES LB*
    | BEGIN MOLECULE_TYPES LB+ (molecule_type_def LB+)* END MOLECULE_TYPES LB*
    ;

molecule_type_def
    : (STRING COLON)? molecule_def POPULATION?  // BNG2 parity: optional population tag
    ;

// Molecules can have optional parentheses (e.g., "dead" or "A()" are both valid)
// Allow block keywords (Species, Molecule, etc.) as molecule names for BNG2 parity
molecule_def
    : (STRING | keyword_as_mol_name) (LPAREN component_def_list? RPAREN)? molecule_attributes?
    ;

molecule_attributes
    : LBRACKET action_arg_list? RBRACKET
    ;

// Allow empty entries in component lists to handle double commas (,,)
component_def_list
    : component_def? (COMMA component_def?)*
    ;

component_def
    : (STRING | INT | keyword_as_component_name) (TILDE state_list)?
    ;

// Allow math function keywords to be used as component names (e.g., sin, max, exp)
// Also allow action-related keywords that BNG2 permits as identifiers (e.g., type, method)
keyword_as_component_name
    : SIN | COS | TAN | ASIN | ACOS | ATAN | SINH | COSH | TANH
    | ASINH | ACOSH | ATANH | EXP | LN | LOG10 | LOG2 | SQRT | ABS
    | MIN | MAX | SUM | AVG | IF | TIME | SAT |  MM | HILL | ARRHENIUS
    | MRATIO | TFUN | FUNCTIONPRODUCT
    | TYPE | METHOD | PARAMETER | FILE | FORMAT | PREFIX | SUFFIX
    ;

// Allow block header keywords to be used as molecule names
// BNG2 permits this since molecule names are just identifiers
keyword_as_mol_name
    : SPECIES | MOLECULE | MOLECULES | REACTION | REACTIONS | RULES
    | PARAMETERS | OBSERVABLES | FUNCTIONS | COMPARTMENTS | ENERGY | PATTERNS
    | MODEL | SEED | GROUPS | POPULATION | COUNTER
    ;

// Allow numeric states like ~0~p and mixed like ~2P~10P (INT followed by STRING)
state_list
    : state_name (TILDE state_name)*
    ;

// State name can be: STRING, INT, or INT+STRING combined (like 2P, 10P)
state_name
    : STRING
    | INT STRING?
    ;

// Seed species block
// BNG2 allows multiple seed species definitions per line (tab/space separated)
seed_species_block
    : BEGIN (SEED SPECIES | SPECIES) LB+ (seed_species_def+ LB+)* END (SEED SPECIES | SPECIES) LB*
    ;

// Support: "1 @c0:Species(...) concentration" or just species without concentration
seed_species_def
    : INT? (STRING COLON)? DOLLAR? (AT STRING COLON)? species_def expression?
    ;

// Species can optionally have compartment annotation using @ (prefix @comp: or suffix @comp)
species_def
    : (AT STRING COLON)? molecule_pattern molecule_compartment? (DOT molecule_pattern molecule_compartment?)* (AT STRING)?
    ;

// CBNGL: allow compartment annotations on individual molecules in a complex
// e.g. A(t!1,b)@CYT.T1(a!1)
molecule_compartment
    : AT STRING
    ;

// Molecule patterns can have optional parentheses (e.g., ".CK1a" is valid in reactions)
// Molecule tagging: pattern%1, pattern%2 for identifying molecules in reactions
// Bond wildcards can appear after entire patterns: e.g., Smad1(loc~cyt)!+
// Scope prefix: %x:: for local function parameterization (e.g., %x::A())
molecule_pattern
    : scope_prefix? (STRING | keyword_as_mol_name) molecule_compartment? molecule_tag? (LPAREN component_pattern_list? RPAREN)? pattern_bond_wildcard? molecule_tag? molecule_attributes?
    ;

scope_prefix
    : MOLECULE_TAG_TOKEN COLON COLON
    ;

// Bond wildcards that apply to entire molecule patterns
// These specify bond requirements for the whole molecule
pattern_bond_wildcard
    : EMARK PLUS      // !+ = molecule must have one or more bonds
    | EMARK QMARK     // !? = molecule may or may not have bonds  
    ;

molecule_tag
    : MOLECULE_TAG_TOKEN
    ;

// Allow empty entries in component pattern lists to handle double commas (,,)
component_pattern_list
    : component_pattern? (COMMA component_pattern?)*
    ;
// Component patterns can have state, multiple bonds (e.g., !0!1), labels (e.g. %1), or bond wildcards
// Support mixed/interleaved state, bond, and label attributes (e.g. c1%1, site!?~?)
// Support unbound notation with DOT (e.g., sin., ric.)
component_pattern
    : (STRING | INT | keyword_as_component_name) ((TILDE state_value) | bond_spec | component_label | DOT)*
    ;

state_value
    : STRING | INT STRING? | QMARK
    ;

bond_spec
    : DOT                    // Unbound binding site (e.g., sin., ric.)
    | EMARK bond_id
    | EMARK PLUS
    | EMARK QMARK
    ;

component_label
    : MOLECULE_TAG_TOKEN     // Component label (e.g., %1, %reaction_1)
    ;

bond_id
    : INT | STRING
    ;

// Observables block
observables_block
    : BEGIN OBSERVABLES LB+ (observable_def LB+)* END OBSERVABLES LB*
    ;

// Observable definition - type is optional (can be just "Name Pattern")
observable_def
    : (STRING COLON)? observable_type? STRING observable_pattern_list
    ;

observable_type
    : MOLECULES | SPECIES | COUNTER | STRING  // Molecules, Species, Counter, or custom type
    ;

// Support both comma-separated and space-separated patterns
// Also support stoichiometry expressions like R==1, R>20
observable_pattern_list
    : observable_pattern (COMMA? observable_pattern)*
    ;

// Observable pattern can be a species or a stoichiometry comparison (R==1, R>5, etc.)
observable_pattern
    : species_def (GT INT)?  // Added option for count filter (>N)
    | STRING (EQUALS | GT | GTE | LT | LTE) INT  // Stoichiometry comparison: R==1, R>5
    ;

// Reaction rules block - supports "reaction rules", "reaction_rules", and "reactions"
reaction_rules_block
    : BEGIN REACTION RULES LB+ (reaction_rule_def LB+)* END REACTION RULES LB*
    | BEGIN REACTION_RULES LB+ (reaction_rule_def LB+)* END REACTION_RULES LB*
    | BEGIN REACTIONS LB+ (reaction_rule_def LB+)* END REACTIONS LB*
    ;

reaction_rule_def
        : label_def?
            (LBRACKET rule_modifiers (COMMA? rule_modifiers)* RBRACKET)?
            reactant_patterns reaction_sign product_patterns rate_law rule_modifiers*
        ;

// Labels can be: "ruleName:", "1:", "1 Description:", or just inline before the rule
// The multi-word format is: INT (STRING | INT | LPAREN | RPAREN)* COLON
// Also support bare INT labels without colon (e.g. "1 A->B")
label_def
    : (INT | STRING) (STRING | INT | LPAREN STRING? RPAREN)* COLON
    | INT
    ;


reactant_patterns
    : (species_def | INT) (PLUS (species_def | INT))*
    ;

// Products can be species + species, just 0, or mixed like species + 0
product_patterns
    : (species_def | INT) (PLUS (species_def | INT))*
    ;

reaction_sign
    : UNI_REACTION_SIGN
    | BI_REACTION_SIGN
    ;

// Rate law: primary rate expression, optionally followed by comma and reverse rate.
// Note: rate_law_expr avoids MOD (%) as a binary operator since %N is always a molecule tag
// in BNGL (modulo is never used in rate laws in practice).
rate_law
    : rate_law_expr (COMMA rate_law_expr)?
    ;

// Rate law expression: same as expression but without MOD to avoid ambiguity with molecule tags
rate_law_expr
    : rate_law_or_expr
    ;

rate_law_or_expr
    : rate_law_and_expr (LOGICAL_OR rate_law_and_expr)*
    ;

rate_law_and_expr
    : rate_law_eq_expr (LOGICAL_AND rate_law_eq_expr)*
    ;

rate_law_eq_expr
    : rate_law_add_expr ((EQUALS | NOT_EQUALS | GTE | GT | LTE | LT) rate_law_add_expr)*
    ;

rate_law_add_expr
    : rate_law_mul_expr ((PLUS | MINUS) rate_law_mul_expr)*
    ;

// NOTE: No MOD here — % is always a molecule tag in BNGL, never modulo in rate laws
rate_law_mul_expr
    : rate_law_pow_expr ((TIMES | DIV) rate_law_pow_expr)*
    ;

rate_law_pow_expr
    : rate_law_unary_expr (POWER rate_law_unary_expr)*
    ;

rate_law_unary_expr
    : (PLUS | MINUS | EMARK | TILDE)? rate_law_primary_expr
    ;

rate_law_primary_expr
    : LPAREN rate_law_expr RPAREN
    | function_call
    | observable_ref
    | literal
    | arg_name
    ;

rule_modifiers
    : DELETEMOLECULES
    | MOVECONNECTED
    | MATCHONCE
    | TOTALRATE
    | PRIORITY BECOMES expression  // BNG2 parity: priority modifier (priority=5)
    | INCLUDE_REACTANTS LPAREN INT COMMA pattern_list RPAREN
    | EXCLUDE_REACTANTS LPAREN INT COMMA pattern_list RPAREN
    | INCLUDE_PRODUCTS LPAREN INT COMMA pattern_list RPAREN
    | EXCLUDE_PRODUCTS LPAREN INT COMMA pattern_list RPAREN
    ;

pattern_list
    : species_def (COMMA species_def)*
    ;

// Functions block
functions_block
    : BEGIN FUNCTIONS LB+ (function_def LB+)* END FUNCTIONS LB*
    ;

// Support both "funcName() = expr" and "funcName expr" formats
function_def
    : (STRING COLON)? STRING (LPAREN param_list? RPAREN)? BECOMES? expression
    ;

param_list
    : STRING (COMMA STRING)*
    ;

// Compartments block
compartments_block
    : BEGIN COMPARTMENTS LB+ (compartment_def LB+)* END COMPARTMENTS LB*
    ;

compartment_def
    : (STRING COLON)? STRING INT expression STRING?
    ;

// Energy patterns block
energy_patterns_block
    : BEGIN ENERGY PATTERNS LB+ (energy_pattern_def LB+)* END ENERGY PATTERNS LB*
    ;

energy_pattern_def
    : (STRING COLON)? species_def expression
    ;

// Population maps block
population_maps_block
    : BEGIN POPULATION MAPS LB+ (population_map_def LB+)* END POPULATION MAPS LB*
    ;

population_map_def
    : (STRING COLON)? species_def UNI_REACTION_SIGN STRING LPAREN param_list? RPAREN
    ;



// Actions block (unwrapped - for loose commands)
actions_block
    : action_command+
    ;

// Wrapped actions block (BEGIN ACTIONS ... END ACTIONS)
wrapped_actions_block
    : BEGIN ACTIONS LB+ action_command* END ACTIONS LB*
    ;

// Alias for wrapped_actions_block (already referenced in program_block)
begin_actions_block
    : BEGIN ACTIONS LB+ action_command* END ACTIONS LB*
    ;

action_command
    : generate_network_cmd
    | simulate_cmd
    | write_cmd
    | set_cmd
    | other_action_cmd
    ;

generate_network_cmd
    : GENERATENETWORK LPAREN action_args? RPAREN SEMI? LB*
    ;

simulate_cmd
    : (SIMULATE | SIMULATE_ODE | SIMULATE_SSA | SIMULATE_PLA | SIMULATE_NF | SIMULATE_RM | SIMULATE_PSA)
      LPAREN action_args? RPAREN SEMI? LB*
    ;

write_cmd
    : (WRITEFILE | WRITEXML | WRITESBML | WRITESBMLMULTI | WRITENETWORK | WRITEMODEL | WRITEMFILE | WRITEMEXFILE | WRITECPPFILE | WRITECPYFILE | WRITELATEX | WRITEMDL | WRITESSC | WRITESSCCFG)
      LPAREN action_args? RPAREN SEMI? LB*
    ;

// setConcentration/addConcentration: takes quoted species string and value (expression or quoted string)
set_cmd
    : (SETCONCENTRATION | ADDCONCENTRATION | SETPARAMETER) 
      LPAREN quoted_string COMMA action_arg_value RPAREN SEMI? LB*
    ;

other_action_cmd
    : (SAVECONCENTRATIONS | RESETCONCENTRATIONS | SAVEPARAMETERS | RESETPARAMETERS | QUIT
       | PARAMETER_SCAN | BIFURCATE | VISUALIZE | GENERATEHYBRIDMODEL | READFILE | SETVOLUME
       | INCLUDE_MODEL | INCLUDE_NETWORK)
      LPAREN (action_args | action_arg_value)? RPAREN SEMI? LB*
    ;

// Action arguments can be: {key=>val,...} or simple quoted string
action_args
    : LBRACKET action_arg_list? RBRACKET
    ;

action_arg_list
    : action_arg (COMMA action_arg)*
    ;

action_arg
    : arg_name ASSIGNS action_arg_value
    ;

// Action arg values can be: expression, quoted string (double or single), array, nested hash, or keyword
action_arg_value
    : expression
    | keyword_as_value  // NEW: Allow keywords like 'ode', 'ssa' as unquoted values
    | quoted_string
    | LSBRACKET expression_list RSBRACKET
    | LBRACKET nested_hash_list? RBRACKET
    ;

quoted_string
    : QUOTED_STRING
    | SINGLE_QUOTED_STRING
    ;

// Keywords that can be used as action argument values (e.g., suffix=>ode)
keyword_as_value
    : ODE | SSA | NF | PLA | SPARSE | VERBOSE | OVERWRITE | CONTINUE
    | SAFE | EXECUTE | BINARY_OUTPUT | STEADY_STATE | BDF | STIFF
    | METHOD | TRUE | FALSE
    ;

// Nested hash list for things like max_stoich=>{APC=>1,AXIN=>1}
nested_hash_list
    : nested_hash_item (COMMA nested_hash_item)*
    ;

nested_hash_item
    : (STRING | quoted_string | arg_name) ASSIGNS action_arg_value
    ;

// Allow keywords to be used as argument names in action calls
// This includes all possible BNG2.pl action options for full compatibility
arg_name
    : STRING
    // Expression identifiers
    | TIME
    // generate_network options
    | OVERWRITE | MAX_AGG | MAX_ITER | MAX_STOICH | PRINT_ITER | CHECK_ISO
    // simulate options
    | METHOD | T_START | T_END | N_STEPS | N_OUTPUT_STEPS | ATOL | RTOL | STEADY_STATE | SPARSE
    | VERBOSE | NETFILE | CONTINUE | PREFIX | SUFFIX | FORMAT | FILE
    // Additional simulate options
    | PRINT_CDAT | PRINT_FUNCTIONS | PRINT_NET | PRINT_END | STOP_IF | PRINT_ON_STOP
    | SAVE_PROGRESS | MAX_SIM_STEPS | OUTPUT_STEP_INTERVAL | SAMPLE_TIMES
    // SSA/PLA options
    | PLA_CONFIG | PLA_OUTPUT
    // SSA/PSA options
    | SEED | POPLEVEL | ARGFILE
    // NFsim options
    | PARAM | COMPLEX | GET_FINAL_STATE | GML | NOCSLF | NOTF | BINARY_OUTPUT | UTL | EQUIL | NFSIM_EXEC
    // Hybrid model options
    | MOL_THRESHOLD
    // Parameter scan options
    | PARAMETER | PAR_MIN | PAR_MAX | N_SCAN_PTS | LOG_SCALE | RESET_CONC
    // Mfile options
    | BDF | MAX_STEP | MAXORDER | STATS | MAX_NUM_STEPS | MAX_ERR_TEST_FAILS | MAX_CONV_FAILS | STIFF
    // Read/write options
    | ATOMIZE | BLOCKS | SKIPACTIONS | INCLUDE_MODEL | INCLUDE_NETWORK | PRETTY_FORMATTING | EVALUATE_EXPRESSIONS
    // Visualize options
    | TYPE | BACKGROUND | COLLAPSE | OPTS
    // Safe/execute
    // Safe/execute
    | SAFE | EXECUTE
    | TEXTREACTION | TEXTSPECIES
    ;

expression_list
    : expression (COMMA expression)*
    ;

// Expressions
expression
    : conditional_expr
    ;

conditional_expr
    : or_expr
    ;

or_expr
    : and_expr (LOGICAL_OR and_expr)*
    ;

and_expr
    : equality_expr (LOGICAL_AND equality_expr)*
    ;

equality_expr
    : relational_expr ((EQUALS | NOT_EQUALS | GTE | GT | LTE | LT) relational_expr)*
    ;

relational_expr
    : additive_expr
    ;

additive_expr
    : multiplicative_expr ((PLUS | MINUS) multiplicative_expr)*
    ;

multiplicative_expr
    : power_expr ((TIMES | DIV | MOD) power_expr)*
    ;

power_expr
    : unary_expr (POWER unary_expr)*
    ;

unary_expr
    : (PLUS | MINUS | EMARK | TILDE)? primary_expr  // EMARK/TILDE for logical NOT
    ;

primary_expr
    : LPAREN expression RPAREN
    | function_call
    | observable_ref
    | literal
    | arg_name  // Allow keywords like t_end to be used as variable identifiers
    ;

function_call
    : (EXP | LN | LOG10 | LOG2 | SQRT | ABS | SIN | COS | TAN | ASIN | ACOS | ATAN 
       | SINH | COSH | TANH | ASINH | ACOSH | ATANH | RINT | MIN | MAX | SUM | AVG
       | IF | SAT | MM | HILL | ARRHENIUS | TIME
       | MRATIO | TFUN | FUNCTIONPRODUCT)  // BNG2 parity: mratio, TFUN, FunctionProduct
      LPAREN expression_list? RPAREN
    ;

observable_ref
    : STRING LPAREN expression_list? RPAREN
    ;

literal
    : INT
    | FLOAT
    | PI
    | EULERIAN
    ;
