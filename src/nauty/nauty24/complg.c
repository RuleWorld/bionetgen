/* complg.c  version 1.0; B D McKay, Jan 1997. */

#define USAGE "complg [-lrq] [infile [outfile]]"

#define HELPTEXT \
" Take the complements of a file of graphs.\n\
\n\
    The output file has a header if and only if the input file does.\n\
\n\
    -r  Only complement if the complement has fewer edges.\n\
    -l  Canonically label outputs.\n\
    -q  Suppress auxiliary information.\n"

/*************************************************************************/

#include "gtools.h" 

/**************************************************************************/

void
compl(graph *g, int m, int n, graph *h)
/* h := complement of g */
{
	int i,j;
	setword *gi,*hi;
#if MAXN
	set all[MAXM];
#else
	DYNALLSTAT(set,all,all_sz);
	DYNALLOC1(set,all,all_sz,m,"complg");
#endif

	EMPTYSET(all,m);
	for (i = 0; i < n; ++i) ADDELEMENT(all,i);

	gi = (setword*) g;
	hi = (setword*) h;

	for (i = 0; i < n; ++i)
	{
	    for (j = 0; j < m; ++j) hi[j] = gi[j] ^ all[j];
	    DELELEMENT(hi,i);
	    gi += m;
	    hi += m;
	}
}

/**************************************************************************/

int
main(int argc, char *argv[])
{
        char *infilename,*outfilename;
        FILE *infile,*outfile;
        boolean dolabel,badargs,restricted,quiet;
	int j,m,n,argnum;
	int codetype,outcode;
	graph *g;
	long nin,ned,ii;
        char *arg,sw;
	static graph *gq;
	double t;
#if MAXN
	graph gc[MAXN*MAXM],h[MAXN*MAXM];
#else
	DYNALLSTAT(graph,gc,gc_sz);
	DYNALLSTAT(graph,h,h_sz);
#endif

	HELP;

        infilename = outfilename = NULL;
        dolabel = badargs = FALSE;
	restricted = quiet = FALSE;

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
		         SWBOOLEAN('r',restricted)
		    else SWBOOLEAN('l',dolabel)
		    else SWBOOLEAN('q',quiet)
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

	if (badargs)
	{
	    fprintf(stderr,">E Usage: %s\n",USAGE);
	    GETHELP;
	    exit(1);
	}

	if (!quiet)
	{
	    fprintf(stderr,">A complg");
	    if (restricted || dolabel)
		fprintf(stderr," -");
	    if (restricted) fprintf(stderr,"r");
	    if (dolabel) fprintf(stderr,"l");
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

	if (codetype&SPARSE6) outcode = SPARSE6;
	else                  outcode = GRAPH6;

	if (codetype&HAS_HEADER)
	{
	    if (outcode == SPARSE6) writeline(outfile,SPARSE6_HEADER);
	    else    		    writeline(outfile,GRAPH6_HEADER);
	}

	if (dolabel) nauty_check(WORDSIZE,1,1,NAUTYVERSIONID);

	nin = 0;
	t = CPUTIME;
	while (TRUE)
	{
	    if ((g = readg(infile,NULL,0,&m,&n)) == NULL) break;
	    ++nin;
#if !MAXN
	    DYNALLOC2(graph,gc,gc_sz,n,m,"complg");
#endif

	    if (restricted)
	    {
		ned = 0;
		for (ii = (long)n*m; --ii >= 0; ) ned += POPCOUNT(g[ii]);
		if (ned+ned > (long)n*(n-1))
		{
		    compl(g,m,n,gc);
		    gq = gc;
		}
		else 
		    gq = g;
	    }
	    else
	    {		
	    	compl(g,m,n,gc);
		gq = gc;
	    }
	    if (dolabel)
	    {
#if !MAXN
		DYNALLOC2(graph,h,h_sz,n,m,"complg");
#endif
	 	fcanonise(gq,m,n,h,NULL,FALSE);
		gq = h;
	    }
	    if (outcode == SPARSE6) writes6(outfile,gq,m,n);
	    else                    writeg6(outfile,gq,m,n);
	    FREES(g);
	}
	t = CPUTIME - t;

        if (!quiet)
            fprintf(stderr,
                    ">Z  %ld graphs converted from %s to %s in %3.2f sec.\n",
                    nin,infilename,outfilename,t);

	exit(0);
}
