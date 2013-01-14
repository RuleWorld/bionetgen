// $ANTLR 3.3 Nov 30, 2010 12:50:56 BNGGrammar_Expression.g 2013-01-14 14:49:38

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
public class BNGGrammar_BNGGrammar_Expression extends Parser {
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


        public BNGGrammar_BNGGrammar_Expression(TokenStream input, BNGGrammar gBNGGrammar) {
            this(input, new RecognizerSharedState(), gBNGGrammar);
        }
        public BNGGrammar_BNGGrammar_Expression(TokenStream input, RecognizerSharedState state, BNGGrammar gBNGGrammar) {
            super(input, state);
            this.gBNGGrammar = gBNGGrammar;
             
            gParent = gBNGGrammar;
        }
        
    protected StringTemplateGroup templateLib =
      new StringTemplateGroup("BNGGrammar_BNGGrammar_ExpressionTemplates", AngleBracketTemplateLexer.class);

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
    public String getGrammarFileName() { return "BNGGrammar_Expression.g"; }


     public void getParentTemplate(){
     
      this.setTemplateLib(gParent.getTemplateLib());
     
     }
      String[] list = {"sin", "cos", "tan","exp","ln","log10","log2","abs","floor","ceil","sqrt","asin","acos","atan","sinh","cosh","tanh","asinh","acosh","atanh"};
          @Override
      public String getErrorMessage(RecognitionException e,String[] tokenNames){
        return gParent.getErrorMessage(e,tokenNames);
      }
      
      public String getErrorMessage(Token s1,String error){
          return String.format("%s line %d:%d %s: %s\n",input.getSourceName(),s1.getLine(),s1.getCharPositionInLine(),error,s1.getText());
      
      }
      
      public boolean isSpecialFunction(String text){
        if(Arrays.asList(list).contains(text))
          return true;
         return false;
      }
      
      


    protected static class expression_scope {
        Map<String,Register> references;
        Map<String,Register> lmemory;
    }
    protected Stack expression_stack = new Stack();

    public static class expression_return extends ParserRuleReturnScope {
        public Double value;
        public Map reference;
        public StringTemplate st;
        public Object getTemplate() { return st; }
        public String toString() { return st==null?null:st.toString(); }
    };

    // $ANTLR start "expression"
    // BNGGrammar_Expression.g:42:1: expression[ Map<String,Register> memory] returns [Double value, Map reference] : expression2 ;
    public final BNGGrammar_BNGGrammar_Expression.expression_return expression(Map<String,Register> memory) throws RecognitionException {
        expression_stack.push(new expression_scope());
        BNGGrammar_BNGGrammar_Expression.expression_return retval = new BNGGrammar_BNGGrammar_Expression.expression_return();
        retval.start = input.LT(1);

        BNGGrammar_BNGGrammar_Expression.expression2_return expression21 = null;



          ((expression_scope)expression_stack.peek()).references = new HashMap<String,Register>();
          ((expression_scope)expression_stack.peek()).lmemory = memory;

        try {
            // BNGGrammar_Expression.g:51:1: ( expression2 )
            // BNGGrammar_Expression.g:52:3: expression2
            {
            pushFollow(FOLLOW_expression2_in_expression70);
            expression21=expression2();

            state._fsp--;
            if (state.failed) return retval;
            if ( state.backtracking==0 ) {
              retval.value = (expression21!=null?expression21.value:null);retval.reference = ((expression_scope)expression_stack.peek()).references;
            }

            }

            retval.stop = input.LT(-1);

        }
        catch (RecognitionException re) {
            reportError(re);
            recover(input,re);
        }
        finally {
            expression_stack.pop();
        }
        return retval;
    }
    // $ANTLR end "expression"

    public static class expression2_return extends ParserRuleReturnScope {
        public Double value;
        public StringTemplate st;
        public Object getTemplate() { return st; }
        public String toString() { return st==null?null:st.toString(); }
    };

