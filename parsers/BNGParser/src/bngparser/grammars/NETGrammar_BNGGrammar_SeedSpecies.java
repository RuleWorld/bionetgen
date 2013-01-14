// $ANTLR 3.3 Nov 30, 2010 12:50:56 BNGGrammar_SeedSpecies.g 2013-01-14 18:13:03

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
public class NETGrammar_BNGGrammar_SeedSpecies extends Parser {
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


        public NETGrammar_BNGGrammar_SeedSpecies(TokenStream input, NETGrammar gNETGrammar) {
            this(input, new RecognizerSharedState(), gNETGrammar);
        }
        public NETGrammar_BNGGrammar_SeedSpecies(TokenStream input, RecognizerSharedState state, NETGrammar gNETGrammar) {
            super(input, state);
            this.gNETGrammar = gNETGrammar;
             
            gParent = gNETGrammar;
        }
        
    protected StringTemplateGroup templateLib =
      new StringTemplateGroup("NETGrammar_BNGGrammar_SeedSpeciesTemplates", AngleBracketTemplateLexer.class);

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
    public String getGrammarFileName() { return "BNGGrammar_SeedSpecies.g"; }


     public void getParentTemplate(){
     
      this.setTemplateLib(gParent.getTemplateLib());
     }


    protected static class species_def_scope {
        int numMolecules;
        String variableName;
    }
    protected Stack species_def_stack = new Stack();

    public static class species_def_return extends ParserRuleReturnScope {
        public Map<String,List<ReactionRegister>> listOfMolecules;
        public String glabel;
        public boolean constant;
        public String compartment="";
        public Map<String, Register> lmemory;
        public StringTemplate st;
        public Object getTemplate() { return st; }
        public String toString() { return st==null?null:st.toString(); }
    };

