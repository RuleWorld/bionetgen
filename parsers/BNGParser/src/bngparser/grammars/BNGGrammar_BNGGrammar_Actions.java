// $ANTLR 3.3 Nov 30, 2010 12:50:56 BNGGrammar_Actions.g 2012-03-26 20:21:14

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
public class BNGGrammar_BNGGrammar_Actions extends Parser {
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


        public BNGGrammar_BNGGrammar_Actions(TokenStream input, BNGGrammar gBNGGrammar) {
            this(input, new RecognizerSharedState(), gBNGGrammar);
        }
        public BNGGrammar_BNGGrammar_Actions(TokenStream input, RecognizerSharedState state, BNGGrammar gBNGGrammar) {
            super(input, state);
            this.gBNGGrammar = gBNGGrammar;
             
            gParent = gBNGGrammar;
        }
        
    protected StringTemplateGroup templateLib =
      new StringTemplateGroup("BNGGrammar_BNGGrammar_ActionsTemplates", AngleBracketTemplateLexer.class);

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
    public String getGrammarFileName() { return "BNGGrammar_Actions.g"; }


     public void getParentTemplate(){
     
      this.setTemplateLib(gParent.getTemplateLib());
     }
     List<StringTemplate> actions = new ArrayList<StringTemplate>();


    public static class actions_prog_return extends ParserRuleReturnScope {
        public StringTemplate st;
        public Object getTemplate() { return st; }
        public String toString() { return st==null?null:st.toString(); }
    };

