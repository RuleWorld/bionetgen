// $ANTLR 3.3 Nov 30, 2010 12:50:56 /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g 2012-03-26 20:22:59

package bngparser.grammars;


import org.antlr.runtime.*;
import java.util.Stack;
import java.util.List;
import java.util.ArrayList;

public class BNGLexer extends Lexer {
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
    public static final int SIMULATE_NF=61;
    public static final int SIMULATE_PLA=62;
    public static final int READFILE=63;
    public static final int FILE=64;
    public static final int WRITENET=65;
    public static final int WRITESBML=66;
    public static final int WRITEMFILE=67;
    public static final int WRITEXML=68;
    public static final int SETCONCENTRATION=69;
    public static final int SETPARAMETER=70;
    public static final int SAVECONCENTRATIONS=71;
    public static final int RESETCONCENTRATIONS=72;
    public static final int GENERATENETWORK=73;
    public static final int SUBSTANCEUNITS=74;
    public static final int SAT=75;
    public static final int DIGIT=76;
    public static final int EXPONENT=77;
    public static final int FLOAT=78;
    public static final int INT=79;
    public static final int LETTER=80;
    public static final int STRING=81;
    public static final int SEMI=82;
    public static final int COLON=83;
    public static final int LSBRACKET=84;
    public static final int RSBRACKET=85;
    public static final int LBRACKET=86;
    public static final int RBRACKET=87;
    public static final int COMMA=88;
    public static final int DOT=89;
    public static final int LPAREN=90;
    public static final int RPAREN=91;
    public static final int UNI_REACTION_SIGN=92;
    public static final int BI_REACTION_SIGN=93;
    public static final int DOLLAR=94;
    public static final int TILDE=95;
    public static final int AT=96;
    public static final int GTE=97;
    public static final int GT=98;
    public static final int LTE=99;
    public static final int LT=100;
    public static final int ASSIGNS=101;
    public static final int EQUALS=102;
    public static final int BECOMES=103;
    public static final int DIV=104;
    public static final int TIMES=105;
    public static final int MINUS=106;
    public static final int PLUS=107;
    public static final int POWER=108;
    public static final int MOD=109;
    public static final int QMARK=110;
    public static final int EMARK=111;
    public static final int DBQUOTES=112;
    public static final int AMPERSAND=113;
    public static final int NULL=114;
    public static final int LOWER=115;
    public static final int UPPER=116;
    public static final int VERSION_NUMBER=117;

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
            // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:12:14: ( '#' (~ ( '\\n' | '\\r' ) )* ( '\\r' )? '\\n' )
            // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:12:16: '#' (~ ( '\\n' | '\\r' ) )* ( '\\r' )? '\\n'
            {
            match('#'); 
            // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:12:20: (~ ( '\\n' | '\\r' ) )*
            loop1:
            do {
                int alt1=2;
                int LA1_0 = input.LA(1);

                if ( ((LA1_0>='\u0000' && LA1_0<='\t')||(LA1_0>='\u000B' && LA1_0<='\f')||(LA1_0>='\u000E' && LA1_0<='\uFFFF')) ) {
                    alt1=1;
                }


                switch (alt1) {
            	case 1 :
            	    // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:12:20: ~ ( '\\n' | '\\r' )
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

            // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:12:34: ( '\\r' )?
            int alt2=2;
            int LA2_0 = input.LA(1);

            if ( (LA2_0=='\r') ) {
                alt2=1;
            }
            switch (alt2) {
                case 1 :
                    // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:12:34: '\\r'
                    {
                    match('\r'); 

                    }
                    break;

            }

            match('\n'); 
            skip();

            }

            state.type = _type;
            state.channel = _channel;
        }
        finally {
        }
    }
    // $ANTLR end "LINE_COMMENT"

