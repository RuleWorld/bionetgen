lexer grammar BNGLexer;

options {

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

LINE_COMMENT : '#' ~('\n'|'\r')* {skip();} ;
LB : ('\n'|'\r')+; 
WS : (' '|'\t'|('\\')(' ')*('\r')?('\n'))+ { $channel=HIDDEN;};
VERSION : ('v'|'V')('e'|'E')('r'|'R')('s'|'S')('i'|'I')('o'|'O')('n'|'N');
END : ('e'|'E')('n'|'N')('d'|'D');
BEGIN : ('b'|'B')('e'|'E')('g'|'G')('i'|'I')('n'|'N');
SEED : ('s'|'S')('e'|'E')('e'|'E')('d'|'D');
SPECIES : ('s'|'S')('p'|'P')('e'|'E')('c'|'C')('i'|'I')('e'|'E')('s'|'S');
GROUPS : ('G'|'g')('R'|'r')('O'|'o')('U'|'u')('P'|'p')('S'|'s');
MOLECULE : ('m'|'M')('o'|'O')('l'|'L')('e'|'E')('c'|'C')('u'|'l')('l'|'L')('e'|'E');
MOLECULES : MOLECULE('s'|'S');
MODEL: ('m'|'M')('o'|'O')('d'|'D')('e'|'E')('l'|'L');
TYPES : ('t'|'T')('y'|'Y')('p'|'P')('e'|'E')('s'|'S');
REACTION : ('r'|'R')('e'|'E')('a'|'A')('c'|'C')('t'|'T')('i'|'I')('o'|'O')('n'|'N');
REACTIONS : REACTION('s'|'S');
RULES : ('r'|'R')('u'|'U')('l'|'L')('e'|'E')('s'|'S');
FUNCTIONS : ('F'|'f')('U'|'u')('N'|'n')('C'|'c')('T'|'t')('I'|'i')('O'|'o')('n'|'N')('S'|'s');
COMPARTMENTS : ('C'|'c')('O'|'o')('M'|'m')('P'|'p')('A'|'a')('R'|'r')('T'|'t')('M'|'m')('E'|'e')('N'|'n')('T'|'t')('S'|'s');
ACTIONS: ('A'|'a')('C'|'c')('T'|'t')('i'|'I')('o'|'O')('n'|'N')('S'|'s');
OBSERVABLES: 'observables';
POPULATION: 'population';
MAPS: 'maps';

SET_OPTION : 'setOption';
SPECIES_LABEL : 'SPECIESLABEL';
MAX_AGG : 'max_agg';
MAX_ITER : 'max_iter';
MAX_STOICH : 'max_stoich';
OVERWRITE: 'overwrite';
PRINT_ITER : 'print_iter';
AND : 'AND';
OR : 'OR';
ID : 'ID';
IF : 'if';
PREFIX : 'prefix';
SUFFIX : 'suffix';
LABEL : 'LABEL';
PARAMETERS : ('p'|'P')('a'|'A')('r'|'R')('a'|'A')('m'|'M')('e'|'E')('t'|'T')('e'|'E')('r'|'R')('s'|'S');
VERBOSE : ('V'|'v')('E'|'e')('R'|'r')('B'|'b')('O'|'o')('S'|'s')('E '|'e');

MATCHONCE: 'MatchOnce';
DELETEMOLECULES: 'DeleteMolecules';
MOVECONNECTED: 'MoveConnected';
AUTO : 'Auto';
HNAUTY : 'Hnauty';
QUASY : 'Quasy';
NOT: 'not';
INCLUDE_REACTANTS: 'include_reactants';
INCLUDE_PRODUCTS: 'include_products';
EXCLUDE_REACTANTS: 'exclude_reactants';
EXCLUDE_PRODUCTS: 'exclude_products';
GENERATEHYBRIDMODEL: 'generate_hybrid_model';
EXACT: 'exact';
EXECUTE: 'execute';
ATOL: 'atol';
RTOL: 'rtol';
STEADY_STATE: 'steady_state';
SPARSE: 'sparse';
T_END: 't_end';
T_START: 't_start';
N_STEPS: 'n_steps';
N_OUTPUT_STEPS: 'n_output_steps';
MAX_SIM_STEPS: 'max_sim_steps';
OUTPUT_STEP_INTERVAL: 'output_step_interval';
PLA_CONFIG: 'pla_config';
SAMPLE_TIMES: 'sample_times';
SIMULATE_ODE: 'simulate_ode';
SIMULATE_SSA: 'simulate_ssa';
SIMULATE_NF: 'simulate_nf';
SIMULATE_PLA: 'simulate_pla';
SIMULATE: 'simulate';
READFILE: 'readFile';
FILE: 'file';
WRITENET: 'writeNET';
WRITESBML: 'writeSBML';
WRITEMFILE: 'writeMfile';
WRITEXML: 'writeXML';
WRITEMEXFILE: 'writeMex'('F'|'f')'ile';
PRINT_CDAT: 'print_CDAT';
PRINT_FUNCTIONS: 'print_functions';
SETCONCENTRATION: 'setConcentration';
METHOD: 'method';
SETPARAMETER: 'setParameter';
SAVECONCENTRATIONS: 'saveConcentrations';
RESETCONCENTRATIONS: 'resetConcentrations';
ADDCONCENTRATION: 'addConcentration';
GENERATENETWORK: 'generate_network';
WRITENETWORK: 'writeNetwork';
SUBSTANCEUNITS: 'substanceUnits';
PARAM: 'param';
COMPLEX: 'complex';
GET_FINAL_STATE: 'get_final_state';
GML: 'gml';
NOCSLF: 'nocslf';
NOTF: 'notf';
BINARY_OUTPUT: 'binary_output';
UTL: 'utl';
EQUIL: 'equil';
//ELE: 'Ele';
SAT: 'Sat';
MM: 'MM';
HILL: 'Hill';
ARRHENIUS: 'Arrhenius';
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
ARGFILE: 'argfile';
SAVE_PROGRESS: 'save_progress';
NETFILE: 'netfile';
PRINT_NET: 'print_net';
PRINT_END: 'print_end';
TEXTREACTION: 'TextReaction';
CONTINUE: 'continue';
ODE: 'ode';
SSA: 'ssa';
NF: 'nf';
PLA: 'pla';
EVALUATE_EXPRESSIONS: 'evaluate_expressions';
BDF: 'bdf';
FLOAT:
  (DIGIT)+ '.' (DIGIT)* EXPONENT?
| (DIGIT)+ EXPONENT;
INT: DIGIT+;
STRING: (LETTER | DIGIT | '_')+;
SEMI : ';';
COLON: ':';
LSBRACKET : '[';
RSBRACKET : ']';
LBRACKET : '{';
RBRACKET : '}';
COMMA : ',';
DOT : '.'   ({floatLA()}? => ((DIGIT)+ EXPONENT?  {$type=FLOAT;}) | {$type=DOT;});
LPAREN : '(';
RPAREN : ')';
UNI_REACTION_SIGN : '->';
BI_REACTION_SIGN : '<->';
DOLLAR : '$';
TILDE : '~';
AT : '@';
GTE : '>=';
GT : '>';
LTE : '<=';
LT : '<';
ASSIGNS : '=>';
EQUALS : '==';
BECOMES : '=';
DIV : '/';
TIMES : '*';
MINUS : '-';
PLUS : '+';
POWER: '^';
MOD : '%';
PIPE : '|';
QMARK : '?';
EMARK: '!';
DBQUOTES : '"';
AMPERSAND : '&';

fragment NULL: '0';
fragment DIGIT: NULL |'1'..'9';
fragment LETTER: LOWER | UPPER;
fragment LOWER: 'a'..'z';
fragment UPPER: 'A'..'Z';
fragment EXPONENT : ('e'|'E') ('+'|'-')? (DIGIT)+ ;

VERSION_NUMBER : INT'.'INT'.'INT ;

