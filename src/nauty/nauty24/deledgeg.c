/* deledgeg.c  version 1.2; B D McKay, March 2009. */

#define USAGE "deledgeg [-lq] [-d#] [infile [outfile]]"

#define HELPTEXT \
" For each edge e, output G-e\n\
\n\
    The output file has a header if and only if the input file does.\n\
\n\
    -l  Canonically label outputs\n\
    -d# Specify a lower bound on the minimum degree of the output\n\
    -q  Suppress auxiliary information\n"

/*************************************************************************/

#include "gtools.h" 

/**************************************************************************/

int
main(int argc, char *argv[])
{
        char *infilename,*outfilename;
        FILE *infile,*outfile;
        boolean badargs,dolabel,quiet,dswitch;
	int i,j,m,n,v,w,argnum;
	int codetype,outcode;
	graph *g,*gq;
	nauty_counter nin,nout;
        char *arg,sw;
	setword *gv,*gw;
	int mindeg,actmindeg,degv;
	double t;
#if MAXN
	graph h[MAXN*MAXM];
	int deg[MAXN];
#else
	DYNALLSTAT(graph,h,h_sz);
	DYNALLSTAT(int,deg,deg_sz);
#endif

	HELP;

        infilename = outfilename = NULL;
        badargs = FALSE;
	dswitch = dolabel = quiet = FALSE;

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
		         SWBOOLEAN('l',dolabel)
		    else SWBOOLEAN('q',quiet)
                    else SWINT('d',dswitch,mindeg,">E deledgeg -d")
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
	    fprintf(stderr,">A deledgeg");
	    if (dolabel) fprintf(stderr," -l");
	    if (dswitch) fprintf(stderr," -d%d",mindeg);
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

	if (!dswitch) mindeg = 0;

	if (dolabel) nauty_check(WORDSIZE,1,1,NAUTYVERSIONID);

	nin = nout = 0;
	t = CPUTIME;
	while (TRUE)
	{
	    if ((g = readg(infile,NULL,0,&m,&n)) == NULL) break;
	    ++nin;

#if !MAXN
	    DYNALLOC1(int,deg,deg_sz,n,"deledgeg");
#endif

	    actmindeg = n;
	    for (v = 0, gv = g; v < n; ++v, gv += m)
	    {
		degv = 0;
		for (i = 0; i < m; ++i)
		    degv += POPCOUNT(gv[i]);
		if (degv < actmindeg) actmindeg = degv;
		deg[v] = degv;
	    }

	    if (actmindeg < mindeg) continue;

	    for (v = 0, gv = g; v < n; ++v, gv += m)
	    {
		if (deg[v] <= mindeg) continue;

	    	for (w = v; (w = nextelement(gv,m,w)) >= 0; )
	        {
		    if (deg[w] <= mindeg) continue;

		    gw = GRAPHROW(g,w,m);
		    DELELEMENT(gv,w);
		    DELELEMENT(gw,v);
		    gq = g;
	    
	            if (dolabel)
	            {
#if !MAXN
		        DYNALLOC2(graph,h,h_sz,n,m,"deledgeg");
#endif
	 	        fcanonise(g,m,n,h,NULL,FALSE);  /* FIXME (loops) */
		        gq = h;
	            }
	            if (outcode == SPARSE6) writes6(outfile,gq,m,n);
	            else                    writeg6(outfile,gq,m,n);
		    ++nout;
		    ADDELEMENT(gv,w);
                    ADDELEMENT(gw,v);
		}
	    }
	    FREES(g);
	}
	t = CPUTIME - t;

#if LONG_LONG_COUNTERS
        if (!quiet)
            fprintf(stderr,
              ">Z  %lld graphs read from %s, %lld written to %s; %3.2f sec.\n",
                    nin,infilename,nout,outfilename,t);
#else
        if (!quiet)
            fprintf(stderr,
              ">Z  %ld graphs read from %s, %ld written to %s; %3.2f sec.\n",
                    nin,infilename,nout,outfilename,t);
#endif

	exit(0);
}
