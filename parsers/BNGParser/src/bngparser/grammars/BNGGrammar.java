// $ANTLR 3.3 Nov 30, 2010 12:50:56 /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGGrammar.g 2013-01-14 14:49:38

  package bngparser.grammars;
  import org.antlr.stringtemplate.*;
  import bngparser.dataType.BondList;
  import java.util.Map;
  import bngparser.methods.ReactionAction;
  import bngparser.dataType.Register;
  import bngparser.methods.InvertBidirectional;
  import bngparser.dataType.ChangeableChannelTokenStream;
  import bngparser.dataType.ReactionRegister;
  import bngparser.methods.GenericMethods;
  import java.util.Arrays;


import org.antlr.runtime.*;
import java.util.Stack;
import java.util.List;
import java.util.ArrayList;

import org.antlr.stringtemplate.*;
import org.antlr.stringtemplate.language.*;
import java.util.HashMap;
public class BNGGrammar extends Parser {
    public static final String[] tokenNames = new String[] {
        "<invalid>", "<EOR>", "<DOWN>", "<UP>", "LINE_COMMENT", "LB", "WS", "VERSION", "END", "BEGIN", "SEED", "SPECIES", "GROUPS", "MOLECULE", "MOLECULES", "MODEL", "TYPES", "REACTION", "REACTIONS", "RULES", "FUNCTIONS", "COMPARTMENTS", "ACTIONS", "OBSERVABLES", "POPULATION", "MAPS", "SET_OPTION", "SPECIES_LABEL", "GENERATE_NETWORK", "MAX_AGG", "MAX_ITER", "MAX_STOICH", "OVERWRITE", "PRINT_ITER", "AND", "OR", "ID", "IF", "PREFFIX", "SUFFIX", "LABEL", "PARAMETERS", "VERBOSE", "GML", "MATCHONCE", "DELETEMOLECULES", "MOVECONNECTED", "AUTO", "HNAUTY", "QUASY", "NOT", "INCLUDE_REACTANTS", "INCLUDE_PRODUCTS", "EXCLUDE_REACTANTS", "EXCLUDE_PRODUCTS", "GENERATEHYBRIDMODEL", "ATOL", "RTOL", "STEADY_STATE", "SPARSE", "T_END", "T_START", "N_STEPS", "N_OUTPUT_STEPS", "MAX_SIM_STEPS", "OUTPUT_STEP_INTERVAL", "PLA_CONFIG", "SAMPLE_TIMES", "SIMULATE_ODE", "SIMULATE_SSA", "SIMULATE_NF", "SIMULATE_PLA", "SIMULATE", "READFILE", "FILE", "WRITENET", "WRITESBML", "WRITEMFILE", "WRITEXML", "WRITEMEXFILE", "PRINT_CDAT", "PRINT_FUNCTIONS", "SETCONCENTRATION", "METHOD", "SETPARAMETER", "SAVECONCENTRATIONS", "RESETCONCENTRATIONS", "ADDCONCENTRATION", "GENERATENETWORK", "WRITENETWORK", "SUBSTANCEUNITS", "PARAM", "COMPLEX", "GET_FINAL_STATE", "SAT", "EXP", "LOG", "ARGFILE", "SAVE_PROGRESS", "NETFILE", "PRINT_NET", "PRINT_END", "TEXTREACTION", "CONTINUE", "ODE", "SSA", "NF", "PLA", "EVALUATE_EXPRESSIONS", "BDF", "DIGIT", "EXPONENT", "FLOAT", "INT", "LETTER", "STRING", "SEMI", "COLON", "LSBRACKET", "RSBRACKET", "LBRACKET", "RBRACKET", "COMMA", "DOT", "LPAREN", "RPAREN", "UNI_REACTION_SIGN", "BI_REACTION_SIGN", "DOLLAR", "TILDE", "AT", "GTE", "GT", "LTE", "LT", "ASSIGNS", "EQUALS", "BECOMES", "DIV", "TIMES", "MINUS", "PLUS", "POWER", "MOD", "PIPE", "QMARK", "EMARK", "DBQUOTES", "AMPERSAND", "NULL", "LOWER", "UPPER", "VERSION_NUMBER", "DOUBLE", "249", "250", "251", "252", "253", "254", "255", "256", "257", "258", "259", "260", "261", "262", "263", "264", "265", "266", "267", "268", "269", "270", "271", "272", "273", "274", "275"
    };
    public static final int EOF=-1;
    public static final int LINE_COMMENT=4;
    public static final int LB=5;
    public static final int WS=6;
    public static final int VERSION=7;
    public static final int END=8;
    public static final int BEGIN=9;
    public static final int SEED=10;
    public static final int SPECIES=11;
    public static final int GROUPS=12;
    public static final int MOLECULE=13;
    public static final int MOLECULES=14;
    public static final int MODEL=15;
    public static final int TYPES=16;
    public static final int REACTION=17;
    public static final int REACTIONS=18;
    public static final int RULES=19;
    public static final int FUNCTIONS=20;
    public static final int COMPARTMENTS=21;
    public static final int ACTIONS=22;
    public static final int OBSERVABLES=23;
    public static final int POPULATION=24;
    public static final int MAPS=25;
    public static final int SET_OPTION=26;
    public static final int SPECIES_LABEL=27;
    public static final int GENERATE_NETWORK=28;
    public static final int MAX_AGG=29;
    public static final int MAX_ITER=30;
    public static final int MAX_STOICH=31;
    public static final int OVERWRITE=32;
    public static final int PRINT_ITER=33;
    public static final int AND=34;
    public static final int OR=35;
    public static final int ID=36;
    public static final int IF=37;
    public static final int PREFFIX=38;
    public static final int SUFFIX=39;
    public static final int LABEL=40;
    public static final int PARAMETERS=41;
    public static final int VERBOSE=42;
    public static final int GML=43;
    public static final int MATCHONCE=44;
    public static final int DELETEMOLECULES=45;
    public static final int MOVECONNECTED=46;
    public static final int AUTO=47;
    public static final int HNAUTY=48;
    public static final int QUASY=49;
    public static final int NOT=50;
    public static final int INCLUDE_REACTANTS=51;
    public static final int INCLUDE_PRODUCTS=52;
    public static final int EXCLUDE_REACTANTS=53;
    public static final int EXCLUDE_PRODUCTS=54;
    public static final int GENERATEHYBRIDMODEL=55;
    public static final int ATOL=56;
    public static final int RTOL=57;
    public static final int STEADY_STATE=58;
    public static final int SPARSE=59;
    public static final int T_END=60;
    public static final int T_START=61;
    public static final int N_STEPS=62;
    public static final int N_OUTPUT_STEPS=63;
    public static final int MAX_SIM_STEPS=64;
    public static final int OUTPUT_STEP_INTERVAL=65;
    public static final int PLA_CONFIG=66;
    public static final int SAMPLE_TIMES=67;
    public static final int SIMULATE_ODE=68;
    public static final int SIMULATE_SSA=69;
    public static final int SIMULATE_NF=70;
    public static final int SIMULATE_PLA=71;
    public static final int SIMULATE=72;
    public static final int READFILE=73;
    public static final int FILE=74;
    public static final int WRITENET=75;
    public static final int WRITESBML=76;
    public static final int WRITEMFILE=77;
    public static final int WRITEXML=78;
    public static final int WRITEMEXFILE=79;
    public static final int PRINT_CDAT=80;
    public static final int PRINT_FUNCTIONS=81;
    public static final int SETCONCENTRATION=82;
    public static final int METHOD=83;
    public static final int SETPARAMETER=84;
    public static final int SAVECONCENTRATIONS=85;
    public static final int RESETCONCENTRATIONS=86;
    public static final int ADDCONCENTRATION=87;
    public static final int GENERATENETWORK=88;
    public static final int WRITENETWORK=89;
    public static final int SUBSTANCEUNITS=90;
    public static final int PARAM=91;
    public static final int COMPLEX=92;
    public static final int GET_FINAL_STATE=93;
    public static final int SAT=94;
    public static final int EXP=95;
    public static final int LOG=96;
    public static final int ARGFILE=97;
    public static final int SAVE_PROGRESS=98;
    public static final int NETFILE=99;
    public static final int PRINT_NET=100;
    public static final int PRINT_END=101;
    public static final int TEXTREACTION=102;
    public static final int CONTINUE=103;
    public static final int ODE=104;
    public static final int SSA=105;
    public static final int NF=106;
    public static final int PLA=107;
    public static final int EVALUATE_EXPRESSIONS=108;
    public static final int BDF=109;
    public static final int DIGIT=110;
    public static final int EXPONENT=111;
    public static final int FLOAT=112;
    public static final int INT=113;
    public static final int LETTER=114;
    public static final int STRING=115;
    public static final int SEMI=116;
    public static final int COLON=117;
    public static final int LSBRACKET=118;
    public static final int RSBRACKET=119;
    public static final int LBRACKET=120;
    public static final int RBRACKET=121;
    public static final int COMMA=122;
    public static final int DOT=123;
    public static final int LPAREN=124;
    public static final int RPAREN=125;
    public static final int UNI_REACTION_SIGN=126;
    public static final int BI_REACTION_SIGN=127;
    public static final int DOLLAR=128;
    public static final int TILDE=129;
    public static final int AT=130;
    public static final int GTE=131;
    public static final int GT=132;
    public static final int LTE=133;
    public static final int LT=134;
    public static final int ASSIGNS=135;
    public static final int EQUALS=136;
    public static final int BECOMES=137;
    public static final int DIV=138;
    public static final int TIMES=139;
    public static final int MINUS=140;
    public static final int PLUS=141;
    public static final int POWER=142;
    public static final int MOD=143;
    public static final int PIPE=144;
    public static final int QMARK=145;
    public static final int EMARK=146;
    public static final int DBQUOTES=147;
    public static final int AMPERSAND=148;
    public static final int NULL=149;
    public static final int LOWER=150;
    public static final int UPPER=151;
    public static final int VERSION_NUMBER=152;
    public static final int DOUBLE=248;

    // delegates
    public BNGGrammar_BNGGrammar_Expression gBNGGrammar_Expression;
    public BNGGrammar_BNGGrammar_Parameters gBNGGrammar_Parameters;
    public BNGGrammar_BNGGrammar_SeedSpecies gBNGGrammar_SeedSpecies;
    public BNGGrammar_BNGGrammar_MoleculeDef gBNGGrammar_MoleculeDef;
    public BNGGrammar_BNGGrammar_Observables gBNGGrammar_Observables;
    public BNGGrammar_BNGGrammar_ReactionRules gBNGGrammar_ReactionRules;
    public BNGGrammar_BNGGrammar_Actions gBNGGrammar_Actions;
    // delegators


