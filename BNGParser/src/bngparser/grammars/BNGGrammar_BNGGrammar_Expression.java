// $ANTLR 3.3 Nov 30, 2010 12:50:56 BNGGrammar_Expression.g 2012-01-25 16:16:34

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

            retval.value = (expression21!=null?expression21.value:null);retval.reference = ((expression_scope)expression_stack.peek()).references;

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

            retval.value = e1.value; 
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
            	        state.errorRecovery=false;
            	    }
            	    else {
            	        MismatchedSetException mse = new MismatchedSetException(null,input);
            	        throw mse;
            	    }

            	    pushFollow(FOLLOW_relational_expression_in_expression298);
            	    relational_expression();

            	    state._fsp--;


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

            retval.value = a1.value;
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
            	        state.errorRecovery=false;
            	    }
            	    else {
            	        MismatchedSetException mse = new MismatchedSetException(null,input);
            	        throw mse;
            	    }

            	    pushFollow(FOLLOW_adding_expression_in_relational_expression137);
            	    adding_expression();

            	    state._fsp--;


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

            retval.value = s1.value;
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
            	    match(input,PLUS,FOLLOW_PLUS_in_adding_expression161); 
            	    pushFollow(FOLLOW_multiplying_expression_in_adding_expression165);
            	    s2=multiplying_expression();

            	    state._fsp--;

            	    retval.value += s2.value;

            	    }
            	    break;
            	case 2 :
            	    // BNGGrammar_Expression.g:41:3: MINUS s3= multiplying_expression
            	    {
            	    match(input,MINUS,FOLLOW_MINUS_in_adding_expression171); 
            	    pushFollow(FOLLOW_multiplying_expression_in_adding_expression175);
            	    s3=multiplying_expression();

            	    state._fsp--;

            	    retval.value -= s3.value;

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
    // BNGGrammar_Expression.g:44:1: multiplying_expression returns [Double value] : s1= sign_expression ( TIMES s2= sign_expression | DIV s3= sign_expression | MOD s4= sign_expression )* ;
    public final BNGGrammar_BNGGrammar_Expression.multiplying_expression_return multiplying_expression() throws RecognitionException {
        BNGGrammar_BNGGrammar_Expression.multiplying_expression_return retval = new BNGGrammar_BNGGrammar_Expression.multiplying_expression_return();
        retval.start = input.LT(1);

        BNGGrammar_BNGGrammar_Expression.sign_expression_return s1 = null;

        BNGGrammar_BNGGrammar_Expression.sign_expression_return s2 = null;

        BNGGrammar_BNGGrammar_Expression.sign_expression_return s3 = null;

        BNGGrammar_BNGGrammar_Expression.sign_expression_return s4 = null;


        try {
            // BNGGrammar_Expression.g:45:1: (s1= sign_expression ( TIMES s2= sign_expression | DIV s3= sign_expression | MOD s4= sign_expression )* )
            // BNGGrammar_Expression.g:45:3: s1= sign_expression ( TIMES s2= sign_expression | DIV s3= sign_expression | MOD s4= sign_expression )*
            {
            pushFollow(FOLLOW_sign_expression_in_multiplying_expression194);
            s1=sign_expression();

            state._fsp--;

            retval.value = s1.value;
            // BNGGrammar_Expression.g:46:1: ( TIMES s2= sign_expression | DIV s3= sign_expression | MOD s4= sign_expression )*
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
            	    // BNGGrammar_Expression.g:46:3: TIMES s2= sign_expression
            	    {
            	    match(input,TIMES,FOLLOW_TIMES_in_multiplying_expression201); 
            	    pushFollow(FOLLOW_sign_expression_in_multiplying_expression205);
            	    s2=sign_expression();

            	    state._fsp--;

            	    retval.value *= s2.value;

            	    }
            	    break;
            	case 2 :
            	    // BNGGrammar_Expression.g:47:3: DIV s3= sign_expression
            	    {
            	    match(input,DIV,FOLLOW_DIV_in_multiplying_expression211); 
            	    pushFollow(FOLLOW_sign_expression_in_multiplying_expression215);
            	    s3=sign_expression();

            	    state._fsp--;

            	    retval.value /= s3.value;

            	    }
            	    break;
            	case 3 :
            	    // BNGGrammar_Expression.g:48:3: MOD s4= sign_expression
            	    {
            	    match(input,MOD,FOLLOW_MOD_in_multiplying_expression221); 
            	    pushFollow(FOLLOW_sign_expression_in_multiplying_expression225);
            	    s4=sign_expression();

            	    state._fsp--;

            	    retval.value %= s4.value;

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

    public static class sign_expression_return extends ParserRuleReturnScope {
        public Double value;
        public StringTemplate st;
        public Object getTemplate() { return st; }
        public String toString() { return st==null?null:st.toString(); }
    };

    // $ANTLR start "sign_expression"
    // BNGGrammar_Expression.g:51:1: sign_expression returns [Double value] : ( ( MINUS ) )* boolean_negation_expression ;
    public final BNGGrammar_BNGGrammar_Expression.sign_expression_return sign_expression() throws RecognitionException {
        BNGGrammar_BNGGrammar_Expression.sign_expression_return retval = new BNGGrammar_BNGGrammar_Expression.sign_expression_return();
        retval.start = input.LT(1);

        BNGGrammar_BNGGrammar_Expression.boolean_negation_expression_return boolean_negation_expression2 = null;


        try {
            // BNGGrammar_Expression.g:53:1: ( ( ( MINUS ) )* boolean_negation_expression )
            // BNGGrammar_Expression.g:53:3: ( ( MINUS ) )* boolean_negation_expression
            {
            retval.value = new Double(1);
            // BNGGrammar_Expression.g:53:29: ( ( MINUS ) )*
            loop5:
            do {
                int alt5=2;
                int LA5_0 = input.LA(1);

                if ( (LA5_0==MINUS) ) {
                    alt5=1;
                }


                switch (alt5) {
            	case 1 :
            	    // BNGGrammar_Expression.g:53:30: ( MINUS )
            	    {
            	    // BNGGrammar_Expression.g:53:30: ( MINUS )
            	    // BNGGrammar_Expression.g:53:31: MINUS
            	    {
            	    match(input,MINUS,FOLLOW_MINUS_in_sign_expression247); 
            	    retval.value *= -1;

            	    }


            	    }
            	    break;

            	default :
            	    break loop5;
                }
            } while (true);

            pushFollow(FOLLOW_boolean_negation_expression_in_sign_expression254);
            boolean_negation_expression2=boolean_negation_expression();

            state._fsp--;

            retval.value *= (boolean_negation_expression2!=null?boolean_negation_expression2.value:null);

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
    // BNGGrammar_Expression.g:56:1: boolean_negation_expression returns [Double value] : (n= NOT )? e= primitive_element ;
    public final BNGGrammar_BNGGrammar_Expression.boolean_negation_expression_return boolean_negation_expression() throws RecognitionException {
        BNGGrammar_BNGGrammar_Expression.boolean_negation_expression_return retval = new BNGGrammar_BNGGrammar_Expression.boolean_negation_expression_return();
        retval.start = input.LT(1);

        Token n=null;
        BNGGrammar_BNGGrammar_Expression.primitive_element_return e = null;


        try {
            // BNGGrammar_Expression.g:57:1: ( (n= NOT )? e= primitive_element )
            // BNGGrammar_Expression.g:57:3: (n= NOT )? e= primitive_element
            {
            // BNGGrammar_Expression.g:57:3: (n= NOT )?
            int alt6=2;
            int LA6_0 = input.LA(1);

            if ( (LA6_0==NOT) ) {
                alt6=1;
            }
            switch (alt6) {
                case 1 :
                    // BNGGrammar_Expression.g:57:4: n= NOT
                    {
                    n=(Token)match(input,NOT,FOLLOW_NOT_in_boolean_negation_expression272); 

                    }
                    break;

            }

            pushFollow(FOLLOW_primitive_element_in_boolean_negation_expression278);
            e=primitive_element();

            state._fsp--;

            retval.value = e.value * (n == null ? 1 : -1);

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
    // BNGGrammar_Expression.g:60:1: primitive_element returns [Double value] : ( number | variable | LPAREN expression2 RPAREN );
    public final BNGGrammar_BNGGrammar_Expression.primitive_element_return primitive_element() throws RecognitionException {
        BNGGrammar_BNGGrammar_Expression.primitive_element_return retval = new BNGGrammar_BNGGrammar_Expression.primitive_element_return();
        retval.start = input.LT(1);

        BNGGrammar_BNGGrammar_Expression.number_return number3 = null;

        BNGGrammar_BNGGrammar_Expression.variable_return variable4 = null;

        BNGGrammar_BNGGrammar_Expression.expression2_return expression25 = null;


        try {
            // BNGGrammar_Expression.g:60:41: ( number | variable | LPAREN expression2 RPAREN )
            int alt7=3;
            switch ( input.LA(1) ) {
            case FLOAT:
            case INT:
                {
                alt7=1;
                }
                break;
            case STRING:
                {
                alt7=2;
                }
                break;
            case LPAREN:
                {
                alt7=3;
                }
                break;
            default:
                NoViableAltException nvae =
                    new NoViableAltException("", 7, 0, input);

                throw nvae;
            }

            switch (alt7) {
                case 1 :
                    // BNGGrammar_Expression.g:61:9: number
                    {
                    pushFollow(FOLLOW_number_in_primitive_element302);
                    number3=number();

                    state._fsp--;

                    retval.value = (number3!=null?number3.value:null);

                    }
                    break;
                case 2 :
                    // BNGGrammar_Expression.g:62:11: variable
                    {
                    pushFollow(FOLLOW_variable_in_primitive_element317);
                    variable4=variable();

                    state._fsp--;

                    retval.value = (variable4!=null?variable4.value:null);

                    }
                    break;
                case 3 :
                    // BNGGrammar_Expression.g:63:11: LPAREN expression2 RPAREN
                    {
                    match(input,LPAREN,FOLLOW_LPAREN_in_primitive_element331); 
                    pushFollow(FOLLOW_expression2_in_primitive_element333);
                    expression25=expression2();

                    state._fsp--;

                    match(input,RPAREN,FOLLOW_RPAREN_in_primitive_element335); 
                    retval.value = (expression25!=null?expression25.value:null);

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
    // BNGGrammar_Expression.g:66:1: number returns [Double value] : (s1= INT | s1= FLOAT );
    public final BNGGrammar_BNGGrammar_Expression.number_return number() throws RecognitionException {
        BNGGrammar_BNGGrammar_Expression.number_return retval = new BNGGrammar_BNGGrammar_Expression.number_return();
        retval.start = input.LT(1);

        Token s1=null;

        try {
            // BNGGrammar_Expression.g:66:30: (s1= INT | s1= FLOAT )
            int alt8=2;
            int LA8_0 = input.LA(1);

            if ( (LA8_0==INT) ) {
                alt8=1;
            }
            else if ( (LA8_0==FLOAT) ) {
                alt8=2;
            }
            else {
                NoViableAltException nvae =
                    new NoViableAltException("", 8, 0, input);

                throw nvae;
            }
            switch (alt8) {
                case 1 :
                    // BNGGrammar_Expression.g:66:32: s1= INT
                    {
                    s1=(Token)match(input,INT,FOLLOW_INT_in_number359); 
                    retval.value = Double.parseDouble((s1!=null?s1.getText():null));

                    }
                    break;
                case 2 :
                    // BNGGrammar_Expression.g:67:9: s1= FLOAT
                    {
                    s1=(Token)match(input,FLOAT,FOLLOW_FLOAT_in_number373); 
                     retval.value = Double.parseDouble((s1!=null?s1.getText():null));

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
    // BNGGrammar_Expression.g:70:1: variable returns [Double value] : STRING ;
    public final BNGGrammar_BNGGrammar_Expression.variable_return variable() throws RecognitionException {
        BNGGrammar_BNGGrammar_Expression.variable_return retval = new BNGGrammar_BNGGrammar_Expression.variable_return();
        retval.start = input.LT(1);

        Token STRING6=null;

        try {
            // BNGGrammar_Expression.g:70:65: ( STRING )
            // BNGGrammar_Expression.g:70:67: STRING
            {
            STRING6=(Token)match(input,STRING,FOLLOW_STRING_in_variable397); 

                        
                              Register temp = ((expression_scope)expression_stack.peek()).lmemory.get((STRING6!=null?STRING6.getText():null));
                              //if(retval.value == null)
                                //throw [VariableNotFoundException]
                              retval.value = temp.getValue();
                              ((expression_scope)expression_stack.peek()).references.put((STRING6!=null?STRING6.getText():null),((expression_scope)expression_stack.peek()).lmemory.get((STRING6!=null?STRING6.getText():null)));
                              

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

    // Delegated rules


 

    public static final BitSet FOLLOW_expression2_in_expression63 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_relational_expression_in_expression285 = new BitSet(new long[]{0x0000000180000002L});
    public static final BitSet FOLLOW_set_in_expression290 = new BitSet(new long[]{0x0000100000000000L,0x0000002000201600L});
    public static final BitSet FOLLOW_relational_expression_in_expression298 = new BitSet(new long[]{0x0000000180000002L});
    public static final BitSet FOLLOW_adding_expression_in_relational_expression121 = new BitSet(new long[]{0x0000000000000002L,0x00000002F0000000L});
    public static final BitSet FOLLOW_set_in_relational_expression125 = new BitSet(new long[]{0x0000100000000000L,0x0000002000201600L});
    public static final BitSet FOLLOW_adding_expression_in_relational_expression137 = new BitSet(new long[]{0x0000000000000002L,0x00000002F0000000L});
    public static final BitSet FOLLOW_multiplying_expression_in_adding_expression154 = new BitSet(new long[]{0x0000000000000002L,0x0000006000000000L});
    public static final BitSet FOLLOW_PLUS_in_adding_expression161 = new BitSet(new long[]{0x0000100000000000L,0x0000002000201600L});
    public static final BitSet FOLLOW_multiplying_expression_in_adding_expression165 = new BitSet(new long[]{0x0000000000000002L,0x0000006000000000L});
    public static final BitSet FOLLOW_MINUS_in_adding_expression171 = new BitSet(new long[]{0x0000100000000000L,0x0000002000201600L});
    public static final BitSet FOLLOW_multiplying_expression_in_adding_expression175 = new BitSet(new long[]{0x0000000000000002L,0x0000006000000000L});
    public static final BitSet FOLLOW_sign_expression_in_multiplying_expression194 = new BitSet(new long[]{0x0000000000000002L,0x0000009800000000L});
    public static final BitSet FOLLOW_TIMES_in_multiplying_expression201 = new BitSet(new long[]{0x0000100000000000L,0x0000002000201600L});
    public static final BitSet FOLLOW_sign_expression_in_multiplying_expression205 = new BitSet(new long[]{0x0000000000000002L,0x0000009800000000L});
    public static final BitSet FOLLOW_DIV_in_multiplying_expression211 = new BitSet(new long[]{0x0000100000000000L,0x0000002000201600L});
    public static final BitSet FOLLOW_sign_expression_in_multiplying_expression215 = new BitSet(new long[]{0x0000000000000002L,0x0000009800000000L});
    public static final BitSet FOLLOW_MOD_in_multiplying_expression221 = new BitSet(new long[]{0x0000100000000000L,0x0000002000201600L});
    public static final BitSet FOLLOW_sign_expression_in_multiplying_expression225 = new BitSet(new long[]{0x0000000000000002L,0x0000009800000000L});
    public static final BitSet FOLLOW_MINUS_in_sign_expression247 = new BitSet(new long[]{0x0000100000000000L,0x0000002000201600L});
    public static final BitSet FOLLOW_boolean_negation_expression_in_sign_expression254 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_NOT_in_boolean_negation_expression272 = new BitSet(new long[]{0x0000100000000000L,0x0000002000201600L});
    public static final BitSet FOLLOW_primitive_element_in_boolean_negation_expression278 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_number_in_primitive_element302 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_variable_in_primitive_element317 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_LPAREN_in_primitive_element331 = new BitSet(new long[]{0x0000100000000000L,0x0000002000201600L});
    public static final BitSet FOLLOW_expression2_in_primitive_element333 = new BitSet(new long[]{0x0000000000000000L,0x0000000000400000L});
    public static final BitSet FOLLOW_RPAREN_in_primitive_element335 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_INT_in_number359 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_FLOAT_in_number373 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_STRING_in_variable397 = new BitSet(new long[]{0x0000000000000002L});

}