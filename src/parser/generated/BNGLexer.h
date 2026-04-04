
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
    WRITESBML = 122, WRITESBMLMULTI = 123, WRITEMDL = 124, WRITELATEX = 125, 
    INCLUDE_MODEL = 126, INCLUDE_NETWORK = 127, PRETTY_FORMATTING = 128, 
    EVALUATE_EXPRESSIONS = 129, TEXTREACTION = 130, TEXTSPECIES = 131, WRITEMFILE = 132, 
    WRITEMEXFILE = 133, WRITECPPFILE = 134, WRITECPYFILE = 135, BDF = 136, 
    MAX_STEP = 137, MAXORDER = 138, STATS = 139, MAX_NUM_STEPS = 140, MAX_ERR_TEST_FAILS = 141, 
    MAX_CONV_FAILS = 142, STIFF = 143, SETCONCENTRATION = 144, ADDCONCENTRATION = 145, 
    SAVECONCENTRATIONS = 146, RESETCONCENTRATIONS = 147, SETPARAMETER = 148, 
    SAVEPARAMETERS = 149, RESETPARAMETERS = 150, SETVOLUME = 151, SIMULATE_PSA = 152, 
    POPLEVEL = 153, MOL_THRESHOLD = 154, NFSIM_EXEC = 155, QUIT = 156, TRUE = 157, 
    FALSE = 158, SAT = 159, MM = 160, HILL = 161, ARRHENIUS = 162, MRATIO = 163, 
    TFUN = 164, FUNCTIONPRODUCT = 165, PRIORITY = 166, IF = 167, EXP = 168, 
    LN = 169, LOG10 = 170, LOG2 = 171, SQRT = 172, RINT = 173, ABS = 174, 
    SIN = 175, COS = 176, TAN = 177, ASIN = 178, ACOS = 179, ATAN = 180, 
    SINH = 181, COSH = 182, TANH = 183, ASINH = 184, ACOSH = 185, ATANH = 186, 
    PI = 187, EULERIAN = 188, MIN = 189, MAX = 190, SUM = 191, AVG = 192, 
    TIME = 193, FLOAT = 194, INT = 195, STRING = 196, QUOTED_STRING = 197, 
    SINGLE_QUOTED_STRING = 198, SEMI = 199, COLON = 200, LSBRACKET = 201, 
    RSBRACKET = 202, LBRACKET = 203, RBRACKET = 204, COMMA = 205, DOT = 206, 
    LPAREN = 207, RPAREN = 208, UNI_REACTION_SIGN = 209, BI_REACTION_SIGN = 210, 
    DOLLAR = 211, TILDE = 212, AT = 213, GTE = 214, GT = 215, LTE = 216, 
    LT = 217, ASSIGNS = 218, EQUALS = 219, NOT_EQUALS = 220, BECOMES = 221, 
    LOGICAL_AND = 222, LOGICAL_OR = 223, DIV = 224, TIMES = 225, MINUS = 226, 
    PLUS = 227, POWER = 228, MOLECULE_TAG_TOKEN = 229, MOD = 230, PIPE = 231, 
    QMARK = 232, EMARK = 233, AMPERSAND = 234, VERSION_NUMBER = 235, ULB = 236
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

