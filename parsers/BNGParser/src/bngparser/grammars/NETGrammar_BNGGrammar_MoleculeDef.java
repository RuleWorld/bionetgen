// $ANTLR 3.3 Nov 30, 2010 12:50:56 BNGGrammar_MoleculeDef.g 2013-01-14 18:13:03

  package bngparser.grammars;
  import bngparser.grammars.BNGLexer;
  import org.antlr.stringtemplate.*;
  import bngparser.dataType.BondList;
  import java.util.Map;
  import bngparser.methods.ReactionAction;
  import bngparser.dataType.Register;
  import bngparser.methods.InvertBidirectional;
  import bngparser.dataType.ChangeableChannelTokenStream;
  import bngparser.dataType.ReactionRegister;
  import bngparser.dataType.Compartments;
  import bngparser.methods.GenericMethods;


import org.antlr.runtime.*;
import java.util.Stack;
import java.util.List;
import java.util.ArrayList;

import org.antlr.stringtemplate.*;
import org.antlr.stringtemplate.language.*;
import java.util.HashMap;
public class NETGrammar_BNGGrammar_MoleculeDef extends Parser {
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

    // delegates
    // delegators
    public NETGrammar gNETGrammar;
    public NETGrammar gParent;


        public NETGrammar_BNGGrammar_MoleculeDef(TokenStream input, NETGrammar gNETGrammar) {
            this(input, new RecognizerSharedState(), gNETGrammar);
        }
        public NETGrammar_BNGGrammar_MoleculeDef(TokenStream input, RecognizerSharedState state, NETGrammar gNETGrammar) {
            super(input, state);
            this.gNETGrammar = gNETGrammar;
             
            gParent = gNETGrammar;
        }
        
    protected StringTemplateGroup templateLib =
      new StringTemplateGroup("NETGrammar_BNGGrammar_MoleculeDefTemplates", AngleBracketTemplateLexer.class);

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

    public String[] getTokenNames() { return NETGrammar.tokenNames; }
    public String getGrammarFileName() { return "BNGGrammar_MoleculeDef.g"; }


     public void getParentTemplate(){
     
      this.setTemplateLib(gParent.getTemplateLib());
     }


    public static class molecule_types_block_return extends ParserRuleReturnScope {
        public StringTemplate st;
        public Object getTemplate() { return st; }
        public String toString() { return st==null?null:st.toString(); }
    };

