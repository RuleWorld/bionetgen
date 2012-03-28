// $ANTLR 3.3 Nov 30, 2010 12:50:56 BNGGrammar_Observables.g 2012-03-26 20:21:13

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

import org.antlr.stringtemplate.*;
import org.antlr.stringtemplate.language.*;
import java.util.HashMap;
public class BNGGrammar_BNGGrammar_Observables extends Parser {
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
    // BNGGrammar_Observables.g:15:1: observables_block[List observables] : BEGIN OBSERVABLES (s1= observable_def_line[\"O\" + $observables_block::observableCounter] )* END OBSERVABLES ;
    public final BNGGrammar_BNGGrammar_Observables.observables_block_return observables_block(List observables) throws RecognitionException {
        observables_block_stack.push(new observables_block_scope());
        BNGGrammar_BNGGrammar_Observables.observables_block_return retval = new BNGGrammar_BNGGrammar_Observables.observables_block_return();
        retval.start = input.LT(1);

        BNGGrammar_BNGGrammar_Observables.observable_def_line_return s1 = null;



        getParentTemplate();
          ((observables_block_scope)observables_block_stack.peek()).observableCounter = 1;
          ((observables_block_scope)observables_block_stack.peek()).reactionAction = new ReactionAction();

        try {
            // BNGGrammar_Observables.g:26:9: ( BEGIN OBSERVABLES (s1= observable_def_line[\"O\" + $observables_block::observableCounter] )* END OBSERVABLES )
            // BNGGrammar_Observables.g:26:11: BEGIN OBSERVABLES (s1= observable_def_line[\"O\" + $observables_block::observableCounter] )* END OBSERVABLES
            {
            match(input,BEGIN,FOLLOW_BEGIN_in_observables_block58); 
            match(input,OBSERVABLES,FOLLOW_OBSERVABLES_in_observables_block60); 
            // BNGGrammar_Observables.g:26:29: (s1= observable_def_line[\"O\" + $observables_block::observableCounter] )*
            loop1:
            do {
                int alt1=2;
                int LA1_0 = input.LA(1);

                if ( (LA1_0==SPECIES||LA1_0==MOLECULES||LA1_0==INT||LA1_0==STRING) ) {
                    alt1=1;
                }


                switch (alt1) {
            	case 1 :
            	    // BNGGrammar_Observables.g:26:30: s1= observable_def_line[\"O\" + $observables_block::observableCounter]
            	    {
            	    pushFollow(FOLLOW_observable_def_line_in_observables_block65);
            	    s1=observable_def_line("O" + ((observables_block_scope)observables_block_stack.peek()).observableCounter);

            	    state._fsp--;

            	    observables.add(s1.st);((observables_block_scope)observables_block_stack.peek()).observableCounter++;

            	    }
            	    break;

            	default :
            	    break loop1;
                }
            } while (true);

            match(input,END,FOLLOW_END_in_observables_block87); 
            match(input,OBSERVABLES,FOLLOW_OBSERVABLES_in_observables_block89); 

            }

            retval.stop = input.LT(-1);

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
    // BNGGrammar_Observables.g:29:1: observable_def_line[String upperID] : ({...}? INT | ) (s1= observable_type )? STRING ( pattern_list[upperID] ) -> observables_block(id=upperIDtype=$observable_type.textpatterns=$observable_def_line::patternname=$STRING.text);
    public final BNGGrammar_BNGGrammar_Observables.observable_def_line_return observable_def_line(String upperID) throws RecognitionException {
        observable_def_line_stack.push(new observable_def_line_scope());
        BNGGrammar_BNGGrammar_Observables.observable_def_line_return retval = new BNGGrammar_BNGGrammar_Observables.observable_def_line_return();
        retval.start = input.LT(1);

        Token STRING1=null;
        BNGGrammar_BNGGrammar_Observables.observable_type_return s1 = null;

        BNGGrammar_BNGGrammar_Observables.pattern_list_return pattern_list2 = null;



          ((observable_def_line_scope)observable_def_line_stack.peek()).pattern = new ArrayList<StringTemplate>();


        try {
            // BNGGrammar_Observables.g:38:9: ( ({...}? INT | ) (s1= observable_type )? STRING ( pattern_list[upperID] ) -> observables_block(id=upperIDtype=$observable_type.textpatterns=$observable_def_line::patternname=$STRING.text))
            // BNGGrammar_Observables.g:39:9: ({...}? INT | ) (s1= observable_type )? STRING ( pattern_list[upperID] )
            {
            // BNGGrammar_Observables.g:39:9: ({...}? INT | )
            int alt2=2;
            int LA2_0 = input.LA(1);

            if ( (LA2_0==INT) ) {
                alt2=1;
            }
            else if ( (LA2_0==SPECIES||LA2_0==MOLECULES||LA2_0==STRING) ) {
                alt2=2;
            }
            else {
                NoViableAltException nvae =
                    new NoViableAltException("", 2, 0, input);

                throw nvae;
            }
            switch (alt2) {
                case 1 :
                    // BNGGrammar_Observables.g:39:10: {...}? INT
                    {
                    if ( !((gParent.netGrammar)) ) {
                        throw new FailedPredicateException(input, "observable_def_line", "gParent.netGrammar");
                    }
                    match(input,INT,FOLLOW_INT_in_observable_def_line133); 

                    }
                    break;
                case 2 :
                    // BNGGrammar_Observables.g:39:38: 
                    {
                    }
                    break;

            }

            // BNGGrammar_Observables.g:40:9: (s1= observable_type )?
            int alt3=2;
            int LA3_0 = input.LA(1);

            if ( (LA3_0==SPECIES||LA3_0==MOLECULES) ) {
                alt3=1;
            }
            switch (alt3) {
                case 1 :
                    // BNGGrammar_Observables.g:40:10: s1= observable_type
                    {
                    pushFollow(FOLLOW_observable_type_in_observable_def_line150);
                    s1=observable_type();

                    state._fsp--;


                    }
                    break;

            }

            STRING1=(Token)match(input,STRING,FOLLOW_STRING_in_observable_def_line154); 
            gParent.memory.put((STRING1!=null?STRING1.getText():null),new Register(0.0,"observable"));
            // BNGGrammar_Observables.g:41:9: ( pattern_list[upperID] )
            // BNGGrammar_Observables.g:41:10: pattern_list[upperID]
            {
            pushFollow(FOLLOW_pattern_list_in_observable_def_line168);
            pattern_list2=pattern_list(upperID);

            state._fsp--;

             ((observable_def_line_scope)observable_def_line_stack.peek()).pattern.add((pattern_list2!=null?pattern_list2.st:null));

            }



            // TEMPLATE REWRITE
            // 42:9: -> observables_block(id=upperIDtype=$observable_type.textpatterns=$observable_def_line::patternname=$STRING.text)
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
    // BNGGrammar_Observables.g:44:1: observable_type : ( ( MOLECULES ) | ( SPECIES ) );
    public final BNGGrammar_BNGGrammar_Observables.observable_type_return observable_type() throws RecognitionException {
        BNGGrammar_BNGGrammar_Observables.observable_type_return retval = new BNGGrammar_BNGGrammar_Observables.observable_type_return();
        retval.start = input.LT(1);

        try {
            // BNGGrammar_Observables.g:45:9: ( ( MOLECULES ) | ( SPECIES ) )
            int alt4=2;
            int LA4_0 = input.LA(1);

            if ( (LA4_0==MOLECULES) ) {
                alt4=1;
            }
            else if ( (LA4_0==SPECIES) ) {
                alt4=2;
            }
            else {
                NoViableAltException nvae =
                    new NoViableAltException("", 4, 0, input);

                throw nvae;
            }
            switch (alt4) {
                case 1 :
                    // BNGGrammar_Observables.g:45:11: ( MOLECULES )
                    {
                    // BNGGrammar_Observables.g:45:11: ( MOLECULES )
                    // BNGGrammar_Observables.g:45:12: MOLECULES
                    {
                    match(input,MOLECULES,FOLLOW_MOLECULES_in_observable_type226); 

                    }


                    }
                    break;
                case 2 :
                    // BNGGrammar_Observables.g:45:25: ( SPECIES )
                    {
                    // BNGGrammar_Observables.g:45:25: ( SPECIES )
                    // BNGGrammar_Observables.g:45:26: SPECIES
                    {
                    match(input,SPECIES,FOLLOW_SPECIES_in_observable_type232); 

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
    }
    protected Stack pattern_list_stack = new Stack();

    public static class pattern_list_return extends ParserRuleReturnScope {
        public StringTemplate st;
        public Object getTemplate() { return st; }
        public String toString() { return st==null?null:st.toString(); }
    };

    // $ANTLR start "pattern_list"
    // BNGGrammar_Observables.g:48:1: pattern_list[String upperID] : (s1= obs_species_def[upperID + \"_P\" + $pattern_list::patternCounter, $observables_block::reactionAction] ( ( COMMA ) s2= obs_species_def[upperID + \"_P\" + $pattern_list::patternCounter, $observables_block::reactionAction] )* ) -> patterns_block(id=upperID + \"_P\" + $pattern_list::patternCountermolecules=$pattern_list::patterns);
    public final BNGGrammar_BNGGrammar_Observables.pattern_list_return pattern_list(String upperID) throws RecognitionException {
        pattern_list_stack.push(new pattern_list_scope());
        BNGGrammar_BNGGrammar_Observables.pattern_list_return retval = new BNGGrammar_BNGGrammar_Observables.pattern_list_return();
        retval.start = input.LT(1);

        BNGGrammar_BNGGrammar_Observables.obs_species_def_return s1 = null;

        BNGGrammar_BNGGrammar_Observables.obs_species_def_return s2 = null;



          ((pattern_list_scope)pattern_list_stack.peek()).patterns = new ArrayList();
          ((pattern_list_scope)pattern_list_stack.peek()).patternCounter = 1;
               

          // ((ChangeableChannelTokenStream)input).addChannel(0);

        try {
            // BNGGrammar_Observables.g:62:9: ( (s1= obs_species_def[upperID + \"_P\" + $pattern_list::patternCounter, $observables_block::reactionAction] ( ( COMMA ) s2= obs_species_def[upperID + \"_P\" + $pattern_list::patternCounter, $observables_block::reactionAction] )* ) -> patterns_block(id=upperID + \"_P\" + $pattern_list::patternCountermolecules=$pattern_list::patterns))
            // BNGGrammar_Observables.g:62:11: (s1= obs_species_def[upperID + \"_P\" + $pattern_list::patternCounter, $observables_block::reactionAction] ( ( COMMA ) s2= obs_species_def[upperID + \"_P\" + $pattern_list::patternCounter, $observables_block::reactionAction] )* )
            {
            // BNGGrammar_Observables.g:62:11: (s1= obs_species_def[upperID + \"_P\" + $pattern_list::patternCounter, $observables_block::reactionAction] ( ( COMMA ) s2= obs_species_def[upperID + \"_P\" + $pattern_list::patternCounter, $observables_block::reactionAction] )* )
            // BNGGrammar_Observables.g:62:12: s1= obs_species_def[upperID + \"_P\" + $pattern_list::patternCounter, $observables_block::reactionAction] ( ( COMMA ) s2= obs_species_def[upperID + \"_P\" + $pattern_list::patternCounter, $observables_block::reactionAction] )*
            {
            pushFollow(FOLLOW_obs_species_def_in_pattern_list279);
            s1=obs_species_def(upperID + "_P" + ((pattern_list_scope)pattern_list_stack.peek()).patternCounter, ((observables_block_scope)observables_block_stack.peek()).reactionAction);

            state._fsp--;


            	        ((pattern_list_scope)pattern_list_stack.peek()).patterns.add(s1.st);
            	        ((pattern_list_scope)pattern_list_stack.peek()).patternCounter++;
                    
            // BNGGrammar_Observables.g:67:9: ( ( COMMA ) s2= obs_species_def[upperID + \"_P\" + $pattern_list::patternCounter, $observables_block::reactionAction] )*
            loop5:
            do {
                int alt5=2;
                int LA5_0 = input.LA(1);

                if ( (LA5_0==COMMA) ) {
                    alt5=1;
                }


                switch (alt5) {
            	case 1 :
            	    // BNGGrammar_Observables.g:67:10: ( COMMA ) s2= obs_species_def[upperID + \"_P\" + $pattern_list::patternCounter, $observables_block::reactionAction]
            	    {
            	    // BNGGrammar_Observables.g:67:10: ( COMMA )
            	    // BNGGrammar_Observables.g:67:11: COMMA
            	    {
            	    match(input,COMMA,FOLLOW_COMMA_in_pattern_list303); 

            	    }

            	    pushFollow(FOLLOW_obs_species_def_in_pattern_list308);
            	    s2=obs_species_def(upperID + "_P" + ((pattern_list_scope)pattern_list_stack.peek()).patternCounter, ((observables_block_scope)observables_block_stack.peek()).reactionAction);

            	    state._fsp--;


            	    	        ((pattern_list_scope)pattern_list_stack.peek()).patterns.add(s2.st);
            	    	        ((pattern_list_scope)pattern_list_stack.peek()).patternCounter++; 
            	            

            	    }
            	    break;

            	default :
            	    break loop5;
                }
            } while (true);


            }



            // TEMPLATE REWRITE
            // 72:8: -> patterns_block(id=upperID + \"_P\" + $pattern_list::patternCountermolecules=$pattern_list::patterns)
            {
                retval.st = templateLib.getInstanceOf("patterns_block",
              new STAttrMap().put("id", upperID + "_P" + ((pattern_list_scope)pattern_list_stack.peek()).patternCounter).put("molecules", ((pattern_list_scope)pattern_list_stack.peek()).patterns));
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
        public StringTemplate st;
        public Object getTemplate() { return st; }
        public String toString() { return st==null?null:st.toString(); }
    };

    // $ANTLR start "obs_species_def"
    // BNGGrammar_Observables.g:76:2: obs_species_def[String upperID,ReactionAction reactionAction] returns [int stoichiometry,Map <String,List<ReactionRegister>> map] : s1= ( species_def[$obs_species_def::reactants,$obs_species_def::bonds,upperID] ) -> rule_seed_species_block(id=upperIDmolecules=$obs_species_def::reactantsfirstBonds=$obs_species_def::bonds.getLeft()secondBonds=$obs_species_def::bonds.getRight());
    public final BNGGrammar_BNGGrammar_Observables.obs_species_def_return obs_species_def(String upperID, ReactionAction reactionAction) throws RecognitionException {
        obs_species_def_stack.push(new obs_species_def_scope());
        BNGGrammar_BNGGrammar_Observables.obs_species_def_return retval = new BNGGrammar_BNGGrammar_Observables.obs_species_def_return();
        retval.start = input.LT(1);

        Token s1=null;
        BNGGrammar_BNGGrammar_SeedSpecies.species_def_return species_def3 = null;



          ((obs_species_def_scope)obs_species_def_stack.peek()).reactants = new ArrayList();
          ((obs_species_def_scope)obs_species_def_stack.peek()).bonds = new BondList();
          retval.stoichiometry = 1;

        try {
            // BNGGrammar_Observables.g:86:1: (s1= ( species_def[$obs_species_def::reactants,$obs_species_def::bonds,upperID] ) -> rule_seed_species_block(id=upperIDmolecules=$obs_species_def::reactantsfirstBonds=$obs_species_def::bonds.getLeft()secondBonds=$obs_species_def::bonds.getRight()))
            // BNGGrammar_Observables.g:88:2: s1= ( species_def[$obs_species_def::reactants,$obs_species_def::bonds,upperID] )
            {
            // BNGGrammar_Observables.g:88:5: ( species_def[$obs_species_def::reactants,$obs_species_def::bonds,upperID] )
            // BNGGrammar_Observables.g:88:6: species_def[$obs_species_def::reactants,$obs_species_def::bonds,upperID]
            {
            pushFollow(FOLLOW_species_def_in_obs_species_def380);
            species_def3=gBNGGrammar.species_def(((obs_species_def_scope)obs_species_def_stack.peek()).reactants, ((obs_species_def_scope)obs_species_def_stack.peek()).bonds, upperID);

            state._fsp--;


                   reactionAction.addMolecule(upperID,(species_def3!=null?input.toString(species_def3.start,species_def3.stop):null),((obs_species_def_scope)obs_species_def_stack.peek()).bonds);
                   retval.map = (species_def3!=null?species_def3.listOfMolecules:null);
              

            }



            // TEMPLATE REWRITE
            // 94:5: -> rule_seed_species_block(id=upperIDmolecules=$obs_species_def::reactantsfirstBonds=$obs_species_def::bonds.getLeft()secondBonds=$obs_species_def::bonds.getRight())
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

    // Delegated rules


 

    public static final BitSet FOLLOW_BEGIN_in_observables_block58 = new BitSet(new long[]{0x0000000000400000L});
    public static final BitSet FOLLOW_OBSERVABLES_in_observables_block60 = new BitSet(new long[]{0x0000000000002480L,0x000000000000A000L});
    public static final BitSet FOLLOW_observable_def_line_in_observables_block65 = new BitSet(new long[]{0x0000000000002480L,0x000000000000A000L});
    public static final BitSet FOLLOW_END_in_observables_block87 = new BitSet(new long[]{0x0000000000400000L});
    public static final BitSet FOLLOW_OBSERVABLES_in_observables_block89 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_INT_in_observable_def_line133 = new BitSet(new long[]{0x0000000000002400L,0x0000000000008000L});
    public static final BitSet FOLLOW_observable_type_in_observable_def_line150 = new BitSet(new long[]{0x0000000000000000L,0x0000000000008000L});
    public static final BitSet FOLLOW_STRING_in_observable_def_line154 = new BitSet(new long[]{0x0000000000000000L,0x0000000050008000L});
    public static final BitSet FOLLOW_pattern_list_in_observable_def_line168 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_MOLECULES_in_observable_type226 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_SPECIES_in_observable_type232 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_obs_species_def_in_pattern_list279 = new BitSet(new long[]{0x0000000000000002L,0x0000000000400000L});
    public static final BitSet FOLLOW_COMMA_in_pattern_list303 = new BitSet(new long[]{0x0000000000000000L,0x0000000050008000L});
    public static final BitSet FOLLOW_obs_species_def_in_pattern_list308 = new BitSet(new long[]{0x0000000000000002L,0x0000000000400000L});
    public static final BitSet FOLLOW_species_def_in_obs_species_def380 = new BitSet(new long[]{0x0000000000000002L});

}