        public BNGGrammar(TokenStream input) {
            this(input, new RecognizerSharedState());
        }
        public BNGGrammar(TokenStream input, RecognizerSharedState state) {
            super(input, state);
            gBNGGrammar_Expression = new BNGGrammar_BNGGrammar_Expression(input, state, this);
            gBNGGrammar_Parameters = new BNGGrammar_BNGGrammar_Parameters(input, state, this);
            gBNGGrammar_SeedSpecies = new BNGGrammar_BNGGrammar_SeedSpecies(input, state, this);
            gBNGGrammar_MoleculeDef = new BNGGrammar_BNGGrammar_MoleculeDef(input, state, this);
            gBNGGrammar_Observables = new BNGGrammar_BNGGrammar_Observables(input, state, this);
            gBNGGrammar_ReactionRules = new BNGGrammar_BNGGrammar_ReactionRules(input, state, this);
            gBNGGrammar_Actions = new BNGGrammar_BNGGrammar_Actions(input, state, this);         
        }
        
    protected StringTemplateGroup templateLib =
      new StringTemplateGroup("BNGGrammarTemplates", AngleBracketTemplateLexer.class);

    public void setTemplateLib(StringTemplateGroup templateLib) {
      this.templateLib = templateLib;
    }
    public StringTemplateGroup getTemplateLib() {
      return templateLib;
    }
    /** allows convenient multi-value initialization:
     *  "new STAttrMap().put(...).put(...)"
     */
    public static class STAttrMap extends HashMap {
      public STAttrMap put(String attrName, Object value) {
        super.put(attrName, value);
        return this;
      }
      public STAttrMap put(String attrName, int value) {
        super.put(attrName, new Integer(value));
        return this;
      }
    }

    public String[] getTokenNames() { return BNGGrammar.tokenNames; }
    public String getGrammarFileName() { return "/home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGGrammar.g"; }


      public Stack<String> paraphrases = new Stack<String>();
      public Map<String,Register> memory = new HashMap<String,Register>();
      public boolean netGrammar = false;
      public List<String> moleculeName;
      public List<String> moleculeNumber;
        public List<String> variableName = new ArrayList<String>();
      public boolean conserveNewMolecules = true;
      public List<StringTemplate> molecules = new ArrayList<StringTemplate>();
      
      @Override
      public String getErrorMessage(RecognitionException e,String[] tokenNames){
        String msg = super.getErrorMessage(e,tokenNames);
        if(paraphrases.size(  )>0) {
          String paraphrase = paraphrases.peek();
          msg += " " + paraphrase;
        }
        return msg;
      }
        


    protected static class prog_scope {
        List parameters;
        List seedSpecies;
        List observables;
        List reactionRules;
        List functions;
        List compartments;
        Stack elements;
    }
    protected Stack prog_stack = new Stack();

    public static class prog_return extends ParserRuleReturnScope {
        public StringTemplate actionsTemplate;
        public StringTemplate st;
        public Object getTemplate() { return st; }
        public String toString() { return st==null?null:st.toString(); }
    };

    // $ANTLR start "prog"
    // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGGrammar.g:48:1: prog returns [StringTemplate actionsTemplate] : ( LB )* ( header_block )* ( ( BEGIN MODEL ( LB )+ ( program_block )* END MODEL ( LB )* ) | ( program_block )* ) (a1= actions_block )? EOF -> prog2(parameters=$prog::parametersmolecules=moleculesspecies=$prog::seedSpeciesreactions=$prog::reactionRulesobservables=$prog::observablesfunctions=$prog::functionscompartments=$prog::compartments);
    public final BNGGrammar.prog_return prog() throws RecognitionException {
        prog_stack.push(new prog_scope());
        BNGGrammar.prog_return retval = new BNGGrammar.prog_return();
        retval.start = input.LT(1);

        BNGGrammar_BNGGrammar_Actions.actions_block_return a1 = null;



          ((prog_scope)prog_stack.peek()).parameters = new ArrayList();
          ((prog_scope)prog_stack.peek()).seedSpecies = new ArrayList();
          ((prog_scope)prog_stack.peek()).reactionRules = new ArrayList();
          ((prog_scope)prog_stack.peek()).observables = new ArrayList();
          ((prog_scope)prog_stack.peek()).functions = new ArrayList();
          ((prog_scope)prog_stack.peek()).compartments = new ArrayList();
         // memory = new HashMap<String,Register>();
          ((prog_scope)prog_stack.peek()).elements = new Stack();
          
          paraphrases.push("in model");

        try {
            // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGGrammar.g:75:1: ( ( LB )* ( header_block )* ( ( BEGIN MODEL ( LB )+ ( program_block )* END MODEL ( LB )* ) | ( program_block )* ) (a1= actions_block )? EOF -> prog2(parameters=$prog::parametersmolecules=moleculesspecies=$prog::seedSpeciesreactions=$prog::reactionRulesobservables=$prog::observablesfunctions=$prog::functionscompartments=$prog::compartments))
            // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGGrammar.g:76:1: ( LB )* ( header_block )* ( ( BEGIN MODEL ( LB )+ ( program_block )* END MODEL ( LB )* ) | ( program_block )* ) (a1= actions_block )? EOF
            {
            // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGGrammar.g:76:1: ( LB )*
            loop1:
            do {
                int alt1=2;
                int LA1_0 = input.LA(1);

                if ( (LA1_0==LB) ) {
                    alt1=1;
                }


                switch (alt1) {
            	case 1 :
            	    // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGGrammar.g:76:1: LB
            	    {
            	    match(input,LB,FOLLOW_LB_in_prog92); 

            	    }
            	    break;

            	default :
            	    break loop1;
                }
            } while (true);

            // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGGrammar.g:77:1: ( header_block )*
            loop2:
            do {
                int alt2=2;
                int LA2_0 = input.LA(1);

                if ( (LA2_0==VERSION||LA2_0==SET_OPTION||LA2_0==SUBSTANCEUNITS) ) {
                    alt2=1;
                }


                switch (alt2) {
            	case 1 :
            	    // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGGrammar.g:77:1: header_block
            	    {
            	    pushFollow(FOLLOW_header_block_in_prog95);
            	    header_block();

            	    state._fsp--;


            	    }
            	    break;

            	default :
            	    break loop2;
                }
            } while (true);

            // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGGrammar.g:79:1: ( ( BEGIN MODEL ( LB )+ ( program_block )* END MODEL ( LB )* ) | ( program_block )* )
            int alt7=2;
            int LA7_0 = input.LA(1);

            if ( (LA7_0==BEGIN) ) {
                int LA7_1 = input.LA(2);

                if ( (LA7_1==MODEL) ) {
                    alt7=1;
                }
                else if ( ((LA7_1>=SEED && LA7_1<=SPECIES)||LA7_1==MOLECULE||LA7_1==REACTION||(LA7_1>=FUNCTIONS && LA7_1<=POPULATION)||LA7_1==PARAMETERS) ) {
                    alt7=2;
                }
                else {
                    NoViableAltException nvae =
                        new NoViableAltException("", 7, 1, input);

                    throw nvae;
                }
            }
            else if ( (LA7_0==EOF||(LA7_0>=SIMULATE_ODE && LA7_0<=READFILE)||(LA7_0>=WRITENET && LA7_0<=WRITEMEXFILE)||LA7_0==SETCONCENTRATION||(LA7_0>=SETPARAMETER && LA7_0<=WRITENETWORK)) ) {
                alt7=2;
            }
            else {
                NoViableAltException nvae =
                    new NoViableAltException("", 7, 0, input);

                throw nvae;
            }
            switch (alt7) {
                case 1 :
                    // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGGrammar.g:79:2: ( BEGIN MODEL ( LB )+ ( program_block )* END MODEL ( LB )* )
                    {
                    // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGGrammar.g:79:2: ( BEGIN MODEL ( LB )+ ( program_block )* END MODEL ( LB )* )
                    // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGGrammar.g:79:3: BEGIN MODEL ( LB )+ ( program_block )* END MODEL ( LB )*
                    {
                    match(input,BEGIN,FOLLOW_BEGIN_in_prog101); 
                    match(input,MODEL,FOLLOW_MODEL_in_prog103); 
                    // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGGrammar.g:79:15: ( LB )+
                    int cnt3=0;
                    loop3:
                    do {
                        int alt3=2;
                        int LA3_0 = input.LA(1);

                        if ( (LA3_0==LB) ) {
                            alt3=1;
                        }


                        switch (alt3) {
                    	case 1 :
                    	    // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGGrammar.g:79:15: LB
                    	    {
                    	    match(input,LB,FOLLOW_LB_in_prog105); 

                    	    }
                    	    break;

                    	default :
                    	    if ( cnt3 >= 1 ) break loop3;
                                EarlyExitException eee =
                                    new EarlyExitException(3, input);
                                throw eee;
                        }
                        cnt3++;
                    } while (true);

                    // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGGrammar.g:79:19: ( program_block )*
                    loop4:
                    do {
                        int alt4=2;
                        int LA4_0 = input.LA(1);

                        if ( (LA4_0==BEGIN) ) {
                            alt4=1;
                        }


                        switch (alt4) {
                    	case 1 :
                    	    // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGGrammar.g:79:20: program_block
                    	    {
                    	    pushFollow(FOLLOW_program_block_in_prog109);
                    	    program_block();

                    	    state._fsp--;


                    	    }
                    	    break;

                    	default :
                    	    break loop4;
                        }
                    } while (true);

                    match(input,END,FOLLOW_END_in_prog113); 
                    match(input,MODEL,FOLLOW_MODEL_in_prog115); 
                    // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGGrammar.g:79:46: ( LB )*
                    loop5:
                    do {
                        int alt5=2;
                        int LA5_0 = input.LA(1);

                        if ( (LA5_0==LB) ) {
                            alt5=1;
                        }


                        switch (alt5) {
                    	case 1 :
                    	    // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGGrammar.g:79:46: LB
                    	    {
                    	    match(input,LB,FOLLOW_LB_in_prog117); 

                    	    }
                    	    break;

                    	default :
                    	    break loop5;
                        }
                    } while (true);


                    }


                    }
                    break;
                case 2 :
                    // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGGrammar.g:79:53: ( program_block )*
                    {
                    // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGGrammar.g:79:53: ( program_block )*
                    loop6:
                    do {
                        int alt6=2;
                        int LA6_0 = input.LA(1);

                        if ( (LA6_0==BEGIN) ) {
                            int LA6_1 = input.LA(2);

                            if ( ((LA6_1>=SEED && LA6_1<=SPECIES)||LA6_1==MOLECULE||LA6_1==REACTION||(LA6_1>=FUNCTIONS && LA6_1<=COMPARTMENTS)||(LA6_1>=OBSERVABLES && LA6_1<=POPULATION)||LA6_1==PARAMETERS) ) {
                                alt6=1;
                            }


                        }


                        switch (alt6) {
                    	case 1 :
                    	    // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGGrammar.g:79:54: program_block
                    	    {
                    	    pushFollow(FOLLOW_program_block_in_prog124);
                    	    program_block();

                    	    state._fsp--;


                    	    }
                    	    break;

                    	default :
                    	    break loop6;
                        }
                    } while (true);


                    }
                    break;

            }

            // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGGrammar.g:80:1: (a1= actions_block )?
            int alt8=2;
            int LA8_0 = input.LA(1);

            if ( (LA8_0==BEGIN||(LA8_0>=SIMULATE_ODE && LA8_0<=READFILE)||(LA8_0>=WRITENET && LA8_0<=WRITEMEXFILE)||LA8_0==SETCONCENTRATION||(LA8_0>=SETPARAMETER && LA8_0<=WRITENETWORK)) ) {
                alt8=1;
            }
            switch (alt8) {
                case 1 :
                    // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGGrammar.g:80:2: a1= actions_block
                    {
                    pushFollow(FOLLOW_actions_block_in_prog132);
                    a1=actions_block();

                    state._fsp--;

                    retval.actionsTemplate = (a1!=null?a1.st:null);

                    }
                    break;

            }

            match(input,EOF,FOLLOW_EOF_in_prog140); 


            // TEMPLATE REWRITE
            // 81:6: -> prog2(parameters=$prog::parametersmolecules=moleculesspecies=$prog::seedSpeciesreactions=$prog::reactionRulesobservables=$prog::observablesfunctions=$prog::functionscompartments=$prog::compartments)
            {
                retval.st = templateLib.getInstanceOf("prog2",
              new STAttrMap().put("parameters", ((prog_scope)prog_stack.peek()).parameters).put("molecules", molecules).put("species", ((prog_scope)prog_stack.peek()).seedSpecies).put("reactions", ((prog_scope)prog_stack.peek()).reactionRules).put("observables", ((prog_scope)prog_stack.peek()).observables).put("functions", ((prog_scope)prog_stack.peek()).functions).put("compartments", ((prog_scope)prog_stack.peek()).compartments));
            }


            }

            retval.stop = input.LT(-1);


              paraphrases.pop();

        }
        catch (RecognitionException re) {
            reportError(re);
            recover(input,re);
        }
        finally {
            prog_stack.pop();
        }
        return retval;
    }
    // $ANTLR end "prog"

