lexer grammar BNGLexer;

options{
  language = Java;
}
@header{
  package bngparser.grammars;
}
@members{
  public boolean floatLA(){
    int counter = 1;
    while(true){
      int LA8_0 = input.LT(counter);
      if ((LA8_0>='0' && LA8_0<='9')){
        counter++;
      }
      else{
        break;
      }
    }
    int LA14_0 = input.LT(counter);
    if((LA14_0>='A' && LA14_0<='Z')||LA14_0=='_'||(LA14_0>='a' && LA14_0<='z'))
      return false;
    return true;
  }
}

LINE_COMMENT: '#' ~('\n'|'\r')* {skip();};
LB: ('\n'|'\r')+; 
WS: (' '|'\t')+ { $channel=HIDDEN;};

BEGIN: 'begin';
END: 'end';
MODEL: 'model';
PARAMETERS: 'parameters';
COMPARTMENTS: 'compartments';
MOLECULE: ('m'|'M')'olecule';
MOLECULES: MOLECULE's';
TYPES: 'types';
SEED: 'seed';
SPECIES: ('s'|'S')'pecies';
OBSERVABLES: 'observables';
FUNCTIONS: 'functions';
REACTION: 'reaction';
REACTIONS: REACTION's';
RULES: 'rules';
GROUPS: 'groups';
ACTIONS: 'actions';
POPULATION: 'population';
MAPS: 'maps';
ENERGY: 'energy';
PATTERNS: 'patterns';

MATCHONCE: 'MatchOnce';
DELETEMOLECULES: 'DeleteMolecules';
MOVECONNECTED: 'MoveConnected';
INCLUDE_REACTANTS: 'include_reactants'; // To be deprecated
INCLUDE_PRODUCTS: 'include_products';
EXCLUDE_REACTANTS: 'exclude_reactants'; // To be deprecated
EXCLUDE_PRODUCTS: 'exclude_products';

VERSION: 'version';
SET_OPTION: 'setOption';
SET_MODEL_NAME: 'setModelName';
SUBSTANCEUNITS: 'substanceUnits';

PREFIX: 'prefix';
SUFFIX: 'suffix';

GENERATENETWORK: 'generate_network';
OVERWRITE: 'overwrite';
MAX_AGG: 'max_agg';
MAX_ITER: 'max_iter';
MAX_STOICH: 'max_stoich';
PRINT_ITER: 'print_iter';

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

//WRITEFILE: 'writeFile';
//WRITENET: 'writeNET';
WRITEMODEL: 'writeModel';
WRITEXML: 'writeXML';
WRITENETWORK: 'writeNetwork';
WRITESBML: 'writeSBML';
WRITEMDL: 'writeMDL';
INCLUDE_MODEL: 'include_model';
PRETTY_FORMATTING: 'pretty_formatting';
EVALUATE_EXPRESSIONS: 'evaluate_expressions';
TEXTREACTION: 'TextReaction';
TEXTSPECIES: 'TextSpecies';

WRITEMFILE: 'writeMfile';
WRITEMEXFILE: 'writeMexfile';
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
QUIT: 'quit';

//ELE: 'Ele';
SAT: 'Sat';
MM: 'MM';
HILL: 'Hill';
ARRHENIUS: 'Arrhenius';

IF: 'if';
EXP: 'exp';
//LOG: 'log'; deprecated
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
EULERIAN:  '_e';
MIN: 'min';
MAX: 'max';
SUM: 'sum';
AVG: 'avg';
TIME: 'time';

FLOAT:
  (DIGIT)+ '.' (DIGIT)* EXPONENT?
| (DIGIT)+ EXPONENT;
INT: DIGIT+;
STRING: (LETTER | '_') (LETTER | DIGIT | '_')*;
SEMI: ';';
COLON: ':';
LSBRACKET: '[';
RSBRACKET: ']';
LBRACKET: '{';
RBRACKET: '}';
COMMA: ',';
DOT: '.'   ({floatLA()}? => ((DIGIT)+ EXPONENT?  {$type=FLOAT;}) | {$type=DOT;});
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
BECOMES: '=';
DIV: '/';
TIMES: '*';
MINUS: '-';
PLUS: '+';
POWER: '^';
MOD: '%';
PIPE: '|';
QMARK: '?';
EMARK: '!';
DBQUOTES: '"';
AMPERSAND: '&';

fragment NULL: '0';
fragment DIGIT: NULL |'1'..'9';
fragment LETTER: LOWER | UPPER;
fragment LOWER: 'a'..'z';
fragment UPPER: 'A'..'Z';
fragment EXPONENT: ('e'|'E') ('+'|'-')? (DIGIT)+ ;

VERSION_NUMBER : INT'.'INT'.'INT(PLUS|MINUS)? ;
ULB:('\\'(' ')*'\r'?'\n'(WS)*) {skip();};
