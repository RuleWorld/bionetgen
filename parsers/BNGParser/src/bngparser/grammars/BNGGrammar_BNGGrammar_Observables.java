// $ANTLR 3.3 Nov 30, 2010 12:50:56 BNGGrammar_Observables.g 2013-01-14 14:49:39

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
public class BNGGrammar_BNGGrammar_Observables extends Parser {
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


        public BNGGrammar_BNGGrammar_Observables(TokenStream input, BNGGrammar gBNGGrammar) {
            this(input, new RecognizerSharedState(), gBNGGrammar);
        }
        public BNGGrammar_BNGGrammar_Observables(TokenStream input, RecognizerSharedState state, BNGGrammar gBNGGrammar) {
            super(input, state);
            this.gBNGGrammar = gBNGGrammar;
             
            gParent = gBNGGrammar;
        }
        
    protected StringTemplateGroup templateLib =
      new StringTemplateGroup("BNGGrammar_BNGGrammar_ObservablesTemplates", AngleBracketTemplateLexer.class);

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
    public String getGrammarFileName() { return "BNGGrammar_Observables.g"; }


     public void getParentTemplate(){
     
      this.setTemplateLib(gParent.getTemplateLib());

     }
         @Override
      public String getErrorMessage(RecognitionException e,String[] tokenNames){
        return gParent.getErrorMessage(e,tokenNames);
      }


    protected static class observables_block_scope {
        int observableCounter;
        ReactionAction reactionAction;
    }
    protected Stack observables_block_stack = new Stack();

    public static class observables_block_return extends ParserRuleReturnScope {
        public StringTemplate st;
        public Object getTemplate() { return st; }
        public String toString() { return st==null?null:st.toString(); }
    };