    public static class header_block_return extends ParserRuleReturnScope {
        public StringTemplate st;
        public Object getTemplate() { return st; }
        public String toString() { return st==null?null:st.toString(); }
    };

    // $ANTLR start "header_block"
    // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGGrammar.g:84:1: header_block : ( version_def | substance_def | set_option );
    public final BNGGrammar.header_block_return header_block() throws RecognitionException {
        BNGGrammar.header_block_return retval = new BNGGrammar.header_block_return();
        retval.start = input.LT(1);

        try {
            // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGGrammar.g:85:1: ( version_def | substance_def | set_option )
            int alt9=3;
            switch ( input.LA(1) ) {
            case VERSION:
                {
                alt9=1;
                }
                break;
            case SUBSTANCEUNITS:
                {
                alt9=2;
                }
                break;
            case SET_OPTION:
                {
                alt9=3;
                }
                break;
            default:
                NoViableAltException nvae =
                    new NoViableAltException("", 9, 0, input);

                throw nvae;
            }

            switch (alt9) {
                case 1 :
                    // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGGrammar.g:85:3: version_def
                    {
                    pushFollow(FOLLOW_version_def_in_header_block212);
                    version_def();

                    state._fsp--;


                    }
                    break;
                case 2 :
                    // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGGrammar.g:86:3: substance_def
                    {
                    pushFollow(FOLLOW_substance_def_in_header_block216);
                    substance_def();

                    state._fsp--;


                    }
                    break;
                case 3 :
                    // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGGrammar.g:87:3: set_option
                    {
                    pushFollow(FOLLOW_set_option_in_header_block220);
                    set_option();

                    state._fsp--;


                    }
                    break;

            }
            retval.stop = input.LT(-1);

        }
        catch (RecognitionException re) {
            reportError(re);
            recover(input,re);
        }
        finally {
        }
        return retval;
    }
    // $ANTLR end "header_block"

    public static class substance_def_return extends ParserRuleReturnScope {
        public StringTemplate st;
        public Object getTemplate() { return st; }
        public String toString() { return st==null?null:st.toString(); }
    };

    // $ANTLR start "substance_def"
    // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGGrammar.g:90:1: substance_def : ( SUBSTANCEUNITS LPAREN DBQUOTES STRING DBQUOTES RPAREN ( LB )+ ) ;
    public final BNGGrammar.substance_def_return substance_def() throws RecognitionException {
        BNGGrammar.substance_def_return retval = new BNGGrammar.substance_def_return();
        retval.start = input.LT(1);

        try {
            // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGGrammar.g:90:14: ( ( SUBSTANCEUNITS LPAREN DBQUOTES STRING DBQUOTES RPAREN ( LB )+ ) )
            // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGGrammar.g:90:16: ( SUBSTANCEUNITS LPAREN DBQUOTES STRING DBQUOTES RPAREN ( LB )+ )
            {
            // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGGrammar.g:90:16: ( SUBSTANCEUNITS LPAREN DBQUOTES STRING DBQUOTES RPAREN ( LB )+ )
            // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGGrammar.g:90:17: SUBSTANCEUNITS LPAREN DBQUOTES STRING DBQUOTES RPAREN ( LB )+
            {
            match(input,SUBSTANCEUNITS,FOLLOW_SUBSTANCEUNITS_in_substance_def229); 
            match(input,LPAREN,FOLLOW_LPAREN_in_substance_def231); 
            match(input,DBQUOTES,FOLLOW_DBQUOTES_in_substance_def233); 
            match(input,STRING,FOLLOW_STRING_in_substance_def235); 
            match(input,DBQUOTES,FOLLOW_DBQUOTES_in_substance_def237); 
            match(input,RPAREN,FOLLOW_RPAREN_in_substance_def239); 
            // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGGrammar.g:90:72: ( LB )+
            int cnt10=0;
            loop10:
            do {
                int alt10=2;
                int LA10_0 = input.LA(1);

                if ( (LA10_0==LB) ) {
                    alt10=1;
                }


                switch (alt10) {
            	case 1 :
            	    // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGGrammar.g:90:72: LB
            	    {
            	    match(input,LB,FOLLOW_LB_in_substance_def242); 

            	    }
            	    break;

            	default :
            	    if ( cnt10 >= 1 ) break loop10;
                        EarlyExitException eee =
                            new EarlyExitException(10, input);
                        throw eee;
                }
                cnt10++;
            } while (true);


            }


            }

            retval.stop = input.LT(-1);

        }
        catch (RecognitionException re) {
            reportError(re);
            recover(input,re);
        }
        finally {
        }
        return retval;
    }
    // $ANTLR end "substance_def"

    public static class version_def_return extends ParserRuleReturnScope {
        public StringTemplate st;
        public Object getTemplate() { return st; }
        public String toString() { return st==null?null:st.toString(); }
    };

    // $ANTLR start "version_def"
    // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGGrammar.g:91:1: version_def : VERSION LPAREN DBQUOTES VERSION_NUMBER DBQUOTES RPAREN ( LB )+ ;
    public final BNGGrammar.version_def_return version_def() throws RecognitionException {
        BNGGrammar.version_def_return retval = new BNGGrammar.version_def_return();
        retval.start = input.LT(1);

        try {
            // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGGrammar.g:91:12: ( VERSION LPAREN DBQUOTES VERSION_NUMBER DBQUOTES RPAREN ( LB )+ )
            // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGGrammar.g:91:14: VERSION LPAREN DBQUOTES VERSION_NUMBER DBQUOTES RPAREN ( LB )+
            {
            match(input,VERSION,FOLLOW_VERSION_in_version_def250); 
            match(input,LPAREN,FOLLOW_LPAREN_in_version_def252); 
            match(input,DBQUOTES,FOLLOW_DBQUOTES_in_version_def254); 
            match(input,VERSION_NUMBER,FOLLOW_VERSION_NUMBER_in_version_def256); 
            match(input,DBQUOTES,FOLLOW_DBQUOTES_in_version_def258); 
            match(input,RPAREN,FOLLOW_RPAREN_in_version_def260); 
            // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGGrammar.g:91:70: ( LB )+
            int cnt11=0;
            loop11:
            do {
                int alt11=2;
                int LA11_0 = input.LA(1);

                if ( (LA11_0==LB) ) {
                    alt11=1;
                }


                switch (alt11) {
            	case 1 :
            	    // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGGrammar.g:91:70: LB
            	    {
            	    match(input,LB,FOLLOW_LB_in_version_def263); 

            	    }
            	    break;

            	default :
            	    if ( cnt11 >= 1 ) break loop11;
                        EarlyExitException eee =
                            new EarlyExitException(11, input);
                        throw eee;
                }
                cnt11++;
            } while (true);


            }

            retval.stop = input.LT(-1);

        }
        catch (RecognitionException re) {
            reportError(re);
            recover(input,re);
        }
        finally {
        }
        return retval;
    }
    // $ANTLR end "version_def"

    public static class set_option_return extends ParserRuleReturnScope {
        public StringTemplate st;
        public Object getTemplate() { return st; }
        public String toString() { return st==null?null:st.toString(); }
    };