    // $ANTLR start "species_def"
    // BNGGrammar_SeedSpecies.g:80:1: species_def[List molecules,BondList bonds,String speciesCounter] returns [Map<String,List<ReactionRegister>> listOfMolecules, String glabel, boolean constant,String compartment=\"\",Map<String, Register> lmemory] : ( AT scomp= STRING COLON ({...}? COLON | ) )? ( MOD slabel= STRING COLON ( COLON )? )? ( DOLLAR )? (s1= species_element[bonds,speciesCounter + \"_M\" + $species_def::numMolecules,$scomp.text] ( DOT s2= species_element[bonds,speciesCounter + \"_M\" + $species_def::numMolecules,$scomp.text] )* ) -> molecules_def(id=$species_def::variableName);
    public final NETGrammar_BNGGrammar_SeedSpecies.species_def_return species_def(List molecules, BondList bonds, String speciesCounter) throws RecognitionException {
        species_def_stack.push(new species_def_scope());
        NETGrammar_BNGGrammar_SeedSpecies.species_def_return retval = new NETGrammar_BNGGrammar_SeedSpecies.species_def_return();
        retval.start = input.LT(1);

        Token scomp=null;
        Token slabel=null;
        NETGrammar_BNGGrammar_SeedSpecies.species_element_return s1 = null;

        NETGrammar_BNGGrammar_SeedSpecies.species_element_return s2 = null;



          ((species_def_scope)species_def_stack.peek()).numMolecules =1;
          retval.listOfMolecules = new HashMap<String,List<ReactionRegister>>();
           //We cannot have spaces between the elements of a species definition, so we activate space recognition. Should there be a space
           //the parser will not continue with this rule
         //  ((ChangeableChannelTokenStream)input).setChannel(Token.DEFAULT_CHANNEL|HIDDEN);
          ((species_def_scope)species_def_stack.peek()).variableName = "";
          retval.constant = false;
          retval.lmemory = new HashMap<String,Register>();
          //this is done so we can temporarily recognize whitespaces
        ((ChangeableChannelTokenStream)input).addChannel(HIDDEN);
          gParent.paraphrases.push("in species def section");

        try {
            // BNGGrammar_SeedSpecies.g:108:1: ( ( AT scomp= STRING COLON ({...}? COLON | ) )? ( MOD slabel= STRING COLON ( COLON )? )? ( DOLLAR )? (s1= species_element[bonds,speciesCounter + \"_M\" + $species_def::numMolecules,$scomp.text] ( DOT s2= species_element[bonds,speciesCounter + \"_M\" + $species_def::numMolecules,$scomp.text] )* ) -> molecules_def(id=$species_def::variableName))
            // BNGGrammar_SeedSpecies.g:109:4: ( AT scomp= STRING COLON ({...}? COLON | ) )? ( MOD slabel= STRING COLON ( COLON )? )? ( DOLLAR )? (s1= species_element[bonds,speciesCounter + \"_M\" + $species_def::numMolecules,$scomp.text] ( DOT s2= species_element[bonds,speciesCounter + \"_M\" + $species_def::numMolecules,$scomp.text] )* )
            {
            // BNGGrammar_SeedSpecies.g:109:4: ( AT scomp= STRING COLON ({...}? COLON | ) )?
            int alt10=2;
            int LA10_0 = input.LA(1);

            if ( (LA10_0==AT) ) {
                alt10=1;
            }
            switch (alt10) {
                case 1 :
                    // BNGGrammar_SeedSpecies.g:109:5: AT scomp= STRING COLON ({...}? COLON | )
                    {
                    match(input,AT,FOLLOW_AT_in_species_def249); 
                    scomp=(Token)match(input,STRING,FOLLOW_STRING_in_species_def253); 

                        retval.compartment = (scomp!=null?scomp.getText():null);
                       //((ChangeableChannelTokenStream)input).addChannel(HIDDEN);
                       
                    match(input,COLON,FOLLOW_COLON_in_species_def268); 
                    // BNGGrammar_SeedSpecies.g:115:10: ({...}? COLON | )
                    int alt9=2;
                    int LA9_0 = input.LA(1);

                    if ( (LA9_0==COLON) ) {
                        alt9=1;
                    }
                    else if ( (LA9_0==STRING||LA9_0==DOLLAR||LA9_0==MOD) ) {
                        alt9=2;
                    }
                    else {
                        NoViableAltException nvae =
                            new NoViableAltException("", 9, 0, input);

                        throw nvae;
                    }
                    switch (alt9) {
                        case 1 :
                            // BNGGrammar_SeedSpecies.g:115:11: {...}? COLON
                            {
                            if ( !((gParent.netGrammar)) ) {
                                throw new FailedPredicateException(input, "species_def", "gParent.netGrammar");
                            }
                            match(input,COLON,FOLLOW_COLON_in_species_def273); 

                            }
                            break;
                        case 2 :
                            // BNGGrammar_SeedSpecies.g:115:41: 
                            {
                            }
                            break;

                    }


                    }
                    break;

            }

            // BNGGrammar_SeedSpecies.g:117:4: ( MOD slabel= STRING COLON ( COLON )? )?
            int alt12=2;
            int LA12_0 = input.LA(1);

            if ( (LA12_0==MOD) ) {
                alt12=1;
            }
            switch (alt12) {
                case 1 :
                    // BNGGrammar_SeedSpecies.g:117:5: MOD slabel= STRING COLON ( COLON )?
                    {
                    match(input,MOD,FOLLOW_MOD_in_species_def291); 
                    slabel=(Token)match(input,STRING,FOLLOW_STRING_in_species_def295); 
                    match(input,COLON,FOLLOW_COLON_in_species_def297); 
                    // BNGGrammar_SeedSpecies.g:117:29: ( COLON )?
                    int alt11=2;
                    int LA11_0 = input.LA(1);

                    if ( (LA11_0==COLON) ) {
                        alt11=1;
                    }
                    switch (alt11) {
                        case 1 :
                            // BNGGrammar_SeedSpecies.g:117:29: COLON
                            {
                            match(input,COLON,FOLLOW_COLON_in_species_def299); 

                            }
                            break;

                    }


                        retval.lmemory.put((slabel!=null?slabel.getText():null),new Register(1.0,"observable"));
                        retval.glabel = (slabel!=null?slabel.getText():null);
                       

                    }
                    break;

            }

            // BNGGrammar_SeedSpecies.g:123:4: ( DOLLAR )?
            int alt13=2;
            int LA13_0 = input.LA(1);

            if ( (LA13_0==DOLLAR) ) {
                alt13=1;
            }
            switch (alt13) {
                case 1 :
                    // BNGGrammar_SeedSpecies.g:123:5: DOLLAR
                    {
                    match(input,DOLLAR,FOLLOW_DOLLAR_in_species_def318); 
                    retval.constant = true;

                    }
                    break;

            }

            // BNGGrammar_SeedSpecies.g:125:4: (s1= species_element[bonds,speciesCounter + \"_M\" + $species_def::numMolecules,$scomp.text] ( DOT s2= species_element[bonds,speciesCounter + \"_M\" + $species_def::numMolecules,$scomp.text] )* )
            // BNGGrammar_SeedSpecies.g:125:5: s1= species_element[bonds,speciesCounter + \"_M\" + $species_def::numMolecules,$scomp.text] ( DOT s2= species_element[bonds,speciesCounter + \"_M\" + $species_def::numMolecules,$scomp.text] )*
            {
            pushFollow(FOLLOW_species_element_in_species_def334);
            s1=species_element(bonds, speciesCounter + "_M" + ((species_def_scope)species_def_stack.peek()).numMolecules, (scomp!=null?scomp.getText():null));

            state._fsp--;


                molecules.add(s1.st);
                ((species_def_scope)species_def_stack.peek()).numMolecules++;
                ReactionRegister.addElement(retval.listOfMolecules,(s1!=null?s1.name:null),(s1!=null?s1.information:null));
                ((species_def_scope)species_def_stack.peek()).variableName += (s1!=null?input.toString(s1.start,s1.stop):null);
                //in this step we are adding the label to local memory so that the expression grammar doesn't throw a variable not found error
                //i don't think it needs to make numerical sense.
                if((s1!=null?s1.myLabel:null) != null){
                  retval.lmemory.put((s1!=null?s1.myLabel:null),new Register(1.0,"observable"));
                }
               
            // BNGGrammar_SeedSpecies.g:137:3: ( DOT s2= species_element[bonds,speciesCounter + \"_M\" + $species_def::numMolecules,$scomp.text] )*
            loop14:
            do {
                int alt14=2;
                int LA14_0 = input.LA(1);

                if ( (LA14_0==DOT) ) {
                    alt14=1;
                }


                switch (alt14) {
            	case 1 :
            	    // BNGGrammar_SeedSpecies.g:137:4: DOT s2= species_element[bonds,speciesCounter + \"_M\" + $species_def::numMolecules,$scomp.text]
            	    {
            	    match(input,DOT,FOLLOW_DOT_in_species_def346); 
            	    pushFollow(FOLLOW_species_element_in_species_def350);
            	    s2=species_element(bonds, speciesCounter + "_M" + ((species_def_scope)species_def_stack.peek()).numMolecules, (scomp!=null?scomp.getText():null));

            	    state._fsp--;

            	    molecules.add(s2.st);
            	            ((species_def_scope)species_def_stack.peek()).numMolecules++;
            	             ((species_def_scope)species_def_stack.peek()).variableName += "."+ (s2!=null?input.toString(s2.start,s2.stop):null);
            	             if((s2!=null?s2.myLabel:null) != null){
            	              retval.lmemory.put((s2!=null?s2.myLabel:null),new Register(1.0,"observable"));
            	             }
            	            

            	    }
            	    break;

            	default :
            	    break loop14;
                }
            } while (true);


               //We are done with space recognition so we turn it off and get the next token that is not a space
              // ((ChangeableChannelTokenStream)input).setChannel(Token.DEFAULT_CHANNEL);
              // ((ChangeableChannelTokenStream)input).seek(((ChangeableChannelTokenStream)input).skipOffTokenChannels(((ChangeableChannelTokenStream)input).index()));
             
               

            }


                //((ChangeableChannelTokenStream)input).delChannel(HIDDEN);
                //if(!gParent.variableName.contains(((species_def_scope)species_def_stack.peek()).variableName) && gParent.conserveNewMolecules){
                  //gParent.variableName.add(((species_def_scope)species_def_stack.peek()).variableName);
                 // gParent.molecules.add(new StringTemplate(((species_def_scope)species_def_stack.peek()).variableName));
                 // }
                


            // TEMPLATE REWRITE
            // 161:4: -> molecules_def(id=$species_def::variableName)
            {
                retval.st = templateLib.getInstanceOf("molecules_def",
              new STAttrMap().put("id", ((species_def_scope)species_def_stack.peek()).variableName));
            }


            }

            retval.stop = input.LT(-1);


            ((ChangeableChannelTokenStream)input).delChannel(HIDDEN);
            //go back one space 
            ((ChangeableChannelTokenStream)input).seek(((ChangeableChannelTokenStream)input).index()-1);
            ((ChangeableChannelTokenStream)input).consume();

              gParent.paraphrases.pop();

        }
        catch (RecognitionException re) {
            reportError(re);
            recover(input,re);
        }
        finally {
            species_def_stack.pop();
        }
        return retval;
    }
    // $ANTLR end "species_def"

