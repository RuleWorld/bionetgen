// $ANTLR 3.3 Nov 30, 2010 12:50:56 BNGGrammar_Parameters.g 2012-01-25 16:16:34

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
public class BNGGrammar_BNGGrammar_Parameters extends Parser {
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
    // BNGGrammar_Parameters.g:15:1: parameters_block[ Map<String,Register> memory, List parameters] : BEGIN PARAMETERS ( parameter_def[$memory] )* END PARAMETERS ;
    public final BNGGrammar_BNGGrammar_Parameters.parameters_block_return parameters_block(Map<String,Register> memory, List parameters) throws RecognitionException {
        parameters_block_stack.push(new parameters_block_scope());
        BNGGrammar_BNGGrammar_Parameters.parameters_block_return retval = new BNGGrammar_BNGGrammar_Parameters.parameters_block_return();
        retval.start = input.LT(1);

        BNGGrammar_BNGGrammar_Parameters.parameter_def_return parameter_def1 = null;



          getParentTemplate();
          ((parameters_block_scope)parameters_block_stack.peek()).lmemory = memory;
         // System.out.println(gParent.netGrammar);

        try {
            // BNGGrammar_Parameters.g:25:1: ( BEGIN PARAMETERS ( parameter_def[$memory] )* END PARAMETERS )
            // BNGGrammar_Parameters.g:25:3: BEGIN PARAMETERS ( parameter_def[$memory] )* END PARAMETERS
            {
            match(input,BEGIN,FOLLOW_BEGIN_in_parameters_block51); 
            match(input,PARAMETERS,FOLLOW_PARAMETERS_in_parameters_block53); 
            // BNGGrammar_Parameters.g:26:1: ( parameter_def[$memory] )*
            loop1:
            do {
                int alt1=2;
                int LA1_0 = input.LA(1);

                if ( (LA1_0==INT||LA1_0==STRING) ) {
                    alt1=1;
                }


                switch (alt1) {
            	case 1 :
            	    // BNGGrammar_Parameters.g:26:2: parameter_def[$memory]
            	    {
            	    pushFollow(FOLLOW_parameter_def_in_parameters_block57);
            	    parameter_def1=parameter_def(memory);

            	    state._fsp--;

            	    parameters.add((parameter_def1!=null?parameter_def1.st:null));

            	    }
            	    break;

            	default :
            	    break loop1;
                }
            } while (true);

            match(input,END,FOLLOW_END_in_parameters_block67); 
            match(input,PARAMETERS,FOLLOW_PARAMETERS_in_parameters_block69); 

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
    // BNGGrammar_Parameters.g:31:1: parameter_def[Map<String,Register> lmemory] : ({...}? INT | ) STRING ( BECOMES )? expression[lmemory] -> parameter_def(id=$STRING.textvalue=$expression.text);
    public final BNGGrammar_BNGGrammar_Parameters.parameter_def_return parameter_def(Map<String,Register> lmemory) throws RecognitionException {
        BNGGrammar_BNGGrammar_Parameters.parameter_def_return retval = new BNGGrammar_BNGGrammar_Parameters.parameter_def_return();
        retval.start = input.LT(1);

        Token STRING2=null;
        BNGGrammar_BNGGrammar_Expression.expression_return expression3 = null;


        try {
            // BNGGrammar_Parameters.g:32:1: ( ({...}? INT | ) STRING ( BECOMES )? expression[lmemory] -> parameter_def(id=$STRING.textvalue=$expression.text))
            // BNGGrammar_Parameters.g:33:1: ({...}? INT | ) STRING ( BECOMES )? expression[lmemory]
            {
            // BNGGrammar_Parameters.g:33:1: ({...}? INT | )
            int alt2=2;
            int LA2_0 = input.LA(1);

            if ( (LA2_0==INT) ) {
                alt2=1;
            }
            else if ( (LA2_0==STRING) ) {
                alt2=2;
            }
            else {
                NoViableAltException nvae =
                    new NoViableAltException("", 2, 0, input);

                throw nvae;
            }
            switch (alt2) {
                case 1 :
                    // BNGGrammar_Parameters.g:33:2: {...}? INT
                    {
                    if ( !((gParent.netGrammar)) ) {
                        throw new FailedPredicateException(input, "parameter_def", "gParent.netGrammar");
                    }
                    match(input,INT,FOLLOW_INT_in_parameter_def82); 

                    }
                    break;
                case 2 :
                    // BNGGrammar_Parameters.g:33:31: 
                    {
                    }
                    break;

            }

            STRING2=(Token)match(input,STRING,FOLLOW_STRING_in_parameter_def89); 
            // BNGGrammar_Parameters.g:33:40: ( BECOMES )?
            int alt3=2;
            int LA3_0 = input.LA(1);

            if ( (LA3_0==BECOMES) ) {
                alt3=1;
            }
            switch (alt3) {
                case 1 :
                    // BNGGrammar_Parameters.g:33:41: BECOMES
                    {
                    match(input,BECOMES,FOLLOW_BECOMES_in_parameter_def92); 

                    }
                    break;

            }

            pushFollow(FOLLOW_expression_in_parameter_def96);
            expression3=gBNGGrammar.expression(lmemory);

            state._fsp--;


                    lmemory.put((STRING2!=null?STRING2.getText():null),new Register((expression3!=null?expression3.value:null),"parameter"));
                    


            // TEMPLATE REWRITE
            // 35:13: -> parameter_def(id=$STRING.textvalue=$expression.text)
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


 

    public static final BitSet FOLLOW_BEGIN_in_parameters_block51 = new BitSet(new long[]{0x0000002000000000L});
    public static final BitSet FOLLOW_PARAMETERS_in_parameters_block53 = new BitSet(new long[]{0x0000000000000080L,0x0000000000001400L});
    public static final BitSet FOLLOW_parameter_def_in_parameters_block57 = new BitSet(new long[]{0x0000000000000080L,0x0000000000001400L});
    public static final BitSet FOLLOW_END_in_parameters_block67 = new BitSet(new long[]{0x0000002000000000L});
    public static final BitSet FOLLOW_PARAMETERS_in_parameters_block69 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_INT_in_parameter_def82 = new BitSet(new long[]{0x0000000000000000L,0x0000000000001000L});
    public static final BitSet FOLLOW_STRING_in_parameter_def89 = new BitSet(new long[]{0x0000100000000000L,0x0000002400201600L});
    public static final BitSet FOLLOW_BECOMES_in_parameter_def92 = new BitSet(new long[]{0x0000100000000000L,0x0000002400201600L});
    public static final BitSet FOLLOW_expression_in_parameter_def96 = new BitSet(new long[]{0x0000000000000002L});

}