    // $ANTLR start "set_option"
    // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGGrammar.g:92:1: set_option : SET_OPTION LPAREN DBQUOTES STRING DBQUOTES COMMA DBQUOTES STRING DBQUOTES RPAREN ( LB )+ ;
    public final BNGGrammar.set_option_return set_option() throws RecognitionException {
        BNGGrammar.set_option_return retval = new BNGGrammar.set_option_return();
        retval.start = input.LT(1);

        try {
            // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGGrammar.g:92:11: ( SET_OPTION LPAREN DBQUOTES STRING DBQUOTES COMMA DBQUOTES STRING DBQUOTES RPAREN ( LB )+ )
            // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGGrammar.g:92:13: SET_OPTION LPAREN DBQUOTES STRING DBQUOTES COMMA DBQUOTES STRING DBQUOTES RPAREN ( LB )+
            {
            match(input,SET_OPTION,FOLLOW_SET_OPTION_in_set_option270); 
            match(input,LPAREN,FOLLOW_LPAREN_in_set_option272); 
            match(input,DBQUOTES,FOLLOW_DBQUOTES_in_set_option274); 
            match(input,STRING,FOLLOW_STRING_in_set_option276); 
            match(input,DBQUOTES,FOLLOW_DBQUOTES_in_set_option278); 
            match(input,COMMA,FOLLOW_COMMA_in_set_option280); 
            match(input,DBQUOTES,FOLLOW_DBQUOTES_in_set_option282); 
            match(input,STRING,FOLLOW_STRING_in_set_option284); 
            match(input,DBQUOTES,FOLLOW_DBQUOTES_in_set_option286); 
            match(input,RPAREN,FOLLOW_RPAREN_in_set_option288); 
            // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGGrammar.g:92:95: ( LB )+
            int cnt12=0;
            loop12:
            do {
                int alt12=2;
                int LA12_0 = input.LA(1);

                if ( (LA12_0==LB) ) {
                    alt12=1;
                }


                switch (alt12) {
            	case 1 :
            	    // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGGrammar.g:92:95: LB
            	    {
            	    match(input,LB,FOLLOW_LB_in_set_option291); 

            	    }
            	    break;

            	default :
            	    if ( cnt12 >= 1 ) break loop12;
                        EarlyExitException eee =
                            new EarlyExitException(12, input);
                        throw eee;
                }
                cnt12++;
            } while (true);


            }

            retval.stop = input.LT(-1);

        }
        catch (RecognitionException re) {
            reportError(re);
            recover(input,re);
        }
        finally {
        }
        return retval;
    }
    // $ANTLR end "set_option"

    public static class program_block_return extends ParserRuleReturnScope {
        public StringTemplate st;
        public Object getTemplate() { return st; }
        public String toString() { return st==null?null:st.toString(); }
    };

    // $ANTLR start "program_block"
    // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGGrammar.g:96:1: program_block : ( parameters_block[memory,$prog::parameters] | molecule_types_block | seed_species_block[$prog::seedSpecies] | observables_block[$prog::observables] | reaction_rules_block[$prog::reactionRules] | functions_block | population_maps_block | compartments_block );
    public final BNGGrammar.program_block_return program_block() throws RecognitionException {
        BNGGrammar.program_block_return retval = new BNGGrammar.program_block_return();
        retval.start = input.LT(1);

        try {
            // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGGrammar.g:97:1: ( parameters_block[memory,$prog::parameters] | molecule_types_block | seed_species_block[$prog::seedSpecies] | observables_block[$prog::observables] | reaction_rules_block[$prog::reactionRules] | functions_block | population_maps_block | compartments_block )
            int alt13=8;
            alt13 = dfa13.predict(input);
            switch (alt13) {
                case 1 :
                    // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGGrammar.g:97:3: parameters_block[memory,$prog::parameters]
                    {
                    pushFollow(FOLLOW_parameters_block_in_program_block302);
                    parameters_block(memory, ((prog_scope)prog_stack.peek()).parameters);

                    state._fsp--;


                    }
                    break;
                case 2 :
                    // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGGrammar.g:98:3: molecule_types_block
                    {
                    pushFollow(FOLLOW_molecule_types_block_in_program_block307);
                    molecule_types_block();

                    state._fsp--;


                    }
                    break;
                case 3 :
                    // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGGrammar.g:99:3: seed_species_block[$prog::seedSpecies]
                    {
                    pushFollow(FOLLOW_seed_species_block_in_program_block311);
                    seed_species_block(((prog_scope)prog_stack.peek()).seedSpecies);

                    state._fsp--;


                    }
                    break;
                case 4 :
                    // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGGrammar.g:100:3: observables_block[$prog::observables]
                    {
                    pushFollow(FOLLOW_observables_block_in_program_block316);
                    observables_block(((prog_scope)prog_stack.peek()).observables);

                    state._fsp--;


                    }
                    break;
                case 5 :
                    // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGGrammar.g:101:3: reaction_rules_block[$prog::reactionRules]
                    {
                    pushFollow(FOLLOW_reaction_rules_block_in_program_block321);
                    reaction_rules_block(((prog_scope)prog_stack.peek()).reactionRules);

                    state._fsp--;


                    }
                    break;
                case 6 :
                    // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGGrammar.g:102:3: functions_block
                    {
                    pushFollow(FOLLOW_functions_block_in_program_block326);
                    functions_block();

                    state._fsp--;


                    }
                    break;
                case 7 :
                    // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGGrammar.g:103:3: population_maps_block
                    {
                    pushFollow(FOLLOW_population_maps_block_in_program_block330);
                    population_maps_block();

                    state._fsp--;


                    }
                    break;
                case 8 :
                    // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGGrammar.g:104:3: compartments_block
                    {
                    pushFollow(FOLLOW_compartments_block_in_program_block334);
                    compartments_block();

                    state._fsp--;


                    }
                    break;

            }
            retval.stop = input.LT(-1);

        }
        catch (RecognitionException re) {
            reportError(re);
            recover(input,re);
        }
        finally {
        }
        return retval;
    }
    // $ANTLR end "program_block"

    public static class functions_block_return extends ParserRuleReturnScope {
        public StringTemplate st;
        public Object getTemplate() { return st; }
        public String toString() { return st==null?null:st.toString(); }
    };

    // $ANTLR start "functions_block"
    // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGGrammar.g:110:1: functions_block : BEGIN FUNCTIONS ( LB )+ ( function_def ( LB )+ )* END FUNCTIONS ( LB )* ;
    public final BNGGrammar.functions_block_return functions_block() throws RecognitionException {
        BNGGrammar.functions_block_return retval = new BNGGrammar.functions_block_return();
        retval.start = input.LT(1);

        BNGGrammar.function_def_return function_def1 = null;



          paraphrases.push("in function block");

        try {
            // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGGrammar.g:117:1: ( BEGIN FUNCTIONS ( LB )+ ( function_def ( LB )+ )* END FUNCTIONS ( LB )* )
            // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGGrammar.g:118:7: BEGIN FUNCTIONS ( LB )+ ( function_def ( LB )+ )* END FUNCTIONS ( LB )*
            {
            match(input,BEGIN,FOLLOW_BEGIN_in_functions_block360); 
            match(input,FUNCTIONS,FOLLOW_FUNCTIONS_in_functions_block362); 
            // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGGrammar.g:118:23: ( LB )+
            int cnt14=0;
            loop14:
            do {
                int alt14=2;
                int LA14_0 = input.LA(1);

                if ( (LA14_0==LB) ) {
                    alt14=1;
                }


                switch (alt14) {
            	case 1 :
            	    // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGGrammar.g:118:23: LB
            	    {
            	    match(input,LB,FOLLOW_LB_in_functions_block364); 

            	    }
            	    break;

            	default :
            	    if ( cnt14 >= 1 ) break loop14;
                        EarlyExitException eee =
                            new EarlyExitException(14, input);
                        throw eee;
                }
                cnt14++;
            } while (true);

            // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGGrammar.g:119:8: ( function_def ( LB )+ )*
            loop16:
            do {
                int alt16=2;
                int LA16_0 = input.LA(1);

                if ( (LA16_0==STRING) ) {
                    alt16=1;
                }


                switch (alt16) {
            	case 1 :
            	    // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGGrammar.g:119:9: function_def ( LB )+
            	    {
            	    pushFollow(FOLLOW_function_def_in_functions_block375);
            	    function_def1=function_def();

            	    state._fsp--;

            	    ((prog_scope)prog_stack.peek()).functions.add((function_def1!=null?function_def1.st:null));
            	    // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGGrammar.g:119:64: ( LB )+
            	    int cnt15=0;
            	    loop15:
            	    do {
            	        int alt15=2;
            	        int LA15_0 = input.LA(1);

            	        if ( (LA15_0==LB) ) {
            	            alt15=1;
            	        }


            	        switch (alt15) {
            	    	case 1 :
            	    	    // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGGrammar.g:119:64: LB
            	    	    {
            	    	    match(input,LB,FOLLOW_LB_in_functions_block379); 

            	    	    }
            	    	    break;

            	    	default :
            	    	    if ( cnt15 >= 1 ) break loop15;
            	                EarlyExitException eee =
            	                    new EarlyExitException(15, input);
            	                throw eee;
            	        }
            	        cnt15++;
            	    } while (true);


            	    }
            	    break;

            	default :
            	    break loop16;
                }
            } while (true);

            match(input,END,FOLLOW_END_in_functions_block395); 
            match(input,FUNCTIONS,FOLLOW_FUNCTIONS_in_functions_block397); 
            // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGGrammar.g:120:21: ( LB )*
            loop17:
            do {
                int alt17=2;
                int LA17_0 = input.LA(1);

                if ( (LA17_0==LB) ) {
                    alt17=1;
                }


                switch (alt17) {
            	case 1 :
            	    // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGGrammar.g:120:21: LB
            	    {
            	    match(input,LB,FOLLOW_LB_in_functions_block399); 

            	    }
            	    break;

            	default :
            	    break loop17;
                }
            } while (true);


            }

            retval.stop = input.LT(-1);


              paraphrases.pop();

        }
        catch (RecognitionException re) {
            reportError(re);
            recover(input,re);
        }
        finally {
        }
        return retval;
    }
    // $ANTLR end "functions_block"

    protected static class function_def_scope {
        Map<String,Register> lmemory;
    }
    protected Stack function_def_stack = new Stack();

    public static class function_def_return extends ParserRuleReturnScope {
        public StringTemplate st;
        public Object getTemplate() { return st; }
        public String toString() { return st==null?null:st.toString(); }
    };

