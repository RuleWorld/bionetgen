/*****************************************************************************
*                                                                            *
* This is the main file for dreadnaut() version 2.4, which is a test-bed     *
*   for nauty() version 2.4.                                                 *
*                                                                            *
* ACTUALLY IT ISN'T.  This is a modified edition of dreadnaut that performs  *
* tests of conformity.  Instead of reporting the results of computations to  *
* the human, it compares them to values stored in the input and reports any  *
* differences.                                                               *
* It is assumed that "long int" has at least 32 bits.                        *
* In the following, # is a hex value (without 0x) that is compared to a      *
* hash certificate of the specified value.  An error message is given if     *
* they don't match exactly.                                                  *
* The following no longer have their old meanings:                           *
*    a #   - compare the generators                                          *
*    b #   - compare the canonical labelling and the labelled graph          *
*    o #   - compare the orbits                                              *
*    & #   - compare the partition                                           *
*    S #   - compare the statistics                                          *
*    C     - run tests on bits, sets, etc.                                   *
*                                                                            *
*   Copyright (1984-2007) Brendan McKay.  All rights reserved.               *
*   Subject to the waivers and disclaimers in nauty.h.                       *
*                                                                            *
*   CHANGE HISTORY                                                           *
*        1-Sep-02 - Initial write starting at dreadnaut.c                    *
*       17-Nov-03 - Change INFINITY to NAUTY_INFINITY                        *
*       23-Nov-06 - Fix for version 2.4.                                     *
*                                                                            *
*****************************************************************************/

#include "naututil.h"    /* which includes nauty.h, which includes stdio.h */
#include "nautinv.h"    /* which includes nauty.h, which includes stdio.h */

#define PM(x) ((x) ? '+' : '-')
#define SS(n,sing,plur)  (n),((n)==1?(sing):(plur))
#define WORKSIZE 60

#define INFILE fileptr[curfile]
#define OUTFILE outfile

#if !MAXN
DYNALLSTAT(graph,g,g_sz);
DYNALLSTAT(graph,canong,canong_sz);
DYNALLSTAT(graph,savedg,savedg_sz);
DYNALLSTAT(setword,workspace,workspace_sz);
DYNALLSTAT(int,lab,lab_sz);
DYNALLSTAT(int,ptn,ptn_sz);
DYNALLSTAT(int,orbits,orbits_sz);
DYNALLSTAT(int,savedlab,savedlab_sz);
DYNALLSTAT(permutation,perm,perm_sz);
DYNALLSTAT(set,active,active_sz);
#else
static graph g[MAXM*1L*MAXN];
static graph canong[MAXM*1L*MAXN];
static graph savedg[MAXM*1L*MAXN];
static setword workspace[MAXM*2L*WORKSIZE];
static int lab[MAXN];
static int ptn[MAXN];
static int orbits[MAXN];
static int savedlab[MAXN];
static permutation perm[MAXN];
static set active[MAXM];
#endif

static DEFAULTOPTIONS_GRAPH(options);
static statsblk stats;
static int curfile;
static FILE *fileptr[MAXIFILES];
static FILE *outfile;
static char def_ext[] = DEFEXT;
static boolean firstpath;       /* used in usernode() */

#define U_NODE  1               /* masks for u values */
#define U_AUTOM 2
#define U_LEVEL 4
#define U_TCELL 8    /* At version 2.4, usertcellproc() is gone */
#define U_REF  16

#ifndef  NODEPROC
#define NODEPROC usernode
#else
extern void NODEPROC(graph*,int*,int*,int,int,int,int,int,int);
#endif

#ifndef  AUTOMPROC
#define AUTOMPROC userautom
#else
extern void AUTOMPROC(int,permutation*,int*,int,int,int);
#endif

#ifndef  LEVELPROC
#define LEVELPROC userlevel
#else
extern void LEVELPROC(int*,int*,int,int*,statsblk*,int,int,int,int,int,int);
#endif

#ifndef  REFPROC
#define REFPROC NULL
#else
extern void REFPROC(graph*,int*,int*,int,int*,permutation*,set*,int*,int,int);
#endif

#ifndef  INVARPROC
#define INVARPROC NULL
#define INVARPROCNAME "none"
#else
extern void INVARPROC(graph*,int*,int*,int,int,int,permutation*,
                      int,boolean,int,int);
#define INVARPROCNAME "user-defined"
#endif


static struct invarrec
{
    void (*entrypoint)(graph*,int*,int*,int,int,int,permutation*,
                      int,boolean,int,int);
    char *name;
} invarproc[]
    = {INVARPROC, INVARPROCNAME,
       NULL, "none",
       twopaths,    "twopaths",
       adjtriang,   "adjtriang",
       triples,     "triples",
       quadruples,  "quadruples",
       celltrips,   "celltrips",
       cellquads,   "cellquads",
       cellquins,   "cellquins",
       distances,   "distances",
       indsets,     "indsets",
       cliques,     "cliques",
       cellcliq,    "cellcliq",
       cellind,     "cellind",
       adjacencies, "adjacencies",
       cellfano,    "cellfano",
       cellfano2,   "cellfano2"};
#define NUMINVARS ((int)(sizeof(invarproc)/sizeof(struct invarrec)))

#ifdef  NLMAP
#define GETNW(c,f) do c = getc(f); while (c==' '||c=='\t')
#define GETNWC(c,f) do c = getc(f); while (c==' '||c==','||c=='\t')
#define GETNWL(c,f) do c = getc(f); while (c==' '||c=='\n'||c=='\t')
#else
#define GETNW(c,f) do c = getc(f); while (c==' '||c=='\t'||c=='\r')
#define GETNWC(c,f) do c = getc(f); while (c==' '||c==','||c=='\t'||c=='\r')
#define GETNWL(c,f) do c = getc(f); while (c==' '||c=='\n'||c=='\t'||c=='\r')
#endif