    protected static class species_element_scope {
        List sites;
        String lcompartment;
        String lname;
    }
    protected Stack species_element_stack = new Stack();

    public static class species_element_return extends ParserRuleReturnScope {
        public String myLabel;
        public String name;
        public ReactionRegister information;
        public StringTemplate st;
        public Object getTemplate() { return st; }
        public String toString() { return st==null?null:st.toString(); }
    };

    // $ANTLR start "species_element"
    // BNGGrammar_SeedSpecies.g:168:1: species_element[BondList bonds,String upperID,String compartment] returns [String myLabel,String name, ReactionRegister information] : s1= STRING ( label ) ( LPAREN site_list[$species_element::sites,bonds,upperID] RPAREN ) ( AT s2= STRING )? -> list_molecule_def(id=upperIDname=$s1.textsites=$species_element::sitescompartment=$species_element::lcompartmentlabel=$myLabel);
    public final NETGrammar_BNGGrammar_SeedSpecies.species_element_return species_element(BondList bonds, String upperID, String compartment) throws RecognitionException {
        species_element_stack.push(new species_element_scope());
        NETGrammar_BNGGrammar_SeedSpecies.species_element_return retval = new NETGrammar_BNGGrammar_SeedSpecies.species_element_return();
        retval.start = input.LT(1);

        Token s1=null;
        Token s2=null;
        NETGrammar.label_return label1 = null;



        getParentTemplate();
          ((species_element_scope)species_element_stack.peek()).sites = new ArrayList();
          ((species_element_scope)species_element_stack.peek()).lcompartment = compartment;
          retval.information = new ReactionRegister();
          retval.information.setNumBonds(bonds.getNumBonds());
          retval.information.setCompartment(compartment);
          

        try {
            // BNGGrammar_SeedSpecies.g:188:1: (s1= STRING ( label ) ( LPAREN site_list[$species_element::sites,bonds,upperID] RPAREN ) ( AT s2= STRING )? -> list_molecule_def(id=upperIDname=$s1.textsites=$species_element::sitescompartment=$species_element::lcompartmentlabel=$myLabel))
            // BNGGrammar_SeedSpecies.g:189:3: s1= STRING ( label ) ( LPAREN site_list[$species_element::sites,bonds,upperID] RPAREN ) ( AT s2= STRING )?
            {
            s1=(Token)match(input,STRING,FOLLOW_STRING_in_species_element454); 
            retval.name = (s1!=null?s1.getText():null);((species_element_scope)species_element_stack.peek()).lname =(s1!=null?s1.getText():null);
            // BNGGrammar_SeedSpecies.g:189:67: ( label )
            // BNGGrammar_SeedSpecies.g:189:68: label
            {
            pushFollow(FOLLOW_label_in_species_element459);
            label1=gNETGrammar.label();

            state._fsp--;

            retval.myLabel = (label1!=null?label1.label:null);

            }

            // BNGGrammar_SeedSpecies.g:190:3: ( LPAREN site_list[$species_element::sites,bonds,upperID] RPAREN )
            // BNGGrammar_SeedSpecies.g:190:4: LPAREN site_list[$species_element::sites,bonds,upperID] RPAREN
            {
            match(input,LPAREN,FOLLOW_LPAREN_in_species_element467); 
            pushFollow(FOLLOW_site_list_in_species_element469);
            site_list(((species_element_scope)species_element_stack.peek()).sites, bonds, upperID);

            state._fsp--;

            match(input,RPAREN,FOLLOW_RPAREN_in_species_element472); 

            }

            // BNGGrammar_SeedSpecies.g:193:3: ( AT s2= STRING )?
            int alt15=2;
            int LA15_0 = input.LA(1);

            if ( (LA15_0==AT) ) {
                int LA15_1 = input.LA(2);

                if ( (LA15_1==STRING) ) {
                    int LA15_3 = input.LA(3);

                    if ( (LA15_3==EOF||(LA15_3>=LB && LA15_3<=WS)||LA15_3==IF||LA15_3==NOT||(LA15_3>=EXP && LA15_3<=LOG)||(LA15_3>=FLOAT && LA15_3<=INT)||LA15_3==STRING||(LA15_3>=COMMA && LA15_3<=LPAREN)||(LA15_3>=UNI_REACTION_SIGN && LA15_3<=DOLLAR)||(LA15_3>=AT && LA15_3<=LT)||LA15_3==EQUALS||(LA15_3>=MINUS && LA15_3<=PLUS)||LA15_3==MOD) ) {
                        alt15=1;
                    }
                }
            }
            switch (alt15) {
                case 1 :
                    // BNGGrammar_SeedSpecies.g:193:4: AT s2= STRING
                    {
                    match(input,AT,FOLLOW_AT_in_species_element484); 
                    s2=(Token)match(input,STRING,FOLLOW_STRING_in_species_element488); 

                        ((species_element_scope)species_element_stack.peek()).lcompartment = (s2!=null?s2.getText():null); 
                        retval.information.setCompartment((s2!=null?s2.getText():null));
                        retval.information.setBondList(bonds);
                      

                    }
                    break;

            }



            // TEMPLATE REWRITE
            // 199:3: -> list_molecule_def(id=upperIDname=$s1.textsites=$species_element::sitescompartment=$species_element::lcompartmentlabel=$myLabel)
            {
                retval.st = templateLib.getInstanceOf("list_molecule_def",
              new STAttrMap().put("id", upperID).put("name", (s1!=null?s1.getText():null)).put("sites", ((species_element_scope)species_element_stack.peek()).sites).put("compartment", ((species_element_scope)species_element_stack.peek()).lcompartment).put("label", retval.myLabel));
            }


            }

            retval.stop = input.LT(-1);


              retval.information.setNumBonds(bonds.getNumBonds()-retval.information.getNumBonds());
             

        }
        catch (RecognitionException re) {
            reportError(re);
            recover(input,re);
        }
        finally {
            species_element_stack.pop();
        }
        return retval;
    }
    // $ANTLR end "species_element"