    // $ANTLR start "function_def"
    // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGGrammar.g:123:1: function_def : s1= STRING LPAREN (parameter= (s2= STRING ( COMMA s3= STRING )* ) )? RPAREN ( BECOMES )? expression[$function_def::lmemory] -> functions_block(id=$s1.textreferencesName=$expression.reference.keySet()referencesType=Register.getTypes($expression.reference)expression=$expression.text);
    public final BNGGrammar.function_def_return function_def() throws RecognitionException {
        function_def_stack.push(new function_def_scope());
        BNGGrammar.function_def_return retval = new BNGGrammar.function_def_return();
        retval.start = input.LT(1);

        Token s1=null;
        Token s2=null;
        Token s3=null;
        Token parameter=null;
        BNGGrammar_BNGGrammar_Expression.expression_return expression2 = null;



          ((function_def_scope)function_def_stack.peek()).lmemory =  new HashMap<String,Register>();
          ((function_def_scope)function_def_stack.peek()).lmemory.putAll(memory);

        try {
            // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGGrammar.g:130:2: (s1= STRING LPAREN (parameter= (s2= STRING ( COMMA s3= STRING )* ) )? RPAREN ( BECOMES )? expression[$function_def::lmemory] -> functions_block(id=$s1.textreferencesName=$expression.reference.keySet()referencesType=Register.getTypes($expression.reference)expression=$expression.text))
            // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGGrammar.g:131:5: s1= STRING LPAREN (parameter= (s2= STRING ( COMMA s3= STRING )* ) )? RPAREN ( BECOMES )? expression[$function_def::lmemory]
            {
            s1=(Token)match(input,STRING,FOLLOW_STRING_in_function_def421); 
            match(input,LPAREN,FOLLOW_LPAREN_in_function_def423); 
            // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGGrammar.g:132:14: (parameter= (s2= STRING ( COMMA s3= STRING )* ) )?
            int alt19=2;
            int LA19_0 = input.LA(1);

            if ( (LA19_0==STRING) ) {
                alt19=1;
            }
            switch (alt19) {
                case 1 :
                    // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGGrammar.g:132:14: parameter= (s2= STRING ( COMMA s3= STRING )* )
                    {
                    // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGGrammar.g:132:15: (s2= STRING ( COMMA s3= STRING )* )
                    // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGGrammar.g:132:16: s2= STRING ( COMMA s3= STRING )*
                    {
                    s2=(Token)match(input,STRING,FOLLOW_STRING_in_function_def435); 

                          ((function_def_scope)function_def_stack.peek()).lmemory.put((s2!=null?s2.getText():null),new Register(1.0,"ConstantExpression"));
                        
                    // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGGrammar.g:137:5: ( COMMA s3= STRING )*
                    loop18:
                    do {
                        int alt18=2;
                        int LA18_0 = input.LA(1);

                        if ( (LA18_0==COMMA) ) {
                            alt18=1;
                        }


                        switch (alt18) {
                    	case 1 :
                    	    // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGGrammar.g:137:6: COMMA s3= STRING
                    	    {
                    	    match(input,COMMA,FOLLOW_COMMA_in_function_def453); 
                    	    s3=(Token)match(input,STRING,FOLLOW_STRING_in_function_def457); 

                    	    }
                    	    break;

                    	default :
                    	    break loop18;
                        }
                    } while (true);


                          ((function_def_scope)function_def_stack.peek()).lmemory.put((s3!=null?s3.getText():null),new Register(1.0,"ConstantExpression"));
                        

                    }


                    }
                    break;

            }

            match(input,RPAREN,FOLLOW_RPAREN_in_function_def474); 
            // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGGrammar.g:141:15: ( BECOMES )?
            int alt20=2;
            int LA20_0 = input.LA(1);

            if ( (LA20_0==BECOMES) ) {
                alt20=1;
            }
            switch (alt20) {
                case 1 :
                    // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGGrammar.g:141:16: BECOMES
                    {
                    match(input,BECOMES,FOLLOW_BECOMES_in_function_def477); 

                    }
                    break;

            }

            pushFollow(FOLLOW_expression_in_function_def488);
            expression2=expression(((function_def_scope)function_def_stack.peek()).lmemory);

            state._fsp--;


                 memory.put((s1!=null?s1.getText():null),new Register((expression2!=null?expression2.value:null),"function"));
                


            // TEMPLATE REWRITE
            // 147:5: -> functions_block(id=$s1.textreferencesName=$expression.reference.keySet()referencesType=Register.getTypes($expression.reference)expression=$expression.text)
            {
                retval.st = templateLib.getInstanceOf("functions_block",
              new STAttrMap().put("id", (s1!=null?s1.getText():null)).put("referencesName", (expression2!=null?expression2.reference:null).keySet()).put("referencesType", Register.getTypes((expression2!=null?expression2.reference:null))).put("expression", (expression2!=null?input.toString(expression2.start,expression2.stop):null)));
            }


            }

            retval.stop = input.LT(-1);

        }
        catch (RecognitionException re) {
            reportError(re);
            recover(input,re);
        }
        finally {
            function_def_stack.pop();
        }
        return retval;
    }
    // $ANTLR end "function_def"

    public static class compartments_block_return extends ParserRuleReturnScope {
        public StringTemplate st;
        public Object getTemplate() { return st; }
        public String toString() { return st==null?null:st.toString(); }
    };

    // $ANTLR start "compartments_block"
    // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGGrammar.g:154:1: compartments_block : BEGIN COMPARTMENTS ( LB )+ ( compartment ( LB )+ )* END COMPARTMENTS ( LB )+ ;
    public final BNGGrammar.compartments_block_return compartments_block() throws RecognitionException {
        BNGGrammar.compartments_block_return retval = new BNGGrammar.compartments_block_return();
        retval.start = input.LT(1);

        BNGGrammar.compartment_return compartment3 = null;


        try {
            // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGGrammar.g:154:19: ( BEGIN COMPARTMENTS ( LB )+ ( compartment ( LB )+ )* END COMPARTMENTS ( LB )+ )
            // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGGrammar.g:156:3: BEGIN COMPARTMENTS ( LB )+ ( compartment ( LB )+ )* END COMPARTMENTS ( LB )+
            {
            match(input,BEGIN,FOLLOW_BEGIN_in_compartments_block595); 
            match(input,COMPARTMENTS,FOLLOW_COMPARTMENTS_in_compartments_block597); 
            // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGGrammar.g:156:22: ( LB )+
            int cnt21=0;
            loop21:
            do {
                int alt21=2;
                int LA21_0 = input.LA(1);

                if ( (LA21_0==LB) ) {
                    alt21=1;
                }


                switch (alt21) {
            	case 1 :
            	    // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGGrammar.g:156:22: LB
            	    {
            	    match(input,LB,FOLLOW_LB_in_compartments_block599); 

            	    }
            	    break;

            	default :
            	    if ( cnt21 >= 1 ) break loop21;
                        EarlyExitException eee =
                            new EarlyExitException(21, input);
                        throw eee;
                }
                cnt21++;
            } while (true);

            // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGGrammar.g:157:7: ( compartment ( LB )+ )*
            loop23:
            do {
                int alt23=2;
                int LA23_0 = input.LA(1);

                if ( (LA23_0==STRING) ) {
                    alt23=1;
                }


                switch (alt23) {
            	case 1 :
            	    // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGGrammar.g:157:8: compartment ( LB )+
            	    {
            	    pushFollow(FOLLOW_compartment_in_compartments_block609);
            	    compartment3=compartment();

            	    state._fsp--;

            	    // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGGrammar.g:157:20: ( LB )+
            	    int cnt22=0;
            	    loop22:
            	    do {
            	        int alt22=2;
            	        int LA22_0 = input.LA(1);

            	        if ( (LA22_0==LB) ) {
            	            alt22=1;
            	        }


            	        switch (alt22) {
            	    	case 1 :
            	    	    // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGGrammar.g:157:20: LB
            	    	    {
            	    	    match(input,LB,FOLLOW_LB_in_compartments_block611); 

            	    	    }
            	    	    break;

            	    	default :
            	    	    if ( cnt22 >= 1 ) break loop22;
            	                EarlyExitException eee =
            	                    new EarlyExitException(22, input);
            	                throw eee;
            	        }
            	        cnt22++;
            	    } while (true);

            	    ((prog_scope)prog_stack.peek()).compartments.add((compartment3!=null?compartment3.st:null));

            	    }
            	    break;

            	default :
            	    break loop23;
                }
            } while (true);

            match(input,END,FOLLOW_END_in_compartments_block619); 
            match(input,COMPARTMENTS,FOLLOW_COMPARTMENTS_in_compartments_block621); 
            // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGGrammar.g:158:20: ( LB )+
            int cnt24=0;
            loop24:
            do {
                int alt24=2;
                int LA24_0 = input.LA(1);

                if ( (LA24_0==LB) ) {
                    alt24=1;
                }


                switch (alt24) {
            	case 1 :
            	    // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGGrammar.g:158:20: LB
            	    {
            	    match(input,LB,FOLLOW_LB_in_compartments_block623); 

            	    }
            	    break;

            	default :
            	    if ( cnt24 >= 1 ) break loop24;
                        EarlyExitException eee =
                            new EarlyExitException(24, input);
                        throw eee;
                }
                cnt24++;
            } while (true);


            }

            retval.stop = input.LT(-1);

        }
        catch (RecognitionException re) {
            reportError(re);
            recover(input,re);
        }
        finally {
        }
        return retval;
    }
    // $ANTLR end "compartments_block"

    public static class compartment_return extends ParserRuleReturnScope {
        public StringTemplate st;
        public Object getTemplate() { return st; }
        public String toString() { return st==null?null:st.toString(); }
    };

    // $ANTLR start "compartment"
    // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGGrammar.g:160:1: compartment : s1= STRING INT s3= expression[memory] (s2= STRING )? -> compartments_block(id=$s1.textdimensions=$INT.textsize=$s3.valueoutside=$s2.text);
    public final BNGGrammar.compartment_return compartment() throws RecognitionException {
        BNGGrammar.compartment_return retval = new BNGGrammar.compartment_return();
        retval.start = input.LT(1);

        Token s1=null;
        Token s2=null;
        Token INT4=null;
        BNGGrammar_BNGGrammar_Expression.expression_return s3 = null;


        try {
            // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGGrammar.g:160:12: (s1= STRING INT s3= expression[memory] (s2= STRING )? -> compartments_block(id=$s1.textdimensions=$INT.textsize=$s3.valueoutside=$s2.text))
            // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGGrammar.g:161:4: s1= STRING INT s3= expression[memory] (s2= STRING )?
            {
            s1=(Token)match(input,STRING,FOLLOW_STRING_in_compartment638); 
            INT4=(Token)match(input,INT,FOLLOW_INT_in_compartment640); 
            pushFollow(FOLLOW_expression_in_compartment644);
            s3=expression(memory);

            state._fsp--;

            // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGGrammar.g:161:40: (s2= STRING )?
            int alt25=2;
            int LA25_0 = input.LA(1);

            if ( (LA25_0==STRING) ) {
                alt25=1;
            }
            switch (alt25) {
                case 1 :
                    // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGGrammar.g:161:41: s2= STRING
                    {
                    s2=(Token)match(input,STRING,FOLLOW_STRING_in_compartment650); 

                    }
                    break;

            }



            // TEMPLATE REWRITE
            // 161:53: -> compartments_block(id=$s1.textdimensions=$INT.textsize=$s3.valueoutside=$s2.text)
            {
                retval.st = templateLib.getInstanceOf("compartments_block",
              new STAttrMap().put("id", (s1!=null?s1.getText():null)).put("dimensions", (INT4!=null?INT4.getText():null)).put("size", (s3!=null?s3.value:null)).put("outside", (s2!=null?s2.getText():null)));
            }


            }

            retval.stop = input.LT(-1);

        }
        catch (RecognitionException re) {
            reportError(re);
            recover(input,re);
        }
        finally {
        }
        return retval;
    }
    // $ANTLR end "compartment"

