// $ANTLR 3.3 Nov 30, 2010 12:50:56 BNGGrammar_Expression.g 2012-03-26 20:21:13

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
  import bngparser.exceptions.VariableNotFoundException;


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
    public static final int IF=34;
    public static final int PREFFIX=35;
    public static final int SUFFIX=36;
    public static final int LABEL=37;
    public static final int PARAMETERS=38;
    public static final int VERBOSE=39;
    public static final int MATCHONCE=40;
    public static final int DELETEMOLECULES=41;
    public static final int MOVECONNECTED=42;
    public static final int AUTO=43;
    public static final int HNAUTY=44;
    public static final int QUASY=45;
    public static final int NOT=46;
    public static final int INCLUDE_REACTANTS=47;
    public static final int INCLUDE_PRODUCTS=48;
    public static final int EXCLUDE_REACTANTS=49;
    public static final int EXCLUDE_PRODUCTS=50;
    public static final int ATOL=51;
    public static final int RTOL=52;
    public static final int STEADY_STATE=53;
    public static final int SPARSE=54;
    public static final int T_END=55;
    public static final int T_START=56;
    public static final int N_STEPS=57;
    public static final int SAMPLE_TIMES=58;
    public static final int SIMULATE_ODE=59;
    public static final int SIMULATE_SSA=60;
    public static final int READFILE=61;
    public static final int FILE=62;
    public static final int WRITENET=63;
    public static final int WRITESBML=64;
    public static final int WRITEMFILE=65;
    public static final int WRITEXML=66;
    public static final int SETCONCENTRATION=67;
    public static final int SETPARAMETER=68;
    public static final int SAVECONCENTRATIONS=69;
    public static final int RESETCONCENTRATIONS=70;
    public static final int GENERATENETWORK=71;
    public static final int SUBSTANCEUNITS=72;
    public static final int SAT=73;
    public static final int DIGIT=74;
    public static final int EXPONENT=75;
    public static final int FLOAT=76;
    public static final int INT=77;
    public static final int LETTER=78;
    public static final int STRING=79;
    public static final int SEMI=80;
    public static final int COLON=81;
    public static final int LSBRACKET=82;
    public static final int RSBRACKET=83;
    public static final int LBRACKET=84;
    public static final int RBRACKET=85;
    public static final int COMMA=86;
    public static final int DOT=87;
    public static final int LPAREN=88;
    public static final int RPAREN=89;
    public static final int UNI_REACTION_SIGN=90;
    public static final int BI_REACTION_SIGN=91;
    public static final int DOLLAR=92;
    public static final int TILDE=93;
    public static final int AT=94;
    public static final int GTE=95;
    public static final int GT=96;
    public static final int LTE=97;
    public static final int LT=98;
    public static final int ASSIGNS=99;
    public static final int EQUALS=100;
    public static final int BECOMES=101;
    public static final int DIV=102;
    public static final int TIMES=103;
    public static final int MINUS=104;
    public static final int PLUS=105;
    public static final int POWER=106;
    public static final int MOD=107;
    public static final int QMARK=108;
    public static final int EMARK=109;
    public static final int DBQUOTES=110;
    public static final int AMPERSAND=111;
    public static final int NULL=112;
    public static final int LOWER=113;
    public static final int UPPER=114;
    public static final int VERSION_NUMBER=115;

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
    // BNGGrammar_Expression.g:15:1: expression[ Map<String,Register> memory] returns [Double value, Map reference] : expression2 ;
    public final BNGGrammar_BNGGrammar_Expression.expression_return expression(Map<String,Register> memory) throws RecognitionException {
        expression_stack.push(new expression_scope());
        BNGGrammar_BNGGrammar_Expression.expression_return retval = new BNGGrammar_BNGGrammar_Expression.expression_return();
        retval.start = input.LT(1);

        BNGGrammar_BNGGrammar_Expression.expression2_return expression21 = null;



          ((expression_scope)expression_stack.peek()).references = new HashMap<String,Register>();
          ((expression_scope)expression_stack.peek()).lmemory = memory;

        try {
            // BNGGrammar_Expression.g:24:1: ( expression2 )
            // BNGGrammar_Expression.g:25:3: expression2
            {
            pushFollow(FOLLOW_expression2_in_expression63);
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
    // BNGGrammar_Expression.g:28:1: expression2 returns [Double value] : e1= relational_expression ( ( AND | OR ) relational_expression )* ;
    public final BNGGrammar_BNGGrammar_Expression.expression2_return expression2() throws RecognitionException {
        BNGGrammar_BNGGrammar_Expression.expression2_return retval = new BNGGrammar_BNGGrammar_Expression.expression2_return();
        retval.start = input.LT(1);

        BNGGrammar_BNGGrammar_Expression.relational_expression_return e1 = null;


        try {
            // BNGGrammar_Expression.g:29:1: (e1= relational_expression ( ( AND | OR ) relational_expression )* )
            // BNGGrammar_Expression.g:30:3: e1= relational_expression ( ( AND | OR ) relational_expression )*
            {
            pushFollow(FOLLOW_relational_expression_in_expression285);
            e1=relational_expression();

            state._fsp--;
            if (state.failed) return retval;
            if ( state.backtracking==0 ) {
              retval.value = e1.value; 
            }
            // BNGGrammar_Expression.g:30:50: ( ( AND | OR ) relational_expression )*
            loop1:
            do {
                int alt1=2;
                int LA1_0 = input.LA(1);

                if ( ((LA1_0>=AND && LA1_0<=OR)) ) {
                    alt1=1;
                }


                switch (alt1) {
            	case 1 :
            	    // BNGGrammar_Expression.g:30:51: ( AND | OR ) relational_expression
            	    {
            	    if ( (input.LA(1)>=AND && input.LA(1)<=OR) ) {
            	        input.consume();
            	        state.errorRecovery=false;state.failed=false;
            	    }
            	    else {
            	        if (state.backtracking>0) {state.failed=true; return retval;}
            	        MismatchedSetException mse = new MismatchedSetException(null,input);
            	        throw mse;
            	    }

            	    pushFollow(FOLLOW_relational_expression_in_expression298);
            	    relational_expression();

            	    state._fsp--;
            	    if (state.failed) return retval;

            	    }
            	    break;

            	default :
            	    break loop1;
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
    // BNGGrammar_Expression.g:34:1: relational_expression returns [Double value] : a1= adding_expression ( ( EQUALS | GT | GTE | LT | LTE ) adding_expression )* ;
    public final BNGGrammar_BNGGrammar_Expression.relational_expression_return relational_expression() throws RecognitionException {
        BNGGrammar_BNGGrammar_Expression.relational_expression_return retval = new BNGGrammar_BNGGrammar_Expression.relational_expression_return();
        retval.start = input.LT(1);

        BNGGrammar_BNGGrammar_Expression.adding_expression_return a1 = null;


        try {
            // BNGGrammar_Expression.g:34:45: (a1= adding_expression ( ( EQUALS | GT | GTE | LT | LTE ) adding_expression )* )
            // BNGGrammar_Expression.g:35:3: a1= adding_expression ( ( EQUALS | GT | GTE | LT | LTE ) adding_expression )*
            {
            pushFollow(FOLLOW_adding_expression_in_relational_expression121);
            a1=adding_expression();

            state._fsp--;
            if (state.failed) return retval;
            if ( state.backtracking==0 ) {
              retval.value = a1.value;
            }
            // BNGGrammar_Expression.g:35:44: ( ( EQUALS | GT | GTE | LT | LTE ) adding_expression )*
            loop2:
            do {
                int alt2=2;
                int LA2_0 = input.LA(1);

                if ( ((LA2_0>=GTE && LA2_0<=LT)||LA2_0==EQUALS) ) {
                    alt2=1;
                }


                switch (alt2) {
            	case 1 :
            	    // BNGGrammar_Expression.g:35:45: ( EQUALS | GT | GTE | LT | LTE ) adding_expression
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

            	    pushFollow(FOLLOW_adding_expression_in_relational_expression137);
            	    adding_expression();

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
    // $ANTLR end "relational_expression"

    public static class adding_expression_return extends ParserRuleReturnScope {
        public Double value;
        public StringTemplate st;
        public Object getTemplate() { return st; }
        public String toString() { return st==null?null:st.toString(); }
    };

    // $ANTLR start "adding_expression"
    // BNGGrammar_Expression.g:38:1: adding_expression returns [Double value] : s1= multiplying_expression ( PLUS s2= multiplying_expression | MINUS s3= multiplying_expression )* ;
    public final BNGGrammar_BNGGrammar_Expression.adding_expression_return adding_expression() throws RecognitionException {
        BNGGrammar_BNGGrammar_Expression.adding_expression_return retval = new BNGGrammar_BNGGrammar_Expression.adding_expression_return();
        retval.start = input.LT(1);

        BNGGrammar_BNGGrammar_Expression.multiplying_expression_return s1 = null;

        BNGGrammar_BNGGrammar_Expression.multiplying_expression_return s2 = null;

        BNGGrammar_BNGGrammar_Expression.multiplying_expression_return s3 = null;


        try {
            // BNGGrammar_Expression.g:39:1: (s1= multiplying_expression ( PLUS s2= multiplying_expression | MINUS s3= multiplying_expression )* )
            // BNGGrammar_Expression.g:39:3: s1= multiplying_expression ( PLUS s2= multiplying_expression | MINUS s3= multiplying_expression )*
            {
            pushFollow(FOLLOW_multiplying_expression_in_adding_expression154);
            s1=multiplying_expression();

            state._fsp--;
            if (state.failed) return retval;
            if ( state.backtracking==0 ) {
              retval.value = s1.value;
            }
            // BNGGrammar_Expression.g:40:1: ( PLUS s2= multiplying_expression | MINUS s3= multiplying_expression )*
            loop3:
            do {
                int alt3=3;
                int LA3_0 = input.LA(1);

                if ( (LA3_0==PLUS) ) {
                    alt3=1;
                }
                else if ( (LA3_0==MINUS) ) {
                    alt3=2;
                }


                switch (alt3) {
            	case 1 :
            	    // BNGGrammar_Expression.g:40:3: PLUS s2= multiplying_expression
            	    {
            	    match(input,PLUS,FOLLOW_PLUS_in_adding_expression161); if (state.failed) return retval;
            	    pushFollow(FOLLOW_multiplying_expression_in_adding_expression165);
            	    s2=multiplying_expression();

            	    state._fsp--;
            	    if (state.failed) return retval;
            	    if ( state.backtracking==0 ) {
            	      retval.value += s2.value;
            	    }

            	    }
            	    break;
            	case 2 :
            	    // BNGGrammar_Expression.g:41:3: MINUS s3= multiplying_expression
            	    {
            	    match(input,MINUS,FOLLOW_MINUS_in_adding_expression171); if (state.failed) return retval;
            	    pushFollow(FOLLOW_multiplying_expression_in_adding_expression175);
            	    s3=multiplying_expression();

            	    state._fsp--;
            	    if (state.failed) return retval;
            	    if ( state.backtracking==0 ) {
            	      retval.value -= s3.value;
            	    }

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
    // $ANTLR end "adding_expression"

    public static class multiplying_expression_return extends ParserRuleReturnScope {
        public Double value;
        public StringTemplate st;
        public Object getTemplate() { return st; }
        public String toString() { return st==null?null:st.toString(); }
    };

    // $ANTLR start "multiplying_expression"
    // BNGGrammar_Expression.g:44:1: multiplying_expression returns [Double value] : s1= power_expression ( TIMES s2= power_expression | DIV s3= power_expression | MOD s4= power_expression )* ;
    public final BNGGrammar_BNGGrammar_Expression.multiplying_expression_return multiplying_expression() throws RecognitionException {
        BNGGrammar_BNGGrammar_Expression.multiplying_expression_return retval = new BNGGrammar_BNGGrammar_Expression.multiplying_expression_return();
        retval.start = input.LT(1);

        BNGGrammar_BNGGrammar_Expression.power_expression_return s1 = null;

        BNGGrammar_BNGGrammar_Expression.power_expression_return s2 = null;

        BNGGrammar_BNGGrammar_Expression.power_expression_return s3 = null;

        BNGGrammar_BNGGrammar_Expression.power_expression_return s4 = null;


        try {
            // BNGGrammar_Expression.g:45:1: (s1= power_expression ( TIMES s2= power_expression | DIV s3= power_expression | MOD s4= power_expression )* )
            // BNGGrammar_Expression.g:45:3: s1= power_expression ( TIMES s2= power_expression | DIV s3= power_expression | MOD s4= power_expression )*
            {
            pushFollow(FOLLOW_power_expression_in_multiplying_expression194);
            s1=power_expression();

            state._fsp--;
            if (state.failed) return retval;
            if ( state.backtracking==0 ) {
              retval.value = s1.value;
            }
            // BNGGrammar_Expression.g:46:1: ( TIMES s2= power_expression | DIV s3= power_expression | MOD s4= power_expression )*
            loop4:
            do {
                int alt4=4;
                switch ( input.LA(1) ) {
                case TIMES:
                    {
                    alt4=1;
                    }
                    break;
                case DIV:
                    {
                    alt4=2;
                    }
                    break;
                case MOD:
                    {
                    alt4=3;
                    }
                    break;

                }

                switch (alt4) {
            	case 1 :
            	    // BNGGrammar_Expression.g:46:3: TIMES s2= power_expression
            	    {
            	    match(input,TIMES,FOLLOW_TIMES_in_multiplying_expression201); if (state.failed) return retval;
            	    pushFollow(FOLLOW_power_expression_in_multiplying_expression205);
            	    s2=power_expression();

            	    state._fsp--;
            	    if (state.failed) return retval;
            	    if ( state.backtracking==0 ) {
            	      retval.value *= s2.value;
            	    }

            	    }
            	    break;
            	case 2 :
            	    // BNGGrammar_Expression.g:47:3: DIV s3= power_expression
            	    {
            	    match(input,DIV,FOLLOW_DIV_in_multiplying_expression211); if (state.failed) return retval;
            	    pushFollow(FOLLOW_power_expression_in_multiplying_expression215);
            	    s3=power_expression();

            	    state._fsp--;
            	    if (state.failed) return retval;
            	    if ( state.backtracking==0 ) {
            	      retval.value /= s3.value;
            	    }

            	    }
            	    break;
            	case 3 :
            	    // BNGGrammar_Expression.g:48:3: MOD s4= power_expression
            	    {
            	    match(input,MOD,FOLLOW_MOD_in_multiplying_expression221); if (state.failed) return retval;
            	    pushFollow(FOLLOW_power_expression_in_multiplying_expression225);
            	    s4=power_expression();

            	    state._fsp--;
            	    if (state.failed) return retval;
            	    if ( state.backtracking==0 ) {
            	      retval.value %= s4.value;
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
    // $ANTLR end "multiplying_expression"

    public static class power_expression_return extends ParserRuleReturnScope {
        public Double value;
        public StringTemplate st;
        public Object getTemplate() { return st; }
        public String toString() { return st==null?null:st.toString(); }
    };

    // $ANTLR start "power_expression"
    // BNGGrammar_Expression.g:51:1: power_expression returns [Double value] : s1= sign_expression ( POWER s2= sign_expression )* ;
    public final BNGGrammar_BNGGrammar_Expression.power_expression_return power_expression() throws RecognitionException {
        BNGGrammar_BNGGrammar_Expression.power_expression_return retval = new BNGGrammar_BNGGrammar_Expression.power_expression_return();
        retval.start = input.LT(1);

        BNGGrammar_BNGGrammar_Expression.sign_expression_return s1 = null;

        BNGGrammar_BNGGrammar_Expression.sign_expression_return s2 = null;


        try {
            // BNGGrammar_Expression.g:52:1: (s1= sign_expression ( POWER s2= sign_expression )* )
            // BNGGrammar_Expression.g:52:3: s1= sign_expression ( POWER s2= sign_expression )*
            {
            pushFollow(FOLLOW_sign_expression_in_power_expression245);
            s1=sign_expression();

            state._fsp--;
            if (state.failed) return retval;
            if ( state.backtracking==0 ) {
              retval.value = s1.value;
            }
            // BNGGrammar_Expression.g:53:1: ( POWER s2= sign_expression )*
            loop5:
            do {
                int alt5=2;
                int LA5_0 = input.LA(1);

                if ( (LA5_0==POWER) ) {
                    alt5=1;
                }


                switch (alt5) {
            	case 1 :
            	    // BNGGrammar_Expression.g:53:2: POWER s2= sign_expression
            	    {
            	    match(input,POWER,FOLLOW_POWER_in_power_expression250); if (state.failed) return retval;
            	    pushFollow(FOLLOW_sign_expression_in_power_expression254);
            	    s2=sign_expression();

            	    state._fsp--;
            	    if (state.failed) return retval;
            	    if ( state.backtracking==0 ) {
            	      retval.value = Math.pow(retval.value,s2.value);
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
    // $ANTLR end "power_expression"

    public static class sign_expression_return extends ParserRuleReturnScope {
        public Double value;
        public StringTemplate st;
        public Object getTemplate() { return st; }
        public String toString() { return st==null?null:st.toString(); }
    };

    // $ANTLR start "sign_expression"
    // BNGGrammar_Expression.g:56:1: sign_expression returns [Double value] : ( ( MINUS ) )* boolean_negation_expression ;
    public final BNGGrammar_BNGGrammar_Expression.sign_expression_return sign_expression() throws RecognitionException {
        BNGGrammar_BNGGrammar_Expression.sign_expression_return retval = new BNGGrammar_BNGGrammar_Expression.sign_expression_return();
        retval.start = input.LT(1);

        BNGGrammar_BNGGrammar_Expression.boolean_negation_expression_return boolean_negation_expression2 = null;


        try {
            // BNGGrammar_Expression.g:58:1: ( ( ( MINUS ) )* boolean_negation_expression )
            // BNGGrammar_Expression.g:58:3: ( ( MINUS ) )* boolean_negation_expression
            {
            if ( state.backtracking==0 ) {
              retval.value = new Double(1);
            }
            // BNGGrammar_Expression.g:58:29: ( ( MINUS ) )*
            loop6:
            do {
                int alt6=2;
                int LA6_0 = input.LA(1);

                if ( (LA6_0==MINUS) ) {
                    alt6=1;
                }


                switch (alt6) {
            	case 1 :
            	    // BNGGrammar_Expression.g:58:30: ( MINUS )
            	    {
            	    // BNGGrammar_Expression.g:58:30: ( MINUS )
            	    // BNGGrammar_Expression.g:58:31: MINUS
            	    {
            	    match(input,MINUS,FOLLOW_MINUS_in_sign_expression276); if (state.failed) return retval;
            	    if ( state.backtracking==0 ) {
            	      retval.value *= -1;
            	    }

            	    }


            	    }
            	    break;

            	default :
            	    break loop6;
                }
            } while (true);

            pushFollow(FOLLOW_boolean_negation_expression_in_sign_expression283);
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
    // BNGGrammar_Expression.g:61:1: boolean_negation_expression returns [Double value] : (n= NOT )? e= primitive_element ;
    public final BNGGrammar_BNGGrammar_Expression.boolean_negation_expression_return boolean_negation_expression() throws RecognitionException {
        BNGGrammar_BNGGrammar_Expression.boolean_negation_expression_return retval = new BNGGrammar_BNGGrammar_Expression.boolean_negation_expression_return();
        retval.start = input.LT(1);

        Token n=null;
        BNGGrammar_BNGGrammar_Expression.primitive_element_return e = null;


        try {
            // BNGGrammar_Expression.g:62:1: ( (n= NOT )? e= primitive_element )
            // BNGGrammar_Expression.g:62:3: (n= NOT )? e= primitive_element
            {
            // BNGGrammar_Expression.g:62:3: (n= NOT )?
            int alt7=2;
            int LA7_0 = input.LA(1);

            if ( (LA7_0==NOT) ) {
                alt7=1;
            }
            switch (alt7) {
                case 1 :
                    // BNGGrammar_Expression.g:62:4: n= NOT
                    {
                    n=(Token)match(input,NOT,FOLLOW_NOT_in_boolean_negation_expression301); if (state.failed) return retval;

                    }
                    break;

            }

            pushFollow(FOLLOW_primitive_element_in_boolean_negation_expression307);
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
    // BNGGrammar_Expression.g:65:1: primitive_element returns [Double value] : ( number | ( STRING LPAREN RPAREN )=> function | variable | LPAREN expression2 RPAREN );
    public final BNGGrammar_BNGGrammar_Expression.primitive_element_return primitive_element() throws RecognitionException {
        BNGGrammar_BNGGrammar_Expression.primitive_element_return retval = new BNGGrammar_BNGGrammar_Expression.primitive_element_return();
        retval.start = input.LT(1);

        BNGGrammar_BNGGrammar_Expression.number_return number3 = null;

        BNGGrammar_BNGGrammar_Expression.function_return function4 = null;

        BNGGrammar_BNGGrammar_Expression.variable_return variable5 = null;

        BNGGrammar_BNGGrammar_Expression.expression2_return expression26 = null;


        try {
            // BNGGrammar_Expression.g:65:41: ( number | ( STRING LPAREN RPAREN )=> function | variable | LPAREN expression2 RPAREN )
            int alt8=4;
            switch ( input.LA(1) ) {
            case FLOAT:
            case INT:
                {
                alt8=1;
                }
                break;
            case STRING:
                {
                int LA8_2 = input.LA(2);

                if ( (LA8_2==LPAREN) && (synpred1_BNGGrammar_Expression())) {
                    alt8=2;
                }
                else if ( (LA8_2==END||(LA8_2>=AND && LA8_2<=OR)||(LA8_2>=DELETEMOLECULES && LA8_2<=MOVECONNECTED)||(LA8_2>=INCLUDE_REACTANTS && LA8_2<=EXCLUDE_PRODUCTS)||LA8_2==INT||LA8_2==STRING||LA8_2==LBRACKET||LA8_2==COMMA||LA8_2==RPAREN||LA8_2==DOLLAR||(LA8_2>=AT && LA8_2<=LT)||LA8_2==EQUALS||(LA8_2>=DIV && LA8_2<=MOD)||LA8_2==DBQUOTES) ) {
                    alt8=3;
                }
                else {
                    if (state.backtracking>0) {state.failed=true; return retval;}
                    NoViableAltException nvae =
                        new NoViableAltException("", 8, 2, input);

                    throw nvae;
                }
                }
                break;
            case LPAREN:
                {
                alt8=4;
                }
                break;
            default:
                if (state.backtracking>0) {state.failed=true; return retval;}
                NoViableAltException nvae =
                    new NoViableAltException("", 8, 0, input);

                throw nvae;
            }

            switch (alt8) {
                case 1 :
                    // BNGGrammar_Expression.g:66:9: number
                    {
                    pushFollow(FOLLOW_number_in_primitive_element331);
                    number3=number();

                    state._fsp--;
                    if (state.failed) return retval;
                    if ( state.backtracking==0 ) {
                      retval.value = (number3!=null?number3.value:null);
                    }

                    }
                    break;
                case 2 :
                    // BNGGrammar_Expression.g:67:11: ( STRING LPAREN RPAREN )=> function
                    {
                    pushFollow(FOLLOW_function_in_primitive_element356);
                    function4=function();

                    state._fsp--;
                    if (state.failed) return retval;
                    if ( state.backtracking==0 ) {
                      retval.value = (function4!=null?function4.value:null);
                    }

                    }
                    break;
                case 3 :
                    // BNGGrammar_Expression.g:68:12: variable
                    {
                    pushFollow(FOLLOW_variable_in_primitive_element371);
                    variable5=variable();

                    state._fsp--;
                    if (state.failed) return retval;
                    if ( state.backtracking==0 ) {
                      retval.value = (variable5!=null?variable5.value:null);
                    }

                    }
                    break;
                case 4 :
                    // BNGGrammar_Expression.g:69:11: LPAREN expression2 RPAREN
                    {
                    match(input,LPAREN,FOLLOW_LPAREN_in_primitive_element385); if (state.failed) return retval;
                    pushFollow(FOLLOW_expression2_in_primitive_element387);
                    expression26=expression2();

                    state._fsp--;
                    if (state.failed) return retval;
                    match(input,RPAREN,FOLLOW_RPAREN_in_primitive_element389); if (state.failed) return retval;
                    if ( state.backtracking==0 ) {
                      retval.value = (expression26!=null?expression26.value:null);
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
    // BNGGrammar_Expression.g:73:1: number returns [Double value] : (s1= INT | s1= FLOAT );
    public final BNGGrammar_BNGGrammar_Expression.number_return number() throws RecognitionException {
        BNGGrammar_BNGGrammar_Expression.number_return retval = new BNGGrammar_BNGGrammar_Expression.number_return();
        retval.start = input.LT(1);

        Token s1=null;

        try {
            // BNGGrammar_Expression.g:73:30: (s1= INT | s1= FLOAT )
            int alt9=2;
            int LA9_0 = input.LA(1);

            if ( (LA9_0==INT) ) {
                alt9=1;
            }
            else if ( (LA9_0==FLOAT) ) {
                alt9=2;
            }
            else {
                if (state.backtracking>0) {state.failed=true; return retval;}
                NoViableAltException nvae =
                    new NoViableAltException("", 9, 0, input);

                throw nvae;
            }
            switch (alt9) {
                case 1 :
                    // BNGGrammar_Expression.g:73:32: s1= INT
                    {
                    s1=(Token)match(input,INT,FOLLOW_INT_in_number422); if (state.failed) return retval;
                    if ( state.backtracking==0 ) {
                      retval.value = Double.parseDouble((s1!=null?s1.getText():null));
                    }

                    }
                    break;
                case 2 :
                    // BNGGrammar_Expression.g:74:9: s1= FLOAT
                    {
                    s1=(Token)match(input,FLOAT,FOLLOW_FLOAT_in_number436); if (state.failed) return retval;
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
    // BNGGrammar_Expression.g:77:1: variable returns [Double value] : STRING ;
    public final BNGGrammar_BNGGrammar_Expression.variable_return variable() throws RecognitionException {
        BNGGrammar_BNGGrammar_Expression.variable_return retval = new BNGGrammar_BNGGrammar_Expression.variable_return();
        retval.start = input.LT(1);

        Token STRING7=null;

        try {
            // BNGGrammar_Expression.g:77:32: ( STRING )
            // BNGGrammar_Expression.g:77:34: STRING
            {
            STRING7=(Token)match(input,STRING,FOLLOW_STRING_in_variable456); if (state.failed) return retval;
            if ( state.backtracking==0 ) {

                                try{
                                
                                //if(retval.value == null)
                                  //throw [VariableNotFoundException]
                                if(((expression_scope)expression_stack.peek()).lmemory.containsKey((STRING7!=null?STRING7.getText():null))){
                                  Register temp = ((expression_scope)expression_stack.peek()).lmemory.get((STRING7!=null?STRING7.getText():null));
                                  retval.value = temp.getValue();
              	                  if(!temp.getType().equals("parameter") && !temp.getType().equals("observable")){
              	                    System.err.println((STRING7!=null?STRING7.getText():null) + " is in memory but it is not a variable or observable. Check syntax");
              	                  }
                                }
                                else{
                                  retval.value = 1.0;
                                  System.err.println("variable not found: " + (STRING7!=null?STRING7.getText():null));
                                }
                                ((expression_scope)expression_stack.peek()).references.put((STRING7!=null?STRING7.getText():null),((expression_scope)expression_stack.peek()).lmemory.get((STRING7!=null?STRING7.getText():null)));
                                }
                                catch(NullPointerException e){
                                  System.err.println("Variable not found: " + (STRING7!=null?STRING7.getText():null));
                                  
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

    public static class function_return extends ParserRuleReturnScope {
        public Double value;
        public StringTemplate st;
        public Object getTemplate() { return st; }
        public String toString() { return st==null?null:st.toString(); }
    };

    // $ANTLR start "function"
    // BNGGrammar_Expression.g:102:1: function returns [Double value] : STRING LPAREN RPAREN ;
    public final BNGGrammar_BNGGrammar_Expression.function_return function() throws RecognitionException {
        BNGGrammar_BNGGrammar_Expression.function_return retval = new BNGGrammar_BNGGrammar_Expression.function_return();
        retval.start = input.LT(1);

        Token STRING8=null;

        try {
            // BNGGrammar_Expression.g:102:32: ( STRING LPAREN RPAREN )
            // BNGGrammar_Expression.g:103:3: STRING LPAREN RPAREN
            {
            STRING8=(Token)match(input,STRING,FOLLOW_STRING_in_function472); if (state.failed) return retval;
            match(input,LPAREN,FOLLOW_LPAREN_in_function474); if (state.failed) return retval;
            match(input,RPAREN,FOLLOW_RPAREN_in_function476); if (state.failed) return retval;
            if ( state.backtracking==0 ) {

                  try{
                    if(((expression_scope)expression_stack.peek()).lmemory.containsKey((STRING8!=null?STRING8.getText():null))){
                                  Register temp = ((expression_scope)expression_stack.peek()).lmemory.get((STRING8!=null?STRING8.getText():null));
                                  retval.value = temp.getValue();
                                  if(!temp.getType().equals("function")){
                                    System.err.println((STRING8!=null?STRING8.getText():null) + "is in memory but it is not a function. Check syntax");
                                  }
                     }
                     else{
                                  retval.value = 1.0;
                                  System.err.println("function not found: " + (STRING8!=null?STRING8.getText():null));
                     }
                      ((expression_scope)expression_stack.peek()).references.put((STRING8!=null?STRING8.getText():null),((expression_scope)expression_stack.peek()).lmemory.get((STRING8!=null?STRING8.getText():null)));
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
        // BNGGrammar_Expression.g:67:11: ( STRING LPAREN RPAREN )
        // BNGGrammar_Expression.g:67:12: STRING LPAREN RPAREN
        {
        match(input,STRING,FOLLOW_STRING_in_synpred1_BNGGrammar_Expression347); if (state.failed) return ;
        match(input,LPAREN,FOLLOW_LPAREN_in_synpred1_BNGGrammar_Expression349); if (state.failed) return ;
        match(input,RPAREN,FOLLOW_RPAREN_in_synpred1_BNGGrammar_Expression351); if (state.failed) return ;

        }
    }
    // $ANTLR end synpred1_BNGGrammar_Expression

    // Delegated rules

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


 

    public static final BitSet FOLLOW_expression2_in_expression63 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_relational_expression_in_expression285 = new BitSet(new long[]{0x0000000180000002L});
    public static final BitSet FOLLOW_set_in_expression290 = new BitSet(new long[]{0x0000400000000000L,0x000001000100B000L});
    public static final BitSet FOLLOW_relational_expression_in_expression298 = new BitSet(new long[]{0x0000000180000002L});
    public static final BitSet FOLLOW_adding_expression_in_relational_expression121 = new BitSet(new long[]{0x0000000000000002L,0x0000001780000000L});
    public static final BitSet FOLLOW_set_in_relational_expression125 = new BitSet(new long[]{0x0000400000000000L,0x000001000100B000L});
    public static final BitSet FOLLOW_adding_expression_in_relational_expression137 = new BitSet(new long[]{0x0000000000000002L,0x0000001780000000L});
    public static final BitSet FOLLOW_multiplying_expression_in_adding_expression154 = new BitSet(new long[]{0x0000000000000002L,0x0000030000000000L});
    public static final BitSet FOLLOW_PLUS_in_adding_expression161 = new BitSet(new long[]{0x0000400000000000L,0x000001000100B000L});
    public static final BitSet FOLLOW_multiplying_expression_in_adding_expression165 = new BitSet(new long[]{0x0000000000000002L,0x0000030000000000L});
    public static final BitSet FOLLOW_MINUS_in_adding_expression171 = new BitSet(new long[]{0x0000400000000000L,0x000001000100B000L});
    public static final BitSet FOLLOW_multiplying_expression_in_adding_expression175 = new BitSet(new long[]{0x0000000000000002L,0x0000030000000000L});
    public static final BitSet FOLLOW_power_expression_in_multiplying_expression194 = new BitSet(new long[]{0x0000000000000002L,0x000008C000000000L});
    public static final BitSet FOLLOW_TIMES_in_multiplying_expression201 = new BitSet(new long[]{0x0000400000000000L,0x000001000100B000L});
    public static final BitSet FOLLOW_power_expression_in_multiplying_expression205 = new BitSet(new long[]{0x0000000000000002L,0x000008C000000000L});
    public static final BitSet FOLLOW_DIV_in_multiplying_expression211 = new BitSet(new long[]{0x0000400000000000L,0x000001000100B000L});
    public static final BitSet FOLLOW_power_expression_in_multiplying_expression215 = new BitSet(new long[]{0x0000000000000002L,0x000008C000000000L});
    public static final BitSet FOLLOW_MOD_in_multiplying_expression221 = new BitSet(new long[]{0x0000400000000000L,0x000001000100B000L});
    public static final BitSet FOLLOW_power_expression_in_multiplying_expression225 = new BitSet(new long[]{0x0000000000000002L,0x000008C000000000L});
    public static final BitSet FOLLOW_sign_expression_in_power_expression245 = new BitSet(new long[]{0x0000000000000002L,0x0000040000000000L});
    public static final BitSet FOLLOW_POWER_in_power_expression250 = new BitSet(new long[]{0x0000400000000000L,0x000001000100B000L});
    public static final BitSet FOLLOW_sign_expression_in_power_expression254 = new BitSet(new long[]{0x0000000000000002L,0x0000040000000000L});
    public static final BitSet FOLLOW_MINUS_in_sign_expression276 = new BitSet(new long[]{0x0000400000000000L,0x000001000100B000L});
    public static final BitSet FOLLOW_boolean_negation_expression_in_sign_expression283 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_NOT_in_boolean_negation_expression301 = new BitSet(new long[]{0x0000400000000000L,0x000001000100B000L});
    public static final BitSet FOLLOW_primitive_element_in_boolean_negation_expression307 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_number_in_primitive_element331 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_function_in_primitive_element356 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_variable_in_primitive_element371 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_LPAREN_in_primitive_element385 = new BitSet(new long[]{0x0000400000000000L,0x000001000100B000L});
    public static final BitSet FOLLOW_expression2_in_primitive_element387 = new BitSet(new long[]{0x0000000000000000L,0x0000000002000000L});
    public static final BitSet FOLLOW_RPAREN_in_primitive_element389 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_INT_in_number422 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_FLOAT_in_number436 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_STRING_in_variable456 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_STRING_in_function472 = new BitSet(new long[]{0x0000000000000000L,0x0000000001000000L});
    public static final BitSet FOLLOW_LPAREN_in_function474 = new BitSet(new long[]{0x0000000000000000L,0x0000000002000000L});
    public static final BitSet FOLLOW_RPAREN_in_function476 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_STRING_in_synpred1_BNGGrammar_Expression347 = new BitSet(new long[]{0x0000000000000000L,0x0000000001000000L});
    public static final BitSet FOLLOW_LPAREN_in_synpred1_BNGGrammar_Expression349 = new BitSet(new long[]{0x0000000000000000L,0x0000000002000000L});
    public static final BitSet FOLLOW_RPAREN_in_synpred1_BNGGrammar_Expression351 = new BitSet(new long[]{0x0000000000000002L});

}