static void help(FILE*, int);
static void userautom(int,permutation*,int*,int,int,int);
static void usernode(graph*,int*,int*,int,int,int,int,int,int);
static void userlevel(int*,int*,int,int*,statsblk*,int,int,int,int,int,int);


#ifdef  EXTRADECLS
EXTRADECLS
#endif

static long afound;

/*****************************************************************************
*                                                                            *
*  gethex(f) reads a long hex integer from f, optionally preceded by '='     *
*  and white space.  -1 is returned if the attempt was unsuccessful.         *
*  Not much error checking.                                                  *
*                                                                            *
*****************************************************************************/

static long
gethex(FILE *f)
{
        int c;
	long i;

        GETNWL(c,f);
        if (c != '=') ungetc((char)c,f);

        if (fscanf(f,"%lx",&i) == 1) return i;
        else                         return -1;
}

/*****************************************************************************
*                                                                            *
*  permhash(s,n,seed,key) is a function whose value depends only on the      *
*  permutation s, a long seed, and an integer key.  It is intended to be     *
*  independent of the machine and whether permutation has 16 or 32 bits.     *
*  n is the length.                                                          *
*  28 bits of seed and 15 bits of key are significant.                       *
*  The result is in the low 28 bits.                                         *
*                                                                            *
*****************************************************************************/

static long
permhash(permutation *s, int n, long seed, int key)
{
        int i,j,lsh,rsh;
        long l,res,salt,lshmask;
        long si;

        lsh = key & 0xF;
        rsh = 28 - lsh;
        salt = (key >> 4) & 0x7FFL;
        res = seed & 0xFFFFFFFL;
        lshmask = (1L << lsh) - 1;

        j = 0;
        for (i = 0; i < n; ++i)
        {
            si = s[i];
            l = si & 0xFFFFL;
            res = (((res << lsh) ^ ((res >> rsh) & lshmask) ^ l) + salt) 
                                                                & 0xFFFFFFFL;
            l = si >> 16;
            res = (((res << lsh) ^ ((res >> rsh) & lshmask) ^ l) + salt) 
                                                                & 0xFFFFFFFL;
	}

	return res;
}

/*****************************************************************************
*                                                                            *
*  inthash(s,n,seed,key) is a function whose value depends only on the       *
*  int array p, a long seed, and an integer key.  It is intended to be       *
*  independent of the machine and whether int has 16 or 32 bits.             *
*  64 bits is also ok provided the values are not above 2^31-1.              *
*  n is the length.                                                          *
*  28 bits of seed and 15 bits of key are significant.                       *
*  The result is in the low 28 bits.                                         *
*                                                                            *
*****************************************************************************/

static long
inthash(int *s, int n, long seed, int key)
{
        int i,j,lsh,rsh;
        long l,res,salt,lshmask;
        long si;

        lsh = key & 0xF;
        rsh = 28 - lsh;
        salt = (key >> 4) & 0x7FFL;
        res = seed & 0xFFFFFFFL;
        lshmask = (1L << lsh) - 1;

        j = 0;
        for (i = 0; i < n; ++i)
        {
            si = s[i];
            l = si & 0xFFFFL;
            res = (((res << lsh) ^ ((res >> rsh) & lshmask) ^ l) + salt)
                                                                & 0xFFFFFFFL;
            l = si >> 16;
            res = (((res << lsh) ^ ((res >> rsh) & lshmask) ^ l) + salt)
                                                                & 0xFFFFFFFL;
        }

        return res;
}

/*****************************************************************************
* bit_tests()   Run some configuration tests                                 *
*****************************************************************************/

static int
bit_tests(void)
{
	int i,j,bad;
	setword w;

	printf("NAUTYVERSION=%s NAUTYVERSIONID=%d NAUTYREQUIRED=%d\n",
		NAUTYVERSION,NAUTYVERSIONID,NAUTYREQUIRED);
	printf("MAXN=%d MAXM=%d WORDSIZE=%d NAUTY_INFINITY=%d",
		MAXN,MAXM,WORDSIZE,NAUTY_INFINITY);
#ifdef SYS_UNIX
	printf(" SYS_UNIX");
#endif
#ifdef SYS_CRAY
        printf(" SYS_CRAY");
#endif

	printf("\n");


	bad = 0;

	if (SIZEOF_INT != sizeof(int))
	{
	    printf(" ***** NOTE: sizeof problem (int) *****\n");
	    ++bad;
	}
	if (SIZEOF_LONG != sizeof(long))
	{
	    printf(" ***** NOTE: sizeof problem (long) *****\n");
	    ++bad;
	}
#if SIZEOF_LONGLONG > 0
	if (SIZEOF_LONGLONG != sizeof(long long))
	{
	    printf(" ***** NOTE: sizeof problem (long long) *****\n");
	    ++bad;
	}
#endif

	if (8*sizeof(setword) < WORDSIZE)
	{
	    printf("\n ***** NOTE:  WORDSIZE mismatch *****\n");
	    ++bad;
	}
	if (8*sizeof(setword) > WORDSIZE)
	{
	    printf("\n WORDSIZE < 8*sizeof(setword)  [This is legal.]\n");
	}

	for (i = 0; i < WORDSIZE; ++i)
	{
	    w = ALLMASK(i);
	    if (POPCOUNT(w) != i)
	    {
		printf("\n ***** POPCOUNT(ALLMASK) error %d *****\n\n",i);
		++bad;
	    }
	}

	for (i = 0; i < WORDSIZE; ++i)
        {
            w = BITMASK(i);
            if (POPCOUNT(w) != WORDSIZE-i-1)
            {
                printf("\n ***** POPCOUNT(BITMASK) error %d *****\n\n",i);
                ++bad;
            }
        }

	for (i = 0; i < WORDSIZE; ++i)
	    if (POPCOUNT(ALLMASK(i)) != i)
	    {
		printf("\n ***** POPCOUNT(ALLMASK) error %d *****\n\n",i);
		++bad;
	    }

	for (i = 0; i < WORDSIZE; ++i)
            if (FIRSTBIT(BITT[i]) != i)
	    {
		printf("\n ***** FIRSTBIT(BITT) error %d *****\n\n",i);
		++bad;
	    }

	w = ALLBITS;
	for (i = 0; i < WORDSIZE; ++i)
	{
	    TAKEBIT(j,w);
	    if (j != i)
            {
                printf("\n ***** TAKEBIT error %d *****\n\n",i);
                ++bad;
            }
	}

	for (i = 0; i < WORDSIZE; ++i)
            if (POPCOUNT(BITT[i]) != 1)
	    {
                printf("\n ***** POPCOUNT(BITT) error %d *****\n\n",i);
		++bad;
	    }

	for (i = 0; i < WORDSIZE; ++i)
	{
	    w = 0;
	    for (j = 1; j <= WORDSIZE; ++j)
	    {
		w |= BITT[(j*97+i)%WORDSIZE];
		if (POPCOUNT(w) != j)
		{
		    printf("\n ***** POPCOUNT(w) error %d %d *****\n\n",i,j);
		    ++bad;
		}
	    }
	}

	if (bad) printf("\nXXXXXXX %d errors found XXXXXXX\n",bad);

	return bad;
}