    protected static class site_list_scope {
        int numSites;
    }
    protected Stack site_list_stack = new Stack();

    public static class site_list_return extends ParserRuleReturnScope {
        public StringTemplate st;
        public Object getTemplate() { return st; }
        public String toString() { return st==null?null:st.toString(); }
    };

    // $ANTLR start "site_list"
    // BNGGrammar_SeedSpecies.g:201:1: site_list[List sites,BondList bonds,String upperID] : (s1= site_obs[bonds,upperID+\"_C\" + $site_list::numSites] ( COMMA s2= site_obs[bonds,upperID+\"_C\" + $site_list::numSites] )* )? ;
    public final NETGrammar_BNGGrammar_SeedSpecies.site_list_return site_list(List sites, BondList bonds, String upperID) throws RecognitionException {
        site_list_stack.push(new site_list_scope());
        NETGrammar_BNGGrammar_SeedSpecies.site_list_return retval = new NETGrammar_BNGGrammar_SeedSpecies.site_list_return();
        retval.start = input.LT(1);

        NETGrammar_BNGGrammar_SeedSpecies.site_obs_return s1 = null;

        NETGrammar_BNGGrammar_SeedSpecies.site_obs_return s2 = null;



          ((site_list_scope)site_list_stack.peek()).numSites = 1;

        try {
            // BNGGrammar_SeedSpecies.g:208:1: ( (s1= site_obs[bonds,upperID+\"_C\" + $site_list::numSites] ( COMMA s2= site_obs[bonds,upperID+\"_C\" + $site_list::numSites] )* )? )
            // BNGGrammar_SeedSpecies.g:209:3: (s1= site_obs[bonds,upperID+\"_C\" + $site_list::numSites] ( COMMA s2= site_obs[bonds,upperID+\"_C\" + $site_list::numSites] )* )?
            {
            // BNGGrammar_SeedSpecies.g:209:3: (s1= site_obs[bonds,upperID+\"_C\" + $site_list::numSites] ( COMMA s2= site_obs[bonds,upperID+\"_C\" + $site_list::numSites] )* )?
            int alt17=2;
            int LA17_0 = input.LA(1);

            if ( (LA17_0==STRING) ) {
                alt17=1;
            }
            switch (alt17) {
                case 1 :
                    // BNGGrammar_SeedSpecies.g:209:4: s1= site_obs[bonds,upperID+\"_C\" + $site_list::numSites] ( COMMA s2= site_obs[bonds,upperID+\"_C\" + $site_list::numSites] )*
                    {
                    pushFollow(FOLLOW_site_obs_in_site_list545);
                    s1=site_obs(bonds, upperID+"_C" + ((site_list_scope)site_list_stack.peek()).numSites);

                    state._fsp--;

                    sites.add(s1.st);((site_list_scope)site_list_stack.peek()).numSites++;
                    // BNGGrammar_SeedSpecies.g:210:3: ( COMMA s2= site_obs[bonds,upperID+\"_C\" + $site_list::numSites] )*
                    loop16:
                    do {
                        int alt16=2;
                        int LA16_0 = input.LA(1);

                        if ( (LA16_0==COMMA) ) {
                            alt16=1;
                        }


                        switch (alt16) {
                    	case 1 :
                    	    // BNGGrammar_SeedSpecies.g:210:4: COMMA s2= site_obs[bonds,upperID+\"_C\" + $site_list::numSites]
                    	    {
                    	    match(input,COMMA,FOLLOW_COMMA_in_site_list554); 
                    	    pushFollow(FOLLOW_site_obs_in_site_list558);
                    	    s2=site_obs(bonds, upperID+"_C" + ((site_list_scope)site_list_stack.peek()).numSites);

                    	    state._fsp--;

                    	    sites.add(s2.st);((site_list_scope)site_list_stack.peek()).numSites++;

                    	    }
                    	    break;

                    	default :
                    	    break loop16;
                        }
                    } while (true);


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
            site_list_stack.pop();
        }
        return retval;
    }
    // $ANTLR end "site_list"

    protected static class site_obs_scope {
        String defaultState;
        String bondIndicator;
    }
    protected Stack site_obs_stack = new Stack();

    public static class site_obs_return extends ParserRuleReturnScope {
        public String myLabel;
        public StringTemplate st;
        public Object getTemplate() { return st; }
        public String toString() { return st==null?null:st.toString(); }
    };

    // $ANTLR start "site_obs"
    // BNGGrammar_SeedSpecies.g:214:1: site_obs[BondList bonds,String upperID] returns [String myLabel] : s1= STRING ( ( (sos= site_obs_site[bonds,upperID] )? ( site_obs_state )? ) | site_obs_state sos2= site_obs_site[bonds,upperID] ) ( label )? -> component_def(id=upperIDname=$s1.textstate=$site_obs::defaultStatenumberOfBonds=$site_obs::bondIndicator);
    public final NETGrammar_BNGGrammar_SeedSpecies.site_obs_return site_obs(BondList bonds, String upperID) throws RecognitionException {
        site_obs_stack.push(new site_obs_scope());
        NETGrammar_BNGGrammar_SeedSpecies.site_obs_return retval = new NETGrammar_BNGGrammar_SeedSpecies.site_obs_return();
        retval.start = input.LT(1);

        Token s1=null;
        NETGrammar_BNGGrammar_SeedSpecies.site_obs_site_return sos = null;

        NETGrammar_BNGGrammar_SeedSpecies.site_obs_site_return sos2 = null;

        NETGrammar.label_return label2 = null;



          ((site_obs_scope)site_obs_stack.peek()).defaultState = null;
          ((site_obs_scope)site_obs_stack.peek()).bondIndicator = "0";
          
        try {
            // BNGGrammar_SeedSpecies.g:224:1: (s1= STRING ( ( (sos= site_obs_site[bonds,upperID] )? ( site_obs_state )? ) | site_obs_state sos2= site_obs_site[bonds,upperID] ) ( label )? -> component_def(id=upperIDname=$s1.textstate=$site_obs::defaultStatenumberOfBonds=$site_obs::bondIndicator))
            // BNGGrammar_SeedSpecies.g:225:3: s1= STRING ( ( (sos= site_obs_site[bonds,upperID] )? ( site_obs_state )? ) | site_obs_state sos2= site_obs_site[bonds,upperID] ) ( label )?
            {
            s1=(Token)match(input,STRING,FOLLOW_STRING_in_site_obs597); 
            // BNGGrammar_SeedSpecies.g:226:3: ( ( (sos= site_obs_site[bonds,upperID] )? ( site_obs_state )? ) | site_obs_state sos2= site_obs_site[bonds,upperID] )
            int alt20=2;
            alt20 = dfa20.predict(input);
            switch (alt20) {
                case 1 :
                    // BNGGrammar_SeedSpecies.g:227:3: ( (sos= site_obs_site[bonds,upperID] )? ( site_obs_state )? )
                    {
                    // BNGGrammar_SeedSpecies.g:227:3: ( (sos= site_obs_site[bonds,upperID] )? ( site_obs_state )? )
                    // BNGGrammar_SeedSpecies.g:227:4: (sos= site_obs_site[bonds,upperID] )? ( site_obs_state )?
                    {
                    // BNGGrammar_SeedSpecies.g:227:4: (sos= site_obs_site[bonds,upperID] )?
                    int alt18=2;
                    int LA18_0 = input.LA(1);

                    if ( (LA18_0==EMARK) ) {
                        alt18=1;
                    }
                    switch (alt18) {
                        case 1 :
                            // BNGGrammar_SeedSpecies.g:227:5: sos= site_obs_site[bonds,upperID]
                            {
                            pushFollow(FOLLOW_site_obs_site_in_site_obs610);
                            sos=site_obs_site(bonds, upperID);

                            state._fsp--;

                            ((site_obs_scope)site_obs_stack.peek()).bondIndicator = (sos!=null?sos.bond:null);

                            }
                            break;

                    }

                    // BNGGrammar_SeedSpecies.g:227:79: ( site_obs_state )?
                    int alt19=2;
                    int LA19_0 = input.LA(1);

                    if ( (LA19_0==TILDE) ) {
                        alt19=1;
                    }
                    switch (alt19) {
                        case 1 :
                            // BNGGrammar_SeedSpecies.g:227:79: site_obs_state
                            {
                            pushFollow(FOLLOW_site_obs_state_in_site_obs616);
                            site_obs_state();

                            state._fsp--;


                            }
                            break;

                    }


                    }


                    }
                    break;
                case 2 :
                    // BNGGrammar_SeedSpecies.g:228:5: site_obs_state sos2= site_obs_site[bonds,upperID]
                    {
                    pushFollow(FOLLOW_site_obs_state_in_site_obs624);
                    site_obs_state();

                    state._fsp--;

                    pushFollow(FOLLOW_site_obs_site_in_site_obs628);
                    sos2=site_obs_site(bonds, upperID);

                    state._fsp--;

                    ((site_obs_scope)site_obs_stack.peek()).bondIndicator = (sos2!=null?sos2.bond:null);

                    }
                    break;

            }

            // BNGGrammar_SeedSpecies.g:230:8: ( label )?
            int alt21=2;
            int LA21_0 = input.LA(1);

            if ( (LA21_0==MOD) ) {
                alt21=1;
            }
            switch (alt21) {
                case 1 :
                    // BNGGrammar_SeedSpecies.g:230:9: label
                    {
                    pushFollow(FOLLOW_label_in_site_obs643);
                    label2=gNETGrammar.label();

                    state._fsp--;

                    retval.myLabel = (label2!=null?input.toString(label2.start,label2.stop):null);

                    }
                    break;

            }



            // TEMPLATE REWRITE
            // 231:3: -> component_def(id=upperIDname=$s1.textstate=$site_obs::defaultStatenumberOfBonds=$site_obs::bondIndicator)
            {
                retval.st = templateLib.getInstanceOf("component_def",
              new STAttrMap().put("id", upperID).put("name", (s1!=null?s1.getText():null)).put("state", ((site_obs_scope)site_obs_stack.peek()).defaultState).put("numberOfBonds", ((site_obs_scope)site_obs_stack.peek()).bondIndicator));
            }


            }

            retval.stop = input.LT(-1);

        }
        catch (RecognitionException re) {
            reportError(re);
            recover(input,re);
        }
        finally {
            site_obs_stack.pop();
        }
        return retval;
    }
    // $ANTLR end "site_obs"

    public static class site_obs_state_return extends ParserRuleReturnScope {
        public StringTemplate st;
        public Object getTemplate() { return st; }
        public String toString() { return st==null?null:st.toString(); }
    };

    // $ANTLR start "site_obs_state"
    // BNGGrammar_SeedSpecies.g:233:1: site_obs_state : ( TILDE state_label ) ;
    public final NETGrammar_BNGGrammar_SeedSpecies.site_obs_state_return site_obs_state() throws RecognitionException {
        NETGrammar_BNGGrammar_SeedSpecies.site_obs_state_return retval = new NETGrammar_BNGGrammar_SeedSpecies.site_obs_state_return();
        retval.start = input.LT(1);

        NETGrammar_BNGGrammar_MoleculeDef.state_label_return state_label3 = null;


        try {
            // BNGGrammar_SeedSpecies.g:233:15: ( ( TILDE state_label ) )
            // BNGGrammar_SeedSpecies.g:233:17: ( TILDE state_label )
            {
            // BNGGrammar_SeedSpecies.g:233:17: ( TILDE state_label )
            // BNGGrammar_SeedSpecies.g:233:18: TILDE state_label
            {
            match(input,TILDE,FOLLOW_TILDE_in_site_obs_state679); 
            pushFollow(FOLLOW_state_label_in_site_obs_state681);
            state_label3=gNETGrammar.state_label();

            state._fsp--;

            ((site_obs_scope)site_obs_stack.peek()).defaultState = (state_label3!=null?input.toString(state_label3.start,state_label3.stop):null);

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
    // $ANTLR end "site_obs_state"

    protected static class site_obs_site_scope {
        int acc;
        String bondIndicator;
    }
    protected Stack site_obs_site_stack = new Stack();

    public static class site_obs_site_return extends ParserRuleReturnScope {
        public String bond;
        public StringTemplate st;
        public Object getTemplate() { return st; }
        public String toString() { return st==null?null:st.toString(); }
    };

    // $ANTLR start "site_obs_site"
    // BNGGrammar_SeedSpecies.g:235:1: site_obs_site[BondList bonds,String upperID] returns [String bond] : ( EMARK ( ( PLUS ) | ( QMARK ) | s1= bond_name ) )+ ;
    public final NETGrammar_BNGGrammar_SeedSpecies.site_obs_site_return site_obs_site(BondList bonds, String upperID) throws RecognitionException {
        site_obs_site_stack.push(new site_obs_site_scope());
        NETGrammar_BNGGrammar_SeedSpecies.site_obs_site_return retval = new NETGrammar_BNGGrammar_SeedSpecies.site_obs_site_return();
        retval.start = input.LT(1);

        Token PLUS4=null;
        Token QMARK5=null;
        NETGrammar_BNGGrammar_SeedSpecies.bond_name_return s1 = null;



          ((site_obs_site_scope)site_obs_site_stack.peek()).acc = 0;
          ((site_obs_site_scope)site_obs_site_stack.peek()).bondIndicator = "";
          
        try {
            // BNGGrammar_SeedSpecies.g:244:3: ( ( EMARK ( ( PLUS ) | ( QMARK ) | s1= bond_name ) )+ )
            // BNGGrammar_SeedSpecies.g:245:9: ( EMARK ( ( PLUS ) | ( QMARK ) | s1= bond_name ) )+
            {
            // BNGGrammar_SeedSpecies.g:245:9: ( EMARK ( ( PLUS ) | ( QMARK ) | s1= bond_name ) )+
            int cnt23=0;
            loop23:
            do {
                int alt23=2;
                int LA23_0 = input.LA(1);

                if ( (LA23_0==EMARK) ) {
                    alt23=1;
                }


                switch (alt23) {
            	case 1 :
            	    // BNGGrammar_SeedSpecies.g:245:10: EMARK ( ( PLUS ) | ( QMARK ) | s1= bond_name )
            	    {
            	    match(input,EMARK,FOLLOW_EMARK_in_site_obs_site715); 
            	    // BNGGrammar_SeedSpecies.g:245:16: ( ( PLUS ) | ( QMARK ) | s1= bond_name )
            	    int alt22=3;
            	    switch ( input.LA(1) ) {
            	    case PLUS:
            	        {
            	        alt22=1;
            	        }
            	        break;
            	    case QMARK:
            	        {
            	        alt22=2;
            	        }
            	        break;
            	    case INT:
            	    case STRING:
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
            	            // BNGGrammar_SeedSpecies.g:245:17: ( PLUS )
            	            {
            	            // BNGGrammar_SeedSpecies.g:245:17: ( PLUS )
            	            // BNGGrammar_SeedSpecies.g:245:18: PLUS
            	            {
            	            PLUS4=(Token)match(input,PLUS,FOLLOW_PLUS_in_site_obs_site719); 
            	            ((site_obs_site_scope)site_obs_site_stack.peek()).bondIndicator += (PLUS4!=null?PLUS4.getText():null);

            	            }


            	            }
            	            break;
            	        case 2 :
            	            // BNGGrammar_SeedSpecies.g:246:11: ( QMARK )
            	            {
            	            // BNGGrammar_SeedSpecies.g:246:11: ( QMARK )
            	            // BNGGrammar_SeedSpecies.g:246:12: QMARK
            	            {
            	            QMARK5=(Token)match(input,QMARK,FOLLOW_QMARK_in_site_obs_site735); 
            	            ((site_obs_site_scope)site_obs_site_stack.peek()).bondIndicator += (QMARK5!=null?QMARK5.getText():null);

            	            }


            	            }
            	            break;
            	        case 3 :
            	            // BNGGrammar_SeedSpecies.g:247:11: s1= bond_name
            	            {
            	            pushFollow(FOLLOW_bond_name_in_site_obs_site752);
            	            s1=bond_name();

            	            state._fsp--;


            	                            bonds.addElement((s1!=null?input.toString(s1.start,s1.stop):null),upperID,((species_element_scope)species_element_stack.peek()).lname);  
            	                            ((site_obs_site_scope)site_obs_site_stack.peek()).acc++;
            	                            

            	            }
            	            break;

            	    }


            	    }
            	    break;

            	default :
            	    if ( cnt23 >= 1 ) break loop23;
                        EarlyExitException eee =
                            new EarlyExitException(23, input);
                        throw eee;
                }
                cnt23++;
            } while (true);


                       if(((site_obs_site_scope)site_obs_site_stack.peek()).acc >0)
                        retval.bond = ((site_obs_site_scope)site_obs_site_stack.peek()).acc + ((site_obs_site_scope)site_obs_site_stack.peek()).bondIndicator;
                       else if(!((site_obs_site_scope)site_obs_site_stack.peek()).bondIndicator.equals(""))
                        retval.bond = ((site_obs_site_scope)site_obs_site_stack.peek()).bondIndicator;
                       else 
                         retval.bond = "0";
                    

            }

            retval.stop = input.LT(-1);

        }
        catch (RecognitionException re) {
            reportError(re);
            recover(input,re);
        }
        finally {
            site_obs_site_stack.pop();
        }
        return retval;
    }
    // $ANTLR end "site_obs_site"

    public static class bond_name_return extends ParserRuleReturnScope {
        public StringTemplate st;
        public Object getTemplate() { return st; }
        public String toString() { return st==null?null:st.toString(); }
    };

    // $ANTLR start "bond_name"
    // BNGGrammar_SeedSpecies.g:262:1: bond_name : ( INT | STRING );
    public final NETGrammar_BNGGrammar_SeedSpecies.bond_name_return bond_name() throws RecognitionException {
        NETGrammar_BNGGrammar_SeedSpecies.bond_name_return retval = new NETGrammar_BNGGrammar_SeedSpecies.bond_name_return();
        retval.start = input.LT(1);

        try {
            // BNGGrammar_SeedSpecies.g:262:10: ( INT | STRING )
            // BNGGrammar_SeedSpecies.g:
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
    // $ANTLR end "bond_name"

    // Delegated rules


    protected DFA20 dfa20 = new DFA20(this);
    static final String DFA20_eotS =
        "\5\uffff";
    static final String DFA20_eofS =
        "\5\uffff";
    static final String DFA20_minS =
        "\1\172\1\uffff\2\161\1\uffff";
    static final String DFA20_maxS =
        "\1\u0092\1\uffff\1\u0091\1\u0092\1\uffff";
    static final String DFA20_acceptS =
        "\1\uffff\1\1\2\uffff\1\2";
    static final String DFA20_specialS =
        "\5\uffff}>";
    static final String[] DFA20_transitionS = {
            "\1\1\2\uffff\1\1\3\uffff\1\2\15\uffff\1\1\2\uffff\1\1",
            "",
            "\1\3\1\uffff\1\3\35\uffff\1\3",
            "\1\3\1\uffff\1\3\6\uffff\1\1\2\uffff\1\1\21\uffff\1\1\1\uffff"+
            "\1\3\1\4",
            ""
    };

    static final short[] DFA20_eot = DFA.unpackEncodedString(DFA20_eotS);
    static final short[] DFA20_eof = DFA.unpackEncodedString(DFA20_eofS);
    static final char[] DFA20_min = DFA.unpackEncodedStringToUnsignedChars(DFA20_minS);
    static final char[] DFA20_max = DFA.unpackEncodedStringToUnsignedChars(DFA20_maxS);
    static final short[] DFA20_accept = DFA.unpackEncodedString(DFA20_acceptS);
    static final short[] DFA20_special = DFA.unpackEncodedString(DFA20_specialS);
    static final short[][] DFA20_transition;

    static {
        int numStates = DFA20_transitionS.length;
        DFA20_transition = new short[numStates][];
        for (int i=0; i<numStates; i++) {
            DFA20_transition[i] = DFA.unpackEncodedString(DFA20_transitionS[i]);
        }
    }

    class DFA20 extends DFA {

        public DFA20(BaseRecognizer recognizer) {
            this.recognizer = recognizer;
            this.decisionNumber = 20;
            this.eot = DFA20_eot;
            this.eof = DFA20_eof;
            this.min = DFA20_min;
            this.max = DFA20_max;
            this.accept = DFA20_accept;
            this.special = DFA20_special;
            this.transition = DFA20_transition;
        }
        public String getDescription() {
            return "226:3: ( ( (sos= site_obs_site[bonds,upperID] )? ( site_obs_state )? ) | site_obs_state sos2= site_obs_site[bonds,upperID] )";
        }
    }
 

    public static final BitSet FOLLOW_AT_in_species_def249 = new BitSet(new long[]{0x0000000000000000L,0x0008000000000000L});
    public static final BitSet FOLLOW_STRING_in_species_def253 = new BitSet(new long[]{0x0000000000000000L,0x0020000000000000L});
    public static final BitSet FOLLOW_COLON_in_species_def268 = new BitSet(new long[]{0x0000000000000000L,0x0028000000000000L,0x0000000000008001L});
    public static final BitSet FOLLOW_COLON_in_species_def273 = new BitSet(new long[]{0x0000000000000000L,0x0028000000000000L,0x0000000000008001L});
    public static final BitSet FOLLOW_MOD_in_species_def291 = new BitSet(new long[]{0x0000000000000000L,0x0008000000000000L});
    public static final BitSet FOLLOW_STRING_in_species_def295 = new BitSet(new long[]{0x0000000000000000L,0x0020000000000000L});
    public static final BitSet FOLLOW_COLON_in_species_def297 = new BitSet(new long[]{0x0000000000000000L,0x0028000000000000L,0x0000000000008001L});
    public static final BitSet FOLLOW_COLON_in_species_def299 = new BitSet(new long[]{0x0000000000000000L,0x0028000000000000L,0x0000000000008001L});
    public static final BitSet FOLLOW_DOLLAR_in_species_def318 = new BitSet(new long[]{0x0000000000000000L,0x0028000000000000L,0x0000000000008001L});
    public static final BitSet FOLLOW_species_element_in_species_def334 = new BitSet(new long[]{0x0000000000000002L,0x0800000000000000L});
    public static final BitSet FOLLOW_DOT_in_species_def346 = new BitSet(new long[]{0x0000000000000000L,0x0028000000000000L,0x0000000000008001L});
    public static final BitSet FOLLOW_species_element_in_species_def350 = new BitSet(new long[]{0x0000000000000002L,0x0800000000000000L});
    public static final BitSet FOLLOW_STRING_in_species_element454 = new BitSet(new long[]{0x0000000000000000L,0x0000000000000000L,0x0000000000008000L});
    public static final BitSet FOLLOW_label_in_species_element459 = new BitSet(new long[]{0x0000000000000000L,0x1000000000000000L});
    public static final BitSet FOLLOW_LPAREN_in_species_element467 = new BitSet(new long[]{0x0000000000000000L,0x2008000000000000L});
    public static final BitSet FOLLOW_site_list_in_species_element469 = new BitSet(new long[]{0x0000000000000000L,0x2000000000000000L});
    public static final BitSet FOLLOW_RPAREN_in_species_element472 = new BitSet(new long[]{0x0000000000000002L,0x0000000000000000L,0x0000000000000004L});
    public static final BitSet FOLLOW_AT_in_species_element484 = new BitSet(new long[]{0x0000000000000000L,0x0008000000000000L});
    public static final BitSet FOLLOW_STRING_in_species_element488 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_site_obs_in_site_list545 = new BitSet(new long[]{0x0000000000000002L,0x0400000000000000L});
    public static final BitSet FOLLOW_COMMA_in_site_list554 = new BitSet(new long[]{0x0000000000000000L,0x0008000000000000L});
    public static final BitSet FOLLOW_site_obs_in_site_list558 = new BitSet(new long[]{0x0000000000000002L,0x0400000000000000L});
    public static final BitSet FOLLOW_STRING_in_site_obs597 = new BitSet(new long[]{0x0000000000000002L,0x0000000000000000L,0x0000000000048002L});
    public static final BitSet FOLLOW_site_obs_site_in_site_obs610 = new BitSet(new long[]{0x0000000000000002L,0x0000000000000000L,0x0000000000008002L});
    public static final BitSet FOLLOW_site_obs_state_in_site_obs616 = new BitSet(new long[]{0x0000000000000002L,0x0000000000000000L,0x0000000000008000L});
    public static final BitSet FOLLOW_site_obs_state_in_site_obs624 = new BitSet(new long[]{0x0000000000000000L,0x0000000000000000L,0x0000000000040000L});
    public static final BitSet FOLLOW_site_obs_site_in_site_obs628 = new BitSet(new long[]{0x0000000000000002L,0x0000000000000000L,0x0000000000008000L});
    public static final BitSet FOLLOW_label_in_site_obs643 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_TILDE_in_site_obs_state679 = new BitSet(new long[]{0x0000000000000000L,0x000A000000000000L,0x0000000000020000L});
    public static final BitSet FOLLOW_state_label_in_site_obs_state681 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_EMARK_in_site_obs_site715 = new BitSet(new long[]{0x0000000000000000L,0x000A000000000000L,0x0000000000022000L});
    public static final BitSet FOLLOW_PLUS_in_site_obs_site719 = new BitSet(new long[]{0x0000000000000002L,0x0000000000000000L,0x0000000000040000L});
    public static final BitSet FOLLOW_QMARK_in_site_obs_site735 = new BitSet(new long[]{0x0000000000000002L,0x0000000000000000L,0x0000000000040000L});
    public static final BitSet FOLLOW_bond_name_in_site_obs_site752 = new BitSet(new long[]{0x0000000000000002L,0x0000000000000000L,0x0000000000040000L});
    public static final BitSet FOLLOW_set_in_bond_name0 = new BitSet(new long[]{0x0000000000000002L});

}