// $ANTLR 3.3 Nov 30, 2010 12:50:56 /home/proto/workspace/bionetgen/BNGParser/src/bngparser/grammars/BNGGrammar.g 2012-01-25 16:16:34

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


import org.antlr.runtime.*;
import java.util.Stack;
import java.util.List;
import java.util.ArrayList;

import org.antlr.stringtemplate.*;
import org.antlr.stringtemplate.language.*;
import java.util.HashMap;
public class BNGGrammar extends Parser {
    public static final String[] tokenNames = new String[] {
        "<invalid>", "<EOR>", "<DOWN>", "<UP>", "LINE_COMMENT", "WS", "VERSION", "END", "BEGIN", "SEED", "SPECIES", "GROUPS", "MOLECULE", "MOLECULES", "MODEL", "TYPES", "REACTION", "REACTIONS", "RULES", "FUNCTIONS", "COMPARTMENTS", "ACTIONS", "OBSERVABLES", "SET_OPTION", "SPECIES_LABEL", "GENERATE_NETWORK", "MAX_AGG", "MAX_ITER", "MAX_STOICH", "OVERWRITE", "PRINT_ITER", "AND", "OR", "ID", "PREFFIX", "SUFFIX", "LABEL", "PARAMETERS", "MATCHONCE", "DELETEMOLECULES", "MOVECONNECTED", "AUTO", "HNAUTY", "QUASY", "NOT", "INCLUDE_REACTANTS", "INCLUDE_PRODUCTS", "EXCLUDE_REACTANTS", "EXCLUDE_PRODUCTS", "ATOL", "RTOL", "STEADY_STATE", "SPARSE", "T_END", "T_START", "N_STEPS", "SAMPLE_TIMES", "SIMULATE_ODE", "SIMULATE_SSA", "READFILE", "FILE", "WRITENET", "WRITESBML", "WRITEMFILE", "WRITEXML", "SETCONCENTRATION", "SETPARAMETER", "SAVECONCENTRATIONS", "RESETCONCENTRATIONS", "GENERATENETWORK", "SUBSTANCEUNITS", "DIGIT", "EXPONENT", "FLOAT", "INT", "LETTER", "STRING", "SEMI", "COLON", "LSBRACKET", "RSBRACKET", "LBRACKET", "RBRACKET", "COMMA", "DOT", "LPAREN", "RPAREN", "UNI_REACTION_SIGN", "BI_REACTION_SIGN", "DOLLAR", "TILDE", "AT", "GTE", "GT", "LTE", "LT", "ASSIGNS", "EQUALS", "BECOMES", "DIV", "TIMES", "MINUS", "PLUS", "MOD", "QMARK", "EMARK", "DBQUOTES", "AMPERSAND", "NULL", "LOWER", "UPPER", "VERSION_NUMBER", "112", "113", "114", "115", "116", "117", "118", "119", "120", "121", "122", "123", "124", "125", "126", "127", "128", "129", "130", "131", "132", "133", "134", "135", "136", "137", "138", "139", "140", "141", "142", "143", "144", "145", "146", "147", "148", "149", "150", "151", "152", "153", "154", "155", "156", "157", "158", "159", "160", "161", "162", "163", "164", "165", "166", "167", "168", "169", "170", "171", "172", "173", "174", "175", "176", "177", "178", "179", "180", "181", "182", "183", "184", "185", "186", "187", "188", "189", "190", "191", "192", "193", "194", "195"
    };
    public static final int EOF=-1;
    public static final int LINE_COMMENT=4;
    public static final int WS=5;
    public static final int VERSION=6;
    public static final int END=7;
    public static final int BEGIN=8;
    public static final int SEED=9;
    public static final int SPECIES=10;
    public static final int GROUPS=11;
    public static final int MOLECULE=12;
    public static final int MOLECULES=13;
    public static final int MODEL=14;
    public static final int TYPES=15;
    public static final int REACTION=16;
    public static final int REACTIONS=17;
    public static final int RULES=18;
    public static final int FUNCTIONS=19;
    public static final int COMPARTMENTS=20;
    public static final int ACTIONS=21;
    public static final int OBSERVABLES=22;
    public static final int SET_OPTION=23;
    public static final int SPECIES_LABEL=24;
    public static final int GENERATE_NETWORK=25;
    public static final int MAX_AGG=26;
    public static final int MAX_ITER=27;
    public static final int MAX_STOICH=28;
    public static final int OVERWRITE=29;
    public static final int PRINT_ITER=30;
    public static final int AND=31;
    public static final int OR=32;
    public static final int ID=33;
    public static final int PREFFIX=34;
    public static final int SUFFIX=35;
    public static final int LABEL=36;
    public static final int PARAMETERS=37;
    public static final int MATCHONCE=38;
    public static final int DELETEMOLECULES=39;
    public static final int MOVECONNECTED=40;
    public static final int AUTO=41;
    public static final int HNAUTY=42;
    public static final int QUASY=43;
    public static final int NOT=44;
    public static final int INCLUDE_REACTANTS=45;
    public static final int INCLUDE_PRODUCTS=46;
    public static final int EXCLUDE_REACTANTS=47;
    public static final int EXCLUDE_PRODUCTS=48;
    public static final int ATOL=49;
    public static final int RTOL=50;
    public static final int STEADY_STATE=51;
    public static final int SPARSE=52;
    public static final int T_END=53;
    public static final int T_START=54;
    public static final int N_STEPS=55;
    public static final int SAMPLE_TIMES=56;
    public static final int SIMULATE_ODE=57;
    public static final int SIMULATE_SSA=58;
    public static final int READFILE=59;
    public static final int FILE=60;
    public static final int WRITENET=61;
    public static final int WRITESBML=62;
    public static final int WRITEMFILE=63;
    public static final int WRITEXML=64;
    public static final int SETCONCENTRATION=65;
    public static final int SETPARAMETER=66;
    public static final int SAVECONCENTRATIONS=67;
    public static final int RESETCONCENTRATIONS=68;
    public static final int GENERATENETWORK=69;
    public static final int SUBSTANCEUNITS=70;
    public static final int DIGIT=71;
    public static final int EXPONENT=72;
    public static final int FLOAT=73;
    public static final int INT=74;
    public static final int LETTER=75;
    public static final int STRING=76;
    public static final int SEMI=77;
    public static final int COLON=78;
    public static final int LSBRACKET=79;
    public static final int RSBRACKET=80;
    public static final int LBRACKET=81;
    public static final int RBRACKET=82;
    public static final int COMMA=83;
    public static final int DOT=84;
    public static final int LPAREN=85;
    public static final int RPAREN=86;
    public static final int UNI_REACTION_SIGN=87;
    public static final int BI_REACTION_SIGN=88;
    public static final int DOLLAR=89;
    public static final int TILDE=90;
    public static final int AT=91;
    public static final int GTE=92;
    public static final int GT=93;
    public static final int LTE=94;
    public static final int LT=95;
    public static final int ASSIGNS=96;
    public static final int EQUALS=97;
    public static final int BECOMES=98;
    public static final int DIV=99;
    public static final int TIMES=100;
    public static final int MINUS=101;
    public static final int PLUS=102;
    public static final int MOD=103;
    public static final int QMARK=104;
    public static final int EMARK=105;
    public static final int DBQUOTES=106;
    public static final int AMPERSAND=107;
    public static final int NULL=108;
    public static final int LOWER=109;
    public static final int UPPER=110;
    public static final int VERSION_NUMBER=111;

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
    public String getGrammarFileName() { return "/home/proto/workspace/bionetgen/BNGParser/src/bngparser/grammars/BNGGrammar.g"; }


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
    // /home/proto/workspace/bionetgen/BNGParser/src/bngparser/grammars/BNGGrammar.g:47:1: prog returns [StringTemplate actionsTemplate] : ( ( BEGIN MODEL ( program_block )* END MODEL ) | ( program_block )* ) (a1= actions_block | ( BEGIN ACTIONS a2= actions_block END ACTIONS ) )? EOF -> prog2(parameters=$prog::parametersmolecules=moleculesspecies=$prog::seedSpeciesreactions=$prog::reactionRulesobservables=$prog::observablesfunctions=$prog::functionscompartments=$prog::compartments);
    public final BNGGrammar.prog_return prog() throws RecognitionException {
        prog_stack.push(new prog_scope());
        BNGGrammar.prog_return retval = new BNGGrammar.prog_return();
        retval.start = input.LT(1);

        BNGGrammar_BNGGrammar_Actions.actions_block_return a1 = null;

        BNGGrammar_BNGGrammar_Actions.actions_block_return a2 = null;



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
            // /home/proto/workspace/bionetgen/BNGParser/src/bngparser/grammars/BNGGrammar.g:75:1: ( ( ( BEGIN MODEL ( program_block )* END MODEL ) | ( program_block )* ) (a1= actions_block | ( BEGIN ACTIONS a2= actions_block END ACTIONS ) )? EOF -> prog2(parameters=$prog::parametersmolecules=moleculesspecies=$prog::seedSpeciesreactions=$prog::reactionRulesobservables=$prog::observablesfunctions=$prog::functionscompartments=$prog::compartments))
            // /home/proto/workspace/bionetgen/BNGParser/src/bngparser/grammars/BNGGrammar.g:76:1: ( ( BEGIN MODEL ( program_block )* END MODEL ) | ( program_block )* ) (a1= actions_block | ( BEGIN ACTIONS a2= actions_block END ACTIONS ) )? EOF
            {
            // /home/proto/workspace/bionetgen/BNGParser/src/bngparser/grammars/BNGGrammar.g:76:1: ( ( BEGIN MODEL ( program_block )* END MODEL ) | ( program_block )* )
            int alt3=2;
            int LA3_0 = input.LA(1);

            if ( (LA3_0==BEGIN) ) {
                int LA3_1 = input.LA(2);

                if ( (LA3_1==MODEL) ) {
                    alt3=1;
                }
                else if ( ((LA3_1>=SEED && LA3_1<=SPECIES)||LA3_1==MOLECULE||LA3_1==REACTION||(LA3_1>=FUNCTIONS && LA3_1<=OBSERVABLES)||LA3_1==PARAMETERS) ) {
                    alt3=2;
                }
                else {
                    NoViableAltException nvae =
                        new NoViableAltException("", 3, 1, input);

                    throw nvae;
                }
            }
            else if ( (LA3_0==EOF||(LA3_0>=SIMULATE_ODE && LA3_0<=READFILE)||(LA3_0>=WRITENET && LA3_0<=GENERATENETWORK)) ) {
                alt3=2;
            }
            else {
                NoViableAltException nvae =
                    new NoViableAltException("", 3, 0, input);

                throw nvae;
            }
            switch (alt3) {
                case 1 :
                    // /home/proto/workspace/bionetgen/BNGParser/src/bngparser/grammars/BNGGrammar.g:76:2: ( BEGIN MODEL ( program_block )* END MODEL )
                    {
                    // /home/proto/workspace/bionetgen/BNGParser/src/bngparser/grammars/BNGGrammar.g:76:2: ( BEGIN MODEL ( program_block )* END MODEL )
                    // /home/proto/workspace/bionetgen/BNGParser/src/bngparser/grammars/BNGGrammar.g:76:3: BEGIN MODEL ( program_block )* END MODEL
                    {
                    match(input,BEGIN,FOLLOW_BEGIN_in_prog94); 
                    match(input,MODEL,FOLLOW_MODEL_in_prog96); 
                    // /home/proto/workspace/bionetgen/BNGParser/src/bngparser/grammars/BNGGrammar.g:76:15: ( program_block )*
                    loop1:
                    do {
                        int alt1=2;
                        int LA1_0 = input.LA(1);

                        if ( (LA1_0==BEGIN) ) {
                            alt1=1;
                        }


                        switch (alt1) {
                    	case 1 :
                    	    // /home/proto/workspace/bionetgen/BNGParser/src/bngparser/grammars/BNGGrammar.g:76:16: program_block
                    	    {
                    	    pushFollow(FOLLOW_program_block_in_prog99);
                    	    program_block();

                    	    state._fsp--;


                    	    }
                    	    break;

                    	default :
                    	    break loop1;
                        }
                    } while (true);

                    match(input,END,FOLLOW_END_in_prog103); 
                    match(input,MODEL,FOLLOW_MODEL_in_prog105); 

                    }


                    }
                    break;
                case 2 :
                    // /home/proto/workspace/bionetgen/BNGParser/src/bngparser/grammars/BNGGrammar.g:76:45: ( program_block )*
                    {
                    // /home/proto/workspace/bionetgen/BNGParser/src/bngparser/grammars/BNGGrammar.g:76:45: ( program_block )*
                    loop2:
                    do {
                        int alt2=2;
                        int LA2_0 = input.LA(1);

                        if ( (LA2_0==BEGIN) ) {
                            int LA2_2 = input.LA(2);

                            if ( ((LA2_2>=SEED && LA2_2<=SPECIES)||LA2_2==MOLECULE||LA2_2==REACTION||(LA2_2>=FUNCTIONS && LA2_2<=COMPARTMENTS)||LA2_2==OBSERVABLES||LA2_2==PARAMETERS) ) {
                                alt2=1;
                            }


                        }


                        switch (alt2) {
                    	case 1 :
                    	    // /home/proto/workspace/bionetgen/BNGParser/src/bngparser/grammars/BNGGrammar.g:76:46: program_block
                    	    {
                    	    pushFollow(FOLLOW_program_block_in_prog111);
                    	    program_block();

                    	    state._fsp--;


                    	    }
                    	    break;

                    	default :
                    	    break loop2;
                        }
                    } while (true);


                    }
                    break;

            }

            // /home/proto/workspace/bionetgen/BNGParser/src/bngparser/grammars/BNGGrammar.g:77:1: (a1= actions_block | ( BEGIN ACTIONS a2= actions_block END ACTIONS ) )?
            int alt4=3;
            int LA4_0 = input.LA(1);

            if ( ((LA4_0>=SIMULATE_ODE && LA4_0<=READFILE)||(LA4_0>=WRITENET && LA4_0<=GENERATENETWORK)) ) {
                alt4=1;
            }
            else if ( (LA4_0==BEGIN) ) {
                alt4=2;
            }
            switch (alt4) {
                case 1 :
                    // /home/proto/workspace/bionetgen/BNGParser/src/bngparser/grammars/BNGGrammar.g:77:2: a1= actions_block
                    {
                    pushFollow(FOLLOW_actions_block_in_prog119);
                    a1=actions_block();

                    state._fsp--;

                    retval.actionsTemplate = (a1!=null?a1.st:null);

                    }
                    break;
                case 2 :
                    // /home/proto/workspace/bionetgen/BNGParser/src/bngparser/grammars/BNGGrammar.g:77:49: ( BEGIN ACTIONS a2= actions_block END ACTIONS )
                    {
                    // /home/proto/workspace/bionetgen/BNGParser/src/bngparser/grammars/BNGGrammar.g:77:49: ( BEGIN ACTIONS a2= actions_block END ACTIONS )
                    // /home/proto/workspace/bionetgen/BNGParser/src/bngparser/grammars/BNGGrammar.g:77:50: BEGIN ACTIONS a2= actions_block END ACTIONS
                    {
                    match(input,BEGIN,FOLLOW_BEGIN_in_prog125); 
                    match(input,ACTIONS,FOLLOW_ACTIONS_in_prog127); 
                    pushFollow(FOLLOW_actions_block_in_prog131);
                    a2=actions_block();

                    state._fsp--;

                    match(input,END,FOLLOW_END_in_prog133); 
                    match(input,ACTIONS,FOLLOW_ACTIONS_in_prog135); 
                    retval.actionsTemplate = (a2!=null?a2.st:null);

                    }


                    }
                    break;

            }

            match(input,EOF,FOLLOW_EOF_in_prog144); 


            // TEMPLATE REWRITE
            // 78:6: -> prog2(parameters=$prog::parametersmolecules=moleculesspecies=$prog::seedSpeciesreactions=$prog::reactionRulesobservables=$prog::observablesfunctions=$prog::functionscompartments=$prog::compartments)
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

    public static class version_def_return extends ParserRuleReturnScope {
        public StringTemplate st;
        public Object getTemplate() { return st; }
        public String toString() { return st==null?null:st.toString(); }
    };

    // $ANTLR start "version_def"
    // /home/proto/workspace/bionetgen/BNGParser/src/bngparser/grammars/BNGGrammar.g:83:1: version_def : VERSION LPAREN DBQUOTES VERSION_NUMBER DBQUOTES RPAREN SEMI ;
    public final BNGGrammar.version_def_return version_def() throws RecognitionException {
        BNGGrammar.version_def_return retval = new BNGGrammar.version_def_return();
        retval.start = input.LT(1);

        try {
            // /home/proto/workspace/bionetgen/BNGParser/src/bngparser/grammars/BNGGrammar.g:83:12: ( VERSION LPAREN DBQUOTES VERSION_NUMBER DBQUOTES RPAREN SEMI )
            // /home/proto/workspace/bionetgen/BNGParser/src/bngparser/grammars/BNGGrammar.g:83:14: VERSION LPAREN DBQUOTES VERSION_NUMBER DBQUOTES RPAREN SEMI
            {
            match(input,VERSION,FOLLOW_VERSION_in_version_def217); 
            match(input,LPAREN,FOLLOW_LPAREN_in_version_def219); 
            match(input,DBQUOTES,FOLLOW_DBQUOTES_in_version_def221); 
            match(input,VERSION_NUMBER,FOLLOW_VERSION_NUMBER_in_version_def223); 
            match(input,DBQUOTES,FOLLOW_DBQUOTES_in_version_def225); 
            match(input,RPAREN,FOLLOW_RPAREN_in_version_def227); 
            match(input,SEMI,FOLLOW_SEMI_in_version_def229); 

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

    public static class program_block_return extends ParserRuleReturnScope {
        public StringTemplate st;
        public Object getTemplate() { return st; }
        public String toString() { return st==null?null:st.toString(); }
    };

    // $ANTLR start "program_block"
    // /home/proto/workspace/bionetgen/BNGParser/src/bngparser/grammars/BNGGrammar.g:87:1: program_block : ( parameters_block[memory,$prog::parameters] | molecule_types_block | seed_species_block[$prog::seedSpecies] | observables_block[$prog::observables] | reaction_rules_block[$prog::reactionRules] | functions_block | compartments_block );
    public final BNGGrammar.program_block_return program_block() throws RecognitionException {
        BNGGrammar.program_block_return retval = new BNGGrammar.program_block_return();
        retval.start = input.LT(1);

        try {
            // /home/proto/workspace/bionetgen/BNGParser/src/bngparser/grammars/BNGGrammar.g:88:1: ( parameters_block[memory,$prog::parameters] | molecule_types_block | seed_species_block[$prog::seedSpecies] | observables_block[$prog::observables] | reaction_rules_block[$prog::reactionRules] | functions_block | compartments_block )
            int alt5=7;
            int LA5_0 = input.LA(1);

            if ( (LA5_0==BEGIN) ) {
                switch ( input.LA(2) ) {
                case PARAMETERS:
                    {
                    alt5=1;
                    }
                    break;
                case MOLECULE:
                    {
                    alt5=2;
                    }
                    break;
                case OBSERVABLES:
                    {
                    alt5=4;
                    }
                    break;
                case REACTION:
                    {
                    alt5=5;
                    }
                    break;
                case FUNCTIONS:
                    {
                    alt5=6;
                    }
                    break;
                case COMPARTMENTS:
                    {
                    alt5=7;
                    }
                    break;
                case SEED:
                case SPECIES:
                    {
                    alt5=3;
                    }
                    break;
                default:
                    NoViableAltException nvae =
                        new NoViableAltException("", 5, 1, input);

                    throw nvae;
                }

            }
            else {
                NoViableAltException nvae =
                    new NoViableAltException("", 5, 0, input);

                throw nvae;
            }
            switch (alt5) {
                case 1 :
                    // /home/proto/workspace/bionetgen/BNGParser/src/bngparser/grammars/BNGGrammar.g:88:3: parameters_block[memory,$prog::parameters]
                    {
                    pushFollow(FOLLOW_parameters_block_in_program_block239);
                    parameters_block(memory, ((prog_scope)prog_stack.peek()).parameters);

                    state._fsp--;


                    }
                    break;
                case 2 :
                    // /home/proto/workspace/bionetgen/BNGParser/src/bngparser/grammars/BNGGrammar.g:89:3: molecule_types_block
                    {
                    pushFollow(FOLLOW_molecule_types_block_in_program_block244);
                    molecule_types_block();

                    state._fsp--;


                    }
                    break;
                case 3 :
                    // /home/proto/workspace/bionetgen/BNGParser/src/bngparser/grammars/BNGGrammar.g:90:3: seed_species_block[$prog::seedSpecies]
                    {
                    pushFollow(FOLLOW_seed_species_block_in_program_block248);
                    seed_species_block(((prog_scope)prog_stack.peek()).seedSpecies);

                    state._fsp--;


                    }
                    break;
                case 4 :
                    // /home/proto/workspace/bionetgen/BNGParser/src/bngparser/grammars/BNGGrammar.g:91:3: observables_block[$prog::observables]
                    {
                    pushFollow(FOLLOW_observables_block_in_program_block253);
                    observables_block(((prog_scope)prog_stack.peek()).observables);

                    state._fsp--;


                    }
                    break;
                case 5 :
                    // /home/proto/workspace/bionetgen/BNGParser/src/bngparser/grammars/BNGGrammar.g:92:3: reaction_rules_block[$prog::reactionRules]
                    {
                    pushFollow(FOLLOW_reaction_rules_block_in_program_block258);
                    reaction_rules_block(((prog_scope)prog_stack.peek()).reactionRules);

                    state._fsp--;


                    }
                    break;
                case 6 :
                    // /home/proto/workspace/bionetgen/BNGParser/src/bngparser/grammars/BNGGrammar.g:93:3: functions_block
                    {
                    pushFollow(FOLLOW_functions_block_in_program_block263);
                    functions_block();

                    state._fsp--;


                    }
                    break;
                case 7 :
                    // /home/proto/workspace/bionetgen/BNGParser/src/bngparser/grammars/BNGGrammar.g:94:3: compartments_block
                    {
                    pushFollow(FOLLOW_compartments_block_in_program_block267);
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
    // /home/proto/workspace/bionetgen/BNGParser/src/bngparser/grammars/BNGGrammar.g:100:1: functions_block : BEGIN FUNCTIONS ( function_def )* END FUNCTIONS ;
    public final BNGGrammar.functions_block_return functions_block() throws RecognitionException {
        BNGGrammar.functions_block_return retval = new BNGGrammar.functions_block_return();
        retval.start = input.LT(1);

        BNGGrammar.function_def_return function_def1 = null;



          paraphrases.push("in function block");

        try {
            // /home/proto/workspace/bionetgen/BNGParser/src/bngparser/grammars/BNGGrammar.g:107:1: ( BEGIN FUNCTIONS ( function_def )* END FUNCTIONS )
            // /home/proto/workspace/bionetgen/BNGParser/src/bngparser/grammars/BNGGrammar.g:108:7: BEGIN FUNCTIONS ( function_def )* END FUNCTIONS
            {
            match(input,BEGIN,FOLLOW_BEGIN_in_functions_block293); 
            match(input,FUNCTIONS,FOLLOW_FUNCTIONS_in_functions_block295); 
            // /home/proto/workspace/bionetgen/BNGParser/src/bngparser/grammars/BNGGrammar.g:109:8: ( function_def )*
            loop6:
            do {
                int alt6=2;
                int LA6_0 = input.LA(1);

                if ( (LA6_0==STRING) ) {
                    alt6=1;
                }


                switch (alt6) {
            	case 1 :
            	    // /home/proto/workspace/bionetgen/BNGParser/src/bngparser/grammars/BNGGrammar.g:109:9: function_def
            	    {
            	    pushFollow(FOLLOW_function_def_in_functions_block306);
            	    function_def1=function_def();

            	    state._fsp--;

            	    ((prog_scope)prog_stack.peek()).functions.add((function_def1!=null?function_def1.st:null));

            	    }
            	    break;

            	default :
            	    break loop6;
                }
            } while (true);

            match(input,END,FOLLOW_END_in_functions_block323); 
            match(input,FUNCTIONS,FOLLOW_FUNCTIONS_in_functions_block325); 

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

    public static class function_def_return extends ParserRuleReturnScope {
        public StringTemplate st;
        public Object getTemplate() { return st; }
        public String toString() { return st==null?null:st.toString(); }
    };

    // $ANTLR start "function_def"
    // /home/proto/workspace/bionetgen/BNGParser/src/bngparser/grammars/BNGGrammar.g:113:1: function_def : s1= STRING LPAREN ( STRING )? RPAREN ( BECOMES )? expression[memory] -> functions_block(id=$s1.textreferencesName=$expression.reference.keySet()referencesType=Register.getTypes($expression.reference)expression=$expression.text);
    public final BNGGrammar.function_def_return function_def() throws RecognitionException {
        BNGGrammar.function_def_return retval = new BNGGrammar.function_def_return();
        retval.start = input.LT(1);

        Token s1=null;
        BNGGrammar_BNGGrammar_Expression.expression_return expression2 = null;


        try {
            // /home/proto/workspace/bionetgen/BNGParser/src/bngparser/grammars/BNGGrammar.g:113:13: (s1= STRING LPAREN ( STRING )? RPAREN ( BECOMES )? expression[memory] -> functions_block(id=$s1.textreferencesName=$expression.reference.keySet()referencesType=Register.getTypes($expression.reference)expression=$expression.text))
            // /home/proto/workspace/bionetgen/BNGParser/src/bngparser/grammars/BNGGrammar.g:114:5: s1= STRING LPAREN ( STRING )? RPAREN ( BECOMES )? expression[memory]
            {
            s1=(Token)match(input,STRING,FOLLOW_STRING_in_function_def339); 
            match(input,LPAREN,FOLLOW_LPAREN_in_function_def341); 
            // /home/proto/workspace/bionetgen/BNGParser/src/bngparser/grammars/BNGGrammar.g:114:22: ( STRING )?
            int alt7=2;
            int LA7_0 = input.LA(1);

            if ( (LA7_0==STRING) ) {
                alt7=1;
            }
            switch (alt7) {
                case 1 :
                    // /home/proto/workspace/bionetgen/BNGParser/src/bngparser/grammars/BNGGrammar.g:114:23: STRING
                    {
                    match(input,STRING,FOLLOW_STRING_in_function_def344); 

                    }
                    break;

            }

            match(input,RPAREN,FOLLOW_RPAREN_in_function_def348); 
            // /home/proto/workspace/bionetgen/BNGParser/src/bngparser/grammars/BNGGrammar.g:114:39: ( BECOMES )?
            int alt8=2;
            int LA8_0 = input.LA(1);

            if ( (LA8_0==BECOMES) ) {
                alt8=1;
            }
            switch (alt8) {
                case 1 :
                    // /home/proto/workspace/bionetgen/BNGParser/src/bngparser/grammars/BNGGrammar.g:114:40: BECOMES
                    {
                    match(input,BECOMES,FOLLOW_BECOMES_in_function_def351); 

                    }
                    break;

            }

            pushFollow(FOLLOW_expression_in_function_def356);
            expression2=expression(memory);

            state._fsp--;



            // TEMPLATE REWRITE
            // 114:70: -> functions_block(id=$s1.textreferencesName=$expression.reference.keySet()referencesType=Register.getTypes($expression.reference)expression=$expression.text)
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
    // /home/proto/workspace/bionetgen/BNGParser/src/bngparser/grammars/BNGGrammar.g:118:1: compartments_block : BEGIN COMPARTMENTS ( compartment )* END COMPARTMENTS ;
    public final BNGGrammar.compartments_block_return compartments_block() throws RecognitionException {
        BNGGrammar.compartments_block_return retval = new BNGGrammar.compartments_block_return();
        retval.start = input.LT(1);

        BNGGrammar.compartment_return compartment3 = null;


        try {
            // /home/proto/workspace/bionetgen/BNGParser/src/bngparser/grammars/BNGGrammar.g:118:19: ( BEGIN COMPARTMENTS ( compartment )* END COMPARTMENTS )
            // /home/proto/workspace/bionetgen/BNGParser/src/bngparser/grammars/BNGGrammar.g:120:3: BEGIN COMPARTMENTS ( compartment )* END COMPARTMENTS
            {
            match(input,BEGIN,FOLLOW_BEGIN_in_compartments_block419); 
            match(input,COMPARTMENTS,FOLLOW_COMPARTMENTS_in_compartments_block421); 
            // /home/proto/workspace/bionetgen/BNGParser/src/bngparser/grammars/BNGGrammar.g:121:7: ( compartment )*
            loop9:
            do {
                int alt9=2;
                int LA9_0 = input.LA(1);

                if ( (LA9_0==STRING) ) {
                    alt9=1;
                }


                switch (alt9) {
            	case 1 :
            	    // /home/proto/workspace/bionetgen/BNGParser/src/bngparser/grammars/BNGGrammar.g:121:8: compartment
            	    {
            	    pushFollow(FOLLOW_compartment_in_compartments_block430);
            	    compartment3=compartment();

            	    state._fsp--;

            	    ((prog_scope)prog_stack.peek()).compartments.add((compartment3!=null?compartment3.st:null));

            	    }
            	    break;

            	default :
            	    break loop9;
                }
            } while (true);

            match(input,END,FOLLOW_END_in_compartments_block438); 
            match(input,COMPARTMENTS,FOLLOW_COMPARTMENTS_in_compartments_block440); 

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
    // /home/proto/workspace/bionetgen/BNGParser/src/bngparser/grammars/BNGGrammar.g:124:1: compartment : s1= STRING INT s3= expression[memory] (s2= STRING )? -> compartments_block(id=$s1.textdimensions=$INT.textsize=$s3.valueoutside=$s2.text);
    public final BNGGrammar.compartment_return compartment() throws RecognitionException {
        BNGGrammar.compartment_return retval = new BNGGrammar.compartment_return();
        retval.start = input.LT(1);

        Token s1=null;
        Token s2=null;
        Token INT4=null;
        BNGGrammar_BNGGrammar_Expression.expression_return s3 = null;


        try {
            // /home/proto/workspace/bionetgen/BNGParser/src/bngparser/grammars/BNGGrammar.g:124:12: (s1= STRING INT s3= expression[memory] (s2= STRING )? -> compartments_block(id=$s1.textdimensions=$INT.textsize=$s3.valueoutside=$s2.text))
            // /home/proto/workspace/bionetgen/BNGParser/src/bngparser/grammars/BNGGrammar.g:125:4: s1= STRING INT s3= expression[memory] (s2= STRING )?
            {
            s1=(Token)match(input,STRING,FOLLOW_STRING_in_compartment454); 
            INT4=(Token)match(input,INT,FOLLOW_INT_in_compartment456); 
            pushFollow(FOLLOW_expression_in_compartment460);
            s3=expression(memory);

            state._fsp--;

            // /home/proto/workspace/bionetgen/BNGParser/src/bngparser/grammars/BNGGrammar.g:125:40: (s2= STRING )?
            int alt10=2;
            int LA10_0 = input.LA(1);

            if ( (LA10_0==STRING) ) {
                int LA10_1 = input.LA(2);

                if ( (LA10_1==END||LA10_1==STRING) ) {
                    alt10=1;
                }
            }
            switch (alt10) {
                case 1 :
                    // /home/proto/workspace/bionetgen/BNGParser/src/bngparser/grammars/BNGGrammar.g:125:41: s2= STRING
                    {
                    s2=(Token)match(input,STRING,FOLLOW_STRING_in_compartment466); 

                    }
                    break;

            }



            // TEMPLATE REWRITE
            // 125:53: -> compartments_block(id=$s1.textdimensions=$INT.textsize=$s3.valueoutside=$s2.text)
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
    // /home/proto/workspace/bionetgen/BNGParser/src/bngparser/grammars/BNGGrammar.g:128:1: label returns [String label] : MOD STRING ;
    public final BNGGrammar.label_return label() throws RecognitionException {
        BNGGrammar.label_return retval = new BNGGrammar.label_return();
        retval.start = input.LT(1);

        Token STRING5=null;

        try {
            // /home/proto/workspace/bionetgen/BNGParser/src/bngparser/grammars/BNGGrammar.g:128:29: ( MOD STRING )
            // /home/proto/workspace/bionetgen/BNGParser/src/bngparser/grammars/BNGGrammar.g:129:3: MOD STRING
            {
            match(input,MOD,FOLLOW_MOD_in_label506); 
            STRING5=(Token)match(input,STRING,FOLLOW_STRING_in_label508); 
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
    // $ANTLR end "label"

    public static class dereference_return extends ParserRuleReturnScope {
        public String label;
        public StringTemplate st;
        public Object getTemplate() { return st; }
        public String toString() { return st==null?null:st.toString(); }
    };

    // $ANTLR start "dereference"
    // /home/proto/workspace/bionetgen/BNGParser/src/bngparser/grammars/BNGGrammar.g:131:1: dereference returns [String label] : AMPERSAND STRING ;
    public final BNGGrammar.dereference_return dereference() throws RecognitionException {
        BNGGrammar.dereference_return retval = new BNGGrammar.dereference_return();
        retval.start = input.LT(1);

        Token STRING6=null;

        try {
            // /home/proto/workspace/bionetgen/BNGParser/src/bngparser/grammars/BNGGrammar.g:131:35: ( AMPERSAND STRING )
            // /home/proto/workspace/bionetgen/BNGParser/src/bngparser/grammars/BNGGrammar.g:132:3: AMPERSAND STRING
            {
            match(input,AMPERSAND,FOLLOW_AMPERSAND_in_dereference524); 
            STRING6=(Token)match(input,STRING,FOLLOW_STRING_in_dereference526); 
            retval.label = (STRING6!=null?STRING6.getText():null);

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
    public BNGGrammar_BNGGrammar_Actions.generate_network_return generate_network() throws RecognitionException { return gBNGGrammar_Actions.generate_network(); }
    public BNGGrammar_BNGGrammar_SeedSpecies.bond_name_return bond_name() throws RecognitionException { return gBNGGrammar_SeedSpecies.bond_name(); }
    public BNGGrammar_BNGGrammar_Actions.read_file_return read_file() throws RecognitionException { return gBNGGrammar_Actions.read_file(); }
    public BNGGrammar_BNGGrammar_Observables.pattern_list_return pattern_list(String upperID) throws RecognitionException { return gBNGGrammar_Observables.pattern_list(upperID); }
    public BNGGrammar_BNGGrammar_MoleculeDef.site_def_return site_def() throws RecognitionException { return gBNGGrammar_MoleculeDef.site_def(); }
    public BNGGrammar_BNGGrammar_Actions.simulate_par_def_return simulate_par_def(Map<String,String> map) throws RecognitionException { return gBNGGrammar_Actions.simulate_par_def(map); }
    public BNGGrammar_BNGGrammar_Observables.observable_def_line_return observable_def_line(String upperID) throws RecognitionException { return gBNGGrammar_Observables.observable_def_line(upperID); }
    public BNGGrammar_BNGGrammar_Actions.assignment_list_return assignment_list() throws RecognitionException { return gBNGGrammar_Actions.assignment_list(); }
    public BNGGrammar_BNGGrammar_Actions.reset_concentrations_return reset_concentrations() throws RecognitionException { return gBNGGrammar_Actions.reset_concentrations(); }
    public BNGGrammar_BNGGrammar_ReactionRules.pattern_return pattern() throws RecognitionException { return gBNGGrammar_ReactionRules.pattern(); }
    public BNGGrammar_BNGGrammar_Parameters.parameters_block_return parameters_block(Map<String,Register> memory, List parameters) throws RecognitionException { return gBNGGrammar_Parameters.parameters_block(memory, parameters); }
    public BNGGrammar_BNGGrammar_Actions.ps_par_def_return ps_par_def(Map<String,String> map) throws RecognitionException { return gBNGGrammar_Actions.ps_par_def(map); }
    public BNGGrammar_BNGGrammar_Actions.variable_definition_return variable_definition() throws RecognitionException { return gBNGGrammar_Actions.variable_definition(); }
    public BNGGrammar_BNGGrammar_SeedSpecies.species_element_return species_element(BondList bonds, String upperID, String compartment) throws RecognitionException { return gBNGGrammar_SeedSpecies.species_element(bonds, upperID, compartment); }
    public BNGGrammar_BNGGrammar_Expression.number_return number() throws RecognitionException { return gBNGGrammar_Expression.number(); }
    public BNGGrammar_BNGGrammar_Expression.multiplying_expression_return multiplying_expression() throws RecognitionException { return gBNGGrammar_Expression.multiplying_expression(); }
    public BNGGrammar_BNGGrammar_SeedSpecies.pre_species_def_return pre_species_def(List molecules, BondList bonds, int speciesCounter) throws RecognitionException { return gBNGGrammar_SeedSpecies.pre_species_def(molecules, bonds, speciesCounter); }
    public BNGGrammar_BNGGrammar_Actions.simulate_ode_return simulate_ode(Map<String,String> map) throws RecognitionException { return gBNGGrammar_Actions.simulate_ode(map); }
    public BNGGrammar_BNGGrammar_Expression.expression2_return expression2() throws RecognitionException { return gBNGGrammar_Expression.expression2(); }
    public BNGGrammar_BNGGrammar_ReactionRules.rate_list_return rate_list(List<Double> rateList) throws RecognitionException { return gBNGGrammar_ReactionRules.rate_list(rateList); }
    public BNGGrammar_BNGGrammar_Expression.variable_return variable() throws RecognitionException { return gBNGGrammar_Expression.variable(); }
    public BNGGrammar_BNGGrammar_Observables.observables_block_return observables_block(List observables) throws RecognitionException { return gBNGGrammar_Observables.observables_block(observables); }
    public BNGGrammar_BNGGrammar_Expression.expression_return expression(Map<String,Register> memory) throws RecognitionException { return gBNGGrammar_Expression.expression(memory); }
    public BNGGrammar_BNGGrammar_ReactionRules.reaction_rule_def_return reaction_rule_def(int reactionCounter) throws RecognitionException { return gBNGGrammar_ReactionRules.reaction_rule_def(reactionCounter); }
    public BNGGrammar_BNGGrammar_Parameters.parameter_def_return parameter_def(Map<String,Register> lmemory) throws RecognitionException { return gBNGGrammar_Parameters.parameter_def(lmemory); }
    public BNGGrammar_BNGGrammar_MoleculeDef.molecule_types_block_return molecule_types_block() throws RecognitionException { return gBNGGrammar_MoleculeDef.molecule_types_block(); }
    public BNGGrammar_BNGGrammar_ReactionRules.modif_command_return modif_command() throws RecognitionException { return gBNGGrammar_ReactionRules.modif_command(); }
    public BNGGrammar_BNGGrammar_Actions.action_return action() throws RecognitionException { return gBNGGrammar_Actions.action(); }
    public BNGGrammar_BNGGrammar_Observables.obs_species_def_return obs_species_def(String upperID, ReactionAction reactionAction) throws RecognitionException { return gBNGGrammar_Observables.obs_species_def(upperID, reactionAction); }
    public BNGGrammar_BNGGrammar_SeedSpecies.site_obs_site_return site_obs_site(BondList bonds, String upperID) throws RecognitionException { return gBNGGrammar_SeedSpecies.site_obs_site(bonds, upperID); }
    public BNGGrammar_BNGGrammar_Actions.array_value_return array_value() throws RecognitionException { return gBNGGrammar_Actions.array_value(); }
    public BNGGrammar_BNGGrammar_Expression.sign_expression_return sign_expression() throws RecognitionException { return gBNGGrammar_Expression.sign_expression(); }
    public BNGGrammar_BNGGrammar_ReactionRules.reactionLabel_return reactionLabel() throws RecognitionException { return gBNGGrammar_ReactionRules.reactionLabel(); }
    public BNGGrammar_BNGGrammar_SeedSpecies.seed_species_def_return seed_species_def(int counter) throws RecognitionException { return gBNGGrammar_SeedSpecies.seed_species_def(counter); }
    public BNGGrammar_BNGGrammar_MoleculeDef.molecule_def_return molecule_def() throws RecognitionException { return gBNGGrammar_MoleculeDef.molecule_def(); }
    public BNGGrammar_BNGGrammar_Actions.simulate_ode_par_def_return simulate_ode_par_def(Map<String,String> map) throws RecognitionException { return gBNGGrammar_Actions.simulate_ode_par_def(map); }
    public BNGGrammar_BNGGrammar_Expression.adding_expression_return adding_expression() throws RecognitionException { return gBNGGrammar_Expression.adding_expression(); }
    public BNGGrammar_BNGGrammar_Observables.observable_type_return observable_type() throws RecognitionException { return gBNGGrammar_Observables.observable_type(); }
    public BNGGrammar_BNGGrammar_Actions.parameter_definition_return parameter_definition() throws RecognitionException { return gBNGGrammar_Actions.parameter_definition(); }
    public BNGGrammar_BNGGrammar_Actions.set_concentration_return set_concentration() throws RecognitionException { return gBNGGrammar_Actions.set_concentration(); }
    public BNGGrammar_BNGGrammar_SeedSpecies.site_obs_return site_obs(BondList bonds, String upperID) throws RecognitionException { return gBNGGrammar_SeedSpecies.site_obs(bonds, upperID); }
    public BNGGrammar_BNGGrammar_SeedSpecies.seed_species_block_return seed_species_block(List seedSpecies) throws RecognitionException { return gBNGGrammar_SeedSpecies.seed_species_block(seedSpecies); }
    public BNGGrammar_BNGGrammar_ReactionRules.rule_species_def_return rule_species_def(String upperID, ReactionAction reactionAction) throws RecognitionException { return gBNGGrammar_ReactionRules.rule_species_def(upperID, reactionAction); }
    public BNGGrammar_BNGGrammar_Actions.write_file_return write_file() throws RecognitionException { return gBNGGrammar_Actions.write_file(); }
    public BNGGrammar_BNGGrammar_ReactionRules.reaction_def_return reaction_def(List patternsReactants, List patternsProducts, String upperID) throws RecognitionException { return gBNGGrammar_ReactionRules.reaction_def(patternsReactants, patternsProducts, upperID); }
    public BNGGrammar_BNGGrammar_Actions.gn_action_par_def_return gn_action_par_def(Map<String,String> map) throws RecognitionException { return gBNGGrammar_Actions.gn_action_par_def(map); }
    public BNGGrammar_BNGGrammar_Actions.simulate_method_return simulate_method() throws RecognitionException { return gBNGGrammar_Actions.simulate_method(); }
    public BNGGrammar_BNGGrammar_Expression.boolean_negation_expression_return boolean_negation_expression() throws RecognitionException { return gBNGGrammar_Expression.boolean_negation_expression(); }
    public BNGGrammar_BNGGrammar_MoleculeDef.state_label_return state_label() throws RecognitionException { return gBNGGrammar_MoleculeDef.state_label(); }
    public BNGGrammar_BNGGrammar_Actions.hash_value_return hash_value() throws RecognitionException { return gBNGGrammar_Actions.hash_value(); }
    public BNGGrammar_BNGGrammar_ReactionRules.exclude_command_return exclude_command() throws RecognitionException { return gBNGGrammar_ReactionRules.exclude_command(); }
    public BNGGrammar_BNGGrammar_ReactionRules.include_command_return include_command() throws RecognitionException { return gBNGGrammar_ReactionRules.include_command(); }
    public BNGGrammar_BNGGrammar_Actions.simulate_ssa_return simulate_ssa(Map<String,String> map) throws RecognitionException { return gBNGGrammar_Actions.simulate_ssa(map); }
    public BNGGrammar_BNGGrammar_Expression.relational_expression_return relational_expression() throws RecognitionException { return gBNGGrammar_Expression.relational_expression(); }
    public BNGGrammar_BNGGrammar_Actions.save_concentrations_return save_concentrations() throws RecognitionException { return gBNGGrammar_Actions.save_concentrations(); }
    public BNGGrammar_BNGGrammar_Actions.set_parameter_return set_parameter() throws RecognitionException { return gBNGGrammar_Actions.set_parameter(); }
    public BNGGrammar_BNGGrammar_Actions.actions_block_return actions_block() throws RecognitionException { return gBNGGrammar_Actions.actions_block(); }
    public BNGGrammar_BNGGrammar_Expression.primitive_element_return primitive_element() throws RecognitionException { return gBNGGrammar_Expression.primitive_element(); }
    public BNGGrammar_BNGGrammar_Actions.write_type_return write_type() throws RecognitionException { return gBNGGrammar_Actions.write_type(); }
    public BNGGrammar_BNGGrammar_Actions.actions_prog_return actions_prog() throws RecognitionException { return gBNGGrammar_Actions.actions_prog(); }
    public BNGGrammar_BNGGrammar_ReactionRules.reaction_rules_block_return reaction_rules_block(List reactionRules) throws RecognitionException { return gBNGGrammar_ReactionRules.reaction_rules_block(reactionRules); }
    public BNGGrammar_BNGGrammar_SeedSpecies.species_def_return species_def(List molecules, BondList bonds, String speciesCounter) throws RecognitionException { return gBNGGrammar_SeedSpecies.species_def(molecules, bonds, speciesCounter); }
    public BNGGrammar_BNGGrammar_Actions.value_return value() throws RecognitionException { return gBNGGrammar_Actions.value(); }
    public BNGGrammar_BNGGrammar_SeedSpecies.site_list_return site_list(List sites, BondList bonds, String upperID) throws RecognitionException { return gBNGGrammar_SeedSpecies.site_list(sites, bonds, upperID); }
    public BNGGrammar_BNGGrammar_SeedSpecies.site_obs_state_return site_obs_state() throws RecognitionException { return gBNGGrammar_SeedSpecies.site_obs_state(); }
    public BNGGrammar_BNGGrammar_ReactionRules.match_attribute_return match_attribute() throws RecognitionException { return gBNGGrammar_ReactionRules.match_attribute(); }


 

    public static final BitSet FOLLOW_BEGIN_in_prog94 = new BitSet(new long[]{0x0000000000004000L});
    public static final BitSet FOLLOW_MODEL_in_prog96 = new BitSet(new long[]{0x0000000000000180L});
    public static final BitSet FOLLOW_program_block_in_prog99 = new BitSet(new long[]{0x0000000000000180L});
    public static final BitSet FOLLOW_END_in_prog103 = new BitSet(new long[]{0x0000000000004000L});
    public static final BitSet FOLLOW_MODEL_in_prog105 = new BitSet(new long[]{0xEE00000000000100L,0x000000000000003FL});
    public static final BitSet FOLLOW_program_block_in_prog111 = new BitSet(new long[]{0xEE00000000000100L,0x000000000000003FL});
    public static final BitSet FOLLOW_actions_block_in_prog119 = new BitSet(new long[]{0x0000000000000000L});
    public static final BitSet FOLLOW_BEGIN_in_prog125 = new BitSet(new long[]{0x0000000000200000L});
    public static final BitSet FOLLOW_ACTIONS_in_prog127 = new BitSet(new long[]{0xEE00000000000000L,0x000000000000003FL});
    public static final BitSet FOLLOW_actions_block_in_prog131 = new BitSet(new long[]{0x0000000000000080L});
    public static final BitSet FOLLOW_END_in_prog133 = new BitSet(new long[]{0x0000000000200000L});
    public static final BitSet FOLLOW_ACTIONS_in_prog135 = new BitSet(new long[]{0x0000000000000000L});
    public static final BitSet FOLLOW_EOF_in_prog144 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_VERSION_in_version_def217 = new BitSet(new long[]{0x0000000000000000L,0x0000000000200000L});
    public static final BitSet FOLLOW_LPAREN_in_version_def219 = new BitSet(new long[]{0x0000000000000000L,0x0000040000000000L});
    public static final BitSet FOLLOW_DBQUOTES_in_version_def221 = new BitSet(new long[]{0x0000000000000000L,0x0000800000000000L});
    public static final BitSet FOLLOW_VERSION_NUMBER_in_version_def223 = new BitSet(new long[]{0x0000000000000000L,0x0000040000000000L});
    public static final BitSet FOLLOW_DBQUOTES_in_version_def225 = new BitSet(new long[]{0x0000000000000000L,0x0000000000400000L});
    public static final BitSet FOLLOW_RPAREN_in_version_def227 = new BitSet(new long[]{0x0000000000000000L,0x0000000000002000L});
    public static final BitSet FOLLOW_SEMI_in_version_def229 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_parameters_block_in_program_block239 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_molecule_types_block_in_program_block244 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_seed_species_block_in_program_block248 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_observables_block_in_program_block253 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_reaction_rules_block_in_program_block258 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_functions_block_in_program_block263 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_compartments_block_in_program_block267 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_BEGIN_in_functions_block293 = new BitSet(new long[]{0x0000000000080000L});
    public static final BitSet FOLLOW_FUNCTIONS_in_functions_block295 = new BitSet(new long[]{0x0000000000000080L,0x0000000000001000L});
    public static final BitSet FOLLOW_function_def_in_functions_block306 = new BitSet(new long[]{0x0000000000000080L,0x0000000000001000L});
    public static final BitSet FOLLOW_END_in_functions_block323 = new BitSet(new long[]{0x0000000000080000L});
    public static final BitSet FOLLOW_FUNCTIONS_in_functions_block325 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_STRING_in_function_def339 = new BitSet(new long[]{0x0000000000000000L,0x0000000000200000L});
    public static final BitSet FOLLOW_LPAREN_in_function_def341 = new BitSet(new long[]{0x0000000000000000L,0x0000000000401000L});
    public static final BitSet FOLLOW_STRING_in_function_def344 = new BitSet(new long[]{0x0000000000000000L,0x0000000000400000L});
    public static final BitSet FOLLOW_RPAREN_in_function_def348 = new BitSet(new long[]{0x0000100000000000L,0x0000002400201600L});
    public static final BitSet FOLLOW_BECOMES_in_function_def351 = new BitSet(new long[]{0x0000100000000000L,0x0000002400201600L});
    public static final BitSet FOLLOW_expression_in_function_def356 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_BEGIN_in_compartments_block419 = new BitSet(new long[]{0x0000000000100000L});
    public static final BitSet FOLLOW_COMPARTMENTS_in_compartments_block421 = new BitSet(new long[]{0x0000000000000080L,0x0000000000001000L});
    public static final BitSet FOLLOW_compartment_in_compartments_block430 = new BitSet(new long[]{0x0000000000000080L,0x0000000000001000L});
    public static final BitSet FOLLOW_END_in_compartments_block438 = new BitSet(new long[]{0x0000000000100000L});
    public static final BitSet FOLLOW_COMPARTMENTS_in_compartments_block440 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_STRING_in_compartment454 = new BitSet(new long[]{0x0000000000000000L,0x0000000000000400L});
    public static final BitSet FOLLOW_INT_in_compartment456 = new BitSet(new long[]{0x0000100000000000L,0x0000002400201600L});
    public static final BitSet FOLLOW_expression_in_compartment460 = new BitSet(new long[]{0x0000000000000002L,0x0000000000001000L});
    public static final BitSet FOLLOW_STRING_in_compartment466 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_MOD_in_label506 = new BitSet(new long[]{0x0000000000000000L,0x0000000000001000L});
    public static final BitSet FOLLOW_STRING_in_label508 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_AMPERSAND_in_dereference524 = new BitSet(new long[]{0x0000000000000000L,0x0000000000001000L});
    public static final BitSet FOLLOW_STRING_in_dereference526 = new BitSet(new long[]{0x0000000000000002L});

}