/*****************************************************************************
*                                                                            *
*  This is a program which illustrates the use of nauty.                     *
*  Commands are read from stdin, and may be separated by white space,        *
*  commas or not separated.  Output is written to stdout.                    *
*  For a short description, see the nauty User's Guide.                      *
*                                                                            *
*****************************************************************************/

int
main(int argc, char *argv[])
{
        int m,n,newm,newn;
        boolean gvalid,ovalid,cvalid,pvalid,minus,prompt,doquot;
        int i,worksize,numcells,refcode,umask,qinvar;
        int oldorg;
        char *s1,*s2,*invarprocname;
        int c,d;
        long li;
        unsigned long uli;
        set *gp;
        double timebefore,timeafter;
        char filename[200];
        int sgn,sgorg,nperm;
	int bad,multiplicity;
	boolean options_writeautoms,options_writemarkers;
	long zseed;
	long need,found;

        curfile = 0;
        fileptr[curfile] = stdin;
        prompt = FALSE;
        outfile = stdout;
	options_writeautoms = options_writemarkers = TRUE;
        n = m = 1;
        worksize = 2*WORKSIZE;

#if !MAXN
	n = WORDSIZE;
        DYNALLOC2(graph,g,g_sz,n,m,"dreadnaut");
        DYNALLOC1(int,lab,lab_sz,n,"dreadnaut");
        DYNALLOC1(int,ptn,ptn_sz,n,"dreadnaut");
        DYNALLOC1(setword,workspace,workspace_sz,
                                            worksize,"dreadnaut");
        DYNALLOC1(int,orbits,orbits_sz,n,"dreadnaut");
        DYNALLOC1(permutation,perm,perm_sz,n,"dreadnaut");
        DYNALLOC1(set,active,active_sz,m,"dreadnaut");
	n = 1;
#endif

	ran_init(37);

	bad = 0;

        umask = 0;
        pvalid = FALSE;
        gvalid = FALSE;
        ovalid = FALSE;
        cvalid = FALSE;
        minus = FALSE;
        labelorg = oldorg = 0;
        multiplicity = 1;
	options.userautomproc = userautom;

#ifdef  INITIALIZE
        INITIALIZE;
#endif

        invarprocname = "none";
        if (prompt)
        {
            fprintf(PROMPTFILE,"Dreadnaut version %s.\n",NAUTYVERSION);
            fprintf(PROMPTFILE,"> ");
        }

        nauty_check(WORDSIZE,1,1,NAUTYVERSIONID);
        nautinv_check(WORDSIZE,1,1,NAUTYVERSIONID);
        nautil_check(WORDSIZE,1,1,NAUTYVERSIONID);
        naututil_check(WORDSIZE,1,1,NAUTYVERSIONID);
        naugraph_check(WORDSIZE,1,1,NAUTYVERSIONID);

        while (curfile >= 0)
            if ((c = getc(INFILE)) == EOF || c == '\004')
            {
                fclose(INFILE);
                --curfile;
                if (curfile >= 0)
                    prompt = FALSE;
            }
            else switch (c)
            {
            case '\n':  /* possibly issue prompt */
                if (prompt)
                    fprintf(PROMPTFILE,"> ");
                minus = FALSE;
                break;

            case ' ':   /* do nothing */
            case '\t':
#ifndef  NLMAP
            case '\r':
#endif
            case '\f':
                break;

            case '-':   /* remember this for next time */
                minus = TRUE;
                break;

            case '+':   /* forget - */
            case ',':
            case ';':
                minus = FALSE;
                break;

            case '<':   /* new input file */
                minus = FALSE;
                if (curfile == MAXIFILES - 1)
                    fprintf(ERRFILE,"exceeded maximum input nesting of %d\n\n",
                            MAXIFILES);
                if (!readstring(INFILE,filename,200))
                {
                    fprintf(ERRFILE,
                            "missing file name on '>' command : ignored\n\n");
                    break;
                }
                if ((fileptr[curfile+1] = fopen(filename,"r")) == NULL)
                {
                    for (s1 = filename; *s1 != '\0'; ++s1) {}
                    for (s2 = def_ext; (*s1 = *s2) != '\0'; ++s1, ++s2) {}
                    fileptr[curfile+1] = fopen(filename,"r");
                }
                if (fileptr[curfile+1] != NULL)
                {
                    ++curfile;
                    prompt = FALSE;
                    if (prompt)
                        fprintf(PROMPTFILE,"> ");
                }
                else
                    fprintf(ERRFILE,"can't open input file\n\n");
                break;

            case '>':   /* new output file */
                if ((d = getc(INFILE)) != '>')
                    ungetc((char)d,INFILE);
                if (minus)
                {
                    minus = FALSE;
                    if (outfile != stdout)
                    {
                        fclose(outfile);
                        outfile = stdout;
                    }
                }
                else
                {
                    if (!readstring(INFILE,filename,200))
                    {
                        fprintf(ERRFILE,
                            "improper file name, reverting to stdout\n\n");
                        outfile = stdout;
                        break;
                    }
                    OPENOUT(outfile,filename,d=='>');
                    if (outfile == NULL)
                    {
                        fprintf(ERRFILE,
                            "can't open output file, reverting to stdout\n\n");
                        outfile = stdout;
                    }
                }
                break;

            case '!':   /* ignore rest of line */
                do
                    c = getc(INFILE);
                while (c != '\n' && c != EOF);
                if (c == '\n')
                    ungetc('\n',INFILE);
                break;

            case 'n':   /* read n value */
                minus = FALSE;
                i = getint(INFILE);
                if (i <= 0 || (MAXN && i > MAXN)
			   || (!MAXN && i > NAUTY_INFINITY-2))
                    fprintf(ERRFILE,
                         " n can't be less than 1 or more than %d\n\n",
                           MAXN ? MAXN : NAUTY_INFINITY-2);
                else
                {
                    gvalid = FALSE;
                    ovalid = FALSE;
                    cvalid = FALSE;
                    pvalid = FALSE;
                    n = i;
                    m = (n + WORDSIZE - 1) / WORDSIZE;
#if !MAXN
		    worksize = 2 * m * WORKSIZE;
		    DYNALLOC2(graph,g,g_sz,n,m,"dreadnaut");
		    DYNALLOC1(int,lab,lab_sz,n,"dreadnaut");
		    DYNALLOC1(int,ptn,ptn_sz,n,"dreadnaut");
                    DYNALLOC1(setword,workspace,workspace_sz,
                                                        worksize,"dreadnaut");
		    DYNALLOC1(int,orbits,orbits_sz,n,"dreadnaut");
		    DYNALLOC1(permutation,perm,perm_sz,n,"dreadnaut");
		    DYNALLOC1(set,active,active_sz,m,"dreadnaut");
#endif
                }
                break;

            case 'g':   /* read graph */
                minus = FALSE;
                readgraph(INFILE,g,options.digraph,prompt,FALSE,
                          options.linelength,m,n);
                gvalid = TRUE;
                cvalid = FALSE;
                ovalid = FALSE;
                break;

            case 'e':   /* edit graph */
                minus = FALSE;
                readgraph(INFILE,g,options.digraph,prompt,gvalid,
                          options.linelength,m,n);
                gvalid = TRUE;
                cvalid = FALSE;
                ovalid = FALSE;
                break;

            case 'r':   /* relabel graph and current partition */
                minus = FALSE;
                if (gvalid)
                {
#if !MAXN
		    DYNALLOC2(graph,canong,canong_sz,n,m,"dreadnaut");
#endif
                    readvperm(INFILE,perm,prompt,n,&nperm);
                    relabel(g,(pvalid ? lab : NULL),perm,canong,m,n);
                    cvalid = FALSE;
                    ovalid = FALSE;
                }
                else
                    fprintf(ERRFILE,"g is not defined\n\n");
                break;

            case 'R':   /* form subgraph */
                if (gvalid)
                {
#if !MAXN
                    DYNALLOC2(graph,canong,canong_sz,n,m,"dreadnaut");
#endif
                    readvperm(INFILE,perm,prompt,n,&nperm);
		    if (minus && nperm == n || !minus && nperm == 0)
			fprintf(ERRFILE,"can't form null graph\n\n");
		    else if (minus)
		    {
                        sublabel(g,perm+nperm,n-nperm,canong,m,n);
			n = n - nperm;
		    }
		    else
                    {
                        sublabel(g,perm,nperm,canong,m,n);
                        n = nperm;
                    }
                    cvalid = FALSE;
		    pvalid = FALSE;
                    ovalid = FALSE;
		    m = (n + WORDSIZE - 1) / WORDSIZE;
                }
                else
                    fprintf(ERRFILE,"g is not defined\n\n");
                minus = FALSE;
                break;

            case '_':   /* complement graph or converse digraph */
                minus = FALSE;
                if ((d = getc(INFILE)) != '_') ungetc((char)d,INFILE);

                if (gvalid)
                {
                    if (d == '_') converse(g,m,n);
                    else          complement(g,m,n);
                    cvalid = FALSE;
                    ovalid = FALSE;
                }
                else
                    fprintf(ERRFILE,"g is not defined\n\n");
                break;

            case '@':   /* copy canong into savedg */
                minus = FALSE;
                if (cvalid)
                {
#if !MAXN
		    DYNALLOC2(graph,savedg,savedg_sz,n,m,"dreadnaut");
		    DYNALLOC1(int,savedlab,savedlab_sz,n,"dreadnaut");
#endif
                    sgn = n;
                    for (li = (long)n * (long)m; --li >= 0;)
                        savedg[li] = canong[li];
                    for (i = n; --i >= 0;)
                        savedlab[i] = lab[i];
                    sgorg = labelorg;
                }
                else
                    fprintf(ERRFILE,"h is not defined\n\n");
                break;

            case '#':   /* compare canong to savedg */
                if ((d = getc(INFILE)) != '#') ungetc((char)d,INFILE);

                if (cvalid)
                {
                    if (sgn > 0)
                    {
                        if (sgn != n)
                            fprintf(OUTFILE,
                                  "h and h' have different sizes.\n");
                        else
                        {
                            for (li = (long)n * (long)m; --li >= 0;)
                                if (savedg[li] != canong[li]) break;
                            if (li >= 0)
                                fprintf(OUTFILE,"h and h' are different.\n");
                            else
                            {
                                fprintf(OUTFILE,
                                   "h and h' are identical.\n");
                                if (d == '#')
                                    putmapping(OUTFILE,savedlab,sgorg,
                                           lab,labelorg,options.linelength,n);
                            }
                        }
                    }
                    else
                        fprintf(ERRFILE,"h' is not defined\n\n");
                }
                else
                    fprintf(ERRFILE,"h is not defined\n\n");
                break;

            case 'j':   /* relabel graph randomly */
                minus = FALSE;
                if (gvalid)
                {
#if !MAXN
		    DYNALLOC2(graph,canong,canong_sz,n,m,"dreadnaut");
#endif
                    ranperm(perm,n);
                    relabel(g,(pvalid ? lab : NULL),perm,canong,m,n);
                    cvalid = FALSE;
                    ovalid = FALSE;
                }
                else
                    fprintf(ERRFILE,"g is not defined\n\n");
                break;

            case 'v':   /* write vertex degrees */
                minus = FALSE;
                if (gvalid)
                    putdegs(OUTFILE,g,options.linelength,m,n);
                else
                    fprintf(ERRFILE,"g is not defined\n\n");
                break;

            case '%':   /* do Mathon doubling operation */
                minus = FALSE;
                if (gvalid)
                {
#if !MAXN
		    if (2L * ((long)n + 1L) > NAUTY_INFINITY-2)
                    {
                        fprintf(ERRFILE,
			     "n can't be more than %d\n\n",NAUTY_INFINITY-2);
                        break;
                    }
#else
                    if (2L * ((long)n + 1L) > MAXN)
                    {
                        fprintf(ERRFILE,"n can't be more than %d\n\n",MAXN);
                        break;
                    }
#endif
                    newn = 2 * (n + 1);
                    newm = (newn + WORDSIZE - 1) / WORDSIZE;
#if !MAXN
		    DYNALLOC2(graph,canong,canong_sz,n,m,"dreadnaut");
#endif

                    for (li = (long)n * (long)m; --li >= 0;)
                        canong[li] = g[li];

#if !MAXN
                    DYNALLOC2(graph,g,g_sz,newn,newm,"dreadnaut");
                    DYNALLOC1(int,lab,lab_sz,newn,"dreadnaut");
                    DYNALLOC1(int,ptn,ptn_sz,newn,"dreadnaut");
		    worksize = 2*WORKSIZE*newm;
                    DYNALLOC1(setword,workspace,workspace_sz,
                                                        worksize,"dreadnaut");
                    DYNALLOC1(int,orbits,orbits_sz,newn,"dreadnaut");
                    DYNALLOC1(permutation,perm,perm_sz,newn,"dreadnaut");
                    DYNALLOC1(set,active,active_sz,newm,"dreadnaut");
#endif
                    mathon(canong,m,n,g,newm,newn);
                    m = newm;
                    n = newn;
                    cvalid = FALSE;
                    ovalid = FALSE;
                    pvalid = FALSE;
                }
                else
                    fprintf(ERRFILE,"g is not defined\n\n");
                break;

            case 's':   /* generate random graph */
                minus = FALSE;
                i = getint(INFILE);
                if (i <= 0)
                    i = 2;
                rangraph(g,options.digraph,i,m,n);
                gvalid = TRUE;
                cvalid = FALSE;
                ovalid = FALSE;
                break;

            case 'q':   /* quit */
                if (bad == 0) printf("OK");
		printf("\n");
                exit(bad);
                break;

            case '"':   /* copy comment to output */
                minus = FALSE;
                copycomment(INFILE,OUTFILE,'"');
                break;

            case 'I':   /* do refinement and invariants procedure */
                if (!pvalid)
                    unitptn(lab,ptn,&numcells,n);
                cellstarts(ptn,0,active,m,n);
                doref(g,lab,ptn,0,&numcells,&qinvar,perm,active,&refcode,
                        options.userrefproc ? options.userrefproc : 
			(m == 1 ? refine1 : refine),
                        options.invarproc,0,0,
                        options.invararg,options.digraph,m,n);
                fprintf(OUTFILE," %d cell%s; code = %x",
                        SS(numcells,"","s"),refcode);
                if (options.invarproc != NULL)
                    fprintf(OUTFILE," (%s %s)",invarprocname,
                        (qinvar == 2 ? "worked" : "failed"));
                fprintf(OUTFILE,"\n");
                if (numcells > 1)
                    pvalid = TRUE;
                break;

            case 'i':   /* do refinement */
                if (!pvalid)
                    unitptn(lab,ptn,&numcells,n);
                cellstarts(ptn,0,active,m,n);
		if (options.userrefproc)
		    (*options.userrefproc)
                         (g,lab,ptn,0,&numcells,perm,active,&refcode,m,n);
                else if (m == 1)
                    refine1(g,lab,ptn,0,&numcells,perm,active,&refcode,m,n);
                else
                    refine(g,lab,ptn,0,&numcells,perm,active,&refcode,m,n);
                fprintf(OUTFILE," %d cell%s; code = %x\n",
                        SS(numcells,"","s"),refcode);
                if (numcells > 1)
                    pvalid = TRUE;
                break;

            case 'x':   /* execute nauty */
                minus = FALSE;
                ovalid = FALSE;
                cvalid = FALSE;
                if (!gvalid)
                {
                    fprintf(ERRFILE,"g is not defined\n\n");
                    break;
                }
                if (pvalid)
                {
                    /* fprintf(OUTFILE,"[fixing partition]\n"); */
                    options.defaultptn = FALSE;
                }
                else
                    options.defaultptn = TRUE;
                options.outfile = outfile;

                if (options.getcanon)
                {
#if !MAXN
		    DYNALLOC2(graph,canong,canong_sz,n,m,"dreadnaut");
#endif
                }

                firstpath = TRUE;
		options.writeautoms = FALSE;
		options.writemarkers = FALSE;
		for (i = 0; i < multiplicity; ++i)
		{
		    afound = 1;
                    nauty(g,lab,ptn,NULL,orbits,&options,&stats,workspace,
                         worksize,m,n,canong);
		    options.writeautoms = FALSE;
                    options.writemarkers = FALSE;
		}
                if (stats.errstatus != 0)
                    fprintf(ERRFILE,
                      "nauty returned error status %d [this can't happen]\n\n",
                       stats.errstatus);
                else
                {
                    if (options.getcanon) cvalid = TRUE;
                    ovalid = TRUE;
                 /* fprintf(OUTFILE,
			       "%d orbit%s",SS(stats.numorbits,"","s")); 
                    if (stats.grpsize2 == 0)
                        fprintf(OUTFILE,"; grpsize=%.0f",stats.grpsize1+0.1);
                    else
                    {
                        while (stats.grpsize1 >= 10.0)
                        {
                            stats.grpsize1 /= 10.0;
                            ++stats.grpsize2;
                        }
                        fprintf(OUTFILE,"; grpsize=%12.10fe%d",
                                   stats.grpsize1,stats.grpsize2);
                    }
                    fprintf(OUTFILE,"; %d gen%s",
                            SS(stats.numgenerators,"","s"));
                    fprintf(OUTFILE,"; %lu node%s",SS(stats.numnodes,"","s"));
                    if (stats.numbadleaves)
                        fprintf(OUTFILE," (%lu bad lea%s)",
                                SS(stats.numbadleaves,"f","ves"));
                    fprintf(OUTFILE,"; maxlev=%d\n", stats.maxlevel);
                    fprintf(OUTFILE,"tctotal=%lu",stats.tctotal);
                    if (options.getcanon)
                        fprintf(OUTFILE,"; canupdates=%lu",stats.canupdates);
                    fprintf(OUTFILE,"\n");
                    if (options.invarproc != NULL &&
                                           options.maxinvarlevel != 0)
                    {
                        fprintf(OUTFILE,"invarproc \"%s\" succeeded %lu/%lu",
                            invarprocname,stats.invsuccesses,stats.invapplics);
                        if (stats.invarsuclevel > 0)
                            fprintf(OUTFILE," beginning at level %d.\n",
                                    stats.invarsuclevel);
                        else
                            fprintf(OUTFILE,".\n");
                    }
		*/
                }
                break;

            case 'f':   /* read initial partition */
                if (minus)
                {
                    pvalid = FALSE;
                    minus = FALSE;
                }
                else
                {
                    readptn(INFILE,lab,ptn,&numcells,prompt,n);
                    pvalid = TRUE;
                }
                break;

            case 't':   /* type graph */
                minus = FALSE;
                if (!gvalid)
                    fprintf(ERRFILE,"g is not defined\n\n");
                else
                    putgraph(OUTFILE,g,options.linelength,m,n);
                break;

            case 'T':   /* type graph preceded by n, $ and g commands */
                minus = FALSE;
                if (!gvalid)
                    fprintf(ERRFILE,"g is not defined\n\n");
                else
                {
                    fprintf(OUTFILE,"n=%d $=%d g\n",n,labelorg);
                    putgraph(OUTFILE,g,options.linelength,m,n);
                    fprintf(OUTFILE,"$$\n");
                }
                break;

            case 'u':   /* call user procs */
                if (minus)
                {
                    umask = 0;
                    minus = FALSE;
                }
                else
                {
                    umask = getint(INFILE);
                    if (umask < 0)
                        umask = ~0;
                }
                if (umask & U_NODE)
                    options.usernodeproc = NODEPROC;
                else
                    options.usernodeproc = NULL;
                if (umask & U_AUTOM)
                    options.userautomproc = AUTOMPROC;
                else
                    options.userautomproc = userautom;
                if (umask & U_LEVEL)
                    options.userlevelproc = LEVELPROC;
                else
                    options.userlevelproc = NULL;
                if (umask & U_TCELL)
		    fprintf(ERRFILE,"usertcellproc() gone at version 2.4\n\n");
                if (umask & U_REF)
                    options.userrefproc = REFPROC;
                else
                    options.userrefproc = NULL;
                break;

            case 'o':   /* type orbits */
                minus = FALSE;
		need = gethex(INFILE);
                if (ovalid)
                {
		    found = inthash(orbits,n,761L,3);
		    if (found != need)
		    {
			printf("\nERROR: need=%lx found=%lx\n",need,found);
			++bad;
		    }
		    else
			printf("+"); fflush(stdout);
		}
                else
                    fprintf(ERRFILE,"orbits are not defined\n\n");
                break;

            case 'b':   /* type canonlab and canong */
                minus = FALSE;
                need = gethex(INFILE);
                if (cvalid)
                {
		    zseed = n;
                    for (i = 0, gp = canong; i < n; ++i, gp += m)
                        zseed = sethash(gp,n,zseed,3109);
		    found = inthash(lab,n,165,2) ^ zseed;
                    if (found != need)
		    {
                        printf("\nERROR: need=%lx found=%lx\n",need,found);
			++bad;
		    }
                    else
                        printf("+"); fflush(stdout);
                }
                else
                    fprintf(ERRFILE,"automorphisms are not defined\n\n");
                break;

            case 'z':   /* type hashcode for canong */
                minus = FALSE;
                if (cvalid)
		{
		    zseed = n;
		    for (i = 0, gp = canong; i < n; ++i, gp += m)   
			zseed = sethash(gp,n,zseed,321);
                    fprintf(OUTFILE,"[%7lx",zseed);
		    
                    for (i = 0, gp = canong; i < n; ++i, gp += m) 
                        zseed = sethash(gp,n,zseed,3109);
                    fprintf(OUTFILE," %7lx",zseed);
                    
                    for (i = 0, gp = canong; i < n; ++i, gp += m) 
                        zseed = sethash(gp,n,zseed,4317); 
                    fprintf(OUTFILE," %7lx]\n",zseed);
		}
                else
                    fprintf(ERRFILE,"h is not defined\n\n");
                break;

            case 'c':   /* set getcanon option */
                options.getcanon = !minus;
                minus = FALSE;
                break;

            case 'w':   /* read size of workspace */
                minus = FALSE;
                worksize = getint(INFILE);
#if !MAXN
		DYNALLOC1(setword,workspace,workspace_sz,worksize,"dreadnaut");
#else
                if (worksize > 2*MAXM*WORKSIZE)
                {
                    fprintf(ERRFILE,
                       "too big - setting worksize = %d\n\n", 2*MAXM*WORKSIZE);
                    worksize = 2*MAXM*WORKSIZE;
                }
#endif
                break;

            case 'l':   /* read linelength for output */
                options.linelength = getint(INFILE);
                minus = FALSE;
                break;

            case 'y':   /* set tc_level field of options */
                options.tc_level = getint(INFILE);
                minus = FALSE;
                break;

            case 'M':   /* set multiplicity */
                multiplicity = getint(INFILE);
		if (multiplicity <= 0) multiplicity = 1;
                minus = FALSE;
                break;

            case 'k':   /* set invarlev fields of options */
                options.mininvarlevel = getint(INFILE);
                options.maxinvarlevel = getint(INFILE);
                minus = FALSE;
                break;

            case 'K':   /* set invararg field of options */
                options.invararg = getint(INFILE);
                minus = FALSE;
                break;

            case '*':   /* set invarproc field of options */
                minus = FALSE;
                d = getint(INFILE);
                if (d >= -1 && d <= NUMINVARS-2)
                {
                    options.invarproc = invarproc[d+1].entrypoint;
                    invarprocname = invarproc[d+1].name;
		    if (options.invarproc != NULL)
		    {
			options.mininvarlevel = 0;
			options.maxinvarlevel = 1;
			if (options.invarproc == indsets ||
			    options.invarproc == cliques ||
			    options.invarproc == cellind ||
			    options.invarproc == cellcliq)
				options.invararg = 3;
			else
			    options.invararg = 0;
		    }
                }
                else
                    fprintf(ERRFILE,"no such vertex-invariant\n\n");
                break;

            case 'a':   /* set writeautoms option */
                minus = FALSE;
                need = gethex(INFILE);
                if (ovalid)
                {
                    if (afound != need)
		    {
                        printf("\nERROR: need=%lx found=%lx\n",need,afound);
			++bad;
		    }
                    else
                        printf("+"); fflush(stdout);
                }
                else
                    fprintf(ERRFILE,"automorphisms are not defined\n\n");
                break;

            case 'C':   /* check bit and set operations */
                minus = FALSE;
		bit_tests();
                break;

            case 'm':   /* set writemarkers option */
                options_writemarkers = !minus;
                minus = FALSE;
                break;

            case 'p':   /* set cartesian option */
                options.cartesian = !minus;
                minus = FALSE;
                break;

            case 'd':   /* set digraph option */
                if (options.digraph && minus)
                    gvalid = FALSE;
                options.digraph = !minus;
                minus = FALSE;
                break;

            case '$':   /* set label origin */
                if ((d = getc(INFILE)) == '$')
                    labelorg = oldorg;
                else
                {
                    ungetc((char)d,INFILE);
                    oldorg = labelorg;
                    i = getint(INFILE);
                    if (i < 0)
                        fprintf(ERRFILE,"labelorg must be >= 0\n\n");
                    else
                        labelorg = i;
                }
                break;

            case '?':   /* type options, etc. */
                minus = FALSE;
                fprintf(OUTFILE,"m=%d n=%d labelorg=%d",m,n,labelorg);
                if (!gvalid)
                    fprintf(OUTFILE," g=undef");
                else
                {
                    uli = 0;
                    for (i = 0, gp = g; i < n; ++i, gp += m)
                        uli += setsize(gp,m);
                    if (options.digraph)
                        fprintf(OUTFILE," arcs=%lu",uli);
                    else
                        fprintf(OUTFILE," edges=%lu",uli/2);
                }
                fprintf(OUTFILE," options=(%cc%ca%cm%cp%cd",
                            PM(options.getcanon),PM(options_writeautoms),
                            PM(options_writemarkers),PM(options.cartesian),
                            PM(options.digraph));
                if (umask & 31)
                    fprintf(OUTFILE," u=%d",umask&31);
                if (options.tc_level > 0)
                    fprintf(OUTFILE," y=%d",options.tc_level);
                if (options.mininvarlevel != 0 || options.maxinvarlevel != 0)
                    fprintf(OUTFILE," k=(%d,%d)",
                                  options.mininvarlevel,options.maxinvarlevel);
                if (options.invararg > 0)
                    fprintf(OUTFILE," K=%d",options.invararg);
		if (multiplicity > 1) fprintf(OUTFILE," M=%d",multiplicity);
                fprintf(OUTFILE,")\n");
                fprintf(OUTFILE,"linelen=%d worksize=%d input_depth=%d",
                                options.linelength,worksize,curfile);
                if (options.invarproc != NULL)
                    fprintf(OUTFILE," invarproc=%s",invarprocname);
                if (pvalid)
                    fprintf(OUTFILE,"; %d cell%s",SS(numcells,"","s"));
                else
                    fprintf(OUTFILE,"; 1 cell");
                fprintf(OUTFILE,"\n");
                if (OUTFILE != PROMPTFILE)
                    fprintf(PROMPTFILE,"m=%d n=%d depth=%d labelorg=%d\n",
                            m,n,curfile,labelorg);
                break;

            case '&':   /* list the partition and possibly the quotient */
		need = gethex(INFILE);
                if ((d = getc(INFILE)) == '&')
                    doquot = TRUE;
                else
                {
                    ungetc((char)d,INFILE);
                    doquot = FALSE;
                }
                minus = FALSE;
                if (pvalid)
		{
		    for (i = 0; i < n; ++i) if (ptn[i] > n) ptn[i] = n;
		    found = inthash(lab,n,4123,12) ^ inthash(ptn,n,4123,12);
		}
                else
		    found = 1001;

                if (found != need)
		{
                    printf("\nERROR: need=%lx found=%lx\n",need,found);
		    ++bad;
		}
                else
                    printf("+"); fflush(stdout);

                if (doquot)
                {
		 /*
                    if (!pvalid)
                        unitptn(lab,ptn,&numcells,n);
                    putquotient(OUTFILE,g,lab,ptn,0,options.linelength,m,n);
		 */
                }
                break;

            case 'h':   /* type help information */
	    case 'H':
                minus = FALSE;
                printf("a, o, & are reassigned and output is suppressed\n");
		printf("Use dreadnaut for other functions\n");
                break;

            default:    /* illegal command */
                fprintf(ERRFILE,"'%c' is illegal - type 'h' for help\n\n",c);
                flushline(INFILE);
                if (prompt)
                    fprintf(PROMPTFILE,"> ");
                break;

            }  /* end of switch */

	return 0;
}

