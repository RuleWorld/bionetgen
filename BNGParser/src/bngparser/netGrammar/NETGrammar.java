// $ANTLR 3.3 Nov 30, 2010 12:50:56 /home/proto/workspace/bionetgen/BNGParser/src/bngparser/netGrammar/NETGrammar.g 2012-02-06 17:01:16

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
public class NETGrammar extends Parser {
    public static final String[] tokenNames = new String[] {
        "<invalid>", "<EOR>", "<DOWN>", "<UP>", "LINE_COMMENT", "WS", "VERSION", "END", "BEGIN", "SEED", "SPECIES", "GROUPS", "MOLECULE", "MOLECULES", "MODEL", "TYPES", "REACTION", "REACTIONS", "RULES", "FUNCTIONS", "COMPARTMENTS", "ACTIONS", "OBSERVABLES", "SET_OPTION", "SPECIES_LABEL", "GENERATE_NETWORK", "MAX_AGG", "MAX_ITER", "MAX_STOICH", "OVERWRITE", "PRINT_ITER", "AND", "OR", "ID", "PREFFIX", "SUFFIX", "LABEL", "PARAMETERS", "MATCHONCE", "DELETEMOLECULES", "MOVECONNECTED", "AUTO", "HNAUTY", "QUASY", "NOT", "INCLUDE_REACTANTS", "INCLUDE_PRODUCTS", "EXCLUDE_REACTANTS", "EXCLUDE_PRODUCTS", "ATOL", "RTOL", "STEADY_STATE", "SPARSE", "T_END", "T_START", "N_STEPS", "SAMPLE_TIMES", "SIMULATE_ODE", "SIMULATE_SSA", "READFILE", "FILE", "WRITENET", "WRITESBML", "WRITEMFILE", "WRITEXML", "SETCONCENTRATION", "SETPARAMETER", "SAVECONCENTRATIONS", "RESETCONCENTRATIONS", "GENERATENETWORK", "SUBSTANCEUNITS", "DIGIT", "EXPONENT", "FLOAT", "INT", "LETTER", "STRING", "SEMI", "COLON", "LSBRACKET", "RSBRACKET", "LBRACKET", "RBRACKET", "COMMA", "DOT", "LPAREN", "RPAREN", "UNI_REACTION_SIGN", "BI_REACTION_SIGN", "DOLLAR", "TILDE", "AT", "GTE", "GT", "LTE", "LT", "ASSIGNS", "EQUALS", "BECOMES", "DIV", "TIMES", "MINUS", "PLUS", "MOD", "QMARK", "EMARK", "DBQUOTES", "AMPERSAND", "NULL", "LOWER", "UPPER", "VERSION_NUMBER", "112", "113", "114", "115", "116", "117", "118", "119", "120", "121", "122", "123", "124", "125", "126", "127", "128", "129", "130", "131", "132", "133", "134", "135", "136", "137", "138", "139", "140", "141", "142", "143", "144", "145", "146", "147", "148", "149", "150", "151", "152", "153", "154", "155", "156", "157", "158", "159", "160", "161"
    };
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
    public NETGrammar_BNGGrammar_Expression gBNGGrammar_Expression;
    public NETGrammar_BNGGrammar_Parameters gBNGGrammar_Parameters;
    public NETGrammar_BNGGrammar_SeedSpecies gBNGGrammar_SeedSpecies;
    public NETGrammar_BNGGrammar_MoleculeDef gBNGGrammar_MoleculeDef;
    public NETGrammar_BNGGrammar_Observables gBNGGrammar_Observables;
    public NETGrammar_BNGGrammar_ReactionRules gBNGGrammar_ReactionRules;
    // delegators


        public NETGrammar(TokenStream input) {
            this(input, new RecognizerSharedState());
        }
        public NETGrammar(TokenStream input, RecognizerSharedState state) {
            super(input, state);
            gBNGGrammar_Expression = new NETGrammar_BNGGrammar_Expression(input, state, this);
            gBNGGrammar_Parameters = new NETGrammar_BNGGrammar_Parameters(input, state, this);
            gBNGGrammar_SeedSpecies = new NETGrammar_BNGGrammar_SeedSpecies(input, state, this);
            gBNGGrammar_MoleculeDef = new NETGrammar_BNGGrammar_MoleculeDef(input, state, this);
            gBNGGrammar_Observables = new NETGrammar_BNGGrammar_Observables(input, state, this);
            gBNGGrammar_ReactionRules = new NETGrammar_BNGGrammar_ReactionRules(input, state, this);         
        }
        
    protected StringTemplateGroup templateLib =
      new StringTemplateGroup("NETGrammarTemplates", AngleBracketTemplateLexer.class);

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
    public String getGrammarFileName() { return "/home/proto/workspace/bionetgen/BNGParser/src/bngparser/netGrammar/NETGrammar.g"; }


      public Stack<String> paraphrases = new Stack<String>();
      public Map<String,Register> memory = new HashMap<String,Register>();
      
      private Map<String, Map<String,String>> options = new HashMap<String, Map<String,String>>();

      public boolean netGrammar = true;
     
      public List<StringTemplate> molecules = new ArrayList<StringTemplate>();
      public List<StringTemplate> reactions = new ArrayList<StringTemplate>();
      public List<StringTemplate> groups = new ArrayList<StringTemplate>();
      public Map<String,String> speciesMap = new HashMap<String,String>();
      
      @Override
      public String getErrorMessage(RecognitionException e,String[] tokenNames){
        String msg = super.getErrorMessage(e,tokenNames);
        if(paraphrases.size(  )>0) {
          String paraphrase = paraphrases.peek();
          msg += " " + paraphrase;
        }
        return msg;
      }
      
      public void addOption(String element, String option, String setting){
        if(!options.containsKey(element)){
          options.put(element,new HashMap<String,String>());
        }
        
        (options.get(element)).put(option,setting);
      }
      
      public void addOptions(String element, Map<String,String> option){
        if(!options.containsKey(element)){
          options.put(element,option);
        }
        else{
          options.get(element).putAll(option);
        }
      }
      


    protected static class seed_species_def_scope {
        List molecules;
        BondList bonds;
    }
    protected Stack seed_species_def_stack = new Stack();

    public static class seed_species_def_return extends ParserRuleReturnScope {
        public StringTemplate st;
        public Object getTemplate() { return st; }
        public String toString() { return st==null?null:st.toString(); }
    };

    // $ANTLR start "seed_species_def"
    // /home/proto/workspace/bionetgen/BNGParser/src/bngparser/netGrammar/NETGrammar.g:67:1: seed_species_def[int counter] : pre_species_def[$seed_species_def::molecules,$seed_species_def::bonds,counter] WS expression[memory] -> seed_species_block(id=counterconcentration=$expression.textname=$pre_species_def.textmolecules=$seed_species_def::moleculesfirstBonds=$seed_species_def::bonds.getLeft()secondBonds=$seed_species_def::bonds.getRight());
    public final NETGrammar.seed_species_def_return seed_species_def(int counter) throws RecognitionException {
        seed_species_def_stack.push(new seed_species_def_scope());
        NETGrammar.seed_species_def_return retval = new NETGrammar.seed_species_def_return();
        retval.start = input.LT(1);

        NETGrammar_BNGGrammar_Expression.expression_return expression1 = null;

        NETGrammar.pre_species_def_return pre_species_def2 = null;



          ((seed_species_def_scope)seed_species_def_stack.peek()).molecules = new ArrayList();
          ((seed_species_def_scope)seed_species_def_stack.peek()).bonds = new BondList();

        try {
            // /home/proto/workspace/bionetgen/BNGParser/src/bngparser/netGrammar/NETGrammar.g:77:1: ( pre_species_def[$seed_species_def::molecules,$seed_species_def::bonds,counter] WS expression[memory] -> seed_species_block(id=counterconcentration=$expression.textname=$pre_species_def.textmolecules=$seed_species_def::moleculesfirstBonds=$seed_species_def::bonds.getLeft()secondBonds=$seed_species_def::bonds.getRight()))
            // /home/proto/workspace/bionetgen/BNGParser/src/bngparser/netGrammar/NETGrammar.g:78:2: pre_species_def[$seed_species_def::molecules,$seed_species_def::bonds,counter] WS expression[memory]
            {
            pushFollow(FOLLOW_pre_species_def_in_seed_species_def82);
            pre_species_def2=pre_species_def(((seed_species_def_scope)seed_species_def_stack.peek()).molecules, ((seed_species_def_scope)seed_species_def_stack.peek()).bonds, counter);

            state._fsp--;


             //There needs to need a space between species and the expression token, so we go back and make sure there was one
            ((ChangeableChannelTokenStream)input).seek(((ChangeableChannelTokenStream)input).index()-1)  ;
            // System.out.println(input);

            match(input,WS,FOLLOW_WS_in_seed_species_def94); 
            pushFollow(FOLLOW_expression_in_seed_species_def102);
            expression1=expression(memory);

            state._fsp--;



            // TEMPLATE REWRITE
            // 86:6: -> seed_species_block(id=counterconcentration=$expression.textname=$pre_species_def.textmolecules=$seed_species_def::moleculesfirstBonds=$seed_species_def::bonds.getLeft()secondBonds=$seed_species_def::bonds.getRight())
            {
                retval.st = templateLib.getInstanceOf("seed_species_block",
              new STAttrMap().put("id", counter).put("concentration", (expression1!=null?input.toString(expression1.start,expression1.stop):null)).put("name", (pre_species_def2!=null?input.toString(pre_species_def2.start,pre_species_def2.stop):null)).put("molecules", ((seed_species_def_scope)seed_species_def_stack.peek()).molecules).put("firstBonds", ((seed_species_def_scope)seed_species_def_stack.peek()).bonds.getLeft()).put("secondBonds", ((seed_species_def_scope)seed_species_def_stack.peek()).bonds.getRight()));
            }


            }

            retval.stop = input.LT(-1);

        }
        catch (RecognitionException re) {
            reportError(re);
            recover(input,re);
        }
        finally {
            seed_species_def_stack.pop();
        }
        return retval;
    }
    // $ANTLR end "seed_species_def"