    // $ANTLR start "observables_block"
    // BNGGrammar_Observables.g:20:1: observables_block[List observables] : BEGIN OBSERVABLES ( LB )+ (s1= observable_def_line[\"O\" + $observables_block::observableCounter] ( LB )+ )* END OBSERVABLES ( LB )* ;
    public final BNGGrammar_BNGGrammar_Observables.observables_block_return observables_block(List observables) throws RecognitionException {
        observables_block_stack.push(new observables_block_scope());
        BNGGrammar_BNGGrammar_Observables.observables_block_return retval = new BNGGrammar_BNGGrammar_Observables.observables_block_return();
        retval.start = input.LT(1);

        BNGGrammar_BNGGrammar_Observables.observable_def_line_return s1 = null;



        getParentTemplate();
        gParent.paraphrases.push("in the observables section");
          ((observables_block_scope)observables_block_stack.peek()).observableCounter = 1;
          ((observables_block_scope)observables_block_stack.peek()).reactionAction = new ReactionAction();

        try {
            // BNGGrammar_Observables.g:35:9: ( BEGIN OBSERVABLES ( LB )+ (s1= observable_def_line[\"O\" + $observables_block::observableCounter] ( LB )+ )* END OBSERVABLES ( LB )* )
            // BNGGrammar_Observables.g:35:11: BEGIN OBSERVABLES ( LB )+ (s1= observable_def_line[\"O\" + $observables_block::observableCounter] ( LB )+ )* END OBSERVABLES ( LB )*
            {
            match(input,BEGIN,FOLLOW_BEGIN_in_observables_block62); 
            match(input,OBSERVABLES,FOLLOW_OBSERVABLES_in_observables_block64); 
            // BNGGrammar_Observables.g:35:29: ( LB )+
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
            	    // BNGGrammar_Observables.g:35:29: LB
            	    {
            	    match(input,LB,FOLLOW_LB_in_observables_block66); 

            	    }
            	    break;

            	default :
            	    if ( cnt1 >= 1 ) break loop1;
                        EarlyExitException eee =
                            new EarlyExitException(1, input);
                        throw eee;
                }
                cnt1++;
            } while (true);

            // BNGGrammar_Observables.g:35:32: (s1= observable_def_line[\"O\" + $observables_block::observableCounter] ( LB )+ )*
            loop3:
            do {
                int alt3=2;
                int LA3_0 = input.LA(1);

                if ( (LA3_0==SPECIES||LA3_0==MOLECULES||LA3_0==INT||LA3_0==STRING) ) {
                    alt3=1;
                }


                switch (alt3) {
            	case 1 :
            	    // BNGGrammar_Observables.g:35:33: s1= observable_def_line[\"O\" + $observables_block::observableCounter] ( LB )+
            	    {
            	    pushFollow(FOLLOW_observable_def_line_in_observables_block71);
            	    s1=observable_def_line("O" + ((observables_block_scope)observables_block_stack.peek()).observableCounter);

            	    state._fsp--;

            	    observables.add(s1.st);((observables_block_scope)observables_block_stack.peek()).observableCounter++;
            	    // BNGGrammar_Observables.g:36:80: ( LB )+
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
            	    	    // BNGGrammar_Observables.g:36:80: LB
            	    	    {
            	    	    match(input,LB,FOLLOW_LB_in_observables_block90); 

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


            	    }
            	    break;

            	default :
            	    break loop3;
                }
            } while (true);

            match(input,END,FOLLOW_END_in_observables_block95); 
            match(input,OBSERVABLES,FOLLOW_OBSERVABLES_in_observables_block97); 
            // BNGGrammar_Observables.g:36:102: ( LB )*
            loop4:
            do {
                int alt4=2;
                int LA4_0 = input.LA(1);

                if ( (LA4_0==LB) ) {
                    alt4=1;
                }


                switch (alt4) {
            	case 1 :
            	    // BNGGrammar_Observables.g:36:102: LB
            	    {
            	    match(input,LB,FOLLOW_LB_in_observables_block99); 

            	    }
            	    break;

            	default :
            	    break loop4;
                }
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
            observables_block_stack.pop();
        }
        return retval;
    }
    // $ANTLR end "observables_block"

    protected static class observable_def_line_scope {
        List<StringTemplate> pattern;
    }
    protected Stack observable_def_line_stack = new Stack();

    public static class observable_def_line_return extends ParserRuleReturnScope {
        public StringTemplate st;
        public Object getTemplate() { return st; }
        public String toString() { return st==null?null:st.toString(); }
    };

    // $ANTLR start "observable_def_line"
    // BNGGrammar_Observables.g:38:1: observable_def_line[String upperID] : ({...}? INT | ) (s1= observable_type )? STRING ({...}? LBRACKET MATCHONCE RBRACKET | ) ( pattern_list[upperID] ) -> observables_block(id=upperIDtype=$observable_type.textpatterns=$observable_def_line::patternname=$STRING.text);
    public final BNGGrammar_BNGGrammar_Observables.observable_def_line_return observable_def_line(String upperID) throws RecognitionException {
        observable_def_line_stack.push(new observable_def_line_scope());
        BNGGrammar_BNGGrammar_Observables.observable_def_line_return retval = new BNGGrammar_BNGGrammar_Observables.observable_def_line_return();
        retval.start = input.LT(1);

        Token STRING1=null;
        BNGGrammar_BNGGrammar_Observables.observable_type_return s1 = null;

        BNGGrammar_BNGGrammar_Observables.pattern_list_return pattern_list2 = null;



          ((observable_def_line_scope)observable_def_line_stack.peek()).pattern = new ArrayList<StringTemplate>();


        try {
            // BNGGrammar_Observables.g:47:9: ( ({...}? INT | ) (s1= observable_type )? STRING ({...}? LBRACKET MATCHONCE RBRACKET | ) ( pattern_list[upperID] ) -> observables_block(id=upperIDtype=$observable_type.textpatterns=$observable_def_line::patternname=$STRING.text))
            // BNGGrammar_Observables.g:48:9: ({...}? INT | ) (s1= observable_type )? STRING ({...}? LBRACKET MATCHONCE RBRACKET | ) ( pattern_list[upperID] )
            {
            // BNGGrammar_Observables.g:48:9: ({...}? INT | )
            int alt5=2;
            int LA5_0 = input.LA(1);

            if ( (LA5_0==INT) ) {
                alt5=1;
            }
            else if ( (LA5_0==SPECIES||LA5_0==MOLECULES||LA5_0==STRING) ) {
                alt5=2;
            }
            else {
                NoViableAltException nvae =
                    new NoViableAltException("", 5, 0, input);

                throw nvae;
            }
            switch (alt5) {
                case 1 :
                    // BNGGrammar_Observables.g:48:10: {...}? INT
                    {
                    if ( !((gParent.netGrammar)) ) {
                        throw new FailedPredicateException(input, "observable_def_line", "gParent.netGrammar");
                    }
                    match(input,INT,FOLLOW_INT_in_observable_def_line144); 

                    }
                    break;
                case 2 :
                    // BNGGrammar_Observables.g:48:38: 
                    {
                    }
                    break;

            }

            // BNGGrammar_Observables.g:49:9: (s1= observable_type )?
            int alt6=2;
            int LA6_0 = input.LA(1);

            if ( (LA6_0==SPECIES||LA6_0==MOLECULES) ) {
                alt6=1;
            }
            switch (alt6) {
                case 1 :
                    // BNGGrammar_Observables.g:49:10: s1= observable_type
                    {
                    pushFollow(FOLLOW_observable_type_in_observable_def_line161);
                    s1=observable_type();

                    state._fsp--;


                    }
                    break;

            }

            STRING1=(Token)match(input,STRING,FOLLOW_STRING_in_observable_def_line165); 
            gParent.memory.put((STRING1!=null?STRING1.getText():null),new Register(0.0,"observable"));
            // BNGGrammar_Observables.g:50:9: ({...}? LBRACKET MATCHONCE RBRACKET | )
            int alt7=2;
            int LA7_0 = input.LA(1);

            if ( (LA7_0==LBRACKET) ) {
                alt7=1;
            }
            else if ( (LA7_0==STRING||LA7_0==DOLLAR||LA7_0==AT||LA7_0==MOD) ) {
                alt7=2;
            }
            else {
                NoViableAltException nvae =
                    new NoViableAltException("", 7, 0, input);

                throw nvae;
            }
            switch (alt7) {
                case 1 :
                    // BNGGrammar_Observables.g:50:10: {...}? LBRACKET MATCHONCE RBRACKET
                    {
                    if ( !((gParent.netGrammar)) ) {
                        throw new FailedPredicateException(input, "observable_def_line", "gParent.netGrammar");
                    }
                    match(input,LBRACKET,FOLLOW_LBRACKET_in_observable_def_line181); 
                    match(input,MATCHONCE,FOLLOW_MATCHONCE_in_observable_def_line183); 
                    match(input,RBRACKET,FOLLOW_RBRACKET_in_observable_def_line185); 

                    }
                    break;
                case 2 :
                    // BNGGrammar_Observables.g:50:62: 
                    {
                    }
                    break;

            }

            // BNGGrammar_Observables.g:50:64: ( pattern_list[upperID] )
            // BNGGrammar_Observables.g:50:65: pattern_list[upperID]
            {
            pushFollow(FOLLOW_pattern_list_in_observable_def_line192);
            pattern_list2=pattern_list(upperID);

            state._fsp--;

             ((observable_def_line_scope)observable_def_line_stack.peek()).pattern.add((pattern_list2!=null?pattern_list2.st:null));

            }



            // TEMPLATE REWRITE
            // 51:9: -> observables_block(id=upperIDtype=$observable_type.textpatterns=$observable_def_line::patternname=$STRING.text)
            {
                retval.st = templateLib.getInstanceOf("observables_block",
              new STAttrMap().put("id", upperID).put("type", (s1!=null?input.toString(s1.start,s1.stop):null)).put("patterns", ((observable_def_line_scope)observable_def_line_stack.peek()).pattern).put("name", (STRING1!=null?STRING1.getText():null)));
            }


            }

            retval.stop = input.LT(-1);

        }
        catch (RecognitionException re) {
            reportError(re);
            recover(input,re);
        }
        finally {
            observable_def_line_stack.pop();
        }
        return retval;
    }
    // $ANTLR end "observable_def_line"

    public static class observable_type_return extends ParserRuleReturnScope {
        public StringTemplate st;
        public Object getTemplate() { return st; }
        public String toString() { return st==null?null:st.toString(); }
    };

    // $ANTLR start "observable_type"
    // BNGGrammar_Observables.g:53:1: observable_type : ( ( MOLECULES ) | ( SPECIES ) );
    public final BNGGrammar_BNGGrammar_Observables.observable_type_return observable_type() throws RecognitionException {
        BNGGrammar_BNGGrammar_Observables.observable_type_return retval = new BNGGrammar_BNGGrammar_Observables.observable_type_return();
        retval.start = input.LT(1);

        try {
            // BNGGrammar_Observables.g:54:9: ( ( MOLECULES ) | ( SPECIES ) )
            int alt8=2;
            int LA8_0 = input.LA(1);

            if ( (LA8_0==MOLECULES) ) {
                alt8=1;
            }
            else if ( (LA8_0==SPECIES) ) {
                alt8=2;
            }
            else {
                NoViableAltException nvae =
                    new NoViableAltException("", 8, 0, input);

                throw nvae;
            }
            switch (alt8) {
                case 1 :
                    // BNGGrammar_Observables.g:54:11: ( MOLECULES )
                    {
                    // BNGGrammar_Observables.g:54:11: ( MOLECULES )
                    // BNGGrammar_Observables.g:54:12: MOLECULES
                    {
                    match(input,MOLECULES,FOLLOW_MOLECULES_in_observable_type250); 

                    }


                    }
                    break;
                case 2 :
                    // BNGGrammar_Observables.g:54:25: ( SPECIES )
                    {
                    // BNGGrammar_Observables.g:54:25: ( SPECIES )
                    // BNGGrammar_Observables.g:54:26: SPECIES
                    {
                    match(input,SPECIES,FOLLOW_SPECIES_in_observable_type256); 

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
    // $ANTLR end "observable_type"

    protected static class pattern_list_scope {
        List patterns;
        int patternCounter;
        List<String> relationships;
        List<String> quantity;
    }
    protected Stack pattern_list_stack = new Stack();

    public static class pattern_list_return extends ParserRuleReturnScope {
        public StringTemplate st;
        public Object getTemplate() { return st; }
        public String toString() { return st==null?null:st.toString(); }
    };

    // $ANTLR start "pattern_list"
    // BNGGrammar_Observables.g:57:1: pattern_list[String upperID] : (s1= obs_species_def[upperID + \"_P\" + $pattern_list::patternCounter, $observables_block::reactionAction] ( ( COMMA )? s2= obs_species_def[upperID + \"_P\" + $pattern_list::patternCounter, $observables_block::reactionAction] )* ) -> patterns_block(id=upperID + \"_P\" + $pattern_list::patternCountermolecules=$pattern_list::patternsrelationship=$pattern_list::relationshipsquantity=$pattern_list::quantity);
    public final BNGGrammar_BNGGrammar_Observables.pattern_list_return pattern_list(String upperID) throws RecognitionException {
        pattern_list_stack.push(new pattern_list_scope());
        BNGGrammar_BNGGrammar_Observables.pattern_list_return retval = new BNGGrammar_BNGGrammar_Observables.pattern_list_return();
        retval.start = input.LT(1);

        BNGGrammar_BNGGrammar_Observables.obs_species_def_return s1 = null;

        BNGGrammar_BNGGrammar_Observables.obs_species_def_return s2 = null;



          ((pattern_list_scope)pattern_list_stack.peek()).patterns = new ArrayList();
          ((pattern_list_scope)pattern_list_stack.peek()).patternCounter = 1;
          ((pattern_list_scope)pattern_list_stack.peek()).relationships = new ArrayList<String>();
          ((pattern_list_scope)pattern_list_stack.peek()).quantity = new ArrayList<String>();
           

          // ((ChangeableChannelTokenStream)input).addChannel(0);

        try {
            // BNGGrammar_Observables.g:75:9: ( (s1= obs_species_def[upperID + \"_P\" + $pattern_list::patternCounter, $observables_block::reactionAction] ( ( COMMA )? s2= obs_species_def[upperID + \"_P\" + $pattern_list::patternCounter, $observables_block::reactionAction] )* ) -> patterns_block(id=upperID + \"_P\" + $pattern_list::patternCountermolecules=$pattern_list::patternsrelationship=$pattern_list::relationshipsquantity=$pattern_list::quantity))
            // BNGGrammar_Observables.g:75:11: (s1= obs_species_def[upperID + \"_P\" + $pattern_list::patternCounter, $observables_block::reactionAction] ( ( COMMA )? s2= obs_species_def[upperID + \"_P\" + $pattern_list::patternCounter, $observables_block::reactionAction] )* )
            {
            // BNGGrammar_Observables.g:75:11: (s1= obs_species_def[upperID + \"_P\" + $pattern_list::patternCounter, $observables_block::reactionAction] ( ( COMMA )? s2= obs_species_def[upperID + \"_P\" + $pattern_list::patternCounter, $observables_block::reactionAction] )* )
            // BNGGrammar_Observables.g:75:12: s1= obs_species_def[upperID + \"_P\" + $pattern_list::patternCounter, $observables_block::reactionAction] ( ( COMMA )? s2= obs_species_def[upperID + \"_P\" + $pattern_list::patternCounter, $observables_block::reactionAction] )*
            {
            pushFollow(FOLLOW_obs_species_def_in_pattern_list303);
            s1=obs_species_def(upperID + "_P" + ((pattern_list_scope)pattern_list_stack.peek()).patternCounter, ((observables_block_scope)observables_block_stack.peek()).reactionAction);

            state._fsp--;


            	        ((pattern_list_scope)pattern_list_stack.peek()).patterns.add(s1.st);
            	        ((pattern_list_scope)pattern_list_stack.peek()).patternCounter++;
            	        if((s1!=null?s1.relationship:null).equals("")){
            	          ((pattern_list_scope)pattern_list_stack.peek()).relationships.add("");
            	          ((pattern_list_scope)pattern_list_stack.peek()).quantity.add("");
            	        }
            	        else{
            	          ((pattern_list_scope)pattern_list_stack.peek()).relationships.add((s1!=null?s1.relationship:null));
                        ((pattern_list_scope)pattern_list_stack.peek()).quantity.add((s1!=null?s1.quantity:null));
            	        }
            	        
            	        
                    
            // BNGGrammar_Observables.g:90:9: ( ( COMMA )? s2= obs_species_def[upperID + \"_P\" + $pattern_list::patternCounter, $observables_block::reactionAction] )*
            loop10:
            do {
                int alt10=2;
                int LA10_0 = input.LA(1);

                if ( (LA10_0==STRING||LA10_0==COMMA||LA10_0==DOLLAR||LA10_0==AT||LA10_0==MOD) ) {
                    alt10=1;
                }


                switch (alt10) {
            	case 1 :
            	    // BNGGrammar_Observables.g:90:10: ( COMMA )? s2= obs_species_def[upperID + \"_P\" + $pattern_list::patternCounter, $observables_block::reactionAction]
            	    {
            	    // BNGGrammar_Observables.g:90:10: ( COMMA )?
            	    int alt9=2;
            	    int LA9_0 = input.LA(1);

            	    if ( (LA9_0==COMMA) ) {
            	        alt9=1;
            	    }
            	    switch (alt9) {
            	        case 1 :
            	            // BNGGrammar_Observables.g:90:11: COMMA
            	            {
            	            match(input,COMMA,FOLLOW_COMMA_in_pattern_list327); 

            	            }
            	            break;

            	    }

            	    pushFollow(FOLLOW_obs_species_def_in_pattern_list333);
            	    s2=obs_species_def(upperID + "_P" + ((pattern_list_scope)pattern_list_stack.peek()).patternCounter, ((observables_block_scope)observables_block_stack.peek()).reactionAction);

            	    state._fsp--;


            	    	        ((pattern_list_scope)pattern_list_stack.peek()).patterns.add(s2.st);
            	    	        ((pattern_list_scope)pattern_list_stack.peek()).patternCounter++; 
            	    	        if((s2!=null?s2.relationship:null).equals("")){
            	                ((pattern_list_scope)pattern_list_stack.peek()).relationships.add("");
            	                ((pattern_list_scope)pattern_list_stack.peek()).quantity.add("");
            	              }
            	              else{
            	                ((pattern_list_scope)pattern_list_stack.peek()).relationships.add((s2!=null?s2.relationship:null));
            	                ((pattern_list_scope)pattern_list_stack.peek()).quantity.add((s2!=null?s2.quantity:null));
            	                
            	              }
            	            

            	    }
            	    break;

            	default :
            	    break loop10;
                }
            } while (true);


            }



            // TEMPLATE REWRITE
            // 104:8: -> patterns_block(id=upperID + \"_P\" + $pattern_list::patternCountermolecules=$pattern_list::patternsrelationship=$pattern_list::relationshipsquantity=$pattern_list::quantity)
            {
                retval.st = templateLib.getInstanceOf("patterns_block",
              new STAttrMap().put("id", upperID + "_P" + ((pattern_list_scope)pattern_list_stack.peek()).patternCounter).put("molecules", ((pattern_list_scope)pattern_list_stack.peek()).patterns).put("relationship", ((pattern_list_scope)pattern_list_stack.peek()).relationships).put("quantity", ((pattern_list_scope)pattern_list_stack.peek()).quantity));
            }


            }

            retval.stop = input.LT(-1);

        }
        catch (RecognitionException re) {
            reportError(re);
            recover(input,re);
        }
        finally {
            pattern_list_stack.pop();
        }
        return retval;
    }
    // $ANTLR end "pattern_list"

    protected static class obs_species_def_scope {
        List reactants;
        BondList bonds;
    }
    protected Stack obs_species_def_stack = new Stack();

    public static class obs_species_def_return extends ParserRuleReturnScope {
        public int stoichiometry;
        public Map <String,List<ReactionRegister>> map;
        public String relationship = "";
        public String quantity = "";
        public StringTemplate st;
        public Object getTemplate() { return st; }
        public String toString() { return st==null?null:st.toString(); }
    };

    // $ANTLR start "obs_species_def"
    // BNGGrammar_Observables.g:109:2: obs_species_def[String upperID,ReactionAction reactionAction] returns [int stoichiometry,Map <String,List<ReactionRegister>> map, \n String relationship = \"\", String quantity = \"\"] : s1= ( species_def[$obs_species_def::reactants,$obs_species_def::bonds,upperID] ) ( relational_observer )? -> rule_seed_species_block(id=upperIDmolecules=$obs_species_def::reactantsfirstBonds=$obs_species_def::bonds.getLeft()secondBonds=$obs_species_def::bonds.getRight());
    public final BNGGrammar_BNGGrammar_Observables.obs_species_def_return obs_species_def(String upperID, ReactionAction reactionAction) throws RecognitionException {
        obs_species_def_stack.push(new obs_species_def_scope());
        BNGGrammar_BNGGrammar_Observables.obs_species_def_return retval = new BNGGrammar_BNGGrammar_Observables.obs_species_def_return();
        retval.start = input.LT(1);

        Token s1=null;
        BNGGrammar_BNGGrammar_SeedSpecies.species_def_return species_def3 = null;

        BNGGrammar_BNGGrammar_Observables.relational_observer_return relational_observer4 = null;



          ((obs_species_def_scope)obs_species_def_stack.peek()).reactants = new ArrayList();
          ((obs_species_def_scope)obs_species_def_stack.peek()).bonds = new BondList();
          retval.stoichiometry = 1;

        try {
            // BNGGrammar_Observables.g:120:1: (s1= ( species_def[$obs_species_def::reactants,$obs_species_def::bonds,upperID] ) ( relational_observer )? -> rule_seed_species_block(id=upperIDmolecules=$obs_species_def::reactantsfirstBonds=$obs_species_def::bonds.getLeft()secondBonds=$obs_species_def::bonds.getRight()))
            // BNGGrammar_Observables.g:122:2: s1= ( species_def[$obs_species_def::reactants,$obs_species_def::bonds,upperID] ) ( relational_observer )?
            {
            // BNGGrammar_Observables.g:122:5: ( species_def[$obs_species_def::reactants,$obs_species_def::bonds,upperID] )
            // BNGGrammar_Observables.g:122:6: species_def[$obs_species_def::reactants,$obs_species_def::bonds,upperID]
            {
            pushFollow(FOLLOW_species_def_in_obs_species_def422);
            species_def3=gBNGGrammar.species_def(((obs_species_def_scope)obs_species_def_stack.peek()).reactants, ((obs_species_def_scope)obs_species_def_stack.peek()).bonds, upperID);

            state._fsp--;


                   reactionAction.addMolecule(upperID,(species_def3!=null?input.toString(species_def3.start,species_def3.stop):null),((obs_species_def_scope)obs_species_def_stack.peek()).bonds);
                   retval.map = (species_def3!=null?species_def3.listOfMolecules:null);
              

            }

            // BNGGrammar_Observables.g:126:6: ( relational_observer )?
            int alt11=2;
            int LA11_0 = input.LA(1);

            if ( ((LA11_0>=GTE && LA11_0<=LT)||LA11_0==EQUALS) ) {
                alt11=1;
            }
            switch (alt11) {
                case 1 :
                    // BNGGrammar_Observables.g:126:7: relational_observer
                    {
                    pushFollow(FOLLOW_relational_observer_in_obs_species_def432);
                    relational_observer4=relational_observer();

                    state._fsp--;


                        retval.relationship = (relational_observer4!=null?relational_observer4.relationship:null);
                        retval.quantity = (relational_observer4!=null?relational_observer4.quantity:null);
                      

                    }
                    break;

            }



            // TEMPLATE REWRITE
            // 135:5: -> rule_seed_species_block(id=upperIDmolecules=$obs_species_def::reactantsfirstBonds=$obs_species_def::bonds.getLeft()secondBonds=$obs_species_def::bonds.getRight())
            {
                retval.st = templateLib.getInstanceOf("rule_seed_species_block",
              new STAttrMap().put("id", upperID).put("molecules", ((obs_species_def_scope)obs_species_def_stack.peek()).reactants).put("firstBonds", ((obs_species_def_scope)obs_species_def_stack.peek()).bonds.getLeft()).put("secondBonds", ((obs_species_def_scope)obs_species_def_stack.peek()).bonds.getRight()));
            }


            }

            retval.stop = input.LT(-1);

        }
        catch (RecognitionException re) {
            reportError(re);
            recover(input,re);
        }
        finally {
            obs_species_def_stack.pop();
        }
        return retval;
    }
    // $ANTLR end "obs_species_def"

    public static class relational_observer_return extends ParserRuleReturnScope {
        public String relationship;
        public String quantity;
        public StringTemplate st;
        public Object getTemplate() { return st; }
        public String toString() { return st==null?null:st.toString(); }
    };

    // $ANTLR start "relational_observer"
    // BNGGrammar_Observables.g:139:1: relational_observer returns [String relationship, String quantity] : s1= ( EQUALS | GT | GTE | LT | LTE ) s2= ( expression2 ) ;
    public final BNGGrammar_BNGGrammar_Observables.relational_observer_return relational_observer() throws RecognitionException {
        BNGGrammar_BNGGrammar_Observables.relational_observer_return retval = new BNGGrammar_BNGGrammar_Observables.relational_observer_return();
        retval.start = input.LT(1);

        Token s1=null;
        Token s2=null;
        BNGGrammar_BNGGrammar_Expression.expression2_return expression25 = null;


        try {
            // BNGGrammar_Observables.g:140:1: (s1= ( EQUALS | GT | GTE | LT | LTE ) s2= ( expression2 ) )
            // BNGGrammar_Observables.g:141:1: s1= ( EQUALS | GT | GTE | LT | LTE ) s2= ( expression2 )
            {
            s1=(Token)input.LT(1);
            if ( (input.LA(1)>=GTE && input.LA(1)<=LT)||input.LA(1)==EQUALS ) {
                input.consume();
                state.errorRecovery=false;
            }
            else {
                MismatchedSetException mse = new MismatchedSetException(null,input);
                throw mse;
            }


            retval.relationship = (s1!=null?s1.getText():null);

            // BNGGrammar_Observables.g:145:7: ( expression2 )
            // BNGGrammar_Observables.g:145:8: expression2
            {
            pushFollow(FOLLOW_expression2_in_relational_observer530);
            expression25=gBNGGrammar.expression2();

            state._fsp--;


            }


             retval.quantity = (expression25!=null?input.toString(expression25.start,expression25.stop):null);
             

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
    // $ANTLR end "relational_observer"

    // Delegated rules


 

    public static final BitSet FOLLOW_BEGIN_in_observables_block62 = new BitSet(new long[]{0x0000000000800000L});
    public static final BitSet FOLLOW_OBSERVABLES_in_observables_block64 = new BitSet(new long[]{0x0000000000000020L});
    public static final BitSet FOLLOW_LB_in_observables_block66 = new BitSet(new long[]{0x0000000000004920L,0x000A000000000000L});
    public static final BitSet FOLLOW_observable_def_line_in_observables_block71 = new BitSet(new long[]{0x0000000000000020L});
    public static final BitSet FOLLOW_LB_in_observables_block90 = new BitSet(new long[]{0x0000000000004920L,0x000A000000000000L});
    public static final BitSet FOLLOW_END_in_observables_block95 = new BitSet(new long[]{0x0000000000800000L});
    public static final BitSet FOLLOW_OBSERVABLES_in_observables_block97 = new BitSet(new long[]{0x0000000000000022L});
    public static final BitSet FOLLOW_LB_in_observables_block99 = new BitSet(new long[]{0x0000000000000022L});
    public static final BitSet FOLLOW_INT_in_observable_def_line144 = new BitSet(new long[]{0x0000000000004800L,0x0008000000000000L});
    public static final BitSet FOLLOW_observable_type_in_observable_def_line161 = new BitSet(new long[]{0x0000000000000000L,0x0008000000000000L});
    public static final BitSet FOLLOW_STRING_in_observable_def_line165 = new BitSet(new long[]{0x0000000000000000L,0x0108000000000000L,0x0000000000008005L});
    public static final BitSet FOLLOW_LBRACKET_in_observable_def_line181 = new BitSet(new long[]{0x0000100000000000L});
    public static final BitSet FOLLOW_MATCHONCE_in_observable_def_line183 = new BitSet(new long[]{0x0000000000000000L,0x0200000000000000L});
    public static final BitSet FOLLOW_RBRACKET_in_observable_def_line185 = new BitSet(new long[]{0x0000000000000000L,0x0108000000000000L,0x0000000000008005L});
    public static final BitSet FOLLOW_pattern_list_in_observable_def_line192 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_MOLECULES_in_observable_type250 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_SPECIES_in_observable_type256 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_obs_species_def_in_pattern_list303 = new BitSet(new long[]{0x0000000000000002L,0x0508000000000000L,0x0000000000008005L});
    public static final BitSet FOLLOW_COMMA_in_pattern_list327 = new BitSet(new long[]{0x0000000000000000L,0x0108000000000000L,0x0000000000008005L});
    public static final BitSet FOLLOW_obs_species_def_in_pattern_list333 = new BitSet(new long[]{0x0000000000000002L,0x0508000000000000L,0x0000000000008005L});
    public static final BitSet FOLLOW_species_def_in_obs_species_def422 = new BitSet(new long[]{0x0000000000000002L,0x0000000000000000L,0x0000000000000178L});
    public static final BitSet FOLLOW_relational_observer_in_obs_species_def432 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_set_in_relational_observer503 = new BitSet(new long[]{0x0004002000000000L,0x100B000180000000L,0x0000000000001000L});
    public static final BitSet FOLLOW_expression2_in_relational_observer530 = new BitSet(new long[]{0x0000000000000002L});

}