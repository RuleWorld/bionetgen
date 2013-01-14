// $ANTLR 3.3 Nov 30, 2010 12:50:56 BNGGrammar_Parameters.g 2013-01-14 14:49:38

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
public class BNGGrammar_BNGGrammar_Parameters extends Parser {
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


        public BNGGrammar_BNGGrammar_Parameters(TokenStream input, BNGGrammar gBNGGrammar) {
            this(input, new RecognizerSharedState(), gBNGGrammar);
        }
        public BNGGrammar_BNGGrammar_Parameters(TokenStream input, RecognizerSharedState state, BNGGrammar gBNGGrammar) {
            super(input, state);
            this.gBNGGrammar = gBNGGrammar;
             
            gParent = gBNGGrammar;
        }
        
    protected StringTemplateGroup templateLib =
      new StringTemplateGroup("BNGGrammar_BNGGrammar_ParametersTemplates", AngleBracketTemplateLexer.class);

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
    public String getGrammarFileName() { return "BNGGrammar_Parameters.g"; }


     public void getParentTemplate(){
     
      this.setTemplateLib(gParent.getTemplateLib());
     }
     
     private void addHidden(){
      ((ChangeableChannelTokenStream)input).addChannel(Token.DEFAULT_CHANNEL);
     }
     
     private void deleteHidden(){
     
     }


    protected static class parameters_block_scope {
        Map<String,Register> lmemory;
    }
    protected Stack parameters_block_stack = new Stack();

    public static class parameters_block_return extends ParserRuleReturnScope {
        public StringTemplate st;
        public Object getTemplate() { return st; }
        public String toString() { return st==null?null:st.toString(); }
    };

