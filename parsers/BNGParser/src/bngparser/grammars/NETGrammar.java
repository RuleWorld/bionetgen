// $ANTLR 3.3 Nov 30, 2010 12:50:56 /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/NETGrammar.g 2013-01-14 18:13:02

  package bngparser.grammars;
  import bngparser.grammars.BNGLexer;
  import org.antlr.stringtemplate.*;
  import bngparser.dataType.BondList;
  import java.util.Map;
  import bngparser.methods.ReactionAction;
  import bngparser.dataType.Register;
  import bngparser.methods.InvertBidirectional;
  import bngparser.dataType.ChangeableChannelTokenStream;
  import bngparser.dataType.ReactionRegister;
  import bngparser.dataType.Compartments;
  import bngparser.methods.GenericMethods;


import org.antlr.runtime.*;
import java.util.Stack;
import java.util.List;
import java.util.ArrayList;

import org.antlr.stringtemplate.*;
import org.antlr.stringtemplate.language.*;
import java.util.HashMap;
public class NETGrammar extends Parser {
    public static final String[] tokenNames = new String[] {
        "<invalid>", "<EOR>", "<DOWN>", "<UP>", "LINE_COMMENT", "LB", "WS", "VERSION", "END", "BEGIN", "SEED", "SPECIES", "GROUPS", "MOLECULE", "MOLECULES", "MODEL", "TYPES", "REACTION", "REACTIONS", "RULES", "FUNCTIONS", "COMPARTMENTS", "ACTIONS", "OBSERVABLES", "POPULATION", "MAPS", "SET_OPTION", "SPECIES_LABEL", "GENERATE_NETWORK", "MAX_AGG", "MAX_ITER", "MAX_STOICH", "OVERWRITE", "PRINT_ITER", "AND", "OR", "ID", "IF", "PREFFIX", "SUFFIX", "LABEL", "PARAMETERS", "VERBOSE", "GML", "MATCHONCE", "DELETEMOLECULES", "MOVECONNECTED", "AUTO", "HNAUTY", "QUASY", "NOT", "INCLUDE_REACTANTS", "INCLUDE_PRODUCTS", "EXCLUDE_REACTANTS", "EXCLUDE_PRODUCTS", "GENERATEHYBRIDMODEL", "ATOL", "RTOL", "STEADY_STATE", "SPARSE", "T_END", "T_START", "N_STEPS", "N_OUTPUT_STEPS", "MAX_SIM_STEPS", "OUTPUT_STEP_INTERVAL", "PLA_CONFIG", "SAMPLE_TIMES", "SIMULATE_ODE", "SIMULATE_SSA", "SIMULATE_NF", "SIMULATE_PLA", "SIMULATE", "READFILE", "FILE", "WRITENET", "WRITESBML", "WRITEMFILE", "WRITEXML", "WRITEMEXFILE", "PRINT_CDAT", "PRINT_FUNCTIONS", "SETCONCENTRATION", "METHOD", "SETPARAMETER", "SAVECONCENTRATIONS", "RESETCONCENTRATIONS", "ADDCONCENTRATION", "GENERATENETWORK", "WRITENETWORK", "SUBSTANCEUNITS", "PARAM", "COMPLEX", "GET_FINAL_STATE", "SAT", "EXP", "LOG", "ARGFILE", "SAVE_PROGRESS", "NETFILE", "PRINT_NET", "PRINT_END", "TEXTREACTION", "CONTINUE", "ODE", "SSA", "NF", "PLA", "EVALUATE_EXPRESSIONS", "BDF", "DIGIT", "EXPONENT", "FLOAT", "INT", "LETTER", "STRING", "SEMI", "COLON", "LSBRACKET", "RSBRACKET", "LBRACKET", "RBRACKET", "COMMA", "DOT", "LPAREN", "RPAREN", "UNI_REACTION_SIGN", "BI_REACTION_SIGN", "DOLLAR", "TILDE", "AT", "GTE", "GT", "LTE", "LT", "ASSIGNS", "EQUALS", "BECOMES", "DIV", "TIMES", "MINUS", "PLUS", "POWER", "MOD", "PIPE", "QMARK", "EMARK", "DBQUOTES", "AMPERSAND", "NULL", "LOWER", "UPPER", "VERSION_NUMBER", "153", "154", "155", "156", "157", "158", "159", "160", "161", "162", "163", "164", "165", "166", "167", "168", "169", "170", "171", "172", "173", "174", "175", "176", "177", "178", "179", "180", "181", "182", "183", "184", "185", "186", "187", "188", "189", "190", "191", "192", "193", "194", "195", "196", "197", "198", "199", "200", "201", "202", "203", "204", "205", "206", "207", "208", "209", "210"
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

    // delegates
    public NETGrammar_BNGGrammar_Expression gBNGGrammar_Expression;
    public NETGrammar_BNGGrammar_Parameters gBNGGrammar_Parameters;
    public NETGrammar_BNGGrammar_SeedSpecies gBNGGrammar_SeedSpecies;
    public NETGrammar_BNGGrammar_MoleculeDef gBNGGrammar_MoleculeDef;
    public NETGrammar_BNGGrammar_Observables gBNGGrammar_Observables;
    public NETGrammar_BNGGrammar_ReactionRules gBNGGrammar_ReactionRules;
    // delegators


        public NETGrammar(TokenStream input) {
            this(input, new RecognizerSharedState());
        }
        public NETGrammar(TokenStream input, RecognizerSharedState state) {
            super(input, state);
            gBNGGrammar_Expression = new NETGrammar_BNGGrammar_Expression(input, state, this);
            gBNGGrammar_Parameters = new NETGrammar_BNGGrammar_Parameters(input, state, this);
            gBNGGrammar_SeedSpecies = new NETGrammar_BNGGrammar_SeedSpecies(input, state, this);
            gBNGGrammar_MoleculeDef = new NETGrammar_BNGGrammar_MoleculeDef(input, state, this);
            gBNGGrammar_Observables = new NETGrammar_BNGGrammar_Observables(input, state, this);
            gBNGGrammar_ReactionRules = new NETGrammar_BNGGrammar_ReactionRules(input, state, this);         
        }
        
    protected StringTemplateGroup templateLib =
      new StringTemplateGroup("NETGrammarTemplates", AngleBracketTemplateLexer.class);

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

    public String[] getTokenNames() { return NETGrammar.tokenNames; }
    public String getGrammarFileName() { return "/home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/NETGrammar.g"; }


      public Stack<String> paraphrases = new Stack<String>();
      public Map<String,Register> memory = new HashMap<String,Register>();
      
      private Map<String, Map<String,String>> options = new HashMap<String, Map<String,String>>();

      public boolean netGrammar = true;
      
      //This is a pretty complex flag. 
      //Surface compartments require pretty special treatment as opposed to volume compartments
      //If this flag is not activated, it will treat all compartments the same, however if it is activated
      // it will generate the corresponding surface syntax. This is still experimental
      public boolean compartmentToSurface = true;
      public List<StringTemplate> molecules = new ArrayList<StringTemplate>();
      public List<StringTemplate> reactions = new ArrayList<StringTemplate>();
      public List<StringTemplate> groups = new ArrayList<StringTemplate>();
      public Map<String,String> speciesMap = new HashMap<String,String>();
      
      //this object contains a list of all comparments and their characteristics.
      public Compartments compartmentList = new Compartments();
      //this map contains a list of the surface compartments and which species (stringtemplates)are associated with them
      public Map<String, List<StringTemplate>> compartmentSurfaces = new HashMap<String,List<StringTemplate>>();
      //the same as the previous one but in this one we keep track of all compartments and only 
      //molecule names
      public Map<String, List<String>> compartmentMolecules = new HashMap<String,List<String>>();
      public List<String> surfaces = new ArrayList<String>();
      public String cyto_volume = "3.0e-12";
      public String ecm_volume = "1.0e-10";
      public double cyto_volume_value = 3.0e12;
      public double ecm_volume_value = 1.0e-10;
      public double avogadro_constant = 6.02e23;
      @Override
      public String getErrorMessage(RecognitionException e,String[] tokenNames){
        String msg = super.getErrorMessage(e,tokenNames);
        if(paraphrases.size(  )>0) {
          String paraphrase = paraphrases.peek();
          msg += " " + paraphrase;
        }
        return msg;
      }
      
      public void addOption(String element, String option, String setting){
        if(!options.containsKey(element)){
          options.put(element,new HashMap<String,String>());
        }
        
        (options.get(element)).put(option,setting);
      }
      
      public void addOptions(String element, Map<String,String> option){
        if(!options.containsKey(element)){
          options.put(element,option);
        }
        else{
          options.get(element).putAll(option);
        }
      }
      
      public void addSurfaceToCompartment(String surfaceName,StringTemplate template ){
         //String parent = compartmentList.getParentCompartment(surfaceName);
         if(!compartmentSurfaces.containsKey(surfaceName)){
            compartmentSurfaces.put(surfaceName,new ArrayList<StringTemplate>());
         }
         compartmentSurfaces.get(surfaceName).add(template);
      }
      
        public void addMoleculeToCompartment(String compartment,String name ){
         //String parent = compartmentList.getParentCompartment(surfaceName);
         if(!compartmentMolecules.containsKey(compartment)){
            compartmentMolecules.put(compartment,new ArrayList<String>());
         }
         compartmentMolecules.get(compartment).add(name);
      }
      
       public String getCompartment(String molecule){
        String temp = "";
        for(String compartment: compartmentMolecules.keySet()){
          if(compartmentMolecules.get(compartment).contains(molecule))
            return compartment;
        }
        
        return temp;
        
      }


    protected static class seed_species_block_scope {
        int numSpecies;
    }
    protected Stack seed_species_block_stack = new Stack();

    public static class seed_species_block_return extends ParserRuleReturnScope {
        public StringTemplate st;
        public Object getTemplate() { return st; }
        public String toString() { return st==null?null:st.toString(); }
    };

    // $ANTLR start "seed_species_block"
    // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/NETGrammar.g:110:1: seed_species_block[List seedSpecies] : BEGIN ( SEED )? SPECIES ( LB )+ ( seed_species_def[$seed_species_block::numSpecies] ( LB )+ )* END ( SEED )? SPECIES ( LB )+ ;
    public final NETGrammar.seed_species_block_return seed_species_block(List seedSpecies) throws RecognitionException {
        seed_species_block_stack.push(new seed_species_block_scope());
        NETGrammar.seed_species_block_return retval = new NETGrammar.seed_species_block_return();
        retval.start = input.LT(1);

        NETGrammar.seed_species_def_return seed_species_def1 = null;



         ((seed_species_block_scope)seed_species_block_stack.peek()).numSpecies = 1;
           paraphrases.push("in seed species section");


        try {
            // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/NETGrammar.g:126:1: ( BEGIN ( SEED )? SPECIES ( LB )+ ( seed_species_def[$seed_species_block::numSpecies] ( LB )+ )* END ( SEED )? SPECIES ( LB )+ )
            // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/NETGrammar.g:126:3: BEGIN ( SEED )? SPECIES ( LB )+ ( seed_species_def[$seed_species_block::numSpecies] ( LB )+ )* END ( SEED )? SPECIES ( LB )+
            {
            match(input,BEGIN,FOLLOW_BEGIN_in_seed_species_block84); 
            // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/NETGrammar.g:126:9: ( SEED )?
            int alt1=2;
            int LA1_0 = input.LA(1);

            if ( (LA1_0==SEED) ) {
                alt1=1;
            }
            switch (alt1) {
                case 1 :
                    // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/NETGrammar.g:126:10: SEED
                    {
                    match(input,SEED,FOLLOW_SEED_in_seed_species_block87); 

                    }
                    break;

            }

            match(input,SPECIES,FOLLOW_SPECIES_in_seed_species_block91); 
            // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/NETGrammar.g:126:25: ( LB )+
            int cnt2=0;
            loop2:
            do {
                int alt2=2;
                int LA2_0 = input.LA(1);

                if ( (LA2_0==LB) ) {
                    alt2=1;
                }


                switch (alt2) {
            	case 1 :
            	    // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/NETGrammar.g:126:25: LB
            	    {
            	    match(input,LB,FOLLOW_LB_in_seed_species_block93); 

            	    }
            	    break;

            	default :
            	    if ( cnt2 >= 1 ) break loop2;
                        EarlyExitException eee =
                            new EarlyExitException(2, input);
                        throw eee;
                }
                cnt2++;
            } while (true);

            // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/NETGrammar.g:127:1: ( seed_species_def[$seed_species_block::numSpecies] ( LB )+ )*
            loop4:
            do {
                int alt4=2;
                int LA4_0 = input.LA(1);

                if ( (LA4_0==INT) ) {
                    alt4=1;
                }


                switch (alt4) {
            	case 1 :
            	    // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/NETGrammar.g:127:2: seed_species_def[$seed_species_block::numSpecies] ( LB )+
            	    {
            	    pushFollow(FOLLOW_seed_species_def_in_seed_species_block97);
            	    seed_species_def1=seed_species_def(((seed_species_block_scope)seed_species_block_stack.peek()).numSpecies);

            	    state._fsp--;


            	                                          //System.out.println("aaaaa " + (seed_species_def1!=null?seed_species_def1.compartment:null));
            	                                          if(!(seed_species_def1!=null?seed_species_def1.concentration:null).equals("0")){
            	    	                                      if(!compartmentToSurface || compartmentList.isVolume((seed_species_def1!=null?seed_species_def1.compartment:null)))
            	    	                                        seedSpecies.add((seed_species_def1!=null?seed_species_def1.st:null));
            	    	                                      else
            	    	                                        addSurfaceToCompartment((seed_species_def1!=null?seed_species_def1.compartment:null),(seed_species_def1!=null?seed_species_def1.st:null));
            	                                          }
            	                                          addMoleculeToCompartment((seed_species_def1!=null?seed_species_def1.compartment:null),"S" + ((seed_species_block_scope)seed_species_block_stack.peek()).numSpecies);
            	                                          ((seed_species_block_scope)seed_species_block_stack.peek()).numSpecies++;
            	                                          //}
            	                                          
            	    // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/NETGrammar.g:138:41: ( LB )+
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
            	    	    // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/NETGrammar.g:138:41: LB
            	    	    {
            	    	    match(input,LB,FOLLOW_LB_in_seed_species_block101); 

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


            	    }
            	    break;

            	default :
            	    break loop4;
                }
            } while (true);

            match(input,END,FOLLOW_END_in_seed_species_block107); 
            // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/NETGrammar.g:139:5: ( SEED )?
            int alt5=2;
            int LA5_0 = input.LA(1);

            if ( (LA5_0==SEED) ) {
                alt5=1;
            }
            switch (alt5) {
                case 1 :
                    // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/NETGrammar.g:139:6: SEED
                    {
                    match(input,SEED,FOLLOW_SEED_in_seed_species_block110); 

                    }
                    break;

            }

            match(input,SPECIES,FOLLOW_SPECIES_in_seed_species_block114); 
            // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/NETGrammar.g:139:21: ( LB )+
            int cnt6=0;
            loop6:
            do {
                int alt6=2;
                int LA6_0 = input.LA(1);

                if ( (LA6_0==LB) ) {
                    alt6=1;
                }


                switch (alt6) {
            	case 1 :
            	    // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/NETGrammar.g:139:21: LB
            	    {
            	    match(input,LB,FOLLOW_LB_in_seed_species_block116); 

            	    }
            	    break;

            	default :
            	    if ( cnt6 >= 1 ) break loop6;
                        EarlyExitException eee =
                            new EarlyExitException(6, input);
                        throw eee;
                }
                cnt6++;
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
            seed_species_block_stack.pop();
        }
        return retval;
    }
    // $ANTLR end "seed_species_block"

    protected static class seed_species_def_scope {
        List molecules;
        BondList bonds;
        String location;
        boolean isVolume;
    }
    protected Stack seed_species_def_stack = new Stack();

    public static class seed_species_def_return extends ParserRuleReturnScope {
        public String compartment;
        public String concentration;
        public StringTemplate st;
        public Object getTemplate() { return st; }
        public String toString() { return st==null?null:st.toString(); }
    };

    // $ANTLR start "seed_species_def"
    // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/NETGrammar.g:143:1: seed_species_def[int counter] returns [String compartment, String concentration] : pre= pre_species_def[$seed_species_def::molecules,$seed_species_def::bonds,counter] expression[memory] -> seed_species_block(id=counterconcentration=$expression.textname=$pre_species_def.textmolecules=$seed_species_def::moleculesfirstBonds=$seed_species_def::bonds.getLeft()secondBonds=$seed_species_def::bonds.getRight()isVolume=$seed_species_def::isVolumelocation=$seed_species_def::location);
    public final NETGrammar.seed_species_def_return seed_species_def(int counter) throws RecognitionException {
        seed_species_def_stack.push(new seed_species_def_scope());
        NETGrammar.seed_species_def_return retval = new NETGrammar.seed_species_def_return();
        retval.start = input.LT(1);

        NETGrammar.pre_species_def_return pre = null;

        NETGrammar_BNGGrammar_Expression.expression_return expression2 = null;



          ((seed_species_def_scope)seed_species_def_stack.peek()).molecules = new ArrayList();
          ((seed_species_def_scope)seed_species_def_stack.peek()).bonds = new BondList();

        try {
            // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/NETGrammar.g:159:1: (pre= pre_species_def[$seed_species_def::molecules,$seed_species_def::bonds,counter] expression[memory] -> seed_species_block(id=counterconcentration=$expression.textname=$pre_species_def.textmolecules=$seed_species_def::moleculesfirstBonds=$seed_species_def::bonds.getLeft()secondBonds=$seed_species_def::bonds.getRight()isVolume=$seed_species_def::isVolumelocation=$seed_species_def::location))
            // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/NETGrammar.g:160:2: pre= pre_species_def[$seed_species_def::molecules,$seed_species_def::bonds,counter] expression[memory]
            {
            pushFollow(FOLLOW_pre_species_def_in_seed_species_def145);
            pre=pre_species_def(((seed_species_def_scope)seed_species_def_stack.peek()).molecules, ((seed_species_def_scope)seed_species_def_stack.peek()).bonds, counter);

            state._fsp--;


             //There needs to need a space between species and the expression token, so we go back and make sure there was one
            //((ChangeableChannelTokenStream)input).seek(((ChangeableChannelTokenStream)input).index()-1)  ;
            retval.compartment = (pre!=null?pre.compartment:null);
            ((seed_species_def_scope)seed_species_def_stack.peek()).isVolume = !compartmentToSurface || compartmentList.isVolume(pre.compartment);
             //System.out.println("aaaa " +input);
             //((ChangeableChannelTokenStream)input).seek(((ChangeableChannelTokenStream)input).index()-1)  ;

            pushFollow(FOLLOW_expression_in_seed_species_def164);
            expression2=expression(memory);

            state._fsp--;


                  retval.concentration = (expression2!=null?input.toString(expression2.start,expression2.stop):null);
                  //fixme: using static coordinates for now. Fix for something better
                  if(compartmentToSurface &&  compartmentList.isOuterCompartment(retval.compartment)){
                    ((seed_species_def_scope)seed_species_def_stack.peek()).location = "-0.15,-0.15,-0.15";
                  }
                  else{
                    ((seed_species_def_scope)seed_species_def_stack.peek()).location = "0,0,0";
                  }
                 


            // TEMPLATE REWRITE
            // 180:6: -> seed_species_block(id=counterconcentration=$expression.textname=$pre_species_def.textmolecules=$seed_species_def::moleculesfirstBonds=$seed_species_def::bonds.getLeft()secondBonds=$seed_species_def::bonds.getRight()isVolume=$seed_species_def::isVolumelocation=$seed_species_def::location)
            {
                retval.st = templateLib.getInstanceOf("seed_species_block",
              new STAttrMap().put("id", counter).put("concentration", (expression2!=null?input.toString(expression2.start,expression2.stop):null)).put("name", (pre!=null?input.toString(pre.start,pre.stop):null)).put("molecules", ((seed_species_def_scope)seed_species_def_stack.peek()).molecules).put("firstBonds", ((seed_species_def_scope)seed_species_def_stack.peek()).bonds.getLeft()).put("secondBonds", ((seed_species_def_scope)seed_species_def_stack.peek()).bonds.getRight()).put("isVolume", ((seed_species_def_scope)seed_species_def_stack.peek()).isVolume).put("location", ((seed_species_def_scope)seed_species_def_stack.peek()).location));
            }


            }

            retval.stop = input.LT(-1);

        }
        catch (RecognitionException re) {
            reportError(re);
            recover(input,re);
        }
        finally {
            seed_species_def_stack.pop();
        }
        return retval;
    }
    // $ANTLR end "seed_species_def"

    protected static class pre_species_def_scope {
        boolean constant;
    }
    protected Stack pre_species_def_stack = new Stack();

    public static class pre_species_def_return extends ParserRuleReturnScope {
        public String compartment;
        public StringTemplate st;
        public Object getTemplate() { return st; }
        public String toString() { return st==null?null:st.toString(); }
    };

    // $ANTLR start "pre_species_def"
    // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/NETGrammar.g:188:1: pre_species_def[List molecules,BondList bonds, int speciesCounter] returns [String compartment] : INT species_def[molecules,bonds,\"S\" + speciesCounter] ;
    public final NETGrammar.pre_species_def_return pre_species_def(List molecules, BondList bonds, int speciesCounter) throws RecognitionException {
        pre_species_def_stack.push(new pre_species_def_scope());
        NETGrammar.pre_species_def_return retval = new NETGrammar.pre_species_def_return();
        retval.start = input.LT(1);

        Token INT4=null;
        NETGrammar_BNGGrammar_SeedSpecies.species_def_return species_def3 = null;


        try {
            // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/NETGrammar.g:194:1: ( INT species_def[molecules,bonds,\"S\" + speciesCounter] )
            // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/NETGrammar.g:195:1: INT species_def[molecules,bonds,\"S\" + speciesCounter]
            {
            INT4=(Token)match(input,INT,FOLLOW_INT_in_pre_species_def265); 
            pushFollow(FOLLOW_species_def_in_pre_species_def271);
            species_def3=species_def(molecules, bonds, "S" + speciesCounter);

            state._fsp--;


                ((pre_species_def_scope)pre_species_def_stack.peek()).constant = (species_def3!=null?species_def3.constant:false);
                 speciesMap.put((species_def3!=null?input.toString(species_def3.start,species_def3.stop):null),(INT4!=null?INT4.getText():null));
                 retval.compartment = (species_def3!=null?species_def3.compartment:null);
             

            }

            retval.stop = input.LT(-1);

        }
        catch (RecognitionException re) {
            reportError(re);
            recover(input,re);
        }
        finally {
            pre_species_def_stack.pop();
        }
        return retval;
    }
    // $ANTLR end "pre_species_def"

    public static class groups_block_return extends ParserRuleReturnScope {
        public StringTemplate st;
        public Object getTemplate() { return st; }
        public String toString() { return st==null?null:st.toString(); }
    };

    // $ANTLR start "groups_block"
    // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/NETGrammar.g:206:1: groups_block : BEGIN GROUPS ( LB )+ ( group_line ( LB )+ )* END GROUPS ( LB )+ ;
    public final NETGrammar.groups_block_return groups_block() throws RecognitionException {
        NETGrammar.groups_block_return retval = new NETGrammar.groups_block_return();
        retval.start = input.LT(1);

        try {
            // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/NETGrammar.g:209:13: ( BEGIN GROUPS ( LB )+ ( group_line ( LB )+ )* END GROUPS ( LB )+ )
            // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/NETGrammar.g:210:1: BEGIN GROUPS ( LB )+ ( group_line ( LB )+ )* END GROUPS ( LB )+
            {
            match(input,BEGIN,FOLLOW_BEGIN_in_groups_block290); 
            match(input,GROUPS,FOLLOW_GROUPS_in_groups_block292); 
            // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/NETGrammar.g:210:14: ( LB )+
            int cnt7=0;
            loop7:
            do {
                int alt7=2;
                int LA7_0 = input.LA(1);

                if ( (LA7_0==LB) ) {
                    alt7=1;
                }


                switch (alt7) {
            	case 1 :
            	    // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/NETGrammar.g:210:14: LB
            	    {
            	    match(input,LB,FOLLOW_LB_in_groups_block294); 

            	    }
            	    break;

            	default :
            	    if ( cnt7 >= 1 ) break loop7;
                        EarlyExitException eee =
                            new EarlyExitException(7, input);
                        throw eee;
                }
                cnt7++;
            } while (true);

            // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/NETGrammar.g:211:3: ( group_line ( LB )+ )*
            loop9:
            do {
                int alt9=2;
                int LA9_0 = input.LA(1);

                if ( (LA9_0==INT) ) {
                    alt9=1;
                }


                switch (alt9) {
            	case 1 :
            	    // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/NETGrammar.g:211:4: group_line ( LB )+
            	    {
            	    pushFollow(FOLLOW_group_line_in_groups_block300);
            	    group_line();

            	    state._fsp--;

            	    // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/NETGrammar.g:211:15: ( LB )+
            	    int cnt8=0;
            	    loop8:
            	    do {
            	        int alt8=2;
            	        int LA8_0 = input.LA(1);

            	        if ( (LA8_0==LB) ) {
            	            alt8=1;
            	        }


            	        switch (alt8) {
            	    	case 1 :
            	    	    // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/NETGrammar.g:211:15: LB
            	    	    {
            	    	    match(input,LB,FOLLOW_LB_in_groups_block302); 

            	    	    }
            	    	    break;

            	    	default :
            	    	    if ( cnt8 >= 1 ) break loop8;
            	                EarlyExitException eee =
            	                    new EarlyExitException(8, input);
            	                throw eee;
            	        }
            	        cnt8++;
            	    } while (true);


            	    }
            	    break;

            	default :
            	    break loop9;
                }
            } while (true);

            match(input,END,FOLLOW_END_in_groups_block307); 
            match(input,GROUPS,FOLLOW_GROUPS_in_groups_block309); 
            // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/NETGrammar.g:212:12: ( LB )+
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
            	    // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/NETGrammar.g:212:12: LB
            	    {
            	    match(input,LB,FOLLOW_LB_in_groups_block311); 

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
    // $ANTLR end "groups_block"

    public static class group_line_return extends ParserRuleReturnScope {
        public StringTemplate st;
        public Object getTemplate() { return st; }
        public String toString() { return st==null?null:st.toString(); }
    };

    // $ANTLR start "group_line"
    // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/NETGrammar.g:217:1: group_line : INT STRING group_list[$STRING.text] ;
    public final NETGrammar.group_line_return group_line() throws RecognitionException {
        NETGrammar.group_line_return retval = new NETGrammar.group_line_return();
        retval.start = input.LT(1);

        Token STRING5=null;
        NETGrammar.group_list_return group_list6 = null;


        try {
            // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/NETGrammar.g:217:11: ( INT STRING group_list[$STRING.text] )
            // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/NETGrammar.g:218:1: INT STRING group_list[$STRING.text]
            {
            match(input,INT,FOLLOW_INT_in_group_line322); 
            STRING5=(Token)match(input,STRING,FOLLOW_STRING_in_group_line324); 
            pushFollow(FOLLOW_group_list_in_group_line326);
            group_list6=group_list((STRING5!=null?STRING5.getText():null));

            state._fsp--;

            groups.add((group_list6!=null?group_list6.st:null));

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
    // $ANTLR end "group_line"

    protected static class group_list_scope {
        int counter;
        List<StringTemplate> groupList;
    }
    protected Stack group_list_stack = new Stack();

    public static class group_list_return extends ParserRuleReturnScope {
        public StringTemplate st;
        public Object getTemplate() { return st; }
        public String toString() { return st==null?null:st.toString(); }
    };

    // $ANTLR start "group_list"
    // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/NETGrammar.g:221:1: group_list[String name] : g1= group_element[$name,$group_list::counter] ( COMMA g2= group_element[$name,$group_list::counter] )* -> groups_groups(groupList=$group_list::groupListname=$name);
    public final NETGrammar.group_list_return group_list(String name) throws RecognitionException {
        group_list_stack.push(new group_list_scope());
        NETGrammar.group_list_return retval = new NETGrammar.group_list_return();
        retval.start = input.LT(1);

        NETGrammar.group_element_return g1 = null;

        NETGrammar.group_element_return g2 = null;



          ((group_list_scope)group_list_stack.peek()).counter = 0;
          ((group_list_scope)group_list_stack.peek()).groupList = new ArrayList<StringTemplate>();

        try {
            // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/NETGrammar.g:229:2: (g1= group_element[$name,$group_list::counter] ( COMMA g2= group_element[$name,$group_list::counter] )* -> groups_groups(groupList=$group_list::groupListname=$name))
            // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/NETGrammar.g:230:3: g1= group_element[$name,$group_list::counter] ( COMMA g2= group_element[$name,$group_list::counter] )*
            {
            pushFollow(FOLLOW_group_element_in_group_list349);
            g1=group_element(name, ((group_list_scope)group_list_stack.peek()).counter);

            state._fsp--;

            ((group_list_scope)group_list_stack.peek()).groupList.add((g1!=null?g1.st:null));((group_list_scope)group_list_stack.peek()).counter++;
            // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/NETGrammar.g:231:3: ( COMMA g2= group_element[$name,$group_list::counter] )*
            loop11:
            do {
                int alt11=2;
                int LA11_0 = input.LA(1);

                if ( (LA11_0==COMMA) ) {
                    alt11=1;
                }


                switch (alt11) {
            	case 1 :
            	    // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/NETGrammar.g:231:4: COMMA g2= group_element[$name,$group_list::counter]
            	    {
            	    match(input,COMMA,FOLLOW_COMMA_in_group_list357); 
            	    pushFollow(FOLLOW_group_element_in_group_list361);
            	    g2=group_element(name, ((group_list_scope)group_list_stack.peek()).counter);

            	    state._fsp--;

            	    ((group_list_scope)group_list_stack.peek()).groupList.add((g2!=null?g2.st:null));((group_list_scope)group_list_stack.peek()).counter++;

            	    }
            	    break;

            	default :
            	    break loop11;
                }
            } while (true);



            // TEMPLATE REWRITE
            // 232:3: -> groups_groups(groupList=$group_list::groupListname=$name)
            {
                retval.st = templateLib.getInstanceOf("groups_groups",
              new STAttrMap().put("groupList", ((group_list_scope)group_list_stack.peek()).groupList).put("name", name));
            }


            }

            retval.stop = input.LT(-1);

        }
        catch (RecognitionException re) {
            reportError(re);
            recover(input,re);
        }
        finally {
            group_list_stack.pop();
        }
        return retval;
    }
    // $ANTLR end "group_list"

    public static class group_element_return extends ParserRuleReturnScope {
        public StringTemplate st;
        public Object getTemplate() { return st; }
        public String toString() { return st==null?null:st.toString(); }
    };

    // $ANTLR start "group_element"
    // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/NETGrammar.g:235:1: group_element[String name,int counter] : (i1= INT -> groups_block(id=$i1.text) | (i2= INT TIMES i3= INT ) -> groups_block(id=$i3.texttimes=$i2.text));
    public final NETGrammar.group_element_return group_element(String name, int counter) throws RecognitionException {
        NETGrammar.group_element_return retval = new NETGrammar.group_element_return();
        retval.start = input.LT(1);

        Token i1=null;
        Token i2=null;
        Token i3=null;

        try {
            // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/NETGrammar.g:235:39: (i1= INT -> groups_block(id=$i1.text) | (i2= INT TIMES i3= INT ) -> groups_block(id=$i3.texttimes=$i2.text))
            int alt12=2;
            int LA12_0 = input.LA(1);

            if ( (LA12_0==INT) ) {
                int LA12_1 = input.LA(2);

                if ( (LA12_1==TIMES) ) {
                    alt12=2;
                }
                else if ( (LA12_1==LB||LA12_1==COMMA) ) {
                    alt12=1;
                }
                else {
                    NoViableAltException nvae =
                        new NoViableAltException("", 12, 1, input);

                    throw nvae;
                }
            }
            else {
                NoViableAltException nvae =
                    new NoViableAltException("", 12, 0, input);

                throw nvae;
            }
            switch (alt12) {
                case 1 :
                    // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/NETGrammar.g:236:3: i1= INT
                    {
                    i1=(Token)match(input,INT,FOLLOW_INT_in_group_element398); 


                    // TEMPLATE REWRITE
                    // 236:10: -> groups_block(id=$i1.text)
                    {
                        retval.st = templateLib.getInstanceOf("groups_block",
                      new STAttrMap().put("id", (i1!=null?i1.getText():null)));
                    }


                    }
                    break;
                case 2 :
                    // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/NETGrammar.g:237:5: (i2= INT TIMES i3= INT )
                    {
                    // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/NETGrammar.g:237:5: (i2= INT TIMES i3= INT )
                    // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/NETGrammar.g:237:6: i2= INT TIMES i3= INT
                    {
                    i2=(Token)match(input,INT,FOLLOW_INT_in_group_element419); 
                    match(input,TIMES,FOLLOW_TIMES_in_group_element421); 
                    i3=(Token)match(input,INT,FOLLOW_INT_in_group_element425); 

                    }



                    // TEMPLATE REWRITE
                    // 237:27: -> groups_block(id=$i3.texttimes=$i2.text)
                    {
                        retval.st = templateLib.getInstanceOf("groups_block",
                      new STAttrMap().put("id", (i3!=null?i3.getText():null)).put("times", (i2!=null?i2.getText():null)));
                    }


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
    // $ANTLR end "group_element"

    protected static class prog_scope {
        List parameters;
        List seedSpecies;
        List observables;
        List reactionRules;
        List functions;
        List compartments;
        Stack elements;
        Map<String,List<StringTemplate>> surfaces;
        List<StringTemplate> moleculeDeclarations;
    }
    protected Stack prog_stack = new Stack();

    public static class prog_return extends ParserRuleReturnScope {
        public StringTemplate st;
        public Object getTemplate() { return st; }
        public String toString() { return st==null?null:st.toString(); }
    };

    // $ANTLR start "prog"
    // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/NETGrammar.g:240:1: prog : ( LB )* SUBSTANCEUNITS LPAREN DBQUOTES STRING DBQUOTES RPAREN SEMI ( LB )+ ( program_block )* EOF -> prog2(parameters=$prog::parametersmolecules=$prog::moleculeDeclarationsspecies=$prog::seedSpeciesreactions=reactionsobservables=groupsfunctions=$prog::functionscompartments=$prog::compartmentsboxes=$prog::surfacescompartmentsSurface=compartmentToSurface);
    public final NETGrammar.prog_return prog() throws RecognitionException {
        prog_stack.push(new prog_scope());
        NETGrammar.prog_return retval = new NETGrammar.prog_return();
        retval.start = input.LT(1);


          ((prog_scope)prog_stack.peek()).parameters = new ArrayList();
         
          ((prog_scope)prog_stack.peek()).seedSpecies = new ArrayList();
          ((prog_scope)prog_stack.peek()).reactionRules = new ArrayList();
          ((prog_scope)prog_stack.peek()).observables = new ArrayList();
          ((prog_scope)prog_stack.peek()).functions = new ArrayList();
          ((prog_scope)prog_stack.peek()).compartments = new ArrayList();
         // memory = new HashMap<String,Register>();
          ((prog_scope)prog_stack.peek()).elements = new Stack();
          ((prog_scope)prog_stack.peek()).surfaces = new HashMap<String,List<StringTemplate>>();
          ((prog_scope)prog_stack.peek()).moleculeDeclarations = new ArrayList<StringTemplate>();
          paraphrases.push("in model");

        try {
            // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/NETGrammar.g:274:1: ( ( LB )* SUBSTANCEUNITS LPAREN DBQUOTES STRING DBQUOTES RPAREN SEMI ( LB )+ ( program_block )* EOF -> prog2(parameters=$prog::parametersmolecules=$prog::moleculeDeclarationsspecies=$prog::seedSpeciesreactions=reactionsobservables=groupsfunctions=$prog::functionscompartments=$prog::compartmentsboxes=$prog::surfacescompartmentsSurface=compartmentToSurface))
            // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/NETGrammar.g:275:1: ( LB )* SUBSTANCEUNITS LPAREN DBQUOTES STRING DBQUOTES RPAREN SEMI ( LB )+ ( program_block )* EOF
            {
            // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/NETGrammar.g:275:1: ( LB )*
            loop13:
            do {
                int alt13=2;
                int LA13_0 = input.LA(1);

                if ( (LA13_0==LB) ) {
                    alt13=1;
                }


                switch (alt13) {
            	case 1 :
            	    // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/NETGrammar.g:275:1: LB
            	    {
            	    match(input,LB,FOLLOW_LB_in_prog469); 

            	    }
            	    break;

            	default :
            	    break loop13;
                }
            } while (true);

            match(input,SUBSTANCEUNITS,FOLLOW_SUBSTANCEUNITS_in_prog472); 
            match(input,LPAREN,FOLLOW_LPAREN_in_prog474); 
            match(input,DBQUOTES,FOLLOW_DBQUOTES_in_prog476); 
            match(input,STRING,FOLLOW_STRING_in_prog478); 
            match(input,DBQUOTES,FOLLOW_DBQUOTES_in_prog480); 
            match(input,RPAREN,FOLLOW_RPAREN_in_prog482); 
            match(input,SEMI,FOLLOW_SEMI_in_prog484); 
            // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/NETGrammar.g:276:60: ( LB )+
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
            	    // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/NETGrammar.g:276:60: LB
            	    {
            	    match(input,LB,FOLLOW_LB_in_prog486); 

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

            // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/NETGrammar.g:277:1: ( program_block )*
            loop15:
            do {
                int alt15=2;
                int LA15_0 = input.LA(1);

                if ( (LA15_0==BEGIN) ) {
                    alt15=1;
                }


                switch (alt15) {
            	case 1 :
            	    // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/NETGrammar.g:277:2: program_block
            	    {
            	    pushFollow(FOLLOW_program_block_in_prog490);
            	    program_block();

            	    state._fsp--;


            	    }
            	    break;

            	default :
            	    break loop15;
                }
            } while (true);



            //generating the code for surfaces and volumes
              for(String element: compartmentSurfaces.keySet()){
                if(!compartmentList.isSurface(element))
                  continue;
                StringTemplate temp;
                //temp = template("create_surface_region");
                
                //this is the equivalent of manually calling a template from a stg file
                //where the options aray contains the parameters, and the templateLib calls the method
                //This is necessary because of the stark differences in mdl syntax between a surface and a volume
                STAttrMap options = new STAttrMap();
                options.put("name", element);
                options.put("molecules",compartmentSurfaces.get(element));
                temp = templateLib.getInstanceOf("create_surface_region",options);
                if(!((prog_scope)prog_stack.peek()).surfaces.containsKey(compartmentList.getParentCompartment(element))){
                  ((prog_scope)prog_stack.peek()).surfaces.put(compartmentList.getParentCompartment(element),new ArrayList<StringTemplate>());
                }
                ((prog_scope)prog_stack.peek()).surfaces.get(compartmentList.getParentCompartment(element)).add(temp);
               // System.out.println(temp);
              }
              for(String tag: speciesMap.keySet()){
                String compartment = getCompartment("S" + speciesMap.get(tag));
                String dimension = "3";
                if(compartmentToSurface)
                  dimension = compartmentList.getDimension(compartment);;
                String difussion;
                STAttrMap options = new STAttrMap();
                options.put("index",speciesMap.get(tag));
                options.put("name",tag);
                options.put("dimension",dimension);
                if(dimension.equals("2") && compartmentToSurface){
                  difussion = "1e-6";
                }
                else{
                  difussion="1e-6";
                }
                options.put("difussion",difussion);
                StringTemplate temp = templateLib.getInstanceOf("define_molecules",options);
                ((prog_scope)prog_stack.peek()).moleculeDeclarations.add(temp);
                //System.out.println("hello " + compartment + compartmentList.getDimension(compartment));
              }

            match(input,EOF,FOLLOW_EOF_in_prog499); 


            // TEMPLATE REWRITE
            // 326:2: -> prog2(parameters=$prog::parametersmolecules=$prog::moleculeDeclarationsspecies=$prog::seedSpeciesreactions=reactionsobservables=groupsfunctions=$prog::functionscompartments=$prog::compartmentsboxes=$prog::surfacescompartmentsSurface=compartmentToSurface)
            {
                retval.st = templateLib.getInstanceOf("prog2",
              new STAttrMap().put("parameters", ((prog_scope)prog_stack.peek()).parameters).put("molecules", ((prog_scope)prog_stack.peek()).moleculeDeclarations).put("species", ((prog_scope)prog_stack.peek()).seedSpecies).put("reactions", reactions).put("observables", groups).put("functions", ((prog_scope)prog_stack.peek()).functions).put("compartments", ((prog_scope)prog_stack.peek()).compartments).put("boxes", ((prog_scope)prog_stack.peek()).surfaces).put("compartmentsSurface", compartmentToSurface));
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
    // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/NETGrammar.g:330:1: version_def : VERSION LPAREN DBQUOTES VERSION_NUMBER DBQUOTES RPAREN SEMI ;
    public final NETGrammar.version_def_return version_def() throws RecognitionException {
        NETGrammar.version_def_return retval = new NETGrammar.version_def_return();
        retval.start = input.LT(1);

        try {
            // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/NETGrammar.g:330:12: ( VERSION LPAREN DBQUOTES VERSION_NUMBER DBQUOTES RPAREN SEMI )
            // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/NETGrammar.g:330:14: VERSION LPAREN DBQUOTES VERSION_NUMBER DBQUOTES RPAREN SEMI
            {
            match(input,VERSION,FOLLOW_VERSION_in_version_def609); 
            match(input,LPAREN,FOLLOW_LPAREN_in_version_def611); 
            match(input,DBQUOTES,FOLLOW_DBQUOTES_in_version_def613); 
            match(input,VERSION_NUMBER,FOLLOW_VERSION_NUMBER_in_version_def615); 
            match(input,DBQUOTES,FOLLOW_DBQUOTES_in_version_def617); 
            match(input,RPAREN,FOLLOW_RPAREN_in_version_def619); 
            match(input,SEMI,FOLLOW_SEMI_in_version_def621); 

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
    // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/NETGrammar.g:336:1: program_block : ( parameters_block[memory,$prog::parameters] | molecule_types_block | seed_species_block[$prog::seedSpecies] | observables_block[$prog::observables] | reaction_rules_block[$prog::reactionRules] | functions_block | compartments_block | reactions_block[reactions] | groups_block );
    public final NETGrammar.program_block_return program_block() throws RecognitionException {
        NETGrammar.program_block_return retval = new NETGrammar.program_block_return();
        retval.start = input.LT(1);

        try {
            // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/NETGrammar.g:337:1: ( parameters_block[memory,$prog::parameters] | molecule_types_block | seed_species_block[$prog::seedSpecies] | observables_block[$prog::observables] | reaction_rules_block[$prog::reactionRules] | functions_block | compartments_block | reactions_block[reactions] | groups_block )
            int alt16=9;
            alt16 = dfa16.predict(input);
            switch (alt16) {
                case 1 :
                    // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/NETGrammar.g:337:3: parameters_block[memory,$prog::parameters]
                    {
                    pushFollow(FOLLOW_parameters_block_in_program_block633);
                    parameters_block(memory, ((prog_scope)prog_stack.peek()).parameters);

                    state._fsp--;


                    }
                    break;
                case 2 :
                    // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/NETGrammar.g:338:3: molecule_types_block
                    {
                    pushFollow(FOLLOW_molecule_types_block_in_program_block638);
                    molecule_types_block();

                    state._fsp--;


                    }
                    break;
                case 3 :
                    // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/NETGrammar.g:339:3: seed_species_block[$prog::seedSpecies]
                    {
                    pushFollow(FOLLOW_seed_species_block_in_program_block642);
                    seed_species_block(((prog_scope)prog_stack.peek()).seedSpecies);

                    state._fsp--;


                    }
                    break;
                case 4 :
                    // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/NETGrammar.g:340:3: observables_block[$prog::observables]
                    {
                    pushFollow(FOLLOW_observables_block_in_program_block647);
                    observables_block(((prog_scope)prog_stack.peek()).observables);

                    state._fsp--;


                    }
                    break;
                case 5 :
                    // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/NETGrammar.g:341:3: reaction_rules_block[$prog::reactionRules]
                    {
                    pushFollow(FOLLOW_reaction_rules_block_in_program_block652);
                    reaction_rules_block(((prog_scope)prog_stack.peek()).reactionRules);

                    state._fsp--;


                    }
                    break;
                case 6 :
                    // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/NETGrammar.g:342:3: functions_block
                    {
                    pushFollow(FOLLOW_functions_block_in_program_block657);
                    functions_block();

                    state._fsp--;


                    }
                    break;
                case 7 :
                    // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/NETGrammar.g:343:3: compartments_block
                    {
                    pushFollow(FOLLOW_compartments_block_in_program_block661);
                    compartments_block();

                    state._fsp--;


                    }
                    break;
                case 8 :
                    // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/NETGrammar.g:344:3: reactions_block[reactions]
                    {
                    pushFollow(FOLLOW_reactions_block_in_program_block665);
                    reactions_block(reactions);

                    state._fsp--;


                    }
                    break;
                case 9 :
                    // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/NETGrammar.g:345:3: groups_block
                    {
                    pushFollow(FOLLOW_groups_block_in_program_block670);
                    groups_block();

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
    // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/NETGrammar.g:350:1: functions_block : BEGIN FUNCTIONS ( LB )+ ( function_def ( LB )+ )* END FUNCTIONS ( LB )+ ;
    public final NETGrammar.functions_block_return functions_block() throws RecognitionException {
        NETGrammar.functions_block_return retval = new NETGrammar.functions_block_return();
        retval.start = input.LT(1);

        NETGrammar.function_def_return function_def7 = null;



          paraphrases.push("in function block");

        try {
            // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/NETGrammar.g:357:1: ( BEGIN FUNCTIONS ( LB )+ ( function_def ( LB )+ )* END FUNCTIONS ( LB )+ )
            // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/NETGrammar.g:358:7: BEGIN FUNCTIONS ( LB )+ ( function_def ( LB )+ )* END FUNCTIONS ( LB )+
            {
            match(input,BEGIN,FOLLOW_BEGIN_in_functions_block695); 
            match(input,FUNCTIONS,FOLLOW_FUNCTIONS_in_functions_block697); 
            // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/NETGrammar.g:358:23: ( LB )+
            int cnt17=0;
            loop17:
            do {
                int alt17=2;
                int LA17_0 = input.LA(1);

                if ( (LA17_0==LB) ) {
                    alt17=1;
                }


                switch (alt17) {
            	case 1 :
            	    // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/NETGrammar.g:358:23: LB
            	    {
            	    match(input,LB,FOLLOW_LB_in_functions_block699); 

            	    }
            	    break;

            	default :
            	    if ( cnt17 >= 1 ) break loop17;
                        EarlyExitException eee =
                            new EarlyExitException(17, input);
                        throw eee;
                }
                cnt17++;
            } while (true);

            // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/NETGrammar.g:359:8: ( function_def ( LB )+ )*
            loop19:
            do {
                int alt19=2;
                int LA19_0 = input.LA(1);

                if ( (LA19_0==STRING) ) {
                    alt19=1;
                }


                switch (alt19) {
            	case 1 :
            	    // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/NETGrammar.g:359:9: function_def ( LB )+
            	    {
            	    pushFollow(FOLLOW_function_def_in_functions_block710);
            	    function_def7=function_def();

            	    state._fsp--;

            	    // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/NETGrammar.g:359:22: ( LB )+
            	    int cnt18=0;
            	    loop18:
            	    do {
            	        int alt18=2;
            	        int LA18_0 = input.LA(1);

            	        if ( (LA18_0==LB) ) {
            	            alt18=1;
            	        }


            	        switch (alt18) {
            	    	case 1 :
            	    	    // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/NETGrammar.g:359:22: LB
            	    	    {
            	    	    match(input,LB,FOLLOW_LB_in_functions_block712); 

            	    	    }
            	    	    break;

            	    	default :
            	    	    if ( cnt18 >= 1 ) break loop18;
            	                EarlyExitException eee =
            	                    new EarlyExitException(18, input);
            	                throw eee;
            	        }
            	        cnt18++;
            	    } while (true);

            	    ((prog_scope)prog_stack.peek()).functions.add((function_def7!=null?function_def7.st:null));

            	    }
            	    break;

            	default :
            	    break loop19;
                }
            } while (true);

            match(input,END,FOLLOW_END_in_functions_block729); 
            match(input,FUNCTIONS,FOLLOW_FUNCTIONS_in_functions_block731); 
            // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/NETGrammar.g:360:21: ( LB )+
            int cnt20=0;
            loop20:
            do {
                int alt20=2;
                int LA20_0 = input.LA(1);

                if ( (LA20_0==LB) ) {
                    alt20=1;
                }


                switch (alt20) {
            	case 1 :
            	    // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/NETGrammar.g:360:21: LB
            	    {
            	    match(input,LB,FOLLOW_LB_in_functions_block733); 

            	    }
            	    break;

            	default :
            	    if ( cnt20 >= 1 ) break loop20;
                        EarlyExitException eee =
                            new EarlyExitException(20, input);
                        throw eee;
                }
                cnt20++;
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

    public static class function_def_return extends ParserRuleReturnScope {
        public StringTemplate st;
        public Object getTemplate() { return st; }
        public String toString() { return st==null?null:st.toString(); }
    };

    // $ANTLR start "function_def"
    // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/NETGrammar.g:363:1: function_def : s1= STRING LPAREN ( STRING )? RPAREN ( BECOMES )? expression[memory] -> functions_block(id=$s1.textreferencesName=$expression.reference.keySet()referencesType=Register.getTypes($expression.reference)expression=$expression.text);
    public final NETGrammar.function_def_return function_def() throws RecognitionException {
        NETGrammar.function_def_return retval = new NETGrammar.function_def_return();
        retval.start = input.LT(1);

        Token s1=null;
        NETGrammar_BNGGrammar_Expression.expression_return expression8 = null;


        try {
            // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/NETGrammar.g:363:13: (s1= STRING LPAREN ( STRING )? RPAREN ( BECOMES )? expression[memory] -> functions_block(id=$s1.textreferencesName=$expression.reference.keySet()referencesType=Register.getTypes($expression.reference)expression=$expression.text))
            // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/NETGrammar.g:364:5: s1= STRING LPAREN ( STRING )? RPAREN ( BECOMES )? expression[memory]
            {
            s1=(Token)match(input,STRING,FOLLOW_STRING_in_function_def748); 
            match(input,LPAREN,FOLLOW_LPAREN_in_function_def750); 
            // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/NETGrammar.g:364:22: ( STRING )?
            int alt21=2;
            int LA21_0 = input.LA(1);

            if ( (LA21_0==STRING) ) {
                alt21=1;
            }
            switch (alt21) {
                case 1 :
                    // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/NETGrammar.g:364:23: STRING
                    {
                    match(input,STRING,FOLLOW_STRING_in_function_def753); 

                    }
                    break;

            }

            match(input,RPAREN,FOLLOW_RPAREN_in_function_def757); 
            // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/NETGrammar.g:364:39: ( BECOMES )?
            int alt22=2;
            int LA22_0 = input.LA(1);

            if ( (LA22_0==BECOMES) ) {
                alt22=1;
            }
            switch (alt22) {
                case 1 :
                    // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/NETGrammar.g:364:40: BECOMES
                    {
                    match(input,BECOMES,FOLLOW_BECOMES_in_function_def760); 

                    }
                    break;

            }

            pushFollow(FOLLOW_expression_in_function_def765);
            expression8=expression(memory);

            state._fsp--;



            // TEMPLATE REWRITE
            // 364:70: -> functions_block(id=$s1.textreferencesName=$expression.reference.keySet()referencesType=Register.getTypes($expression.reference)expression=$expression.text)
            {
                retval.st = templateLib.getInstanceOf("functions_block",
              new STAttrMap().put("id", (s1!=null?s1.getText():null)).put("referencesName", (expression8!=null?expression8.reference:null).keySet()).put("referencesType", Register.getTypes((expression8!=null?expression8.reference:null))).put("expression", (expression8!=null?input.toString(expression8.start,expression8.stop):null)));
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
    // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/NETGrammar.g:368:1: compartments_block : BEGIN COMPARTMENTS ( LB )+ ( compartment ( LB )+ )* END COMPARTMENTS ( LB )+ ;
    public final NETGrammar.compartments_block_return compartments_block() throws RecognitionException {
        NETGrammar.compartments_block_return retval = new NETGrammar.compartments_block_return();
        retval.start = input.LT(1);

        NETGrammar.compartment_return compartment9 = null;


        try {
            // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/NETGrammar.g:368:19: ( BEGIN COMPARTMENTS ( LB )+ ( compartment ( LB )+ )* END COMPARTMENTS ( LB )+ )
            // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/NETGrammar.g:370:3: BEGIN COMPARTMENTS ( LB )+ ( compartment ( LB )+ )* END COMPARTMENTS ( LB )+
            {
            match(input,BEGIN,FOLLOW_BEGIN_in_compartments_block828); 
            match(input,COMPARTMENTS,FOLLOW_COMPARTMENTS_in_compartments_block830); 
            // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/NETGrammar.g:370:22: ( LB )+
            int cnt23=0;
            loop23:
            do {
                int alt23=2;
                int LA23_0 = input.LA(1);

                if ( (LA23_0==LB) ) {
                    alt23=1;
                }


                switch (alt23) {
            	case 1 :
            	    // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/NETGrammar.g:370:22: LB
            	    {
            	    match(input,LB,FOLLOW_LB_in_compartments_block832); 

            	    }
            	    break;

            	default :
            	    if ( cnt23 >= 1 ) break loop23;
                        EarlyExitException eee =
                            new EarlyExitException(23, input);
                        throw eee;
                }
                cnt23++;
            } while (true);

            // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/NETGrammar.g:371:7: ( compartment ( LB )+ )*
            loop25:
            do {
                int alt25=2;
                int LA25_0 = input.LA(1);

                if ( (LA25_0==STRING) ) {
                    alt25=1;
                }


                switch (alt25) {
            	case 1 :
            	    // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/NETGrammar.g:371:8: compartment ( LB )+
            	    {
            	    pushFollow(FOLLOW_compartment_in_compartments_block842);
            	    compartment9=compartment();

            	    state._fsp--;

            	    // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/NETGrammar.g:371:20: ( LB )+
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
            	    	    // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/NETGrammar.g:371:20: LB
            	    	    {
            	    	    match(input,LB,FOLLOW_LB_in_compartments_block844); 

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

            	    ((prog_scope)prog_stack.peek()).compartments.add((compartment9!=null?compartment9.st:null));

            	    }
            	    break;

            	default :
            	    break loop25;
                }
            } while (true);

            match(input,END,FOLLOW_END_in_compartments_block852); 
            match(input,COMPARTMENTS,FOLLOW_COMPARTMENTS_in_compartments_block854); 
            // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/NETGrammar.g:372:20: ( LB )+
            int cnt26=0;
            loop26:
            do {
                int alt26=2;
                int LA26_0 = input.LA(1);

                if ( (LA26_0==LB) ) {
                    alt26=1;
                }


                switch (alt26) {
            	case 1 :
            	    // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/NETGrammar.g:372:20: LB
            	    {
            	    match(input,LB,FOLLOW_LB_in_compartments_block856); 

            	    }
            	    break;

            	default :
            	    if ( cnt26 >= 1 ) break loop26;
                        EarlyExitException eee =
                            new EarlyExitException(26, input);
                        throw eee;
                }
                cnt26++;
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
    // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/NETGrammar.g:374:1: compartment : s1= STRING INT s3= expression[memory] (s2= STRING )? -> compartments_block(id=$s1.textdimensions=$INT.textsize=$s3.valueoutside=$s2.text);
    public final NETGrammar.compartment_return compartment() throws RecognitionException {
        NETGrammar.compartment_return retval = new NETGrammar.compartment_return();
        retval.start = input.LT(1);

        Token s1=null;
        Token s2=null;
        Token INT10=null;
        NETGrammar_BNGGrammar_Expression.expression_return s3 = null;


        try {
            // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/NETGrammar.g:374:12: (s1= STRING INT s3= expression[memory] (s2= STRING )? -> compartments_block(id=$s1.textdimensions=$INT.textsize=$s3.valueoutside=$s2.text))
            // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/NETGrammar.g:375:4: s1= STRING INT s3= expression[memory] (s2= STRING )?
            {
            s1=(Token)match(input,STRING,FOLLOW_STRING_in_compartment871); 
            INT10=(Token)match(input,INT,FOLLOW_INT_in_compartment873); 
            pushFollow(FOLLOW_expression_in_compartment877);
            s3=expression(memory);

            state._fsp--;


                  compartmentList.addCompartment((s1!=null?s1.getText():null),(INT10!=null?INT10.getText():null),((s3!=null?s3.value:null)).toString());
               
            // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/NETGrammar.g:379:4: (s2= STRING )?
            int alt27=2;
            int LA27_0 = input.LA(1);

            if ( (LA27_0==STRING) ) {
                alt27=1;
            }
            switch (alt27) {
                case 1 :
                    // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/NETGrammar.g:379:5: s2= STRING
                    {
                    s2=(Token)match(input,STRING,FOLLOW_STRING_in_compartment892); 

                          compartmentList.addContainer((s1!=null?s1.getText():null),(s2!=null?s2.getText():null));
                       

                    }
                    break;

            }



            // TEMPLATE REWRITE
            // 384:4: -> compartments_block(id=$s1.textdimensions=$INT.textsize=$s3.valueoutside=$s2.text)
            {
                retval.st = templateLib.getInstanceOf("compartments_block",
              new STAttrMap().put("id", (s1!=null?s1.getText():null)).put("dimensions", (INT10!=null?INT10.getText():null)).put("size", (s3!=null?s3.value:null)).put("outside", (s2!=null?s2.getText():null)));
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
    // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/NETGrammar.g:387:1: label returns [String label] : MOD STRING ;
    public final NETGrammar.label_return label() throws RecognitionException {
        NETGrammar.label_return retval = new NETGrammar.label_return();
        retval.start = input.LT(1);

        Token STRING11=null;

        try {
            // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/NETGrammar.g:387:29: ( MOD STRING )
            // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/NETGrammar.g:388:3: MOD STRING
            {
            match(input,MOD,FOLLOW_MOD_in_label945); 
            STRING11=(Token)match(input,STRING,FOLLOW_STRING_in_label947); 
            retval.label = (STRING11!=null?STRING11.getText():null);

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
    // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/NETGrammar.g:390:1: dereference returns [String label] : AMPERSAND STRING ;
    public final NETGrammar.dereference_return dereference() throws RecognitionException {
        NETGrammar.dereference_return retval = new NETGrammar.dereference_return();
        retval.start = input.LT(1);

        Token STRING12=null;

        try {
            // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/NETGrammar.g:390:35: ( AMPERSAND STRING )
            // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/NETGrammar.g:391:3: AMPERSAND STRING
            {
            match(input,AMPERSAND,FOLLOW_AMPERSAND_in_dereference963); 
            STRING12=(Token)match(input,STRING,FOLLOW_STRING_in_dereference965); 
            retval.label = (STRING12!=null?STRING12.getText():null);

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

    public static class reactions_block_return extends ParserRuleReturnScope {
        public StringTemplate st;
        public Object getTemplate() { return st; }
        public String toString() { return st==null?null:st.toString(); }
    };

    // $ANTLR start "reactions_block"
    // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/NETGrammar.g:395:2: reactions_block[List<StringTemplate> templates] : BEGIN REACTIONS ( LB )+ ( INT reaction ( LB )+ )* END REACTIONS ( LB )+ ;
    public final NETGrammar.reactions_block_return reactions_block(List<StringTemplate> templates) throws RecognitionException {
        NETGrammar.reactions_block_return retval = new NETGrammar.reactions_block_return();
        retval.start = input.LT(1);

        NETGrammar.reaction_return reaction13 = null;


        try {
            // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/NETGrammar.g:395:49: ( BEGIN REACTIONS ( LB )+ ( INT reaction ( LB )+ )* END REACTIONS ( LB )+ )
            // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/NETGrammar.g:396:1: BEGIN REACTIONS ( LB )+ ( INT reaction ( LB )+ )* END REACTIONS ( LB )+
            {
            match(input,BEGIN,FOLLOW_BEGIN_in_reactions_block983); 
            match(input,REACTIONS,FOLLOW_REACTIONS_in_reactions_block985); 
            // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/NETGrammar.g:396:17: ( LB )+
            int cnt28=0;
            loop28:
            do {
                int alt28=2;
                int LA28_0 = input.LA(1);

                if ( (LA28_0==LB) ) {
                    alt28=1;
                }


                switch (alt28) {
            	case 1 :
            	    // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/NETGrammar.g:396:17: LB
            	    {
            	    match(input,LB,FOLLOW_LB_in_reactions_block987); 

            	    }
            	    break;

            	default :
            	    if ( cnt28 >= 1 ) break loop28;
                        EarlyExitException eee =
                            new EarlyExitException(28, input);
                        throw eee;
                }
                cnt28++;
            } while (true);

            // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/NETGrammar.g:397:3: ( INT reaction ( LB )+ )*
            loop30:
            do {
                int alt30=2;
                int LA30_0 = input.LA(1);

                if ( (LA30_0==INT) ) {
                    alt30=1;
                }


                switch (alt30) {
            	case 1 :
            	    // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/NETGrammar.g:397:4: INT reaction ( LB )+
            	    {
            	    match(input,INT,FOLLOW_INT_in_reactions_block993); 
            	    pushFollow(FOLLOW_reaction_in_reactions_block995);
            	    reaction13=reaction();

            	    state._fsp--;

            	    // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/NETGrammar.g:397:17: ( LB )+
            	    int cnt29=0;
            	    loop29:
            	    do {
            	        int alt29=2;
            	        int LA29_0 = input.LA(1);

            	        if ( (LA29_0==LB) ) {
            	            alt29=1;
            	        }


            	        switch (alt29) {
            	    	case 1 :
            	    	    // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/NETGrammar.g:397:17: LB
            	    	    {
            	    	    match(input,LB,FOLLOW_LB_in_reactions_block997); 

            	    	    }
            	    	    break;

            	    	default :
            	    	    if ( cnt29 >= 1 ) break loop29;
            	                EarlyExitException eee =
            	                    new EarlyExitException(29, input);
            	                throw eee;
            	        }
            	        cnt29++;
            	    } while (true);


            	        templates.add((reaction13!=null?reaction13.st:null));
            	      

            	    }
            	    break;

            	default :
            	    break loop30;
                }
            } while (true);

            match(input,END,FOLLOW_END_in_reactions_block1006); 
            match(input,REACTIONS,FOLLOW_REACTIONS_in_reactions_block1008); 
            // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/NETGrammar.g:401:15: ( LB )+
            int cnt31=0;
            loop31:
            do {
                int alt31=2;
                int LA31_0 = input.LA(1);

                if ( (LA31_0==LB) ) {
                    alt31=1;
                }


                switch (alt31) {
            	case 1 :
            	    // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/NETGrammar.g:401:15: LB
            	    {
            	    match(input,LB,FOLLOW_LB_in_reactions_block1010); 

            	    }
            	    break;

            	default :
            	    if ( cnt31 >= 1 ) break loop31;
                        EarlyExitException eee =
                            new EarlyExitException(31, input);
                        throw eee;
                }
                cnt31++;
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
    // $ANTLR end "reactions_block"

    protected static class reaction_scope {
        List<String> reactant;
        List<String> product;
        double volume;
    }
    protected Stack reaction_stack = new Stack();

    public static class reaction_return extends ParserRuleReturnScope {
        public StringTemplate st;
        public Object getTemplate() { return st; }
        public String toString() { return st==null?null:st.toString(); }
    };

    // $ANTLR start "reaction"
    // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/NETGrammar.g:404:1: reaction : r1= reactant[$reaction::reactant] r2= reactant[$reaction::product] expression[memory] -> reactions(reactant=$reaction::reactantproduct=$reaction::productrate=$expression.value*$reaction::volumesurfaceCompartment=$r1.surfaceCompartment||$r2.surfaceCompartmentreactantOrientation=$r1.orientationproductOrientation=$r2.orientationfactor=\"\");
    public final NETGrammar.reaction_return reaction() throws RecognitionException {
        reaction_stack.push(new reaction_scope());
        NETGrammar.reaction_return retval = new NETGrammar.reaction_return();
        retval.start = input.LT(1);

        NETGrammar.reactant_return r1 = null;

        NETGrammar.reactant_return r2 = null;

        NETGrammar_BNGGrammar_Expression.expression_return expression14 = null;



          ((reaction_scope)reaction_stack.peek()).reactant = new ArrayList<String>();
          ((reaction_scope)reaction_stack.peek()).product = new ArrayList<String>();
          ((reaction_scope)reaction_stack.peek()).volume = 0.0;

        try {
            // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/NETGrammar.g:415:1: (r1= reactant[$reaction::reactant] r2= reactant[$reaction::product] expression[memory] -> reactions(reactant=$reaction::reactantproduct=$reaction::productrate=$expression.value*$reaction::volumesurfaceCompartment=$r1.surfaceCompartment||$r2.surfaceCompartmentreactantOrientation=$r1.orientationproductOrientation=$r2.orientationfactor=\"\"))
            // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/NETGrammar.g:416:2: r1= reactant[$reaction::reactant] r2= reactant[$reaction::product] expression[memory]
            {
            pushFollow(FOLLOW_reactant_in_reaction1030);
            r1=reactant(((reaction_scope)reaction_stack.peek()).reactant);

            state._fsp--;

            pushFollow(FOLLOW_reactant_in_reaction1035);
            r2=reactant(((reaction_scope)reaction_stack.peek()).product);

            state._fsp--;

            pushFollow(FOLLOW_expression_in_reaction1038);
            expression14=expression(memory);

            state._fsp--;


              if((r1!=null?r1.size:0)>1){
            	  if(!compartmentToSurface){
            	    ((reaction_scope)reaction_stack.peek()).volume =new Double(compartmentList.getECMVolume()) *avogadro_constant;
            	    //((reaction_scope)reaction_stack.peek()).volume = ecm_volume_value *avogadro_constant;
            	  } 
            	  else if((r1!=null?r1.volume:null).equals(ecm_volume) || (r2!=null?r2.volume:null).equals(ecm_volume)){
            	  //System.out.println(compartmentList.getECMVolume());
            	  ((reaction_scope)reaction_stack.peek()).volume = new Double(compartmentList.getECMVolume()) *avogadro_constant/1e4;
            	  //  ((reaction_scope)reaction_stack.peek()).volume = ecm_volume_value *avogadro_constant;
            	  }
            	  else if((r1!=null?r1.volume:null).equals(cyto_volume) || (r2!=null?r2.volume:null).equals(cyto_volume)){
            	    ((reaction_scope)reaction_stack.peek()).volume =new Double(compartmentList.getCytoplasmicVolume()) *avogadro_constant/1e4;
            	    //((reaction_scope)reaction_stack.peek()).volume = cyto_volume_value *avogadro_constant;
            	  }
            	  else{
            	  //((reaction_scope)reaction_stack.peek()).volume = 1*0.01/new Double(compartmentList.getMembraneVolume())*1e15;
            	  
            	  //fixme: this should be automatically calculated
            	    ((reaction_scope)reaction_stack.peek()).volume = 4*3.141592*5*5;
            	  }
              }
              else{
                ((reaction_scope)reaction_stack.peek()).volume = 1;
              }
             
             


            // TEMPLATE REWRITE
            // 444:2: -> reactions(reactant=$reaction::reactantproduct=$reaction::productrate=$expression.value*$reaction::volumesurfaceCompartment=$r1.surfaceCompartment||$r2.surfaceCompartmentreactantOrientation=$r1.orientationproductOrientation=$r2.orientationfactor=\"\")
            {
                retval.st = templateLib.getInstanceOf("reactions",
              new STAttrMap().put("reactant", ((reaction_scope)reaction_stack.peek()).reactant).put("product", ((reaction_scope)reaction_stack.peek()).product).put("rate", (expression14!=null?expression14.value:null)*((reaction_scope)reaction_stack.peek()).volume).put("surfaceCompartment", (r1!=null?r1.surfaceCompartment:false)||(r2!=null?r2.surfaceCompartment:false)).put("reactantOrientation", (r1!=null?r1.orientation:null)).put("productOrientation", (r2!=null?r2.orientation:null)).put("factor", ""));
            }


            }

            retval.stop = input.LT(-1);

        }
        catch (RecognitionException re) {
            reportError(re);
            recover(input,re);
        }
        finally {
            reaction_stack.pop();
        }
        return retval;
    }
    // $ANTLR end "reaction"

    public static class reactant_return extends ParserRuleReturnScope {
        public boolean surfaceCompartment=false;
        public String volume="1";
        public List<String> orientation;
        public int size=0;
        public StringTemplate st;
        public Object getTemplate() { return st; }
        public String toString() { return st==null?null:st.toString(); }
    };

    // $ANTLR start "reactant"
    // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/NETGrammar.g:450:1: reactant[List<String> elements] returns [boolean surfaceCompartment=false, String volume=\"1\", List<String> orientation, int size=0] : i1= INT ( COMMA i2= INT )* ;
    public final NETGrammar.reactant_return reactant(List<String> elements) throws RecognitionException {
        NETGrammar.reactant_return retval = new NETGrammar.reactant_return();
        retval.start = input.LT(1);

        Token i1=null;
        Token i2=null;


          retval.orientation = new ArrayList<String>();

        try {
            // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/NETGrammar.g:457:1: (i1= INT ( COMMA i2= INT )* )
            // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/NETGrammar.g:459:3: i1= INT ( COMMA i2= INT )*
            {
            i1=(Token)match(input,INT,FOLLOW_INT_in_reactant1111); 

                elements.add((i1!=null?i1.getText():null));
                retval.size++;
              
            // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/NETGrammar.g:465:3: ( COMMA i2= INT )*
            loop32:
            do {
                int alt32=2;
                int LA32_0 = input.LA(1);

                if ( (LA32_0==COMMA) ) {
                    alt32=1;
                }


                switch (alt32) {
            	case 1 :
            	    // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/NETGrammar.g:465:4: COMMA i2= INT
            	    {
            	    match(input,COMMA,FOLLOW_COMMA_in_reactant1124); 
            	    i2=(Token)match(input,INT,FOLLOW_INT_in_reactant1128); 

            	        elements.add((i2!=null?i2.getText():null));
            	        retval.size++;
            	      

            	    }
            	    break;

            	default :
            	    break loop32;
                }
            } while (true);


                if(compartmentToSurface){
            	    for(String molecules: elements){
            	      if(compartmentList.isSurface(getCompartment("S" + molecules))){
            	        retval.surfaceCompartment =  true;
            	      }
            	      
            	      //here we are assiginging a preset volume to a compartment depending if its the cytoplasm or the ecm
            	      else if(compartmentList.isOuterCompartment(getCompartment("S" + molecules))){
            	        retval.volume = ecm_volume;
            	      }
            	      else{
            	        retval.volume = cyto_volume;
            	      }
            	    }
                }
                
                

                if(compartmentToSurface){
            	    for(String molecules: elements){
            	        if(compartmentList.isOuterCompartment(getCompartment("S" + molecules)) || compartmentList.isSurface(getCompartment("S" + molecules))){
            	          retval.orientation.add("'");
            	        }
            	        else{
            	          retval.orientation.add(",");
            	        }
            	    }
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
    // $ANTLR end "reactant"

    // Delegated rules
    public NETGrammar_BNGGrammar_ReactionRules.exclude_command_return exclude_command() throws RecognitionException { return gBNGGrammar_ReactionRules.exclude_command(); }
    public NETGrammar_BNGGrammar_SeedSpecies.site_list_return site_list(List sites, BondList bonds, String upperID) throws RecognitionException { return gBNGGrammar_SeedSpecies.site_list(sites, bonds, upperID); }
    public NETGrammar_BNGGrammar_SeedSpecies.species_def_return species_def(List molecules, BondList bonds, String speciesCounter) throws RecognitionException { return gBNGGrammar_SeedSpecies.species_def(molecules, bonds, speciesCounter); }
    public NETGrammar_BNGGrammar_Observables.observable_type_return observable_type() throws RecognitionException { return gBNGGrammar_Observables.observable_type(); }
    public NETGrammar_BNGGrammar_MoleculeDef.molecule_def_return molecule_def() throws RecognitionException { return gBNGGrammar_MoleculeDef.molecule_def(); }
    public NETGrammar_BNGGrammar_ReactionRules.function_keyword_return function_keyword() throws RecognitionException { return gBNGGrammar_ReactionRules.function_keyword(); }
    public NETGrammar_BNGGrammar_MoleculeDef.state_label_return state_label() throws RecognitionException { return gBNGGrammar_MoleculeDef.state_label(); }
    public NETGrammar_BNGGrammar_Expression.sign_expression_return sign_expression() throws RecognitionException { return gBNGGrammar_Expression.sign_expression(); }
    public NETGrammar_BNGGrammar_Observables.pattern_list_return pattern_list(String upperID) throws RecognitionException { return gBNGGrammar_Observables.pattern_list(upperID); }
    public NETGrammar_BNGGrammar_ReactionRules.pattern_return pattern() throws RecognitionException { return gBNGGrammar_ReactionRules.pattern(); }
    public NETGrammar_BNGGrammar_Expression.primitive_element_return primitive_element() throws RecognitionException { return gBNGGrammar_Expression.primitive_element(); }
    public NETGrammar_BNGGrammar_ReactionRules.population_maps_block_return population_maps_block() throws RecognitionException { return gBNGGrammar_ReactionRules.population_maps_block(); }
    public NETGrammar_BNGGrammar_ReactionRules.match_attribute_return match_attribute() throws RecognitionException { return gBNGGrammar_ReactionRules.match_attribute(); }
    public NETGrammar_BNGGrammar_Expression.expression_return expression(Map<String,Register> memory) throws RecognitionException { return gBNGGrammar_Expression.expression(memory); }
    public NETGrammar_BNGGrammar_Expression.variable_return variable() throws RecognitionException { return gBNGGrammar_Expression.variable(); }
    public NETGrammar_BNGGrammar_Expression.expression2_return expression2() throws RecognitionException { return gBNGGrammar_Expression.expression2(); }
    public NETGrammar_BNGGrammar_ReactionRules.reaction_def_return reaction_def(List patternsReactants, List patternsProducts, String upperID) throws RecognitionException { return gBNGGrammar_ReactionRules.reaction_def(patternsReactants, patternsProducts, upperID); }
    public NETGrammar_BNGGrammar_SeedSpecies.site_obs_return site_obs(BondList bonds, String upperID) throws RecognitionException { return gBNGGrammar_SeedSpecies.site_obs(bonds, upperID); }
    public NETGrammar_BNGGrammar_ReactionRules.reaction_rules_block_return reaction_rules_block(List reactionRules) throws RecognitionException { return gBNGGrammar_ReactionRules.reaction_rules_block(reactionRules); }
    public NETGrammar_BNGGrammar_Expression.adding_expression_return adding_expression() throws RecognitionException { return gBNGGrammar_Expression.adding_expression(); }
    public NETGrammar_BNGGrammar_Observables.observable_def_line_return observable_def_line(String upperID) throws RecognitionException { return gBNGGrammar_Observables.observable_def_line(upperID); }
    public NETGrammar_BNGGrammar_Parameters.parameters_block_return parameters_block(Map<String,Register> memory, List parameters) throws RecognitionException { return gBNGGrammar_Parameters.parameters_block(memory, parameters); }
    public NETGrammar_BNGGrammar_ReactionRules.rate_list_return rate_list(List<String> rateList, boolean bidirectional) throws RecognitionException { return gBNGGrammar_ReactionRules.rate_list(rateList, bidirectional); }
    public NETGrammar_BNGGrammar_Expression.relational_expression_return relational_expression() throws RecognitionException { return gBNGGrammar_Expression.relational_expression(); }
    public NETGrammar_BNGGrammar_ReactionRules.reactionReference_return reactionReference() throws RecognitionException { return gBNGGrammar_ReactionRules.reactionReference(); }
    public NETGrammar_BNGGrammar_ReactionRules.reactionLabel_return reactionLabel() throws RecognitionException { return gBNGGrammar_ReactionRules.reactionLabel(); }
    public NETGrammar_BNGGrammar_Expression.function_return function() throws RecognitionException { return gBNGGrammar_Expression.function(); }
    public NETGrammar_BNGGrammar_MoleculeDef.site_def_return site_def() throws RecognitionException { return gBNGGrammar_MoleculeDef.site_def(); }
    public NETGrammar_BNGGrammar_MoleculeDef.molecule_types_block_return molecule_types_block() throws RecognitionException { return gBNGGrammar_MoleculeDef.molecule_types_block(); }
    public NETGrammar_BNGGrammar_ReactionRules.rule_species_def_return rule_species_def(String upperID, ReactionAction reactionAction) throws RecognitionException { return gBNGGrammar_ReactionRules.rule_species_def(upperID, reactionAction); }
    public NETGrammar_BNGGrammar_Expression.boolean_negation_expression_return boolean_negation_expression() throws RecognitionException { return gBNGGrammar_Expression.boolean_negation_expression(); }
    public NETGrammar_BNGGrammar_SeedSpecies.bond_name_return bond_name() throws RecognitionException { return gBNGGrammar_SeedSpecies.bond_name(); }
    public NETGrammar_BNGGrammar_SeedSpecies.site_obs_site_return site_obs_site(BondList bonds, String upperID) throws RecognitionException { return gBNGGrammar_SeedSpecies.site_obs_site(bonds, upperID); }
    public NETGrammar_BNGGrammar_Observables.observables_block_return observables_block(List observables) throws RecognitionException { return gBNGGrammar_Observables.observables_block(observables); }
    public NETGrammar_BNGGrammar_ReactionRules.rate_function_return rate_function(List<String> rateList, boolean bidirectional) throws RecognitionException { return gBNGGrammar_ReactionRules.rate_function(rateList, bidirectional); }
    public NETGrammar_BNGGrammar_Observables.obs_species_def_return obs_species_def(String upperID, ReactionAction reactionAction) throws RecognitionException { return gBNGGrammar_Observables.obs_species_def(upperID, reactionAction); }
    public NETGrammar_BNGGrammar_Parameters.parameter_def_return parameter_def(Map<String,Register> lmemory) throws RecognitionException { return gBNGGrammar_Parameters.parameter_def(lmemory); }
    public NETGrammar_BNGGrammar_SeedSpecies.species_element_return species_element(BondList bonds, String upperID, String compartment) throws RecognitionException { return gBNGGrammar_SeedSpecies.species_element(bonds, upperID, compartment); }
    public NETGrammar_BNGGrammar_Expression.power_expression_return power_expression() throws RecognitionException { return gBNGGrammar_Expression.power_expression(); }
    public NETGrammar_BNGGrammar_Observables.relational_observer_return relational_observer() throws RecognitionException { return gBNGGrammar_Observables.relational_observer(); }
    public NETGrammar_BNGGrammar_ReactionRules.modif_command_return modif_command() throws RecognitionException { return gBNGGrammar_ReactionRules.modif_command(); }
    public NETGrammar_BNGGrammar_Expression.multiplying_expression_return multiplying_expression() throws RecognitionException { return gBNGGrammar_Expression.multiplying_expression(); }
    public NETGrammar_BNGGrammar_Expression.if_expression_return if_expression() throws RecognitionException { return gBNGGrammar_Expression.if_expression(); }
    public NETGrammar_BNGGrammar_Expression.number_return number() throws RecognitionException { return gBNGGrammar_Expression.number(); }
    public NETGrammar_BNGGrammar_ReactionRules.reaction_rule_def_return reaction_rule_def(int reactionCounter) throws RecognitionException { return gBNGGrammar_ReactionRules.reaction_rule_def(reactionCounter); }
    public NETGrammar_BNGGrammar_SeedSpecies.site_obs_state_return site_obs_state() throws RecognitionException { return gBNGGrammar_SeedSpecies.site_obs_state(); }
    public NETGrammar_BNGGrammar_ReactionRules.include_command_return include_command() throws RecognitionException { return gBNGGrammar_ReactionRules.include_command(); }


    protected DFA16 dfa16 = new DFA16(this);
    static final String DFA16_eotS =
        "\13\uffff";
    static final String DFA16_eofS =
        "\13\uffff";
    static final String DFA16_minS =
        "\1\11\1\12\11\uffff";
    static final String DFA16_maxS =
        "\1\11\1\51\11\uffff";
    static final String DFA16_acceptS =
        "\2\uffff\1\1\1\2\1\4\1\5\1\6\1\7\1\10\1\11\1\3";
    static final String DFA16_specialS =
        "\13\uffff}>";
    static final String[] DFA16_transitionS = {
            "\1\1",
            "\2\12\1\11\1\3\3\uffff\1\5\1\10\1\uffff\1\6\1\7\1\uffff\1\4"+
            "\21\uffff\1\2",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            ""
    };

    static final short[] DFA16_eot = DFA.unpackEncodedString(DFA16_eotS);
    static final short[] DFA16_eof = DFA.unpackEncodedString(DFA16_eofS);
    static final char[] DFA16_min = DFA.unpackEncodedStringToUnsignedChars(DFA16_minS);
    static final char[] DFA16_max = DFA.unpackEncodedStringToUnsignedChars(DFA16_maxS);
    static final short[] DFA16_accept = DFA.unpackEncodedString(DFA16_acceptS);
    static final short[] DFA16_special = DFA.unpackEncodedString(DFA16_specialS);
    static final short[][] DFA16_transition;

    static {
        int numStates = DFA16_transitionS.length;
        DFA16_transition = new short[numStates][];
        for (int i=0; i<numStates; i++) {
            DFA16_transition[i] = DFA.unpackEncodedString(DFA16_transitionS[i]);
        }
    }

    class DFA16 extends DFA {

        public DFA16(BaseRecognizer recognizer) {
            this.recognizer = recognizer;
            this.decisionNumber = 16;
            this.eot = DFA16_eot;
            this.eof = DFA16_eof;
            this.min = DFA16_min;
            this.max = DFA16_max;
            this.accept = DFA16_accept;
            this.special = DFA16_special;
            this.transition = DFA16_transition;
        }
        public String getDescription() {
            return "336:1: program_block : ( parameters_block[memory,$prog::parameters] | molecule_types_block | seed_species_block[$prog::seedSpecies] | observables_block[$prog::observables] | reaction_rules_block[$prog::reactionRules] | functions_block | compartments_block | reactions_block[reactions] | groups_block );";
        }
    }
 

    public static final BitSet FOLLOW_BEGIN_in_seed_species_block84 = new BitSet(new long[]{0x0000000000000C00L});
    public static final BitSet FOLLOW_SEED_in_seed_species_block87 = new BitSet(new long[]{0x0000000000000800L});
    public static final BitSet FOLLOW_SPECIES_in_seed_species_block91 = new BitSet(new long[]{0x0000000000000020L});
    public static final BitSet FOLLOW_LB_in_seed_species_block93 = new BitSet(new long[]{0x0000000000000120L,0x0002000000000000L});
    public static final BitSet FOLLOW_seed_species_def_in_seed_species_block97 = new BitSet(new long[]{0x0000000000000020L});
    public static final BitSet FOLLOW_LB_in_seed_species_block101 = new BitSet(new long[]{0x0000000000000120L,0x0002000000000000L});
    public static final BitSet FOLLOW_END_in_seed_species_block107 = new BitSet(new long[]{0x0000000000000C00L});
    public static final BitSet FOLLOW_SEED_in_seed_species_block110 = new BitSet(new long[]{0x0000000000000800L});
    public static final BitSet FOLLOW_SPECIES_in_seed_species_block114 = new BitSet(new long[]{0x0000000000000020L});
    public static final BitSet FOLLOW_LB_in_seed_species_block116 = new BitSet(new long[]{0x0000000000000022L});
    public static final BitSet FOLLOW_pre_species_def_in_seed_species_def145 = new BitSet(new long[]{0x0004002000000000L,0x100B000180000000L,0x0000000000001000L});
    public static final BitSet FOLLOW_expression_in_seed_species_def164 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_INT_in_pre_species_def265 = new BitSet(new long[]{0x0000000000000000L,0x0008000000000000L,0x0000000000008005L});
    public static final BitSet FOLLOW_species_def_in_pre_species_def271 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_BEGIN_in_groups_block290 = new BitSet(new long[]{0x0000000000001000L});
    public static final BitSet FOLLOW_GROUPS_in_groups_block292 = new BitSet(new long[]{0x0000000000000020L});
    public static final BitSet FOLLOW_LB_in_groups_block294 = new BitSet(new long[]{0x0000000000000120L,0x0002000000000000L});
    public static final BitSet FOLLOW_group_line_in_groups_block300 = new BitSet(new long[]{0x0000000000000020L});
    public static final BitSet FOLLOW_LB_in_groups_block302 = new BitSet(new long[]{0x0000000000000120L,0x0002000000000000L});
    public static final BitSet FOLLOW_END_in_groups_block307 = new BitSet(new long[]{0x0000000000001000L});
    public static final BitSet FOLLOW_GROUPS_in_groups_block309 = new BitSet(new long[]{0x0000000000000020L});
    public static final BitSet FOLLOW_LB_in_groups_block311 = new BitSet(new long[]{0x0000000000000022L});
    public static final BitSet FOLLOW_INT_in_group_line322 = new BitSet(new long[]{0x0000000000000000L,0x0008000000000000L});
    public static final BitSet FOLLOW_STRING_in_group_line324 = new BitSet(new long[]{0x0000000000000000L,0x0002000000000000L});
    public static final BitSet FOLLOW_group_list_in_group_line326 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_group_element_in_group_list349 = new BitSet(new long[]{0x0000000000000002L,0x0400000000000000L});
    public static final BitSet FOLLOW_COMMA_in_group_list357 = new BitSet(new long[]{0x0000000000000000L,0x0002000000000000L});
    public static final BitSet FOLLOW_group_element_in_group_list361 = new BitSet(new long[]{0x0000000000000002L,0x0400000000000000L});
    public static final BitSet FOLLOW_INT_in_group_element398 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_INT_in_group_element419 = new BitSet(new long[]{0x0000000000000000L,0x0000000000000000L,0x0000000000000800L});
    public static final BitSet FOLLOW_TIMES_in_group_element421 = new BitSet(new long[]{0x0000000000000000L,0x0002000000000000L});
    public static final BitSet FOLLOW_INT_in_group_element425 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_LB_in_prog469 = new BitSet(new long[]{0x0000000000000020L,0x0000000004000000L});
    public static final BitSet FOLLOW_SUBSTANCEUNITS_in_prog472 = new BitSet(new long[]{0x0000000000000000L,0x1000000000000000L});
    public static final BitSet FOLLOW_LPAREN_in_prog474 = new BitSet(new long[]{0x0000000000000000L,0x0000000000000000L,0x0000000000080000L});
    public static final BitSet FOLLOW_DBQUOTES_in_prog476 = new BitSet(new long[]{0x0000000000000000L,0x0008000000000000L});
    public static final BitSet FOLLOW_STRING_in_prog478 = new BitSet(new long[]{0x0000000000000000L,0x0000000000000000L,0x0000000000080000L});
    public static final BitSet FOLLOW_DBQUOTES_in_prog480 = new BitSet(new long[]{0x0000000000000000L,0x2000000000000000L});
    public static final BitSet FOLLOW_RPAREN_in_prog482 = new BitSet(new long[]{0x0000000000000000L,0x0010000000000000L});
    public static final BitSet FOLLOW_SEMI_in_prog484 = new BitSet(new long[]{0x0000000000000020L});
    public static final BitSet FOLLOW_LB_in_prog486 = new BitSet(new long[]{0x0000000000000220L});
    public static final BitSet FOLLOW_program_block_in_prog490 = new BitSet(new long[]{0x0000000000000200L});
    public static final BitSet FOLLOW_EOF_in_prog499 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_VERSION_in_version_def609 = new BitSet(new long[]{0x0000000000000000L,0x1000000000000000L});
    public static final BitSet FOLLOW_LPAREN_in_version_def611 = new BitSet(new long[]{0x0000000000000000L,0x0000000000000000L,0x0000000000080000L});
    public static final BitSet FOLLOW_DBQUOTES_in_version_def613 = new BitSet(new long[]{0x0000000000000000L,0x0000000000000000L,0x0000000001000000L});
    public static final BitSet FOLLOW_VERSION_NUMBER_in_version_def615 = new BitSet(new long[]{0x0000000000000000L,0x0000000000000000L,0x0000000000080000L});
    public static final BitSet FOLLOW_DBQUOTES_in_version_def617 = new BitSet(new long[]{0x0000000000000000L,0x2000000000000000L});
    public static final BitSet FOLLOW_RPAREN_in_version_def619 = new BitSet(new long[]{0x0000000000000000L,0x0010000000000000L});
    public static final BitSet FOLLOW_SEMI_in_version_def621 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_parameters_block_in_program_block633 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_molecule_types_block_in_program_block638 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_seed_species_block_in_program_block642 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_observables_block_in_program_block647 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_reaction_rules_block_in_program_block652 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_functions_block_in_program_block657 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_compartments_block_in_program_block661 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_reactions_block_in_program_block665 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_groups_block_in_program_block670 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_BEGIN_in_functions_block695 = new BitSet(new long[]{0x0000000000100000L});
    public static final BitSet FOLLOW_FUNCTIONS_in_functions_block697 = new BitSet(new long[]{0x0000000000000020L});
    public static final BitSet FOLLOW_LB_in_functions_block699 = new BitSet(new long[]{0x0000000000000120L,0x0008000000000000L});
    public static final BitSet FOLLOW_function_def_in_functions_block710 = new BitSet(new long[]{0x0000000000000020L});
    public static final BitSet FOLLOW_LB_in_functions_block712 = new BitSet(new long[]{0x0000000000000120L,0x0008000000000000L});
    public static final BitSet FOLLOW_END_in_functions_block729 = new BitSet(new long[]{0x0000000000100000L});
    public static final BitSet FOLLOW_FUNCTIONS_in_functions_block731 = new BitSet(new long[]{0x0000000000000020L});
    public static final BitSet FOLLOW_LB_in_functions_block733 = new BitSet(new long[]{0x0000000000000022L});
    public static final BitSet FOLLOW_STRING_in_function_def748 = new BitSet(new long[]{0x0000000000000000L,0x1000000000000000L});
    public static final BitSet FOLLOW_LPAREN_in_function_def750 = new BitSet(new long[]{0x0000000000000000L,0x2008000000000000L});
    public static final BitSet FOLLOW_STRING_in_function_def753 = new BitSet(new long[]{0x0000000000000000L,0x2000000000000000L});
    public static final BitSet FOLLOW_RPAREN_in_function_def757 = new BitSet(new long[]{0x0004002000000000L,0x100B000180000000L,0x0000000000001200L});
    public static final BitSet FOLLOW_BECOMES_in_function_def760 = new BitSet(new long[]{0x0004002000000000L,0x100B000180000000L,0x0000000000001000L});
    public static final BitSet FOLLOW_expression_in_function_def765 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_BEGIN_in_compartments_block828 = new BitSet(new long[]{0x0000000000200000L});
    public static final BitSet FOLLOW_COMPARTMENTS_in_compartments_block830 = new BitSet(new long[]{0x0000000000000020L});
    public static final BitSet FOLLOW_LB_in_compartments_block832 = new BitSet(new long[]{0x0000000000000120L,0x0008000000000000L});
    public static final BitSet FOLLOW_compartment_in_compartments_block842 = new BitSet(new long[]{0x0000000000000020L});
    public static final BitSet FOLLOW_LB_in_compartments_block844 = new BitSet(new long[]{0x0000000000000120L,0x0008000000000000L});
    public static final BitSet FOLLOW_END_in_compartments_block852 = new BitSet(new long[]{0x0000000000200000L});
    public static final BitSet FOLLOW_COMPARTMENTS_in_compartments_block854 = new BitSet(new long[]{0x0000000000000020L});
    public static final BitSet FOLLOW_LB_in_compartments_block856 = new BitSet(new long[]{0x0000000000000022L});
    public static final BitSet FOLLOW_STRING_in_compartment871 = new BitSet(new long[]{0x0000000000000000L,0x0002000000000000L});
    public static final BitSet FOLLOW_INT_in_compartment873 = new BitSet(new long[]{0x0004002000000000L,0x100B000180000000L,0x0000000000001000L});
    public static final BitSet FOLLOW_expression_in_compartment877 = new BitSet(new long[]{0x0000000000000002L,0x0008000000000000L});
    public static final BitSet FOLLOW_STRING_in_compartment892 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_MOD_in_label945 = new BitSet(new long[]{0x0000000000000000L,0x0008000000000000L});
    public static final BitSet FOLLOW_STRING_in_label947 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_AMPERSAND_in_dereference963 = new BitSet(new long[]{0x0000000000000000L,0x0008000000000000L});
    public static final BitSet FOLLOW_STRING_in_dereference965 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_BEGIN_in_reactions_block983 = new BitSet(new long[]{0x0000000000040000L});
    public static final BitSet FOLLOW_REACTIONS_in_reactions_block985 = new BitSet(new long[]{0x0000000000000020L});
    public static final BitSet FOLLOW_LB_in_reactions_block987 = new BitSet(new long[]{0x0000000000000120L,0x0002000000000000L});
    public static final BitSet FOLLOW_INT_in_reactions_block993 = new BitSet(new long[]{0x0000000000000000L,0x0002000000000000L});
    public static final BitSet FOLLOW_reaction_in_reactions_block995 = new BitSet(new long[]{0x0000000000000020L});
    public static final BitSet FOLLOW_LB_in_reactions_block997 = new BitSet(new long[]{0x0000000000000120L,0x0002000000000000L});
    public static final BitSet FOLLOW_END_in_reactions_block1006 = new BitSet(new long[]{0x0000000000040000L});
    public static final BitSet FOLLOW_REACTIONS_in_reactions_block1008 = new BitSet(new long[]{0x0000000000000020L});
    public static final BitSet FOLLOW_LB_in_reactions_block1010 = new BitSet(new long[]{0x0000000000000022L});
    public static final BitSet FOLLOW_reactant_in_reaction1030 = new BitSet(new long[]{0x0000000000000000L,0x0002000000000000L});
    public static final BitSet FOLLOW_reactant_in_reaction1035 = new BitSet(new long[]{0x0004002000000000L,0x100B000180000000L,0x0000000000001000L});
    public static final BitSet FOLLOW_expression_in_reaction1038 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_INT_in_reactant1111 = new BitSet(new long[]{0x0000000000000002L,0x0400000000000000L});
    public static final BitSet FOLLOW_COMMA_in_reactant1124 = new BitSet(new long[]{0x0000000000000000L,0x0002000000000000L});
    public static final BitSet FOLLOW_INT_in_reactant1128 = new BitSet(new long[]{0x0000000000000002L,0x0400000000000000L});

}