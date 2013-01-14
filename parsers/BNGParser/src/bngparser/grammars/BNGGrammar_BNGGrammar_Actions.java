// $ANTLR 3.3 Nov 30, 2010 12:50:56 BNGGrammar_Actions.g 2013-01-14 14:49:39

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
public class BNGGrammar_BNGGrammar_Actions extends Parser {
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
           @Override
      public String getErrorMessage(RecognitionException e,String[] tokenNames){
        return gParent.getErrorMessage(e,tokenNames);
      }


    public static class actions_prog_return extends ParserRuleReturnScope {
        public StringTemplate st;
        public Object getTemplate() { return st; }
        public String toString() { return st==null?null:st.toString(); }
    };

    // $ANTLR start "actions_prog"
    // BNGGrammar_Actions.g:21:1: actions_prog : ( . )* actions_block EOF -> actions(actions=actions);
    public final BNGGrammar_BNGGrammar_Actions.actions_prog_return actions_prog() throws RecognitionException {
        BNGGrammar_BNGGrammar_Actions.actions_prog_return retval = new BNGGrammar_BNGGrammar_Actions.actions_prog_return();
        retval.start = input.LT(1);

        try {
            // BNGGrammar_Actions.g:21:13: ( ( . )* actions_block EOF -> actions(actions=actions))
            // BNGGrammar_Actions.g:22:1: ( . )* actions_block EOF
            {
            // BNGGrammar_Actions.g:22:1: ( . )*
            loop1:
            do {
                int alt1=2;
                alt1 = dfa1.predict(input);
                switch (alt1) {
            	case 1 :
            	    // BNGGrammar_Actions.g:22:1: .
            	    {
            	    matchAny(input); 

            	    }
            	    break;

            	default :
            	    break loop1;
                }
            } while (true);

            pushFollow(FOLLOW_actions_block_in_actions_prog44);
            actions_block();

            state._fsp--;

            match(input,EOF,FOLLOW_EOF_in_actions_prog50); 


            // TEMPLATE REWRITE
            // 26:5: -> actions(actions=actions)
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
    // BNGGrammar_Actions.g:28:1: actions_block : ( BEGIN ACTIONS ( LB )+ )? ( action ( LB )* )+ ( END ACTIONS ( LB )* )? ;
    public final BNGGrammar_BNGGrammar_Actions.actions_block_return actions_block() throws RecognitionException {
        BNGGrammar_BNGGrammar_Actions.actions_block_return retval = new BNGGrammar_BNGGrammar_Actions.actions_block_return();
        retval.start = input.LT(1);


          getParentTemplate();
          gParent.paraphrases.push("in the actions block");

        try {
            // BNGGrammar_Actions.g:37:9: ( ( BEGIN ACTIONS ( LB )+ )? ( action ( LB )* )+ ( END ACTIONS ( LB )* )? )
            // BNGGrammar_Actions.g:38:9: ( BEGIN ACTIONS ( LB )+ )? ( action ( LB )* )+ ( END ACTIONS ( LB )* )?
            {
            // BNGGrammar_Actions.g:38:9: ( BEGIN ACTIONS ( LB )+ )?
            int alt3=2;
            int LA3_0 = input.LA(1);

            if ( (LA3_0==BEGIN) ) {
                alt3=1;
            }
            switch (alt3) {
                case 1 :
                    // BNGGrammar_Actions.g:38:10: BEGIN ACTIONS ( LB )+
                    {
                    match(input,BEGIN,FOLLOW_BEGIN_in_actions_block96); 
                    match(input,ACTIONS,FOLLOW_ACTIONS_in_actions_block98); 
                    // BNGGrammar_Actions.g:38:24: ( LB )+
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
                    	    // BNGGrammar_Actions.g:38:24: LB
                    	    {
                    	    match(input,LB,FOLLOW_LB_in_actions_block100); 

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

            }

            // BNGGrammar_Actions.g:39:9: ( action ( LB )* )+
            int cnt5=0;
            loop5:
            do {
                int alt5=2;
                int LA5_0 = input.LA(1);

                if ( ((LA5_0>=SIMULATE_ODE && LA5_0<=READFILE)||(LA5_0>=WRITENET && LA5_0<=WRITEMEXFILE)||LA5_0==SETCONCENTRATION||(LA5_0>=SETPARAMETER && LA5_0<=WRITENETWORK)) ) {
                    alt5=1;
                }


                switch (alt5) {
            	case 1 :
            	    // BNGGrammar_Actions.g:39:10: action ( LB )*
            	    {
            	    pushFollow(FOLLOW_action_in_actions_block114);
            	    action();

            	    state._fsp--;

            	    // BNGGrammar_Actions.g:39:17: ( LB )*
            	    loop4:
            	    do {
            	        int alt4=2;
            	        int LA4_0 = input.LA(1);

            	        if ( (LA4_0==LB) ) {
            	            alt4=1;
            	        }


            	        switch (alt4) {
            	    	case 1 :
            	    	    // BNGGrammar_Actions.g:39:17: LB
            	    	    {
            	    	    match(input,LB,FOLLOW_LB_in_actions_block116); 

            	    	    }
            	    	    break;

            	    	default :
            	    	    break loop4;
            	        }
            	    } while (true);


            	    }
            	    break;

            	default :
            	    if ( cnt5 >= 1 ) break loop5;
                        EarlyExitException eee =
                            new EarlyExitException(5, input);
                        throw eee;
                }
                cnt5++;
            } while (true);

            // BNGGrammar_Actions.g:40:9: ( END ACTIONS ( LB )* )?
            int alt7=2;
            int LA7_0 = input.LA(1);

            if ( (LA7_0==END) ) {
                alt7=1;
            }
            switch (alt7) {
                case 1 :
                    // BNGGrammar_Actions.g:40:10: END ACTIONS ( LB )*
                    {
                    match(input,END,FOLLOW_END_in_actions_block130); 
                    match(input,ACTIONS,FOLLOW_ACTIONS_in_actions_block132); 
                    // BNGGrammar_Actions.g:40:22: ( LB )*
                    loop6:
                    do {
                        int alt6=2;
                        int LA6_0 = input.LA(1);

                        if ( (LA6_0==LB) ) {
                            alt6=1;
                        }


                        switch (alt6) {
                    	case 1 :
                    	    // BNGGrammar_Actions.g:40:22: LB
                    	    {
                    	    match(input,LB,FOLLOW_LB_in_actions_block134); 

                    	    }
                    	    break;

                    	default :
                    	    break loop6;
                        }
                    } while (true);


                    }
                    break;

            }


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
    // BNGGrammar_Actions.g:42:1: action : ( generate_network | simulate_method | read_file | write_file | set_concentration | set_parameter | save_concentrations | reset_concentrations | add_concentration );
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

        BNGGrammar_BNGGrammar_Actions.add_concentration_return add_concentration9 = null;


        try {
            // BNGGrammar_Actions.g:42:9: ( generate_network | simulate_method | read_file | write_file | set_concentration | set_parameter | save_concentrations | reset_concentrations | add_concentration )
            int alt8=9;
            switch ( input.LA(1) ) {
            case GENERATENETWORK:
                {
                alt8=1;
                }
                break;
            case SIMULATE_ODE:
            case SIMULATE_SSA:
            case SIMULATE_NF:
            case SIMULATE_PLA:
            case SIMULATE:
            case WRITEMFILE:
            case WRITEMEXFILE:
            case WRITENETWORK:
                {
                alt8=2;
                }
                break;
            case READFILE:
                {
                alt8=3;
                }
                break;
            case WRITENET:
            case WRITESBML:
            case WRITEXML:
                {
                alt8=4;
                }
                break;
            case SETCONCENTRATION:
                {
                alt8=5;
                }
                break;
            case SETPARAMETER:
                {
                alt8=6;
                }
                break;
            case SAVECONCENTRATIONS:
                {
                alt8=7;
                }
                break;
            case RESETCONCENTRATIONS:
                {
                alt8=8;
                }
                break;
            case ADDCONCENTRATION:
                {
                alt8=9;
                }
                break;
            default:
                NoViableAltException nvae =
                    new NoViableAltException("", 8, 0, input);

                throw nvae;
            }

            switch (alt8) {
                case 1 :
                    // BNGGrammar_Actions.g:42:11: generate_network
                    {
                    pushFollow(FOLLOW_generate_network_in_action146);
                    generate_network1=generate_network();

                    state._fsp--;

                    actions.add((generate_network1!=null?generate_network1.st:null));

                    }
                    break;
                case 2 :
                    // BNGGrammar_Actions.g:43:11: simulate_method
                    {
                    pushFollow(FOLLOW_simulate_method_in_action160);
                    simulate_method2=simulate_method();

                    state._fsp--;

                    actions.add((simulate_method2!=null?simulate_method2.st:null));

                    }
                    break;
                case 3 :
                    // BNGGrammar_Actions.g:44:11: read_file
                    {
                    pushFollow(FOLLOW_read_file_in_action174);
                    read_file3=read_file();

                    state._fsp--;

                    actions.add((read_file3!=null?read_file3.st:null));

                    }
                    break;
                case 4 :
                    // BNGGrammar_Actions.g:45:11: write_file
                    {
                    pushFollow(FOLLOW_write_file_in_action188);
                    write_file4=write_file();

                    state._fsp--;

                    actions.add((write_file4!=null?write_file4.st:null));

                    }
                    break;
                case 5 :
                    // BNGGrammar_Actions.g:46:11: set_concentration
                    {
                    pushFollow(FOLLOW_set_concentration_in_action202);
                    set_concentration5=set_concentration();

                    state._fsp--;

                    actions.add((set_concentration5!=null?set_concentration5.st:null));

                    }
                    break;
                case 6 :
                    // BNGGrammar_Actions.g:47:11: set_parameter
                    {
                    pushFollow(FOLLOW_set_parameter_in_action216);
                    set_parameter6=set_parameter();

                    state._fsp--;

                    actions.add((set_parameter6!=null?set_parameter6.st:null));

                    }
                    break;
                case 7 :
                    // BNGGrammar_Actions.g:48:11: save_concentrations
                    {
                    pushFollow(FOLLOW_save_concentrations_in_action230);
                    save_concentrations7=save_concentrations();

                    state._fsp--;

                    actions.add((save_concentrations7!=null?save_concentrations7.st:null));

                    }
                    break;
                case 8 :
                    // BNGGrammar_Actions.g:49:11: reset_concentrations
                    {
                    pushFollow(FOLLOW_reset_concentrations_in_action244);
                    reset_concentrations8=reset_concentrations();

                    state._fsp--;

                    actions.add((reset_concentrations8!=null?reset_concentrations8.st:null));

                    }
                    break;
                case 9 :
                    // BNGGrammar_Actions.g:50:11: add_concentration
                    {
                    pushFollow(FOLLOW_add_concentration_in_action258);
                    add_concentration9=add_concentration();

                    state._fsp--;

                    actions.add((add_concentration9!=null?add_concentration9.st:null));

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
    // BNGGrammar_Actions.g:52:1: generate_network : GENERATENETWORK LPAREN ( LBRACKET ( ( gn_action_par_def[$generate_network::actions] ) ( COMMA gn_action_par_def[$generate_network::actions] )* )? RBRACKET )? RPAREN ( SEMI )? -> action(id=$GENERATENETWORK.textoptionMap=$generate_network::actions);
    public final BNGGrammar_BNGGrammar_Actions.generate_network_return generate_network() throws RecognitionException {
        generate_network_stack.push(new generate_network_scope());
        BNGGrammar_BNGGrammar_Actions.generate_network_return retval = new BNGGrammar_BNGGrammar_Actions.generate_network_return();
        retval.start = input.LT(1);

        Token GENERATENETWORK10=null;


          ((generate_network_scope)generate_network_stack.peek()).actions = new HashMap<String,String>();

        try {
            // BNGGrammar_Actions.g:59:9: ( GENERATENETWORK LPAREN ( LBRACKET ( ( gn_action_par_def[$generate_network::actions] ) ( COMMA gn_action_par_def[$generate_network::actions] )* )? RBRACKET )? RPAREN ( SEMI )? -> action(id=$GENERATENETWORK.textoptionMap=$generate_network::actions))
            // BNGGrammar_Actions.g:59:11: GENERATENETWORK LPAREN ( LBRACKET ( ( gn_action_par_def[$generate_network::actions] ) ( COMMA gn_action_par_def[$generate_network::actions] )* )? RBRACKET )? RPAREN ( SEMI )?
            {
            GENERATENETWORK10=(Token)match(input,GENERATENETWORK,FOLLOW_GENERATENETWORK_in_generate_network291); 
            match(input,LPAREN,FOLLOW_LPAREN_in_generate_network293); 
            // BNGGrammar_Actions.g:59:34: ( LBRACKET ( ( gn_action_par_def[$generate_network::actions] ) ( COMMA gn_action_par_def[$generate_network::actions] )* )? RBRACKET )?
            int alt11=2;
            int LA11_0 = input.LA(1);

            if ( (LA11_0==LBRACKET) ) {
                alt11=1;
            }
            switch (alt11) {
                case 1 :
                    // BNGGrammar_Actions.g:59:35: LBRACKET ( ( gn_action_par_def[$generate_network::actions] ) ( COMMA gn_action_par_def[$generate_network::actions] )* )? RBRACKET
                    {
                    match(input,LBRACKET,FOLLOW_LBRACKET_in_generate_network296); 
                    // BNGGrammar_Actions.g:60:9: ( ( gn_action_par_def[$generate_network::actions] ) ( COMMA gn_action_par_def[$generate_network::actions] )* )?
                    int alt10=2;
                    int LA10_0 = input.LA(1);

                    if ( ((LA10_0>=MAX_AGG && LA10_0<=PRINT_ITER)||(LA10_0>=PREFFIX && LA10_0<=SUFFIX)||LA10_0==TEXTREACTION) ) {
                        alt10=1;
                    }
                    switch (alt10) {
                        case 1 :
                            // BNGGrammar_Actions.g:60:10: ( gn_action_par_def[$generate_network::actions] ) ( COMMA gn_action_par_def[$generate_network::actions] )*
                            {
                            // BNGGrammar_Actions.g:60:10: ( gn_action_par_def[$generate_network::actions] )
                            // BNGGrammar_Actions.g:60:11: gn_action_par_def[$generate_network::actions]
                            {
                            pushFollow(FOLLOW_gn_action_par_def_in_generate_network308);
                            gn_action_par_def(((generate_network_scope)generate_network_stack.peek()).actions);

                            state._fsp--;


                            }

                            // BNGGrammar_Actions.g:61:9: ( COMMA gn_action_par_def[$generate_network::actions] )*
                            loop9:
                            do {
                                int alt9=2;
                                int LA9_0 = input.LA(1);

                                if ( (LA9_0==COMMA) ) {
                                    alt9=1;
                                }


                                switch (alt9) {
                            	case 1 :
                            	    // BNGGrammar_Actions.g:61:10: COMMA gn_action_par_def[$generate_network::actions]
                            	    {
                            	    match(input,COMMA,FOLLOW_COMMA_in_generate_network322); 
                            	    pushFollow(FOLLOW_gn_action_par_def_in_generate_network324);
                            	    gn_action_par_def(((generate_network_scope)generate_network_stack.peek()).actions);

                            	    state._fsp--;


                            	    }
                            	    break;

                            	default :
                            	    break loop9;
                                }
                            } while (true);


                            }
                            break;

                    }

                    match(input,RBRACKET,FOLLOW_RBRACKET_in_generate_network339); 

                    }
                    break;

            }

            match(input,RPAREN,FOLLOW_RPAREN_in_generate_network343); 
            // BNGGrammar_Actions.g:62:27: ( SEMI )?
            int alt12=2;
            int LA12_0 = input.LA(1);

            if ( (LA12_0==SEMI) ) {
                alt12=1;
            }
            switch (alt12) {
                case 1 :
                    // BNGGrammar_Actions.g:62:27: SEMI
                    {
                    match(input,SEMI,FOLLOW_SEMI_in_generate_network345); 

                    }
                    break;

            }



            // TEMPLATE REWRITE
            // 62:33: -> action(id=$GENERATENETWORK.textoptionMap=$generate_network::actions)
            {
                retval.st = templateLib.getInstanceOf("action",
              new STAttrMap().put("id", (GENERATENETWORK10!=null?GENERATENETWORK10.getText():null)).put("optionMap", ((generate_network_scope)generate_network_stack.peek()).actions));
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
    // BNGGrammar_Actions.g:65:1: gn_action_par_def[Map<String,String> map] : ( ( MAX_AGG ASSIGNS i1= INT ) | ( MAX_ITER ASSIGNS i2= INT ) | ( MAX_STOICH ASSIGNS hash_value ) | ( OVERWRITE ASSIGNS i3= INT ) | ( PRINT_ITER ASSIGNS i4= INT ) | ( TEXTREACTION ASSIGNS i5= INT ) | ps_par_def[map] );
    public final BNGGrammar_BNGGrammar_Actions.gn_action_par_def_return gn_action_par_def(Map<String,String> map) throws RecognitionException {
        BNGGrammar_BNGGrammar_Actions.gn_action_par_def_return retval = new BNGGrammar_BNGGrammar_Actions.gn_action_par_def_return();
        retval.start = input.LT(1);

        Token i1=null;
        Token i2=null;
        Token i3=null;
        Token i4=null;
        Token i5=null;
        Token MAX_AGG11=null;
        Token MAX_ITER12=null;
        Token OVERWRITE13=null;
        Token PRINT_ITER14=null;
        Token TEXTREACTION15=null;

        try {
            // BNGGrammar_Actions.g:66:9: ( ( MAX_AGG ASSIGNS i1= INT ) | ( MAX_ITER ASSIGNS i2= INT ) | ( MAX_STOICH ASSIGNS hash_value ) | ( OVERWRITE ASSIGNS i3= INT ) | ( PRINT_ITER ASSIGNS i4= INT ) | ( TEXTREACTION ASSIGNS i5= INT ) | ps_par_def[map] )
            int alt13=7;
            switch ( input.LA(1) ) {
            case MAX_AGG:
                {
                alt13=1;
                }
                break;
            case MAX_ITER:
                {
                alt13=2;
                }
                break;
            case MAX_STOICH:
                {
                alt13=3;
                }
                break;
            case OVERWRITE:
                {
                alt13=4;
                }
                break;
            case PRINT_ITER:
                {
                alt13=5;
                }
                break;
            case TEXTREACTION:
                {
                alt13=6;
                }
                break;
            case PREFFIX:
            case SUFFIX:
                {
                alt13=7;
                }
                break;
            default:
                NoViableAltException nvae =
                    new NoViableAltException("", 13, 0, input);

                throw nvae;
            }

            switch (alt13) {
                case 1 :
                    // BNGGrammar_Actions.g:66:11: ( MAX_AGG ASSIGNS i1= INT )
                    {
                    // BNGGrammar_Actions.g:66:11: ( MAX_AGG ASSIGNS i1= INT )
                    // BNGGrammar_Actions.g:66:12: MAX_AGG ASSIGNS i1= INT
                    {
                    MAX_AGG11=(Token)match(input,MAX_AGG,FOLLOW_MAX_AGG_in_gn_action_par_def394); 
                    match(input,ASSIGNS,FOLLOW_ASSIGNS_in_gn_action_par_def396); 
                    i1=(Token)match(input,INT,FOLLOW_INT_in_gn_action_par_def400); 
                    map.put((MAX_AGG11!=null?MAX_AGG11.getText():null),(i1!=null?i1.getText():null));

                    }


                    }
                    break;
                case 2 :
                    // BNGGrammar_Actions.g:67:11: ( MAX_ITER ASSIGNS i2= INT )
                    {
                    // BNGGrammar_Actions.g:67:11: ( MAX_ITER ASSIGNS i2= INT )
                    // BNGGrammar_Actions.g:67:12: MAX_ITER ASSIGNS i2= INT
                    {
                    MAX_ITER12=(Token)match(input,MAX_ITER,FOLLOW_MAX_ITER_in_gn_action_par_def416); 
                    match(input,ASSIGNS,FOLLOW_ASSIGNS_in_gn_action_par_def418); 
                    i2=(Token)match(input,INT,FOLLOW_INT_in_gn_action_par_def422); 
                    map.put((MAX_ITER12!=null?MAX_ITER12.getText():null),(i2!=null?i2.getText():null));

                    }


                    }
                    break;
                case 3 :
                    // BNGGrammar_Actions.g:68:11: ( MAX_STOICH ASSIGNS hash_value )
                    {
                    // BNGGrammar_Actions.g:68:11: ( MAX_STOICH ASSIGNS hash_value )
                    // BNGGrammar_Actions.g:68:12: MAX_STOICH ASSIGNS hash_value
                    {
                    match(input,MAX_STOICH,FOLLOW_MAX_STOICH_in_gn_action_par_def438); 
                    match(input,ASSIGNS,FOLLOW_ASSIGNS_in_gn_action_par_def440); 
                    pushFollow(FOLLOW_hash_value_in_gn_action_par_def442);
                    hash_value();

                    state._fsp--;


                    }


                    }
                    break;
                case 4 :
                    // BNGGrammar_Actions.g:69:11: ( OVERWRITE ASSIGNS i3= INT )
                    {
                    // BNGGrammar_Actions.g:69:11: ( OVERWRITE ASSIGNS i3= INT )
                    // BNGGrammar_Actions.g:69:12: OVERWRITE ASSIGNS i3= INT
                    {
                    OVERWRITE13=(Token)match(input,OVERWRITE,FOLLOW_OVERWRITE_in_gn_action_par_def456); 
                    match(input,ASSIGNS,FOLLOW_ASSIGNS_in_gn_action_par_def458); 
                    i3=(Token)match(input,INT,FOLLOW_INT_in_gn_action_par_def462); 
                    map.put((OVERWRITE13!=null?OVERWRITE13.getText():null),(i3!=null?i3.getText():null));

                    }


                    }
                    break;
                case 5 :
                    // BNGGrammar_Actions.g:70:11: ( PRINT_ITER ASSIGNS i4= INT )
                    {
                    // BNGGrammar_Actions.g:70:11: ( PRINT_ITER ASSIGNS i4= INT )
                    // BNGGrammar_Actions.g:70:12: PRINT_ITER ASSIGNS i4= INT
                    {
                    PRINT_ITER14=(Token)match(input,PRINT_ITER,FOLLOW_PRINT_ITER_in_gn_action_par_def478); 
                    match(input,ASSIGNS,FOLLOW_ASSIGNS_in_gn_action_par_def480); 
                    i4=(Token)match(input,INT,FOLLOW_INT_in_gn_action_par_def484); 
                    map.put((PRINT_ITER14!=null?PRINT_ITER14.getText():null),(i4!=null?i4.getText():null));

                    }


                    }
                    break;
                case 6 :
                    // BNGGrammar_Actions.g:71:11: ( TEXTREACTION ASSIGNS i5= INT )
                    {
                    // BNGGrammar_Actions.g:71:11: ( TEXTREACTION ASSIGNS i5= INT )
                    // BNGGrammar_Actions.g:71:12: TEXTREACTION ASSIGNS i5= INT
                    {
                    TEXTREACTION15=(Token)match(input,TEXTREACTION,FOLLOW_TEXTREACTION_in_gn_action_par_def500); 
                    match(input,ASSIGNS,FOLLOW_ASSIGNS_in_gn_action_par_def502); 
                    i5=(Token)match(input,INT,FOLLOW_INT_in_gn_action_par_def506); 
                    map.put((TEXTREACTION15!=null?TEXTREACTION15.getText():null),(i5!=null?i5.getText():null));

                    }


                    }
                    break;
                case 7 :
                    // BNGGrammar_Actions.g:72:11: ps_par_def[map]
                    {
                    pushFollow(FOLLOW_ps_par_def_in_gn_action_par_def521);
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
    // BNGGrammar_Actions.g:75:1: simulate_method : ( simulate_ode[$simulate_method::actions] | simulate_ssa[$simulate_method::actions] | write_m_file[$simulate_method::actions] | write_mex_file[$simulate_method::actions] | write_network[$simulate_method::actions] | simulate_pla[$simulate_method::actions] | simulate_nf[$simulate_method::actions] | simulate[$simulate_method::actions] ) -> action(id=$simulate_method::methodoptionMap=$simulate_method::actions);
    public final BNGGrammar_BNGGrammar_Actions.simulate_method_return simulate_method() throws RecognitionException {
        simulate_method_stack.push(new simulate_method_scope());
        BNGGrammar_BNGGrammar_Actions.simulate_method_return retval = new BNGGrammar_BNGGrammar_Actions.simulate_method_return();
        retval.start = input.LT(1);


          ((simulate_method_scope)simulate_method_stack.peek()).method = "";
          ((simulate_method_scope)simulate_method_stack.peek()).actions = new HashMap<String,String>();

        try {
            // BNGGrammar_Actions.g:85:9: ( ( simulate_ode[$simulate_method::actions] | simulate_ssa[$simulate_method::actions] | write_m_file[$simulate_method::actions] | write_mex_file[$simulate_method::actions] | write_network[$simulate_method::actions] | simulate_pla[$simulate_method::actions] | simulate_nf[$simulate_method::actions] | simulate[$simulate_method::actions] ) -> action(id=$simulate_method::methodoptionMap=$simulate_method::actions))
            // BNGGrammar_Actions.g:85:11: ( simulate_ode[$simulate_method::actions] | simulate_ssa[$simulate_method::actions] | write_m_file[$simulate_method::actions] | write_mex_file[$simulate_method::actions] | write_network[$simulate_method::actions] | simulate_pla[$simulate_method::actions] | simulate_nf[$simulate_method::actions] | simulate[$simulate_method::actions] )
            {
            // BNGGrammar_Actions.g:85:11: ( simulate_ode[$simulate_method::actions] | simulate_ssa[$simulate_method::actions] | write_m_file[$simulate_method::actions] | write_mex_file[$simulate_method::actions] | write_network[$simulate_method::actions] | simulate_pla[$simulate_method::actions] | simulate_nf[$simulate_method::actions] | simulate[$simulate_method::actions] )
            int alt14=8;
            switch ( input.LA(1) ) {
            case SIMULATE_ODE:
                {
                alt14=1;
                }
                break;
            case SIMULATE_SSA:
                {
                alt14=2;
                }
                break;
            case WRITEMFILE:
                {
                alt14=3;
                }
                break;
            case WRITEMEXFILE:
                {
                alt14=4;
                }
                break;
            case WRITENETWORK:
                {
                alt14=5;
                }
                break;
            case SIMULATE_PLA:
                {
                alt14=6;
                }
                break;
            case SIMULATE_NF:
                {
                alt14=7;
                }
                break;
            case SIMULATE:
                {
                alt14=8;
                }
                break;
            default:
                NoViableAltException nvae =
                    new NoViableAltException("", 14, 0, input);

                throw nvae;
            }

            switch (alt14) {
                case 1 :
                    // BNGGrammar_Actions.g:85:12: simulate_ode[$simulate_method::actions]
                    {
                    pushFollow(FOLLOW_simulate_ode_in_simulate_method556);
                    simulate_ode(((simulate_method_scope)simulate_method_stack.peek()).actions);

                    state._fsp--;

                    ((simulate_method_scope)simulate_method_stack.peek()).method = "simulate_ode";

                    }
                    break;
                case 2 :
                    // BNGGrammar_Actions.g:86:11: simulate_ssa[$simulate_method::actions]
                    {
                    pushFollow(FOLLOW_simulate_ssa_in_simulate_method571);
                    simulate_ssa(((simulate_method_scope)simulate_method_stack.peek()).actions);

                    state._fsp--;

                    ((simulate_method_scope)simulate_method_stack.peek()).method = "simulate_ssa";

                    }
                    break;
                case 3 :
                    // BNGGrammar_Actions.g:87:11: write_m_file[$simulate_method::actions]
                    {
                    pushFollow(FOLLOW_write_m_file_in_simulate_method586);
                    write_m_file(((simulate_method_scope)simulate_method_stack.peek()).actions);

                    state._fsp--;

                    ((simulate_method_scope)simulate_method_stack.peek()).method = "writeMfile";

                    }
                    break;
                case 4 :
                    // BNGGrammar_Actions.g:88:11: write_mex_file[$simulate_method::actions]
                    {
                    pushFollow(FOLLOW_write_mex_file_in_simulate_method601);
                    write_mex_file(((simulate_method_scope)simulate_method_stack.peek()).actions);

                    state._fsp--;

                    ((simulate_method_scope)simulate_method_stack.peek()).method = "writeMexfile";

                    }
                    break;
                case 5 :
                    // BNGGrammar_Actions.g:89:11: write_network[$simulate_method::actions]
                    {
                    pushFollow(FOLLOW_write_network_in_simulate_method616);
                    write_network(((simulate_method_scope)simulate_method_stack.peek()).actions);

                    state._fsp--;

                    ((simulate_method_scope)simulate_method_stack.peek()).method = "writeNetwork";

                    }
                    break;
                case 6 :
                    // BNGGrammar_Actions.g:90:11: simulate_pla[$simulate_method::actions]
                    {
                    pushFollow(FOLLOW_simulate_pla_in_simulate_method631);
                    simulate_pla(((simulate_method_scope)simulate_method_stack.peek()).actions);

                    state._fsp--;

                    ((simulate_method_scope)simulate_method_stack.peek()).method = "simulate_pla";

                    }
                    break;
                case 7 :
                    // BNGGrammar_Actions.g:91:11: simulate_nf[$simulate_method::actions]
                    {
                    pushFollow(FOLLOW_simulate_nf_in_simulate_method646);
                    simulate_nf(((simulate_method_scope)simulate_method_stack.peek()).actions);

                    state._fsp--;

                    ((simulate_method_scope)simulate_method_stack.peek()).method = "simulate_nf";

                    }
                    break;
                case 8 :
                    // BNGGrammar_Actions.g:92:11: simulate[$simulate_method::actions]
                    {
                    pushFollow(FOLLOW_simulate_in_simulate_method661);
                    simulate(((simulate_method_scope)simulate_method_stack.peek()).actions);

                    state._fsp--;

                    ((simulate_method_scope)simulate_method_stack.peek()).method = "simulate";

                    }
                    break;

            }



            // TEMPLATE REWRITE
            // 93:9: -> action(id=$simulate_method::methodoptionMap=$simulate_method::actions)
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

    public static class simulate_return extends ParserRuleReturnScope {
        public StringTemplate st;
        public Object getTemplate() { return st; }
        public String toString() { return st==null?null:st.toString(); }
    };

    // $ANTLR start "simulate"
    // BNGGrammar_Actions.g:95:1: simulate[Map<String,String> map] : SIMULATE LPAREN ( LBRACKET ( ( ps_par_def[map] | simulate_par_def[map] | simulate_ode_par_def[map] ) ( COMMA ( ps_par_def[map] | simulate_par_def[map] | simulate_ode_par_def[map] ) )* )? RBRACKET )? RPAREN ( SEMI )? ;
    public final BNGGrammar_BNGGrammar_Actions.simulate_return simulate(Map<String,String> map) throws RecognitionException {
        BNGGrammar_BNGGrammar_Actions.simulate_return retval = new BNGGrammar_BNGGrammar_Actions.simulate_return();
        retval.start = input.LT(1);

        try {
            // BNGGrammar_Actions.g:95:33: ( SIMULATE LPAREN ( LBRACKET ( ( ps_par_def[map] | simulate_par_def[map] | simulate_ode_par_def[map] ) ( COMMA ( ps_par_def[map] | simulate_par_def[map] | simulate_ode_par_def[map] ) )* )? RBRACKET )? RPAREN ( SEMI )? )
            // BNGGrammar_Actions.g:96:9: SIMULATE LPAREN ( LBRACKET ( ( ps_par_def[map] | simulate_par_def[map] | simulate_ode_par_def[map] ) ( COMMA ( ps_par_def[map] | simulate_par_def[map] | simulate_ode_par_def[map] ) )* )? RBRACKET )? RPAREN ( SEMI )?
            {
            match(input,SIMULATE,FOLLOW_SIMULATE_in_simulate710); 
            match(input,LPAREN,FOLLOW_LPAREN_in_simulate712); 
            // BNGGrammar_Actions.g:96:25: ( LBRACKET ( ( ps_par_def[map] | simulate_par_def[map] | simulate_ode_par_def[map] ) ( COMMA ( ps_par_def[map] | simulate_par_def[map] | simulate_ode_par_def[map] ) )* )? RBRACKET )?
            int alt19=2;
            int LA19_0 = input.LA(1);

            if ( (LA19_0==LBRACKET) ) {
                alt19=1;
            }
            switch (alt19) {
                case 1 :
                    // BNGGrammar_Actions.g:96:26: LBRACKET ( ( ps_par_def[map] | simulate_par_def[map] | simulate_ode_par_def[map] ) ( COMMA ( ps_par_def[map] | simulate_par_def[map] | simulate_ode_par_def[map] ) )* )? RBRACKET
                    {
                    match(input,LBRACKET,FOLLOW_LBRACKET_in_simulate715); 
                    // BNGGrammar_Actions.g:97:9: ( ( ps_par_def[map] | simulate_par_def[map] | simulate_ode_par_def[map] ) ( COMMA ( ps_par_def[map] | simulate_par_def[map] | simulate_ode_par_def[map] ) )* )?
                    int alt18=2;
                    int LA18_0 = input.LA(1);

                    if ( (LA18_0==OVERWRITE||(LA18_0>=PREFFIX && LA18_0<=SUFFIX)||LA18_0==VERBOSE||(LA18_0>=ATOL && LA18_0<=MAX_SIM_STEPS)||LA18_0==SAMPLE_TIMES||(LA18_0>=PRINT_CDAT && LA18_0<=PRINT_FUNCTIONS)||LA18_0==METHOD||(LA18_0>=ARGFILE && LA18_0<=PRINT_END)||LA18_0==CONTINUE||LA18_0==EVALUATE_EXPRESSIONS) ) {
                        alt18=1;
                    }
                    switch (alt18) {
                        case 1 :
                            // BNGGrammar_Actions.g:97:10: ( ps_par_def[map] | simulate_par_def[map] | simulate_ode_par_def[map] ) ( COMMA ( ps_par_def[map] | simulate_par_def[map] | simulate_ode_par_def[map] ) )*
                            {
                            // BNGGrammar_Actions.g:97:10: ( ps_par_def[map] | simulate_par_def[map] | simulate_ode_par_def[map] )
                            int alt15=3;
                            switch ( input.LA(1) ) {
                            case PREFFIX:
                            case SUFFIX:
                                {
                                alt15=1;
                                }
                                break;
                            case OVERWRITE:
                            case VERBOSE:
                            case T_END:
                            case T_START:
                            case N_STEPS:
                            case N_OUTPUT_STEPS:
                            case MAX_SIM_STEPS:
                            case SAMPLE_TIMES:
                            case PRINT_CDAT:
                            case PRINT_FUNCTIONS:
                            case METHOD:
                            case ARGFILE:
                            case SAVE_PROGRESS:
                            case NETFILE:
                            case PRINT_NET:
                            case PRINT_END:
                            case CONTINUE:
                            case EVALUATE_EXPRESSIONS:
                                {
                                alt15=2;
                                }
                                break;
                            case ATOL:
                            case RTOL:
                            case STEADY_STATE:
                            case SPARSE:
                                {
                                alt15=3;
                                }
                                break;
                            default:
                                NoViableAltException nvae =
                                    new NoViableAltException("", 15, 0, input);

                                throw nvae;
                            }

                            switch (alt15) {
                                case 1 :
                                    // BNGGrammar_Actions.g:97:11: ps_par_def[map]
                                    {
                                    pushFollow(FOLLOW_ps_par_def_in_simulate728);
                                    ps_par_def(map);

                                    state._fsp--;


                                    }
                                    break;
                                case 2 :
                                    // BNGGrammar_Actions.g:97:27: simulate_par_def[map]
                                    {
                                    pushFollow(FOLLOW_simulate_par_def_in_simulate731);
                                    simulate_par_def(map);

                                    state._fsp--;


                                    }
                                    break;
                                case 3 :
                                    // BNGGrammar_Actions.g:97:49: simulate_ode_par_def[map]
                                    {
                                    pushFollow(FOLLOW_simulate_ode_par_def_in_simulate734);
                                    simulate_ode_par_def(map);

                                    state._fsp--;


                                    }
                                    break;

                            }

                            // BNGGrammar_Actions.g:98:11: ( COMMA ( ps_par_def[map] | simulate_par_def[map] | simulate_ode_par_def[map] ) )*
                            loop17:
                            do {
                                int alt17=2;
                                int LA17_0 = input.LA(1);

                                if ( (LA17_0==COMMA) ) {
                                    alt17=1;
                                }


                                switch (alt17) {
                            	case 1 :
                            	    // BNGGrammar_Actions.g:98:12: COMMA ( ps_par_def[map] | simulate_par_def[map] | simulate_ode_par_def[map] )
                            	    {
                            	    match(input,COMMA,FOLLOW_COMMA_in_simulate749); 
                            	    // BNGGrammar_Actions.g:98:18: ( ps_par_def[map] | simulate_par_def[map] | simulate_ode_par_def[map] )
                            	    int alt16=3;
                            	    switch ( input.LA(1) ) {
                            	    case PREFFIX:
                            	    case SUFFIX:
                            	        {
                            	        alt16=1;
                            	        }
                            	        break;
                            	    case OVERWRITE:
                            	    case VERBOSE:
                            	    case T_END:
                            	    case T_START:
                            	    case N_STEPS:
                            	    case N_OUTPUT_STEPS:
                            	    case MAX_SIM_STEPS:
                            	    case SAMPLE_TIMES:
                            	    case PRINT_CDAT:
                            	    case PRINT_FUNCTIONS:
                            	    case METHOD:
                            	    case ARGFILE:
                            	    case SAVE_PROGRESS:
                            	    case NETFILE:
                            	    case PRINT_NET:
                            	    case PRINT_END:
                            	    case CONTINUE:
                            	    case EVALUATE_EXPRESSIONS:
                            	        {
                            	        alt16=2;
                            	        }
                            	        break;
                            	    case ATOL:
                            	    case RTOL:
                            	    case STEADY_STATE:
                            	    case SPARSE:
                            	        {
                            	        alt16=3;
                            	        }
                            	        break;
                            	    default:
                            	        NoViableAltException nvae =
                            	            new NoViableAltException("", 16, 0, input);

                            	        throw nvae;
                            	    }

                            	    switch (alt16) {
                            	        case 1 :
                            	            // BNGGrammar_Actions.g:98:19: ps_par_def[map]
                            	            {
                            	            pushFollow(FOLLOW_ps_par_def_in_simulate752);
                            	            ps_par_def(map);

                            	            state._fsp--;


                            	            }
                            	            break;
                            	        case 2 :
                            	            // BNGGrammar_Actions.g:98:35: simulate_par_def[map]
                            	            {
                            	            pushFollow(FOLLOW_simulate_par_def_in_simulate755);
                            	            simulate_par_def(map);

                            	            state._fsp--;


                            	            }
                            	            break;
                            	        case 3 :
                            	            // BNGGrammar_Actions.g:98:57: simulate_ode_par_def[map]
                            	            {
                            	            pushFollow(FOLLOW_simulate_ode_par_def_in_simulate758);
                            	            simulate_ode_par_def(map);

                            	            state._fsp--;


                            	            }
                            	            break;

                            	    }


                            	    }
                            	    break;

                            	default :
                            	    break loop17;
                                }
                            } while (true);


                            }
                            break;

                    }

                    match(input,RBRACKET,FOLLOW_RBRACKET_in_simulate775); 

                    }
                    break;

            }

            match(input,RPAREN,FOLLOW_RPAREN_in_simulate779); 
            // BNGGrammar_Actions.g:99:27: ( SEMI )?
            int alt20=2;
            int LA20_0 = input.LA(1);

            if ( (LA20_0==SEMI) ) {
                alt20=1;
            }
            switch (alt20) {
                case 1 :
                    // BNGGrammar_Actions.g:99:27: SEMI
                    {
                    match(input,SEMI,FOLLOW_SEMI_in_simulate781); 

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
    // $ANTLR end "simulate"

    public static class simulate_ode_return extends ParserRuleReturnScope {
        public StringTemplate st;
        public Object getTemplate() { return st; }
        public String toString() { return st==null?null:st.toString(); }
    };

    // $ANTLR start "simulate_ode"
    // BNGGrammar_Actions.g:101:1: simulate_ode[Map<String,String> map] : SIMULATE_ODE LPAREN ( LBRACKET ( ( ps_par_def[map] | simulate_par_def[map] | simulate_ode_par_def[map] ) ( COMMA ( ps_par_def[map] | simulate_par_def[map] | simulate_ode_par_def[map] ) )* )? RBRACKET )? RPAREN ( SEMI )? ;
    public final BNGGrammar_BNGGrammar_Actions.simulate_ode_return simulate_ode(Map<String,String> map) throws RecognitionException {
        BNGGrammar_BNGGrammar_Actions.simulate_ode_return retval = new BNGGrammar_BNGGrammar_Actions.simulate_ode_return();
        retval.start = input.LT(1);

        try {
            // BNGGrammar_Actions.g:102:9: ( SIMULATE_ODE LPAREN ( LBRACKET ( ( ps_par_def[map] | simulate_par_def[map] | simulate_ode_par_def[map] ) ( COMMA ( ps_par_def[map] | simulate_par_def[map] | simulate_ode_par_def[map] ) )* )? RBRACKET )? RPAREN ( SEMI )? )
            // BNGGrammar_Actions.g:102:11: SIMULATE_ODE LPAREN ( LBRACKET ( ( ps_par_def[map] | simulate_par_def[map] | simulate_ode_par_def[map] ) ( COMMA ( ps_par_def[map] | simulate_par_def[map] | simulate_ode_par_def[map] ) )* )? RBRACKET )? RPAREN ( SEMI )?
            {
            match(input,SIMULATE_ODE,FOLLOW_SIMULATE_ODE_in_simulate_ode807); 
            match(input,LPAREN,FOLLOW_LPAREN_in_simulate_ode809); 
            // BNGGrammar_Actions.g:102:31: ( LBRACKET ( ( ps_par_def[map] | simulate_par_def[map] | simulate_ode_par_def[map] ) ( COMMA ( ps_par_def[map] | simulate_par_def[map] | simulate_ode_par_def[map] ) )* )? RBRACKET )?
            int alt25=2;
            int LA25_0 = input.LA(1);

            if ( (LA25_0==LBRACKET) ) {
                alt25=1;
            }
            switch (alt25) {
                case 1 :
                    // BNGGrammar_Actions.g:102:32: LBRACKET ( ( ps_par_def[map] | simulate_par_def[map] | simulate_ode_par_def[map] ) ( COMMA ( ps_par_def[map] | simulate_par_def[map] | simulate_ode_par_def[map] ) )* )? RBRACKET
                    {
                    match(input,LBRACKET,FOLLOW_LBRACKET_in_simulate_ode812); 
                    // BNGGrammar_Actions.g:103:10: ( ( ps_par_def[map] | simulate_par_def[map] | simulate_ode_par_def[map] ) ( COMMA ( ps_par_def[map] | simulate_par_def[map] | simulate_ode_par_def[map] ) )* )?
                    int alt24=2;
                    int LA24_0 = input.LA(1);

                    if ( (LA24_0==OVERWRITE||(LA24_0>=PREFFIX && LA24_0<=SUFFIX)||LA24_0==VERBOSE||(LA24_0>=ATOL && LA24_0<=MAX_SIM_STEPS)||LA24_0==SAMPLE_TIMES||(LA24_0>=PRINT_CDAT && LA24_0<=PRINT_FUNCTIONS)||LA24_0==METHOD||(LA24_0>=ARGFILE && LA24_0<=PRINT_END)||LA24_0==CONTINUE||LA24_0==EVALUATE_EXPRESSIONS) ) {
                        alt24=1;
                    }
                    switch (alt24) {
                        case 1 :
                            // BNGGrammar_Actions.g:103:11: ( ps_par_def[map] | simulate_par_def[map] | simulate_ode_par_def[map] ) ( COMMA ( ps_par_def[map] | simulate_par_def[map] | simulate_ode_par_def[map] ) )*
                            {
                            // BNGGrammar_Actions.g:103:11: ( ps_par_def[map] | simulate_par_def[map] | simulate_ode_par_def[map] )
                            int alt21=3;
                            switch ( input.LA(1) ) {
                            case PREFFIX:
                            case SUFFIX:
                                {
                                alt21=1;
                                }
                                break;
                            case OVERWRITE:
                            case VERBOSE:
                            case T_END:
                            case T_START:
                            case N_STEPS:
                            case N_OUTPUT_STEPS:
                            case MAX_SIM_STEPS:
                            case SAMPLE_TIMES:
                            case PRINT_CDAT:
                            case PRINT_FUNCTIONS:
                            case METHOD:
                            case ARGFILE:
                            case SAVE_PROGRESS:
                            case NETFILE:
                            case PRINT_NET:
                            case PRINT_END:
                            case CONTINUE:
                            case EVALUATE_EXPRESSIONS:
                                {
                                alt21=2;
                                }
                                break;
                            case ATOL:
                            case RTOL:
                            case STEADY_STATE:
                            case SPARSE:
                                {
                                alt21=3;
                                }
                                break;
                            default:
                                NoViableAltException nvae =
                                    new NoViableAltException("", 21, 0, input);

                                throw nvae;
                            }

                            switch (alt21) {
                                case 1 :
                                    // BNGGrammar_Actions.g:103:12: ps_par_def[map]
                                    {
                                    pushFollow(FOLLOW_ps_par_def_in_simulate_ode825);
                                    ps_par_def(map);

                                    state._fsp--;


                                    }
                                    break;
                                case 2 :
                                    // BNGGrammar_Actions.g:103:28: simulate_par_def[map]
                                    {
                                    pushFollow(FOLLOW_simulate_par_def_in_simulate_ode828);
                                    simulate_par_def(map);

                                    state._fsp--;


                                    }
                                    break;
                                case 3 :
                                    // BNGGrammar_Actions.g:103:50: simulate_ode_par_def[map]
                                    {
                                    pushFollow(FOLLOW_simulate_ode_par_def_in_simulate_ode831);
                                    simulate_ode_par_def(map);

                                    state._fsp--;


                                    }
                                    break;

                            }

                            // BNGGrammar_Actions.g:104:11: ( COMMA ( ps_par_def[map] | simulate_par_def[map] | simulate_ode_par_def[map] ) )*
                            loop23:
                            do {
                                int alt23=2;
                                int LA23_0 = input.LA(1);

                                if ( (LA23_0==COMMA) ) {
                                    alt23=1;
                                }


                                switch (alt23) {
                            	case 1 :
                            	    // BNGGrammar_Actions.g:104:12: COMMA ( ps_par_def[map] | simulate_par_def[map] | simulate_ode_par_def[map] )
                            	    {
                            	    match(input,COMMA,FOLLOW_COMMA_in_simulate_ode846); 
                            	    // BNGGrammar_Actions.g:104:18: ( ps_par_def[map] | simulate_par_def[map] | simulate_ode_par_def[map] )
                            	    int alt22=3;
                            	    switch ( input.LA(1) ) {
                            	    case PREFFIX:
                            	    case SUFFIX:
                            	        {
                            	        alt22=1;
                            	        }
                            	        break;
                            	    case OVERWRITE:
                            	    case VERBOSE:
                            	    case T_END:
                            	    case T_START:
                            	    case N_STEPS:
                            	    case N_OUTPUT_STEPS:
                            	    case MAX_SIM_STEPS:
                            	    case SAMPLE_TIMES:
                            	    case PRINT_CDAT:
                            	    case PRINT_FUNCTIONS:
                            	    case METHOD:
                            	    case ARGFILE:
                            	    case SAVE_PROGRESS:
                            	    case NETFILE:
                            	    case PRINT_NET:
                            	    case PRINT_END:
                            	    case CONTINUE:
                            	    case EVALUATE_EXPRESSIONS:
                            	        {
                            	        alt22=2;
                            	        }
                            	        break;
                            	    case ATOL:
                            	    case RTOL:
                            	    case STEADY_STATE:
                            	    case SPARSE:
                            	        {
                            	        alt22=3;
                            	        }
                            	        break;
                            	    default:
                            	        NoViableAltException nvae =
                            	            new NoViableAltException("", 22, 0, input);

                            	        throw nvae;
                            	    }

                            	    switch (alt22) {
                            	        case 1 :
                            	            // BNGGrammar_Actions.g:104:19: ps_par_def[map]
                            	            {
                            	            pushFollow(FOLLOW_ps_par_def_in_simulate_ode849);
                            	            ps_par_def(map);

                            	            state._fsp--;


                            	            }
                            	            break;
                            	        case 2 :
                            	            // BNGGrammar_Actions.g:104:35: simulate_par_def[map]
                            	            {
                            	            pushFollow(FOLLOW_simulate_par_def_in_simulate_ode852);
                            	            simulate_par_def(map);

                            	            state._fsp--;


                            	            }
                            	            break;
                            	        case 3 :
                            	            // BNGGrammar_Actions.g:104:57: simulate_ode_par_def[map]
                            	            {
                            	            pushFollow(FOLLOW_simulate_ode_par_def_in_simulate_ode855);
                            	            simulate_ode_par_def(map);

                            	            state._fsp--;


                            	            }
                            	            break;

                            	    }


                            	    }
                            	    break;

                            	default :
                            	    break loop23;
                                }
                            } while (true);


                            }
                            break;

                    }

                    match(input,RBRACKET,FOLLOW_RBRACKET_in_simulate_ode874); 

                    }
                    break;

            }

            match(input,RPAREN,FOLLOW_RPAREN_in_simulate_ode878); 
            // BNGGrammar_Actions.g:105:29: ( SEMI )?
            int alt26=2;
            int LA26_0 = input.LA(1);

            if ( (LA26_0==SEMI) ) {
                alt26=1;
            }
            switch (alt26) {
                case 1 :
                    // BNGGrammar_Actions.g:105:29: SEMI
                    {
                    match(input,SEMI,FOLLOW_SEMI_in_simulate_ode880); 

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
    // $ANTLR end "simulate_ode"

    public static class simulate_ssa_return extends ParserRuleReturnScope {
        public StringTemplate st;
        public Object getTemplate() { return st; }
        public String toString() { return st==null?null:st.toString(); }
    };

    // $ANTLR start "simulate_ssa"
    // BNGGrammar_Actions.g:107:1: simulate_ssa[Map<String,String> map] : SIMULATE_SSA LPAREN ( LBRACKET ( ( ps_par_def[map] | simulate_par_def[map] ) ( COMMA ( ps_par_def[map] | simulate_par_def[map] ) )* )? RBRACKET )? RPAREN ( SEMI )? ;
    public final BNGGrammar_BNGGrammar_Actions.simulate_ssa_return simulate_ssa(Map<String,String> map) throws RecognitionException {
        BNGGrammar_BNGGrammar_Actions.simulate_ssa_return retval = new BNGGrammar_BNGGrammar_Actions.simulate_ssa_return();
        retval.start = input.LT(1);

        try {
            // BNGGrammar_Actions.g:108:9: ( SIMULATE_SSA LPAREN ( LBRACKET ( ( ps_par_def[map] | simulate_par_def[map] ) ( COMMA ( ps_par_def[map] | simulate_par_def[map] ) )* )? RBRACKET )? RPAREN ( SEMI )? )
            // BNGGrammar_Actions.g:108:11: SIMULATE_SSA LPAREN ( LBRACKET ( ( ps_par_def[map] | simulate_par_def[map] ) ( COMMA ( ps_par_def[map] | simulate_par_def[map] ) )* )? RBRACKET )? RPAREN ( SEMI )?
            {
            match(input,SIMULATE_SSA,FOLLOW_SIMULATE_SSA_in_simulate_ssa907); 
            match(input,LPAREN,FOLLOW_LPAREN_in_simulate_ssa909); 
            // BNGGrammar_Actions.g:108:31: ( LBRACKET ( ( ps_par_def[map] | simulate_par_def[map] ) ( COMMA ( ps_par_def[map] | simulate_par_def[map] ) )* )? RBRACKET )?
            int alt31=2;
            int LA31_0 = input.LA(1);

            if ( (LA31_0==LBRACKET) ) {
                alt31=1;
            }
            switch (alt31) {
                case 1 :
                    // BNGGrammar_Actions.g:108:32: LBRACKET ( ( ps_par_def[map] | simulate_par_def[map] ) ( COMMA ( ps_par_def[map] | simulate_par_def[map] ) )* )? RBRACKET
                    {
                    match(input,LBRACKET,FOLLOW_LBRACKET_in_simulate_ssa912); 
                    // BNGGrammar_Actions.g:109:11: ( ( ps_par_def[map] | simulate_par_def[map] ) ( COMMA ( ps_par_def[map] | simulate_par_def[map] ) )* )?
                    int alt30=2;
                    int LA30_0 = input.LA(1);

                    if ( (LA30_0==OVERWRITE||(LA30_0>=PREFFIX && LA30_0<=SUFFIX)||LA30_0==VERBOSE||(LA30_0>=T_END && LA30_0<=MAX_SIM_STEPS)||LA30_0==SAMPLE_TIMES||(LA30_0>=PRINT_CDAT && LA30_0<=PRINT_FUNCTIONS)||LA30_0==METHOD||(LA30_0>=ARGFILE && LA30_0<=PRINT_END)||LA30_0==CONTINUE||LA30_0==EVALUATE_EXPRESSIONS) ) {
                        alt30=1;
                    }
                    switch (alt30) {
                        case 1 :
                            // BNGGrammar_Actions.g:109:12: ( ps_par_def[map] | simulate_par_def[map] ) ( COMMA ( ps_par_def[map] | simulate_par_def[map] ) )*
                            {
                            // BNGGrammar_Actions.g:109:12: ( ps_par_def[map] | simulate_par_def[map] )
                            int alt27=2;
                            int LA27_0 = input.LA(1);

                            if ( ((LA27_0>=PREFFIX && LA27_0<=SUFFIX)) ) {
                                alt27=1;
                            }
                            else if ( (LA27_0==OVERWRITE||LA27_0==VERBOSE||(LA27_0>=T_END && LA27_0<=MAX_SIM_STEPS)||LA27_0==SAMPLE_TIMES||(LA27_0>=PRINT_CDAT && LA27_0<=PRINT_FUNCTIONS)||LA27_0==METHOD||(LA27_0>=ARGFILE && LA27_0<=PRINT_END)||LA27_0==CONTINUE||LA27_0==EVALUATE_EXPRESSIONS) ) {
                                alt27=2;
                            }
                            else {
                                NoViableAltException nvae =
                                    new NoViableAltException("", 27, 0, input);

                                throw nvae;
                            }
                            switch (alt27) {
                                case 1 :
                                    // BNGGrammar_Actions.g:109:13: ps_par_def[map]
                                    {
                                    pushFollow(FOLLOW_ps_par_def_in_simulate_ssa926);
                                    ps_par_def(map);

                                    state._fsp--;


                                    }
                                    break;
                                case 2 :
                                    // BNGGrammar_Actions.g:109:29: simulate_par_def[map]
                                    {
                                    pushFollow(FOLLOW_simulate_par_def_in_simulate_ssa929);
                                    simulate_par_def(map);

                                    state._fsp--;


                                    }
                                    break;

                            }

                            // BNGGrammar_Actions.g:109:52: ( COMMA ( ps_par_def[map] | simulate_par_def[map] ) )*
                            loop29:
                            do {
                                int alt29=2;
                                int LA29_0 = input.LA(1);

                                if ( (LA29_0==COMMA) ) {
                                    alt29=1;
                                }


                                switch (alt29) {
                            	case 1 :
                            	    // BNGGrammar_Actions.g:109:53: COMMA ( ps_par_def[map] | simulate_par_def[map] )
                            	    {
                            	    match(input,COMMA,FOLLOW_COMMA_in_simulate_ssa934); 
                            	    // BNGGrammar_Actions.g:109:59: ( ps_par_def[map] | simulate_par_def[map] )
                            	    int alt28=2;
                            	    int LA28_0 = input.LA(1);

                            	    if ( ((LA28_0>=PREFFIX && LA28_0<=SUFFIX)) ) {
                            	        alt28=1;
                            	    }
                            	    else if ( (LA28_0==OVERWRITE||LA28_0==VERBOSE||(LA28_0>=T_END && LA28_0<=MAX_SIM_STEPS)||LA28_0==SAMPLE_TIMES||(LA28_0>=PRINT_CDAT && LA28_0<=PRINT_FUNCTIONS)||LA28_0==METHOD||(LA28_0>=ARGFILE && LA28_0<=PRINT_END)||LA28_0==CONTINUE||LA28_0==EVALUATE_EXPRESSIONS) ) {
                            	        alt28=2;
                            	    }
                            	    else {
                            	        NoViableAltException nvae =
                            	            new NoViableAltException("", 28, 0, input);

                            	        throw nvae;
                            	    }
                            	    switch (alt28) {
                            	        case 1 :
                            	            // BNGGrammar_Actions.g:109:60: ps_par_def[map]
                            	            {
                            	            pushFollow(FOLLOW_ps_par_def_in_simulate_ssa937);
                            	            ps_par_def(map);

                            	            state._fsp--;


                            	            }
                            	            break;
                            	        case 2 :
                            	            // BNGGrammar_Actions.g:109:76: simulate_par_def[map]
                            	            {
                            	            pushFollow(FOLLOW_simulate_par_def_in_simulate_ssa940);
                            	            simulate_par_def(map);

                            	            state._fsp--;


                            	            }
                            	            break;

                            	    }


                            	    }
                            	    break;

                            	default :
                            	    break loop29;
                                }
                            } while (true);


                            }
                            break;

                    }

                    match(input,RBRACKET,FOLLOW_RBRACKET_in_simulate_ssa958); 

                    }
                    break;

            }

            match(input,RPAREN,FOLLOW_RPAREN_in_simulate_ssa962); 
            // BNGGrammar_Actions.g:110:29: ( SEMI )?
            int alt32=2;
            int LA32_0 = input.LA(1);

            if ( (LA32_0==SEMI) ) {
                alt32=1;
            }
            switch (alt32) {
                case 1 :
                    // BNGGrammar_Actions.g:110:29: SEMI
                    {
                    match(input,SEMI,FOLLOW_SEMI_in_simulate_ssa964); 

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
    // $ANTLR end "simulate_ssa"

    public static class simulate_nf_return extends ParserRuleReturnScope {
        public StringTemplate st;
        public Object getTemplate() { return st; }
        public String toString() { return st==null?null:st.toString(); }
    };

    // $ANTLR start "simulate_nf"
    // BNGGrammar_Actions.g:112:1: simulate_nf[Map<String,String> map] : SIMULATE_NF LPAREN ( LBRACKET ( ( ps_par_def[map] | simulate_par_def[map] | simulate_nf_par_def[map] ) ( COMMA ( ps_par_def[map] | simulate_par_def[map] | simulate_nf_par_def[map] ) )* )? RBRACKET )? RPAREN ( SEMI )? ;
    public final BNGGrammar_BNGGrammar_Actions.simulate_nf_return simulate_nf(Map<String,String> map) throws RecognitionException {
        BNGGrammar_BNGGrammar_Actions.simulate_nf_return retval = new BNGGrammar_BNGGrammar_Actions.simulate_nf_return();
        retval.start = input.LT(1);

        try {
            // BNGGrammar_Actions.g:113:9: ( SIMULATE_NF LPAREN ( LBRACKET ( ( ps_par_def[map] | simulate_par_def[map] | simulate_nf_par_def[map] ) ( COMMA ( ps_par_def[map] | simulate_par_def[map] | simulate_nf_par_def[map] ) )* )? RBRACKET )? RPAREN ( SEMI )? )
            // BNGGrammar_Actions.g:113:11: SIMULATE_NF LPAREN ( LBRACKET ( ( ps_par_def[map] | simulate_par_def[map] | simulate_nf_par_def[map] ) ( COMMA ( ps_par_def[map] | simulate_par_def[map] | simulate_nf_par_def[map] ) )* )? RBRACKET )? RPAREN ( SEMI )?
            {
            match(input,SIMULATE_NF,FOLLOW_SIMULATE_NF_in_simulate_nf983); 
            match(input,LPAREN,FOLLOW_LPAREN_in_simulate_nf985); 
            // BNGGrammar_Actions.g:113:30: ( LBRACKET ( ( ps_par_def[map] | simulate_par_def[map] | simulate_nf_par_def[map] ) ( COMMA ( ps_par_def[map] | simulate_par_def[map] | simulate_nf_par_def[map] ) )* )? RBRACKET )?
            int alt37=2;
            int LA37_0 = input.LA(1);

            if ( (LA37_0==LBRACKET) ) {
                alt37=1;
            }
            switch (alt37) {
                case 1 :
                    // BNGGrammar_Actions.g:113:31: LBRACKET ( ( ps_par_def[map] | simulate_par_def[map] | simulate_nf_par_def[map] ) ( COMMA ( ps_par_def[map] | simulate_par_def[map] | simulate_nf_par_def[map] ) )* )? RBRACKET
                    {
                    match(input,LBRACKET,FOLLOW_LBRACKET_in_simulate_nf988); 
                    // BNGGrammar_Actions.g:114:10: ( ( ps_par_def[map] | simulate_par_def[map] | simulate_nf_par_def[map] ) ( COMMA ( ps_par_def[map] | simulate_par_def[map] | simulate_nf_par_def[map] ) )* )?
                    int alt36=2;
                    int LA36_0 = input.LA(1);

                    if ( (LA36_0==OVERWRITE||(LA36_0>=PREFFIX && LA36_0<=SUFFIX)||(LA36_0>=VERBOSE && LA36_0<=GML)||(LA36_0>=T_END && LA36_0<=MAX_SIM_STEPS)||LA36_0==SAMPLE_TIMES||(LA36_0>=PRINT_CDAT && LA36_0<=PRINT_FUNCTIONS)||LA36_0==METHOD||(LA36_0>=PARAM && LA36_0<=GET_FINAL_STATE)||(LA36_0>=ARGFILE && LA36_0<=PRINT_END)||LA36_0==CONTINUE||LA36_0==EVALUATE_EXPRESSIONS) ) {
                        alt36=1;
                    }
                    switch (alt36) {
                        case 1 :
                            // BNGGrammar_Actions.g:114:11: ( ps_par_def[map] | simulate_par_def[map] | simulate_nf_par_def[map] ) ( COMMA ( ps_par_def[map] | simulate_par_def[map] | simulate_nf_par_def[map] ) )*
                            {
                            // BNGGrammar_Actions.g:114:11: ( ps_par_def[map] | simulate_par_def[map] | simulate_nf_par_def[map] )
                            int alt33=3;
                            switch ( input.LA(1) ) {
                            case PREFFIX:
                            case SUFFIX:
                                {
                                alt33=1;
                                }
                                break;
                            case OVERWRITE:
                            case VERBOSE:
                            case T_END:
                            case T_START:
                            case N_STEPS:
                            case N_OUTPUT_STEPS:
                            case MAX_SIM_STEPS:
                            case SAMPLE_TIMES:
                            case PRINT_CDAT:
                            case PRINT_FUNCTIONS:
                            case METHOD:
                            case ARGFILE:
                            case SAVE_PROGRESS:
                            case NETFILE:
                            case PRINT_NET:
                            case PRINT_END:
                            case CONTINUE:
                            case EVALUATE_EXPRESSIONS:
                                {
                                alt33=2;
                                }
                                break;
                            case GML:
                            case PARAM:
                            case COMPLEX:
                            case GET_FINAL_STATE:
                                {
                                alt33=3;
                                }
                                break;
                            default:
                                NoViableAltException nvae =
                                    new NoViableAltException("", 33, 0, input);

                                throw nvae;
                            }

                            switch (alt33) {
                                case 1 :
                                    // BNGGrammar_Actions.g:114:12: ps_par_def[map]
                                    {
                                    pushFollow(FOLLOW_ps_par_def_in_simulate_nf1001);
                                    ps_par_def(map);

                                    state._fsp--;


                                    }
                                    break;
                                case 2 :
                                    // BNGGrammar_Actions.g:114:28: simulate_par_def[map]
                                    {
                                    pushFollow(FOLLOW_simulate_par_def_in_simulate_nf1004);
                                    simulate_par_def(map);

                                    state._fsp--;


                                    }
                                    break;
                                case 3 :
                                    // BNGGrammar_Actions.g:114:50: simulate_nf_par_def[map]
                                    {
                                    pushFollow(FOLLOW_simulate_nf_par_def_in_simulate_nf1007);
                                    simulate_nf_par_def(map);

                                    state._fsp--;


                                    }
                                    break;

                            }

                            // BNGGrammar_Actions.g:115:11: ( COMMA ( ps_par_def[map] | simulate_par_def[map] | simulate_nf_par_def[map] ) )*
                            loop35:
                            do {
                                int alt35=2;
                                int LA35_0 = input.LA(1);

                                if ( (LA35_0==COMMA) ) {
                                    alt35=1;
                                }


                                switch (alt35) {
                            	case 1 :
                            	    // BNGGrammar_Actions.g:115:12: COMMA ( ps_par_def[map] | simulate_par_def[map] | simulate_nf_par_def[map] )
                            	    {
                            	    match(input,COMMA,FOLLOW_COMMA_in_simulate_nf1022); 
                            	    // BNGGrammar_Actions.g:115:18: ( ps_par_def[map] | simulate_par_def[map] | simulate_nf_par_def[map] )
                            	    int alt34=3;
                            	    switch ( input.LA(1) ) {
                            	    case PREFFIX:
                            	    case SUFFIX:
                            	        {
                            	        alt34=1;
                            	        }
                            	        break;
                            	    case OVERWRITE:
                            	    case VERBOSE:
                            	    case T_END:
                            	    case T_START:
                            	    case N_STEPS:
                            	    case N_OUTPUT_STEPS:
                            	    case MAX_SIM_STEPS:
                            	    case SAMPLE_TIMES:
                            	    case PRINT_CDAT:
                            	    case PRINT_FUNCTIONS:
                            	    case METHOD:
                            	    case ARGFILE:
                            	    case SAVE_PROGRESS:
                            	    case NETFILE:
                            	    case PRINT_NET:
                            	    case PRINT_END:
                            	    case CONTINUE:
                            	    case EVALUATE_EXPRESSIONS:
                            	        {
                            	        alt34=2;
                            	        }
                            	        break;
                            	    case GML:
                            	    case PARAM:
                            	    case COMPLEX:
                            	    case GET_FINAL_STATE:
                            	        {
                            	        alt34=3;
                            	        }
                            	        break;
                            	    default:
                            	        NoViableAltException nvae =
                            	            new NoViableAltException("", 34, 0, input);

                            	        throw nvae;
                            	    }

                            	    switch (alt34) {
                            	        case 1 :
                            	            // BNGGrammar_Actions.g:115:19: ps_par_def[map]
                            	            {
                            	            pushFollow(FOLLOW_ps_par_def_in_simulate_nf1025);
                            	            ps_par_def(map);

                            	            state._fsp--;


                            	            }
                            	            break;
                            	        case 2 :
                            	            // BNGGrammar_Actions.g:115:35: simulate_par_def[map]
                            	            {
                            	            pushFollow(FOLLOW_simulate_par_def_in_simulate_nf1028);
                            	            simulate_par_def(map);

                            	            state._fsp--;


                            	            }
                            	            break;
                            	        case 3 :
                            	            // BNGGrammar_Actions.g:115:57: simulate_nf_par_def[map]
                            	            {
                            	            pushFollow(FOLLOW_simulate_nf_par_def_in_simulate_nf1031);
                            	            simulate_nf_par_def(map);

                            	            state._fsp--;


                            	            }
                            	            break;

                            	    }


                            	    }
                            	    break;

                            	default :
                            	    break loop35;
                                }
                            } while (true);


                            }
                            break;

                    }

                    match(input,RBRACKET,FOLLOW_RBRACKET_in_simulate_nf1050); 

                    }
                    break;

            }

            match(input,RPAREN,FOLLOW_RPAREN_in_simulate_nf1054); 
            // BNGGrammar_Actions.g:116:29: ( SEMI )?
            int alt38=2;
            int LA38_0 = input.LA(1);

            if ( (LA38_0==SEMI) ) {
                alt38=1;
            }
            switch (alt38) {
                case 1 :
                    // BNGGrammar_Actions.g:116:29: SEMI
                    {
                    match(input,SEMI,FOLLOW_SEMI_in_simulate_nf1056); 

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
    // $ANTLR end "simulate_nf"

    public static class write_m_file_return extends ParserRuleReturnScope {
        public StringTemplate st;
        public Object getTemplate() { return st; }
        public String toString() { return st==null?null:st.toString(); }
    };

    // $ANTLR start "write_m_file"
    // BNGGrammar_Actions.g:119:1: write_m_file[Map<String,String> map] : WRITEMFILE LPAREN ( LBRACKET ( ( write_m_par_def[map] ) ( COMMA write_m_par_def[map] )* )? RBRACKET )? RPAREN ( SEMI )? ;
    public final BNGGrammar_BNGGrammar_Actions.write_m_file_return write_m_file(Map<String,String> map) throws RecognitionException {
        BNGGrammar_BNGGrammar_Actions.write_m_file_return retval = new BNGGrammar_BNGGrammar_Actions.write_m_file_return();
        retval.start = input.LT(1);

        try {
            // BNGGrammar_Actions.g:120:9: ( WRITEMFILE LPAREN ( LBRACKET ( ( write_m_par_def[map] ) ( COMMA write_m_par_def[map] )* )? RBRACKET )? RPAREN ( SEMI )? )
            // BNGGrammar_Actions.g:120:11: WRITEMFILE LPAREN ( LBRACKET ( ( write_m_par_def[map] ) ( COMMA write_m_par_def[map] )* )? RBRACKET )? RPAREN ( SEMI )?
            {
            match(input,WRITEMFILE,FOLLOW_WRITEMFILE_in_write_m_file1084); 
            match(input,LPAREN,FOLLOW_LPAREN_in_write_m_file1086); 
            // BNGGrammar_Actions.g:120:29: ( LBRACKET ( ( write_m_par_def[map] ) ( COMMA write_m_par_def[map] )* )? RBRACKET )?
            int alt41=2;
            int LA41_0 = input.LA(1);

            if ( (LA41_0==LBRACKET) ) {
                alt41=1;
            }
            switch (alt41) {
                case 1 :
                    // BNGGrammar_Actions.g:120:30: LBRACKET ( ( write_m_par_def[map] ) ( COMMA write_m_par_def[map] )* )? RBRACKET
                    {
                    match(input,LBRACKET,FOLLOW_LBRACKET_in_write_m_file1089); 
                    // BNGGrammar_Actions.g:121:9: ( ( write_m_par_def[map] ) ( COMMA write_m_par_def[map] )* )?
                    int alt40=2;
                    int LA40_0 = input.LA(1);

                    if ( ((LA40_0>=ATOL && LA40_0<=RTOL)||(LA40_0>=SPARSE && LA40_0<=N_STEPS)||LA40_0==BDF) ) {
                        alt40=1;
                    }
                    switch (alt40) {
                        case 1 :
                            // BNGGrammar_Actions.g:121:10: ( write_m_par_def[map] ) ( COMMA write_m_par_def[map] )*
                            {
                            // BNGGrammar_Actions.g:121:10: ( write_m_par_def[map] )
                            // BNGGrammar_Actions.g:121:11: write_m_par_def[map]
                            {
                            pushFollow(FOLLOW_write_m_par_def_in_write_m_file1101);
                            write_m_par_def(map);

                            state._fsp--;


                            }

                            // BNGGrammar_Actions.g:121:33: ( COMMA write_m_par_def[map] )*
                            loop39:
                            do {
                                int alt39=2;
                                int LA39_0 = input.LA(1);

                                if ( (LA39_0==COMMA) ) {
                                    alt39=1;
                                }


                                switch (alt39) {
                            	case 1 :
                            	    // BNGGrammar_Actions.g:121:34: COMMA write_m_par_def[map]
                            	    {
                            	    match(input,COMMA,FOLLOW_COMMA_in_write_m_file1106); 
                            	    pushFollow(FOLLOW_write_m_par_def_in_write_m_file1108);
                            	    write_m_par_def(map);

                            	    state._fsp--;


                            	    }
                            	    break;

                            	default :
                            	    break loop39;
                                }
                            } while (true);


                            }
                            break;

                    }

                    match(input,RBRACKET,FOLLOW_RBRACKET_in_write_m_file1123); 

                    }
                    break;

            }

            match(input,RPAREN,FOLLOW_RPAREN_in_write_m_file1127); 
            // BNGGrammar_Actions.g:122:27: ( SEMI )?
            int alt42=2;
            int LA42_0 = input.LA(1);

            if ( (LA42_0==SEMI) ) {
                alt42=1;
            }
            switch (alt42) {
                case 1 :
                    // BNGGrammar_Actions.g:122:27: SEMI
                    {
                    match(input,SEMI,FOLLOW_SEMI_in_write_m_file1129); 

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
    // $ANTLR end "write_m_file"

    public static class write_mex_file_return extends ParserRuleReturnScope {
        public StringTemplate st;
        public Object getTemplate() { return st; }
        public String toString() { return st==null?null:st.toString(); }
    };

    // $ANTLR start "write_mex_file"
    // BNGGrammar_Actions.g:124:1: write_mex_file[Map<String,String> map] : WRITEMEXFILE LPAREN LBRACKET ( ( write_m_par_def[map] | ps_par_def[map] ) ( COMMA write_m_par_def[map] | ps_par_def[map] )* )? RBRACKET RPAREN ( SEMI )? ;
    public final BNGGrammar_BNGGrammar_Actions.write_mex_file_return write_mex_file(Map<String,String> map) throws RecognitionException {
        BNGGrammar_BNGGrammar_Actions.write_mex_file_return retval = new BNGGrammar_BNGGrammar_Actions.write_mex_file_return();
        retval.start = input.LT(1);

        try {
            // BNGGrammar_Actions.g:125:9: ( WRITEMEXFILE LPAREN LBRACKET ( ( write_m_par_def[map] | ps_par_def[map] ) ( COMMA write_m_par_def[map] | ps_par_def[map] )* )? RBRACKET RPAREN ( SEMI )? )
            // BNGGrammar_Actions.g:125:11: WRITEMEXFILE LPAREN LBRACKET ( ( write_m_par_def[map] | ps_par_def[map] ) ( COMMA write_m_par_def[map] | ps_par_def[map] )* )? RBRACKET RPAREN ( SEMI )?
            {
            match(input,WRITEMEXFILE,FOLLOW_WRITEMEXFILE_in_write_mex_file1147); 
            match(input,LPAREN,FOLLOW_LPAREN_in_write_mex_file1149); 
            match(input,LBRACKET,FOLLOW_LBRACKET_in_write_mex_file1151); 
            // BNGGrammar_Actions.g:126:9: ( ( write_m_par_def[map] | ps_par_def[map] ) ( COMMA write_m_par_def[map] | ps_par_def[map] )* )?
            int alt45=2;
            int LA45_0 = input.LA(1);

            if ( ((LA45_0>=PREFFIX && LA45_0<=SUFFIX)||(LA45_0>=ATOL && LA45_0<=RTOL)||(LA45_0>=SPARSE && LA45_0<=N_STEPS)||LA45_0==BDF) ) {
                alt45=1;
            }
            switch (alt45) {
                case 1 :
                    // BNGGrammar_Actions.g:126:10: ( write_m_par_def[map] | ps_par_def[map] ) ( COMMA write_m_par_def[map] | ps_par_def[map] )*
                    {
                    // BNGGrammar_Actions.g:126:10: ( write_m_par_def[map] | ps_par_def[map] )
                    int alt43=2;
                    int LA43_0 = input.LA(1);

                    if ( ((LA43_0>=ATOL && LA43_0<=RTOL)||(LA43_0>=SPARSE && LA43_0<=N_STEPS)||LA43_0==BDF) ) {
                        alt43=1;
                    }
                    else if ( ((LA43_0>=PREFFIX && LA43_0<=SUFFIX)) ) {
                        alt43=2;
                    }
                    else {
                        NoViableAltException nvae =
                            new NoViableAltException("", 43, 0, input);

                        throw nvae;
                    }
                    switch (alt43) {
                        case 1 :
                            // BNGGrammar_Actions.g:126:11: write_m_par_def[map]
                            {
                            pushFollow(FOLLOW_write_m_par_def_in_write_mex_file1163);
                            write_m_par_def(map);

                            state._fsp--;


                            }
                            break;
                        case 2 :
                            // BNGGrammar_Actions.g:126:33: ps_par_def[map]
                            {
                            pushFollow(FOLLOW_ps_par_def_in_write_mex_file1167);
                            ps_par_def(map);

                            state._fsp--;


                            }
                            break;

                    }

                    // BNGGrammar_Actions.g:126:50: ( COMMA write_m_par_def[map] | ps_par_def[map] )*
                    loop44:
                    do {
                        int alt44=3;
                        int LA44_0 = input.LA(1);

                        if ( (LA44_0==COMMA) ) {
                            alt44=1;
                        }
                        else if ( ((LA44_0>=PREFFIX && LA44_0<=SUFFIX)) ) {
                            alt44=2;
                        }


                        switch (alt44) {
                    	case 1 :
                    	    // BNGGrammar_Actions.g:126:51: COMMA write_m_par_def[map]
                    	    {
                    	    match(input,COMMA,FOLLOW_COMMA_in_write_mex_file1172); 
                    	    pushFollow(FOLLOW_write_m_par_def_in_write_mex_file1174);
                    	    write_m_par_def(map);

                    	    state._fsp--;


                    	    }
                    	    break;
                    	case 2 :
                    	    // BNGGrammar_Actions.g:126:79: ps_par_def[map]
                    	    {
                    	    pushFollow(FOLLOW_ps_par_def_in_write_mex_file1178);
                    	    ps_par_def(map);

                    	    state._fsp--;


                    	    }
                    	    break;

                    	default :
                    	    break loop44;
                        }
                    } while (true);


                    }
                    break;

            }

            match(input,RBRACKET,FOLLOW_RBRACKET_in_write_mex_file1193); 
            match(input,RPAREN,FOLLOW_RPAREN_in_write_mex_file1195); 
            // BNGGrammar_Actions.g:127:25: ( SEMI )?
            int alt46=2;
            int LA46_0 = input.LA(1);

            if ( (LA46_0==SEMI) ) {
                alt46=1;
            }
            switch (alt46) {
                case 1 :
                    // BNGGrammar_Actions.g:127:25: SEMI
                    {
                    match(input,SEMI,FOLLOW_SEMI_in_write_mex_file1197); 

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
    // $ANTLR end "write_mex_file"

    public static class write_network_return extends ParserRuleReturnScope {
        public StringTemplate st;
        public Object getTemplate() { return st; }
        public String toString() { return st==null?null:st.toString(); }
    };

    // $ANTLR start "write_network"
    // BNGGrammar_Actions.g:129:1: write_network[Map<String,String> map] : WRITENETWORK LPAREN ( LBRACKET ( ( ps_par_def[map] | simulate_par_def[map] ) ( COMMA ( ps_par_def[map] | simulate_par_def[map] ) )* )? RBRACKET )? RPAREN ;
    public final BNGGrammar_BNGGrammar_Actions.write_network_return write_network(Map<String,String> map) throws RecognitionException {
        BNGGrammar_BNGGrammar_Actions.write_network_return retval = new BNGGrammar_BNGGrammar_Actions.write_network_return();
        retval.start = input.LT(1);

        try {
            // BNGGrammar_Actions.g:130:7: ( WRITENETWORK LPAREN ( LBRACKET ( ( ps_par_def[map] | simulate_par_def[map] ) ( COMMA ( ps_par_def[map] | simulate_par_def[map] ) )* )? RBRACKET )? RPAREN )
            // BNGGrammar_Actions.g:130:9: WRITENETWORK LPAREN ( LBRACKET ( ( ps_par_def[map] | simulate_par_def[map] ) ( COMMA ( ps_par_def[map] | simulate_par_def[map] ) )* )? RBRACKET )? RPAREN
            {
            match(input,WRITENETWORK,FOLLOW_WRITENETWORK_in_write_network1213); 
            match(input,LPAREN,FOLLOW_LPAREN_in_write_network1215); 
            // BNGGrammar_Actions.g:130:29: ( LBRACKET ( ( ps_par_def[map] | simulate_par_def[map] ) ( COMMA ( ps_par_def[map] | simulate_par_def[map] ) )* )? RBRACKET )?
            int alt51=2;
            int LA51_0 = input.LA(1);

            if ( (LA51_0==LBRACKET) ) {
                alt51=1;
            }
            switch (alt51) {
                case 1 :
                    // BNGGrammar_Actions.g:130:30: LBRACKET ( ( ps_par_def[map] | simulate_par_def[map] ) ( COMMA ( ps_par_def[map] | simulate_par_def[map] ) )* )? RBRACKET
                    {
                    match(input,LBRACKET,FOLLOW_LBRACKET_in_write_network1218); 
                    // BNGGrammar_Actions.g:131:7: ( ( ps_par_def[map] | simulate_par_def[map] ) ( COMMA ( ps_par_def[map] | simulate_par_def[map] ) )* )?
                    int alt50=2;
                    int LA50_0 = input.LA(1);

                    if ( (LA50_0==OVERWRITE||(LA50_0>=PREFFIX && LA50_0<=SUFFIX)||LA50_0==VERBOSE||(LA50_0>=T_END && LA50_0<=MAX_SIM_STEPS)||LA50_0==SAMPLE_TIMES||(LA50_0>=PRINT_CDAT && LA50_0<=PRINT_FUNCTIONS)||LA50_0==METHOD||(LA50_0>=ARGFILE && LA50_0<=PRINT_END)||LA50_0==CONTINUE||LA50_0==EVALUATE_EXPRESSIONS) ) {
                        alt50=1;
                    }
                    switch (alt50) {
                        case 1 :
                            // BNGGrammar_Actions.g:131:8: ( ps_par_def[map] | simulate_par_def[map] ) ( COMMA ( ps_par_def[map] | simulate_par_def[map] ) )*
                            {
                            // BNGGrammar_Actions.g:131:8: ( ps_par_def[map] | simulate_par_def[map] )
                            int alt47=2;
                            int LA47_0 = input.LA(1);

                            if ( ((LA47_0>=PREFFIX && LA47_0<=SUFFIX)) ) {
                                alt47=1;
                            }
                            else if ( (LA47_0==OVERWRITE||LA47_0==VERBOSE||(LA47_0>=T_END && LA47_0<=MAX_SIM_STEPS)||LA47_0==SAMPLE_TIMES||(LA47_0>=PRINT_CDAT && LA47_0<=PRINT_FUNCTIONS)||LA47_0==METHOD||(LA47_0>=ARGFILE && LA47_0<=PRINT_END)||LA47_0==CONTINUE||LA47_0==EVALUATE_EXPRESSIONS) ) {
                                alt47=2;
                            }
                            else {
                                NoViableAltException nvae =
                                    new NoViableAltException("", 47, 0, input);

                                throw nvae;
                            }
                            switch (alt47) {
                                case 1 :
                                    // BNGGrammar_Actions.g:131:9: ps_par_def[map]
                                    {
                                    pushFollow(FOLLOW_ps_par_def_in_write_network1229);
                                    ps_par_def(map);

                                    state._fsp--;


                                    }
                                    break;
                                case 2 :
                                    // BNGGrammar_Actions.g:131:25: simulate_par_def[map]
                                    {
                                    pushFollow(FOLLOW_simulate_par_def_in_write_network1232);
                                    simulate_par_def(map);

                                    state._fsp--;


                                    }
                                    break;

                            }

                            // BNGGrammar_Actions.g:132:11: ( COMMA ( ps_par_def[map] | simulate_par_def[map] ) )*
                            loop49:
                            do {
                                int alt49=2;
                                int LA49_0 = input.LA(1);

                                if ( (LA49_0==COMMA) ) {
                                    alt49=1;
                                }


                                switch (alt49) {
                            	case 1 :
                            	    // BNGGrammar_Actions.g:132:12: COMMA ( ps_par_def[map] | simulate_par_def[map] )
                            	    {
                            	    match(input,COMMA,FOLLOW_COMMA_in_write_network1247); 
                            	    // BNGGrammar_Actions.g:132:18: ( ps_par_def[map] | simulate_par_def[map] )
                            	    int alt48=2;
                            	    int LA48_0 = input.LA(1);

                            	    if ( ((LA48_0>=PREFFIX && LA48_0<=SUFFIX)) ) {
                            	        alt48=1;
                            	    }
                            	    else if ( (LA48_0==OVERWRITE||LA48_0==VERBOSE||(LA48_0>=T_END && LA48_0<=MAX_SIM_STEPS)||LA48_0==SAMPLE_TIMES||(LA48_0>=PRINT_CDAT && LA48_0<=PRINT_FUNCTIONS)||LA48_0==METHOD||(LA48_0>=ARGFILE && LA48_0<=PRINT_END)||LA48_0==CONTINUE||LA48_0==EVALUATE_EXPRESSIONS) ) {
                            	        alt48=2;
                            	    }
                            	    else {
                            	        NoViableAltException nvae =
                            	            new NoViableAltException("", 48, 0, input);

                            	        throw nvae;
                            	    }
                            	    switch (alt48) {
                            	        case 1 :
                            	            // BNGGrammar_Actions.g:132:19: ps_par_def[map]
                            	            {
                            	            pushFollow(FOLLOW_ps_par_def_in_write_network1250);
                            	            ps_par_def(map);

                            	            state._fsp--;


                            	            }
                            	            break;
                            	        case 2 :
                            	            // BNGGrammar_Actions.g:132:35: simulate_par_def[map]
                            	            {
                            	            pushFollow(FOLLOW_simulate_par_def_in_write_network1253);
                            	            simulate_par_def(map);

                            	            state._fsp--;


                            	            }
                            	            break;

                            	    }


                            	    }
                            	    break;

                            	default :
                            	    break loop49;
                                }
                            } while (true);


                            }
                            break;

                    }

                    match(input,RBRACKET,FOLLOW_RBRACKET_in_write_network1268); 

                    }
                    break;

            }

            match(input,RPAREN,FOLLOW_RPAREN_in_write_network1279); 

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
    // $ANTLR end "write_network"

    public static class simulate_pla_return extends ParserRuleReturnScope {
        public StringTemplate st;
        public Object getTemplate() { return st; }
        public String toString() { return st==null?null:st.toString(); }
    };

    // $ANTLR start "simulate_pla"
    // BNGGrammar_Actions.g:137:1: simulate_pla[Map<String,String> map] : SIMULATE_PLA LPAREN ( LBRACKET ( ( ps_par_def[map] | simulate_par_def[map] | simulate_pla_par_def[map] ) ( COMMA ( ps_par_def[map] | simulate_par_def[map] | simulate_pla_par_def[map] ) )* )? RBRACKET )? RPAREN ( SEMI )? ;
    public final BNGGrammar_BNGGrammar_Actions.simulate_pla_return simulate_pla(Map<String,String> map) throws RecognitionException {
        BNGGrammar_BNGGrammar_Actions.simulate_pla_return retval = new BNGGrammar_BNGGrammar_Actions.simulate_pla_return();
        retval.start = input.LT(1);

        try {
            // BNGGrammar_Actions.g:138:5: ( SIMULATE_PLA LPAREN ( LBRACKET ( ( ps_par_def[map] | simulate_par_def[map] | simulate_pla_par_def[map] ) ( COMMA ( ps_par_def[map] | simulate_par_def[map] | simulate_pla_par_def[map] ) )* )? RBRACKET )? RPAREN ( SEMI )? )
            // BNGGrammar_Actions.g:138:7: SIMULATE_PLA LPAREN ( LBRACKET ( ( ps_par_def[map] | simulate_par_def[map] | simulate_pla_par_def[map] ) ( COMMA ( ps_par_def[map] | simulate_par_def[map] | simulate_pla_par_def[map] ) )* )? RBRACKET )? RPAREN ( SEMI )?
            {
            match(input,SIMULATE_PLA,FOLLOW_SIMULATE_PLA_in_simulate_pla1305); 
            match(input,LPAREN,FOLLOW_LPAREN_in_simulate_pla1307); 
            // BNGGrammar_Actions.g:138:27: ( LBRACKET ( ( ps_par_def[map] | simulate_par_def[map] | simulate_pla_par_def[map] ) ( COMMA ( ps_par_def[map] | simulate_par_def[map] | simulate_pla_par_def[map] ) )* )? RBRACKET )?
            int alt56=2;
            int LA56_0 = input.LA(1);

            if ( (LA56_0==LBRACKET) ) {
                alt56=1;
            }
            switch (alt56) {
                case 1 :
                    // BNGGrammar_Actions.g:138:28: LBRACKET ( ( ps_par_def[map] | simulate_par_def[map] | simulate_pla_par_def[map] ) ( COMMA ( ps_par_def[map] | simulate_par_def[map] | simulate_pla_par_def[map] ) )* )? RBRACKET
                    {
                    match(input,LBRACKET,FOLLOW_LBRACKET_in_simulate_pla1310); 
                    // BNGGrammar_Actions.g:139:14: ( ( ps_par_def[map] | simulate_par_def[map] | simulate_pla_par_def[map] ) ( COMMA ( ps_par_def[map] | simulate_par_def[map] | simulate_pla_par_def[map] ) )* )?
                    int alt55=2;
                    int LA55_0 = input.LA(1);

                    if ( (LA55_0==OVERWRITE||(LA55_0>=PREFFIX && LA55_0<=SUFFIX)||LA55_0==VERBOSE||(LA55_0>=T_END && LA55_0<=MAX_SIM_STEPS)||(LA55_0>=PLA_CONFIG && LA55_0<=SAMPLE_TIMES)||(LA55_0>=PRINT_CDAT && LA55_0<=PRINT_FUNCTIONS)||LA55_0==METHOD||(LA55_0>=ARGFILE && LA55_0<=PRINT_END)||LA55_0==CONTINUE||LA55_0==EVALUATE_EXPRESSIONS) ) {
                        alt55=1;
                    }
                    switch (alt55) {
                        case 1 :
                            // BNGGrammar_Actions.g:139:15: ( ps_par_def[map] | simulate_par_def[map] | simulate_pla_par_def[map] ) ( COMMA ( ps_par_def[map] | simulate_par_def[map] | simulate_pla_par_def[map] ) )*
                            {
                            // BNGGrammar_Actions.g:139:15: ( ps_par_def[map] | simulate_par_def[map] | simulate_pla_par_def[map] )
                            int alt52=3;
                            switch ( input.LA(1) ) {
                            case PREFFIX:
                            case SUFFIX:
                                {
                                alt52=1;
                                }
                                break;
                            case OVERWRITE:
                            case VERBOSE:
                            case T_END:
                            case T_START:
                            case N_STEPS:
                            case N_OUTPUT_STEPS:
                            case MAX_SIM_STEPS:
                            case SAMPLE_TIMES:
                            case PRINT_CDAT:
                            case PRINT_FUNCTIONS:
                            case METHOD:
                            case ARGFILE:
                            case SAVE_PROGRESS:
                            case NETFILE:
                            case PRINT_NET:
                            case PRINT_END:
                            case CONTINUE:
                            case EVALUATE_EXPRESSIONS:
                                {
                                alt52=2;
                                }
                                break;
                            case PLA_CONFIG:
                                {
                                alt52=3;
                                }
                                break;
                            default:
                                NoViableAltException nvae =
                                    new NoViableAltException("", 52, 0, input);

                                throw nvae;
                            }

                            switch (alt52) {
                                case 1 :
                                    // BNGGrammar_Actions.g:139:16: ps_par_def[map]
                                    {
                                    pushFollow(FOLLOW_ps_par_def_in_simulate_pla1327);
                                    ps_par_def(map);

                                    state._fsp--;


                                    }
                                    break;
                                case 2 :
                                    // BNGGrammar_Actions.g:139:32: simulate_par_def[map]
                                    {
                                    pushFollow(FOLLOW_simulate_par_def_in_simulate_pla1330);
                                    simulate_par_def(map);

                                    state._fsp--;


                                    }
                                    break;
                                case 3 :
                                    // BNGGrammar_Actions.g:139:54: simulate_pla_par_def[map]
                                    {
                                    pushFollow(FOLLOW_simulate_pla_par_def_in_simulate_pla1333);
                                    simulate_pla_par_def(map);

                                    state._fsp--;


                                    }
                                    break;

                            }

                            // BNGGrammar_Actions.g:140:11: ( COMMA ( ps_par_def[map] | simulate_par_def[map] | simulate_pla_par_def[map] ) )*
                            loop54:
                            do {
                                int alt54=2;
                                int LA54_0 = input.LA(1);

                                if ( (LA54_0==COMMA) ) {
                                    alt54=1;
                                }


                                switch (alt54) {
                            	case 1 :
                            	    // BNGGrammar_Actions.g:140:12: COMMA ( ps_par_def[map] | simulate_par_def[map] | simulate_pla_par_def[map] )
                            	    {
                            	    match(input,COMMA,FOLLOW_COMMA_in_simulate_pla1348); 
                            	    // BNGGrammar_Actions.g:140:18: ( ps_par_def[map] | simulate_par_def[map] | simulate_pla_par_def[map] )
                            	    int alt53=3;
                            	    switch ( input.LA(1) ) {
                            	    case PREFFIX:
                            	    case SUFFIX:
                            	        {
                            	        alt53=1;
                            	        }
                            	        break;
                            	    case OVERWRITE:
                            	    case VERBOSE:
                            	    case T_END:
                            	    case T_START:
                            	    case N_STEPS:
                            	    case N_OUTPUT_STEPS:
                            	    case MAX_SIM_STEPS:
                            	    case SAMPLE_TIMES:
                            	    case PRINT_CDAT:
                            	    case PRINT_FUNCTIONS:
                            	    case METHOD:
                            	    case ARGFILE:
                            	    case SAVE_PROGRESS:
                            	    case NETFILE:
                            	    case PRINT_NET:
                            	    case PRINT_END:
                            	    case CONTINUE:
                            	    case EVALUATE_EXPRESSIONS:
                            	        {
                            	        alt53=2;
                            	        }
                            	        break;
                            	    case PLA_CONFIG:
                            	        {
                            	        alt53=3;
                            	        }
                            	        break;
                            	    default:
                            	        NoViableAltException nvae =
                            	            new NoViableAltException("", 53, 0, input);

                            	        throw nvae;
                            	    }

                            	    switch (alt53) {
                            	        case 1 :
                            	            // BNGGrammar_Actions.g:140:19: ps_par_def[map]
                            	            {
                            	            pushFollow(FOLLOW_ps_par_def_in_simulate_pla1351);
                            	            ps_par_def(map);

                            	            state._fsp--;


                            	            }
                            	            break;
                            	        case 2 :
                            	            // BNGGrammar_Actions.g:140:35: simulate_par_def[map]
                            	            {
                            	            pushFollow(FOLLOW_simulate_par_def_in_simulate_pla1354);
                            	            simulate_par_def(map);

                            	            state._fsp--;


                            	            }
                            	            break;
                            	        case 3 :
                            	            // BNGGrammar_Actions.g:140:57: simulate_pla_par_def[map]
                            	            {
                            	            pushFollow(FOLLOW_simulate_pla_par_def_in_simulate_pla1357);
                            	            simulate_pla_par_def(map);

                            	            state._fsp--;


                            	            }
                            	            break;

                            	    }


                            	    }
                            	    break;

                            	default :
                            	    break loop54;
                                }
                            } while (true);


                            }
                            break;

                    }

                    match(input,RBRACKET,FOLLOW_RBRACKET_in_simulate_pla1376); 

                    }
                    break;

            }

            match(input,RPAREN,FOLLOW_RPAREN_in_simulate_pla1380); 
            // BNGGrammar_Actions.g:141:29: ( SEMI )?
            int alt57=2;
            int LA57_0 = input.LA(1);

            if ( (LA57_0==SEMI) ) {
                alt57=1;
            }
            switch (alt57) {
                case 1 :
                    // BNGGrammar_Actions.g:141:29: SEMI
                    {
                    match(input,SEMI,FOLLOW_SEMI_in_simulate_pla1382); 

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
    // $ANTLR end "simulate_pla"

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
    // BNGGrammar_Actions.g:144:1: read_file : READFILE LPAREN ( LBRACKET ( FILE ASSIGNS DBQUOTES s1= ( filename ) DBQUOTES )? RBRACKET )? RPAREN ( SEMI )? -> action(id=$READFILE.textoptionMap=$read_file::actions);
    public final BNGGrammar_BNGGrammar_Actions.read_file_return read_file() throws RecognitionException {
        read_file_stack.push(new read_file_scope());
        BNGGrammar_BNGGrammar_Actions.read_file_return retval = new BNGGrammar_BNGGrammar_Actions.read_file_return();
        retval.start = input.LT(1);

        Token s1=null;
        Token FILE16=null;
        Token READFILE17=null;


          ((read_file_scope)read_file_stack.peek()).actions = new HashMap<String,String>();

        try {
            // BNGGrammar_Actions.g:151:9: ( READFILE LPAREN ( LBRACKET ( FILE ASSIGNS DBQUOTES s1= ( filename ) DBQUOTES )? RBRACKET )? RPAREN ( SEMI )? -> action(id=$READFILE.textoptionMap=$read_file::actions))
            // BNGGrammar_Actions.g:151:11: READFILE LPAREN ( LBRACKET ( FILE ASSIGNS DBQUOTES s1= ( filename ) DBQUOTES )? RBRACKET )? RPAREN ( SEMI )?
            {
            READFILE17=(Token)match(input,READFILE,FOLLOW_READFILE_in_read_file1412); 
            match(input,LPAREN,FOLLOW_LPAREN_in_read_file1414); 
            // BNGGrammar_Actions.g:152:9: ( LBRACKET ( FILE ASSIGNS DBQUOTES s1= ( filename ) DBQUOTES )? RBRACKET )?
            int alt59=2;
            int LA59_0 = input.LA(1);

            if ( (LA59_0==LBRACKET) ) {
                alt59=1;
            }
            switch (alt59) {
                case 1 :
                    // BNGGrammar_Actions.g:152:10: LBRACKET ( FILE ASSIGNS DBQUOTES s1= ( filename ) DBQUOTES )? RBRACKET
                    {
                    match(input,LBRACKET,FOLLOW_LBRACKET_in_read_file1426); 
                    // BNGGrammar_Actions.g:152:19: ( FILE ASSIGNS DBQUOTES s1= ( filename ) DBQUOTES )?
                    int alt58=2;
                    int LA58_0 = input.LA(1);

                    if ( (LA58_0==FILE) ) {
                        alt58=1;
                    }
                    switch (alt58) {
                        case 1 :
                            // BNGGrammar_Actions.g:152:20: FILE ASSIGNS DBQUOTES s1= ( filename ) DBQUOTES
                            {
                            FILE16=(Token)match(input,FILE,FOLLOW_FILE_in_read_file1429); 
                            match(input,ASSIGNS,FOLLOW_ASSIGNS_in_read_file1431); 
                            match(input,DBQUOTES,FOLLOW_DBQUOTES_in_read_file1433); 
                            // BNGGrammar_Actions.g:152:45: ( filename )
                            // BNGGrammar_Actions.g:152:46: filename
                            {
                            pushFollow(FOLLOW_filename_in_read_file1438);
                            filename();

                            state._fsp--;


                            }

                            match(input,DBQUOTES,FOLLOW_DBQUOTES_in_read_file1441); 
                            ((read_file_scope)read_file_stack.peek()).actions.put((FILE16!=null?FILE16.getText():null),(s1!=null?s1.getText():null));

                            }
                            break;

                    }

                    match(input,RBRACKET,FOLLOW_RBRACKET_in_read_file1447); 

                    }
                    break;

            }

            match(input,RPAREN,FOLLOW_RPAREN_in_read_file1460); 
            // BNGGrammar_Actions.g:153:16: ( SEMI )?
            int alt60=2;
            int LA60_0 = input.LA(1);

            if ( (LA60_0==SEMI) ) {
                alt60=1;
            }
            switch (alt60) {
                case 1 :
                    // BNGGrammar_Actions.g:153:16: SEMI
                    {
                    match(input,SEMI,FOLLOW_SEMI_in_read_file1462); 

                    }
                    break;

            }



            // TEMPLATE REWRITE
            // 153:22: -> action(id=$READFILE.textoptionMap=$read_file::actions)
            {
                retval.st = templateLib.getInstanceOf("action",
              new STAttrMap().put("id", (READFILE17!=null?READFILE17.getText():null)).put("optionMap", ((read_file_scope)read_file_stack.peek()).actions));
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
    // BNGGrammar_Actions.g:156:1: write_file : write_type LPAREN ( LBRACKET ( write_par_def ( COMMA write_par_def )* )? RBRACKET )? RPAREN ( SEMI )? -> action(id=$write_type.text);
    public final BNGGrammar_BNGGrammar_Actions.write_file_return write_file() throws RecognitionException {
        write_file_stack.push(new write_file_scope());
        BNGGrammar_BNGGrammar_Actions.write_file_return retval = new BNGGrammar_BNGGrammar_Actions.write_file_return();
        retval.start = input.LT(1);

        BNGGrammar_BNGGrammar_Actions.write_type_return write_type18 = null;



          ((write_file_scope)write_file_stack.peek()).actions = new HashMap<String,String>();

        try {
            // BNGGrammar_Actions.g:163:9: ( write_type LPAREN ( LBRACKET ( write_par_def ( COMMA write_par_def )* )? RBRACKET )? RPAREN ( SEMI )? -> action(id=$write_type.text))
            // BNGGrammar_Actions.g:163:11: write_type LPAREN ( LBRACKET ( write_par_def ( COMMA write_par_def )* )? RBRACKET )? RPAREN ( SEMI )?
            {
            pushFollow(FOLLOW_write_type_in_write_file1508);
            write_type18=write_type();

            state._fsp--;

            match(input,LPAREN,FOLLOW_LPAREN_in_write_file1520); 
            // BNGGrammar_Actions.g:164:18: ( LBRACKET ( write_par_def ( COMMA write_par_def )* )? RBRACKET )?
            int alt63=2;
            int LA63_0 = input.LA(1);

            if ( (LA63_0==LBRACKET) ) {
                alt63=1;
            }
            switch (alt63) {
                case 1 :
                    // BNGGrammar_Actions.g:164:19: LBRACKET ( write_par_def ( COMMA write_par_def )* )? RBRACKET
                    {
                    match(input,LBRACKET,FOLLOW_LBRACKET_in_write_file1523); 
                    // BNGGrammar_Actions.g:164:28: ( write_par_def ( COMMA write_par_def )* )?
                    int alt62=2;
                    int LA62_0 = input.LA(1);

                    if ( (LA62_0==SUFFIX) ) {
                        alt62=1;
                    }
                    switch (alt62) {
                        case 1 :
                            // BNGGrammar_Actions.g:164:29: write_par_def ( COMMA write_par_def )*
                            {
                            pushFollow(FOLLOW_write_par_def_in_write_file1526);
                            write_par_def();

                            state._fsp--;

                            // BNGGrammar_Actions.g:164:43: ( COMMA write_par_def )*
                            loop61:
                            do {
                                int alt61=2;
                                int LA61_0 = input.LA(1);

                                if ( (LA61_0==COMMA) ) {
                                    alt61=1;
                                }


                                switch (alt61) {
                            	case 1 :
                            	    // BNGGrammar_Actions.g:164:44: COMMA write_par_def
                            	    {
                            	    match(input,COMMA,FOLLOW_COMMA_in_write_file1529); 
                            	    pushFollow(FOLLOW_write_par_def_in_write_file1531);
                            	    write_par_def();

                            	    state._fsp--;


                            	    }
                            	    break;

                            	default :
                            	    break loop61;
                                }
                            } while (true);


                            }
                            break;

                    }

                    match(input,RBRACKET,FOLLOW_RBRACKET_in_write_file1537); 

                    }
                    break;

            }

            match(input,RPAREN,FOLLOW_RPAREN_in_write_file1552); 
            // BNGGrammar_Actions.g:165:19: ( SEMI )?
            int alt64=2;
            int LA64_0 = input.LA(1);

            if ( (LA64_0==SEMI) ) {
                alt64=1;
            }
            switch (alt64) {
                case 1 :
                    // BNGGrammar_Actions.g:165:19: SEMI
                    {
                    match(input,SEMI,FOLLOW_SEMI_in_write_file1554); 

                    }
                    break;

            }



            // TEMPLATE REWRITE
            // 165:25: -> action(id=$write_type.text)
            {
                retval.st = templateLib.getInstanceOf("action",
              new STAttrMap().put("id", (write_type18!=null?input.toString(write_type18.start,write_type18.stop):null)));
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
    // BNGGrammar_Actions.g:167:1: write_type : ( WRITENET | WRITESBML | WRITEXML );
    public final BNGGrammar_BNGGrammar_Actions.write_type_return write_type() throws RecognitionException {
        BNGGrammar_BNGGrammar_Actions.write_type_return retval = new BNGGrammar_BNGGrammar_Actions.write_type_return();
        retval.start = input.LT(1);

        try {
            // BNGGrammar_Actions.g:168:9: ( WRITENET | WRITESBML | WRITEXML )
            // BNGGrammar_Actions.g:
            {
            if ( (input.LA(1)>=WRITENET && input.LA(1)<=WRITESBML)||input.LA(1)==WRITEXML ) {
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
    // BNGGrammar_Actions.g:172:1: set_concentration : SETCONCENTRATION LPAREN variable_definition RPAREN ( SEMI )? -> action(id=$SETCONCENTRATION.textoptionMap=$set_concentration::options);
    public final BNGGrammar_BNGGrammar_Actions.set_concentration_return set_concentration() throws RecognitionException {
        set_concentration_stack.push(new set_concentration_scope());
        BNGGrammar_BNGGrammar_Actions.set_concentration_return retval = new BNGGrammar_BNGGrammar_Actions.set_concentration_return();
        retval.start = input.LT(1);

        Token SETCONCENTRATION20=null;
        BNGGrammar_BNGGrammar_Actions.variable_definition_return variable_definition19 = null;



          ((set_concentration_scope)set_concentration_stack.peek()).options = new HashMap<String,String>();

        try {
            // BNGGrammar_Actions.g:179:9: ( SETCONCENTRATION LPAREN variable_definition RPAREN ( SEMI )? -> action(id=$SETCONCENTRATION.textoptionMap=$set_concentration::options))
            // BNGGrammar_Actions.g:179:11: SETCONCENTRATION LPAREN variable_definition RPAREN ( SEMI )?
            {
            SETCONCENTRATION20=(Token)match(input,SETCONCENTRATION,FOLLOW_SETCONCENTRATION_in_set_concentration1644); 
            match(input,LPAREN,FOLLOW_LPAREN_in_set_concentration1646); 
            pushFollow(FOLLOW_variable_definition_in_set_concentration1648);
            variable_definition19=variable_definition();

            state._fsp--;

            ((set_concentration_scope)set_concentration_stack.peek()).options.put((variable_definition19!=null?variable_definition19.variableName:null),(variable_definition19!=null?variable_definition19.variableValue:null));
            match(input,RPAREN,FOLLOW_RPAREN_in_set_concentration1669); 
            // BNGGrammar_Actions.g:181:16: ( SEMI )?
            int alt65=2;
            int LA65_0 = input.LA(1);

            if ( (LA65_0==SEMI) ) {
                alt65=1;
            }
            switch (alt65) {
                case 1 :
                    // BNGGrammar_Actions.g:181:16: SEMI
                    {
                    match(input,SEMI,FOLLOW_SEMI_in_set_concentration1671); 

                    }
                    break;

            }



            // TEMPLATE REWRITE
            // 181:22: -> action(id=$SETCONCENTRATION.textoptionMap=$set_concentration::options)
            {
                retval.st = templateLib.getInstanceOf("action",
              new STAttrMap().put("id", (SETCONCENTRATION20!=null?SETCONCENTRATION20.getText():null)).put("optionMap", ((set_concentration_scope)set_concentration_stack.peek()).options));
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

    protected static class add_concentration_scope {
        Map<String,String> options;
    }
    protected Stack add_concentration_stack = new Stack();

    public static class add_concentration_return extends ParserRuleReturnScope {
        public StringTemplate st;
        public Object getTemplate() { return st; }
        public String toString() { return st==null?null:st.toString(); }
    };

    // $ANTLR start "add_concentration"
    // BNGGrammar_Actions.g:184:1: add_concentration : ADDCONCENTRATION LPAREN variable_definition RPAREN ( SEMI )? -> action(id=$ADDCONCENTRATION.textoptionMap=$add_concentration::options);
    public final BNGGrammar_BNGGrammar_Actions.add_concentration_return add_concentration() throws RecognitionException {
        add_concentration_stack.push(new add_concentration_scope());
        BNGGrammar_BNGGrammar_Actions.add_concentration_return retval = new BNGGrammar_BNGGrammar_Actions.add_concentration_return();
        retval.start = input.LT(1);

        Token ADDCONCENTRATION22=null;
        BNGGrammar_BNGGrammar_Actions.variable_definition_return variable_definition21 = null;



          ((add_concentration_scope)add_concentration_stack.peek()).options = new HashMap<String,String>();

        try {
            // BNGGrammar_Actions.g:191:9: ( ADDCONCENTRATION LPAREN variable_definition RPAREN ( SEMI )? -> action(id=$ADDCONCENTRATION.textoptionMap=$add_concentration::options))
            // BNGGrammar_Actions.g:191:11: ADDCONCENTRATION LPAREN variable_definition RPAREN ( SEMI )?
            {
            ADDCONCENTRATION22=(Token)match(input,ADDCONCENTRATION,FOLLOW_ADDCONCENTRATION_in_add_concentration1727); 
            match(input,LPAREN,FOLLOW_LPAREN_in_add_concentration1729); 
            pushFollow(FOLLOW_variable_definition_in_add_concentration1731);
            variable_definition21=variable_definition();

            state._fsp--;

            ((add_concentration_scope)add_concentration_stack.peek()).options.put((variable_definition21!=null?variable_definition21.variableName:null),(variable_definition21!=null?variable_definition21.variableValue:null));
            match(input,RPAREN,FOLLOW_RPAREN_in_add_concentration1752); 
            // BNGGrammar_Actions.g:193:16: ( SEMI )?
            int alt66=2;
            int LA66_0 = input.LA(1);

            if ( (LA66_0==SEMI) ) {
                alt66=1;
            }
            switch (alt66) {
                case 1 :
                    // BNGGrammar_Actions.g:193:16: SEMI
                    {
                    match(input,SEMI,FOLLOW_SEMI_in_add_concentration1754); 

                    }
                    break;

            }



            // TEMPLATE REWRITE
            // 193:22: -> action(id=$ADDCONCENTRATION.textoptionMap=$add_concentration::options)
            {
                retval.st = templateLib.getInstanceOf("action",
              new STAttrMap().put("id", (ADDCONCENTRATION22!=null?ADDCONCENTRATION22.getText():null)).put("optionMap", ((add_concentration_scope)add_concentration_stack.peek()).options));
            }


            }

            retval.stop = input.LT(-1);

        }
        catch (RecognitionException re) {
            reportError(re);
            recover(input,re);
        }
        finally {
            add_concentration_stack.pop();
        }
        return retval;
    }
    // $ANTLR end "add_concentration"

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
    // BNGGrammar_Actions.g:196:1: set_parameter : SETPARAMETER LPAREN parameter_definition RPAREN ( SEMI )? -> action(id=$SETPARAMETER.textoptionMap=$set_parameter::options);
    public final BNGGrammar_BNGGrammar_Actions.set_parameter_return set_parameter() throws RecognitionException {
        set_parameter_stack.push(new set_parameter_scope());
        BNGGrammar_BNGGrammar_Actions.set_parameter_return retval = new BNGGrammar_BNGGrammar_Actions.set_parameter_return();
        retval.start = input.LT(1);

        Token SETPARAMETER24=null;
        BNGGrammar_BNGGrammar_Actions.parameter_definition_return parameter_definition23 = null;



          ((set_parameter_scope)set_parameter_stack.peek()).options = new HashMap<String,String>();

        try {
            // BNGGrammar_Actions.g:203:9: ( SETPARAMETER LPAREN parameter_definition RPAREN ( SEMI )? -> action(id=$SETPARAMETER.textoptionMap=$set_parameter::options))
            // BNGGrammar_Actions.g:203:12: SETPARAMETER LPAREN parameter_definition RPAREN ( SEMI )?
            {
            SETPARAMETER24=(Token)match(input,SETPARAMETER,FOLLOW_SETPARAMETER_in_set_parameter1810); 
            match(input,LPAREN,FOLLOW_LPAREN_in_set_parameter1812); 
            pushFollow(FOLLOW_parameter_definition_in_set_parameter1814);
            parameter_definition23=parameter_definition();

            state._fsp--;

            ((set_parameter_scope)set_parameter_stack.peek()).options.put((parameter_definition23!=null?parameter_definition23.parameterName:null),(parameter_definition23!=null?parameter_definition23.parameterValue:null));
            match(input,RPAREN,FOLLOW_RPAREN_in_set_parameter1837); 
            // BNGGrammar_Actions.g:205:16: ( SEMI )?
            int alt67=2;
            int LA67_0 = input.LA(1);

            if ( (LA67_0==SEMI) ) {
                alt67=1;
            }
            switch (alt67) {
                case 1 :
                    // BNGGrammar_Actions.g:205:16: SEMI
                    {
                    match(input,SEMI,FOLLOW_SEMI_in_set_parameter1839); 

                    }
                    break;

            }



            // TEMPLATE REWRITE
            // 205:22: -> action(id=$SETPARAMETER.textoptionMap=$set_parameter::options)
            {
                retval.st = templateLib.getInstanceOf("action",
              new STAttrMap().put("id", (SETPARAMETER24!=null?SETPARAMETER24.getText():null)).put("optionMap", ((set_parameter_scope)set_parameter_stack.peek()).options));
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
    // BNGGrammar_Actions.g:208:1: parameter_definition returns [String parameterName,String parameterValue] : DBQUOTES STRING DBQUOTES COMMA (e1= expression[gParent.memory] | ( DBQUOTES e2= expression[gParent.memory] DBQUOTES ) ) ;
    public final BNGGrammar_BNGGrammar_Actions.parameter_definition_return parameter_definition() throws RecognitionException {
        BNGGrammar_BNGGrammar_Actions.parameter_definition_return retval = new BNGGrammar_BNGGrammar_Actions.parameter_definition_return();
        retval.start = input.LT(1);

        Token STRING25=null;
        BNGGrammar_BNGGrammar_Expression.expression_return e1 = null;

        BNGGrammar_BNGGrammar_Expression.expression_return e2 = null;


        try {
            // BNGGrammar_Actions.g:209:1: ( DBQUOTES STRING DBQUOTES COMMA (e1= expression[gParent.memory] | ( DBQUOTES e2= expression[gParent.memory] DBQUOTES ) ) )
            // BNGGrammar_Actions.g:210:1: DBQUOTES STRING DBQUOTES COMMA (e1= expression[gParent.memory] | ( DBQUOTES e2= expression[gParent.memory] DBQUOTES ) )
            {
            match(input,DBQUOTES,FOLLOW_DBQUOTES_in_parameter_definition1874); 
            STRING25=(Token)match(input,STRING,FOLLOW_STRING_in_parameter_definition1877); 
            retval.parameterName = (STRING25!=null?STRING25.getText():null);
            match(input,DBQUOTES,FOLLOW_DBQUOTES_in_parameter_definition1882); 
            match(input,COMMA,FOLLOW_COMMA_in_parameter_definition1884); 
            // BNGGrammar_Actions.g:213:1: (e1= expression[gParent.memory] | ( DBQUOTES e2= expression[gParent.memory] DBQUOTES ) )
            int alt68=2;
            int LA68_0 = input.LA(1);

            if ( (LA68_0==IF||LA68_0==NOT||(LA68_0>=EXP && LA68_0<=LOG)||(LA68_0>=FLOAT && LA68_0<=INT)||LA68_0==STRING||LA68_0==LPAREN||LA68_0==MINUS) ) {
                alt68=1;
            }
            else if ( (LA68_0==DBQUOTES) ) {
                alt68=2;
            }
            else {
                NoViableAltException nvae =
                    new NoViableAltException("", 68, 0, input);

                throw nvae;
            }
            switch (alt68) {
                case 1 :
                    // BNGGrammar_Actions.g:213:2: e1= expression[gParent.memory]
                    {
                    pushFollow(FOLLOW_expression_in_parameter_definition1890);
                    e1=gBNGGrammar.expression(gParent.memory);

                    state._fsp--;

                    retval.parameterValue = (e1!=null?input.toString(e1.start,e1.stop):null); 

                    }
                    break;
                case 2 :
                    // BNGGrammar_Actions.g:214:1: ( DBQUOTES e2= expression[gParent.memory] DBQUOTES )
                    {
                    // BNGGrammar_Actions.g:214:1: ( DBQUOTES e2= expression[gParent.memory] DBQUOTES )
                    // BNGGrammar_Actions.g:214:2: DBQUOTES e2= expression[gParent.memory] DBQUOTES
                    {
                    match(input,DBQUOTES,FOLLOW_DBQUOTES_in_parameter_definition1898); 
                    pushFollow(FOLLOW_expression_in_parameter_definition1902);
                    e2=gBNGGrammar.expression(gParent.memory);

                    state._fsp--;

                    match(input,DBQUOTES,FOLLOW_DBQUOTES_in_parameter_definition1905); 
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
    // BNGGrammar_Actions.g:217:1: variable_definition returns [String variableName, String variableValue] : DBQUOTES species_def[$variable_definition::reactants,$variable_definition::bonds,\"\"] DBQUOTES COMMA (e1= expression[gParent.memory] | ( DBQUOTES e2= expression[gParent.memory] DBQUOTES ) ) ;
    public final BNGGrammar_BNGGrammar_Actions.variable_definition_return variable_definition() throws RecognitionException {
        variable_definition_stack.push(new variable_definition_scope());
        BNGGrammar_BNGGrammar_Actions.variable_definition_return retval = new BNGGrammar_BNGGrammar_Actions.variable_definition_return();
        retval.start = input.LT(1);

        BNGGrammar_BNGGrammar_Expression.expression_return e1 = null;

        BNGGrammar_BNGGrammar_Expression.expression_return e2 = null;

        BNGGrammar_BNGGrammar_SeedSpecies.species_def_return species_def26 = null;



          ((variable_definition_scope)variable_definition_stack.peek()).reactants = new ArrayList();
          ((variable_definition_scope)variable_definition_stack.peek()).bonds = new BondList();

        try {
            // BNGGrammar_Actions.g:227:1: ( DBQUOTES species_def[$variable_definition::reactants,$variable_definition::bonds,\"\"] DBQUOTES COMMA (e1= expression[gParent.memory] | ( DBQUOTES e2= expression[gParent.memory] DBQUOTES ) ) )
            // BNGGrammar_Actions.g:228:1: DBQUOTES species_def[$variable_definition::reactants,$variable_definition::bonds,\"\"] DBQUOTES COMMA (e1= expression[gParent.memory] | ( DBQUOTES e2= expression[gParent.memory] DBQUOTES ) )
            {
            match(input,DBQUOTES,FOLLOW_DBQUOTES_in_variable_definition1937); 
            pushFollow(FOLLOW_species_def_in_variable_definition1940);
            species_def26=gBNGGrammar.species_def(((variable_definition_scope)variable_definition_stack.peek()).reactants, ((variable_definition_scope)variable_definition_stack.peek()).bonds, "");

            state._fsp--;

            retval.variableName = (species_def26!=null?input.toString(species_def26.start,species_def26.stop):null);
            match(input,DBQUOTES,FOLLOW_DBQUOTES_in_variable_definition1946); 
            match(input,COMMA,FOLLOW_COMMA_in_variable_definition1948); 
            // BNGGrammar_Actions.g:231:1: (e1= expression[gParent.memory] | ( DBQUOTES e2= expression[gParent.memory] DBQUOTES ) )
            int alt69=2;
            int LA69_0 = input.LA(1);

            if ( (LA69_0==IF||LA69_0==NOT||(LA69_0>=EXP && LA69_0<=LOG)||(LA69_0>=FLOAT && LA69_0<=INT)||LA69_0==STRING||LA69_0==LPAREN||LA69_0==MINUS) ) {
                alt69=1;
            }
            else if ( (LA69_0==DBQUOTES) ) {
                alt69=2;
            }
            else {
                NoViableAltException nvae =
                    new NoViableAltException("", 69, 0, input);

                throw nvae;
            }
            switch (alt69) {
                case 1 :
                    // BNGGrammar_Actions.g:231:2: e1= expression[gParent.memory]
                    {
                    pushFollow(FOLLOW_expression_in_variable_definition1954);
                    e1=gBNGGrammar.expression(gParent.memory);

                    state._fsp--;

                    retval.variableValue = (e1!=null?input.toString(e1.start,e1.stop):null); 

                    }
                    break;
                case 2 :
                    // BNGGrammar_Actions.g:232:1: ( DBQUOTES e2= expression[gParent.memory] DBQUOTES )
                    {
                    // BNGGrammar_Actions.g:232:1: ( DBQUOTES e2= expression[gParent.memory] DBQUOTES )
                    // BNGGrammar_Actions.g:232:2: DBQUOTES e2= expression[gParent.memory] DBQUOTES
                    {
                    match(input,DBQUOTES,FOLLOW_DBQUOTES_in_variable_definition1962); 
                    pushFollow(FOLLOW_expression_in_variable_definition1966);
                    e2=gBNGGrammar.expression(gParent.memory);

                    state._fsp--;

                    match(input,DBQUOTES,FOLLOW_DBQUOTES_in_variable_definition1969); 
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
    // BNGGrammar_Actions.g:238:1: save_concentrations : SAVECONCENTRATIONS LPAREN ( DBQUOTES STRING DBQUOTES )? RPAREN ( SEMI )? -> action(id=$SAVECONCENTRATIONS.text);
    public final BNGGrammar_BNGGrammar_Actions.save_concentrations_return save_concentrations() throws RecognitionException {
        BNGGrammar_BNGGrammar_Actions.save_concentrations_return retval = new BNGGrammar_BNGGrammar_Actions.save_concentrations_return();
        retval.start = input.LT(1);

        Token SAVECONCENTRATIONS27=null;

        try {
            // BNGGrammar_Actions.g:239:9: ( SAVECONCENTRATIONS LPAREN ( DBQUOTES STRING DBQUOTES )? RPAREN ( SEMI )? -> action(id=$SAVECONCENTRATIONS.text))
            // BNGGrammar_Actions.g:239:11: SAVECONCENTRATIONS LPAREN ( DBQUOTES STRING DBQUOTES )? RPAREN ( SEMI )?
            {
            SAVECONCENTRATIONS27=(Token)match(input,SAVECONCENTRATIONS,FOLLOW_SAVECONCENTRATIONS_in_save_concentrations2026); 
            match(input,LPAREN,FOLLOW_LPAREN_in_save_concentrations2028); 
            // BNGGrammar_Actions.g:239:37: ( DBQUOTES STRING DBQUOTES )?
            int alt70=2;
            int LA70_0 = input.LA(1);

            if ( (LA70_0==DBQUOTES) ) {
                alt70=1;
            }
            switch (alt70) {
                case 1 :
                    // BNGGrammar_Actions.g:239:38: DBQUOTES STRING DBQUOTES
                    {
                    match(input,DBQUOTES,FOLLOW_DBQUOTES_in_save_concentrations2031); 
                    match(input,STRING,FOLLOW_STRING_in_save_concentrations2033); 
                    match(input,DBQUOTES,FOLLOW_DBQUOTES_in_save_concentrations2035); 

                    }
                    break;

            }

            match(input,RPAREN,FOLLOW_RPAREN_in_save_concentrations2039); 
            // BNGGrammar_Actions.g:239:72: ( SEMI )?
            int alt71=2;
            int LA71_0 = input.LA(1);

            if ( (LA71_0==SEMI) ) {
                alt71=1;
            }
            switch (alt71) {
                case 1 :
                    // BNGGrammar_Actions.g:239:72: SEMI
                    {
                    match(input,SEMI,FOLLOW_SEMI_in_save_concentrations2041); 

                    }
                    break;

            }



            // TEMPLATE REWRITE
            // 239:78: -> action(id=$SAVECONCENTRATIONS.text)
            {
                retval.st = templateLib.getInstanceOf("action",
              new STAttrMap().put("id", (SAVECONCENTRATIONS27!=null?SAVECONCENTRATIONS27.getText():null)));
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
    // BNGGrammar_Actions.g:241:1: reset_concentrations : RESETCONCENTRATIONS LPAREN ( DBQUOTES STRING DBQUOTES )? RPAREN ( SEMI )? -> action(id=$RESETCONCENTRATIONS.text);
    public final BNGGrammar_BNGGrammar_Actions.reset_concentrations_return reset_concentrations() throws RecognitionException {
        BNGGrammar_BNGGrammar_Actions.reset_concentrations_return retval = new BNGGrammar_BNGGrammar_Actions.reset_concentrations_return();
        retval.start = input.LT(1);

        Token RESETCONCENTRATIONS28=null;

        try {
            // BNGGrammar_Actions.g:242:9: ( RESETCONCENTRATIONS LPAREN ( DBQUOTES STRING DBQUOTES )? RPAREN ( SEMI )? -> action(id=$RESETCONCENTRATIONS.text))
            // BNGGrammar_Actions.g:242:11: RESETCONCENTRATIONS LPAREN ( DBQUOTES STRING DBQUOTES )? RPAREN ( SEMI )?
            {
            RESETCONCENTRATIONS28=(Token)match(input,RESETCONCENTRATIONS,FOLLOW_RESETCONCENTRATIONS_in_reset_concentrations2075); 
            match(input,LPAREN,FOLLOW_LPAREN_in_reset_concentrations2077); 
            // BNGGrammar_Actions.g:242:38: ( DBQUOTES STRING DBQUOTES )?
            int alt72=2;
            int LA72_0 = input.LA(1);

            if ( (LA72_0==DBQUOTES) ) {
                alt72=1;
            }
            switch (alt72) {
                case 1 :
                    // BNGGrammar_Actions.g:242:39: DBQUOTES STRING DBQUOTES
                    {
                    match(input,DBQUOTES,FOLLOW_DBQUOTES_in_reset_concentrations2080); 
                    match(input,STRING,FOLLOW_STRING_in_reset_concentrations2082); 
                    match(input,DBQUOTES,FOLLOW_DBQUOTES_in_reset_concentrations2084); 

                    }
                    break;

            }

            match(input,RPAREN,FOLLOW_RPAREN_in_reset_concentrations2088); 
            // BNGGrammar_Actions.g:242:73: ( SEMI )?
            int alt73=2;
            int LA73_0 = input.LA(1);

            if ( (LA73_0==SEMI) ) {
                alt73=1;
            }
            switch (alt73) {
                case 1 :
                    // BNGGrammar_Actions.g:242:73: SEMI
                    {
                    match(input,SEMI,FOLLOW_SEMI_in_reset_concentrations2090); 

                    }
                    break;

            }



            // TEMPLATE REWRITE
            // 242:79: -> action(id=$RESETCONCENTRATIONS.text)
            {
                retval.st = templateLib.getInstanceOf("action",
              new STAttrMap().put("id", (RESETCONCENTRATIONS28!=null?RESETCONCENTRATIONS28.getText():null)));
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
    // BNGGrammar_Actions.g:244:1: hash_value : STRING ASSIGNS LBRACKET assignment_list RBRACKET ;
    public final BNGGrammar_BNGGrammar_Actions.hash_value_return hash_value() throws RecognitionException {
        BNGGrammar_BNGGrammar_Actions.hash_value_return retval = new BNGGrammar_BNGGrammar_Actions.hash_value_return();
        retval.start = input.LT(1);

        try {
            // BNGGrammar_Actions.g:245:9: ( STRING ASSIGNS LBRACKET assignment_list RBRACKET )
            // BNGGrammar_Actions.g:245:11: STRING ASSIGNS LBRACKET assignment_list RBRACKET
            {
            match(input,STRING,FOLLOW_STRING_in_hash_value2124); 
            match(input,ASSIGNS,FOLLOW_ASSIGNS_in_hash_value2126); 
            match(input,LBRACKET,FOLLOW_LBRACKET_in_hash_value2128); 
            pushFollow(FOLLOW_assignment_list_in_hash_value2130);
            assignment_list();

            state._fsp--;

            match(input,RBRACKET,FOLLOW_RBRACKET_in_hash_value2132); 

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
    // BNGGrammar_Actions.g:247:1: assignment_list : STRING ASSIGNS value ( COMMA STRING ASSIGNS value ) ;
    public final BNGGrammar_BNGGrammar_Actions.assignment_list_return assignment_list() throws RecognitionException {
        BNGGrammar_BNGGrammar_Actions.assignment_list_return retval = new BNGGrammar_BNGGrammar_Actions.assignment_list_return();
        retval.start = input.LT(1);

        try {
            // BNGGrammar_Actions.g:248:9: ( STRING ASSIGNS value ( COMMA STRING ASSIGNS value ) )
            // BNGGrammar_Actions.g:248:11: STRING ASSIGNS value ( COMMA STRING ASSIGNS value )
            {
            match(input,STRING,FOLLOW_STRING_in_assignment_list2156); 
            match(input,ASSIGNS,FOLLOW_ASSIGNS_in_assignment_list2158); 
            pushFollow(FOLLOW_value_in_assignment_list2160);
            value();

            state._fsp--;

            // BNGGrammar_Actions.g:248:32: ( COMMA STRING ASSIGNS value )
            // BNGGrammar_Actions.g:248:33: COMMA STRING ASSIGNS value
            {
            match(input,COMMA,FOLLOW_COMMA_in_assignment_list2163); 
            match(input,STRING,FOLLOW_STRING_in_assignment_list2165); 
            match(input,ASSIGNS,FOLLOW_ASSIGNS_in_assignment_list2167); 
            pushFollow(FOLLOW_value_in_assignment_list2169);
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
    // BNGGrammar_Actions.g:250:1: value : ( INT | FLOAT | STRING );
    public final BNGGrammar_BNGGrammar_Actions.value_return value() throws RecognitionException {
        BNGGrammar_BNGGrammar_Actions.value_return retval = new BNGGrammar_BNGGrammar_Actions.value_return();
        retval.start = input.LT(1);

        try {
            // BNGGrammar_Actions.g:250:9: ( INT | FLOAT | STRING )
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
    // BNGGrammar_Actions.g:252:1: ps_par_def[Map<String,String> map] : ( PREFFIX ASSIGNS ( ( DBQUOTES (~ ( DBQUOTES ) )* DBQUOTES ) ) | SUFFIX ASSIGNS ( ( DBQUOTES (~ ( DBQUOTES ) )* DBQUOTES ) ) );
    public final BNGGrammar_BNGGrammar_Actions.ps_par_def_return ps_par_def(Map<String,String> map) throws RecognitionException {
        BNGGrammar_BNGGrammar_Actions.ps_par_def_return retval = new BNGGrammar_BNGGrammar_Actions.ps_par_def_return();
        retval.start = input.LT(1);

        try {
            // BNGGrammar_Actions.g:253:9: ( PREFFIX ASSIGNS ( ( DBQUOTES (~ ( DBQUOTES ) )* DBQUOTES ) ) | SUFFIX ASSIGNS ( ( DBQUOTES (~ ( DBQUOTES ) )* DBQUOTES ) ) )
            int alt76=2;
            int LA76_0 = input.LA(1);

            if ( (LA76_0==PREFFIX) ) {
                alt76=1;
            }
            else if ( (LA76_0==SUFFIX) ) {
                alt76=2;
            }
            else {
                NoViableAltException nvae =
                    new NoViableAltException("", 76, 0, input);

                throw nvae;
            }
            switch (alt76) {
                case 1 :
                    // BNGGrammar_Actions.g:253:11: PREFFIX ASSIGNS ( ( DBQUOTES (~ ( DBQUOTES ) )* DBQUOTES ) )
                    {
                    match(input,PREFFIX,FOLLOW_PREFFIX_in_ps_par_def2221); 
                    match(input,ASSIGNS,FOLLOW_ASSIGNS_in_ps_par_def2223); 
                    // BNGGrammar_Actions.g:253:27: ( ( DBQUOTES (~ ( DBQUOTES ) )* DBQUOTES ) )
                    // BNGGrammar_Actions.g:253:28: ( DBQUOTES (~ ( DBQUOTES ) )* DBQUOTES )
                    {
                    // BNGGrammar_Actions.g:253:28: ( DBQUOTES (~ ( DBQUOTES ) )* DBQUOTES )
                    // BNGGrammar_Actions.g:253:29: DBQUOTES (~ ( DBQUOTES ) )* DBQUOTES
                    {
                    match(input,DBQUOTES,FOLLOW_DBQUOTES_in_ps_par_def2227); 
                    // BNGGrammar_Actions.g:253:39: (~ ( DBQUOTES ) )*
                    loop74:
                    do {
                        int alt74=2;
                        int LA74_0 = input.LA(1);

                        if ( ((LA74_0>=LINE_COMMENT && LA74_0<=EMARK)||(LA74_0>=AMPERSAND && LA74_0<=275)) ) {
                            alt74=1;
                        }


                        switch (alt74) {
                    	case 1 :
                    	    // BNGGrammar_Actions.g:253:39: ~ ( DBQUOTES )
                    	    {
                    	    if ( (input.LA(1)>=LINE_COMMENT && input.LA(1)<=EMARK)||(input.LA(1)>=AMPERSAND && input.LA(1)<=275) ) {
                    	        input.consume();
                    	        state.errorRecovery=false;
                    	    }
                    	    else {
                    	        MismatchedSetException mse = new MismatchedSetException(null,input);
                    	        throw mse;
                    	    }


                    	    }
                    	    break;

                    	default :
                    	    break loop74;
                        }
                    } while (true);

                    match(input,DBQUOTES,FOLLOW_DBQUOTES_in_ps_par_def2237); 

                    }


                    }


                    }
                    break;
                case 2 :
                    // BNGGrammar_Actions.g:254:11: SUFFIX ASSIGNS ( ( DBQUOTES (~ ( DBQUOTES ) )* DBQUOTES ) )
                    {
                    match(input,SUFFIX,FOLLOW_SUFFIX_in_ps_par_def2251); 
                    match(input,ASSIGNS,FOLLOW_ASSIGNS_in_ps_par_def2253); 
                    // BNGGrammar_Actions.g:254:26: ( ( DBQUOTES (~ ( DBQUOTES ) )* DBQUOTES ) )
                    // BNGGrammar_Actions.g:254:27: ( DBQUOTES (~ ( DBQUOTES ) )* DBQUOTES )
                    {
                    // BNGGrammar_Actions.g:254:27: ( DBQUOTES (~ ( DBQUOTES ) )* DBQUOTES )
                    // BNGGrammar_Actions.g:254:28: DBQUOTES (~ ( DBQUOTES ) )* DBQUOTES
                    {
                    match(input,DBQUOTES,FOLLOW_DBQUOTES_in_ps_par_def2257); 
                    // BNGGrammar_Actions.g:254:38: (~ ( DBQUOTES ) )*
                    loop75:
                    do {
                        int alt75=2;
                        int LA75_0 = input.LA(1);

                        if ( ((LA75_0>=LINE_COMMENT && LA75_0<=EMARK)||(LA75_0>=AMPERSAND && LA75_0<=275)) ) {
                            alt75=1;
                        }


                        switch (alt75) {
                    	case 1 :
                    	    // BNGGrammar_Actions.g:254:38: ~ ( DBQUOTES )
                    	    {
                    	    if ( (input.LA(1)>=LINE_COMMENT && input.LA(1)<=EMARK)||(input.LA(1)>=AMPERSAND && input.LA(1)<=275) ) {
                    	        input.consume();
                    	        state.errorRecovery=false;
                    	    }
                    	    else {
                    	        MismatchedSetException mse = new MismatchedSetException(null,input);
                    	        throw mse;
                    	    }


                    	    }
                    	    break;

                    	default :
                    	    break loop75;
                        }
                    } while (true);

                    match(input,DBQUOTES,FOLLOW_DBQUOTES_in_ps_par_def2267); 

                    }


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
    // BNGGrammar_Actions.g:256:1: simulate_ode_par_def[Map<String,String> map] : ( ATOL ASSIGNS f1= FLOAT | RTOL ASSIGNS f2= FLOAT | STEADY_STATE ASSIGNS i1= INT | SPARSE ASSIGNS i2= INT );
    public final BNGGrammar_BNGGrammar_Actions.simulate_ode_par_def_return simulate_ode_par_def(Map<String,String> map) throws RecognitionException {
        BNGGrammar_BNGGrammar_Actions.simulate_ode_par_def_return retval = new BNGGrammar_BNGGrammar_Actions.simulate_ode_par_def_return();
        retval.start = input.LT(1);

        Token f1=null;
        Token f2=null;
        Token i1=null;
        Token i2=null;
        Token ATOL29=null;
        Token RTOL30=null;
        Token STEADY_STATE31=null;
        Token SPARSE32=null;

        try {
            // BNGGrammar_Actions.g:257:9: ( ATOL ASSIGNS f1= FLOAT | RTOL ASSIGNS f2= FLOAT | STEADY_STATE ASSIGNS i1= INT | SPARSE ASSIGNS i2= INT )
            int alt77=4;
            switch ( input.LA(1) ) {
            case ATOL:
                {
                alt77=1;
                }
                break;
            case RTOL:
                {
                alt77=2;
                }
                break;
            case STEADY_STATE:
                {
                alt77=3;
                }
                break;
            case SPARSE:
                {
                alt77=4;
                }
                break;
            default:
                NoViableAltException nvae =
                    new NoViableAltException("", 77, 0, input);

                throw nvae;
            }

            switch (alt77) {
                case 1 :
                    // BNGGrammar_Actions.g:257:11: ATOL ASSIGNS f1= FLOAT
                    {
                    ATOL29=(Token)match(input,ATOL,FOLLOW_ATOL_in_simulate_ode_par_def2294); 
                    match(input,ASSIGNS,FOLLOW_ASSIGNS_in_simulate_ode_par_def2296); 
                    f1=(Token)match(input,FLOAT,FOLLOW_FLOAT_in_simulate_ode_par_def2300); 
                    map.put((ATOL29!=null?ATOL29.getText():null),(f1!=null?f1.getText():null));

                    }
                    break;
                case 2 :
                    // BNGGrammar_Actions.g:258:11: RTOL ASSIGNS f2= FLOAT
                    {
                    RTOL30=(Token)match(input,RTOL,FOLLOW_RTOL_in_simulate_ode_par_def2314); 
                    match(input,ASSIGNS,FOLLOW_ASSIGNS_in_simulate_ode_par_def2316); 
                    f2=(Token)match(input,FLOAT,FOLLOW_FLOAT_in_simulate_ode_par_def2320); 
                    map.put((RTOL30!=null?RTOL30.getText():null),(f2!=null?f2.getText():null));

                    }
                    break;
                case 3 :
                    // BNGGrammar_Actions.g:259:11: STEADY_STATE ASSIGNS i1= INT
                    {
                    STEADY_STATE31=(Token)match(input,STEADY_STATE,FOLLOW_STEADY_STATE_in_simulate_ode_par_def2334); 
                    match(input,ASSIGNS,FOLLOW_ASSIGNS_in_simulate_ode_par_def2336); 
                    i1=(Token)match(input,INT,FOLLOW_INT_in_simulate_ode_par_def2340); 
                    map.put((STEADY_STATE31!=null?STEADY_STATE31.getText():null),(i1!=null?i1.getText():null));

                    }
                    break;
                case 4 :
                    // BNGGrammar_Actions.g:260:11: SPARSE ASSIGNS i2= INT
                    {
                    SPARSE32=(Token)match(input,SPARSE,FOLLOW_SPARSE_in_simulate_ode_par_def2354); 
                    match(input,ASSIGNS,FOLLOW_ASSIGNS_in_simulate_ode_par_def2356); 
                    i2=(Token)match(input,INT,FOLLOW_INT_in_simulate_ode_par_def2360); 
                    map.put((SPARSE32!=null?SPARSE32.getText():null),(i2!=null?i2.getText():null));

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

    protected static class simulate_pla_par_def_scope {
        String temp;
    }
    protected Stack simulate_pla_par_def_stack = new Stack();

    public static class simulate_pla_par_def_return extends ParserRuleReturnScope {
        public StringTemplate st;
        public Object getTemplate() { return st; }
        public String toString() { return st==null?null:st.toString(); }
    };

    // $ANTLR start "simulate_pla_par_def"
    // BNGGrammar_Actions.g:263:1: simulate_pla_par_def[Map<String,String> map] : PLA_CONFIG ASSIGNS DBQUOTES (s1=~ ( DBQUOTES ) )* DBQUOTES ;
    public final BNGGrammar_BNGGrammar_Actions.simulate_pla_par_def_return simulate_pla_par_def(Map<String,String> map) throws RecognitionException {
        simulate_pla_par_def_stack.push(new simulate_pla_par_def_scope());
        BNGGrammar_BNGGrammar_Actions.simulate_pla_par_def_return retval = new BNGGrammar_BNGGrammar_Actions.simulate_pla_par_def_return();
        retval.start = input.LT(1);

        Token s1=null;
        Token PLA_CONFIG33=null;


          ((simulate_pla_par_def_scope)simulate_pla_par_def_stack.peek()).temp = "";

        try {
            // BNGGrammar_Actions.g:270:9: ( PLA_CONFIG ASSIGNS DBQUOTES (s1=~ ( DBQUOTES ) )* DBQUOTES )
            // BNGGrammar_Actions.g:270:11: PLA_CONFIG ASSIGNS DBQUOTES (s1=~ ( DBQUOTES ) )* DBQUOTES
            {
            PLA_CONFIG33=(Token)match(input,PLA_CONFIG,FOLLOW_PLA_CONFIG_in_simulate_pla_par_def2403); 
            match(input,ASSIGNS,FOLLOW_ASSIGNS_in_simulate_pla_par_def2405); 
            match(input,DBQUOTES,FOLLOW_DBQUOTES_in_simulate_pla_par_def2407); 
            // BNGGrammar_Actions.g:270:39: (s1=~ ( DBQUOTES ) )*
            loop78:
            do {
                int alt78=2;
                int LA78_0 = input.LA(1);

                if ( ((LA78_0>=LINE_COMMENT && LA78_0<=EMARK)||(LA78_0>=AMPERSAND && LA78_0<=275)) ) {
                    alt78=1;
                }


                switch (alt78) {
            	case 1 :
            	    // BNGGrammar_Actions.g:270:40: s1=~ ( DBQUOTES )
            	    {
            	    s1=(Token)input.LT(1);
            	    if ( (input.LA(1)>=LINE_COMMENT && input.LA(1)<=EMARK)||(input.LA(1)>=AMPERSAND && input.LA(1)<=275) ) {
            	        input.consume();
            	        state.errorRecovery=false;
            	    }
            	    else {
            	        MismatchedSetException mse = new MismatchedSetException(null,input);
            	        throw mse;
            	    }

            	    ((simulate_pla_par_def_scope)simulate_pla_par_def_stack.peek()).temp += (s1!=null?s1.getText():null);

            	    }
            	    break;

            	default :
            	    break loop78;
                }
            } while (true);

            match(input,DBQUOTES,FOLLOW_DBQUOTES_in_simulate_pla_par_def2421); 
            map.put((PLA_CONFIG33!=null?PLA_CONFIG33.getText():null),((simulate_pla_par_def_scope)simulate_pla_par_def_stack.peek()).temp);

            }

            retval.stop = input.LT(-1);

        }
        catch (RecognitionException re) {
            reportError(re);
            recover(input,re);
        }
        finally {
            simulate_pla_par_def_stack.pop();
        }
        return retval;
    }
    // $ANTLR end "simulate_pla_par_def"

    public static class simulate_nf_par_def_return extends ParserRuleReturnScope {
        public StringTemplate st;
        public Object getTemplate() { return st; }
        public String toString() { return st==null?null:st.toString(); }
    };

    // $ANTLR start "simulate_nf_par_def"
    // BNGGrammar_Actions.g:273:1: simulate_nf_par_def[Map<String,String> map] : ( PARAM ASSIGNS DBQUOTES ( MINUS s1= STRING s2= ( STRING | INT | DOUBLE ) )* DBQUOTES | COMPLEX ASSIGNS i1= INT | GET_FINAL_STATE ASSIGNS i2= INT | GML ASSIGNS i3= INT );
    public final BNGGrammar_BNGGrammar_Actions.simulate_nf_par_def_return simulate_nf_par_def(Map<String,String> map) throws RecognitionException {
        BNGGrammar_BNGGrammar_Actions.simulate_nf_par_def_return retval = new BNGGrammar_BNGGrammar_Actions.simulate_nf_par_def_return();
        retval.start = input.LT(1);

        Token s1=null;
        Token s2=null;
        Token i1=null;
        Token i2=null;
        Token i3=null;
        Token PARAM34=null;
        Token COMPLEX35=null;
        Token GET_FINAL_STATE36=null;
        Token GML37=null;

        try {
            // BNGGrammar_Actions.g:274:9: ( PARAM ASSIGNS DBQUOTES ( MINUS s1= STRING s2= ( STRING | INT | DOUBLE ) )* DBQUOTES | COMPLEX ASSIGNS i1= INT | GET_FINAL_STATE ASSIGNS i2= INT | GML ASSIGNS i3= INT )
            int alt80=4;
            switch ( input.LA(1) ) {
            case PARAM:
                {
                alt80=1;
                }
                break;
            case COMPLEX:
                {
                alt80=2;
                }
                break;
            case GET_FINAL_STATE:
                {
                alt80=3;
                }
                break;
            case GML:
                {
                alt80=4;
                }
                break;
            default:
                NoViableAltException nvae =
                    new NoViableAltException("", 80, 0, input);

                throw nvae;
            }

            switch (alt80) {
                case 1 :
                    // BNGGrammar_Actions.g:274:11: PARAM ASSIGNS DBQUOTES ( MINUS s1= STRING s2= ( STRING | INT | DOUBLE ) )* DBQUOTES
                    {
                    PARAM34=(Token)match(input,PARAM,FOLLOW_PARAM_in_simulate_nf_par_def2454); 
                    match(input,ASSIGNS,FOLLOW_ASSIGNS_in_simulate_nf_par_def2456); 
                    match(input,DBQUOTES,FOLLOW_DBQUOTES_in_simulate_nf_par_def2458); 
                    // BNGGrammar_Actions.g:274:34: ( MINUS s1= STRING s2= ( STRING | INT | DOUBLE ) )*
                    loop79:
                    do {
                        int alt79=2;
                        int LA79_0 = input.LA(1);

                        if ( (LA79_0==MINUS) ) {
                            alt79=1;
                        }


                        switch (alt79) {
                    	case 1 :
                    	    // BNGGrammar_Actions.g:274:35: MINUS s1= STRING s2= ( STRING | INT | DOUBLE )
                    	    {
                    	    match(input,MINUS,FOLLOW_MINUS_in_simulate_nf_par_def2461); 
                    	    s1=(Token)match(input,STRING,FOLLOW_STRING_in_simulate_nf_par_def2465); 
                    	    s2=(Token)input.LT(1);
                    	    if ( input.LA(1)==INT||input.LA(1)==STRING||input.LA(1)==DOUBLE ) {
                    	        input.consume();
                    	        state.errorRecovery=false;
                    	    }
                    	    else {
                    	        MismatchedSetException mse = new MismatchedSetException(null,input);
                    	        throw mse;
                    	    }


                    	    }
                    	    break;

                    	default :
                    	    break loop79;
                        }
                    } while (true);

                    match(input,DBQUOTES,FOLLOW_DBQUOTES_in_simulate_nf_par_def2479); 
                    map.put((PARAM34!=null?PARAM34.getText():null),"-" + (s1!=null?s1.getText():null) + " " + (s2!=null?s2.getText():null));

                    }
                    break;
                case 2 :
                    // BNGGrammar_Actions.g:275:11: COMPLEX ASSIGNS i1= INT
                    {
                    COMPLEX35=(Token)match(input,COMPLEX,FOLLOW_COMPLEX_in_simulate_nf_par_def2494); 
                    match(input,ASSIGNS,FOLLOW_ASSIGNS_in_simulate_nf_par_def2496); 
                    i1=(Token)match(input,INT,FOLLOW_INT_in_simulate_nf_par_def2500); 
                    map.put((COMPLEX35!=null?COMPLEX35.getText():null),(i1!=null?i1.getText():null));

                    }
                    break;
                case 3 :
                    // BNGGrammar_Actions.g:276:11: GET_FINAL_STATE ASSIGNS i2= INT
                    {
                    GET_FINAL_STATE36=(Token)match(input,GET_FINAL_STATE,FOLLOW_GET_FINAL_STATE_in_simulate_nf_par_def2514); 
                    match(input,ASSIGNS,FOLLOW_ASSIGNS_in_simulate_nf_par_def2516); 
                    i2=(Token)match(input,INT,FOLLOW_INT_in_simulate_nf_par_def2520); 
                    map.put((GET_FINAL_STATE36!=null?GET_FINAL_STATE36.getText():null),(i2!=null?i2.getText():null));

                    }
                    break;
                case 4 :
                    // BNGGrammar_Actions.g:277:11: GML ASSIGNS i3= INT
                    {
                    GML37=(Token)match(input,GML,FOLLOW_GML_in_simulate_nf_par_def2534); 
                    match(input,ASSIGNS,FOLLOW_ASSIGNS_in_simulate_nf_par_def2536); 
                    i3=(Token)match(input,INT,FOLLOW_INT_in_simulate_nf_par_def2540); 
                    map.put((GML37!=null?GML37.getText():null),(i3!=null?i3.getText():null));

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
    // $ANTLR end "simulate_nf_par_def"

    public static class write_par_def_return extends ParserRuleReturnScope {
        public StringTemplate st;
        public Object getTemplate() { return st; }
        public String toString() { return st==null?null:st.toString(); }
    };

    // $ANTLR start "write_par_def"
    // BNGGrammar_Actions.g:281:1: write_par_def : SUFFIX ASSIGNS DBQUOTES (~ ( DBQUOTES ) )* DBQUOTES ;
    public final BNGGrammar_BNGGrammar_Actions.write_par_def_return write_par_def() throws RecognitionException {
        BNGGrammar_BNGGrammar_Actions.write_par_def_return retval = new BNGGrammar_BNGGrammar_Actions.write_par_def_return();
        retval.start = input.LT(1);

        try {
            // BNGGrammar_Actions.g:281:14: ( SUFFIX ASSIGNS DBQUOTES (~ ( DBQUOTES ) )* DBQUOTES )
            // BNGGrammar_Actions.g:282:3: SUFFIX ASSIGNS DBQUOTES (~ ( DBQUOTES ) )* DBQUOTES
            {
            match(input,SUFFIX,FOLLOW_SUFFIX_in_write_par_def2579); 
            match(input,ASSIGNS,FOLLOW_ASSIGNS_in_write_par_def2581); 
            match(input,DBQUOTES,FOLLOW_DBQUOTES_in_write_par_def2583); 
            // BNGGrammar_Actions.g:282:27: (~ ( DBQUOTES ) )*
            loop81:
            do {
                int alt81=2;
                int LA81_0 = input.LA(1);

                if ( ((LA81_0>=LINE_COMMENT && LA81_0<=EMARK)||(LA81_0>=AMPERSAND && LA81_0<=275)) ) {
                    alt81=1;
                }


                switch (alt81) {
            	case 1 :
            	    // BNGGrammar_Actions.g:282:27: ~ ( DBQUOTES )
            	    {
            	    if ( (input.LA(1)>=LINE_COMMENT && input.LA(1)<=EMARK)||(input.LA(1)>=AMPERSAND && input.LA(1)<=275) ) {
            	        input.consume();
            	        state.errorRecovery=false;
            	    }
            	    else {
            	        MismatchedSetException mse = new MismatchedSetException(null,input);
            	        throw mse;
            	    }


            	    }
            	    break;

            	default :
            	    break loop81;
                }
            } while (true);

            match(input,DBQUOTES,FOLLOW_DBQUOTES_in_write_par_def2592); 

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
    // $ANTLR end "write_par_def"

    public static class write_m_par_def_return extends ParserRuleReturnScope {
        public StringTemplate st;
        public Object getTemplate() { return st; }
        public String toString() { return st==null?null:st.toString(); }
    };

    // $ANTLR start "write_m_par_def"
    // BNGGrammar_Actions.g:285:1: write_m_par_def[Map<String,String> map] : ( ATOL ASSIGNS f1= FLOAT | RTOL ASSIGNS f2= FLOAT | T_END ASSIGNS (i1= INT | f1= FLOAT ) | T_START ASSIGNS (i2= INT | f2= FLOAT ) | N_STEPS ASSIGNS i3= INT | SPARSE ASSIGNS i4= INT | BDF ASSIGNS i5= INT );
    public final BNGGrammar_BNGGrammar_Actions.write_m_par_def_return write_m_par_def(Map<String,String> map) throws RecognitionException {
        BNGGrammar_BNGGrammar_Actions.write_m_par_def_return retval = new BNGGrammar_BNGGrammar_Actions.write_m_par_def_return();
        retval.start = input.LT(1);

        Token f1=null;
        Token f2=null;
        Token i1=null;
        Token i2=null;
        Token i3=null;
        Token i4=null;
        Token i5=null;
        Token ATOL38=null;
        Token RTOL39=null;
        Token T_END40=null;
        Token T_START41=null;
        Token N_STEPS42=null;
        Token SPARSE43=null;
        Token BDF44=null;

        try {
            // BNGGrammar_Actions.g:286:3: ( ATOL ASSIGNS f1= FLOAT | RTOL ASSIGNS f2= FLOAT | T_END ASSIGNS (i1= INT | f1= FLOAT ) | T_START ASSIGNS (i2= INT | f2= FLOAT ) | N_STEPS ASSIGNS i3= INT | SPARSE ASSIGNS i4= INT | BDF ASSIGNS i5= INT )
            int alt84=7;
            switch ( input.LA(1) ) {
            case ATOL:
                {
                alt84=1;
                }
                break;
            case RTOL:
                {
                alt84=2;
                }
                break;
            case T_END:
                {
                alt84=3;
                }
                break;
            case T_START:
                {
                alt84=4;
                }
                break;
            case N_STEPS:
                {
                alt84=5;
                }
                break;
            case SPARSE:
                {
                alt84=6;
                }
                break;
            case BDF:
                {
                alt84=7;
                }
                break;
            default:
                NoViableAltException nvae =
                    new NoViableAltException("", 84, 0, input);

                throw nvae;
            }

            switch (alt84) {
                case 1 :
                    // BNGGrammar_Actions.g:286:5: ATOL ASSIGNS f1= FLOAT
                    {
                    ATOL38=(Token)match(input,ATOL,FOLLOW_ATOL_in_write_m_par_def2613); 
                    match(input,ASSIGNS,FOLLOW_ASSIGNS_in_write_m_par_def2615); 
                    f1=(Token)match(input,FLOAT,FOLLOW_FLOAT_in_write_m_par_def2619); 
                    map.put((ATOL38!=null?ATOL38.getText():null),(f1!=null?f1.getText():null));

                    }
                    break;
                case 2 :
                    // BNGGrammar_Actions.g:287:5: RTOL ASSIGNS f2= FLOAT
                    {
                    RTOL39=(Token)match(input,RTOL,FOLLOW_RTOL_in_write_m_par_def2627); 
                    match(input,ASSIGNS,FOLLOW_ASSIGNS_in_write_m_par_def2629); 
                    f2=(Token)match(input,FLOAT,FOLLOW_FLOAT_in_write_m_par_def2633); 
                    map.put((RTOL39!=null?RTOL39.getText():null),(f2!=null?f2.getText():null));

                    }
                    break;
                case 3 :
                    // BNGGrammar_Actions.g:288:5: T_END ASSIGNS (i1= INT | f1= FLOAT )
                    {
                    T_END40=(Token)match(input,T_END,FOLLOW_T_END_in_write_m_par_def2641); 
                    match(input,ASSIGNS,FOLLOW_ASSIGNS_in_write_m_par_def2643); 
                    // BNGGrammar_Actions.g:288:19: (i1= INT | f1= FLOAT )
                    int alt82=2;
                    int LA82_0 = input.LA(1);

                    if ( (LA82_0==INT) ) {
                        alt82=1;
                    }
                    else if ( (LA82_0==FLOAT) ) {
                        alt82=2;
                    }
                    else {
                        NoViableAltException nvae =
                            new NoViableAltException("", 82, 0, input);

                        throw nvae;
                    }
                    switch (alt82) {
                        case 1 :
                            // BNGGrammar_Actions.g:288:20: i1= INT
                            {
                            i1=(Token)match(input,INT,FOLLOW_INT_in_write_m_par_def2648); 
                            map.put((T_END40!=null?T_END40.getText():null),(i1!=null?i1.getText():null));

                            }
                            break;
                        case 2 :
                            // BNGGrammar_Actions.g:288:60: f1= FLOAT
                            {
                            f1=(Token)match(input,FLOAT,FOLLOW_FLOAT_in_write_m_par_def2654); 
                            map.put((T_END40!=null?T_END40.getText():null),(f1!=null?f1.getText():null));

                            }
                            break;

                    }


                    }
                    break;
                case 4 :
                    // BNGGrammar_Actions.g:289:5: T_START ASSIGNS (i2= INT | f2= FLOAT )
                    {
                    T_START41=(Token)match(input,T_START,FOLLOW_T_START_in_write_m_par_def2664); 
                    match(input,ASSIGNS,FOLLOW_ASSIGNS_in_write_m_par_def2666); 
                    // BNGGrammar_Actions.g:289:21: (i2= INT | f2= FLOAT )
                    int alt83=2;
                    int LA83_0 = input.LA(1);

                    if ( (LA83_0==INT) ) {
                        alt83=1;
                    }
                    else if ( (LA83_0==FLOAT) ) {
                        alt83=2;
                    }
                    else {
                        NoViableAltException nvae =
                            new NoViableAltException("", 83, 0, input);

                        throw nvae;
                    }
                    switch (alt83) {
                        case 1 :
                            // BNGGrammar_Actions.g:289:22: i2= INT
                            {
                            i2=(Token)match(input,INT,FOLLOW_INT_in_write_m_par_def2671); 
                            map.put((T_START41!=null?T_START41.getText():null),(i2!=null?i2.getText():null));

                            }
                            break;
                        case 2 :
                            // BNGGrammar_Actions.g:289:64: f2= FLOAT
                            {
                            f2=(Token)match(input,FLOAT,FOLLOW_FLOAT_in_write_m_par_def2677); 
                            map.put((T_START41!=null?T_START41.getText():null),(f2!=null?f2.getText():null));

                            }
                            break;

                    }


                    }
                    break;
                case 5 :
                    // BNGGrammar_Actions.g:290:5: N_STEPS ASSIGNS i3= INT
                    {
                    N_STEPS42=(Token)match(input,N_STEPS,FOLLOW_N_STEPS_in_write_m_par_def2686); 
                    match(input,ASSIGNS,FOLLOW_ASSIGNS_in_write_m_par_def2688); 
                    i3=(Token)match(input,INT,FOLLOW_INT_in_write_m_par_def2692); 
                    map.put((N_STEPS42!=null?N_STEPS42.getText():null),(i3!=null?i3.getText():null));

                    }
                    break;
                case 6 :
                    // BNGGrammar_Actions.g:291:5: SPARSE ASSIGNS i4= INT
                    {
                    SPARSE43=(Token)match(input,SPARSE,FOLLOW_SPARSE_in_write_m_par_def2700); 
                    match(input,ASSIGNS,FOLLOW_ASSIGNS_in_write_m_par_def2702); 
                    i4=(Token)match(input,INT,FOLLOW_INT_in_write_m_par_def2706); 
                    map.put((SPARSE43!=null?SPARSE43.getText():null),(i4!=null?i4.getText():null));

                    }
                    break;
                case 7 :
                    // BNGGrammar_Actions.g:292:5: BDF ASSIGNS i5= INT
                    {
                    BDF44=(Token)match(input,BDF,FOLLOW_BDF_in_write_m_par_def2714); 
                    match(input,ASSIGNS,FOLLOW_ASSIGNS_in_write_m_par_def2716); 
                    i5=(Token)match(input,INT,FOLLOW_INT_in_write_m_par_def2720); 
                    map.put((BDF44!=null?BDF44.getText():null),(i5!=null?i5.getText():null));

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
    // $ANTLR end "write_m_par_def"

    public static class simulate_par_def_return extends ParserRuleReturnScope {
        public StringTemplate st;
        public Object getTemplate() { return st; }
        public String toString() { return st==null?null:st.toString(); }
    };

    // $ANTLR start "simulate_par_def"
    // BNGGrammar_Actions.g:296:1: simulate_par_def[Map<String,String> map] : ( T_END ASSIGNS (i1= INT | f1= FLOAT ) | T_START ASSIGNS (i2= INT | f2= FLOAT ) | N_STEPS ASSIGNS i3= ( INT | FLOAT ) | SAMPLE_TIMES ASSIGNS i4= array_value | VERBOSE ASSIGNS i5= INT | PRINT_CDAT ASSIGNS i6= INT | PRINT_FUNCTIONS ASSIGNS i7= INT | N_OUTPUT_STEPS ASSIGNS multiple_definition | MAX_SIM_STEPS ASSIGNS multiple_definition | ARGFILE ASSIGNS DBQUOTES s3= filename DBQUOTES | SAVE_PROGRESS ASSIGNS i10= INT | PRINT_NET ASSIGNS i11= INT | PRINT_END ASSIGNS i12= INT | NETFILE ASSIGNS DBQUOTES s4= filename DBQUOTES | METHOD ASSIGNS method_definition | CONTINUE ASSIGNS i13= INT | EVALUATE_EXPRESSIONS ASSIGNS i14= INT | OVERWRITE ASSIGNS i15= INT );
    public final BNGGrammar_BNGGrammar_Actions.simulate_par_def_return simulate_par_def(Map<String,String> map) throws RecognitionException {
        BNGGrammar_BNGGrammar_Actions.simulate_par_def_return retval = new BNGGrammar_BNGGrammar_Actions.simulate_par_def_return();
        retval.start = input.LT(1);

        Token i1=null;
        Token f1=null;
        Token i2=null;
        Token f2=null;
        Token i3=null;
        Token i5=null;
        Token i6=null;
        Token i7=null;
        Token i10=null;
        Token i11=null;
        Token i12=null;
        Token i13=null;
        Token i14=null;
        Token i15=null;
        Token T_END45=null;
        Token T_START46=null;
        Token N_STEPS47=null;
        Token VERBOSE48=null;
        Token PRINT_CDAT49=null;
        Token PRINT_FUNCTIONS50=null;
        Token N_OUTPUT_STEPS51=null;
        Token MAX_SIM_STEPS53=null;
        Token ARGFILE55=null;
        Token SAVE_PROGRESS56=null;
        Token PRINT_NET57=null;
        Token PRINT_END58=null;
        Token NETFILE59=null;
        Token METHOD60=null;
        Token CONTINUE62=null;
        Token EVALUATE_EXPRESSIONS63=null;
        Token OVERWRITE64=null;
        BNGGrammar_BNGGrammar_Actions.array_value_return i4 = null;

        BNGGrammar_BNGGrammar_Actions.filename_return s3 = null;

        BNGGrammar_BNGGrammar_Actions.filename_return s4 = null;

        BNGGrammar_BNGGrammar_Actions.multiple_definition_return multiple_definition52 = null;

        BNGGrammar_BNGGrammar_Actions.multiple_definition_return multiple_definition54 = null;

        BNGGrammar_BNGGrammar_Actions.method_definition_return method_definition61 = null;


        try {
            // BNGGrammar_Actions.g:297:9: ( T_END ASSIGNS (i1= INT | f1= FLOAT ) | T_START ASSIGNS (i2= INT | f2= FLOAT ) | N_STEPS ASSIGNS i3= ( INT | FLOAT ) | SAMPLE_TIMES ASSIGNS i4= array_value | VERBOSE ASSIGNS i5= INT | PRINT_CDAT ASSIGNS i6= INT | PRINT_FUNCTIONS ASSIGNS i7= INT | N_OUTPUT_STEPS ASSIGNS multiple_definition | MAX_SIM_STEPS ASSIGNS multiple_definition | ARGFILE ASSIGNS DBQUOTES s3= filename DBQUOTES | SAVE_PROGRESS ASSIGNS i10= INT | PRINT_NET ASSIGNS i11= INT | PRINT_END ASSIGNS i12= INT | NETFILE ASSIGNS DBQUOTES s4= filename DBQUOTES | METHOD ASSIGNS method_definition | CONTINUE ASSIGNS i13= INT | EVALUATE_EXPRESSIONS ASSIGNS i14= INT | OVERWRITE ASSIGNS i15= INT )
            int alt87=18;
            switch ( input.LA(1) ) {
            case T_END:
                {
                alt87=1;
                }
                break;
            case T_START:
                {
                alt87=2;
                }
                break;
            case N_STEPS:
                {
                alt87=3;
                }
                break;
            case SAMPLE_TIMES:
                {
                alt87=4;
                }
                break;
            case VERBOSE:
                {
                alt87=5;
                }
                break;
            case PRINT_CDAT:
                {
                alt87=6;
                }
                break;
            case PRINT_FUNCTIONS:
                {
                alt87=7;
                }
                break;
            case N_OUTPUT_STEPS:
                {
                alt87=8;
                }
                break;
            case MAX_SIM_STEPS:
                {
                alt87=9;
                }
                break;
            case ARGFILE:
                {
                alt87=10;
                }
                break;
            case SAVE_PROGRESS:
                {
                alt87=11;
                }
                break;
            case PRINT_NET:
                {
                alt87=12;
                }
                break;
            case PRINT_END:
                {
                alt87=13;
                }
                break;
            case NETFILE:
                {
                alt87=14;
                }
                break;
            case METHOD:
                {
                alt87=15;
                }
                break;
            case CONTINUE:
                {
                alt87=16;
                }
                break;
            case EVALUATE_EXPRESSIONS:
                {
                alt87=17;
                }
                break;
            case OVERWRITE:
                {
                alt87=18;
                }
                break;
            default:
                NoViableAltException nvae =
                    new NoViableAltException("", 87, 0, input);

                throw nvae;
            }

            switch (alt87) {
                case 1 :
                    // BNGGrammar_Actions.g:297:11: T_END ASSIGNS (i1= INT | f1= FLOAT )
                    {
                    T_END45=(Token)match(input,T_END,FOLLOW_T_END_in_simulate_par_def2743); 
                    match(input,ASSIGNS,FOLLOW_ASSIGNS_in_simulate_par_def2745); 
                    // BNGGrammar_Actions.g:297:25: (i1= INT | f1= FLOAT )
                    int alt85=2;
                    int LA85_0 = input.LA(1);

                    if ( (LA85_0==INT) ) {
                        alt85=1;
                    }
                    else if ( (LA85_0==FLOAT) ) {
                        alt85=2;
                    }
                    else {
                        NoViableAltException nvae =
                            new NoViableAltException("", 85, 0, input);

                        throw nvae;
                    }
                    switch (alt85) {
                        case 1 :
                            // BNGGrammar_Actions.g:297:26: i1= INT
                            {
                            i1=(Token)match(input,INT,FOLLOW_INT_in_simulate_par_def2750); 
                            map.put((T_END45!=null?T_END45.getText():null),(i1!=null?i1.getText():null));

                            }
                            break;
                        case 2 :
                            // BNGGrammar_Actions.g:297:66: f1= FLOAT
                            {
                            f1=(Token)match(input,FLOAT,FOLLOW_FLOAT_in_simulate_par_def2756); 
                            map.put((T_END45!=null?T_END45.getText():null),(f1!=null?f1.getText():null));

                            }
                            break;

                    }


                    }
                    break;
                case 2 :
                    // BNGGrammar_Actions.g:298:11: T_START ASSIGNS (i2= INT | f2= FLOAT )
                    {
                    T_START46=(Token)match(input,T_START,FOLLOW_T_START_in_simulate_par_def2772); 
                    match(input,ASSIGNS,FOLLOW_ASSIGNS_in_simulate_par_def2774); 
                    // BNGGrammar_Actions.g:298:27: (i2= INT | f2= FLOAT )
                    int alt86=2;
                    int LA86_0 = input.LA(1);

                    if ( (LA86_0==INT) ) {
                        alt86=1;
                    }
                    else if ( (LA86_0==FLOAT) ) {
                        alt86=2;
                    }
                    else {
                        NoViableAltException nvae =
                            new NoViableAltException("", 86, 0, input);

                        throw nvae;
                    }
                    switch (alt86) {
                        case 1 :
                            // BNGGrammar_Actions.g:298:28: i2= INT
                            {
                            i2=(Token)match(input,INT,FOLLOW_INT_in_simulate_par_def2779); 
                            map.put((T_START46!=null?T_START46.getText():null),(i2!=null?i2.getText():null));

                            }
                            break;
                        case 2 :
                            // BNGGrammar_Actions.g:298:70: f2= FLOAT
                            {
                            f2=(Token)match(input,FLOAT,FOLLOW_FLOAT_in_simulate_par_def2785); 
                            map.put((T_START46!=null?T_START46.getText():null),(f2!=null?f2.getText():null));

                            }
                            break;

                    }


                    }
                    break;
                case 3 :
                    // BNGGrammar_Actions.g:299:11: N_STEPS ASSIGNS i3= ( INT | FLOAT )
                    {
                    N_STEPS47=(Token)match(input,N_STEPS,FOLLOW_N_STEPS_in_simulate_par_def2800); 
                    match(input,ASSIGNS,FOLLOW_ASSIGNS_in_simulate_par_def2802); 
                    i3=(Token)input.LT(1);
                    if ( (input.LA(1)>=FLOAT && input.LA(1)<=INT) ) {
                        input.consume();
                        state.errorRecovery=false;
                    }
                    else {
                        MismatchedSetException mse = new MismatchedSetException(null,input);
                        throw mse;
                    }

                    map.put((N_STEPS47!=null?N_STEPS47.getText():null),(i3!=null?i3.getText():null));

                    }
                    break;
                case 4 :
                    // BNGGrammar_Actions.g:300:11: SAMPLE_TIMES ASSIGNS i4= array_value
                    {
                    match(input,SAMPLE_TIMES,FOLLOW_SAMPLE_TIMES_in_simulate_par_def2824); 
                    match(input,ASSIGNS,FOLLOW_ASSIGNS_in_simulate_par_def2826); 
                    pushFollow(FOLLOW_array_value_in_simulate_par_def2830);
                    i4=array_value();

                    state._fsp--;


                    }
                    break;
                case 5 :
                    // BNGGrammar_Actions.g:301:11: VERBOSE ASSIGNS i5= INT
                    {
                    VERBOSE48=(Token)match(input,VERBOSE,FOLLOW_VERBOSE_in_simulate_par_def2843); 
                    match(input,ASSIGNS,FOLLOW_ASSIGNS_in_simulate_par_def2845); 
                    i5=(Token)match(input,INT,FOLLOW_INT_in_simulate_par_def2849); 
                    map.put((VERBOSE48!=null?VERBOSE48.getText():null),(i5!=null?i5.getText():null));

                    }
                    break;
                case 6 :
                    // BNGGrammar_Actions.g:302:11: PRINT_CDAT ASSIGNS i6= INT
                    {
                    PRINT_CDAT49=(Token)match(input,PRINT_CDAT,FOLLOW_PRINT_CDAT_in_simulate_par_def2863); 
                    match(input,ASSIGNS,FOLLOW_ASSIGNS_in_simulate_par_def2865); 
                    i6=(Token)match(input,INT,FOLLOW_INT_in_simulate_par_def2869); 
                    map.put((PRINT_CDAT49!=null?PRINT_CDAT49.getText():null),(i6!=null?i6.getText():null));

                    }
                    break;
                case 7 :
                    // BNGGrammar_Actions.g:303:11: PRINT_FUNCTIONS ASSIGNS i7= INT
                    {
                    PRINT_FUNCTIONS50=(Token)match(input,PRINT_FUNCTIONS,FOLLOW_PRINT_FUNCTIONS_in_simulate_par_def2883); 
                    match(input,ASSIGNS,FOLLOW_ASSIGNS_in_simulate_par_def2885); 
                    i7=(Token)match(input,INT,FOLLOW_INT_in_simulate_par_def2889); 
                    map.put((PRINT_FUNCTIONS50!=null?PRINT_FUNCTIONS50.getText():null),(i7!=null?i7.getText():null));

                    }
                    break;
                case 8 :
                    // BNGGrammar_Actions.g:304:11: N_OUTPUT_STEPS ASSIGNS multiple_definition
                    {
                    N_OUTPUT_STEPS51=(Token)match(input,N_OUTPUT_STEPS,FOLLOW_N_OUTPUT_STEPS_in_simulate_par_def2903); 
                    match(input,ASSIGNS,FOLLOW_ASSIGNS_in_simulate_par_def2905); 
                    pushFollow(FOLLOW_multiple_definition_in_simulate_par_def2907);
                    multiple_definition52=multiple_definition();

                    state._fsp--;

                    map.put((N_OUTPUT_STEPS51!=null?N_OUTPUT_STEPS51.getText():null),(multiple_definition52!=null?multiple_definition52.value:null));

                    }
                    break;
                case 9 :
                    // BNGGrammar_Actions.g:305:11: MAX_SIM_STEPS ASSIGNS multiple_definition
                    {
                    MAX_SIM_STEPS53=(Token)match(input,MAX_SIM_STEPS,FOLLOW_MAX_SIM_STEPS_in_simulate_par_def2921); 
                    match(input,ASSIGNS,FOLLOW_ASSIGNS_in_simulate_par_def2923); 
                    pushFollow(FOLLOW_multiple_definition_in_simulate_par_def2925);
                    multiple_definition54=multiple_definition();

                    state._fsp--;

                    map.put((MAX_SIM_STEPS53!=null?MAX_SIM_STEPS53.getText():null),(multiple_definition54!=null?multiple_definition54.value:null));

                    }
                    break;
                case 10 :
                    // BNGGrammar_Actions.g:306:11: ARGFILE ASSIGNS DBQUOTES s3= filename DBQUOTES
                    {
                    ARGFILE55=(Token)match(input,ARGFILE,FOLLOW_ARGFILE_in_simulate_par_def2939); 
                    match(input,ASSIGNS,FOLLOW_ASSIGNS_in_simulate_par_def2941); 
                    match(input,DBQUOTES,FOLLOW_DBQUOTES_in_simulate_par_def2943); 
                    pushFollow(FOLLOW_filename_in_simulate_par_def2947);
                    s3=filename();

                    state._fsp--;

                    match(input,DBQUOTES,FOLLOW_DBQUOTES_in_simulate_par_def2949); 
                    map.put((ARGFILE55!=null?ARGFILE55.getText():null),(s3!=null?input.toString(s3.start,s3.stop):null));

                    }
                    break;
                case 11 :
                    // BNGGrammar_Actions.g:307:11: SAVE_PROGRESS ASSIGNS i10= INT
                    {
                    SAVE_PROGRESS56=(Token)match(input,SAVE_PROGRESS,FOLLOW_SAVE_PROGRESS_in_simulate_par_def2963); 
                    match(input,ASSIGNS,FOLLOW_ASSIGNS_in_simulate_par_def2965); 
                    i10=(Token)match(input,INT,FOLLOW_INT_in_simulate_par_def2969); 
                    map.put((SAVE_PROGRESS56!=null?SAVE_PROGRESS56.getText():null),(i10!=null?i10.getText():null));

                    }
                    break;
                case 12 :
                    // BNGGrammar_Actions.g:308:11: PRINT_NET ASSIGNS i11= INT
                    {
                    PRINT_NET57=(Token)match(input,PRINT_NET,FOLLOW_PRINT_NET_in_simulate_par_def2983); 
                    match(input,ASSIGNS,FOLLOW_ASSIGNS_in_simulate_par_def2985); 
                    i11=(Token)match(input,INT,FOLLOW_INT_in_simulate_par_def2989); 
                    map.put((PRINT_NET57!=null?PRINT_NET57.getText():null),(i11!=null?i11.getText():null));

                    }
                    break;
                case 13 :
                    // BNGGrammar_Actions.g:309:11: PRINT_END ASSIGNS i12= INT
                    {
                    PRINT_END58=(Token)match(input,PRINT_END,FOLLOW_PRINT_END_in_simulate_par_def3003); 
                    match(input,ASSIGNS,FOLLOW_ASSIGNS_in_simulate_par_def3005); 
                    i12=(Token)match(input,INT,FOLLOW_INT_in_simulate_par_def3009); 
                    map.put((PRINT_END58!=null?PRINT_END58.getText():null),(i12!=null?i12.getText():null));

                    }
                    break;
                case 14 :
                    // BNGGrammar_Actions.g:310:11: NETFILE ASSIGNS DBQUOTES s4= filename DBQUOTES
                    {
                    NETFILE59=(Token)match(input,NETFILE,FOLLOW_NETFILE_in_simulate_par_def3027); 
                    match(input,ASSIGNS,FOLLOW_ASSIGNS_in_simulate_par_def3029); 
                    match(input,DBQUOTES,FOLLOW_DBQUOTES_in_simulate_par_def3031); 
                    pushFollow(FOLLOW_filename_in_simulate_par_def3035);
                    s4=filename();

                    state._fsp--;

                    match(input,DBQUOTES,FOLLOW_DBQUOTES_in_simulate_par_def3037); 
                    map.put((NETFILE59!=null?NETFILE59.getText():null),(s4!=null?input.toString(s4.start,s4.stop):null));

                    }
                    break;
                case 15 :
                    // BNGGrammar_Actions.g:311:11: METHOD ASSIGNS method_definition
                    {
                    METHOD60=(Token)match(input,METHOD,FOLLOW_METHOD_in_simulate_par_def3051); 
                    match(input,ASSIGNS,FOLLOW_ASSIGNS_in_simulate_par_def3053); 
                    pushFollow(FOLLOW_method_definition_in_simulate_par_def3055);
                    method_definition61=method_definition();

                    state._fsp--;

                    map.put((METHOD60!=null?METHOD60.getText():null),(method_definition61!=null?input.toString(method_definition61.start,method_definition61.stop):null));

                    }
                    break;
                case 16 :
                    // BNGGrammar_Actions.g:312:11: CONTINUE ASSIGNS i13= INT
                    {
                    CONTINUE62=(Token)match(input,CONTINUE,FOLLOW_CONTINUE_in_simulate_par_def3069); 
                    match(input,ASSIGNS,FOLLOW_ASSIGNS_in_simulate_par_def3071); 
                    i13=(Token)match(input,INT,FOLLOW_INT_in_simulate_par_def3075); 
                    map.put((CONTINUE62!=null?CONTINUE62.getText():null),(i13!=null?i13.getText():null));

                    }
                    break;
                case 17 :
                    // BNGGrammar_Actions.g:313:11: EVALUATE_EXPRESSIONS ASSIGNS i14= INT
                    {
                    EVALUATE_EXPRESSIONS63=(Token)match(input,EVALUATE_EXPRESSIONS,FOLLOW_EVALUATE_EXPRESSIONS_in_simulate_par_def3089); 
                    match(input,ASSIGNS,FOLLOW_ASSIGNS_in_simulate_par_def3091); 
                    i14=(Token)match(input,INT,FOLLOW_INT_in_simulate_par_def3095); 
                    map.put((EVALUATE_EXPRESSIONS63!=null?EVALUATE_EXPRESSIONS63.getText():null),(i14!=null?i14.getText():null));

                    }
                    break;
                case 18 :
                    // BNGGrammar_Actions.g:314:11: OVERWRITE ASSIGNS i15= INT
                    {
                    OVERWRITE64=(Token)match(input,OVERWRITE,FOLLOW_OVERWRITE_in_simulate_par_def3109); 
                    match(input,ASSIGNS,FOLLOW_ASSIGNS_in_simulate_par_def3111); 
                    i15=(Token)match(input,INT,FOLLOW_INT_in_simulate_par_def3115); 
                    map.put((OVERWRITE64!=null?OVERWRITE64.getText():null),(i15!=null?i15.getText():null));

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

    public static class multiple_definition_return extends ParserRuleReturnScope {
        public String value;
        public StringTemplate st;
        public Object getTemplate() { return st; }
        public String toString() { return st==null?null:st.toString(); }
    };

    // $ANTLR start "multiple_definition"
    // BNGGrammar_Actions.g:317:1: multiple_definition returns [String value] : ( INT | FLOAT | STRING );
    public final BNGGrammar_BNGGrammar_Actions.multiple_definition_return multiple_definition() throws RecognitionException {
        BNGGrammar_BNGGrammar_Actions.multiple_definition_return retval = new BNGGrammar_BNGGrammar_Actions.multiple_definition_return();
        retval.start = input.LT(1);

        Token INT65=null;
        Token FLOAT66=null;
        Token STRING67=null;

        try {
            // BNGGrammar_Actions.g:317:43: ( INT | FLOAT | STRING )
            int alt88=3;
            switch ( input.LA(1) ) {
            case INT:
                {
                alt88=1;
                }
                break;
            case FLOAT:
                {
                alt88=2;
                }
                break;
            case STRING:
                {
                alt88=3;
                }
                break;
            default:
                NoViableAltException nvae =
                    new NoViableAltException("", 88, 0, input);

                throw nvae;
            }

            switch (alt88) {
                case 1 :
                    // BNGGrammar_Actions.g:318:3: INT
                    {
                    INT65=(Token)match(input,INT,FOLLOW_INT_in_multiple_definition3147); 
                    retval.value =(INT65!=null?INT65.getText():null);

                    }
                    break;
                case 2 :
                    // BNGGrammar_Actions.g:318:28: FLOAT
                    {
                    FLOAT66=(Token)match(input,FLOAT,FOLLOW_FLOAT_in_multiple_definition3152); 
                    retval.value =(FLOAT66!=null?FLOAT66.getText():null);

                    }
                    break;
                case 3 :
                    // BNGGrammar_Actions.g:319:3: STRING
                    {
                    STRING67=(Token)match(input,STRING,FOLLOW_STRING_in_multiple_definition3160); 

                        if((STRING67!=null?STRING67.getText():null).equals("INT_MAX") || (STRING67!=null?STRING67.getText():null).equals("FLOAT_MAX"))
                          retval.value =(STRING67!=null?STRING67.getText():null);
                         else{
                          String err = String.format("%s line %d:%d %s\n",input.getSourceName(),STRING67.getLine(),STRING67.getCharPositionInLine(),"Input should be an INT,FLOAT, 'FLOAT_MAX' or 'INT_MAX'");
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
    // $ANTLR end "multiple_definition"

    public static class method_definition_return extends ParserRuleReturnScope {
        public StringTemplate st;
        public Object getTemplate() { return st; }
        public String toString() { return st==null?null:st.toString(); }
    };

    // $ANTLR start "method_definition"
    // BNGGrammar_Actions.g:327:1: method_definition : ( ODE | SSA | PLA | NF );
    public final BNGGrammar_BNGGrammar_Actions.method_definition_return method_definition() throws RecognitionException {
        BNGGrammar_BNGGrammar_Actions.method_definition_return retval = new BNGGrammar_BNGGrammar_Actions.method_definition_return();
        retval.start = input.LT(1);

        try {
            // BNGGrammar_Actions.g:327:19: ( ODE | SSA | PLA | NF )
            // BNGGrammar_Actions.g:
            {
            if ( (input.LA(1)>=ODE && input.LA(1)<=PLA) ) {
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
    // $ANTLR end "method_definition"

    public static class array_value_return extends ParserRuleReturnScope {
        public StringTemplate st;
        public Object getTemplate() { return st; }
        public String toString() { return st==null?null:st.toString(); }
    };

    // $ANTLR start "array_value"
    // BNGGrammar_Actions.g:330:1: array_value : LSBRACKET STRING ( COMMA STRING )* RSBRACKET ;
    public final BNGGrammar_BNGGrammar_Actions.array_value_return array_value() throws RecognitionException {
        BNGGrammar_BNGGrammar_Actions.array_value_return retval = new BNGGrammar_BNGGrammar_Actions.array_value_return();
        retval.start = input.LT(1);

        try {
            // BNGGrammar_Actions.g:331:9: ( LSBRACKET STRING ( COMMA STRING )* RSBRACKET )
            // BNGGrammar_Actions.g:331:11: LSBRACKET STRING ( COMMA STRING )* RSBRACKET
            {
            match(input,LSBRACKET,FOLLOW_LSBRACKET_in_array_value3199); 
            match(input,STRING,FOLLOW_STRING_in_array_value3201); 
            // BNGGrammar_Actions.g:331:28: ( COMMA STRING )*
            loop89:
            do {
                int alt89=2;
                int LA89_0 = input.LA(1);

                if ( (LA89_0==COMMA) ) {
                    alt89=1;
                }


                switch (alt89) {
            	case 1 :
            	    // BNGGrammar_Actions.g:331:29: COMMA STRING
            	    {
            	    match(input,COMMA,FOLLOW_COMMA_in_array_value3204); 
            	    match(input,STRING,FOLLOW_STRING_in_array_value3206); 

            	    }
            	    break;

            	default :
            	    break loop89;
                }
            } while (true);

            match(input,RSBRACKET,FOLLOW_RSBRACKET_in_array_value3210); 

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

    public static class filename_return extends ParserRuleReturnScope {
        public StringTemplate st;
        public Object getTemplate() { return st; }
        public String toString() { return st==null?null:st.toString(); }
    };

    // $ANTLR start "filename"
    // BNGGrammar_Actions.g:334:1: filename : ( STRING | DOT | DIV | MINUS )+ ;
    public final BNGGrammar_BNGGrammar_Actions.filename_return filename() throws RecognitionException {
        BNGGrammar_BNGGrammar_Actions.filename_return retval = new BNGGrammar_BNGGrammar_Actions.filename_return();
        retval.start = input.LT(1);

        try {
            // BNGGrammar_Actions.g:334:9: ( ( STRING | DOT | DIV | MINUS )+ )
            // BNGGrammar_Actions.g:335:1: ( STRING | DOT | DIV | MINUS )+
            {
            // BNGGrammar_Actions.g:335:1: ( STRING | DOT | DIV | MINUS )+
            int cnt90=0;
            loop90:
            do {
                int alt90=2;
                int LA90_0 = input.LA(1);

                if ( (LA90_0==STRING||LA90_0==DOT||LA90_0==DIV||LA90_0==MINUS) ) {
                    alt90=1;
                }


                switch (alt90) {
            	case 1 :
            	    // BNGGrammar_Actions.g:
            	    {
            	    if ( input.LA(1)==STRING||input.LA(1)==DOT||input.LA(1)==DIV||input.LA(1)==MINUS ) {
            	        input.consume();
            	        state.errorRecovery=false;
            	    }
            	    else {
            	        MismatchedSetException mse = new MismatchedSetException(null,input);
            	        throw mse;
            	    }


            	    }
            	    break;

            	default :
            	    if ( cnt90 >= 1 ) break loop90;
                        EarlyExitException eee =
                            new EarlyExitException(90, input);
                        throw eee;
                }
                cnt90++;
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
    // $ANTLR end "filename"

    // Delegated rules


    protected DFA1 dfa1 = new DFA1(this);
    static final String DFA1_eotS =
        "\23\uffff";
    static final String DFA1_eofS =
        "\23\uffff";
    static final String DFA1_minS =
        "\1\4\22\uffff";
    static final String DFA1_maxS =
        "\1\u0113\22\uffff";
    static final String DFA1_acceptS =
        "\1\uffff\21\2\1\1";
    static final String DFA1_specialS =
        "\23\uffff}>";
    static final String[] DFA1_transitionS = {
            "\5\22\1\1\72\22\1\3\1\4\1\11\1\10\1\12\1\13\1\22\2\14\1\5\1"+
            "\14\1\6\2\22\1\15\1\22\1\16\1\17\1\20\1\21\1\2\1\7\u00ba\22",
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
            return "()* loopback of 22:1: ( . )*";
        }
    }
 

    public static final BitSet FOLLOW_actions_block_in_actions_prog44 = new BitSet(new long[]{0x0000000000000000L});
    public static final BitSet FOLLOW_EOF_in_actions_prog50 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_BEGIN_in_actions_block96 = new BitSet(new long[]{0x0000000000400000L});
    public static final BitSet FOLLOW_ACTIONS_in_actions_block98 = new BitSet(new long[]{0x0000000000000020L});
    public static final BitSet FOLLOW_LB_in_actions_block100 = new BitSet(new long[]{0x0000000000000020L,0x0000000003F4FBF0L});
    public static final BitSet FOLLOW_action_in_actions_block114 = new BitSet(new long[]{0x0000000000000122L,0x0000000003F4FBF0L});
    public static final BitSet FOLLOW_LB_in_actions_block116 = new BitSet(new long[]{0x0000000000000122L,0x0000000003F4FBF0L});
    public static final BitSet FOLLOW_END_in_actions_block130 = new BitSet(new long[]{0x0000000000400000L});
    public static final BitSet FOLLOW_ACTIONS_in_actions_block132 = new BitSet(new long[]{0x0000000000000022L});
    public static final BitSet FOLLOW_LB_in_actions_block134 = new BitSet(new long[]{0x0000000000000022L});
    public static final BitSet FOLLOW_generate_network_in_action146 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_simulate_method_in_action160 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_read_file_in_action174 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_write_file_in_action188 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_set_concentration_in_action202 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_set_parameter_in_action216 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_save_concentrations_in_action230 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_reset_concentrations_in_action244 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_add_concentration_in_action258 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_GENERATENETWORK_in_generate_network291 = new BitSet(new long[]{0x0000000000000000L,0x1000000000000000L});
    public static final BitSet FOLLOW_LPAREN_in_generate_network293 = new BitSet(new long[]{0x0000000000000000L,0x2100000000000000L});
    public static final BitSet FOLLOW_LBRACKET_in_generate_network296 = new BitSet(new long[]{0x000000C3E0000000L,0x0200004000000000L});
    public static final BitSet FOLLOW_gn_action_par_def_in_generate_network308 = new BitSet(new long[]{0x0000000000000000L,0x0600000000000000L});
    public static final BitSet FOLLOW_COMMA_in_generate_network322 = new BitSet(new long[]{0x000000C3E0000000L,0x0000004000000000L});
    public static final BitSet FOLLOW_gn_action_par_def_in_generate_network324 = new BitSet(new long[]{0x0000000000000000L,0x0600000000000000L});
    public static final BitSet FOLLOW_RBRACKET_in_generate_network339 = new BitSet(new long[]{0x0000000000000000L,0x2000000000000000L});
    public static final BitSet FOLLOW_RPAREN_in_generate_network343 = new BitSet(new long[]{0x0000000000000002L,0x0010000000000000L});
    public static final BitSet FOLLOW_SEMI_in_generate_network345 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_MAX_AGG_in_gn_action_par_def394 = new BitSet(new long[]{0x0000000000000000L,0x0000000000000000L,0x0000000000000080L});
    public static final BitSet FOLLOW_ASSIGNS_in_gn_action_par_def396 = new BitSet(new long[]{0x0000000000000000L,0x0002000000000000L});
    public static final BitSet FOLLOW_INT_in_gn_action_par_def400 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_MAX_ITER_in_gn_action_par_def416 = new BitSet(new long[]{0x0000000000000000L,0x0000000000000000L,0x0000000000000080L});
    public static final BitSet FOLLOW_ASSIGNS_in_gn_action_par_def418 = new BitSet(new long[]{0x0000000000000000L,0x0002000000000000L});
    public static final BitSet FOLLOW_INT_in_gn_action_par_def422 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_MAX_STOICH_in_gn_action_par_def438 = new BitSet(new long[]{0x0000000000000000L,0x0000000000000000L,0x0000000000000080L});
    public static final BitSet FOLLOW_ASSIGNS_in_gn_action_par_def440 = new BitSet(new long[]{0x0000000000000000L,0x0008000000000000L});
    public static final BitSet FOLLOW_hash_value_in_gn_action_par_def442 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_OVERWRITE_in_gn_action_par_def456 = new BitSet(new long[]{0x0000000000000000L,0x0000000000000000L,0x0000000000000080L});
    public static final BitSet FOLLOW_ASSIGNS_in_gn_action_par_def458 = new BitSet(new long[]{0x0000000000000000L,0x0002000000000000L});
    public static final BitSet FOLLOW_INT_in_gn_action_par_def462 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_PRINT_ITER_in_gn_action_par_def478 = new BitSet(new long[]{0x0000000000000000L,0x0000000000000000L,0x0000000000000080L});
    public static final BitSet FOLLOW_ASSIGNS_in_gn_action_par_def480 = new BitSet(new long[]{0x0000000000000000L,0x0002000000000000L});
    public static final BitSet FOLLOW_INT_in_gn_action_par_def484 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_TEXTREACTION_in_gn_action_par_def500 = new BitSet(new long[]{0x0000000000000000L,0x0000000000000000L,0x0000000000000080L});
    public static final BitSet FOLLOW_ASSIGNS_in_gn_action_par_def502 = new BitSet(new long[]{0x0000000000000000L,0x0002000000000000L});
    public static final BitSet FOLLOW_INT_in_gn_action_par_def506 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_ps_par_def_in_gn_action_par_def521 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_simulate_ode_in_simulate_method556 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_simulate_ssa_in_simulate_method571 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_write_m_file_in_simulate_method586 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_write_mex_file_in_simulate_method601 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_write_network_in_simulate_method616 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_simulate_pla_in_simulate_method631 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_simulate_nf_in_simulate_method646 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_simulate_in_simulate_method661 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_SIMULATE_in_simulate710 = new BitSet(new long[]{0x0000000000000000L,0x1000000000000000L});
    public static final BitSet FOLLOW_LPAREN_in_simulate712 = new BitSet(new long[]{0x0000000000000000L,0x2100000000000000L});
    public static final BitSet FOLLOW_LBRACKET_in_simulate715 = new BitSet(new long[]{0xFF0004C3E0000000L,0x020010FE000B0009L});
    public static final BitSet FOLLOW_ps_par_def_in_simulate728 = new BitSet(new long[]{0x0000000000000000L,0x0600000000000000L});
    public static final BitSet FOLLOW_simulate_par_def_in_simulate731 = new BitSet(new long[]{0x0000000000000000L,0x0600000000000000L});
    public static final BitSet FOLLOW_simulate_ode_par_def_in_simulate734 = new BitSet(new long[]{0x0000000000000000L,0x0600000000000000L});
    public static final BitSet FOLLOW_COMMA_in_simulate749 = new BitSet(new long[]{0xFF0004C3E0000000L,0x000010FE000B0009L});
    public static final BitSet FOLLOW_ps_par_def_in_simulate752 = new BitSet(new long[]{0x0000000000000000L,0x0600000000000000L});
    public static final BitSet FOLLOW_simulate_par_def_in_simulate755 = new BitSet(new long[]{0x0000000000000000L,0x0600000000000000L});
    public static final BitSet FOLLOW_simulate_ode_par_def_in_simulate758 = new BitSet(new long[]{0x0000000000000000L,0x0600000000000000L});
    public static final BitSet FOLLOW_RBRACKET_in_simulate775 = new BitSet(new long[]{0x0000000000000000L,0x2000000000000000L});
    public static final BitSet FOLLOW_RPAREN_in_simulate779 = new BitSet(new long[]{0x0000000000000002L,0x0010000000000000L});
    public static final BitSet FOLLOW_SEMI_in_simulate781 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_SIMULATE_ODE_in_simulate_ode807 = new BitSet(new long[]{0x0000000000000000L,0x1000000000000000L});
    public static final BitSet FOLLOW_LPAREN_in_simulate_ode809 = new BitSet(new long[]{0x0000000000000000L,0x2100000000000000L});
    public static final BitSet FOLLOW_LBRACKET_in_simulate_ode812 = new BitSet(new long[]{0xFF0004C3E0000000L,0x020010FE000B0009L});
    public static final BitSet FOLLOW_ps_par_def_in_simulate_ode825 = new BitSet(new long[]{0x0000000000000000L,0x0600000000000000L});
    public static final BitSet FOLLOW_simulate_par_def_in_simulate_ode828 = new BitSet(new long[]{0x0000000000000000L,0x0600000000000000L});
    public static final BitSet FOLLOW_simulate_ode_par_def_in_simulate_ode831 = new BitSet(new long[]{0x0000000000000000L,0x0600000000000000L});
    public static final BitSet FOLLOW_COMMA_in_simulate_ode846 = new BitSet(new long[]{0xFF0004C3E0000000L,0x000010FE000B0009L});
    public static final BitSet FOLLOW_ps_par_def_in_simulate_ode849 = new BitSet(new long[]{0x0000000000000000L,0x0600000000000000L});
    public static final BitSet FOLLOW_simulate_par_def_in_simulate_ode852 = new BitSet(new long[]{0x0000000000000000L,0x0600000000000000L});
    public static final BitSet FOLLOW_simulate_ode_par_def_in_simulate_ode855 = new BitSet(new long[]{0x0000000000000000L,0x0600000000000000L});
    public static final BitSet FOLLOW_RBRACKET_in_simulate_ode874 = new BitSet(new long[]{0x0000000000000000L,0x2000000000000000L});
    public static final BitSet FOLLOW_RPAREN_in_simulate_ode878 = new BitSet(new long[]{0x0000000000000002L,0x0010000000000000L});
    public static final BitSet FOLLOW_SEMI_in_simulate_ode880 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_SIMULATE_SSA_in_simulate_ssa907 = new BitSet(new long[]{0x0000000000000000L,0x1000000000000000L});
    public static final BitSet FOLLOW_LPAREN_in_simulate_ssa909 = new BitSet(new long[]{0x0000000000000000L,0x2100000000000000L});
    public static final BitSet FOLLOW_LBRACKET_in_simulate_ssa912 = new BitSet(new long[]{0xF00004C3E0000000L,0x020010FE000B0009L});
    public static final BitSet FOLLOW_ps_par_def_in_simulate_ssa926 = new BitSet(new long[]{0x0000000000000000L,0x0600000000000000L});
    public static final BitSet FOLLOW_simulate_par_def_in_simulate_ssa929 = new BitSet(new long[]{0x0000000000000000L,0x0600000000000000L});
    public static final BitSet FOLLOW_COMMA_in_simulate_ssa934 = new BitSet(new long[]{0xF00004C3E0000000L,0x000010FE000B0009L});
    public static final BitSet FOLLOW_ps_par_def_in_simulate_ssa937 = new BitSet(new long[]{0x0000000000000000L,0x0600000000000000L});
    public static final BitSet FOLLOW_simulate_par_def_in_simulate_ssa940 = new BitSet(new long[]{0x0000000000000000L,0x0600000000000000L});
    public static final BitSet FOLLOW_RBRACKET_in_simulate_ssa958 = new BitSet(new long[]{0x0000000000000000L,0x2000000000000000L});
    public static final BitSet FOLLOW_RPAREN_in_simulate_ssa962 = new BitSet(new long[]{0x0000000000000002L,0x0010000000000000L});
    public static final BitSet FOLLOW_SEMI_in_simulate_ssa964 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_SIMULATE_NF_in_simulate_nf983 = new BitSet(new long[]{0x0000000000000000L,0x1000000000000000L});
    public static final BitSet FOLLOW_LPAREN_in_simulate_nf985 = new BitSet(new long[]{0x0000000000000000L,0x2100000000000000L});
    public static final BitSet FOLLOW_LBRACKET_in_simulate_nf988 = new BitSet(new long[]{0xF0000CC3E0000000L,0x020010FE380B0009L});
    public static final BitSet FOLLOW_ps_par_def_in_simulate_nf1001 = new BitSet(new long[]{0x0000000000000000L,0x0600000000000000L});
    public static final BitSet FOLLOW_simulate_par_def_in_simulate_nf1004 = new BitSet(new long[]{0x0000000000000000L,0x0600000000000000L});
    public static final BitSet FOLLOW_simulate_nf_par_def_in_simulate_nf1007 = new BitSet(new long[]{0x0000000000000000L,0x0600000000000000L});
    public static final BitSet FOLLOW_COMMA_in_simulate_nf1022 = new BitSet(new long[]{0xF0000CC3E0000000L,0x000010FE380B0009L});
    public static final BitSet FOLLOW_ps_par_def_in_simulate_nf1025 = new BitSet(new long[]{0x0000000000000000L,0x0600000000000000L});
    public static final BitSet FOLLOW_simulate_par_def_in_simulate_nf1028 = new BitSet(new long[]{0x0000000000000000L,0x0600000000000000L});
    public static final BitSet FOLLOW_simulate_nf_par_def_in_simulate_nf1031 = new BitSet(new long[]{0x0000000000000000L,0x0600000000000000L});
    public static final BitSet FOLLOW_RBRACKET_in_simulate_nf1050 = new BitSet(new long[]{0x0000000000000000L,0x2000000000000000L});
    public static final BitSet FOLLOW_RPAREN_in_simulate_nf1054 = new BitSet(new long[]{0x0000000000000002L,0x0010000000000000L});
    public static final BitSet FOLLOW_SEMI_in_simulate_nf1056 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_WRITEMFILE_in_write_m_file1084 = new BitSet(new long[]{0x0000000000000000L,0x1000000000000000L});
    public static final BitSet FOLLOW_LPAREN_in_write_m_file1086 = new BitSet(new long[]{0x0000000000000000L,0x2100000000000000L});
    public static final BitSet FOLLOW_LBRACKET_in_write_m_file1089 = new BitSet(new long[]{0x7B00000000000000L,0x0200200000000000L});
    public static final BitSet FOLLOW_write_m_par_def_in_write_m_file1101 = new BitSet(new long[]{0x0000000000000000L,0x0600000000000000L});
    public static final BitSet FOLLOW_COMMA_in_write_m_file1106 = new BitSet(new long[]{0x7B00000000000000L,0x0000200000000000L});
    public static final BitSet FOLLOW_write_m_par_def_in_write_m_file1108 = new BitSet(new long[]{0x0000000000000000L,0x0600000000000000L});
    public static final BitSet FOLLOW_RBRACKET_in_write_m_file1123 = new BitSet(new long[]{0x0000000000000000L,0x2000000000000000L});
    public static final BitSet FOLLOW_RPAREN_in_write_m_file1127 = new BitSet(new long[]{0x0000000000000002L,0x0010000000000000L});
    public static final BitSet FOLLOW_SEMI_in_write_m_file1129 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_WRITEMEXFILE_in_write_mex_file1147 = new BitSet(new long[]{0x0000000000000000L,0x1000000000000000L});
    public static final BitSet FOLLOW_LPAREN_in_write_mex_file1149 = new BitSet(new long[]{0x0000000000000000L,0x0100000000000000L});
    public static final BitSet FOLLOW_LBRACKET_in_write_mex_file1151 = new BitSet(new long[]{0x7B0000C3E0000000L,0x0200204000000000L});
    public static final BitSet FOLLOW_write_m_par_def_in_write_mex_file1163 = new BitSet(new long[]{0x000000C3E0000000L,0x0600004000000000L});
    public static final BitSet FOLLOW_ps_par_def_in_write_mex_file1167 = new BitSet(new long[]{0x000000C3E0000000L,0x0600004000000000L});
    public static final BitSet FOLLOW_COMMA_in_write_mex_file1172 = new BitSet(new long[]{0x7B00000000000000L,0x0000200000000000L});
    public static final BitSet FOLLOW_write_m_par_def_in_write_mex_file1174 = new BitSet(new long[]{0x000000C3E0000000L,0x0600004000000000L});
    public static final BitSet FOLLOW_ps_par_def_in_write_mex_file1178 = new BitSet(new long[]{0x000000C3E0000000L,0x0600004000000000L});
    public static final BitSet FOLLOW_RBRACKET_in_write_mex_file1193 = new BitSet(new long[]{0x0000000000000000L,0x2000000000000000L});
    public static final BitSet FOLLOW_RPAREN_in_write_mex_file1195 = new BitSet(new long[]{0x0000000000000002L,0x0010000000000000L});
    public static final BitSet FOLLOW_SEMI_in_write_mex_file1197 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_WRITENETWORK_in_write_network1213 = new BitSet(new long[]{0x0000000000000000L,0x1000000000000000L});
    public static final BitSet FOLLOW_LPAREN_in_write_network1215 = new BitSet(new long[]{0x0000000000000000L,0x2100000000000000L});
    public static final BitSet FOLLOW_LBRACKET_in_write_network1218 = new BitSet(new long[]{0xF00004C3E0000000L,0x020010FE000B0009L});
    public static final BitSet FOLLOW_ps_par_def_in_write_network1229 = new BitSet(new long[]{0x0000000000000000L,0x0600000000000000L});
    public static final BitSet FOLLOW_simulate_par_def_in_write_network1232 = new BitSet(new long[]{0x0000000000000000L,0x0600000000000000L});
    public static final BitSet FOLLOW_COMMA_in_write_network1247 = new BitSet(new long[]{0xF00004C3E0000000L,0x000010FE000B0009L});
    public static final BitSet FOLLOW_ps_par_def_in_write_network1250 = new BitSet(new long[]{0x0000000000000000L,0x0600000000000000L});
    public static final BitSet FOLLOW_simulate_par_def_in_write_network1253 = new BitSet(new long[]{0x0000000000000000L,0x0600000000000000L});
    public static final BitSet FOLLOW_RBRACKET_in_write_network1268 = new BitSet(new long[]{0x0000000000000000L,0x2000000000000000L});
    public static final BitSet FOLLOW_RPAREN_in_write_network1279 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_SIMULATE_PLA_in_simulate_pla1305 = new BitSet(new long[]{0x0000000000000000L,0x1000000000000000L});
    public static final BitSet FOLLOW_LPAREN_in_simulate_pla1307 = new BitSet(new long[]{0x0000000000000000L,0x2100000000000000L});
    public static final BitSet FOLLOW_LBRACKET_in_simulate_pla1310 = new BitSet(new long[]{0xF00004C3E0000000L,0x020010FE000B000DL});
    public static final BitSet FOLLOW_ps_par_def_in_simulate_pla1327 = new BitSet(new long[]{0x0000000000000000L,0x0600000000000000L});
    public static final BitSet FOLLOW_simulate_par_def_in_simulate_pla1330 = new BitSet(new long[]{0x0000000000000000L,0x0600000000000000L});
    public static final BitSet FOLLOW_simulate_pla_par_def_in_simulate_pla1333 = new BitSet(new long[]{0x0000000000000000L,0x0600000000000000L});
    public static final BitSet FOLLOW_COMMA_in_simulate_pla1348 = new BitSet(new long[]{0xF00004C3E0000000L,0x000010FE000B000DL});
    public static final BitSet FOLLOW_ps_par_def_in_simulate_pla1351 = new BitSet(new long[]{0x0000000000000000L,0x0600000000000000L});
    public static final BitSet FOLLOW_simulate_par_def_in_simulate_pla1354 = new BitSet(new long[]{0x0000000000000000L,0x0600000000000000L});
    public static final BitSet FOLLOW_simulate_pla_par_def_in_simulate_pla1357 = new BitSet(new long[]{0x0000000000000000L,0x0600000000000000L});
    public static final BitSet FOLLOW_RBRACKET_in_simulate_pla1376 = new BitSet(new long[]{0x0000000000000000L,0x2000000000000000L});
    public static final BitSet FOLLOW_RPAREN_in_simulate_pla1380 = new BitSet(new long[]{0x0000000000000002L,0x0010000000000000L});
    public static final BitSet FOLLOW_SEMI_in_simulate_pla1382 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_READFILE_in_read_file1412 = new BitSet(new long[]{0x0000000000000000L,0x1000000000000000L});
    public static final BitSet FOLLOW_LPAREN_in_read_file1414 = new BitSet(new long[]{0x0000000000000000L,0x2100000000000000L});
    public static final BitSet FOLLOW_LBRACKET_in_read_file1426 = new BitSet(new long[]{0x0000000000000000L,0x0200000000000400L});
    public static final BitSet FOLLOW_FILE_in_read_file1429 = new BitSet(new long[]{0x0000000000000000L,0x0000000000000000L,0x0000000000000080L});
    public static final BitSet FOLLOW_ASSIGNS_in_read_file1431 = new BitSet(new long[]{0x0000000000000000L,0x0000000000000000L,0x0000000000080000L});
    public static final BitSet FOLLOW_DBQUOTES_in_read_file1433 = new BitSet(new long[]{0x0000000000000000L,0x0808000000000000L,0x0000000000001400L});
    public static final BitSet FOLLOW_filename_in_read_file1438 = new BitSet(new long[]{0x0000000000000000L,0x0000000000000000L,0x0000000000080000L});
    public static final BitSet FOLLOW_DBQUOTES_in_read_file1441 = new BitSet(new long[]{0x0000000000000000L,0x0200000000000000L});
    public static final BitSet FOLLOW_RBRACKET_in_read_file1447 = new BitSet(new long[]{0x0000000000000000L,0x2000000000000000L});
    public static final BitSet FOLLOW_RPAREN_in_read_file1460 = new BitSet(new long[]{0x0000000000000002L,0x0010000000000000L});
    public static final BitSet FOLLOW_SEMI_in_read_file1462 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_write_type_in_write_file1508 = new BitSet(new long[]{0x0000000000000000L,0x1000000000000000L});
    public static final BitSet FOLLOW_LPAREN_in_write_file1520 = new BitSet(new long[]{0x0000000000000000L,0x2100000000000000L});
    public static final BitSet FOLLOW_LBRACKET_in_write_file1523 = new BitSet(new long[]{0x0000008000000000L,0x0200000000000000L});
    public static final BitSet FOLLOW_write_par_def_in_write_file1526 = new BitSet(new long[]{0x0000000000000000L,0x0600000000000000L});
    public static final BitSet FOLLOW_COMMA_in_write_file1529 = new BitSet(new long[]{0x0000008000000000L});
    public static final BitSet FOLLOW_write_par_def_in_write_file1531 = new BitSet(new long[]{0x0000000000000000L,0x0600000000000000L});
    public static final BitSet FOLLOW_RBRACKET_in_write_file1537 = new BitSet(new long[]{0x0000000000000000L,0x2000000000000000L});
    public static final BitSet FOLLOW_RPAREN_in_write_file1552 = new BitSet(new long[]{0x0000000000000002L,0x0010000000000000L});
    public static final BitSet FOLLOW_SEMI_in_write_file1554 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_set_in_write_type0 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_SETCONCENTRATION_in_set_concentration1644 = new BitSet(new long[]{0x0000000000000000L,0x1000000000000000L});
    public static final BitSet FOLLOW_LPAREN_in_set_concentration1646 = new BitSet(new long[]{0x0000000000000000L,0x0000000000000000L,0x0000000000080000L});
    public static final BitSet FOLLOW_variable_definition_in_set_concentration1648 = new BitSet(new long[]{0x0000000000000000L,0x2000000000000000L});
    public static final BitSet FOLLOW_RPAREN_in_set_concentration1669 = new BitSet(new long[]{0x0000000000000002L,0x0010000000000000L});
    public static final BitSet FOLLOW_SEMI_in_set_concentration1671 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_ADDCONCENTRATION_in_add_concentration1727 = new BitSet(new long[]{0x0000000000000000L,0x1000000000000000L});
    public static final BitSet FOLLOW_LPAREN_in_add_concentration1729 = new BitSet(new long[]{0x0000000000000000L,0x0000000000000000L,0x0000000000080000L});
    public static final BitSet FOLLOW_variable_definition_in_add_concentration1731 = new BitSet(new long[]{0x0000000000000000L,0x2000000000000000L});
    public static final BitSet FOLLOW_RPAREN_in_add_concentration1752 = new BitSet(new long[]{0x0000000000000002L,0x0010000000000000L});
    public static final BitSet FOLLOW_SEMI_in_add_concentration1754 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_SETPARAMETER_in_set_parameter1810 = new BitSet(new long[]{0x0000000000000000L,0x1000000000000000L});
    public static final BitSet FOLLOW_LPAREN_in_set_parameter1812 = new BitSet(new long[]{0x0000000000000000L,0x0000000000000000L,0x0000000000080000L});
    public static final BitSet FOLLOW_parameter_definition_in_set_parameter1814 = new BitSet(new long[]{0x0000000000000000L,0x2000000000000000L});
    public static final BitSet FOLLOW_RPAREN_in_set_parameter1837 = new BitSet(new long[]{0x0000000000000002L,0x0010000000000000L});
    public static final BitSet FOLLOW_SEMI_in_set_parameter1839 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_DBQUOTES_in_parameter_definition1874 = new BitSet(new long[]{0x0000000000000000L,0x0008000000000000L});
    public static final BitSet FOLLOW_STRING_in_parameter_definition1877 = new BitSet(new long[]{0x0000000000000000L,0x0000000000000000L,0x0000000000080000L});
    public static final BitSet FOLLOW_DBQUOTES_in_parameter_definition1882 = new BitSet(new long[]{0x0000000000000000L,0x0400000000000000L});
    public static final BitSet FOLLOW_COMMA_in_parameter_definition1884 = new BitSet(new long[]{0x0004002000000000L,0x100B000180000000L,0x0000000000081000L});
    public static final BitSet FOLLOW_expression_in_parameter_definition1890 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_DBQUOTES_in_parameter_definition1898 = new BitSet(new long[]{0x0004002000000000L,0x100B000180000000L,0x0000000000001000L});
    public static final BitSet FOLLOW_expression_in_parameter_definition1902 = new BitSet(new long[]{0x0000000000000000L,0x0000000000000000L,0x0000000000080000L});
    public static final BitSet FOLLOW_DBQUOTES_in_parameter_definition1905 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_DBQUOTES_in_variable_definition1937 = new BitSet(new long[]{0x0000000000000000L,0x0008000000000000L,0x0000000000008005L});
    public static final BitSet FOLLOW_species_def_in_variable_definition1940 = new BitSet(new long[]{0x0000000000000000L,0x0000000000000000L,0x0000000000080000L});
    public static final BitSet FOLLOW_DBQUOTES_in_variable_definition1946 = new BitSet(new long[]{0x0000000000000000L,0x0400000000000000L});
    public static final BitSet FOLLOW_COMMA_in_variable_definition1948 = new BitSet(new long[]{0x0004002000000000L,0x100B000180000000L,0x0000000000081000L});
    public static final BitSet FOLLOW_expression_in_variable_definition1954 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_DBQUOTES_in_variable_definition1962 = new BitSet(new long[]{0x0004002000000000L,0x100B000180000000L,0x0000000000001000L});
    public static final BitSet FOLLOW_expression_in_variable_definition1966 = new BitSet(new long[]{0x0000000000000000L,0x0000000000000000L,0x0000000000080000L});
    public static final BitSet FOLLOW_DBQUOTES_in_variable_definition1969 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_SAVECONCENTRATIONS_in_save_concentrations2026 = new BitSet(new long[]{0x0000000000000000L,0x1000000000000000L});
    public static final BitSet FOLLOW_LPAREN_in_save_concentrations2028 = new BitSet(new long[]{0x0000000000000000L,0x2000000000000000L,0x0000000000080000L});
    public static final BitSet FOLLOW_DBQUOTES_in_save_concentrations2031 = new BitSet(new long[]{0x0000000000000000L,0x0008000000000000L});
    public static final BitSet FOLLOW_STRING_in_save_concentrations2033 = new BitSet(new long[]{0x0000000000000000L,0x0000000000000000L,0x0000000000080000L});
    public static final BitSet FOLLOW_DBQUOTES_in_save_concentrations2035 = new BitSet(new long[]{0x0000000000000000L,0x2000000000000000L});
    public static final BitSet FOLLOW_RPAREN_in_save_concentrations2039 = new BitSet(new long[]{0x0000000000000002L,0x0010000000000000L});
    public static final BitSet FOLLOW_SEMI_in_save_concentrations2041 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_RESETCONCENTRATIONS_in_reset_concentrations2075 = new BitSet(new long[]{0x0000000000000000L,0x1000000000000000L});
    public static final BitSet FOLLOW_LPAREN_in_reset_concentrations2077 = new BitSet(new long[]{0x0000000000000000L,0x2000000000000000L,0x0000000000080000L});
    public static final BitSet FOLLOW_DBQUOTES_in_reset_concentrations2080 = new BitSet(new long[]{0x0000000000000000L,0x0008000000000000L});
    public static final BitSet FOLLOW_STRING_in_reset_concentrations2082 = new BitSet(new long[]{0x0000000000000000L,0x0000000000000000L,0x0000000000080000L});
    public static final BitSet FOLLOW_DBQUOTES_in_reset_concentrations2084 = new BitSet(new long[]{0x0000000000000000L,0x2000000000000000L});
    public static final BitSet FOLLOW_RPAREN_in_reset_concentrations2088 = new BitSet(new long[]{0x0000000000000002L,0x0010000000000000L});
    public static final BitSet FOLLOW_SEMI_in_reset_concentrations2090 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_STRING_in_hash_value2124 = new BitSet(new long[]{0x0000000000000000L,0x0000000000000000L,0x0000000000000080L});
    public static final BitSet FOLLOW_ASSIGNS_in_hash_value2126 = new BitSet(new long[]{0x0000000000000000L,0x0100000000000000L});
    public static final BitSet FOLLOW_LBRACKET_in_hash_value2128 = new BitSet(new long[]{0x0000000000000000L,0x0008000000000000L});
    public static final BitSet FOLLOW_assignment_list_in_hash_value2130 = new BitSet(new long[]{0x0000000000000000L,0x0200000000000000L});
    public static final BitSet FOLLOW_RBRACKET_in_hash_value2132 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_STRING_in_assignment_list2156 = new BitSet(new long[]{0x0000000000000000L,0x0000000000000000L,0x0000000000000080L});
    public static final BitSet FOLLOW_ASSIGNS_in_assignment_list2158 = new BitSet(new long[]{0x0000000000000000L,0x000B000000000000L});
    public static final BitSet FOLLOW_value_in_assignment_list2160 = new BitSet(new long[]{0x0000000000000000L,0x0400000000000000L});
    public static final BitSet FOLLOW_COMMA_in_assignment_list2163 = new BitSet(new long[]{0x0000000000000000L,0x0008000000000000L});
    public static final BitSet FOLLOW_STRING_in_assignment_list2165 = new BitSet(new long[]{0x0000000000000000L,0x0000000000000000L,0x0000000000000080L});
    public static final BitSet FOLLOW_ASSIGNS_in_assignment_list2167 = new BitSet(new long[]{0x0000000000000000L,0x000B000000000000L});
    public static final BitSet FOLLOW_value_in_assignment_list2169 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_set_in_value0 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_PREFFIX_in_ps_par_def2221 = new BitSet(new long[]{0x0000000000000000L,0x0000000000000000L,0x0000000000000080L});
    public static final BitSet FOLLOW_ASSIGNS_in_ps_par_def2223 = new BitSet(new long[]{0x0000000000000000L,0x0000000000000000L,0x0000000000080000L});
    public static final BitSet FOLLOW_DBQUOTES_in_ps_par_def2227 = new BitSet(new long[]{0xFFFFFFFFFFFFFFF0L,0xFFFFFFFFFFFFFFFFL,0xFFFFFFFFFFFFFFFFL,0xFFFFFFFFFFFFFFFFL,0x00000000000FFFFFL});
    public static final BitSet FOLLOW_set_in_ps_par_def2230 = new BitSet(new long[]{0xFFFFFFFFFFFFFFF0L,0xFFFFFFFFFFFFFFFFL,0xFFFFFFFFFFFFFFFFL,0xFFFFFFFFFFFFFFFFL,0x00000000000FFFFFL});
    public static final BitSet FOLLOW_DBQUOTES_in_ps_par_def2237 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_SUFFIX_in_ps_par_def2251 = new BitSet(new long[]{0x0000000000000000L,0x0000000000000000L,0x0000000000000080L});
    public static final BitSet FOLLOW_ASSIGNS_in_ps_par_def2253 = new BitSet(new long[]{0x0000000000000000L,0x0000000000000000L,0x0000000000080000L});
    public static final BitSet FOLLOW_DBQUOTES_in_ps_par_def2257 = new BitSet(new long[]{0xFFFFFFFFFFFFFFF0L,0xFFFFFFFFFFFFFFFFL,0xFFFFFFFFFFFFFFFFL,0xFFFFFFFFFFFFFFFFL,0x00000000000FFFFFL});
    public static final BitSet FOLLOW_set_in_ps_par_def2260 = new BitSet(new long[]{0xFFFFFFFFFFFFFFF0L,0xFFFFFFFFFFFFFFFFL,0xFFFFFFFFFFFFFFFFL,0xFFFFFFFFFFFFFFFFL,0x00000000000FFFFFL});
    public static final BitSet FOLLOW_DBQUOTES_in_ps_par_def2267 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_ATOL_in_simulate_ode_par_def2294 = new BitSet(new long[]{0x0000000000000000L,0x0000000000000000L,0x0000000000000080L});
    public static final BitSet FOLLOW_ASSIGNS_in_simulate_ode_par_def2296 = new BitSet(new long[]{0x0000000000000000L,0x0001000000000000L});
    public static final BitSet FOLLOW_FLOAT_in_simulate_ode_par_def2300 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_RTOL_in_simulate_ode_par_def2314 = new BitSet(new long[]{0x0000000000000000L,0x0000000000000000L,0x0000000000000080L});
    public static final BitSet FOLLOW_ASSIGNS_in_simulate_ode_par_def2316 = new BitSet(new long[]{0x0000000000000000L,0x0001000000000000L});
    public static final BitSet FOLLOW_FLOAT_in_simulate_ode_par_def2320 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_STEADY_STATE_in_simulate_ode_par_def2334 = new BitSet(new long[]{0x0000000000000000L,0x0000000000000000L,0x0000000000000080L});
    public static final BitSet FOLLOW_ASSIGNS_in_simulate_ode_par_def2336 = new BitSet(new long[]{0x0000000000000000L,0x0002000000000000L});
    public static final BitSet FOLLOW_INT_in_simulate_ode_par_def2340 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_SPARSE_in_simulate_ode_par_def2354 = new BitSet(new long[]{0x0000000000000000L,0x0000000000000000L,0x0000000000000080L});
    public static final BitSet FOLLOW_ASSIGNS_in_simulate_ode_par_def2356 = new BitSet(new long[]{0x0000000000000000L,0x0002000000000000L});
    public static final BitSet FOLLOW_INT_in_simulate_ode_par_def2360 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_PLA_CONFIG_in_simulate_pla_par_def2403 = new BitSet(new long[]{0x0000000000000000L,0x0000000000000000L,0x0000000000000080L});
    public static final BitSet FOLLOW_ASSIGNS_in_simulate_pla_par_def2405 = new BitSet(new long[]{0x0000000000000000L,0x0000000000000000L,0x0000000000080000L});
    public static final BitSet FOLLOW_DBQUOTES_in_simulate_pla_par_def2407 = new BitSet(new long[]{0xFFFFFFFFFFFFFFF0L,0xFFFFFFFFFFFFFFFFL,0xFFFFFFFFFFFFFFFFL,0xFFFFFFFFFFFFFFFFL,0x00000000000FFFFFL});
    public static final BitSet FOLLOW_set_in_simulate_pla_par_def2412 = new BitSet(new long[]{0xFFFFFFFFFFFFFFF0L,0xFFFFFFFFFFFFFFFFL,0xFFFFFFFFFFFFFFFFL,0xFFFFFFFFFFFFFFFFL,0x00000000000FFFFFL});
    public static final BitSet FOLLOW_DBQUOTES_in_simulate_pla_par_def2421 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_PARAM_in_simulate_nf_par_def2454 = new BitSet(new long[]{0x0000000000000000L,0x0000000000000000L,0x0000000000000080L});
    public static final BitSet FOLLOW_ASSIGNS_in_simulate_nf_par_def2456 = new BitSet(new long[]{0x0000000000000000L,0x0000000000000000L,0x0000000000080000L});
    public static final BitSet FOLLOW_DBQUOTES_in_simulate_nf_par_def2458 = new BitSet(new long[]{0x0000000000000000L,0x0000000000000000L,0x0000000000081000L});
    public static final BitSet FOLLOW_MINUS_in_simulate_nf_par_def2461 = new BitSet(new long[]{0x0000000000000000L,0x0008000000000000L});
    public static final BitSet FOLLOW_STRING_in_simulate_nf_par_def2465 = new BitSet(new long[]{0x0000000000000000L,0x000A000000000000L,0x0000000000000000L,0x0100000000000000L});
    public static final BitSet FOLLOW_set_in_simulate_nf_par_def2469 = new BitSet(new long[]{0x0000000000000000L,0x0000000000000000L,0x0000000000081000L});
    public static final BitSet FOLLOW_DBQUOTES_in_simulate_nf_par_def2479 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_COMPLEX_in_simulate_nf_par_def2494 = new BitSet(new long[]{0x0000000000000000L,0x0000000000000000L,0x0000000000000080L});
    public static final BitSet FOLLOW_ASSIGNS_in_simulate_nf_par_def2496 = new BitSet(new long[]{0x0000000000000000L,0x0002000000000000L});
    public static final BitSet FOLLOW_INT_in_simulate_nf_par_def2500 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_GET_FINAL_STATE_in_simulate_nf_par_def2514 = new BitSet(new long[]{0x0000000000000000L,0x0000000000000000L,0x0000000000000080L});
    public static final BitSet FOLLOW_ASSIGNS_in_simulate_nf_par_def2516 = new BitSet(new long[]{0x0000000000000000L,0x0002000000000000L});
    public static final BitSet FOLLOW_INT_in_simulate_nf_par_def2520 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_GML_in_simulate_nf_par_def2534 = new BitSet(new long[]{0x0000000000000000L,0x0000000000000000L,0x0000000000000080L});
    public static final BitSet FOLLOW_ASSIGNS_in_simulate_nf_par_def2536 = new BitSet(new long[]{0x0000000000000000L,0x0002000000000000L});
    public static final BitSet FOLLOW_INT_in_simulate_nf_par_def2540 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_SUFFIX_in_write_par_def2579 = new BitSet(new long[]{0x0000000000000000L,0x0000000000000000L,0x0000000000000080L});
    public static final BitSet FOLLOW_ASSIGNS_in_write_par_def2581 = new BitSet(new long[]{0x0000000000000000L,0x0000000000000000L,0x0000000000080000L});
    public static final BitSet FOLLOW_DBQUOTES_in_write_par_def2583 = new BitSet(new long[]{0xFFFFFFFFFFFFFFF0L,0xFFFFFFFFFFFFFFFFL,0xFFFFFFFFFFFFFFFFL,0xFFFFFFFFFFFFFFFFL,0x00000000000FFFFFL});
    public static final BitSet FOLLOW_set_in_write_par_def2585 = new BitSet(new long[]{0xFFFFFFFFFFFFFFF0L,0xFFFFFFFFFFFFFFFFL,0xFFFFFFFFFFFFFFFFL,0xFFFFFFFFFFFFFFFFL,0x00000000000FFFFFL});
    public static final BitSet FOLLOW_DBQUOTES_in_write_par_def2592 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_ATOL_in_write_m_par_def2613 = new BitSet(new long[]{0x0000000000000000L,0x0000000000000000L,0x0000000000000080L});
    public static final BitSet FOLLOW_ASSIGNS_in_write_m_par_def2615 = new BitSet(new long[]{0x0000000000000000L,0x0001000000000000L});
    public static final BitSet FOLLOW_FLOAT_in_write_m_par_def2619 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_RTOL_in_write_m_par_def2627 = new BitSet(new long[]{0x0000000000000000L,0x0000000000000000L,0x0000000000000080L});
    public static final BitSet FOLLOW_ASSIGNS_in_write_m_par_def2629 = new BitSet(new long[]{0x0000000000000000L,0x0001000000000000L});
    public static final BitSet FOLLOW_FLOAT_in_write_m_par_def2633 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_T_END_in_write_m_par_def2641 = new BitSet(new long[]{0x0000000000000000L,0x0000000000000000L,0x0000000000000080L});
    public static final BitSet FOLLOW_ASSIGNS_in_write_m_par_def2643 = new BitSet(new long[]{0x0000000000000000L,0x0003000000000000L});
    public static final BitSet FOLLOW_INT_in_write_m_par_def2648 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_FLOAT_in_write_m_par_def2654 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_T_START_in_write_m_par_def2664 = new BitSet(new long[]{0x0000000000000000L,0x0000000000000000L,0x0000000000000080L});
    public static final BitSet FOLLOW_ASSIGNS_in_write_m_par_def2666 = new BitSet(new long[]{0x0000000000000000L,0x0003000000000000L});
    public static final BitSet FOLLOW_INT_in_write_m_par_def2671 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_FLOAT_in_write_m_par_def2677 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_N_STEPS_in_write_m_par_def2686 = new BitSet(new long[]{0x0000000000000000L,0x0000000000000000L,0x0000000000000080L});
    public static final BitSet FOLLOW_ASSIGNS_in_write_m_par_def2688 = new BitSet(new long[]{0x0000000000000000L,0x0002000000000000L});
    public static final BitSet FOLLOW_INT_in_write_m_par_def2692 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_SPARSE_in_write_m_par_def2700 = new BitSet(new long[]{0x0000000000000000L,0x0000000000000000L,0x0000000000000080L});
    public static final BitSet FOLLOW_ASSIGNS_in_write_m_par_def2702 = new BitSet(new long[]{0x0000000000000000L,0x0002000000000000L});
    public static final BitSet FOLLOW_INT_in_write_m_par_def2706 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_BDF_in_write_m_par_def2714 = new BitSet(new long[]{0x0000000000000000L,0x0000000000000000L,0x0000000000000080L});
    public static final BitSet FOLLOW_ASSIGNS_in_write_m_par_def2716 = new BitSet(new long[]{0x0000000000000000L,0x0002000000000000L});
    public static final BitSet FOLLOW_INT_in_write_m_par_def2720 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_T_END_in_simulate_par_def2743 = new BitSet(new long[]{0x0000000000000000L,0x0000000000000000L,0x0000000000000080L});
    public static final BitSet FOLLOW_ASSIGNS_in_simulate_par_def2745 = new BitSet(new long[]{0x0000000000000000L,0x0003000000000000L});
    public static final BitSet FOLLOW_INT_in_simulate_par_def2750 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_FLOAT_in_simulate_par_def2756 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_T_START_in_simulate_par_def2772 = new BitSet(new long[]{0x0000000000000000L,0x0000000000000000L,0x0000000000000080L});
    public static final BitSet FOLLOW_ASSIGNS_in_simulate_par_def2774 = new BitSet(new long[]{0x0000000000000000L,0x0003000000000000L});
    public static final BitSet FOLLOW_INT_in_simulate_par_def2779 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_FLOAT_in_simulate_par_def2785 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_N_STEPS_in_simulate_par_def2800 = new BitSet(new long[]{0x0000000000000000L,0x0000000000000000L,0x0000000000000080L});
    public static final BitSet FOLLOW_ASSIGNS_in_simulate_par_def2802 = new BitSet(new long[]{0x0000000000000000L,0x0003000000000000L});
    public static final BitSet FOLLOW_set_in_simulate_par_def2806 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_SAMPLE_TIMES_in_simulate_par_def2824 = new BitSet(new long[]{0x0000000000000000L,0x0000000000000000L,0x0000000000000080L});
    public static final BitSet FOLLOW_ASSIGNS_in_simulate_par_def2826 = new BitSet(new long[]{0x0000000000000000L,0x0040000000000000L});
    public static final BitSet FOLLOW_array_value_in_simulate_par_def2830 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_VERBOSE_in_simulate_par_def2843 = new BitSet(new long[]{0x0000000000000000L,0x0000000000000000L,0x0000000000000080L});
    public static final BitSet FOLLOW_ASSIGNS_in_simulate_par_def2845 = new BitSet(new long[]{0x0000000000000000L,0x0002000000000000L});
    public static final BitSet FOLLOW_INT_in_simulate_par_def2849 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_PRINT_CDAT_in_simulate_par_def2863 = new BitSet(new long[]{0x0000000000000000L,0x0000000000000000L,0x0000000000000080L});
    public static final BitSet FOLLOW_ASSIGNS_in_simulate_par_def2865 = new BitSet(new long[]{0x0000000000000000L,0x0002000000000000L});
    public static final BitSet FOLLOW_INT_in_simulate_par_def2869 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_PRINT_FUNCTIONS_in_simulate_par_def2883 = new BitSet(new long[]{0x0000000000000000L,0x0000000000000000L,0x0000000000000080L});
    public static final BitSet FOLLOW_ASSIGNS_in_simulate_par_def2885 = new BitSet(new long[]{0x0000000000000000L,0x0002000000000000L});
    public static final BitSet FOLLOW_INT_in_simulate_par_def2889 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_N_OUTPUT_STEPS_in_simulate_par_def2903 = new BitSet(new long[]{0x0000000000000000L,0x0000000000000000L,0x0000000000000080L});
    public static final BitSet FOLLOW_ASSIGNS_in_simulate_par_def2905 = new BitSet(new long[]{0x0000000000000000L,0x000B000000000000L});
    public static final BitSet FOLLOW_multiple_definition_in_simulate_par_def2907 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_MAX_SIM_STEPS_in_simulate_par_def2921 = new BitSet(new long[]{0x0000000000000000L,0x0000000000000000L,0x0000000000000080L});
    public static final BitSet FOLLOW_ASSIGNS_in_simulate_par_def2923 = new BitSet(new long[]{0x0000000000000000L,0x000B000000000000L});
    public static final BitSet FOLLOW_multiple_definition_in_simulate_par_def2925 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_ARGFILE_in_simulate_par_def2939 = new BitSet(new long[]{0x0000000000000000L,0x0000000000000000L,0x0000000000000080L});
    public static final BitSet FOLLOW_ASSIGNS_in_simulate_par_def2941 = new BitSet(new long[]{0x0000000000000000L,0x0000000000000000L,0x0000000000080000L});
    public static final BitSet FOLLOW_DBQUOTES_in_simulate_par_def2943 = new BitSet(new long[]{0x0000000000000000L,0x0808000000000000L,0x0000000000001400L});
    public static final BitSet FOLLOW_filename_in_simulate_par_def2947 = new BitSet(new long[]{0x0000000000000000L,0x0000000000000000L,0x0000000000080000L});
    public static final BitSet FOLLOW_DBQUOTES_in_simulate_par_def2949 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_SAVE_PROGRESS_in_simulate_par_def2963 = new BitSet(new long[]{0x0000000000000000L,0x0000000000000000L,0x0000000000000080L});
    public static final BitSet FOLLOW_ASSIGNS_in_simulate_par_def2965 = new BitSet(new long[]{0x0000000000000000L,0x0002000000000000L});
    public static final BitSet FOLLOW_INT_in_simulate_par_def2969 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_PRINT_NET_in_simulate_par_def2983 = new BitSet(new long[]{0x0000000000000000L,0x0000000000000000L,0x0000000000000080L});
    public static final BitSet FOLLOW_ASSIGNS_in_simulate_par_def2985 = new BitSet(new long[]{0x0000000000000000L,0x0002000000000000L});
    public static final BitSet FOLLOW_INT_in_simulate_par_def2989 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_PRINT_END_in_simulate_par_def3003 = new BitSet(new long[]{0x0000000000000000L,0x0000000000000000L,0x0000000000000080L});
    public static final BitSet FOLLOW_ASSIGNS_in_simulate_par_def3005 = new BitSet(new long[]{0x0000000000000000L,0x0002000000000000L});
    public static final BitSet FOLLOW_INT_in_simulate_par_def3009 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_NETFILE_in_simulate_par_def3027 = new BitSet(new long[]{0x0000000000000000L,0x0000000000000000L,0x0000000000000080L});
    public static final BitSet FOLLOW_ASSIGNS_in_simulate_par_def3029 = new BitSet(new long[]{0x0000000000000000L,0x0000000000000000L,0x0000000000080000L});
    public static final BitSet FOLLOW_DBQUOTES_in_simulate_par_def3031 = new BitSet(new long[]{0x0000000000000000L,0x0808000000000000L,0x0000000000001400L});
    public static final BitSet FOLLOW_filename_in_simulate_par_def3035 = new BitSet(new long[]{0x0000000000000000L,0x0000000000000000L,0x0000000000080000L});
    public static final BitSet FOLLOW_DBQUOTES_in_simulate_par_def3037 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_METHOD_in_simulate_par_def3051 = new BitSet(new long[]{0x0000000000000000L,0x0000000000000000L,0x0000000000000080L});
    public static final BitSet FOLLOW_ASSIGNS_in_simulate_par_def3053 = new BitSet(new long[]{0x0000000000000000L,0x00000F0000000000L});
    public static final BitSet FOLLOW_method_definition_in_simulate_par_def3055 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_CONTINUE_in_simulate_par_def3069 = new BitSet(new long[]{0x0000000000000000L,0x0000000000000000L,0x0000000000000080L});
    public static final BitSet FOLLOW_ASSIGNS_in_simulate_par_def3071 = new BitSet(new long[]{0x0000000000000000L,0x0002000000000000L});
    public static final BitSet FOLLOW_INT_in_simulate_par_def3075 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_EVALUATE_EXPRESSIONS_in_simulate_par_def3089 = new BitSet(new long[]{0x0000000000000000L,0x0000000000000000L,0x0000000000000080L});
    public static final BitSet FOLLOW_ASSIGNS_in_simulate_par_def3091 = new BitSet(new long[]{0x0000000000000000L,0x0002000000000000L});
    public static final BitSet FOLLOW_INT_in_simulate_par_def3095 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_OVERWRITE_in_simulate_par_def3109 = new BitSet(new long[]{0x0000000000000000L,0x0000000000000000L,0x0000000000000080L});
    public static final BitSet FOLLOW_ASSIGNS_in_simulate_par_def3111 = new BitSet(new long[]{0x0000000000000000L,0x0002000000000000L});
    public static final BitSet FOLLOW_INT_in_simulate_par_def3115 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_INT_in_multiple_definition3147 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_FLOAT_in_multiple_definition3152 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_STRING_in_multiple_definition3160 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_set_in_method_definition0 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_LSBRACKET_in_array_value3199 = new BitSet(new long[]{0x0000000000000000L,0x0008000000000000L});
    public static final BitSet FOLLOW_STRING_in_array_value3201 = new BitSet(new long[]{0x0000000000000000L,0x0480000000000000L});
    public static final BitSet FOLLOW_COMMA_in_array_value3204 = new BitSet(new long[]{0x0000000000000000L,0x0008000000000000L});
    public static final BitSet FOLLOW_STRING_in_array_value3206 = new BitSet(new long[]{0x0000000000000000L,0x0480000000000000L});
    public static final BitSet FOLLOW_RSBRACKET_in_array_value3210 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_set_in_filename3226 = new BitSet(new long[]{0x0000000000000002L,0x0808000000000000L,0x0000000000001400L});

}