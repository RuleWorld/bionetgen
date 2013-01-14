// $ANTLR 3.3 Nov 30, 2010 12:50:56 BNGGrammar_ReactionRules.g 2013-01-14 14:49:39

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
import java.util.Map;
import java.util.HashMap;
import org.antlr.stringtemplate.*;
import org.antlr.stringtemplate.language.*;
import java.util.HashMap;
public class BNGGrammar_BNGGrammar_ReactionRules extends Parser {
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


        public BNGGrammar_BNGGrammar_ReactionRules(TokenStream input, BNGGrammar gBNGGrammar) {
            this(input, new RecognizerSharedState(), gBNGGrammar);
        }
        public BNGGrammar_BNGGrammar_ReactionRules(TokenStream input, RecognizerSharedState state, BNGGrammar gBNGGrammar) {
            super(input, state);
            this.gBNGGrammar = gBNGGrammar;
             
            gParent = gBNGGrammar;
        }
        
    protected StringTemplateGroup templateLib =
      new StringTemplateGroup("BNGGrammar_BNGGrammar_ReactionRulesTemplates", AngleBracketTemplateLexer.class);

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
    public String getGrammarFileName() { return "BNGGrammar_ReactionRules.g"; }


     public void getParentTemplate(){
     
      this.setTemplateLib(gParent.getTemplateLib());
     }
          @Override
      public String getErrorMessage(RecognitionException e,String[] tokenNames){
        return gParent.getErrorMessage(e,tokenNames);
      }
      
     
      public String getErrorMessage2(Token s1,String error){
          return String.format("%s line %d:%d %s: %s\n",input.getSourceName(),s1.getLine(),s1.getCharPositionInLine(),error,s1.getText());
      
      }
      



    public static class population_maps_block_return extends ParserRuleReturnScope {
        public StringTemplate st;
        public Object getTemplate() { return st; }
        public String toString() { return st==null?null:st.toString(); }
    };

    // $ANTLR start "population_maps_block"
    // BNGGrammar_ReactionRules.g:27:1: population_maps_block : BEGIN POPULATION MAPS ( LB )+ ( reaction_rule_def[1] ( LB )+ )* END POPULATION MAPS ( LB )+ ;
    public final BNGGrammar_BNGGrammar_ReactionRules.population_maps_block_return population_maps_block() throws RecognitionException {
        BNGGrammar_BNGGrammar_ReactionRules.population_maps_block_return retval = new BNGGrammar_BNGGrammar_ReactionRules.population_maps_block_return();
        retval.start = input.LT(1);

        try {
            // BNGGrammar_ReactionRules.g:27:22: ( BEGIN POPULATION MAPS ( LB )+ ( reaction_rule_def[1] ( LB )+ )* END POPULATION MAPS ( LB )+ )
            // BNGGrammar_ReactionRules.g:28:1: BEGIN POPULATION MAPS ( LB )+ ( reaction_rule_def[1] ( LB )+ )* END POPULATION MAPS ( LB )+
            {
            match(input,BEGIN,FOLLOW_BEGIN_in_population_maps_block39); if (state.failed) return retval;
            match(input,POPULATION,FOLLOW_POPULATION_in_population_maps_block41); if (state.failed) return retval;
            match(input,MAPS,FOLLOW_MAPS_in_population_maps_block43); if (state.failed) return retval;
            // BNGGrammar_ReactionRules.g:28:23: ( LB )+
            int cnt1=0;
            loop1:
            do {
                int alt1=2;
                int LA1_0 = input.LA(1);

                if ( (LA1_0==LB) ) {
                    alt1=1;
                }


                switch (alt1) {
            	case 1 :
            	    // BNGGrammar_ReactionRules.g:28:23: LB
            	    {
            	    match(input,LB,FOLLOW_LB_in_population_maps_block45); if (state.failed) return retval;

            	    }
            	    break;

            	default :
            	    if ( cnt1 >= 1 ) break loop1;
            	    if (state.backtracking>0) {state.failed=true; return retval;}
                        EarlyExitException eee =
                            new EarlyExitException(1, input);
                        throw eee;
                }
                cnt1++;
            } while (true);

            // BNGGrammar_ReactionRules.g:29:3: ( reaction_rule_def[1] ( LB )+ )*
            loop3:
            do {
                int alt3=2;
                int LA3_0 = input.LA(1);

                if ( (LA3_0==INT||LA3_0==STRING||LA3_0==LBRACKET||LA3_0==DOLLAR||LA3_0==AT||LA3_0==MOD) ) {
                    alt3=1;
                }


                switch (alt3) {
            	case 1 :
            	    // BNGGrammar_ReactionRules.g:29:4: reaction_rule_def[1] ( LB )+
            	    {
            	    pushFollow(FOLLOW_reaction_rule_def_in_population_maps_block51);
            	    reaction_rule_def(1);

            	    state._fsp--;
            	    if (state.failed) return retval;
            	    // BNGGrammar_ReactionRules.g:29:25: ( LB )+
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
            	    	    // BNGGrammar_ReactionRules.g:29:25: LB
            	    	    {
            	    	    match(input,LB,FOLLOW_LB_in_population_maps_block54); if (state.failed) return retval;

            	    	    }
            	    	    break;

            	    	default :
            	    	    if ( cnt2 >= 1 ) break loop2;
            	    	    if (state.backtracking>0) {state.failed=true; return retval;}
            	                EarlyExitException eee =
            	                    new EarlyExitException(2, input);
            	                throw eee;
            	        }
            	        cnt2++;
            	    } while (true);


            	    }
            	    break;

            	default :
            	    break loop3;
                }
            } while (true);

            match(input,END,FOLLOW_END_in_population_maps_block60); if (state.failed) return retval;
            match(input,POPULATION,FOLLOW_POPULATION_in_population_maps_block62); if (state.failed) return retval;
            match(input,MAPS,FOLLOW_MAPS_in_population_maps_block64); if (state.failed) return retval;
            // BNGGrammar_ReactionRules.g:30:21: ( LB )+
            int cnt4=0;
            loop4:
            do {
                int alt4=2;
                int LA4_0 = input.LA(1);

                if ( (LA4_0==LB) ) {
                    alt4=1;
                }


                switch (alt4) {
            	case 1 :
            	    // BNGGrammar_ReactionRules.g:30:21: LB
            	    {
            	    match(input,LB,FOLLOW_LB_in_population_maps_block66); if (state.failed) return retval;

            	    }
            	    break;

            	default :
            	    if ( cnt4 >= 1 ) break loop4;
            	    if (state.backtracking>0) {state.failed=true; return retval;}
                        EarlyExitException eee =
                            new EarlyExitException(4, input);
                        throw eee;
                }
                cnt4++;
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
    // $ANTLR end "population_maps_block"

    protected static class reaction_rules_block_scope {
        int reactionCounter;
    }
    protected Stack reaction_rules_block_stack = new Stack();

    public static class reaction_rules_block_return extends ParserRuleReturnScope {
        public StringTemplate st;
        public Object getTemplate() { return st; }
        public String toString() { return st==null?null:st.toString(); }
    };

    // $ANTLR start "reaction_rules_block"
    // BNGGrammar_ReactionRules.g:37:1: reaction_rules_block[List reactionRules] : BEGIN REACTION RULES ( LB )+ ( reaction_rule_def[$reaction_rules_block::reactionCounter] ( LB )+ )* END REACTION RULES ( LB )* ;
    public final BNGGrammar_BNGGrammar_ReactionRules.reaction_rules_block_return reaction_rules_block(List reactionRules) throws RecognitionException {
        reaction_rules_block_stack.push(new reaction_rules_block_scope());
        BNGGrammar_BNGGrammar_ReactionRules.reaction_rules_block_return retval = new BNGGrammar_BNGGrammar_ReactionRules.reaction_rules_block_return();
        retval.start = input.LT(1);

        BNGGrammar_BNGGrammar_ReactionRules.reaction_rule_def_return reaction_rule_def1 = null;



        getParentTemplate();
          ((reaction_rules_block_scope)reaction_rules_block_stack.peek()).reactionCounter = 1;
          gParent.paraphrases.push("in reaction rules block"); 

        try {
            // BNGGrammar_ReactionRules.g:51:9: ( BEGIN REACTION RULES ( LB )+ ( reaction_rule_def[$reaction_rules_block::reactionCounter] ( LB )+ )* END REACTION RULES ( LB )* )
            // BNGGrammar_ReactionRules.g:51:11: BEGIN REACTION RULES ( LB )+ ( reaction_rule_def[$reaction_rules_block::reactionCounter] ( LB )+ )* END REACTION RULES ( LB )*
            {
            match(input,BEGIN,FOLLOW_BEGIN_in_reaction_rules_block100); if (state.failed) return retval;
            match(input,REACTION,FOLLOW_REACTION_in_reaction_rules_block102); if (state.failed) return retval;
            match(input,RULES,FOLLOW_RULES_in_reaction_rules_block104); if (state.failed) return retval;
            // BNGGrammar_ReactionRules.g:51:32: ( LB )+
            int cnt5=0;
            loop5:
            do {
                int alt5=2;
                int LA5_0 = input.LA(1);

                if ( (LA5_0==LB) ) {
                    alt5=1;
                }


                switch (alt5) {
            	case 1 :
            	    // BNGGrammar_ReactionRules.g:51:32: LB
            	    {
            	    match(input,LB,FOLLOW_LB_in_reaction_rules_block106); if (state.failed) return retval;

            	    }
            	    break;

            	default :
            	    if ( cnt5 >= 1 ) break loop5;
            	    if (state.backtracking>0) {state.failed=true; return retval;}
                        EarlyExitException eee =
                            new EarlyExitException(5, input);
                        throw eee;
                }
                cnt5++;
            } while (true);

            // BNGGrammar_ReactionRules.g:51:35: ( reaction_rule_def[$reaction_rules_block::reactionCounter] ( LB )+ )*
            loop7:
            do {
                int alt7=2;
                int LA7_0 = input.LA(1);

                if ( (LA7_0==INT||LA7_0==STRING||LA7_0==LBRACKET||LA7_0==DOLLAR||LA7_0==AT||LA7_0==MOD) ) {
                    alt7=1;
                }


                switch (alt7) {
            	case 1 :
            	    // BNGGrammar_ReactionRules.g:51:36: reaction_rule_def[$reaction_rules_block::reactionCounter] ( LB )+
            	    {
            	    pushFollow(FOLLOW_reaction_rule_def_in_reaction_rules_block109);
            	    reaction_rule_def1=reaction_rule_def(((reaction_rules_block_scope)reaction_rules_block_stack.peek()).reactionCounter);

            	    state._fsp--;
            	    if (state.failed) return retval;
            	    if ( state.backtracking==0 ) {

            	                        reactionRules.add((reaction_rule_def1!=null?reaction_rule_def1.st:null));
            	                        StringTemplate sInvert = null;
            	                        //TODO: crashes when handling an error in a bidirectional reaction
            	                        if((reaction_rule_def1!=null?reaction_rule_def1.numReactions:0) == 2)
            	                          sInvert = InvertBidirectional.invert((reaction_rule_def1!=null?reaction_rule_def1.st:null).toString(),((reaction_rules_block_scope)reaction_rules_block_stack.peek()).reactionCounter+1);
            	                        reactionRules.add(sInvert);
            	                        ((reaction_rules_block_scope)reaction_rules_block_stack.peek()).reactionCounter+= (reaction_rule_def1!=null?reaction_rule_def1.numReactions:0);
            	                        
            	                        
            	    }
            	    // BNGGrammar_ReactionRules.g:61:20: ( LB )+
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
            	    	    // BNGGrammar_ReactionRules.g:61:20: LB
            	    	    {
            	    	    match(input,LB,FOLLOW_LB_in_reaction_rules_block132); if (state.failed) return retval;

            	    	    }
            	    	    break;

            	    	default :
            	    	    if ( cnt6 >= 1 ) break loop6;
            	    	    if (state.backtracking>0) {state.failed=true; return retval;}
            	                EarlyExitException eee =
            	                    new EarlyExitException(6, input);
            	                throw eee;
            	        }
            	        cnt6++;
            	    } while (true);


            	    }
            	    break;

            	default :
            	    break loop7;
                }
            } while (true);

            match(input,END,FOLLOW_END_in_reaction_rules_block137); if (state.failed) return retval;
            match(input,REACTION,FOLLOW_REACTION_in_reaction_rules_block139); if (state.failed) return retval;
            match(input,RULES,FOLLOW_RULES_in_reaction_rules_block141); if (state.failed) return retval;
            // BNGGrammar_ReactionRules.g:61:45: ( LB )*
            loop8:
            do {
                int alt8=2;
                int LA8_0 = input.LA(1);

                if ( (LA8_0==LB) ) {
                    alt8=1;
                }


                switch (alt8) {
            	case 1 :
            	    // BNGGrammar_ReactionRules.g:61:45: LB
            	    {
            	    match(input,LB,FOLLOW_LB_in_reaction_rules_block143); if (state.failed) return retval;

            	    }
            	    break;

            	default :
            	    break loop8;
                }
            } while (true);


            }

            retval.stop = input.LT(-1);

            if ( state.backtracking==0 ) {

                gParent.paraphrases.pop();

            }
        }
        catch (RecognitionException re) {
            reportError(re);
            recover(input,re);
        }
        finally {
            reaction_rules_block_stack.pop();
        }
        return retval;
    }
    // $ANTLR end "reaction_rules_block"

