// $ANTLR 3.3 Nov 30, 2010 12:50:56 BNGGrammar_MoleculeDef.g 2012-02-06 17:01:16

  package bngparser.netGrammar;
  import bngparser.grammars.BNGLexer;
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
public class NETGrammar_BNGGrammar_MoleculeDef extends Parser {
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
    // BNGGrammar_MoleculeDef.g:15:1: molecule_types_block : BEGIN MOLECULE TYPES ( molecule_def )* END MOLECULE TYPES ;
    public final NETGrammar_BNGGrammar_MoleculeDef.molecule_types_block_return molecule_types_block() throws RecognitionException {
        NETGrammar_BNGGrammar_MoleculeDef.molecule_types_block_return retval = new NETGrammar_BNGGrammar_MoleculeDef.molecule_types_block_return();
        retval.start = input.LT(1);

        NETGrammar_BNGGrammar_MoleculeDef.molecule_def_return molecule_def1 = null;



        getParentTemplate();
         gParent.paraphrases.push("in molecule types section");

        try {
            // BNGGrammar_MoleculeDef.g:23:1: ( BEGIN MOLECULE TYPES ( molecule_def )* END MOLECULE TYPES )
            // BNGGrammar_MoleculeDef.g:24:2: BEGIN MOLECULE TYPES ( molecule_def )* END MOLECULE TYPES
            {
            match(input,BEGIN,FOLLOW_BEGIN_in_molecule_types_block50); 
            match(input,MOLECULE,FOLLOW_MOLECULE_in_molecule_types_block52); 
            match(input,TYPES,FOLLOW_TYPES_in_molecule_types_block54); 
            // BNGGrammar_MoleculeDef.g:24:23: ( molecule_def )*
            loop1:
            do {
                int alt1=2;
                int LA1_0 = input.LA(1);

                if ( (LA1_0==INT||LA1_0==STRING) ) {
                    alt1=1;
                }


                switch (alt1) {
            	case 1 :
            	    // BNGGrammar_MoleculeDef.g:24:24: molecule_def
            	    {
            	    pushFollow(FOLLOW_molecule_def_in_molecule_types_block57);
            	    molecule_def1=molecule_def();

            	    state._fsp--;


            	     gParent.molecules.add((molecule_def1!=null?molecule_def1.st:null));
            	     //gParent.variableName.add((molecule_def1!=null?input.toString(molecule_def1.start,molecule_def1.stop):null));
            	     

            	    }
            	    break;

            	default :
            	    break loop1;
                }
            } while (true);

            match(input,END,FOLLOW_END_in_molecule_types_block71); 
            match(input,MOLECULE,FOLLOW_MOLECULE_in_molecule_types_block73); 
            match(input,TYPES,FOLLOW_TYPES_in_molecule_types_block75); 

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
                    // BNGGrammar_MoleculeDef.g:40:4: {...}? INT
                    {
                    if ( !((gParent.netGrammar)) ) {
                        throw new FailedPredicateException(input, "molecule_def", "gParent.netGrammar");
                    }
                    match(input,INT,FOLLOW_INT_in_molecule_def93); 

                    }
                    break;
                case 2 :
                    // BNGGrammar_MoleculeDef.g:40:32: 
                    {
                    }
                    break;

            }

            STRING2=(Token)match(input,STRING,FOLLOW_STRING_in_molecule_def99); 
            // BNGGrammar_MoleculeDef.g:40:41: ( LPAREN ( (s1= site_def ( COMMA s2= site_def )* ) | ) RPAREN )?
            int alt5=2;
            int LA5_0 = input.LA(1);

            if ( (LA5_0==LPAREN) ) {
                alt5=1;
            }
            switch (alt5) {
                case 1 :
                    // BNGGrammar_MoleculeDef.g:40:42: LPAREN ( (s1= site_def ( COMMA s2= site_def )* ) | ) RPAREN
                    {
                    match(input,LPAREN,FOLLOW_LPAREN_in_molecule_def102); 
                    // BNGGrammar_MoleculeDef.g:40:49: ( (s1= site_def ( COMMA s2= site_def )* ) | )
                    int alt4=2;
                    int LA4_0 = input.LA(1);

                    if ( (LA4_0==STRING) ) {
                        alt4=1;
                    }
                    else if ( (LA4_0==RPAREN) ) {
                        alt4=2;
                    }
                    else {
                        NoViableAltException nvae =
                            new NoViableAltException("", 4, 0, input);

                        throw nvae;
                    }
                    switch (alt4) {
                        case 1 :
                            // BNGGrammar_MoleculeDef.g:41:3: (s1= site_def ( COMMA s2= site_def )* )
                            {
                            // BNGGrammar_MoleculeDef.g:41:3: (s1= site_def ( COMMA s2= site_def )* )
                            // BNGGrammar_MoleculeDef.g:41:4: s1= site_def ( COMMA s2= site_def )*
                            {
                            pushFollow(FOLLOW_site_def_in_molecule_def111);
                            s1=site_def();

                            state._fsp--;

                            ((molecule_def_scope)molecule_def_stack.peek()).sites.add((s1!=null?s1.st:null));
                            // BNGGrammar_MoleculeDef.g:42:3: ( COMMA s2= site_def )*
                            loop3:
                            do {
                                int alt3=2;
                                int LA3_0 = input.LA(1);

                                if ( (LA3_0==COMMA) ) {
                                    alt3=1;
                                }


                                switch (alt3) {
                            	case 1 :
                            	    // BNGGrammar_MoleculeDef.g:42:4: COMMA s2= site_def
                            	    {
                            	    match(input,COMMA,FOLLOW_COMMA_in_molecule_def118); 
                            	    pushFollow(FOLLOW_site_def_in_molecule_def122);
                            	    s2=site_def();

                            	    state._fsp--;

                            	    ((molecule_def_scope)molecule_def_stack.peek()).sites.add((s2!=null?s2.st:null));

                            	    }
                            	    break;

                            	default :
                            	    break loop3;
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

                    match(input,RPAREN,FOLLOW_RPAREN_in_molecule_def139); 

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
            STRING4=(Token)match(input,STRING,FOLLOW_STRING_in_site_def178); 
            // BNGGrammar_MoleculeDef.g:57:10: ( TILDE state_label )*
            loop6:
            do {
                int alt6=2;
                int LA6_0 = input.LA(1);

                if ( (LA6_0==TILDE) ) {
                    alt6=1;
                }


                switch (alt6) {
            	case 1 :
            	    // BNGGrammar_MoleculeDef.g:57:11: TILDE state_label
            	    {
            	    match(input,TILDE,FOLLOW_TILDE_in_site_def181); 
            	    pushFollow(FOLLOW_state_label_in_site_def183);
            	    state_label3=state_label();

            	    state._fsp--;

            	    ((site_def_scope)site_def_stack.peek()).states.add((state_label3!=null?input.toString(state_label3.start,state_label3.stop):null));

            	    }
            	    break;

            	default :
            	    break loop6;
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
    // BNGGrammar_MoleculeDef.g:59:1: state_label : ( STRING | INT );
    public final NETGrammar_BNGGrammar_MoleculeDef.state_label_return state_label() throws RecognitionException {
        NETGrammar_BNGGrammar_MoleculeDef.state_label_return retval = new NETGrammar_BNGGrammar_MoleculeDef.state_label_return();
        retval.start = input.LT(1);

        try {
            // BNGGrammar_MoleculeDef.g:59:12: ( STRING | INT )
            // BNGGrammar_MoleculeDef.g:
            {
            if ( input.LA(1)==INT||input.LA(1)==STRING ) {
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
    // $ANTLR end "state_label"

    // Delegated rules


 

    public static final BitSet FOLLOW_BEGIN_in_molecule_types_block50 = new BitSet(new long[]{0x0000000000001000L});
    public static final BitSet FOLLOW_MOLECULE_in_molecule_types_block52 = new BitSet(new long[]{0x0000000000008000L});
    public static final BitSet FOLLOW_TYPES_in_molecule_types_block54 = new BitSet(new long[]{0x0000000000000080L,0x0000000000001400L});
    public static final BitSet FOLLOW_molecule_def_in_molecule_types_block57 = new BitSet(new long[]{0x0000000000000080L,0x0000000000001400L});
    public static final BitSet FOLLOW_END_in_molecule_types_block71 = new BitSet(new long[]{0x0000000000001000L});
    public static final BitSet FOLLOW_MOLECULE_in_molecule_types_block73 = new BitSet(new long[]{0x0000000000008000L});
    public static final BitSet FOLLOW_TYPES_in_molecule_types_block75 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_INT_in_molecule_def93 = new BitSet(new long[]{0x0000000000000000L,0x0000000000001000L});
    public static final BitSet FOLLOW_STRING_in_molecule_def99 = new BitSet(new long[]{0x0000000000000002L,0x0000000000200000L});
    public static final BitSet FOLLOW_LPAREN_in_molecule_def102 = new BitSet(new long[]{0x0000000000000000L,0x0000000000401000L});
    public static final BitSet FOLLOW_site_def_in_molecule_def111 = new BitSet(new long[]{0x0000000000000000L,0x0000000000480000L});
    public static final BitSet FOLLOW_COMMA_in_molecule_def118 = new BitSet(new long[]{0x0000000000000000L,0x0000000000001000L});
    public static final BitSet FOLLOW_site_def_in_molecule_def122 = new BitSet(new long[]{0x0000000000000000L,0x0000000000480000L});
    public static final BitSet FOLLOW_RPAREN_in_molecule_def139 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_STRING_in_site_def178 = new BitSet(new long[]{0x0000000000000002L,0x0000000004000000L});
    public static final BitSet FOLLOW_TILDE_in_site_def181 = new BitSet(new long[]{0x0000000000000000L,0x0000000000001400L});
    public static final BitSet FOLLOW_state_label_in_site_def183 = new BitSet(new long[]{0x0000000000000002L,0x0000000004000000L});
    public static final BitSet FOLLOW_set_in_state_label0 = new BitSet(new long[]{0x0000000000000002L});

}