    // $ANTLR start "WS"
    public final void mWS() throws RecognitionException {
        try {
            int _type = WS;
            int _channel = DEFAULT_TOKEN_CHANNEL;
            // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:13:4: ( ( ' ' | '\\t' | '\\n' | '\\r' | '\\\\\\n' )+ )
            // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:13:6: ( ' ' | '\\t' | '\\n' | '\\r' | '\\\\\\n' )+
            {
            // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:13:6: ( ' ' | '\\t' | '\\n' | '\\r' | '\\\\\\n' )+
            int cnt3=0;
            loop3:
            do {
                int alt3=6;
                switch ( input.LA(1) ) {
                case ' ':
                    {
                    alt3=1;
                    }
                    break;
                case '\t':
                    {
                    alt3=2;
                    }
                    break;
                case '\n':
                    {
                    alt3=3;
                    }
                    break;
                case '\r':
                    {
                    alt3=4;
                    }
                    break;
                case '\\':
                    {
                    alt3=5;
                    }
                    break;

                }

                switch (alt3) {
            	case 1 :
            	    // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:13:7: ' '
            	    {
            	    match(' '); 

            	    }
            	    break;
            	case 2 :
            	    // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:13:11: '\\t'
            	    {
            	    match('\t'); 

            	    }
            	    break;
            	case 3 :
            	    // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:13:16: '\\n'
            	    {
            	    match('\n'); 

            	    }
            	    break;
            	case 4 :
            	    // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:13:21: '\\r'
            	    {
            	    match('\r'); 

            	    }
            	    break;
            	case 5 :
            	    // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:13:26: '\\\\\\n'
            	    {
            	    match("\\\n"); 


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
            // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:14:9: ( ( 'v' | 'V' ) ( 'e' | 'E' ) ( 'r' | 'R' ) ( 's' | 'S' ) ( 'i' | 'I' ) ( 'o' | 'O' ) ( 'n' | 'N' ) )
            // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:14:11: ( 'v' | 'V' ) ( 'e' | 'E' ) ( 'r' | 'R' ) ( 's' | 'S' ) ( 'i' | 'I' ) ( 'o' | 'O' ) ( 'n' | 'N' )
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
            // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:15:5: ( ( 'e' | 'E' ) ( 'n' | 'N' ) ( 'd' | 'D' ) )
            // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:15:7: ( 'e' | 'E' ) ( 'n' | 'N' ) ( 'd' | 'D' )
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
            // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:16:7: ( ( 'b' | 'B' ) ( 'e' | 'E' ) ( 'g' | 'G' ) ( 'i' | 'I' ) ( 'n' | 'N' ) )
            // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:16:9: ( 'b' | 'B' ) ( 'e' | 'E' ) ( 'g' | 'G' ) ( 'i' | 'I' ) ( 'n' | 'N' )
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
            // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:17:6: ( ( 's' | 'S' ) ( 'e' | 'E' ) ( 'e' | 'E' ) ( 'd' | 'D' ) )
            // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:17:8: ( 's' | 'S' ) ( 'e' | 'E' ) ( 'e' | 'E' ) ( 'd' | 'D' )
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
            // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:18:9: ( ( 's' | 'S' ) ( 'p' | 'P' ) ( 'e' | 'E' ) ( 'c' | 'C' ) ( 'i' | 'I' ) ( 'e' | 'E' ) ( 's' | 'S' ) )
            // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:18:11: ( 's' | 'S' ) ( 'p' | 'P' ) ( 'e' | 'E' ) ( 'c' | 'C' ) ( 'i' | 'I' ) ( 'e' | 'E' ) ( 's' | 'S' )
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
            // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:19:8: ( ( 'G' | 'g' ) ( 'R' | 'r' ) ( 'O' | 'o' ) ( 'U' | 'u' ) ( 'P' | 'p' ) ( 'S' | 's' ) )
            // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:19:10: ( 'G' | 'g' ) ( 'R' | 'r' ) ( 'O' | 'o' ) ( 'U' | 'u' ) ( 'P' | 'p' ) ( 'S' | 's' )
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
            // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:20:10: ( ( 'm' | 'M' ) ( 'o' | 'O' ) ( 'l' | 'L' ) ( 'e' | 'E' ) ( 'c' | 'C' ) ( 'u' | 'l' ) ( 'l' | 'L' ) ( 'e' | 'E' ) )
            // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:20:12: ( 'm' | 'M' ) ( 'o' | 'O' ) ( 'l' | 'L' ) ( 'e' | 'E' ) ( 'c' | 'C' ) ( 'u' | 'l' ) ( 'l' | 'L' ) ( 'e' | 'E' )
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
            // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:21:11: ( MOLECULE ( 's' | 'S' ) )
            // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:21:13: MOLECULE ( 's' | 'S' )
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
            // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:22:6: ( ( 'm' | 'M' ) ( 'o' | 'O' ) ( 'd' | 'D' ) ( 'e' | 'E' ) ( 'l' | 'L' ) )
            // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:22:8: ( 'm' | 'M' ) ( 'o' | 'O' ) ( 'd' | 'D' ) ( 'e' | 'E' ) ( 'l' | 'L' )
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
            // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:23:7: ( ( 't' | 'T' ) ( 'y' | 'Y' ) ( 'p' | 'P' ) ( 'e' | 'E' ) ( 's' | 'S' ) )
            // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:23:9: ( 't' | 'T' ) ( 'y' | 'Y' ) ( 'p' | 'P' ) ( 'e' | 'E' ) ( 's' | 'S' )
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
            // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:24:10: ( ( 'r' | 'R' ) ( 'e' | 'E' ) ( 'a' | 'A' ) ( 'c' | 'C' ) ( 't' | 'T' ) ( 'i' | 'I' ) ( 'o' | 'O' ) ( 'n' | 'N' ) )
            // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:24:12: ( 'r' | 'R' ) ( 'e' | 'E' ) ( 'a' | 'A' ) ( 'c' | 'C' ) ( 't' | 'T' ) ( 'i' | 'I' ) ( 'o' | 'O' ) ( 'n' | 'N' )
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
            // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:25:11: ( REACTION ( 's' | 'S' ) )
            // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:25:13: REACTION ( 's' | 'S' )
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
            // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:26:7: ( ( 'r' | 'R' ) ( 'u' | 'U' ) ( 'l' | 'L' ) ( 'e' | 'E' ) ( 's' | 'S' ) )
            // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:26:9: ( 'r' | 'R' ) ( 'u' | 'U' ) ( 'l' | 'L' ) ( 'e' | 'E' ) ( 's' | 'S' )
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
            // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:27:11: ( ( 'F' | 'f' ) ( 'U' | 'u' ) ( 'N' | 'n' ) ( 'C' | 'c' ) ( 'T' | 't' ) ( 'I' | 'i' ) ( 'O' | 'o' ) ( 'n' | 'N' ) ( 'S' | 's' ) )
            // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:27:13: ( 'F' | 'f' ) ( 'U' | 'u' ) ( 'N' | 'n' ) ( 'C' | 'c' ) ( 'T' | 't' ) ( 'I' | 'i' ) ( 'O' | 'o' ) ( 'n' | 'N' ) ( 'S' | 's' )
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
            // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:28:14: ( ( 'C' | 'c' ) ( 'O' | 'o' ) ( 'M' | 'm' ) ( 'P' | 'p' ) ( 'A' | 'a' ) ( 'R' | 'r' ) ( 'T' | 't' ) ( 'M' | 'm' ) ( 'E' | 'e' ) ( 'N' | 'n' ) ( 'T' | 't' ) ( 'S' | 's' ) )
            // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:28:16: ( 'C' | 'c' ) ( 'O' | 'o' ) ( 'M' | 'm' ) ( 'P' | 'p' ) ( 'A' | 'a' ) ( 'R' | 'r' ) ( 'T' | 't' ) ( 'M' | 'm' ) ( 'E' | 'e' ) ( 'N' | 'n' ) ( 'T' | 't' ) ( 'S' | 's' )
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
            // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:29:8: ( ( 'A' | 'a' ) ( 'C' | 'c' ) ( 'T' | 't' ) ( 'i' | 'I' ) ( 'o' | 'O' ) ( 'n' | 'N' ) ( 'S' | 's' ) )
            // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:29:10: ( 'A' | 'a' ) ( 'C' | 'c' ) ( 'T' | 't' ) ( 'i' | 'I' ) ( 'o' | 'O' ) ( 'n' | 'N' ) ( 'S' | 's' )
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
            // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:30:12: ( 'observables' )
            // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:30:14: 'observables'
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

    // $ANTLR start "SET_OPTION"
    public final void mSET_OPTION() throws RecognitionException {
        try {
            int _type = SET_OPTION;
            int _channel = DEFAULT_TOKEN_CHANNEL;
            // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:32:12: ( 'SETOPTION' )
            // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:32:14: 'SETOPTION'
            {
            match("SETOPTION"); 


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
            // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:33:15: ( 'SPECIESLABEL' )
            // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:33:17: 'SPECIESLABEL'
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
            // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:34:18: ( 'GENERATENETWORK' )
            // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:34:20: 'GENERATENETWORK'
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
            // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:35:9: ( 'MAXAGG' )
            // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:35:11: 'MAXAGG'
            {
            match("MAXAGG"); 


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
            // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:36:10: ( ( 'M' | 'm' ) ( 'A' | 'a' ) ( 'X' | 'x' ) '_' ( 'I' | 'i' ) ( 'T' | 't' ) ( 'E' | 'e' ) ( 'R' | 'r' ) )
            // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:36:12: ( 'M' | 'm' ) ( 'A' | 'a' ) ( 'X' | 'x' ) '_' ( 'I' | 'i' ) ( 'T' | 't' ) ( 'E' | 'e' ) ( 'R' | 'r' )
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
            // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:37:12: ( 'MAXSTOICH' )
            // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:37:14: 'MAXSTOICH'
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
            // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:38:10: ( 'overwrite' )
            // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:38:12: 'overwrite'
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
            // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:39:12: ( 'PRINTITER' )
            // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:39:14: 'PRINTITER'
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
            // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:40:5: ( 'AND' )
            // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:40:7: 'AND'
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
            // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:41:4: ( 'OR' )
            // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:41:6: 'OR'
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
            // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:42:4: ( 'ID' )
            // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:42:6: 'ID'
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
            // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:43:4: ( 'if' )
            // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:43:6: 'if'
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
            // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:44:9: ( 'PREFFIX' )
            // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:44:11: 'PREFFIX'
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
            // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:45:8: ( 'SUFFIX' | 'suffix' )
            int alt4=2;
            int LA4_0 = input.LA(1);

            if ( (LA4_0=='S') ) {
                alt4=1;
            }
            else if ( (LA4_0=='s') ) {
                alt4=2;
            }
            else {
                NoViableAltException nvae =
                    new NoViableAltException("", 4, 0, input);

                throw nvae;
            }
            switch (alt4) {
                case 1 :
                    // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:45:10: 'SUFFIX'
                    {
                    match("SUFFIX"); 


                    }
                    break;
                case 2 :
                    // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:45:19: 'suffix'
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
            // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:46:7: ( 'LABEL' )
            // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:46:9: 'LABEL'
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
            // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:47:12: ( ( 'p' | 'P' ) ( 'a' | 'A' ) ( 'r' | 'R' ) ( 'a' | 'A' ) ( 'm' | 'M' ) ( 'e' | 'E' ) ( 't' | 'T' ) ( 'e' | 'E' ) ( 'r' | 'R' ) ( 's' | 'S' ) )
            // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:47:14: ( 'p' | 'P' ) ( 'a' | 'A' ) ( 'r' | 'R' ) ( 'a' | 'A' ) ( 'm' | 'M' ) ( 'e' | 'E' ) ( 't' | 'T' ) ( 'e' | 'E' ) ( 'r' | 'R' ) ( 's' | 'S' )
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
            // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:48:9: ( ( 'V' | 'v' ) ( 'E' | 'e' ) ( 'R' | 'r' ) ( 'B' | 'b' ) ( 'O' | 'o' ) ( 'S' | 's' ) ( 'E ' | 'e' ) )
            // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:48:11: ( 'V' | 'v' ) ( 'E' | 'e' ) ( 'R' | 'r' ) ( 'B' | 'b' ) ( 'O' | 'o' ) ( 'S' | 's' ) ( 'E ' | 'e' )
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

            // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:48:65: ( 'E ' | 'e' )
            int alt5=2;
            int LA5_0 = input.LA(1);

            if ( (LA5_0=='E') ) {
                alt5=1;
            }
            else if ( (LA5_0=='e') ) {
                alt5=2;
            }
            else {
                NoViableAltException nvae =
                    new NoViableAltException("", 5, 0, input);

                throw nvae;
            }
            switch (alt5) {
                case 1 :
                    // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:48:66: 'E '
                    {
                    match("E "); 


                    }
                    break;
                case 2 :
                    // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:48:71: 'e'
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

    // $ANTLR start "MATCHONCE"
    public final void mMATCHONCE() throws RecognitionException {
        try {
            int _type = MATCHONCE;
            int _channel = DEFAULT_TOKEN_CHANNEL;
            // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:50:10: ( 'MatchOnce' )
            // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:50:12: 'MatchOnce'
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
            // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:51:16: ( 'DeleteMolecules' )
            // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:51:18: 'DeleteMolecules'
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
            // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:52:14: ( 'MoveConnected' )
            // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:52:16: 'MoveConnected'
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
            // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:53:6: ( 'Auto' )
            // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:53:8: 'Auto'
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
            // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:54:8: ( 'Hnauty' )
            // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:54:10: 'Hnauty'
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
            // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:55:7: ( 'Quasy' )
            // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:55:9: 'Quasy'
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
            // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:56:4: ( 'not' )
            // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:56:6: 'not'
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
            // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:57:18: ( 'include_reactants' )
            // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:57:20: 'include_reactants'
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
            // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:58:17: ( 'include_products' )
            // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:58:19: 'include_products'
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
            // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:59:18: ( 'exclude_reactants' )
            // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:59:20: 'exclude_reactants'
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
            // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:60:17: ( 'exclude_products' )
            // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:60:19: 'exclude_products'
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

    // $ANTLR start "ATOL"
    public final void mATOL() throws RecognitionException {
        try {
            int _type = ATOL;
            int _channel = DEFAULT_TOKEN_CHANNEL;
            // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:62:5: ( 'atol' )
            // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:62:7: 'atol'
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
            // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:63:5: ( 'rtol' )
            // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:63:7: 'rtol'
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
            // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:64:13: ( 'steady_state' )
            // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:64:15: 'steady_state'
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
            // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:65:7: ( 'sparse' )
            // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:65:9: 'sparse'
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
            // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:66:6: ( 't_end' )
            // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:66:8: 't_end'
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
            // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:67:8: ( 't_start' )
            // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:67:10: 't_start'
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
            // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:68:8: ( 'n_steps' )
            // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:68:10: 'n_steps'
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

    // $ANTLR start "SAMPLE_TIMES"
    public final void mSAMPLE_TIMES() throws RecognitionException {
        try {
            int _type = SAMPLE_TIMES;
            int _channel = DEFAULT_TOKEN_CHANNEL;
            // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:69:13: ( 'sample_times' )
            // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:69:15: 'sample_times'
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
            // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:70:13: ( 'simulate_ode' )
            // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:70:15: 'simulate_ode'
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
            // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:71:13: ( 'simulate_ssa' )
            // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:71:15: 'simulate_ssa'
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
            // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:72:12: ( 'simulate_nf' )
            // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:72:14: 'simulate_nf'
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
            // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:73:13: ( 'simulate_pla' )
            // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:73:15: 'simulate_pla'
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

    // $ANTLR start "READFILE"
    public final void mREADFILE() throws RecognitionException {
        try {
            int _type = READFILE;
            int _channel = DEFAULT_TOKEN_CHANNEL;
            // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:74:9: ( 'readFile' )
            // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:74:11: 'readFile'
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
            // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:75:5: ( 'file' )
            // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:75:7: 'file'
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
            // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:76:9: ( 'writeNET' )
            // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:76:11: 'writeNET'
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
            // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:77:10: ( 'writeSBML' )
            // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:77:12: 'writeSBML'
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
            // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:78:11: ( 'writeMfile' )
            // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:78:13: 'writeMfile'
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
            // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:79:9: ( 'writeXML' )
            // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:79:11: 'writeXML'
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

    // $ANTLR start "SETCONCENTRATION"
    public final void mSETCONCENTRATION() throws RecognitionException {
        try {
            int _type = SETCONCENTRATION;
            int _channel = DEFAULT_TOKEN_CHANNEL;
            // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:80:17: ( 'setConcentration' )
            // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:80:19: 'setConcentration'
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

    // $ANTLR start "SETPARAMETER"
    public final void mSETPARAMETER() throws RecognitionException {
        try {
            int _type = SETPARAMETER;
            int _channel = DEFAULT_TOKEN_CHANNEL;
            // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:81:13: ( 'setParameter' )
            // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:81:15: 'setParameter'
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
            // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:82:19: ( 'saveConcentrations' )
            // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:82:21: 'saveConcentrations'
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
            // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:83:20: ( 'resetConcentrations' )
            // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:83:22: 'resetConcentrations'
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

    // $ANTLR start "GENERATENETWORK"
    public final void mGENERATENETWORK() throws RecognitionException {
        try {
            int _type = GENERATENETWORK;
            int _channel = DEFAULT_TOKEN_CHANNEL;
            // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:84:16: ( 'generate_network' )
            // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:84:18: 'generate_network'
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

    // $ANTLR start "SUBSTANCEUNITS"
    public final void mSUBSTANCEUNITS() throws RecognitionException {
        try {
            int _type = SUBSTANCEUNITS;
            int _channel = DEFAULT_TOKEN_CHANNEL;
            // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:85:15: ( 'substanceUnits' )
            // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:85:17: 'substanceUnits'
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

    // $ANTLR start "SAT"
    public final void mSAT() throws RecognitionException {
        try {
            int _type = SAT;
            int _channel = DEFAULT_TOKEN_CHANNEL;
            // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:86:4: ( 'Sat' )
            // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:86:6: 'Sat'
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

    // $ANTLR start "FLOAT"
    public final void mFLOAT() throws RecognitionException {
        try {
            int _type = FLOAT;
            int _channel = DEFAULT_TOKEN_CHANNEL;
            // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:87:6: ( ( DIGIT )+ '.' ( DIGIT )* ( EXPONENT )? | '.' ( DIGIT )+ ( EXPONENT )? | ( DIGIT )+ EXPONENT )
            int alt12=3;
            alt12 = dfa12.predict(input);
            switch (alt12) {
                case 1 :
                    // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:88:3: ( DIGIT )+ '.' ( DIGIT )* ( EXPONENT )?
                    {
                    // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:88:3: ( DIGIT )+
                    int cnt6=0;
                    loop6:
                    do {
                        int alt6=2;
                        int LA6_0 = input.LA(1);

                        if ( ((LA6_0>='0' && LA6_0<='9')) ) {
                            alt6=1;
                        }


                        switch (alt6) {
                    	case 1 :
                    	    // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:88:4: DIGIT
                    	    {
                    	    mDIGIT(); 

                    	    }
                    	    break;

                    	default :
                    	    if ( cnt6 >= 1 ) break loop6;
                                EarlyExitException eee =
                                    new EarlyExitException(6, input);
                                throw eee;
                        }
                        cnt6++;
                    } while (true);

                    match('.'); 
                    // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:88:16: ( DIGIT )*
                    loop7:
                    do {
                        int alt7=2;
                        int LA7_0 = input.LA(1);

                        if ( ((LA7_0>='0' && LA7_0<='9')) ) {
                            alt7=1;
                        }


                        switch (alt7) {
                    	case 1 :
                    	    // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:88:17: DIGIT
                    	    {
                    	    mDIGIT(); 

                    	    }
                    	    break;

                    	default :
                    	    break loop7;
                        }
                    } while (true);

                    // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:88:25: ( EXPONENT )?
                    int alt8=2;
                    int LA8_0 = input.LA(1);

                    if ( (LA8_0=='E'||LA8_0=='e') ) {
                        alt8=1;
                    }
                    switch (alt8) {
                        case 1 :
                            // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:88:25: EXPONENT
                            {
                            mEXPONENT(); 

                            }
                            break;

                    }


                    }
                    break;
                case 2 :
                    // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:89:3: '.' ( DIGIT )+ ( EXPONENT )?
                    {
                    match('.'); 
                    // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:89:7: ( DIGIT )+
                    int cnt9=0;
                    loop9:
                    do {
                        int alt9=2;
                        int LA9_0 = input.LA(1);

                        if ( ((LA9_0>='0' && LA9_0<='9')) ) {
                            alt9=1;
                        }


                        switch (alt9) {
                    	case 1 :
                    	    // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:89:8: DIGIT
                    	    {
                    	    mDIGIT(); 

                    	    }
                    	    break;

                    	default :
                    	    if ( cnt9 >= 1 ) break loop9;
                                EarlyExitException eee =
                                    new EarlyExitException(9, input);
                                throw eee;
                        }
                        cnt9++;
                    } while (true);

                    // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:89:16: ( EXPONENT )?
                    int alt10=2;
                    int LA10_0 = input.LA(1);

                    if ( (LA10_0=='E'||LA10_0=='e') ) {
                        alt10=1;
                    }
                    switch (alt10) {
                        case 1 :
                            // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:89:16: EXPONENT
                            {
                            mEXPONENT(); 

                            }
                            break;

                    }


                    }
                    break;
                case 3 :
                    // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:90:3: ( DIGIT )+ EXPONENT
                    {
                    // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:90:3: ( DIGIT )+
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
                    	    // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:90:4: DIGIT
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
            // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:91:4: ( ( DIGIT )+ )
            // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:91:6: ( DIGIT )+
            {
            // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:91:6: ( DIGIT )+
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
            	    // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:91:6: DIGIT
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
            // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:92:7: ( LETTER ( LETTER | DIGIT | '_' )* )
            // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:92:9: LETTER ( LETTER | DIGIT | '_' )*
            {
            mLETTER(); 
            // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:92:16: ( LETTER | DIGIT | '_' )*
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
            	    break loop14;
                }
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
            // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:95:6: ( ';' )
            // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:95:8: ';'
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
            // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:96:6: ( ':' )
            // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:96:8: ':'
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
            // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:97:11: ( '[' )
            // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:97:13: '['
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
            // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:98:11: ( ']' )
            // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:98:13: ']'
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
            // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:99:10: ( '{' )
            // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:99:12: '{'
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
            // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:100:10: ( '}' )
            // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:100:12: '}'
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
            // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:101:7: ( ',' )
            // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:101:9: ','
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
            // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:102:5: ( '.' )
            // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:102:7: '.'
            {
            match('.'); 

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
            // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:103:8: ( '(' )
            // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:103:10: '('
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
            // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:104:8: ( ')' )
            // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:104:10: ')'
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
            // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:105:19: ( '->' )
            // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:105:21: '->'
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
            // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:106:18: ( '<->' )
            // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:106:20: '<->'
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
            // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:107:8: ( '$' )
            // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:107:10: '$'
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
            // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:108:7: ( '~' )
            // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:108:9: '~'
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
            // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:109:4: ( '@' )
            // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:109:6: '@'
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
            // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:110:5: ( '>=' )
            // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:110:7: '>='
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
            // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:111:4: ( '>' )
            // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:111:6: '>'
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
            // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:112:5: ( '<=' )
            // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:112:7: '<='
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
            // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:113:4: ( '<' )
            // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:113:6: '<'
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
            // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:114:9: ( '=>' )
            // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:114:11: '=>'
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
            // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:115:8: ( '==' )
            // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:115:10: '=='
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
            // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:116:9: ( '=' )
            // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:116:11: '='
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
            // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:117:5: ( '/' )
            // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:117:7: '/'
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
            // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:118:7: ( '*' )
            // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:118:9: '*'
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
            // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:119:7: ( '-' )
            // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:119:9: '-'
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
            // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:120:6: ( '+' )
            // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:120:8: '+'
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
            // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:121:6: ( '^' )
            // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:121:8: '^'
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
            // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:122:5: ( '%' )
            // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:122:7: '%'
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

    // $ANTLR start "QMARK"
    public final void mQMARK() throws RecognitionException {
        try {
            int _type = QMARK;
            int _channel = DEFAULT_TOKEN_CHANNEL;
            // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:123:7: ( '?' )
            // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:123:9: '?'
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
            // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:124:6: ( '!' )
            // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:124:8: '!'
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
            // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:125:10: ( '\"' )
            // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:125:12: '\"'
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
            // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:126:11: ( '&' )
            // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:126:13: '&'
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
            // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:128:14: ( '0' )
            // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:128:16: '0'
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
            // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:129:15: ( NULL | '1' .. '9' )
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
            // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:130:16: ( LOWER | UPPER )
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
            // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:131:15: ( 'a' .. 'z' )
            // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:131:17: 'a' .. 'z'
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
            // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:132:15: ( 'A' .. 'Z' )
            // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:132:17: 'A' .. 'Z'
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
            // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:133:19: ( ( 'e' | 'E' ) ( '+' | '-' )? ( DIGIT )+ )
            // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:133:21: ( 'e' | 'E' ) ( '+' | '-' )? ( DIGIT )+
            {
            if ( input.LA(1)=='E'||input.LA(1)=='e' ) {
                input.consume();

            }
            else {
                MismatchedSetException mse = new MismatchedSetException(null,input);
                recover(mse);
                throw mse;}

            // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:133:31: ( '+' | '-' )?
            int alt15=2;
            int LA15_0 = input.LA(1);

            if ( (LA15_0=='+'||LA15_0=='-') ) {
                alt15=1;
            }
            switch (alt15) {
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

            // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:133:42: ( DIGIT )+
            int cnt16=0;
            loop16:
            do {
                int alt16=2;
                int LA16_0 = input.LA(1);

                if ( ((LA16_0>='0' && LA16_0<='9')) ) {
                    alt16=1;
                }


                switch (alt16) {
            	case 1 :
            	    // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:133:43: DIGIT
            	    {
            	    mDIGIT(); 

            	    }
            	    break;

            	default :
            	    if ( cnt16 >= 1 ) break loop16;
                        EarlyExitException eee =
                            new EarlyExitException(16, input);
                        throw eee;
                }
                cnt16++;
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
            // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:136:16: ( ( DIGIT ) | ( DIGIT '.' DIGIT ) | ( DIGIT '.' DIGIT '.' DIGIT ) )
            int alt17=3;
            int LA17_0 = input.LA(1);

            if ( ((LA17_0>='0' && LA17_0<='9')) ) {
                int LA17_1 = input.LA(2);

                if ( (LA17_1=='.') ) {
                    int LA17_3 = input.LA(3);

                    if ( ((LA17_3>='0' && LA17_3<='9')) ) {
                        int LA17_4 = input.LA(4);

                        if ( (LA17_4=='.') ) {
                            alt17=3;
                        }
                        else {
                            alt17=2;}
                    }
                    else {
                        NoViableAltException nvae =
                            new NoViableAltException("", 17, 3, input);

                        throw nvae;
                    }
                }
                else {
                    alt17=1;}
            }
            else {
                NoViableAltException nvae =
                    new NoViableAltException("", 17, 0, input);

                throw nvae;
            }
            switch (alt17) {
                case 1 :
                    // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:136:18: ( DIGIT )
                    {
                    // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:136:18: ( DIGIT )
                    // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:136:19: DIGIT
                    {
                    mDIGIT(); 

                    }


                    }
                    break;
                case 2 :
                    // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:136:28: ( DIGIT '.' DIGIT )
                    {
                    // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:136:28: ( DIGIT '.' DIGIT )
                    // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:136:29: DIGIT '.' DIGIT
                    {
                    mDIGIT(); 
                    match('.'); 
                    mDIGIT(); 

                    }


                    }
                    break;
                case 3 :
                    // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:136:46: ( DIGIT '.' DIGIT '.' DIGIT )
                    {
                    // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:136:46: ( DIGIT '.' DIGIT '.' DIGIT )
                    // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:136:47: DIGIT '.' DIGIT '.' DIGIT
                    {
                    mDIGIT(); 
                    match('.'); 
                    mDIGIT(); 
                    match('.'); 
                    mDIGIT(); 

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
        // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:1:8: ( LINE_COMMENT | WS | VERSION | END | BEGIN | SEED | SPECIES | GROUPS | MOLECULE | MOLECULES | MODEL | TYPES | REACTION | REACTIONS | RULES | FUNCTIONS | COMPARTMENTS | ACTIONS | OBSERVABLES | SET_OPTION | SPECIES_LABEL | GENERATE_NETWORK | MAX_AGG | MAX_ITER | MAX_STOICH | OVERWRITE | PRINT_ITER | AND | OR | ID | IF | PREFFIX | SUFFIX | LABEL | PARAMETERS | VERBOSE | MATCHONCE | DELETEMOLECULES | MOVECONNECTED | AUTO | HNAUTY | QUASY | NOT | INCLUDE_REACTANTS | INCLUDE_PRODUCTS | EXCLUDE_REACTANTS | EXCLUDE_PRODUCTS | ATOL | RTOL | STEADY_STATE | SPARSE | T_END | T_START | N_STEPS | SAMPLE_TIMES | SIMULATE_ODE | SIMULATE_SSA | SIMULATE_NF | SIMULATE_PLA | READFILE | FILE | WRITENET | WRITESBML | WRITEMFILE | WRITEXML | SETCONCENTRATION | SETPARAMETER | SAVECONCENTRATIONS | RESETCONCENTRATIONS | GENERATENETWORK | SUBSTANCEUNITS | SAT | FLOAT | INT | STRING | SEMI | COLON | LSBRACKET | RSBRACKET | LBRACKET | RBRACKET | COMMA | DOT | LPAREN | RPAREN | UNI_REACTION_SIGN | BI_REACTION_SIGN | DOLLAR | TILDE | AT | GTE | GT | LTE | LT | ASSIGNS | EQUALS | BECOMES | DIV | TIMES | MINUS | PLUS | POWER | MOD | QMARK | EMARK | DBQUOTES | AMPERSAND | VERSION_NUMBER )
        int alt18=108;
        alt18 = dfa18.predict(input);
        switch (alt18) {
            case 1 :
                // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:1:10: LINE_COMMENT
                {
                mLINE_COMMENT(); 

                }
                break;
            case 2 :
                // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:1:23: WS
                {
                mWS(); 

                }
                break;
            case 3 :
                // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:1:26: VERSION
                {
                mVERSION(); 

                }
                break;
            case 4 :
                // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:1:34: END
                {
                mEND(); 

                }
                break;
            case 5 :
                // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:1:38: BEGIN
                {
                mBEGIN(); 

                }
                break;
            case 6 :
                // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:1:44: SEED
                {
                mSEED(); 

                }
                break;
            case 7 :
                // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:1:49: SPECIES
                {
                mSPECIES(); 

                }
                break;
            case 8 :
                // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:1:57: GROUPS
                {
                mGROUPS(); 

                }
                break;
            case 9 :
                // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:1:64: MOLECULE
                {
                mMOLECULE(); 

                }
                break;
            case 10 :
                // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:1:73: MOLECULES
                {
                mMOLECULES(); 

                }
                break;
            case 11 :
                // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:1:83: MODEL
                {
                mMODEL(); 

                }
                break;
            case 12 :
                // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:1:89: TYPES
                {
                mTYPES(); 

                }
                break;
            case 13 :
                // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:1:95: REACTION
                {
                mREACTION(); 

                }
                break;
            case 14 :
                // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:1:104: REACTIONS
                {
                mREACTIONS(); 

                }
                break;
            case 15 :
                // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:1:114: RULES
                {
                mRULES(); 

                }
                break;
            case 16 :
                // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:1:120: FUNCTIONS
                {
                mFUNCTIONS(); 

                }
                break;
            case 17 :
                // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:1:130: COMPARTMENTS
                {
                mCOMPARTMENTS(); 

                }
                break;
            case 18 :
                // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:1:143: ACTIONS
                {
                mACTIONS(); 

                }
                break;
            case 19 :
                // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:1:151: OBSERVABLES
                {
                mOBSERVABLES(); 

                }
                break;
            case 20 :
                // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:1:163: SET_OPTION
                {
                mSET_OPTION(); 

                }
                break;
            case 21 :
                // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:1:174: SPECIES_LABEL
                {
                mSPECIES_LABEL(); 

                }
                break;
            case 22 :
                // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:1:188: GENERATE_NETWORK
                {
                mGENERATE_NETWORK(); 

                }
                break;
            case 23 :
                // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:1:205: MAX_AGG
                {
                mMAX_AGG(); 

                }
                break;
            case 24 :
                // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:1:213: MAX_ITER
                {
                mMAX_ITER(); 

                }
                break;
            case 25 :
                // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:1:222: MAX_STOICH
                {
                mMAX_STOICH(); 

                }
                break;
            case 26 :
                // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:1:233: OVERWRITE
                {
                mOVERWRITE(); 

                }
                break;
            case 27 :
                // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:1:243: PRINT_ITER
                {
                mPRINT_ITER(); 

                }
                break;
            case 28 :
                // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:1:254: AND
                {
                mAND(); 

                }
                break;
            case 29 :
                // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:1:258: OR
                {
                mOR(); 

                }
                break;
            case 30 :
                // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:1:261: ID
                {
                mID(); 

                }
                break;
            case 31 :
                // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:1:264: IF
                {
                mIF(); 

                }
                break;
            case 32 :
                // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:1:267: PREFFIX
                {
                mPREFFIX(); 

                }
                break;
            case 33 :
                // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:1:275: SUFFIX
                {
                mSUFFIX(); 

                }
                break;
            case 34 :
                // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:1:282: LABEL
                {
                mLABEL(); 

                }
                break;
            case 35 :
                // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:1:288: PARAMETERS
                {
                mPARAMETERS(); 

                }
                break;
            case 36 :
                // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:1:299: VERBOSE
                {
                mVERBOSE(); 

                }
                break;
            case 37 :
                // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:1:307: MATCHONCE
                {
                mMATCHONCE(); 

                }
                break;
            case 38 :
                // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:1:317: DELETEMOLECULES
                {
                mDELETEMOLECULES(); 

                }
                break;
            case 39 :
                // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:1:333: MOVECONNECTED
                {
                mMOVECONNECTED(); 

                }
                break;
            case 40 :
                // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:1:347: AUTO
                {
                mAUTO(); 

                }
                break;
            case 41 :
                // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:1:352: HNAUTY
                {
                mHNAUTY(); 

                }
                break;
            case 42 :
                // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:1:359: QUASY
                {
                mQUASY(); 

                }
                break;
            case 43 :
                // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:1:365: NOT
                {
                mNOT(); 

                }
                break;
            case 44 :
                // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:1:369: INCLUDE_REACTANTS
                {
                mINCLUDE_REACTANTS(); 

                }
                break;
            case 45 :
                // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:1:387: INCLUDE_PRODUCTS
                {
                mINCLUDE_PRODUCTS(); 

                }
                break;
            case 46 :
                // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:1:404: EXCLUDE_REACTANTS
                {
                mEXCLUDE_REACTANTS(); 

                }
                break;
            case 47 :
                // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:1:422: EXCLUDE_PRODUCTS
                {
                mEXCLUDE_PRODUCTS(); 

                }
                break;
            case 48 :
                // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:1:439: ATOL
                {
                mATOL(); 

                }
                break;
            case 49 :
                // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:1:444: RTOL
                {
                mRTOL(); 

                }
                break;
            case 50 :
                // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:1:449: STEADY_STATE
                {
                mSTEADY_STATE(); 

                }
                break;
            case 51 :
                // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:1:462: SPARSE
                {
                mSPARSE(); 

                }
                break;
            case 52 :
                // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:1:469: T_END
                {
                mT_END(); 

                }
                break;
            case 53 :
                // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:1:475: T_START
                {
                mT_START(); 

                }
                break;
            case 54 :
                // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:1:483: N_STEPS
                {
                mN_STEPS(); 

                }
                break;
            case 55 :
                // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:1:491: SAMPLE_TIMES
                {
                mSAMPLE_TIMES(); 

                }
                break;
            case 56 :
                // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:1:504: SIMULATE_ODE
                {
                mSIMULATE_ODE(); 

                }
                break;
            case 57 :
                // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:1:517: SIMULATE_SSA
                {
                mSIMULATE_SSA(); 

                }
                break;
            case 58 :
                // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:1:530: SIMULATE_NF
                {
                mSIMULATE_NF(); 

                }
                break;
            case 59 :
                // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:1:542: SIMULATE_PLA
                {
                mSIMULATE_PLA(); 

                }
                break;
            case 60 :
                // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:1:555: READFILE
                {
                mREADFILE(); 

                }
                break;
            case 61 :
                // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:1:564: FILE
                {
                mFILE(); 

                }
                break;
            case 62 :
                // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:1:569: WRITENET
                {
                mWRITENET(); 

                }
                break;
            case 63 :
                // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:1:578: WRITESBML
                {
                mWRITESBML(); 

                }
                break;
            case 64 :
                // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:1:588: WRITEMFILE
                {
                mWRITEMFILE(); 

                }
                break;
            case 65 :
                // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:1:599: WRITEXML
                {
                mWRITEXML(); 

                }
                break;
            case 66 :
                // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:1:608: SETCONCENTRATION
                {
                mSETCONCENTRATION(); 

                }
                break;
            case 67 :
                // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:1:625: SETPARAMETER
                {
                mSETPARAMETER(); 

                }
                break;
            case 68 :
                // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:1:638: SAVECONCENTRATIONS
                {
                mSAVECONCENTRATIONS(); 

                }
                break;
            case 69 :
                // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:1:657: RESETCONCENTRATIONS
                {
                mRESETCONCENTRATIONS(); 

                }
                break;
            case 70 :
                // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:1:677: GENERATENETWORK
                {
                mGENERATENETWORK(); 

                }
                break;
            case 71 :
                // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:1:693: SUBSTANCEUNITS
                {
                mSUBSTANCEUNITS(); 

                }
                break;
            case 72 :
                // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:1:708: SAT
                {
                mSAT(); 

                }
                break;
            case 73 :
                // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:1:712: FLOAT
                {
                mFLOAT(); 

                }
                break;
            case 74 :
                // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:1:718: INT
                {
                mINT(); 

                }
                break;
            case 75 :
                // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:1:722: STRING
                {
                mSTRING(); 

                }
                break;
            case 76 :
                // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:1:729: SEMI
                {
                mSEMI(); 

                }
                break;
            case 77 :
                // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:1:734: COLON
                {
                mCOLON(); 

                }
                break;
            case 78 :
                // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:1:740: LSBRACKET
                {
                mLSBRACKET(); 

                }
                break;
            case 79 :
                // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:1:750: RSBRACKET
                {
                mRSBRACKET(); 

                }
                break;
            case 80 :
                // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:1:760: LBRACKET
                {
                mLBRACKET(); 

                }
                break;
            case 81 :
                // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:1:769: RBRACKET
                {
                mRBRACKET(); 

                }
                break;
            case 82 :
                // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:1:778: COMMA
                {
                mCOMMA(); 

                }
                break;
            case 83 :
                // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:1:784: DOT
                {
                mDOT(); 

                }
                break;
            case 84 :
                // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:1:788: LPAREN
                {
                mLPAREN(); 

                }
                break;
            case 85 :
                // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:1:795: RPAREN
                {
                mRPAREN(); 

                }
                break;
            case 86 :
                // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:1:802: UNI_REACTION_SIGN
                {
                mUNI_REACTION_SIGN(); 

                }
                break;
            case 87 :
                // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:1:820: BI_REACTION_SIGN
                {
                mBI_REACTION_SIGN(); 

                }
                break;
            case 88 :
                // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:1:837: DOLLAR
                {
                mDOLLAR(); 

                }
                break;
            case 89 :
                // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:1:844: TILDE
                {
                mTILDE(); 

                }
                break;
            case 90 :
                // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:1:850: AT
                {
                mAT(); 

                }
                break;
            case 91 :
                // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:1:853: GTE
                {
                mGTE(); 

                }
                break;
            case 92 :
                // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:1:857: GT
                {
                mGT(); 

                }
                break;
            case 93 :
                // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:1:860: LTE
                {
                mLTE(); 

                }
                break;
            case 94 :
                // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:1:864: LT
                {
                mLT(); 

                }
                break;
            case 95 :
                // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:1:867: ASSIGNS
                {
                mASSIGNS(); 

                }
                break;
            case 96 :
                // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:1:875: EQUALS
                {
                mEQUALS(); 

                }
                break;
            case 97 :
                // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:1:882: BECOMES
                {
                mBECOMES(); 

                }
                break;
            case 98 :
                // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:1:890: DIV
                {
                mDIV(); 

                }
                break;
            case 99 :
                // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:1:894: TIMES
                {
                mTIMES(); 

                }
                break;
            case 100 :
                // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:1:900: MINUS
                {
                mMINUS(); 

                }
                break;
            case 101 :
                // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:1:906: PLUS
                {
                mPLUS(); 

                }
                break;
            case 102 :
                // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:1:911: POWER
                {
                mPOWER(); 

                }
                break;
            case 103 :
                // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:1:917: MOD
                {
                mMOD(); 

                }
                break;
            case 104 :
                // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:1:921: QMARK
                {
                mQMARK(); 

                }
                break;
            case 105 :
                // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:1:927: EMARK
                {
                mEMARK(); 

                }
                break;
            case 106 :
                // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:1:933: DBQUOTES
                {
                mDBQUOTES(); 

                }
                break;
            case 107 :
                // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:1:942: AMPERSAND
                {
                mAMPERSAND(); 

                }
                break;
            case 108 :
                // /home/proto/workspace/bionetgen/parsers/BNGParser/src/bngparser/grammars/BNGLexer.g:1:952: VERSION_NUMBER
                {
                mVERSION_NUMBER(); 

                }
                break;

        }

    }


    protected DFA12 dfa12 = new DFA12(this);
    protected DFA18 dfa18 = new DFA18(this);
    static final String DFA12_eotS =
        "\5\uffff";
    static final String DFA12_eofS =
        "\5\uffff";
    static final String DFA12_minS =
        "\2\56\3\uffff";
    static final String DFA12_maxS =
        "\1\71\1\145\3\uffff";
    static final String DFA12_acceptS =
        "\2\uffff\1\2\1\1\1\3";
    static final String DFA12_specialS =
        "\5\uffff}>";
    static final String[] DFA12_transitionS = {
            "\1\2\1\uffff\12\1",
            "\1\3\1\uffff\12\1\13\uffff\1\4\37\uffff\1\4",
            "",
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
            return "87:1: FLOAT : ( ( DIGIT )+ '.' ( DIGIT )* ( EXPONENT )? | '.' ( DIGIT )+ ( EXPONENT )? | ( DIGIT )+ EXPONENT );";
        }
    }
    static final String DFA18_eotS =
        "\3\uffff\37\44\1\162\1\166\12\uffff\1\170\1\173\3\uffff\1\175\1"+
        "\u0080\11\uffff\51\44\1\u00b0\1\u00b1\1\u00b2\10\44\1\uffff\1\165"+
        "\1\162\14\uffff\1\44\1\u00be\7\44\1\u00c6\24\44\1\u00dc\20\44\3"+
        "\uffff\5\44\1\u00f3\2\44\1\165\2\44\1\uffff\2\44\1\u00fb\4\44\1"+
        "\uffff\20\44\1\u0110\1\44\1\u0112\2\44\1\uffff\1\u0115\17\44\1\u0125"+
        "\5\44\1\uffff\2\44\1\uffff\3\44\1\u0130\1\uffff\7\44\1\u0138\5\44"+
        "\1\u013e\1\u013f\4\44\1\u0144\1\uffff\1\44\1\uffff\2\44\1\uffff"+
        "\17\44\1\uffff\1\44\1\u0158\2\44\1\u015b\5\44\1\uffff\3\44\1\u0168"+
        "\1\u0169\2\44\1\uffff\1\44\1\u016d\3\44\2\uffff\4\44\1\uffff\7\44"+
        "\1\u017c\1\u0168\12\44\1\uffff\1\44\1\u0188\1\uffff\5\44\1\u018e"+
        "\1\44\1\u018f\2\44\2\u0193\2\uffff\3\44\1\uffff\3\44\1\u019a\5\44"+
        "\1\u01a0\4\44\1\uffff\7\44\1\u01ac\3\44\1\uffff\1\u01b0\4\44\2\uffff"+
        "\3\44\1\uffff\1\44\1\u01ba\1\44\1\u01bd\2\44\1\uffff\1\u01c0\1\u01c2"+
        "\3\44\1\uffff\13\44\1\uffff\3\44\1\uffff\1\u01d5\2\44\1\u01d8\2"+
        "\44\1\u01db\2\44\1\uffff\1\u01de\1\44\1\uffff\1\u01e0\1\u01e1\1"+
        "\uffff\1\u01e2\1\uffff\1\44\1\u01e4\2\44\1\u01e7\10\44\1\u01f3\4"+
        "\44\1\uffff\1\u01f8\1\44\1\uffff\2\44\1\uffff\2\44\1\uffff\1\44"+
        "\3\uffff\1\44\1\uffff\2\44\1\uffff\13\44\1\uffff\1\u020d\3\44\1"+
        "\uffff\1\u0211\7\44\1\u0219\10\44\1\u0222\2\44\1\uffff\3\44\1\uffff"+
        "\2\44\1\u022a\3\44\1\u022e\1\uffff\1\44\1\u0230\1\44\1\u0232\1\u0233"+
        "\1\44\1\u0235\1\u0236\1\uffff\1\u0237\6\44\1\uffff\1\44\1\u023f"+
        "\1\44\1\uffff\1\44\1\uffff\1\44\2\uffff\1\44\3\uffff\7\44\1\uffff"+
        "\2\44\1\u024d\7\44\1\u0255\2\44\1\uffff\4\44\1\u025c\1\44\1\u025e"+
        "\1\uffff\1\44\1\u0260\1\44\1\u0262\1\44\1\u0264\1\uffff\1\u0265"+
        "\1\uffff\1\44\1\uffff\1\44\1\uffff\1\u0268\2\uffff\1\44\1\u026a"+
        "\1\uffff\1\u026b\2\uffff";
    static final String DFA18_eofS =
        "\u026c\uffff";
    static final String DFA18_minS =
        "\1\11\2\uffff\1\105\1\116\3\105\1\101\1\131\1\105\1\125\1\117\1"+
        "\103\1\142\1\105\1\122\2\101\1\103\1\122\1\104\1\146\2\101\1\145"+
        "\1\156\1\165\1\137\1\116\1\105\1\131\1\125\1\162\1\56\1\60\12\uffff"+
        "\1\76\1\55\3\uffff\2\75\11\uffff\1\122\1\104\1\143\1\107\4\105\1"+
        "\106\1\164\1\117\1\116\1\104\2\130\1\104\1\120\1\145\1\101\1\114"+
        "\1\157\1\101\1\116\1\154\1\115\1\124\1\104\1\164\1\163\1\145\2\105"+
        "\1\142\1\145\2\155\1\156\1\130\1\105\1\122\1\157\3\60\1\143\1\102"+
        "\1\154\2\141\1\164\1\163\1\151\1\uffff\1\60\1\56\14\uffff\1\102"+
        "\1\60\1\154\1\111\1\104\1\117\2\103\1\106\1\60\1\125\3\105\1\145"+
        "\1\101\1\137\1\143\1\105\1\156\1\164\2\103\1\145\1\105\1\154\1\103"+
        "\1\145\1\120\1\111\1\60\1\157\1\145\1\162\1\103\1\162\1\146\1\163"+
        "\1\141\1\160\1\145\1\165\1\145\1\116\1\106\1\101\1\154\3\uffff\1"+
        "\154\1\105\1\145\1\165\1\163\1\60\2\164\1\56\1\111\1\117\1\uffff"+
        "\1\165\1\116\1\60\1\120\3\111\1\uffff\1\120\1\122\1\103\1\114\1"+
        "\103\1\107\1\111\1\124\1\150\1\123\1\144\1\141\1\124\1\106\1\164"+
        "\1\123\1\60\1\124\1\60\1\101\1\117\1\uffff\1\60\1\162\1\167\1\157"+
        "\1\141\1\163\1\151\1\164\1\144\1\154\1\103\1\154\1\162\1\124\1\106"+
        "\1\115\1\60\1\165\1\114\2\164\1\171\1\uffff\2\145\1\uffff\1\117"+
        "\1\123\1\144\1\60\1\uffff\1\124\2\105\1\130\1\123\1\101\1\154\1"+
        "\60\1\157\1\107\1\124\2\117\2\60\1\162\1\111\1\151\1\103\1\60\1"+
        "\uffff\1\111\1\uffff\1\122\1\116\1\uffff\1\166\1\162\1\156\1\162"+
        "\1\145\1\170\1\141\1\171\1\145\1\157\2\141\2\111\1\105\1\uffff\1"+
        "\144\1\60\1\145\1\171\1\60\1\160\1\115\1\116\1\105\1\145\1\uffff"+
        "\1\111\2\123\2\60\1\124\1\114\1\uffff\1\156\1\60\1\105\1\111\1\156"+
        "\2\uffff\1\164\1\117\1\154\1\157\1\uffff\1\117\1\124\1\123\1\141"+
        "\1\151\1\143\1\141\2\60\1\156\2\137\1\156\2\164\1\124\1\130\1\124"+
        "\1\145\1\uffff\1\115\1\60\1\uffff\1\163\1\105\1\102\1\146\1\115"+
        "\1\60\1\40\1\60\1\137\1\117\2\60\2\uffff\2\105\1\156\1\uffff\1\122"+
        "\1\103\1\143\1\60\1\116\1\145\1\156\1\116\1\115\1\60\1\142\1\164"+
        "\1\145\1\155\1\uffff\1\143\1\163\1\164\1\143\2\145\1\105\1\60\1"+
        "\105\1\137\1\157\1\uffff\1\60\1\124\1\115\1\151\1\114\2\uffff\1"+
        "\160\1\116\1\101\1\uffff\1\116\1\60\1\145\1\60\1\110\1\145\1\uffff"+
        "\2\60\1\143\1\123\1\105\1\uffff\1\154\1\145\1\156\2\145\1\164\1"+
        "\151\1\145\2\137\1\122\1\uffff\1\122\1\160\1\154\1\uffff\1\60\1"+
        "\114\1\154\1\60\1\145\1\162\1\60\1\102\1\105\1\uffff\1\60\1\143"+
        "\1\uffff\2\60\1\uffff\1\60\1\uffff\1\145\1\60\1\116\1\145\1\60\2"+
        "\164\1\125\1\141\1\155\3\156\1\60\1\123\1\145\1\162\1\145\1\uffff"+
        "\1\60\1\145\1\uffff\1\141\1\157\1\uffff\1\105\1\124\1\uffff\1\164"+
        "\3\uffff\1\156\1\uffff\1\124\1\163\1\uffff\1\162\1\145\1\156\1\164"+
        "\1\145\1\164\1\144\1\163\1\146\1\154\1\145\1\uffff\1\60\1\141\1"+
        "\157\1\143\1\uffff\1\60\1\143\1\144\1\114\1\127\1\145\1\164\1\123"+
        "\1\60\1\141\1\162\1\151\1\145\1\163\1\162\1\145\1\141\1\60\1\141"+
        "\1\164\1\uffff\1\143\1\144\1\165\1\uffff\1\164\1\165\1\60\1\117"+
        "\1\144\1\162\1\60\1\uffff\1\164\1\60\1\164\2\60\1\141\2\60\1\uffff"+
        "\1\60\1\167\1\164\1\165\1\154\1\141\1\143\1\uffff\1\122\1\60\1\141"+
        "\1\uffff\1\151\1\uffff\1\163\2\uffff\1\164\3\uffff\1\157\1\141\1"+
        "\143\1\145\1\156\1\164\1\113\1\uffff\1\164\1\157\1\60\1\151\1\162"+
        "\1\156\1\164\1\163\1\164\1\163\1\60\1\151\1\156\1\uffff\1\157\1"+
        "\153\1\164\1\163\1\60\1\163\1\60\1\uffff\1\157\1\60\1\156\1\60\1"+
        "\163\1\60\1\uffff\1\60\1\uffff\1\156\1\uffff\1\163\1\uffff\1\60"+
        "\2\uffff\1\163\1\60\1\uffff\1\60\2\uffff";
    static final String DFA18_maxS =
        "\1\176\2\uffff\1\145\1\170\1\145\1\160\1\162\1\157\1\171\2\165\1"+
        "\157\1\165\1\166\1\165\1\162\1\157\1\141\1\164\1\122\1\104\1\156"+
        "\1\101\1\141\1\145\1\156\1\165\1\157\1\156\1\165\1\171\1\165\1\162"+
        "\1\145\1\71\12\uffff\1\76\1\75\3\uffff\1\75\1\76\11\uffff\1\162"+
        "\1\144\1\143\1\147\4\145\1\106\1\164\1\157\1\116\1\166\2\170\1\154"+
        "\1\160\2\163\1\154\1\157\1\141\1\156\1\154\1\155\1\164\1\104\1\164"+
        "\1\163\1\145\1\164\1\145\1\146\1\145\1\166\1\155\1\156\1\170\1\111"+
        "\1\162\1\157\3\172\1\143\1\102\1\154\2\141\1\164\1\163\1\151\1\uffff"+
        "\1\71\1\145\14\uffff\1\163\1\172\1\154\1\151\1\144\1\117\2\143\1"+
        "\106\1\172\1\165\1\105\3\145\2\137\1\143\1\145\1\156\1\164\1\144"+
        "\1\143\2\145\1\154\1\143\1\145\1\160\1\151\1\172\1\157\1\145\1\162"+
        "\1\120\1\162\1\146\1\163\1\141\1\160\1\145\1\165\1\145\1\116\1\106"+
        "\1\141\1\154\3\uffff\1\154\1\105\1\145\1\165\1\163\1\172\2\164\1"+
        "\56\1\151\1\157\1\uffff\1\165\1\156\1\172\1\120\2\151\1\111\1\uffff"+
        "\1\160\1\122\1\143\1\154\1\103\1\107\1\151\1\124\1\150\1\163\1\144"+
        "\1\141\1\164\1\106\1\164\1\163\1\172\1\164\1\172\1\141\1\157\1\uffff"+
        "\1\172\1\162\1\167\1\157\1\141\1\163\1\151\1\164\1\144\1\154\1\103"+
        "\1\154\1\162\1\124\1\106\1\155\1\172\1\165\1\114\2\164\1\171\1\uffff"+
        "\2\145\1\uffff\1\157\1\163\1\144\1\172\1\uffff\1\124\2\145\1\130"+
        "\1\163\1\101\1\165\1\172\1\157\1\107\1\164\2\117\2\172\1\162\2\151"+
        "\1\103\1\172\1\uffff\1\151\1\uffff\1\162\1\156\1\uffff\1\166\1\162"+
        "\1\156\1\162\1\145\1\170\1\141\1\171\1\145\1\157\2\141\2\111\1\145"+
        "\1\uffff\1\144\1\172\1\145\1\171\1\172\1\160\1\130\1\156\2\145\1"+
        "\uffff\1\111\2\163\2\172\1\124\1\154\1\uffff\1\156\1\172\1\145\1"+
        "\111\1\156\2\uffff\1\164\1\157\1\154\1\157\1\uffff\1\157\1\164\1"+
        "\163\1\141\1\151\1\143\1\141\2\172\1\156\2\137\1\156\2\164\1\124"+
        "\1\130\1\164\1\145\1\uffff\1\115\1\172\1\uffff\1\163\1\105\1\102"+
        "\1\146\1\115\1\172\1\40\1\172\1\137\1\117\2\172\2\uffff\1\105\1"+
        "\145\1\156\1\uffff\1\162\1\103\1\143\1\172\1\156\1\145\2\156\1\155"+
        "\1\172\1\142\1\164\1\145\1\155\1\uffff\1\143\1\163\1\164\1\143\2"+
        "\145\1\105\1\172\1\145\1\137\1\157\1\uffff\1\172\1\124\1\115\1\151"+
        "\1\114\2\uffff\1\162\1\116\1\101\1\uffff\1\116\1\172\1\145\1\172"+
        "\1\110\1\145\1\uffff\2\172\1\143\1\163\1\145\1\uffff\1\154\1\145"+
        "\1\156\2\145\1\164\1\151\1\145\2\137\1\122\1\uffff\2\162\1\154\1"+
        "\uffff\1\172\1\114\1\154\1\172\1\145\1\162\1\172\1\102\1\105\1\uffff"+
        "\1\172\1\143\1\uffff\2\172\1\uffff\1\172\1\uffff\1\145\1\172\1\156"+
        "\1\145\1\172\2\164\1\125\1\141\1\155\1\156\1\163\1\156\1\172\1\163"+
        "\1\145\1\162\1\145\1\uffff\1\172\1\145\1\uffff\1\141\1\157\1\uffff"+
        "\1\105\1\124\1\uffff\1\164\3\uffff\1\156\1\uffff\1\164\1\163\1\uffff"+
        "\1\162\1\145\1\156\1\164\1\145\1\164\1\144\1\163\1\146\1\154\1\145"+
        "\1\uffff\1\172\1\141\1\157\1\143\1\uffff\1\172\1\143\1\144\1\114"+
        "\1\127\1\145\1\164\1\163\1\172\1\141\1\162\1\151\1\145\1\163\1\162"+
        "\1\145\1\141\1\172\1\141\1\164\1\uffff\1\143\1\144\1\165\1\uffff"+
        "\1\164\1\165\1\172\1\117\1\144\1\162\1\172\1\uffff\1\164\1\172\1"+
        "\164\2\172\1\141\2\172\1\uffff\1\172\1\167\1\164\1\165\1\154\1\141"+
        "\1\143\1\uffff\1\122\1\172\1\141\1\uffff\1\151\1\uffff\1\163\2\uffff"+
        "\1\164\3\uffff\1\157\1\141\1\143\1\145\1\156\1\164\1\113\1\uffff"+
        "\1\164\1\157\1\172\1\151\1\162\1\156\1\164\1\163\1\164\1\163\1\172"+
        "\1\151\1\156\1\uffff\1\157\1\153\1\164\1\163\1\172\1\163\1\172\1"+
        "\uffff\1\157\1\172\1\156\1\172\1\163\1\172\1\uffff\1\172\1\uffff"+
        "\1\156\1\uffff\1\163\1\uffff\1\172\2\uffff\1\163\1\172\1\uffff\1"+
        "\172\2\uffff";
    static final String DFA18_acceptS =
        "\1\uffff\1\1\1\2\41\uffff\1\113\1\114\1\115\1\116\1\117\1\120\1"+
        "\121\1\122\1\124\1\125\2\uffff\1\130\1\131\1\132\2\uffff\1\142\1"+
        "\143\1\145\1\146\1\147\1\150\1\151\1\152\1\153\64\uffff\1\112\2"+
        "\uffff\1\111\1\123\1\126\1\144\1\127\1\135\1\136\1\133\1\134\1\137"+
        "\1\140\1\141\57\uffff\1\35\1\36\1\37\13\uffff\1\4\7\uffff\1\110"+
        "\25\uffff\1\34\26\uffff\1\53\2\uffff\1\154\4\uffff\1\6\24\uffff"+
        "\1\61\1\uffff\1\75\2\uffff\1\50\17\uffff\1\60\12\uffff\1\5\7\uffff"+
        "\1\13\5\uffff\1\14\1\64\4\uffff\1\17\23\uffff\1\42\2\uffff\1\52"+
        "\14\uffff\1\41\1\10\3\uffff\1\27\16\uffff\1\63\13\uffff\1\51\5\uffff"+
        "\1\3\1\44\3\uffff\1\7\6\uffff\1\65\5\uffff\1\22\13\uffff\1\40\3"+
        "\uffff\1\66\11\uffff\1\11\2\uffff\1\30\2\uffff\1\15\1\uffff\1\74"+
        "\22\uffff\1\76\2\uffff\1\101\2\uffff\1\24\2\uffff\1\12\1\uffff\1"+
        "\31\1\45\1\16\1\uffff\1\20\2\uffff\1\32\13\uffff\1\33\4\uffff\1"+
        "\77\24\uffff\1\43\3\uffff\1\100\7\uffff\1\23\10\uffff\1\72\7\uffff"+
        "\1\25\3\uffff\1\21\1\uffff\1\103\1\uffff\1\62\1\67\1\uffff\1\70"+
        "\1\71\1\73\7\uffff\1\47\15\uffff\1\107\7\uffff\1\26\6\uffff\1\46"+
        "\1\uffff\1\57\1\uffff\1\102\1\uffff\1\106\1\uffff\1\55\1\56\2\uffff"+
        "\1\54\1\uffff\1\104\1\105";
    static final String DFA18_specialS =
        "\u026c\uffff}>";
    static final String[] DFA18_transitionS = {
            "\2\2\2\uffff\1\2\22\uffff\1\2\1\73\1\74\1\1\1\60\1\71\1\75\1"+
            "\uffff\1\54\1\55\1\66\1\67\1\53\1\56\1\43\1\65\12\42\1\46\1"+
            "\45\1\57\1\64\1\63\1\72\1\62\1\15\1\5\1\14\1\31\1\35\1\40\1"+
            "\7\1\32\1\25\2\44\1\27\1\10\1\44\1\24\1\22\1\33\1\36\1\6\1\37"+
            "\1\44\1\3\4\44\1\47\1\2\1\50\1\70\2\uffff\1\23\1\5\1\14\1\44"+
            "\1\4\1\13\1\20\1\44\1\26\3\44\1\21\1\34\1\16\1\30\1\44\1\12"+
            "\1\17\1\11\1\44\1\3\1\41\3\44\1\51\1\uffff\1\52\1\61",
            "",
            "",
            "\1\76\37\uffff\1\76",
            "\1\77\37\uffff\1\77\11\uffff\1\100",
            "\1\101\37\uffff\1\101",
            "\1\102\12\uffff\1\103\4\uffff\1\106\13\uffff\1\107\3\uffff"+
            "\1\104\12\uffff\1\105",
            "\1\111\14\uffff\1\110\37\uffff\1\110",
            "\1\113\15\uffff\1\115\21\uffff\1\114\15\uffff\1\112",
            "\1\116\5\uffff\1\117\31\uffff\1\116",
            "\1\123\17\uffff\1\121\17\uffff\1\120\16\uffff\1\122\1\121",
            "\1\124\23\uffff\1\125\13\uffff\1\124",
            "\1\126\37\uffff\1\126",
            "\1\127\12\uffff\1\130\24\uffff\1\127\21\uffff\1\131",
            "\1\132\23\uffff\1\133",
            "\1\104\12\uffff\1\105\20\uffff\1\140\3\uffff\1\134\3\uffff"+
            "\1\141\6\uffff\1\135\3\uffff\1\137\1\136",
            "\1\110\22\uffff\1\142\14\uffff\1\110",
            "\1\143\15\uffff\1\115\21\uffff\1\143\15\uffff\1\115",
            "\1\145\20\uffff\1\144\16\uffff\1\145",
            "\1\127\37\uffff\1\127\20\uffff\1\146",
            "\1\147",
            "\1\150",
            "\1\151\7\uffff\1\152",
            "\1\153",
            "\1\145\37\uffff\1\145",
            "\1\154",
            "\1\155",
            "\1\156",
            "\1\160\17\uffff\1\157",
            "\1\77\37\uffff\1\77",
            "\1\123\17\uffff\1\121\17\uffff\1\123\17\uffff\1\121",
            "\1\116\37\uffff\1\116",
            "\1\124\37\uffff\1\124",
            "\1\161",
            "\1\163\1\uffff\12\164\13\uffff\1\165\37\uffff\1\165",
            "\12\165",
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
            "\1\167",
            "\1\171\17\uffff\1\172",
            "",
            "",
            "",
            "\1\174",
            "\1\177\1\176",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "\1\u0081\37\uffff\1\u0081",
            "\1\u0082\37\uffff\1\u0082",
            "\1\u0083",
            "\1\u0084\37\uffff\1\u0084",
            "\1\u0085\16\uffff\1\u0086\20\uffff\1\u0085",
            "\1\u0087\37\uffff\1\u0088",
            "\1\u0085\37\uffff\1\u0085",
            "\1\u0088\37\uffff\1\u0088",
            "\1\u0089",
            "\1\u008a",
            "\1\u008b\37\uffff\1\u008b",
            "\1\u008c",
            "\1\u008e\7\uffff\1\u008d\27\uffff\1\u008e\7\uffff\1\u008d\11"+
            "\uffff\1\u008f",
            "\1\u0090\37\uffff\1\u0091",
            "\1\u0091\33\uffff\1\u0092\3\uffff\1\u0091",
            "\1\u008e\7\uffff\1\u008d\27\uffff\1\u008e\7\uffff\1\u008d",
            "\1\u0093\37\uffff\1\u0093",
            "\1\u0094\15\uffff\1\u0095",
            "\1\u0097\37\uffff\1\u0096\21\uffff\1\u0098",
            "\1\u0099\37\uffff\1\u0099",
            "\1\u009a",
            "\1\u0097\37\uffff\1\u0097",
            "\1\u009b\37\uffff\1\u009b",
            "\1\u009c",
            "\1\u009d\37\uffff\1\u009d",
            "\1\u009e\37\uffff\1\u009e",
            "\1\u009f",
            "\1\u00a0",
            "\1\u00a1",
            "\1\u00a2",
            "\1\u0085\37\uffff\1\u0085\16\uffff\1\u00a3",
            "\1\u0088\33\uffff\1\u00a4\3\uffff\1\u0088",
            "\1\u00a6\3\uffff\1\u00a5",
            "\1\u00a7",
            "\1\u00a8\10\uffff\1\u00a9",
            "\1\u00aa",
            "\1\u00ab",
            "\1\u0091\37\uffff\1\u0091",
            "\1\u00ad\3\uffff\1\u00ac",
            "\1\u00ae\37\uffff\1\u00ae",
            "\1\u00af",
            "\12\44\7\uffff\32\44\4\uffff\1\44\1\uffff\32\44",
            "\12\44\7\uffff\32\44\4\uffff\1\44\1\uffff\32\44",
            "\12\44\7\uffff\32\44\4\uffff\1\44\1\uffff\32\44",
            "\1\u00b3",
            "\1\u00b4",
            "\1\u00b5",
            "\1\u00b6",
            "\1\u00b7",
            "\1\u00b8",
            "\1\u00b9",
            "\1\u00ba",
            "",
            "\12\u00bb",
            "\1\165\1\uffff\12\164\13\uffff\1\165\37\uffff\1\165",
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
            "\1\u00bd\20\uffff\1\u00bc\16\uffff\1\u00bd\20\uffff\1\u00bc",
            "\12\44\7\uffff\32\44\4\uffff\1\44\1\uffff\32\44",
            "\1\u00bf",
            "\1\u00c0\37\uffff\1\u00c0",
            "\1\u00c1\37\uffff\1\u00c1",
            "\1\u00c2",
            "\1\u00c3\37\uffff\1\u00c4",
            "\1\u00c4\37\uffff\1\u00c4",
            "\1\u00c5",
            "\12\44\7\uffff\32\44\4\uffff\1\44\1\uffff\32\44",
            "\1\u00c7\37\uffff\1\u00c7",
            "\1\u00c8",
            "\1\u00c9\37\uffff\1\u00c9",
            "\1\u00ca\37\uffff\1\u00ca",
            "\1\u00cb",
            "\1\u00cc\21\uffff\1\u00ce\13\uffff\1\u00cd",
            "\1\u00cd",
            "\1\u00cf",
            "\1\u00d0\37\uffff\1\u00d0",
            "\1\u00d1",
            "\1\u00d2",
            "\1\u00d3\37\uffff\1\u00d3\1\u00d4",
            "\1\u00d3\37\uffff\1\u00d3",
            "\1\u00d5",
            "\1\u00d6\37\uffff\1\u00d6",
            "\1\u00d7",
            "\1\u00d8\37\uffff\1\u00d8",
            "\1\u00d9",
            "\1\u00da\37\uffff\1\u00da",
            "\1\u00db\37\uffff\1\u00db",
            "\12\44\7\uffff\32\44\4\uffff\1\44\1\uffff\32\44",
            "\1\u00dd",
            "\1\u00de",
            "\1\u00df",
            "\1\u00e0\14\uffff\1\u00e1",
            "\1\u00e2",
            "\1\u00e3",
            "\1\u00e4",
            "\1\u00e5",
            "\1\u00e6",
            "\1\u00e7",
            "\1\u00e8",
            "\1\u00e9",
            "\1\u00ea",
            "\1\u00eb",
            "\1\u00ec\37\uffff\1\u00ec",
            "\1\u00ed",
            "",
            "",
            "",
            "\1\u00ee",
            "\1\u00ef",
            "\1\u00f0",
            "\1\u00f1",
            "\1\u00f2",
            "\12\44\7\uffff\32\44\4\uffff\1\44\1\uffff\32\44",
            "\1\u00f4",
            "\1\u00f5",
            "\1\u00f6",
            "\1\u00f7\37\uffff\1\u00f7",
            "\1\u00f8\37\uffff\1\u00f8",
            "",
            "\1\u00f9",
            "\1\u00fa\37\uffff\1\u00fa",
            "\12\44\7\uffff\32\44\4\uffff\1\44\1\uffff\32\44",
            "\1\u00fc",
            "\1\u00fd\37\uffff\1\u00fe",
            "\1\u00fe\37\uffff\1\u00fe",
            "\1\u00ff",
            "",
            "\1\u0100\37\uffff\1\u0100",
            "\1\u0101",
            "\1\u0102\37\uffff\1\u0102",
            "\1\u0103\37\uffff\1\u0103",
            "\1\u0104",
            "\1\u0105",
            "\1\u0106\37\uffff\1\u0106",
            "\1\u0107",
            "\1\u0108",
            "\1\u0109\37\uffff\1\u0109",
            "\1\u010a",
            "\1\u010b",
            "\1\u010c\37\uffff\1\u010c",
            "\1\u010d",
            "\1\u010e",
            "\1\u010f\37\uffff\1\u010f",
            "\12\44\7\uffff\32\44\4\uffff\1\44\1\uffff\32\44",
            "\1\u0111\37\uffff\1\u0111",
            "\12\44\7\uffff\32\44\4\uffff\1\44\1\uffff\32\44",
            "\1\u0113\37\uffff\1\u0113",
            "\1\u0114\37\uffff\1\u0114",
            "",
            "\12\44\7\uffff\32\44\4\uffff\1\44\1\uffff\32\44",
            "\1\u0116",
            "\1\u0117",
            "\1\u0118",
            "\1\u0119",
            "\1\u011a",
            "\1\u011b",
            "\1\u011c",
            "\1\u011d",
            "\1\u011e",
            "\1\u011f",
            "\1\u0120",
            "\1\u0121",
            "\1\u0122",
            "\1\u0123",
            "\1\u0124\37\uffff\1\u0124",
            "\12\44\7\uffff\32\44\4\uffff\1\44\1\uffff\32\44",
            "\1\u0126",
            "\1\u0127",
            "\1\u0128",
            "\1\u0129",
            "\1\u012a",
            "",
            "\1\u012b",
            "\1\u012c",
            "",
            "\1\u012d\37\uffff\1\u012d",
            "\1\u012e\37\uffff\1\u012e",
            "\1\u012f",
            "\12\44\7\uffff\32\44\4\uffff\1\44\1\uffff\32\44",
            "",
            "\1\u0131",
            "\1\u0132\37\uffff\1\u0133",
            "\1\u0133\37\uffff\1\u0133",
            "\1\u0134",
            "\1\u0135\37\uffff\1\u0135",
            "\1\u0136",
            "\1\u0137\10\uffff\1\u0137",
            "\12\44\7\uffff\32\44\4\uffff\1\44\1\uffff\32\44",
            "\1\u0139",
            "\1\u013a",
            "\1\u013b\37\uffff\1\u013b",
            "\1\u013c",
            "\1\u013d",
            "\12\44\7\uffff\32\44\4\uffff\1\44\1\uffff\32\44",
            "\12\44\7\uffff\32\44\4\uffff\1\44\1\uffff\32\44",
            "\1\u0140",
            "\1\u0141\37\uffff\1\u0141",
            "\1\u0142",
            "\1\u0143",
            "\12\44\7\uffff\32\44\4\uffff\1\44\1\uffff\32\44",
            "",
            "\1\u0145\37\uffff\1\u0145",
            "",
            "\1\u0146\37\uffff\1\u0146",
            "\1\u0147\37\uffff\1\u0147",
            "",
            "\1\u0148",
            "\1\u0149",
            "\1\u014a",
            "\1\u014b",
            "\1\u014c",
            "\1\u014d",
            "\1\u014e",
            "\1\u014f",
            "\1\u0150",
            "\1\u0151",
            "\1\u0152",
            "\1\u0153",
            "\1\u0154",
            "\1\u0155",
            "\1\u0156\37\uffff\1\u0156",
            "",
            "\1\u0157",
            "\12\44\7\uffff\32\44\4\uffff\1\44\1\uffff\32\44",
            "\1\u0159",
            "\1\u015a",
            "\12\44\7\uffff\32\44\4\uffff\1\44\1\uffff\32\44",
            "\1\u015c",
            "\1\u015f\1\u015d\4\uffff\1\u015e\4\uffff\1\u0160",
            "\1\u0161\37\uffff\1\u0161",
            "\1\u0162\37\uffff\1\u0163",
            "\1\u0164",
            "",
            "\1\u0165",
            "\1\u0166\37\uffff\1\u0167",
            "\1\u0167\37\uffff\1\u0167",
            "\12\44\7\uffff\32\44\4\uffff\1\44\1\uffff\32\44",
            "\12\44\7\uffff\32\44\4\uffff\1\44\1\uffff\32\44",
            "\1\u016a",
            "\1\u016b\37\uffff\1\u016b",
            "",
            "\1\u016c",
            "\12\44\7\uffff\32\44\4\uffff\1\44\1\uffff\32\44",
            "\1\u016e\37\uffff\1\u016e",
            "\1\u016f",
            "\1\u0170",
            "",
            "",
            "\1\u0171",
            "\1\u0172\37\uffff\1\u0172",
            "\1\u0173",
            "\1\u0174",
            "",
            "\1\u0175\37\uffff\1\u0175",
            "\1\u0176\37\uffff\1\u0176",
            "\1\u0177\37\uffff\1\u0177",
            "\1\u0178",
            "\1\u0179",
            "\1\u017a",
            "\1\u017b",
            "\12\44\7\uffff\32\44\4\uffff\1\44\1\uffff\32\44",
            "\12\44\7\uffff\32\44\4\uffff\1\44\1\uffff\32\44",
            "\1\u017d",
            "\1\u017e",
            "\1\u017f",
            "\1\u0180",
            "\1\u0181",
            "\1\u0182",
            "\1\u0183",
            "\1\u0184",
            "\1\u0185\37\uffff\1\u0185",
            "\1\u0186",
            "",
            "\1\u0187",
            "\12\44\7\uffff\32\44\4\uffff\1\44\1\uffff\32\44",
            "",
            "\1\u0189",
            "\1\u018a",
            "\1\u018b",
            "\1\u018c",
            "\1\u018d",
            "\12\44\7\uffff\32\44\4\uffff\1\44\1\uffff\32\44",
            "\1\u018f",
            "\12\44\7\uffff\32\44\4\uffff\1\44\1\uffff\32\44",
            "\1\u0190",
            "\1\u0191",
            "\12\44\7\uffff\13\44\1\u0192\16\44\4\uffff\1\44\1\uffff\32"+
            "\44",
            "\12\44\7\uffff\32\44\4\uffff\1\44\1\uffff\32\44",
            "",
            "",
            "\1\u0194",
            "\1\u0195\37\uffff\1\u0195",
            "\1\u0196",
            "",
            "\1\u0197\37\uffff\1\u0197",
            "\1\u0198",
            "\1\u0199",
            "\12\44\7\uffff\32\44\4\uffff\1\44\1\uffff\32\44",
            "\1\u019b\37\uffff\1\u019b",
            "\1\u019c",
            "\1\u019d",
            "\1\u019e\37\uffff\1\u019e",
            "\1\u019f\37\uffff\1\u019f",
            "\12\44\7\uffff\32\44\4\uffff\1\44\1\uffff\32\44",
            "\1\u01a1",
            "\1\u01a2",
            "\1\u01a3",
            "\1\u01a4",
            "",
            "\1\u01a5",
            "\1\u01a6",
            "\1\u01a7",
            "\1\u01a8",
            "\1\u01a9",
            "\1\u01aa",
            "\1\u01ab",
            "\12\44\7\uffff\32\44\4\uffff\1\44\1\uffff\32\44",
            "\1\u01ad\37\uffff\1\u01ad",
            "\1\u01ae",
            "\1\u01af",
            "",
            "\12\44\7\uffff\32\44\4\uffff\1\44\1\uffff\32\44",
            "\1\u01b1",
            "\1\u01b2",
            "\1\u01b3",
            "\1\u01b4",
            "",
            "",
            "\1\u01b6\1\uffff\1\u01b5",
            "\1\u01b7",
            "\1\u01b8",
            "",
            "\1\u01b9",
            "\12\44\7\uffff\22\44\1\u01bb\7\44\4\uffff\1\44\1\uffff\22\44"+
            "\1\u01bb\7\44",
            "\1\u01bc",
            "\12\44\7\uffff\32\44\4\uffff\1\44\1\uffff\32\44",
            "\1\u01be",
            "\1\u01bf",
            "",
            "\12\44\7\uffff\22\44\1\u01c1\7\44\4\uffff\1\44\1\uffff\22\44"+
            "\1\u01c1\7\44",
            "\12\44\7\uffff\32\44\4\uffff\1\44\1\uffff\32\44",
            "\1\u01c3",
            "\1\u01c4\37\uffff\1\u01c4",
            "\1\u01c5\37\uffff\1\u01c5",
            "",
            "\1\u01c6",
            "\1\u01c7",
            "\1\u01c8",
            "\1\u01c9",
            "\1\u01ca",
            "\1\u01cb",
            "\1\u01cc",
            "\1\u01cd",
            "\1\u01ce",
            "\1\u01cf",
            "\1\u01d0",
            "",
            "\1\u01d1\37\uffff\1\u01d1",
            "\1\u01d3\1\uffff\1\u01d2",
            "\1\u01d4",
            "",
            "\12\44\7\uffff\32\44\4\uffff\1\44\1\uffff\32\44",
            "\1\u01d6",
            "\1\u01d7",
            "\12\44\7\uffff\32\44\4\uffff\1\44\1\uffff\32\44",
            "\1\u01d9",
            "\1\u01da",
            "\12\44\7\uffff\32\44\4\uffff\1\44\1\uffff\32\44",
            "\1\u01dc",
            "\1\u01dd",
            "",
            "\12\44\7\uffff\32\44\4\uffff\1\44\1\uffff\32\44",
            "\1\u01df",
            "",
            "\12\44\7\uffff\32\44\4\uffff\1\44\1\uffff\32\44",
            "\12\44\7\uffff\32\44\4\uffff\1\44\1\uffff\32\44",
            "",
            "\12\44\7\uffff\32\44\4\uffff\1\44\1\uffff\32\44",
            "",
            "\1\u01e3",
            "\12\44\7\uffff\32\44\4\uffff\1\44\1\uffff\32\44",
            "\1\u01e5\37\uffff\1\u01e5",
            "\1\u01e6",
            "\12\44\7\uffff\32\44\4\uffff\1\44\1\uffff\32\44",
            "\1\u01e8",
            "\1\u01e9",
            "\1\u01ea",
            "\1\u01eb",
            "\1\u01ec",
            "\1\u01ed",
            "\1\u01f0\1\u01ee\1\u01f1\2\uffff\1\u01ef",
            "\1\u01f2",
            "\12\44\7\uffff\32\44\4\uffff\1\44\1\uffff\32\44",
            "\1\u01f4\37\uffff\1\u01f4",
            "\1\u01f5",
            "\1\u01f6",
            "\1\u01f7",
            "",
            "\12\44\7\uffff\32\44\4\uffff\1\44\1\uffff\32\44",
            "\1\u01f9",
            "",
            "\1\u01fa",
            "\1\u01fb",
            "",
            "\1\u01fc",
            "\1\u01fd",
            "",
            "\1\u01fe",
            "",
            "",
            "",
            "\1\u01ff",
            "",
            "\1\u0200\37\uffff\1\u0200",
            "\1\u0201",
            "",
            "\1\u0202",
            "\1\u0203",
            "\1\u0204",
            "\1\u0205",
            "\1\u0206",
            "\1\u0207",
            "\1\u0208",
            "\1\u0209",
            "\1\u020a",
            "\1\u020b",
            "\1\u020c",
            "",
            "\12\44\7\uffff\32\44\4\uffff\1\44\1\uffff\32\44",
            "\1\u020e",
            "\1\u020f",
            "\1\u0210",
            "",
            "\12\44\7\uffff\32\44\4\uffff\1\44\1\uffff\32\44",
            "\1\u0212",
            "\1\u0213",
            "\1\u0214",
            "\1\u0215",
            "\1\u0216",
            "\1\u0217",
            "\1\u0218\37\uffff\1\u0218",
            "\12\44\7\uffff\32\44\4\uffff\1\44\1\uffff\32\44",
            "\1\u021a",
            "\1\u021b",
            "\1\u021c",
            "\1\u021d",
            "\1\u021e",
            "\1\u021f",
            "\1\u0220",
            "\1\u0221",
            "\12\44\7\uffff\32\44\4\uffff\1\44\1\uffff\32\44",
            "\1\u0223",
            "\1\u0224",
            "",
            "\1\u0225",
            "\1\u0226",
            "\1\u0227",
            "",
            "\1\u0228",
            "\1\u0229",
            "\12\44\7\uffff\32\44\4\uffff\1\44\1\uffff\32\44",
            "\1\u022b",
            "\1\u022c",
            "\1\u022d",
            "\12\44\7\uffff\32\44\4\uffff\1\44\1\uffff\32\44",
            "",
            "\1\u022f",
            "\12\44\7\uffff\32\44\4\uffff\1\44\1\uffff\32\44",
            "\1\u0231",
            "\12\44\7\uffff\32\44\4\uffff\1\44\1\uffff\32\44",
            "\12\44\7\uffff\32\44\4\uffff\1\44\1\uffff\32\44",
            "\1\u0234",
            "\12\44\7\uffff\32\44\4\uffff\1\44\1\uffff\32\44",
            "\12\44\7\uffff\32\44\4\uffff\1\44\1\uffff\32\44",
            "",
            "\12\44\7\uffff\32\44\4\uffff\1\44\1\uffff\32\44",
            "\1\u0238",
            "\1\u0239",
            "\1\u023a",
            "\1\u023b",
            "\1\u023c",
            "\1\u023d",
            "",
            "\1\u023e",
            "\12\44\7\uffff\32\44\4\uffff\1\44\1\uffff\32\44",
            "\1\u0240",
            "",
            "\1\u0241",
            "",
            "\1\u0242",
            "",
            "",
            "\1\u0243",
            "",
            "",
            "",
            "\1\u0244",
            "\1\u0245",
            "\1\u0246",
            "\1\u0247",
            "\1\u0248",
            "\1\u0249",
            "\1\u024a",
            "",
            "\1\u024b",
            "\1\u024c",
            "\12\44\7\uffff\32\44\4\uffff\1\44\1\uffff\32\44",
            "\1\u024e",
            "\1\u024f",
            "\1\u0250",
            "\1\u0251",
            "\1\u0252",
            "\1\u0253",
            "\1\u0254",
            "\12\44\7\uffff\32\44\4\uffff\1\44\1\uffff\32\44",
            "\1\u0256",
            "\1\u0257",
            "",
            "\1\u0258",
            "\1\u0259",
            "\1\u025a",
            "\1\u025b",
            "\12\44\7\uffff\32\44\4\uffff\1\44\1\uffff\32\44",
            "\1\u025d",
            "\12\44\7\uffff\32\44\4\uffff\1\44\1\uffff\32\44",
            "",
            "\1\u025f",
            "\12\44\7\uffff\32\44\4\uffff\1\44\1\uffff\32\44",
            "\1\u0261",
            "\12\44\7\uffff\32\44\4\uffff\1\44\1\uffff\32\44",
            "\1\u0263",
            "\12\44\7\uffff\32\44\4\uffff\1\44\1\uffff\32\44",
            "",
            "\12\44\7\uffff\32\44\4\uffff\1\44\1\uffff\32\44",
            "",
            "\1\u0266",
            "",
            "\1\u0267",
            "",
            "\12\44\7\uffff\32\44\4\uffff\1\44\1\uffff\32\44",
            "",
            "",
            "\1\u0269",
            "\12\44\7\uffff\32\44\4\uffff\1\44\1\uffff\32\44",
            "",
            "\12\44\7\uffff\32\44\4\uffff\1\44\1\uffff\32\44",
            "",
            ""
    };

    static final short[] DFA18_eot = DFA.unpackEncodedString(DFA18_eotS);
    static final short[] DFA18_eof = DFA.unpackEncodedString(DFA18_eofS);
    static final char[] DFA18_min = DFA.unpackEncodedStringToUnsignedChars(DFA18_minS);
    static final char[] DFA18_max = DFA.unpackEncodedStringToUnsignedChars(DFA18_maxS);
    static final short[] DFA18_accept = DFA.unpackEncodedString(DFA18_acceptS);
    static final short[] DFA18_special = DFA.unpackEncodedString(DFA18_specialS);
    static final short[][] DFA18_transition;

    static {
        int numStates = DFA18_transitionS.length;
        DFA18_transition = new short[numStates][];
        for (int i=0; i<numStates; i++) {
            DFA18_transition[i] = DFA.unpackEncodedString(DFA18_transitionS[i]);
        }
    }

    class DFA18 extends DFA {

        public DFA18(BaseRecognizer recognizer) {
            this.recognizer = recognizer;
            this.decisionNumber = 18;
            this.eot = DFA18_eot;
            this.eof = DFA18_eof;
            this.min = DFA18_min;
            this.max = DFA18_max;
            this.accept = DFA18_accept;
            this.special = DFA18_special;
            this.transition = DFA18_transition;
        }
        public String getDescription() {
            return "1:1: Tokens : ( LINE_COMMENT | WS | VERSION | END | BEGIN | SEED | SPECIES | GROUPS | MOLECULE | MOLECULES | MODEL | TYPES | REACTION | REACTIONS | RULES | FUNCTIONS | COMPARTMENTS | ACTIONS | OBSERVABLES | SET_OPTION | SPECIES_LABEL | GENERATE_NETWORK | MAX_AGG | MAX_ITER | MAX_STOICH | OVERWRITE | PRINT_ITER | AND | OR | ID | IF | PREFFIX | SUFFIX | LABEL | PARAMETERS | VERBOSE | MATCHONCE | DELETEMOLECULES | MOVECONNECTED | AUTO | HNAUTY | QUASY | NOT | INCLUDE_REACTANTS | INCLUDE_PRODUCTS | EXCLUDE_REACTANTS | EXCLUDE_PRODUCTS | ATOL | RTOL | STEADY_STATE | SPARSE | T_END | T_START | N_STEPS | SAMPLE_TIMES | SIMULATE_ODE | SIMULATE_SSA | SIMULATE_NF | SIMULATE_PLA | READFILE | FILE | WRITENET | WRITESBML | WRITEMFILE | WRITEXML | SETCONCENTRATION | SETPARAMETER | SAVECONCENTRATIONS | RESETCONCENTRATIONS | GENERATENETWORK | SUBSTANCEUNITS | SAT | FLOAT | INT | STRING | SEMI | COLON | LSBRACKET | RSBRACKET | LBRACKET | RBRACKET | COMMA | DOT | LPAREN | RPAREN | UNI_REACTION_SIGN | BI_REACTION_SIGN | DOLLAR | TILDE | AT | GTE | GT | LTE | LT | ASSIGNS | EQUALS | BECOMES | DIV | TIMES | MINUS | PLUS | POWER | MOD | QMARK | EMARK | DBQUOTES | AMPERSAND | VERSION_NUMBER );";
        }
    }
 

}