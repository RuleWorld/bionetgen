
// Generated from src/parser/BNGLexer.g4 by ANTLR 4.13.1

#pragma once


#include "antlr4-runtime.h"




class  BNGLexer : public antlr4::Lexer {
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

  explicit BNGLexer(antlr4::CharStream *input);

  ~BNGLexer() override;


  std::string getGrammarFileName() const override;

  const std::vector<std::string>& getRuleNames() const override;

  const std::vector<std::string>& getChannelNames() const override;

  const std::vector<std::string>& getModeNames() const override;

  const antlr4::dfa::Vocabulary& getVocabulary() const override;

  antlr4::atn::SerializedATNView getSerializedATN() const override;

  const antlr4::atn::ATN& getATN() const override;

  bool sempred(antlr4::RuleContext *_localctx, size_t ruleIndex, size_t predicateIndex) override;

  // By default the static state used to implement the lexer is lazily initialized during the first
  // call to the constructor. You can call this function if you wish to initialize the static state
  // ahead of time.
  static void initialize();

private:

  // Individual action functions triggered by action() above.

  // Individual semantic predicate functions triggered by sempred() above.
  bool FLOATSempred(antlr4::RuleContext *_localctx, size_t predicateIndex);

};