    public static class reactionLabel_return extends ParserRuleReturnScope {
        public String label;
        public StringTemplate st;
        public Object getTemplate() { return st; }
        public String toString() { return st==null?null:st.toString(); }
    };

    // $ANTLR start "reactionLabel"
    // BNGGrammar_ReactionRules.g:66:1: reactionLabel returns [String label] : ( STRING | INT )+ COLON ;
    public final BNGGrammar_BNGGrammar_ReactionRules.reactionLabel_return reactionLabel() throws RecognitionException {
        BNGGrammar_BNGGrammar_ReactionRules.reactionLabel_return retval = new BNGGrammar_BNGGrammar_ReactionRules.reactionLabel_return();
        retval.start = input.LT(1);

        Token STRING2=null;
        Token INT3=null;


          retval.label = "";
          
        try {
            // BNGGrammar_ReactionRules.g:70:1: ( ( STRING | INT )+ COLON )
            // BNGGrammar_ReactionRules.g:71:1: ( STRING | INT )+ COLON
            {
            // BNGGrammar_ReactionRules.g:71:1: ( STRING | INT )+
            int cnt9=0;
            loop9:
            do {
                int alt9=3;
                int LA9_0 = input.LA(1);

                if ( (LA9_0==STRING) ) {
                    alt9=1;
                }
                else if ( (LA9_0==INT) ) {
                    alt9=2;
                }


                switch (alt9) {
            	case 1 :
            	    // BNGGrammar_ReactionRules.g:71:2: STRING
            	    {
            	    STRING2=(Token)match(input,STRING,FOLLOW_STRING_in_reactionLabel172); if (state.failed) return retval;
            	    if ( state.backtracking==0 ) {
            	      retval.label += (STRING2!=null?STRING2.getText():null) + " ";
            	    }

            	    }
            	    break;
            	case 2 :
            	    // BNGGrammar_ReactionRules.g:72:12: INT
            	    {
            	    INT3=(Token)match(input,INT,FOLLOW_INT_in_reactionLabel187); if (state.failed) return retval;
            	    if ( state.backtracking==0 ) {
            	      retval.label += (INT3!=null?INT3.getText():null) + " ";
            	    }

            	    }
            	    break;

            	default :
            	    if ( cnt9 >= 1 ) break loop9;
            	    if (state.backtracking>0) {state.failed=true; return retval;}
                        EarlyExitException eee =
                            new EarlyExitException(9, input);
                        throw eee;
                }
                cnt9++;
            } while (true);

            match(input,COLON,FOLLOW_COLON_in_reactionLabel200); if (state.failed) return retval;

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
    // $ANTLR end "reactionLabel"

    public static class reactionReference_return extends ParserRuleReturnScope {
        public StringTemplate st;
        public Object getTemplate() { return st; }
        public String toString() { return st==null?null:st.toString(); }
    };

    // $ANTLR start "reactionReference"
    // BNGGrammar_ReactionRules.g:77:1: reactionReference : STRING COLON COLON ;
    public final BNGGrammar_BNGGrammar_ReactionRules.reactionReference_return reactionReference() throws RecognitionException {
        BNGGrammar_BNGGrammar_ReactionRules.reactionReference_return retval = new BNGGrammar_BNGGrammar_ReactionRules.reactionReference_return();
        retval.start = input.LT(1);

        try {
            // BNGGrammar_ReactionRules.g:77:18: ( STRING COLON COLON )
            // BNGGrammar_ReactionRules.g:78:3: STRING COLON COLON
            {
            match(input,STRING,FOLLOW_STRING_in_reactionReference212); if (state.failed) return retval;
            match(input,COLON,FOLLOW_COLON_in_reactionReference214); if (state.failed) return retval;
            match(input,COLON,FOLLOW_COLON_in_reactionReference216); if (state.failed) return retval;

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
    // $ANTLR end "reactionReference"

    protected static class reaction_rule_def_scope {
        List patternsReactants;
        List patternsProducts;
        List<String> rateList;
        ReactionAction reactionAction;
        String name;
        String text;
        Map<String,Register> lmemory;
    }
    protected Stack reaction_rule_def_stack = new Stack();

    public static class reaction_rule_def_return extends ParserRuleReturnScope {
        public int numReactions;
        public String secondRate;
        public StringTemplate st;
        public Object getTemplate() { return st; }
        public String toString() { return st==null?null:st.toString(); }
    };

    // $ANTLR start "reaction_rule_def"
    // BNGGrammar_ReactionRules.g:81:1: reaction_rule_def[int reactionCounter] returns [int numReactions, String secondRate] : ( ( match_attribute )? (l1= reactionLabel )? ) reaction_def[$reaction_rule_def::patternsReactants,$reaction_rule_def::patternsProducts,\"RR\" + reactionCounter] WS bi= rate_function[$reaction_rule_def::rateList,$reaction_def.bidirectional] ( modif_command )* ( DELETEMOLECULES )? ( MOVECONNECTED )? -> reaction_block(id=\"RR\" + reactionCounterreactant=$reaction_rule_def::patternsReactantsproduct=$reaction_rule_def::patternsProductsname=$reaction_rule_def::namerate=$reaction_rule_def::rateListbidirectional=bilawType=$rate_function.functionNameleftMap=$reaction_rule_def::reactionAction.getLeft()rightMap=$reaction_rule_def::reactionAction.getRight()operations=$reaction_rule_def::reactionAction.getOperations()operator1=$reaction_rule_def::reactionAction.getOperator1()operator2=$reaction_rule_def::reactionAction.getOperator2()expression=$reaction_rule_def::text);
    public final BNGGrammar_BNGGrammar_ReactionRules.reaction_rule_def_return reaction_rule_def(int reactionCounter) throws RecognitionException {
        reaction_rule_def_stack.push(new reaction_rule_def_scope());
        BNGGrammar_BNGGrammar_ReactionRules.reaction_rule_def_return retval = new BNGGrammar_BNGGrammar_ReactionRules.reaction_rule_def_return();
        retval.start = input.LT(1);

        BNGGrammar_BNGGrammar_ReactionRules.reactionLabel_return l1 = null;

        BNGGrammar_BNGGrammar_ReactionRules.rate_function_return bi = null;

        BNGGrammar_BNGGrammar_ReactionRules.reaction_def_return reaction_def4 = null;



          ((reaction_rule_def_scope)reaction_rule_def_stack.peek()).patternsReactants = new ArrayList();
          ((reaction_rule_def_scope)reaction_rule_def_stack.peek()).patternsProducts = new ArrayList();
          ((reaction_rule_def_scope)reaction_rule_def_stack.peek()).rateList = new ArrayList<String>();
          ((reaction_rule_def_scope)reaction_rule_def_stack.peek()).reactionAction = new ReactionAction();
          ((reaction_rule_def_scope)reaction_rule_def_stack.peek()).name = "Rule" + reactionCounter;
          ((reaction_rule_def_scope)reaction_rule_def_stack.peek()).text = "";
          ((reaction_rule_def_scope)reaction_rule_def_stack.peek()).lmemory = new HashMap<String,Register>();
          

        try {
            // BNGGrammar_ReactionRules.g:101:9: ( ( ( match_attribute )? (l1= reactionLabel )? ) reaction_def[$reaction_rule_def::patternsReactants,$reaction_rule_def::patternsProducts,\"RR\" + reactionCounter] WS bi= rate_function[$reaction_rule_def::rateList,$reaction_def.bidirectional] ( modif_command )* ( DELETEMOLECULES )? ( MOVECONNECTED )? -> reaction_block(id=\"RR\" + reactionCounterreactant=$reaction_rule_def::patternsReactantsproduct=$reaction_rule_def::patternsProductsname=$reaction_rule_def::namerate=$reaction_rule_def::rateListbidirectional=bilawType=$rate_function.functionNameleftMap=$reaction_rule_def::reactionAction.getLeft()rightMap=$reaction_rule_def::reactionAction.getRight()operations=$reaction_rule_def::reactionAction.getOperations()operator1=$reaction_rule_def::reactionAction.getOperator1()operator2=$reaction_rule_def::reactionAction.getOperator2()expression=$reaction_rule_def::text))
            // BNGGrammar_ReactionRules.g:101:12: ( ( match_attribute )? (l1= reactionLabel )? ) reaction_def[$reaction_rule_def::patternsReactants,$reaction_rule_def::patternsProducts,\"RR\" + reactionCounter] WS bi= rate_function[$reaction_rule_def::rateList,$reaction_def.bidirectional] ( modif_command )* ( DELETEMOLECULES )? ( MOVECONNECTED )?
            {
            // BNGGrammar_ReactionRules.g:101:12: ( ( match_attribute )? (l1= reactionLabel )? )
            // BNGGrammar_ReactionRules.g:101:13: ( match_attribute )? (l1= reactionLabel )?
            {
            // BNGGrammar_ReactionRules.g:101:13: ( match_attribute )?
            int alt10=2;
            int LA10_0 = input.LA(1);

            if ( (LA10_0==LBRACKET) ) {
                alt10=1;
            }
            switch (alt10) {
                case 1 :
                    // BNGGrammar_ReactionRules.g:101:14: match_attribute
                    {
                    pushFollow(FOLLOW_match_attribute_in_reaction_rule_def248);
                    match_attribute();

                    state._fsp--;
                    if (state.failed) return retval;

                    }
                    break;

            }

            // BNGGrammar_ReactionRules.g:101:32: (l1= reactionLabel )?
            int alt11=2;
            int LA11_0 = input.LA(1);

            if ( (LA11_0==STRING) ) {
                int LA11_1 = input.LA(2);

                if ( (LA11_1==INT||LA11_1==STRING||LA11_1==COLON) ) {
                    alt11=1;
                }
            }
            else if ( (LA11_0==INT) ) {
                int LA11_2 = input.LA(2);

                if ( (LA11_2==INT||LA11_2==STRING||LA11_2==COLON) ) {
                    alt11=1;
                }
            }
            switch (alt11) {
                case 1 :
                    // BNGGrammar_ReactionRules.g:103:10: l1= reactionLabel
                    {
                    pushFollow(FOLLOW_reactionLabel_in_reaction_rule_def276);
                    l1=reactionLabel();

                    state._fsp--;
                    if (state.failed) return retval;
                    if ( state.backtracking==0 ) {
                      ((reaction_rule_def_scope)reaction_rule_def_stack.peek()).text += (l1!=null?l1.label:null) + ":"; 
                      	                         ((reaction_rule_def_scope)reaction_rule_def_stack.peek()).name = (l1!=null?l1.label:null);
                      	                  
                    }

                    }
                    break;

            }


            }

            pushFollow(FOLLOW_reaction_def_in_reaction_rule_def309);
            reaction_def4=reaction_def(((reaction_rule_def_scope)reaction_rule_def_stack.peek()).patternsReactants, ((reaction_rule_def_scope)reaction_rule_def_stack.peek()).patternsProducts, "RR" + reactionCounter);

            state._fsp--;
            if (state.failed) return retval;
            if ( state.backtracking==0 ) {

                        ((reaction_rule_def_scope)reaction_rule_def_stack.peek()).reactionAction.execute();
                        if((reaction_def4!=null?reaction_def4.bidirectional:false))
                            retval.numReactions = 2;
                         else
                            retval.numReactions = 1;
                         
                        ((reaction_rule_def_scope)reaction_rule_def_stack.peek()).text += (reaction_def4!=null?input.toString(reaction_def4.start,reaction_def4.stop):null);
                      
                      
            }
            if ( state.backtracking==0 ) {

                      //Whitespaces are normally skipped but they are still in the stream. In this case if this rule is valid
                      //a valid WS would be located on the previous token
                      ((ChangeableChannelTokenStream)input).seek(((ChangeableChannelTokenStream)input).index()-1);
                      
            }
            match(input,WS,FOLLOW_WS_in_reaction_rule_def341); if (state.failed) return retval;
            pushFollow(FOLLOW_rate_function_in_reaction_rule_def369);
            bi=rate_function(((reaction_rule_def_scope)reaction_rule_def_stack.peek()).rateList, (reaction_def4!=null?reaction_def4.bidirectional:false));

            state._fsp--;
            if (state.failed) return retval;
            if ( state.backtracking==0 ) {

                      //TODO: add a try catch exception to check that if a bidirectional reaction is required it asks for two reaction rates
                          if(retval.numReactions == 2 && ((reaction_rule_def_scope)reaction_rule_def_stack.peek()).rateList.size() > 1)
                              retval.secondRate =((reaction_rule_def_scope)reaction_rule_def_stack.peek()).rateList.get(1);
                          else
                              retval.secondRate ="0";
                          ((reaction_rule_def_scope)reaction_rule_def_stack.peek()).text += " " + (bi!=null?input.toString(bi.start,bi.stop):null);
                      
            }
            // BNGGrammar_ReactionRules.g:136:9: ( modif_command )*
            loop12:
            do {
                int alt12=2;
                int LA12_0 = input.LA(1);

                if ( ((LA12_0>=INCLUDE_REACTANTS && LA12_0<=EXCLUDE_PRODUCTS)) ) {
                    alt12=1;
                }


                switch (alt12) {
            	case 1 :
            	    // BNGGrammar_ReactionRules.g:136:10: modif_command
            	    {
            	    pushFollow(FOLLOW_modif_command_in_reaction_rule_def383);
            	    modif_command();

            	    state._fsp--;
            	    if (state.failed) return retval;

            	    }
            	    break;

            	default :
            	    break loop12;
                }
            } while (true);

            // BNGGrammar_ReactionRules.g:136:26: ( DELETEMOLECULES )?
            int alt13=2;
            int LA13_0 = input.LA(1);

            if ( (LA13_0==DELETEMOLECULES) ) {
                alt13=1;
            }
            switch (alt13) {
                case 1 :
                    // BNGGrammar_ReactionRules.g:136:27: DELETEMOLECULES
                    {
                    match(input,DELETEMOLECULES,FOLLOW_DELETEMOLECULES_in_reaction_rule_def388); if (state.failed) return retval;

                    }
                    break;

            }

            // BNGGrammar_ReactionRules.g:136:45: ( MOVECONNECTED )?
            int alt14=2;
            int LA14_0 = input.LA(1);

            if ( (LA14_0==MOVECONNECTED) ) {
                alt14=1;
            }
            switch (alt14) {
                case 1 :
                    // BNGGrammar_ReactionRules.g:136:46: MOVECONNECTED
                    {
                    match(input,MOVECONNECTED,FOLLOW_MOVECONNECTED_in_reaction_rule_def393); if (state.failed) return retval;

                    }
                    break;

            }

            if ( state.backtracking==0 ) {

                        ((reaction_rule_def_scope)reaction_rule_def_stack.peek()).text = ((reaction_rule_def_scope)reaction_rule_def_stack.peek()).text.replaceAll("<","&lt;");
                        ((reaction_rule_def_scope)reaction_rule_def_stack.peek()).text = ((reaction_rule_def_scope)reaction_rule_def_stack.peek()).text.replaceAll(">","&gt;");
                        ((reaction_rule_def_scope)reaction_rule_def_stack.peek()).text = ((reaction_rule_def_scope)reaction_rule_def_stack.peek()).text.replaceAll("\\$","&#36;");
                      
            }


            // TEMPLATE REWRITE
            if ( state.backtracking==0 ) {
              // 144:9: -> reaction_block(id=\"RR\" + reactionCounterreactant=$reaction_rule_def::patternsReactantsproduct=$reaction_rule_def::patternsProductsname=$reaction_rule_def::namerate=$reaction_rule_def::rateListbidirectional=bilawType=$rate_function.functionNameleftMap=$reaction_rule_def::reactionAction.getLeft()rightMap=$reaction_rule_def::reactionAction.getRight()operations=$reaction_rule_def::reactionAction.getOperations()operator1=$reaction_rule_def::reactionAction.getOperator1()operator2=$reaction_rule_def::reactionAction.getOperator2()expression=$reaction_rule_def::text)
              {
                  retval.st = templateLib.getInstanceOf("reaction_block",
                new STAttrMap().put("id", "RR" + reactionCounter).put("reactant", ((reaction_rule_def_scope)reaction_rule_def_stack.peek()).patternsReactants).put("product", ((reaction_rule_def_scope)reaction_rule_def_stack.peek()).patternsProducts).put("name", ((reaction_rule_def_scope)reaction_rule_def_stack.peek()).name).put("rate", ((reaction_rule_def_scope)reaction_rule_def_stack.peek()).rateList).put("bidirectional", bi).put("lawType", (bi!=null?bi.functionName:null)).put("leftMap", ((reaction_rule_def_scope)reaction_rule_def_stack.peek()).reactionAction.getLeft()).put("rightMap", ((reaction_rule_def_scope)reaction_rule_def_stack.peek()).reactionAction.getRight()).put("operations", ((reaction_rule_def_scope)reaction_rule_def_stack.peek()).reactionAction.getOperations()).put("operator1", ((reaction_rule_def_scope)reaction_rule_def_stack.peek()).reactionAction.getOperator1()).put("operator2", ((reaction_rule_def_scope)reaction_rule_def_stack.peek()).reactionAction.getOperator2()).put("expression", ((reaction_rule_def_scope)reaction_rule_def_stack.peek()).text));
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
            reaction_rule_def_stack.pop();
        }
        return retval;
    }
    // $ANTLR end "reaction_rule_def"

    public static class match_attribute_return extends ParserRuleReturnScope {
        public StringTemplate st;
        public Object getTemplate() { return st; }
        public String toString() { return st==null?null:st.toString(); }
    };

    // $ANTLR start "match_attribute"
    // BNGGrammar_ReactionRules.g:155:1: match_attribute : LBRACKET MATCHONCE RBRACKET ;
    public final BNGGrammar_BNGGrammar_ReactionRules.match_attribute_return match_attribute() throws RecognitionException {
        BNGGrammar_BNGGrammar_ReactionRules.match_attribute_return retval = new BNGGrammar_BNGGrammar_ReactionRules.match_attribute_return();
        retval.start = input.LT(1);

        try {
            // BNGGrammar_ReactionRules.g:156:9: ( LBRACKET MATCHONCE RBRACKET )
            // BNGGrammar_ReactionRules.g:156:11: LBRACKET MATCHONCE RBRACKET
            {
            match(input,LBRACKET,FOLLOW_LBRACKET_in_match_attribute585); if (state.failed) return retval;
            match(input,MATCHONCE,FOLLOW_MATCHONCE_in_match_attribute587); if (state.failed) return retval;
            match(input,RBRACKET,FOLLOW_RBRACKET_in_match_attribute589); if (state.failed) return retval;

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
    // $ANTLR end "match_attribute"

    protected static class reaction_def_scope {
        int reactantPatternCounter;
    }
    protected Stack reaction_def_stack = new Stack();

    public static class reaction_def_return extends ParserRuleReturnScope {
        public boolean bidirectional;
        public Map<String,List<ReactionRegister>> reactionStack;
        public Map<String,List<ReactionRegister>> productStack;
        public StringTemplate st;
        public Object getTemplate() { return st; }
        public String toString() { return st==null?null:st.toString(); }
    };

    // $ANTLR start "reaction_def"
    // BNGGrammar_ReactionRules.g:159:1: reaction_def[List patternsReactants,List patternsProducts,String upperID] returns [boolean bidirectional,Map<String,List<ReactionRegister>> reactionStack,\nMap<String,List<ReactionRegister>> productStack] : s1= rule_species_def[upperID+\"_RP\" + $reaction_def::reactantPatternCounter,$reaction_rule_def::reactionAction] ( PLUS s2= rule_species_def[upperID+\"_RP\"+ $reaction_def::reactantPatternCounter,$reaction_rule_def::reactionAction] )* ( UNI_REACTION_SIGN | BI_REACTION_SIGN ) (s3= rule_species_def[upperID+\"_PP\"+ 1,$reaction_rule_def::reactionAction] ) ( PLUS s4= rule_species_def[upperID+\"_PP\"+ $reaction_def::reactantPatternCounter,$reaction_rule_def::reactionAction] )* ;
    public final BNGGrammar_BNGGrammar_ReactionRules.reaction_def_return reaction_def(List patternsReactants, List patternsProducts, String upperID) throws RecognitionException {
        reaction_def_stack.push(new reaction_def_scope());
        BNGGrammar_BNGGrammar_ReactionRules.reaction_def_return retval = new BNGGrammar_BNGGrammar_ReactionRules.reaction_def_return();
        retval.start = input.LT(1);

        BNGGrammar_BNGGrammar_ReactionRules.rule_species_def_return s1 = null;

        BNGGrammar_BNGGrammar_ReactionRules.rule_species_def_return s2 = null;

        BNGGrammar_BNGGrammar_ReactionRules.rule_species_def_return s3 = null;

        BNGGrammar_BNGGrammar_ReactionRules.rule_species_def_return s4 = null;



          ((reaction_def_scope)reaction_def_stack.peek()).reactantPatternCounter =1;
          retval.reactionStack = new HashMap<String,List<ReactionRegister>>();
          retval.productStack = new HashMap<String,List<ReactionRegister>>();
          

        try {
            // BNGGrammar_ReactionRules.g:174:1: (s1= rule_species_def[upperID+\"_RP\" + $reaction_def::reactantPatternCounter,$reaction_rule_def::reactionAction] ( PLUS s2= rule_species_def[upperID+\"_RP\"+ $reaction_def::reactantPatternCounter,$reaction_rule_def::reactionAction] )* ( UNI_REACTION_SIGN | BI_REACTION_SIGN ) (s3= rule_species_def[upperID+\"_PP\"+ 1,$reaction_rule_def::reactionAction] ) ( PLUS s4= rule_species_def[upperID+\"_PP\"+ $reaction_def::reactantPatternCounter,$reaction_rule_def::reactionAction] )* )
            // BNGGrammar_ReactionRules.g:175:2: s1= rule_species_def[upperID+\"_RP\" + $reaction_def::reactantPatternCounter,$reaction_rule_def::reactionAction] ( PLUS s2= rule_species_def[upperID+\"_RP\"+ $reaction_def::reactantPatternCounter,$reaction_rule_def::reactionAction] )* ( UNI_REACTION_SIGN | BI_REACTION_SIGN ) (s3= rule_species_def[upperID+\"_PP\"+ 1,$reaction_rule_def::reactionAction] ) ( PLUS s4= rule_species_def[upperID+\"_PP\"+ $reaction_def::reactantPatternCounter,$reaction_rule_def::reactionAction] )*
            {
            pushFollow(FOLLOW_rule_species_def_in_reaction_def632);
            s1=rule_species_def(upperID+"_RP" + ((reaction_def_scope)reaction_def_stack.peek()).reactantPatternCounter, ((reaction_rule_def_scope)reaction_rule_def_stack.peek()).reactionAction);

            state._fsp--;
            if (state.failed) return retval;
            if ( state.backtracking==0 ) {

                          int counter = ((reaction_def_scope)reaction_def_stack.peek()).reactantPatternCounter;
                          for(int i=0;i<s1.stoichiometry;i++){
                             StringTemplate correctedString = GenericMethods.replace(s1.st,"RP" + counter,"RP" + ((reaction_def_scope)reaction_def_stack.peek()).reactantPatternCounter);
                             patternsReactants.add(correctedString);
                             ReactionRegister.mergeMaps((s1!=null?s1.map:null),retval.reactionStack);
                            ((reaction_def_scope)reaction_def_stack.peek()).reactantPatternCounter++;
                          } 
                         
            }
            // BNGGrammar_ReactionRules.g:187:3: ( PLUS s2= rule_species_def[upperID+\"_RP\"+ $reaction_def::reactantPatternCounter,$reaction_rule_def::reactionAction] )*
            loop15:
            do {
                int alt15=2;
                int LA15_0 = input.LA(1);

                if ( (LA15_0==PLUS) ) {
                    alt15=1;
                }


                switch (alt15) {
            	case 1 :
            	    // BNGGrammar_ReactionRules.g:187:4: PLUS s2= rule_species_def[upperID+\"_RP\"+ $reaction_def::reactantPatternCounter,$reaction_rule_def::reactionAction]
            	    {
            	    match(input,PLUS,FOLLOW_PLUS_in_reaction_def677); if (state.failed) return retval;
            	    pushFollow(FOLLOW_rule_species_def_in_reaction_def681);
            	    s2=rule_species_def(upperID+"_RP"+ ((reaction_def_scope)reaction_def_stack.peek()).reactantPatternCounter, ((reaction_rule_def_scope)reaction_rule_def_stack.peek()).reactionAction);

            	    state._fsp--;
            	    if (state.failed) return retval;
            	    if ( state.backtracking==0 ) {

            	                  int counter = ((reaction_def_scope)reaction_def_stack.peek()).reactantPatternCounter;
            	                  for(int i=0;i<s2.stoichiometry;i++){ 
            	                    StringTemplate correctedString = GenericMethods.replace(s2.st,"RP" + counter,"RP" + ((reaction_def_scope)reaction_def_stack.peek()).reactantPatternCounter);
            	                     patternsReactants.add(correctedString);
            	                     ReactionRegister.mergeMaps((s2!=null?s2.map:null),retval.reactionStack);
            	                    ((reaction_def_scope)reaction_def_stack.peek()).reactantPatternCounter++;
            	                  }
            	                  
            	    }

            	    }
            	    break;

            	default :
            	    break loop15;
                }
            } while (true);

            // BNGGrammar_ReactionRules.g:197:9: ( UNI_REACTION_SIGN | BI_REACTION_SIGN )
            int alt16=2;
            int LA16_0 = input.LA(1);

            if ( (LA16_0==UNI_REACTION_SIGN) ) {
                alt16=1;
            }
            else if ( (LA16_0==BI_REACTION_SIGN) ) {
                alt16=2;
            }
            else {
                if (state.backtracking>0) {state.failed=true; return retval;}
                NoViableAltException nvae =
                    new NoViableAltException("", 16, 0, input);

                throw nvae;
            }
            switch (alt16) {
                case 1 :
                    // BNGGrammar_ReactionRules.g:197:10: UNI_REACTION_SIGN
                    {
                    match(input,UNI_REACTION_SIGN,FOLLOW_UNI_REACTION_SIGN_in_reaction_def710); if (state.failed) return retval;
                    if ( state.backtracking==0 ) {
                      retval.bidirectional = false;
                    }

                    }
                    break;
                case 2 :
                    // BNGGrammar_ReactionRules.g:197:55: BI_REACTION_SIGN
                    {
                    match(input,BI_REACTION_SIGN,FOLLOW_BI_REACTION_SIGN_in_reaction_def715); if (state.failed) return retval;
                    if ( state.backtracking==0 ) {
                      retval.bidirectional = true;
                    }

                    }
                    break;

            }

            // BNGGrammar_ReactionRules.g:198:3: (s3= rule_species_def[upperID+\"_PP\"+ 1,$reaction_rule_def::reactionAction] )
            // BNGGrammar_ReactionRules.g:198:4: s3= rule_species_def[upperID+\"_PP\"+ 1,$reaction_rule_def::reactionAction]
            {
            pushFollow(FOLLOW_rule_species_def_in_reaction_def726);
            s3=rule_species_def(upperID+"_PP"+ 1, ((reaction_rule_def_scope)reaction_rule_def_stack.peek()).reactionAction);

            state._fsp--;
            if (state.failed) return retval;
            if ( state.backtracking==0 ) {

                      ((reaction_def_scope)reaction_def_stack.peek()).reactantPatternCounter =1;
                        int counter = ((reaction_def_scope)reaction_def_stack.peek()).reactantPatternCounter;
                        for(int i=0;i<s3.stoichiometry;i++){ 
                          StringTemplate correctedString = GenericMethods.replace(s3.st,"PP" + counter,"PP" + ((reaction_def_scope)reaction_def_stack.peek()).reactantPatternCounter);
                          patternsProducts.add(correctedString);
                          ReactionRegister.mergeMaps((s3!=null?s3.map:null),retval.productStack);
                          ((reaction_def_scope)reaction_def_stack.peek()).reactantPatternCounter++;
                      }
                      
            }

            }

            // BNGGrammar_ReactionRules.g:209:9: ( PLUS s4= rule_species_def[upperID+\"_PP\"+ $reaction_def::reactantPatternCounter,$reaction_rule_def::reactionAction] )*
            loop17:
            do {
                int alt17=2;
                int LA17_0 = input.LA(1);

                if ( (LA17_0==PLUS) ) {
                    alt17=1;
                }


                switch (alt17) {
            	case 1 :
            	    // BNGGrammar_ReactionRules.g:209:10: PLUS s4= rule_species_def[upperID+\"_PP\"+ $reaction_def::reactantPatternCounter,$reaction_rule_def::reactionAction]
            	    {
            	    match(input,PLUS,FOLLOW_PLUS_in_reaction_def751); if (state.failed) return retval;
            	    pushFollow(FOLLOW_rule_species_def_in_reaction_def755);
            	    s4=rule_species_def(upperID+"_PP"+ ((reaction_def_scope)reaction_def_stack.peek()).reactantPatternCounter, ((reaction_rule_def_scope)reaction_rule_def_stack.peek()).reactionAction);

            	    state._fsp--;
            	    if (state.failed) return retval;
            	    if ( state.backtracking==0 ) {

            	                  int counter = ((reaction_def_scope)reaction_def_stack.peek()).reactantPatternCounter;
            	                 for(int i=0;i<s4.stoichiometry;i++){ 
            	                     StringTemplate correctedString = GenericMethods.replace(s4.st,"PP" + counter,"PP" + ((reaction_def_scope)reaction_def_stack.peek()).reactantPatternCounter);
            	                     patternsProducts.add(correctedString);
            	                    ((reaction_def_scope)reaction_def_stack.peek()).reactantPatternCounter++;
            	                    ReactionRegister.mergeMaps((s4!=null?s4.map:null),retval.productStack);
            	                 }
            	              
            	    }

            	    }
            	    break;

            	default :
            	    break loop17;
                }
            } while (true);


            }

            retval.stop = input.LT(-1);

        }
        catch (RecognitionException re) {
            reportError(re);
            recover(input,re);
        }
        finally {
            reaction_def_stack.pop();
        }
        return retval;
    }
    // $ANTLR end "reaction_def"

    protected static class rule_species_def_scope {
        List reactants;
        BondList bonds;
    }
    protected Stack rule_species_def_stack = new Stack();

    public static class rule_species_def_return extends ParserRuleReturnScope {
        public int stoichiometry;
        public Map <String,List<ReactionRegister>> map;
        public boolean fixed;
        public StringTemplate st;
        public Object getTemplate() { return st; }
        public String toString() { return st==null?null:st.toString(); }
    };

    // $ANTLR start "rule_species_def"
    // BNGGrammar_ReactionRules.g:222:1: rule_species_def[String upperID,ReactionAction reactionAction] returns [int stoichiometry,Map <String,List<ReactionRegister>> map,boolean fixed] : ( (i1= INT TIMES )? s1= species_def[$rule_species_def::reactants,$rule_species_def::bonds,upperID] | i2= INT ) -> rule_seed_species_block(id=upperIDmolecules=$rule_species_def::reactantsfirstBonds=$rule_species_def::bonds.getLeft()secondBonds=$rule_species_def::bonds.getRight());
    public final BNGGrammar_BNGGrammar_ReactionRules.rule_species_def_return rule_species_def(String upperID, ReactionAction reactionAction) throws RecognitionException {
        rule_species_def_stack.push(new rule_species_def_scope());
        BNGGrammar_BNGGrammar_ReactionRules.rule_species_def_return retval = new BNGGrammar_BNGGrammar_ReactionRules.rule_species_def_return();
        retval.start = input.LT(1);

        Token i1=null;
        Token i2=null;
        BNGGrammar_BNGGrammar_SeedSpecies.species_def_return s1 = null;



          ((rule_species_def_scope)rule_species_def_stack.peek()).reactants = new ArrayList();
          ((rule_species_def_scope)rule_species_def_stack.peek()).bonds = new BondList();
          retval.stoichiometry = 1;
          retval.fixed = false;

        try {
            // BNGGrammar_ReactionRules.g:233:1: ( ( (i1= INT TIMES )? s1= species_def[$rule_species_def::reactants,$rule_species_def::bonds,upperID] | i2= INT ) -> rule_seed_species_block(id=upperIDmolecules=$rule_species_def::reactantsfirstBonds=$rule_species_def::bonds.getLeft()secondBonds=$rule_species_def::bonds.getRight()))
            // BNGGrammar_ReactionRules.g:234:1: ( (i1= INT TIMES )? s1= species_def[$rule_species_def::reactants,$rule_species_def::bonds,upperID] | i2= INT )
            {
            // BNGGrammar_ReactionRules.g:234:1: ( (i1= INT TIMES )? s1= species_def[$rule_species_def::reactants,$rule_species_def::bonds,upperID] | i2= INT )
            int alt19=2;
            int LA19_0 = input.LA(1);

            if ( (LA19_0==INT) ) {
                int LA19_1 = input.LA(2);

                if ( (LA19_1==TIMES) ) {
                    alt19=1;
                }
                else if ( (LA19_1==WS||(LA19_1>=UNI_REACTION_SIGN && LA19_1<=BI_REACTION_SIGN)||LA19_1==PLUS) ) {
                    alt19=2;
                }
                else {
                    if (state.backtracking>0) {state.failed=true; return retval;}
                    NoViableAltException nvae =
                        new NoViableAltException("", 19, 1, input);

                    throw nvae;
                }
            }
            else if ( (LA19_0==STRING||LA19_0==DOLLAR||LA19_0==AT||LA19_0==MOD) ) {
                alt19=1;
            }
            else {
                if (state.backtracking>0) {state.failed=true; return retval;}
                NoViableAltException nvae =
                    new NoViableAltException("", 19, 0, input);

                throw nvae;
            }
            switch (alt19) {
                case 1 :
                    // BNGGrammar_ReactionRules.g:235:1: (i1= INT TIMES )? s1= species_def[$rule_species_def::reactants,$rule_species_def::bonds,upperID]
                    {
                    // BNGGrammar_ReactionRules.g:235:1: (i1= INT TIMES )?
                    int alt18=2;
                    int LA18_0 = input.LA(1);

                    if ( (LA18_0==INT) ) {
                        alt18=1;
                    }
                    switch (alt18) {
                        case 1 :
                            // BNGGrammar_ReactionRules.g:235:2: i1= INT TIMES
                            {
                            i1=(Token)match(input,INT,FOLLOW_INT_in_rule_species_def819); if (state.failed) return retval;
                            if ( state.backtracking==0 ) {
                              retval.stoichiometry = Integer.parseInt((i1!=null?i1.getText():null));
                            }
                            match(input,TIMES,FOLLOW_TIMES_in_rule_species_def823); if (state.failed) return retval;

                            }
                            break;

                    }

                    pushFollow(FOLLOW_species_def_in_rule_species_def831);
                    s1=gBNGGrammar.species_def(((rule_species_def_scope)rule_species_def_stack.peek()).reactants, ((rule_species_def_scope)rule_species_def_stack.peek()).bonds, upperID);

                    state._fsp--;
                    if (state.failed) return retval;
                    if ( state.backtracking==0 ) {

                             String trimmedName = (s1!=null?input.toString(s1.start,s1.stop):null);
                             trimmedName = trimmedName.replaceAll("\\$","");
                             reactionAction.addMolecule(upperID,trimmedName,((rule_species_def_scope)rule_species_def_stack.peek()).bonds);
                             retval.map = (s1!=null?s1.listOfMolecules:null);
                             ((reaction_rule_def_scope)reaction_rule_def_stack.peek()).lmemory.putAll((s1!=null?s1.lmemory:null));
                             if((s1!=null?s1.constant:false))
                                retval.fixed = true;
                        
                    }
                    if ( state.backtracking==0 ) {

                         
                             if(!((rule_species_def_scope)rule_species_def_stack.peek()).bonds.validateBonds(0,0)){
                                String err = String.format("%s line %d:%d %s\n",input.getSourceName(),s1.start.getLine(),s1.start.getCharPositionInLine(),"dangling bond");
                        
                                System.err.println(err);
                              }
                        
                    }

                    }
                    break;
                case 2 :
                    // BNGGrammar_ReactionRules.g:254:5: i2= INT
                    {
                    i2=(Token)match(input,INT,FOLLOW_INT_in_rule_species_def847); if (state.failed) return retval;
                    if ( state.backtracking==0 ) {

                              retval.map = new HashMap<String,List<ReactionRegister>>();
                              if(!(i2!=null?i2.getText():null).equals("0")){
                              throw new RecognitionException();
                              }
                            
                    }

                    }
                    break;

            }



            // TEMPLATE REWRITE
            if ( state.backtracking==0 ) {
              // 263:5: -> rule_seed_species_block(id=upperIDmolecules=$rule_species_def::reactantsfirstBonds=$rule_species_def::bonds.getLeft()secondBonds=$rule_species_def::bonds.getRight())
              {
                  retval.st = templateLib.getInstanceOf("rule_seed_species_block",
                new STAttrMap().put("id", upperID).put("molecules", ((rule_species_def_scope)rule_species_def_stack.peek()).reactants).put("firstBonds", ((rule_species_def_scope)rule_species_def_stack.peek()).bonds.getLeft()).put("secondBonds", ((rule_species_def_scope)rule_species_def_stack.peek()).bonds.getRight()));
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
            rule_species_def_stack.pop();
        }
        return retval;
    }
    // $ANTLR end "rule_species_def"

    public static class rate_function_return extends ParserRuleReturnScope {
        public String functionName;
        public StringTemplate st;
        public Object getTemplate() { return st; }
        public String toString() { return st==null?null:st.toString(); }
    };

    // $ANTLR start "rate_function"
    // BNGGrammar_ReactionRules.g:267:1: rate_function[List<String> rateList,boolean bidirectional] returns [String functionName] : ( ( function_keyword LPAREN )=> function_keyword LPAREN rate_list[rateList,bidirectional] RPAREN | rate_list[rateList,bidirectional] );
    public final BNGGrammar_BNGGrammar_ReactionRules.rate_function_return rate_function(List<String> rateList, boolean bidirectional) throws RecognitionException {
        BNGGrammar_BNGGrammar_ReactionRules.rate_function_return retval = new BNGGrammar_BNGGrammar_ReactionRules.rate_function_return();
        retval.start = input.LT(1);

        BNGGrammar_BNGGrammar_ReactionRules.function_keyword_return function_keyword5 = null;



          retval.functionName = "Ele";

        try {
            // BNGGrammar_ReactionRules.g:270:2: ( ( function_keyword LPAREN )=> function_keyword LPAREN rate_list[rateList,bidirectional] RPAREN | rate_list[rateList,bidirectional] )
            int alt20=2;
            int LA20_0 = input.LA(1);

            if ( (LA20_0==SAT) && (synpred1_BNGGrammar_ReactionRules())) {
                alt20=1;
            }
            else if ( (LA20_0==IF||LA20_0==NOT||(LA20_0>=EXP && LA20_0<=LOG)||(LA20_0>=FLOAT && LA20_0<=INT)||LA20_0==STRING||LA20_0==LPAREN||LA20_0==MINUS) ) {
                alt20=2;
            }
            else {
                if (state.backtracking>0) {state.failed=true; return retval;}
                NoViableAltException nvae =
                    new NoViableAltException("", 20, 0, input);

                throw nvae;
            }
            switch (alt20) {
                case 1 :
                    // BNGGrammar_ReactionRules.g:271:5: ( function_keyword LPAREN )=> function_keyword LPAREN rate_list[rateList,bidirectional] RPAREN
                    {
                    pushFollow(FOLLOW_function_keyword_in_rate_function922);
                    function_keyword5=function_keyword();

                    state._fsp--;
                    if (state.failed) return retval;
                    if ( state.backtracking==0 ) {
                      retval.functionName = (function_keyword5!=null?input.toString(function_keyword5.start,function_keyword5.stop):null);
                    }
                    match(input,LPAREN,FOLLOW_LPAREN_in_rate_function926); if (state.failed) return retval;
                    pushFollow(FOLLOW_rate_list_in_rate_function928);
                    rate_list(rateList, bidirectional);

                    state._fsp--;
                    if (state.failed) return retval;
                    match(input,RPAREN,FOLLOW_RPAREN_in_rate_function931); if (state.failed) return retval;

                    }
                    break;
                case 2 :
                    // BNGGrammar_ReactionRules.g:272:5: rate_list[rateList,bidirectional]
                    {
                    pushFollow(FOLLOW_rate_list_in_rate_function939);
                    rate_list(rateList, bidirectional);

                    state._fsp--;
                    if (state.failed) return retval;

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
    // $ANTLR end "rate_function"

    public static class function_keyword_return extends ParserRuleReturnScope {
        public StringTemplate st;
        public Object getTemplate() { return st; }
        public String toString() { return st==null?null:st.toString(); }
    };

    // $ANTLR start "function_keyword"
    // BNGGrammar_ReactionRules.g:276:1: function_keyword : SAT ;
    public final BNGGrammar_BNGGrammar_ReactionRules.function_keyword_return function_keyword() throws RecognitionException {
        BNGGrammar_BNGGrammar_ReactionRules.function_keyword_return retval = new BNGGrammar_BNGGrammar_ReactionRules.function_keyword_return();
        retval.start = input.LT(1);

        try {
            // BNGGrammar_ReactionRules.g:276:17: ( SAT )
            // BNGGrammar_ReactionRules.g:277:3: SAT
            {
            match(input,SAT,FOLLOW_SAT_in_function_keyword955); if (state.failed) return retval;

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
    // $ANTLR end "function_keyword"

    protected static class rate_list_scope {
        Map<String,Register> memoryWithLocal;
        int numberRateLaws;
    }
    protected Stack rate_list_stack = new Stack();

    public static class rate_list_return extends ParserRuleReturnScope {
        public StringTemplate st;
        public Object getTemplate() { return st; }
        public String toString() { return st==null?null:st.toString(); }
    };

    // $ANTLR start "rate_list"
    // BNGGrammar_ReactionRules.g:280:1: rate_list[List<String> rateList,boolean bidirectional] : e1= expression[$rate_list::memoryWithLocal] ( ( COMMA )? e2= expression[$rate_list::memoryWithLocal] )? ;
    public final BNGGrammar_BNGGrammar_ReactionRules.rate_list_return rate_list(List<String> rateList, boolean bidirectional) throws RecognitionException {
        rate_list_stack.push(new rate_list_scope());
        BNGGrammar_BNGGrammar_ReactionRules.rate_list_return retval = new BNGGrammar_BNGGrammar_ReactionRules.rate_list_return();
        retval.start = input.LT(1);

        BNGGrammar_BNGGrammar_Expression.expression_return e1 = null;

        BNGGrammar_BNGGrammar_Expression.expression_return e2 = null;



          ((rate_list_scope)rate_list_stack.peek()).memoryWithLocal = new HashMap<String,Register>();
          ((rate_list_scope)rate_list_stack.peek()).memoryWithLocal.putAll(gParent.memory);
          ((rate_list_scope)rate_list_stack.peek()).memoryWithLocal.putAll(((reaction_rule_def_scope)reaction_rule_def_stack.peek()).lmemory);
          ((rate_list_scope)rate_list_stack.peek()).numberRateLaws = 1;

        try {
            // BNGGrammar_ReactionRules.g:291:9: (e1= expression[$rate_list::memoryWithLocal] ( ( COMMA )? e2= expression[$rate_list::memoryWithLocal] )? )
            // BNGGrammar_ReactionRules.g:291:11: e1= expression[$rate_list::memoryWithLocal] ( ( COMMA )? e2= expression[$rate_list::memoryWithLocal] )?
            {
            pushFollow(FOLLOW_expression_in_rate_list983);
            e1=gBNGGrammar.expression(((rate_list_scope)rate_list_stack.peek()).memoryWithLocal);

            state._fsp--;
            if (state.failed) return retval;
            if ( state.backtracking==0 ) {
              rateList.add((e1!=null?input.toString(e1.start,e1.stop):null));
            }
            // BNGGrammar_ReactionRules.g:291:79: ( ( COMMA )? e2= expression[$rate_list::memoryWithLocal] )?
            int alt22=2;
            int LA22_0 = input.LA(1);

            if ( (LA22_0==IF||LA22_0==NOT||(LA22_0>=EXP && LA22_0<=LOG)||(LA22_0>=FLOAT && LA22_0<=INT)||LA22_0==STRING||LA22_0==COMMA||LA22_0==LPAREN||LA22_0==MINUS) ) {
                alt22=1;
            }
            switch (alt22) {
                case 1 :
                    // BNGGrammar_ReactionRules.g:291:80: ( COMMA )? e2= expression[$rate_list::memoryWithLocal]
                    {
                    // BNGGrammar_ReactionRules.g:291:80: ( COMMA )?
                    int alt21=2;
                    int LA21_0 = input.LA(1);

                    if ( (LA21_0==COMMA) ) {
                        alt21=1;
                    }
                    switch (alt21) {
                        case 1 :
                            // BNGGrammar_ReactionRules.g:291:80: COMMA
                            {
                            match(input,COMMA,FOLLOW_COMMA_in_rate_list988); if (state.failed) return retval;

                            }
                            break;

                    }

                    pushFollow(FOLLOW_expression_in_rate_list993);
                    e2=gBNGGrammar.expression(((rate_list_scope)rate_list_stack.peek()).memoryWithLocal);

                    state._fsp--;
                    if (state.failed) return retval;
                    if ( state.backtracking==0 ) {
                      rateList.add((e2!=null?input.toString(e2.start,e2.stop):null));
                              ((rate_list_scope)rate_list_stack.peek()).numberRateLaws = 2;
                              
                    }

                    }
                    break;

            }

            if ( state.backtracking==0 ) {

                      if ((bidirectional && ((rate_list_scope)rate_list_stack.peek()).numberRateLaws == 1) || (!bidirectional && ((rate_list_scope)rate_list_stack.peek()).numberRateLaws == 2)){
                                  String msg = getErrorMessage2((Token)e1.getStart(),"Incorrect number of rate laws");
                                  System.err.println(msg);
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
            rate_list_stack.pop();
        }
        return retval;
    }
    // $ANTLR end "rate_list"

    public static class modif_command_return extends ParserRuleReturnScope {
        public StringTemplate st;
        public Object getTemplate() { return st; }
        public String toString() { return st==null?null:st.toString(); }
    };

    // $ANTLR start "modif_command"
    // BNGGrammar_ReactionRules.g:303:1: modif_command : ( include_command | exclude_command );
    public final BNGGrammar_BNGGrammar_ReactionRules.modif_command_return modif_command() throws RecognitionException {
        BNGGrammar_BNGGrammar_ReactionRules.modif_command_return retval = new BNGGrammar_BNGGrammar_ReactionRules.modif_command_return();
        retval.start = input.LT(1);

        try {
            // BNGGrammar_ReactionRules.g:304:9: ( include_command | exclude_command )
            int alt23=2;
            int LA23_0 = input.LA(1);

            if ( ((LA23_0>=INCLUDE_REACTANTS && LA23_0<=INCLUDE_PRODUCTS)) ) {
                alt23=1;
            }
            else if ( ((LA23_0>=EXCLUDE_REACTANTS && LA23_0<=EXCLUDE_PRODUCTS)) ) {
                alt23=2;
            }
            else {
                if (state.backtracking>0) {state.failed=true; return retval;}
                NoViableAltException nvae =
                    new NoViableAltException("", 23, 0, input);

                throw nvae;
            }
            switch (alt23) {
                case 1 :
                    // BNGGrammar_ReactionRules.g:304:11: include_command
                    {
                    pushFollow(FOLLOW_include_command_in_modif_command1041);
                    include_command();

                    state._fsp--;
                    if (state.failed) return retval;

                    }
                    break;
                case 2 :
                    // BNGGrammar_ReactionRules.g:305:11: exclude_command
                    {
                    pushFollow(FOLLOW_exclude_command_in_modif_command1053);
                    exclude_command();

                    state._fsp--;
                    if (state.failed) return retval;

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
    // $ANTLR end "modif_command"

    public static class include_command_return extends ParserRuleReturnScope {
        public StringTemplate st;
        public Object getTemplate() { return st; }
        public String toString() { return st==null?null:st.toString(); }
    };

    // $ANTLR start "include_command"
    // BNGGrammar_ReactionRules.g:308:1: include_command : ( INCLUDE_REACTANTS | INCLUDE_PRODUCTS ) LPAREN INT COMMA pattern ( COMMA pattern )* RPAREN ;
    public final BNGGrammar_BNGGrammar_ReactionRules.include_command_return include_command() throws RecognitionException {
        BNGGrammar_BNGGrammar_ReactionRules.include_command_return retval = new BNGGrammar_BNGGrammar_ReactionRules.include_command_return();
        retval.start = input.LT(1);

        try {
            // BNGGrammar_ReactionRules.g:309:9: ( ( INCLUDE_REACTANTS | INCLUDE_PRODUCTS ) LPAREN INT COMMA pattern ( COMMA pattern )* RPAREN )
            // BNGGrammar_ReactionRules.g:309:12: ( INCLUDE_REACTANTS | INCLUDE_PRODUCTS ) LPAREN INT COMMA pattern ( COMMA pattern )* RPAREN
            {
            if ( (input.LA(1)>=INCLUDE_REACTANTS && input.LA(1)<=INCLUDE_PRODUCTS) ) {
                input.consume();
                state.errorRecovery=false;state.failed=false;
            }
            else {
                if (state.backtracking>0) {state.failed=true; return retval;}
                MismatchedSetException mse = new MismatchedSetException(null,input);
                throw mse;
            }

            match(input,LPAREN,FOLLOW_LPAREN_in_include_command1107); if (state.failed) return retval;
            match(input,INT,FOLLOW_INT_in_include_command1109); if (state.failed) return retval;
            match(input,COMMA,FOLLOW_COMMA_in_include_command1111); if (state.failed) return retval;
            pushFollow(FOLLOW_pattern_in_include_command1113);
            pattern();

            state._fsp--;
            if (state.failed) return retval;
            // BNGGrammar_ReactionRules.g:311:36: ( COMMA pattern )*
            loop24:
            do {
                int alt24=2;
                int LA24_0 = input.LA(1);

                if ( (LA24_0==COMMA) ) {
                    alt24=1;
                }


                switch (alt24) {
            	case 1 :
            	    // BNGGrammar_ReactionRules.g:311:37: COMMA pattern
            	    {
            	    match(input,COMMA,FOLLOW_COMMA_in_include_command1116); if (state.failed) return retval;
            	    pushFollow(FOLLOW_pattern_in_include_command1118);
            	    pattern();

            	    state._fsp--;
            	    if (state.failed) return retval;

            	    }
            	    break;

            	default :
            	    break loop24;
                }
            } while (true);

            match(input,RPAREN,FOLLOW_RPAREN_in_include_command1122); if (state.failed) return retval;

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
    // $ANTLR end "include_command"

    public static class exclude_command_return extends ParserRuleReturnScope {
        public StringTemplate st;
        public Object getTemplate() { return st; }
        public String toString() { return st==null?null:st.toString(); }
    };

    // $ANTLR start "exclude_command"
    // BNGGrammar_ReactionRules.g:313:1: exclude_command : ( EXCLUDE_REACTANTS | EXCLUDE_PRODUCTS ) LPAREN INT COMMA pattern ( COMMA pattern )* RPAREN ;
    public final BNGGrammar_BNGGrammar_ReactionRules.exclude_command_return exclude_command() throws RecognitionException {
        BNGGrammar_BNGGrammar_ReactionRules.exclude_command_return retval = new BNGGrammar_BNGGrammar_ReactionRules.exclude_command_return();
        retval.start = input.LT(1);

        try {
            // BNGGrammar_ReactionRules.g:314:9: ( ( EXCLUDE_REACTANTS | EXCLUDE_PRODUCTS ) LPAREN INT COMMA pattern ( COMMA pattern )* RPAREN )
            // BNGGrammar_ReactionRules.g:314:11: ( EXCLUDE_REACTANTS | EXCLUDE_PRODUCTS ) LPAREN INT COMMA pattern ( COMMA pattern )* RPAREN
            {
            if ( (input.LA(1)>=EXCLUDE_REACTANTS && input.LA(1)<=EXCLUDE_PRODUCTS) ) {
                input.consume();
                state.errorRecovery=false;state.failed=false;
            }
            else {
                if (state.backtracking>0) {state.failed=true; return retval;}
                MismatchedSetException mse = new MismatchedSetException(null,input);
                throw mse;
            }

            match(input,LPAREN,FOLLOW_LPAREN_in_exclude_command1170); if (state.failed) return retval;
            match(input,INT,FOLLOW_INT_in_exclude_command1172); if (state.failed) return retval;
            match(input,COMMA,FOLLOW_COMMA_in_exclude_command1174); if (state.failed) return retval;
            pushFollow(FOLLOW_pattern_in_exclude_command1176);
            pattern();

            state._fsp--;
            if (state.failed) return retval;
            // BNGGrammar_ReactionRules.g:316:34: ( COMMA pattern )*
            loop25:
            do {
                int alt25=2;
                int LA25_0 = input.LA(1);

                if ( (LA25_0==COMMA) ) {
                    alt25=1;
                }


                switch (alt25) {
            	case 1 :
            	    // BNGGrammar_ReactionRules.g:316:35: COMMA pattern
            	    {
            	    match(input,COMMA,FOLLOW_COMMA_in_exclude_command1179); if (state.failed) return retval;
            	    pushFollow(FOLLOW_pattern_in_exclude_command1181);
            	    pattern();

            	    state._fsp--;
            	    if (state.failed) return retval;

            	    }
            	    break;

            	default :
            	    break loop25;
                }
            } while (true);

            match(input,RPAREN,FOLLOW_RPAREN_in_exclude_command1185); if (state.failed) return retval;

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
    // $ANTLR end "exclude_command"

    public static class pattern_return extends ParserRuleReturnScope {
        public StringTemplate st;
        public Object getTemplate() { return st; }
        public String toString() { return st==null?null:st.toString(); }
    };

    // $ANTLR start "pattern"
    // BNGGrammar_ReactionRules.g:321:1: pattern : ( STRING | INT );
    public final BNGGrammar_BNGGrammar_ReactionRules.pattern_return pattern() throws RecognitionException {
        BNGGrammar_BNGGrammar_ReactionRules.pattern_return retval = new BNGGrammar_BNGGrammar_ReactionRules.pattern_return();
        retval.start = input.LT(1);

        try {
            // BNGGrammar_ReactionRules.g:321:9: ( STRING | INT )
            // BNGGrammar_ReactionRules.g:
            {
            if ( input.LA(1)==INT||input.LA(1)==STRING ) {
                input.consume();
                state.errorRecovery=false;state.failed=false;
            }
            else {
                if (state.backtracking>0) {state.failed=true; return retval;}
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
    // $ANTLR end "pattern"

    // $ANTLR start synpred1_BNGGrammar_ReactionRules
    public final void synpred1_BNGGrammar_ReactionRules_fragment() throws RecognitionException {   
        // BNGGrammar_ReactionRules.g:271:5: ( function_keyword LPAREN )
        // BNGGrammar_ReactionRules.g:271:6: function_keyword LPAREN
        {
        pushFollow(FOLLOW_function_keyword_in_synpred1_BNGGrammar_ReactionRules915);
        function_keyword();

        state._fsp--;
        if (state.failed) return ;
        match(input,LPAREN,FOLLOW_LPAREN_in_synpred1_BNGGrammar_ReactionRules917); if (state.failed) return ;

        }
    }
    // $ANTLR end synpred1_BNGGrammar_ReactionRules

    // Delegated rules

    public final boolean synpred1_BNGGrammar_ReactionRules() {
        state.backtracking++;
        int start = input.mark();
        try {
            synpred1_BNGGrammar_ReactionRules_fragment(); // can never throw exception
        } catch (RecognitionException re) {
            System.err.println("impossible: "+re);
        }
        boolean success = !state.failed;
        input.rewind(start);
        state.backtracking--;
        state.failed=false;
        return success;
    }


 

    public static final BitSet FOLLOW_BEGIN_in_population_maps_block39 = new BitSet(new long[]{0x0000000001000000L});
    public static final BitSet FOLLOW_POPULATION_in_population_maps_block41 = new BitSet(new long[]{0x0000000002000000L});
    public static final BitSet FOLLOW_MAPS_in_population_maps_block43 = new BitSet(new long[]{0x0000000000000020L});
    public static final BitSet FOLLOW_LB_in_population_maps_block45 = new BitSet(new long[]{0x0000000000000120L,0x010A000000000000L,0x0000000000008005L});
    public static final BitSet FOLLOW_reaction_rule_def_in_population_maps_block51 = new BitSet(new long[]{0x0000000000000020L});
    public static final BitSet FOLLOW_LB_in_population_maps_block54 = new BitSet(new long[]{0x0000000000000120L,0x010A000000000000L,0x0000000000008005L});
    public static final BitSet FOLLOW_END_in_population_maps_block60 = new BitSet(new long[]{0x0000000001000000L});
    public static final BitSet FOLLOW_POPULATION_in_population_maps_block62 = new BitSet(new long[]{0x0000000002000000L});
    public static final BitSet FOLLOW_MAPS_in_population_maps_block64 = new BitSet(new long[]{0x0000000000000020L});
    public static final BitSet FOLLOW_LB_in_population_maps_block66 = new BitSet(new long[]{0x0000000000000022L});
    public static final BitSet FOLLOW_BEGIN_in_reaction_rules_block100 = new BitSet(new long[]{0x0000000000020000L});
    public static final BitSet FOLLOW_REACTION_in_reaction_rules_block102 = new BitSet(new long[]{0x0000000000080000L});
    public static final BitSet FOLLOW_RULES_in_reaction_rules_block104 = new BitSet(new long[]{0x0000000000000020L});
    public static final BitSet FOLLOW_LB_in_reaction_rules_block106 = new BitSet(new long[]{0x0000000000000120L,0x010A000000000000L,0x0000000000008005L});
    public static final BitSet FOLLOW_reaction_rule_def_in_reaction_rules_block109 = new BitSet(new long[]{0x0000000000000020L});
    public static final BitSet FOLLOW_LB_in_reaction_rules_block132 = new BitSet(new long[]{0x0000000000000120L,0x010A000000000000L,0x0000000000008005L});
    public static final BitSet FOLLOW_END_in_reaction_rules_block137 = new BitSet(new long[]{0x0000000000020000L});
    public static final BitSet FOLLOW_REACTION_in_reaction_rules_block139 = new BitSet(new long[]{0x0000000000080000L});
    public static final BitSet FOLLOW_RULES_in_reaction_rules_block141 = new BitSet(new long[]{0x0000000000000022L});
    public static final BitSet FOLLOW_LB_in_reaction_rules_block143 = new BitSet(new long[]{0x0000000000000022L});
    public static final BitSet FOLLOW_STRING_in_reactionLabel172 = new BitSet(new long[]{0x0000000000000000L,0x002A000000000000L});
    public static final BitSet FOLLOW_INT_in_reactionLabel187 = new BitSet(new long[]{0x0000000000000000L,0x002A000000000000L});
    public static final BitSet FOLLOW_COLON_in_reactionLabel200 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_STRING_in_reactionReference212 = new BitSet(new long[]{0x0000000000000000L,0x0020000000000000L});
    public static final BitSet FOLLOW_COLON_in_reactionReference214 = new BitSet(new long[]{0x0000000000000000L,0x0020000000000000L});
    public static final BitSet FOLLOW_COLON_in_reactionReference216 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_match_attribute_in_reaction_rule_def248 = new BitSet(new long[]{0x0000000000000000L,0x010A000000000000L,0x0000000000008005L});
    public static final BitSet FOLLOW_reactionLabel_in_reaction_rule_def276 = new BitSet(new long[]{0x0000000000000000L,0x010A000000000000L,0x0000000000008005L});
    public static final BitSet FOLLOW_reaction_def_in_reaction_rule_def309 = new BitSet(new long[]{0x0000000000000040L});
    public static final BitSet FOLLOW_WS_in_reaction_rule_def341 = new BitSet(new long[]{0x0004002000000000L,0x100B0001C0000000L,0x0000000000001000L});
    public static final BitSet FOLLOW_rate_function_in_reaction_rule_def369 = new BitSet(new long[]{0x0078600000000002L});
    public static final BitSet FOLLOW_modif_command_in_reaction_rule_def383 = new BitSet(new long[]{0x0078600000000002L});
    public static final BitSet FOLLOW_DELETEMOLECULES_in_reaction_rule_def388 = new BitSet(new long[]{0x0000400000000002L});
    public static final BitSet FOLLOW_MOVECONNECTED_in_reaction_rule_def393 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_LBRACKET_in_match_attribute585 = new BitSet(new long[]{0x0000100000000000L});
    public static final BitSet FOLLOW_MATCHONCE_in_match_attribute587 = new BitSet(new long[]{0x0000000000000000L,0x0200000000000000L});
    public static final BitSet FOLLOW_RBRACKET_in_match_attribute589 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_rule_species_def_in_reaction_def632 = new BitSet(new long[]{0x0000000000000000L,0xC000000000000000L,0x0000000000002000L});
    public static final BitSet FOLLOW_PLUS_in_reaction_def677 = new BitSet(new long[]{0x0000000000000000L,0x010A000000000000L,0x0000000000008005L});
    public static final BitSet FOLLOW_rule_species_def_in_reaction_def681 = new BitSet(new long[]{0x0000000000000000L,0xC000000000000000L,0x0000000000002000L});
    public static final BitSet FOLLOW_UNI_REACTION_SIGN_in_reaction_def710 = new BitSet(new long[]{0x0000000000000000L,0x010A000000000000L,0x0000000000008005L});
    public static final BitSet FOLLOW_BI_REACTION_SIGN_in_reaction_def715 = new BitSet(new long[]{0x0000000000000000L,0x010A000000000000L,0x0000000000008005L});
    public static final BitSet FOLLOW_rule_species_def_in_reaction_def726 = new BitSet(new long[]{0x0000000000000002L,0x0000000000000000L,0x0000000000002000L});
    public static final BitSet FOLLOW_PLUS_in_reaction_def751 = new BitSet(new long[]{0x0000000000000000L,0x010A000000000000L,0x0000000000008005L});
    public static final BitSet FOLLOW_rule_species_def_in_reaction_def755 = new BitSet(new long[]{0x0000000000000002L,0x0000000000000000L,0x0000000000002000L});
    public static final BitSet FOLLOW_INT_in_rule_species_def819 = new BitSet(new long[]{0x0000000000000000L,0x0000000000000000L,0x0000000000000800L});
    public static final BitSet FOLLOW_TIMES_in_rule_species_def823 = new BitSet(new long[]{0x0000000000000000L,0x000A000000000000L,0x0000000000008005L});
    public static final BitSet FOLLOW_species_def_in_rule_species_def831 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_INT_in_rule_species_def847 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_function_keyword_in_rate_function922 = new BitSet(new long[]{0x0000000000000000L,0x1000000000000000L});
    public static final BitSet FOLLOW_LPAREN_in_rate_function926 = new BitSet(new long[]{0x0004002000000000L,0x100B0001C0000000L,0x0000000000001000L});
    public static final BitSet FOLLOW_rate_list_in_rate_function928 = new BitSet(new long[]{0x0000000000000000L,0x2000000000000000L});
    public static final BitSet FOLLOW_RPAREN_in_rate_function931 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_rate_list_in_rate_function939 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_SAT_in_function_keyword955 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_expression_in_rate_list983 = new BitSet(new long[]{0x0004002000000002L,0x140B0001C0000000L,0x0000000000001000L});
    public static final BitSet FOLLOW_COMMA_in_rate_list988 = new BitSet(new long[]{0x0004002000000000L,0x100B0001C0000000L,0x0000000000001000L});
    public static final BitSet FOLLOW_expression_in_rate_list993 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_include_command_in_modif_command1041 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_exclude_command_in_modif_command1053 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_set_in_include_command1079 = new BitSet(new long[]{0x0000000000000000L,0x1000000000000000L});
    public static final BitSet FOLLOW_LPAREN_in_include_command1107 = new BitSet(new long[]{0x0000000000000000L,0x0002000000000000L});
    public static final BitSet FOLLOW_INT_in_include_command1109 = new BitSet(new long[]{0x0000000000000000L,0x0400000000000000L});
    public static final BitSet FOLLOW_COMMA_in_include_command1111 = new BitSet(new long[]{0x0000000000000000L,0x000A000000000000L});
    public static final BitSet FOLLOW_pattern_in_include_command1113 = new BitSet(new long[]{0x0000000000000000L,0x2400000000000000L});
    public static final BitSet FOLLOW_COMMA_in_include_command1116 = new BitSet(new long[]{0x0000000000000000L,0x000A000000000000L});
    public static final BitSet FOLLOW_pattern_in_include_command1118 = new BitSet(new long[]{0x0000000000000000L,0x2400000000000000L});
    public static final BitSet FOLLOW_RPAREN_in_include_command1122 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_set_in_exclude_command1146 = new BitSet(new long[]{0x0000000000000000L,0x1000000000000000L});
    public static final BitSet FOLLOW_LPAREN_in_exclude_command1170 = new BitSet(new long[]{0x0000000000000000L,0x0002000000000000L});
    public static final BitSet FOLLOW_INT_in_exclude_command1172 = new BitSet(new long[]{0x0000000000000000L,0x0400000000000000L});
    public static final BitSet FOLLOW_COMMA_in_exclude_command1174 = new BitSet(new long[]{0x0000000000000000L,0x000A000000000000L});
    public static final BitSet FOLLOW_pattern_in_exclude_command1176 = new BitSet(new long[]{0x0000000000000000L,0x2400000000000000L});
    public static final BitSet FOLLOW_COMMA_in_exclude_command1179 = new BitSet(new long[]{0x0000000000000000L,0x000A000000000000L});
    public static final BitSet FOLLOW_pattern_in_exclude_command1181 = new BitSet(new long[]{0x0000000000000000L,0x2400000000000000L});
    public static final BitSet FOLLOW_RPAREN_in_exclude_command1185 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_set_in_pattern0 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_function_keyword_in_synpred1_BNGGrammar_ReactionRules915 = new BitSet(new long[]{0x0000000000000000L,0x1000000000000000L});
    public static final BitSet FOLLOW_LPAREN_in_synpred1_BNGGrammar_ReactionRules917 = new BitSet(new long[]{0x0000000000000002L});

}