    public static class label_return extends ParserRuleReturnScope {
        public String label;
        public StringTemplate st;
        public Object getTemplate() { return st; }
        public String toString() { return st==null?null:st.toString(); }
    };

    // $ANTLR start "label"
    // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGGrammar.g:165:1: label returns [String label] : MOD l1= ( STRING | INT ) ;
    public final BNGGrammar.label_return label() throws RecognitionException {
        BNGGrammar.label_return retval = new BNGGrammar.label_return();
        retval.start = input.LT(1);

        Token l1=null;

        try {
            // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGGrammar.g:165:29: ( MOD l1= ( STRING | INT ) )
            // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGGrammar.g:166:3: MOD l1= ( STRING | INT )
            {
            match(input,MOD,FOLLOW_MOD_in_label692); 
            l1=(Token)input.LT(1);
            if ( input.LA(1)==INT||input.LA(1)==STRING ) {
                input.consume();
                state.errorRecovery=false;
            }
            else {
                MismatchedSetException mse = new MismatchedSetException(null,input);
                throw mse;
            }

            retval.label = (l1!=null?l1.getText():null);

            }

            retval.stop = input.LT(-1);

        }
        catch (RecognitionException re) {
            reportError(re);
            recover(input,re);
        }
        finally {
        }
        return retval;
    }
    // $ANTLR end "label"

    public static class dereference_return extends ParserRuleReturnScope {
        public String label;
        public StringTemplate st;
        public Object getTemplate() { return st; }
        public String toString() { return st==null?null:st.toString(); }
    };

    // $ANTLR start "dereference"
    // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGGrammar.g:168:1: dereference returns [String label] : AMPERSAND STRING ;
    public final BNGGrammar.dereference_return dereference() throws RecognitionException {
        BNGGrammar.dereference_return retval = new BNGGrammar.dereference_return();
        retval.start = input.LT(1);

        Token STRING5=null;

        try {
            // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGGrammar.g:168:35: ( AMPERSAND STRING )
            // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGGrammar.g:169:3: AMPERSAND STRING
            {
            match(input,AMPERSAND,FOLLOW_AMPERSAND_in_dereference718); 
            STRING5=(Token)match(input,STRING,FOLLOW_STRING_in_dereference720); 
            retval.label = (STRING5!=null?STRING5.getText():null);

            }

            retval.stop = input.LT(-1);

        }
        catch (RecognitionException re) {
            reportError(re);
            recover(input,re);
        }
        finally {
        }
        return retval;
    }
    // $ANTLR end "dereference"

