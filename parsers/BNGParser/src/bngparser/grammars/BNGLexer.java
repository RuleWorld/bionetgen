// $ANTLR 3.3 Nov 30, 2010 12:50:56 /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g 2013-01-14 18:13:01

package bngparser.grammars;


import org.antlr.runtime.*;
import java.util.Stack;
import java.util.List;
import java.util.ArrayList;

public class BNGLexer extends Lexer {
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

     public boolean floatLA(){
        int counter = 1;
        while(true){
          int LA8_0 = input.LT(counter);
          if ((LA8_0>='0' && LA8_0<='9')){
            counter++;
          }
          else{
            break;
          }
        }
        int LA14_0 = input.LT(counter);
        if((LA14_0>='A' && LA14_0<='Z')||LA14_0=='_'||(LA14_0>='a' && LA14_0<='z'))
          return false;
        return true;
     }


    // delegates
    // delegators

    public BNGLexer() {;} 
    public BNGLexer(CharStream input) {
        this(input, new RecognizerSharedState());
    }
    public BNGLexer(CharStream input, RecognizerSharedState state) {
        super(input,state);

    }
    public String getGrammarFileName() { return "/home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g"; }

    // $ANTLR start "LINE_COMMENT"
    public final void mLINE_COMMENT() throws RecognitionException {
        try {
            int _type = LINE_COMMENT;
            int _channel = DEFAULT_TOKEN_CHANNEL;
            // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:30:14: ( '#' (~ ( '\\n' | '\\r' ) )* )
            // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:30:16: '#' (~ ( '\\n' | '\\r' ) )*
            {
            match('#'); 
            // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:30:20: (~ ( '\\n' | '\\r' ) )*
            loop1:
            do {
                int alt1=2;
                int LA1_0 = input.LA(1);

                if ( ((LA1_0>='\u0000' && LA1_0<='\t')||(LA1_0>='\u000B' && LA1_0<='\f')||(LA1_0>='\u000E' && LA1_0<='\uFFFF')) ) {
                    alt1=1;
                }


                switch (alt1) {
            	case 1 :
            	    // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:30:20: ~ ( '\\n' | '\\r' )
            	    {
            	    if ( (input.LA(1)>='\u0000' && input.LA(1)<='\t')||(input.LA(1)>='\u000B' && input.LA(1)<='\f')||(input.LA(1)>='\u000E' && input.LA(1)<='\uFFFF') ) {
            	        input.consume();

            	    }
            	    else {
            	        MismatchedSetException mse = new MismatchedSetException(null,input);
            	        recover(mse);
            	        throw mse;}


            	    }
            	    break;

            	default :
            	    break loop1;
                }
            } while (true);

            skip();

            }

            state.type = _type;
            state.channel = _channel;
        }
        finally {
        }
    }
    // $ANTLR end "LINE_COMMENT"

