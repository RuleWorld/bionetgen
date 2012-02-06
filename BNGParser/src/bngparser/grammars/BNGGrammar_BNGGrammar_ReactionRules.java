// $ANTLR 3.3 Nov 30, 2010 12:50:56 BNGGrammar_ReactionRules.g 2012-01-25 16:16:35

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
public class BNGGrammar_BNGGrammar_ReactionRules extends Parser {
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
    // BNGGrammar_ReactionRules.g:15:1: reaction_rules_block[List reactionRules] : BEGIN REACTION RULES ( reaction_rule_def[$reaction_rules_block::reactionCounter] )* END REACTION RULES ;
    public final BNGGrammar_BNGGrammar_ReactionRules.reaction_rules_block_return reaction_rules_block(List reactionRules) throws RecognitionException {
        reaction_rules_block_stack.push(new reaction_rules_block_scope());
        BNGGrammar_BNGGrammar_ReactionRules.reaction_rules_block_return retval = new BNGGrammar_BNGGrammar_ReactionRules.reaction_rules_block_return();
        retval.start = input.LT(1);

        BNGGrammar_BNGGrammar_ReactionRules.reaction_rule_def_return reaction_rule_def1 = null;



        getParentTemplate();
          ((reaction_rules_block_scope)reaction_rules_block_stack.peek()).reactionCounter = 1;
          gParent.paraphrases.push("in reaction rules block"); 

        try {
            // BNGGrammar_ReactionRules.g:29:9: ( BEGIN REACTION RULES ( reaction_rule_def[$reaction_rules_block::reactionCounter] )* END REACTION RULES )
            // BNGGrammar_ReactionRules.g:29:11: BEGIN REACTION RULES ( reaction_rule_def[$reaction_rules_block::reactionCounter] )* END REACTION RULES
            {
            match(input,BEGIN,FOLLOW_BEGIN_in_reaction_rules_block61); 
            match(input,REACTION,FOLLOW_REACTION_in_reaction_rules_block63); 
            match(input,RULES,FOLLOW_RULES_in_reaction_rules_block65); 
            // BNGGrammar_ReactionRules.g:29:32: ( reaction_rule_def[$reaction_rules_block::reactionCounter] )*
            loop1:
            do {
                int alt1=2;
                int LA1_0 = input.LA(1);

                if ( (LA1_0==INT||LA1_0==STRING||LA1_0==LBRACKET||LA1_0==DOLLAR||LA1_0==AT) ) {
                    alt1=1;
                }


                switch (alt1) {
            	case 1 :
            	    // BNGGrammar_ReactionRules.g:29:33: reaction_rule_def[$reaction_rules_block::reactionCounter]
            	    {
            	    pushFollow(FOLLOW_reaction_rule_def_in_reaction_rules_block68);
            	    reaction_rule_def1=reaction_rule_def(((reaction_rules_block_scope)reaction_rules_block_stack.peek()).reactionCounter);

            	    state._fsp--;


            	                      reactionRules.add((reaction_rule_def1!=null?reaction_rule_def1.st:null));
            	                      StringTemplate sInvert = null;
            	                      if((reaction_rule_def1!=null?reaction_rule_def1.numReactions:0) == 2)
            	                        sInvert = InvertBidirectional.invert((reaction_rule_def1!=null?reaction_rule_def1.st:null).toString(),((reaction_rules_block_scope)reaction_rules_block_stack.peek()).reactionCounter+1);
            	                      reactionRules.add(sInvert);
            	                      ((reaction_rules_block_scope)reaction_rules_block_stack.peek()).reactionCounter+= (reaction_rule_def1!=null?reaction_rule_def1.numReactions:0);
            	                      
            	                      

            	    }
            	    break;

            	default :
            	    break loop1;
                }
            } while (true);

            match(input,END,FOLLOW_END_in_reaction_rules_block94); 
            match(input,REACTION,FOLLOW_REACTION_in_reaction_rules_block96); 
            match(input,RULES,FOLLOW_RULES_in_reaction_rules_block98); 

            }

            retval.stop = input.LT(-1);


              gParent.paraphrases.pop();

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
    // BNGGrammar_ReactionRules.g:43:1: reactionLabel returns [String label] : ( STRING | INT )+ COLON ;
    public final BNGGrammar_BNGGrammar_ReactionRules.reactionLabel_return reactionLabel() throws RecognitionException {
        BNGGrammar_BNGGrammar_ReactionRules.reactionLabel_return retval = new BNGGrammar_BNGGrammar_ReactionRules.reactionLabel_return();
        retval.start = input.LT(1);

        Token STRING2=null;
        Token INT3=null;


          retval.label = "";
          
        try {
            // BNGGrammar_ReactionRules.g:47:1: ( ( STRING | INT )+ COLON )
            // BNGGrammar_ReactionRules.g:48:1: ( STRING | INT )+ COLON
            {
            // BNGGrammar_ReactionRules.g:48:1: ( STRING | INT )+
            int cnt2=0;
            loop2:
            do {
                int alt2=3;
                int LA2_0 = input.LA(1);

                if ( (LA2_0==STRING) ) {
                    alt2=1;
                }
                else if ( (LA2_0==INT) ) {
                    alt2=2;
                }


                switch (alt2) {
            	case 1 :
            	    // BNGGrammar_ReactionRules.g:48:2: STRING
            	    {
            	    STRING2=(Token)match(input,STRING,FOLLOW_STRING_in_reactionLabel126); 
            	    retval.label += (STRING2!=null?STRING2.getText():null) + " ";

            	    }
            	    break;
            	case 2 :
            	    // BNGGrammar_ReactionRules.g:49:12: INT
            	    {
            	    INT3=(Token)match(input,INT,FOLLOW_INT_in_reactionLabel141); 
            	    retval.label += (INT3!=null?INT3.getText():null) + " ";

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

            match(input,COLON,FOLLOW_COLON_in_reactionLabel154); 

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

    protected static class reaction_rule_def_scope {
        List patternsReactants;
        List patternsProducts;
        List<Double> rateList;
        ReactionAction reactionAction;
        String name;
        String text;
    }
    protected Stack reaction_rule_def_stack = new Stack();

    public static class reaction_rule_def_return extends ParserRuleReturnScope {
        public int numReactions;
        public double secondRate;
        public StringTemplate st;
        public Object getTemplate() { return st; }
        public String toString() { return st==null?null:st.toString(); }
    };

    // $ANTLR start "reaction_rule_def"
    // BNGGrammar_ReactionRules.g:53:1: reaction_rule_def[int reactionCounter] returns [int numReactions, double secondRate] : ( ( match_attribute )? (l1= reactionLabel )? ) reaction_def[$reaction_rule_def::patternsReactants,$reaction_rule_def::patternsProducts,\"RR\" + reactionCounter] WS bi= rate_list[$reaction_rule_def::rateList] ( modif_command )* ( DELETEMOLECULES )? ( MOVECONNECTED )? -> reaction_block(id=\"RR\" + reactionCounterreactant=$reaction_rule_def::patternsReactantsproduct=$reaction_rule_def::patternsProductsname=$reaction_rule_def::namerate=$reaction_rule_def::rateList.get(0)bidirectional=bileftMap=$reaction_rule_def::reactionAction.getLeft()rightMap=$reaction_rule_def::reactionAction.getRight()operations=$reaction_rule_def::reactionAction.getOperations()operator1=$reaction_rule_def::reactionAction.getOperator1()operator2=$reaction_rule_def::reactionAction.getOperator2()expression=$reaction_rule_def::text);
    public final BNGGrammar_BNGGrammar_ReactionRules.reaction_rule_def_return reaction_rule_def(int reactionCounter) throws RecognitionException {
        reaction_rule_def_stack.push(new reaction_rule_def_scope());
        BNGGrammar_BNGGrammar_ReactionRules.reaction_rule_def_return retval = new BNGGrammar_BNGGrammar_ReactionRules.reaction_rule_def_return();
        retval.start = input.LT(1);

        BNGGrammar_BNGGrammar_ReactionRules.reactionLabel_return l1 = null;

        BNGGrammar_BNGGrammar_ReactionRules.rate_list_return bi = null;

        BNGGrammar_BNGGrammar_ReactionRules.reaction_def_return reaction_def4 = null;



          ((reaction_rule_def_scope)reaction_rule_def_stack.peek()).patternsReactants = new ArrayList();
          ((reaction_rule_def_scope)reaction_rule_def_stack.peek()).patternsProducts = new ArrayList();
          ((reaction_rule_def_scope)reaction_rule_def_stack.peek()).rateList = new ArrayList<Double>();
          ((reaction_rule_def_scope)reaction_rule_def_stack.peek()).reactionAction = new ReactionAction();
          ((reaction_rule_def_scope)reaction_rule_def_stack.peek()).name = "Rule" + reactionCounter;
          ((reaction_rule_def_scope)reaction_rule_def_stack.peek()).text = "";

        try {
            // BNGGrammar_ReactionRules.g:70:9: ( ( ( match_attribute )? (l1= reactionLabel )? ) reaction_def[$reaction_rule_def::patternsReactants,$reaction_rule_def::patternsProducts,\"RR\" + reactionCounter] WS bi= rate_list[$reaction_rule_def::rateList] ( modif_command )* ( DELETEMOLECULES )? ( MOVECONNECTED )? -> reaction_block(id=\"RR\" + reactionCounterreactant=$reaction_rule_def::patternsReactantsproduct=$reaction_rule_def::patternsProductsname=$reaction_rule_def::namerate=$reaction_rule_def::rateList.get(0)bidirectional=bileftMap=$reaction_rule_def::reactionAction.getLeft()rightMap=$reaction_rule_def::reactionAction.getRight()operations=$reaction_rule_def::reactionAction.getOperations()operator1=$reaction_rule_def::reactionAction.getOperator1()operator2=$reaction_rule_def::reactionAction.getOperator2()expression=$reaction_rule_def::text))
            // BNGGrammar_ReactionRules.g:70:12: ( ( match_attribute )? (l1= reactionLabel )? ) reaction_def[$reaction_rule_def::patternsReactants,$reaction_rule_def::patternsProducts,\"RR\" + reactionCounter] WS bi= rate_list[$reaction_rule_def::rateList] ( modif_command )* ( DELETEMOLECULES )? ( MOVECONNECTED )?
            {
            // BNGGrammar_ReactionRules.g:70:12: ( ( match_attribute )? (l1= reactionLabel )? )
            // BNGGrammar_ReactionRules.g:70:13: ( match_attribute )? (l1= reactionLabel )?
            {
            // BNGGrammar_ReactionRules.g:70:13: ( match_attribute )?
            int alt3=2;
            int LA3_0 = input.LA(1);

            if ( (LA3_0==LBRACKET) ) {
                alt3=1;
            }
            switch (alt3) {
                case 1 :
                    // BNGGrammar_ReactionRules.g:70:14: match_attribute
                    {
                    pushFollow(FOLLOW_match_attribute_in_reaction_rule_def187);
                    match_attribute();

                    state._fsp--;


                    }
                    break;

            }

            // BNGGrammar_ReactionRules.g:70:32: (l1= reactionLabel )?
            int alt4=2;
            int LA4_0 = input.LA(1);

            if ( (LA4_0==STRING) ) {
                int LA4_1 = input.LA(2);

                if ( (LA4_1==INT||LA4_1==STRING||LA4_1==COLON) ) {
                    alt4=1;
                }
            }
            else if ( (LA4_0==INT) ) {
                int LA4_2 = input.LA(2);

                if ( (LA4_2==INT||LA4_2==STRING||LA4_2==COLON) ) {
                    alt4=1;
                }
            }
            switch (alt4) {
                case 1 :
                    // BNGGrammar_ReactionRules.g:72:10: l1= reactionLabel
                    {
                    pushFollow(FOLLOW_reactionLabel_in_reaction_rule_def215);
                    l1=reactionLabel();

                    state._fsp--;

                    ((reaction_rule_def_scope)reaction_rule_def_stack.peek()).text += (l1!=null?l1.label:null) + ":"; 
                    	                         ((reaction_rule_def_scope)reaction_rule_def_stack.peek()).name = (l1!=null?l1.label:null);
                    	                  

                    }
                    break;

            }


            }

            pushFollow(FOLLOW_reaction_def_in_reaction_rule_def254);
            reaction_def4=reaction_def(((reaction_rule_def_scope)reaction_rule_def_stack.peek()).patternsReactants, ((reaction_rule_def_scope)reaction_rule_def_stack.peek()).patternsProducts, "RR" + reactionCounter);

            state._fsp--;


                      ((reaction_rule_def_scope)reaction_rule_def_stack.peek()).reactionAction.execute();
                      if((reaction_def4!=null?reaction_def4.bidirectional:false))
                          retval.numReactions = 2;
                       else
                          retval.numReactions = 1;
                       
                      ((reaction_rule_def_scope)reaction_rule_def_stack.peek()).text += (reaction_def4!=null?input.toString(reaction_def4.start,reaction_def4.stop):null);
                    
                    

                    //Whitespaces are normally skipped but they are still in the stream. In this case if this rule is valid
                    //a valid WS would be located on the previous token
                    ((ChangeableChannelTokenStream)input).seek(((ChangeableChannelTokenStream)input).index()-1)  ;
                    
            match(input,WS,FOLLOW_WS_in_reaction_rule_def277); 
            pushFollow(FOLLOW_rate_list_in_reaction_rule_def289);
            bi=rate_list(((reaction_rule_def_scope)reaction_rule_def_stack.peek()).rateList);

            state._fsp--;

            retval.secondRate =((reaction_rule_def_scope)reaction_rule_def_stack.peek()).rateList.get(1);((reaction_rule_def_scope)reaction_rule_def_stack.peek()).text += " " + (bi!=null?input.toString(bi.start,bi.stop):null);
            // BNGGrammar_ReactionRules.g:95:9: ( modif_command )*
            loop5:
            do {
                int alt5=2;
                int LA5_0 = input.LA(1);

                if ( ((LA5_0>=INCLUDE_REACTANTS && LA5_0<=EXCLUDE_PRODUCTS)) ) {
                    alt5=1;
                }


                switch (alt5) {
            	case 1 :
            	    // BNGGrammar_ReactionRules.g:95:10: modif_command
            	    {
            	    pushFollow(FOLLOW_modif_command_in_reaction_rule_def303);
            	    modif_command();

            	    state._fsp--;


            	    }
            	    break;

            	default :
            	    break loop5;
                }
            } while (true);

            // BNGGrammar_ReactionRules.g:95:26: ( DELETEMOLECULES )?
            int alt6=2;
            int LA6_0 = input.LA(1);

            if ( (LA6_0==DELETEMOLECULES) ) {
                alt6=1;
            }
            switch (alt6) {
                case 1 :
                    // BNGGrammar_ReactionRules.g:95:27: DELETEMOLECULES
                    {
                    match(input,DELETEMOLECULES,FOLLOW_DELETEMOLECULES_in_reaction_rule_def308); 

                    }
                    break;

            }

            // BNGGrammar_ReactionRules.g:95:45: ( MOVECONNECTED )?
            int alt7=2;
            int LA7_0 = input.LA(1);

            if ( (LA7_0==MOVECONNECTED) ) {
                alt7=1;
            }
            switch (alt7) {
                case 1 :
                    // BNGGrammar_ReactionRules.g:95:46: MOVECONNECTED
                    {
                    match(input,MOVECONNECTED,FOLLOW_MOVECONNECTED_in_reaction_rule_def313); 

                    }
                    break;

            }


                      ((reaction_rule_def_scope)reaction_rule_def_stack.peek()).text = ((reaction_rule_def_scope)reaction_rule_def_stack.peek()).text.replaceAll("<","&lt;");
                      ((reaction_rule_def_scope)reaction_rule_def_stack.peek()).text = ((reaction_rule_def_scope)reaction_rule_def_stack.peek()).text.replaceAll(">","&gt;");
                    


            // TEMPLATE REWRITE
            // 102:9: -> reaction_block(id=\"RR\" + reactionCounterreactant=$reaction_rule_def::patternsReactantsproduct=$reaction_rule_def::patternsProductsname=$reaction_rule_def::namerate=$reaction_rule_def::rateList.get(0)bidirectional=bileftMap=$reaction_rule_def::reactionAction.getLeft()rightMap=$reaction_rule_def::reactionAction.getRight()operations=$reaction_rule_def::reactionAction.getOperations()operator1=$reaction_rule_def::reactionAction.getOperator1()operator2=$reaction_rule_def::reactionAction.getOperator2()expression=$reaction_rule_def::text)
            {
                retval.st = templateLib.getInstanceOf("reaction_block",
              new STAttrMap().put("id", "RR" + reactionCounter).put("reactant", ((reaction_rule_def_scope)reaction_rule_def_stack.peek()).patternsReactants).put("product", ((reaction_rule_def_scope)reaction_rule_def_stack.peek()).patternsProducts).put("name", ((reaction_rule_def_scope)reaction_rule_def_stack.peek()).name).put("rate", ((reaction_rule_def_scope)reaction_rule_def_stack.peek()).rateList.get(0)).put("bidirectional", bi).put("leftMap", ((reaction_rule_def_scope)reaction_rule_def_stack.peek()).reactionAction.getLeft()).put("rightMap", ((reaction_rule_def_scope)reaction_rule_def_stack.peek()).reactionAction.getRight()).put("operations", ((reaction_rule_def_scope)reaction_rule_def_stack.peek()).reactionAction.getOperations()).put("operator1", ((reaction_rule_def_scope)reaction_rule_def_stack.peek()).reactionAction.getOperator1()).put("operator2", ((reaction_rule_def_scope)reaction_rule_def_stack.peek()).reactionAction.getOperator2()).put("expression", ((reaction_rule_def_scope)reaction_rule_def_stack.peek()).text));
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
    // BNGGrammar_ReactionRules.g:108:1: match_attribute : LBRACKET MATCHONCE RBRACKET ;
    public final BNGGrammar_BNGGrammar_ReactionRules.match_attribute_return match_attribute() throws RecognitionException {
        BNGGrammar_BNGGrammar_ReactionRules.match_attribute_return retval = new BNGGrammar_BNGGrammar_ReactionRules.match_attribute_return();
        retval.start = input.LT(1);

        try {
            // BNGGrammar_ReactionRules.g:109:9: ( LBRACKET MATCHONCE RBRACKET )
            // BNGGrammar_ReactionRules.g:109:11: LBRACKET MATCHONCE RBRACKET
            {
            match(input,LBRACKET,FOLLOW_LBRACKET_in_match_attribute456); 
            match(input,MATCHONCE,FOLLOW_MATCHONCE_in_match_attribute458); 
            match(input,RBRACKET,FOLLOW_RBRACKET_in_match_attribute460); 

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
    // BNGGrammar_ReactionRules.g:112:1: reaction_def[List patternsReactants,List patternsProducts,String upperID] returns [boolean bidirectional,Map<String,List<ReactionRegister>> reactionStack,\nMap<String,List<ReactionRegister>> productStack] : s1= rule_species_def[upperID+\"_RP\" + $reaction_def::reactantPatternCounter,$reaction_rule_def::reactionAction] ( PLUS s2= rule_species_def[upperID+\"_RP\"+ $reaction_def::reactantPatternCounter,$reaction_rule_def::reactionAction] )* ( UNI_REACTION_SIGN | BI_REACTION_SIGN ) (s3= rule_species_def[upperID+\"_PP\"+ 1,$reaction_rule_def::reactionAction] ) ( PLUS s4= rule_species_def[upperID+\"_PP\"+ $reaction_def::reactantPatternCounter,$reaction_rule_def::reactionAction] )* ;
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
            // BNGGrammar_ReactionRules.g:127:1: (s1= rule_species_def[upperID+\"_RP\" + $reaction_def::reactantPatternCounter,$reaction_rule_def::reactionAction] ( PLUS s2= rule_species_def[upperID+\"_RP\"+ $reaction_def::reactantPatternCounter,$reaction_rule_def::reactionAction] )* ( UNI_REACTION_SIGN | BI_REACTION_SIGN ) (s3= rule_species_def[upperID+\"_PP\"+ 1,$reaction_rule_def::reactionAction] ) ( PLUS s4= rule_species_def[upperID+\"_PP\"+ $reaction_def::reactantPatternCounter,$reaction_rule_def::reactionAction] )* )
            // BNGGrammar_ReactionRules.g:128:2: s1= rule_species_def[upperID+\"_RP\" + $reaction_def::reactantPatternCounter,$reaction_rule_def::reactionAction] ( PLUS s2= rule_species_def[upperID+\"_RP\"+ $reaction_def::reactantPatternCounter,$reaction_rule_def::reactionAction] )* ( UNI_REACTION_SIGN | BI_REACTION_SIGN ) (s3= rule_species_def[upperID+\"_PP\"+ 1,$reaction_rule_def::reactionAction] ) ( PLUS s4= rule_species_def[upperID+\"_PP\"+ $reaction_def::reactantPatternCounter,$reaction_rule_def::reactionAction] )*
            {
            pushFollow(FOLLOW_rule_species_def_in_reaction_def503);
            s1=rule_species_def(upperID+"_RP" + ((reaction_def_scope)reaction_def_stack.peek()).reactantPatternCounter, ((reaction_rule_def_scope)reaction_rule_def_stack.peek()).reactionAction);

            state._fsp--;


                        int counter = ((reaction_def_scope)reaction_def_stack.peek()).reactantPatternCounter;
                        for(int i=0;i<s1.stoichiometry;i++){
                           StringTemplate correctedString = GenericMethods.replace(s1.st,"RP" + counter,"RP" + ((reaction_def_scope)reaction_def_stack.peek()).reactantPatternCounter);
                           patternsReactants.add(correctedString);
                           ReactionRegister.mergeMaps((s1!=null?s1.map:null),retval.reactionStack);
                          ((reaction_def_scope)reaction_def_stack.peek()).reactantPatternCounter++;
                        } 
                       
            // BNGGrammar_ReactionRules.g:140:3: ( PLUS s2= rule_species_def[upperID+\"_RP\"+ $reaction_def::reactantPatternCounter,$reaction_rule_def::reactionAction] )*
            loop8:
            do {
                int alt8=2;
                int LA8_0 = input.LA(1);

                if ( (LA8_0==PLUS) ) {
                    alt8=1;
                }


                switch (alt8) {
            	case 1 :
            	    // BNGGrammar_ReactionRules.g:140:4: PLUS s2= rule_species_def[upperID+\"_RP\"+ $reaction_def::reactantPatternCounter,$reaction_rule_def::reactionAction]
            	    {
            	    match(input,PLUS,FOLLOW_PLUS_in_reaction_def548); 
            	    pushFollow(FOLLOW_rule_species_def_in_reaction_def552);
            	    s2=rule_species_def(upperID+"_RP"+ ((reaction_def_scope)reaction_def_stack.peek()).reactantPatternCounter, ((reaction_rule_def_scope)reaction_rule_def_stack.peek()).reactionAction);

            	    state._fsp--;


            	                counter = ((reaction_def_scope)reaction_def_stack.peek()).reactantPatternCounter;
            	                for(int i=0;i<s2.stoichiometry;i++){ 
            	                  StringTemplate correctedString = GenericMethods.replace(s2.st,"RP" + counter,"RP" + ((reaction_def_scope)reaction_def_stack.peek()).reactantPatternCounter);
            	                   patternsReactants.add(correctedString);
            	                   ReactionRegister.mergeMaps((s2!=null?s2.map:null),retval.reactionStack);
            	                  ((reaction_def_scope)reaction_def_stack.peek()).reactantPatternCounter++;
            	                }
            	                

            	    }
            	    break;

            	default :
            	    break loop8;
                }
            } while (true);

            // BNGGrammar_ReactionRules.g:150:9: ( UNI_REACTION_SIGN | BI_REACTION_SIGN )
            int alt9=2;
            int LA9_0 = input.LA(1);

            if ( (LA9_0==UNI_REACTION_SIGN) ) {
                alt9=1;
            }
            else if ( (LA9_0==BI_REACTION_SIGN) ) {
                alt9=2;
            }
            else {
                NoViableAltException nvae =
                    new NoViableAltException("", 9, 0, input);

                throw nvae;
            }
            switch (alt9) {
                case 1 :
                    // BNGGrammar_ReactionRules.g:150:10: UNI_REACTION_SIGN
                    {
                    match(input,UNI_REACTION_SIGN,FOLLOW_UNI_REACTION_SIGN_in_reaction_def581); 
                    retval.bidirectional = false;

                    }
                    break;
                case 2 :
                    // BNGGrammar_ReactionRules.g:150:55: BI_REACTION_SIGN
                    {
                    match(input,BI_REACTION_SIGN,FOLLOW_BI_REACTION_SIGN_in_reaction_def586); 
                    retval.bidirectional = true;

                    }
                    break;

            }

            // BNGGrammar_ReactionRules.g:151:3: (s3= rule_species_def[upperID+\"_PP\"+ 1,$reaction_rule_def::reactionAction] )
            // BNGGrammar_ReactionRules.g:151:4: s3= rule_species_def[upperID+\"_PP\"+ 1,$reaction_rule_def::reactionAction]
            {
            pushFollow(FOLLOW_rule_species_def_in_reaction_def597);
            s3=rule_species_def(upperID+"_PP"+ 1, ((reaction_rule_def_scope)reaction_rule_def_stack.peek()).reactionAction);

            state._fsp--;


                    ((reaction_def_scope)reaction_def_stack.peek()).reactantPatternCounter =1;
                      counter = ((reaction_def_scope)reaction_def_stack.peek()).reactantPatternCounter;
                      for(int i=0;i<s3.stoichiometry;i++){ 
                        StringTemplate correctedString = GenericMethods.replace(s3.st,"PP" + counter,"PP" + ((reaction_def_scope)reaction_def_stack.peek()).reactantPatternCounter);
                        patternsProducts.add(correctedString);
                        ReactionRegister.mergeMaps((s3!=null?s3.map:null),retval.productStack);
                        ((reaction_def_scope)reaction_def_stack.peek()).reactantPatternCounter++;
                    }
                    

            }

            // BNGGrammar_ReactionRules.g:162:9: ( PLUS s4= rule_species_def[upperID+\"_PP\"+ $reaction_def::reactantPatternCounter,$reaction_rule_def::reactionAction] )*
            loop10:
            do {
                int alt10=2;
                int LA10_0 = input.LA(1);

                if ( (LA10_0==PLUS) ) {
                    alt10=1;
                }


                switch (alt10) {
            	case 1 :
            	    // BNGGrammar_ReactionRules.g:162:10: PLUS s4= rule_species_def[upperID+\"_PP\"+ $reaction_def::reactantPatternCounter,$reaction_rule_def::reactionAction]
            	    {
            	    match(input,PLUS,FOLLOW_PLUS_in_reaction_def622); 
            	    pushFollow(FOLLOW_rule_species_def_in_reaction_def626);
            	    s4=rule_species_def(upperID+"_PP"+ ((reaction_def_scope)reaction_def_stack.peek()).reactantPatternCounter, ((reaction_rule_def_scope)reaction_rule_def_stack.peek()).reactionAction);

            	    state._fsp--;


            	                counter = ((reaction_def_scope)reaction_def_stack.peek()).reactantPatternCounter;
            	               for(int i=0;i<s4.stoichiometry;i++){ 
            	                   StringTemplate correctedString = GenericMethods.replace(s4.st,"PP" + counter,"PP" + ((reaction_def_scope)reaction_def_stack.peek()).reactantPatternCounter);
            	                   patternsProducts.add(correctedString);
            	                  ((reaction_def_scope)reaction_def_stack.peek()).reactantPatternCounter++;
            	                  ReactionRegister.mergeMaps((s4!=null?s4.map:null),retval.productStack);
            	               }
            	            

            	    }
            	    break;

            	default :
            	    break loop10;
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
        public StringTemplate st;
        public Object getTemplate() { return st; }
        public String toString() { return st==null?null:st.toString(); }
    };

    // $ANTLR start "rule_species_def"
    // BNGGrammar_ReactionRules.g:175:1: rule_species_def[String upperID,ReactionAction reactionAction] returns [int stoichiometry,Map <String,List<ReactionRegister>> map] : ( (i1= INT TIMES )? (s1= ( species_def[$rule_species_def::reactants,$rule_species_def::bonds,upperID] ) ) | i2= INT ) -> rule_seed_species_block(id=upperIDmolecules=$rule_species_def::reactantsfirstBonds=$rule_species_def::bonds.getLeft()secondBonds=$rule_species_def::bonds.getRight());
    public final BNGGrammar_BNGGrammar_ReactionRules.rule_species_def_return rule_species_def(String upperID, ReactionAction reactionAction) throws RecognitionException {
        rule_species_def_stack.push(new rule_species_def_scope());
        BNGGrammar_BNGGrammar_ReactionRules.rule_species_def_return retval = new BNGGrammar_BNGGrammar_ReactionRules.rule_species_def_return();
        retval.start = input.LT(1);

        Token i1=null;
        Token s1=null;
        Token i2=null;
        BNGGrammar_BNGGrammar_SeedSpecies.species_def_return species_def5 = null;



          ((rule_species_def_scope)rule_species_def_stack.peek()).reactants = new ArrayList();
          ((rule_species_def_scope)rule_species_def_stack.peek()).bonds = new BondList();
          retval.stoichiometry = 1;

        try {
            // BNGGrammar_ReactionRules.g:185:1: ( ( (i1= INT TIMES )? (s1= ( species_def[$rule_species_def::reactants,$rule_species_def::bonds,upperID] ) ) | i2= INT ) -> rule_seed_species_block(id=upperIDmolecules=$rule_species_def::reactantsfirstBonds=$rule_species_def::bonds.getLeft()secondBonds=$rule_species_def::bonds.getRight()))
            // BNGGrammar_ReactionRules.g:186:1: ( (i1= INT TIMES )? (s1= ( species_def[$rule_species_def::reactants,$rule_species_def::bonds,upperID] ) ) | i2= INT )
            {
            // BNGGrammar_ReactionRules.g:186:1: ( (i1= INT TIMES )? (s1= ( species_def[$rule_species_def::reactants,$rule_species_def::bonds,upperID] ) ) | i2= INT )
            int alt12=2;
            int LA12_0 = input.LA(1);

            if ( (LA12_0==INT) ) {
                int LA12_1 = input.LA(2);

                if ( (LA12_1==TIMES) ) {
                    alt12=1;
                }
                else if ( (LA12_1==WS||(LA12_1>=UNI_REACTION_SIGN && LA12_1<=BI_REACTION_SIGN)||LA12_1==PLUS) ) {
                    alt12=2;
                }
                else {
                    NoViableAltException nvae =
                        new NoViableAltException("", 12, 1, input);

                    throw nvae;
                }
            }
            else if ( (LA12_0==STRING||LA12_0==DOLLAR||LA12_0==AT) ) {
                alt12=1;
            }
            else {
                NoViableAltException nvae =
                    new NoViableAltException("", 12, 0, input);

                throw nvae;
            }
            switch (alt12) {
                case 1 :
                    // BNGGrammar_ReactionRules.g:187:1: (i1= INT TIMES )? (s1= ( species_def[$rule_species_def::reactants,$rule_species_def::bonds,upperID] ) )
                    {
                    // BNGGrammar_ReactionRules.g:187:1: (i1= INT TIMES )?
                    int alt11=2;
                    int LA11_0 = input.LA(1);

                    if ( (LA11_0==INT) ) {
                        alt11=1;
                    }
                    switch (alt11) {
                        case 1 :
                            // BNGGrammar_ReactionRules.g:187:2: i1= INT TIMES
                            {
                            i1=(Token)match(input,INT,FOLLOW_INT_in_rule_species_def686); 
                            retval.stoichiometry = Integer.parseInt((i1!=null?i1.getText():null));
                            match(input,TIMES,FOLLOW_TIMES_in_rule_species_def690); 

                            }
                            break;

                    }

                    // BNGGrammar_ReactionRules.g:188:2: (s1= ( species_def[$rule_species_def::reactants,$rule_species_def::bonds,upperID] ) )
                    // BNGGrammar_ReactionRules.g:188:3: s1= ( species_def[$rule_species_def::reactants,$rule_species_def::bonds,upperID] )
                    {
                    // BNGGrammar_ReactionRules.g:188:6: ( species_def[$rule_species_def::reactants,$rule_species_def::bonds,upperID] )
                    // BNGGrammar_ReactionRules.g:188:7: species_def[$rule_species_def::reactants,$rule_species_def::bonds,upperID]
                    {
                    pushFollow(FOLLOW_species_def_in_rule_species_def700);
                    species_def5=gBNGGrammar.species_def(((rule_species_def_scope)rule_species_def_stack.peek()).reactants, ((rule_species_def_scope)rule_species_def_stack.peek()).bonds, upperID);

                    state._fsp--;


                           reactionAction.addMolecule(upperID,(species_def5!=null?input.toString(species_def5.start,species_def5.stop):null),((rule_species_def_scope)rule_species_def_stack.peek()).bonds);
                           retval.map = (species_def5!=null?species_def5.listOfMolecules:null);
                      

                    }


                    }


                    }
                    break;
                case 2 :
                    // BNGGrammar_ReactionRules.g:192:5: i2= INT
                    {
                    i2=(Token)match(input,INT,FOLLOW_INT_in_rule_species_def714); 

                            retval.map = new HashMap<String,List<ReactionRegister>>();
                            if(!(i2!=null?i2.getText():null).equals("0")){
                            throw new RecognitionException();
                            }
                          

                    }
                    break;

            }



            // TEMPLATE REWRITE
            // 199:5: -> rule_seed_species_block(id=upperIDmolecules=$rule_species_def::reactantsfirstBonds=$rule_species_def::bonds.getLeft()secondBonds=$rule_species_def::bonds.getRight())
            {
                retval.st = templateLib.getInstanceOf("rule_seed_species_block",
              new STAttrMap().put("id", upperID).put("molecules", ((rule_species_def_scope)rule_species_def_stack.peek()).reactants).put("firstBonds", ((rule_species_def_scope)rule_species_def_stack.peek()).bonds.getLeft()).put("secondBonds", ((rule_species_def_scope)rule_species_def_stack.peek()).bonds.getRight()));
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

    public static class rate_list_return extends ParserRuleReturnScope {
        public StringTemplate st;
        public Object getTemplate() { return st; }
        public String toString() { return st==null?null:st.toString(); }
    };

    // $ANTLR start "rate_list"
    // BNGGrammar_ReactionRules.g:204:1: rate_list[List<Double> rateList] : e1= expression[gParent.memory] ( COMMA e2= expression[gParent.memory] )? ;
    public final BNGGrammar_BNGGrammar_ReactionRules.rate_list_return rate_list(List<Double> rateList) throws RecognitionException {
        BNGGrammar_BNGGrammar_ReactionRules.rate_list_return retval = new BNGGrammar_BNGGrammar_ReactionRules.rate_list_return();
        retval.start = input.LT(1);

        BNGGrammar_BNGGrammar_Expression.expression_return e1 = null;

        BNGGrammar_BNGGrammar_Expression.expression_return e2 = null;


        try {
            // BNGGrammar_ReactionRules.g:205:9: (e1= expression[gParent.memory] ( COMMA e2= expression[gParent.memory] )? )
            // BNGGrammar_ReactionRules.g:205:11: e1= expression[gParent.memory] ( COMMA e2= expression[gParent.memory] )?
            {
            pushFollow(FOLLOW_expression_in_rate_list778);
            e1=gBNGGrammar.expression(gParent.memory);

            state._fsp--;

            rateList.add(e1.value);rateList.add(0.0);
            // BNGGrammar_ReactionRules.g:205:84: ( COMMA e2= expression[gParent.memory] )?
            int alt13=2;
            int LA13_0 = input.LA(1);

            if ( (LA13_0==COMMA) ) {
                alt13=1;
            }
            switch (alt13) {
                case 1 :
                    // BNGGrammar_ReactionRules.g:205:85: COMMA e2= expression[gParent.memory]
                    {
                    match(input,COMMA,FOLLOW_COMMA_in_rate_list783); 
                    pushFollow(FOLLOW_expression_in_rate_list787);
                    e2=gBNGGrammar.expression(gParent.memory);

                    state._fsp--;

                    rateList.set(1,e2.value);

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
    // BNGGrammar_ReactionRules.g:207:1: modif_command : ( include_command | exclude_command );
    public final BNGGrammar_BNGGrammar_ReactionRules.modif_command_return modif_command() throws RecognitionException {
        BNGGrammar_BNGGrammar_ReactionRules.modif_command_return retval = new BNGGrammar_BNGGrammar_ReactionRules.modif_command_return();
        retval.start = input.LT(1);

        try {
            // BNGGrammar_ReactionRules.g:208:9: ( include_command | exclude_command )
            int alt14=2;
            int LA14_0 = input.LA(1);

            if ( ((LA14_0>=INCLUDE_REACTANTS && LA14_0<=INCLUDE_PRODUCTS)) ) {
                alt14=1;
            }
            else if ( ((LA14_0>=EXCLUDE_REACTANTS && LA14_0<=EXCLUDE_PRODUCTS)) ) {
                alt14=2;
            }
            else {
                NoViableAltException nvae =
                    new NoViableAltException("", 14, 0, input);

                throw nvae;
            }
            switch (alt14) {
                case 1 :
                    // BNGGrammar_ReactionRules.g:208:11: include_command
                    {
                    pushFollow(FOLLOW_include_command_in_modif_command816);
                    include_command();

                    state._fsp--;


                    }
                    break;
                case 2 :
                    // BNGGrammar_ReactionRules.g:209:11: exclude_command
                    {
                    pushFollow(FOLLOW_exclude_command_in_modif_command828);
                    exclude_command();

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
    // $ANTLR end "modif_command"

    public static class include_command_return extends ParserRuleReturnScope {
        public StringTemplate st;
        public Object getTemplate() { return st; }
        public String toString() { return st==null?null:st.toString(); }
    };

    // $ANTLR start "include_command"
    // BNGGrammar_ReactionRules.g:212:1: include_command : ( INCLUDE_REACTANTS | INCLUDE_PRODUCTS ) LPAREN INT COMMA pattern ( COMMA pattern )* RPAREN ;
    public final BNGGrammar_BNGGrammar_ReactionRules.include_command_return include_command() throws RecognitionException {
        BNGGrammar_BNGGrammar_ReactionRules.include_command_return retval = new BNGGrammar_BNGGrammar_ReactionRules.include_command_return();
        retval.start = input.LT(1);

        try {
            // BNGGrammar_ReactionRules.g:213:9: ( ( INCLUDE_REACTANTS | INCLUDE_PRODUCTS ) LPAREN INT COMMA pattern ( COMMA pattern )* RPAREN )
            // BNGGrammar_ReactionRules.g:213:12: ( INCLUDE_REACTANTS | INCLUDE_PRODUCTS ) LPAREN INT COMMA pattern ( COMMA pattern )* RPAREN
            {
            if ( (input.LA(1)>=INCLUDE_REACTANTS && input.LA(1)<=INCLUDE_PRODUCTS) ) {
                input.consume();
                state.errorRecovery=false;
            }
            else {
                MismatchedSetException mse = new MismatchedSetException(null,input);
                throw mse;
            }

            match(input,LPAREN,FOLLOW_LPAREN_in_include_command882); 
            match(input,INT,FOLLOW_INT_in_include_command884); 
            match(input,COMMA,FOLLOW_COMMA_in_include_command886); 
            pushFollow(FOLLOW_pattern_in_include_command888);
            pattern();

            state._fsp--;

            // BNGGrammar_ReactionRules.g:215:36: ( COMMA pattern )*
            loop15:
            do {
                int alt15=2;
                int LA15_0 = input.LA(1);

                if ( (LA15_0==COMMA) ) {
                    alt15=1;
                }


                switch (alt15) {
            	case 1 :
            	    // BNGGrammar_ReactionRules.g:215:37: COMMA pattern
            	    {
            	    match(input,COMMA,FOLLOW_COMMA_in_include_command891); 
            	    pushFollow(FOLLOW_pattern_in_include_command893);
            	    pattern();

            	    state._fsp--;


            	    }
            	    break;

            	default :
            	    break loop15;
                }
            } while (true);

            match(input,RPAREN,FOLLOW_RPAREN_in_include_command897); 

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
    // BNGGrammar_ReactionRules.g:217:1: exclude_command : ( EXCLUDE_REACTANTS | EXCLUDE_PRODUCTS ) LPAREN INT COMMA pattern ( COMMA pattern )* RPAREN ;
    public final BNGGrammar_BNGGrammar_ReactionRules.exclude_command_return exclude_command() throws RecognitionException {
        BNGGrammar_BNGGrammar_ReactionRules.exclude_command_return retval = new BNGGrammar_BNGGrammar_ReactionRules.exclude_command_return();
        retval.start = input.LT(1);

        try {
            // BNGGrammar_ReactionRules.g:218:9: ( ( EXCLUDE_REACTANTS | EXCLUDE_PRODUCTS ) LPAREN INT COMMA pattern ( COMMA pattern )* RPAREN )
            // BNGGrammar_ReactionRules.g:218:11: ( EXCLUDE_REACTANTS | EXCLUDE_PRODUCTS ) LPAREN INT COMMA pattern ( COMMA pattern )* RPAREN
            {
            if ( (input.LA(1)>=EXCLUDE_REACTANTS && input.LA(1)<=EXCLUDE_PRODUCTS) ) {
                input.consume();
                state.errorRecovery=false;
            }
            else {
                MismatchedSetException mse = new MismatchedSetException(null,input);
                throw mse;
            }

            match(input,LPAREN,FOLLOW_LPAREN_in_exclude_command945); 
            match(input,INT,FOLLOW_INT_in_exclude_command947); 
            match(input,COMMA,FOLLOW_COMMA_in_exclude_command949); 
            pushFollow(FOLLOW_pattern_in_exclude_command951);
            pattern();

            state._fsp--;

            // BNGGrammar_ReactionRules.g:220:34: ( COMMA pattern )*
            loop16:
            do {
                int alt16=2;
                int LA16_0 = input.LA(1);

                if ( (LA16_0==COMMA) ) {
                    alt16=1;
                }


                switch (alt16) {
            	case 1 :
            	    // BNGGrammar_ReactionRules.g:220:35: COMMA pattern
            	    {
            	    match(input,COMMA,FOLLOW_COMMA_in_exclude_command954); 
            	    pushFollow(FOLLOW_pattern_in_exclude_command956);
            	    pattern();

            	    state._fsp--;


            	    }
            	    break;

            	default :
            	    break loop16;
                }
            } while (true);

            match(input,RPAREN,FOLLOW_RPAREN_in_exclude_command960); 

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
    // BNGGrammar_ReactionRules.g:225:1: pattern : ;
    public final BNGGrammar_BNGGrammar_ReactionRules.pattern_return pattern() throws RecognitionException {
        BNGGrammar_BNGGrammar_ReactionRules.pattern_return retval = new BNGGrammar_BNGGrammar_ReactionRules.pattern_return();
        retval.start = input.LT(1);

        try {
            // BNGGrammar_ReactionRules.g:225:9: ()
            // BNGGrammar_ReactionRules.g:225:17: 
            {
            }

            retval.stop = input.LT(-1);

        }
        finally {
        }
        return retval;
    }
    // $ANTLR end "pattern"

    // Delegated rules


 

    public static final BitSet FOLLOW_BEGIN_in_reaction_rules_block61 = new BitSet(new long[]{0x0000000000010000L});
    public static final BitSet FOLLOW_REACTION_in_reaction_rules_block63 = new BitSet(new long[]{0x0000000000040000L});
    public static final BitSet FOLLOW_RULES_in_reaction_rules_block65 = new BitSet(new long[]{0x0000000000000080L,0x000000000A021400L});
    public static final BitSet FOLLOW_reaction_rule_def_in_reaction_rules_block68 = new BitSet(new long[]{0x0000000000000080L,0x000000000A021400L});
    public static final BitSet FOLLOW_END_in_reaction_rules_block94 = new BitSet(new long[]{0x0000000000010000L});
    public static final BitSet FOLLOW_REACTION_in_reaction_rules_block96 = new BitSet(new long[]{0x0000000000040000L});
    public static final BitSet FOLLOW_RULES_in_reaction_rules_block98 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_STRING_in_reactionLabel126 = new BitSet(new long[]{0x0000000000000000L,0x0000000000005400L});
    public static final BitSet FOLLOW_INT_in_reactionLabel141 = new BitSet(new long[]{0x0000000000000000L,0x0000000000005400L});
    public static final BitSet FOLLOW_COLON_in_reactionLabel154 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_match_attribute_in_reaction_rule_def187 = new BitSet(new long[]{0x0000000000000000L,0x000000000A021400L});
    public static final BitSet FOLLOW_reactionLabel_in_reaction_rule_def215 = new BitSet(new long[]{0x0000000000000000L,0x000000000A021400L});
    public static final BitSet FOLLOW_reaction_def_in_reaction_rule_def254 = new BitSet(new long[]{0x0000000000000020L});
    public static final BitSet FOLLOW_WS_in_reaction_rule_def277 = new BitSet(new long[]{0x0000100000000000L,0x0000002000201600L});
    public static final BitSet FOLLOW_rate_list_in_reaction_rule_def289 = new BitSet(new long[]{0x0001E18000000002L});
    public static final BitSet FOLLOW_modif_command_in_reaction_rule_def303 = new BitSet(new long[]{0x0001E18000000002L});
    public static final BitSet FOLLOW_DELETEMOLECULES_in_reaction_rule_def308 = new BitSet(new long[]{0x0000010000000002L});
    public static final BitSet FOLLOW_MOVECONNECTED_in_reaction_rule_def313 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_LBRACKET_in_match_attribute456 = new BitSet(new long[]{0x0000004000000000L});
    public static final BitSet FOLLOW_MATCHONCE_in_match_attribute458 = new BitSet(new long[]{0x0000000000000000L,0x0000000000040000L});
    public static final BitSet FOLLOW_RBRACKET_in_match_attribute460 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_rule_species_def_in_reaction_def503 = new BitSet(new long[]{0x0000000000000000L,0x0000004001800000L});
    public static final BitSet FOLLOW_PLUS_in_reaction_def548 = new BitSet(new long[]{0x0000000000000000L,0x000000000A021400L});
    public static final BitSet FOLLOW_rule_species_def_in_reaction_def552 = new BitSet(new long[]{0x0000000000000000L,0x0000004001800000L});
    public static final BitSet FOLLOW_UNI_REACTION_SIGN_in_reaction_def581 = new BitSet(new long[]{0x0000000000000000L,0x000000000A021400L});
    public static final BitSet FOLLOW_BI_REACTION_SIGN_in_reaction_def586 = new BitSet(new long[]{0x0000000000000000L,0x000000000A021400L});
    public static final BitSet FOLLOW_rule_species_def_in_reaction_def597 = new BitSet(new long[]{0x0000000000000002L,0x0000004000000000L});
    public static final BitSet FOLLOW_PLUS_in_reaction_def622 = new BitSet(new long[]{0x0000000000000000L,0x000000000A021400L});
    public static final BitSet FOLLOW_rule_species_def_in_reaction_def626 = new BitSet(new long[]{0x0000000000000002L,0x0000004000000000L});
    public static final BitSet FOLLOW_INT_in_rule_species_def686 = new BitSet(new long[]{0x0000000000000000L,0x0000001000000000L});
    public static final BitSet FOLLOW_TIMES_in_rule_species_def690 = new BitSet(new long[]{0x0000000000000000L,0x000000000A001400L});
    public static final BitSet FOLLOW_species_def_in_rule_species_def700 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_INT_in_rule_species_def714 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_expression_in_rate_list778 = new BitSet(new long[]{0x0000000000000002L,0x0000000000080000L});
    public static final BitSet FOLLOW_COMMA_in_rate_list783 = new BitSet(new long[]{0x0000100000000000L,0x0000002000201600L});
    public static final BitSet FOLLOW_expression_in_rate_list787 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_include_command_in_modif_command816 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_exclude_command_in_modif_command828 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_set_in_include_command854 = new BitSet(new long[]{0x0000000000000000L,0x0000000000200000L});
    public static final BitSet FOLLOW_LPAREN_in_include_command882 = new BitSet(new long[]{0x0000000000000000L,0x0000000000000400L});
    public static final BitSet FOLLOW_INT_in_include_command884 = new BitSet(new long[]{0x0000000000000000L,0x0000000000080000L});
    public static final BitSet FOLLOW_COMMA_in_include_command886 = new BitSet(new long[]{0x0000000000000000L,0x0000000000480000L});
    public static final BitSet FOLLOW_pattern_in_include_command888 = new BitSet(new long[]{0x0000000000000000L,0x0000000000480000L});
    public static final BitSet FOLLOW_COMMA_in_include_command891 = new BitSet(new long[]{0x0000000000000000L,0x0000000000480000L});
    public static final BitSet FOLLOW_pattern_in_include_command893 = new BitSet(new long[]{0x0000000000000000L,0x0000000000480000L});
    public static final BitSet FOLLOW_RPAREN_in_include_command897 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_set_in_exclude_command921 = new BitSet(new long[]{0x0000000000000000L,0x0000000000200000L});
    public static final BitSet FOLLOW_LPAREN_in_exclude_command945 = new BitSet(new long[]{0x0000000000000000L,0x0000000000000400L});
    public static final BitSet FOLLOW_INT_in_exclude_command947 = new BitSet(new long[]{0x0000000000000000L,0x0000000000080000L});
    public static final BitSet FOLLOW_COMMA_in_exclude_command949 = new BitSet(new long[]{0x0000000000000000L,0x0000000000480000L});
    public static final BitSet FOLLOW_pattern_in_exclude_command951 = new BitSet(new long[]{0x0000000000000000L,0x0000000000480000L});
    public static final BitSet FOLLOW_COMMA_in_exclude_command954 = new BitSet(new long[]{0x0000000000000000L,0x0000000000480000L});
    public static final BitSet FOLLOW_pattern_in_exclude_command956 = new BitSet(new long[]{0x0000000000000000L,0x0000000000480000L});
    public static final BitSet FOLLOW_RPAREN_in_exclude_command960 = new BitSet(new long[]{0x0000000000000002L});

}