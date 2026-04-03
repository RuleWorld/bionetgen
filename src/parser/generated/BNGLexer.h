
// Generated from BNGLexer.g4 by ANTLR 4.13.2

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
    WRITECPPFILE = 133, WRITECPYFILE = 134, BDF = 135, MAX_STEP = 136, MAXORDER = 137, 
    STATS = 138, MAX_NUM_STEPS = 139, MAX_ERR_TEST_FAILS = 140, MAX_CONV_FAILS = 141, 
    STIFF = 142, SETCONCENTRATION = 143, ADDCONCENTRATION = 144, SAVECONCENTRATIONS = 145, 
    RESETCONCENTRATIONS = 146, SETPARAMETER = 147, SAVEPARAMETERS = 148, 
    RESETPARAMETERS = 149, SETVOLUME = 150, SIMULATE_PSA = 151, QUIT = 152, 
    TRUE = 153, FALSE = 154, SAT = 155, MM = 156, HILL = 157, ARRHENIUS = 158, 
    MRATIO = 159, TFUN = 160, FUNCTIONPRODUCT = 161, PRIORITY = 162, IF = 163, 
    EXP = 164, LN = 165, LOG10 = 166, LOG2 = 167, SQRT = 168, RINT = 169, 
    ABS = 170, SIN = 171, COS = 172, TAN = 173, ASIN = 174, ACOS = 175, 
    ATAN = 176, SINH = 177, COSH = 178, TANH = 179, ASINH = 180, ACOSH = 181, 
    ATANH = 182, PI = 183, EULERIAN = 184, MIN = 185, MAX = 186, SUM = 187, 
    AVG = 188, TIME = 189, FLOAT = 190, INT = 191, STRING = 192, QUOTED_STRING = 193, 
    SINGLE_QUOTED_STRING = 194, SEMI = 195, COLON = 196, LSBRACKET = 197, 
    RSBRACKET = 198, LBRACKET = 199, RBRACKET = 200, COMMA = 201, DOT = 202, 
    LPAREN = 203, RPAREN = 204, UNI_REACTION_SIGN = 205, BI_REACTION_SIGN = 206, 
    DOLLAR = 207, TILDE = 208, AT = 209, GTE = 210, GT = 211, LTE = 212, 
    LT = 213, ASSIGNS = 214, EQUALS = 215, NOT_EQUALS = 216, BECOMES = 217, 
    LOGICAL_AND = 218, LOGICAL_OR = 219, DIV = 220, TIMES = 221, MINUS = 222, 
    PLUS = 223, POWER = 224, MOD = 225, PIPE = 226, QMARK = 227, EMARK = 228, 
    AMPERSAND = 229, VERSION_NUMBER = 230, ULB = 231
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