    protected static class pre_species_def_scope {
        boolean constant;
    }
    protected Stack pre_species_def_stack = new Stack();

    public static class pre_species_def_return extends ParserRuleReturnScope {
        public String compartment;
        public StringTemplate st;
        public Object getTemplate() { return st; }
        public String toString() { return st==null?null:st.toString(); }
    };

    // $ANTLR start "pre_species_def"
    // /home/proto/workspace/bionetgen/BNGParser/src/bngparser/netGrammar/NETGrammar.g:92:1: pre_species_def[List molecules,BondList bonds, int speciesCounter] returns [String compartment] : INT species_def[molecules,bonds,\"S\" + speciesCounter] ;
    public final NETGrammar.pre_species_def_return pre_species_def(List molecules, BondList bonds, int speciesCounter) throws RecognitionException {
        pre_species_def_stack.push(new pre_species_def_scope());
        NETGrammar.pre_species_def_return retval = new NETGrammar.pre_species_def_return();
        retval.start = input.LT(1);

        Token INT4=null;
        NETGrammar_BNGGrammar_SeedSpecies.species_def_return species_def3 = null;


        try {
            // /home/proto/workspace/bionetgen/BNGParser/src/bngparser/netGrammar/NETGrammar.g:98:1: ( INT species_def[molecules,bonds,\"S\" + speciesCounter] )
            // /home/proto/workspace/bionetgen/BNGParser/src/bngparser/netGrammar/NETGrammar.g:99:1: INT species_def[molecules,bonds,\"S\" + speciesCounter]
            {
            INT4=(Token)match(input,INT,FOLLOW_INT_in_pre_species_def175); 
            pushFollow(FOLLOW_species_def_in_pre_species_def181);
            species_def3=species_def(molecules, bonds, "S" + speciesCounter);

            state._fsp--;


                ((pre_species_def_scope)pre_species_def_stack.peek()).constant = (species_def3!=null?species_def3.constant:false);
                 speciesMap.put((species_def3!=null?input.toString(species_def3.start,species_def3.stop):null),(INT4!=null?INT4.getText():null));
             

            }

            retval.stop = input.LT(-1);

        }
        catch (RecognitionException re) {
            reportError(re);
            recover(input,re);
        }
        finally {
            pre_species_def_stack.pop();
        }
        return retval;
    }
    // $ANTLR end "pre_species_def"

    public static class groups_block_return extends ParserRuleReturnScope {
        public StringTemplate st;
        public Object getTemplate() { return st; }
        public String toString() { return st==null?null:st.toString(); }
    };