    // $ANTLR start "expression2"
    // BNGGrammar_Expression.g:55:1: expression2 returns [Double value] : e1= relational_expression ( ( ( AMPERSAND AMPERSAND ) | ( PIPE PIPE ) ) relational_expression )* ;
    public final BNGGrammar_BNGGrammar_Expression.expression2_return expression2() throws RecognitionException {
        BNGGrammar_BNGGrammar_Expression.expression2_return retval = new BNGGrammar_BNGGrammar_Expression.expression2_return();
        retval.start = input.LT(1);

        BNGGrammar_BNGGrammar_Expression.relational_expression_return e1 = null;


        try {
            // BNGGrammar_Expression.g:56:1: (e1= relational_expression ( ( ( AMPERSAND AMPERSAND ) | ( PIPE PIPE ) ) relational_expression )* )
            // BNGGrammar_Expression.g:57:3: e1= relational_expression ( ( ( AMPERSAND AMPERSAND ) | ( PIPE PIPE ) ) relational_expression )*
            {
            pushFollow(FOLLOW_relational_expression_in_expression292);
            e1=relational_expression();

            state._fsp--;
            if (state.failed) return retval;
            if ( state.backtracking==0 ) {
              retval.value = e1.value; 
            }
            // BNGGrammar_Expression.g:57:50: ( ( ( AMPERSAND AMPERSAND ) | ( PIPE PIPE ) ) relational_expression )*
            loop2:
            do {
                int alt2=2;
                int LA2_0 = input.LA(1);

                if ( (LA2_0==PIPE||LA2_0==AMPERSAND) ) {
                    alt2=1;
                }


                switch (alt2) {
            	case 1 :
            	    // BNGGrammar_Expression.g:57:51: ( ( AMPERSAND AMPERSAND ) | ( PIPE PIPE ) ) relational_expression
            	    {
            	    // BNGGrammar_Expression.g:57:51: ( ( AMPERSAND AMPERSAND ) | ( PIPE PIPE ) )
            	    int alt1=2;
            	    int LA1_0 = input.LA(1);

            	    if ( (LA1_0==AMPERSAND) ) {
            	        alt1=1;
            	    }
            	    else if ( (LA1_0==PIPE) ) {
            	        alt1=2;
            	    }
            	    else {
            	        if (state.backtracking>0) {state.failed=true; return retval;}
            	        NoViableAltException nvae =
            	            new NoViableAltException("", 1, 0, input);

            	        throw nvae;
            	    }
            	    switch (alt1) {
            	        case 1 :
            	            // BNGGrammar_Expression.g:57:52: ( AMPERSAND AMPERSAND )
            	            {
            	            // BNGGrammar_Expression.g:57:52: ( AMPERSAND AMPERSAND )
            	            // BNGGrammar_Expression.g:57:53: AMPERSAND AMPERSAND
            	            {
            	            match(input,AMPERSAND,FOLLOW_AMPERSAND_in_expression299); if (state.failed) return retval;
            	            match(input,AMPERSAND,FOLLOW_AMPERSAND_in_expression2101); if (state.failed) return retval;

            	            }


            	            }
            	            break;
            	        case 2 :
            	            // BNGGrammar_Expression.g:57:75: ( PIPE PIPE )
            	            {
            	            // BNGGrammar_Expression.g:57:75: ( PIPE PIPE )
            	            // BNGGrammar_Expression.g:57:76: PIPE PIPE
            	            {
            	            match(input,PIPE,FOLLOW_PIPE_in_expression2106); if (state.failed) return retval;
            	            match(input,PIPE,FOLLOW_PIPE_in_expression2108); if (state.failed) return retval;

            	            }


            	            }
            	            break;

            	    }

            	    pushFollow(FOLLOW_relational_expression_in_expression2112);
            	    relational_expression();

            	    state._fsp--;
            	    if (state.failed) return retval;

            	    }
            	    break;

            	default :
            	    break loop2;
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
        }
        return retval;
    }
    // $ANTLR end "expression2"

    public static class relational_expression_return extends ParserRuleReturnScope {
        public Double value;
        public StringTemplate st;
        public Object getTemplate() { return st; }
        public String toString() { return st==null?null:st.toString(); }
    };

    // $ANTLR start "relational_expression"
    // BNGGrammar_Expression.g:61:1: relational_expression returns [Double value] : a1= adding_expression ( ( EQUALS | GT | GTE | LT | LTE ) adding_expression )* ;
    public final BNGGrammar_BNGGrammar_Expression.relational_expression_return relational_expression() throws RecognitionException {
        BNGGrammar_BNGGrammar_Expression.relational_expression_return retval = new BNGGrammar_BNGGrammar_Expression.relational_expression_return();
        retval.start = input.LT(1);

        BNGGrammar_BNGGrammar_Expression.adding_expression_return a1 = null;


        try {
            // BNGGrammar_Expression.g:61:45: (a1= adding_expression ( ( EQUALS | GT | GTE | LT | LTE ) adding_expression )* )
            // BNGGrammar_Expression.g:62:3: a1= adding_expression ( ( EQUALS | GT | GTE | LT | LTE ) adding_expression )*
            {
            pushFollow(FOLLOW_adding_expression_in_relational_expression135);
            a1=adding_expression();

            state._fsp--;
            if (state.failed) return retval;
            if ( state.backtracking==0 ) {
              retval.value = a1.value;
            }
            // BNGGrammar_Expression.g:62:44: ( ( EQUALS | GT | GTE | LT | LTE ) adding_expression )*
            loop3:
            do {
                int alt3=2;
                int LA3_0 = input.LA(1);

                if ( ((LA3_0>=GTE && LA3_0<=LT)||LA3_0==EQUALS) ) {
                    alt3=1;
                }


                switch (alt3) {
            	case 1 :
            	    // BNGGrammar_Expression.g:62:45: ( EQUALS | GT | GTE | LT | LTE ) adding_expression
            	    {
            	    if ( (input.LA(1)>=GTE && input.LA(1)<=LT)||input.LA(1)==EQUALS ) {
            	        input.consume();
            	        state.errorRecovery=false;state.failed=false;
            	    }
            	    else {
            	        if (state.backtracking>0) {state.failed=true; return retval;}
            	        MismatchedSetException mse = new MismatchedSetException(null,input);
            	        throw mse;
            	    }

            	    pushFollow(FOLLOW_adding_expression_in_relational_expression151);
            	    adding_expression();

            	    state._fsp--;
            	    if (state.failed) return retval;

            	    }
            	    break;

            	default :
            	    break loop3;
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
        }
        return retval;
    }
    // $ANTLR end "relational_expression"

    public static class adding_expression_return extends ParserRuleReturnScope {
        public Double value;
        public StringTemplate st;
        public Object getTemplate() { return st; }
        public String toString() { return st==null?null:st.toString(); }
    };

    // $ANTLR start "adding_expression"
    // BNGGrammar_Expression.g:65:1: adding_expression returns [Double value] : s1= multiplying_expression ( PLUS s2= multiplying_expression | MINUS s3= multiplying_expression )* ;
    public final BNGGrammar_BNGGrammar_Expression.adding_expression_return adding_expression() throws RecognitionException {
        BNGGrammar_BNGGrammar_Expression.adding_expression_return retval = new BNGGrammar_BNGGrammar_Expression.adding_expression_return();
        retval.start = input.LT(1);

        BNGGrammar_BNGGrammar_Expression.multiplying_expression_return s1 = null;

        BNGGrammar_BNGGrammar_Expression.multiplying_expression_return s2 = null;

        BNGGrammar_BNGGrammar_Expression.multiplying_expression_return s3 = null;


        try {
            // BNGGrammar_Expression.g:66:1: (s1= multiplying_expression ( PLUS s2= multiplying_expression | MINUS s3= multiplying_expression )* )
            // BNGGrammar_Expression.g:66:3: s1= multiplying_expression ( PLUS s2= multiplying_expression | MINUS s3= multiplying_expression )*
            {
            pushFollow(FOLLOW_multiplying_expression_in_adding_expression168);
            s1=multiplying_expression();

            state._fsp--;
            if (state.failed) return retval;
            if ( state.backtracking==0 ) {
              retval.value = s1.value;
            }
            // BNGGrammar_Expression.g:67:1: ( PLUS s2= multiplying_expression | MINUS s3= multiplying_expression )*
            loop4:
            do {
                int alt4=3;
                int LA4_0 = input.LA(1);

                if ( (LA4_0==PLUS) ) {
                    alt4=1;
                }
                else if ( (LA4_0==MINUS) ) {
                    alt4=2;
                }


                switch (alt4) {
            	case 1 :
            	    // BNGGrammar_Expression.g:67:3: PLUS s2= multiplying_expression
            	    {
            	    match(input,PLUS,FOLLOW_PLUS_in_adding_expression175); if (state.failed) return retval;
            	    pushFollow(FOLLOW_multiplying_expression_in_adding_expression179);
            	    s2=multiplying_expression();

            	    state._fsp--;
            	    if (state.failed) return retval;
            	    if ( state.backtracking==0 ) {
            	      retval.value += s2.value;
            	    }

            	    }
            	    break;
            	case 2 :
            	    // BNGGrammar_Expression.g:68:3: MINUS s3= multiplying_expression
            	    {
            	    match(input,MINUS,FOLLOW_MINUS_in_adding_expression185); if (state.failed) return retval;
            	    pushFollow(FOLLOW_multiplying_expression_in_adding_expression189);
            	    s3=multiplying_expression();

            	    state._fsp--;
            	    if (state.failed) return retval;
            	    if ( state.backtracking==0 ) {
            	      retval.value -= s3.value;
            	    }

            	    }
            	    break;

            	default :
            	    break loop4;
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
        }
        return retval;
    }
    // $ANTLR end "adding_expression"

    public static class multiplying_expression_return extends ParserRuleReturnScope {
        public Double value;
        public StringTemplate st;
        public Object getTemplate() { return st; }
        public String toString() { return st==null?null:st.toString(); }
    };

    // $ANTLR start "multiplying_expression"
    // BNGGrammar_Expression.g:71:1: multiplying_expression returns [Double value] : s1= power_expression ( TIMES s2= power_expression | DIV s3= power_expression )* ;
    public final BNGGrammar_BNGGrammar_Expression.multiplying_expression_return multiplying_expression() throws RecognitionException {
        BNGGrammar_BNGGrammar_Expression.multiplying_expression_return retval = new BNGGrammar_BNGGrammar_Expression.multiplying_expression_return();
        retval.start = input.LT(1);

        BNGGrammar_BNGGrammar_Expression.power_expression_return s1 = null;

        BNGGrammar_BNGGrammar_Expression.power_expression_return s2 = null;

        BNGGrammar_BNGGrammar_Expression.power_expression_return s3 = null;


        try {
            // BNGGrammar_Expression.g:72:1: (s1= power_expression ( TIMES s2= power_expression | DIV s3= power_expression )* )
            // BNGGrammar_Expression.g:72:3: s1= power_expression ( TIMES s2= power_expression | DIV s3= power_expression )*
            {
            pushFollow(FOLLOW_power_expression_in_multiplying_expression208);
            s1=power_expression();

            state._fsp--;
            if (state.failed) return retval;
            if ( state.backtracking==0 ) {
              retval.value = s1.value;
            }
            // BNGGrammar_Expression.g:73:1: ( TIMES s2= power_expression | DIV s3= power_expression )*
            loop5:
            do {
                int alt5=3;
                int LA5_0 = input.LA(1);

                if ( (LA5_0==TIMES) ) {
                    alt5=1;
                }
                else if ( (LA5_0==DIV) ) {
                    alt5=2;
                }


                switch (alt5) {
            	case 1 :
            	    // BNGGrammar_Expression.g:73:3: TIMES s2= power_expression
            	    {
            	    match(input,TIMES,FOLLOW_TIMES_in_multiplying_expression215); if (state.failed) return retval;
            	    pushFollow(FOLLOW_power_expression_in_multiplying_expression219);
            	    s2=power_expression();

            	    state._fsp--;
            	    if (state.failed) return retval;
            	    if ( state.backtracking==0 ) {
            	      retval.value *= s2.value;
            	    }

            	    }
            	    break;
            	case 2 :
            	    // BNGGrammar_Expression.g:74:3: DIV s3= power_expression
            	    {
            	    match(input,DIV,FOLLOW_DIV_in_multiplying_expression225); if (state.failed) return retval;
            	    pushFollow(FOLLOW_power_expression_in_multiplying_expression229);
            	    s3=power_expression();

            	    state._fsp--;
            	    if (state.failed) return retval;
            	    if ( state.backtracking==0 ) {
            	      retval.value /= s3.value;
            	    }

            	    }
            	    break;

            	default :
            	    break loop5;
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
        }
        return retval;
    }
    // $ANTLR end "multiplying_expression"

    public static class power_expression_return extends ParserRuleReturnScope {
        public Double value;
        public StringTemplate st;
        public Object getTemplate() { return st; }
        public String toString() { return st==null?null:st.toString(); }
    };

    // $ANTLR start "power_expression"
    // BNGGrammar_Expression.g:79:1: power_expression returns [Double value] : (s1= sign_expression ( POWER s2= sign_expression )* ) ;
    public final BNGGrammar_BNGGrammar_Expression.power_expression_return power_expression() throws RecognitionException {
        BNGGrammar_BNGGrammar_Expression.power_expression_return retval = new BNGGrammar_BNGGrammar_Expression.power_expression_return();
        retval.start = input.LT(1);

        BNGGrammar_BNGGrammar_Expression.sign_expression_return s1 = null;

        BNGGrammar_BNGGrammar_Expression.sign_expression_return s2 = null;


        try {
            // BNGGrammar_Expression.g:80:1: ( (s1= sign_expression ( POWER s2= sign_expression )* ) )
            // BNGGrammar_Expression.g:81:1: (s1= sign_expression ( POWER s2= sign_expression )* )
            {
            // BNGGrammar_Expression.g:81:1: (s1= sign_expression ( POWER s2= sign_expression )* )
            // BNGGrammar_Expression.g:81:2: s1= sign_expression ( POWER s2= sign_expression )*
            {
            pushFollow(FOLLOW_sign_expression_in_power_expression253);
            s1=sign_expression();

            state._fsp--;
            if (state.failed) return retval;
            if ( state.backtracking==0 ) {
              retval.value = s1.value;
            }
            // BNGGrammar_Expression.g:82:1: ( POWER s2= sign_expression )*
            loop6:
            do {
                int alt6=2;
                int LA6_0 = input.LA(1);

                if ( (LA6_0==POWER) ) {
                    alt6=1;
                }


                switch (alt6) {
            	case 1 :
            	    // BNGGrammar_Expression.g:82:2: POWER s2= sign_expression
            	    {
            	    match(input,POWER,FOLLOW_POWER_in_power_expression258); if (state.failed) return retval;
            	    pushFollow(FOLLOW_sign_expression_in_power_expression262);
            	    s2=sign_expression();

            	    state._fsp--;
            	    if (state.failed) return retval;
            	    if ( state.backtracking==0 ) {
            	      retval.value = Math.pow(retval.value,s2.value);
            	    }

            	    }
            	    break;

            	default :
            	    break loop6;
                }
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
    // $ANTLR end "power_expression"

    public static class sign_expression_return extends ParserRuleReturnScope {
        public Double value;
        public StringTemplate st;
        public Object getTemplate() { return st; }
        public String toString() { return st==null?null:st.toString(); }
    };

    // $ANTLR start "sign_expression"
    // BNGGrammar_Expression.g:87:1: sign_expression returns [Double value] : ( ( MINUS ) )* boolean_negation_expression ;
    public final BNGGrammar_BNGGrammar_Expression.sign_expression_return sign_expression() throws RecognitionException {
        BNGGrammar_BNGGrammar_Expression.sign_expression_return retval = new BNGGrammar_BNGGrammar_Expression.sign_expression_return();
        retval.start = input.LT(1);

        BNGGrammar_BNGGrammar_Expression.boolean_negation_expression_return boolean_negation_expression2 = null;


        try {
            // BNGGrammar_Expression.g:89:1: ( ( ( MINUS ) )* boolean_negation_expression )
            // BNGGrammar_Expression.g:89:3: ( ( MINUS ) )* boolean_negation_expression
            {
            if ( state.backtracking==0 ) {
              retval.value = new Double(1);
            }
            // BNGGrammar_Expression.g:89:29: ( ( MINUS ) )*
            loop7:
            do {
                int alt7=2;
                int LA7_0 = input.LA(1);

                if ( (LA7_0==MINUS) ) {
                    alt7=1;
                }


                switch (alt7) {
            	case 1 :
            	    // BNGGrammar_Expression.g:89:30: ( MINUS )
            	    {
            	    // BNGGrammar_Expression.g:89:30: ( MINUS )
            	    // BNGGrammar_Expression.g:89:31: MINUS
            	    {
            	    match(input,MINUS,FOLLOW_MINUS_in_sign_expression287); if (state.failed) return retval;
            	    if ( state.backtracking==0 ) {
            	      retval.value *= -1;
            	    }

            	    }


            	    }
            	    break;

            	default :
            	    break loop7;
                }
            } while (true);

            pushFollow(FOLLOW_boolean_negation_expression_in_sign_expression294);
            boolean_negation_expression2=boolean_negation_expression();

            state._fsp--;
            if (state.failed) return retval;
            if ( state.backtracking==0 ) {
              retval.value *= (boolean_negation_expression2!=null?boolean_negation_expression2.value:null);
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
    // $ANTLR end "sign_expression"

    public static class boolean_negation_expression_return extends ParserRuleReturnScope {
        public Double value;
        public StringTemplate st;
        public Object getTemplate() { return st; }
        public String toString() { return st==null?null:st.toString(); }
    };

    // $ANTLR start "boolean_negation_expression"
    // BNGGrammar_Expression.g:92:1: boolean_negation_expression returns [Double value] : (n= NOT )? e= primitive_element ;
    public final BNGGrammar_BNGGrammar_Expression.boolean_negation_expression_return boolean_negation_expression() throws RecognitionException {
        BNGGrammar_BNGGrammar_Expression.boolean_negation_expression_return retval = new BNGGrammar_BNGGrammar_Expression.boolean_negation_expression_return();
        retval.start = input.LT(1);

        Token n=null;
        BNGGrammar_BNGGrammar_Expression.primitive_element_return e = null;


        try {
            // BNGGrammar_Expression.g:93:1: ( (n= NOT )? e= primitive_element )
            // BNGGrammar_Expression.g:93:3: (n= NOT )? e= primitive_element
            {
            // BNGGrammar_Expression.g:93:3: (n= NOT )?
            int alt8=2;
            int LA8_0 = input.LA(1);

            if ( (LA8_0==NOT) ) {
                alt8=1;
            }
            switch (alt8) {
                case 1 :
                    // BNGGrammar_Expression.g:93:4: n= NOT
                    {
                    n=(Token)match(input,NOT,FOLLOW_NOT_in_boolean_negation_expression312); if (state.failed) return retval;

                    }
                    break;

            }

            pushFollow(FOLLOW_primitive_element_in_boolean_negation_expression318);
            e=primitive_element();

            state._fsp--;
            if (state.failed) return retval;
            if ( state.backtracking==0 ) {
              retval.value = e.value * (n == null ? 1 : -1);
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
    // $ANTLR end "boolean_negation_expression"

    public static class primitive_element_return extends ParserRuleReturnScope {
        public Double value;
        public StringTemplate st;
        public Object getTemplate() { return st; }
        public String toString() { return st==null?null:st.toString(); }
    };

    // $ANTLR start "primitive_element"
    // BNGGrammar_Expression.g:96:1: primitive_element returns [Double value] : ( number | ( IF LPAREN (~ RPAREN )+ RPAREN )=> if_expression | ( STRING LPAREN ( expression2 ( COMMA expression2 )* )? RPAREN )=> function | variable | ( EXP | LOG ) LPAREN e1= expression2 RPAREN | LPAREN e2= expression2 RPAREN );
    public final BNGGrammar_BNGGrammar_Expression.primitive_element_return primitive_element() throws RecognitionException {
        BNGGrammar_BNGGrammar_Expression.primitive_element_return retval = new BNGGrammar_BNGGrammar_Expression.primitive_element_return();
        retval.start = input.LT(1);

        BNGGrammar_BNGGrammar_Expression.expression2_return e1 = null;

        BNGGrammar_BNGGrammar_Expression.expression2_return e2 = null;

        BNGGrammar_BNGGrammar_Expression.number_return number3 = null;

        BNGGrammar_BNGGrammar_Expression.if_expression_return if_expression4 = null;

        BNGGrammar_BNGGrammar_Expression.function_return function5 = null;

        BNGGrammar_BNGGrammar_Expression.variable_return variable6 = null;


        try {
            // BNGGrammar_Expression.g:97:1: ( number | ( IF LPAREN (~ RPAREN )+ RPAREN )=> if_expression | ( STRING LPAREN ( expression2 ( COMMA expression2 )* )? RPAREN )=> function | variable | ( EXP | LOG ) LPAREN e1= expression2 RPAREN | LPAREN e2= expression2 RPAREN )
            int alt9=6;
            int LA9_0 = input.LA(1);

            if ( ((LA9_0>=FLOAT && LA9_0<=INT)) ) {
                alt9=1;
            }
            else if ( (LA9_0==IF) && (synpred1_BNGGrammar_Expression())) {
                alt9=2;
            }
            else if ( (LA9_0==STRING) ) {
                int LA9_4 = input.LA(2);

                if ( (synpred2_BNGGrammar_Expression()) ) {
                    alt9=3;
                }
                else if ( (true) ) {
                    alt9=4;
                }
                else {
                    if (state.backtracking>0) {state.failed=true; return retval;}
                    NoViableAltException nvae =
                        new NoViableAltException("", 9, 4, input);

                    throw nvae;
                }
            }
            else if ( ((LA9_0>=EXP && LA9_0<=LOG)) ) {
                alt9=5;
            }
            else if ( (LA9_0==LPAREN) ) {
                alt9=6;
            }
            else {
                if (state.backtracking>0) {state.failed=true; return retval;}
                NoViableAltException nvae =
                    new NoViableAltException("", 9, 0, input);

                throw nvae;
            }
            switch (alt9) {
                case 1 :
                    // BNGGrammar_Expression.g:98:9: number
                    {
                    pushFollow(FOLLOW_number_in_primitive_element343);
                    number3=number();

                    state._fsp--;
                    if (state.failed) return retval;
                    if ( state.backtracking==0 ) {
                      retval.value = (number3!=null?number3.value:null);
                    }

                    }
                    break;
                case 2 :
                    // BNGGrammar_Expression.g:99:11: ( IF LPAREN (~ RPAREN )+ RPAREN )=> if_expression
                    {
                    pushFollow(FOLLOW_if_expression_in_primitive_element374);
                    if_expression4=if_expression();

                    state._fsp--;
                    if (state.failed) return retval;
                    if ( state.backtracking==0 ) {
                      retval.value = (if_expression4!=null?if_expression4.value:null);
                    }

                    }
                    break;
                case 3 :
                    // BNGGrammar_Expression.g:100:11: ( STRING LPAREN ( expression2 ( COMMA expression2 )* )? RPAREN )=> function
                    {
                    pushFollow(FOLLOW_function_in_primitive_element410);
                    function5=function();

                    state._fsp--;
                    if (state.failed) return retval;
                    if ( state.backtracking==0 ) {
                      retval.value = (function5!=null?function5.value:null);
                    }

                    }
                    break;
                case 4 :
                    // BNGGrammar_Expression.g:101:12: variable
                    {
                    pushFollow(FOLLOW_variable_in_primitive_element425);
                    variable6=variable();

                    state._fsp--;
                    if (state.failed) return retval;
                    if ( state.backtracking==0 ) {
                      retval.value = (variable6!=null?variable6.value:null);
                    }

                    }
                    break;
                case 5 :
                    // BNGGrammar_Expression.g:102:11: ( EXP | LOG ) LPAREN e1= expression2 RPAREN
                    {
                    if ( (input.LA(1)>=EXP && input.LA(1)<=LOG) ) {
                        input.consume();
                        state.errorRecovery=false;state.failed=false;
                    }
                    else {
                        if (state.backtracking>0) {state.failed=true; return retval;}
                        MismatchedSetException mse = new MismatchedSetException(null,input);
                        throw mse;
                    }

                    match(input,LPAREN,FOLLOW_LPAREN_in_primitive_element447); if (state.failed) return retval;
                    pushFollow(FOLLOW_expression2_in_primitive_element451);
                    e1=expression2();

                    state._fsp--;
                    if (state.failed) return retval;
                    match(input,RPAREN,FOLLOW_RPAREN_in_primitive_element453); if (state.failed) return retval;
                    if ( state.backtracking==0 ) {
                      retval.value = Math.exp((e1!=null?e1.value:null));
                    }

                    }
                    break;
                case 6 :
                    // BNGGrammar_Expression.g:103:11: LPAREN e2= expression2 RPAREN
                    {
                    match(input,LPAREN,FOLLOW_LPAREN_in_primitive_element468); if (state.failed) return retval;
                    pushFollow(FOLLOW_expression2_in_primitive_element472);
                    e2=expression2();

                    state._fsp--;
                    if (state.failed) return retval;
                    match(input,RPAREN,FOLLOW_RPAREN_in_primitive_element474); if (state.failed) return retval;
                    if ( state.backtracking==0 ) {
                      retval.value = (e2!=null?e2.value:null);
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
    // $ANTLR end "primitive_element"

    public static class number_return extends ParserRuleReturnScope {
        public Double value;
        public StringTemplate st;
        public Object getTemplate() { return st; }
        public String toString() { return st==null?null:st.toString(); }
    };

    // $ANTLR start "number"
    // BNGGrammar_Expression.g:107:1: number returns [Double value] : (s1= INT | s1= FLOAT );
    public final BNGGrammar_BNGGrammar_Expression.number_return number() throws RecognitionException {
        BNGGrammar_BNGGrammar_Expression.number_return retval = new BNGGrammar_BNGGrammar_Expression.number_return();
        retval.start = input.LT(1);

        Token s1=null;

        try {
            // BNGGrammar_Expression.g:107:30: (s1= INT | s1= FLOAT )
            int alt10=2;
            int LA10_0 = input.LA(1);

            if ( (LA10_0==INT) ) {
                alt10=1;
            }
            else if ( (LA10_0==FLOAT) ) {
                alt10=2;
            }
            else {
                if (state.backtracking>0) {state.failed=true; return retval;}
                NoViableAltException nvae =
                    new NoViableAltException("", 10, 0, input);

                throw nvae;
            }
            switch (alt10) {
                case 1 :
                    // BNGGrammar_Expression.g:107:32: s1= INT
                    {
                    s1=(Token)match(input,INT,FOLLOW_INT_in_number507); if (state.failed) return retval;
                    if ( state.backtracking==0 ) {
                      retval.value = Double.parseDouble((s1!=null?s1.getText():null));
                    }

                    }
                    break;
                case 2 :
                    // BNGGrammar_Expression.g:108:9: s1= FLOAT
                    {
                    s1=(Token)match(input,FLOAT,FOLLOW_FLOAT_in_number521); if (state.failed) return retval;
                    if ( state.backtracking==0 ) {
                       retval.value = Double.parseDouble((s1!=null?s1.getText():null));
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
    // $ANTLR end "number"

    public static class variable_return extends ParserRuleReturnScope {
        public Double value;
        public StringTemplate st;
        public Object getTemplate() { return st; }
        public String toString() { return st==null?null:st.toString(); }
    };

    // $ANTLR start "variable"
    // BNGGrammar_Expression.g:111:1: variable returns [Double value] : s1= STRING ;
    public final BNGGrammar_BNGGrammar_Expression.variable_return variable() throws RecognitionException {
        BNGGrammar_BNGGrammar_Expression.variable_return retval = new BNGGrammar_BNGGrammar_Expression.variable_return();
        retval.start = input.LT(1);

        Token s1=null;

        try {
            // BNGGrammar_Expression.g:111:32: (s1= STRING )
            // BNGGrammar_Expression.g:111:34: s1= STRING
            {
            s1=(Token)match(input,STRING,FOLLOW_STRING_in_variable543); if (state.failed) return retval;
            if ( state.backtracking==0 ) {

                                try{
                                
                                //if(retval.value == null)
                                  //throw [VariableNotFoundException]
                                if(((expression_scope)expression_stack.peek()).lmemory.containsKey((s1!=null?s1.getText():null))){
                                  Register temp = ((expression_scope)expression_stack.peek()).lmemory.get((s1!=null?s1.getText():null));
                                  retval.value = temp.getValue();
              	                  if(!temp.getType().equals("ConstantExpression") && !temp.getType().equals("observable")){
              	                    String msg = getErrorMessage(s1,"the following token is in memory but is not a variable or an observable");
              	                    System.err.println(msg);
              	                  }
                                }
                                else{
                                  retval.value = 1.0;
                                  String msg = getErrorMessage(s1,"variable not found");
                                  System.err.println(msg);
                                }
                                ((expression_scope)expression_stack.peek()).references.put((s1!=null?s1.getText():null),((expression_scope)expression_stack.peek()).lmemory.get((s1!=null?s1.getText():null)));
                                }
                                catch(NullPointerException e){
                                  retval.value = 1.0;
                                  String msg = getErrorMessage(s1,"variable not found");
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
        }
        return retval;
    }
    // $ANTLR end "variable"

    public static class if_expression_return extends ParserRuleReturnScope {
        public Double value;
        public StringTemplate st;
        public Object getTemplate() { return st; }
        public String toString() { return st==null?null:st.toString(); }
    };

    // $ANTLR start "if_expression"
    // BNGGrammar_Expression.g:140:1: if_expression returns [Double value] : IF LPAREN expression2 COMMA e1= expression2 COMMA expression2 RPAREN ;
    public final BNGGrammar_BNGGrammar_Expression.if_expression_return if_expression() throws RecognitionException {
        BNGGrammar_BNGGrammar_Expression.if_expression_return retval = new BNGGrammar_BNGGrammar_Expression.if_expression_return();
        retval.start = input.LT(1);

        BNGGrammar_BNGGrammar_Expression.expression2_return e1 = null;


        try {
            // BNGGrammar_Expression.g:140:38: ( IF LPAREN expression2 COMMA e1= expression2 COMMA expression2 RPAREN )
            // BNGGrammar_Expression.g:141:3: IF LPAREN expression2 COMMA e1= expression2 COMMA expression2 RPAREN
            {
            match(input,IF,FOLLOW_IF_in_if_expression560); if (state.failed) return retval;
            match(input,LPAREN,FOLLOW_LPAREN_in_if_expression562); if (state.failed) return retval;
            pushFollow(FOLLOW_expression2_in_if_expression564);
            expression2();

            state._fsp--;
            if (state.failed) return retval;
            match(input,COMMA,FOLLOW_COMMA_in_if_expression566); if (state.failed) return retval;
            pushFollow(FOLLOW_expression2_in_if_expression570);
            e1=expression2();

            state._fsp--;
            if (state.failed) return retval;
            match(input,COMMA,FOLLOW_COMMA_in_if_expression572); if (state.failed) return retval;
            pushFollow(FOLLOW_expression2_in_if_expression574);
            expression2();

            state._fsp--;
            if (state.failed) return retval;
            match(input,RPAREN,FOLLOW_RPAREN_in_if_expression576); if (state.failed) return retval;
            if ( state.backtracking==0 ) {

                  retval.value = (e1!=null?e1.value:null);
                
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
    // $ANTLR end "if_expression"

    public static class function_return extends ParserRuleReturnScope {
        public Double value;
        public StringTemplate st;
        public Object getTemplate() { return st; }
        public String toString() { return st==null?null:st.toString(); }
    };

    // $ANTLR start "function"
    // BNGGrammar_Expression.g:147:1: function returns [Double value] : s1= STRING LPAREN (e1= ( expression2 ( COMMA expression2 )* ) )? RPAREN ;
    public final BNGGrammar_BNGGrammar_Expression.function_return function() throws RecognitionException {
        BNGGrammar_BNGGrammar_Expression.function_return retval = new BNGGrammar_BNGGrammar_Expression.function_return();
        retval.start = input.LT(1);

        Token s1=null;
        Token e1=null;

        try {
            // BNGGrammar_Expression.g:147:32: (s1= STRING LPAREN (e1= ( expression2 ( COMMA expression2 )* ) )? RPAREN )
            // BNGGrammar_Expression.g:148:3: s1= STRING LPAREN (e1= ( expression2 ( COMMA expression2 )* ) )? RPAREN
            {
            s1=(Token)match(input,STRING,FOLLOW_STRING_in_function596); if (state.failed) return retval;
            match(input,LPAREN,FOLLOW_LPAREN_in_function598); if (state.failed) return retval;
            // BNGGrammar_Expression.g:148:22: (e1= ( expression2 ( COMMA expression2 )* ) )?
            int alt12=2;
            int LA12_0 = input.LA(1);

            if ( (LA12_0==IF||LA12_0==NOT||(LA12_0>=EXP && LA12_0<=LOG)||(LA12_0>=FLOAT && LA12_0<=INT)||LA12_0==STRING||LA12_0==LPAREN||LA12_0==MINUS) ) {
                alt12=1;
            }
            switch (alt12) {
                case 1 :
                    // BNGGrammar_Expression.g:148:22: e1= ( expression2 ( COMMA expression2 )* )
                    {
                    // BNGGrammar_Expression.g:150:3: ( expression2 ( COMMA expression2 )* )
                    // BNGGrammar_Expression.g:150:4: expression2 ( COMMA expression2 )*
                    {
                    pushFollow(FOLLOW_expression2_in_function609);
                    expression2();

                    state._fsp--;
                    if (state.failed) return retval;
                    // BNGGrammar_Expression.g:150:16: ( COMMA expression2 )*
                    loop11:
                    do {
                        int alt11=2;
                        int LA11_0 = input.LA(1);

                        if ( (LA11_0==COMMA) ) {
                            alt11=1;
                        }


                        switch (alt11) {
                    	case 1 :
                    	    // BNGGrammar_Expression.g:150:17: COMMA expression2
                    	    {
                    	    match(input,COMMA,FOLLOW_COMMA_in_function612); if (state.failed) return retval;
                    	    pushFollow(FOLLOW_expression2_in_function614);
                    	    expression2();

                    	    state._fsp--;
                    	    if (state.failed) return retval;

                    	    }
                    	    break;

                    	default :
                    	    break loop11;
                        }
                    } while (true);


                    }


                    }
                    break;

            }

            match(input,RPAREN,FOLLOW_RPAREN_in_function620); if (state.failed) return retval;
            if ( state.backtracking==0 ) {

                  try{
                    if(((expression_scope)expression_stack.peek()).lmemory.containsKey((s1!=null?s1.getText():null))){
                                  Register temp = ((expression_scope)expression_stack.peek()).lmemory.get((s1!=null?s1.getText():null));
                                  retval.value = temp.getValue();
                                  if(!temp.getType().equals("function") && !temp.getType().equals("observable")){
                                    //throw new RuntimeException((s1!=null?s1.getText():null) + "is in memory but it is not a function. Check syntax", new BNGSemanticException((s1!=null?s1.getText():null) + " is in memory but it is not a function. Check syntax",s1.getLine()));
                                    retval.value = 1.0;
                                     String msg = getErrorMessage(s1,"the following token is in memory but is not a function or an observable");
                                    
                                    System.err.println(msg);
                                  }
                     }
                     else if(isSpecialFunction((s1!=null?s1.getText():null))){
                     //add xml code in here
                      retval.value =1.0;
                     }
                     else if((s1!=null?s1.getText():null).equals("MM")){
                      retval.value =1.0;
                     }
                     else{
                                  retval.value = 1.0;
                                  //throw new RuntimeException("function not found: " + (s1!=null?s1.getText():null), new BNGSemanticException("function not found: " + (s1!=null?s1.getText():null),s1.getLine()));
                                  String msg = getErrorMessage(s1,"function not found");
                                  System.err.println(msg);
                     }
                     
                      ((expression_scope)expression_stack.peek()).references.put((s1!=null?s1.getText():null),((expression_scope)expression_stack.peek()).lmemory.get((s1!=null?s1.getText():null)));
                  }
                  catch(NullPointerException e){
                  
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
    // $ANTLR end "function"

    // $ANTLR start synpred1_BNGGrammar_Expression
    public final void synpred1_BNGGrammar_Expression_fragment() throws RecognitionException {   
        // BNGGrammar_Expression.g:99:11: ( IF LPAREN (~ RPAREN )+ RPAREN )
        // BNGGrammar_Expression.g:99:12: IF LPAREN (~ RPAREN )+ RPAREN
        {
        match(input,IF,FOLLOW_IF_in_synpred1_BNGGrammar_Expression359); if (state.failed) return ;
        match(input,LPAREN,FOLLOW_LPAREN_in_synpred1_BNGGrammar_Expression361); if (state.failed) return ;
        // BNGGrammar_Expression.g:99:22: (~ RPAREN )+
        int cnt13=0;
        loop13:
        do {
            int alt13=2;
            int LA13_0 = input.LA(1);

            if ( ((LA13_0>=LINE_COMMENT && LA13_0<=LPAREN)||(LA13_0>=UNI_REACTION_SIGN && LA13_0<=275)) ) {
                alt13=1;
            }


            switch (alt13) {
        	case 1 :
        	    // BNGGrammar_Expression.g:99:23: ~ RPAREN
        	    {
        	    if ( (input.LA(1)>=LINE_COMMENT && input.LA(1)<=LPAREN)||(input.LA(1)>=UNI_REACTION_SIGN && input.LA(1)<=275) ) {
        	        input.consume();
        	        state.errorRecovery=false;state.failed=false;
        	    }
        	    else {
        	        if (state.backtracking>0) {state.failed=true; return ;}
        	        MismatchedSetException mse = new MismatchedSetException(null,input);
        	        throw mse;
        	    }


        	    }
        	    break;

        	default :
        	    if ( cnt13 >= 1 ) break loop13;
        	    if (state.backtracking>0) {state.failed=true; return ;}
                    EarlyExitException eee =
                        new EarlyExitException(13, input);
                    throw eee;
            }
            cnt13++;
        } while (true);

        match(input,RPAREN,FOLLOW_RPAREN_in_synpred1_BNGGrammar_Expression369); if (state.failed) return ;

        }
    }
    // $ANTLR end synpred1_BNGGrammar_Expression

    // $ANTLR start synpred2_BNGGrammar_Expression
    public final void synpred2_BNGGrammar_Expression_fragment() throws RecognitionException {   
        // BNGGrammar_Expression.g:100:11: ( STRING LPAREN ( expression2 ( COMMA expression2 )* )? RPAREN )
        // BNGGrammar_Expression.g:100:12: STRING LPAREN ( expression2 ( COMMA expression2 )* )? RPAREN
        {
        match(input,STRING,FOLLOW_STRING_in_synpred2_BNGGrammar_Expression389); if (state.failed) return ;
        match(input,LPAREN,FOLLOW_LPAREN_in_synpred2_BNGGrammar_Expression391); if (state.failed) return ;
        // BNGGrammar_Expression.g:100:26: ( expression2 ( COMMA expression2 )* )?
        int alt15=2;
        int LA15_0 = input.LA(1);

        if ( (LA15_0==IF||LA15_0==NOT||(LA15_0>=EXP && LA15_0<=LOG)||(LA15_0>=FLOAT && LA15_0<=INT)||LA15_0==STRING||LA15_0==LPAREN||LA15_0==MINUS) ) {
            alt15=1;
        }
        switch (alt15) {
            case 1 :
                // BNGGrammar_Expression.g:100:27: expression2 ( COMMA expression2 )*
                {
                pushFollow(FOLLOW_expression2_in_synpred2_BNGGrammar_Expression394);
                expression2();

                state._fsp--;
                if (state.failed) return ;
                // BNGGrammar_Expression.g:100:39: ( COMMA expression2 )*
                loop14:
                do {
                    int alt14=2;
                    int LA14_0 = input.LA(1);

                    if ( (LA14_0==COMMA) ) {
                        alt14=1;
                    }


                    switch (alt14) {
                	case 1 :
                	    // BNGGrammar_Expression.g:100:40: COMMA expression2
                	    {
                	    match(input,COMMA,FOLLOW_COMMA_in_synpred2_BNGGrammar_Expression397); if (state.failed) return ;
                	    pushFollow(FOLLOW_expression2_in_synpred2_BNGGrammar_Expression399);
                	    expression2();

                	    state._fsp--;
                	    if (state.failed) return ;

                	    }
                	    break;

                	default :
                	    break loop14;
                    }
                } while (true);


                }
                break;

        }

        match(input,RPAREN,FOLLOW_RPAREN_in_synpred2_BNGGrammar_Expression405); if (state.failed) return ;

        }
    }
    // $ANTLR end synpred2_BNGGrammar_Expression

    // Delegated rules

    public final boolean synpred2_BNGGrammar_Expression() {
        state.backtracking++;
        int start = input.mark();
        try {
            synpred2_BNGGrammar_Expression_fragment(); // can never throw exception
        } catch (RecognitionException re) {
            System.err.println("impossible: "+re);
        }
        boolean success = !state.failed;
        input.rewind(start);
        state.backtracking--;
        state.failed=false;
        return success;
    }
    public final boolean synpred1_BNGGrammar_Expression() {
        state.backtracking++;
        int start = input.mark();
        try {
            synpred1_BNGGrammar_Expression_fragment(); // can never throw exception
        } catch (RecognitionException re) {
            System.err.println("impossible: "+re);
        }
        boolean success = !state.failed;
        input.rewind(start);
        state.backtracking--;
        state.failed=false;
        return success;
    }


 

    public static final BitSet FOLLOW_expression2_in_expression70 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_relational_expression_in_expression292 = new BitSet(new long[]{0x0000000000000002L,0x0000000000000000L,0x0000000000110000L});
    public static final BitSet FOLLOW_AMPERSAND_in_expression299 = new BitSet(new long[]{0x0000000000000000L,0x0000000000000000L,0x0000000000100000L});
    public static final BitSet FOLLOW_AMPERSAND_in_expression2101 = new BitSet(new long[]{0x0004002000000000L,0x100B000180000000L,0x0000000000001000L});
    public static final BitSet FOLLOW_PIPE_in_expression2106 = new BitSet(new long[]{0x0000000000000000L,0x0000000000000000L,0x0000000000010000L});
    public static final BitSet FOLLOW_PIPE_in_expression2108 = new BitSet(new long[]{0x0004002000000000L,0x100B000180000000L,0x0000000000001000L});
    public static final BitSet FOLLOW_relational_expression_in_expression2112 = new BitSet(new long[]{0x0000000000000002L,0x0000000000000000L,0x0000000000110000L});
    public static final BitSet FOLLOW_adding_expression_in_relational_expression135 = new BitSet(new long[]{0x0000000000000002L,0x0000000000000000L,0x0000000000000178L});
    public static final BitSet FOLLOW_set_in_relational_expression139 = new BitSet(new long[]{0x0004002000000000L,0x100B000180000000L,0x0000000000001000L});
    public static final BitSet FOLLOW_adding_expression_in_relational_expression151 = new BitSet(new long[]{0x0000000000000002L,0x0000000000000000L,0x0000000000000178L});
    public static final BitSet FOLLOW_multiplying_expression_in_adding_expression168 = new BitSet(new long[]{0x0000000000000002L,0x0000000000000000L,0x0000000000003000L});
    public static final BitSet FOLLOW_PLUS_in_adding_expression175 = new BitSet(new long[]{0x0004002000000000L,0x100B000180000000L,0x0000000000001000L});
    public static final BitSet FOLLOW_multiplying_expression_in_adding_expression179 = new BitSet(new long[]{0x0000000000000002L,0x0000000000000000L,0x0000000000003000L});
    public static final BitSet FOLLOW_MINUS_in_adding_expression185 = new BitSet(new long[]{0x0004002000000000L,0x100B000180000000L,0x0000000000001000L});
    public static final BitSet FOLLOW_multiplying_expression_in_adding_expression189 = new BitSet(new long[]{0x0000000000000002L,0x0000000000000000L,0x0000000000003000L});
    public static final BitSet FOLLOW_power_expression_in_multiplying_expression208 = new BitSet(new long[]{0x0000000000000002L,0x0000000000000000L,0x0000000000000C00L});
    public static final BitSet FOLLOW_TIMES_in_multiplying_expression215 = new BitSet(new long[]{0x0004002000000000L,0x100B000180000000L,0x0000000000001000L});
    public static final BitSet FOLLOW_power_expression_in_multiplying_expression219 = new BitSet(new long[]{0x0000000000000002L,0x0000000000000000L,0x0000000000000C00L});
    public static final BitSet FOLLOW_DIV_in_multiplying_expression225 = new BitSet(new long[]{0x0004002000000000L,0x100B000180000000L,0x0000000000001000L});
    public static final BitSet FOLLOW_power_expression_in_multiplying_expression229 = new BitSet(new long[]{0x0000000000000002L,0x0000000000000000L,0x0000000000000C00L});
    public static final BitSet FOLLOW_sign_expression_in_power_expression253 = new BitSet(new long[]{0x0000000000000002L,0x0000000000000000L,0x0000000000004000L});
    public static final BitSet FOLLOW_POWER_in_power_expression258 = new BitSet(new long[]{0x0004002000000000L,0x100B000180000000L,0x0000000000001000L});
    public static final BitSet FOLLOW_sign_expression_in_power_expression262 = new BitSet(new long[]{0x0000000000000002L,0x0000000000000000L,0x0000000000004000L});
    public static final BitSet FOLLOW_MINUS_in_sign_expression287 = new BitSet(new long[]{0x0004002000000000L,0x100B000180000000L,0x0000000000001000L});
    public static final BitSet FOLLOW_boolean_negation_expression_in_sign_expression294 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_NOT_in_boolean_negation_expression312 = new BitSet(new long[]{0x0004002000000000L,0x100B000180000000L,0x0000000000001000L});
    public static final BitSet FOLLOW_primitive_element_in_boolean_negation_expression318 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_number_in_primitive_element343 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_if_expression_in_primitive_element374 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_function_in_primitive_element410 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_variable_in_primitive_element425 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_set_in_primitive_element439 = new BitSet(new long[]{0x0000000000000000L,0x1000000000000000L});
    public static final BitSet FOLLOW_LPAREN_in_primitive_element447 = new BitSet(new long[]{0x0004002000000000L,0x100B000180000000L,0x0000000000001000L});
    public static final BitSet FOLLOW_expression2_in_primitive_element451 = new BitSet(new long[]{0x0000000000000000L,0x2000000000000000L});
    public static final BitSet FOLLOW_RPAREN_in_primitive_element453 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_LPAREN_in_primitive_element468 = new BitSet(new long[]{0x0004002000000000L,0x100B000180000000L,0x0000000000001000L});
    public static final BitSet FOLLOW_expression2_in_primitive_element472 = new BitSet(new long[]{0x0000000000000000L,0x2000000000000000L});
    public static final BitSet FOLLOW_RPAREN_in_primitive_element474 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_INT_in_number507 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_FLOAT_in_number521 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_STRING_in_variable543 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_IF_in_if_expression560 = new BitSet(new long[]{0x0000000000000000L,0x1000000000000000L});
    public static final BitSet FOLLOW_LPAREN_in_if_expression562 = new BitSet(new long[]{0x0004002000000000L,0x100B000180000000L,0x0000000000001000L});
    public static final BitSet FOLLOW_expression2_in_if_expression564 = new BitSet(new long[]{0x0000000000000000L,0x0400000000000000L});
    public static final BitSet FOLLOW_COMMA_in_if_expression566 = new BitSet(new long[]{0x0004002000000000L,0x100B000180000000L,0x0000000000001000L});
    public static final BitSet FOLLOW_expression2_in_if_expression570 = new BitSet(new long[]{0x0000000000000000L,0x0400000000000000L});
    public static final BitSet FOLLOW_COMMA_in_if_expression572 = new BitSet(new long[]{0x0004002000000000L,0x100B000180000000L,0x0000000000001000L});
    public static final BitSet FOLLOW_expression2_in_if_expression574 = new BitSet(new long[]{0x0000000000000000L,0x2000000000000000L});
    public static final BitSet FOLLOW_RPAREN_in_if_expression576 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_STRING_in_function596 = new BitSet(new long[]{0x0000000000000000L,0x1000000000000000L});
    public static final BitSet FOLLOW_LPAREN_in_function598 = new BitSet(new long[]{0x0004002000000000L,0x300B000180000000L,0x0000000000001000L});
    public static final BitSet FOLLOW_expression2_in_function609 = new BitSet(new long[]{0x0000000000000000L,0x2400000000000000L});
    public static final BitSet FOLLOW_COMMA_in_function612 = new BitSet(new long[]{0x0004002000000000L,0x100B000180000000L,0x0000000000001000L});
    public static final BitSet FOLLOW_expression2_in_function614 = new BitSet(new long[]{0x0000000000000000L,0x2400000000000000L});
    public static final BitSet FOLLOW_RPAREN_in_function620 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_IF_in_synpred1_BNGGrammar_Expression359 = new BitSet(new long[]{0x0000000000000000L,0x1000000000000000L});
    public static final BitSet FOLLOW_LPAREN_in_synpred1_BNGGrammar_Expression361 = new BitSet(new long[]{0xFFFFFFFFFFFFFFF0L,0xDFFFFFFFFFFFFFFFL,0xFFFFFFFFFFFFFFFFL,0xFFFFFFFFFFFFFFFFL,0x00000000000FFFFFL});
    public static final BitSet FOLLOW_set_in_synpred1_BNGGrammar_Expression364 = new BitSet(new long[]{0xFFFFFFFFFFFFFFF0L,0xFFFFFFFFFFFFFFFFL,0xFFFFFFFFFFFFFFFFL,0xFFFFFFFFFFFFFFFFL,0x00000000000FFFFFL});
    public static final BitSet FOLLOW_RPAREN_in_synpred1_BNGGrammar_Expression369 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_STRING_in_synpred2_BNGGrammar_Expression389 = new BitSet(new long[]{0x0000000000000000L,0x1000000000000000L});
    public static final BitSet FOLLOW_LPAREN_in_synpred2_BNGGrammar_Expression391 = new BitSet(new long[]{0x0004002000000000L,0x300B000180000000L,0x0000000000001000L});
    public static final BitSet FOLLOW_expression2_in_synpred2_BNGGrammar_Expression394 = new BitSet(new long[]{0x0000000000000000L,0x2400000000000000L});
    public static final BitSet FOLLOW_COMMA_in_synpred2_BNGGrammar_Expression397 = new BitSet(new long[]{0x0004002000000000L,0x100B000180000000L,0x0000000000001000L});
    public static final BitSet FOLLOW_expression2_in_synpred2_BNGGrammar_Expression399 = new BitSet(new long[]{0x0000000000000000L,0x2400000000000000L});
    public static final BitSet FOLLOW_RPAREN_in_synpred2_BNGGrammar_Expression405 = new BitSet(new long[]{0x0000000000000002L});

}