// $ANTLR 3.3 Nov 30, 2010 12:50:56 /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGTree.g 2012-03-26 19:12:11

  package bngparser.grammars;
  import java.util.HashMap;
  import java.util.Stack;
  import bngparser.dataType.SingleMoleculeType;
  import bngparser.dataType.MoleculeType;
  


import org.antlr.runtime.*;
import org.antlr.runtime.tree.*;import java.util.Stack;
import java.util.List;
import java.util.ArrayList;

public class BNGTree extends TreeParser {
    public static final String[] tokenNames = new String[] {
        "<invalid>", "<EOR>", "<DOWN>", "<UP>", "LINE_COMMENT", "WS", "VERSION", "END", "BEGIN", "SEED", "SPECIES", "GROUPS", "MOLECULE", "MOLECULES", "MODEL", "TYPES", "REACTION", "REACTIONS", "RULES", "FUNCTIONS", "COMPARTMENTS", "ACTIONS", "OBSERVABLES", "SET_OPTION", "SPECIES_LABEL", "GENERATE_NETWORK", "MAX_AGG", "MAX_ITER", "MAX_STOICH", "OVERWRITE", "PRINT_ITER", "AND", "OR", "ID", "IF", "PREFFIX", "SUFFIX", "LABEL", "PARAMETERS", "VERBOSE", "MATCHONCE", "DELETEMOLECULES", "MOVECONNECTED", "AUTO", "HNAUTY", "QUASY", "NOT", "INCLUDE_REACTANTS", "INCLUDE_PRODUCTS", "EXCLUDE_REACTANTS", "EXCLUDE_PRODUCTS", "ATOL", "RTOL", "STEADY_STATE", "SPARSE", "T_END", "T_START", "N_STEPS", "SAMPLE_TIMES", "SIMULATE_ODE", "SIMULATE_SSA", "READFILE", "FILE", "WRITENET", "WRITESBML", "WRITEMFILE", "WRITEXML", "SETCONCENTRATION", "SETPARAMETER", "SAVECONCENTRATIONS", "RESETCONCENTRATIONS", "GENERATENETWORK", "SUBSTANCEUNITS", "SAT", "DIGIT", "EXPONENT", "FLOAT", "INT", "LETTER", "STRING", "SEMI", "COLON", "LSBRACKET", "RSBRACKET", "LBRACKET", "RBRACKET", "COMMA", "DOT", "LPAREN", "RPAREN", "UNI_REACTION_SIGN", "BI_REACTION_SIGN", "DOLLAR", "TILDE", "AT", "GTE", "GT", "LTE", "LT", "ASSIGNS", "EQUALS", "BECOMES", "DIV", "TIMES", "MINUS", "PLUS", "POWER", "MOD", "QMARK", "EMARK", "DBQUOTES", "AMPERSAND", "NULL", "LOWER", "UPPER", "VERSION_NUMBER"
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


        public BNGTree(TreeNodeStream input) {
            this(input, new RecognizerSharedState());
        }
        public BNGTree(TreeNodeStream input, RecognizerSharedState state) {
            super(input, state);
             
        }
        

    public String[] getTokenNames() { return BNGTree.tokenNames; }
    public String getGrammarFileName() { return "/home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGTree.g"; }


      HashMap memory = new HashMap();
      Stack<String> stack = new Stack<String>();



    // $ANTLR start "prog"
    // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGTree.g:23:1: prog : program_body ;
    public final void prog() throws RecognitionException {
        try {
            // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGTree.g:23:5: ( program_body )
            // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGTree.g:23:7: program_body
            {

            System.out.println("Ready to rock");

            pushFollow(FOLLOW_program_body_in_prog53);
            program_body();

            state._fsp--;


            }

        }
        catch (RecognitionException re) {
            reportError(re);
            recover(input,re);
        }
        finally {
        }
        return ;
    }
    // $ANTLR end "prog"


    // $ANTLR start "program_body"
    // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGTree.g:27:1: program_body : ( program_block )* ;
    public final void program_body() throws RecognitionException {
        try {
            // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGTree.g:27:13: ( ( program_block )* )
            // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGTree.g:27:16: ( program_block )*
            {
            // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGTree.g:27:16: ( program_block )*
            loop1:
            do {
                int alt1=2;
                int LA1_0 = input.LA(1);

                if ( (LA1_0==SEED||LA1_0==MOLECULE||LA1_0==PARAMETERS) ) {
                    alt1=1;
                }


                switch (alt1) {
            	case 1 :
            	    // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGTree.g:27:17: program_block
            	    {
            	    pushFollow(FOLLOW_program_block_in_program_body62);
            	    program_block();

            	    state._fsp--;


            	    }
            	    break;

            	default :
            	    break loop1;
                }
            } while (true);


            }

        }
        catch (RecognitionException re) {
            reportError(re);
            recover(input,re);
        }
        finally {
        }
        return ;
    }
    // $ANTLR end "program_body"


    // $ANTLR start "program_block"
    // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGTree.g:29:1: program_block : ( parameters_block | molecule_types_block | seed_species_block );
    public final void program_block() throws RecognitionException {
        try {
            // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGTree.g:30:1: ( parameters_block | molecule_types_block | seed_species_block )
            int alt2=3;
            switch ( input.LA(1) ) {
            case PARAMETERS:
                {
                alt2=1;
                }
                break;
            case MOLECULE:
                {
                alt2=2;
                }
                break;
            case SEED:
                {
                alt2=3;
                }
                break;
            default:
                NoViableAltException nvae =
                    new NoViableAltException("", 2, 0, input);

                throw nvae;
            }

            switch (alt2) {
                case 1 :
                    // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGTree.g:30:3: parameters_block
                    {
                    pushFollow(FOLLOW_parameters_block_in_program_block72);
                    parameters_block();

                    state._fsp--;


                    }
                    break;
                case 2 :
                    // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGTree.g:31:3: molecule_types_block
                    {
                    pushFollow(FOLLOW_molecule_types_block_in_program_block76);
                    molecule_types_block();

                    state._fsp--;


                    }
                    break;
                case 3 :
                    // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGTree.g:32:3: seed_species_block
                    {
                    pushFollow(FOLLOW_seed_species_block_in_program_block80);
                    seed_species_block();

                    state._fsp--;


                    }
                    break;

            }
        }
        catch (RecognitionException re) {
            reportError(re);
            recover(input,re);
        }
        finally {
        }
        return ;
    }
    // $ANTLR end "program_block"


    // $ANTLR start "parameters_block"
    // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGTree.g:38:1: parameters_block : PARAMETERS ( parameters_def )* ;
    public final void parameters_block() throws RecognitionException {
        try {
            // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGTree.g:38:17: ( PARAMETERS ( parameters_def )* )
            // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGTree.g:39:3: PARAMETERS ( parameters_def )*
            {
            match(input,PARAMETERS,FOLLOW_PARAMETERS_in_parameters_block94); 
            // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGTree.g:39:14: ( parameters_def )*
            loop3:
            do {
                int alt3=2;
                int LA3_0 = input.LA(1);

                if ( (LA3_0==BECOMES) ) {
                    alt3=1;
                }


                switch (alt3) {
            	case 1 :
            	    // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGTree.g:39:14: parameters_def
            	    {
            	    pushFollow(FOLLOW_parameters_def_in_parameters_block96);
            	    parameters_def();

            	    state._fsp--;


            	    }
            	    break;

            	default :
            	    break loop3;
                }
            } while (true);


            }

        }
        catch (RecognitionException re) {
            reportError(re);
            recover(input,re);
        }
        finally {
        }
        return ;
    }
    // $ANTLR end "parameters_block"


    // $ANTLR start "parameters_def"
    // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGTree.g:41:1: parameters_def : ^( BECOMES STRING expr ) ;
    public final void parameters_def() throws RecognitionException {
        CommonTree STRING1=null;
        float expr2 = 0.0f;


        try {
            // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGTree.g:41:15: ( ^( BECOMES STRING expr ) )
            // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGTree.g:41:17: ^( BECOMES STRING expr )
            {
            match(input,BECOMES,FOLLOW_BECOMES_in_parameters_def105); 

            match(input, Token.DOWN, null); 
            STRING1=(CommonTree)match(input,STRING,FOLLOW_STRING_in_parameters_def107); 
            pushFollow(FOLLOW_expr_in_parameters_def109);
            expr2=expr();

            state._fsp--;


            match(input, Token.UP, null); 
            memory.put((STRING1!=null?STRING1.getText():null),new Float(expr2));
            System.out.println("parameter: " + (STRING1!=null?STRING1.getText():null) + " = " + expr2);

            }

        }
        catch (RecognitionException re) {
            reportError(re);
            recover(input,re);
        }
        finally {
        }
        return ;
    }
    // $ANTLR end "parameters_def"


    // $ANTLR start "expr"
    // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGTree.g:44:1: expr returns [float value] : ( ^( PLUS a= expr b= expr ) | ^( MINUS a= expr b= expr ) | ^( TIMES a= expr b= expr ) | ^( DIV a= expr b= expr ) | ^( MOD a= expr b= expr ) | FLOAT | STRING );
    public final float expr() throws RecognitionException {
        float value = 0.0f;

        CommonTree FLOAT3=null;
        CommonTree STRING4=null;
        float a = 0.0f;

        float b = 0.0f;


        try {
            // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGTree.g:44:27: ( ^( PLUS a= expr b= expr ) | ^( MINUS a= expr b= expr ) | ^( TIMES a= expr b= expr ) | ^( DIV a= expr b= expr ) | ^( MOD a= expr b= expr ) | FLOAT | STRING )
            int alt4=7;
            switch ( input.LA(1) ) {
            case PLUS:
                {
                alt4=1;
                }
                break;
            case MINUS:
                {
                alt4=2;
                }
                break;
            case TIMES:
                {
                alt4=3;
                }
                break;
            case DIV:
                {
                alt4=4;
                }
                break;
            case MOD:
                {
                alt4=5;
                }
                break;
            case FLOAT:
                {
                alt4=6;
                }
                break;
            case STRING:
                {
                alt4=7;
                }
                break;
            default:
                NoViableAltException nvae =
                    new NoViableAltException("", 4, 0, input);

                throw nvae;
            }

            switch (alt4) {
                case 1 :
                    // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGTree.g:45:4: ^( PLUS a= expr b= expr )
                    {
                    match(input,PLUS,FOLLOW_PLUS_in_expr126); 

                    match(input, Token.DOWN, null); 
                    pushFollow(FOLLOW_expr_in_expr130);
                    a=expr();

                    state._fsp--;

                    pushFollow(FOLLOW_expr_in_expr134);
                    b=expr();

                    state._fsp--;


                    match(input, Token.UP, null); 
                    value = a+b;

                    }
                    break;
                case 2 :
                    // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGTree.g:46:4: ^( MINUS a= expr b= expr )
                    {
                    match(input,MINUS,FOLLOW_MINUS_in_expr143); 

                    match(input, Token.DOWN, null); 
                    pushFollow(FOLLOW_expr_in_expr147);
                    a=expr();

                    state._fsp--;

                    pushFollow(FOLLOW_expr_in_expr151);
                    b=expr();

                    state._fsp--;


                    match(input, Token.UP, null); 
                    value = a-b;

                    }
                    break;
                case 3 :
                    // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGTree.g:47:4: ^( TIMES a= expr b= expr )
                    {
                    match(input,TIMES,FOLLOW_TIMES_in_expr160); 

                    match(input, Token.DOWN, null); 
                    pushFollow(FOLLOW_expr_in_expr164);
                    a=expr();

                    state._fsp--;

                    pushFollow(FOLLOW_expr_in_expr168);
                    b=expr();

                    state._fsp--;


                    match(input, Token.UP, null); 
                    value = a*b;

                    }
                    break;
                case 4 :
                    // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGTree.g:48:4: ^( DIV a= expr b= expr )
                    {
                    match(input,DIV,FOLLOW_DIV_in_expr177); 

                    match(input, Token.DOWN, null); 
                    pushFollow(FOLLOW_expr_in_expr181);
                    a=expr();

                    state._fsp--;

                    pushFollow(FOLLOW_expr_in_expr185);
                    b=expr();

                    state._fsp--;


                    match(input, Token.UP, null); 
                    value = a/b;

                    }
                    break;
                case 5 :
                    // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGTree.g:49:4: ^( MOD a= expr b= expr )
                    {
                    match(input,MOD,FOLLOW_MOD_in_expr194); 

                    match(input, Token.DOWN, null); 
                    pushFollow(FOLLOW_expr_in_expr198);
                    a=expr();

                    state._fsp--;

                    pushFollow(FOLLOW_expr_in_expr202);
                    b=expr();

                    state._fsp--;


                    match(input, Token.UP, null); 
                    value = a%b;

                    }
                    break;
                case 6 :
                    // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGTree.g:50:4: FLOAT
                    {
                    FLOAT3=(CommonTree)match(input,FLOAT,FOLLOW_FLOAT_in_expr210); 
                    value = Float.parseFloat((FLOAT3!=null?FLOAT3.getText():null));

                    }
                    break;
                case 7 :
                    // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGTree.g:51:3: STRING
                    {
                    STRING4=(CommonTree)match(input,STRING,FOLLOW_STRING_in_expr216); 

                        Float v = (Float)memory.get((STRING4!=null?STRING4.getText():null));
                        if(v!=null) value = v.floatValue();
                       

                    }
                    break;

            }
        }
        catch (RecognitionException re) {
            reportError(re);
            recover(input,re);
        }
        finally {
        }
        return value;
    }
    // $ANTLR end "expr"


    // $ANTLR start "molecule_types_block"
    // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGTree.g:57:1: molecule_types_block : MOLECULE TYPES ( molecule_def )* ;
    public final void molecule_types_block() throws RecognitionException {
        try {
            // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGTree.g:57:21: ( MOLECULE TYPES ( molecule_def )* )
            // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGTree.g:57:23: MOLECULE TYPES ( molecule_def )*
            {
            match(input,MOLECULE,FOLLOW_MOLECULE_in_molecule_types_block226); 
            match(input,TYPES,FOLLOW_TYPES_in_molecule_types_block228); 
            // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGTree.g:57:38: ( molecule_def )*
            loop5:
            do {
                int alt5=2;
                int LA5_0 = input.LA(1);

                if ( (LA5_0==STRING) ) {
                    alt5=1;
                }


                switch (alt5) {
            	case 1 :
            	    // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGTree.g:57:38: molecule_def
            	    {
            	    pushFollow(FOLLOW_molecule_def_in_molecule_types_block230);
            	    molecule_def();

            	    state._fsp--;


            	    }
            	    break;

            	default :
            	    break loop5;
                }
            } while (true);


            }

        }
        catch (RecognitionException re) {
            reportError(re);
            recover(input,re);
        }
        finally {
        }
        return ;
    }
    // $ANTLR end "molecule_types_block"


    // $ANTLR start "molecule_def"
    // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGTree.g:59:1: molecule_def : ^( STRING ( site_def )* ) ;
    public final void molecule_def() throws RecognitionException {
        CommonTree STRING5=null;

        try {
            // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGTree.g:59:13: ( ^( STRING ( site_def )* ) )
            // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGTree.g:59:15: ^( STRING ( site_def )* )
            {
            STRING5=(CommonTree)match(input,STRING,FOLLOW_STRING_in_molecule_def239); 

            if ( input.LA(1)==Token.DOWN ) {
                match(input, Token.DOWN, null); 
                // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGTree.g:59:24: ( site_def )*
                loop6:
                do {
                    int alt6=2;
                    int LA6_0 = input.LA(1);

                    if ( (LA6_0==STRING) ) {
                        alt6=1;
                    }


                    switch (alt6) {
                	case 1 :
                	    // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGTree.g:59:24: site_def
                	    {
                	    pushFollow(FOLLOW_site_def_in_molecule_def241);
                	    site_def();

                	    state._fsp--;


                	    }
                	    break;

                	default :
                	    break loop6;
                    }
                } while (true);


                match(input, Token.UP, null); 
            }

              SingleMoleculeType moleculeTypes = new SingleMoleculeType((STRING5!=null?STRING5.getText():null));
              while(!stack.empty()){
                String pointer = stack.pop();
                String[] temp = pointer.split(",");
                if(temp[0].equals("siteDef"))
                  moleculeTypes.addNewSiteElement(temp[1]);
                else
                  moleculeTypes.addElement(temp[1]);
              }
              stack.clear();
              moleculeTypes.reverse();
              memory.put((STRING5!=null?STRING5.getText():null), moleculeTypes);
              
              System.out.println(memory.get((STRING5!=null?STRING5.getText():null)));
              

            }

        }
        catch (RecognitionException re) {
            reportError(re);
            recover(input,re);
        }
        finally {
        }
        return ;
    }
    // $ANTLR end "molecule_def"


    // $ANTLR start "site_def"
    // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGTree.g:76:1: site_def : ^( STRING ( state_label )* ) ;
    public final void site_def() throws RecognitionException {
        CommonTree STRING6=null;

        try {
            // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGTree.g:76:9: ( ^( STRING ( state_label )* ) )
            // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGTree.g:76:11: ^( STRING ( state_label )* )
            {
            STRING6=(CommonTree)match(input,STRING,FOLLOW_STRING_in_site_def254); 

            if ( input.LA(1)==Token.DOWN ) {
                match(input, Token.DOWN, null); 
                // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGTree.g:76:20: ( state_label )*
                loop7:
                do {
                    int alt7=2;
                    int LA7_0 = input.LA(1);

                    if ( (LA7_0==STRING) ) {
                        alt7=1;
                    }


                    switch (alt7) {
                	case 1 :
                	    // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGTree.g:76:20: state_label
                	    {
                	    pushFollow(FOLLOW_state_label_in_site_def256);
                	    state_label();

                	    state._fsp--;


                	    }
                	    break;

                	default :
                	    break loop7;
                    }
                } while (true);


                match(input, Token.UP, null); 
            }

              stack.push(new String("siteDef," + (STRING6!=null?STRING6.getText():null)));
              

            }

        }
        catch (RecognitionException re) {
            reportError(re);
            recover(input,re);
        }
        finally {
        }
        return ;
    }
    // $ANTLR end "site_def"


    // $ANTLR start "state_label"
    // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGTree.g:80:1: state_label : STRING ;
    public final void state_label() throws RecognitionException {
        CommonTree STRING7=null;

        try {
            // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGTree.g:80:12: ( STRING )
            // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGTree.g:80:14: STRING
            {
            STRING7=(CommonTree)match(input,STRING,FOLLOW_STRING_in_state_label266); 

              stack.push(new String("stateLabel," + (STRING7!=null?STRING7.getText():null)));
              
              

            }

        }
        catch (RecognitionException re) {
            reportError(re);
            recover(input,re);
        }
        finally {
        }
        return ;
    }
    // $ANTLR end "state_label"


    // $ANTLR start "seed_species_block"
    // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGTree.g:85:1: seed_species_block : SEED SPECIES ( seed_species_def )* ;
    public final void seed_species_block() throws RecognitionException {
        try {
            // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGTree.g:85:19: ( SEED SPECIES ( seed_species_def )* )
            // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGTree.g:85:21: SEED SPECIES ( seed_species_def )*
            {
            match(input,SEED,FOLLOW_SEED_in_seed_species_block275); 
            match(input,SPECIES,FOLLOW_SPECIES_in_seed_species_block277); 
            // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGTree.g:85:34: ( seed_species_def )*
            loop8:
            do {
                int alt8=2;
                int LA8_0 = input.LA(1);

                if ( (LA8_0==STRING||LA8_0==DOLLAR) ) {
                    alt8=1;
                }


                switch (alt8) {
            	case 1 :
            	    // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGTree.g:85:34: seed_species_def
            	    {
            	    pushFollow(FOLLOW_seed_species_def_in_seed_species_block279);
            	    seed_species_def();

            	    state._fsp--;


            	    }
            	    break;

            	default :
            	    break loop8;
                }
            } while (true);


            }

        }
        catch (RecognitionException re) {
            reportError(re);
            recover(input,re);
        }
        finally {
        }
        return ;
    }
    // $ANTLR end "seed_species_block"


    // $ANTLR start "seed_species_def"
    // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGTree.g:87:1: seed_species_def : ( constant_species_def expr | ( species_def expr ) );
    public final void seed_species_def() throws RecognitionException {
        String constant_species_def8 = null;

        float expr9 = 0.0f;


        try {
            // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGTree.g:87:17: ( constant_species_def expr | ( species_def expr ) )
            int alt9=2;
            int LA9_0 = input.LA(1);

            if ( (LA9_0==DOLLAR) ) {
                alt9=1;
            }
            else if ( (LA9_0==STRING) ) {
                alt9=2;
            }
            else {
                NoViableAltException nvae =
                    new NoViableAltException("", 9, 0, input);

                throw nvae;
            }
            switch (alt9) {
                case 1 :
                    // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGTree.g:87:19: constant_species_def expr
                    {
                    pushFollow(FOLLOW_constant_species_def_in_seed_species_def287);
                    constant_species_def8=constant_species_def();

                    state._fsp--;

                    pushFollow(FOLLOW_expr_in_seed_species_def289);
                    expr9=expr();

                    state._fsp--;


                                        memory.put(constant_species_def8, expr9);
                                      

                    }
                    break;
                case 2 :
                    // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGTree.g:90:21: ( species_def expr )
                    {
                    // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGTree.g:90:21: ( species_def expr )
                    // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGTree.g:90:22: species_def expr
                    {
                    pushFollow(FOLLOW_species_def_in_seed_species_def313);
                    species_def();

                    state._fsp--;

                    pushFollow(FOLLOW_expr_in_seed_species_def315);
                    expr();

                    state._fsp--;


                    }


                                          MoleculeType moleculeType = new MoleculeType();
                                          while(!stack.empty()){
                                              String pointer = stack.pop();
                                              String[] temp = pointer.split(",");
                                              if(temp[0].equals("bondLabelInt")){
                                                System.out.println("bondLabel " +temp[1]);
                                              }
                                              else if(temp[0].equals("bondLabelString")){
                                                System.out.println("bondLabelString "+  temp[1]);
                                              
                                              }
                                              
                                              if(temp[0].equals("stateLabel")){
                                                System.out.println("stateLabel " + temp[1]);
                                              
                                              }
                                              
                                              else if(temp[0].equals("siteLabel")){
                                              
                                                System.out.println("siteLabel " + temp[1]);
                                              }
                                              
                                              else if(temp[0].equals("speciesLabel")){
                                                SingleMoleculeType v = (SingleMoleculeType)memory.get(temp[1]);
                                                moleculeType.addMolecules(v);
                                                System.out.println("speciesLabel " + temp[1]);
                                                //memory.put($STRING.text, moleculeTypes);
                                                
                                              }
                                              else{
                                              
                                              }   
                                           
                                          }
                                          
                                          stack.clear();
                                      

                    }
                    break;

            }
        }
        catch (RecognitionException re) {
            reportError(re);
            recover(input,re);
        }
        finally {
        }
        return ;
    }
    // $ANTLR end "seed_species_def"


    // $ANTLR start "constant_species_def"
    // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGTree.g:129:1: constant_species_def returns [String value] : DOLLAR STRING ;
    public final String constant_species_def() throws RecognitionException {
        String value = null;

        CommonTree STRING10=null;

        try {
            // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGTree.g:129:44: ( DOLLAR STRING )
            // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGTree.g:129:46: DOLLAR STRING
            {
            match(input,DOLLAR,FOLLOW_DOLLAR_in_constant_species_def328); 
            STRING10=(CommonTree)match(input,STRING,FOLLOW_STRING_in_constant_species_def330); 

                    value = (STRING10!=null?STRING10.getText():null);
                  

            }

        }
        catch (RecognitionException re) {
            reportError(re);
            recover(input,re);
        }
        finally {
        }
        return value;
    }
    // $ANTLR end "constant_species_def"


    // $ANTLR start "species_def"
    // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGTree.g:134:1: species_def : species_element ( DOT species_element )* ;
    public final void species_def() throws RecognitionException {
        try {
            // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGTree.g:134:12: ( species_element ( DOT species_element )* )
            // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGTree.g:134:14: species_element ( DOT species_element )*
            {
            pushFollow(FOLLOW_species_element_in_species_def345);
            species_element();

            state._fsp--;

            // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGTree.g:134:30: ( DOT species_element )*
            loop10:
            do {
                int alt10=2;
                int LA10_0 = input.LA(1);

                if ( (LA10_0==DOT) ) {
                    alt10=1;
                }


                switch (alt10) {
            	case 1 :
            	    // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGTree.g:134:31: DOT species_element
            	    {
            	    match(input,DOT,FOLLOW_DOT_in_species_def348); 
            	    pushFollow(FOLLOW_species_element_in_species_def350);
            	    species_element();

            	    state._fsp--;


            	    }
            	    break;

            	default :
            	    break loop10;
                }
            } while (true);


            }

        }
        catch (RecognitionException re) {
            reportError(re);
            recover(input,re);
        }
        finally {
        }
        return ;
    }
    // $ANTLR end "species_def"


    // $ANTLR start "species_element"
    // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGTree.g:137:1: species_element : ^( STRING site_list ) ;
    public final void species_element() throws RecognitionException {
        CommonTree STRING11=null;

        try {
            // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGTree.g:137:16: ( ^( STRING site_list ) )
            // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGTree.g:137:18: ^( STRING site_list )
            {
            STRING11=(CommonTree)match(input,STRING,FOLLOW_STRING_in_species_element362); 

            if ( input.LA(1)==Token.DOWN ) {
                match(input, Token.DOWN, null); 
                pushFollow(FOLLOW_site_list_in_species_element364);
                site_list();

                state._fsp--;


                match(input, Token.UP, null); 
            }

                stack.push(new String("speciesLabel" + (STRING11!=null?STRING11.getText():null)));
               
                


            }

        }
        catch (RecognitionException re) {
            reportError(re);
            recover(input,re);
        }
        finally {
        }
        return ;
    }
    // $ANTLR end "species_element"


    // $ANTLR start "site_list"
    // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGTree.g:144:1: site_list : ( site_obs ( COMMA site_obs )* | );
    public final void site_list() throws RecognitionException {
        try {
            // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGTree.g:144:10: ( site_obs ( COMMA site_obs )* | )
            int alt12=2;
            int LA12_0 = input.LA(1);

            if ( (LA12_0==STRING) ) {
                alt12=1;
            }
            else if ( (LA12_0==UP) ) {
                alt12=2;
            }
            else {
                NoViableAltException nvae =
                    new NoViableAltException("", 12, 0, input);

                throw nvae;
            }
            switch (alt12) {
                case 1 :
                    // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGTree.g:144:14: site_obs ( COMMA site_obs )*
                    {
                    pushFollow(FOLLOW_site_obs_in_site_list376);
                    site_obs();

                    state._fsp--;

                    // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGTree.g:144:23: ( COMMA site_obs )*
                    loop11:
                    do {
                        int alt11=2;
                        int LA11_0 = input.LA(1);

                        if ( (LA11_0==COMMA) ) {
                            alt11=1;
                        }


                        switch (alt11) {
                    	case 1 :
                    	    // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGTree.g:144:24: COMMA site_obs
                    	    {
                    	    match(input,COMMA,FOLLOW_COMMA_in_site_list379); 
                    	    pushFollow(FOLLOW_site_obs_in_site_list381);
                    	    site_obs();

                    	    state._fsp--;


                    	    }
                    	    break;

                    	default :
                    	    break loop11;
                        }
                    } while (true);


                    }
                    break;
                case 2 :
                    // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGTree.g:146:3: 
                    {
                    }
                    break;

            }
        }
        catch (RecognitionException re) {
            reportError(re);
            recover(input,re);
        }
        finally {
        }
        return ;
    }
    // $ANTLR end "site_list"


    // $ANTLR start "site_obs"
    // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGTree.g:148:1: site_obs : STRING ( TILDE state_label )? ( EMARK ( PLUS | QMARK | bond_name ) )? ;
    public final void site_obs() throws RecognitionException {
        CommonTree STRING12=null;

        try {
            // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGTree.g:148:9: ( STRING ( TILDE state_label )? ( EMARK ( PLUS | QMARK | bond_name ) )? )
            // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGTree.g:148:11: STRING ( TILDE state_label )? ( EMARK ( PLUS | QMARK | bond_name ) )?
            {
            STRING12=(CommonTree)match(input,STRING,FOLLOW_STRING_in_site_obs398); 

                stack.push(new String("siteLabel," + (STRING12!=null?STRING12.getText():null)));
              
            // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGTree.g:151:1: ( TILDE state_label )?
            int alt13=2;
            int LA13_0 = input.LA(1);

            if ( (LA13_0==TILDE) ) {
                alt13=1;
            }
            switch (alt13) {
                case 1 :
                    // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGTree.g:151:2: TILDE state_label
                    {
                    match(input,TILDE,FOLLOW_TILDE_in_site_obs403); 
                    pushFollow(FOLLOW_state_label_in_site_obs405);
                    state_label();

                    state._fsp--;


                    }
                    break;

            }

            // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGTree.g:151:22: ( EMARK ( PLUS | QMARK | bond_name ) )?
            int alt15=2;
            int LA15_0 = input.LA(1);

            if ( (LA15_0==EMARK) ) {
                alt15=1;
            }
            switch (alt15) {
                case 1 :
                    // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGTree.g:151:23: EMARK ( PLUS | QMARK | bond_name )
                    {
                    match(input,EMARK,FOLLOW_EMARK_in_site_obs410); 
                    // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGTree.g:151:29: ( PLUS | QMARK | bond_name )
                    int alt14=3;
                    switch ( input.LA(1) ) {
                    case PLUS:
                        {
                        alt14=1;
                        }
                        break;
                    case QMARK:
                        {
                        alt14=2;
                        }
                        break;
                    case INT:
                    case STRING:
                        {
                        alt14=3;
                        }
                        break;
                    default:
                        NoViableAltException nvae =
                            new NoViableAltException("", 14, 0, input);

                        throw nvae;
                    }

                    switch (alt14) {
                        case 1 :
                            // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGTree.g:151:30: PLUS
                            {
                            match(input,PLUS,FOLLOW_PLUS_in_site_obs413); 

                            }
                            break;
                        case 2 :
                            // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGTree.g:151:35: QMARK
                            {
                            match(input,QMARK,FOLLOW_QMARK_in_site_obs415); 

                            }
                            break;
                        case 3 :
                            // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGTree.g:151:41: bond_name
                            {
                            pushFollow(FOLLOW_bond_name_in_site_obs417);
                            bond_name();

                            state._fsp--;


                            }
                            break;

                    }


                    }
                    break;

            }


            }

        }
        catch (RecognitionException re) {
            reportError(re);
            recover(input,re);
        }
        finally {
        }
        return ;
    }
    // $ANTLR end "site_obs"


    // $ANTLR start "bond_name"
    // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGTree.g:153:1: bond_name : ( INT | STRING );
    public final void bond_name() throws RecognitionException {
        CommonTree INT13=null;
        CommonTree STRING14=null;

        try {
            // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGTree.g:153:10: ( INT | STRING )
            int alt16=2;
            int LA16_0 = input.LA(1);

            if ( (LA16_0==INT) ) {
                alt16=1;
            }
            else if ( (LA16_0==STRING) ) {
                alt16=2;
            }
            else {
                NoViableAltException nvae =
                    new NoViableAltException("", 16, 0, input);

                throw nvae;
            }
            switch (alt16) {
                case 1 :
                    // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGTree.g:153:12: INT
                    {
                    INT13=(CommonTree)match(input,INT,FOLLOW_INT_in_bond_name427); 

                      stack.push(new String("bondLabelInt," + (INT13!=null?INT13.getText():null)));


                    }
                    break;
                case 2 :
                    // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGTree.g:157:12: STRING
                    {
                    STRING14=(CommonTree)match(input,STRING,FOLLOW_STRING_in_bond_name442); 

                      stack.push(new String("bondLabelString," + (STRING14!=null?STRING14.getText():null)));


                    }
                    break;

            }
        }
        catch (RecognitionException re) {
            reportError(re);
            recover(input,re);
        }
        finally {
        }
        return ;
    }
    // $ANTLR end "bond_name"


    // $ANTLR start "reaction_rules_block"
    // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGTree.g:161:1: reaction_rules_block : REACTION ( reaction_rule_def )+ ;
    public final void reaction_rules_block() throws RecognitionException {
        try {
            // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGTree.g:161:21: ( REACTION ( reaction_rule_def )+ )
            // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGTree.g:161:23: REACTION ( reaction_rule_def )+
            {
            match(input,REACTION,FOLLOW_REACTION_in_reaction_rules_block450); 
            // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGTree.g:161:32: ( reaction_rule_def )+
            int cnt17=0;
            loop17:
            do {
                int alt17=2;
                int LA17_0 = input.LA(1);

                if ( (LA17_0==BEGIN) ) {
                    alt17=1;
                }


                switch (alt17) {
            	case 1 :
            	    // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGTree.g:161:32: reaction_rule_def
            	    {
            	    pushFollow(FOLLOW_reaction_rule_def_in_reaction_rules_block452);
            	    reaction_rule_def();

            	    state._fsp--;


            	    }
            	    break;

            	default :
            	    if ( cnt17 >= 1 ) break loop17;
                        EarlyExitException eee =
                            new EarlyExitException(17, input);
                        throw eee;
                }
                cnt17++;
            } while (true);


            }

        }
        catch (RecognitionException re) {
            reportError(re);
            recover(input,re);
        }
        finally {
        }
        return ;
    }
    // $ANTLR end "reaction_rules_block"


    // $ANTLR start "reaction_rule_def"
    // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGTree.g:163:1: reaction_rule_def : BEGIN ( modif_command )* ;
    public final void reaction_rule_def() throws RecognitionException {
        try {
            // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGTree.g:163:18: ( BEGIN ( modif_command )* )
            // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGTree.g:163:20: BEGIN ( modif_command )*
            {
            match(input,BEGIN,FOLLOW_BEGIN_in_reaction_rule_def464); 
            // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGTree.g:163:26: ( modif_command )*
            loop18:
            do {
                int alt18=2;
                int LA18_0 = input.LA(1);

                if ( (LA18_0==END) ) {
                    alt18=1;
                }


                switch (alt18) {
            	case 1 :
            	    // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGTree.g:163:27: modif_command
            	    {
            	    pushFollow(FOLLOW_modif_command_in_reaction_rule_def467);
            	    modif_command();

            	    state._fsp--;


            	    }
            	    break;

            	default :
            	    break loop18;
                }
            } while (true);


            }

        }
        catch (RecognitionException re) {
            reportError(re);
            recover(input,re);
        }
        finally {
        }
        return ;
    }
    // $ANTLR end "reaction_rule_def"


    // $ANTLR start "modif_command"
    // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGTree.g:165:1: modif_command : END ;
    public final void modif_command() throws RecognitionException {
        try {
            // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGTree.g:165:14: ( END )
            // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGTree.g:165:17: END
            {
            match(input,END,FOLLOW_END_in_modif_command477); 

            }

        }
        catch (RecognitionException re) {
            reportError(re);
            recover(input,re);
        }
        finally {
        }
        return ;
    }
    // $ANTLR end "modif_command"


    // $ANTLR start "primitive_element"
    // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGTree.g:180:1: primitive_element : ( number | variable );
    public final void primitive_element() throws RecognitionException {
        try {
            // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGTree.g:180:18: ( number | variable )
            int alt19=2;
            int LA19_0 = input.LA(1);

            if ( ((LA19_0>=FLOAT && LA19_0<=INT)) ) {
                alt19=1;
            }
            else if ( (LA19_0==STRING) ) {
                alt19=2;
            }
            else {
                NoViableAltException nvae =
                    new NoViableAltException("", 19, 0, input);

                throw nvae;
            }
            switch (alt19) {
                case 1 :
                    // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGTree.g:180:20: number
                    {
                    pushFollow(FOLLOW_number_in_primitive_element518);
                    number();

                    state._fsp--;


                    }
                    break;
                case 2 :
                    // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGTree.g:180:29: variable
                    {
                    pushFollow(FOLLOW_variable_in_primitive_element522);
                    variable();

                    state._fsp--;


                    }
                    break;

            }
        }
        catch (RecognitionException re) {
            reportError(re);
            recover(input,re);
        }
        finally {
        }
        return ;
    }
    // $ANTLR end "primitive_element"


    // $ANTLR start "number"
    // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGTree.g:181:1: number : ( INT | FLOAT );
    public final void number() throws RecognitionException {
        try {
            // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGTree.g:181:8: ( INT | FLOAT )
            // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGTree.g:
            {
            if ( (input.LA(1)>=FLOAT && input.LA(1)<=INT) ) {
                input.consume();
                state.errorRecovery=false;
            }
            else {
                MismatchedSetException mse = new MismatchedSetException(null,input);
                throw mse;
            }


            }

        }
        catch (RecognitionException re) {
            reportError(re);
            recover(input,re);
        }
        finally {
        }
        return ;
    }
    // $ANTLR end "number"


    // $ANTLR start "variable"
    // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGTree.g:182:1: variable : STRING ;
    public final void variable() throws RecognitionException {
        try {
            // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGTree.g:182:10: ( STRING )
            // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGTree.g:182:12: STRING
            {
            match(input,STRING,FOLLOW_STRING_in_variable540); 

            }

        }
        catch (RecognitionException re) {
            reportError(re);
            recover(input,re);
        }
        finally {
        }
        return ;
    }
    // $ANTLR end "variable"


    // $ANTLR start "actions_block"
    // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGTree.g:186:3: actions_block : END ;
    public final void actions_block() throws RecognitionException {
        try {
            // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGTree.g:186:16: ( END )
            // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGTree.g:186:18: END
            {
            match(input,END,FOLLOW_END_in_actions_block551); 

            }

        }
        catch (RecognitionException re) {
            reportError(re);
            recover(input,re);
        }
        finally {
        }
        return ;
    }
    // $ANTLR end "actions_block"

    // Delegated rules


 

    public static final BitSet FOLLOW_program_body_in_prog53 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_program_block_in_program_body62 = new BitSet(new long[]{0x0000004000001202L});
    public static final BitSet FOLLOW_parameters_block_in_program_block72 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_molecule_types_block_in_program_block76 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_seed_species_block_in_program_block80 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_PARAMETERS_in_parameters_block94 = new BitSet(new long[]{0x0000000000000002L,0x0000002000000000L});
    public static final BitSet FOLLOW_parameters_def_in_parameters_block96 = new BitSet(new long[]{0x0000000000000002L,0x0000002000000000L});
    public static final BitSet FOLLOW_BECOMES_in_parameters_def105 = new BitSet(new long[]{0x0000000000000004L});
    public static final BitSet FOLLOW_STRING_in_parameters_def107 = new BitSet(new long[]{0x0000000000000000L,0x00000BC000009000L});
    public static final BitSet FOLLOW_expr_in_parameters_def109 = new BitSet(new long[]{0x0000000000000008L});
    public static final BitSet FOLLOW_PLUS_in_expr126 = new BitSet(new long[]{0x0000000000000004L});
    public static final BitSet FOLLOW_expr_in_expr130 = new BitSet(new long[]{0x0000000000000000L,0x00000BC000009000L});
    public static final BitSet FOLLOW_expr_in_expr134 = new BitSet(new long[]{0x0000000000000008L});
    public static final BitSet FOLLOW_MINUS_in_expr143 = new BitSet(new long[]{0x0000000000000004L});
    public static final BitSet FOLLOW_expr_in_expr147 = new BitSet(new long[]{0x0000000000000000L,0x00000BC000009000L});
    public static final BitSet FOLLOW_expr_in_expr151 = new BitSet(new long[]{0x0000000000000008L});
    public static final BitSet FOLLOW_TIMES_in_expr160 = new BitSet(new long[]{0x0000000000000004L});
    public static final BitSet FOLLOW_expr_in_expr164 = new BitSet(new long[]{0x0000000000000000L,0x00000BC000009000L});
    public static final BitSet FOLLOW_expr_in_expr168 = new BitSet(new long[]{0x0000000000000008L});
    public static final BitSet FOLLOW_DIV_in_expr177 = new BitSet(new long[]{0x0000000000000004L});
    public static final BitSet FOLLOW_expr_in_expr181 = new BitSet(new long[]{0x0000000000000000L,0x00000BC000009000L});
    public static final BitSet FOLLOW_expr_in_expr185 = new BitSet(new long[]{0x0000000000000008L});
    public static final BitSet FOLLOW_MOD_in_expr194 = new BitSet(new long[]{0x0000000000000004L});
    public static final BitSet FOLLOW_expr_in_expr198 = new BitSet(new long[]{0x0000000000000000L,0x00000BC000009000L});
    public static final BitSet FOLLOW_expr_in_expr202 = new BitSet(new long[]{0x0000000000000008L});
    public static final BitSet FOLLOW_FLOAT_in_expr210 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_STRING_in_expr216 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_MOLECULE_in_molecule_types_block226 = new BitSet(new long[]{0x0000000000008000L});
    public static final BitSet FOLLOW_TYPES_in_molecule_types_block228 = new BitSet(new long[]{0x0000000000000002L,0x0000000000008000L});
    public static final BitSet FOLLOW_molecule_def_in_molecule_types_block230 = new BitSet(new long[]{0x0000000000000002L,0x0000000000008000L});
    public static final BitSet FOLLOW_STRING_in_molecule_def239 = new BitSet(new long[]{0x0000000000000004L});
    public static final BitSet FOLLOW_site_def_in_molecule_def241 = new BitSet(new long[]{0x0000000000000008L,0x0000000000008000L});
    public static final BitSet FOLLOW_STRING_in_site_def254 = new BitSet(new long[]{0x0000000000000004L});
    public static final BitSet FOLLOW_state_label_in_site_def256 = new BitSet(new long[]{0x0000000000000008L,0x0000000000008000L});
    public static final BitSet FOLLOW_STRING_in_state_label266 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_SEED_in_seed_species_block275 = new BitSet(new long[]{0x0000000000000400L});
    public static final BitSet FOLLOW_SPECIES_in_seed_species_block277 = new BitSet(new long[]{0x0000000000000002L,0x0000000010008000L});
    public static final BitSet FOLLOW_seed_species_def_in_seed_species_block279 = new BitSet(new long[]{0x0000000000000002L,0x0000000010008000L});
    public static final BitSet FOLLOW_constant_species_def_in_seed_species_def287 = new BitSet(new long[]{0x0000000000000000L,0x00000BC000009000L});
    public static final BitSet FOLLOW_expr_in_seed_species_def289 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_species_def_in_seed_species_def313 = new BitSet(new long[]{0x0000000000000000L,0x00000BC000009000L});
    public static final BitSet FOLLOW_expr_in_seed_species_def315 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_DOLLAR_in_constant_species_def328 = new BitSet(new long[]{0x0000000000000000L,0x0000000000008000L});
    public static final BitSet FOLLOW_STRING_in_constant_species_def330 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_species_element_in_species_def345 = new BitSet(new long[]{0x0000000000000002L,0x0000000000800000L});
    public static final BitSet FOLLOW_DOT_in_species_def348 = new BitSet(new long[]{0x0000000000000000L,0x0000000010008000L});
    public static final BitSet FOLLOW_species_element_in_species_def350 = new BitSet(new long[]{0x0000000000000002L,0x0000000000800000L});
    public static final BitSet FOLLOW_STRING_in_species_element362 = new BitSet(new long[]{0x0000000000000004L});
    public static final BitSet FOLLOW_site_list_in_species_element364 = new BitSet(new long[]{0x0000000000000008L});
    public static final BitSet FOLLOW_site_obs_in_site_list376 = new BitSet(new long[]{0x0000000000000002L,0x0000000000400000L});
    public static final BitSet FOLLOW_COMMA_in_site_list379 = new BitSet(new long[]{0x0000000000000000L,0x0000000000008000L});
    public static final BitSet FOLLOW_site_obs_in_site_list381 = new BitSet(new long[]{0x0000000000000002L,0x0000000000400000L});
    public static final BitSet FOLLOW_STRING_in_site_obs398 = new BitSet(new long[]{0x0000000000000002L,0x0000200020000000L});
    public static final BitSet FOLLOW_TILDE_in_site_obs403 = new BitSet(new long[]{0x0000000000000008L,0x0000000000008000L});
    public static final BitSet FOLLOW_state_label_in_site_obs405 = new BitSet(new long[]{0x0000000000000002L,0x0000200000000000L});
    public static final BitSet FOLLOW_EMARK_in_site_obs410 = new BitSet(new long[]{0x0000000000000000L,0x000012000000A000L});
    public static final BitSet FOLLOW_PLUS_in_site_obs413 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_QMARK_in_site_obs415 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_bond_name_in_site_obs417 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_INT_in_bond_name427 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_STRING_in_bond_name442 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_REACTION_in_reaction_rules_block450 = new BitSet(new long[]{0x0000000000000100L});
    public static final BitSet FOLLOW_reaction_rule_def_in_reaction_rules_block452 = new BitSet(new long[]{0x0000000000000102L});
    public static final BitSet FOLLOW_BEGIN_in_reaction_rule_def464 = new BitSet(new long[]{0x0000000000000082L});
    public static final BitSet FOLLOW_modif_command_in_reaction_rule_def467 = new BitSet(new long[]{0x0000000000000082L});
    public static final BitSet FOLLOW_END_in_modif_command477 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_number_in_primitive_element518 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_variable_in_primitive_element522 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_set_in_number0 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_STRING_in_variable540 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_END_in_actions_block551 = new BitSet(new long[]{0x0000000000000002L});

}