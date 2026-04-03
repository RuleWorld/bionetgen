/**
 * ANTLR4 Lexer Grammar for BNGL
 * Ported from RuleWorld/BNGParser (ANTLR3)
 * https://github.com/RuleWorld/BNGParser
 */
lexer grammar BNGLexer;

// Comments and whitespace
LINE_COMMENT: '#' ~[\r\n]* -> skip;
LB: [\r\n]+;
WS: [ \t]+ -> channel(HIDDEN);

// Keywords - Blocks
BEGIN: 'begin';
END: 'end';
MODEL: 'model';
PARAMETERS: 'parameters';
COMPARTMENTS: 'compartments';
MOLECULE: [mM] 'olecule';
MOLECULES: MOLECULE 's';
COUNTER: 'Counter';
TYPES: 'types';
SEED: 'seed';
SPECIES: [sS] 'pecies';
OBSERVABLES: 'observables';
FUNCTIONS: 'functions';
REACTION: 'reaction';
REACTIONS: REACTION 's';
RULES: 'rules';
// Support underscore variants: reaction_rules, molecule_types
REACTION_RULES: 'reaction_rules';
MOLECULE_TYPES: 'molecule_types';
GROUPS: 'groups';
ACTIONS: 'actions';
POPULATION: 'population';
MAPS: 'maps';
ENERGY: 'energy';
PATTERNS: 'patterns';

MOLECULAR: 'molecular';

// Keywords - Modifiers
MATCHONCE: 'MatchOnce';
DELETEMOLECULES: 'DeleteMolecules';
MOVECONNECTED: 'MoveConnected';
INCLUDE_REACTANTS: 'include_reactants';
INCLUDE_PRODUCTS: 'include_products';
EXCLUDE_REACTANTS: 'exclude_reactants';
EXCLUDE_PRODUCTS: 'exclude_products';
TOTALRATE: 'TotalRate';

// Keywords - Header
VERSION: 'version';
SET_OPTION: 'setOption';
SET_MODEL_NAME: 'setModelName';
SUBSTANCEUNITS: 'substanceUnits';

// Keywords - Actions
PREFIX: 'prefix';
SUFFIX: 'suffix';

GENERATENETWORK: 'generate_network';
OVERWRITE: 'overwrite';
MAX_AGG: 'max_agg';
MAX_ITER: 'max_iter';
MAX_STOICH: 'max_stoich';
PRINT_ITER: 'print_iter';
CHECK_ISO: 'check_iso';

GENERATEHYBRIDMODEL: 'generate_hybrid_model';
SAFE: 'safe';
EXECUTE: 'execute';

SIMULATE: 'simulate';
METHOD: 'method';
ODE: 'ode';
SSA: 'ssa';
PLA: 'pla';
NF: 'nf';
VERBOSE: 'verbose';
NETFILE: 'netfile';
ARGFILE: 'argfile';
CONTINUE: 'continue';
T_START: 't_start';
T_END: 't_end';
N_STEPS: 'n_steps';
N_OUTPUT_STEPS: 'n_output_steps';
MAX_SIM_STEPS: 'max_sim_steps';
OUTPUT_STEP_INTERVAL: 'output_step_interval';
SAMPLE_TIMES: 'sample_times';
SAVE_PROGRESS: 'save_progress';
PRINT_CDAT: 'print_CDAT';
PRINT_FUNCTIONS: 'print_functions';
PRINT_NET: 'print_net';
PRINT_END: 'print_end';
STOP_IF: 'stop_if';
PRINT_ON_STOP: 'print_on_stop';

SIMULATE_ODE: 'simulate_ode';
ATOL: 'atol';
RTOL: 'rtol';
STEADY_STATE: 'steady_state';
SPARSE: 'sparse';

SIMULATE_SSA: 'simulate_ssa';

SIMULATE_PLA: 'simulate_pla';
PLA_CONFIG: 'pla_config';
PLA_OUTPUT: 'pla_output';

SIMULATE_NF: 'simulate_nf';
SIMULATE_RM: 'simulate_rm';
PARAM: 'param';
COMPLEX: 'complex';
GET_FINAL_STATE: 'get_final_state';
GML: 'gml';
NOCSLF: 'nocslf';
NOTF: 'notf';
BINARY_OUTPUT: 'binary_output';
UTL: 'utl';
EQUIL: 'equil';

PARAMETER_SCAN: 'parameter_scan';
BIFURCATE: 'bifurcate';
PARAMETER: 'parameter';
PAR_MIN: 'par_min';
PAR_MAX: 'par_max';
N_SCAN_PTS: 'n_scan_pts';
LOG_SCALE: 'log_scale';
RESET_CONC: 'reset_conc';

READFILE: 'readFile';
FILE: 'file';
ATOMIZE: 'atomize';
BLOCKS: 'blocks';
SKIPACTIONS: 'skip_actions';

// Visualize commands
VISUALIZE: 'visualize';
TYPE: 'type';
BACKGROUND: 'background';
COLLAPSE: 'collapse';
OPTS: 'opts';