    // $ANTLR start "actions_prog"
    // BNGGrammar_Actions.g:17:1: actions_prog : ( . )* ( actions_block | ( BEGIN ACTIONS actions_block END ACTIONS ) )? EOF -> actions(actions=actions);
    public final BNGGrammar_BNGGrammar_Actions.actions_prog_return actions_prog() throws RecognitionException {
        BNGGrammar_BNGGrammar_Actions.actions_prog_return retval = new BNGGrammar_BNGGrammar_Actions.actions_prog_return();
        retval.start = input.LT(1);

        try {
            // BNGGrammar_Actions.g:17:13: ( ( . )* ( actions_block | ( BEGIN ACTIONS actions_block END ACTIONS ) )? EOF -> actions(actions=actions))
            // BNGGrammar_Actions.g:18:1: ( . )* ( actions_block | ( BEGIN ACTIONS actions_block END ACTIONS ) )? EOF
            {
            // BNGGrammar_Actions.g:18:1: ( . )*
            loop1:
            do {
                int alt1=2;
                alt1 = dfa1.predict(input);
                switch (alt1) {
            	case 1 :
            	    // BNGGrammar_Actions.g:18:1: .
            	    {
            	    matchAny(input); 

            	    }
            	    break;

            	default :
            	    break loop1;
                }
            } while (true);

            // BNGGrammar_Actions.g:19:1: ( actions_block | ( BEGIN ACTIONS actions_block END ACTIONS ) )?
            int alt2=3;
            int LA2_0 = input.LA(1);

            if ( ((LA2_0>=SIMULATE_ODE && LA2_0<=READFILE)||(LA2_0>=WRITENET && LA2_0<=GENERATENETWORK)) ) {
                alt2=1;
            }
            else if ( (LA2_0==BEGIN) ) {
                alt2=2;
            }
            switch (alt2) {
                case 1 :
                    // BNGGrammar_Actions.g:19:2: actions_block
                    {
                    pushFollow(FOLLOW_actions_block_in_actions_prog44);
                    actions_block();

                    state._fsp--;


                    }
                    break;
                case 2 :
                    // BNGGrammar_Actions.g:19:19: ( BEGIN ACTIONS actions_block END ACTIONS )
                    {
                    // BNGGrammar_Actions.g:19:19: ( BEGIN ACTIONS actions_block END ACTIONS )
                    // BNGGrammar_Actions.g:19:20: BEGIN ACTIONS actions_block END ACTIONS
                    {
                    match(input,BEGIN,FOLLOW_BEGIN_in_actions_prog50); 
                    match(input,ACTIONS,FOLLOW_ACTIONS_in_actions_prog52); 
                    pushFollow(FOLLOW_actions_block_in_actions_prog54);
                    actions_block();

                    state._fsp--;

                    match(input,END,FOLLOW_END_in_actions_prog56); 
                    match(input,ACTIONS,FOLLOW_ACTIONS_in_actions_prog58); 

                    }


                    }
                    break;

            }

            match(input,EOF,FOLLOW_EOF_in_actions_prog64); 


            // TEMPLATE REWRITE
            // 20:5: -> actions(actions=actions)
            {
                retval.st = templateLib.getInstanceOf("actions",
              new STAttrMap().put("actions", actions));
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
    // $ANTLR end "actions_prog"

    public static class actions_block_return extends ParserRuleReturnScope {
        public StringTemplate st;
        public Object getTemplate() { return st; }
        public String toString() { return st==null?null:st.toString(); }
    };

    // $ANTLR start "actions_block"
    // BNGGrammar_Actions.g:22:1: actions_block : ( action )+ ;
    public final BNGGrammar_BNGGrammar_Actions.actions_block_return actions_block() throws RecognitionException {
        BNGGrammar_BNGGrammar_Actions.actions_block_return retval = new BNGGrammar_BNGGrammar_Actions.actions_block_return();
        retval.start = input.LT(1);


          getParentTemplate();
          gParent.paraphrases.push("in the actions block");

        try {
            // BNGGrammar_Actions.g:31:9: ( ( action )+ )
            // BNGGrammar_Actions.g:31:11: ( action )+
            {
            // BNGGrammar_Actions.g:31:11: ( action )+
            int cnt3=0;
            loop3:
            do {
                int alt3=2;
                int LA3_0 = input.LA(1);

                if ( ((LA3_0>=SIMULATE_ODE && LA3_0<=READFILE)||(LA3_0>=WRITENET && LA3_0<=GENERATENETWORK)) ) {
                    alt3=1;
                }


                switch (alt3) {
            	case 1 :
            	    // BNGGrammar_Actions.g:31:12: action
            	    {
            	    pushFollow(FOLLOW_action_in_actions_block101);
            	    action();

            	    state._fsp--;


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

            retval.stop = input.LT(-1);


              gParent.paraphrases.pop();

        }
        catch (RecognitionException re) {
            reportError(re);
            recover(input,re);
        }
        finally {
        }
        return retval;
    }
    // $ANTLR end "actions_block"

    public static class action_return extends ParserRuleReturnScope {
        public StringTemplate st;
        public Object getTemplate() { return st; }
        public String toString() { return st==null?null:st.toString(); }
    };

    // $ANTLR start "action"
    // BNGGrammar_Actions.g:33:1: action : ( generate_network | simulate_method | read_file | write_file | set_concentration | set_parameter | save_concentrations | reset_concentrations );
    public final BNGGrammar_BNGGrammar_Actions.action_return action() throws RecognitionException {
        BNGGrammar_BNGGrammar_Actions.action_return retval = new BNGGrammar_BNGGrammar_Actions.action_return();
        retval.start = input.LT(1);

        BNGGrammar_BNGGrammar_Actions.generate_network_return generate_network1 = null;

        BNGGrammar_BNGGrammar_Actions.simulate_method_return simulate_method2 = null;

        BNGGrammar_BNGGrammar_Actions.read_file_return read_file3 = null;

        BNGGrammar_BNGGrammar_Actions.write_file_return write_file4 = null;

        BNGGrammar_BNGGrammar_Actions.set_concentration_return set_concentration5 = null;

        BNGGrammar_BNGGrammar_Actions.set_parameter_return set_parameter6 = null;

        BNGGrammar_BNGGrammar_Actions.save_concentrations_return save_concentrations7 = null;

        BNGGrammar_BNGGrammar_Actions.reset_concentrations_return reset_concentrations8 = null;


        try {
            // BNGGrammar_Actions.g:33:9: ( generate_network | simulate_method | read_file | write_file | set_concentration | set_parameter | save_concentrations | reset_concentrations )
            int alt4=8;
            switch ( input.LA(1) ) {
            case GENERATENETWORK:
                {
                alt4=1;
                }
                break;
            case SIMULATE_ODE:
            case SIMULATE_SSA:
                {
                alt4=2;
                }
                break;
            case READFILE:
                {
                alt4=3;
                }
                break;
            case WRITENET:
            case WRITESBML:
            case WRITEMFILE:
            case WRITEXML:
                {
                alt4=4;
                }
                break;
            case SETCONCENTRATION:
                {
                alt4=5;
                }
                break;
            case SETPARAMETER:
                {
                alt4=6;
                }
                break;
            case SAVECONCENTRATIONS:
                {
                alt4=7;
                }
                break;
            case RESETCONCENTRATIONS:
                {
                alt4=8;
                }
                break;
            default:
                NoViableAltException nvae =
                    new NoViableAltException("", 4, 0, input);

                throw nvae;
            }

            switch (alt4) {
                case 1 :
                    // BNGGrammar_Actions.g:33:11: generate_network
                    {
                    pushFollow(FOLLOW_generate_network_in_action113);
                    generate_network1=generate_network();

                    state._fsp--;

                    actions.add((generate_network1!=null?generate_network1.st:null));

                    }
                    break;
                case 2 :
                    // BNGGrammar_Actions.g:34:11: simulate_method
                    {
                    pushFollow(FOLLOW_simulate_method_in_action127);
                    simulate_method2=simulate_method();

                    state._fsp--;

                    actions.add((simulate_method2!=null?simulate_method2.st:null));

                    }
                    break;
                case 3 :
                    // BNGGrammar_Actions.g:35:11: read_file
                    {
                    pushFollow(FOLLOW_read_file_in_action141);
                    read_file3=read_file();

                    state._fsp--;

                    actions.add((read_file3!=null?read_file3.st:null));

                    }
                    break;
                case 4 :
                    // BNGGrammar_Actions.g:36:11: write_file
                    {
                    pushFollow(FOLLOW_write_file_in_action155);
                    write_file4=write_file();

                    state._fsp--;

                    actions.add((write_file4!=null?write_file4.st:null));

                    }
                    break;
                case 5 :
                    // BNGGrammar_Actions.g:37:11: set_concentration
                    {
                    pushFollow(FOLLOW_set_concentration_in_action169);
                    set_concentration5=set_concentration();

                    state._fsp--;

                    actions.add((set_concentration5!=null?set_concentration5.st:null));

                    }
                    break;
                case 6 :
                    // BNGGrammar_Actions.g:38:11: set_parameter
                    {
                    pushFollow(FOLLOW_set_parameter_in_action183);
                    set_parameter6=set_parameter();

                    state._fsp--;

                    actions.add((set_parameter6!=null?set_parameter6.st:null));

                    }
                    break;
                case 7 :
                    // BNGGrammar_Actions.g:39:11: save_concentrations
                    {
                    pushFollow(FOLLOW_save_concentrations_in_action197);
                    save_concentrations7=save_concentrations();

                    state._fsp--;

                    actions.add((save_concentrations7!=null?save_concentrations7.st:null));

                    }
                    break;
                case 8 :
                    // BNGGrammar_Actions.g:40:11: reset_concentrations
                    {
                    pushFollow(FOLLOW_reset_concentrations_in_action211);
                    reset_concentrations8=reset_concentrations();

                    state._fsp--;

                    actions.add((reset_concentrations8!=null?reset_concentrations8.st:null));

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
    // $ANTLR end "action"

    protected static class generate_network_scope {
        Map<String,String> actions;
    }
    protected Stack generate_network_stack = new Stack();

    public static class generate_network_return extends ParserRuleReturnScope {
        public StringTemplate st;
        public Object getTemplate() { return st; }
        public String toString() { return st==null?null:st.toString(); }
    };

    // $ANTLR start "generate_network"
    // BNGGrammar_Actions.g:42:1: generate_network : GENERATENETWORK LPAREN ( LBRACKET ( ( gn_action_par_def[$generate_network::actions] ) ( COMMA gn_action_par_def[$generate_network::actions] )* )? RBRACKET )? RPAREN SEMI -> action(id=$GENERATENETWORK.textoptionMap=$generate_network::actions);
    public final BNGGrammar_BNGGrammar_Actions.generate_network_return generate_network() throws RecognitionException {
        generate_network_stack.push(new generate_network_scope());
        BNGGrammar_BNGGrammar_Actions.generate_network_return retval = new BNGGrammar_BNGGrammar_Actions.generate_network_return();
        retval.start = input.LT(1);

        Token GENERATENETWORK9=null;


          ((generate_network_scope)generate_network_stack.peek()).actions = new HashMap<String,String>();

        try {
            // BNGGrammar_Actions.g:49:9: ( GENERATENETWORK LPAREN ( LBRACKET ( ( gn_action_par_def[$generate_network::actions] ) ( COMMA gn_action_par_def[$generate_network::actions] )* )? RBRACKET )? RPAREN SEMI -> action(id=$GENERATENETWORK.textoptionMap=$generate_network::actions))
            // BNGGrammar_Actions.g:49:11: GENERATENETWORK LPAREN ( LBRACKET ( ( gn_action_par_def[$generate_network::actions] ) ( COMMA gn_action_par_def[$generate_network::actions] )* )? RBRACKET )? RPAREN SEMI
            {
            GENERATENETWORK9=(Token)match(input,GENERATENETWORK,FOLLOW_GENERATENETWORK_in_generate_network244); 
            match(input,LPAREN,FOLLOW_LPAREN_in_generate_network246); 
            // BNGGrammar_Actions.g:49:34: ( LBRACKET ( ( gn_action_par_def[$generate_network::actions] ) ( COMMA gn_action_par_def[$generate_network::actions] )* )? RBRACKET )?
            int alt7=2;
            int LA7_0 = input.LA(1);

            if ( (LA7_0==LBRACKET) ) {
                alt7=1;
            }
            switch (alt7) {
                case 1 :
                    // BNGGrammar_Actions.g:49:35: LBRACKET ( ( gn_action_par_def[$generate_network::actions] ) ( COMMA gn_action_par_def[$generate_network::actions] )* )? RBRACKET
                    {
                    match(input,LBRACKET,FOLLOW_LBRACKET_in_generate_network249); 
                    // BNGGrammar_Actions.g:50:9: ( ( gn_action_par_def[$generate_network::actions] ) ( COMMA gn_action_par_def[$generate_network::actions] )* )?
                    int alt6=2;
                    int LA6_0 = input.LA(1);

                    if ( ((LA6_0>=MAX_AGG && LA6_0<=PRINT_ITER)||(LA6_0>=PREFFIX && LA6_0<=SUFFIX)) ) {
                        alt6=1;
                    }
                    switch (alt6) {
                        case 1 :
                            // BNGGrammar_Actions.g:50:10: ( gn_action_par_def[$generate_network::actions] ) ( COMMA gn_action_par_def[$generate_network::actions] )*
                            {
                            // BNGGrammar_Actions.g:50:10: ( gn_action_par_def[$generate_network::actions] )
                            // BNGGrammar_Actions.g:50:11: gn_action_par_def[$generate_network::actions]
                            {
                            pushFollow(FOLLOW_gn_action_par_def_in_generate_network261);
                            gn_action_par_def(((generate_network_scope)generate_network_stack.peek()).actions);

                            state._fsp--;


                            }

                            // BNGGrammar_Actions.g:51:9: ( COMMA gn_action_par_def[$generate_network::actions] )*
                            loop5:
                            do {
                                int alt5=2;
                                int LA5_0 = input.LA(1);

                                if ( (LA5_0==COMMA) ) {
                                    alt5=1;
                                }


                                switch (alt5) {
                            	case 1 :
                            	    // BNGGrammar_Actions.g:51:10: COMMA gn_action_par_def[$generate_network::actions]
                            	    {
                            	    match(input,COMMA,FOLLOW_COMMA_in_generate_network275); 
                            	    pushFollow(FOLLOW_gn_action_par_def_in_generate_network277);
                            	    gn_action_par_def(((generate_network_scope)generate_network_stack.peek()).actions);

                            	    state._fsp--;


                            	    }
                            	    break;

                            	default :
                            	    break loop5;
                                }
                            } while (true);


                            }
                            break;

                    }

                    match(input,RBRACKET,FOLLOW_RBRACKET_in_generate_network292); 

                    }
                    break;

            }

            match(input,RPAREN,FOLLOW_RPAREN_in_generate_network296); 
            match(input,SEMI,FOLLOW_SEMI_in_generate_network298); 


            // TEMPLATE REWRITE
            // 52:32: -> action(id=$GENERATENETWORK.textoptionMap=$generate_network::actions)
            {
                retval.st = templateLib.getInstanceOf("action",
              new STAttrMap().put("id", (GENERATENETWORK9!=null?GENERATENETWORK9.getText():null)).put("optionMap", ((generate_network_scope)generate_network_stack.peek()).actions));
            }


            }

            retval.stop = input.LT(-1);

        }
        catch (RecognitionException re) {
            reportError(re);
            recover(input,re);
        }
        finally {
            generate_network_stack.pop();
        }
        return retval;
    }
    // $ANTLR end "generate_network"

    public static class gn_action_par_def_return extends ParserRuleReturnScope {
        public StringTemplate st;
        public Object getTemplate() { return st; }
        public String toString() { return st==null?null:st.toString(); }
    };

    // $ANTLR start "gn_action_par_def"
    // BNGGrammar_Actions.g:55:1: gn_action_par_def[Map<String,String> map] : ( ( MAX_AGG ASSIGNS i1= INT ) | ( MAX_ITER ASSIGNS i2= INT ) | ( MAX_STOICH ASSIGNS hash_value ) | ( OVERWRITE ASSIGNS i3= INT ) | ( PRINT_ITER ASSIGNS i4= INT ) | ps_par_def[map] );
    public final BNGGrammar_BNGGrammar_Actions.gn_action_par_def_return gn_action_par_def(Map<String,String> map) throws RecognitionException {
        BNGGrammar_BNGGrammar_Actions.gn_action_par_def_return retval = new BNGGrammar_BNGGrammar_Actions.gn_action_par_def_return();
        retval.start = input.LT(1);

        Token i1=null;
        Token i2=null;
        Token i3=null;
        Token i4=null;
        Token MAX_AGG10=null;
        Token MAX_ITER11=null;
        Token OVERWRITE12=null;
        Token PRINT_ITER13=null;

        try {
            // BNGGrammar_Actions.g:56:9: ( ( MAX_AGG ASSIGNS i1= INT ) | ( MAX_ITER ASSIGNS i2= INT ) | ( MAX_STOICH ASSIGNS hash_value ) | ( OVERWRITE ASSIGNS i3= INT ) | ( PRINT_ITER ASSIGNS i4= INT ) | ps_par_def[map] )
            int alt8=6;
            switch ( input.LA(1) ) {
            case MAX_AGG:
                {
                alt8=1;
                }
                break;
            case MAX_ITER:
                {
                alt8=2;
                }
                break;
            case MAX_STOICH:
                {
                alt8=3;
                }
                break;
            case OVERWRITE:
                {
                alt8=4;
                }
                break;
            case PRINT_ITER:
                {
                alt8=5;
                }
                break;
            case PREFFIX:
            case SUFFIX:
                {
                alt8=6;
                }
                break;
            default:
                NoViableAltException nvae =
                    new NoViableAltException("", 8, 0, input);

                throw nvae;
            }

            switch (alt8) {
                case 1 :
                    // BNGGrammar_Actions.g:56:11: ( MAX_AGG ASSIGNS i1= INT )
                    {
                    // BNGGrammar_Actions.g:56:11: ( MAX_AGG ASSIGNS i1= INT )
                    // BNGGrammar_Actions.g:56:12: MAX_AGG ASSIGNS i1= INT
                    {
                    MAX_AGG10=(Token)match(input,MAX_AGG,FOLLOW_MAX_AGG_in_gn_action_par_def346); 
                    match(input,ASSIGNS,FOLLOW_ASSIGNS_in_gn_action_par_def348); 
                    i1=(Token)match(input,INT,FOLLOW_INT_in_gn_action_par_def352); 
                    map.put((MAX_AGG10!=null?MAX_AGG10.getText():null),(i1!=null?i1.getText():null));

                    }


                    }
                    break;
                case 2 :
                    // BNGGrammar_Actions.g:57:11: ( MAX_ITER ASSIGNS i2= INT )
                    {
                    // BNGGrammar_Actions.g:57:11: ( MAX_ITER ASSIGNS i2= INT )
                    // BNGGrammar_Actions.g:57:12: MAX_ITER ASSIGNS i2= INT
                    {
                    MAX_ITER11=(Token)match(input,MAX_ITER,FOLLOW_MAX_ITER_in_gn_action_par_def368); 
                    match(input,ASSIGNS,FOLLOW_ASSIGNS_in_gn_action_par_def370); 
                    i2=(Token)match(input,INT,FOLLOW_INT_in_gn_action_par_def374); 
                    map.put((MAX_ITER11!=null?MAX_ITER11.getText():null),(i2!=null?i2.getText():null));

                    }


                    }
                    break;
                case 3 :
                    // BNGGrammar_Actions.g:58:11: ( MAX_STOICH ASSIGNS hash_value )
                    {
                    // BNGGrammar_Actions.g:58:11: ( MAX_STOICH ASSIGNS hash_value )
                    // BNGGrammar_Actions.g:58:12: MAX_STOICH ASSIGNS hash_value
                    {
                    match(input,MAX_STOICH,FOLLOW_MAX_STOICH_in_gn_action_par_def390); 
                    match(input,ASSIGNS,FOLLOW_ASSIGNS_in_gn_action_par_def392); 
                    pushFollow(FOLLOW_hash_value_in_gn_action_par_def394);
                    hash_value();

                    state._fsp--;


                    }


                    }
                    break;
                case 4 :
                    // BNGGrammar_Actions.g:59:11: ( OVERWRITE ASSIGNS i3= INT )
                    {
                    // BNGGrammar_Actions.g:59:11: ( OVERWRITE ASSIGNS i3= INT )
                    // BNGGrammar_Actions.g:59:12: OVERWRITE ASSIGNS i3= INT
                    {
                    OVERWRITE12=(Token)match(input,OVERWRITE,FOLLOW_OVERWRITE_in_gn_action_par_def408); 
                    match(input,ASSIGNS,FOLLOW_ASSIGNS_in_gn_action_par_def410); 
                    i3=(Token)match(input,INT,FOLLOW_INT_in_gn_action_par_def414); 
                    map.put((OVERWRITE12!=null?OVERWRITE12.getText():null),(i3!=null?i3.getText():null));

                    }


                    }
                    break;
                case 5 :
                    // BNGGrammar_Actions.g:60:11: ( PRINT_ITER ASSIGNS i4= INT )
                    {
                    // BNGGrammar_Actions.g:60:11: ( PRINT_ITER ASSIGNS i4= INT )
                    // BNGGrammar_Actions.g:60:12: PRINT_ITER ASSIGNS i4= INT
                    {
                    PRINT_ITER13=(Token)match(input,PRINT_ITER,FOLLOW_PRINT_ITER_in_gn_action_par_def430); 
                    match(input,ASSIGNS,FOLLOW_ASSIGNS_in_gn_action_par_def432); 
                    i4=(Token)match(input,INT,FOLLOW_INT_in_gn_action_par_def436); 
                    map.put((PRINT_ITER13!=null?PRINT_ITER13.getText():null),(i4!=null?i4.getText():null));

                    }


                    }
                    break;
                case 6 :
                    // BNGGrammar_Actions.g:61:11: ps_par_def[map]
                    {
                    pushFollow(FOLLOW_ps_par_def_in_gn_action_par_def451);
                    ps_par_def(map);

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
    // $ANTLR end "gn_action_par_def"

    protected static class simulate_method_scope {
        Map<String,String> actions;
        String method;
    }
    protected Stack simulate_method_stack = new Stack();

    public static class simulate_method_return extends ParserRuleReturnScope {
        public StringTemplate st;
        public Object getTemplate() { return st; }
        public String toString() { return st==null?null:st.toString(); }
    };

    // $ANTLR start "simulate_method"
    // BNGGrammar_Actions.g:64:1: simulate_method : ( simulate_ode[$simulate_method::actions] | simulate_ssa[$simulate_method::actions] ) -> action(id=$simulate_method::methodoptionMap=$simulate_method::actions);
    public final BNGGrammar_BNGGrammar_Actions.simulate_method_return simulate_method() throws RecognitionException {
        simulate_method_stack.push(new simulate_method_scope());
        BNGGrammar_BNGGrammar_Actions.simulate_method_return retval = new BNGGrammar_BNGGrammar_Actions.simulate_method_return();
        retval.start = input.LT(1);


          ((simulate_method_scope)simulate_method_stack.peek()).method = "";
          ((simulate_method_scope)simulate_method_stack.peek()).actions = new HashMap<String,String>();

        try {
            // BNGGrammar_Actions.g:74:9: ( ( simulate_ode[$simulate_method::actions] | simulate_ssa[$simulate_method::actions] ) -> action(id=$simulate_method::methodoptionMap=$simulate_method::actions))
            // BNGGrammar_Actions.g:74:11: ( simulate_ode[$simulate_method::actions] | simulate_ssa[$simulate_method::actions] )
            {
            // BNGGrammar_Actions.g:74:11: ( simulate_ode[$simulate_method::actions] | simulate_ssa[$simulate_method::actions] )
            int alt9=2;
            int LA9_0 = input.LA(1);

            if ( (LA9_0==SIMULATE_ODE) ) {
                alt9=1;
            }
            else if ( (LA9_0==SIMULATE_SSA) ) {
                alt9=2;
            }
            else {
                NoViableAltException nvae =
                    new NoViableAltException("", 9, 0, input);

                throw nvae;
            }
            switch (alt9) {
                case 1 :
                    // BNGGrammar_Actions.g:74:12: simulate_ode[$simulate_method::actions]
                    {
                    pushFollow(FOLLOW_simulate_ode_in_simulate_method494);
                    simulate_ode(((simulate_method_scope)simulate_method_stack.peek()).actions);

                    state._fsp--;

                    ((simulate_method_scope)simulate_method_stack.peek()).method = "simulate_ode";

                    }
                    break;
                case 2 :
                    // BNGGrammar_Actions.g:75:11: simulate_ssa[$simulate_method::actions]
                    {
                    pushFollow(FOLLOW_simulate_ssa_in_simulate_method509);
                    simulate_ssa(((simulate_method_scope)simulate_method_stack.peek()).actions);

                    state._fsp--;

                    ((simulate_method_scope)simulate_method_stack.peek()).method = "simulate_ssa";

                    }
                    break;

            }



            // TEMPLATE REWRITE
            // 76:9: -> action(id=$simulate_method::methodoptionMap=$simulate_method::actions)
            {
                retval.st = templateLib.getInstanceOf("action",
              new STAttrMap().put("id", ((simulate_method_scope)simulate_method_stack.peek()).method).put("optionMap", ((simulate_method_scope)simulate_method_stack.peek()).actions));
            }


            }

            retval.stop = input.LT(-1);

        }
        catch (RecognitionException re) {
            reportError(re);
            recover(input,re);
        }
        finally {
            simulate_method_stack.pop();
        }
        return retval;
    }
    // $ANTLR end "simulate_method"

    public static class simulate_ode_return extends ParserRuleReturnScope {
        public StringTemplate st;
        public Object getTemplate() { return st; }
        public String toString() { return st==null?null:st.toString(); }
    };

    // $ANTLR start "simulate_ode"
    // BNGGrammar_Actions.g:78:1: simulate_ode[Map<String,String> map] : SIMULATE_ODE LPAREN LBRACKET ( ( ps_par_def[map] | simulate_par_def[map] | simulate_ode_par_def[map] ) ( COMMA ( ps_par_def[map] | simulate_par_def[map] | simulate_ode_par_def[map] ) )* )? RBRACKET RPAREN SEMI ;
    public final BNGGrammar_BNGGrammar_Actions.simulate_ode_return simulate_ode(Map<String,String> map) throws RecognitionException {
        BNGGrammar_BNGGrammar_Actions.simulate_ode_return retval = new BNGGrammar_BNGGrammar_Actions.simulate_ode_return();
        retval.start = input.LT(1);

        try {
            // BNGGrammar_Actions.g:79:9: ( SIMULATE_ODE LPAREN LBRACKET ( ( ps_par_def[map] | simulate_par_def[map] | simulate_ode_par_def[map] ) ( COMMA ( ps_par_def[map] | simulate_par_def[map] | simulate_ode_par_def[map] ) )* )? RBRACKET RPAREN SEMI )
            // BNGGrammar_Actions.g:79:11: SIMULATE_ODE LPAREN LBRACKET ( ( ps_par_def[map] | simulate_par_def[map] | simulate_ode_par_def[map] ) ( COMMA ( ps_par_def[map] | simulate_par_def[map] | simulate_ode_par_def[map] ) )* )? RBRACKET RPAREN SEMI
            {
            match(input,SIMULATE_ODE,FOLLOW_SIMULATE_ODE_in_simulate_ode559); 
            match(input,LPAREN,FOLLOW_LPAREN_in_simulate_ode561); 
            match(input,LBRACKET,FOLLOW_LBRACKET_in_simulate_ode563); 
            // BNGGrammar_Actions.g:80:10: ( ( ps_par_def[map] | simulate_par_def[map] | simulate_ode_par_def[map] ) ( COMMA ( ps_par_def[map] | simulate_par_def[map] | simulate_ode_par_def[map] ) )* )?
            int alt13=2;
            int LA13_0 = input.LA(1);

            if ( ((LA13_0>=PREFFIX && LA13_0<=SUFFIX)||LA13_0==VERBOSE||(LA13_0>=ATOL && LA13_0<=SAMPLE_TIMES)) ) {
                alt13=1;
            }
            switch (alt13) {
                case 1 :
                    // BNGGrammar_Actions.g:80:11: ( ps_par_def[map] | simulate_par_def[map] | simulate_ode_par_def[map] ) ( COMMA ( ps_par_def[map] | simulate_par_def[map] | simulate_ode_par_def[map] ) )*
                    {
                    // BNGGrammar_Actions.g:80:11: ( ps_par_def[map] | simulate_par_def[map] | simulate_ode_par_def[map] )
                    int alt10=3;
                    switch ( input.LA(1) ) {
                    case PREFFIX:
                    case SUFFIX:
                        {
                        alt10=1;
                        }
                        break;
                    case VERBOSE:
                    case T_END:
                    case T_START:
                    case N_STEPS:
                    case SAMPLE_TIMES:
                        {
                        alt10=2;
                        }
                        break;
                    case ATOL:
                    case RTOL:
                    case STEADY_STATE:
                    case SPARSE:
                        {
                        alt10=3;
                        }
                        break;
                    default:
                        NoViableAltException nvae =
                            new NoViableAltException("", 10, 0, input);

                        throw nvae;
                    }

                    switch (alt10) {
                        case 1 :
                            // BNGGrammar_Actions.g:80:12: ps_par_def[map]
                            {
                            pushFollow(FOLLOW_ps_par_def_in_simulate_ode576);
                            ps_par_def(map);

                            state._fsp--;


                            }
                            break;
                        case 2 :
                            // BNGGrammar_Actions.g:80:28: simulate_par_def[map]
                            {
                            pushFollow(FOLLOW_simulate_par_def_in_simulate_ode579);
                            simulate_par_def(map);

                            state._fsp--;


                            }
                            break;
                        case 3 :
                            // BNGGrammar_Actions.g:80:50: simulate_ode_par_def[map]
                            {
                            pushFollow(FOLLOW_simulate_ode_par_def_in_simulate_ode582);
                            simulate_ode_par_def(map);

                            state._fsp--;


                            }
                            break;

                    }

                    // BNGGrammar_Actions.g:81:11: ( COMMA ( ps_par_def[map] | simulate_par_def[map] | simulate_ode_par_def[map] ) )*
                    loop12:
                    do {
                        int alt12=2;
                        int LA12_0 = input.LA(1);

                        if ( (LA12_0==COMMA) ) {
                            alt12=1;
                        }


                        switch (alt12) {
                    	case 1 :
                    	    // BNGGrammar_Actions.g:81:12: COMMA ( ps_par_def[map] | simulate_par_def[map] | simulate_ode_par_def[map] )
                    	    {
                    	    match(input,COMMA,FOLLOW_COMMA_in_simulate_ode597); 
                    	    // BNGGrammar_Actions.g:81:18: ( ps_par_def[map] | simulate_par_def[map] | simulate_ode_par_def[map] )
                    	    int alt11=3;
                    	    switch ( input.LA(1) ) {
                    	    case PREFFIX:
                    	    case SUFFIX:
                    	        {
                    	        alt11=1;
                    	        }
                    	        break;
                    	    case VERBOSE:
                    	    case T_END:
                    	    case T_START:
                    	    case N_STEPS:
                    	    case SAMPLE_TIMES:
                    	        {
                    	        alt11=2;
                    	        }
                    	        break;
                    	    case ATOL:
                    	    case RTOL:
                    	    case STEADY_STATE:
                    	    case SPARSE:
                    	        {
                    	        alt11=3;
                    	        }
                    	        break;
                    	    default:
                    	        NoViableAltException nvae =
                    	            new NoViableAltException("", 11, 0, input);

                    	        throw nvae;
                    	    }

                    	    switch (alt11) {
                    	        case 1 :
                    	            // BNGGrammar_Actions.g:81:19: ps_par_def[map]
                    	            {
                    	            pushFollow(FOLLOW_ps_par_def_in_simulate_ode600);
                    	            ps_par_def(map);

                    	            state._fsp--;


                    	            }
                    	            break;
                    	        case 2 :
                    	            // BNGGrammar_Actions.g:81:35: simulate_par_def[map]
                    	            {
                    	            pushFollow(FOLLOW_simulate_par_def_in_simulate_ode603);
                    	            simulate_par_def(map);

                    	            state._fsp--;


                    	            }
                    	            break;
                    	        case 3 :
                    	            // BNGGrammar_Actions.g:81:57: simulate_ode_par_def[map]
                    	            {
                    	            pushFollow(FOLLOW_simulate_ode_par_def_in_simulate_ode606);
                    	            simulate_ode_par_def(map);

                    	            state._fsp--;


                    	            }
                    	            break;

                    	    }


                    	    }
                    	    break;

                    	default :
                    	    break loop12;
                        }
                    } while (true);


                    }
                    break;

            }

            match(input,RBRACKET,FOLLOW_RBRACKET_in_simulate_ode625); 
            match(input,RPAREN,FOLLOW_RPAREN_in_simulate_ode627); 
            match(input,SEMI,FOLLOW_SEMI_in_simulate_ode629); 

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
    // $ANTLR end "simulate_ode"

    public static class simulate_ssa_return extends ParserRuleReturnScope {
        public StringTemplate st;
        public Object getTemplate() { return st; }
        public String toString() { return st==null?null:st.toString(); }
    };

    // $ANTLR start "simulate_ssa"
    // BNGGrammar_Actions.g:84:1: simulate_ssa[Map<String,String> map] : SIMULATE_SSA LPAREN LBRACKET ( ( ps_par_def[map] | simulate_par_def[map] ) ( COMMA ( ps_par_def[map] | simulate_par_def[map] ) )* )? RBRACKET RPAREN SEMI ;
    public final BNGGrammar_BNGGrammar_Actions.simulate_ssa_return simulate_ssa(Map<String,String> map) throws RecognitionException {
        BNGGrammar_BNGGrammar_Actions.simulate_ssa_return retval = new BNGGrammar_BNGGrammar_Actions.simulate_ssa_return();
        retval.start = input.LT(1);

        try {
            // BNGGrammar_Actions.g:85:9: ( SIMULATE_SSA LPAREN LBRACKET ( ( ps_par_def[map] | simulate_par_def[map] ) ( COMMA ( ps_par_def[map] | simulate_par_def[map] ) )* )? RBRACKET RPAREN SEMI )
            // BNGGrammar_Actions.g:85:11: SIMULATE_SSA LPAREN LBRACKET ( ( ps_par_def[map] | simulate_par_def[map] ) ( COMMA ( ps_par_def[map] | simulate_par_def[map] ) )* )? RBRACKET RPAREN SEMI
            {
            match(input,SIMULATE_SSA,FOLLOW_SIMULATE_SSA_in_simulate_ssa655); 
            match(input,LPAREN,FOLLOW_LPAREN_in_simulate_ssa657); 
            match(input,LBRACKET,FOLLOW_LBRACKET_in_simulate_ssa659); 
            // BNGGrammar_Actions.g:86:11: ( ( ps_par_def[map] | simulate_par_def[map] ) ( COMMA ( ps_par_def[map] | simulate_par_def[map] ) )* )?
            int alt17=2;
            int LA17_0 = input.LA(1);

            if ( ((LA17_0>=PREFFIX && LA17_0<=SUFFIX)||LA17_0==VERBOSE||(LA17_0>=T_END && LA17_0<=SAMPLE_TIMES)) ) {
                alt17=1;
            }
            switch (alt17) {
                case 1 :
                    // BNGGrammar_Actions.g:86:12: ( ps_par_def[map] | simulate_par_def[map] ) ( COMMA ( ps_par_def[map] | simulate_par_def[map] ) )*
                    {
                    // BNGGrammar_Actions.g:86:12: ( ps_par_def[map] | simulate_par_def[map] )
                    int alt14=2;
                    int LA14_0 = input.LA(1);

                    if ( ((LA14_0>=PREFFIX && LA14_0<=SUFFIX)) ) {
                        alt14=1;
                    }
                    else if ( (LA14_0==VERBOSE||(LA14_0>=T_END && LA14_0<=SAMPLE_TIMES)) ) {
                        alt14=2;
                    }
                    else {
                        NoViableAltException nvae =
                            new NoViableAltException("", 14, 0, input);

                        throw nvae;
                    }
                    switch (alt14) {
                        case 1 :
                            // BNGGrammar_Actions.g:86:13: ps_par_def[map]
                            {
                            pushFollow(FOLLOW_ps_par_def_in_simulate_ssa673);
                            ps_par_def(map);

                            state._fsp--;


                            }
                            break;
                        case 2 :
                            // BNGGrammar_Actions.g:86:29: simulate_par_def[map]
                            {
                            pushFollow(FOLLOW_simulate_par_def_in_simulate_ssa676);
                            simulate_par_def(map);

                            state._fsp--;


                            }
                            break;

                    }

                    // BNGGrammar_Actions.g:86:52: ( COMMA ( ps_par_def[map] | simulate_par_def[map] ) )*
                    loop16:
                    do {
                        int alt16=2;
                        int LA16_0 = input.LA(1);

                        if ( (LA16_0==COMMA) ) {
                            alt16=1;
                        }


                        switch (alt16) {
                    	case 1 :
                    	    // BNGGrammar_Actions.g:86:53: COMMA ( ps_par_def[map] | simulate_par_def[map] )
                    	    {
                    	    match(input,COMMA,FOLLOW_COMMA_in_simulate_ssa681); 
                    	    // BNGGrammar_Actions.g:86:59: ( ps_par_def[map] | simulate_par_def[map] )
                    	    int alt15=2;
                    	    int LA15_0 = input.LA(1);

                    	    if ( ((LA15_0>=PREFFIX && LA15_0<=SUFFIX)) ) {
                    	        alt15=1;
                    	    }
                    	    else if ( (LA15_0==VERBOSE||(LA15_0>=T_END && LA15_0<=SAMPLE_TIMES)) ) {
                    	        alt15=2;
                    	    }
                    	    else {
                    	        NoViableAltException nvae =
                    	            new NoViableAltException("", 15, 0, input);

                    	        throw nvae;
                    	    }
                    	    switch (alt15) {
                    	        case 1 :
                    	            // BNGGrammar_Actions.g:86:60: ps_par_def[map]
                    	            {
                    	            pushFollow(FOLLOW_ps_par_def_in_simulate_ssa684);
                    	            ps_par_def(map);

                    	            state._fsp--;


                    	            }
                    	            break;
                    	        case 2 :
                    	            // BNGGrammar_Actions.g:86:76: simulate_par_def[map]
                    	            {
                    	            pushFollow(FOLLOW_simulate_par_def_in_simulate_ssa687);
                    	            simulate_par_def(map);

                    	            state._fsp--;


                    	            }
                    	            break;

                    	    }


                    	    }
                    	    break;

                    	default :
                    	    break loop16;
                        }
                    } while (true);


                    }
                    break;

            }

            match(input,RBRACKET,FOLLOW_RBRACKET_in_simulate_ssa705); 
            match(input,RPAREN,FOLLOW_RPAREN_in_simulate_ssa707); 
            match(input,SEMI,FOLLOW_SEMI_in_simulate_ssa709); 

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
    // $ANTLR end "simulate_ssa"

    protected static class read_file_scope {
        Map<String,String> actions;
    }
    protected Stack read_file_stack = new Stack();

    public static class read_file_return extends ParserRuleReturnScope {
        public StringTemplate st;
        public Object getTemplate() { return st; }
        public String toString() { return st==null?null:st.toString(); }
    };

    // $ANTLR start "read_file"
    // BNGGrammar_Actions.g:88:1: read_file : READFILE LPAREN ( LBRACKET ( FILE ASSIGNS STRING )? RBRACKET )? RPAREN SEMI -> action(id=$READFILE.textoptionMap=$read_file::actions);
    public final BNGGrammar_BNGGrammar_Actions.read_file_return read_file() throws RecognitionException {
        read_file_stack.push(new read_file_scope());
        BNGGrammar_BNGGrammar_Actions.read_file_return retval = new BNGGrammar_BNGGrammar_Actions.read_file_return();
        retval.start = input.LT(1);

        Token FILE14=null;
        Token STRING15=null;
        Token READFILE16=null;


          ((read_file_scope)read_file_stack.peek()).actions = new HashMap<String,String>();

        try {
            // BNGGrammar_Actions.g:95:9: ( READFILE LPAREN ( LBRACKET ( FILE ASSIGNS STRING )? RBRACKET )? RPAREN SEMI -> action(id=$READFILE.textoptionMap=$read_file::actions))
            // BNGGrammar_Actions.g:95:11: READFILE LPAREN ( LBRACKET ( FILE ASSIGNS STRING )? RBRACKET )? RPAREN SEMI
            {
            READFILE16=(Token)match(input,READFILE,FOLLOW_READFILE_in_read_file732); 
            match(input,LPAREN,FOLLOW_LPAREN_in_read_file734); 
            // BNGGrammar_Actions.g:96:9: ( LBRACKET ( FILE ASSIGNS STRING )? RBRACKET )?
            int alt19=2;
            int LA19_0 = input.LA(1);

            if ( (LA19_0==LBRACKET) ) {
                alt19=1;
            }
            switch (alt19) {
                case 1 :
                    // BNGGrammar_Actions.g:96:10: LBRACKET ( FILE ASSIGNS STRING )? RBRACKET
                    {
                    match(input,LBRACKET,FOLLOW_LBRACKET_in_read_file746); 
                    // BNGGrammar_Actions.g:96:19: ( FILE ASSIGNS STRING )?
                    int alt18=2;
                    int LA18_0 = input.LA(1);

                    if ( (LA18_0==FILE) ) {
                        alt18=1;
                    }
                    switch (alt18) {
                        case 1 :
                            // BNGGrammar_Actions.g:96:20: FILE ASSIGNS STRING
                            {
                            FILE14=(Token)match(input,FILE,FOLLOW_FILE_in_read_file749); 
                            match(input,ASSIGNS,FOLLOW_ASSIGNS_in_read_file751); 
                            STRING15=(Token)match(input,STRING,FOLLOW_STRING_in_read_file753); 
                            ((read_file_scope)read_file_stack.peek()).actions.put((FILE14!=null?FILE14.getText():null),(STRING15!=null?STRING15.getText():null));

                            }
                            break;

                    }

                    match(input,RBRACKET,FOLLOW_RBRACKET_in_read_file759); 

                    }
                    break;

            }

            match(input,RPAREN,FOLLOW_RPAREN_in_read_file772); 
            match(input,SEMI,FOLLOW_SEMI_in_read_file774); 


            // TEMPLATE REWRITE
            // 97:21: -> action(id=$READFILE.textoptionMap=$read_file::actions)
            {
                retval.st = templateLib.getInstanceOf("action",
              new STAttrMap().put("id", (READFILE16!=null?READFILE16.getText():null)).put("optionMap", ((read_file_scope)read_file_stack.peek()).actions));
            }


            }

            retval.stop = input.LT(-1);

        }
        catch (RecognitionException re) {
            reportError(re);
            recover(input,re);
        }
        finally {
            read_file_stack.pop();
        }
        return retval;
    }
    // $ANTLR end "read_file"

    protected static class write_file_scope {
        Map<String,String> actions;
    }
    protected Stack write_file_stack = new Stack();

    public static class write_file_return extends ParserRuleReturnScope {
        public StringTemplate st;
        public Object getTemplate() { return st; }
        public String toString() { return st==null?null:st.toString(); }
    };

    // $ANTLR start "write_file"
    // BNGGrammar_Actions.g:99:1: write_file : write_type LPAREN ( LBRACKET RBRACKET )? RPAREN SEMI -> action(id=$write_type.text);
    public final BNGGrammar_BNGGrammar_Actions.write_file_return write_file() throws RecognitionException {
        write_file_stack.push(new write_file_scope());
        BNGGrammar_BNGGrammar_Actions.write_file_return retval = new BNGGrammar_BNGGrammar_Actions.write_file_return();
        retval.start = input.LT(1);

        BNGGrammar_BNGGrammar_Actions.write_type_return write_type17 = null;



          ((write_file_scope)write_file_stack.peek()).actions = new HashMap<String,String>();

        try {
            // BNGGrammar_Actions.g:106:9: ( write_type LPAREN ( LBRACKET RBRACKET )? RPAREN SEMI -> action(id=$write_type.text))
            // BNGGrammar_Actions.g:106:11: write_type LPAREN ( LBRACKET RBRACKET )? RPAREN SEMI
            {
            pushFollow(FOLLOW_write_type_in_write_file818);
            write_type17=write_type();

            state._fsp--;

            match(input,LPAREN,FOLLOW_LPAREN_in_write_file830); 
            // BNGGrammar_Actions.g:107:18: ( LBRACKET RBRACKET )?
            int alt20=2;
            int LA20_0 = input.LA(1);

            if ( (LA20_0==LBRACKET) ) {
                alt20=1;
            }
            switch (alt20) {
                case 1 :
                    // BNGGrammar_Actions.g:107:19: LBRACKET RBRACKET
                    {
                    match(input,LBRACKET,FOLLOW_LBRACKET_in_write_file833); 
                    match(input,RBRACKET,FOLLOW_RBRACKET_in_write_file835); 

                    }
                    break;

            }

            match(input,RPAREN,FOLLOW_RPAREN_in_write_file850); 
            match(input,SEMI,FOLLOW_SEMI_in_write_file852); 


            // TEMPLATE REWRITE
            // 108:24: -> action(id=$write_type.text)
            {
                retval.st = templateLib.getInstanceOf("action",
              new STAttrMap().put("id", (write_type17!=null?input.toString(write_type17.start,write_type17.stop):null)));
            }


            }

            retval.stop = input.LT(-1);

        }
        catch (RecognitionException re) {
            reportError(re);
            recover(input,re);
        }
        finally {
            write_file_stack.pop();
        }
        return retval;
    }
    // $ANTLR end "write_file"

    public static class write_type_return extends ParserRuleReturnScope {
        public StringTemplate st;
        public Object getTemplate() { return st; }
        public String toString() { return st==null?null:st.toString(); }
    };

    // $ANTLR start "write_type"
    // BNGGrammar_Actions.g:110:1: write_type : ( WRITENET | WRITESBML | WRITEMFILE | WRITEXML );
    public final BNGGrammar_BNGGrammar_Actions.write_type_return write_type() throws RecognitionException {
        BNGGrammar_BNGGrammar_Actions.write_type_return retval = new BNGGrammar_BNGGrammar_Actions.write_type_return();
        retval.start = input.LT(1);

        try {
            // BNGGrammar_Actions.g:111:9: ( WRITENET | WRITESBML | WRITEMFILE | WRITEXML )
            // BNGGrammar_Actions.g:
            {
            if ( (input.LA(1)>=WRITENET && input.LA(1)<=WRITEXML) ) {
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
    // $ANTLR end "write_type"

    protected static class set_concentration_scope {
        Map<String,String> options;
    }
    protected Stack set_concentration_stack = new Stack();

    public static class set_concentration_return extends ParserRuleReturnScope {
        public StringTemplate st;
        public Object getTemplate() { return st; }
        public String toString() { return st==null?null:st.toString(); }
    };

    // $ANTLR start "set_concentration"
    // BNGGrammar_Actions.g:116:1: set_concentration : SETCONCENTRATION LPAREN variable_definition RPAREN SEMI -> action(id=$SETCONCENTRATION.textoptionMap=$set_concentration::options);
    public final BNGGrammar_BNGGrammar_Actions.set_concentration_return set_concentration() throws RecognitionException {
        set_concentration_stack.push(new set_concentration_scope());
        BNGGrammar_BNGGrammar_Actions.set_concentration_return retval = new BNGGrammar_BNGGrammar_Actions.set_concentration_return();
        retval.start = input.LT(1);

        Token SETCONCENTRATION19=null;
        BNGGrammar_BNGGrammar_Actions.variable_definition_return variable_definition18 = null;



          ((set_concentration_scope)set_concentration_stack.peek()).options = new HashMap<String,String>();

        try {
            // BNGGrammar_Actions.g:123:9: ( SETCONCENTRATION LPAREN variable_definition RPAREN SEMI -> action(id=$SETCONCENTRATION.textoptionMap=$set_concentration::options))
            // BNGGrammar_Actions.g:123:11: SETCONCENTRATION LPAREN variable_definition RPAREN SEMI
            {
            SETCONCENTRATION19=(Token)match(input,SETCONCENTRATION,FOLLOW_SETCONCENTRATION_in_set_concentration953); 
            match(input,LPAREN,FOLLOW_LPAREN_in_set_concentration955); 
            pushFollow(FOLLOW_variable_definition_in_set_concentration957);
            variable_definition18=variable_definition();

            state._fsp--;

            ((set_concentration_scope)set_concentration_stack.peek()).options.put((variable_definition18!=null?variable_definition18.variableName:null),(variable_definition18!=null?variable_definition18.variableValue:null));
            match(input,RPAREN,FOLLOW_RPAREN_in_set_concentration978); 
            match(input,SEMI,FOLLOW_SEMI_in_set_concentration980); 


            // TEMPLATE REWRITE
            // 125:21: -> action(id=$SETCONCENTRATION.textoptionMap=$set_concentration::options)
            {
                retval.st = templateLib.getInstanceOf("action",
              new STAttrMap().put("id", (SETCONCENTRATION19!=null?SETCONCENTRATION19.getText():null)).put("optionMap", ((set_concentration_scope)set_concentration_stack.peek()).options));
            }


            }

            retval.stop = input.LT(-1);

        }
        catch (RecognitionException re) {
            reportError(re);
            recover(input,re);
        }
        finally {
            set_concentration_stack.pop();
        }
        return retval;
    }
    // $ANTLR end "set_concentration"

    protected static class set_parameter_scope {
        Map<String,String> options;
    }
    protected Stack set_parameter_stack = new Stack();

    public static class set_parameter_return extends ParserRuleReturnScope {
        public StringTemplate st;
        public Object getTemplate() { return st; }
        public String toString() { return st==null?null:st.toString(); }
    };

    // $ANTLR start "set_parameter"
    // BNGGrammar_Actions.g:127:1: set_parameter : SETPARAMETER LPAREN parameter_definition RPAREN SEMI -> action(id=$SETPARAMETER.textoptionMap=$set_parameter::options);
    public final BNGGrammar_BNGGrammar_Actions.set_parameter_return set_parameter() throws RecognitionException {
        set_parameter_stack.push(new set_parameter_scope());
        BNGGrammar_BNGGrammar_Actions.set_parameter_return retval = new BNGGrammar_BNGGrammar_Actions.set_parameter_return();
        retval.start = input.LT(1);

        Token SETPARAMETER21=null;
        BNGGrammar_BNGGrammar_Actions.parameter_definition_return parameter_definition20 = null;



          ((set_parameter_scope)set_parameter_stack.peek()).options = new HashMap<String,String>();

        try {
            // BNGGrammar_Actions.g:134:9: ( SETPARAMETER LPAREN parameter_definition RPAREN SEMI -> action(id=$SETPARAMETER.textoptionMap=$set_parameter::options))
            // BNGGrammar_Actions.g:134:12: SETPARAMETER LPAREN parameter_definition RPAREN SEMI
            {
            SETPARAMETER21=(Token)match(input,SETPARAMETER,FOLLOW_SETPARAMETER_in_set_parameter1026); 
            match(input,LPAREN,FOLLOW_LPAREN_in_set_parameter1028); 
            pushFollow(FOLLOW_parameter_definition_in_set_parameter1030);
            parameter_definition20=parameter_definition();

            state._fsp--;

            ((set_parameter_scope)set_parameter_stack.peek()).options.put((parameter_definition20!=null?parameter_definition20.parameterName:null),(parameter_definition20!=null?parameter_definition20.parameterValue:null));
            match(input,RPAREN,FOLLOW_RPAREN_in_set_parameter1053); 
            match(input,SEMI,FOLLOW_SEMI_in_set_parameter1055); 


            // TEMPLATE REWRITE
            // 136:21: -> action(id=$SETPARAMETER.textoptionMap=$set_parameter::options)
            {
                retval.st = templateLib.getInstanceOf("action",
              new STAttrMap().put("id", (SETPARAMETER21!=null?SETPARAMETER21.getText():null)).put("optionMap", ((set_parameter_scope)set_parameter_stack.peek()).options));
            }


            }

            retval.stop = input.LT(-1);

        }
        catch (RecognitionException re) {
            reportError(re);
            recover(input,re);
        }
        finally {
            set_parameter_stack.pop();
        }
        return retval;
    }
    // $ANTLR end "set_parameter"

    public static class parameter_definition_return extends ParserRuleReturnScope {
        public String parameterName;
        public String parameterValue;
        public StringTemplate st;
        public Object getTemplate() { return st; }
        public String toString() { return st==null?null:st.toString(); }
    };

    // $ANTLR start "parameter_definition"
    // BNGGrammar_Actions.g:139:1: parameter_definition returns [String parameterName,String parameterValue] : DBQUOTES STRING DBQUOTES COMMA (e1= expression[gParent.memory] | ( DBQUOTES e2= expression[gParent.memory] DBQUOTES ) ) ;
    public final BNGGrammar_BNGGrammar_Actions.parameter_definition_return parameter_definition() throws RecognitionException {
        BNGGrammar_BNGGrammar_Actions.parameter_definition_return retval = new BNGGrammar_BNGGrammar_Actions.parameter_definition_return();
        retval.start = input.LT(1);

        Token STRING22=null;
        BNGGrammar_BNGGrammar_Expression.expression_return e1 = null;

        BNGGrammar_BNGGrammar_Expression.expression_return e2 = null;


        try {
            // BNGGrammar_Actions.g:140:1: ( DBQUOTES STRING DBQUOTES COMMA (e1= expression[gParent.memory] | ( DBQUOTES e2= expression[gParent.memory] DBQUOTES ) ) )
            // BNGGrammar_Actions.g:141:1: DBQUOTES STRING DBQUOTES COMMA (e1= expression[gParent.memory] | ( DBQUOTES e2= expression[gParent.memory] DBQUOTES ) )
            {
            match(input,DBQUOTES,FOLLOW_DBQUOTES_in_parameter_definition1089); 
            STRING22=(Token)match(input,STRING,FOLLOW_STRING_in_parameter_definition1092); 
            retval.parameterName = (STRING22!=null?STRING22.getText():null);
            match(input,DBQUOTES,FOLLOW_DBQUOTES_in_parameter_definition1097); 
            match(input,COMMA,FOLLOW_COMMA_in_parameter_definition1099); 
            // BNGGrammar_Actions.g:144:1: (e1= expression[gParent.memory] | ( DBQUOTES e2= expression[gParent.memory] DBQUOTES ) )
            int alt21=2;
            int LA21_0 = input.LA(1);

            if ( (LA21_0==NOT||(LA21_0>=FLOAT && LA21_0<=INT)||LA21_0==STRING||LA21_0==LPAREN||LA21_0==MINUS) ) {
                alt21=1;
            }
            else if ( (LA21_0==DBQUOTES) ) {
                alt21=2;
            }
            else {
                NoViableAltException nvae =
                    new NoViableAltException("", 21, 0, input);

                throw nvae;
            }
            switch (alt21) {
                case 1 :
                    // BNGGrammar_Actions.g:144:2: e1= expression[gParent.memory]
                    {
                    pushFollow(FOLLOW_expression_in_parameter_definition1105);
                    e1=gBNGGrammar.expression(gParent.memory);

                    state._fsp--;

                    retval.parameterValue = (e1!=null?input.toString(e1.start,e1.stop):null); 

                    }
                    break;
                case 2 :
                    // BNGGrammar_Actions.g:145:1: ( DBQUOTES e2= expression[gParent.memory] DBQUOTES )
                    {
                    // BNGGrammar_Actions.g:145:1: ( DBQUOTES e2= expression[gParent.memory] DBQUOTES )
                    // BNGGrammar_Actions.g:145:2: DBQUOTES e2= expression[gParent.memory] DBQUOTES
                    {
                    match(input,DBQUOTES,FOLLOW_DBQUOTES_in_parameter_definition1113); 
                    pushFollow(FOLLOW_expression_in_parameter_definition1117);
                    e2=gBNGGrammar.expression(gParent.memory);

                    state._fsp--;

                    match(input,DBQUOTES,FOLLOW_DBQUOTES_in_parameter_definition1120); 
                    retval.parameterValue = (e2!=null?input.toString(e2.start,e2.stop):null); 

                    }


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
    // $ANTLR end "parameter_definition"

    protected static class variable_definition_scope {
        List reactants;
        BondList bonds;
    }
    protected Stack variable_definition_stack = new Stack();

    public static class variable_definition_return extends ParserRuleReturnScope {
        public String variableName;
        public String variableValue;
        public StringTemplate st;
        public Object getTemplate() { return st; }
        public String toString() { return st==null?null:st.toString(); }
    };

    // $ANTLR start "variable_definition"
    // BNGGrammar_Actions.g:148:1: variable_definition returns [String variableName, String variableValue] : DBQUOTES species_def[$variable_definition::reactants,$variable_definition::bonds,\"\"] DBQUOTES COMMA (e1= expression[gParent.memory] | ( DBQUOTES e2= expression[gParent.memory] DBQUOTES ) ) ;
    public final BNGGrammar_BNGGrammar_Actions.variable_definition_return variable_definition() throws RecognitionException {
        variable_definition_stack.push(new variable_definition_scope());
        BNGGrammar_BNGGrammar_Actions.variable_definition_return retval = new BNGGrammar_BNGGrammar_Actions.variable_definition_return();
        retval.start = input.LT(1);

        BNGGrammar_BNGGrammar_Expression.expression_return e1 = null;

        BNGGrammar_BNGGrammar_Expression.expression_return e2 = null;

        BNGGrammar_BNGGrammar_SeedSpecies.species_def_return species_def23 = null;



          ((variable_definition_scope)variable_definition_stack.peek()).reactants = new ArrayList();
          ((variable_definition_scope)variable_definition_stack.peek()).bonds = new BondList();

        try {
            // BNGGrammar_Actions.g:158:1: ( DBQUOTES species_def[$variable_definition::reactants,$variable_definition::bonds,\"\"] DBQUOTES COMMA (e1= expression[gParent.memory] | ( DBQUOTES e2= expression[gParent.memory] DBQUOTES ) ) )
            // BNGGrammar_Actions.g:159:1: DBQUOTES species_def[$variable_definition::reactants,$variable_definition::bonds,\"\"] DBQUOTES COMMA (e1= expression[gParent.memory] | ( DBQUOTES e2= expression[gParent.memory] DBQUOTES ) )
            {
            match(input,DBQUOTES,FOLLOW_DBQUOTES_in_variable_definition1152); 
            pushFollow(FOLLOW_species_def_in_variable_definition1155);
            species_def23=gBNGGrammar.species_def(((variable_definition_scope)variable_definition_stack.peek()).reactants, ((variable_definition_scope)variable_definition_stack.peek()).bonds, "");

            state._fsp--;

            retval.variableName = (species_def23!=null?input.toString(species_def23.start,species_def23.stop):null);
            match(input,DBQUOTES,FOLLOW_DBQUOTES_in_variable_definition1161); 
            match(input,COMMA,FOLLOW_COMMA_in_variable_definition1163); 
            // BNGGrammar_Actions.g:162:1: (e1= expression[gParent.memory] | ( DBQUOTES e2= expression[gParent.memory] DBQUOTES ) )
            int alt22=2;
            int LA22_0 = input.LA(1);

            if ( (LA22_0==NOT||(LA22_0>=FLOAT && LA22_0<=INT)||LA22_0==STRING||LA22_0==LPAREN||LA22_0==MINUS) ) {
                alt22=1;
            }
            else if ( (LA22_0==DBQUOTES) ) {
                alt22=2;
            }
            else {
                NoViableAltException nvae =
                    new NoViableAltException("", 22, 0, input);

                throw nvae;
            }
            switch (alt22) {
                case 1 :
                    // BNGGrammar_Actions.g:162:2: e1= expression[gParent.memory]
                    {
                    pushFollow(FOLLOW_expression_in_variable_definition1169);
                    e1=gBNGGrammar.expression(gParent.memory);

                    state._fsp--;

                    retval.variableValue = (e1!=null?input.toString(e1.start,e1.stop):null); 

                    }
                    break;
                case 2 :
                    // BNGGrammar_Actions.g:163:1: ( DBQUOTES e2= expression[gParent.memory] DBQUOTES )
                    {
                    // BNGGrammar_Actions.g:163:1: ( DBQUOTES e2= expression[gParent.memory] DBQUOTES )
                    // BNGGrammar_Actions.g:163:2: DBQUOTES e2= expression[gParent.memory] DBQUOTES
                    {
                    match(input,DBQUOTES,FOLLOW_DBQUOTES_in_variable_definition1177); 
                    pushFollow(FOLLOW_expression_in_variable_definition1181);
                    e2=gBNGGrammar.expression(gParent.memory);

                    state._fsp--;

                    match(input,DBQUOTES,FOLLOW_DBQUOTES_in_variable_definition1184); 
                    retval.variableValue = (e2!=null?input.toString(e2.start,e2.stop):null); 

                    }


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
            variable_definition_stack.pop();
        }
        return retval;
    }
    // $ANTLR end "variable_definition"

    public static class save_concentrations_return extends ParserRuleReturnScope {
        public StringTemplate st;
        public Object getTemplate() { return st; }
        public String toString() { return st==null?null:st.toString(); }
    };

    // $ANTLR start "save_concentrations"
    // BNGGrammar_Actions.g:169:1: save_concentrations : SAVECONCENTRATIONS LPAREN RPAREN SEMI -> action(id=$SAVECONCENTRATIONS.text);
    public final BNGGrammar_BNGGrammar_Actions.save_concentrations_return save_concentrations() throws RecognitionException {
        BNGGrammar_BNGGrammar_Actions.save_concentrations_return retval = new BNGGrammar_BNGGrammar_Actions.save_concentrations_return();
        retval.start = input.LT(1);

        Token SAVECONCENTRATIONS24=null;

        try {
            // BNGGrammar_Actions.g:170:9: ( SAVECONCENTRATIONS LPAREN RPAREN SEMI -> action(id=$SAVECONCENTRATIONS.text))
            // BNGGrammar_Actions.g:170:11: SAVECONCENTRATIONS LPAREN RPAREN SEMI
            {
            SAVECONCENTRATIONS24=(Token)match(input,SAVECONCENTRATIONS,FOLLOW_SAVECONCENTRATIONS_in_save_concentrations1241); 
            match(input,LPAREN,FOLLOW_LPAREN_in_save_concentrations1243); 
            match(input,RPAREN,FOLLOW_RPAREN_in_save_concentrations1245); 
            match(input,SEMI,FOLLOW_SEMI_in_save_concentrations1247); 


            // TEMPLATE REWRITE
            // 170:49: -> action(id=$SAVECONCENTRATIONS.text)
            {
                retval.st = templateLib.getInstanceOf("action",
              new STAttrMap().put("id", (SAVECONCENTRATIONS24!=null?SAVECONCENTRATIONS24.getText():null)));
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
    // $ANTLR end "save_concentrations"

    public static class reset_concentrations_return extends ParserRuleReturnScope {
        public StringTemplate st;
        public Object getTemplate() { return st; }
        public String toString() { return st==null?null:st.toString(); }
    };

    // $ANTLR start "reset_concentrations"
    // BNGGrammar_Actions.g:172:1: reset_concentrations : RESETCONCENTRATIONS LPAREN RPAREN SEMI -> action(id=$RESETCONCENTRATIONS.text);
    public final BNGGrammar_BNGGrammar_Actions.reset_concentrations_return reset_concentrations() throws RecognitionException {
        BNGGrammar_BNGGrammar_Actions.reset_concentrations_return retval = new BNGGrammar_BNGGrammar_Actions.reset_concentrations_return();
        retval.start = input.LT(1);

        Token RESETCONCENTRATIONS25=null;

        try {
            // BNGGrammar_Actions.g:173:9: ( RESETCONCENTRATIONS LPAREN RPAREN SEMI -> action(id=$RESETCONCENTRATIONS.text))
            // BNGGrammar_Actions.g:173:11: RESETCONCENTRATIONS LPAREN RPAREN SEMI
            {
            RESETCONCENTRATIONS25=(Token)match(input,RESETCONCENTRATIONS,FOLLOW_RESETCONCENTRATIONS_in_reset_concentrations1280); 
            match(input,LPAREN,FOLLOW_LPAREN_in_reset_concentrations1282); 
            match(input,RPAREN,FOLLOW_RPAREN_in_reset_concentrations1284); 
            match(input,SEMI,FOLLOW_SEMI_in_reset_concentrations1286); 


            // TEMPLATE REWRITE
            // 173:50: -> action(id=$RESETCONCENTRATIONS.text)
            {
                retval.st = templateLib.getInstanceOf("action",
              new STAttrMap().put("id", (RESETCONCENTRATIONS25!=null?RESETCONCENTRATIONS25.getText():null)));
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
    // $ANTLR end "reset_concentrations"

    public static class hash_value_return extends ParserRuleReturnScope {
        public StringTemplate st;
        public Object getTemplate() { return st; }
        public String toString() { return st==null?null:st.toString(); }
    };

    // $ANTLR start "hash_value"
    // BNGGrammar_Actions.g:175:1: hash_value : STRING ASSIGNS LBRACKET assignment_list RBRACKET ;
    public final BNGGrammar_BNGGrammar_Actions.hash_value_return hash_value() throws RecognitionException {
        BNGGrammar_BNGGrammar_Actions.hash_value_return retval = new BNGGrammar_BNGGrammar_Actions.hash_value_return();
        retval.start = input.LT(1);

        try {
            // BNGGrammar_Actions.g:176:9: ( STRING ASSIGNS LBRACKET assignment_list RBRACKET )
            // BNGGrammar_Actions.g:176:11: STRING ASSIGNS LBRACKET assignment_list RBRACKET
            {
            match(input,STRING,FOLLOW_STRING_in_hash_value1319); 
            match(input,ASSIGNS,FOLLOW_ASSIGNS_in_hash_value1321); 
            match(input,LBRACKET,FOLLOW_LBRACKET_in_hash_value1323); 
            pushFollow(FOLLOW_assignment_list_in_hash_value1325);
            assignment_list();

            state._fsp--;

            match(input,RBRACKET,FOLLOW_RBRACKET_in_hash_value1327); 

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
    // $ANTLR end "hash_value"

    public static class assignment_list_return extends ParserRuleReturnScope {
        public StringTemplate st;
        public Object getTemplate() { return st; }
        public String toString() { return st==null?null:st.toString(); }
    };

    // $ANTLR start "assignment_list"
    // BNGGrammar_Actions.g:178:1: assignment_list : STRING ASSIGNS value ( COMMA STRING ASSIGNS value ) ;
    public final BNGGrammar_BNGGrammar_Actions.assignment_list_return assignment_list() throws RecognitionException {
        BNGGrammar_BNGGrammar_Actions.assignment_list_return retval = new BNGGrammar_BNGGrammar_Actions.assignment_list_return();
        retval.start = input.LT(1);

        try {
            // BNGGrammar_Actions.g:179:9: ( STRING ASSIGNS value ( COMMA STRING ASSIGNS value ) )
            // BNGGrammar_Actions.g:179:11: STRING ASSIGNS value ( COMMA STRING ASSIGNS value )
            {
            match(input,STRING,FOLLOW_STRING_in_assignment_list1351); 
            match(input,ASSIGNS,FOLLOW_ASSIGNS_in_assignment_list1353); 
            pushFollow(FOLLOW_value_in_assignment_list1355);
            value();

            state._fsp--;

            // BNGGrammar_Actions.g:179:32: ( COMMA STRING ASSIGNS value )
            // BNGGrammar_Actions.g:179:33: COMMA STRING ASSIGNS value
            {
            match(input,COMMA,FOLLOW_COMMA_in_assignment_list1358); 
            match(input,STRING,FOLLOW_STRING_in_assignment_list1360); 
            match(input,ASSIGNS,FOLLOW_ASSIGNS_in_assignment_list1362); 
            pushFollow(FOLLOW_value_in_assignment_list1364);
            value();

            state._fsp--;


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
    // $ANTLR end "assignment_list"

    public static class value_return extends ParserRuleReturnScope {
        public StringTemplate st;
        public Object getTemplate() { return st; }
        public String toString() { return st==null?null:st.toString(); }
    };

    // $ANTLR start "value"
    // BNGGrammar_Actions.g:181:1: value : ( INT | FLOAT | STRING );
    public final BNGGrammar_BNGGrammar_Actions.value_return value() throws RecognitionException {
        BNGGrammar_BNGGrammar_Actions.value_return retval = new BNGGrammar_BNGGrammar_Actions.value_return();
        retval.start = input.LT(1);

        try {
            // BNGGrammar_Actions.g:181:9: ( INT | FLOAT | STRING )
            // BNGGrammar_Actions.g:
            {
            if ( (input.LA(1)>=FLOAT && input.LA(1)<=INT)||input.LA(1)==STRING ) {
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
    // $ANTLR end "value"

    public static class ps_par_def_return extends ParserRuleReturnScope {
        public StringTemplate st;
        public Object getTemplate() { return st; }
        public String toString() { return st==null?null:st.toString(); }
    };

    // $ANTLR start "ps_par_def"
    // BNGGrammar_Actions.g:183:1: ps_par_def[Map<String,String> map] : ( PREFFIX ASSIGNS ( ( DBQUOTES STRING DBQUOTES ) | STRING ) | SUFFIX ASSIGNS ( ( DBQUOTES STRING DBQUOTES ) | STRING ) );
    public final BNGGrammar_BNGGrammar_Actions.ps_par_def_return ps_par_def(Map<String,String> map) throws RecognitionException {
        BNGGrammar_BNGGrammar_Actions.ps_par_def_return retval = new BNGGrammar_BNGGrammar_Actions.ps_par_def_return();
        retval.start = input.LT(1);

        try {
            // BNGGrammar_Actions.g:184:9: ( PREFFIX ASSIGNS ( ( DBQUOTES STRING DBQUOTES ) | STRING ) | SUFFIX ASSIGNS ( ( DBQUOTES STRING DBQUOTES ) | STRING ) )
            int alt25=2;
            int LA25_0 = input.LA(1);

            if ( (LA25_0==PREFFIX) ) {
                alt25=1;
            }
            else if ( (LA25_0==SUFFIX) ) {
                alt25=2;
            }
            else {
                NoViableAltException nvae =
                    new NoViableAltException("", 25, 0, input);

                throw nvae;
            }
            switch (alt25) {
                case 1 :
                    // BNGGrammar_Actions.g:184:11: PREFFIX ASSIGNS ( ( DBQUOTES STRING DBQUOTES ) | STRING )
                    {
                    match(input,PREFFIX,FOLLOW_PREFFIX_in_ps_par_def1416); 
                    match(input,ASSIGNS,FOLLOW_ASSIGNS_in_ps_par_def1418); 
                    // BNGGrammar_Actions.g:184:27: ( ( DBQUOTES STRING DBQUOTES ) | STRING )
                    int alt23=2;
                    int LA23_0 = input.LA(1);

                    if ( (LA23_0==DBQUOTES) ) {
                        alt23=1;
                    }
                    else if ( (LA23_0==STRING) ) {
                        alt23=2;
                    }
                    else {
                        NoViableAltException nvae =
                            new NoViableAltException("", 23, 0, input);

                        throw nvae;
                    }
                    switch (alt23) {
                        case 1 :
                            // BNGGrammar_Actions.g:184:28: ( DBQUOTES STRING DBQUOTES )
                            {
                            // BNGGrammar_Actions.g:184:28: ( DBQUOTES STRING DBQUOTES )
                            // BNGGrammar_Actions.g:184:29: DBQUOTES STRING DBQUOTES
                            {
                            match(input,DBQUOTES,FOLLOW_DBQUOTES_in_ps_par_def1422); 
                            match(input,STRING,FOLLOW_STRING_in_ps_par_def1425); 
                            match(input,DBQUOTES,FOLLOW_DBQUOTES_in_ps_par_def1427); 

                            }


                            }
                            break;
                        case 2 :
                            // BNGGrammar_Actions.g:184:58: STRING
                            {
                            match(input,STRING,FOLLOW_STRING_in_ps_par_def1432); 

                            }
                            break;

                    }


                    }
                    break;
                case 2 :
                    // BNGGrammar_Actions.g:185:11: SUFFIX ASSIGNS ( ( DBQUOTES STRING DBQUOTES ) | STRING )
                    {
                    match(input,SUFFIX,FOLLOW_SUFFIX_in_ps_par_def1445); 
                    match(input,ASSIGNS,FOLLOW_ASSIGNS_in_ps_par_def1447); 
                    // BNGGrammar_Actions.g:185:26: ( ( DBQUOTES STRING DBQUOTES ) | STRING )
                    int alt24=2;
                    int LA24_0 = input.LA(1);

                    if ( (LA24_0==DBQUOTES) ) {
                        alt24=1;
                    }
                    else if ( (LA24_0==STRING) ) {
                        alt24=2;
                    }
                    else {
                        NoViableAltException nvae =
                            new NoViableAltException("", 24, 0, input);

                        throw nvae;
                    }
                    switch (alt24) {
                        case 1 :
                            // BNGGrammar_Actions.g:185:27: ( DBQUOTES STRING DBQUOTES )
                            {
                            // BNGGrammar_Actions.g:185:27: ( DBQUOTES STRING DBQUOTES )
                            // BNGGrammar_Actions.g:185:28: DBQUOTES STRING DBQUOTES
                            {
                            match(input,DBQUOTES,FOLLOW_DBQUOTES_in_ps_par_def1451); 
                            match(input,STRING,FOLLOW_STRING_in_ps_par_def1454); 
                            match(input,DBQUOTES,FOLLOW_DBQUOTES_in_ps_par_def1456); 

                            }


                            }
                            break;
                        case 2 :
                            // BNGGrammar_Actions.g:185:57: STRING
                            {
                            match(input,STRING,FOLLOW_STRING_in_ps_par_def1461); 

                            }
                            break;

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
    // $ANTLR end "ps_par_def"

    public static class simulate_ode_par_def_return extends ParserRuleReturnScope {
        public StringTemplate st;
        public Object getTemplate() { return st; }
        public String toString() { return st==null?null:st.toString(); }
    };

    // $ANTLR start "simulate_ode_par_def"
    // BNGGrammar_Actions.g:187:1: simulate_ode_par_def[Map<String,String> map] : ( ATOL ASSIGNS f1= FLOAT | RTOL ASSIGNS f2= FLOAT | STEADY_STATE ASSIGNS i1= INT | SPARSE ASSIGNS i2= INT );
    public final BNGGrammar_BNGGrammar_Actions.simulate_ode_par_def_return simulate_ode_par_def(Map<String,String> map) throws RecognitionException {
        BNGGrammar_BNGGrammar_Actions.simulate_ode_par_def_return retval = new BNGGrammar_BNGGrammar_Actions.simulate_ode_par_def_return();
        retval.start = input.LT(1);

        Token f1=null;
        Token f2=null;
        Token i1=null;
        Token i2=null;
        Token ATOL26=null;
        Token RTOL27=null;
        Token STEADY_STATE28=null;
        Token SPARSE29=null;

        try {
            // BNGGrammar_Actions.g:188:9: ( ATOL ASSIGNS f1= FLOAT | RTOL ASSIGNS f2= FLOAT | STEADY_STATE ASSIGNS i1= INT | SPARSE ASSIGNS i2= INT )
            int alt26=4;
            switch ( input.LA(1) ) {
            case ATOL:
                {
                alt26=1;
                }
                break;
            case RTOL:
                {
                alt26=2;
                }
                break;
            case STEADY_STATE:
                {
                alt26=3;
                }
                break;
            case SPARSE:
                {
                alt26=4;
                }
                break;
            default:
                NoViableAltException nvae =
                    new NoViableAltException("", 26, 0, input);

                throw nvae;
            }

            switch (alt26) {
                case 1 :
                    // BNGGrammar_Actions.g:188:11: ATOL ASSIGNS f1= FLOAT
                    {
                    ATOL26=(Token)match(input,ATOL,FOLLOW_ATOL_in_simulate_ode_par_def1487); 
                    match(input,ASSIGNS,FOLLOW_ASSIGNS_in_simulate_ode_par_def1489); 
                    f1=(Token)match(input,FLOAT,FOLLOW_FLOAT_in_simulate_ode_par_def1493); 
                    map.put((ATOL26!=null?ATOL26.getText():null),(f1!=null?f1.getText():null));

                    }
                    break;
                case 2 :
                    // BNGGrammar_Actions.g:189:11: RTOL ASSIGNS f2= FLOAT
                    {
                    RTOL27=(Token)match(input,RTOL,FOLLOW_RTOL_in_simulate_ode_par_def1507); 
                    match(input,ASSIGNS,FOLLOW_ASSIGNS_in_simulate_ode_par_def1509); 
                    f2=(Token)match(input,FLOAT,FOLLOW_FLOAT_in_simulate_ode_par_def1513); 
                    map.put((RTOL27!=null?RTOL27.getText():null),(f2!=null?f2.getText():null));

                    }
                    break;
                case 3 :
                    // BNGGrammar_Actions.g:190:11: STEADY_STATE ASSIGNS i1= INT
                    {
                    STEADY_STATE28=(Token)match(input,STEADY_STATE,FOLLOW_STEADY_STATE_in_simulate_ode_par_def1527); 
                    match(input,ASSIGNS,FOLLOW_ASSIGNS_in_simulate_ode_par_def1529); 
                    i1=(Token)match(input,INT,FOLLOW_INT_in_simulate_ode_par_def1533); 
                    map.put((STEADY_STATE28!=null?STEADY_STATE28.getText():null),(i1!=null?i1.getText():null));

                    }
                    break;
                case 4 :
                    // BNGGrammar_Actions.g:191:11: SPARSE ASSIGNS i2= INT
                    {
                    SPARSE29=(Token)match(input,SPARSE,FOLLOW_SPARSE_in_simulate_ode_par_def1547); 
                    match(input,ASSIGNS,FOLLOW_ASSIGNS_in_simulate_ode_par_def1549); 
                    i2=(Token)match(input,INT,FOLLOW_INT_in_simulate_ode_par_def1553); 
                    map.put((SPARSE29!=null?SPARSE29.getText():null),(i2!=null?i2.getText():null));

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
    // $ANTLR end "simulate_ode_par_def"

    public static class simulate_par_def_return extends ParserRuleReturnScope {
        public StringTemplate st;
        public Object getTemplate() { return st; }
        public String toString() { return st==null?null:st.toString(); }
    };

    // $ANTLR start "simulate_par_def"
    // BNGGrammar_Actions.g:193:1: simulate_par_def[Map<String,String> map] : ( T_END ASSIGNS (i1= INT | f1= FLOAT ) | T_START ASSIGNS (i2= INT | f2= FLOAT ) | N_STEPS ASSIGNS i3= INT | SAMPLE_TIMES ASSIGNS i4= array_value | VERBOSE ASSIGNS i5= INT );
    public final BNGGrammar_BNGGrammar_Actions.simulate_par_def_return simulate_par_def(Map<String,String> map) throws RecognitionException {
        BNGGrammar_BNGGrammar_Actions.simulate_par_def_return retval = new BNGGrammar_BNGGrammar_Actions.simulate_par_def_return();
        retval.start = input.LT(1);

        Token i1=null;
        Token f1=null;
        Token i2=null;
        Token f2=null;
        Token i3=null;
        Token i5=null;
        Token T_END30=null;
        Token T_START31=null;
        Token N_STEPS32=null;
        Token VERBOSE33=null;
        BNGGrammar_BNGGrammar_Actions.array_value_return i4 = null;


        try {
            // BNGGrammar_Actions.g:194:9: ( T_END ASSIGNS (i1= INT | f1= FLOAT ) | T_START ASSIGNS (i2= INT | f2= FLOAT ) | N_STEPS ASSIGNS i3= INT | SAMPLE_TIMES ASSIGNS i4= array_value | VERBOSE ASSIGNS i5= INT )
            int alt29=5;
            switch ( input.LA(1) ) {
            case T_END:
                {
                alt29=1;
                }
                break;
            case T_START:
                {
                alt29=2;
                }
                break;
            case N_STEPS:
                {
                alt29=3;
                }
                break;
            case SAMPLE_TIMES:
                {
                alt29=4;
                }
                break;
            case VERBOSE:
                {
                alt29=5;
                }
                break;
            default:
                NoViableAltException nvae =
                    new NoViableAltException("", 29, 0, input);

                throw nvae;
            }

            switch (alt29) {
                case 1 :
                    // BNGGrammar_Actions.g:194:11: T_END ASSIGNS (i1= INT | f1= FLOAT )
                    {
                    T_END30=(Token)match(input,T_END,FOLLOW_T_END_in_simulate_par_def1580); 
                    match(input,ASSIGNS,FOLLOW_ASSIGNS_in_simulate_par_def1582); 
                    // BNGGrammar_Actions.g:194:25: (i1= INT | f1= FLOAT )
                    int alt27=2;
                    int LA27_0 = input.LA(1);

                    if ( (LA27_0==INT) ) {
                        alt27=1;
                    }
                    else if ( (LA27_0==FLOAT) ) {
                        alt27=2;
                    }
                    else {
                        NoViableAltException nvae =
                            new NoViableAltException("", 27, 0, input);

                        throw nvae;
                    }
                    switch (alt27) {
                        case 1 :
                            // BNGGrammar_Actions.g:194:26: i1= INT
                            {
                            i1=(Token)match(input,INT,FOLLOW_INT_in_simulate_par_def1587); 
                            map.put((T_END30!=null?T_END30.getText():null),(i1!=null?i1.getText():null));

                            }
                            break;
                        case 2 :
                            // BNGGrammar_Actions.g:194:66: f1= FLOAT
                            {
                            f1=(Token)match(input,FLOAT,FOLLOW_FLOAT_in_simulate_par_def1593); 
                            map.put((T_END30!=null?T_END30.getText():null),(f1!=null?f1.getText():null));

                            }
                            break;

                    }


                    }
                    break;
                case 2 :
                    // BNGGrammar_Actions.g:195:11: T_START ASSIGNS (i2= INT | f2= FLOAT )
                    {
                    T_START31=(Token)match(input,T_START,FOLLOW_T_START_in_simulate_par_def1609); 
                    match(input,ASSIGNS,FOLLOW_ASSIGNS_in_simulate_par_def1611); 
                    // BNGGrammar_Actions.g:195:27: (i2= INT | f2= FLOAT )
                    int alt28=2;
                    int LA28_0 = input.LA(1);

                    if ( (LA28_0==INT) ) {
                        alt28=1;
                    }
                    else if ( (LA28_0==FLOAT) ) {
                        alt28=2;
                    }
                    else {
                        NoViableAltException nvae =
                            new NoViableAltException("", 28, 0, input);

                        throw nvae;
                    }
                    switch (alt28) {
                        case 1 :
                            // BNGGrammar_Actions.g:195:28: i2= INT
                            {
                            i2=(Token)match(input,INT,FOLLOW_INT_in_simulate_par_def1616); 
                            map.put((T_START31!=null?T_START31.getText():null),(i2!=null?i2.getText():null));

                            }
                            break;
                        case 2 :
                            // BNGGrammar_Actions.g:195:70: f2= FLOAT
                            {
                            f2=(Token)match(input,FLOAT,FOLLOW_FLOAT_in_simulate_par_def1622); 
                            map.put((T_START31!=null?T_START31.getText():null),(f2!=null?f2.getText():null));

                            }
                            break;

                    }


                    }
                    break;
                case 3 :
                    // BNGGrammar_Actions.g:196:11: N_STEPS ASSIGNS i3= INT
                    {
                    N_STEPS32=(Token)match(input,N_STEPS,FOLLOW_N_STEPS_in_simulate_par_def1637); 
                    match(input,ASSIGNS,FOLLOW_ASSIGNS_in_simulate_par_def1639); 
                    i3=(Token)match(input,INT,FOLLOW_INT_in_simulate_par_def1643); 
                    map.put((N_STEPS32!=null?N_STEPS32.getText():null),(i3!=null?i3.getText():null));

                    }
                    break;
                case 4 :
                    // BNGGrammar_Actions.g:197:11: SAMPLE_TIMES ASSIGNS i4= array_value
                    {
                    match(input,SAMPLE_TIMES,FOLLOW_SAMPLE_TIMES_in_simulate_par_def1657); 
                    match(input,ASSIGNS,FOLLOW_ASSIGNS_in_simulate_par_def1659); 
                    pushFollow(FOLLOW_array_value_in_simulate_par_def1663);
                    i4=array_value();

                    state._fsp--;


                    }
                    break;
                case 5 :
                    // BNGGrammar_Actions.g:198:11: VERBOSE ASSIGNS i5= INT
                    {
                    VERBOSE33=(Token)match(input,VERBOSE,FOLLOW_VERBOSE_in_simulate_par_def1676); 
                    match(input,ASSIGNS,FOLLOW_ASSIGNS_in_simulate_par_def1678); 
                    i5=(Token)match(input,INT,FOLLOW_INT_in_simulate_par_def1682); 
                    map.put((VERBOSE33!=null?VERBOSE33.getText():null),(i4!=null?input.toString(i4.start,i4.stop):null));

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
    // $ANTLR end "simulate_par_def"

    public static class array_value_return extends ParserRuleReturnScope {
        public StringTemplate st;
        public Object getTemplate() { return st; }
        public String toString() { return st==null?null:st.toString(); }
    };

    // $ANTLR start "array_value"
    // BNGGrammar_Actions.g:201:1: array_value : LSBRACKET STRING ( COMMA STRING )* RSBRACKET ;
    public final BNGGrammar_BNGGrammar_Actions.array_value_return array_value() throws RecognitionException {
        BNGGrammar_BNGGrammar_Actions.array_value_return retval = new BNGGrammar_BNGGrammar_Actions.array_value_return();
        retval.start = input.LT(1);

        try {
            // BNGGrammar_Actions.g:202:9: ( LSBRACKET STRING ( COMMA STRING )* RSBRACKET )
            // BNGGrammar_Actions.g:202:11: LSBRACKET STRING ( COMMA STRING )* RSBRACKET
            {
            match(input,LSBRACKET,FOLLOW_LSBRACKET_in_array_value1717); 
            match(input,STRING,FOLLOW_STRING_in_array_value1719); 
            // BNGGrammar_Actions.g:202:28: ( COMMA STRING )*
            loop30:
            do {
                int alt30=2;
                int LA30_0 = input.LA(1);

                if ( (LA30_0==COMMA) ) {
                    alt30=1;
                }


                switch (alt30) {
            	case 1 :
            	    // BNGGrammar_Actions.g:202:29: COMMA STRING
            	    {
            	    match(input,COMMA,FOLLOW_COMMA_in_array_value1722); 
            	    match(input,STRING,FOLLOW_STRING_in_array_value1724); 

            	    }
            	    break;

            	default :
            	    break loop30;
                }
            } while (true);

            match(input,RSBRACKET,FOLLOW_RSBRACKET_in_array_value1728); 

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
    // $ANTLR end "array_value"

    // Delegated rules


    protected DFA1 dfa1 = new DFA1(this);
    static final String DFA1_eotS =
        "\15\uffff";
    static final String DFA1_eofS =
        "\1\13\14\uffff";
    static final String DFA1_minS =
        "\1\4\14\uffff";
    static final String DFA1_maxS =
        "\1\u00ca\14\uffff";
    static final String DFA1_acceptS =
        "\1\uffff\13\2\1\1";
    static final String DFA1_specialS =
        "\15\uffff}>";
    static final String[] DFA1_transitionS = {
            "\4\14\1\12\62\14\1\2\1\3\1\4\1\14\4\5\1\6\1\7\1\10\1\11\1\1"+
            "\u0083\14",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            ""
    };

    static final short[] DFA1_eot = DFA.unpackEncodedString(DFA1_eotS);
    static final short[] DFA1_eof = DFA.unpackEncodedString(DFA1_eofS);
    static final char[] DFA1_min = DFA.unpackEncodedStringToUnsignedChars(DFA1_minS);
    static final char[] DFA1_max = DFA.unpackEncodedStringToUnsignedChars(DFA1_maxS);
    static final short[] DFA1_accept = DFA.unpackEncodedString(DFA1_acceptS);
    static final short[] DFA1_special = DFA.unpackEncodedString(DFA1_specialS);
    static final short[][] DFA1_transition;

    static {
        int numStates = DFA1_transitionS.length;
        DFA1_transition = new short[numStates][];
        for (int i=0; i<numStates; i++) {
            DFA1_transition[i] = DFA.unpackEncodedString(DFA1_transitionS[i]);
        }
    }

    class DFA1 extends DFA {

        public DFA1(BaseRecognizer recognizer) {
            this.recognizer = recognizer;
            this.decisionNumber = 1;
            this.eot = DFA1_eot;
            this.eof = DFA1_eof;
            this.min = DFA1_min;
            this.max = DFA1_max;
            this.accept = DFA1_accept;
            this.special = DFA1_special;
            this.transition = DFA1_transition;
        }
        public String getDescription() {
            return "()* loopback of 18:1: ( . )*";
        }
    }
 

    public static final BitSet FOLLOW_actions_block_in_actions_prog44 = new BitSet(new long[]{0x0000000000000000L});
    public static final BitSet FOLLOW_BEGIN_in_actions_prog50 = new BitSet(new long[]{0x0000000000200000L});
    public static final BitSet FOLLOW_ACTIONS_in_actions_prog52 = new BitSet(new long[]{0xB800000000000000L,0x00000000000000FFL});
    public static final BitSet FOLLOW_actions_block_in_actions_prog54 = new BitSet(new long[]{0x0000000000000080L});
    public static final BitSet FOLLOW_END_in_actions_prog56 = new BitSet(new long[]{0x0000000000200000L});
    public static final BitSet FOLLOW_ACTIONS_in_actions_prog58 = new BitSet(new long[]{0x0000000000000000L});
    public static final BitSet FOLLOW_EOF_in_actions_prog64 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_action_in_actions_block101 = new BitSet(new long[]{0xB800000000000002L,0x00000000000000FFL});
    public static final BitSet FOLLOW_generate_network_in_action113 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_simulate_method_in_action127 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_read_file_in_action141 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_write_file_in_action155 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_set_concentration_in_action169 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_set_parameter_in_action183 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_save_concentrations_in_action197 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_reset_concentrations_in_action211 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_GENERATENETWORK_in_generate_network244 = new BitSet(new long[]{0x0000000000000000L,0x0000000001000000L});
    public static final BitSet FOLLOW_LPAREN_in_generate_network246 = new BitSet(new long[]{0x0000000000000000L,0x0000000002100000L});
    public static final BitSet FOLLOW_LBRACKET_in_generate_network249 = new BitSet(new long[]{0x000000187C000000L,0x0000000000200000L});
    public static final BitSet FOLLOW_gn_action_par_def_in_generate_network261 = new BitSet(new long[]{0x0000000000000000L,0x0000000000600000L});
    public static final BitSet FOLLOW_COMMA_in_generate_network275 = new BitSet(new long[]{0x000000187C000000L});
    public static final BitSet FOLLOW_gn_action_par_def_in_generate_network277 = new BitSet(new long[]{0x0000000000000000L,0x0000000000600000L});
    public static final BitSet FOLLOW_RBRACKET_in_generate_network292 = new BitSet(new long[]{0x0000000000000000L,0x0000000002000000L});
    public static final BitSet FOLLOW_RPAREN_in_generate_network296 = new BitSet(new long[]{0x0000000000000000L,0x0000000000010000L});
    public static final BitSet FOLLOW_SEMI_in_generate_network298 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_MAX_AGG_in_gn_action_par_def346 = new BitSet(new long[]{0x0000000000000000L,0x0000000800000000L});
    public static final BitSet FOLLOW_ASSIGNS_in_gn_action_par_def348 = new BitSet(new long[]{0x0000000000000000L,0x0000000000002000L});
    public static final BitSet FOLLOW_INT_in_gn_action_par_def352 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_MAX_ITER_in_gn_action_par_def368 = new BitSet(new long[]{0x0000000000000000L,0x0000000800000000L});
    public static final BitSet FOLLOW_ASSIGNS_in_gn_action_par_def370 = new BitSet(new long[]{0x0000000000000000L,0x0000000000002000L});
    public static final BitSet FOLLOW_INT_in_gn_action_par_def374 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_MAX_STOICH_in_gn_action_par_def390 = new BitSet(new long[]{0x0000000000000000L,0x0000000800000000L});
    public static final BitSet FOLLOW_ASSIGNS_in_gn_action_par_def392 = new BitSet(new long[]{0x0000000000000000L,0x0000000000008000L});
    public static final BitSet FOLLOW_hash_value_in_gn_action_par_def394 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_OVERWRITE_in_gn_action_par_def408 = new BitSet(new long[]{0x0000000000000000L,0x0000000800000000L});
    public static final BitSet FOLLOW_ASSIGNS_in_gn_action_par_def410 = new BitSet(new long[]{0x0000000000000000L,0x0000000000002000L});
    public static final BitSet FOLLOW_INT_in_gn_action_par_def414 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_PRINT_ITER_in_gn_action_par_def430 = new BitSet(new long[]{0x0000000000000000L,0x0000000800000000L});
    public static final BitSet FOLLOW_ASSIGNS_in_gn_action_par_def432 = new BitSet(new long[]{0x0000000000000000L,0x0000000000002000L});
    public static final BitSet FOLLOW_INT_in_gn_action_par_def436 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_ps_par_def_in_gn_action_par_def451 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_simulate_ode_in_simulate_method494 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_simulate_ssa_in_simulate_method509 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_SIMULATE_ODE_in_simulate_ode559 = new BitSet(new long[]{0x0000000000000000L,0x0000000001000000L});
    public static final BitSet FOLLOW_LPAREN_in_simulate_ode561 = new BitSet(new long[]{0x0000000000000000L,0x0000000000100000L});
    public static final BitSet FOLLOW_LBRACKET_in_simulate_ode563 = new BitSet(new long[]{0x07F800987C000000L,0x0000000000200000L});
    public static final BitSet FOLLOW_ps_par_def_in_simulate_ode576 = new BitSet(new long[]{0x0000000000000000L,0x0000000000600000L});
    public static final BitSet FOLLOW_simulate_par_def_in_simulate_ode579 = new BitSet(new long[]{0x0000000000000000L,0x0000000000600000L});
    public static final BitSet FOLLOW_simulate_ode_par_def_in_simulate_ode582 = new BitSet(new long[]{0x0000000000000000L,0x0000000000600000L});
    public static final BitSet FOLLOW_COMMA_in_simulate_ode597 = new BitSet(new long[]{0x07F800987C000000L});
    public static final BitSet FOLLOW_ps_par_def_in_simulate_ode600 = new BitSet(new long[]{0x0000000000000000L,0x0000000000600000L});
    public static final BitSet FOLLOW_simulate_par_def_in_simulate_ode603 = new BitSet(new long[]{0x0000000000000000L,0x0000000000600000L});
    public static final BitSet FOLLOW_simulate_ode_par_def_in_simulate_ode606 = new BitSet(new long[]{0x0000000000000000L,0x0000000000600000L});
    public static final BitSet FOLLOW_RBRACKET_in_simulate_ode625 = new BitSet(new long[]{0x0000000000000000L,0x0000000002000000L});
    public static final BitSet FOLLOW_RPAREN_in_simulate_ode627 = new BitSet(new long[]{0x0000000000000000L,0x0000000000010000L});
    public static final BitSet FOLLOW_SEMI_in_simulate_ode629 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_SIMULATE_SSA_in_simulate_ssa655 = new BitSet(new long[]{0x0000000000000000L,0x0000000001000000L});
    public static final BitSet FOLLOW_LPAREN_in_simulate_ssa657 = new BitSet(new long[]{0x0000000000000000L,0x0000000000100000L});
    public static final BitSet FOLLOW_LBRACKET_in_simulate_ssa659 = new BitSet(new long[]{0x078000987C000000L,0x0000000000200000L});
    public static final BitSet FOLLOW_ps_par_def_in_simulate_ssa673 = new BitSet(new long[]{0x0000000000000000L,0x0000000000600000L});
    public static final BitSet FOLLOW_simulate_par_def_in_simulate_ssa676 = new BitSet(new long[]{0x0000000000000000L,0x0000000000600000L});
    public static final BitSet FOLLOW_COMMA_in_simulate_ssa681 = new BitSet(new long[]{0x078000987C000000L});
    public static final BitSet FOLLOW_ps_par_def_in_simulate_ssa684 = new BitSet(new long[]{0x0000000000000000L,0x0000000000600000L});
    public static final BitSet FOLLOW_simulate_par_def_in_simulate_ssa687 = new BitSet(new long[]{0x0000000000000000L,0x0000000000600000L});
    public static final BitSet FOLLOW_RBRACKET_in_simulate_ssa705 = new BitSet(new long[]{0x0000000000000000L,0x0000000002000000L});
    public static final BitSet FOLLOW_RPAREN_in_simulate_ssa707 = new BitSet(new long[]{0x0000000000000000L,0x0000000000010000L});
    public static final BitSet FOLLOW_SEMI_in_simulate_ssa709 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_READFILE_in_read_file732 = new BitSet(new long[]{0x0000000000000000L,0x0000000001000000L});
    public static final BitSet FOLLOW_LPAREN_in_read_file734 = new BitSet(new long[]{0x0000000000000000L,0x0000000002100000L});
    public static final BitSet FOLLOW_LBRACKET_in_read_file746 = new BitSet(new long[]{0x4000000000000000L,0x0000000000200000L});
    public static final BitSet FOLLOW_FILE_in_read_file749 = new BitSet(new long[]{0x0000000000000000L,0x0000000800000000L});
    public static final BitSet FOLLOW_ASSIGNS_in_read_file751 = new BitSet(new long[]{0x0000000000000000L,0x0000000000008000L});
    public static final BitSet FOLLOW_STRING_in_read_file753 = new BitSet(new long[]{0x0000000000000000L,0x0000000000200000L});
    public static final BitSet FOLLOW_RBRACKET_in_read_file759 = new BitSet(new long[]{0x0000000000000000L,0x0000000002000000L});
    public static final BitSet FOLLOW_RPAREN_in_read_file772 = new BitSet(new long[]{0x0000000000000000L,0x0000000000010000L});
    public static final BitSet FOLLOW_SEMI_in_read_file774 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_write_type_in_write_file818 = new BitSet(new long[]{0x0000000000000000L,0x0000000001000000L});
    public static final BitSet FOLLOW_LPAREN_in_write_file830 = new BitSet(new long[]{0x0000000000000000L,0x0000000002100000L});
    public static final BitSet FOLLOW_LBRACKET_in_write_file833 = new BitSet(new long[]{0x0000000000000000L,0x0000000000200000L});
    public static final BitSet FOLLOW_RBRACKET_in_write_file835 = new BitSet(new long[]{0x0000000000000000L,0x0000000002000000L});
    public static final BitSet FOLLOW_RPAREN_in_write_file850 = new BitSet(new long[]{0x0000000000000000L,0x0000000000010000L});
    public static final BitSet FOLLOW_SEMI_in_write_file852 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_set_in_write_type0 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_SETCONCENTRATION_in_set_concentration953 = new BitSet(new long[]{0x0000000000000000L,0x0000000001000000L});
    public static final BitSet FOLLOW_LPAREN_in_set_concentration955 = new BitSet(new long[]{0x0000000000000000L,0x0000400000000000L});
    public static final BitSet FOLLOW_variable_definition_in_set_concentration957 = new BitSet(new long[]{0x0000000000000000L,0x0000000002000000L});
    public static final BitSet FOLLOW_RPAREN_in_set_concentration978 = new BitSet(new long[]{0x0000000000000000L,0x0000000000010000L});
    public static final BitSet FOLLOW_SEMI_in_set_concentration980 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_SETPARAMETER_in_set_parameter1026 = new BitSet(new long[]{0x0000000000000000L,0x0000000001000000L});
    public static final BitSet FOLLOW_LPAREN_in_set_parameter1028 = new BitSet(new long[]{0x0000000000000000L,0x0000400000000000L});
    public static final BitSet FOLLOW_parameter_definition_in_set_parameter1030 = new BitSet(new long[]{0x0000000000000000L,0x0000000002000000L});
    public static final BitSet FOLLOW_RPAREN_in_set_parameter1053 = new BitSet(new long[]{0x0000000000000000L,0x0000000000010000L});
    public static final BitSet FOLLOW_SEMI_in_set_parameter1055 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_DBQUOTES_in_parameter_definition1089 = new BitSet(new long[]{0x0000000000000000L,0x0000000000008000L});
    public static final BitSet FOLLOW_STRING_in_parameter_definition1092 = new BitSet(new long[]{0x0000000000000000L,0x0000400000000000L});
    public static final BitSet FOLLOW_DBQUOTES_in_parameter_definition1097 = new BitSet(new long[]{0x0000000000000000L,0x0000000000400000L});
    public static final BitSet FOLLOW_COMMA_in_parameter_definition1099 = new BitSet(new long[]{0x0000400000000000L,0x000041000100B000L});
    public static final BitSet FOLLOW_expression_in_parameter_definition1105 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_DBQUOTES_in_parameter_definition1113 = new BitSet(new long[]{0x0000400000000000L,0x000001000100B000L});
    public static final BitSet FOLLOW_expression_in_parameter_definition1117 = new BitSet(new long[]{0x0000000000000000L,0x0000400000000000L});
    public static final BitSet FOLLOW_DBQUOTES_in_parameter_definition1120 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_DBQUOTES_in_variable_definition1152 = new BitSet(new long[]{0x0000000000000000L,0x0000000050008000L});
    public static final BitSet FOLLOW_species_def_in_variable_definition1155 = new BitSet(new long[]{0x0000000000000000L,0x0000400000000000L});
    public static final BitSet FOLLOW_DBQUOTES_in_variable_definition1161 = new BitSet(new long[]{0x0000000000000000L,0x0000000000400000L});
    public static final BitSet FOLLOW_COMMA_in_variable_definition1163 = new BitSet(new long[]{0x0000400000000000L,0x000041000100B000L});
    public static final BitSet FOLLOW_expression_in_variable_definition1169 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_DBQUOTES_in_variable_definition1177 = new BitSet(new long[]{0x0000400000000000L,0x000001000100B000L});
    public static final BitSet FOLLOW_expression_in_variable_definition1181 = new BitSet(new long[]{0x0000000000000000L,0x0000400000000000L});
    public static final BitSet FOLLOW_DBQUOTES_in_variable_definition1184 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_SAVECONCENTRATIONS_in_save_concentrations1241 = new BitSet(new long[]{0x0000000000000000L,0x0000000001000000L});
    public static final BitSet FOLLOW_LPAREN_in_save_concentrations1243 = new BitSet(new long[]{0x0000000000000000L,0x0000000002000000L});
    public static final BitSet FOLLOW_RPAREN_in_save_concentrations1245 = new BitSet(new long[]{0x0000000000000000L,0x0000000000010000L});
    public static final BitSet FOLLOW_SEMI_in_save_concentrations1247 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_RESETCONCENTRATIONS_in_reset_concentrations1280 = new BitSet(new long[]{0x0000000000000000L,0x0000000001000000L});
    public static final BitSet FOLLOW_LPAREN_in_reset_concentrations1282 = new BitSet(new long[]{0x0000000000000000L,0x0000000002000000L});
    public static final BitSet FOLLOW_RPAREN_in_reset_concentrations1284 = new BitSet(new long[]{0x0000000000000000L,0x0000000000010000L});
    public static final BitSet FOLLOW_SEMI_in_reset_concentrations1286 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_STRING_in_hash_value1319 = new BitSet(new long[]{0x0000000000000000L,0x0000000800000000L});
    public static final BitSet FOLLOW_ASSIGNS_in_hash_value1321 = new BitSet(new long[]{0x0000000000000000L,0x0000000000100000L});
    public static final BitSet FOLLOW_LBRACKET_in_hash_value1323 = new BitSet(new long[]{0x0000000000000000L,0x0000000000008000L});
    public static final BitSet FOLLOW_assignment_list_in_hash_value1325 = new BitSet(new long[]{0x0000000000000000L,0x0000000000200000L});
    public static final BitSet FOLLOW_RBRACKET_in_hash_value1327 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_STRING_in_assignment_list1351 = new BitSet(new long[]{0x0000000000000000L,0x0000000800000000L});
    public static final BitSet FOLLOW_ASSIGNS_in_assignment_list1353 = new BitSet(new long[]{0x0000000000000000L,0x000000000000B000L});
    public static final BitSet FOLLOW_value_in_assignment_list1355 = new BitSet(new long[]{0x0000000000000000L,0x0000000000400000L});
    public static final BitSet FOLLOW_COMMA_in_assignment_list1358 = new BitSet(new long[]{0x0000000000000000L,0x0000000000008000L});
    public static final BitSet FOLLOW_STRING_in_assignment_list1360 = new BitSet(new long[]{0x0000000000000000L,0x0000000800000000L});
    public static final BitSet FOLLOW_ASSIGNS_in_assignment_list1362 = new BitSet(new long[]{0x0000000000000000L,0x000000000000B000L});
    public static final BitSet FOLLOW_value_in_assignment_list1364 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_set_in_value0 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_PREFFIX_in_ps_par_def1416 = new BitSet(new long[]{0x0000000000000000L,0x0000000800000000L});
    public static final BitSet FOLLOW_ASSIGNS_in_ps_par_def1418 = new BitSet(new long[]{0x0000000000000000L,0x0000400000008000L});
    public static final BitSet FOLLOW_DBQUOTES_in_ps_par_def1422 = new BitSet(new long[]{0x0000000000000000L,0x0000000000008000L});
    public static final BitSet FOLLOW_STRING_in_ps_par_def1425 = new BitSet(new long[]{0x0000000000000000L,0x0000400000000000L});
    public static final BitSet FOLLOW_DBQUOTES_in_ps_par_def1427 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_STRING_in_ps_par_def1432 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_SUFFIX_in_ps_par_def1445 = new BitSet(new long[]{0x0000000000000000L,0x0000000800000000L});
    public static final BitSet FOLLOW_ASSIGNS_in_ps_par_def1447 = new BitSet(new long[]{0x0000000000000000L,0x0000400000008000L});
    public static final BitSet FOLLOW_DBQUOTES_in_ps_par_def1451 = new BitSet(new long[]{0x0000000000000000L,0x0000000000008000L});
    public static final BitSet FOLLOW_STRING_in_ps_par_def1454 = new BitSet(new long[]{0x0000000000000000L,0x0000400000000000L});
    public static final BitSet FOLLOW_DBQUOTES_in_ps_par_def1456 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_STRING_in_ps_par_def1461 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_ATOL_in_simulate_ode_par_def1487 = new BitSet(new long[]{0x0000000000000000L,0x0000000800000000L});
    public static final BitSet FOLLOW_ASSIGNS_in_simulate_ode_par_def1489 = new BitSet(new long[]{0x0000000000000000L,0x0000000000001000L});
    public static final BitSet FOLLOW_FLOAT_in_simulate_ode_par_def1493 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_RTOL_in_simulate_ode_par_def1507 = new BitSet(new long[]{0x0000000000000000L,0x0000000800000000L});
    public static final BitSet FOLLOW_ASSIGNS_in_simulate_ode_par_def1509 = new BitSet(new long[]{0x0000000000000000L,0x0000000000001000L});
    public static final BitSet FOLLOW_FLOAT_in_simulate_ode_par_def1513 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_STEADY_STATE_in_simulate_ode_par_def1527 = new BitSet(new long[]{0x0000000000000000L,0x0000000800000000L});
    public static final BitSet FOLLOW_ASSIGNS_in_simulate_ode_par_def1529 = new BitSet(new long[]{0x0000000000000000L,0x0000000000002000L});
    public static final BitSet FOLLOW_INT_in_simulate_ode_par_def1533 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_SPARSE_in_simulate_ode_par_def1547 = new BitSet(new long[]{0x0000000000000000L,0x0000000800000000L});
    public static final BitSet FOLLOW_ASSIGNS_in_simulate_ode_par_def1549 = new BitSet(new long[]{0x0000000000000000L,0x0000000000002000L});
    public static final BitSet FOLLOW_INT_in_simulate_ode_par_def1553 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_T_END_in_simulate_par_def1580 = new BitSet(new long[]{0x0000000000000000L,0x0000000800000000L});
    public static final BitSet FOLLOW_ASSIGNS_in_simulate_par_def1582 = new BitSet(new long[]{0x0000000000000000L,0x0000000000003000L});
    public static final BitSet FOLLOW_INT_in_simulate_par_def1587 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_FLOAT_in_simulate_par_def1593 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_T_START_in_simulate_par_def1609 = new BitSet(new long[]{0x0000000000000000L,0x0000000800000000L});
    public static final BitSet FOLLOW_ASSIGNS_in_simulate_par_def1611 = new BitSet(new long[]{0x0000000000000000L,0x0000000000003000L});
    public static final BitSet FOLLOW_INT_in_simulate_par_def1616 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_FLOAT_in_simulate_par_def1622 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_N_STEPS_in_simulate_par_def1637 = new BitSet(new long[]{0x0000000000000000L,0x0000000800000000L});
    public static final BitSet FOLLOW_ASSIGNS_in_simulate_par_def1639 = new BitSet(new long[]{0x0000000000000000L,0x0000000000002000L});
    public static final BitSet FOLLOW_INT_in_simulate_par_def1643 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_SAMPLE_TIMES_in_simulate_par_def1657 = new BitSet(new long[]{0x0000000000000000L,0x0000000800000000L});
    public static final BitSet FOLLOW_ASSIGNS_in_simulate_par_def1659 = new BitSet(new long[]{0x0000000000000000L,0x0000000000040000L});
    public static final BitSet FOLLOW_array_value_in_simulate_par_def1663 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_VERBOSE_in_simulate_par_def1676 = new BitSet(new long[]{0x0000000000000000L,0x0000000800000000L});
    public static final BitSet FOLLOW_ASSIGNS_in_simulate_par_def1678 = new BitSet(new long[]{0x0000000000000000L,0x0000000000002000L});
    public static final BitSet FOLLOW_INT_in_simulate_par_def1682 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_LSBRACKET_in_array_value1717 = new BitSet(new long[]{0x0000000000000000L,0x0000000000008000L});
    public static final BitSet FOLLOW_STRING_in_array_value1719 = new BitSet(new long[]{0x0000000000000000L,0x0000000000480000L});
    public static final BitSet FOLLOW_COMMA_in_array_value1722 = new BitSet(new long[]{0x0000000000000000L,0x0000000000008000L});
    public static final BitSet FOLLOW_STRING_in_array_value1724 = new BitSet(new long[]{0x0000000000000000L,0x0000000000480000L});
    public static final BitSet FOLLOW_RSBRACKET_in_array_value1728 = new BitSet(new long[]{0x0000000000000002L});

}