/*****************************************************************************
*                                                                            *
*  help(f,i) writes help information to file f (i = 0,1).                    *
*                                                                            *
*****************************************************************************/

static void
help(FILE *f, int i)
{
#define H(ss) fprintf(f," %s\n",ss);

if (i == 0)
{
H("+- a : write automs        v : write degrees    *=# : select invariant:")
H("   b : write canong      w=# : set worksize")
H("+- c : canonise            x : run nauty         -1 = user-defined")
H("+- d : digraph or loops  y=# : set tc_level       0 = none")
H("   e : edit graph          z : write hashcode     1 = twopaths")
H("-f, f=#, f=[...] : set colours                    2 = adjtriang(K=0,1)")
H("   g : read graph        $=# : set origin         3 = triples")
H(" h,H : help               $$ : restore origin     4 = quadruples")
H("   i : refine              ? : type options       5 = celltrips")
H("   I : refine using invar  _ : compl  __ : conv   6 = cellquads")
H("   j : relabel randomly    % : Mathon doubling    7 = cellquins")
H("k=# # : set invar levels   & : type colouring     8 = distances(K)")
H(" K=# : set invar param    && : + quotient matrix  9 = indsets(K)")
H(" l=# : set line length   >ff : write to file     10 = cliques(K)")
H("+- m : write markers    >>ff : append to file    11 = cellcliq(K)")
H(" n=# : set order          -> : revert to stdout  12 = cellind(K)")
H("   o : write orbits      <ff : read from file    13 = adjacencies")
H("+- p : set autom format    @ : save canong       14 = cellfano")
H("   q : quit                # : canong = savedg?  15 = cellfano2")
H(" r,R : relabel/subgraph   ## : + write mapping")
H(" s=# : random g (p=1/#)  \"...\" : copy comment")
H(" t,T : type graph          ! : ignore line      Type H for more..")
}

if (i == 1)
{
H("Commands for g and e : ")
H("   There is always a \"current vertex\" v, initially first vertex.")
H("   # : add edge v=#       ; : increment v (exit if over limit)")
H("  -# : delete edge v=#   #: : set v := #")
H("   ? : list nbhs of v     . : exit")
H("Syntax for f :  f=[2 3|4:9|10]  (rest in extra cell at right)")
H("               -f same as f=[], f=# same as f=[#]")
H("Syntax for r :  r 2:4 1 5;    (rest appended in order)")
H("Syntax for R :  R 2:4 1 5;   or  -R 0 3 6:10;")
H("Arguments for u : 1=node,2=autom,4=level,16=ref (add them)")
H("Accurate times for easy graphs: M=# selects number of times to run.")
}

}