// Write commands
WRITESSC: 'writeSSC';
WRITESSCCFG: 'writeSSCcfg';
FORMAT: 'format';
WRITEFILE: 'writeFile';
WRITEMODEL: 'writeModel';
WRITEXML: 'writeXML';
WRITENETWORK: 'writeNetwork';
WRITESBML: 'writeSBML';
WRITEMDL: 'writeMDL';
WRITELATEX: 'writeLatex';
INCLUDE_MODEL: 'include_model';
INCLUDE_NETWORK: 'include_network';
PRETTY_FORMATTING: 'pretty_formatting';
EVALUATE_EXPRESSIONS: 'evaluate_expressions';
TEXTREACTION: 'TextReaction';
TEXTSPECIES: 'TextSpecies';

WRITEMFILE: 'writeMfile';
WRITEMEXFILE: 'writeMexfile';
WRITECPPFILE: 'writeCPPfile';
WRITECPYFILE: 'writeCPYfile';
BDF: 'bdf';
MAX_STEP: 'max_step';
MAXORDER: 'maxOrder';
STATS: 'stats';
MAX_NUM_STEPS: 'max_num_steps';
MAX_ERR_TEST_FAILS: 'max_err_test_fails';
MAX_CONV_FAILS: 'max_conv_fails';
STIFF: 'stiff';

SETCONCENTRATION: 'setConcentration';
ADDCONCENTRATION: 'addConcentration';
SAVECONCENTRATIONS: 'saveConcentrations';
RESETCONCENTRATIONS: 'resetConcentrations';
SETPARAMETER: 'setParameter';
SAVEPARAMETERS: 'saveParameters';
RESETPARAMETERS: 'resetParameters';
SETVOLUME: 'setVolume';
SIMULATE_PSA: 'simulate_psa';
QUIT: 'quit';

// Boolean literals (text form only - 0/1 handled as INT in expressions)
TRUE: 'true';
FALSE: 'false';

// Math functions
SAT: 'Sat';
MM: 'MM';
HILL: 'Hill';
ARRHENIUS: 'Arrhenius';

// BNG2 parity: mratio (Kummer's function), TFUN (table function), FunctionProduct
MRATIO: 'mratio';
TFUN: 'TFUN';
FUNCTIONPRODUCT: 'FunctionProduct';

// BNG2 parity: reaction priority modifier
PRIORITY: 'priority';

// POPULATION already defined above at line 34

IF: 'if';
EXP: 'exp';
LN: 'ln';
LOG10: 'log10';
LOG2: 'log2';
SQRT: 'sqrt';
RINT: 'rint';
ABS: 'abs';
SIN: 'sin';
COS: 'cos';
TAN: 'tan';
ASIN: 'asin';
ACOS: 'acos';
ATAN: 'atan';
SINH: 'sinh';
COSH: 'cosh';
TANH: 'tanh';
ASINH: 'asinh';
ACOSH: 'acosh';
ATANH: 'atanh';
PI: '_pi';
EULERIAN: '_e';
MIN: 'min';
MAX: 'max';
SUM: 'sum';
AVG: 'avg';
TIME: 'time';

// Literals - FLOAT supports: 1.0, .5, 1e-5, 1.0e5, .01, 1D-10 (Fortran)
// Support bare 1. only if not followed by letter (to avoid matching 1.EGFR as float)
FLOAT
    : DIGIT+ '.' DIGIT+ EXPONENT_ALL?
    | '.' DIGIT+ EXPONENT_ALL?
    | DIGIT+ EXPONENT_ALL
    | DIGIT+ '.' { _input->LA(1) < 'a' || _input->LA(1) > 'z' && _input->LA(1) < 'A' || _input->LA(1) > 'Z' && _input->LA(1) != '_' }?
    ;
INT: DIGIT+;
STRING: (LETTER | '_') (LETTER | DIGIT | '_')*;
QUOTED_STRING: '"' (~["\r\n])* '"';
SINGLE_QUOTED_STRING: '\'' (~['\r\n])* '\'';

// Punctuation
SEMI: ';';
COLON: ':';
LSBRACKET: '[';
RSBRACKET: ']';
LBRACKET: '{';
RBRACKET: '}';
COMMA: ',';
DOT: '.';
LPAREN: '(';
RPAREN: ')';
UNI_REACTION_SIGN: '->';
BI_REACTION_SIGN: '<->';
DOLLAR: '$';
TILDE: '~';
AT: '@';
GTE: '>=';
GT: '>';
LTE: '<=';
LT: '<';
ASSIGNS: '=>';
EQUALS: '==';
NOT_EQUALS: '!=' | '~=';
BECOMES: '=';
LOGICAL_AND: '&&';
LOGICAL_OR: '||';
// Note: '!' is EMARK (used for bonds and logical NOT - context dependent)
DIV: '/';
TIMES: '*';
MINUS: '-';
PLUS: '+';
POWER: '^' | '**';
MOD: '%';
PIPE: '|';
QMARK: '?';
EMARK: '!';
AMPERSAND: '&';

// Fragments
fragment DIGIT: [0-9];
fragment LETTER: [a-zA-Z];
fragment EXPONENT: [eE] [+-]? DIGIT+;
fragment EXPONENT_ALL: [eEdDfFgG] [+-]? DIGIT+;  // Includes Fortran-style exponents

// Version number
VERSION_NUMBER: INT '.' INT '.' INT [+-]?;

// Line continuation
ULB: '\\' [ ]* '\r'? '\n' WS* -> skip;