    // Delegated rules
    public BNGGrammar_BNGGrammar_Expression.if_expression_return if_expression() throws RecognitionException { return gBNGGrammar_Expression.if_expression(); }
    public BNGGrammar_BNGGrammar_Parameters.parameter_def_return parameter_def(Map<String,Register> lmemory) throws RecognitionException { return gBNGGrammar_Parameters.parameter_def(lmemory); }
    public BNGGrammar_BNGGrammar_ReactionRules.rate_function_return rate_function(List<String> rateList, boolean bidirectional) throws RecognitionException { return gBNGGrammar_ReactionRules.rate_function(rateList, bidirectional); }
    public BNGGrammar_BNGGrammar_ReactionRules.exclude_command_return exclude_command() throws RecognitionException { return gBNGGrammar_ReactionRules.exclude_command(); }
    public BNGGrammar_BNGGrammar_Actions.add_concentration_return add_concentration() throws RecognitionException { return gBNGGrammar_Actions.add_concentration(); }
    public BNGGrammar_BNGGrammar_Actions.multiple_definition_return multiple_definition() throws RecognitionException { return gBNGGrammar_Actions.multiple_definition(); }
    public BNGGrammar_BNGGrammar_SeedSpecies.pre_species_def_return pre_species_def(List molecules, BondList bonds, int speciesCounter) throws RecognitionException { return gBNGGrammar_SeedSpecies.pre_species_def(molecules, bonds, speciesCounter); }
    public BNGGrammar_BNGGrammar_MoleculeDef.molecule_def_return molecule_def() throws RecognitionException { return gBNGGrammar_MoleculeDef.molecule_def(); }
    public BNGGrammar_BNGGrammar_MoleculeDef.molecule_types_block_return molecule_types_block() throws RecognitionException { return gBNGGrammar_MoleculeDef.molecule_types_block(); }
    public BNGGrammar_BNGGrammar_SeedSpecies.site_list_return site_list(List sites, BondList bonds, String upperID) throws RecognitionException { return gBNGGrammar_SeedSpecies.site_list(sites, bonds, upperID); }
    public BNGGrammar_BNGGrammar_ReactionRules.reaction_def_return reaction_def(List patternsReactants, List patternsProducts, String upperID) throws RecognitionException { return gBNGGrammar_ReactionRules.reaction_def(patternsReactants, patternsProducts, upperID); }
    public BNGGrammar_BNGGrammar_ReactionRules.include_command_return include_command() throws RecognitionException { return gBNGGrammar_ReactionRules.include_command(); }
    public BNGGrammar_BNGGrammar_Actions.assignment_list_return assignment_list() throws RecognitionException { return gBNGGrammar_Actions.assignment_list(); }
    public BNGGrammar_BNGGrammar_Expression.variable_return variable() throws RecognitionException { return gBNGGrammar_Expression.variable(); }
    public BNGGrammar_BNGGrammar_ReactionRules.match_attribute_return match_attribute() throws RecognitionException { return gBNGGrammar_ReactionRules.match_attribute(); }
    public BNGGrammar_BNGGrammar_Actions.action_return action() throws RecognitionException { return gBNGGrammar_Actions.action(); }
    public BNGGrammar_BNGGrammar_Observables.observable_def_line_return observable_def_line(String upperID) throws RecognitionException { return gBNGGrammar_Observables.observable_def_line(upperID); }
    public BNGGrammar_BNGGrammar_Expression.primitive_element_return primitive_element() throws RecognitionException { return gBNGGrammar_Expression.primitive_element(); }
    public BNGGrammar_BNGGrammar_Actions.simulate_ode_return simulate_ode(Map<String,String> map) throws RecognitionException { return gBNGGrammar_Actions.simulate_ode(map); }
    public BNGGrammar_BNGGrammar_Actions.simulate_pla_par_def_return simulate_pla_par_def(Map<String,String> map) throws RecognitionException { return gBNGGrammar_Actions.simulate_pla_par_def(map); }
    public BNGGrammar_BNGGrammar_Actions.simulate_return simulate(Map<String,String> map) throws RecognitionException { return gBNGGrammar_Actions.simulate(map); }
    public BNGGrammar_BNGGrammar_Expression.expression2_return expression2() throws RecognitionException { return gBNGGrammar_Expression.expression2(); }
    public BNGGrammar_BNGGrammar_ReactionRules.reactionLabel_return reactionLabel() throws RecognitionException { return gBNGGrammar_ReactionRules.reactionLabel(); }
    public BNGGrammar_BNGGrammar_Actions.filename_return filename() throws RecognitionException { return gBNGGrammar_Actions.filename(); }
    public BNGGrammar_BNGGrammar_Actions.hash_value_return hash_value() throws RecognitionException { return gBNGGrammar_Actions.hash_value(); }
    public BNGGrammar_BNGGrammar_Actions.generate_network_return generate_network() throws RecognitionException { return gBNGGrammar_Actions.generate_network(); }
    public BNGGrammar_BNGGrammar_Actions.actions_prog_return actions_prog() throws RecognitionException { return gBNGGrammar_Actions.actions_prog(); }
    public BNGGrammar_BNGGrammar_Actions.save_concentrations_return save_concentrations() throws RecognitionException { return gBNGGrammar_Actions.save_concentrations(); }
    public BNGGrammar_BNGGrammar_Expression.power_expression_return power_expression() throws RecognitionException { return gBNGGrammar_Expression.power_expression(); }
    public BNGGrammar_BNGGrammar_Actions.write_file_return write_file() throws RecognitionException { return gBNGGrammar_Actions.write_file(); }
    public BNGGrammar_BNGGrammar_Observables.observable_type_return observable_type() throws RecognitionException { return gBNGGrammar_Observables.observable_type(); }
    public BNGGrammar_BNGGrammar_Actions.write_mex_file_return write_mex_file(Map<String,String> map) throws RecognitionException { return gBNGGrammar_Actions.write_mex_file(map); }
    public BNGGrammar_BNGGrammar_SeedSpecies.species_element_return species_element(BondList bonds, String upperID, String compartment) throws RecognitionException { return gBNGGrammar_SeedSpecies.species_element(bonds, upperID, compartment); }
    public BNGGrammar_BNGGrammar_Expression.function_return function() throws RecognitionException { return gBNGGrammar_Expression.function(); }
    public BNGGrammar_BNGGrammar_ReactionRules.reaction_rules_block_return reaction_rules_block(List reactionRules) throws RecognitionException { return gBNGGrammar_ReactionRules.reaction_rules_block(reactionRules); }
    public BNGGrammar_BNGGrammar_Actions.read_file_return read_file() throws RecognitionException { return gBNGGrammar_Actions.read_file(); }
    public BNGGrammar_BNGGrammar_Actions.variable_definition_return variable_definition() throws RecognitionException { return gBNGGrammar_Actions.variable_definition(); }
    public BNGGrammar_BNGGrammar_Expression.number_return number() throws RecognitionException { return gBNGGrammar_Expression.number(); }
    public BNGGrammar_BNGGrammar_SeedSpecies.seed_species_def_return seed_species_def(int counter) throws RecognitionException { return gBNGGrammar_SeedSpecies.seed_species_def(counter); }
    public BNGGrammar_BNGGrammar_ReactionRules.reaction_rule_def_return reaction_rule_def(int reactionCounter) throws RecognitionException { return gBNGGrammar_ReactionRules.reaction_rule_def(reactionCounter); }
    public BNGGrammar_BNGGrammar_Actions.set_parameter_return set_parameter() throws RecognitionException { return gBNGGrammar_Actions.set_parameter(); }
    public BNGGrammar_BNGGrammar_Actions.write_m_file_return write_m_file(Map<String,String> map) throws RecognitionException { return gBNGGrammar_Actions.write_m_file(map); }
    public BNGGrammar_BNGGrammar_SeedSpecies.site_obs_site_return site_obs_site(BondList bonds, String upperID) throws RecognitionException { return gBNGGrammar_SeedSpecies.site_obs_site(bonds, upperID); }
    public BNGGrammar_BNGGrammar_Actions.gn_action_par_def_return gn_action_par_def(Map<String,String> map) throws RecognitionException { return gBNGGrammar_Actions.gn_action_par_def(map); }
    public BNGGrammar_BNGGrammar_Actions.actions_block_return actions_block() throws RecognitionException { return gBNGGrammar_Actions.actions_block(); }
    public BNGGrammar_BNGGrammar_ReactionRules.function_keyword_return function_keyword() throws RecognitionException { return gBNGGrammar_ReactionRules.function_keyword(); }
    public BNGGrammar_BNGGrammar_SeedSpecies.site_obs_return site_obs(BondList bonds, String upperID) throws RecognitionException { return gBNGGrammar_SeedSpecies.site_obs(bonds, upperID); }
    public BNGGrammar_BNGGrammar_MoleculeDef.state_label_return state_label() throws RecognitionException { return gBNGGrammar_MoleculeDef.state_label(); }
    public BNGGrammar_BNGGrammar_Actions.simulate_ssa_return simulate_ssa(Map<String,String> map) throws RecognitionException { return gBNGGrammar_Actions.simulate_ssa(map); }
    public BNGGrammar_BNGGrammar_Expression.multiplying_expression_return multiplying_expression() throws RecognitionException { return gBNGGrammar_Expression.multiplying_expression(); }
    public BNGGrammar_BNGGrammar_Actions.write_par_def_return write_par_def() throws RecognitionException { return gBNGGrammar_Actions.write_par_def(); }
    public BNGGrammar_BNGGrammar_ReactionRules.rate_list_return rate_list(List<String> rateList, boolean bidirectional) throws RecognitionException { return gBNGGrammar_ReactionRules.rate_list(rateList, bidirectional); }
    public BNGGrammar_BNGGrammar_Expression.relational_expression_return relational_expression() throws RecognitionException { return gBNGGrammar_Expression.relational_expression(); }
    public BNGGrammar_BNGGrammar_ReactionRules.population_maps_block_return population_maps_block() throws RecognitionException { return gBNGGrammar_ReactionRules.population_maps_block(); }
    public BNGGrammar_BNGGrammar_Actions.simulate_nf_par_def_return simulate_nf_par_def(Map<String,String> map) throws RecognitionException { return gBNGGrammar_Actions.simulate_nf_par_def(map); }
    public BNGGrammar_BNGGrammar_ReactionRules.reactionReference_return reactionReference() throws RecognitionException { return gBNGGrammar_ReactionRules.reactionReference(); }
    public BNGGrammar_BNGGrammar_SeedSpecies.seed_species_block_return seed_species_block(List seedSpecies) throws RecognitionException { return gBNGGrammar_SeedSpecies.seed_species_block(seedSpecies); }
    public BNGGrammar_BNGGrammar_Actions.simulate_method_return simulate_method() throws RecognitionException { return gBNGGrammar_Actions.simulate_method(); }
    public BNGGrammar_BNGGrammar_MoleculeDef.site_def_return site_def() throws RecognitionException { return gBNGGrammar_MoleculeDef.site_def(); }
    public BNGGrammar_BNGGrammar_Actions.array_value_return array_value() throws RecognitionException { return gBNGGrammar_Actions.array_value(); }
    public BNGGrammar_BNGGrammar_ReactionRules.pattern_return pattern() throws RecognitionException { return gBNGGrammar_ReactionRules.pattern(); }
    public BNGGrammar_BNGGrammar_Actions.simulate_nf_return simulate_nf(Map<String,String> map) throws RecognitionException { return gBNGGrammar_Actions.simulate_nf(map); }
    public BNGGrammar_BNGGrammar_Actions.set_concentration_return set_concentration() throws RecognitionException { return gBNGGrammar_Actions.set_concentration(); }
    public BNGGrammar_BNGGrammar_Observables.observables_block_return observables_block(List observables) throws RecognitionException { return gBNGGrammar_Observables.observables_block(observables); }
    public BNGGrammar_BNGGrammar_Actions.value_return value() throws RecognitionException { return gBNGGrammar_Actions.value(); }
    public BNGGrammar_BNGGrammar_SeedSpecies.site_obs_state_return site_obs_state() throws RecognitionException { return gBNGGrammar_SeedSpecies.site_obs_state(); }
    public BNGGrammar_BNGGrammar_Actions.write_network_return write_network(Map<String,String> map) throws RecognitionException { return gBNGGrammar_Actions.write_network(map); }
    public BNGGrammar_BNGGrammar_Actions.parameter_definition_return parameter_definition() throws RecognitionException { return gBNGGrammar_Actions.parameter_definition(); }
    public BNGGrammar_BNGGrammar_Actions.reset_concentrations_return reset_concentrations() throws RecognitionException { return gBNGGrammar_Actions.reset_concentrations(); }
    public BNGGrammar_BNGGrammar_Actions.simulate_par_def_return simulate_par_def(Map<String,String> map) throws RecognitionException { return gBNGGrammar_Actions.simulate_par_def(map); }
    public BNGGrammar_BNGGrammar_Actions.write_m_par_def_return write_m_par_def(Map<String,String> map) throws RecognitionException { return gBNGGrammar_Actions.write_m_par_def(map); }
    public BNGGrammar_BNGGrammar_ReactionRules.rule_species_def_return rule_species_def(String upperID, ReactionAction reactionAction) throws RecognitionException { return gBNGGrammar_ReactionRules.rule_species_def(upperID, reactionAction); }
    public BNGGrammar_BNGGrammar_Observables.obs_species_def_return obs_species_def(String upperID, ReactionAction reactionAction) throws RecognitionException { return gBNGGrammar_Observables.obs_species_def(upperID, reactionAction); }
    public BNGGrammar_BNGGrammar_SeedSpecies.bond_name_return bond_name() throws RecognitionException { return gBNGGrammar_SeedSpecies.bond_name(); }
    public BNGGrammar_BNGGrammar_Parameters.parameters_block_return parameters_block(Map<String,Register> memory, List parameters) throws RecognitionException { return gBNGGrammar_Parameters.parameters_block(memory, parameters); }
    public BNGGrammar_BNGGrammar_Actions.simulate_pla_return simulate_pla(Map<String,String> map) throws RecognitionException { return gBNGGrammar_Actions.simulate_pla(map); }
    public BNGGrammar_BNGGrammar_Expression.sign_expression_return sign_expression() throws RecognitionException { return gBNGGrammar_Expression.sign_expression(); }
    public BNGGrammar_BNGGrammar_ReactionRules.modif_command_return modif_command() throws RecognitionException { return gBNGGrammar_ReactionRules.modif_command(); }
    public BNGGrammar_BNGGrammar_SeedSpecies.species_def_return species_def(List molecules, BondList bonds, String speciesCounter) throws RecognitionException { return gBNGGrammar_SeedSpecies.species_def(molecules, bonds, speciesCounter); }
    public BNGGrammar_BNGGrammar_Actions.simulate_ode_par_def_return simulate_ode_par_def(Map<String,String> map) throws RecognitionException { return gBNGGrammar_Actions.simulate_ode_par_def(map); }
    public BNGGrammar_BNGGrammar_Expression.adding_expression_return adding_expression() throws RecognitionException { return gBNGGrammar_Expression.adding_expression(); }
    public BNGGrammar_BNGGrammar_Observables.relational_observer_return relational_observer() throws RecognitionException { return gBNGGrammar_Observables.relational_observer(); }
    public BNGGrammar_BNGGrammar_Actions.write_type_return write_type() throws RecognitionException { return gBNGGrammar_Actions.write_type(); }
    public BNGGrammar_BNGGrammar_Expression.expression_return expression(Map<String,Register> memory) throws RecognitionException { return gBNGGrammar_Expression.expression(memory); }
    public BNGGrammar_BNGGrammar_Actions.ps_par_def_return ps_par_def(Map<String,String> map) throws RecognitionException { return gBNGGrammar_Actions.ps_par_def(map); }
    public BNGGrammar_BNGGrammar_Observables.pattern_list_return pattern_list(String upperID) throws RecognitionException { return gBNGGrammar_Observables.pattern_list(upperID); }
    public BNGGrammar_BNGGrammar_Actions.method_definition_return method_definition() throws RecognitionException { return gBNGGrammar_Actions.method_definition(); }
    public BNGGrammar_BNGGrammar_Expression.boolean_negation_expression_return boolean_negation_expression() throws RecognitionException { return gBNGGrammar_Expression.boolean_negation_expression(); }


    protected DFA13 dfa13 = new DFA13(this);
    static final String DFA13_eotS =
        "\12\uffff";
    static final String DFA13_eofS =
        "\12\uffff";
    static final String DFA13_minS =
        "\1\11\1\12\10\uffff";
    static final String DFA13_maxS =
        "\1\11\1\51\10\uffff";
    static final String DFA13_acceptS =
        "\2\uffff\1\1\1\2\1\4\1\5\1\6\1\7\1\10\1\3";
    static final String DFA13_specialS =
        "\12\uffff}>";
    static final String[] DFA13_transitionS = {
            "\1\1",
            "\2\11\1\uffff\1\3\3\uffff\1\5\2\uffff\1\6\1\10\1\uffff\1\4"+
            "\1\7\20\uffff\1\2",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            ""
    };

    static final short[] DFA13_eot = DFA.unpackEncodedString(DFA13_eotS);
    static final short[] DFA13_eof = DFA.unpackEncodedString(DFA13_eofS);
    static final char[] DFA13_min = DFA.unpackEncodedStringToUnsignedChars(DFA13_minS);
    static final char[] DFA13_max = DFA.unpackEncodedStringToUnsignedChars(DFA13_maxS);
    static final short[] DFA13_accept = DFA.unpackEncodedString(DFA13_acceptS);
    static final short[] DFA13_special = DFA.unpackEncodedString(DFA13_specialS);
    static final short[][] DFA13_transition;

    static {
        int numStates = DFA13_transitionS.length;
        DFA13_transition = new short[numStates][];
        for (int i=0; i<numStates; i++) {
            DFA13_transition[i] = DFA.unpackEncodedString(DFA13_transitionS[i]);
        }
    }

    class DFA13 extends DFA {