/*****************************************************************************
*                                                                            *
*  usernode(g,lab,ptn,level,numcells,tc,code,m,n) is a simple version of the *
*  procedure named by options.usernodeproc.                                  *
*                                                                            *
*****************************************************************************/

static void
usernode(graph *g, int *lab, int *ptn, int level, int numcells,
         int tc, int code, int m, int n)
{
/*
        register int i;

        for (i = 0; i < level; ++i)
            PUTC('.',OUTFILE);
        if (numcells == n)
            fprintf(OUTFILE,"(n/%d)\n",code);
        else if (tc < 0)
            fprintf(OUTFILE,"(%d/%d)\n",numcells,code);
        else
            fprintf(OUTFILE,"(%d/%d/%d)\n",numcells,code,tc);
        if (firstpath)
            putptn(OUTFILE,lab,ptn,level,options.linelength,n);
        if (numcells == n)
            firstpath = FALSE;
*/
}

/*****************************************************************************
*                                                                            *
*  userautom(count,perm,orbits,numorbits,stabvertex,n) is a simple           *
*  version of the procedure named by options.userautomproc.                  *
*                                                                            *
*****************************************************************************/

static void
userautom(int count, permutation *perm, int *orbits,
          int numorbits, int stabvertex, int n)
{
	afound ^= permhash(perm,n,107651L,count);
}

/*****************************************************************************
*                                                                            *
*  userlevel(lab,ptn,level,orbits,stats,tv,index,tcellsize,numcells,cc,n)    *
*  is a simple version of the procedure named by options.userlevelproc.      *
*                                                                            *
*****************************************************************************/

static void
userlevel(int *lab, int *ptn, int level, int *orbits, statsblk *stats,
          int tv, int index, int tcellsize, int numcells, int cc, int n)
{
/*
      fprintf(OUTFILE,
            "**userlevelproc:  level=%d tv=%d index=%d tcellsize=%d cc=%d\n",
            level,tv+labelorg,index,tcellsize,cc);
      fprintf(OUTFILE,"    nodes=%lu cells=%d orbits=%d generators=%d\n",
            stats->numnodes,numcells,stats->numorbits,stats->numgenerators);
*/
}
