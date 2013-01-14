// $ANTLR 3.3 Nov 30, 2010 12:50:56 BNGGrammar_SeedSpecies.g 2013-01-14 14:49:39

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
public class BNGGrammar_BNGGrammar_SeedSpecies extends Parser {
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
    // delegators
    public BNGGrammar gBNGGrammar;
    public BNGGrammar gParent;


        public BNGGrammar_BNGGrammar_SeedSpecies(TokenStream input, BNGGrammar gBNGGrammar) {
            this(input, new RecognizerSharedState(), gBNGGrammar);
        }
        public BNGGrammar_BNGGrammar_SeedSpecies(TokenStream input, RecognizerSharedState state, BNGGrammar gBNGGrammar) {
            super(input, state);
            this.gBNGGrammar = gBNGGrammar;
             
            gParent = gBNGGrammar;
        }
        
    protected StringTemplateGroup templateLib =
      new StringTemplateGroup("BNGGrammar_BNGGrammar_SeedSpeciesTemplates", AngleBracketTemplateLexer.class);

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
    public String getGrammarFileName() { return "BNGGrammar_SeedSpecies.g"; }


     public void getParentTemplate(){
     
      this.setTemplateLib(gParent.getTemplateLib());
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
    // BNGGrammar_SeedSpecies.g:15:1: seed_species_block[List seedSpecies] : BEGIN ( SEED )? SPECIES ( LB )+ ( seed_species_def[$seed_species_block::numSpecies] ( LB )+ )* END ( SEED )? SPECIES ( LB )+ ;
    public final BNGGrammar_BNGGrammar_SeedSpecies.seed_species_block_return seed_species_block(List seedSpecies) throws RecognitionException {
        seed_species_block_stack.push(new seed_species_block_scope());
        BNGGrammar_BNGGrammar_SeedSpecies.seed_species_block_return retval = new BNGGrammar_BNGGrammar_SeedSpecies.seed_species_block_return();
        retval.start = input.LT(1);

        BNGGrammar_BNGGrammar_SeedSpecies.seed_species_def_return seed_species_def1 = null;



         ((seed_species_block_scope)seed_species_block_stack.peek()).numSpecies = 1;
           gParent.paraphrases.push("in seed species section");
           getParentTemplate();

        try {
            // BNGGrammar_SeedSpecies.g:27:1: ( BEGIN ( SEED )? SPECIES ( LB )+ ( seed_species_def[$seed_species_block::numSpecies] ( LB )+ )* END ( SEED )? SPECIES ( LB )+ )
            // BNGGrammar_SeedSpecies.g:27:3: BEGIN ( SEED )? SPECIES ( LB )+ ( seed_species_def[$seed_species_block::numSpecies] ( LB )+ )* END ( SEED )? SPECIES ( LB )+
            {
            match(input,BEGIN,FOLLOW_BEGIN_in_seed_species_block53); 
            // BNGGrammar_SeedSpecies.g:27:9: ( SEED )?
            int alt1=2;
            int LA1_0 = input.LA(1);

            if ( (LA1_0==SEED) ) {
                alt1=1;
            }
            switch (alt1) {
                case 1 :
                    // BNGGrammar_SeedSpecies.g:27:10: SEED
                    {
                    match(input,SEED,FOLLOW_SEED_in_seed_species_block56); 

                    }
                    break;

            }

            match(input,SPECIES,FOLLOW_SPECIES_in_seed_species_block60); 
            // BNGGrammar_SeedSpecies.g:27:25: ( LB )+
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
            	    // BNGGrammar_SeedSpecies.g:27:25: LB
            	    {
            	    match(input,LB,FOLLOW_LB_in_seed_species_block62); 

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

            // BNGGrammar_SeedSpecies.g:28:1: ( seed_species_def[$seed_species_block::numSpecies] ( LB )+ )*
            loop4:
            do {
                int alt4=2;
                int LA4_0 = input.LA(1);

                if ( (LA4_0==INT||LA4_0==STRING||LA4_0==DOLLAR||LA4_0==AT||LA4_0==MOD) ) {
                    alt4=1;
                }


                switch (alt4) {
            	case 1 :
            	    // BNGGrammar_SeedSpecies.g:28:2: seed_species_def[$seed_species_block::numSpecies] ( LB )+
            	    {
            	    pushFollow(FOLLOW_seed_species_def_in_seed_species_block66);
            	    seed_species_def1=seed_species_def(((seed_species_block_scope)seed_species_block_stack.peek()).numSpecies);

            	    state._fsp--;


            	                                          seedSpecies.add((seed_species_def1!=null?seed_species_def1.st:null)); 
            	                                          ((seed_species_block_scope)seed_species_block_stack.peek()).numSpecies++;
            	                                          
            	    // BNGGrammar_SeedSpecies.g:31:41: ( LB )+
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
            	    	    // BNGGrammar_SeedSpecies.g:31:41: LB
            	    	    {
            	    	    match(input,LB,FOLLOW_LB_in_seed_species_block70); 

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

            match(input,END,FOLLOW_END_in_seed_species_block76); 
            // BNGGrammar_SeedSpecies.g:32:5: ( SEED )?
            int alt5=2;
            int LA5_0 = input.LA(1);

            if ( (LA5_0==SEED) ) {
                alt5=1;
            }
            switch (alt5) {
                case 1 :
                    // BNGGrammar_SeedSpecies.g:32:6: SEED
                    {
                    match(input,SEED,FOLLOW_SEED_in_seed_species_block79); 

                    }
                    break;

            }

            match(input,SPECIES,FOLLOW_SPECIES_in_seed_species_block83); 
            // BNGGrammar_SeedSpecies.g:32:21: ( LB )+
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
            	    // BNGGrammar_SeedSpecies.g:32:21: LB
            	    {
            	    match(input,LB,FOLLOW_LB_in_seed_species_block85); 

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


              gParent.paraphrases.pop();

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
    }
    protected Stack seed_species_def_stack = new Stack();

    public static class seed_species_def_return extends ParserRuleReturnScope {
        public StringTemplate st;
        public Object getTemplate() { return st; }
        public String toString() { return st==null?null:st.toString(); }
    };

    // $ANTLR start "seed_species_def"
    // BNGGrammar_SeedSpecies.g:34:1: seed_species_def[int counter] : ( STRING COLON )? pre_species_def[$seed_species_def::molecules,$seed_species_def::bonds,counter] WS expression[gParent.memory] -> seed_species_block(id=\"S\" + counterconcentration=$expression.textname=$pre_species_def.textmolecules=$seed_species_def::moleculesfirstBonds=$seed_species_def::bonds.getLeft()secondBonds=$seed_species_def::bonds.getRight());
    public final BNGGrammar_BNGGrammar_SeedSpecies.seed_species_def_return seed_species_def(int counter) throws RecognitionException {
        seed_species_def_stack.push(new seed_species_def_scope());
        BNGGrammar_BNGGrammar_SeedSpecies.seed_species_def_return retval = new BNGGrammar_BNGGrammar_SeedSpecies.seed_species_def_return();
        retval.start = input.LT(1);

        BNGGrammar_BNGGrammar_Expression.expression_return expression2 = null;

        BNGGrammar_BNGGrammar_SeedSpecies.pre_species_def_return pre_species_def3 = null;



          ((seed_species_def_scope)seed_species_def_stack.peek()).molecules = new ArrayList();
          ((seed_species_def_scope)seed_species_def_stack.peek()).bonds = new BondList();


        try {
            // BNGGrammar_SeedSpecies.g:45:1: ( ( STRING COLON )? pre_species_def[$seed_species_def::molecules,$seed_species_def::bonds,counter] WS expression[gParent.memory] -> seed_species_block(id=\"S\" + counterconcentration=$expression.textname=$pre_species_def.textmolecules=$seed_species_def::moleculesfirstBonds=$seed_species_def::bonds.getLeft()secondBonds=$seed_species_def::bonds.getRight()))
            // BNGGrammar_SeedSpecies.g:46:1: ( STRING COLON )? pre_species_def[$seed_species_def::molecules,$seed_species_def::bonds,counter] WS expression[gParent.memory]
            {
            // BNGGrammar_SeedSpecies.g:46:1: ( STRING COLON )?
            int alt7=2;
            int LA7_0 = input.LA(1);

            if ( (LA7_0==STRING) ) {
                int LA7_1 = input.LA(2);

                if ( (LA7_1==COLON) ) {
                    alt7=1;
                }
            }
            switch (alt7) {
                case 1 :
                    // BNGGrammar_SeedSpecies.g:46:2: STRING COLON
                    {
                    match(input,STRING,FOLLOW_STRING_in_seed_species_def104); 
                    match(input,COLON,FOLLOW_COLON_in_seed_species_def106); 

                    }
                    break;

            }

            pushFollow(FOLLOW_pre_species_def_in_seed_species_def110);
            pre_species_def3=pre_species_def(((seed_species_def_scope)seed_species_def_stack.peek()).molecules, ((seed_species_def_scope)seed_species_def_stack.peek()).bonds, counter);

            state._fsp--;


             //There needs to need a space between species and the expression token, so we go back and make sure there was one
            ((ChangeableChannelTokenStream)input).seek(((ChangeableChannelTokenStream)input).index()-1)  ;

            match(input,WS,FOLLOW_WS_in_seed_species_def122); 
            pushFollow(FOLLOW_expression_in_seed_species_def130);
            expression2=gBNGGrammar.expression(gParent.memory);

            state._fsp--;



            // TEMPLATE REWRITE
            // 51:33: -> seed_species_block(id=\"S\" + counterconcentration=$expression.textname=$pre_species_def.textmolecules=$seed_species_def::moleculesfirstBonds=$seed_species_def::bonds.getLeft()secondBonds=$seed_species_def::bonds.getRight())
            {
                retval.st = templateLib.getInstanceOf("seed_species_block",
              new STAttrMap().put("id", "S" + counter).put("concentration", (expression2!=null?input.toString(expression2.start,expression2.stop):null)).put("name", (pre_species_def3!=null?input.toString(pre_species_def3.start,pre_species_def3.stop):null)).put("molecules", ((seed_species_def_scope)seed_species_def_stack.peek()).molecules).put("firstBonds", ((seed_species_def_scope)seed_species_def_stack.peek()).bonds.getLeft()).put("secondBonds", ((seed_species_def_scope)seed_species_def_stack.peek()).bonds.getRight()));
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
        public Map<String,Register> lmemory;
        public StringTemplate st;
        public Object getTemplate() { return st; }
        public String toString() { return st==null?null:st.toString(); }
    };

    // $ANTLR start "pre_species_def"
    // BNGGrammar_SeedSpecies.g:56:1: pre_species_def[List molecules,BondList bonds, int speciesCounter] returns [String compartment,Map<String,Register> lmemory] : ({...}? INT | ) species_def[molecules,bonds,\"S\" + speciesCounter] ;
    public final BNGGrammar_BNGGrammar_SeedSpecies.pre_species_def_return pre_species_def(List molecules, BondList bonds, int speciesCounter) throws RecognitionException {
        pre_species_def_stack.push(new pre_species_def_scope());
        BNGGrammar_BNGGrammar_SeedSpecies.pre_species_def_return retval = new BNGGrammar_BNGGrammar_SeedSpecies.pre_species_def_return();
        retval.start = input.LT(1);

        BNGGrammar_BNGGrammar_SeedSpecies.species_def_return species_def4 = null;


        try {
            // BNGGrammar_SeedSpecies.g:62:1: ( ({...}? INT | ) species_def[molecules,bonds,\"S\" + speciesCounter] )
            // BNGGrammar_SeedSpecies.g:64:1: ({...}? INT | ) species_def[molecules,bonds,\"S\" + speciesCounter]
            {
            // BNGGrammar_SeedSpecies.g:64:1: ({...}? INT | )
            int alt8=2;
            int LA8_0 = input.LA(1);

            if ( (LA8_0==INT) ) {
                alt8=1;
            }
            else if ( (LA8_0==STRING||LA8_0==DOLLAR||LA8_0==AT||LA8_0==MOD) ) {
                alt8=2;
            }
            else {
                NoViableAltException nvae =
                    new NoViableAltException("", 8, 0, input);

                throw nvae;
            }
            switch (alt8) {
                case 1 :
                    // BNGGrammar_SeedSpecies.g:64:2: {...}? INT
                    {
                    if ( !((gParent.netGrammar)) ) {
                        throw new FailedPredicateException(input, "pre_species_def", "gParent.netGrammar");
                    }
                    match(input,INT,FOLLOW_INT_in_pre_species_def194); 

                    }
                    break;
                case 2 :
                    // BNGGrammar_SeedSpecies.g:64:30: 
                    {
                    }
                    break;

            }

            pushFollow(FOLLOW_species_def_in_pre_species_def201);
            species_def4=species_def(molecules, bonds, "S" + speciesCounter);

            state._fsp--;


                ((pre_species_def_scope)pre_species_def_stack.peek()).constant = (species_def4!=null?species_def4.constant:false);
                retval.lmemory = (species_def4!=null?species_def4.lmemory:null);
             

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

    protected static class species_def_scope {
        int numMolecules;
        String variableName;
    }
    protected Stack species_def_stack = new Stack();

    public static class species_def_return extends ParserRuleReturnScope {
        public Map<String,List<ReactionRegister>> listOfMolecules;
        public String glabel;
        public boolean constant;
        public String compartment="";
        public Map<String, Register> lmemory;
        public StringTemplate st;
        public Object getTemplate() { return st; }
        public String toString() { return st==null?null:st.toString(); }
    };

    // $ANTLR start "species_def"
    // BNGGrammar_SeedSpecies.g:78:1: species_def[List molecules,BondList bonds,String speciesCounter] returns [Map<String,List<ReactionRegister>> listOfMolecules, String glabel, boolean constant,String compartment=\"\",Map<String, Register> lmemory] : ( AT scomp= STRING COLON ({...}? COLON | ) )? ( MOD slabel= STRING COLON ( COLON )? )? ( DOLLAR )? (s1= species_element[bonds,speciesCounter + \"_M\" + $species_def::numMolecules,$scomp.text] ( DOT s2= species_element[bonds,speciesCounter + \"_M\" + $species_def::numMolecules,$scomp.text] )* ) -> molecules_def(id=$species_def::variableName);
    public final BNGGrammar_BNGGrammar_SeedSpecies.species_def_return species_def(List molecules, BondList bonds, String speciesCounter) throws RecognitionException {
        species_def_stack.push(new species_def_scope());
        BNGGrammar_BNGGrammar_SeedSpecies.species_def_return retval = new BNGGrammar_BNGGrammar_SeedSpecies.species_def_return();
        retval.start = input.LT(1);

        Token scomp=null;
        Token slabel=null;
        BNGGrammar_BNGGrammar_SeedSpecies.species_element_return s1 = null;

        BNGGrammar_BNGGrammar_SeedSpecies.species_element_return s2 = null;



          ((species_def_scope)species_def_stack.peek()).numMolecules =1;
          retval.listOfMolecules = new HashMap<String,List<ReactionRegister>>();
           //We cannot have spaces between the elements of a species definition, so we activate space recognition. Should there be a space
           //the parser will not continue with this rule
         //  ((ChangeableChannelTokenStream)input).setChannel(Token.DEFAULT_CHANNEL|HIDDEN);
          ((species_def_scope)species_def_stack.peek()).variableName = "";
          retval.constant = false;
          retval.lmemory = new HashMap<String,Register>();
        ((ChangeableChannelTokenStream)input).addChannel(HIDDEN);
          gParent.paraphrases.push("in species def section");

        try {
            // BNGGrammar_SeedSpecies.g:104:1: ( ( AT scomp= STRING COLON ({...}? COLON | ) )? ( MOD slabel= STRING COLON ( COLON )? )? ( DOLLAR )? (s1= species_element[bonds,speciesCounter + \"_M\" + $species_def::numMolecules,$scomp.text] ( DOT s2= species_element[bonds,speciesCounter + \"_M\" + $species_def::numMolecules,$scomp.text] )* ) -> molecules_def(id=$species_def::variableName))
            // BNGGrammar_SeedSpecies.g:105:4: ( AT scomp= STRING COLON ({...}? COLON | ) )? ( MOD slabel= STRING COLON ( COLON )? )? ( DOLLAR )? (s1= species_element[bonds,speciesCounter + \"_M\" + $species_def::numMolecules,$scomp.text] ( DOT s2= species_element[bonds,speciesCounter + \"_M\" + $species_def::numMolecules,$scomp.text] )* )
            {
            // BNGGrammar_SeedSpecies.g:105:4: ( AT scomp= STRING COLON ({...}? COLON | ) )?
            int alt10=2;
            int LA10_0 = input.LA(1);

            if ( (LA10_0==AT) ) {
                alt10=1;
            }
            switch (alt10) {
                case 1 :
                    // BNGGrammar_SeedSpecies.g:105:5: AT scomp= STRING COLON ({...}? COLON | )
                    {
                    match(input,AT,FOLLOW_AT_in_species_def249); 
                    scomp=(Token)match(input,STRING,FOLLOW_STRING_in_species_def253); 

                        retval.compartment = (scomp!=null?scomp.getText():null);
                       //((ChangeableChannelTokenStream)input).addChannel(HIDDEN);
                       
                    match(input,COLON,FOLLOW_COLON_in_species_def268); 
                    // BNGGrammar_SeedSpecies.g:111:10: ({...}? COLON | )
                    int alt9=2;
                    int LA9_0 = input.LA(1);

                    if ( (LA9_0==COLON) ) {
                        alt9=1;
                    }
                    else if ( (LA9_0==STRING||LA9_0==DOLLAR||LA9_0==MOD) ) {
                        alt9=2;
                    }
                    else {
                        NoViableAltException nvae =
                            new NoViableAltException("", 9, 0, input);

                        throw nvae;
                    }
                    switch (alt9) {
                        case 1 :
                            // BNGGrammar_SeedSpecies.g:111:11: {...}? COLON
                            {
                            if ( !((gParent.netGrammar)) ) {
                                throw new FailedPredicateException(input, "species_def", "gParent.netGrammar");
                            }
                            match(input,COLON,FOLLOW_COLON_in_species_def273); 

                            }
                            break;
                        case 2 :
                            // BNGGrammar_SeedSpecies.g:111:41: 
                            {
                            }
                            break;

                    }


                    }
                    break;

            }

            // BNGGrammar_SeedSpecies.g:113:4: ( MOD slabel= STRING COLON ( COLON )? )?
            int alt12=2;
            int LA12_0 = input.LA(1);

            if ( (LA12_0==MOD) ) {
                alt12=1;
            }
            switch (alt12) {
                case 1 :
                    // BNGGrammar_SeedSpecies.g:113:5: MOD slabel= STRING COLON ( COLON )?
                    {
                    match(input,MOD,FOLLOW_MOD_in_species_def291); 
                    slabel=(Token)match(input,STRING,FOLLOW_STRING_in_species_def295); 
                    match(input,COLON,FOLLOW_COLON_in_species_def297); 
                    // BNGGrammar_SeedSpecies.g:113:29: ( COLON )?
                    int alt11=2;
                    int LA11_0 = input.LA(1);

                    if ( (LA11_0==COLON) ) {
                        alt11=1;
                    }
                    switch (alt11) {
                        case 1 :
                            // BNGGrammar_SeedSpecies.g:113:29: COLON
                            {
                            match(input,COLON,FOLLOW_COLON_in_species_def299); 

                            }
                            break;

                    }


                        retval.lmemory.put((slabel!=null?slabel.getText():null),new Register(1.0,"observable"));
                        retval.glabel = (slabel!=null?slabel.getText():null);
                       

                    }
                    break;

            }

            // BNGGrammar_SeedSpecies.g:119:4: ( DOLLAR )?
            int alt13=2;
            int LA13_0 = input.LA(1);

            if ( (LA13_0==DOLLAR) ) {
                alt13=1;
            }
            switch (alt13) {
                case 1 :
                    // BNGGrammar_SeedSpecies.g:119:5: DOLLAR
                    {
                    match(input,DOLLAR,FOLLOW_DOLLAR_in_species_def318); 
                    retval.constant = true;

                    }
                    break;

            }

            // BNGGrammar_SeedSpecies.g:121:4: (s1= species_element[bonds,speciesCounter + \"_M\" + $species_def::numMolecules,$scomp.text] ( DOT s2= species_element[bonds,speciesCounter + \"_M\" + $species_def::numMolecules,$scomp.text] )* )
            // BNGGrammar_SeedSpecies.g:121:5: s1= species_element[bonds,speciesCounter + \"_M\" + $species_def::numMolecules,$scomp.text] ( DOT s2= species_element[bonds,speciesCounter + \"_M\" + $species_def::numMolecules,$scomp.text] )*
            {
            pushFollow(FOLLOW_species_element_in_species_def334);
            s1=species_element(bonds, speciesCounter + "_M" + ((species_def_scope)species_def_stack.peek()).numMolecules, (scomp!=null?scomp.getText():null));

            state._fsp--;


                molecules.add(s1.st);
                ((species_def_scope)species_def_stack.peek()).numMolecules++;
                ReactionRegister.addElement(retval.listOfMolecules,(s1!=null?s1.name:null),(s1!=null?s1.information:null));
                ((species_def_scope)species_def_stack.peek()).variableName += (s1!=null?input.toString(s1.start,s1.stop):null);
                //in this step we are adding the label to local memory so that the expression grammar doesn't throw a variable not found error
                //i don't think it needs to make numerical sense.
                if((s1!=null?s1.myLabel:null) != null){
                  retval.lmemory.put((s1!=null?s1.myLabel:null),new Register(1.0,"observable"));
                }
               
            // BNGGrammar_SeedSpecies.g:133:3: ( DOT s2= species_element[bonds,speciesCounter + \"_M\" + $species_def::numMolecules,$scomp.text] )*
            loop14:
            do {
                int alt14=2;
                int LA14_0 = input.LA(1);

                if ( (LA14_0==DOT) ) {
                    alt14=1;
                }


                switch (alt14) {
            	case 1 :
            	    // BNGGrammar_SeedSpecies.g:133:4: DOT s2= species_element[bonds,speciesCounter + \"_M\" + $species_def::numMolecules,$scomp.text]
            	    {
            	    match(input,DOT,FOLLOW_DOT_in_species_def346); 
            	    pushFollow(FOLLOW_species_element_in_species_def350);
            	    s2=species_element(bonds, speciesCounter + "_M" + ((species_def_scope)species_def_stack.peek()).numMolecules, (scomp!=null?scomp.getText():null));

            	    state._fsp--;

            	    molecules.add(s2.st);
            	            ((species_def_scope)species_def_stack.peek()).numMolecules++;
            	             ((species_def_scope)species_def_stack.peek()).variableName += "."+ (s2!=null?input.toString(s2.start,s2.stop):null);
            	             if((s2!=null?s2.myLabel:null) != null){
            	              retval.lmemory.put((s2!=null?s2.myLabel:null),new Register(1.0,"observable"));
            	             }
            	            

            	    }
            	    break;

            	default :
            	    break loop14;
                }
            } while (true);


               //We are done with space recognition so we turn it off and get the next token that is not a space
              // ((ChangeableChannelTokenStream)input).setChannel(Token.DEFAULT_CHANNEL);
              // ((ChangeableChannelTokenStream)input).seek(((ChangeableChannelTokenStream)input).skipOffTokenChannels(((ChangeableChannelTokenStream)input).index()));
             
               

            }


                //((ChangeableChannelTokenStream)input).delChannel(HIDDEN);
                //if(!gParent.variableName.contains(((species_def_scope)species_def_stack.peek()).variableName) && gParent.conserveNewMolecules){
                  //gParent.variableName.add(((species_def_scope)species_def_stack.peek()).variableName);
                 // gParent.molecules.add(new StringTemplate(((species_def_scope)species_def_stack.peek()).variableName));
                 // }
                


            // TEMPLATE REWRITE
            // 157:4: -> molecules_def(id=$species_def::variableName)
            {
                retval.st = templateLib.getInstanceOf("molecules_def",
              new STAttrMap().put("id", ((species_def_scope)species_def_stack.peek()).variableName));
            }


            }

            retval.stop = input.LT(-1);


            ((ChangeableChannelTokenStream)input).delChannel(HIDDEN);
            ((ChangeableChannelTokenStream)input).seek(((ChangeableChannelTokenStream)input).index()-1);
            ((ChangeableChannelTokenStream)input).consume();

              gParent.paraphrases.pop();

        }
        catch (RecognitionException re) {
            reportError(re);
            recover(input,re);
        }
        finally {
            species_def_stack.pop();
        }
        return retval;
    }
    // $ANTLR end "species_def"

    protected static class species_element_scope {
        List sites;
        String lcompartment;
        String lname;
    }
    protected Stack species_element_stack = new Stack();

    public static class species_element_return extends ParserRuleReturnScope {
        public String myLabel;
        public String name;
        public ReactionRegister information;
        public StringTemplate st;
        public Object getTemplate() { return st; }
        public String toString() { return st==null?null:st.toString(); }
    };

    // $ANTLR start "species_element"
    // BNGGrammar_SeedSpecies.g:164:1: species_element[BondList bonds,String upperID,String compartment] returns [String myLabel,String name, ReactionRegister information] : s1= STRING ( label )? ( LPAREN site_list[$species_element::sites,bonds,upperID] RPAREN )? ( AT s2= STRING )? -> list_molecule_def(id=upperIDname=$s1.textsites=$species_element::sitescompartment=$species_element::lcompartmentlabel=$myLabel);
    public final BNGGrammar_BNGGrammar_SeedSpecies.species_element_return species_element(BondList bonds, String upperID, String compartment) throws RecognitionException {
        species_element_stack.push(new species_element_scope());
        BNGGrammar_BNGGrammar_SeedSpecies.species_element_return retval = new BNGGrammar_BNGGrammar_SeedSpecies.species_element_return();
        retval.start = input.LT(1);

        Token s1=null;
        Token s2=null;
        BNGGrammar.label_return label5 = null;



        getParentTemplate();
          ((species_element_scope)species_element_stack.peek()).sites = new ArrayList();
          ((species_element_scope)species_element_stack.peek()).lcompartment = compartment;
          retval.information = new ReactionRegister();
          retval.information.setNumBonds(bonds.getNumBonds());
          retval.information.setCompartment(compartment);
          

        try {
            // BNGGrammar_SeedSpecies.g:184:1: (s1= STRING ( label )? ( LPAREN site_list[$species_element::sites,bonds,upperID] RPAREN )? ( AT s2= STRING )? -> list_molecule_def(id=upperIDname=$s1.textsites=$species_element::sitescompartment=$species_element::lcompartmentlabel=$myLabel))
            // BNGGrammar_SeedSpecies.g:185:3: s1= STRING ( label )? ( LPAREN site_list[$species_element::sites,bonds,upperID] RPAREN )? ( AT s2= STRING )?
            {
            s1=(Token)match(input,STRING,FOLLOW_STRING_in_species_element454); 
            retval.name = (s1!=null?s1.getText():null);((species_element_scope)species_element_stack.peek()).lname =(s1!=null?s1.getText():null);
            // BNGGrammar_SeedSpecies.g:185:67: ( label )?
            int alt15=2;
            int LA15_0 = input.LA(1);

            if ( (LA15_0==MOD) ) {
                int LA15_1 = input.LA(2);

                if ( (LA15_1==STRING) ) {
                    int LA15_3 = input.LA(3);

                    if ( ((LA15_3>=LB && LA15_3<=WS)||LA15_3==STRING||(LA15_3>=COMMA && LA15_3<=LPAREN)||(LA15_3>=UNI_REACTION_SIGN && LA15_3<=DOLLAR)||(LA15_3>=AT && LA15_3<=LT)||LA15_3==EQUALS||LA15_3==PLUS||LA15_3==MOD||LA15_3==DBQUOTES) ) {
                        alt15=1;
                    }
                }
                else if ( (LA15_1==INT) ) {
                    alt15=1;
                }
            }
            switch (alt15) {
                case 1 :
                    // BNGGrammar_SeedSpecies.g:185:68: label
                    {
                    pushFollow(FOLLOW_label_in_species_element459);
                    label5=gBNGGrammar.label();

                    state._fsp--;

                    retval.myLabel = (label5!=null?label5.label:null);

                    }
                    break;

            }

            // BNGGrammar_SeedSpecies.g:186:3: ( LPAREN site_list[$species_element::sites,bonds,upperID] RPAREN )?
            int alt16=2;
            int LA16_0 = input.LA(1);

            if ( (LA16_0==LPAREN) ) {
                alt16=1;
            }
            switch (alt16) {
                case 1 :
                    // BNGGrammar_SeedSpecies.g:186:4: LPAREN site_list[$species_element::sites,bonds,upperID] RPAREN
                    {
                    match(input,LPAREN,FOLLOW_LPAREN_in_species_element469); 
                    pushFollow(FOLLOW_site_list_in_species_element471);
                    site_list(((species_element_scope)species_element_stack.peek()).sites, bonds, upperID);

                    state._fsp--;

                    match(input,RPAREN,FOLLOW_RPAREN_in_species_element474); 

                    }
                    break;

            }

            // BNGGrammar_SeedSpecies.g:187:3: ( AT s2= STRING )?
            int alt17=2;
            int LA17_0 = input.LA(1);

            if ( (LA17_0==AT) ) {
                int LA17_1 = input.LA(2);

                if ( (LA17_1==STRING) ) {
                    int LA17_3 = input.LA(3);

                    if ( ((LA17_3>=LB && LA17_3<=WS)||LA17_3==STRING||(LA17_3>=COMMA && LA17_3<=DOT)||(LA17_3>=UNI_REACTION_SIGN && LA17_3<=DOLLAR)||(LA17_3>=AT && LA17_3<=LT)||LA17_3==EQUALS||LA17_3==PLUS||LA17_3==MOD||LA17_3==DBQUOTES) ) {
                        alt17=1;
                    }
                }
            }
            switch (alt17) {
                case 1 :
                    // BNGGrammar_SeedSpecies.g:187:4: AT s2= STRING
                    {
                    match(input,AT,FOLLOW_AT_in_species_element481); 
                    s2=(Token)match(input,STRING,FOLLOW_STRING_in_species_element485); 

                        ((species_element_scope)species_element_stack.peek()).lcompartment = (s2!=null?s2.getText():null); 
                        retval.information.setCompartment((s2!=null?s2.getText():null));
                        retval.information.setBondList(bonds);
                      

                    }
                    break;

            }



            // TEMPLATE REWRITE
            // 193:3: -> list_molecule_def(id=upperIDname=$s1.textsites=$species_element::sitescompartment=$species_element::lcompartmentlabel=$myLabel)
            {
                retval.st = templateLib.getInstanceOf("list_molecule_def",
              new STAttrMap().put("id", upperID).put("name", (s1!=null?s1.getText():null)).put("sites", ((species_element_scope)species_element_stack.peek()).sites).put("compartment", ((species_element_scope)species_element_stack.peek()).lcompartment).put("label", retval.myLabel));
            }


            }

            retval.stop = input.LT(-1);


              retval.information.setNumBonds(bonds.getNumBonds()-retval.information.getNumBonds());
             

        }
        catch (RecognitionException re) {
            reportError(re);
            recover(input,re);
        }
        finally {
            species_element_stack.pop();
        }
        return retval;
    }
    // $ANTLR end "species_element"

    protected static class site_list_scope {
        int numSites;
    }
    protected Stack site_list_stack = new Stack();

    public static class site_list_return extends ParserRuleReturnScope {
        public StringTemplate st;
        public Object getTemplate() { return st; }
        public String toString() { return st==null?null:st.toString(); }
    };

    // $ANTLR start "site_list"
    // BNGGrammar_SeedSpecies.g:195:1: site_list[List sites,BondList bonds,String upperID] : (s1= site_obs[bonds,upperID+\"_C\" + $site_list::numSites] ( COMMA s2= site_obs[bonds,upperID+\"_C\" + $site_list::numSites] )* )? ;
    public final BNGGrammar_BNGGrammar_SeedSpecies.site_list_return site_list(List sites, BondList bonds, String upperID) throws RecognitionException {
        site_list_stack.push(new site_list_scope());
        BNGGrammar_BNGGrammar_SeedSpecies.site_list_return retval = new BNGGrammar_BNGGrammar_SeedSpecies.site_list_return();
        retval.start = input.LT(1);

        BNGGrammar_BNGGrammar_SeedSpecies.site_obs_return s1 = null;

        BNGGrammar_BNGGrammar_SeedSpecies.site_obs_return s2 = null;



          ((site_list_scope)site_list_stack.peek()).numSites = 1;

        try {
            // BNGGrammar_SeedSpecies.g:202:1: ( (s1= site_obs[bonds,upperID+\"_C\" + $site_list::numSites] ( COMMA s2= site_obs[bonds,upperID+\"_C\" + $site_list::numSites] )* )? )
            // BNGGrammar_SeedSpecies.g:203:3: (s1= site_obs[bonds,upperID+\"_C\" + $site_list::numSites] ( COMMA s2= site_obs[bonds,upperID+\"_C\" + $site_list::numSites] )* )?
            {
            // BNGGrammar_SeedSpecies.g:203:3: (s1= site_obs[bonds,upperID+\"_C\" + $site_list::numSites] ( COMMA s2= site_obs[bonds,upperID+\"_C\" + $site_list::numSites] )* )?
            int alt19=2;
            int LA19_0 = input.LA(1);

            if ( (LA19_0==STRING) ) {
                alt19=1;
            }
            switch (alt19) {
                case 1 :
                    // BNGGrammar_SeedSpecies.g:203:4: s1= site_obs[bonds,upperID+\"_C\" + $site_list::numSites] ( COMMA s2= site_obs[bonds,upperID+\"_C\" + $site_list::numSites] )*
                    {
                    pushFollow(FOLLOW_site_obs_in_site_list542);
                    s1=site_obs(bonds, upperID+"_C" + ((site_list_scope)site_list_stack.peek()).numSites);

                    state._fsp--;

                    sites.add(s1.st);((site_list_scope)site_list_stack.peek()).numSites++;
                    // BNGGrammar_SeedSpecies.g:204:3: ( COMMA s2= site_obs[bonds,upperID+\"_C\" + $site_list::numSites] )*
                    loop18:
                    do {
                        int alt18=2;
                        int LA18_0 = input.LA(1);

                        if ( (LA18_0==COMMA) ) {
                            alt18=1;
                        }


                        switch (alt18) {
                    	case 1 :
                    	    // BNGGrammar_SeedSpecies.g:204:4: COMMA s2= site_obs[bonds,upperID+\"_C\" + $site_list::numSites]
                    	    {
                    	    match(input,COMMA,FOLLOW_COMMA_in_site_list551); 
                    	    pushFollow(FOLLOW_site_obs_in_site_list555);
                    	    s2=site_obs(bonds, upperID+"_C" + ((site_list_scope)site_list_stack.peek()).numSites);

                    	    state._fsp--;

                    	    sites.add(s2.st);((site_list_scope)site_list_stack.peek()).numSites++;

                    	    }
                    	    break;

                    	default :
                    	    break loop18;
                        }
                    } while (true);


                    }
                    break;

            }


            }

            retval.stop = input.LT(-1);

        }
        catch (RecognitionException re) {
            reportError(re);
            recover(input,re);
        }
        finally {
            site_list_stack.pop();
        }
        return retval;
    }
    // $ANTLR end "site_list"

    protected static class site_obs_scope {
        String defaultState;
        String bondIndicator;
    }
    protected Stack site_obs_stack = new Stack();

    public static class site_obs_return extends ParserRuleReturnScope {
        public String myLabel;
        public StringTemplate st;
        public Object getTemplate() { return st; }
        public String toString() { return st==null?null:st.toString(); }
    };

    // $ANTLR start "site_obs"
    // BNGGrammar_SeedSpecies.g:208:1: site_obs[BondList bonds,String upperID] returns [String myLabel] : s1= STRING ( ( (sos= site_obs_site[bonds,upperID] )? ( site_obs_state )? ) | site_obs_state sos2= site_obs_site[bonds,upperID] ) ( label )? -> component_def(id=upperIDname=$s1.textstate=$site_obs::defaultStatenumberOfBonds=$site_obs::bondIndicator);
    public final BNGGrammar_BNGGrammar_SeedSpecies.site_obs_return site_obs(BondList bonds, String upperID) throws RecognitionException {
        site_obs_stack.push(new site_obs_scope());
        BNGGrammar_BNGGrammar_SeedSpecies.site_obs_return retval = new BNGGrammar_BNGGrammar_SeedSpecies.site_obs_return();
        retval.start = input.LT(1);

        Token s1=null;
        BNGGrammar_BNGGrammar_SeedSpecies.site_obs_site_return sos = null;

        BNGGrammar_BNGGrammar_SeedSpecies.site_obs_site_return sos2 = null;

        BNGGrammar.label_return label6 = null;



          ((site_obs_scope)site_obs_stack.peek()).defaultState = null;
          ((site_obs_scope)site_obs_stack.peek()).bondIndicator = "0";
          
        try {
            // BNGGrammar_SeedSpecies.g:218:1: (s1= STRING ( ( (sos= site_obs_site[bonds,upperID] )? ( site_obs_state )? ) | site_obs_state sos2= site_obs_site[bonds,upperID] ) ( label )? -> component_def(id=upperIDname=$s1.textstate=$site_obs::defaultStatenumberOfBonds=$site_obs::bondIndicator))
            // BNGGrammar_SeedSpecies.g:219:3: s1= STRING ( ( (sos= site_obs_site[bonds,upperID] )? ( site_obs_state )? ) | site_obs_state sos2= site_obs_site[bonds,upperID] ) ( label )?
            {
            s1=(Token)match(input,STRING,FOLLOW_STRING_in_site_obs594); 
            // BNGGrammar_SeedSpecies.g:220:3: ( ( (sos= site_obs_site[bonds,upperID] )? ( site_obs_state )? ) | site_obs_state sos2= site_obs_site[bonds,upperID] )
            int alt22=2;
            alt22 = dfa22.predict(input);
            switch (alt22) {
                case 1 :
                    // BNGGrammar_SeedSpecies.g:221:3: ( (sos= site_obs_site[bonds,upperID] )? ( site_obs_state )? )
                    {
                    // BNGGrammar_SeedSpecies.g:221:3: ( (sos= site_obs_site[bonds,upperID] )? ( site_obs_state )? )
                    // BNGGrammar_SeedSpecies.g:221:4: (sos= site_obs_site[bonds,upperID] )? ( site_obs_state )?
                    {
                    // BNGGrammar_SeedSpecies.g:221:4: (sos= site_obs_site[bonds,upperID] )?
                    int alt20=2;
                    int LA20_0 = input.LA(1);

                    if ( (LA20_0==EMARK) ) {
                        alt20=1;
                    }
                    switch (alt20) {
                        case 1 :
                            // BNGGrammar_SeedSpecies.g:221:5: sos= site_obs_site[bonds,upperID]
                            {
                            pushFollow(FOLLOW_site_obs_site_in_site_obs607);
                            sos=site_obs_site(bonds, upperID);

                            state._fsp--;

                            ((site_obs_scope)site_obs_stack.peek()).bondIndicator = (sos!=null?sos.bond:null);

                            }
                            break;

                    }

                    // BNGGrammar_SeedSpecies.g:221:79: ( site_obs_state )?
                    int alt21=2;
                    int LA21_0 = input.LA(1);

                    if ( (LA21_0==TILDE) ) {
                        alt21=1;
                    }
                    switch (alt21) {
                        case 1 :
                            // BNGGrammar_SeedSpecies.g:221:79: site_obs_state
                            {
                            pushFollow(FOLLOW_site_obs_state_in_site_obs613);
                            site_obs_state();

                            state._fsp--;


                            }
                            break;

                    }


                    }


                    }
                    break;
                case 2 :
                    // BNGGrammar_SeedSpecies.g:222:5: site_obs_state sos2= site_obs_site[bonds,upperID]
                    {
                    pushFollow(FOLLOW_site_obs_state_in_site_obs621);
                    site_obs_state();

                    state._fsp--;

                    pushFollow(FOLLOW_site_obs_site_in_site_obs625);
                    sos2=site_obs_site(bonds, upperID);

                    state._fsp--;

                    ((site_obs_scope)site_obs_stack.peek()).bondIndicator = (sos2!=null?sos2.bond:null);

                    }
                    break;

            }

            // BNGGrammar_SeedSpecies.g:224:8: ( label )?
            int alt23=2;
            int LA23_0 = input.LA(1);

            if ( (LA23_0==MOD) ) {
                alt23=1;
            }
            switch (alt23) {
                case 1 :
                    // BNGGrammar_SeedSpecies.g:224:9: label
                    {
                    pushFollow(FOLLOW_label_in_site_obs640);
                    label6=gBNGGrammar.label();

                    state._fsp--;

                    retval.myLabel = (label6!=null?input.toString(label6.start,label6.stop):null);

                    }
                    break;

            }



            // TEMPLATE REWRITE
            // 225:3: -> component_def(id=upperIDname=$s1.textstate=$site_obs::defaultStatenumberOfBonds=$site_obs::bondIndicator)
            {
                retval.st = templateLib.getInstanceOf("component_def",
              new STAttrMap().put("id", upperID).put("name", (s1!=null?s1.getText():null)).put("state", ((site_obs_scope)site_obs_stack.peek()).defaultState).put("numberOfBonds", ((site_obs_scope)site_obs_stack.peek()).bondIndicator));
            }


            }

            retval.stop = input.LT(-1);

        }
        catch (RecognitionException re) {
            reportError(re);
            recover(input,re);
        }
        finally {
            site_obs_stack.pop();
        }
        return retval;
    }
    // $ANTLR end "site_obs"

    public static class site_obs_state_return extends ParserRuleReturnScope {
        public StringTemplate st;
        public Object getTemplate() { return st; }
        public String toString() { return st==null?null:st.toString(); }
    };

    // $ANTLR start "site_obs_state"
    // BNGGrammar_SeedSpecies.g:227:1: site_obs_state : ( TILDE state_label ) ;
    public final BNGGrammar_BNGGrammar_SeedSpecies.site_obs_state_return site_obs_state() throws RecognitionException {
        BNGGrammar_BNGGrammar_SeedSpecies.site_obs_state_return retval = new BNGGrammar_BNGGrammar_SeedSpecies.site_obs_state_return();
        retval.start = input.LT(1);

        BNGGrammar_BNGGrammar_MoleculeDef.state_label_return state_label7 = null;


        try {
            // BNGGrammar_SeedSpecies.g:227:15: ( ( TILDE state_label ) )
            // BNGGrammar_SeedSpecies.g:227:17: ( TILDE state_label )
            {
            // BNGGrammar_SeedSpecies.g:227:17: ( TILDE state_label )
            // BNGGrammar_SeedSpecies.g:227:18: TILDE state_label
            {
            match(input,TILDE,FOLLOW_TILDE_in_site_obs_state676); 
            pushFollow(FOLLOW_state_label_in_site_obs_state678);
            state_label7=gBNGGrammar.state_label();

            state._fsp--;

            ((site_obs_scope)site_obs_stack.peek()).defaultState = (state_label7!=null?input.toString(state_label7.start,state_label7.stop):null);

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
    // $ANTLR end "site_obs_state"

    protected static class site_obs_site_scope {
        int acc;
        String bondIndicator;
    }
    protected Stack site_obs_site_stack = new Stack();

    public static class site_obs_site_return extends ParserRuleReturnScope {
        public String bond;
        public StringTemplate st;
        public Object getTemplate() { return st; }
        public String toString() { return st==null?null:st.toString(); }
    };

    // $ANTLR start "site_obs_site"
    // BNGGrammar_SeedSpecies.g:229:1: site_obs_site[BondList bonds,String upperID] returns [String bond] : ( EMARK ( ( PLUS ) | ( QMARK ) | s1= bond_name ) )+ ;
    public final BNGGrammar_BNGGrammar_SeedSpecies.site_obs_site_return site_obs_site(BondList bonds, String upperID) throws RecognitionException {
        site_obs_site_stack.push(new site_obs_site_scope());
        BNGGrammar_BNGGrammar_SeedSpecies.site_obs_site_return retval = new BNGGrammar_BNGGrammar_SeedSpecies.site_obs_site_return();
        retval.start = input.LT(1);

        Token PLUS8=null;
        Token QMARK9=null;
        BNGGrammar_BNGGrammar_SeedSpecies.bond_name_return s1 = null;



          ((site_obs_site_scope)site_obs_site_stack.peek()).acc = 0;
          ((site_obs_site_scope)site_obs_site_stack.peek()).bondIndicator = "";
          
        try {
            // BNGGrammar_SeedSpecies.g:238:3: ( ( EMARK ( ( PLUS ) | ( QMARK ) | s1= bond_name ) )+ )
            // BNGGrammar_SeedSpecies.g:239:9: ( EMARK ( ( PLUS ) | ( QMARK ) | s1= bond_name ) )+
            {
            // BNGGrammar_SeedSpecies.g:239:9: ( EMARK ( ( PLUS ) | ( QMARK ) | s1= bond_name ) )+
            int cnt25=0;
            loop25:
            do {
                int alt25=2;
                int LA25_0 = input.LA(1);

                if ( (LA25_0==EMARK) ) {
                    alt25=1;
                }


                switch (alt25) {
            	case 1 :
            	    // BNGGrammar_SeedSpecies.g:239:10: EMARK ( ( PLUS ) | ( QMARK ) | s1= bond_name )
            	    {
            	    match(input,EMARK,FOLLOW_EMARK_in_site_obs_site712); 
            	    // BNGGrammar_SeedSpecies.g:239:16: ( ( PLUS ) | ( QMARK ) | s1= bond_name )
            	    int alt24=3;
            	    switch ( input.LA(1) ) {
            	    case PLUS:
            	        {
            	        alt24=1;
            	        }
            	        break;
            	    case QMARK:
            	        {
            	        alt24=2;
            	        }
            	        break;
            	    case INT:
            	    case STRING:
            	        {
            	        alt24=3;
            	        }
            	        break;
            	    default:
            	        NoViableAltException nvae =
            	            new NoViableAltException("", 24, 0, input);

            	        throw nvae;
            	    }

            	    switch (alt24) {
            	        case 1 :
            	            // BNGGrammar_SeedSpecies.g:239:17: ( PLUS )
            	            {
            	            // BNGGrammar_SeedSpecies.g:239:17: ( PLUS )
            	            // BNGGrammar_SeedSpecies.g:239:18: PLUS
            	            {
            	            PLUS8=(Token)match(input,PLUS,FOLLOW_PLUS_in_site_obs_site716); 
            	            ((site_obs_site_scope)site_obs_site_stack.peek()).bondIndicator += (PLUS8!=null?PLUS8.getText():null);

            	            }


            	            }
            	            break;
            	        case 2 :
            	            // BNGGrammar_SeedSpecies.g:240:11: ( QMARK )
            	            {
            	            // BNGGrammar_SeedSpecies.g:240:11: ( QMARK )
            	            // BNGGrammar_SeedSpecies.g:240:12: QMARK
            	            {
            	            QMARK9=(Token)match(input,QMARK,FOLLOW_QMARK_in_site_obs_site732); 
            	            ((site_obs_site_scope)site_obs_site_stack.peek()).bondIndicator += (QMARK9!=null?QMARK9.getText():null);

            	            }


            	            }
            	            break;
            	        case 3 :
            	            // BNGGrammar_SeedSpecies.g:241:11: s1= bond_name
            	            {
            	            pushFollow(FOLLOW_bond_name_in_site_obs_site749);
            	            s1=bond_name();

            	            state._fsp--;


            	                            bonds.addElement((s1!=null?input.toString(s1.start,s1.stop):null),upperID,((species_element_scope)species_element_stack.peek()).lname);  
            	                            ((site_obs_site_scope)site_obs_site_stack.peek()).acc++;
            	                            

            	            }
            	            break;

            	    }


            	    }
            	    break;

            	default :
            	    if ( cnt25 >= 1 ) break loop25;
                        EarlyExitException eee =
                            new EarlyExitException(25, input);
                        throw eee;
                }
                cnt25++;
            } while (true);


                       if(((site_obs_site_scope)site_obs_site_stack.peek()).acc >0)
                        retval.bond = ((site_obs_site_scope)site_obs_site_stack.peek()).acc + ((site_obs_site_scope)site_obs_site_stack.peek()).bondIndicator;
                       else if(!((site_obs_site_scope)site_obs_site_stack.peek()).bondIndicator.equals(""))
                        retval.bond = ((site_obs_site_scope)site_obs_site_stack.peek()).bondIndicator;
                       else 
                         retval.bond = "0";
                    

            }

            retval.stop = input.LT(-1);

        }
        catch (RecognitionException re) {
            reportError(re);
            recover(input,re);
        }
        finally {
            site_obs_site_stack.pop();
        }
        return retval;
    }
    // $ANTLR end "site_obs_site"

    public static class bond_name_return extends ParserRuleReturnScope {
        public StringTemplate st;
        public Object getTemplate() { return st; }
        public String toString() { return st==null?null:st.toString(); }
    };

    // $ANTLR start "bond_name"
    // BNGGrammar_SeedSpecies.g:256:1: bond_name : ( INT | STRING );
    public final BNGGrammar_BNGGrammar_SeedSpecies.bond_name_return bond_name() throws RecognitionException {
        BNGGrammar_BNGGrammar_SeedSpecies.bond_name_return retval = new BNGGrammar_BNGGrammar_SeedSpecies.bond_name_return();
        retval.start = input.LT(1);

        try {
            // BNGGrammar_SeedSpecies.g:256:10: ( INT | STRING )
            // BNGGrammar_SeedSpecies.g:
            {
            if ( input.LA(1)==INT||input.LA(1)==STRING ) {
                input.consume();
                state.errorRecovery=false;
            }
            else {
                MismatchedSetException mse = new MismatchedSetException(null,input);
                throw mse;
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
    // $ANTLR end "bond_name"

    // Delegated rules


    protected DFA22 dfa22 = new DFA22(this);
    static final String DFA22_eotS =
        "\5\uffff";
    static final String DFA22_eofS =
        "\5\uffff";
    static final String DFA22_minS =
        "\1\172\1\uffff\2\161\1\uffff";
    static final String DFA22_maxS =
        "\1\u0092\1\uffff\1\u0091\1\u0092\1\uffff";
    static final String DFA22_acceptS =
        "\1\uffff\1\1\2\uffff\1\2";
    static final String DFA22_specialS =
        "\5\uffff}>";
    static final String[] DFA22_transitionS = {
            "\1\1\2\uffff\1\1\3\uffff\1\2\15\uffff\1\1\2\uffff\1\1",
            "",
            "\1\3\1\uffff\1\3\35\uffff\1\3",
            "\1\3\1\uffff\1\3\6\uffff\1\1\2\uffff\1\1\21\uffff\1\1\1\uffff"+
            "\1\3\1\4",
            ""
    };

    static final short[] DFA22_eot = DFA.unpackEncodedString(DFA22_eotS);
    static final short[] DFA22_eof = DFA.unpackEncodedString(DFA22_eofS);
    static final char[] DFA22_min = DFA.unpackEncodedStringToUnsignedChars(DFA22_minS);
    static final char[] DFA22_max = DFA.unpackEncodedStringToUnsignedChars(DFA22_maxS);
    static final short[] DFA22_accept = DFA.unpackEncodedString(DFA22_acceptS);
    static final short[] DFA22_special = DFA.unpackEncodedString(DFA22_specialS);
    static final short[][] DFA22_transition;

    static {
        int numStates = DFA22_transitionS.length;
        DFA22_transition = new short[numStates][];
        for (int i=0; i<numStates; i++) {
            DFA22_transition[i] = DFA.unpackEncodedString(DFA22_transitionS[i]);
        }
    }

    class DFA22 extends DFA {

        public DFA22(BaseRecognizer recognizer) {
            this.recognizer = recognizer;
            this.decisionNumber = 22;
            this.eot = DFA22_eot;
            this.eof = DFA22_eof;
            this.min = DFA22_min;
            this.max = DFA22_max;
            this.accept = DFA22_accept;
            this.special = DFA22_special;
            this.transition = DFA22_transition;
        }
        public String getDescription() {
            return "220:3: ( ( (sos= site_obs_site[bonds,upperID] )? ( site_obs_state )? ) | site_obs_state sos2= site_obs_site[bonds,upperID] )";
        }
    }
 

    public static final BitSet FOLLOW_BEGIN_in_seed_species_block53 = new BitSet(new long[]{0x0000000000000C00L});
    public static final BitSet FOLLOW_SEED_in_seed_species_block56 = new BitSet(new long[]{0x0000000000000800L});
    public static final BitSet FOLLOW_SPECIES_in_seed_species_block60 = new BitSet(new long[]{0x0000000000000020L});
    public static final BitSet FOLLOW_LB_in_seed_species_block62 = new BitSet(new long[]{0x0000000000000120L,0x000A000000000000L,0x0000000000008005L});
    public static final BitSet FOLLOW_seed_species_def_in_seed_species_block66 = new BitSet(new long[]{0x0000000000000020L});
    public static final BitSet FOLLOW_LB_in_seed_species_block70 = new BitSet(new long[]{0x0000000000000120L,0x000A000000000000L,0x0000000000008005L});
    public static final BitSet FOLLOW_END_in_seed_species_block76 = new BitSet(new long[]{0x0000000000000C00L});
    public static final BitSet FOLLOW_SEED_in_seed_species_block79 = new BitSet(new long[]{0x0000000000000800L});
    public static final BitSet FOLLOW_SPECIES_in_seed_species_block83 = new BitSet(new long[]{0x0000000000000020L});
    public static final BitSet FOLLOW_LB_in_seed_species_block85 = new BitSet(new long[]{0x0000000000000022L});
    public static final BitSet FOLLOW_STRING_in_seed_species_def104 = new BitSet(new long[]{0x0000000000000000L,0x0020000000000000L});
    public static final BitSet FOLLOW_COLON_in_seed_species_def106 = new BitSet(new long[]{0x0000000000000000L,0x000A000000000000L,0x0000000000008005L});
    public static final BitSet FOLLOW_pre_species_def_in_seed_species_def110 = new BitSet(new long[]{0x0000000000000040L});
    public static final BitSet FOLLOW_WS_in_seed_species_def122 = new BitSet(new long[]{0x0004002000000000L,0x100B000180000000L,0x0000000000001000L});
    public static final BitSet FOLLOW_expression_in_seed_species_def130 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_INT_in_pre_species_def194 = new BitSet(new long[]{0x0000000000000000L,0x000A000000000000L,0x0000000000008005L});
    public static final BitSet FOLLOW_species_def_in_pre_species_def201 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_AT_in_species_def249 = new BitSet(new long[]{0x0000000000000000L,0x0008000000000000L});
    public static final BitSet FOLLOW_STRING_in_species_def253 = new BitSet(new long[]{0x0000000000000000L,0x0020000000000000L});
    public static final BitSet FOLLOW_COLON_in_species_def268 = new BitSet(new long[]{0x0000000000000000L,0x002A000000000000L,0x0000000000008005L});
    public static final BitSet FOLLOW_COLON_in_species_def273 = new BitSet(new long[]{0x0000000000000000L,0x000A000000000000L,0x0000000000008005L});
    public static final BitSet FOLLOW_MOD_in_species_def291 = new BitSet(new long[]{0x0000000000000000L,0x0008000000000000L});
    public static final BitSet FOLLOW_STRING_in_species_def295 = new BitSet(new long[]{0x0000000000000000L,0x0020000000000000L});
    public static final BitSet FOLLOW_COLON_in_species_def297 = new BitSet(new long[]{0x0000000000000000L,0x002A000000000000L,0x0000000000008005L});
    public static final BitSet FOLLOW_COLON_in_species_def299 = new BitSet(new long[]{0x0000000000000000L,0x000A000000000000L,0x0000000000008005L});
    public static final BitSet FOLLOW_DOLLAR_in_species_def318 = new BitSet(new long[]{0x0000000000000000L,0x000A000000000000L,0x0000000000008005L});
    public static final BitSet FOLLOW_species_element_in_species_def334 = new BitSet(new long[]{0x0000000000000002L,0x0800000000000000L});
    public static final BitSet FOLLOW_DOT_in_species_def346 = new BitSet(new long[]{0x0000000000000000L,0x000A000000000000L,0x0000000000008005L});
    public static final BitSet FOLLOW_species_element_in_species_def350 = new BitSet(new long[]{0x0000000000000002L,0x0800000000000000L});
    public static final BitSet FOLLOW_STRING_in_species_element454 = new BitSet(new long[]{0x0000000000000002L,0x1000000000000000L,0x0000000000008004L});
    public static final BitSet FOLLOW_label_in_species_element459 = new BitSet(new long[]{0x0000000000000002L,0x1000000000000000L,0x0000000000000004L});
    public static final BitSet FOLLOW_LPAREN_in_species_element469 = new BitSet(new long[]{0x0000000000000000L,0x2008000000000000L});
    public static final BitSet FOLLOW_site_list_in_species_element471 = new BitSet(new long[]{0x0000000000000000L,0x2000000000000000L});
    public static final BitSet FOLLOW_RPAREN_in_species_element474 = new BitSet(new long[]{0x0000000000000002L,0x0000000000000000L,0x0000000000000004L});
    public static final BitSet FOLLOW_AT_in_species_element481 = new BitSet(new long[]{0x0000000000000000L,0x0008000000000000L});
    public static final BitSet FOLLOW_STRING_in_species_element485 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_site_obs_in_site_list542 = new BitSet(new long[]{0x0000000000000002L,0x0400000000000000L});
    public static final BitSet FOLLOW_COMMA_in_site_list551 = new BitSet(new long[]{0x0000000000000000L,0x0008000000000000L});
    public static final BitSet FOLLOW_site_obs_in_site_list555 = new BitSet(new long[]{0x0000000000000002L,0x0400000000000000L});
    public static final BitSet FOLLOW_STRING_in_site_obs594 = new BitSet(new long[]{0x0000000000000002L,0x0000000000000000L,0x0000000000048002L});
    public static final BitSet FOLLOW_site_obs_site_in_site_obs607 = new BitSet(new long[]{0x0000000000000002L,0x0000000000000000L,0x0000000000008002L});
    public static final BitSet FOLLOW_site_obs_state_in_site_obs613 = new BitSet(new long[]{0x0000000000000002L,0x0000000000000000L,0x0000000000008000L});
    public static final BitSet FOLLOW_site_obs_state_in_site_obs621 = new BitSet(new long[]{0x0000000000000000L,0x0000000000000000L,0x0000000000040000L});
    public static final BitSet FOLLOW_site_obs_site_in_site_obs625 = new BitSet(new long[]{0x0000000000000002L,0x0000000000000000L,0x0000000000008000L});
    public static final BitSet FOLLOW_label_in_site_obs640 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_TILDE_in_site_obs_state676 = new BitSet(new long[]{0x0000000000000000L,0x000A000000000000L,0x0000000000020000L});
    public static final BitSet FOLLOW_state_label_in_site_obs_state678 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_EMARK_in_site_obs_site712 = new BitSet(new long[]{0x0000000000000000L,0x000A000000000000L,0x0000000000022000L});
    public static final BitSet FOLLOW_PLUS_in_site_obs_site716 = new BitSet(new long[]{0x0000000000000002L,0x0000000000000000L,0x0000000000040000L});
    public static final BitSet FOLLOW_QMARK_in_site_obs_site732 = new BitSet(new long[]{0x0000000000000002L,0x0000000000000000L,0x0000000000040000L});
    public static final BitSet FOLLOW_bond_name_in_site_obs_site749 = new BitSet(new long[]{0x0000000000000002L,0x0000000000000000L,0x0000000000040000L});
    public static final BitSet FOLLOW_set_in_bond_name0 = new BitSet(new long[]{0x0000000000000002L});

}