    // $ANTLR start "molecule_types_block"
    // BNGGrammar_MoleculeDef.g:15:1: molecule_types_block : BEGIN MOLECULE TYPES ( LB )+ ( molecule_def ( LB )+ )* END MOLECULE TYPES ( LB )+ ;
    public final NETGrammar_BNGGrammar_MoleculeDef.molecule_types_block_return molecule_types_block() throws RecognitionException {
        NETGrammar_BNGGrammar_MoleculeDef.molecule_types_block_return retval = new NETGrammar_BNGGrammar_MoleculeDef.molecule_types_block_return();
        retval.start = input.LT(1);

        NETGrammar_BNGGrammar_MoleculeDef.molecule_def_return molecule_def1 = null;



        getParentTemplate();
         gParent.paraphrases.push("in molecule types section");

        try {
            // BNGGrammar_MoleculeDef.g:23:1: ( BEGIN MOLECULE TYPES ( LB )+ ( molecule_def ( LB )+ )* END MOLECULE TYPES ( LB )+ )
            // BNGGrammar_MoleculeDef.g:24:2: BEGIN MOLECULE TYPES ( LB )+ ( molecule_def ( LB )+ )* END MOLECULE TYPES ( LB )+
            {
            match(input,BEGIN,FOLLOW_BEGIN_in_molecule_types_block50); 
            match(input,MOLECULE,FOLLOW_MOLECULE_in_molecule_types_block52); 
            match(input,TYPES,FOLLOW_TYPES_in_molecule_types_block54); 
            // BNGGrammar_MoleculeDef.g:24:23: ( LB )+
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
            	    // BNGGrammar_MoleculeDef.g:24:23: LB
            	    {
            	    match(input,LB,FOLLOW_LB_in_molecule_types_block56); 

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

            // BNGGrammar_MoleculeDef.g:24:26: ( molecule_def ( LB )+ )*
            loop3:
            do {
                int alt3=2;
                int LA3_0 = input.LA(1);

                if ( (LA3_0==INT||LA3_0==STRING) ) {
                    alt3=1;
                }


                switch (alt3) {
            	case 1 :
            	    // BNGGrammar_MoleculeDef.g:24:27: molecule_def ( LB )+
            	    {
            	    pushFollow(FOLLOW_molecule_def_in_molecule_types_block59);
            	    molecule_def1=molecule_def();

            	    state._fsp--;


            	     gParent.molecules.add((molecule_def1!=null?molecule_def1.st:null));
            	     //gParent.variableName.add((molecule_def1!=null?input.toString(molecule_def1.start,molecule_def1.stop):null));
            	     
            	    // BNGGrammar_MoleculeDef.g:30:2: ( LB )+
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
            	    	    // BNGGrammar_MoleculeDef.g:30:2: LB
            	    	    {
            	    	    match(input,LB,FOLLOW_LB_in_molecule_types_block68); 

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

            match(input,END,FOLLOW_END_in_molecule_types_block75); 
            match(input,MOLECULE,FOLLOW_MOLECULE_in_molecule_types_block77); 
            match(input,TYPES,FOLLOW_TYPES_in_molecule_types_block79); 
            // BNGGrammar_MoleculeDef.g:31:24: ( LB )+
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
            	    // BNGGrammar_MoleculeDef.g:31:24: LB
            	    {
            	    match(input,LB,FOLLOW_LB_in_molecule_types_block81); 

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
    // $ANTLR end "molecule_types_block"

    protected static class molecule_def_scope {
        List sites;
    }
    protected Stack molecule_def_stack = new Stack();

    public static class molecule_def_return extends ParserRuleReturnScope {
        public StringTemplate st;
        public Object getTemplate() { return st; }
        public String toString() { return st==null?null:st.toString(); }
    };

    // $ANTLR start "molecule_def"
    // BNGGrammar_MoleculeDef.g:33:1: molecule_def : ({...}? INT | ) STRING ( LPAREN ( (s1= site_def ( COMMA s2= site_def )* ) | ) RPAREN )? -> molecules_def(id=$STRING.textsites=$molecule_def::sites);
    public final NETGrammar_BNGGrammar_MoleculeDef.molecule_def_return molecule_def() throws RecognitionException {
        molecule_def_stack.push(new molecule_def_scope());
        NETGrammar_BNGGrammar_MoleculeDef.molecule_def_return retval = new NETGrammar_BNGGrammar_MoleculeDef.molecule_def_return();
        retval.start = input.LT(1);

        Token STRING2=null;
        NETGrammar_BNGGrammar_MoleculeDef.site_def_return s1 = null;

        NETGrammar_BNGGrammar_MoleculeDef.site_def_return s2 = null;



          ((molecule_def_scope)molecule_def_stack.peek()).sites = new ArrayList();

        try {
            // BNGGrammar_MoleculeDef.g:40:1: ( ({...}? INT | ) STRING ( LPAREN ( (s1= site_def ( COMMA s2= site_def )* ) | ) RPAREN )? -> molecules_def(id=$STRING.textsites=$molecule_def::sites))
            // BNGGrammar_MoleculeDef.g:40:3: ({...}? INT | ) STRING ( LPAREN ( (s1= site_def ( COMMA s2= site_def )* ) | ) RPAREN )?
            {
            // BNGGrammar_MoleculeDef.g:40:3: ({...}? INT | )
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
                    // BNGGrammar_MoleculeDef.g:40:4: {...}? INT
                    {
                    if ( !((gParent.netGrammar)) ) {
                        throw new FailedPredicateException(input, "molecule_def", "gParent.netGrammar");
                    }
                    match(input,INT,FOLLOW_INT_in_molecule_def100); 

                    }
                    break;
                case 2 :
                    // BNGGrammar_MoleculeDef.g:40:32: 
                    {
                    }
                    break;

            }

            STRING2=(Token)match(input,STRING,FOLLOW_STRING_in_molecule_def106); 
            // BNGGrammar_MoleculeDef.g:40:41: ( LPAREN ( (s1= site_def ( COMMA s2= site_def )* ) | ) RPAREN )?
            int alt8=2;
            int LA8_0 = input.LA(1);

            if ( (LA8_0==LPAREN) ) {
                alt8=1;
            }
            switch (alt8) {
                case 1 :
                    // BNGGrammar_MoleculeDef.g:40:42: LPAREN ( (s1= site_def ( COMMA s2= site_def )* ) | ) RPAREN
                    {
                    match(input,LPAREN,FOLLOW_LPAREN_in_molecule_def109); 
                    // BNGGrammar_MoleculeDef.g:40:49: ( (s1= site_def ( COMMA s2= site_def )* ) | )
                    int alt7=2;
                    int LA7_0 = input.LA(1);

                    if ( (LA7_0==STRING) ) {
                        alt7=1;
                    }
                    else if ( (LA7_0==RPAREN) ) {
                        alt7=2;
                    }
                    else {
                        NoViableAltException nvae =
                            new NoViableAltException("", 7, 0, input);

                        throw nvae;
                    }
                    switch (alt7) {
                        case 1 :
                            // BNGGrammar_MoleculeDef.g:41:3: (s1= site_def ( COMMA s2= site_def )* )
                            {
                            // BNGGrammar_MoleculeDef.g:41:3: (s1= site_def ( COMMA s2= site_def )* )
                            // BNGGrammar_MoleculeDef.g:41:4: s1= site_def ( COMMA s2= site_def )*
                            {
                            pushFollow(FOLLOW_site_def_in_molecule_def118);
                            s1=site_def();

                            state._fsp--;

                            ((molecule_def_scope)molecule_def_stack.peek()).sites.add((s1!=null?s1.st:null));
                            // BNGGrammar_MoleculeDef.g:42:3: ( COMMA s2= site_def )*
                            loop6:
                            do {
                                int alt6=2;
                                int LA6_0 = input.LA(1);

                                if ( (LA6_0==COMMA) ) {
                                    alt6=1;
                                }


                                switch (alt6) {
                            	case 1 :
                            	    // BNGGrammar_MoleculeDef.g:42:4: COMMA s2= site_def
                            	    {
                            	    match(input,COMMA,FOLLOW_COMMA_in_molecule_def125); 
                            	    pushFollow(FOLLOW_site_def_in_molecule_def129);
                            	    s2=site_def();

                            	    state._fsp--;

                            	    ((molecule_def_scope)molecule_def_stack.peek()).sites.add((s2!=null?s2.st:null));

                            	    }
                            	    break;

                            	default :
                            	    break loop6;
                                }
                            } while (true);


                            }


                            }
                            break;
                        case 2 :
                            // BNGGrammar_MoleculeDef.g:44:3: 
                            {
                            }
                            break;

                    }

                    match(input,RPAREN,FOLLOW_RPAREN_in_molecule_def146); 

                    }
                    break;

            }



            // TEMPLATE REWRITE
            // 47:3: -> molecules_def(id=$STRING.textsites=$molecule_def::sites)
            {
                retval.st = templateLib.getInstanceOf("molecules_def",
              new STAttrMap().put("id", (STRING2!=null?STRING2.getText():null)).put("sites", ((molecule_def_scope)molecule_def_stack.peek()).sites));
            }


            }

            retval.stop = input.LT(-1);

        }
        catch (RecognitionException re) {
            reportError(re);
            recover(input,re);
        }
        finally {
            molecule_def_stack.pop();
        }
        return retval;
    }
    // $ANTLR end "molecule_def"

    protected static class site_def_scope {
        List<String> states;
    }
    protected Stack site_def_stack = new Stack();

    public static class site_def_return extends ParserRuleReturnScope {
        public StringTemplate st;
        public Object getTemplate() { return st; }
        public String toString() { return st==null?null:st.toString(); }
    };

    // $ANTLR start "site_def"
    // BNGGrammar_MoleculeDef.g:50:1: site_def : STRING ( TILDE state_label )* -> sites_def(id=$STRING.textstates=$site_def::states);
    public final NETGrammar_BNGGrammar_MoleculeDef.site_def_return site_def() throws RecognitionException {
        site_def_stack.push(new site_def_scope());
        NETGrammar_BNGGrammar_MoleculeDef.site_def_return retval = new NETGrammar_BNGGrammar_MoleculeDef.site_def_return();
        retval.start = input.LT(1);

        Token STRING4=null;
        NETGrammar_BNGGrammar_MoleculeDef.state_label_return state_label3 = null;



          ((site_def_scope)site_def_stack.peek()).states = new ArrayList<String>();

        try {
            // BNGGrammar_MoleculeDef.g:57:1: ( STRING ( TILDE state_label )* -> sites_def(id=$STRING.textstates=$site_def::states))
            // BNGGrammar_MoleculeDef.g:57:3: STRING ( TILDE state_label )*
            {
            STRING4=(Token)match(input,STRING,FOLLOW_STRING_in_site_def185); 
            // BNGGrammar_MoleculeDef.g:57:10: ( TILDE state_label )*
            loop9:
            do {
                int alt9=2;
                int LA9_0 = input.LA(1);

                if ( (LA9_0==TILDE) ) {
                    alt9=1;
                }


                switch (alt9) {
            	case 1 :
            	    // BNGGrammar_MoleculeDef.g:57:11: TILDE state_label
            	    {
            	    match(input,TILDE,FOLLOW_TILDE_in_site_def188); 
            	    pushFollow(FOLLOW_state_label_in_site_def190);
            	    state_label3=state_label();

            	    state._fsp--;

            	    ((site_def_scope)site_def_stack.peek()).states.add((state_label3!=null?input.toString(state_label3.start,state_label3.stop):null));

            	    }
            	    break;

            	default :
            	    break loop9;
                }
            } while (true);



            // TEMPLATE REWRITE
            // 57:75: -> sites_def(id=$STRING.textstates=$site_def::states)
            {
                retval.st = templateLib.getInstanceOf("sites_def",
              new STAttrMap().put("id", (STRING4!=null?STRING4.getText():null)).put("states", ((site_def_scope)site_def_stack.peek()).states));
            }


            }

            retval.stop = input.LT(-1);

        }
        catch (RecognitionException re) {
            reportError(re);
            recover(input,re);
        }
        finally {
            site_def_stack.pop();
        }
        return retval;
    }
    // $ANTLR end "site_def"

    public static class state_label_return extends ParserRuleReturnScope {
        public StringTemplate st;
        public Object getTemplate() { return st; }
        public String toString() { return st==null?null:st.toString(); }
    };

    // $ANTLR start "state_label"
    // BNGGrammar_MoleculeDef.g:59:1: state_label : ( STRING | INT | QMARK )+ ;
    public final NETGrammar_BNGGrammar_MoleculeDef.state_label_return state_label() throws RecognitionException {
        NETGrammar_BNGGrammar_MoleculeDef.state_label_return retval = new NETGrammar_BNGGrammar_MoleculeDef.state_label_return();
        retval.start = input.LT(1);

        try {
            // BNGGrammar_MoleculeDef.g:59:12: ( ( STRING | INT | QMARK )+ )
            // BNGGrammar_MoleculeDef.g:59:14: ( STRING | INT | QMARK )+
            {
            // BNGGrammar_MoleculeDef.g:59:14: ( STRING | INT | QMARK )+
            int cnt10=0;
            loop10:
            do {
                int alt10=2;
                int LA10_0 = input.LA(1);

                if ( (LA10_0==INT||LA10_0==STRING||LA10_0==QMARK) ) {
                    alt10=1;
                }


                switch (alt10) {
            	case 1 :
            	    // BNGGrammar_MoleculeDef.g:
            	    {
            	    if ( input.LA(1)==INT||input.LA(1)==STRING||input.LA(1)==QMARK ) {
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
            	    if ( cnt10 >= 1 ) break loop10;
                        EarlyExitException eee =
                            new EarlyExitException(10, input);
                        throw eee;
                }
                cnt10++;
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
    // $ANTLR end "state_label"

    // Delegated rules


 

    public static final BitSet FOLLOW_BEGIN_in_molecule_types_block50 = new BitSet(new long[]{0x0000000000002000L});
    public static final BitSet FOLLOW_MOLECULE_in_molecule_types_block52 = new BitSet(new long[]{0x0000000000010000L});
    public static final BitSet FOLLOW_TYPES_in_molecule_types_block54 = new BitSet(new long[]{0x0000000000000020L});
    public static final BitSet FOLLOW_LB_in_molecule_types_block56 = new BitSet(new long[]{0x0000000000000120L,0x000A000000000000L});
    public static final BitSet FOLLOW_molecule_def_in_molecule_types_block59 = new BitSet(new long[]{0x0000000000000020L});
    public static final BitSet FOLLOW_LB_in_molecule_types_block68 = new BitSet(new long[]{0x0000000000000120L,0x000A000000000000L});
    public static final BitSet FOLLOW_END_in_molecule_types_block75 = new BitSet(new long[]{0x0000000000002000L});
    public static final BitSet FOLLOW_MOLECULE_in_molecule_types_block77 = new BitSet(new long[]{0x0000000000010000L});
    public static final BitSet FOLLOW_TYPES_in_molecule_types_block79 = new BitSet(new long[]{0x0000000000000020L});
    public static final BitSet FOLLOW_LB_in_molecule_types_block81 = new BitSet(new long[]{0x0000000000000022L});
    public static final BitSet FOLLOW_INT_in_molecule_def100 = new BitSet(new long[]{0x0000000000000000L,0x0008000000000000L});
    public static final BitSet FOLLOW_STRING_in_molecule_def106 = new BitSet(new long[]{0x0000000000000002L,0x1000000000000000L});
    public static final BitSet FOLLOW_LPAREN_in_molecule_def109 = new BitSet(new long[]{0x0000000000000000L,0x2008000000000000L});
    public static final BitSet FOLLOW_site_def_in_molecule_def118 = new BitSet(new long[]{0x0000000000000000L,0x2400000000000000L});
    public static final BitSet FOLLOW_COMMA_in_molecule_def125 = new BitSet(new long[]{0x0000000000000000L,0x0008000000000000L});
    public static final BitSet FOLLOW_site_def_in_molecule_def129 = new BitSet(new long[]{0x0000000000000000L,0x2400000000000000L});
    public static final BitSet FOLLOW_RPAREN_in_molecule_def146 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_STRING_in_site_def185 = new BitSet(new long[]{0x0000000000000002L,0x0000000000000000L,0x0000000000000002L});
    public static final BitSet FOLLOW_TILDE_in_site_def188 = new BitSet(new long[]{0x0000000000000000L,0x000A000000000000L,0x0000000000020000L});
    public static final BitSet FOLLOW_state_label_in_site_def190 = new BitSet(new long[]{0x0000000000000002L,0x0000000000000000L,0x0000000000000002L});
    public static final BitSet FOLLOW_set_in_state_label214 = new BitSet(new long[]{0x0000000000000002L,0x000A000000000000L,0x0000000000020000L});

}