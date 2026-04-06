
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
    BIFURCATE = 98, LINEAR_PARAMETER_SENSITIVITY = 99, PARAMETER = 100, 
    PAR_MIN = 101, PAR_MAX = 102, N_SCAN_PTS = 103, LOG_SCALE = 104, RESET_CONC = 105, 
    READFILE = 106, FILE = 107, ATOMIZE = 108, BLOCKS = 109, SKIPACTIONS = 110, 
    VISUALIZE = 111, TYPE = 112, BACKGROUND = 113, COLLAPSE = 114, OPTS = 115, 
    WRITESSC = 116, WRITESSCCFG = 117, FORMAT = 118, WRITEFILE = 119, WRITEMODEL = 120, 
    WRITEXML = 121, WRITENETWORK = 122, WRITESBML = 123, WRITESBMLMULTI = 124, 
    WRITEMDL = 125, WRITELATEX = 126, INCLUDE_MODEL = 127, INCLUDE_NETWORK = 128, 
    PRETTY_FORMATTING = 129, EVALUATE_EXPRESSIONS = 130, TEXTREACTION = 131, 
    TEXTSPECIES = 132, WRITEMFILE = 133, WRITEMEXFILE = 134, WRITECPPFILE = 135, 
    WRITECPYFILE = 136, BDF = 137, MAX_STEP = 138, MAXORDER = 139, STATS = 140, 
    MAX_NUM_STEPS = 141, MAX_ERR_TEST_FAILS = 142, MAX_CONV_FAILS = 143, 
    STIFF = 144, SETCONCENTRATION = 145, ADDCONCENTRATION = 146, SAVECONCENTRATIONS = 147, 
    RESETCONCENTRATIONS = 148, SETPARAMETER = 149, SAVEPARAMETERS = 150, 
    RESETPARAMETERS = 151, SETVOLUME = 152, SIMULATE_PSA = 153, POPLEVEL = 154, 
    MOL_THRESHOLD = 155, NFSIM_EXEC = 156, QUIT = 157, TRUE = 158, FALSE = 159, 
    SAT = 160, MM = 161, HILL = 162, ARRHENIUS = 163, MRATIO = 164, TFUN = 165, 
    FUNCTIONPRODUCT = 166, PRIORITY = 167, IF = 168, EXP = 169, LN = 170, 
    LOG10 = 171, LOG2 = 172, SQRT = 173, RINT = 174, ABS = 175, SIN = 176, 
    COS = 177, TAN = 178, ASIN = 179, ACOS = 180, ATAN = 181, SINH = 182, 
    COSH = 183, TANH = 184, ASINH = 185, ACOSH = 186, ATANH = 187, PI = 188, 
    EULERIAN = 189, MIN = 190, MAX = 191, SUM = 192, AVG = 193, TIME = 194, 
    FLOAT = 195, INT = 196, STRING = 197, QUOTED_STRING = 198, SINGLE_QUOTED_STRING = 199, 
    SEMI = 200, COLON = 201, LSBRACKET = 202, RSBRACKET = 203, LBRACKET = 204, 
    RBRACKET = 205, COMMA = 206, DOT = 207, LPAREN = 208, RPAREN = 209, 
    UNI_REACTION_SIGN = 210, BI_REACTION_SIGN = 211, DOLLAR = 212, TILDE = 213, 
    AT = 214, GTE = 215, GT = 216, LTE = 217, LT = 218, ASSIGNS = 219, EQUALS = 220, 
    NOT_EQUALS = 221, BECOMES = 222, LOGICAL_AND = 223, LOGICAL_OR = 224, 
    LOGICAL_XOR = 225, DIV = 226, TIMES = 227, MINUS = 228, PLUS = 229, 
    POWER = 230, MOLECULE_TAG_TOKEN = 231, MOD = 232, PIPE = 233, QMARK = 234, 
    EMARK = 235, AMPERSAND = 236, VERSION_NUMBER = 237, ULB = 238
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