    // $ANTLR start "groups_block"
    // /home/proto/workspace/bionetgen/BNGParser/src/bngparser/netGrammar/NETGrammar.g:110:1: groups_block : BEGIN GROUPS ( group_line )* END GROUPS ;
    public final NETGrammar.groups_block_return groups_block() throws RecognitionException {
        NETGrammar.groups_block_return retval = new NETGrammar.groups_block_return();
        retval.start = input.LT(1);

        try {
            // /home/proto/workspace/bionetgen/BNGParser/src/bngparser/netGrammar/NETGrammar.g:110:13: ( BEGIN GROUPS ( group_line )* END GROUPS )
            // /home/proto/workspace/bionetgen/BNGParser/src/bngparser/netGrammar/NETGrammar.g:111:1: BEGIN GROUPS ( group_line )* END GROUPS
            {
            match(input,BEGIN,FOLLOW_BEGIN_in_groups_block208); 
            match(input,GROUPS,FOLLOW_GROUPS_in_groups_block210); 
            // /home/proto/workspace/bionetgen/BNGParser/src/bngparser/netGrammar/NETGrammar.g:112:3: ( group_line )*
            loop1:
            do {
                int alt1=2;
                int LA1_0 = input.LA(1);

                if ( (LA1_0==INT) ) {
                    alt1=1;
                }


                switch (alt1) {
            	case 1 :
            	    // /home/proto/workspace/bionetgen/BNGParser/src/bngparser/netGrammar/NETGrammar.g:112:3: group_line
            	    {
            	    pushFollow(FOLLOW_group_line_in_groups_block214);
            	    group_line();

            	    state._fsp--;


            	    }
            	    break;

            	default :
            	    break loop1;
                }
            } while (true);

            match(input,END,FOLLOW_END_in_groups_block217); 
            match(input,GROUPS,FOLLOW_GROUPS_in_groups_block219); 

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
    // $ANTLR end "groups_block"

    public static class group_line_return extends ParserRuleReturnScope {
        public StringTemplate st;
        public Object getTemplate() { return st; }
        public String toString() { return st==null?null:st.toString(); }
    };

    // $ANTLR start "group_line"
    // /home/proto/workspace/bionetgen/BNGParser/src/bngparser/netGrammar/NETGrammar.g:117:1: group_line : INT STRING group_list[$STRING.text] ;
    public final NETGrammar.group_line_return group_line() throws RecognitionException {
        NETGrammar.group_line_return retval = new NETGrammar.group_line_return();
        retval.start = input.LT(1);

        Token STRING5=null;

        try {
            // /home/proto/workspace/bionetgen/BNGParser/src/bngparser/netGrammar/NETGrammar.g:117:11: ( INT STRING group_list[$STRING.text] )
            // /home/proto/workspace/bionetgen/BNGParser/src/bngparser/netGrammar/NETGrammar.g:118:1: INT STRING group_list[$STRING.text]
            {
            match(input,INT,FOLLOW_INT_in_group_line228); 
            STRING5=(Token)match(input,STRING,FOLLOW_STRING_in_group_line230); 
            pushFollow(FOLLOW_group_list_in_group_line232);
            group_list((STRING5!=null?STRING5.getText():null));

            state._fsp--;


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
    // $ANTLR end "group_line"

    protected static class group_list_scope {
        int counter;
    }
    protected Stack group_list_stack = new Stack();

    public static class group_list_return extends ParserRuleReturnScope {
        public StringTemplate st;
        public Object getTemplate() { return st; }
        public String toString() { return st==null?null:st.toString(); }
    };

    // $ANTLR start "group_list"
    // /home/proto/workspace/bionetgen/BNGParser/src/bngparser/netGrammar/NETGrammar.g:121:1: group_list[String name] : g1= group_element[$name,$group_list::counter] ( COMMA g2= group_element[$name,$group_list::counter] )* ;
    public final NETGrammar.group_list_return group_list(String name) throws RecognitionException {
        group_list_stack.push(new group_list_scope());
        NETGrammar.group_list_return retval = new NETGrammar.group_list_return();
        retval.start = input.LT(1);

        NETGrammar.group_element_return g1 = null;

        NETGrammar.group_element_return g2 = null;



          ((group_list_scope)group_list_stack.peek()).counter = 0;

        try {
            // /home/proto/workspace/bionetgen/BNGParser/src/bngparser/netGrammar/NETGrammar.g:127:2: (g1= group_element[$name,$group_list::counter] ( COMMA g2= group_element[$name,$group_list::counter] )* )
            // /home/proto/workspace/bionetgen/BNGParser/src/bngparser/netGrammar/NETGrammar.g:128:3: g1= group_element[$name,$group_list::counter] ( COMMA g2= group_element[$name,$group_list::counter] )*
            {
            pushFollow(FOLLOW_group_element_in_group_list253);
            g1=group_element(name, ((group_list_scope)group_list_stack.peek()).counter);

            state._fsp--;

            groups.add((g1!=null?g1.st:null));((group_list_scope)group_list_stack.peek()).counter++;
            // /home/proto/workspace/bionetgen/BNGParser/src/bngparser/netGrammar/NETGrammar.g:129:3: ( COMMA g2= group_element[$name,$group_list::counter] )*
            loop2:
            do {
                int alt2=2;
                int LA2_0 = input.LA(1);

                if ( (LA2_0==COMMA) ) {
                    alt2=1;
                }


                switch (alt2) {
            	case 1 :
            	    // /home/proto/workspace/bionetgen/BNGParser/src/bngparser/netGrammar/NETGrammar.g:129:4: COMMA g2= group_element[$name,$group_list::counter]
            	    {
            	    match(input,COMMA,FOLLOW_COMMA_in_group_list261); 
            	    pushFollow(FOLLOW_group_element_in_group_list265);
            	    g2=group_element(name, ((group_list_scope)group_list_stack.peek()).counter);

            	    state._fsp--;

            	    groups.add((g2!=null?g2.st:null));((group_list_scope)group_list_stack.peek()).counter++;

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
            group_list_stack.pop();
        }
        return retval;
    }
    // $ANTLR end "group_list"

    public static class group_element_return extends ParserRuleReturnScope {
        public StringTemplate st;
        public Object getTemplate() { return st; }
        public String toString() { return st==null?null:st.toString(); }
    };

    // $ANTLR start "group_element"
    // /home/proto/workspace/bionetgen/BNGParser/src/bngparser/netGrammar/NETGrammar.g:132:1: group_element[String name,int counter] : (i1= INT -> groups_block(id=$i1.textname=$name + \"_\" + counter) | (i2= INT TIMES i3= INT ) -> groups_block(id=$i3.textname=$name + \"_\" + counter));
    public final NETGrammar.group_element_return group_element(String name, int counter) throws RecognitionException {
        NETGrammar.group_element_return retval = new NETGrammar.group_element_return();
        retval.start = input.LT(1);

        Token i1=null;
        Token i2=null;
        Token i3=null;

        try {
            // /home/proto/workspace/bionetgen/BNGParser/src/bngparser/netGrammar/NETGrammar.g:132:39: (i1= INT -> groups_block(id=$i1.textname=$name + \"_\" + counter) | (i2= INT TIMES i3= INT ) -> groups_block(id=$i3.textname=$name + \"_\" + counter))
            int alt3=2;
            int LA3_0 = input.LA(1);

            if ( (LA3_0==INT) ) {
                int LA3_1 = input.LA(2);

                if ( (LA3_1==TIMES) ) {
                    alt3=2;
                }
                else if ( (LA3_1==END||LA3_1==INT||LA3_1==COMMA) ) {
                    alt3=1;
                }
                else {
                    NoViableAltException nvae =
                        new NoViableAltException("", 3, 1, input);

                    throw nvae;
                }
            }
            else {
                NoViableAltException nvae =
                    new NoViableAltException("", 3, 0, input);

                throw nvae;
            }
            switch (alt3) {
                case 1 :
                    // /home/proto/workspace/bionetgen/BNGParser/src/bngparser/netGrammar/NETGrammar.g:133:3: i1= INT
                    {
                    i1=(Token)match(input,INT,FOLLOW_INT_in_group_element287); 


                    // TEMPLATE REWRITE
                    // 133:10: -> groups_block(id=$i1.textname=$name + \"_\" + counter)
                    {
                        retval.st = templateLib.getInstanceOf("groups_block",
                      new STAttrMap().put("id", (i1!=null?i1.getText():null)).put("name", name + "_" + counter));
                    }


                    }
                    break;
                case 2 :
                    // /home/proto/workspace/bionetgen/BNGParser/src/bngparser/netGrammar/NETGrammar.g:134:5: (i2= INT TIMES i3= INT )
                    {
                    // /home/proto/workspace/bionetgen/BNGParser/src/bngparser/netGrammar/NETGrammar.g:134:5: (i2= INT TIMES i3= INT )
                    // /home/proto/workspace/bionetgen/BNGParser/src/bngparser/netGrammar/NETGrammar.g:134:6: i2= INT TIMES i3= INT
                    {
                    i2=(Token)match(input,INT,FOLLOW_INT_in_group_element312); 
                    match(input,TIMES,FOLLOW_TIMES_in_group_element314); 
                    i3=(Token)match(input,INT,FOLLOW_INT_in_group_element318); 

                    }



                    // TEMPLATE REWRITE
                    // 134:27: -> groups_block(id=$i3.textname=$name + \"_\" + counter)
                    {
                        retval.st = templateLib.getInstanceOf("groups_block",
                      new STAttrMap().put("id", (i3!=null?i3.getText():null)).put("name", name + "_" + counter));
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
    // $ANTLR end "group_element"

    protected static class prog_scope {
        List parameters;
        List seedSpecies;
        List observables;
        List reactionRules;
        List functions;
        List compartments;
        Stack elements;
    }
    protected Stack prog_stack = new Stack();

    public static class prog_return extends ParserRuleReturnScope {
        public StringTemplate st;
        public Object getTemplate() { return st; }
        public String toString() { return st==null?null:st.toString(); }
    };

    // $ANTLR start "prog"
    // /home/proto/workspace/bionetgen/BNGParser/src/bngparser/netGrammar/NETGrammar.g:138:1: prog : SUBSTANCEUNITS LPAREN DBQUOTES STRING DBQUOTES RPAREN SEMI ( program_block )* EOF -> prog2(parameters=$prog::parametersmolecules=speciesMapspecies=$prog::seedSpeciesreactions=reactionsobservables=groupsfunctions=$prog::functionscompartments=$prog::compartments);
    public final NETGrammar.prog_return prog() throws RecognitionException {
        prog_stack.push(new prog_scope());
        NETGrammar.prog_return retval = new NETGrammar.prog_return();
        retval.start = input.LT(1);


          ((prog_scope)prog_stack.peek()).parameters = new ArrayList();
         
          ((prog_scope)prog_stack.peek()).seedSpecies = new ArrayList();
          ((prog_scope)prog_stack.peek()).reactionRules = new ArrayList();
          ((prog_scope)prog_stack.peek()).observables = new ArrayList();
          ((prog_scope)prog_stack.peek()).functions = new ArrayList();
          ((prog_scope)prog_stack.peek()).compartments = new ArrayList();
         // memory = new HashMap<String,Register>();
          ((prog_scope)prog_stack.peek()).elements = new Stack();
          
          paraphrases.push("in model");

        try {
            // /home/proto/workspace/bionetgen/BNGParser/src/bngparser/netGrammar/NETGrammar.g:166:1: ( SUBSTANCEUNITS LPAREN DBQUOTES STRING DBQUOTES RPAREN SEMI ( program_block )* EOF -> prog2(parameters=$prog::parametersmolecules=speciesMapspecies=$prog::seedSpeciesreactions=reactionsobservables=groupsfunctions=$prog::functionscompartments=$prog::compartments))
            // /home/proto/workspace/bionetgen/BNGParser/src/bngparser/netGrammar/NETGrammar.g:167:1: SUBSTANCEUNITS LPAREN DBQUOTES STRING DBQUOTES RPAREN SEMI ( program_block )* EOF
            {
            match(input,SUBSTANCEUNITS,FOLLOW_SUBSTANCEUNITS_in_prog359); 
            match(input,LPAREN,FOLLOW_LPAREN_in_prog361); 
            match(input,DBQUOTES,FOLLOW_DBQUOTES_in_prog363); 
            match(input,STRING,FOLLOW_STRING_in_prog365); 
            match(input,DBQUOTES,FOLLOW_DBQUOTES_in_prog367); 
            match(input,RPAREN,FOLLOW_RPAREN_in_prog369); 
            match(input,SEMI,FOLLOW_SEMI_in_prog371); 
            // /home/proto/workspace/bionetgen/BNGParser/src/bngparser/netGrammar/NETGrammar.g:168:1: ( program_block )*
            loop4:
            do {
                int alt4=2;
                int LA4_0 = input.LA(1);

                if ( (LA4_0==BEGIN) ) {
                    alt4=1;
                }


                switch (alt4) {
            	case 1 :
            	    // /home/proto/workspace/bionetgen/BNGParser/src/bngparser/netGrammar/NETGrammar.g:168:2: program_block
            	    {
            	    pushFollow(FOLLOW_program_block_in_prog374);
            	    program_block();

            	    state._fsp--;


            	    }
            	    break;

            	default :
            	    break loop4;
                }
            } while (true);

            match(input,EOF,FOLLOW_EOF_in_prog378); 


            // TEMPLATE REWRITE
            // 171:2: -> prog2(parameters=$prog::parametersmolecules=speciesMapspecies=$prog::seedSpeciesreactions=reactionsobservables=groupsfunctions=$prog::functionscompartments=$prog::compartments)
            {
                retval.st = templateLib.getInstanceOf("prog2",
              new STAttrMap().put("parameters", ((prog_scope)prog_stack.peek()).parameters).put("molecules", speciesMap).put("species", ((prog_scope)prog_stack.peek()).seedSpecies).put("reactions", reactions).put("observables", groups).put("functions", ((prog_scope)prog_stack.peek()).functions).put("compartments", ((prog_scope)prog_stack.peek()).compartments));
            }


            }

            retval.stop = input.LT(-1);


              paraphrases.pop();

        }
        catch (RecognitionException re) {
            reportError(re);
            recover(input,re);
        }
        finally {
            prog_stack.pop();
        }
        return retval;
    }
    // $ANTLR end "prog"

    public static class version_def_return extends ParserRuleReturnScope {
        public StringTemplate st;
        public Object getTemplate() { return st; }
        public String toString() { return st==null?null:st.toString(); }
    };

    // $ANTLR start "version_def"
    // /home/proto/workspace/bionetgen/BNGParser/src/bngparser/netGrammar/NETGrammar.g:174:1: version_def : VERSION LPAREN DBQUOTES VERSION_NUMBER DBQUOTES RPAREN SEMI ;
    public final NETGrammar.version_def_return version_def() throws RecognitionException {
        NETGrammar.version_def_return retval = new NETGrammar.version_def_return();
        retval.start = input.LT(1);

        try {
            // /home/proto/workspace/bionetgen/BNGParser/src/bngparser/netGrammar/NETGrammar.g:174:12: ( VERSION LPAREN DBQUOTES VERSION_NUMBER DBQUOTES RPAREN SEMI )
            // /home/proto/workspace/bionetgen/BNGParser/src/bngparser/netGrammar/NETGrammar.g:174:14: VERSION LPAREN DBQUOTES VERSION_NUMBER DBQUOTES RPAREN SEMI
            {
            match(input,VERSION,FOLLOW_VERSION_in_version_def452); 
            match(input,LPAREN,FOLLOW_LPAREN_in_version_def454); 
            match(input,DBQUOTES,FOLLOW_DBQUOTES_in_version_def456); 
            match(input,VERSION_NUMBER,FOLLOW_VERSION_NUMBER_in_version_def458); 
            match(input,DBQUOTES,FOLLOW_DBQUOTES_in_version_def460); 
            match(input,RPAREN,FOLLOW_RPAREN_in_version_def462); 
            match(input,SEMI,FOLLOW_SEMI_in_version_def464); 

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
    // $ANTLR end "version_def"

    public static class program_block_return extends ParserRuleReturnScope {
        public StringTemplate st;
        public Object getTemplate() { return st; }
        public String toString() { return st==null?null:st.toString(); }
    };

    // $ANTLR start "program_block"
    // /home/proto/workspace/bionetgen/BNGParser/src/bngparser/netGrammar/NETGrammar.g:178:1: program_block : ( parameters_block[memory,$prog::parameters] | molecule_types_block | seed_species_block[$prog::seedSpecies] | observables_block[$prog::observables] | reaction_rules_block[$prog::reactionRules] | functions_block | compartments_block | reactions_block[reactions] | groups_block );
    public final NETGrammar.program_block_return program_block() throws RecognitionException {
        NETGrammar.program_block_return retval = new NETGrammar.program_block_return();
        retval.start = input.LT(1);

        try {
            // /home/proto/workspace/bionetgen/BNGParser/src/bngparser/netGrammar/NETGrammar.g:179:1: ( parameters_block[memory,$prog::parameters] | molecule_types_block | seed_species_block[$prog::seedSpecies] | observables_block[$prog::observables] | reaction_rules_block[$prog::reactionRules] | functions_block | compartments_block | reactions_block[reactions] | groups_block )
            int alt5=9;
            alt5 = dfa5.predict(input);
            switch (alt5) {
                case 1 :
                    // /home/proto/workspace/bionetgen/BNGParser/src/bngparser/netGrammar/NETGrammar.g:179:3: parameters_block[memory,$prog::parameters]
                    {
                    pushFollow(FOLLOW_parameters_block_in_program_block474);
                    parameters_block(memory, ((prog_scope)prog_stack.peek()).parameters);

                    state._fsp--;


                    }
                    break;
                case 2 :
                    // /home/proto/workspace/bionetgen/BNGParser/src/bngparser/netGrammar/NETGrammar.g:180:3: molecule_types_block
                    {
                    pushFollow(FOLLOW_molecule_types_block_in_program_block479);
                    molecule_types_block();

                    state._fsp--;


                    }
                    break;
                case 3 :
                    // /home/proto/workspace/bionetgen/BNGParser/src/bngparser/netGrammar/NETGrammar.g:181:3: seed_species_block[$prog::seedSpecies]
                    {
                    pushFollow(FOLLOW_seed_species_block_in_program_block483);
                    seed_species_block(((prog_scope)prog_stack.peek()).seedSpecies);

                    state._fsp--;


                    }
                    break;
                case 4 :
                    // /home/proto/workspace/bionetgen/BNGParser/src/bngparser/netGrammar/NETGrammar.g:182:3: observables_block[$prog::observables]
                    {
                    pushFollow(FOLLOW_observables_block_in_program_block488);
                    observables_block(((prog_scope)prog_stack.peek()).observables);

                    state._fsp--;


                    }
                    break;
                case 5 :
                    // /home/proto/workspace/bionetgen/BNGParser/src/bngparser/netGrammar/NETGrammar.g:183:3: reaction_rules_block[$prog::reactionRules]
                    {
                    pushFollow(FOLLOW_reaction_rules_block_in_program_block493);
                    reaction_rules_block(((prog_scope)prog_stack.peek()).reactionRules);

                    state._fsp--;


                    }
                    break;
                case 6 :
                    // /home/proto/workspace/bionetgen/BNGParser/src/bngparser/netGrammar/NETGrammar.g:184:3: functions_block
                    {
                    pushFollow(FOLLOW_functions_block_in_program_block498);
                    functions_block();

                    state._fsp--;


                    }
                    break;
                case 7 :
                    // /home/proto/workspace/bionetgen/BNGParser/src/bngparser/netGrammar/NETGrammar.g:185:3: compartments_block
                    {
                    pushFollow(FOLLOW_compartments_block_in_program_block502);
                    compartments_block();

                    state._fsp--;


                    }
                    break;
                case 8 :
                    // /home/proto/workspace/bionetgen/BNGParser/src/bngparser/netGrammar/NETGrammar.g:186:3: reactions_block[reactions]
                    {
                    pushFollow(FOLLOW_reactions_block_in_program_block506);
                    reactions_block(reactions);

                    state._fsp--;


                    }
                    break;
                case 9 :
                    // /home/proto/workspace/bionetgen/BNGParser/src/bngparser/netGrammar/NETGrammar.g:187:3: groups_block
                    {
                    pushFollow(FOLLOW_groups_block_in_program_block511);
                    groups_block();

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
    // $ANTLR end "program_block"

    public static class functions_block_return extends ParserRuleReturnScope {
        public StringTemplate st;
        public Object getTemplate() { return st; }
        public String toString() { return st==null?null:st.toString(); }
    };

    // $ANTLR start "functions_block"
    // /home/proto/workspace/bionetgen/BNGParser/src/bngparser/netGrammar/NETGrammar.g:192:1: functions_block : BEGIN FUNCTIONS ( function_def )* END FUNCTIONS ;
    public final NETGrammar.functions_block_return functions_block() throws RecognitionException {
        NETGrammar.functions_block_return retval = new NETGrammar.functions_block_return();
        retval.start = input.LT(1);

        NETGrammar.function_def_return function_def6 = null;



          paraphrases.push("in function block");

        try {
            // /home/proto/workspace/bionetgen/BNGParser/src/bngparser/netGrammar/NETGrammar.g:199:1: ( BEGIN FUNCTIONS ( function_def )* END FUNCTIONS )
            // /home/proto/workspace/bionetgen/BNGParser/src/bngparser/netGrammar/NETGrammar.g:200:7: BEGIN FUNCTIONS ( function_def )* END FUNCTIONS
            {
            match(input,BEGIN,FOLLOW_BEGIN_in_functions_block536); 
            match(input,FUNCTIONS,FOLLOW_FUNCTIONS_in_functions_block538); 
            // /home/proto/workspace/bionetgen/BNGParser/src/bngparser/netGrammar/NETGrammar.g:201:8: ( function_def )*
            loop6:
            do {
                int alt6=2;
                int LA6_0 = input.LA(1);

                if ( (LA6_0==STRING) ) {
                    alt6=1;
                }


                switch (alt6) {
            	case 1 :
            	    // /home/proto/workspace/bionetgen/BNGParser/src/bngparser/netGrammar/NETGrammar.g:201:9: function_def
            	    {
            	    pushFollow(FOLLOW_function_def_in_functions_block549);
            	    function_def6=function_def();

            	    state._fsp--;

            	    ((prog_scope)prog_stack.peek()).functions.add((function_def6!=null?function_def6.st:null));

            	    }
            	    break;

            	default :
            	    break loop6;
                }
            } while (true);

            match(input,END,FOLLOW_END_in_functions_block566); 
            match(input,FUNCTIONS,FOLLOW_FUNCTIONS_in_functions_block568); 

            }

            retval.stop = input.LT(-1);


              paraphrases.pop();

        }
        catch (RecognitionException re) {
            reportError(re);
            recover(input,re);
        }
        finally {
        }
        return retval;
    }
    // $ANTLR end "functions_block"

    public static class function_def_return extends ParserRuleReturnScope {
        public StringTemplate st;
        public Object getTemplate() { return st; }
        public String toString() { return st==null?null:st.toString(); }
    };

    // $ANTLR start "function_def"
    // /home/proto/workspace/bionetgen/BNGParser/src/bngparser/netGrammar/NETGrammar.g:205:1: function_def : s1= STRING LPAREN ( STRING )? RPAREN ( BECOMES )? expression[memory] -> functions_block(id=$s1.textreferencesName=$expression.reference.keySet()referencesType=Register.getTypes($expression.reference)expression=$expression.text);
    public final NETGrammar.function_def_return function_def() throws RecognitionException {
        NETGrammar.function_def_return retval = new NETGrammar.function_def_return();
        retval.start = input.LT(1);

        Token s1=null;
        NETGrammar_BNGGrammar_Expression.expression_return expression7 = null;


        try {
            // /home/proto/workspace/bionetgen/BNGParser/src/bngparser/netGrammar/NETGrammar.g:205:13: (s1= STRING LPAREN ( STRING )? RPAREN ( BECOMES )? expression[memory] -> functions_block(id=$s1.textreferencesName=$expression.reference.keySet()referencesType=Register.getTypes($expression.reference)expression=$expression.text))
            // /home/proto/workspace/bionetgen/BNGParser/src/bngparser/netGrammar/NETGrammar.g:206:5: s1= STRING LPAREN ( STRING )? RPAREN ( BECOMES )? expression[memory]
            {
            s1=(Token)match(input,STRING,FOLLOW_STRING_in_function_def582); 
            match(input,LPAREN,FOLLOW_LPAREN_in_function_def584); 
            // /home/proto/workspace/bionetgen/BNGParser/src/bngparser/netGrammar/NETGrammar.g:206:22: ( STRING )?
            int alt7=2;
            int LA7_0 = input.LA(1);

            if ( (LA7_0==STRING) ) {
                alt7=1;
            }
            switch (alt7) {
                case 1 :
                    // /home/proto/workspace/bionetgen/BNGParser/src/bngparser/netGrammar/NETGrammar.g:206:23: STRING
                    {
                    match(input,STRING,FOLLOW_STRING_in_function_def587); 

                    }
                    break;

            }

            match(input,RPAREN,FOLLOW_RPAREN_in_function_def591); 
            // /home/proto/workspace/bionetgen/BNGParser/src/bngparser/netGrammar/NETGrammar.g:206:39: ( BECOMES )?
            int alt8=2;
            int LA8_0 = input.LA(1);

            if ( (LA8_0==BECOMES) ) {
                alt8=1;
            }
            switch (alt8) {
                case 1 :
                    // /home/proto/workspace/bionetgen/BNGParser/src/bngparser/netGrammar/NETGrammar.g:206:40: BECOMES
                    {
                    match(input,BECOMES,FOLLOW_BECOMES_in_function_def594); 

                    }
                    break;

            }

            pushFollow(FOLLOW_expression_in_function_def599);
            expression7=expression(memory);

            state._fsp--;



            // TEMPLATE REWRITE
            // 206:70: -> functions_block(id=$s1.textreferencesName=$expression.reference.keySet()referencesType=Register.getTypes($expression.reference)expression=$expression.text)
            {
                retval.st = templateLib.getInstanceOf("functions_block",
              new STAttrMap().put("id", (s1!=null?s1.getText():null)).put("referencesName", (expression7!=null?expression7.reference:null).keySet()).put("referencesType", Register.getTypes((expression7!=null?expression7.reference:null))).put("expression", (expression7!=null?input.toString(expression7.start,expression7.stop):null)));
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
    // $ANTLR end "function_def"

    public static class compartments_block_return extends ParserRuleReturnScope {
        public StringTemplate st;
        public Object getTemplate() { return st; }
        public String toString() { return st==null?null:st.toString(); }
    };

    // $ANTLR start "compartments_block"
    // /home/proto/workspace/bionetgen/BNGParser/src/bngparser/netGrammar/NETGrammar.g:210:1: compartments_block : BEGIN COMPARTMENTS ( compartment )* END COMPARTMENTS ;
    public final NETGrammar.compartments_block_return compartments_block() throws RecognitionException {
        NETGrammar.compartments_block_return retval = new NETGrammar.compartments_block_return();
        retval.start = input.LT(1);

        NETGrammar.compartment_return compartment8 = null;


        try {
            // /home/proto/workspace/bionetgen/BNGParser/src/bngparser/netGrammar/NETGrammar.g:210:19: ( BEGIN COMPARTMENTS ( compartment )* END COMPARTMENTS )
            // /home/proto/workspace/bionetgen/BNGParser/src/bngparser/netGrammar/NETGrammar.g:212:3: BEGIN COMPARTMENTS ( compartment )* END COMPARTMENTS
            {
            match(input,BEGIN,FOLLOW_BEGIN_in_compartments_block662); 
            match(input,COMPARTMENTS,FOLLOW_COMPARTMENTS_in_compartments_block664); 
            // /home/proto/workspace/bionetgen/BNGParser/src/bngparser/netGrammar/NETGrammar.g:213:7: ( compartment )*
            loop9:
            do {
                int alt9=2;
                int LA9_0 = input.LA(1);

                if ( (LA9_0==STRING) ) {
                    alt9=1;
                }


                switch (alt9) {
            	case 1 :
            	    // /home/proto/workspace/bionetgen/BNGParser/src/bngparser/netGrammar/NETGrammar.g:213:8: compartment
            	    {
            	    pushFollow(FOLLOW_compartment_in_compartments_block673);
            	    compartment8=compartment();

            	    state._fsp--;

            	    ((prog_scope)prog_stack.peek()).compartments.add((compartment8!=null?compartment8.st:null));

            	    }
            	    break;

            	default :
            	    break loop9;
                }
            } while (true);

            match(input,END,FOLLOW_END_in_compartments_block681); 
            match(input,COMPARTMENTS,FOLLOW_COMPARTMENTS_in_compartments_block683); 

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
    // $ANTLR end "compartments_block"

    public static class compartment_return extends ParserRuleReturnScope {
        public StringTemplate st;
        public Object getTemplate() { return st; }
        public String toString() { return st==null?null:st.toString(); }
    };

    // $ANTLR start "compartment"
    // /home/proto/workspace/bionetgen/BNGParser/src/bngparser/netGrammar/NETGrammar.g:216:1: compartment : s1= STRING INT s3= expression[memory] (s2= STRING )? -> compartments_block(id=$s1.textdimensions=$INT.textsize=$s3.valueoutside=$s2.text);
    public final NETGrammar.compartment_return compartment() throws RecognitionException {
        NETGrammar.compartment_return retval = new NETGrammar.compartment_return();
        retval.start = input.LT(1);

        Token s1=null;
        Token s2=null;
        Token INT9=null;
        NETGrammar_BNGGrammar_Expression.expression_return s3 = null;


        try {
            // /home/proto/workspace/bionetgen/BNGParser/src/bngparser/netGrammar/NETGrammar.g:216:12: (s1= STRING INT s3= expression[memory] (s2= STRING )? -> compartments_block(id=$s1.textdimensions=$INT.textsize=$s3.valueoutside=$s2.text))
            // /home/proto/workspace/bionetgen/BNGParser/src/bngparser/netGrammar/NETGrammar.g:217:4: s1= STRING INT s3= expression[memory] (s2= STRING )?
            {
            s1=(Token)match(input,STRING,FOLLOW_STRING_in_compartment697); 
            INT9=(Token)match(input,INT,FOLLOW_INT_in_compartment699); 
            pushFollow(FOLLOW_expression_in_compartment703);
            s3=expression(memory);

            state._fsp--;

            // /home/proto/workspace/bionetgen/BNGParser/src/bngparser/netGrammar/NETGrammar.g:217:40: (s2= STRING )?
            int alt10=2;
            int LA10_0 = input.LA(1);

            if ( (LA10_0==STRING) ) {
                int LA10_1 = input.LA(2);

                if ( (LA10_1==END||LA10_1==STRING) ) {
                    alt10=1;
                }
            }
            switch (alt10) {
                case 1 :
                    // /home/proto/workspace/bionetgen/BNGParser/src/bngparser/netGrammar/NETGrammar.g:217:41: s2= STRING
                    {
                    s2=(Token)match(input,STRING,FOLLOW_STRING_in_compartment709); 

                    }
                    break;

            }



            // TEMPLATE REWRITE
            // 217:53: -> compartments_block(id=$s1.textdimensions=$INT.textsize=$s3.valueoutside=$s2.text)
            {
                retval.st = templateLib.getInstanceOf("compartments_block",
              new STAttrMap().put("id", (s1!=null?s1.getText():null)).put("dimensions", (INT9!=null?INT9.getText():null)).put("size", (s3!=null?s3.value:null)).put("outside", (s2!=null?s2.getText():null)));
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
    // $ANTLR end "compartment"

    public static class label_return extends ParserRuleReturnScope {
        public String label;
        public StringTemplate st;
        public Object getTemplate() { return st; }
        public String toString() { return st==null?null:st.toString(); }
    };

    // $ANTLR start "label"
    // /home/proto/workspace/bionetgen/BNGParser/src/bngparser/netGrammar/NETGrammar.g:220:1: label returns [String label] : MOD STRING ;
    public final NETGrammar.label_return label() throws RecognitionException {
        NETGrammar.label_return retval = new NETGrammar.label_return();
        retval.start = input.LT(1);

        Token STRING10=null;

        try {
            // /home/proto/workspace/bionetgen/BNGParser/src/bngparser/netGrammar/NETGrammar.g:220:29: ( MOD STRING )
            // /home/proto/workspace/bionetgen/BNGParser/src/bngparser/netGrammar/NETGrammar.g:221:3: MOD STRING
            {
            match(input,MOD,FOLLOW_MOD_in_label749); 
            STRING10=(Token)match(input,STRING,FOLLOW_STRING_in_label751); 
            retval.label = (STRING10!=null?STRING10.getText():null);

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
    // $ANTLR end "label"

    public static class dereference_return extends ParserRuleReturnScope {
        public String label;
        public StringTemplate st;
        public Object getTemplate() { return st; }
        public String toString() { return st==null?null:st.toString(); }
    };

    // $ANTLR start "dereference"
    // /home/proto/workspace/bionetgen/BNGParser/src/bngparser/netGrammar/NETGrammar.g:223:1: dereference returns [String label] : AMPERSAND STRING ;
    public final NETGrammar.dereference_return dereference() throws RecognitionException {
        NETGrammar.dereference_return retval = new NETGrammar.dereference_return();
        retval.start = input.LT(1);

        Token STRING11=null;

        try {
            // /home/proto/workspace/bionetgen/BNGParser/src/bngparser/netGrammar/NETGrammar.g:223:35: ( AMPERSAND STRING )
            // /home/proto/workspace/bionetgen/BNGParser/src/bngparser/netGrammar/NETGrammar.g:224:3: AMPERSAND STRING
            {
            match(input,AMPERSAND,FOLLOW_AMPERSAND_in_dereference767); 
            STRING11=(Token)match(input,STRING,FOLLOW_STRING_in_dereference769); 
            retval.label = (STRING11!=null?STRING11.getText():null);

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
    // $ANTLR end "dereference"

    public static class reactions_block_return extends ParserRuleReturnScope {
        public StringTemplate st;
        public Object getTemplate() { return st; }
        public String toString() { return st==null?null:st.toString(); }
    };

    // $ANTLR start "reactions_block"
    // /home/proto/workspace/bionetgen/BNGParser/src/bngparser/netGrammar/NETGrammar.g:228:2: reactions_block[List<StringTemplate> templates] : BEGIN REACTIONS ( INT reaction )* END REACTIONS ;
    public final NETGrammar.reactions_block_return reactions_block(List<StringTemplate> templates) throws RecognitionException {
        NETGrammar.reactions_block_return retval = new NETGrammar.reactions_block_return();
        retval.start = input.LT(1);

        NETGrammar.reaction_return reaction12 = null;


        try {
            // /home/proto/workspace/bionetgen/BNGParser/src/bngparser/netGrammar/NETGrammar.g:228:49: ( BEGIN REACTIONS ( INT reaction )* END REACTIONS )
            // /home/proto/workspace/bionetgen/BNGParser/src/bngparser/netGrammar/NETGrammar.g:229:1: BEGIN REACTIONS ( INT reaction )* END REACTIONS
            {
            match(input,BEGIN,FOLLOW_BEGIN_in_reactions_block787); 
            match(input,REACTIONS,FOLLOW_REACTIONS_in_reactions_block789); 
            // /home/proto/workspace/bionetgen/BNGParser/src/bngparser/netGrammar/NETGrammar.g:230:3: ( INT reaction )*
            loop11:
            do {
                int alt11=2;
                int LA11_0 = input.LA(1);

                if ( (LA11_0==INT) ) {
                    alt11=1;
                }


                switch (alt11) {
            	case 1 :
            	    // /home/proto/workspace/bionetgen/BNGParser/src/bngparser/netGrammar/NETGrammar.g:230:4: INT reaction
            	    {
            	    match(input,INT,FOLLOW_INT_in_reactions_block794); 
            	    pushFollow(FOLLOW_reaction_in_reactions_block796);
            	    reaction12=reaction();

            	    state._fsp--;


            	        templates.add((reaction12!=null?reaction12.st:null));
            	      

            	    }
            	    break;

            	default :
            	    break loop11;
                }
            } while (true);

            match(input,END,FOLLOW_END_in_reactions_block804); 
            match(input,REACTIONS,FOLLOW_REACTIONS_in_reactions_block806); 

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
    // $ANTLR end "reactions_block"

    protected static class reaction_scope {
        List<String> reactant;
        List<String> product;
    }
    protected Stack reaction_stack = new Stack();

    public static class reaction_return extends ParserRuleReturnScope {
        public StringTemplate st;
        public Object getTemplate() { return st; }
        public String toString() { return st==null?null:st.toString(); }
    };

    // $ANTLR start "reaction"
    // /home/proto/workspace/bionetgen/BNGParser/src/bngparser/netGrammar/NETGrammar.g:237:1: reaction : reactant[$reaction::reactant] reactant[$reaction::product] expression[memory] -> reactions(reactant=$reaction::reactantproduct=$reaction::productrate=$expression.text);
    public final NETGrammar.reaction_return reaction() throws RecognitionException {
        reaction_stack.push(new reaction_scope());
        NETGrammar.reaction_return retval = new NETGrammar.reaction_return();
        retval.start = input.LT(1);

        NETGrammar_BNGGrammar_Expression.expression_return expression13 = null;



          ((reaction_scope)reaction_stack.peek()).reactant = new ArrayList<String>();
          ((reaction_scope)reaction_stack.peek()).product = new ArrayList<String>();

        try {
            // /home/proto/workspace/bionetgen/BNGParser/src/bngparser/netGrammar/NETGrammar.g:246:1: ( reactant[$reaction::reactant] reactant[$reaction::product] expression[memory] -> reactions(reactant=$reaction::reactantproduct=$reaction::productrate=$expression.text))
            // /home/proto/workspace/bionetgen/BNGParser/src/bngparser/netGrammar/NETGrammar.g:247:2: reactant[$reaction::reactant] reactant[$reaction::product] expression[memory]
            {
            pushFollow(FOLLOW_reactant_in_reaction823);
            reactant(((reaction_scope)reaction_stack.peek()).reactant);

            state._fsp--;

            pushFollow(FOLLOW_reactant_in_reaction826);
            reactant(((reaction_scope)reaction_stack.peek()).product);

            state._fsp--;

            pushFollow(FOLLOW_expression_in_reaction829);
            expression13=expression(memory);

            state._fsp--;



            // TEMPLATE REWRITE
            // 248:2: -> reactions(reactant=$reaction::reactantproduct=$reaction::productrate=$expression.text)
            {
                retval.st = templateLib.getInstanceOf("reactions",
              new STAttrMap().put("reactant", ((reaction_scope)reaction_stack.peek()).reactant).put("product", ((reaction_scope)reaction_stack.peek()).product).put("rate", (expression13!=null?input.toString(expression13.start,expression13.stop):null)));
            }


            }

            retval.stop = input.LT(-1);

        }
        catch (RecognitionException re) {
            reportError(re);
            recover(input,re);
        }
        finally {
            reaction_stack.pop();
        }
        return retval;
    }
    // $ANTLR end "reaction"

    public static class reactant_return extends ParserRuleReturnScope {
        public StringTemplate st;
        public Object getTemplate() { return st; }
        public String toString() { return st==null?null:st.toString(); }
    };

    // $ANTLR start "reactant"
    // /home/proto/workspace/bionetgen/BNGParser/src/bngparser/netGrammar/NETGrammar.g:251:1: reactant[List<String> elements] : i1= INT ( COMMA i2= INT )* ;
    public final NETGrammar.reactant_return reactant(List<String> elements) throws RecognitionException {
        NETGrammar.reactant_return retval = new NETGrammar.reactant_return();
        retval.start = input.LT(1);

        Token i1=null;
        Token i2=null;

        try {
            // /home/proto/workspace/bionetgen/BNGParser/src/bngparser/netGrammar/NETGrammar.g:251:32: (i1= INT ( COMMA i2= INT )* )
            // /home/proto/workspace/bionetgen/BNGParser/src/bngparser/netGrammar/NETGrammar.g:253:3: i1= INT ( COMMA i2= INT )*
            {
            i1=(Token)match(input,INT,FOLLOW_INT_in_reactant863); 

                elements.add((i1!=null?i1.getText():null));
              
            // /home/proto/workspace/bionetgen/BNGParser/src/bngparser/netGrammar/NETGrammar.g:258:3: ( COMMA i2= INT )*
            loop12:
            do {
                int alt12=2;
                int LA12_0 = input.LA(1);

                if ( (LA12_0==COMMA) ) {
                    alt12=1;
                }


                switch (alt12) {
            	case 1 :
            	    // /home/proto/workspace/bionetgen/BNGParser/src/bngparser/netGrammar/NETGrammar.g:258:4: COMMA i2= INT
            	    {
            	    match(input,COMMA,FOLLOW_COMMA_in_reactant876); 
            	    i2=(Token)match(input,INT,FOLLOW_INT_in_reactant880); 

            	        elements.add((i2!=null?i2.getText():null));
            	      

            	    }
            	    break;

            	default :
            	    break loop12;
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
    // $ANTLR end "reactant"

    // Delegated rules
    public NETGrammar_BNGGrammar_SeedSpecies.species_def_return species_def(List molecules, BondList bonds, String speciesCounter) throws RecognitionException { return gBNGGrammar_SeedSpecies.species_def(molecules, bonds, speciesCounter); }
    public NETGrammar_BNGGrammar_Expression.multiplying_expression_return multiplying_expression() throws RecognitionException { return gBNGGrammar_Expression.multiplying_expression(); }
    public NETGrammar_BNGGrammar_ReactionRules.modif_command_return modif_command() throws RecognitionException { return gBNGGrammar_ReactionRules.modif_command(); }
    public NETGrammar_BNGGrammar_Expression.relational_expression_return relational_expression() throws RecognitionException { return gBNGGrammar_Expression.relational_expression(); }
    public NETGrammar_BNGGrammar_ReactionRules.pattern_return pattern() throws RecognitionException { return gBNGGrammar_ReactionRules.pattern(); }
    public NETGrammar_BNGGrammar_SeedSpecies.site_obs_state_return site_obs_state() throws RecognitionException { return gBNGGrammar_SeedSpecies.site_obs_state(); }
    public NETGrammar_BNGGrammar_MoleculeDef.site_def_return site_def() throws RecognitionException { return gBNGGrammar_MoleculeDef.site_def(); }
    public NETGrammar_BNGGrammar_SeedSpecies.species_element_return species_element(BondList bonds, String upperID, String compartment) throws RecognitionException { return gBNGGrammar_SeedSpecies.species_element(bonds, upperID, compartment); }
    public NETGrammar_BNGGrammar_ReactionRules.exclude_command_return exclude_command() throws RecognitionException { return gBNGGrammar_ReactionRules.exclude_command(); }
    public NETGrammar_BNGGrammar_Observables.observable_type_return observable_type() throws RecognitionException { return gBNGGrammar_Observables.observable_type(); }
    public NETGrammar_BNGGrammar_SeedSpecies.site_list_return site_list(List sites, BondList bonds, String upperID) throws RecognitionException { return gBNGGrammar_SeedSpecies.site_list(sites, bonds, upperID); }
    public NETGrammar_BNGGrammar_ReactionRules.reaction_def_return reaction_def(List patternsReactants, List patternsProducts, String upperID) throws RecognitionException { return gBNGGrammar_ReactionRules.reaction_def(patternsReactants, patternsProducts, upperID); }
    public NETGrammar_BNGGrammar_ReactionRules.rule_species_def_return rule_species_def(String upperID, ReactionAction reactionAction) throws RecognitionException { return gBNGGrammar_ReactionRules.rule_species_def(upperID, reactionAction); }
    public NETGrammar_BNGGrammar_Observables.pattern_list_return pattern_list(String upperID) throws RecognitionException { return gBNGGrammar_Observables.pattern_list(upperID); }
    public NETGrammar_BNGGrammar_SeedSpecies.site_obs_site_return site_obs_site(BondList bonds, String upperID) throws RecognitionException { return gBNGGrammar_SeedSpecies.site_obs_site(bonds, upperID); }
    public NETGrammar_BNGGrammar_SeedSpecies.site_obs_return site_obs(BondList bonds, String upperID) throws RecognitionException { return gBNGGrammar_SeedSpecies.site_obs(bonds, upperID); }
    public NETGrammar_BNGGrammar_Expression.primitive_element_return primitive_element() throws RecognitionException { return gBNGGrammar_Expression.primitive_element(); }
    public NETGrammar_BNGGrammar_MoleculeDef.molecule_def_return molecule_def() throws RecognitionException { return gBNGGrammar_MoleculeDef.molecule_def(); }
    public NETGrammar_BNGGrammar_Expression.sign_expression_return sign_expression() throws RecognitionException { return gBNGGrammar_Expression.sign_expression(); }
    public NETGrammar_BNGGrammar_Expression.expression2_return expression2() throws RecognitionException { return gBNGGrammar_Expression.expression2(); }
    public NETGrammar_BNGGrammar_MoleculeDef.state_label_return state_label() throws RecognitionException { return gBNGGrammar_MoleculeDef.state_label(); }
    public NETGrammar_BNGGrammar_ReactionRules.match_attribute_return match_attribute() throws RecognitionException { return gBNGGrammar_ReactionRules.match_attribute(); }
    public NETGrammar_BNGGrammar_Parameters.parameters_block_return parameters_block(Map<String,Register> memory, List parameters) throws RecognitionException { return gBNGGrammar_Parameters.parameters_block(memory, parameters); }
    public NETGrammar_BNGGrammar_Parameters.parameter_def_return parameter_def(Map<String,Register> lmemory) throws RecognitionException { return gBNGGrammar_Parameters.parameter_def(lmemory); }
    public NETGrammar_BNGGrammar_Observables.obs_species_def_return obs_species_def(String upperID, ReactionAction reactionAction) throws RecognitionException { return gBNGGrammar_Observables.obs_species_def(upperID, reactionAction); }
    public NETGrammar_BNGGrammar_Expression.adding_expression_return adding_expression() throws RecognitionException { return gBNGGrammar_Expression.adding_expression(); }
    public NETGrammar_BNGGrammar_Observables.observables_block_return observables_block(List observables) throws RecognitionException { return gBNGGrammar_Observables.observables_block(observables); }
    public NETGrammar_BNGGrammar_MoleculeDef.molecule_types_block_return molecule_types_block() throws RecognitionException { return gBNGGrammar_MoleculeDef.molecule_types_block(); }
    public NETGrammar_BNGGrammar_Expression.number_return number() throws RecognitionException { return gBNGGrammar_Expression.number(); }
    public NETGrammar_BNGGrammar_Expression.variable_return variable() throws RecognitionException { return gBNGGrammar_Expression.variable(); }
    public NETGrammar_BNGGrammar_ReactionRules.rate_list_return rate_list(List<Double> rateList) throws RecognitionException { return gBNGGrammar_ReactionRules.rate_list(rateList); }
    public NETGrammar_BNGGrammar_SeedSpecies.seed_species_block_return seed_species_block(List seedSpecies) throws RecognitionException { return gBNGGrammar_SeedSpecies.seed_species_block(seedSpecies); }
    public NETGrammar_BNGGrammar_Observables.observable_def_line_return observable_def_line(String upperID) throws RecognitionException { return gBNGGrammar_Observables.observable_def_line(upperID); }
    public NETGrammar_BNGGrammar_ReactionRules.reactionLabel_return reactionLabel() throws RecognitionException { return gBNGGrammar_ReactionRules.reactionLabel(); }
    public NETGrammar_BNGGrammar_ReactionRules.include_command_return include_command() throws RecognitionException { return gBNGGrammar_ReactionRules.include_command(); }
    public NETGrammar_BNGGrammar_ReactionRules.reaction_rules_block_return reaction_rules_block(List reactionRules) throws RecognitionException { return gBNGGrammar_ReactionRules.reaction_rules_block(reactionRules); }
    public NETGrammar_BNGGrammar_Expression.boolean_negation_expression_return boolean_negation_expression() throws RecognitionException { return gBNGGrammar_Expression.boolean_negation_expression(); }
    public NETGrammar_BNGGrammar_Expression.expression_return expression(Map<String,Register> memory) throws RecognitionException { return gBNGGrammar_Expression.expression(memory); }
    public NETGrammar_BNGGrammar_ReactionRules.reaction_rule_def_return reaction_rule_def(int reactionCounter) throws RecognitionException { return gBNGGrammar_ReactionRules.reaction_rule_def(reactionCounter); }
    public NETGrammar_BNGGrammar_SeedSpecies.bond_name_return bond_name() throws RecognitionException { return gBNGGrammar_SeedSpecies.bond_name(); }


    protected DFA5 dfa5 = new DFA5(this);
    static final String DFA5_eotS =
        "\13\uffff";
    static final String DFA5_eofS =
        "\13\uffff";
    static final String DFA5_minS =
        "\1\10\1\11\11\uffff";
    static final String DFA5_maxS =
        "\1\10\1\45\11\uffff";
    static final String DFA5_acceptS =
        "\2\uffff\1\1\1\2\1\4\1\5\1\6\1\7\1\10\1\11\1\3";
    static final String DFA5_specialS =
        "\13\uffff}>";
    static final String[] DFA5_transitionS = {
            "\1\1",
            "\2\12\1\11\1\3\3\uffff\1\5\1\10\1\uffff\1\6\1\7\1\uffff\1\4"+
            "\16\uffff\1\2",
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

    static final short[] DFA5_eot = DFA.unpackEncodedString(DFA5_eotS);
    static final short[] DFA5_eof = DFA.unpackEncodedString(DFA5_eofS);
    static final char[] DFA5_min = DFA.unpackEncodedStringToUnsignedChars(DFA5_minS);
    static final char[] DFA5_max = DFA.unpackEncodedStringToUnsignedChars(DFA5_maxS);
    static final short[] DFA5_accept = DFA.unpackEncodedString(DFA5_acceptS);
    static final short[] DFA5_special = DFA.unpackEncodedString(DFA5_specialS);
    static final short[][] DFA5_transition;

    static {
        int numStates = DFA5_transitionS.length;
        DFA5_transition = new short[numStates][];
        for (int i=0; i<numStates; i++) {
            DFA5_transition[i] = DFA.unpackEncodedString(DFA5_transitionS[i]);
        }
    }

    class DFA5 extends DFA {

        public DFA5(BaseRecognizer recognizer) {
            this.recognizer = recognizer;
            this.decisionNumber = 5;
            this.eot = DFA5_eot;
            this.eof = DFA5_eof;
            this.min = DFA5_min;
            this.max = DFA5_max;
            this.accept = DFA5_accept;
            this.special = DFA5_special;
            this.transition = DFA5_transition;
        }
        public String getDescription() {
            return "178:1: program_block : ( parameters_block[memory,$prog::parameters] | molecule_types_block | seed_species_block[$prog::seedSpecies] | observables_block[$prog::observables] | reaction_rules_block[$prog::reactionRules] | functions_block | compartments_block | reactions_block[reactions] | groups_block );";
        }
    }
 

    public static final BitSet FOLLOW_pre_species_def_in_seed_species_def82 = new BitSet(new long[]{0x0000000000000020L});
    public static final BitSet FOLLOW_WS_in_seed_species_def94 = new BitSet(new long[]{0x0000100000000000L,0x0000002000201600L});
    public static final BitSet FOLLOW_expression_in_seed_species_def102 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_INT_in_pre_species_def175 = new BitSet(new long[]{0x0000000000000000L,0x000000000A001000L});
    public static final BitSet FOLLOW_species_def_in_pre_species_def181 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_BEGIN_in_groups_block208 = new BitSet(new long[]{0x0000000000000800L});
    public static final BitSet FOLLOW_GROUPS_in_groups_block210 = new BitSet(new long[]{0x0000000000000080L,0x0000000000000400L});
    public static final BitSet FOLLOW_group_line_in_groups_block214 = new BitSet(new long[]{0x0000000000000080L,0x0000000000000400L});
    public static final BitSet FOLLOW_END_in_groups_block217 = new BitSet(new long[]{0x0000000000000800L});
    public static final BitSet FOLLOW_GROUPS_in_groups_block219 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_INT_in_group_line228 = new BitSet(new long[]{0x0000000000000000L,0x0000000000001000L});
    public static final BitSet FOLLOW_STRING_in_group_line230 = new BitSet(new long[]{0x0000000000000000L,0x0000000000000400L});
    public static final BitSet FOLLOW_group_list_in_group_line232 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_group_element_in_group_list253 = new BitSet(new long[]{0x0000000000000002L,0x0000000000080000L});
    public static final BitSet FOLLOW_COMMA_in_group_list261 = new BitSet(new long[]{0x0000000000000000L,0x0000000000000400L});
    public static final BitSet FOLLOW_group_element_in_group_list265 = new BitSet(new long[]{0x0000000000000002L,0x0000000000080000L});
    public static final BitSet FOLLOW_INT_in_group_element287 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_INT_in_group_element312 = new BitSet(new long[]{0x0000000000000000L,0x0000001000000000L});
    public static final BitSet FOLLOW_TIMES_in_group_element314 = new BitSet(new long[]{0x0000000000000000L,0x0000000000000400L});
    public static final BitSet FOLLOW_INT_in_group_element318 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_SUBSTANCEUNITS_in_prog359 = new BitSet(new long[]{0x0000000000000000L,0x0000000000200000L});
    public static final BitSet FOLLOW_LPAREN_in_prog361 = new BitSet(new long[]{0x0000000000000000L,0x0000040000000000L});
    public static final BitSet FOLLOW_DBQUOTES_in_prog363 = new BitSet(new long[]{0x0000000000000000L,0x0000000000001000L});
    public static final BitSet FOLLOW_STRING_in_prog365 = new BitSet(new long[]{0x0000000000000000L,0x0000040000000000L});
    public static final BitSet FOLLOW_DBQUOTES_in_prog367 = new BitSet(new long[]{0x0000000000000000L,0x0000000000400000L});
    public static final BitSet FOLLOW_RPAREN_in_prog369 = new BitSet(new long[]{0x0000000000000000L,0x0000000000002000L});
    public static final BitSet FOLLOW_SEMI_in_prog371 = new BitSet(new long[]{0x0000000000000100L});
    public static final BitSet FOLLOW_program_block_in_prog374 = new BitSet(new long[]{0x0000000000000100L});
    public static final BitSet FOLLOW_EOF_in_prog378 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_VERSION_in_version_def452 = new BitSet(new long[]{0x0000000000000000L,0x0000000000200000L});
    public static final BitSet FOLLOW_LPAREN_in_version_def454 = new BitSet(new long[]{0x0000000000000000L,0x0000040000000000L});
    public static final BitSet FOLLOW_DBQUOTES_in_version_def456 = new BitSet(new long[]{0x0000000000000000L,0x0000800000000000L});
    public static final BitSet FOLLOW_VERSION_NUMBER_in_version_def458 = new BitSet(new long[]{0x0000000000000000L,0x0000040000000000L});
    public static final BitSet FOLLOW_DBQUOTES_in_version_def460 = new BitSet(new long[]{0x0000000000000000L,0x0000000000400000L});
    public static final BitSet FOLLOW_RPAREN_in_version_def462 = new BitSet(new long[]{0x0000000000000000L,0x0000000000002000L});
    public static final BitSet FOLLOW_SEMI_in_version_def464 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_parameters_block_in_program_block474 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_molecule_types_block_in_program_block479 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_seed_species_block_in_program_block483 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_observables_block_in_program_block488 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_reaction_rules_block_in_program_block493 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_functions_block_in_program_block498 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_compartments_block_in_program_block502 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_reactions_block_in_program_block506 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_groups_block_in_program_block511 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_BEGIN_in_functions_block536 = new BitSet(new long[]{0x0000000000080000L});
    public static final BitSet FOLLOW_FUNCTIONS_in_functions_block538 = new BitSet(new long[]{0x0000000000000080L,0x0000000000001000L});
    public static final BitSet FOLLOW_function_def_in_functions_block549 = new BitSet(new long[]{0x0000000000000080L,0x0000000000001000L});
    public static final BitSet FOLLOW_END_in_functions_block566 = new BitSet(new long[]{0x0000000000080000L});
    public static final BitSet FOLLOW_FUNCTIONS_in_functions_block568 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_STRING_in_function_def582 = new BitSet(new long[]{0x0000000000000000L,0x0000000000200000L});
    public static final BitSet FOLLOW_LPAREN_in_function_def584 = new BitSet(new long[]{0x0000000000000000L,0x0000000000401000L});
    public static final BitSet FOLLOW_STRING_in_function_def587 = new BitSet(new long[]{0x0000000000000000L,0x0000000000400000L});
    public static final BitSet FOLLOW_RPAREN_in_function_def591 = new BitSet(new long[]{0x0000100000000000L,0x0000002400201600L});
    public static final BitSet FOLLOW_BECOMES_in_function_def594 = new BitSet(new long[]{0x0000100000000000L,0x0000002000201600L});
    public static final BitSet FOLLOW_expression_in_function_def599 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_BEGIN_in_compartments_block662 = new BitSet(new long[]{0x0000000000100000L});
    public static final BitSet FOLLOW_COMPARTMENTS_in_compartments_block664 = new BitSet(new long[]{0x0000000000000080L,0x0000000000001000L});
    public static final BitSet FOLLOW_compartment_in_compartments_block673 = new BitSet(new long[]{0x0000000000000080L,0x0000000000001000L});
    public static final BitSet FOLLOW_END_in_compartments_block681 = new BitSet(new long[]{0x0000000000100000L});
    public static final BitSet FOLLOW_COMPARTMENTS_in_compartments_block683 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_STRING_in_compartment697 = new BitSet(new long[]{0x0000000000000000L,0x0000000000000400L});
    public static final BitSet FOLLOW_INT_in_compartment699 = new BitSet(new long[]{0x0000100000000000L,0x0000002000201600L});
    public static final BitSet FOLLOW_expression_in_compartment703 = new BitSet(new long[]{0x0000000000000002L,0x0000000000001000L});
    public static final BitSet FOLLOW_STRING_in_compartment709 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_MOD_in_label749 = new BitSet(new long[]{0x0000000000000000L,0x0000000000001000L});
    public static final BitSet FOLLOW_STRING_in_label751 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_AMPERSAND_in_dereference767 = new BitSet(new long[]{0x0000000000000000L,0x0000000000001000L});
    public static final BitSet FOLLOW_STRING_in_dereference769 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_BEGIN_in_reactions_block787 = new BitSet(new long[]{0x0000000000020000L});
    public static final BitSet FOLLOW_REACTIONS_in_reactions_block789 = new BitSet(new long[]{0x0000000000000080L,0x0000000000000400L});
    public static final BitSet FOLLOW_INT_in_reactions_block794 = new BitSet(new long[]{0x0000000000000000L,0x0000000000000400L});
    public static final BitSet FOLLOW_reaction_in_reactions_block796 = new BitSet(new long[]{0x0000000000000080L,0x0000000000000400L});
    public static final BitSet FOLLOW_END_in_reactions_block804 = new BitSet(new long[]{0x0000000000020000L});
    public static final BitSet FOLLOW_REACTIONS_in_reactions_block806 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_reactant_in_reaction823 = new BitSet(new long[]{0x0000000000000000L,0x0000000000000400L});
    public static final BitSet FOLLOW_reactant_in_reaction826 = new BitSet(new long[]{0x0000100000000000L,0x0000002000201600L});
    public static final BitSet FOLLOW_expression_in_reaction829 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_INT_in_reactant863 = new BitSet(new long[]{0x0000000000000002L,0x0000000000080000L});
    public static final BitSet FOLLOW_COMMA_in_reactant876 = new BitSet(new long[]{0x0000000000000000L,0x0000000000000400L});
    public static final BitSet FOLLOW_INT_in_reactant880 = new BitSet(new long[]{0x0000000000000002L,0x0000000000080000L});

}