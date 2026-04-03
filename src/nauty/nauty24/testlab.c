/* testlab.c version 1.1; B D McKay, Feb 23 2000 */

#define USAGE "testlab [-qsgu] [-c#] [infile [outfile]]"

#define HELPTEXT \
" Canonically label a file of graphs multiple times as a check.\n\
\n\
    -u  no output\n\
    -s  force output to sparse6 format\n\
    -g  force output to graph6 format\n\
        If neither -s or -g are given, the output format is\n\
        determined by the header or, if there is none, by the\n\
        format of the first input graph.\n\
\n\
    The output file will have a header if and only if the input file does.\n\
\n\
    -c#  After labelling the graph once, this many times randomly\n\
         relabel it then label the result again.  The canonical form\n\
         and number of orbits are compared each time.\n\
\n\
    -q  suppress auxiliary information\n"

/*************************************************************************/

#include "gtools.h"

static long orbtotal;
static double unorbtotal;
extern int gt_numorbits;

/**************************************************************************/

main(argc,argv)
int argc;
char *argv[];
{
	graph *g;
	int m,n,codetype;
	int argnum,j,outcode;
	char *arg,sw,*fmt;
	boolean badargs;
	long code1,code2;
	int numorbs;
	int i,nchecks;
	boolean sswitch,gswitch,qswitch,fswitch,Oswitch;
	boolean uswitch,cswitch;
	double t;
	char *infilename,*outfilename;
	FILE *infile,*outfile;
	long nin;
#if MAXN
	graph h[MAXN*MAXM];
	permutation perm[MAXN];
#else
	DYNALLSTAT(graph,h,h_sz);
	DYNALLSTAT(permutation,perm,perm_sz);
#endif

	HELP;

	nauty_check(WORDSIZE,1,1,NAUTYVERSIONID);

#ifdef INITSEED
	INITSEED;
	ran_init(seed);
#endif

	sswitch = gswitch = uswitch = qswitch = FALSE;
	fswitch = Oswitch = FALSE;
	cswitch = FALSE;
	infilename = outfilename = NULL;

	argnum = 0;
	badargs = FALSE;
	for (j = 1; !badargs && j < argc; ++j)
	{
	    arg = argv[j];
	    if (arg[0] == '-' && arg[1] != '\0')
	    {
		++arg;
		while (*arg != '\0')
		{
		    sw = *arg++;
		         SWBOOLEAN('s',sswitch)
		    else SWBOOLEAN('g',gswitch)
		    else SWBOOLEAN('u',uswitch)
		    else SWBOOLEAN('q',qswitch)
		    else SWBOOLEAN('O',Oswitch)
/*
		    else if (sw == 'f')
		    {
			fswitch = TRUE;
			fmt = arg;
			break;
		    }
*/
		    else SWINT('c',cswitch,nchecks,"testlab -c")
		    else badargs = TRUE;
		}
	    }
	    else
	    {
		++argnum;
		if      (argnum == 1) infilename = arg;
	        else if (argnum == 2) outfilename = arg;
		else                  badargs = TRUE;
	    }
	}

	if ((sswitch != 0) + (uswitch != 0) + (gswitch!= 0) > 1)
            gt_abort(">E testlab: -s, -g and -u are incompatible\n");

	if (badargs || argnum > 2)
	{
	    fprintf(stderr,">E Usage: %s\n",USAGE);
	    exit(1);
	}

	if (!qswitch)
	{
	    fprintf(stderr,">A testlab");
	    if (sswitch || gswitch || fswitch || uswitch || cswitch)
		fprintf(stderr," -");
	    if (sswitch) fprintf(stderr,"s");
	    if (gswitch) fprintf(stderr,"g");
	    if (uswitch) fprintf(stderr,"u");
	    if (cswitch) fprintf(stderr,"c%d",nchecks);
	    if (argnum > 0) fprintf(stderr," %s",infilename);
	    if (argnum > 1) fprintf(stderr," %s",outfilename);
	    fprintf(stderr,"\n");
	    fflush(stderr);
	}

	if (infilename && infilename[0] == '-') infilename = NULL;
	infile = opengraphfile(infilename,&codetype,FALSE,1);
	if (!infile) exit(1);
	if (!infilename) infilename = "stdin";

	if (!outfilename || outfilename[0] == '-')
	{
	    outfilename = "stdout";
	    outfile = stdout;
	}
	else if ((outfile = fopen(outfilename,"w")) == NULL)
	{
	    fprintf(stderr,"Can't open output file %s\n",outfilename);
	    gt_abort(NULL);
	}

	if (sswitch || !gswitch && (codetype&SPARSE6)) outcode = SPARSE6;
	else                                           outcode = GRAPH6;

	if (!fswitch) fmt = NULL;

	if ((codetype&HAS_HEADER) && !uswitch)
	{
	    if (outcode == SPARSE6) writeline(outfile,SPARSE6_HEADER);
	    else    		    writeline(outfile,GRAPH6_HEADER);
	}

	if (!cswitch) nchecks = 1;

	nin = 0;
	orbtotal = 0;
	unorbtotal = 0.0;
	t = CPUTIME;
	while (TRUE)
	{
	    if ((g = readg(infile,NULL,0,&m,&n)) == NULL) break;
	    ++nin;
#if !MAXN
	    DYNALLOC2(graph,h,h_sz,n,m,"testlab");
	    DYNALLOC1(permutation,perm,perm_sz,n,"testlab");
#endif
	    fcanonise(g,m,n,h,fmt);
 	    code1 = hash(h,m*(long)n,3);
 	    code2 = hash(h,m*(long)n,11);
	    numorbs = gt_numorbits;
            for (i = 0; i < nchecks; ++i)
	    {
		ranperm(perm,n);
		relabel(g,NULL,perm,h,m,n);
		fcanonise(g,m,n,h,fmt);
		if (hash(h,m*(long)n,3) != code1 ||
		       hash(h,m*(long)n,11) != code2 ||
		       gt_numorbits != numorbs)
	 	{
		    fprintf(stderr,">E Input %ld poorly labelled\n",nin);
		    exit(1);
		}
	    }
	    orbtotal += gt_numorbits;
	    unorbtotal += 1.0 / gt_numorbits;
	    if (!uswitch)
	    {
	        if (outcode == SPARSE6) writes6(outfile,h,m,n);
	        else                    writeg6(outfile,h,m,n);
	    }
	    FREES(g);
	}
	t = CPUTIME - t;

	if (Oswitch)
	    fprintf(stderr,">C orbit totals = %ld %15.8f\n",
			   orbtotal,unorbtotal);

        if (!qswitch)
            fprintf(stderr,
                    ">Z  %ld graphs labelled from %s to %s in %3.2f sec.\n",
                    nin,infilename,outfilename,t);

	exit(0);
}