    // $ANTLR start "LB"
    public final void mLB() throws RecognitionException {
        try {
            int _type = LB;
            int _channel = DEFAULT_TOKEN_CHANNEL;
            // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:31:4: ( ( '\\n' | '\\r' )+ )
            // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:31:6: ( '\\n' | '\\r' )+
            {
            // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:31:6: ( '\\n' | '\\r' )+
            int cnt2=0;
            loop2:
            do {
                int alt2=2;
                int LA2_0 = input.LA(1);

                if ( (LA2_0=='\n'||LA2_0=='\r') ) {
                    alt2=1;
                }


                switch (alt2) {
            	case 1 :
            	    // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:
            	    {
            	    if ( input.LA(1)=='\n'||input.LA(1)=='\r' ) {
            	        input.consume();

            	    }
            	    else {
            	        MismatchedSetException mse = new MismatchedSetException(null,input);
            	        recover(mse);
            	        throw mse;}


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

            state.type = _type;
            state.channel = _channel;
        }
        finally {
        }
    }
    // $ANTLR end "LB"

    // $ANTLR start "WS"
    public final void mWS() throws RecognitionException {
        try {
            int _type = WS;
            int _channel = DEFAULT_TOKEN_CHANNEL;
            // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:32:4: ( ( ' ' | '\\t' | ( '\\\\' ) ( ' ' )* ( '\\r' )? ( '\\n' ) )+ )
            // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:32:6: ( ' ' | '\\t' | ( '\\\\' ) ( ' ' )* ( '\\r' )? ( '\\n' ) )+
            {
            // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:32:6: ( ' ' | '\\t' | ( '\\\\' ) ( ' ' )* ( '\\r' )? ( '\\n' ) )+
            int cnt5=0;
            loop5:
            do {
                int alt5=4;
                switch ( input.LA(1) ) {
                case ' ':
                    {
                    alt5=1;
                    }
                    break;
                case '\t':
                    {
                    alt5=2;
                    }
                    break;
                case '\\':
                    {
                    alt5=3;
                    }
                    break;

                }

                switch (alt5) {
            	case 1 :
            	    // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:32:7: ' '
            	    {
            	    match(' '); 

            	    }
            	    break;
            	case 2 :
            	    // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:32:11: '\\t'
            	    {
            	    match('\t'); 

            	    }
            	    break;
            	case 3 :
            	    // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:32:16: ( '\\\\' ) ( ' ' )* ( '\\r' )? ( '\\n' )
            	    {
            	    // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:32:16: ( '\\\\' )
            	    // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:32:17: '\\\\'
            	    {
            	    match('\\'); 

            	    }

            	    // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:32:22: ( ' ' )*
            	    loop3:
            	    do {
            	        int alt3=2;
            	        int LA3_0 = input.LA(1);

            	        if ( (LA3_0==' ') ) {
            	            alt3=1;
            	        }


            	        switch (alt3) {
            	    	case 1 :
            	    	    // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:32:23: ' '
            	    	    {
            	    	    match(' '); 

            	    	    }
            	    	    break;

            	    	default :
            	    	    break loop3;
            	        }
            	    } while (true);

            	    // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:32:28: ( '\\r' )?
            	    int alt4=2;
            	    int LA4_0 = input.LA(1);

            	    if ( (LA4_0=='\r') ) {
            	        alt4=1;
            	    }
            	    switch (alt4) {
            	        case 1 :
            	            // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:32:29: '\\r'
            	            {
            	            match('\r'); 

            	            }
            	            break;

            	    }

            	    // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:32:35: ( '\\n' )
            	    // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:32:36: '\\n'
            	    {
            	    match('\n'); 

            	    }


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

             _channel=HIDDEN;

            }

            state.type = _type;
            state.channel = _channel;
        }
        finally {
        }
    }
    // $ANTLR end "WS"

    // $ANTLR start "VERSION"
    public final void mVERSION() throws RecognitionException {
        try {
            int _type = VERSION;
            int _channel = DEFAULT_TOKEN_CHANNEL;
            // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:33:9: ( ( 'v' | 'V' ) ( 'e' | 'E' ) ( 'r' | 'R' ) ( 's' | 'S' ) ( 'i' | 'I' ) ( 'o' | 'O' ) ( 'n' | 'N' ) )
            // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:33:11: ( 'v' | 'V' ) ( 'e' | 'E' ) ( 'r' | 'R' ) ( 's' | 'S' ) ( 'i' | 'I' ) ( 'o' | 'O' ) ( 'n' | 'N' )
            {
            if ( input.LA(1)=='V'||input.LA(1)=='v' ) {
                input.consume();

            }
            else {
                MismatchedSetException mse = new MismatchedSetException(null,input);
                recover(mse);
                throw mse;}

            if ( input.LA(1)=='E'||input.LA(1)=='e' ) {
                input.consume();

            }
            else {
                MismatchedSetException mse = new MismatchedSetException(null,input);
                recover(mse);
                throw mse;}

            if ( input.LA(1)=='R'||input.LA(1)=='r' ) {
                input.consume();

            }
            else {
                MismatchedSetException mse = new MismatchedSetException(null,input);
                recover(mse);
                throw mse;}

            if ( input.LA(1)=='S'||input.LA(1)=='s' ) {
                input.consume();

            }
            else {
                MismatchedSetException mse = new MismatchedSetException(null,input);
                recover(mse);
                throw mse;}

            if ( input.LA(1)=='I'||input.LA(1)=='i' ) {
                input.consume();

            }
            else {
                MismatchedSetException mse = new MismatchedSetException(null,input);
                recover(mse);
                throw mse;}

            if ( input.LA(1)=='O'||input.LA(1)=='o' ) {
                input.consume();

            }
            else {
                MismatchedSetException mse = new MismatchedSetException(null,input);
                recover(mse);
                throw mse;}

            if ( input.LA(1)=='N'||input.LA(1)=='n' ) {
                input.consume();

            }
            else {
                MismatchedSetException mse = new MismatchedSetException(null,input);
                recover(mse);
                throw mse;}


            }

            state.type = _type;
            state.channel = _channel;
        }
        finally {
        }
    }
    // $ANTLR end "VERSION"

    // $ANTLR start "END"
    public final void mEND() throws RecognitionException {
        try {
            int _type = END;
            int _channel = DEFAULT_TOKEN_CHANNEL;
            // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:34:5: ( ( 'e' | 'E' ) ( 'n' | 'N' ) ( 'd' | 'D' ) )
            // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:34:7: ( 'e' | 'E' ) ( 'n' | 'N' ) ( 'd' | 'D' )
            {
            if ( input.LA(1)=='E'||input.LA(1)=='e' ) {
                input.consume();

            }
            else {
                MismatchedSetException mse = new MismatchedSetException(null,input);
                recover(mse);
                throw mse;}

            if ( input.LA(1)=='N'||input.LA(1)=='n' ) {
                input.consume();

            }
            else {
                MismatchedSetException mse = new MismatchedSetException(null,input);
                recover(mse);
                throw mse;}

            if ( input.LA(1)=='D'||input.LA(1)=='d' ) {
                input.consume();

            }
            else {
                MismatchedSetException mse = new MismatchedSetException(null,input);
                recover(mse);
                throw mse;}


            }

            state.type = _type;
            state.channel = _channel;
        }
        finally {
        }
    }
    // $ANTLR end "END"

    // $ANTLR start "BEGIN"
    public final void mBEGIN() throws RecognitionException {
        try {
            int _type = BEGIN;
            int _channel = DEFAULT_TOKEN_CHANNEL;
            // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:35:7: ( ( 'b' | 'B' ) ( 'e' | 'E' ) ( 'g' | 'G' ) ( 'i' | 'I' ) ( 'n' | 'N' ) )
            // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:35:9: ( 'b' | 'B' ) ( 'e' | 'E' ) ( 'g' | 'G' ) ( 'i' | 'I' ) ( 'n' | 'N' )
            {
            if ( input.LA(1)=='B'||input.LA(1)=='b' ) {
                input.consume();

            }
            else {
                MismatchedSetException mse = new MismatchedSetException(null,input);
                recover(mse);
                throw mse;}

            if ( input.LA(1)=='E'||input.LA(1)=='e' ) {
                input.consume();

            }
            else {
                MismatchedSetException mse = new MismatchedSetException(null,input);
                recover(mse);
                throw mse;}

            if ( input.LA(1)=='G'||input.LA(1)=='g' ) {
                input.consume();

            }
            else {
                MismatchedSetException mse = new MismatchedSetException(null,input);
                recover(mse);
                throw mse;}

            if ( input.LA(1)=='I'||input.LA(1)=='i' ) {
                input.consume();

            }
            else {
                MismatchedSetException mse = new MismatchedSetException(null,input);
                recover(mse);
                throw mse;}

            if ( input.LA(1)=='N'||input.LA(1)=='n' ) {
                input.consume();

            }
            else {
                MismatchedSetException mse = new MismatchedSetException(null,input);
                recover(mse);
                throw mse;}


            }

            state.type = _type;
            state.channel = _channel;
        }
        finally {
        }
    }
    // $ANTLR end "BEGIN"

    // $ANTLR start "SEED"
    public final void mSEED() throws RecognitionException {
        try {
            int _type = SEED;
            int _channel = DEFAULT_TOKEN_CHANNEL;
            // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:36:6: ( ( 's' | 'S' ) ( 'e' | 'E' ) ( 'e' | 'E' ) ( 'd' | 'D' ) )
            // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:36:8: ( 's' | 'S' ) ( 'e' | 'E' ) ( 'e' | 'E' ) ( 'd' | 'D' )
            {
            if ( input.LA(1)=='S'||input.LA(1)=='s' ) {
                input.consume();

            }
            else {
                MismatchedSetException mse = new MismatchedSetException(null,input);
                recover(mse);
                throw mse;}

            if ( input.LA(1)=='E'||input.LA(1)=='e' ) {
                input.consume();

            }
            else {
                MismatchedSetException mse = new MismatchedSetException(null,input);
                recover(mse);
                throw mse;}

            if ( input.LA(1)=='E'||input.LA(1)=='e' ) {
                input.consume();

            }
            else {
                MismatchedSetException mse = new MismatchedSetException(null,input);
                recover(mse);
                throw mse;}

            if ( input.LA(1)=='D'||input.LA(1)=='d' ) {
                input.consume();

            }
            else {
                MismatchedSetException mse = new MismatchedSetException(null,input);
                recover(mse);
                throw mse;}


            }

            state.type = _type;
            state.channel = _channel;
        }
        finally {
        }
    }
    // $ANTLR end "SEED"

    // $ANTLR start "SPECIES"
    public final void mSPECIES() throws RecognitionException {
        try {
            int _type = SPECIES;
            int _channel = DEFAULT_TOKEN_CHANNEL;
            // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:37:9: ( ( 's' | 'S' ) ( 'p' | 'P' ) ( 'e' | 'E' ) ( 'c' | 'C' ) ( 'i' | 'I' ) ( 'e' | 'E' ) ( 's' | 'S' ) )
            // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:37:11: ( 's' | 'S' ) ( 'p' | 'P' ) ( 'e' | 'E' ) ( 'c' | 'C' ) ( 'i' | 'I' ) ( 'e' | 'E' ) ( 's' | 'S' )
            {
            if ( input.LA(1)=='S'||input.LA(1)=='s' ) {
                input.consume();

            }
            else {
                MismatchedSetException mse = new MismatchedSetException(null,input);
                recover(mse);
                throw mse;}

            if ( input.LA(1)=='P'||input.LA(1)=='p' ) {
                input.consume();

            }
            else {
                MismatchedSetException mse = new MismatchedSetException(null,input);
                recover(mse);
                throw mse;}

            if ( input.LA(1)=='E'||input.LA(1)=='e' ) {
                input.consume();

            }
            else {
                MismatchedSetException mse = new MismatchedSetException(null,input);
                recover(mse);
                throw mse;}

            if ( input.LA(1)=='C'||input.LA(1)=='c' ) {
                input.consume();

            }
            else {
                MismatchedSetException mse = new MismatchedSetException(null,input);
                recover(mse);
                throw mse;}

            if ( input.LA(1)=='I'||input.LA(1)=='i' ) {
                input.consume();

            }
            else {
                MismatchedSetException mse = new MismatchedSetException(null,input);
                recover(mse);
                throw mse;}

            if ( input.LA(1)=='E'||input.LA(1)=='e' ) {
                input.consume();

            }
            else {
                MismatchedSetException mse = new MismatchedSetException(null,input);
                recover(mse);
                throw mse;}

            if ( input.LA(1)=='S'||input.LA(1)=='s' ) {
                input.consume();

            }
            else {
                MismatchedSetException mse = new MismatchedSetException(null,input);
                recover(mse);
                throw mse;}


            }

            state.type = _type;
            state.channel = _channel;
        }
        finally {
        }
    }
    // $ANTLR end "SPECIES"

    // $ANTLR start "GROUPS"
    public final void mGROUPS() throws RecognitionException {
        try {
            int _type = GROUPS;
            int _channel = DEFAULT_TOKEN_CHANNEL;
            // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:38:8: ( ( 'G' | 'g' ) ( 'R' | 'r' ) ( 'O' | 'o' ) ( 'U' | 'u' ) ( 'P' | 'p' ) ( 'S' | 's' ) )
            // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:38:10: ( 'G' | 'g' ) ( 'R' | 'r' ) ( 'O' | 'o' ) ( 'U' | 'u' ) ( 'P' | 'p' ) ( 'S' | 's' )
            {
            if ( input.LA(1)=='G'||input.LA(1)=='g' ) {
                input.consume();

            }
            else {
                MismatchedSetException mse = new MismatchedSetException(null,input);
                recover(mse);
                throw mse;}

            if ( input.LA(1)=='R'||input.LA(1)=='r' ) {
                input.consume();

            }
            else {
                MismatchedSetException mse = new MismatchedSetException(null,input);
                recover(mse);
                throw mse;}

            if ( input.LA(1)=='O'||input.LA(1)=='o' ) {
                input.consume();

            }
            else {
                MismatchedSetException mse = new MismatchedSetException(null,input);
                recover(mse);
                throw mse;}

            if ( input.LA(1)=='U'||input.LA(1)=='u' ) {
                input.consume();

            }
            else {
                MismatchedSetException mse = new MismatchedSetException(null,input);
                recover(mse);
                throw mse;}

            if ( input.LA(1)=='P'||input.LA(1)=='p' ) {
                input.consume();

            }
            else {
                MismatchedSetException mse = new MismatchedSetException(null,input);
                recover(mse);
                throw mse;}

            if ( input.LA(1)=='S'||input.LA(1)=='s' ) {
                input.consume();

            }
            else {
                MismatchedSetException mse = new MismatchedSetException(null,input);
                recover(mse);
                throw mse;}


            }

            state.type = _type;
            state.channel = _channel;
        }
        finally {
        }
    }
    // $ANTLR end "GROUPS"

    // $ANTLR start "MOLECULE"
    public final void mMOLECULE() throws RecognitionException {
        try {
            int _type = MOLECULE;
            int _channel = DEFAULT_TOKEN_CHANNEL;
            // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:39:10: ( ( 'm' | 'M' ) ( 'o' | 'O' ) ( 'l' | 'L' ) ( 'e' | 'E' ) ( 'c' | 'C' ) ( 'u' | 'l' ) ( 'l' | 'L' ) ( 'e' | 'E' ) )
            // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:39:12: ( 'm' | 'M' ) ( 'o' | 'O' ) ( 'l' | 'L' ) ( 'e' | 'E' ) ( 'c' | 'C' ) ( 'u' | 'l' ) ( 'l' | 'L' ) ( 'e' | 'E' )
            {
            if ( input.LA(1)=='M'||input.LA(1)=='m' ) {
                input.consume();

            }
            else {
                MismatchedSetException mse = new MismatchedSetException(null,input);
                recover(mse);
                throw mse;}

            if ( input.LA(1)=='O'||input.LA(1)=='o' ) {
                input.consume();

            }
            else {
                MismatchedSetException mse = new MismatchedSetException(null,input);
                recover(mse);
                throw mse;}

            if ( input.LA(1)=='L'||input.LA(1)=='l' ) {
                input.consume();

            }
            else {
                MismatchedSetException mse = new MismatchedSetException(null,input);
                recover(mse);
                throw mse;}

            if ( input.LA(1)=='E'||input.LA(1)=='e' ) {
                input.consume();

            }
            else {
                MismatchedSetException mse = new MismatchedSetException(null,input);
                recover(mse);
                throw mse;}

            if ( input.LA(1)=='C'||input.LA(1)=='c' ) {
                input.consume();

            }
            else {
                MismatchedSetException mse = new MismatchedSetException(null,input);
                recover(mse);
                throw mse;}

            if ( input.LA(1)=='l'||input.LA(1)=='u' ) {
                input.consume();

            }
            else {
                MismatchedSetException mse = new MismatchedSetException(null,input);
                recover(mse);
                throw mse;}

            if ( input.LA(1)=='L'||input.LA(1)=='l' ) {
                input.consume();

            }
            else {
                MismatchedSetException mse = new MismatchedSetException(null,input);
                recover(mse);
                throw mse;}

            if ( input.LA(1)=='E'||input.LA(1)=='e' ) {
                input.consume();

            }
            else {
                MismatchedSetException mse = new MismatchedSetException(null,input);
                recover(mse);
                throw mse;}


            }

            state.type = _type;
            state.channel = _channel;
        }
        finally {
        }
    }
    // $ANTLR end "MOLECULE"

    // $ANTLR start "MOLECULES"
    public final void mMOLECULES() throws RecognitionException {
        try {
            int _type = MOLECULES;
            int _channel = DEFAULT_TOKEN_CHANNEL;
            // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:40:11: ( MOLECULE ( 's' | 'S' ) )
            // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:40:13: MOLECULE ( 's' | 'S' )
            {
            mMOLECULE(); 
            if ( input.LA(1)=='S'||input.LA(1)=='s' ) {
                input.consume();

            }
            else {
                MismatchedSetException mse = new MismatchedSetException(null,input);
                recover(mse);
                throw mse;}


            }

            state.type = _type;
            state.channel = _channel;
        }
        finally {
        }
    }
    // $ANTLR end "MOLECULES"

    // $ANTLR start "MODEL"
    public final void mMODEL() throws RecognitionException {
        try {
            int _type = MODEL;
            int _channel = DEFAULT_TOKEN_CHANNEL;
            // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:41:6: ( ( 'm' | 'M' ) ( 'o' | 'O' ) ( 'd' | 'D' ) ( 'e' | 'E' ) ( 'l' | 'L' ) )
            // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:41:8: ( 'm' | 'M' ) ( 'o' | 'O' ) ( 'd' | 'D' ) ( 'e' | 'E' ) ( 'l' | 'L' )
            {
            if ( input.LA(1)=='M'||input.LA(1)=='m' ) {
                input.consume();

            }
            else {
                MismatchedSetException mse = new MismatchedSetException(null,input);
                recover(mse);
                throw mse;}

            if ( input.LA(1)=='O'||input.LA(1)=='o' ) {
                input.consume();

            }
            else {
                MismatchedSetException mse = new MismatchedSetException(null,input);
                recover(mse);
                throw mse;}

            if ( input.LA(1)=='D'||input.LA(1)=='d' ) {
                input.consume();

            }
            else {
                MismatchedSetException mse = new MismatchedSetException(null,input);
                recover(mse);
                throw mse;}

            if ( input.LA(1)=='E'||input.LA(1)=='e' ) {
                input.consume();

            }
            else {
                MismatchedSetException mse = new MismatchedSetException(null,input);
                recover(mse);
                throw mse;}

            if ( input.LA(1)=='L'||input.LA(1)=='l' ) {
                input.consume();

            }
            else {
                MismatchedSetException mse = new MismatchedSetException(null,input);
                recover(mse);
                throw mse;}


            }

            state.type = _type;
            state.channel = _channel;
        }
        finally {
        }
    }
    // $ANTLR end "MODEL"

    // $ANTLR start "TYPES"
    public final void mTYPES() throws RecognitionException {
        try {
            int _type = TYPES;
            int _channel = DEFAULT_TOKEN_CHANNEL;
            // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:42:7: ( ( 't' | 'T' ) ( 'y' | 'Y' ) ( 'p' | 'P' ) ( 'e' | 'E' ) ( 's' | 'S' ) )
            // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:42:9: ( 't' | 'T' ) ( 'y' | 'Y' ) ( 'p' | 'P' ) ( 'e' | 'E' ) ( 's' | 'S' )
            {
            if ( input.LA(1)=='T'||input.LA(1)=='t' ) {
                input.consume();

            }
            else {
                MismatchedSetException mse = new MismatchedSetException(null,input);
                recover(mse);
                throw mse;}

            if ( input.LA(1)=='Y'||input.LA(1)=='y' ) {
                input.consume();

            }
            else {
                MismatchedSetException mse = new MismatchedSetException(null,input);
                recover(mse);
                throw mse;}

            if ( input.LA(1)=='P'||input.LA(1)=='p' ) {
                input.consume();

            }
            else {
                MismatchedSetException mse = new MismatchedSetException(null,input);
                recover(mse);
                throw mse;}

            if ( input.LA(1)=='E'||input.LA(1)=='e' ) {
                input.consume();

            }
            else {
                MismatchedSetException mse = new MismatchedSetException(null,input);
                recover(mse);
                throw mse;}

            if ( input.LA(1)=='S'||input.LA(1)=='s' ) {
                input.consume();

            }
            else {
                MismatchedSetException mse = new MismatchedSetException(null,input);
                recover(mse);
                throw mse;}


            }

            state.type = _type;
            state.channel = _channel;
        }
        finally {
        }
    }
    // $ANTLR end "TYPES"

    // $ANTLR start "REACTION"
    public final void mREACTION() throws RecognitionException {
        try {
            int _type = REACTION;
            int _channel = DEFAULT_TOKEN_CHANNEL;
            // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:43:10: ( ( 'r' | 'R' ) ( 'e' | 'E' ) ( 'a' | 'A' ) ( 'c' | 'C' ) ( 't' | 'T' ) ( 'i' | 'I' ) ( 'o' | 'O' ) ( 'n' | 'N' ) )
            // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:43:12: ( 'r' | 'R' ) ( 'e' | 'E' ) ( 'a' | 'A' ) ( 'c' | 'C' ) ( 't' | 'T' ) ( 'i' | 'I' ) ( 'o' | 'O' ) ( 'n' | 'N' )
            {
            if ( input.LA(1)=='R'||input.LA(1)=='r' ) {
                input.consume();

            }
            else {
                MismatchedSetException mse = new MismatchedSetException(null,input);
                recover(mse);
                throw mse;}

            if ( input.LA(1)=='E'||input.LA(1)=='e' ) {
                input.consume();

            }
            else {
                MismatchedSetException mse = new MismatchedSetException(null,input);
                recover(mse);
                throw mse;}

            if ( input.LA(1)=='A'||input.LA(1)=='a' ) {
                input.consume();

            }
            else {
                MismatchedSetException mse = new MismatchedSetException(null,input);
                recover(mse);
                throw mse;}

            if ( input.LA(1)=='C'||input.LA(1)=='c' ) {
                input.consume();

            }
            else {
                MismatchedSetException mse = new MismatchedSetException(null,input);
                recover(mse);
                throw mse;}

            if ( input.LA(1)=='T'||input.LA(1)=='t' ) {
                input.consume();

            }
            else {
                MismatchedSetException mse = new MismatchedSetException(null,input);
                recover(mse);
                throw mse;}

            if ( input.LA(1)=='I'||input.LA(1)=='i' ) {
                input.consume();

            }
            else {
                MismatchedSetException mse = new MismatchedSetException(null,input);
                recover(mse);
                throw mse;}

            if ( input.LA(1)=='O'||input.LA(1)=='o' ) {
                input.consume();

            }
            else {
                MismatchedSetException mse = new MismatchedSetException(null,input);
                recover(mse);
                throw mse;}

            if ( input.LA(1)=='N'||input.LA(1)=='n' ) {
                input.consume();

            }
            else {
                MismatchedSetException mse = new MismatchedSetException(null,input);
                recover(mse);
                throw mse;}


            }

            state.type = _type;
            state.channel = _channel;
        }
        finally {
        }
    }
    // $ANTLR end "REACTION"

    // $ANTLR start "REACTIONS"
    public final void mREACTIONS() throws RecognitionException {
        try {
            int _type = REACTIONS;
            int _channel = DEFAULT_TOKEN_CHANNEL;
            // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:44:11: ( REACTION ( 's' | 'S' ) )
            // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:44:13: REACTION ( 's' | 'S' )
            {
            mREACTION(); 
            if ( input.LA(1)=='S'||input.LA(1)=='s' ) {
                input.consume();

            }
            else {
                MismatchedSetException mse = new MismatchedSetException(null,input);
                recover(mse);
                throw mse;}


            }

            state.type = _type;
            state.channel = _channel;
        }
        finally {
        }
    }
    // $ANTLR end "REACTIONS"

    // $ANTLR start "RULES"
    public final void mRULES() throws RecognitionException {
        try {
            int _type = RULES;
            int _channel = DEFAULT_TOKEN_CHANNEL;
            // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:45:7: ( ( 'r' | 'R' ) ( 'u' | 'U' ) ( 'l' | 'L' ) ( 'e' | 'E' ) ( 's' | 'S' ) )
            // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:45:9: ( 'r' | 'R' ) ( 'u' | 'U' ) ( 'l' | 'L' ) ( 'e' | 'E' ) ( 's' | 'S' )
            {
            if ( input.LA(1)=='R'||input.LA(1)=='r' ) {
                input.consume();

            }
            else {
                MismatchedSetException mse = new MismatchedSetException(null,input);
                recover(mse);
                throw mse;}

            if ( input.LA(1)=='U'||input.LA(1)=='u' ) {
                input.consume();

            }
            else {
                MismatchedSetException mse = new MismatchedSetException(null,input);
                recover(mse);
                throw mse;}

            if ( input.LA(1)=='L'||input.LA(1)=='l' ) {
                input.consume();

            }
            else {
                MismatchedSetException mse = new MismatchedSetException(null,input);
                recover(mse);
                throw mse;}

            if ( input.LA(1)=='E'||input.LA(1)=='e' ) {
                input.consume();

            }
            else {
                MismatchedSetException mse = new MismatchedSetException(null,input);
                recover(mse);
                throw mse;}

            if ( input.LA(1)=='S'||input.LA(1)=='s' ) {
                input.consume();

            }
            else {
                MismatchedSetException mse = new MismatchedSetException(null,input);
                recover(mse);
                throw mse;}


            }

            state.type = _type;
            state.channel = _channel;
        }
        finally {
        }
    }
    // $ANTLR end "RULES"

    // $ANTLR start "FUNCTIONS"
    public final void mFUNCTIONS() throws RecognitionException {
        try {
            int _type = FUNCTIONS;
            int _channel = DEFAULT_TOKEN_CHANNEL;
            // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:46:11: ( ( 'F' | 'f' ) ( 'U' | 'u' ) ( 'N' | 'n' ) ( 'C' | 'c' ) ( 'T' | 't' ) ( 'I' | 'i' ) ( 'O' | 'o' ) ( 'n' | 'N' ) ( 'S' | 's' ) )
            // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:46:13: ( 'F' | 'f' ) ( 'U' | 'u' ) ( 'N' | 'n' ) ( 'C' | 'c' ) ( 'T' | 't' ) ( 'I' | 'i' ) ( 'O' | 'o' ) ( 'n' | 'N' ) ( 'S' | 's' )
            {
            if ( input.LA(1)=='F'||input.LA(1)=='f' ) {
                input.consume();

            }
            else {
                MismatchedSetException mse = new MismatchedSetException(null,input);
                recover(mse);
                throw mse;}

            if ( input.LA(1)=='U'||input.LA(1)=='u' ) {
                input.consume();

            }
            else {
                MismatchedSetException mse = new MismatchedSetException(null,input);
                recover(mse);
                throw mse;}

            if ( input.LA(1)=='N'||input.LA(1)=='n' ) {
                input.consume();

            }
            else {
                MismatchedSetException mse = new MismatchedSetException(null,input);
                recover(mse);
                throw mse;}

            if ( input.LA(1)=='C'||input.LA(1)=='c' ) {
                input.consume();

            }
            else {
                MismatchedSetException mse = new MismatchedSetException(null,input);
                recover(mse);
                throw mse;}

            if ( input.LA(1)=='T'||input.LA(1)=='t' ) {
                input.consume();

            }
            else {
                MismatchedSetException mse = new MismatchedSetException(null,input);
                recover(mse);
                throw mse;}

            if ( input.LA(1)=='I'||input.LA(1)=='i' ) {
                input.consume();

            }
            else {
                MismatchedSetException mse = new MismatchedSetException(null,input);
                recover(mse);
                throw mse;}

            if ( input.LA(1)=='O'||input.LA(1)=='o' ) {
                input.consume();

            }
            else {
                MismatchedSetException mse = new MismatchedSetException(null,input);
                recover(mse);
                throw mse;}

            if ( input.LA(1)=='N'||input.LA(1)=='n' ) {
                input.consume();

            }
            else {
                MismatchedSetException mse = new MismatchedSetException(null,input);
                recover(mse);
                throw mse;}

            if ( input.LA(1)=='S'||input.LA(1)=='s' ) {
                input.consume();

            }
            else {
                MismatchedSetException mse = new MismatchedSetException(null,input);
                recover(mse);
                throw mse;}


            }

            state.type = _type;
            state.channel = _channel;
        }
        finally {
        }
    }
    // $ANTLR end "FUNCTIONS"

    // $ANTLR start "COMPARTMENTS"
    public final void mCOMPARTMENTS() throws RecognitionException {
        try {
            int _type = COMPARTMENTS;
            int _channel = DEFAULT_TOKEN_CHANNEL;
            // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:47:14: ( ( 'C' | 'c' ) ( 'O' | 'o' ) ( 'M' | 'm' ) ( 'P' | 'p' ) ( 'A' | 'a' ) ( 'R' | 'r' ) ( 'T' | 't' ) ( 'M' | 'm' ) ( 'E' | 'e' ) ( 'N' | 'n' ) ( 'T' | 't' ) ( 'S' | 's' ) )
            // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:47:16: ( 'C' | 'c' ) ( 'O' | 'o' ) ( 'M' | 'm' ) ( 'P' | 'p' ) ( 'A' | 'a' ) ( 'R' | 'r' ) ( 'T' | 't' ) ( 'M' | 'm' ) ( 'E' | 'e' ) ( 'N' | 'n' ) ( 'T' | 't' ) ( 'S' | 's' )
            {
            if ( input.LA(1)=='C'||input.LA(1)=='c' ) {
                input.consume();

            }
            else {
                MismatchedSetException mse = new MismatchedSetException(null,input);
                recover(mse);
                throw mse;}

            if ( input.LA(1)=='O'||input.LA(1)=='o' ) {
                input.consume();

            }
            else {
                MismatchedSetException mse = new MismatchedSetException(null,input);
                recover(mse);
                throw mse;}

            if ( input.LA(1)=='M'||input.LA(1)=='m' ) {
                input.consume();

            }
            else {
                MismatchedSetException mse = new MismatchedSetException(null,input);
                recover(mse);
                throw mse;}

            if ( input.LA(1)=='P'||input.LA(1)=='p' ) {
                input.consume();

            }
            else {
                MismatchedSetException mse = new MismatchedSetException(null,input);
                recover(mse);
                throw mse;}

            if ( input.LA(1)=='A'||input.LA(1)=='a' ) {
                input.consume();

            }
            else {
                MismatchedSetException mse = new MismatchedSetException(null,input);
                recover(mse);
                throw mse;}

            if ( input.LA(1)=='R'||input.LA(1)=='r' ) {
                input.consume();

            }
            else {
                MismatchedSetException mse = new MismatchedSetException(null,input);
                recover(mse);
                throw mse;}

            if ( input.LA(1)=='T'||input.LA(1)=='t' ) {
                input.consume();

            }
            else {
                MismatchedSetException mse = new MismatchedSetException(null,input);
                recover(mse);
                throw mse;}

            if ( input.LA(1)=='M'||input.LA(1)=='m' ) {
                input.consume();

            }
            else {
                MismatchedSetException mse = new MismatchedSetException(null,input);
                recover(mse);
                throw mse;}

            if ( input.LA(1)=='E'||input.LA(1)=='e' ) {
                input.consume();

            }
            else {
                MismatchedSetException mse = new MismatchedSetException(null,input);
                recover(mse);
                throw mse;}

            if ( input.LA(1)=='N'||input.LA(1)=='n' ) {
                input.consume();

            }
            else {
                MismatchedSetException mse = new MismatchedSetException(null,input);
                recover(mse);
                throw mse;}

            if ( input.LA(1)=='T'||input.LA(1)=='t' ) {
                input.consume();

            }
            else {
                MismatchedSetException mse = new MismatchedSetException(null,input);
                recover(mse);
                throw mse;}

            if ( input.LA(1)=='S'||input.LA(1)=='s' ) {
                input.consume();

            }
            else {
                MismatchedSetException mse = new MismatchedSetException(null,input);
                recover(mse);
                throw mse;}


            }

            state.type = _type;
            state.channel = _channel;
        }
        finally {
        }
    }
    // $ANTLR end "COMPARTMENTS"

    // $ANTLR start "ACTIONS"
    public final void mACTIONS() throws RecognitionException {
        try {
            int _type = ACTIONS;
            int _channel = DEFAULT_TOKEN_CHANNEL;
            // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:48:8: ( ( 'A' | 'a' ) ( 'C' | 'c' ) ( 'T' | 't' ) ( 'i' | 'I' ) ( 'o' | 'O' ) ( 'n' | 'N' ) ( 'S' | 's' ) )
            // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:48:10: ( 'A' | 'a' ) ( 'C' | 'c' ) ( 'T' | 't' ) ( 'i' | 'I' ) ( 'o' | 'O' ) ( 'n' | 'N' ) ( 'S' | 's' )
            {
            if ( input.LA(1)=='A'||input.LA(1)=='a' ) {
                input.consume();

            }
            else {
                MismatchedSetException mse = new MismatchedSetException(null,input);
                recover(mse);
                throw mse;}

            if ( input.LA(1)=='C'||input.LA(1)=='c' ) {
                input.consume();

            }
            else {
                MismatchedSetException mse = new MismatchedSetException(null,input);
                recover(mse);
                throw mse;}

            if ( input.LA(1)=='T'||input.LA(1)=='t' ) {
                input.consume();

            }
            else {
                MismatchedSetException mse = new MismatchedSetException(null,input);
                recover(mse);
                throw mse;}

            if ( input.LA(1)=='I'||input.LA(1)=='i' ) {
                input.consume();

            }
            else {
                MismatchedSetException mse = new MismatchedSetException(null,input);
                recover(mse);
                throw mse;}

            if ( input.LA(1)=='O'||input.LA(1)=='o' ) {
                input.consume();

            }
            else {
                MismatchedSetException mse = new MismatchedSetException(null,input);
                recover(mse);
                throw mse;}

            if ( input.LA(1)=='N'||input.LA(1)=='n' ) {
                input.consume();

            }
            else {
                MismatchedSetException mse = new MismatchedSetException(null,input);
                recover(mse);
                throw mse;}

            if ( input.LA(1)=='S'||input.LA(1)=='s' ) {
                input.consume();

            }
            else {
                MismatchedSetException mse = new MismatchedSetException(null,input);
                recover(mse);
                throw mse;}


            }

            state.type = _type;
            state.channel = _channel;
        }
        finally {
        }
    }
    // $ANTLR end "ACTIONS"

    // $ANTLR start "OBSERVABLES"
    public final void mOBSERVABLES() throws RecognitionException {
        try {
            int _type = OBSERVABLES;
            int _channel = DEFAULT_TOKEN_CHANNEL;
            // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:49:12: ( 'observables' )
            // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:49:14: 'observables'
            {
            match("observables"); 


            }

            state.type = _type;
            state.channel = _channel;
        }
        finally {
        }
    }
    // $ANTLR end "OBSERVABLES"

    // $ANTLR start "POPULATION"
    public final void mPOPULATION() throws RecognitionException {
        try {
            int _type = POPULATION;
            int _channel = DEFAULT_TOKEN_CHANNEL;
            // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:50:11: ( 'population' )
            // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:50:13: 'population'
            {
            match("population"); 


            }

            state.type = _type;
            state.channel = _channel;
        }
        finally {
        }
    }
    // $ANTLR end "POPULATION"

    // $ANTLR start "MAPS"
    public final void mMAPS() throws RecognitionException {
        try {
            int _type = MAPS;
            int _channel = DEFAULT_TOKEN_CHANNEL;
            // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:51:5: ( 'maps' )
            // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:51:7: 'maps'
            {
            match("maps"); 


            }

            state.type = _type;
            state.channel = _channel;
        }
        finally {
        }
    }
    // $ANTLR end "MAPS"

    // $ANTLR start "SET_OPTION"
    public final void mSET_OPTION() throws RecognitionException {
        try {
            int _type = SET_OPTION;
            int _channel = DEFAULT_TOKEN_CHANNEL;
            // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:53:12: ( 'setOption' )
            // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:53:14: 'setOption'
            {
            match("setOption"); 


            }

            state.type = _type;
            state.channel = _channel;
        }
        finally {
        }
    }
    // $ANTLR end "SET_OPTION"

    // $ANTLR start "SPECIES_LABEL"
    public final void mSPECIES_LABEL() throws RecognitionException {
        try {
            int _type = SPECIES_LABEL;
            int _channel = DEFAULT_TOKEN_CHANNEL;
            // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:54:15: ( 'SPECIESLABEL' )
            // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:54:17: 'SPECIESLABEL'
            {
            match("SPECIESLABEL"); 


            }

            state.type = _type;
            state.channel = _channel;
        }
        finally {
        }
    }
    // $ANTLR end "SPECIES_LABEL"

    // $ANTLR start "GENERATE_NETWORK"
    public final void mGENERATE_NETWORK() throws RecognitionException {
        try {
            int _type = GENERATE_NETWORK;
            int _channel = DEFAULT_TOKEN_CHANNEL;
            // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:55:18: ( 'GENERATENETWORK' )
            // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:55:20: 'GENERATENETWORK'
            {
            match("GENERATENETWORK"); 


            }

            state.type = _type;
            state.channel = _channel;
        }
        finally {
        }
    }
    // $ANTLR end "GENERATE_NETWORK"

    // $ANTLR start "MAX_AGG"
    public final void mMAX_AGG() throws RecognitionException {
        try {
            int _type = MAX_AGG;
            int _channel = DEFAULT_TOKEN_CHANNEL;
            // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:56:9: ( 'max_agg' )
            // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:56:11: 'max_agg'
            {
            match("max_agg"); 


            }

            state.type = _type;
            state.channel = _channel;
        }
        finally {
        }
    }
    // $ANTLR end "MAX_AGG"

    // $ANTLR start "MAX_ITER"
    public final void mMAX_ITER() throws RecognitionException {
        try {
            int _type = MAX_ITER;
            int _channel = DEFAULT_TOKEN_CHANNEL;
            // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:57:10: ( ( 'M' | 'm' ) ( 'A' | 'a' ) ( 'X' | 'x' ) '_' ( 'I' | 'i' ) ( 'T' | 't' ) ( 'E' | 'e' ) ( 'R' | 'r' ) )
            // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:57:12: ( 'M' | 'm' ) ( 'A' | 'a' ) ( 'X' | 'x' ) '_' ( 'I' | 'i' ) ( 'T' | 't' ) ( 'E' | 'e' ) ( 'R' | 'r' )
            {
            if ( input.LA(1)=='M'||input.LA(1)=='m' ) {
                input.consume();

            }
            else {
                MismatchedSetException mse = new MismatchedSetException(null,input);
                recover(mse);
                throw mse;}

            if ( input.LA(1)=='A'||input.LA(1)=='a' ) {
                input.consume();

            }
            else {
                MismatchedSetException mse = new MismatchedSetException(null,input);
                recover(mse);
                throw mse;}

            if ( input.LA(1)=='X'||input.LA(1)=='x' ) {
                input.consume();

            }
            else {
                MismatchedSetException mse = new MismatchedSetException(null,input);
                recover(mse);
                throw mse;}

            match('_'); 
            if ( input.LA(1)=='I'||input.LA(1)=='i' ) {
                input.consume();

            }
            else {
                MismatchedSetException mse = new MismatchedSetException(null,input);
                recover(mse);
                throw mse;}

            if ( input.LA(1)=='T'||input.LA(1)=='t' ) {
                input.consume();

            }
            else {
                MismatchedSetException mse = new MismatchedSetException(null,input);
                recover(mse);
                throw mse;}

            if ( input.LA(1)=='E'||input.LA(1)=='e' ) {
                input.consume();

            }
            else {
                MismatchedSetException mse = new MismatchedSetException(null,input);
                recover(mse);
                throw mse;}

            if ( input.LA(1)=='R'||input.LA(1)=='r' ) {
                input.consume();

            }
            else {
                MismatchedSetException mse = new MismatchedSetException(null,input);
                recover(mse);
                throw mse;}


            }

            state.type = _type;
            state.channel = _channel;
        }
        finally {
        }
    }
    // $ANTLR end "MAX_ITER"

    // $ANTLR start "MAX_STOICH"
    public final void mMAX_STOICH() throws RecognitionException {
        try {
            int _type = MAX_STOICH;
            int _channel = DEFAULT_TOKEN_CHANNEL;
            // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:58:12: ( 'MAXSTOICH' )
            // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:58:14: 'MAXSTOICH'
            {
            match("MAXSTOICH"); 


            }

            state.type = _type;
            state.channel = _channel;
        }
        finally {
        }
    }
    // $ANTLR end "MAX_STOICH"

    // $ANTLR start "OVERWRITE"
    public final void mOVERWRITE() throws RecognitionException {
        try {
            int _type = OVERWRITE;
            int _channel = DEFAULT_TOKEN_CHANNEL;
            // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:59:10: ( 'overwrite' )
            // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:59:12: 'overwrite'
            {
            match("overwrite"); 


            }

            state.type = _type;
            state.channel = _channel;
        }
        finally {
        }
    }
    // $ANTLR end "OVERWRITE"

    // $ANTLR start "PRINT_ITER"
    public final void mPRINT_ITER() throws RecognitionException {
        try {
            int _type = PRINT_ITER;
            int _channel = DEFAULT_TOKEN_CHANNEL;
            // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:60:12: ( 'PRINTITER' )
            // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:60:14: 'PRINTITER'
            {
            match("PRINTITER"); 


            }

            state.type = _type;
            state.channel = _channel;
        }
        finally {
        }
    }
    // $ANTLR end "PRINT_ITER"

    // $ANTLR start "AND"
    public final void mAND() throws RecognitionException {
        try {
            int _type = AND;
            int _channel = DEFAULT_TOKEN_CHANNEL;
            // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:61:5: ( 'AND' )
            // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:61:7: 'AND'
            {
            match("AND"); 


            }

            state.type = _type;
            state.channel = _channel;
        }
        finally {
        }
    }
    // $ANTLR end "AND"

    // $ANTLR start "OR"
    public final void mOR() throws RecognitionException {
        try {
            int _type = OR;
            int _channel = DEFAULT_TOKEN_CHANNEL;
            // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:62:4: ( 'OR' )
            // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:62:6: 'OR'
            {
            match("OR"); 


            }

            state.type = _type;
            state.channel = _channel;
        }
        finally {
        }
    }
    // $ANTLR end "OR"

    // $ANTLR start "ID"
    public final void mID() throws RecognitionException {
        try {
            int _type = ID;
            int _channel = DEFAULT_TOKEN_CHANNEL;
            // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:63:4: ( 'ID' )
            // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:63:6: 'ID'
            {
            match("ID"); 


            }

            state.type = _type;
            state.channel = _channel;
        }
        finally {
        }
    }
    // $ANTLR end "ID"

    // $ANTLR start "IF"
    public final void mIF() throws RecognitionException {
        try {
            int _type = IF;
            int _channel = DEFAULT_TOKEN_CHANNEL;
            // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:64:4: ( 'if' )
            // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:64:6: 'if'
            {
            match("if"); 


            }

            state.type = _type;
            state.channel = _channel;
        }
        finally {
        }
    }
    // $ANTLR end "IF"

    // $ANTLR start "PREFFIX"
    public final void mPREFFIX() throws RecognitionException {
        try {
            int _type = PREFFIX;
            int _channel = DEFAULT_TOKEN_CHANNEL;
            // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:65:9: ( 'PREFFIX' )
            // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:65:11: 'PREFFIX'
            {
            match("PREFFIX"); 


            }

            state.type = _type;
            state.channel = _channel;
        }
        finally {
        }
    }
    // $ANTLR end "PREFFIX"

    // $ANTLR start "SUFFIX"
    public final void mSUFFIX() throws RecognitionException {
        try {
            int _type = SUFFIX;
            int _channel = DEFAULT_TOKEN_CHANNEL;
            // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:66:8: ( 'SUFFIX' | 'suffix' )
            int alt6=2;
            int LA6_0 = input.LA(1);

            if ( (LA6_0=='S') ) {
                alt6=1;
            }
            else if ( (LA6_0=='s') ) {
                alt6=2;
            }
            else {
                NoViableAltException nvae =
                    new NoViableAltException("", 6, 0, input);

                throw nvae;
            }
            switch (alt6) {
                case 1 :
                    // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:66:10: 'SUFFIX'
                    {
                    match("SUFFIX"); 


                    }
                    break;
                case 2 :
                    // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:66:19: 'suffix'
                    {
                    match("suffix"); 


                    }
                    break;

            }
            state.type = _type;
            state.channel = _channel;
        }
        finally {
        }
    }
    // $ANTLR end "SUFFIX"

    // $ANTLR start "LABEL"
    public final void mLABEL() throws RecognitionException {
        try {
            int _type = LABEL;
            int _channel = DEFAULT_TOKEN_CHANNEL;
            // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:67:7: ( 'LABEL' )
            // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:67:9: 'LABEL'
            {
            match("LABEL"); 


            }

            state.type = _type;
            state.channel = _channel;
        }
        finally {
        }
    }
    // $ANTLR end "LABEL"

    // $ANTLR start "PARAMETERS"
    public final void mPARAMETERS() throws RecognitionException {
        try {
            int _type = PARAMETERS;
            int _channel = DEFAULT_TOKEN_CHANNEL;
            // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:68:12: ( ( 'p' | 'P' ) ( 'a' | 'A' ) ( 'r' | 'R' ) ( 'a' | 'A' ) ( 'm' | 'M' ) ( 'e' | 'E' ) ( 't' | 'T' ) ( 'e' | 'E' ) ( 'r' | 'R' ) ( 's' | 'S' ) )
            // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:68:14: ( 'p' | 'P' ) ( 'a' | 'A' ) ( 'r' | 'R' ) ( 'a' | 'A' ) ( 'm' | 'M' ) ( 'e' | 'E' ) ( 't' | 'T' ) ( 'e' | 'E' ) ( 'r' | 'R' ) ( 's' | 'S' )
            {
            if ( input.LA(1)=='P'||input.LA(1)=='p' ) {
                input.consume();

            }
            else {
                MismatchedSetException mse = new MismatchedSetException(null,input);
                recover(mse);
                throw mse;}

            if ( input.LA(1)=='A'||input.LA(1)=='a' ) {
                input.consume();

            }
            else {
                MismatchedSetException mse = new MismatchedSetException(null,input);
                recover(mse);
                throw mse;}

            if ( input.LA(1)=='R'||input.LA(1)=='r' ) {
                input.consume();

            }
            else {
                MismatchedSetException mse = new MismatchedSetException(null,input);
                recover(mse);
                throw mse;}

            if ( input.LA(1)=='A'||input.LA(1)=='a' ) {
                input.consume();

            }
            else {
                MismatchedSetException mse = new MismatchedSetException(null,input);
                recover(mse);
                throw mse;}

            if ( input.LA(1)=='M'||input.LA(1)=='m' ) {
                input.consume();

            }
            else {
                MismatchedSetException mse = new MismatchedSetException(null,input);
                recover(mse);
                throw mse;}

            if ( input.LA(1)=='E'||input.LA(1)=='e' ) {
                input.consume();

            }
            else {
                MismatchedSetException mse = new MismatchedSetException(null,input);
                recover(mse);
                throw mse;}

            if ( input.LA(1)=='T'||input.LA(1)=='t' ) {
                input.consume();

            }
            else {
                MismatchedSetException mse = new MismatchedSetException(null,input);
                recover(mse);
                throw mse;}

            if ( input.LA(1)=='E'||input.LA(1)=='e' ) {
                input.consume();

            }
            else {
                MismatchedSetException mse = new MismatchedSetException(null,input);
                recover(mse);
                throw mse;}

            if ( input.LA(1)=='R'||input.LA(1)=='r' ) {
                input.consume();

            }
            else {
                MismatchedSetException mse = new MismatchedSetException(null,input);
                recover(mse);
                throw mse;}

            if ( input.LA(1)=='S'||input.LA(1)=='s' ) {
                input.consume();

            }
            else {
                MismatchedSetException mse = new MismatchedSetException(null,input);
                recover(mse);
                throw mse;}


            }

            state.type = _type;
            state.channel = _channel;
        }
        finally {
        }
    }
    // $ANTLR end "PARAMETERS"

    // $ANTLR start "VERBOSE"
    public final void mVERBOSE() throws RecognitionException {
        try {
            int _type = VERBOSE;
            int _channel = DEFAULT_TOKEN_CHANNEL;
            // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:69:9: ( ( 'V' | 'v' ) ( 'E' | 'e' ) ( 'R' | 'r' ) ( 'B' | 'b' ) ( 'O' | 'o' ) ( 'S' | 's' ) ( 'E ' | 'e' ) )
            // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:69:11: ( 'V' | 'v' ) ( 'E' | 'e' ) ( 'R' | 'r' ) ( 'B' | 'b' ) ( 'O' | 'o' ) ( 'S' | 's' ) ( 'E ' | 'e' )
            {
            if ( input.LA(1)=='V'||input.LA(1)=='v' ) {
                input.consume();

            }
            else {
                MismatchedSetException mse = new MismatchedSetException(null,input);
                recover(mse);
                throw mse;}

            if ( input.LA(1)=='E'||input.LA(1)=='e' ) {
                input.consume();

            }
            else {
                MismatchedSetException mse = new MismatchedSetException(null,input);
                recover(mse);
                throw mse;}

            if ( input.LA(1)=='R'||input.LA(1)=='r' ) {
                input.consume();

            }
            else {
                MismatchedSetException mse = new MismatchedSetException(null,input);
                recover(mse);
                throw mse;}

            if ( input.LA(1)=='B'||input.LA(1)=='b' ) {
                input.consume();

            }
            else {
                MismatchedSetException mse = new MismatchedSetException(null,input);
                recover(mse);
                throw mse;}

            if ( input.LA(1)=='O'||input.LA(1)=='o' ) {
                input.consume();

            }
            else {
                MismatchedSetException mse = new MismatchedSetException(null,input);
                recover(mse);
                throw mse;}

            if ( input.LA(1)=='S'||input.LA(1)=='s' ) {
                input.consume();

            }
            else {
                MismatchedSetException mse = new MismatchedSetException(null,input);
                recover(mse);
                throw mse;}

            // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:69:65: ( 'E ' | 'e' )
            int alt7=2;
            int LA7_0 = input.LA(1);

            if ( (LA7_0=='E') ) {
                alt7=1;
            }
            else if ( (LA7_0=='e') ) {
                alt7=2;
            }
            else {
                NoViableAltException nvae =
                    new NoViableAltException("", 7, 0, input);

                throw nvae;
            }
            switch (alt7) {
                case 1 :
                    // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:69:66: 'E '
                    {
                    match("E "); 


                    }
                    break;
                case 2 :
                    // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:69:71: 'e'
                    {
                    match('e'); 

                    }
                    break;

            }


            }

            state.type = _type;
            state.channel = _channel;
        }
        finally {
        }
    }
    // $ANTLR end "VERBOSE"

    // $ANTLR start "GML"
    public final void mGML() throws RecognitionException {
        try {
            int _type = GML;
            int _channel = DEFAULT_TOKEN_CHANNEL;
            // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:71:4: ( 'gml' )
            // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:71:6: 'gml'
            {
            match("gml"); 


            }

            state.type = _type;
            state.channel = _channel;
        }
        finally {
        }
    }
    // $ANTLR end "GML"

    // $ANTLR start "MATCHONCE"
    public final void mMATCHONCE() throws RecognitionException {
        try {
            int _type = MATCHONCE;
            int _channel = DEFAULT_TOKEN_CHANNEL;
            // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:72:10: ( 'MatchOnce' )
            // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:72:12: 'MatchOnce'
            {
            match("MatchOnce"); 


            }

            state.type = _type;
            state.channel = _channel;
        }
        finally {
        }
    }
    // $ANTLR end "MATCHONCE"

    // $ANTLR start "DELETEMOLECULES"
    public final void mDELETEMOLECULES() throws RecognitionException {
        try {
            int _type = DELETEMOLECULES;
            int _channel = DEFAULT_TOKEN_CHANNEL;
            // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:73:16: ( 'DeleteMolecules' )
            // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:73:18: 'DeleteMolecules'
            {
            match("DeleteMolecules"); 


            }

            state.type = _type;
            state.channel = _channel;
        }
        finally {
        }
    }
    // $ANTLR end "DELETEMOLECULES"

    // $ANTLR start "MOVECONNECTED"
    public final void mMOVECONNECTED() throws RecognitionException {
        try {
            int _type = MOVECONNECTED;
            int _channel = DEFAULT_TOKEN_CHANNEL;
            // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:74:14: ( 'MoveConnected' )
            // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:74:16: 'MoveConnected'
            {
            match("MoveConnected"); 


            }

            state.type = _type;
            state.channel = _channel;
        }
        finally {
        }
    }
    // $ANTLR end "MOVECONNECTED"

    // $ANTLR start "AUTO"
    public final void mAUTO() throws RecognitionException {
        try {
            int _type = AUTO;
            int _channel = DEFAULT_TOKEN_CHANNEL;
            // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:75:6: ( 'Auto' )
            // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:75:8: 'Auto'
            {
            match("Auto"); 


            }

            state.type = _type;
            state.channel = _channel;
        }
        finally {
        }
    }
    // $ANTLR end "AUTO"

    // $ANTLR start "HNAUTY"
    public final void mHNAUTY() throws RecognitionException {
        try {
            int _type = HNAUTY;
            int _channel = DEFAULT_TOKEN_CHANNEL;
            // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:76:8: ( 'Hnauty' )
            // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:76:10: 'Hnauty'
            {
            match("Hnauty"); 


            }

            state.type = _type;
            state.channel = _channel;
        }
        finally {
        }
    }
    // $ANTLR end "HNAUTY"

    // $ANTLR start "QUASY"
    public final void mQUASY() throws RecognitionException {
        try {
            int _type = QUASY;
            int _channel = DEFAULT_TOKEN_CHANNEL;
            // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:77:7: ( 'Quasy' )
            // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:77:9: 'Quasy'
            {
            match("Quasy"); 


            }

            state.type = _type;
            state.channel = _channel;
        }
        finally {
        }
    }
    // $ANTLR end "QUASY"

    // $ANTLR start "NOT"
    public final void mNOT() throws RecognitionException {
        try {
            int _type = NOT;
            int _channel = DEFAULT_TOKEN_CHANNEL;
            // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:78:4: ( 'not' )
            // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:78:6: 'not'
            {
            match("not"); 


            }

            state.type = _type;
            state.channel = _channel;
        }
        finally {
        }
    }
    // $ANTLR end "NOT"

    // $ANTLR start "INCLUDE_REACTANTS"
    public final void mINCLUDE_REACTANTS() throws RecognitionException {
        try {
            int _type = INCLUDE_REACTANTS;
            int _channel = DEFAULT_TOKEN_CHANNEL;
            // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:79:18: ( 'include_reactants' )
            // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:79:20: 'include_reactants'
            {
            match("include_reactants"); 


            }

            state.type = _type;
            state.channel = _channel;
        }
        finally {
        }
    }
    // $ANTLR end "INCLUDE_REACTANTS"

    // $ANTLR start "INCLUDE_PRODUCTS"
    public final void mINCLUDE_PRODUCTS() throws RecognitionException {
        try {
            int _type = INCLUDE_PRODUCTS;
            int _channel = DEFAULT_TOKEN_CHANNEL;
            // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:80:17: ( 'include_products' )
            // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:80:19: 'include_products'
            {
            match("include_products"); 


            }

            state.type = _type;
            state.channel = _channel;
        }
        finally {
        }
    }
    // $ANTLR end "INCLUDE_PRODUCTS"

    // $ANTLR start "EXCLUDE_REACTANTS"
    public final void mEXCLUDE_REACTANTS() throws RecognitionException {
        try {
            int _type = EXCLUDE_REACTANTS;
            int _channel = DEFAULT_TOKEN_CHANNEL;
            // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:81:18: ( 'exclude_reactants' )
            // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:81:20: 'exclude_reactants'
            {
            match("exclude_reactants"); 


            }

            state.type = _type;
            state.channel = _channel;
        }
        finally {
        }
    }
    // $ANTLR end "EXCLUDE_REACTANTS"

    // $ANTLR start "EXCLUDE_PRODUCTS"
    public final void mEXCLUDE_PRODUCTS() throws RecognitionException {
        try {
            int _type = EXCLUDE_PRODUCTS;
            int _channel = DEFAULT_TOKEN_CHANNEL;
            // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:82:17: ( 'exclude_products' )
            // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:82:19: 'exclude_products'
            {
            match("exclude_products"); 


            }

            state.type = _type;
            state.channel = _channel;
        }
        finally {
        }
    }
    // $ANTLR end "EXCLUDE_PRODUCTS"

    // $ANTLR start "GENERATEHYBRIDMODEL"
    public final void mGENERATEHYBRIDMODEL() throws RecognitionException {
        try {
            int _type = GENERATEHYBRIDMODEL;
            int _channel = DEFAULT_TOKEN_CHANNEL;
            // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:83:20: ( 'generate_hybrid_model' )
            // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:83:22: 'generate_hybrid_model'
            {
            match("generate_hybrid_model"); 


            }

            state.type = _type;
            state.channel = _channel;
        }
        finally {
        }
    }
    // $ANTLR end "GENERATEHYBRIDMODEL"

    // $ANTLR start "ATOL"
    public final void mATOL() throws RecognitionException {
        try {
            int _type = ATOL;
            int _channel = DEFAULT_TOKEN_CHANNEL;
            // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:84:5: ( 'atol' )
            // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:84:7: 'atol'
            {
            match("atol"); 


            }

            state.type = _type;
            state.channel = _channel;
        }
        finally {
        }
    }
    // $ANTLR end "ATOL"

    // $ANTLR start "RTOL"
    public final void mRTOL() throws RecognitionException {
        try {
            int _type = RTOL;
            int _channel = DEFAULT_TOKEN_CHANNEL;
            // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:85:5: ( 'rtol' )
            // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:85:7: 'rtol'
            {
            match("rtol"); 


            }

            state.type = _type;
            state.channel = _channel;
        }
        finally {
        }
    }
    // $ANTLR end "RTOL"

    // $ANTLR start "STEADY_STATE"
    public final void mSTEADY_STATE() throws RecognitionException {
        try {
            int _type = STEADY_STATE;
            int _channel = DEFAULT_TOKEN_CHANNEL;
            // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:86:13: ( 'steady_state' )
            // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:86:15: 'steady_state'
            {
            match("steady_state"); 


            }

            state.type = _type;
            state.channel = _channel;
        }
        finally {
        }
    }
    // $ANTLR end "STEADY_STATE"

    // $ANTLR start "SPARSE"
    public final void mSPARSE() throws RecognitionException {
        try {
            int _type = SPARSE;
            int _channel = DEFAULT_TOKEN_CHANNEL;
            // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:87:7: ( 'sparse' )
            // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:87:9: 'sparse'
            {
            match("sparse"); 


            }

            state.type = _type;
            state.channel = _channel;
        }
        finally {
        }
    }
    // $ANTLR end "SPARSE"

    // $ANTLR start "T_END"
    public final void mT_END() throws RecognitionException {
        try {
            int _type = T_END;
            int _channel = DEFAULT_TOKEN_CHANNEL;
            // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:88:6: ( 't_end' )
            // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:88:8: 't_end'
            {
            match("t_end"); 


            }

            state.type = _type;
            state.channel = _channel;
        }
        finally {
        }
    }
    // $ANTLR end "T_END"

    // $ANTLR start "T_START"
    public final void mT_START() throws RecognitionException {
        try {
            int _type = T_START;
            int _channel = DEFAULT_TOKEN_CHANNEL;
            // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:89:8: ( 't_start' )
            // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:89:10: 't_start'
            {
            match("t_start"); 


            }

            state.type = _type;
            state.channel = _channel;
        }
        finally {
        }
    }
    // $ANTLR end "T_START"

    // $ANTLR start "N_STEPS"
    public final void mN_STEPS() throws RecognitionException {
        try {
            int _type = N_STEPS;
            int _channel = DEFAULT_TOKEN_CHANNEL;
            // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:90:8: ( 'n_steps' )
            // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:90:10: 'n_steps'
            {
            match("n_steps"); 


            }

            state.type = _type;
            state.channel = _channel;
        }
        finally {
        }
    }
    // $ANTLR end "N_STEPS"

    // $ANTLR start "N_OUTPUT_STEPS"
    public final void mN_OUTPUT_STEPS() throws RecognitionException {
        try {
            int _type = N_OUTPUT_STEPS;
            int _channel = DEFAULT_TOKEN_CHANNEL;
            // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:91:15: ( 'n_output_steps' )
            // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:91:17: 'n_output_steps'
            {
            match("n_output_steps"); 


            }

            state.type = _type;
            state.channel = _channel;
        }
        finally {
        }
    }
    // $ANTLR end "N_OUTPUT_STEPS"

    // $ANTLR start "MAX_SIM_STEPS"
    public final void mMAX_SIM_STEPS() throws RecognitionException {
        try {
            int _type = MAX_SIM_STEPS;
            int _channel = DEFAULT_TOKEN_CHANNEL;
            // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:92:14: ( 'max_sim_steps' )
            // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:92:16: 'max_sim_steps'
            {
            match("max_sim_steps"); 


            }

            state.type = _type;
            state.channel = _channel;
        }
        finally {
        }
    }
    // $ANTLR end "MAX_SIM_STEPS"

    // $ANTLR start "OUTPUT_STEP_INTERVAL"
    public final void mOUTPUT_STEP_INTERVAL() throws RecognitionException {
        try {
            int _type = OUTPUT_STEP_INTERVAL;
            int _channel = DEFAULT_TOKEN_CHANNEL;
            // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:93:21: ( 'output_step_interval' )
            // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:93:23: 'output_step_interval'
            {
            match("output_step_interval"); 


            }

            state.type = _type;
            state.channel = _channel;
        }
        finally {
        }
    }
    // $ANTLR end "OUTPUT_STEP_INTERVAL"

    // $ANTLR start "PLA_CONFIG"
    public final void mPLA_CONFIG() throws RecognitionException {
        try {
            int _type = PLA_CONFIG;
            int _channel = DEFAULT_TOKEN_CHANNEL;
            // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:94:11: ( 'pla_config' )
            // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:94:13: 'pla_config'
            {
            match("pla_config"); 


            }

            state.type = _type;
            state.channel = _channel;
        }
        finally {
        }
    }
    // $ANTLR end "PLA_CONFIG"

    // $ANTLR start "SAMPLE_TIMES"
    public final void mSAMPLE_TIMES() throws RecognitionException {
        try {
            int _type = SAMPLE_TIMES;
            int _channel = DEFAULT_TOKEN_CHANNEL;
            // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:95:13: ( 'sample_times' )
            // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:95:15: 'sample_times'
            {
            match("sample_times"); 


            }

            state.type = _type;
            state.channel = _channel;
        }
        finally {
        }
    }
    // $ANTLR end "SAMPLE_TIMES"

    // $ANTLR start "SIMULATE_ODE"
    public final void mSIMULATE_ODE() throws RecognitionException {
        try {
            int _type = SIMULATE_ODE;
            int _channel = DEFAULT_TOKEN_CHANNEL;
            // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:96:13: ( 'simulate_ode' )
            // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:96:15: 'simulate_ode'
            {
            match("simulate_ode"); 


            }

            state.type = _type;
            state.channel = _channel;
        }
        finally {
        }
    }
    // $ANTLR end "SIMULATE_ODE"

    // $ANTLR start "SIMULATE_SSA"
    public final void mSIMULATE_SSA() throws RecognitionException {
        try {
            int _type = SIMULATE_SSA;
            int _channel = DEFAULT_TOKEN_CHANNEL;
            // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:97:13: ( 'simulate_ssa' )
            // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:97:15: 'simulate_ssa'
            {
            match("simulate_ssa"); 


            }

            state.type = _type;
            state.channel = _channel;
        }
        finally {
        }
    }
    // $ANTLR end "SIMULATE_SSA"

    // $ANTLR start "SIMULATE_NF"
    public final void mSIMULATE_NF() throws RecognitionException {
        try {
            int _type = SIMULATE_NF;
            int _channel = DEFAULT_TOKEN_CHANNEL;
            // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:98:12: ( 'simulate_nf' )
            // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:98:14: 'simulate_nf'
            {
            match("simulate_nf"); 


            }

            state.type = _type;
            state.channel = _channel;
        }
        finally {
        }
    }
    // $ANTLR end "SIMULATE_NF"

    // $ANTLR start "SIMULATE_PLA"
    public final void mSIMULATE_PLA() throws RecognitionException {
        try {
            int _type = SIMULATE_PLA;
            int _channel = DEFAULT_TOKEN_CHANNEL;
            // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:99:13: ( 'simulate_pla' )
            // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:99:15: 'simulate_pla'
            {
            match("simulate_pla"); 


            }

            state.type = _type;
            state.channel = _channel;
        }
        finally {
        }
    }
    // $ANTLR end "SIMULATE_PLA"

    // $ANTLR start "SIMULATE"
    public final void mSIMULATE() throws RecognitionException {
        try {
            int _type = SIMULATE;
            int _channel = DEFAULT_TOKEN_CHANNEL;
            // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:100:9: ( 'simulate' )
            // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:100:11: 'simulate'
            {
            match("simulate"); 


            }

            state.type = _type;
            state.channel = _channel;
        }
        finally {
        }
    }
    // $ANTLR end "SIMULATE"

    // $ANTLR start "READFILE"
    public final void mREADFILE() throws RecognitionException {
        try {
            int _type = READFILE;
            int _channel = DEFAULT_TOKEN_CHANNEL;
            // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:101:9: ( 'readFile' )
            // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:101:11: 'readFile'
            {
            match("readFile"); 


            }

            state.type = _type;
            state.channel = _channel;
        }
        finally {
        }
    }
    // $ANTLR end "READFILE"

    // $ANTLR start "FILE"
    public final void mFILE() throws RecognitionException {
        try {
            int _type = FILE;
            int _channel = DEFAULT_TOKEN_CHANNEL;
            // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:102:5: ( 'file' )
            // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:102:7: 'file'
            {
            match("file"); 


            }

            state.type = _type;
            state.channel = _channel;
        }
        finally {
        }
    }
    // $ANTLR end "FILE"

    // $ANTLR start "WRITENET"
    public final void mWRITENET() throws RecognitionException {
        try {
            int _type = WRITENET;
            int _channel = DEFAULT_TOKEN_CHANNEL;
            // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:103:9: ( 'writeNET' )
            // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:103:11: 'writeNET'
            {
            match("writeNET"); 


            }

            state.type = _type;
            state.channel = _channel;
        }
        finally {
        }
    }
    // $ANTLR end "WRITENET"

    // $ANTLR start "WRITESBML"
    public final void mWRITESBML() throws RecognitionException {
        try {
            int _type = WRITESBML;
            int _channel = DEFAULT_TOKEN_CHANNEL;
            // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:104:10: ( 'writeSBML' )
            // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:104:12: 'writeSBML'
            {
            match("writeSBML"); 


            }

            state.type = _type;
            state.channel = _channel;
        }
        finally {
        }
    }
    // $ANTLR end "WRITESBML"

    // $ANTLR start "WRITEMFILE"
    public final void mWRITEMFILE() throws RecognitionException {
        try {
            int _type = WRITEMFILE;
            int _channel = DEFAULT_TOKEN_CHANNEL;
            // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:105:11: ( 'writeMfile' )
            // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:105:13: 'writeMfile'
            {
            match("writeMfile"); 


            }

            state.type = _type;
            state.channel = _channel;
        }
        finally {
        }
    }
    // $ANTLR end "WRITEMFILE"

    // $ANTLR start "WRITEXML"
    public final void mWRITEXML() throws RecognitionException {
        try {
            int _type = WRITEXML;
            int _channel = DEFAULT_TOKEN_CHANNEL;
            // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:106:9: ( 'writeXML' )
            // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:106:11: 'writeXML'
            {
            match("writeXML"); 


            }

            state.type = _type;
            state.channel = _channel;
        }
        finally {
        }
    }
    // $ANTLR end "WRITEXML"

    // $ANTLR start "WRITEMEXFILE"
    public final void mWRITEMEXFILE() throws RecognitionException {
        try {
            int _type = WRITEMEXFILE;
            int _channel = DEFAULT_TOKEN_CHANNEL;
            // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:107:13: ( 'writeMex' ( 'F' | 'f' ) 'ile' )
            // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:107:15: 'writeMex' ( 'F' | 'f' ) 'ile'
            {
            match("writeMex"); 

            if ( input.LA(1)=='F'||input.LA(1)=='f' ) {
                input.consume();

            }
            else {
                MismatchedSetException mse = new MismatchedSetException(null,input);
                recover(mse);
                throw mse;}

            match("ile"); 


            }

            state.type = _type;
            state.channel = _channel;
        }
        finally {
        }
    }
    // $ANTLR end "WRITEMEXFILE"

    // $ANTLR start "PRINT_CDAT"
    public final void mPRINT_CDAT() throws RecognitionException {
        try {
            int _type = PRINT_CDAT;
            int _channel = DEFAULT_TOKEN_CHANNEL;
            // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:108:11: ( 'print_CDAT' )
            // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:108:13: 'print_CDAT'
            {
            match("print_CDAT"); 


            }

            state.type = _type;
            state.channel = _channel;
        }
        finally {
        }
    }
    // $ANTLR end "PRINT_CDAT"

    // $ANTLR start "PRINT_FUNCTIONS"
    public final void mPRINT_FUNCTIONS() throws RecognitionException {
        try {
            int _type = PRINT_FUNCTIONS;
            int _channel = DEFAULT_TOKEN_CHANNEL;
            // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:109:16: ( 'print_functions' )
            // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:109:18: 'print_functions'
            {
            match("print_functions"); 


            }

            state.type = _type;
            state.channel = _channel;
        }
        finally {
        }
    }
    // $ANTLR end "PRINT_FUNCTIONS"

    // $ANTLR start "SETCONCENTRATION"
    public final void mSETCONCENTRATION() throws RecognitionException {
        try {
            int _type = SETCONCENTRATION;
            int _channel = DEFAULT_TOKEN_CHANNEL;
            // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:110:17: ( 'setConcentration' )
            // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:110:19: 'setConcentration'
            {
            match("setConcentration"); 


            }

            state.type = _type;
            state.channel = _channel;
        }
        finally {
        }
    }
    // $ANTLR end "SETCONCENTRATION"

    // $ANTLR start "METHOD"
    public final void mMETHOD() throws RecognitionException {
        try {
            int _type = METHOD;
            int _channel = DEFAULT_TOKEN_CHANNEL;
            // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:111:7: ( 'method' )
            // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:111:9: 'method'
            {
            match("method"); 


            }

            state.type = _type;
            state.channel = _channel;
        }
        finally {
        }
    }
    // $ANTLR end "METHOD"

    // $ANTLR start "SETPARAMETER"
    public final void mSETPARAMETER() throws RecognitionException {
        try {
            int _type = SETPARAMETER;
            int _channel = DEFAULT_TOKEN_CHANNEL;
            // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:112:13: ( 'setParameter' )
            // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:112:15: 'setParameter'
            {
            match("setParameter"); 


            }

            state.type = _type;
            state.channel = _channel;
        }
        finally {
        }
    }
    // $ANTLR end "SETPARAMETER"

    // $ANTLR start "SAVECONCENTRATIONS"
    public final void mSAVECONCENTRATIONS() throws RecognitionException {
        try {
            int _type = SAVECONCENTRATIONS;
            int _channel = DEFAULT_TOKEN_CHANNEL;
            // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:113:19: ( 'saveConcentrations' )
            // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:113:21: 'saveConcentrations'
            {
            match("saveConcentrations"); 


            }

            state.type = _type;
            state.channel = _channel;
        }
        finally {
        }
    }
    // $ANTLR end "SAVECONCENTRATIONS"

    // $ANTLR start "RESETCONCENTRATIONS"
    public final void mRESETCONCENTRATIONS() throws RecognitionException {
        try {
            int _type = RESETCONCENTRATIONS;
            int _channel = DEFAULT_TOKEN_CHANNEL;
            // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:114:20: ( 'resetConcentrations' )
            // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:114:22: 'resetConcentrations'
            {
            match("resetConcentrations"); 


            }

            state.type = _type;
            state.channel = _channel;
        }
        finally {
        }
    }
    // $ANTLR end "RESETCONCENTRATIONS"

    // $ANTLR start "ADDCONCENTRATION"
    public final void mADDCONCENTRATION() throws RecognitionException {
        try {
            int _type = ADDCONCENTRATION;
            int _channel = DEFAULT_TOKEN_CHANNEL;
            // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:115:17: ( 'addConcentration' )
            // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:115:19: 'addConcentration'
            {
            match("addConcentration"); 


            }

            state.type = _type;
            state.channel = _channel;
        }
        finally {
        }
    }
    // $ANTLR end "ADDCONCENTRATION"

    // $ANTLR start "GENERATENETWORK"
    public final void mGENERATENETWORK() throws RecognitionException {
        try {
            int _type = GENERATENETWORK;
            int _channel = DEFAULT_TOKEN_CHANNEL;
            // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:116:16: ( 'generate_network' )
            // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:116:18: 'generate_network'
            {
            match("generate_network"); 


            }

            state.type = _type;
            state.channel = _channel;
        }
        finally {
        }
    }
    // $ANTLR end "GENERATENETWORK"

    // $ANTLR start "WRITENETWORK"
    public final void mWRITENETWORK() throws RecognitionException {
        try {
            int _type = WRITENETWORK;
            int _channel = DEFAULT_TOKEN_CHANNEL;
            // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:117:13: ( 'writeNetwork' )
            // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:117:15: 'writeNetwork'
            {
            match("writeNetwork"); 


            }

            state.type = _type;
            state.channel = _channel;
        }
        finally {
        }
    }
    // $ANTLR end "WRITENETWORK"

    // $ANTLR start "SUBSTANCEUNITS"
    public final void mSUBSTANCEUNITS() throws RecognitionException {
        try {
            int _type = SUBSTANCEUNITS;
            int _channel = DEFAULT_TOKEN_CHANNEL;
            // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:118:15: ( 'substanceUnits' )
            // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:118:17: 'substanceUnits'
            {
            match("substanceUnits"); 


            }

            state.type = _type;
            state.channel = _channel;
        }
        finally {
        }
    }
    // $ANTLR end "SUBSTANCEUNITS"

    // $ANTLR start "PARAM"
    public final void mPARAM() throws RecognitionException {
        try {
            int _type = PARAM;
            int _channel = DEFAULT_TOKEN_CHANNEL;
            // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:119:6: ( 'param' )
            // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:119:8: 'param'
            {
            match("param"); 


            }

            state.type = _type;
            state.channel = _channel;
        }
        finally {
        }
    }
    // $ANTLR end "PARAM"

    // $ANTLR start "COMPLEX"
    public final void mCOMPLEX() throws RecognitionException {
        try {
            int _type = COMPLEX;
            int _channel = DEFAULT_TOKEN_CHANNEL;
            // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:120:8: ( 'complex' )
            // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:120:10: 'complex'
            {
            match("complex"); 


            }

            state.type = _type;
            state.channel = _channel;
        }
        finally {
        }
    }
    // $ANTLR end "COMPLEX"

    // $ANTLR start "GET_FINAL_STATE"
    public final void mGET_FINAL_STATE() throws RecognitionException {
        try {
            int _type = GET_FINAL_STATE;
            int _channel = DEFAULT_TOKEN_CHANNEL;
            // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:121:16: ( 'get_final_state' )
            // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:121:18: 'get_final_state'
            {
            match("get_final_state"); 


            }

            state.type = _type;
            state.channel = _channel;
        }
        finally {
        }
    }
    // $ANTLR end "GET_FINAL_STATE"

    // $ANTLR start "SAT"
    public final void mSAT() throws RecognitionException {
        try {
            int _type = SAT;
            int _channel = DEFAULT_TOKEN_CHANNEL;
            // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:122:4: ( 'Sat' )
            // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:122:6: 'Sat'
            {
            match("Sat"); 


            }

            state.type = _type;
            state.channel = _channel;
        }
        finally {
        }
    }
    // $ANTLR end "SAT"

    // $ANTLR start "EXP"
    public final void mEXP() throws RecognitionException {
        try {
            int _type = EXP;
            int _channel = DEFAULT_TOKEN_CHANNEL;
            // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:123:4: ( 'exp' )
            // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:123:6: 'exp'
            {
            match("exp"); 


            }

            state.type = _type;
            state.channel = _channel;
        }
        finally {
        }
    }
    // $ANTLR end "EXP"

    // $ANTLR start "LOG"
    public final void mLOG() throws RecognitionException {
        try {
            int _type = LOG;
            int _channel = DEFAULT_TOKEN_CHANNEL;
            // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:124:4: ( 'log' )
            // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:124:6: 'log'
            {
            match("log"); 


            }

            state.type = _type;
            state.channel = _channel;
        }
        finally {
        }
    }
    // $ANTLR end "LOG"

    // $ANTLR start "ARGFILE"
    public final void mARGFILE() throws RecognitionException {
        try {
            int _type = ARGFILE;
            int _channel = DEFAULT_TOKEN_CHANNEL;
            // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:125:8: ( 'argfile' )
            // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:125:10: 'argfile'
            {
            match("argfile"); 


            }

            state.type = _type;
            state.channel = _channel;
        }
        finally {
        }
    }
    // $ANTLR end "ARGFILE"

    // $ANTLR start "SAVE_PROGRESS"
    public final void mSAVE_PROGRESS() throws RecognitionException {
        try {
            int _type = SAVE_PROGRESS;
            int _channel = DEFAULT_TOKEN_CHANNEL;
            // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:126:14: ( 'save_progress' )
            // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:126:16: 'save_progress'
            {
            match("save_progress"); 


            }

            state.type = _type;
            state.channel = _channel;
        }
        finally {
        }
    }
    // $ANTLR end "SAVE_PROGRESS"

    // $ANTLR start "NETFILE"
    public final void mNETFILE() throws RecognitionException {
        try {
            int _type = NETFILE;
            int _channel = DEFAULT_TOKEN_CHANNEL;
            // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:127:8: ( 'netfile' )
            // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:127:10: 'netfile'
            {
            match("netfile"); 


            }

            state.type = _type;
            state.channel = _channel;
        }
        finally {
        }
    }
    // $ANTLR end "NETFILE"

    // $ANTLR start "PRINT_NET"
    public final void mPRINT_NET() throws RecognitionException {
        try {
            int _type = PRINT_NET;
            int _channel = DEFAULT_TOKEN_CHANNEL;
            // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:128:10: ( 'print_net' )
            // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:128:12: 'print_net'
            {
            match("print_net"); 


            }

            state.type = _type;
            state.channel = _channel;
        }
        finally {
        }
    }
    // $ANTLR end "PRINT_NET"

    // $ANTLR start "PRINT_END"
    public final void mPRINT_END() throws RecognitionException {
        try {
            int _type = PRINT_END;
            int _channel = DEFAULT_TOKEN_CHANNEL;
            // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:129:10: ( 'print_end' )
            // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:129:12: 'print_end'
            {
            match("print_end"); 


            }

            state.type = _type;
            state.channel = _channel;
        }
        finally {
        }
    }
    // $ANTLR end "PRINT_END"

    // $ANTLR start "TEXTREACTION"
    public final void mTEXTREACTION() throws RecognitionException {
        try {
            int _type = TEXTREACTION;
            int _channel = DEFAULT_TOKEN_CHANNEL;
            // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:130:13: ( 'TextReaction' )
            // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:130:15: 'TextReaction'
            {
            match("TextReaction"); 


            }

            state.type = _type;
            state.channel = _channel;
        }
        finally {
        }
    }
    // $ANTLR end "TEXTREACTION"

    // $ANTLR start "CONTINUE"
    public final void mCONTINUE() throws RecognitionException {
        try {
            int _type = CONTINUE;
            int _channel = DEFAULT_TOKEN_CHANNEL;
            // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:131:9: ( 'continue' )
            // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:131:11: 'continue'
            {
            match("continue"); 


            }

            state.type = _type;
            state.channel = _channel;
        }
        finally {
        }
    }
    // $ANTLR end "CONTINUE"

    // $ANTLR start "ODE"
    public final void mODE() throws RecognitionException {
        try {
            int _type = ODE;
            int _channel = DEFAULT_TOKEN_CHANNEL;
            // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:132:4: ( 'ode' )
            // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:132:6: 'ode'
            {
            match("ode"); 


            }

            state.type = _type;
            state.channel = _channel;
        }
        finally {
        }
    }
    // $ANTLR end "ODE"

    // $ANTLR start "SSA"
    public final void mSSA() throws RecognitionException {
        try {
            int _type = SSA;
            int _channel = DEFAULT_TOKEN_CHANNEL;
            // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:133:4: ( 'ssa' )
            // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:133:6: 'ssa'
            {
            match("ssa"); 


            }

            state.type = _type;
            state.channel = _channel;
        }
        finally {
        }
    }
    // $ANTLR end "SSA"

    // $ANTLR start "NF"
    public final void mNF() throws RecognitionException {
        try {
            int _type = NF;
            int _channel = DEFAULT_TOKEN_CHANNEL;
            // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:134:3: ( 'nf' )
            // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:134:5: 'nf'
            {
            match("nf"); 


            }

            state.type = _type;
            state.channel = _channel;
        }
        finally {
        }
    }
    // $ANTLR end "NF"

    // $ANTLR start "PLA"
    public final void mPLA() throws RecognitionException {
        try {
            int _type = PLA;
            int _channel = DEFAULT_TOKEN_CHANNEL;
            // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:135:4: ( 'pla' )
            // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:135:6: 'pla'
            {
            match("pla"); 


            }

            state.type = _type;
            state.channel = _channel;
        }
        finally {
        }
    }
    // $ANTLR end "PLA"

    // $ANTLR start "EVALUATE_EXPRESSIONS"
    public final void mEVALUATE_EXPRESSIONS() throws RecognitionException {
        try {
            int _type = EVALUATE_EXPRESSIONS;
            int _channel = DEFAULT_TOKEN_CHANNEL;
            // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:136:21: ( 'evaluate_expressions' )
            // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:136:23: 'evaluate_expressions'
            {
            match("evaluate_expressions"); 


            }

            state.type = _type;
            state.channel = _channel;
        }
        finally {
        }
    }
    // $ANTLR end "EVALUATE_EXPRESSIONS"

    // $ANTLR start "BDF"
    public final void mBDF() throws RecognitionException {
        try {
            int _type = BDF;
            int _channel = DEFAULT_TOKEN_CHANNEL;
            // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:137:4: ( 'bdf' )
            // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:137:6: 'bdf'
            {
            match("bdf"); 


            }

            state.type = _type;
            state.channel = _channel;
        }
        finally {
        }
    }
    // $ANTLR end "BDF"

    // $ANTLR start "FLOAT"
    public final void mFLOAT() throws RecognitionException {
        try {
            int _type = FLOAT;
            int _channel = DEFAULT_TOKEN_CHANNEL;
            // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:138:6: ( ( DIGIT )+ '.' ( DIGIT )* ( EXPONENT )? | ( DIGIT )+ EXPONENT )
            int alt12=2;
            alt12 = dfa12.predict(input);
            switch (alt12) {
                case 1 :
                    // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:139:3: ( DIGIT )+ '.' ( DIGIT )* ( EXPONENT )?
                    {
                    // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:139:3: ( DIGIT )+
                    int cnt8=0;
                    loop8:
                    do {
                        int alt8=2;
                        int LA8_0 = input.LA(1);

                        if ( ((LA8_0>='0' && LA8_0<='9')) ) {
                            alt8=1;
                        }


                        switch (alt8) {
                    	case 1 :
                    	    // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:139:4: DIGIT
                    	    {
                    	    mDIGIT(); 

                    	    }
                    	    break;

                    	default :
                    	    if ( cnt8 >= 1 ) break loop8;
                                EarlyExitException eee =
                                    new EarlyExitException(8, input);
                                throw eee;
                        }
                        cnt8++;
                    } while (true);

                    match('.'); 
                    // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:139:16: ( DIGIT )*
                    loop9:
                    do {
                        int alt9=2;
                        int LA9_0 = input.LA(1);

                        if ( ((LA9_0>='0' && LA9_0<='9')) ) {
                            alt9=1;
                        }


                        switch (alt9) {
                    	case 1 :
                    	    // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:139:17: DIGIT
                    	    {
                    	    mDIGIT(); 

                    	    }
                    	    break;

                    	default :
                    	    break loop9;
                        }
                    } while (true);

                    // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:139:25: ( EXPONENT )?
                    int alt10=2;
                    int LA10_0 = input.LA(1);

                    if ( (LA10_0=='E'||LA10_0=='e') ) {
                        alt10=1;
                    }
                    switch (alt10) {
                        case 1 :
                            // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:139:25: EXPONENT
                            {
                            mEXPONENT(); 

                            }
                            break;

                    }


                    }
                    break;
                case 2 :
                    // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:140:3: ( DIGIT )+ EXPONENT
                    {
                    // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:140:3: ( DIGIT )+
                    int cnt11=0;
                    loop11:
                    do {
                        int alt11=2;
                        int LA11_0 = input.LA(1);

                        if ( ((LA11_0>='0' && LA11_0<='9')) ) {
                            alt11=1;
                        }


                        switch (alt11) {
                    	case 1 :
                    	    // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:140:4: DIGIT
                    	    {
                    	    mDIGIT(); 

                    	    }
                    	    break;

                    	default :
                    	    if ( cnt11 >= 1 ) break loop11;
                                EarlyExitException eee =
                                    new EarlyExitException(11, input);
                                throw eee;
                        }
                        cnt11++;
                    } while (true);

                    mEXPONENT(); 

                    }
                    break;

            }
            state.type = _type;
            state.channel = _channel;
        }
        finally {
        }
    }
    // $ANTLR end "FLOAT"

    // $ANTLR start "INT"
    public final void mINT() throws RecognitionException {
        try {
            int _type = INT;
            int _channel = DEFAULT_TOKEN_CHANNEL;
            // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:141:4: ( ( DIGIT )+ )
            // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:141:6: ( DIGIT )+
            {
            // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:141:6: ( DIGIT )+
            int cnt13=0;
            loop13:
            do {
                int alt13=2;
                int LA13_0 = input.LA(1);

                if ( ((LA13_0>='0' && LA13_0<='9')) ) {
                    alt13=1;
                }


                switch (alt13) {
            	case 1 :
            	    // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:141:6: DIGIT
            	    {
            	    mDIGIT(); 

            	    }
            	    break;

            	default :
            	    if ( cnt13 >= 1 ) break loop13;
                        EarlyExitException eee =
                            new EarlyExitException(13, input);
                        throw eee;
                }
                cnt13++;
            } while (true);


            }

            state.type = _type;
            state.channel = _channel;
        }
        finally {
        }
    }
    // $ANTLR end "INT"

    // $ANTLR start "STRING"
    public final void mSTRING() throws RecognitionException {
        try {
            int _type = STRING;
            int _channel = DEFAULT_TOKEN_CHANNEL;
            // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:142:7: ( ( LETTER | DIGIT | '_' )+ )
            // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:142:9: ( LETTER | DIGIT | '_' )+
            {
            // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:142:9: ( LETTER | DIGIT | '_' )+
            int cnt14=0;
            loop14:
            do {
                int alt14=2;
                int LA14_0 = input.LA(1);

                if ( ((LA14_0>='0' && LA14_0<='9')||(LA14_0>='A' && LA14_0<='Z')||LA14_0=='_'||(LA14_0>='a' && LA14_0<='z')) ) {
                    alt14=1;
                }


                switch (alt14) {
            	case 1 :
            	    // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:
            	    {
            	    if ( (input.LA(1)>='0' && input.LA(1)<='9')||(input.LA(1)>='A' && input.LA(1)<='Z')||input.LA(1)=='_'||(input.LA(1)>='a' && input.LA(1)<='z') ) {
            	        input.consume();

            	    }
            	    else {
            	        MismatchedSetException mse = new MismatchedSetException(null,input);
            	        recover(mse);
            	        throw mse;}


            	    }
            	    break;

            	default :
            	    if ( cnt14 >= 1 ) break loop14;
                        EarlyExitException eee =
                            new EarlyExitException(14, input);
                        throw eee;
                }
                cnt14++;
            } while (true);


            }

            state.type = _type;
            state.channel = _channel;
        }
        finally {
        }
    }
    // $ANTLR end "STRING"

    // $ANTLR start "SEMI"
    public final void mSEMI() throws RecognitionException {
        try {
            int _type = SEMI;
            int _channel = DEFAULT_TOKEN_CHANNEL;
            // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:144:6: ( ';' )
            // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:144:8: ';'
            {
            match(';'); 

            }

            state.type = _type;
            state.channel = _channel;
        }
        finally {
        }
    }
    // $ANTLR end "SEMI"

    // $ANTLR start "COLON"
    public final void mCOLON() throws RecognitionException {
        try {
            int _type = COLON;
            int _channel = DEFAULT_TOKEN_CHANNEL;
            // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:145:6: ( ':' )
            // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:145:8: ':'
            {
            match(':'); 

            }

            state.type = _type;
            state.channel = _channel;
        }
        finally {
        }
    }
    // $ANTLR end "COLON"

    // $ANTLR start "LSBRACKET"
    public final void mLSBRACKET() throws RecognitionException {
        try {
            int _type = LSBRACKET;
            int _channel = DEFAULT_TOKEN_CHANNEL;
            // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:146:11: ( '[' )
            // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:146:13: '['
            {
            match('['); 

            }

            state.type = _type;
            state.channel = _channel;
        }
        finally {
        }
    }
    // $ANTLR end "LSBRACKET"

    // $ANTLR start "RSBRACKET"
    public final void mRSBRACKET() throws RecognitionException {
        try {
            int _type = RSBRACKET;
            int _channel = DEFAULT_TOKEN_CHANNEL;
            // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:147:11: ( ']' )
            // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:147:13: ']'
            {
            match(']'); 

            }

            state.type = _type;
            state.channel = _channel;
        }
        finally {
        }
    }
    // $ANTLR end "RSBRACKET"

    // $ANTLR start "LBRACKET"
    public final void mLBRACKET() throws RecognitionException {
        try {
            int _type = LBRACKET;
            int _channel = DEFAULT_TOKEN_CHANNEL;
            // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:148:10: ( '{' )
            // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:148:12: '{'
            {
            match('{'); 

            }

            state.type = _type;
            state.channel = _channel;
        }
        finally {
        }
    }
    // $ANTLR end "LBRACKET"

    // $ANTLR start "RBRACKET"
    public final void mRBRACKET() throws RecognitionException {
        try {
            int _type = RBRACKET;
            int _channel = DEFAULT_TOKEN_CHANNEL;
            // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:149:10: ( '}' )
            // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:149:12: '}'
            {
            match('}'); 

            }

            state.type = _type;
            state.channel = _channel;
        }
        finally {
        }
    }
    // $ANTLR end "RBRACKET"

    // $ANTLR start "COMMA"
    public final void mCOMMA() throws RecognitionException {
        try {
            int _type = COMMA;
            int _channel = DEFAULT_TOKEN_CHANNEL;
            // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:150:7: ( ',' )
            // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:150:9: ','
            {
            match(','); 

            }

            state.type = _type;
            state.channel = _channel;
        }
        finally {
        }
    }
    // $ANTLR end "COMMA"

    // $ANTLR start "DOT"
    public final void mDOT() throws RecognitionException {
        try {
            int _type = DOT;
            int _channel = DEFAULT_TOKEN_CHANNEL;
            // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:151:5: ( '.' ({...}? => ( ( DIGIT )+ ( EXPONENT )? ) | ) )
            // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:151:7: '.' ({...}? => ( ( DIGIT )+ ( EXPONENT )? ) | )
            {
            match('.'); 
            // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:151:13: ({...}? => ( ( DIGIT )+ ( EXPONENT )? ) | )
            int alt17=2;
            int LA17_0 = input.LA(1);

            if ( ((LA17_0>='0' && LA17_0<='9')) && ((floatLA()))) {
                alt17=1;
            }
            else {
                alt17=2;}
            switch (alt17) {
                case 1 :
                    // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:151:14: {...}? => ( ( DIGIT )+ ( EXPONENT )? )
                    {
                    if ( !((floatLA())) ) {
                        throw new FailedPredicateException(input, "DOT", "floatLA()");
                    }
                    // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:151:30: ( ( DIGIT )+ ( EXPONENT )? )
                    // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:151:31: ( DIGIT )+ ( EXPONENT )?
                    {
                    // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:151:31: ( DIGIT )+
                    int cnt15=0;
                    loop15:
                    do {
                        int alt15=2;
                        int LA15_0 = input.LA(1);

                        if ( ((LA15_0>='0' && LA15_0<='9')) ) {
                            alt15=1;
                        }


                        switch (alt15) {
                    	case 1 :
                    	    // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:151:32: DIGIT
                    	    {
                    	    mDIGIT(); 

                    	    }
                    	    break;

                    	default :
                    	    if ( cnt15 >= 1 ) break loop15;
                                EarlyExitException eee =
                                    new EarlyExitException(15, input);
                                throw eee;
                        }
                        cnt15++;
                    } while (true);

                    // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:151:40: ( EXPONENT )?
                    int alt16=2;
                    int LA16_0 = input.LA(1);

                    if ( (LA16_0=='E'||LA16_0=='e') ) {
                        alt16=1;
                    }
                    switch (alt16) {
                        case 1 :
                            // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:151:40: EXPONENT
                            {
                            mEXPONENT(); 

                            }
                            break;

                    }

                    _type=FLOAT;

                    }


                    }
                    break;
                case 2 :
                    // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:151:69: 
                    {
                    _type=DOT;

                    }
                    break;

            }


            }

            state.type = _type;
            state.channel = _channel;
        }
        finally {
        }
    }
    // $ANTLR end "DOT"

    // $ANTLR start "LPAREN"
    public final void mLPAREN() throws RecognitionException {
        try {
            int _type = LPAREN;
            int _channel = DEFAULT_TOKEN_CHANNEL;
            // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:152:8: ( '(' )
            // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:152:10: '('
            {
            match('('); 

            }

            state.type = _type;
            state.channel = _channel;
        }
        finally {
        }
    }
    // $ANTLR end "LPAREN"

    // $ANTLR start "RPAREN"
    public final void mRPAREN() throws RecognitionException {
        try {
            int _type = RPAREN;
            int _channel = DEFAULT_TOKEN_CHANNEL;
            // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:153:8: ( ')' )
            // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:153:10: ')'
            {
            match(')'); 

            }

            state.type = _type;
            state.channel = _channel;
        }
        finally {
        }
    }
    // $ANTLR end "RPAREN"

    // $ANTLR start "UNI_REACTION_SIGN"
    public final void mUNI_REACTION_SIGN() throws RecognitionException {
        try {
            int _type = UNI_REACTION_SIGN;
            int _channel = DEFAULT_TOKEN_CHANNEL;
            // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:154:19: ( '->' )
            // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:154:21: '->'
            {
            match("->"); 


            }

            state.type = _type;
            state.channel = _channel;
        }
        finally {
        }
    }
    // $ANTLR end "UNI_REACTION_SIGN"

    // $ANTLR start "BI_REACTION_SIGN"
    public final void mBI_REACTION_SIGN() throws RecognitionException {
        try {
            int _type = BI_REACTION_SIGN;
            int _channel = DEFAULT_TOKEN_CHANNEL;
            // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:155:18: ( '<->' )
            // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:155:20: '<->'
            {
            match("<->"); 


            }

            state.type = _type;
            state.channel = _channel;
        }
        finally {
        }
    }
    // $ANTLR end "BI_REACTION_SIGN"

    // $ANTLR start "DOLLAR"
    public final void mDOLLAR() throws RecognitionException {
        try {
            int _type = DOLLAR;
            int _channel = DEFAULT_TOKEN_CHANNEL;
            // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:156:8: ( '$' )
            // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:156:10: '$'
            {
            match('$'); 

            }

            state.type = _type;
            state.channel = _channel;
        }
        finally {
        }
    }
    // $ANTLR end "DOLLAR"

    // $ANTLR start "TILDE"
    public final void mTILDE() throws RecognitionException {
        try {
            int _type = TILDE;
            int _channel = DEFAULT_TOKEN_CHANNEL;
            // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:157:7: ( '~' )
            // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:157:9: '~'
            {
            match('~'); 

            }

            state.type = _type;
            state.channel = _channel;
        }
        finally {
        }
    }
    // $ANTLR end "TILDE"

    // $ANTLR start "AT"
    public final void mAT() throws RecognitionException {
        try {
            int _type = AT;
            int _channel = DEFAULT_TOKEN_CHANNEL;
            // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:158:4: ( '@' )
            // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:158:6: '@'
            {
            match('@'); 

            }

            state.type = _type;
            state.channel = _channel;
        }
        finally {
        }
    }
    // $ANTLR end "AT"

    // $ANTLR start "GTE"
    public final void mGTE() throws RecognitionException {
        try {
            int _type = GTE;
            int _channel = DEFAULT_TOKEN_CHANNEL;
            // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:159:5: ( '>=' )
            // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:159:7: '>='
            {
            match(">="); 


            }

            state.type = _type;
            state.channel = _channel;
        }
        finally {
        }
    }
    // $ANTLR end "GTE"

    // $ANTLR start "GT"
    public final void mGT() throws RecognitionException {
        try {
            int _type = GT;
            int _channel = DEFAULT_TOKEN_CHANNEL;
            // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:160:4: ( '>' )
            // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:160:6: '>'
            {
            match('>'); 

            }

            state.type = _type;
            state.channel = _channel;
        }
        finally {
        }
    }
    // $ANTLR end "GT"

    // $ANTLR start "LTE"
    public final void mLTE() throws RecognitionException {
        try {
            int _type = LTE;
            int _channel = DEFAULT_TOKEN_CHANNEL;
            // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:161:5: ( '<=' )
            // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:161:7: '<='
            {
            match("<="); 


            }

            state.type = _type;
            state.channel = _channel;
        }
        finally {
        }
    }
    // $ANTLR end "LTE"

    // $ANTLR start "LT"
    public final void mLT() throws RecognitionException {
        try {
            int _type = LT;
            int _channel = DEFAULT_TOKEN_CHANNEL;
            // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:162:4: ( '<' )
            // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:162:6: '<'
            {
            match('<'); 

            }

            state.type = _type;
            state.channel = _channel;
        }
        finally {
        }
    }
    // $ANTLR end "LT"

    // $ANTLR start "ASSIGNS"
    public final void mASSIGNS() throws RecognitionException {
        try {
            int _type = ASSIGNS;
            int _channel = DEFAULT_TOKEN_CHANNEL;
            // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:163:9: ( '=>' )
            // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:163:11: '=>'
            {
            match("=>"); 


            }

            state.type = _type;
            state.channel = _channel;
        }
        finally {
        }
    }
    // $ANTLR end "ASSIGNS"

    // $ANTLR start "EQUALS"
    public final void mEQUALS() throws RecognitionException {
        try {
            int _type = EQUALS;
            int _channel = DEFAULT_TOKEN_CHANNEL;
            // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:164:8: ( '==' )
            // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:164:10: '=='
            {
            match("=="); 


            }

            state.type = _type;
            state.channel = _channel;
        }
        finally {
        }
    }
    // $ANTLR end "EQUALS"

    // $ANTLR start "BECOMES"
    public final void mBECOMES() throws RecognitionException {
        try {
            int _type = BECOMES;
            int _channel = DEFAULT_TOKEN_CHANNEL;
            // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:165:9: ( '=' )
            // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:165:11: '='
            {
            match('='); 

            }

            state.type = _type;
            state.channel = _channel;
        }
        finally {
        }
    }
    // $ANTLR end "BECOMES"

    // $ANTLR start "DIV"
    public final void mDIV() throws RecognitionException {
        try {
            int _type = DIV;
            int _channel = DEFAULT_TOKEN_CHANNEL;
            // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:166:5: ( '/' )
            // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:166:7: '/'
            {
            match('/'); 

            }

            state.type = _type;
            state.channel = _channel;
        }
        finally {
        }
    }
    // $ANTLR end "DIV"

    // $ANTLR start "TIMES"
    public final void mTIMES() throws RecognitionException {
        try {
            int _type = TIMES;
            int _channel = DEFAULT_TOKEN_CHANNEL;
            // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:167:7: ( '*' )
            // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:167:9: '*'
            {
            match('*'); 

            }

            state.type = _type;
            state.channel = _channel;
        }
        finally {
        }
    }
    // $ANTLR end "TIMES"

    // $ANTLR start "MINUS"
    public final void mMINUS() throws RecognitionException {
        try {
            int _type = MINUS;
            int _channel = DEFAULT_TOKEN_CHANNEL;
            // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:168:7: ( '-' )
            // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:168:9: '-'
            {
            match('-'); 

            }

            state.type = _type;
            state.channel = _channel;
        }
        finally {
        }
    }
    // $ANTLR end "MINUS"

    // $ANTLR start "PLUS"
    public final void mPLUS() throws RecognitionException {
        try {
            int _type = PLUS;
            int _channel = DEFAULT_TOKEN_CHANNEL;
            // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:169:6: ( '+' )
            // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:169:8: '+'
            {
            match('+'); 

            }

            state.type = _type;
            state.channel = _channel;
        }
        finally {
        }
    }
    // $ANTLR end "PLUS"

    // $ANTLR start "POWER"
    public final void mPOWER() throws RecognitionException {
        try {
            int _type = POWER;
            int _channel = DEFAULT_TOKEN_CHANNEL;
            // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:170:6: ( '^' )
            // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:170:8: '^'
            {
            match('^'); 

            }

            state.type = _type;
            state.channel = _channel;
        }
        finally {
        }
    }
    // $ANTLR end "POWER"

    // $ANTLR start "MOD"
    public final void mMOD() throws RecognitionException {
        try {
            int _type = MOD;
            int _channel = DEFAULT_TOKEN_CHANNEL;
            // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:171:5: ( '%' )
            // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:171:7: '%'
            {
            match('%'); 

            }

            state.type = _type;
            state.channel = _channel;
        }
        finally {
        }
    }
    // $ANTLR end "MOD"

    // $ANTLR start "PIPE"
    public final void mPIPE() throws RecognitionException {
        try {
            int _type = PIPE;
            int _channel = DEFAULT_TOKEN_CHANNEL;
            // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:172:6: ( '|' )
            // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:172:8: '|'
            {
            match('|'); 

            }

            state.type = _type;
            state.channel = _channel;
        }
        finally {
        }
    }
    // $ANTLR end "PIPE"

    // $ANTLR start "QMARK"
    public final void mQMARK() throws RecognitionException {
        try {
            int _type = QMARK;
            int _channel = DEFAULT_TOKEN_CHANNEL;
            // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:173:7: ( '?' )
            // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:173:9: '?'
            {
            match('?'); 

            }

            state.type = _type;
            state.channel = _channel;
        }
        finally {
        }
    }
    // $ANTLR end "QMARK"

    // $ANTLR start "EMARK"
    public final void mEMARK() throws RecognitionException {
        try {
            int _type = EMARK;
            int _channel = DEFAULT_TOKEN_CHANNEL;
            // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:174:6: ( '!' )
            // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:174:8: '!'
            {
            match('!'); 

            }

            state.type = _type;
            state.channel = _channel;
        }
        finally {
        }
    }
    // $ANTLR end "EMARK"

    // $ANTLR start "DBQUOTES"
    public final void mDBQUOTES() throws RecognitionException {
        try {
            int _type = DBQUOTES;
            int _channel = DEFAULT_TOKEN_CHANNEL;
            // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:175:10: ( '\"' )
            // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:175:12: '\"'
            {
            match('\"'); 

            }

            state.type = _type;
            state.channel = _channel;
        }
        finally {
        }
    }
    // $ANTLR end "DBQUOTES"

    // $ANTLR start "AMPERSAND"
    public final void mAMPERSAND() throws RecognitionException {
        try {
            int _type = AMPERSAND;
            int _channel = DEFAULT_TOKEN_CHANNEL;
            // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:176:11: ( '&' )
            // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:176:13: '&'
            {
            match('&'); 

            }

            state.type = _type;
            state.channel = _channel;
        }
        finally {
        }
    }
    // $ANTLR end "AMPERSAND"

    // $ANTLR start "NULL"
    public final void mNULL() throws RecognitionException {
        try {
            // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:178:14: ( '0' )
            // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:178:16: '0'
            {
            match('0'); 

            }

        }
        finally {
        }
    }
    // $ANTLR end "NULL"

    // $ANTLR start "DIGIT"
    public final void mDIGIT() throws RecognitionException {
        try {
            // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:179:15: ( NULL | '1' .. '9' )
            // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:
            {
            if ( (input.LA(1)>='0' && input.LA(1)<='9') ) {
                input.consume();

            }
            else {
                MismatchedSetException mse = new MismatchedSetException(null,input);
                recover(mse);
                throw mse;}


            }

        }
        finally {
        }
    }
    // $ANTLR end "DIGIT"

    // $ANTLR start "LETTER"
    public final void mLETTER() throws RecognitionException {
        try {
            // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:180:16: ( LOWER | UPPER )
            // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:
            {
            if ( (input.LA(1)>='A' && input.LA(1)<='Z')||(input.LA(1)>='a' && input.LA(1)<='z') ) {
                input.consume();

            }
            else {
                MismatchedSetException mse = new MismatchedSetException(null,input);
                recover(mse);
                throw mse;}


            }

        }
        finally {
        }
    }
    // $ANTLR end "LETTER"

    // $ANTLR start "LOWER"
    public final void mLOWER() throws RecognitionException {
        try {
            // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:181:15: ( 'a' .. 'z' )
            // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:181:17: 'a' .. 'z'
            {
            matchRange('a','z'); 

            }

        }
        finally {
        }
    }
    // $ANTLR end "LOWER"

    // $ANTLR start "UPPER"
    public final void mUPPER() throws RecognitionException {
        try {
            // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:182:15: ( 'A' .. 'Z' )
            // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:182:17: 'A' .. 'Z'
            {
            matchRange('A','Z'); 

            }

        }
        finally {
        }
    }
    // $ANTLR end "UPPER"

    // $ANTLR start "EXPONENT"
    public final void mEXPONENT() throws RecognitionException {
        try {
            // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:183:19: ( ( 'e' | 'E' ) ( '+' | '-' )? ( DIGIT )+ )
            // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:183:21: ( 'e' | 'E' ) ( '+' | '-' )? ( DIGIT )+
            {
            if ( input.LA(1)=='E'||input.LA(1)=='e' ) {
                input.consume();

            }
            else {
                MismatchedSetException mse = new MismatchedSetException(null,input);
                recover(mse);
                throw mse;}

            // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:183:31: ( '+' | '-' )?
            int alt18=2;
            int LA18_0 = input.LA(1);

            if ( (LA18_0=='+'||LA18_0=='-') ) {
                alt18=1;
            }
            switch (alt18) {
                case 1 :
                    // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:
                    {
                    if ( input.LA(1)=='+'||input.LA(1)=='-' ) {
                        input.consume();

                    }
                    else {
                        MismatchedSetException mse = new MismatchedSetException(null,input);
                        recover(mse);
                        throw mse;}


                    }
                    break;

            }

            // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:183:42: ( DIGIT )+
            int cnt19=0;
            loop19:
            do {
                int alt19=2;
                int LA19_0 = input.LA(1);

                if ( ((LA19_0>='0' && LA19_0<='9')) ) {
                    alt19=1;
                }


                switch (alt19) {
            	case 1 :
            	    // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:183:43: DIGIT
            	    {
            	    mDIGIT(); 

            	    }
            	    break;

            	default :
            	    if ( cnt19 >= 1 ) break loop19;
                        EarlyExitException eee =
                            new EarlyExitException(19, input);
                        throw eee;
                }
                cnt19++;
            } while (true);


            }

        }
        finally {
        }
    }
    // $ANTLR end "EXPONENT"

    // $ANTLR start "VERSION_NUMBER"
    public final void mVERSION_NUMBER() throws RecognitionException {
        try {
            int _type = VERSION_NUMBER;
            int _channel = DEFAULT_TOKEN_CHANNEL;
            // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:186:16: ( ( INT ) | ( INT '.' INT ) | ( INT '.' INT '.' INT ) )
            int alt20=3;
            alt20 = dfa20.predict(input);
            switch (alt20) {
                case 1 :
                    // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:186:18: ( INT )
                    {
                    // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:186:18: ( INT )
                    // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:186:19: INT
                    {
                    mINT(); 

                    }


                    }
                    break;
                case 2 :
                    // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:186:26: ( INT '.' INT )
                    {
                    // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:186:26: ( INT '.' INT )
                    // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:186:27: INT '.' INT
                    {
                    mINT(); 
                    match('.'); 
                    mINT(); 

                    }


                    }
                    break;
                case 3 :
                    // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:186:40: ( INT '.' INT '.' INT )
                    {
                    // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:186:40: ( INT '.' INT '.' INT )
                    // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:186:41: INT '.' INT '.' INT
                    {
                    mINT(); 
                    match('.'); 
                    mINT(); 
                    match('.'); 
                    mINT(); 

                    }


                    }
                    break;

            }
            state.type = _type;
            state.channel = _channel;
        }
        finally {
        }
    }
    // $ANTLR end "VERSION_NUMBER"

    public void mTokens() throws RecognitionException {
        // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:1:8: ( LINE_COMMENT | LB | WS | VERSION | END | BEGIN | SEED | SPECIES | GROUPS | MOLECULE | MOLECULES | MODEL | TYPES | REACTION | REACTIONS | RULES | FUNCTIONS | COMPARTMENTS | ACTIONS | OBSERVABLES | POPULATION | MAPS | SET_OPTION | SPECIES_LABEL | GENERATE_NETWORK | MAX_AGG | MAX_ITER | MAX_STOICH | OVERWRITE | PRINT_ITER | AND | OR | ID | IF | PREFFIX | SUFFIX | LABEL | PARAMETERS | VERBOSE | GML | MATCHONCE | DELETEMOLECULES | MOVECONNECTED | AUTO | HNAUTY | QUASY | NOT | INCLUDE_REACTANTS | INCLUDE_PRODUCTS | EXCLUDE_REACTANTS | EXCLUDE_PRODUCTS | GENERATEHYBRIDMODEL | ATOL | RTOL | STEADY_STATE | SPARSE | T_END | T_START | N_STEPS | N_OUTPUT_STEPS | MAX_SIM_STEPS | OUTPUT_STEP_INTERVAL | PLA_CONFIG | SAMPLE_TIMES | SIMULATE_ODE | SIMULATE_SSA | SIMULATE_NF | SIMULATE_PLA | SIMULATE | READFILE | FILE | WRITENET | WRITESBML | WRITEMFILE | WRITEXML | WRITEMEXFILE | PRINT_CDAT | PRINT_FUNCTIONS | SETCONCENTRATION | METHOD | SETPARAMETER | SAVECONCENTRATIONS | RESETCONCENTRATIONS | ADDCONCENTRATION | GENERATENETWORK | WRITENETWORK | SUBSTANCEUNITS | PARAM | COMPLEX | GET_FINAL_STATE | SAT | EXP | LOG | ARGFILE | SAVE_PROGRESS | NETFILE | PRINT_NET | PRINT_END | TEXTREACTION | CONTINUE | ODE | SSA | NF | PLA | EVALUATE_EXPRESSIONS | BDF | FLOAT | INT | STRING | SEMI | COLON | LSBRACKET | RSBRACKET | LBRACKET | RBRACKET | COMMA | DOT | LPAREN | RPAREN | UNI_REACTION_SIGN | BI_REACTION_SIGN | DOLLAR | TILDE | AT | GTE | GT | LTE | LT | ASSIGNS | EQUALS | BECOMES | DIV | TIMES | MINUS | PLUS | POWER | MOD | PIPE | QMARK | EMARK | DBQUOTES | AMPERSAND | VERSION_NUMBER )
        int alt21=143;
        alt21 = dfa21.predict(input);
        switch (alt21) {
            case 1 :
                // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:1:10: LINE_COMMENT
                {
                mLINE_COMMENT(); 

                }
                break;
            case 2 :
                // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:1:23: LB
                {
                mLB(); 

                }
                break;
            case 3 :
                // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:1:26: WS
                {
                mWS(); 

                }
                break;
            case 4 :
                // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:1:29: VERSION
                {
                mVERSION(); 

                }
                break;
            case 5 :
                // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:1:37: END
                {
                mEND(); 

                }
                break;
            case 6 :
                // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:1:41: BEGIN
                {
                mBEGIN(); 

                }
                break;
            case 7 :
                // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:1:47: SEED
                {
                mSEED(); 

                }
                break;
            case 8 :
                // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:1:52: SPECIES
                {
                mSPECIES(); 

                }
                break;
            case 9 :
                // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:1:60: GROUPS
                {
                mGROUPS(); 

                }
                break;
            case 10 :
                // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:1:67: MOLECULE
                {
                mMOLECULE(); 

                }
                break;
            case 11 :
                // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:1:76: MOLECULES
                {
                mMOLECULES(); 

                }
                break;
            case 12 :
                // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:1:86: MODEL
                {
                mMODEL(); 

                }
                break;
            case 13 :
                // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:1:92: TYPES
                {
                mTYPES(); 

                }
                break;
            case 14 :
                // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:1:98: REACTION
                {
                mREACTION(); 

                }
                break;
            case 15 :
                // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:1:107: REACTIONS
                {
                mREACTIONS(); 

                }
                break;
            case 16 :
                // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:1:117: RULES
                {
                mRULES(); 

                }
                break;
            case 17 :
                // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:1:123: FUNCTIONS
                {
                mFUNCTIONS(); 

                }
                break;
            case 18 :
                // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:1:133: COMPARTMENTS
                {
                mCOMPARTMENTS(); 

                }
                break;
            case 19 :
                // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:1:146: ACTIONS
                {
                mACTIONS(); 

                }
                break;
            case 20 :
                // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:1:154: OBSERVABLES
                {
                mOBSERVABLES(); 

                }
                break;
            case 21 :
                // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:1:166: POPULATION
                {
                mPOPULATION(); 

                }
                break;
            case 22 :
                // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:1:177: MAPS
                {
                mMAPS(); 

                }
                break;
            case 23 :
                // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:1:182: SET_OPTION
                {
                mSET_OPTION(); 

                }
                break;
            case 24 :
                // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:1:193: SPECIES_LABEL
                {
                mSPECIES_LABEL(); 

                }
                break;
            case 25 :
                // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:1:207: GENERATE_NETWORK
                {
                mGENERATE_NETWORK(); 

                }
                break;
            case 26 :
                // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:1:224: MAX_AGG
                {
                mMAX_AGG(); 

                }
                break;
            case 27 :
                // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:1:232: MAX_ITER
                {
                mMAX_ITER(); 

                }
                break;
            case 28 :
                // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:1:241: MAX_STOICH
                {
                mMAX_STOICH(); 

                }
                break;
            case 29 :
                // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:1:252: OVERWRITE
                {
                mOVERWRITE(); 

                }
                break;
            case 30 :
                // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:1:262: PRINT_ITER
                {
                mPRINT_ITER(); 

                }
                break;
            case 31 :
                // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:1:273: AND
                {
                mAND(); 

                }
                break;
            case 32 :
                // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:1:277: OR
                {
                mOR(); 

                }
                break;
            case 33 :
                // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:1:280: ID
                {
                mID(); 

                }
                break;
            case 34 :
                // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:1:283: IF
                {
                mIF(); 

                }
                break;
            case 35 :
                // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:1:286: PREFFIX
                {
                mPREFFIX(); 

                }
                break;
            case 36 :
                // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:1:294: SUFFIX
                {
                mSUFFIX(); 

                }
                break;
            case 37 :
                // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:1:301: LABEL
                {
                mLABEL(); 

                }
                break;
            case 38 :
                // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:1:307: PARAMETERS
                {
                mPARAMETERS(); 

                }
                break;
            case 39 :
                // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:1:318: VERBOSE
                {
                mVERBOSE(); 

                }
                break;
            case 40 :
                // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:1:326: GML
                {
                mGML(); 

                }
                break;
            case 41 :
                // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:1:330: MATCHONCE
                {
                mMATCHONCE(); 

                }
                break;
            case 42 :
                // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:1:340: DELETEMOLECULES
                {
                mDELETEMOLECULES(); 

                }
                break;
            case 43 :
                // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:1:356: MOVECONNECTED
                {
                mMOVECONNECTED(); 

                }
                break;
            case 44 :
                // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:1:370: AUTO
                {
                mAUTO(); 

                }
                break;
            case 45 :
                // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:1:375: HNAUTY
                {
                mHNAUTY(); 

                }
                break;
            case 46 :
                // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:1:382: QUASY
                {
                mQUASY(); 

                }
                break;
            case 47 :
                // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:1:388: NOT
                {
                mNOT(); 

                }
                break;
            case 48 :
                // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:1:392: INCLUDE_REACTANTS
                {
                mINCLUDE_REACTANTS(); 

                }
                break;
            case 49 :
                // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:1:410: INCLUDE_PRODUCTS
                {
                mINCLUDE_PRODUCTS(); 

                }
                break;
            case 50 :
                // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:1:427: EXCLUDE_REACTANTS
                {
                mEXCLUDE_REACTANTS(); 

                }
                break;
            case 51 :
                // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:1:445: EXCLUDE_PRODUCTS
                {
                mEXCLUDE_PRODUCTS(); 

                }
                break;
            case 52 :
                // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:1:462: GENERATEHYBRIDMODEL
                {
                mGENERATEHYBRIDMODEL(); 

                }
                break;
            case 53 :
                // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:1:482: ATOL
                {
                mATOL(); 

                }
                break;
            case 54 :
                // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:1:487: RTOL
                {
                mRTOL(); 

                }
                break;
            case 55 :
                // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:1:492: STEADY_STATE
                {
                mSTEADY_STATE(); 

                }
                break;
            case 56 :
                // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:1:505: SPARSE
                {
                mSPARSE(); 

                }
                break;
            case 57 :
                // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:1:512: T_END
                {
                mT_END(); 

                }
                break;
            case 58 :
                // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:1:518: T_START
                {
                mT_START(); 

                }
                break;
            case 59 :
                // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:1:526: N_STEPS
                {
                mN_STEPS(); 

                }
                break;
            case 60 :
                // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:1:534: N_OUTPUT_STEPS
                {
                mN_OUTPUT_STEPS(); 

                }
                break;
            case 61 :
                // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:1:549: MAX_SIM_STEPS
                {
                mMAX_SIM_STEPS(); 

                }
                break;
            case 62 :
                // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:1:563: OUTPUT_STEP_INTERVAL
                {
                mOUTPUT_STEP_INTERVAL(); 

                }
                break;
            case 63 :
                // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:1:584: PLA_CONFIG
                {
                mPLA_CONFIG(); 

                }
                break;
            case 64 :
                // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:1:595: SAMPLE_TIMES
                {
                mSAMPLE_TIMES(); 

                }
                break;
            case 65 :
                // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:1:608: SIMULATE_ODE
                {
                mSIMULATE_ODE(); 

                }
                break;
            case 66 :
                // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:1:621: SIMULATE_SSA
                {
                mSIMULATE_SSA(); 

                }
                break;
            case 67 :
                // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:1:634: SIMULATE_NF
                {
                mSIMULATE_NF(); 

                }
                break;
            case 68 :
                // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:1:646: SIMULATE_PLA
                {
                mSIMULATE_PLA(); 

                }
                break;
            case 69 :
                // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:1:659: SIMULATE
                {
                mSIMULATE(); 

                }
                break;
            case 70 :
                // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:1:668: READFILE
                {
                mREADFILE(); 

                }
                break;
            case 71 :
                // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:1:677: FILE
                {
                mFILE(); 

                }
                break;
            case 72 :
                // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:1:682: WRITENET
                {
                mWRITENET(); 

                }
                break;
            case 73 :
                // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:1:691: WRITESBML
                {
                mWRITESBML(); 

                }
                break;
            case 74 :
                // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:1:701: WRITEMFILE
                {
                mWRITEMFILE(); 

                }
                break;
            case 75 :
                // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:1:712: WRITEXML
                {
                mWRITEXML(); 

                }
                break;
            case 76 :
                // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:1:721: WRITEMEXFILE
                {
                mWRITEMEXFILE(); 

                }
                break;
            case 77 :
                // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:1:734: PRINT_CDAT
                {
                mPRINT_CDAT(); 

                }
                break;
            case 78 :
                // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:1:745: PRINT_FUNCTIONS
                {
                mPRINT_FUNCTIONS(); 

                }
                break;
            case 79 :
                // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:1:761: SETCONCENTRATION
                {
                mSETCONCENTRATION(); 

                }
                break;
            case 80 :
                // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:1:778: METHOD
                {
                mMETHOD(); 

                }
                break;
            case 81 :
                // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:1:785: SETPARAMETER
                {
                mSETPARAMETER(); 

                }
                break;
            case 82 :
                // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:1:798: SAVECONCENTRATIONS
                {
                mSAVECONCENTRATIONS(); 

                }
                break;
            case 83 :
                // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:1:817: RESETCONCENTRATIONS
                {
                mRESETCONCENTRATIONS(); 

                }
                break;
            case 84 :
                // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:1:837: ADDCONCENTRATION
                {
                mADDCONCENTRATION(); 

                }
                break;
            case 85 :
                // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:1:854: GENERATENETWORK
                {
                mGENERATENETWORK(); 

                }
                break;
            case 86 :
                // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:1:870: WRITENETWORK
                {
                mWRITENETWORK(); 

                }
                break;
            case 87 :
                // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:1:883: SUBSTANCEUNITS
                {
                mSUBSTANCEUNITS(); 

                }
                break;
            case 88 :
                // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:1:898: PARAM
                {
                mPARAM(); 

                }
                break;
            case 89 :
                // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:1:904: COMPLEX
                {
                mCOMPLEX(); 

                }
                break;
            case 90 :
                // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:1:912: GET_FINAL_STATE
                {
                mGET_FINAL_STATE(); 

                }
                break;
            case 91 :
                // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:1:928: SAT
                {
                mSAT(); 

                }
                break;
            case 92 :
                // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:1:932: EXP
                {
                mEXP(); 

                }
                break;
            case 93 :
                // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:1:936: LOG
                {
                mLOG(); 

                }
                break;
            case 94 :
                // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:1:940: ARGFILE
                {
                mARGFILE(); 

                }
                break;
            case 95 :
                // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:1:948: SAVE_PROGRESS
                {
                mSAVE_PROGRESS(); 

                }
                break;
            case 96 :
                // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:1:962: NETFILE
                {
                mNETFILE(); 

                }
                break;
            case 97 :
                // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:1:970: PRINT_NET
                {
                mPRINT_NET(); 

                }
                break;
            case 98 :
                // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:1:980: PRINT_END
                {
                mPRINT_END(); 

                }
                break;
            case 99 :
                // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:1:990: TEXTREACTION
                {
                mTEXTREACTION(); 

                }
                break;
            case 100 :
                // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:1:1003: CONTINUE
                {
                mCONTINUE(); 

                }
                break;
            case 101 :
                // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:1:1012: ODE
                {
                mODE(); 

                }
                break;
            case 102 :
                // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:1:1016: SSA
                {
                mSSA(); 

                }
                break;
            case 103 :
                // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:1:1020: NF
                {
                mNF(); 

                }
                break;
            case 104 :
                // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:1:1023: PLA
                {
                mPLA(); 

                }
                break;
            case 105 :
                // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:1:1027: EVALUATE_EXPRESSIONS
                {
                mEVALUATE_EXPRESSIONS(); 

                }
                break;
            case 106 :
                // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:1:1048: BDF
                {
                mBDF(); 

                }
                break;
            case 107 :
                // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:1:1052: FLOAT
                {
                mFLOAT(); 

                }
                break;
            case 108 :
                // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:1:1058: INT
                {
                mINT(); 

                }
                break;
            case 109 :
                // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:1:1062: STRING
                {
                mSTRING(); 

                }
                break;
            case 110 :
                // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:1:1069: SEMI
                {
                mSEMI(); 

                }
                break;
            case 111 :
                // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:1:1074: COLON
                {
                mCOLON(); 

                }
                break;
            case 112 :
                // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:1:1080: LSBRACKET
                {
                mLSBRACKET(); 

                }
                break;
            case 113 :
                // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:1:1090: RSBRACKET
                {
                mRSBRACKET(); 

                }
                break;
            case 114 :
                // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:1:1100: LBRACKET
                {
                mLBRACKET(); 

                }
                break;
            case 115 :
                // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:1:1109: RBRACKET
                {
                mRBRACKET(); 

                }
                break;
            case 116 :
                // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:1:1118: COMMA
                {
                mCOMMA(); 

                }
                break;
            case 117 :
                // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:1:1124: DOT
                {
                mDOT(); 

                }
                break;
            case 118 :
                // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:1:1128: LPAREN
                {
                mLPAREN(); 

                }
                break;
            case 119 :
                // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:1:1135: RPAREN
                {
                mRPAREN(); 

                }
                break;
            case 120 :
                // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:1:1142: UNI_REACTION_SIGN
                {
                mUNI_REACTION_SIGN(); 

                }
                break;
            case 121 :
                // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:1:1160: BI_REACTION_SIGN
                {
                mBI_REACTION_SIGN(); 

                }
                break;
            case 122 :
                // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:1:1177: DOLLAR
                {
                mDOLLAR(); 

                }
                break;
            case 123 :
                // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:1:1184: TILDE
                {
                mTILDE(); 

                }
                break;
            case 124 :
                // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:1:1190: AT
                {
                mAT(); 

                }
                break;
            case 125 :
                // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:1:1193: GTE
                {
                mGTE(); 

                }
                break;
            case 126 :
                // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:1:1197: GT
                {
                mGT(); 

                }
                break;
            case 127 :
                // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:1:1200: LTE
                {
                mLTE(); 

                }
                break;
            case 128 :
                // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:1:1204: LT
                {
                mLT(); 

                }
                break;
            case 129 :
                // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:1:1207: ASSIGNS
                {
                mASSIGNS(); 

                }
                break;
            case 130 :
                // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:1:1215: EQUALS
                {
                mEQUALS(); 

                }
                break;
            case 131 :
                // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:1:1222: BECOMES
                {
                mBECOMES(); 

                }
                break;
            case 132 :
                // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:1:1230: DIV
                {
                mDIV(); 

                }
                break;
            case 133 :
                // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:1:1234: TIMES
                {
                mTIMES(); 

                }
                break;
            case 134 :
                // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:1:1240: MINUS
                {
                mMINUS(); 

                }
                break;
            case 135 :
                // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:1:1246: PLUS
                {
                mPLUS(); 

                }
                break;
            case 136 :
                // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:1:1251: POWER
                {
                mPOWER(); 

                }
                break;
            case 137 :
                // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:1:1257: MOD
                {
                mMOD(); 

                }
                break;
            case 138 :
                // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:1:1261: PIPE
                {
                mPIPE(); 

                }
                break;
            case 139 :
                // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:1:1266: QMARK
                {
                mQMARK(); 

                }
                break;
            case 140 :
                // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:1:1272: EMARK
                {
                mEMARK(); 

                }
                break;
            case 141 :
                // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:1:1278: DBQUOTES
                {
                mDBQUOTES(); 

                }
                break;
            case 142 :
                // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:1:1287: AMPERSAND
                {
                mAMPERSAND(); 

                }
                break;
            case 143 :
                // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:1:1297: VERSION_NUMBER
                {
                mVERSION_NUMBER(); 

                }
                break;

        }

    }


    protected DFA12 dfa12 = new DFA12(this);
    protected DFA20 dfa20 = new DFA20(this);
    protected DFA21 dfa21 = new DFA21(this);
    static final String DFA12_eotS =
        "\4\uffff";
    static final String DFA12_eofS =
        "\4\uffff";
    static final String DFA12_minS =
        "\1\60\1\56\2\uffff";
    static final String DFA12_maxS =
        "\1\71\1\145\2\uffff";
    static final String DFA12_acceptS =
        "\2\uffff\1\1\1\2";
    static final String DFA12_specialS =
        "\4\uffff}>";
    static final String[] DFA12_transitionS = {
            "\12\1",
            "\1\2\1\uffff\12\1\13\uffff\1\3\37\uffff\1\3",
            "",
            ""
    };

    static final short[] DFA12_eot = DFA.unpackEncodedString(DFA12_eotS);
    static final short[] DFA12_eof = DFA.unpackEncodedString(DFA12_eofS);
    static final char[] DFA12_min = DFA.unpackEncodedStringToUnsignedChars(DFA12_minS);
    static final char[] DFA12_max = DFA.unpackEncodedStringToUnsignedChars(DFA12_maxS);
    static final short[] DFA12_accept = DFA.unpackEncodedString(DFA12_acceptS);
    static final short[] DFA12_special = DFA.unpackEncodedString(DFA12_specialS);
    static final short[][] DFA12_transition;

    static {
        int numStates = DFA12_transitionS.length;
        DFA12_transition = new short[numStates][];
        for (int i=0; i<numStates; i++) {
            DFA12_transition[i] = DFA.unpackEncodedString(DFA12_transitionS[i]);
        }
    }

    class DFA12 extends DFA {

        public DFA12(BaseRecognizer recognizer) {
            this.recognizer = recognizer;
            this.decisionNumber = 12;
            this.eot = DFA12_eot;
            this.eof = DFA12_eof;
            this.min = DFA12_min;
            this.max = DFA12_max;
            this.accept = DFA12_accept;
            this.special = DFA12_special;
            this.transition = DFA12_transition;
        }
        public String getDescription() {
            return "138:1: FLOAT : ( ( DIGIT )+ '.' ( DIGIT )* ( EXPONENT )? | ( DIGIT )+ EXPONENT );";
        }
    }
    static final String DFA20_eotS =
        "\1\uffff\1\2\2\uffff\1\5\2\uffff";
    static final String DFA20_eofS =
        "\7\uffff";
    static final String DFA20_minS =
        "\1\60\1\56\1\uffff\1\60\1\56\2\uffff";
    static final String DFA20_maxS =
        "\2\71\1\uffff\2\71\2\uffff";
    static final String DFA20_acceptS =
        "\2\uffff\1\1\2\uffff\1\2\1\3";
    static final String DFA20_specialS =
        "\7\uffff}>";
    static final String[] DFA20_transitionS = {
            "\12\1",
            "\1\3\1\uffff\12\1",
            "",
            "\12\4",
            "\1\6\1\uffff\12\4",
            "",
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
            return "186:1: VERSION_NUMBER : ( ( INT ) | ( INT '.' INT ) | ( INT '.' INT '.' INT ) );";
        }
    }
    static final String DFA21_eotS =
        "\4\uffff\42\47\1\u0089\13\uffff\1\u008d\1\u0090\3\uffff\1\u0092"+
        "\1\u0095\12\uffff\67\47\1\u00d7\1\u00d8\1\u00d9\10\47\1\u00e3\3"+
        "\47\1\uffff\1\u00e8\1\47\12\uffff\1\47\1\u00ec\1\47\1\u00ee\2\47"+
        "\1\u00f1\12\47\1\u00fe\26\47\1\u0115\4\47\1\u011a\3\47\1\u011f\6"+
        "\47\1\u0126\1\u0127\7\47\3\uffff\5\47\1\u0134\3\47\1\uffff\2\47"+
        "\1\u013a\1\u00e8\1\uffff\1\u00e8\2\47\1\uffff\1\47\1\uffff\2\47"+
        "\1\uffff\1\u0141\13\47\1\uffff\4\47\1\u0152\12\47\1\u015e\1\47\1"+
        "\u0160\4\47\1\uffff\1\u0165\3\47\1\uffff\4\47\1\uffff\6\47\2\uffff"+
        "\4\47\1\u0177\7\47\1\uffff\5\47\2\uffff\4\47\1\u0188\1\uffff\17"+
        "\47\1\u0198\1\uffff\4\47\1\u019d\1\u019e\4\47\1\u01a3\1\uffff\1"+
        "\47\1\uffff\4\47\1\uffff\4\47\1\u01ae\14\47\1\uffff\3\47\1\u01bd"+
        "\2\47\1\u01c0\11\47\1\uffff\4\47\1\u01d2\1\u01d3\6\47\1\u01da\2"+
        "\47\1\uffff\3\47\1\u01e0\2\uffff\4\47\1\uffff\12\47\1\uffff\6\47"+
        "\1\u01d3\7\47\1\uffff\1\47\1\u0200\1\uffff\10\47\1\u020b\1\47\1"+
        "\u020c\5\47\1\u0212\2\uffff\6\47\1\uffff\2\47\1\u021b\2\47\1\uffff"+
        "\1\u021e\5\47\1\u0224\1\47\1\u0226\15\47\1\u0212\3\47\1\u0238\1"+
        "\47\1\u023a\2\47\1\uffff\1\u023d\1\47\1\u023f\7\47\2\uffff\5\47"+
        "\1\uffff\5\47\1\u0253\1\47\1\u0255\1\uffff\1\u0257\1\47\1\uffff"+
        "\1\u0259\1\u025b\3\47\1\uffff\1\u025f\1\uffff\21\47\1\uffff\1\47"+
        "\1\uffff\2\47\1\uffff\1\47\1\uffff\1\47\1\u0277\4\47\1\u027c\3\47"+
        "\1\u0280\10\47\1\uffff\1\47\1\uffff\1\u028d\1\uffff\1\47\1\uffff"+
        "\1\u028f\1\uffff\1\47\1\u0291\1\47\1\uffff\1\47\1\u0294\6\47\1\u029b"+
        "\1\u029c\1\47\1\u029e\1\u029f\3\47\1\u02a4\6\47\1\uffff\1\47\1\u02ac"+
        "\2\47\1\uffff\3\47\1\uffff\14\47\1\uffff\1\47\1\uffff\1\47\1\uffff"+
        "\2\47\1\uffff\1\47\1\u02c3\1\u02c4\1\u02c5\1\u02c6\1\47\2\uffff"+
        "\1\47\2\uffff\4\47\1\uffff\7\47\1\uffff\1\u02d4\15\47\1\u02e2\5"+
        "\47\1\u02e8\1\47\4\uffff\15\47\1\uffff\5\47\1\u02fc\1\47\1\u02fe"+
        "\1\u02ff\2\47\1\u0302\1\u0303\1\uffff\1\u0304\3\47\1\u0308\1\uffff"+
        "\3\47\1\u030c\10\47\1\u0315\1\u0316\1\u0317\4\47\1\uffff\1\47\2"+
        "\uffff\1\47\1\u031e\3\uffff\1\47\1\u0320\1\47\1\uffff\2\47\1\u0324"+
        "\1\uffff\10\47\3\uffff\4\47\1\u0331\1\47\1\uffff\1\47\1\uffff\3"+
        "\47\1\uffff\7\47\1\u033e\4\47\1\uffff\1\47\1\u0344\2\47\1\u0347"+
        "\2\47\1\u034a\3\47\1\u034e\1\uffff\1\47\1\u0350\1\47\1\u0352\1\47"+
        "\1\uffff\2\47\1\uffff\1\47\1\u0357\1\uffff\1\u0358\1\47\1\u035a"+
        "\1\uffff\1\u035b\1\uffff\1\47\1\uffff\4\47\2\uffff\1\u0361\2\uffff"+
        "\1\47\1\u0363\3\47\1\uffff\1\47\1\uffff\1\u0368\2\47\1\u036b\1\uffff"+
        "\1\u036c\1\47\2\uffff\1\u036e\1\uffff";
    static final String DFA21_eofS =
        "\u036f\uffff";
    static final String DFA21_minS =
        "\1\11\3\uffff\1\105\1\116\3\105\1\101\1\131\1\105\1\125\1\117\1"+
        "\103\1\142\2\101\1\105\1\122\1\101\1\103\1\122\1\104\1\146\1\101"+
        "\1\145\1\156\1\165\1\137\1\116\1\105\1\131\1\125\1\162\1\117\1\157"+
        "\1\105\1\56\13\uffff\1\76\1\55\3\uffff\2\75\12\uffff\1\122\1\104"+
        "\1\143\1\141\1\107\1\146\3\105\1\142\1\145\1\105\2\155\1\141\1\117"+
        "\1\116\1\104\2\130\1\164\1\120\1\145\1\101\1\114\1\157\1\101\1\116"+
        "\1\154\2\115\1\124\1\104\1\164\1\163\1\145\1\164\1\145\1\160\1\122"+
        "\1\141\1\151\1\122\1\104\2\130\1\105\1\106\1\164\1\154\1\156\1\105"+
        "\1\157\1\144\1\147\3\60\1\143\1\102\1\154\2\141\1\164\1\157\1\164"+
        "\1\60\1\170\1\151\1\147\1\uffff\1\60\1\53\12\uffff\1\102\1\60\1"+
        "\154\1\60\1\154\1\111\1\60\1\104\2\103\1\162\1\146\1\163\1\141\1"+
        "\160\1\145\1\165\1\60\1\125\3\105\1\163\2\137\1\150\1\105\1\156"+
        "\1\164\2\103\1\145\1\105\1\154\1\103\1\145\2\120\1\164\1\111\1\60"+
        "\1\157\1\145\1\162\1\160\1\60\1\165\2\101\1\60\1\156\1\145\1\123"+
        "\1\143\1\103\1\106\2\60\1\145\1\137\1\116\1\106\1\154\1\103\1\146"+
        "\3\uffff\1\154\1\105\1\145\1\165\1\163\1\60\1\164\1\165\1\146\1"+
        "\uffff\2\164\1\60\1\56\1\uffff\1\60\1\111\1\117\1\uffff\1\165\1"+
        "\uffff\1\165\1\116\1\uffff\1\60\1\160\1\157\1\141\1\111\1\163\1"+
        "\151\1\164\1\144\1\154\1\103\1\154\1\uffff\1\120\1\122\1\103\1\114"+
        "\1\60\2\111\1\157\1\123\1\144\1\141\1\124\1\106\1\164\1\123\1\60"+
        "\1\124\1\60\2\101\1\151\1\117\1\uffff\1\60\1\162\1\167\1\165\1\uffff"+
        "\1\154\2\115\1\143\1\uffff\1\164\1\103\1\124\1\150\2\111\2\uffff"+
        "\1\162\1\146\1\124\1\106\1\60\1\157\1\151\1\165\1\114\2\164\1\171"+
        "\1\uffff\1\145\1\164\1\151\1\122\1\145\2\uffff\1\117\1\123\1\144"+
        "\1\141\1\60\1\uffff\1\164\1\156\1\162\1\105\1\145\1\170\1\141\1"+
        "\171\1\145\1\157\1\160\1\141\1\123\1\101\1\154\1\60\1\uffff\1\147"+
        "\1\124\1\151\1\144\2\60\1\162\1\111\1\151\1\103\1\60\1\uffff\1\111"+
        "\1\uffff\1\122\1\145\1\156\1\116\1\uffff\1\166\1\162\1\164\1\141"+
        "\1\60\1\105\1\157\1\137\1\157\2\117\1\105\1\130\1\141\1\151\2\111"+
        "\1\uffff\1\156\1\154\1\144\1\60\1\145\1\171\1\60\2\160\1\154\1\145"+
        "\1\115\1\116\1\105\1\145\1\164\1\uffff\1\151\1\143\1\141\1\123\2"+
        "\60\1\156\2\137\1\156\1\162\1\164\1\60\1\124\1\114\1\uffff\1\147"+
        "\1\105\1\155\1\60\2\uffff\1\164\1\117\1\154\1\157\1\uffff\1\117"+
        "\1\124\1\170\1\165\1\123\1\141\1\151\1\137\1\164\1\124\1\uffff\1"+
        "\156\1\103\1\156\1\111\1\156\1\123\1\60\1\164\1\156\1\124\1\130"+
        "\1\143\2\145\1\uffff\1\115\1\60\1\uffff\1\163\1\165\1\145\1\141"+
        "\1\105\1\102\1\145\1\115\1\60\1\40\1\60\1\137\1\145\1\157\1\145"+
        "\1\155\1\60\2\uffff\1\143\1\163\1\164\1\143\1\157\1\145\1\uffff"+
        "\2\105\1\60\1\122\1\137\1\uffff\1\60\1\116\1\145\1\156\1\116\1\115"+
        "\1\60\1\145\1\60\1\142\1\164\1\163\1\151\1\105\1\146\1\104\1\165"+
        "\1\145\2\156\1\103\1\143\1\60\1\145\1\141\1\105\1\60\1\145\1\60"+
        "\1\137\1\157\1\uffff\1\60\1\164\1\60\1\143\1\124\1\164\1\115\1\151"+
        "\1\170\1\114\2\uffff\1\160\1\137\2\156\1\145\1\uffff\1\145\1\164"+
        "\1\151\1\145\1\147\1\60\1\116\1\60\1\uffff\1\60\1\163\1\uffff\2"+
        "\60\1\143\1\123\1\105\1\uffff\1\60\1\uffff\1\154\1\145\1\164\1\157"+
        "\1\122\1\151\1\101\1\156\1\164\1\144\1\145\1\110\1\145\1\101\1\137"+
        "\1\154\1\122\1\uffff\1\156\1\uffff\1\160\1\154\1\uffff\1\137\1\uffff"+
        "\1\164\1\60\1\167\1\114\1\154\1\106\1\60\1\145\1\162\1\145\1\60"+
        "\2\164\1\125\1\141\1\155\1\156\1\162\1\156\1\uffff\1\105\1\uffff"+
        "\1\60\1\uffff\1\164\1\uffff\1\60\1\uffff\1\145\1\60\1\116\1\uffff"+
        "\1\145\1\60\1\145\1\156\1\123\1\147\1\124\1\143\2\60\1\143\2\60"+
        "\1\102\1\150\1\137\1\60\1\164\1\145\1\162\1\145\1\163\1\151\1\uffff"+
        "\1\157\1\60\1\145\1\151\1\uffff\1\141\1\157\1\170\1\uffff\1\162"+
        "\1\145\1\156\1\164\1\145\1\164\1\145\1\144\1\163\1\146\1\154\1\124"+
        "\1\uffff\1\145\1\uffff\1\156\1\uffff\1\124\1\163\1\uffff\1\160\4"+
        "\60\1\164\2\uffff\1\164\2\uffff\1\105\1\171\1\145\1\163\1\uffff"+
        "\1\162\1\141\1\157\1\143\1\164\1\157\1\162\1\uffff\1\60\1\154\1"+
        "\143\1\144\1\160\1\141\1\162\1\151\1\145\1\163\1\162\1\163\1\145"+
        "\1\141\1\60\1\141\1\127\1\160\1\164\1\123\1\60\1\137\4\uffff\1\151"+
        "\1\145\1\114\1\142\2\164\1\141\1\143\1\144\1\165\1\145\1\156\1\153"+
        "\1\uffff\1\145\1\164\1\165\1\162\1\164\1\60\1\164\2\60\1\141\1\163"+
        "\2\60\1\uffff\1\60\1\117\1\163\1\162\1\60\1\uffff\1\151\1\157\1"+
        "\144\1\60\1\162\1\167\1\141\2\164\1\165\1\154\1\160\3\60\1\141\1"+
        "\143\1\145\1\151\1\uffff\1\163\2\uffff\1\164\1\60\3\uffff\1\122"+
        "\1\60\1\141\1\uffff\2\156\1\60\1\uffff\1\151\1\157\1\164\1\151\1"+
        "\141\1\143\1\145\1\163\3\uffff\1\156\1\164\1\163\1\157\1\60\1\151"+
        "\1\uffff\1\113\1\uffff\2\164\1\163\1\uffff\1\144\1\162\1\145\1\157"+
        "\1\156\1\164\1\163\1\60\1\164\2\163\1\156\1\uffff\1\157\1\60\1\151"+
        "\1\145\1\60\1\137\1\153\1\60\1\156\1\164\1\163\1\60\1\uffff\1\163"+
        "\1\60\1\151\1\60\1\156\1\uffff\1\157\1\162\1\uffff\1\155\1\60\1"+
        "\uffff\1\60\1\163\1\60\1\uffff\1\60\1\uffff\1\157\1\uffff\1\163"+
        "\1\156\1\166\1\157\2\uffff\1\60\2\uffff\1\156\1\60\1\163\1\141\1"+
        "\144\1\uffff\1\163\1\uffff\1\60\1\154\1\145\1\60\1\uffff\1\60\1"+
        "\154\2\uffff\1\60\1\uffff";
    static final String DFA21_maxS =
        "\1\176\3\uffff\1\145\1\170\1\145\1\165\1\162\1\157\1\171\2\165\1"+
        "\157\1\165\1\166\1\162\1\157\1\160\1\162\1\141\1\164\1\122\1\104"+
        "\1\156\1\101\1\145\1\156\1\165\1\157\1\156\1\165\1\171\1\165\1\162"+
        "\2\157\1\145\1\172\13\uffff\1\76\1\75\3\uffff\1\75\1\76\12\uffff"+
        "\1\162\1\144\1\160\1\141\1\147\1\146\1\164\2\145\1\146\2\145\1\166"+
        "\1\155\1\141\1\157\1\116\1\154\2\170\1\164\1\160\2\163\1\154\1\157"+
        "\1\141\1\156\1\154\1\156\1\155\1\164\1\104\1\164\1\163\1\145\1\164"+
        "\1\145\1\160\1\162\1\141\1\151\1\162\1\166\2\170\1\145\1\106\1\164"+
        "\1\154\1\164\1\111\1\157\1\144\1\147\3\172\1\143\1\102\1\154\2\141"+
        "\1\164\1\163\1\164\1\172\1\170\1\151\1\147\1\uffff\2\71\12\uffff"+
        "\1\163\1\172\1\154\1\172\1\154\1\151\1\172\1\144\1\120\1\143\1\162"+
        "\1\146\1\163\1\141\1\160\1\145\1\165\1\172\1\165\1\105\2\145\1\163"+
        "\2\137\1\150\1\145\1\156\1\164\1\144\1\143\2\145\1\154\1\143\1\145"+
        "\2\160\1\164\1\151\1\172\1\157\1\145\1\162\1\160\1\172\1\165\2\141"+
        "\1\172\1\156\1\145\1\137\2\143\1\106\2\172\1\145\1\137\1\116\1\106"+
        "\1\154\1\103\1\146\3\uffff\1\154\1\105\1\145\1\165\1\163\1\172\1"+
        "\164\1\165\1\146\1\uffff\2\164\1\172\1\71\1\uffff\1\172\1\151\1"+
        "\157\1\uffff\1\165\1\uffff\1\165\1\156\1\uffff\1\172\1\160\1\157"+
        "\1\141\1\151\1\163\1\151\1\164\1\144\1\154\1\137\1\154\1\uffff\1"+
        "\160\1\122\1\143\1\154\1\172\1\163\1\151\1\157\1\163\1\144\1\141"+
        "\1\164\1\106\1\164\1\163\1\172\1\164\1\172\1\154\1\141\1\151\1\157"+
        "\1\uffff\1\172\1\162\1\167\1\165\1\uffff\1\154\2\155\1\143\1\uffff"+
        "\1\164\1\103\1\124\1\150\1\151\1\111\2\uffff\1\162\1\146\1\124\1"+
        "\106\1\172\1\157\1\151\1\165\1\114\2\164\1\171\1\uffff\1\145\1\164"+
        "\1\151\1\122\1\145\2\uffff\1\157\1\163\1\144\1\141\1\172\1\uffff"+
        "\1\164\1\156\1\162\2\145\1\170\1\141\1\171\1\145\1\157\1\160\1\141"+
        "\1\163\1\101\1\165\1\172\1\uffff\1\147\1\164\1\151\1\144\2\172\1"+
        "\162\2\151\1\103\1\172\1\uffff\1\151\1\uffff\1\162\1\145\2\156\1"+
        "\uffff\1\166\1\162\1\164\1\141\1\172\1\145\1\157\1\137\1\157\2\117"+
        "\1\145\1\130\1\141\1\151\2\111\1\uffff\1\156\1\154\1\144\1\172\1"+
        "\145\1\171\1\172\2\160\1\154\1\145\1\130\1\156\2\145\1\164\1\uffff"+
        "\1\151\1\143\1\141\1\163\2\172\1\156\2\137\1\156\1\162\1\164\1\172"+
        "\1\124\1\154\1\uffff\1\147\1\145\1\155\1\172\2\uffff\1\164\1\157"+
        "\1\154\1\157\1\uffff\1\157\1\164\1\170\1\165\1\163\1\141\1\151\1"+
        "\137\2\164\1\uffff\3\156\1\111\1\156\1\163\1\172\1\164\1\156\1\124"+
        "\1\130\1\143\2\145\1\uffff\1\115\1\172\1\uffff\1\163\1\165\1\145"+
        "\1\141\1\145\1\102\1\146\1\115\1\172\1\40\1\172\1\137\1\145\1\157"+
        "\1\145\1\155\1\172\2\uffff\1\143\1\163\1\164\1\143\1\157\1\145\1"+
        "\uffff\1\105\1\145\1\172\1\162\1\137\1\uffff\1\172\1\156\1\145\2"+
        "\156\1\155\1\172\1\145\1\172\1\142\1\164\1\163\1\151\1\145\1\146"+
        "\1\104\1\165\1\145\2\156\1\103\1\143\1\172\1\145\1\141\1\105\1\172"+
        "\1\145\1\172\1\137\1\157\1\uffff\1\172\1\164\1\172\1\143\1\124\1"+
        "\164\1\115\1\151\1\170\1\114\2\uffff\1\162\1\137\2\156\1\145\1\uffff"+
        "\1\145\1\164\1\151\1\145\1\147\1\172\1\116\1\172\1\uffff\1\172\1"+
        "\163\1\uffff\2\172\1\143\1\163\1\145\1\uffff\1\172\1\uffff\1\154"+
        "\1\145\1\164\1\157\1\162\1\151\1\101\1\156\1\164\1\144\1\145\1\110"+
        "\1\145\1\101\1\137\1\154\1\122\1\uffff\1\156\1\uffff\1\162\1\154"+
        "\1\uffff\1\137\1\uffff\1\164\1\172\1\167\1\114\1\154\1\146\1\172"+
        "\1\145\1\162\1\145\1\172\2\164\1\125\1\141\1\155\1\156\1\162\1\163"+
        "\1\uffff\1\105\1\uffff\1\172\1\uffff\1\164\1\uffff\1\172\1\uffff"+
        "\1\145\1\172\1\156\1\uffff\1\145\1\172\1\145\1\156\1\163\1\147\1"+
        "\124\1\143\2\172\1\143\2\172\1\102\1\156\1\137\1\172\1\164\1\145"+
        "\1\162\1\145\1\163\1\151\1\uffff\1\157\1\172\1\145\1\151\1\uffff"+
        "\1\141\1\157\1\170\1\uffff\1\162\1\145\1\156\1\164\1\145\1\164\1"+
        "\145\1\144\1\163\1\146\1\154\1\124\1\uffff\1\145\1\uffff\1\156\1"+
        "\uffff\1\164\1\163\1\uffff\1\160\4\172\1\164\2\uffff\1\164\2\uffff"+
        "\1\105\1\171\1\145\1\163\1\uffff\1\162\1\141\1\157\1\143\1\164\1"+
        "\157\1\162\1\uffff\1\172\1\154\1\143\1\144\1\160\1\141\1\162\1\151"+
        "\1\145\1\163\1\162\1\163\1\145\1\141\1\172\1\141\1\127\1\160\1\164"+
        "\1\163\1\172\1\137\4\uffff\1\151\1\145\1\114\1\142\2\164\1\141\1"+
        "\143\1\144\1\165\1\145\1\156\1\153\1\uffff\1\145\1\164\1\165\1\162"+
        "\1\164\1\172\1\164\2\172\1\141\1\163\2\172\1\uffff\1\172\1\117\1"+
        "\163\1\162\1\172\1\uffff\1\151\1\157\1\144\1\172\1\162\1\167\1\141"+
        "\2\164\1\165\1\154\1\160\3\172\1\141\1\143\1\145\1\151\1\uffff\1"+
        "\163\2\uffff\1\164\1\172\3\uffff\1\122\1\172\1\141\1\uffff\2\156"+
        "\1\172\1\uffff\1\151\1\157\1\164\1\151\1\141\1\143\1\145\1\163\3"+
        "\uffff\1\156\1\164\1\163\1\157\1\172\1\151\1\uffff\1\113\1\uffff"+
        "\2\164\1\163\1\uffff\1\144\1\162\1\145\1\157\1\156\1\164\1\163\1"+
        "\172\1\164\2\163\1\156\1\uffff\1\157\1\172\1\151\1\145\1\172\1\137"+
        "\1\153\1\172\1\156\1\164\1\163\1\172\1\uffff\1\163\1\172\1\151\1"+
        "\172\1\156\1\uffff\1\157\1\162\1\uffff\1\155\1\172\1\uffff\1\172"+
        "\1\163\1\172\1\uffff\1\172\1\uffff\1\157\1\uffff\1\163\1\156\1\166"+
        "\1\157\2\uffff\1\172\2\uffff\1\156\1\172\1\163\1\141\1\144\1\uffff"+
        "\1\163\1\uffff\1\172\1\154\1\145\1\172\1\uffff\1\172\1\154\2\uffff"+
        "\1\172\1\uffff";
    static final String DFA21_acceptS =
        "\1\uffff\1\1\1\2\1\3\43\uffff\1\155\1\156\1\157\1\160\1\161\1\162"+
        "\1\163\1\164\1\165\1\166\1\167\2\uffff\1\172\1\173\1\174\2\uffff"+
        "\1\u0084\1\u0085\1\u0087\1\u0088\1\u0089\1\u008a\1\u008b\1\u008c"+
        "\1\u008d\1\u008e\106\uffff\1\154\2\uffff\1\170\1\u0086\1\171\1\177"+
        "\1\u0080\1\175\1\176\1\u0081\1\u0082\1\u0083\101\uffff\1\40\1\41"+
        "\1\42\11\uffff\1\147\4\uffff\1\153\3\uffff\1\5\1\uffff\1\134\2\uffff"+
        "\1\152\14\uffff\1\146\26\uffff\1\37\4\uffff\1\145\4\uffff\1\150"+
        "\6\uffff\1\133\1\50\14\uffff\1\57\5\uffff\1\135\1\u008f\5\uffff"+
        "\1\7\20\uffff\1\26\13\uffff\1\66\1\uffff\1\107\4\uffff\1\54\21\uffff"+
        "\1\65\20\uffff\1\6\17\uffff\1\14\4\uffff\1\15\1\71\4\uffff\1\20"+
        "\12\uffff\1\130\16\uffff\1\45\2\uffff\1\56\21\uffff\1\70\1\44\6"+
        "\uffff\1\11\5\uffff\1\120\37\uffff\1\55\12\uffff\1\4\1\47\5\uffff"+
        "\1\10\10\uffff\1\32\2\uffff\1\72\5\uffff\1\131\1\uffff\1\23\21\uffff"+
        "\1\43\1\uffff\1\136\2\uffff\1\73\1\uffff\1\140\23\uffff\1\105\1"+
        "\uffff\1\12\1\uffff\1\33\1\uffff\1\16\1\uffff\1\106\3\uffff\1\144"+
        "\27\uffff\1\110\4\uffff\1\113\3\uffff\1\27\14\uffff\1\13\1\uffff"+
        "\1\17\1\uffff\1\21\2\uffff\1\35\6\uffff\1\141\1\142\1\uffff\1\34"+
        "\1\51\4\uffff\1\36\7\uffff\1\111\26\uffff\1\25\1\46\1\77\1\115\15"+
        "\uffff\1\112\15\uffff\1\103\5\uffff\1\24\23\uffff\1\121\1\uffff"+
        "\1\67\1\100\2\uffff\1\101\1\102\1\104\3\uffff\1\22\3\uffff\1\30"+
        "\10\uffff\1\143\1\126\1\114\6\uffff\1\137\1\uffff\1\75\3\uffff\1"+
        "\53\14\uffff\1\127\14\uffff\1\74\5\uffff\1\31\2\uffff\1\116\2\uffff"+
        "\1\132\3\uffff\1\52\1\uffff\1\63\1\uffff\1\117\4\uffff\1\125\1\124"+
        "\1\uffff\1\61\1\62\5\uffff\1\60\1\uffff\1\122\4\uffff\1\123\2\uffff"+
        "\1\151\1\76\1\uffff\1\64";
    static final String DFA21_specialS =
        "\u036f\uffff}>";
    static final String[] DFA21_transitionS = {
            "\1\3\1\2\2\uffff\1\2\22\uffff\1\3\1\100\1\101\1\1\1\64\1\75"+
            "\1\102\1\uffff\1\60\1\61\1\72\1\73\1\56\1\62\1\57\1\71\12\46"+
            "\1\51\1\50\1\63\1\70\1\67\1\77\1\66\1\16\1\45\1\43\1\32\1\36"+
            "\1\41\1\10\1\33\1\27\2\47\1\31\1\21\1\47\1\26\1\24\1\34\1\37"+
            "\1\22\1\40\1\47\1\4\4\47\1\52\1\3\1\53\1\74\1\47\1\uffff\1\25"+
            "\1\6\1\15\1\47\1\5\1\14\1\23\1\47\1\30\2\47\1\44\1\11\1\35\1"+
            "\17\1\20\1\47\1\13\1\7\1\12\1\47\1\4\1\42\3\47\1\54\1\76\1\55"+
            "\1\65",
            "",
            "",
            "",
            "\1\103\37\uffff\1\103",
            "\1\104\37\uffff\1\104\7\uffff\1\106\1\uffff\1\105",
            "\1\107\36\uffff\1\110\1\107",
            "\1\113\12\uffff\1\116\20\uffff\1\117\3\uffff\1\111\3\uffff"+
            "\1\120\6\uffff\1\112\2\uffff\1\121\1\115\1\114",
            "\1\123\14\uffff\1\122\37\uffff\1\122",
            "\1\126\15\uffff\1\124\21\uffff\1\125\3\uffff\1\127\11\uffff"+
            "\1\124",
            "\1\130\5\uffff\1\131\31\uffff\1\130",
            "\1\135\17\uffff\1\133\17\uffff\1\132\16\uffff\1\134\1\133",
            "\1\136\23\uffff\1\137\13\uffff\1\136",
            "\1\141\37\uffff\1\140",
            "\1\142\12\uffff\1\143\24\uffff\1\142\21\uffff\1\144",
            "\1\145\1\uffff\1\150\20\uffff\1\147\1\146",
            "\1\155\37\uffff\1\152\12\uffff\1\153\2\uffff\1\151\2\uffff"+
            "\1\154",
            "\1\157\15\uffff\1\124\21\uffff\1\160\15\uffff\1\156",
            "\1\113\12\uffff\1\161\4\uffff\1\162\13\uffff\1\163\3\uffff"+
            "\1\113\12\uffff\1\116",
            "\1\122\22\uffff\1\165\7\uffff\1\164\4\uffff\1\122",
            "\1\155\20\uffff\1\166\16\uffff\1\155",
            "\1\142\37\uffff\1\142\1\170\15\uffff\1\171\1\uffff\1\167",
            "\1\172",
            "\1\173",
            "\1\174\7\uffff\1\175",
            "\1\176",
            "\1\177",
            "\1\u0080",
            "\1\u0081",
            "\1\u0083\5\uffff\1\u0084\1\u0085\10\uffff\1\u0082",
            "\1\104\37\uffff\1\104",
            "\1\135\17\uffff\1\133\17\uffff\1\135\17\uffff\1\133",
            "\1\130\13\uffff\1\u0086\23\uffff\1\130",
            "\1\136\37\uffff\1\136",
            "\1\u0087",
            "\1\141\37\uffff\1\141",
            "\1\u0088",
            "\1\107\37\uffff\1\107",
            "\1\u008a\1\uffff\12\46\7\uffff\4\47\1\u008b\25\47\4\uffff\1"+
            "\47\1\uffff\4\47\1\u008b\25\47",
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
            "\1\u008c",
            "\1\u008e\17\uffff\1\u008f",
            "",
            "",
            "",
            "\1\u0091",
            "\1\u0094\1\u0093",
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
            "\1\u0096\37\uffff\1\u0096",
            "\1\u0097\37\uffff\1\u0097",
            "\1\u0098\14\uffff\1\u0099",
            "\1\u009a",
            "\1\u009b\37\uffff\1\u009b",
            "\1\u009c",
            "\1\u009d\37\uffff\1\u009d\16\uffff\1\u009e",
            "\1\u009f\33\uffff\1\u00a0\3\uffff\1\u009f",
            "\1\u009d\37\uffff\1\u009d",
            "\1\u00a2\3\uffff\1\u00a1",
            "\1\u00a3",
            "\1\u009f\37\uffff\1\u009f",
            "\1\u00a4\10\uffff\1\u00a5",
            "\1\u00a6",
            "\1\u00a7",
            "\1\u00a8\37\uffff\1\u00a8",
            "\1\u00a9",
            "\1\u00ab\7\uffff\1\u00aa\27\uffff\1\u00ab\7\uffff\1\u00aa",
            "\1\u00ae\27\uffff\1\u00ac\7\uffff\1\u00ad",
            "\1\u00ae\37\uffff\1\u00ae",
            "\1\u00af",
            "\1\u00b0\37\uffff\1\u00b0",
            "\1\u00b1\15\uffff\1\u00b2",
            "\1\u00b4\37\uffff\1\u00b3\21\uffff\1\u00b5",
            "\1\u00b6\37\uffff\1\u00b6",
            "\1\u00b7",
            "\1\u00b4\37\uffff\1\u00b4",
            "\1\u00b8\37\uffff\1\u00b8",
            "\1\u00b9",
            "\1\u00bb\37\uffff\1\u00ba\1\u00bc",
            "\1\u00bb\37\uffff\1\u00bb",
            "\1\u00bd\37\uffff\1\u00bd",
            "\1\u00be",
            "\1\u00bf",
            "\1\u00c0",
            "\1\u00c1",
            "\1\u00c2",
            "\1\u00c3",
            "\1\u00c4",
            "\1\u00c6\37\uffff\1\u00c5",
            "\1\u00c7",
            "\1\u00c8",
            "\1\u00c6\37\uffff\1\u00c6",
            "\1\u00ab\7\uffff\1\u00aa\27\uffff\1\u00ab\7\uffff\1\u00aa\11"+
            "\uffff\1\u00c9",
            "\1\u00ca\37\uffff\1\u00ae",
            "\1\u00ae\33\uffff\1\u00cb\3\uffff\1\u00ae",
            "\1\u00cc\37\uffff\1\u009f",
            "\1\u00cd",
            "\1\u00ce",
            "\1\u00cf",
            "\1\u00d0\5\uffff\1\u00d1",
            "\1\u00d3\3\uffff\1\u00d2",
            "\1\u00d4",
            "\1\u00d5",
            "\1\u00d6",
            "\12\47\7\uffff\32\47\4\uffff\1\47\1\uffff\32\47",
            "\12\47\7\uffff\32\47\4\uffff\1\47\1\uffff\32\47",
            "\12\47\7\uffff\32\47\4\uffff\1\47\1\uffff\32\47",
            "\1\u00da",
            "\1\u00db",
            "\1\u00dc",
            "\1\u00dd",
            "\1\u00de",
            "\1\u00df",
            "\1\u00e1\3\uffff\1\u00e0",
            "\1\u00e2",
            "\12\47\7\uffff\32\47\4\uffff\1\47\1\uffff\32\47",
            "\1\u00e4",
            "\1\u00e5",
            "\1\u00e6",
            "",
            "\12\u00e7",
            "\1\u00e8\1\uffff\1\u00e8\2\uffff\12\u00e9",
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
            "\1\u00eb\20\uffff\1\u00ea\16\uffff\1\u00eb\20\uffff\1\u00ea",
            "\12\47\7\uffff\32\47\4\uffff\1\47\1\uffff\32\47",
            "\1\u00ed",
            "\12\47\7\uffff\32\47\4\uffff\1\47\1\uffff\32\47",
            "\1\u00ef",
            "\1\u00f0\37\uffff\1\u00f0",
            "\12\47\7\uffff\32\47\4\uffff\1\47\1\uffff\32\47",
            "\1\u00f2\37\uffff\1\u00f2",
            "\1\u00f4\13\uffff\1\u00f3\1\u00f5",
            "\1\u00f6\37\uffff\1\u00f6",
            "\1\u00f7",
            "\1\u00f8",
            "\1\u00f9",
            "\1\u00fa",
            "\1\u00fb",
            "\1\u00fc",
            "\1\u00fd",
            "\12\47\7\uffff\32\47\4\uffff\1\47\1\uffff\32\47",
            "\1\u00ff\37\uffff\1\u00ff",
            "\1\u0100",
            "\1\u0101\37\uffff\1\u0101",
            "\1\u0102\37\uffff\1\u0102",
            "\1\u0103",
            "\1\u0104",
            "\1\u0105",
            "\1\u0106",
            "\1\u0107\37\uffff\1\u0107",
            "\1\u0108",
            "\1\u0109",
            "\1\u010a\37\uffff\1\u010a\1\u010b",
            "\1\u010a\37\uffff\1\u010a",
            "\1\u010c",
            "\1\u010d\37\uffff\1\u010d",
            "\1\u010e",
            "\1\u010f\37\uffff\1\u010f",
            "\1\u0110",
            "\1\u0112\37\uffff\1\u0111",
            "\1\u0112\37\uffff\1\u0112",
            "\1\u0113",
            "\1\u0114\37\uffff\1\u0114",
            "\12\47\7\uffff\32\47\4\uffff\1\47\1\uffff\32\47",
            "\1\u0116",
            "\1\u0117",
            "\1\u0118",
            "\1\u0119",
            "\12\47\7\uffff\32\47\4\uffff\1\47\1\uffff\32\47",
            "\1\u011b",
            "\1\u011d\37\uffff\1\u011c",
            "\1\u011d\37\uffff\1\u011d",
            "\12\47\7\uffff\32\47\4\uffff\1\u011e\1\uffff\32\47",
            "\1\u0120",
            "\1\u0121",
            "\1\u0122\13\uffff\1\u0105",
            "\1\u0123",
            "\1\u0124\37\uffff\1\u00f6",
            "\1\u0125",
            "\12\47\7\uffff\32\47\4\uffff\1\47\1\uffff\32\47",
            "\12\47\7\uffff\32\47\4\uffff\1\47\1\uffff\32\47",
            "\1\u0128",
            "\1\u0129",
            "\1\u012a",
            "\1\u012b",
            "\1\u012c",
            "\1\u012d",
            "\1\u012e",
            "",
            "",
            "",
            "\1\u012f",
            "\1\u0130",
            "\1\u0131",
            "\1\u0132",
            "\1\u0133",
            "\12\47\7\uffff\32\47\4\uffff\1\47\1\uffff\32\47",
            "\1\u0135",
            "\1\u0136",
            "\1\u0137",
            "",
            "\1\u0138",
            "\1\u0139",
            "\12\47\7\uffff\32\47\4\uffff\1\47\1\uffff\32\47",
            "\1\u013b\1\uffff\12\u00e7",
            "",
            "\12\u00e9\7\uffff\32\47\4\uffff\1\47\1\uffff\32\47",
            "\1\u013c\37\uffff\1\u013c",
            "\1\u013d\37\uffff\1\u013d",
            "",
            "\1\u013e",
            "",
            "\1\u013f",
            "\1\u0140\37\uffff\1\u0140",
            "",
            "\12\47\7\uffff\32\47\4\uffff\1\47\1\uffff\32\47",
            "\1\u0142",
            "\1\u0143",
            "\1\u0144",
            "\1\u0145\37\uffff\1\u0145",
            "\1\u0146",
            "\1\u0147",
            "\1\u0148",
            "\1\u0149",
            "\1\u014a",
            "\1\u014b\33\uffff\1\u014c",
            "\1\u014d",
            "",
            "\1\u014e\37\uffff\1\u014e",
            "\1\u014f",
            "\1\u0150\37\uffff\1\u0150",
            "\1\u0151\37\uffff\1\u0151",
            "\12\47\7\uffff\32\47\4\uffff\1\47\1\uffff\32\47",
            "\1\u0154\27\uffff\1\u0153\7\uffff\1\u0154\11\uffff\1\u0155",
            "\1\u0154\37\uffff\1\u0154",
            "\1\u0156",
            "\1\u0157\37\uffff\1\u0157",
            "\1\u0158",
            "\1\u0159",
            "\1\u015a\37\uffff\1\u015a",
            "\1\u015b",
            "\1\u015c",
            "\1\u015d\37\uffff\1\u015d",
            "\12\47\7\uffff\32\47\4\uffff\1\47\1\uffff\32\47",
            "\1\u015f\37\uffff\1\u015f",
            "\12\47\7\uffff\32\47\4\uffff\1\47\1\uffff\32\47",
            "\1\u0161\37\uffff\1\u0161\12\uffff\1\u0162",
            "\1\u0161\37\uffff\1\u0161",
            "\1\u0163",
            "\1\u0164\37\uffff\1\u0164",
            "",
            "\12\47\7\uffff\32\47\4\uffff\1\47\1\uffff\32\47",
            "\1\u0166",
            "\1\u0167",
            "\1\u0168",
            "",
            "\1\u0169",
            "\1\u016b\37\uffff\1\u016a",
            "\1\u016b\37\uffff\1\u016b",
            "\1\u016c",
            "",
            "\1\u016d",
            "\1\u016e",
            "\1\u016f",
            "\1\u0170",
            "\1\u0171\37\uffff\1\u0145",
            "\1\u0172",
            "",
            "",
            "\1\u0173",
            "\1\u0174",
            "\1\u0175",
            "\1\u0176",
            "\12\47\7\uffff\32\47\4\uffff\1\47\1\uffff\32\47",
            "\1\u0178",
            "\1\u0179",
            "\1\u017a",
            "\1\u017b",
            "\1\u017c",
            "\1\u017d",
            "\1\u017e",
            "",
            "\1\u017f",
            "\1\u0180",
            "\1\u0181",
            "\1\u0182",
            "\1\u0183",
            "",
            "",
            "\1\u0184\37\uffff\1\u0184",
            "\1\u0185\37\uffff\1\u0185",
            "\1\u0186",
            "\1\u0187",
            "\12\47\7\uffff\32\47\4\uffff\1\47\1\uffff\32\47",
            "",
            "\1\u0189",
            "\1\u018a",
            "\1\u018b",
            "\1\u018c\37\uffff\1\u018c",
            "\1\u018d",
            "\1\u018e",
            "\1\u018f",
            "\1\u0190",
            "\1\u0191",
            "\1\u0192",
            "\1\u0193",
            "\1\u0194",
            "\1\u0195\37\uffff\1\u0195",
            "\1\u0196",
            "\1\u0197\10\uffff\1\u0197",
            "\12\47\7\uffff\32\47\4\uffff\1\47\1\uffff\32\47",
            "",
            "\1\u0199",
            "\1\u019a\37\uffff\1\u019a",
            "\1\u019b",
            "\1\u019c",
            "\12\47\7\uffff\32\47\4\uffff\1\47\1\uffff\32\47",
            "\12\47\7\uffff\32\47\4\uffff\1\47\1\uffff\32\47",
            "\1\u019f",
            "\1\u01a0\37\uffff\1\u01a0",
            "\1\u01a1",
            "\1\u01a2",
            "\12\47\7\uffff\32\47\4\uffff\1\47\1\uffff\32\47",
            "",
            "\1\u01a4\37\uffff\1\u01a4",
            "",
            "\1\u01a5\37\uffff\1\u01a5",
            "\1\u01a6",
            "\1\u01a7",
            "\1\u01a8\37\uffff\1\u01a8",
            "",
            "\1\u01a9",
            "\1\u01aa",
            "\1\u01ab",
            "\1\u01ac",
            "\12\47\7\uffff\4\47\1\u01ad\25\47\4\uffff\1\47\1\uffff\4\47"+
            "\1\u01ad\25\47",
            "\1\u01ad\37\uffff\1\u01ad",
            "\1\u01af",
            "\1\u01b0",
            "\1\u01b1",
            "\1\u01b2",
            "\1\u01b3",
            "\1\u01b4\37\uffff\1\u018c",
            "\1\u01b5",
            "\1\u01b6",
            "\1\u01b7",
            "\1\u01b8",
            "\1\u01b9",
            "",
            "\1\u01ba",
            "\1\u01bb",
            "\1\u01bc",
            "\12\47\7\uffff\32\47\4\uffff\1\47\1\uffff\32\47",
            "\1\u01be",
            "\1\u01bf",
            "\12\47\7\uffff\32\47\4\uffff\1\47\1\uffff\32\47",
            "\1\u01c1",
            "\1\u01c2",
            "\1\u01c3",
            "\1\u01c4",
            "\1\u01c7\1\u01c5\4\uffff\1\u01c6\4\uffff\1\u01c8",
            "\1\u01c9\37\uffff\1\u01c9",
            "\1\u01ca\37\uffff\1\u01cb",
            "\1\u01cc",
            "\1\u01cd",
            "",
            "\1\u01ce",
            "\1\u01cf",
            "\1\u01d0",
            "\1\u01d1\37\uffff\1\u01d1",
            "\12\47\7\uffff\32\47\4\uffff\1\47\1\uffff\32\47",
            "\12\47\7\uffff\32\47\4\uffff\1\47\1\uffff\32\47",
            "\1\u01d4",
            "\1\u01d5",
            "\1\u01d6",
            "\1\u01d7",
            "\1\u01d8",
            "\1\u01d9",
            "\12\47\7\uffff\32\47\4\uffff\1\47\1\uffff\32\47",
            "\1\u01db",
            "\1\u01dc\37\uffff\1\u01dc",
            "",
            "\1\u01dd",
            "\1\u01de\37\uffff\1\u01de",
            "\1\u01df",
            "\12\47\7\uffff\32\47\4\uffff\1\47\1\uffff\32\47",
            "",
            "",
            "\1\u01e1",
            "\1\u01e2\37\uffff\1\u01e2",
            "\1\u01e3",
            "\1\u01e4",
            "",
            "\1\u01e5\37\uffff\1\u01e5",
            "\1\u01e6\37\uffff\1\u01e6",
            "\1\u01e7",
            "\1\u01e8",
            "\1\u01e9\37\uffff\1\u01e9",
            "\1\u01ea",
            "\1\u01eb",
            "\1\u01ec",
            "\1\u01ed",
            "\1\u01ee\37\uffff\1\u01ee",
            "",
            "\1\u01ef",
            "\1\u01f0\41\uffff\1\u01f3\1\u01f1\7\uffff\1\u01f2",
            "\1\u01f4",
            "\1\u01f5",
            "\1\u01f6",
            "\1\u01f7\37\uffff\1\u01d1",
            "\12\47\7\uffff\32\47\4\uffff\1\47\1\uffff\32\47",
            "\1\u01f8",
            "\1\u01f9",
            "\1\u01fa",
            "\1\u01fb",
            "\1\u01fc",
            "\1\u01fd",
            "\1\u01fe",
            "",
            "\1\u01ff",
            "\12\47\7\uffff\32\47\4\uffff\1\47\1\uffff\32\47",
            "",
            "\1\u0201",
            "\1\u0202",
            "\1\u0203",
            "\1\u0204",
            "\1\u0205\37\uffff\1\u0206",
            "\1\u0207",
            "\1\u0209\1\u0208",
            "\1\u020a",
            "\12\47\7\uffff\32\47\4\uffff\1\47\1\uffff\32\47",
            "\1\u020c",
            "\12\47\7\uffff\32\47\4\uffff\1\47\1\uffff\32\47",
            "\1\u020d",
            "\1\u020e",
            "\1\u020f",
            "\1\u0210",
            "\1\u0211",
            "\12\47\7\uffff\32\47\4\uffff\1\47\1\uffff\32\47",
            "",
            "",
            "\1\u0213",
            "\1\u0214",
            "\1\u0215",
            "\1\u0216",
            "\1\u0217",
            "\1\u0218",
            "",
            "\1\u0219",
            "\1\u021a\37\uffff\1\u021a",
            "\12\47\7\uffff\32\47\4\uffff\1\47\1\uffff\32\47",
            "\1\u021c\37\uffff\1\u021c",
            "\1\u021d",
            "",
            "\12\47\7\uffff\32\47\4\uffff\1\47\1\uffff\32\47",
            "\1\u021f\37\uffff\1\u021f",
            "\1\u0220",
            "\1\u0221",
            "\1\u0222\37\uffff\1\u0222",
            "\1\u0223\37\uffff\1\u0223",
            "\12\47\7\uffff\32\47\4\uffff\1\47\1\uffff\32\47",
            "\1\u0225",
            "\12\47\7\uffff\32\47\4\uffff\1\47\1\uffff\32\47",
            "\1\u0227",
            "\1\u0228",
            "\1\u0229",
            "\1\u022a",
            "\1\u022b\37\uffff\1\u022b",
            "\1\u022c",
            "\1\u022d",
            "\1\u022e",
            "\1\u022f",
            "\1\u0230",
            "\1\u0231",
            "\1\u0232",
            "\1\u0233",
            "\12\47\7\uffff\13\47\1\u0234\16\47\4\uffff\1\47\1\uffff\32"+
            "\47",
            "\1\u0235",
            "\1\u0236",
            "\1\u0237",
            "\12\47\7\uffff\32\47\4\uffff\1\47\1\uffff\32\47",
            "\1\u0239",
            "\12\47\7\uffff\32\47\4\uffff\1\47\1\uffff\32\47",
            "\1\u023b",
            "\1\u023c",
            "",
            "\12\47\7\uffff\32\47\4\uffff\1\47\1\uffff\32\47",
            "\1\u023e",
            "\12\47\7\uffff\32\47\4\uffff\1\47\1\uffff\32\47",
            "\1\u0240",
            "\1\u0241",
            "\1\u0242",
            "\1\u0243",
            "\1\u0244",
            "\1\u0245",
            "\1\u0246",
            "",
            "",
            "\1\u0248\1\uffff\1\u0247",
            "\1\u0249",
            "\1\u024a",
            "\1\u024b",
            "\1\u024c",
            "",
            "\1\u024d",
            "\1\u024e",
            "\1\u024f",
            "\1\u0250",
            "\1\u0251",
            "\12\47\7\uffff\32\47\4\uffff\1\u0252\1\uffff\32\47",
            "\1\u0254",
            "\12\47\7\uffff\22\47\1\u0256\7\47\4\uffff\1\47\1\uffff\22\47"+
            "\1\u0256\7\47",
            "",
            "\12\47\7\uffff\32\47\4\uffff\1\47\1\uffff\32\47",
            "\1\u0258",
            "",
            "\12\47\7\uffff\22\47\1\u025a\7\47\4\uffff\1\47\1\uffff\22\47"+
            "\1\u025a\7\47",
            "\12\47\7\uffff\32\47\4\uffff\1\47\1\uffff\32\47",
            "\1\u025c",
            "\1\u025d\37\uffff\1\u025d",
            "\1\u025e\37\uffff\1\u025e",
            "",
            "\12\47\7\uffff\32\47\4\uffff\1\47\1\uffff\32\47",
            "",
            "\1\u0260",
            "\1\u0261",
            "\1\u0262",
            "\1\u0263",
            "\1\u0264\37\uffff\1\u0264",
            "\1\u0265",
            "\1\u0266",
            "\1\u0267",
            "\1\u0268",
            "\1\u0269",
            "\1\u026a",
            "\1\u026b",
            "\1\u026c",
            "\1\u026d",
            "\1\u026e",
            "\1\u026f",
            "\1\u0270",
            "",
            "\1\u0271",
            "",
            "\1\u0273\1\uffff\1\u0272",
            "\1\u0274",
            "",
            "\1\u0275",
            "",
            "\1\u0276",
            "\12\47\7\uffff\32\47\4\uffff\1\47\1\uffff\32\47",
            "\1\u0278",
            "\1\u0279",
            "\1\u027a",
            "\1\u027b\37\uffff\1\u027b",
            "\12\47\7\uffff\32\47\4\uffff\1\47\1\uffff\32\47",
            "\1\u027d",
            "\1\u027e",
            "\1\u027f",
            "\12\47\7\uffff\32\47\4\uffff\1\47\1\uffff\32\47",
            "\1\u0281",
            "\1\u0282",
            "\1\u0283",
            "\1\u0284",
            "\1\u0285",
            "\1\u0286",
            "\1\u0287",
            "\1\u028a\1\u0288\1\u028b\2\uffff\1\u0289",
            "",
            "\1\u028c",
            "",
            "\12\47\7\uffff\32\47\4\uffff\1\47\1\uffff\32\47",
            "",
            "\1\u028e",
            "",
            "\12\47\7\uffff\32\47\4\uffff\1\47\1\uffff\32\47",
            "",
            "\1\u0290",
            "\12\47\7\uffff\32\47\4\uffff\1\47\1\uffff\32\47",
            "\1\u0292\37\uffff\1\u0292",
            "",
            "\1\u0293",
            "\12\47\7\uffff\32\47\4\uffff\1\47\1\uffff\32\47",
            "\1\u0295",
            "\1\u0296",
            "\1\u0297\37\uffff\1\u0297",
            "\1\u0298",
            "\1\u0299",
            "\1\u029a",
            "\12\47\7\uffff\32\47\4\uffff\1\47\1\uffff\32\47",
            "\12\47\7\uffff\32\47\4\uffff\1\47\1\uffff\32\47",
            "\1\u029d",
            "\12\47\7\uffff\32\47\4\uffff\1\47\1\uffff\32\47",
            "\12\47\7\uffff\32\47\4\uffff\1\47\1\uffff\32\47",
            "\1\u02a0",
            "\1\u02a1\5\uffff\1\u02a2",
            "\1\u02a3",
            "\12\47\7\uffff\32\47\4\uffff\1\47\1\uffff\32\47",
            "\1\u02a5",
            "\1\u02a6",
            "\1\u02a7",
            "\1\u02a8",
            "\1\u02a9",
            "\1\u02aa",
            "",
            "\1\u02ab",
            "\12\47\7\uffff\32\47\4\uffff\1\47\1\uffff\32\47",
            "\1\u02ad",
            "\1\u02ae",
            "",
            "\1\u02af",
            "\1\u02b0",
            "\1\u02b1",
            "",
            "\1\u02b2",
            "\1\u02b3",
            "\1\u02b4",
            "\1\u02b5",
            "\1\u02b6",
            "\1\u02b7",
            "\1\u02b8",
            "\1\u02b9",
            "\1\u02ba",
            "\1\u02bb",
            "\1\u02bc",
            "\1\u02bd",
            "",
            "\1\u02be",
            "",
            "\1\u02bf",
            "",
            "\1\u02c0\37\uffff\1\u02c0",
            "\1\u02c1",
            "",
            "\1\u02c2",
            "\12\47\7\uffff\32\47\4\uffff\1\47\1\uffff\32\47",
            "\12\47\7\uffff\32\47\4\uffff\1\47\1\uffff\32\47",
            "\12\47\7\uffff\32\47\4\uffff\1\47\1\uffff\32\47",
            "\12\47\7\uffff\32\47\4\uffff\1\47\1\uffff\32\47",
            "\1\u02c7",
            "",
            "",
            "\1\u02c8",
            "",
            "",
            "\1\u02c9",
            "\1\u02ca",
            "\1\u02cb",
            "\1\u02cc",
            "",
            "\1\u02cd",
            "\1\u02ce",
            "\1\u02cf",
            "\1\u02d0",
            "\1\u02d1",
            "\1\u02d2",
            "\1\u02d3",
            "",
            "\12\47\7\uffff\32\47\4\uffff\1\47\1\uffff\32\47",
            "\1\u02d5",
            "\1\u02d6",
            "\1\u02d7",
            "\1\u02d8",
            "\1\u02d9",
            "\1\u02da",
            "\1\u02db",
            "\1\u02dc",
            "\1\u02dd",
            "\1\u02de",
            "\1\u02df",
            "\1\u02e0",
            "\1\u02e1",
            "\12\47\7\uffff\32\47\4\uffff\1\47\1\uffff\32\47",
            "\1\u02e3",
            "\1\u02e4",
            "\1\u02e5",
            "\1\u02e6",
            "\1\u02e7\37\uffff\1\u02e7",
            "\12\47\7\uffff\32\47\4\uffff\1\47\1\uffff\32\47",
            "\1\u02e9",
            "",
            "",
            "",
            "",
            "\1\u02ea",
            "\1\u02eb",
            "\1\u02ec",
            "\1\u02ed",
            "\1\u02ee",
            "\1\u02ef",
            "\1\u02f0",
            "\1\u02f1",
            "\1\u02f2",
            "\1\u02f3",
            "\1\u02f4",
            "\1\u02f5",
            "\1\u02f6",
            "",
            "\1\u02f7",
            "\1\u02f8",
            "\1\u02f9",
            "\1\u02fa",
            "\1\u02fb",
            "\12\47\7\uffff\32\47\4\uffff\1\47\1\uffff\32\47",
            "\1\u02fd",
            "\12\47\7\uffff\32\47\4\uffff\1\47\1\uffff\32\47",
            "\12\47\7\uffff\32\47\4\uffff\1\47\1\uffff\32\47",
            "\1\u0300",
            "\1\u0301",
            "\12\47\7\uffff\32\47\4\uffff\1\47\1\uffff\32\47",
            "\12\47\7\uffff\32\47\4\uffff\1\47\1\uffff\32\47",
            "",
            "\12\47\7\uffff\32\47\4\uffff\1\47\1\uffff\32\47",
            "\1\u0305",
            "\1\u0306",
            "\1\u0307",
            "\12\47\7\uffff\32\47\4\uffff\1\47\1\uffff\32\47",
            "",
            "\1\u0309",
            "\1\u030a",
            "\1\u030b",
            "\12\47\7\uffff\32\47\4\uffff\1\47\1\uffff\32\47",
            "\1\u030d",
            "\1\u030e",
            "\1\u030f",
            "\1\u0310",
            "\1\u0311",
            "\1\u0312",
            "\1\u0313",
            "\1\u0314",
            "\12\47\7\uffff\32\47\4\uffff\1\47\1\uffff\32\47",
            "\12\47\7\uffff\32\47\4\uffff\1\47\1\uffff\32\47",
            "\12\47\7\uffff\32\47\4\uffff\1\47\1\uffff\32\47",
            "\1\u0318",
            "\1\u0319",
            "\1\u031a",
            "\1\u031b",
            "",
            "\1\u031c",
            "",
            "",
            "\1\u031d",
            "\12\47\7\uffff\32\47\4\uffff\1\47\1\uffff\32\47",
            "",
            "",
            "",
            "\1\u031f",
            "\12\47\7\uffff\32\47\4\uffff\1\47\1\uffff\32\47",
            "\1\u0321",
            "",
            "\1\u0322",
            "\1\u0323",
            "\12\47\7\uffff\32\47\4\uffff\1\47\1\uffff\32\47",
            "",
            "\1\u0325",
            "\1\u0326",
            "\1\u0327",
            "\1\u0328",
            "\1\u0329",
            "\1\u032a",
            "\1\u032b",
            "\1\u032c",
            "",
            "",
            "",
            "\1\u032d",
            "\1\u032e",
            "\1\u032f",
            "\1\u0330",
            "\12\47\7\uffff\32\47\4\uffff\1\47\1\uffff\32\47",
            "\1\u0332",
            "",
            "\1\u0333",
            "",
            "\1\u0334",
            "\1\u0335",
            "\1\u0336",
            "",
            "\1\u0337",
            "\1\u0338",
            "\1\u0339",
            "\1\u033a",
            "\1\u033b",
            "\1\u033c",
            "\1\u033d",
            "\12\47\7\uffff\32\47\4\uffff\1\47\1\uffff\32\47",
            "\1\u033f",
            "\1\u0340",
            "\1\u0341",
            "\1\u0342",
            "",
            "\1\u0343",
            "\12\47\7\uffff\32\47\4\uffff\1\47\1\uffff\32\47",
            "\1\u0345",
            "\1\u0346",
            "\12\47\7\uffff\32\47\4\uffff\1\47\1\uffff\32\47",
            "\1\u0348",
            "\1\u0349",
            "\12\47\7\uffff\32\47\4\uffff\1\47\1\uffff\32\47",
            "\1\u034b",
            "\1\u034c",
            "\1\u034d",
            "\12\47\7\uffff\32\47\4\uffff\1\47\1\uffff\32\47",
            "",
            "\1\u034f",
            "\12\47\7\uffff\32\47\4\uffff\1\47\1\uffff\32\47",
            "\1\u0351",
            "\12\47\7\uffff\32\47\4\uffff\1\47\1\uffff\32\47",
            "\1\u0353",
            "",
            "\1\u0354",
            "\1\u0355",
            "",
            "\1\u0356",
            "\12\47\7\uffff\32\47\4\uffff\1\47\1\uffff\32\47",
            "",
            "\12\47\7\uffff\32\47\4\uffff\1\47\1\uffff\32\47",
            "\1\u0359",
            "\12\47\7\uffff\32\47\4\uffff\1\47\1\uffff\32\47",
            "",
            "\12\47\7\uffff\32\47\4\uffff\1\47\1\uffff\32\47",
            "",
            "\1\u035c",
            "",
            "\1\u035d",
            "\1\u035e",
            "\1\u035f",
            "\1\u0360",
            "",
            "",
            "\12\47\7\uffff\32\47\4\uffff\1\47\1\uffff\32\47",
            "",
            "",
            "\1\u0362",
            "\12\47\7\uffff\32\47\4\uffff\1\47\1\uffff\32\47",
            "\1\u0364",
            "\1\u0365",
            "\1\u0366",
            "",
            "\1\u0367",
            "",
            "\12\47\7\uffff\32\47\4\uffff\1\47\1\uffff\32\47",
            "\1\u0369",
            "\1\u036a",
            "\12\47\7\uffff\32\47\4\uffff\1\47\1\uffff\32\47",
            "",
            "\12\47\7\uffff\32\47\4\uffff\1\47\1\uffff\32\47",
            "\1\u036d",
            "",
            "",
            "\12\47\7\uffff\32\47\4\uffff\1\47\1\uffff\32\47",
            ""
    };

    static final short[] DFA21_eot = DFA.unpackEncodedString(DFA21_eotS);
    static final short[] DFA21_eof = DFA.unpackEncodedString(DFA21_eofS);
    static final char[] DFA21_min = DFA.unpackEncodedStringToUnsignedChars(DFA21_minS);
    static final char[] DFA21_max = DFA.unpackEncodedStringToUnsignedChars(DFA21_maxS);
    static final short[] DFA21_accept = DFA.unpackEncodedString(DFA21_acceptS);
    static final short[] DFA21_special = DFA.unpackEncodedString(DFA21_specialS);
    static final short[][] DFA21_transition;

    static {
        int numStates = DFA21_transitionS.length;
        DFA21_transition = new short[numStates][];
        for (int i=0; i<numStates; i++) {
            DFA21_transition[i] = DFA.unpackEncodedString(DFA21_transitionS[i]);
        }
    }

    class DFA21 extends DFA {

        public DFA21(BaseRecognizer recognizer) {
            this.recognizer = recognizer;
            this.decisionNumber = 21;
            this.eot = DFA21_eot;
            this.eof = DFA21_eof;
            this.min = DFA21_min;
            this.max = DFA21_max;
            this.accept = DFA21_accept;
            this.special = DFA21_special;
            this.transition = DFA21_transition;
        }
        public String getDescription() {
            return "1:1: Tokens : ( LINE_COMMENT | LB | WS | VERSION | END | BEGIN | SEED | SPECIES | GROUPS | MOLECULE | MOLECULES | MODEL | TYPES | REACTION | REACTIONS | RULES | FUNCTIONS | COMPARTMENTS | ACTIONS | OBSERVABLES | POPULATION | MAPS | SET_OPTION | SPECIES_LABEL | GENERATE_NETWORK | MAX_AGG | MAX_ITER | MAX_STOICH | OVERWRITE | PRINT_ITER | AND | OR | ID | IF | PREFFIX | SUFFIX | LABEL | PARAMETERS | VERBOSE | GML | MATCHONCE | DELETEMOLECULES | MOVECONNECTED | AUTO | HNAUTY | QUASY | NOT | INCLUDE_REACTANTS | INCLUDE_PRODUCTS | EXCLUDE_REACTANTS | EXCLUDE_PRODUCTS | GENERATEHYBRIDMODEL | ATOL | RTOL | STEADY_STATE | SPARSE | T_END | T_START | N_STEPS | N_OUTPUT_STEPS | MAX_SIM_STEPS | OUTPUT_STEP_INTERVAL | PLA_CONFIG | SAMPLE_TIMES | SIMULATE_ODE | SIMULATE_SSA | SIMULATE_NF | SIMULATE_PLA | SIMULATE | READFILE | FILE | WRITENET | WRITESBML | WRITEMFILE | WRITEXML | WRITEMEXFILE | PRINT_CDAT | PRINT_FUNCTIONS | SETCONCENTRATION | METHOD | SETPARAMETER | SAVECONCENTRATIONS | RESETCONCENTRATIONS | ADDCONCENTRATION | GENERATENETWORK | WRITENETWORK | SUBSTANCEUNITS | PARAM | COMPLEX | GET_FINAL_STATE | SAT | EXP | LOG | ARGFILE | SAVE_PROGRESS | NETFILE | PRINT_NET | PRINT_END | TEXTREACTION | CONTINUE | ODE | SSA | NF | PLA | EVALUATE_EXPRESSIONS | BDF | FLOAT | INT | STRING | SEMI | COLON | LSBRACKET | RSBRACKET | LBRACKET | RBRACKET | COMMA | DOT | LPAREN | RPAREN | UNI_REACTION_SIGN | BI_REACTION_SIGN | DOLLAR | TILDE | AT | GTE | GT | LTE | LT | ASSIGNS | EQUALS | BECOMES | DIV | TIMES | MINUS | PLUS | POWER | MOD | PIPE | QMARK | EMARK | DBQUOTES | AMPERSAND | VERSION_NUMBER );";
        }
    }
 

}