    // $ANTLR start "parameters_block"
    // BNGGrammar_Parameters.g:23:1: parameters_block[ Map<String,Register> memory, List parameters] : BEGIN PARAMETERS ( LB )+ ( parameter_def[$memory] ( LB )+ )* END PARAMETERS ( LB )+ ;
    public final BNGGrammar_BNGGrammar_Parameters.parameters_block_return parameters_block(Map<String,Register> memory, List parameters) throws RecognitionException {
        parameters_block_stack.push(new parameters_block_scope());
        BNGGrammar_BNGGrammar_Parameters.parameters_block_return retval = new BNGGrammar_BNGGrammar_Parameters.parameters_block_return();
        retval.start = input.LT(1);

        BNGGrammar_BNGGrammar_Parameters.parameter_def_return parameter_def1 = null;



          getParentTemplate();
          ((parameters_block_scope)parameters_block_stack.peek()).lmemory = memory;
         // System.out.println(gParent.netGrammar);

        try {
            // BNGGrammar_Parameters.g:33:1: ( BEGIN PARAMETERS ( LB )+ ( parameter_def[$memory] ( LB )+ )* END PARAMETERS ( LB )+ )
            // BNGGrammar_Parameters.g:33:3: BEGIN PARAMETERS ( LB )+ ( parameter_def[$memory] ( LB )+ )* END PARAMETERS ( LB )+
            {
            match(input,BEGIN,FOLLOW_BEGIN_in_parameters_block51); 
            match(input,PARAMETERS,FOLLOW_PARAMETERS_in_parameters_block53); 
            // BNGGrammar_Parameters.g:33:20: ( LB )+
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
            	    // BNGGrammar_Parameters.g:33:20: LB
            	    {
            	    match(input,LB,FOLLOW_LB_in_parameters_block55); 

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

            // BNGGrammar_Parameters.g:34:1: ( parameter_def[$memory] ( LB )+ )*
            loop3:
            do {
                int alt3=2;
                int LA3_0 = input.LA(1);

                if ( (LA3_0==INT||LA3_0==STRING) ) {
                    alt3=1;
                }


                switch (alt3) {
            	case 1 :
            	    // BNGGrammar_Parameters.g:34:2: parameter_def[$memory] ( LB )+
            	    {
            	    pushFollow(FOLLOW_parameter_def_in_parameters_block59);
            	    parameter_def1=parameter_def(memory);

            	    state._fsp--;

            	    parameters.add((parameter_def1!=null?parameter_def1.st:null));
            	    // BNGGrammar_Parameters.g:34:63: ( LB )+
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
            	    	    // BNGGrammar_Parameters.g:34:63: LB
            	    	    {
            	    	    match(input,LB,FOLLOW_LB_in_parameters_block64); 

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

            match(input,END,FOLLOW_END_in_parameters_block72); 
            match(input,PARAMETERS,FOLLOW_PARAMETERS_in_parameters_block74); 
            // BNGGrammar_Parameters.g:36:16: ( LB )+
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
            	    // BNGGrammar_Parameters.g:36:16: LB
            	    {
            	    match(input,LB,FOLLOW_LB_in_parameters_block76); 

            	    }
            	    break;

            	default :
            	    if ( cnt4 >= 1 ) break loop4;
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
            parameters_block_stack.pop();
        }
        return retval;
    }
    // $ANTLR end "parameters_block"

    public static class parameter_def_return extends ParserRuleReturnScope {
        public StringTemplate st;
        public Object getTemplate() { return st; }
        public String toString() { return st==null?null:st.toString(); }
    };

    // $ANTLR start "parameter_def"
    // BNGGrammar_Parameters.g:39:1: parameter_def[Map<String,Register> lmemory] : ({...}? INT | ) STRING ( BECOMES )? expression[lmemory] -> parameter_def(id=$STRING.textvalue=$expression.text);
    public final BNGGrammar_BNGGrammar_Parameters.parameter_def_return parameter_def(Map<String,Register> lmemory) throws RecognitionException {
        BNGGrammar_BNGGrammar_Parameters.parameter_def_return retval = new BNGGrammar_BNGGrammar_Parameters.parameter_def_return();
        retval.start = input.LT(1);

        Token STRING2=null;
        BNGGrammar_BNGGrammar_Expression.expression_return expression3 = null;


        try {
            // BNGGrammar_Parameters.g:40:1: ( ({...}? INT | ) STRING ( BECOMES )? expression[lmemory] -> parameter_def(id=$STRING.textvalue=$expression.text))
            // BNGGrammar_Parameters.g:41:1: ({...}? INT | ) STRING ( BECOMES )? expression[lmemory]
            {
            // BNGGrammar_Parameters.g:41:1: ({...}? INT | )
            int alt5=2;
            int LA5_0 = input.LA(1);

            if ( (LA5_0==INT) ) {
                alt5=1;
            }
            else if ( (LA5_0==STRING) ) {
                alt5=2;
            }
            else {
                NoViableAltException nvae =
                    new NoViableAltException("", 5, 0, input);

                throw nvae;
            }
            switch (alt5) {
                case 1 :
                    // BNGGrammar_Parameters.g:41:2: {...}? INT
                    {
                    if ( !((gParent.netGrammar)) ) {
                        throw new FailedPredicateException(input, "parameter_def", "gParent.netGrammar");
                    }
                    match(input,INT,FOLLOW_INT_in_parameter_def90); 

                    }
                    break;
                case 2 :
                    // BNGGrammar_Parameters.g:41:31: 
                    {
                    }
                    break;

            }

            STRING2=(Token)match(input,STRING,FOLLOW_STRING_in_parameter_def97); 
            // BNGGrammar_Parameters.g:41:40: ( BECOMES )?
            int alt6=2;
            int LA6_0 = input.LA(1);

            if ( (LA6_0==BECOMES) ) {
                alt6=1;
            }
            switch (alt6) {
                case 1 :
                    // BNGGrammar_Parameters.g:41:41: BECOMES
                    {
                    match(input,BECOMES,FOLLOW_BECOMES_in_parameter_def100); 

                    }
                    break;

            }

            pushFollow(FOLLOW_expression_in_parameter_def104);
            expression3=gBNGGrammar.expression(lmemory);

            state._fsp--;


                    lmemory.put((STRING2!=null?STRING2.getText():null),new Register((expression3!=null?expression3.value:null),"ConstantExpression"));
                    


            // TEMPLATE REWRITE
            // 43:13: -> parameter_def(id=$STRING.textvalue=$expression.text)
            {
                retval.st = templateLib.getInstanceOf("parameter_def",
              new STAttrMap().put("id", (STRING2!=null?STRING2.getText():null)).put("value", (expression3!=null?input.toString(expression3.start,expression3.stop):null)));
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
    // $ANTLR end "parameter_def"

    // Delegated rules


 

    public static final BitSet FOLLOW_BEGIN_in_parameters_block51 = new BitSet(new long[]{0x0000020000000000L});
    public static final BitSet FOLLOW_PARAMETERS_in_parameters_block53 = new BitSet(new long[]{0x0000000000000020L});
    public static final BitSet FOLLOW_LB_in_parameters_block55 = new BitSet(new long[]{0x0000000000000120L,0x000A000000000000L});
    public static final BitSet FOLLOW_parameter_def_in_parameters_block59 = new BitSet(new long[]{0x0000000000000020L});
    public static final BitSet FOLLOW_LB_in_parameters_block64 = new BitSet(new long[]{0x0000000000000120L,0x000A000000000000L});
    public static final BitSet FOLLOW_END_in_parameters_block72 = new BitSet(new long[]{0x0000020000000000L});
    public static final BitSet FOLLOW_PARAMETERS_in_parameters_block74 = new BitSet(new long[]{0x0000000000000020L});
    public static final BitSet FOLLOW_LB_in_parameters_block76 = new BitSet(new long[]{0x0000000000000022L});
    public static final BitSet FOLLOW_INT_in_parameter_def90 = new BitSet(new long[]{0x0000000000000000L,0x0008000000000000L});
    public static final BitSet FOLLOW_STRING_in_parameter_def97 = new BitSet(new long[]{0x0004002000000000L,0x100B000180000000L,0x0000000000001200L});
    public static final BitSet FOLLOW_BECOMES_in_parameter_def100 = new BitSet(new long[]{0x0004002000000000L,0x100B000180000000L,0x0000000000001200L});
    public static final BitSet FOLLOW_expression_in_parameter_def104 = new BitSet(new long[]{0x0000000000000002L});

}