        public DFA13(BaseRecognizer recognizer) {
            this.recognizer = recognizer;
            this.decisionNumber = 13;
            this.eot = DFA13_eot;
            this.eof = DFA13_eof;
            this.min = DFA13_min;
            this.max = DFA13_max;
            this.accept = DFA13_accept;
            this.special = DFA13_special;
            this.transition = DFA13_transition;
        }
        public String getDescription() {
            return "96:1: program_block : ( parameters_block[memory,$prog::parameters] | molecule_types_block | seed_species_block[$prog::seedSpecies] | observables_block[$prog::observables] | reaction_rules_block[$prog::reactionRules] | functions_block | population_maps_block | compartments_block );";
        }
    }
 

    public static final BitSet FOLLOW_LB_in_prog92 = new BitSet(new long[]{0x00000000040002A0L,0x0000000007F4FBF0L});
    public static final BitSet FOLLOW_header_block_in_prog95 = new BitSet(new long[]{0x0000000004000280L,0x0000000007F4FBF0L});
    public static final BitSet FOLLOW_BEGIN_in_prog101 = new BitSet(new long[]{0x0000000000008000L});
    public static final BitSet FOLLOW_MODEL_in_prog103 = new BitSet(new long[]{0x0000000000000020L});
    public static final BitSet FOLLOW_LB_in_prog105 = new BitSet(new long[]{0x0000000000000320L});
    public static final BitSet FOLLOW_program_block_in_prog109 = new BitSet(new long[]{0x0000000000000300L});
    public static final BitSet FOLLOW_END_in_prog113 = new BitSet(new long[]{0x0000000000008000L});
    public static final BitSet FOLLOW_MODEL_in_prog115 = new BitSet(new long[]{0x0000000000000220L,0x0000000003F4FBF0L});
    public static final BitSet FOLLOW_LB_in_prog117 = new BitSet(new long[]{0x0000000000000220L,0x0000000003F4FBF0L});
    public static final BitSet FOLLOW_program_block_in_prog124 = new BitSet(new long[]{0x0000000000000200L,0x0000000003F4FBF0L});
    public static final BitSet FOLLOW_actions_block_in_prog132 = new BitSet(new long[]{0x0000000000000000L});
    public static final BitSet FOLLOW_EOF_in_prog140 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_version_def_in_header_block212 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_substance_def_in_header_block216 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_set_option_in_header_block220 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_SUBSTANCEUNITS_in_substance_def229 = new BitSet(new long[]{0x0000000000000000L,0x1000000000000000L});
    public static final BitSet FOLLOW_LPAREN_in_substance_def231 = new BitSet(new long[]{0x0000000000000000L,0x0000000000000000L,0x0000000000080000L});
    public static final BitSet FOLLOW_DBQUOTES_in_substance_def233 = new BitSet(new long[]{0x0000000000000000L,0x0008000000000000L});
    public static final BitSet FOLLOW_STRING_in_substance_def235 = new BitSet(new long[]{0x0000000000000000L,0x0000000000000000L,0x0000000000080000L});
    public static final BitSet FOLLOW_DBQUOTES_in_substance_def237 = new BitSet(new long[]{0x0000000000000000L,0x2000000000000000L});
    public static final BitSet FOLLOW_RPAREN_in_substance_def239 = new BitSet(new long[]{0x0000000000000020L});
    public static final BitSet FOLLOW_LB_in_substance_def242 = new BitSet(new long[]{0x0000000000000022L});
    public static final BitSet FOLLOW_VERSION_in_version_def250 = new BitSet(new long[]{0x0000000000000000L,0x1000000000000000L});
    public static final BitSet FOLLOW_LPAREN_in_version_def252 = new BitSet(new long[]{0x0000000000000000L,0x0000000000000000L,0x0000000000080000L});
    public static final BitSet FOLLOW_DBQUOTES_in_version_def254 = new BitSet(new long[]{0x0000000000000000L,0x0000000000000000L,0x0000000001000000L});
    public static final BitSet FOLLOW_VERSION_NUMBER_in_version_def256 = new BitSet(new long[]{0x0000000000000000L,0x0000000000000000L,0x0000000000080000L});
    public static final BitSet FOLLOW_DBQUOTES_in_version_def258 = new BitSet(new long[]{0x0000000000000000L,0x2000000000000000L});
    public static final BitSet FOLLOW_RPAREN_in_version_def260 = new BitSet(new long[]{0x0000000000000020L});
    public static final BitSet FOLLOW_LB_in_version_def263 = new BitSet(new long[]{0x0000000000000022L});
    public static final BitSet FOLLOW_SET_OPTION_in_set_option270 = new BitSet(new long[]{0x0000000000000000L,0x1000000000000000L});
    public static final BitSet FOLLOW_LPAREN_in_set_option272 = new BitSet(new long[]{0x0000000000000000L,0x0000000000000000L,0x0000000000080000L});
    public static final BitSet FOLLOW_DBQUOTES_in_set_option274 = new BitSet(new long[]{0x0000000000000000L,0x0008000000000000L});
    public static final BitSet FOLLOW_STRING_in_set_option276 = new BitSet(new long[]{0x0000000000000000L,0x0000000000000000L,0x0000000000080000L});
    public static final BitSet FOLLOW_DBQUOTES_in_set_option278 = new BitSet(new long[]{0x0000000000000000L,0x0400000000000000L});
    public static final BitSet FOLLOW_COMMA_in_set_option280 = new BitSet(new long[]{0x0000000000000000L,0x0000000000000000L,0x0000000000080000L});
    public static final BitSet FOLLOW_DBQUOTES_in_set_option282 = new BitSet(new long[]{0x0000000000000000L,0x0008000000000000L});
    public static final BitSet FOLLOW_STRING_in_set_option284 = new BitSet(new long[]{0x0000000000000000L,0x0000000000000000L,0x0000000000080000L});
    public static final BitSet FOLLOW_DBQUOTES_in_set_option286 = new BitSet(new long[]{0x0000000000000000L,0x2000000000000000L});
    public static final BitSet FOLLOW_RPAREN_in_set_option288 = new BitSet(new long[]{0x0000000000000020L});
    public static final BitSet FOLLOW_LB_in_set_option291 = new BitSet(new long[]{0x0000000000000022L});
    public static final BitSet FOLLOW_parameters_block_in_program_block302 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_molecule_types_block_in_program_block307 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_seed_species_block_in_program_block311 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_observables_block_in_program_block316 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_reaction_rules_block_in_program_block321 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_functions_block_in_program_block326 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_population_maps_block_in_program_block330 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_compartments_block_in_program_block334 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_BEGIN_in_functions_block360 = new BitSet(new long[]{0x0000000000100000L});
    public static final BitSet FOLLOW_FUNCTIONS_in_functions_block362 = new BitSet(new long[]{0x0000000000000020L});
    public static final BitSet FOLLOW_LB_in_functions_block364 = new BitSet(new long[]{0x0000000000000120L,0x0008000000000000L});
    public static final BitSet FOLLOW_function_def_in_functions_block375 = new BitSet(new long[]{0x0000000000000020L});
    public static final BitSet FOLLOW_LB_in_functions_block379 = new BitSet(new long[]{0x0000000000000120L,0x0008000000000000L});
    public static final BitSet FOLLOW_END_in_functions_block395 = new BitSet(new long[]{0x0000000000100000L});
    public static final BitSet FOLLOW_FUNCTIONS_in_functions_block397 = new BitSet(new long[]{0x0000000000000022L});
    public static final BitSet FOLLOW_LB_in_functions_block399 = new BitSet(new long[]{0x0000000000000022L});
    public static final BitSet FOLLOW_STRING_in_function_def421 = new BitSet(new long[]{0x0000000000000000L,0x1000000000000000L});
    public static final BitSet FOLLOW_LPAREN_in_function_def423 = new BitSet(new long[]{0x0000000000000000L,0x2008000000000000L});
    public static final BitSet FOLLOW_STRING_in_function_def435 = new BitSet(new long[]{0x0000000000000000L,0x2400000000000000L});
    public static final BitSet FOLLOW_COMMA_in_function_def453 = new BitSet(new long[]{0x0000000000000000L,0x0008000000000000L});
    public static final BitSet FOLLOW_STRING_in_function_def457 = new BitSet(new long[]{0x0000000000000000L,0x2400000000000000L});
    public static final BitSet FOLLOW_RPAREN_in_function_def474 = new BitSet(new long[]{0x0004002000000000L,0x100B000180000000L,0x0000000000001200L});
    public static final BitSet FOLLOW_BECOMES_in_function_def477 = new BitSet(new long[]{0x0004002000000000L,0x100B000180000000L,0x0000000000001200L});
    public static final BitSet FOLLOW_expression_in_function_def488 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_BEGIN_in_compartments_block595 = new BitSet(new long[]{0x0000000000200000L});
    public static final BitSet FOLLOW_COMPARTMENTS_in_compartments_block597 = new BitSet(new long[]{0x0000000000000020L});
    public static final BitSet FOLLOW_LB_in_compartments_block599 = new BitSet(new long[]{0x0000000000000120L,0x0008000000000000L});
    public static final BitSet FOLLOW_compartment_in_compartments_block609 = new BitSet(new long[]{0x0000000000000020L});
    public static final BitSet FOLLOW_LB_in_compartments_block611 = new BitSet(new long[]{0x0000000000000120L,0x0008000000000000L});
    public static final BitSet FOLLOW_END_in_compartments_block619 = new BitSet(new long[]{0x0000000000200000L});
    public static final BitSet FOLLOW_COMPARTMENTS_in_compartments_block621 = new BitSet(new long[]{0x0000000000000020L});
    public static final BitSet FOLLOW_LB_in_compartments_block623 = new BitSet(new long[]{0x0000000000000022L});
    public static final BitSet FOLLOW_STRING_in_compartment638 = new BitSet(new long[]{0x0000000000000000L,0x0002000000000000L});
    public static final BitSet FOLLOW_INT_in_compartment640 = new BitSet(new long[]{0x0004002000000000L,0x100B000180000000L,0x0000000000001200L});
    public static final BitSet FOLLOW_expression_in_compartment644 = new BitSet(new long[]{0x0000000000000002L,0x0008000000000000L});
    public static final BitSet FOLLOW_STRING_in_compartment650 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_MOD_in_label692 = new BitSet(new long[]{0x0000000000000000L,0x000A000000000000L});
    public static final BitSet FOLLOW_set_in_label696 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_AMPERSAND_in_dereference718 = new BitSet(new long[]{0x0000000000000000L,0x0008000000000000L});
    public static final BitSet FOLLOW_STRING_in_dereference720 = new BitSet(new long[]{0x0000000000000002L});

}