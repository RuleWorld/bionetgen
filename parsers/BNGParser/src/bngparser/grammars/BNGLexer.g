lexer grammar BNGLexer;

options {

  language = Java;
}

@header{
package bngparser.grammars;
}

LINE_COMMENT : '#' ~('\n'|'\r')* {skip();} ;
LB : ('\n'|'\r')+; 
WS : (' '|'\t'|'\\\n'|'\\\r\n')+ { $channel=HIDDEN;};
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

SET_OPTION : 'SETOPTION';
SPECIES_LABEL : 'SPECIESLABEL';
GENERATE_NETWORK : 'GENERATENETWORK';
MAX_AGG : 'MAXAGG';
MAX_ITER : ('M'|'m')('A'|'a')('X'|'x')'_'('I'|'i')('T'|'t')('E'|'e')('R'|'r');
MAX_STOICH : 'MAXSTOICH';
OVERWRITE: 'overwrite';
PRINT_ITER : 'PRINTITER';
AND : 'AND';
OR : 'OR';
ID : 'ID';
IF : 'if';
PREFFIX : 'PREFFIX';
SUFFIX : 'SUFFIX'|'suffix';
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

ATOL: 'atol';
RTOL: 'rtol';
STEADY_STATE: 'steady_state';
SPARSE: 'sparse';
T_END: 't_end';
T_START: 't_start';
N_STEPS: 'n_steps';
PLA_CONFIG: 'pla_config';
SAMPLE_TIMES: 'sample_times';
SIMULATE_ODE: 'simulate_ode';
SIMULATE_SSA: 'simulate_ssa';
SIMULATE_NF: 'simulate_nf';
SIMULATE_PLA: 'simulate_pla';
READFILE: 'readFile';
FILE: 'file';
WRITENET: 'writeNET';
WRITESBML: 'writeSBML';
WRITEMFILE: 'writeMfile';
WRITEXML: 'writeXML';
SETCONCENTRATION: 'setConcentration';
SETPARAMETER: 'setParameter';
SAVECONCENTRATIONS: 'saveConcentrations';
RESETCONCENTRATIONS: 'resetConcentrations';
GENERATENETWORK: 'generate_network';
SUBSTANCEUNITS: 'substanceUnits';
PARAM: 'param';
SAT: 'Sat';
EXP: 'exp';

FLOAT:
  (DIGIT)+ '.' (DIGIT)* EXPONENT?
| '.' (DIGIT)+ EXPONENT?
| (DIGIT)+ EXPONENT;
INT: DIGIT+;
STRING: LETTER (LETTER | DIGIT | '_')*;


SEMI : ';';
COLON: ':';
LSBRACKET : '[';
RSBRACKET : ']';
LBRACKET : '{';
RBRACKET : '}';
COMMA : ',';
DOT : '.';
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


VERSION_NUMBER : (DIGIT) | (DIGIT'.'DIGIT) | (DIGIT'.'DIGIT'.'DIGIT) ;




