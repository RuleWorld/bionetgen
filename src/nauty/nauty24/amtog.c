/* amtog.c  version 1.0; B D McKay, Jan 1997. */

#define USAGE "amtog [-n#sghq] [infile [outfile]]"

#define HELPTEXT \
" Read graphs in matrix format.\n\
\n\
    -n#   Set the initial graph order to # (no default).\n\
          This can be overridden in the input.\n\
    -g    Write the output in graph6 format (default).\n\
    -s    Write the output in sparse6 format.\n\
    -h    Write a header (according to -g or -s).\n\
    -q    Suppress auxiliary information.\n\
\n\
   Input consists of a sequence of commands restricted to:\n\
\n\
    n=#   set number of vertices (no default)\n\
          The = is optional.\n\
    m     Matrix to follow (01 any spacing or no spacing)\n\
          An 'm' is also assumed if 0 or 1 is encountered.\n\
    M     Complement of matrix to follow (as m)\n\
    t     Upper triangle of matrix to follow, row by row\n\
           excluding the diagonal. (01 in any or no spacing)\n\
    T     Complement of upper trangle to follow (as t)\n\
    q     exit (optional)\n"

/*************************************************************************/

#include "gtools.h"  /* which includes nauty.h and stdio.h */

extern int labelorg;

/**************************************************************************/
/**************************************************************************/

main(argc,argv)
int argc;
char *argv[];
{
	int m,n;
	int argnum,i,j,outcode,val;
	char *arg,sw;
	boolean badargs;
	boolean nswitch,sswitch,gswitch,hswitch,qswitch;
	boolean loop,unsymm,compl,triangle;
	char *infilename,*outfilename;
	FILE *infile,*outfile;
	long ii;
	unsigned long nin;
	char s[10];
#if MAXN
	graph g[MAXN*MAXM];
#else
	DYNALLSTAT(graph,g,g_sz);
#endif

	HELP;

	sswitch = gswitch = FALSE;
	qswitch = nswitch = hswitch = FALSE;
	infilename = outfilename = NULL;
	n = -1;

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
		    else SWBOOLEAN('h',hswitch)
		    else SWBOOLEAN('q',qswitch)
		    else SWINT('n',nswitch,n,">E amtog -n")
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

	if (sswitch && gswitch) 
            gt_abort(">E amtog: -s and -g are incompatible\n");

	if (badargs || argnum > 2)
	{
	    fprintf(stderr,">E Usage: %s\n",USAGE);
	    GETHELP;
	    exit(1);
	}

	if (!infilename || infilename[0] == '-')
	{
	    infilename = "stdin";
	    infile = stdin;
	}
	else if ((infile = fopen(infilename,"r")) == NULL)
	{
	    fprintf(stderr,"Can't open input file %s\n",infilename);
	    gt_abort(NULL);
	}

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

	if (sswitch) outcode = SPARSE6;
	else         outcode = GRAPH6;

	if (hswitch)
	{
	    if (outcode == SPARSE6) writeline(outfile,SPARSE6_HEADER);
	    else    		    writeline(outfile,GRAPH6_HEADER);
	}

#if MAXN
	if (nswitch && n > MAXN)
	{
            gt_abort(">E amtog: value of -n too large\n");
            exit(2);
        }
#else
	if (nswitch)
	{
	    m = (n + WORDSIZE - 1) / WORDSIZE;
            DYNALLOC2(graph,g,g_sz,n,m,"amtog");
        }
#endif
	

     /* perform scanning required */

	nin = 0;
	while (fscanf(infile,"%1s",s) == 1)
	{
	    if (s[0] == 'n')
	    {
		if (fscanf(infile,"=%d",&n) != 1)
		{
		    gt_abort(">E amtog: invalid n=# command\n");
		    exit(2);
		}
		m = (n + WORDSIZE - 1) / WORDSIZE;
#if MAXN
		if (n < 1 || n > MAXN || m > MAXM)
		    gt_abort(">E amtog: n<0 or n,m too big\n");
#else
		DYNALLOC2(graph,g,g_sz,n,m,"amtog");
#endif
	    } 
	    else if (s[0] == 'm' || s[0] == 'M' || s[0] == 't' ||
                     s[0] == 'T' || s[0] == '0' || s[0] == '1')
	    {
		if (n < 0)
		{
		    fprintf(stderr,
                        ">E amtog: matrix found before n is defined\n");
		    exit(2);
		}
		if (s[0] == '0' || s[0] == '1') ungetc(s[0],infile);
		m = (n + WORDSIZE - 1) / WORDSIZE;
	
		for (ii = (long)m*n; --ii >= 0;) g[ii] = 0;

		loop = unsymm = FALSE;
                triangle = (s[0] == 't') || (s[0] == 'T');
		compl = (s[0] == 'M') || (s[0] == 'T');

		++nin;
		for (i = 0; i < n; ++i)
		for (j = (triangle ? i+1 : 0); j < n; ++j)
		{
		    if (fscanf(infile,"%1s",s) != 1)
		    {
			fprintf(stderr,">E amtog: incomplete matrix\n");
			ABORT(">E amtog");
		    }
		    if (s[0] == '0' || s[0] == '1')
		    {
			if (i == j)
			{
			    if (s[0] == '1') loop = TRUE;
			}
			else
			{
			    val = compl ^ (s[0] == '1');
			    if (val == 1)
			    {
				if (triangle)
				{
				    ADDELEMENT(GRAPHROW(g,i,m),j);
				    ADDELEMENT(GRAPHROW(g,j,m),i);
				}
				else
				{
			            if (j < i && !ISELEMENT(GRAPHROW(g,j,m),i))
				        unsymm = TRUE;
			            ADDELEMENT(GRAPHROW(g,i,m),j);
				}
		   	    }
			    else if (j < i && ISELEMENT(GRAPHROW(g,j,m),i))
			        unsymm = TRUE;
			}
		    }
		    else
		    {
			fprintf(stderr,
                          ">E amtog: illegal character in matrix: \"%c\"\n",
                          s[0]);
			exit(2);
		    }
		}

		if (loop) fprintf(stderr,
                       ">E amtog: warning, loop in graph %lu\n",nin);

		if (unsymm) fprintf(stderr,
		       ">E amtog: warning, graph %lu is unsymmetric\n",nin);
	
		if (outcode == SPARSE6) writes6(outfile,g,m,n);
		else                    writeg6(outfile,g,m,n);
	    }
	    else if (s[0] == 'q')
	    {
		exit(0);
	    }
	    else
	    {
	 	fprintf(stderr,">E amtog: invalid command \"%c\"\n",s[0]);
		exit(2);
	    }
	}

	if (!qswitch)
	    fprintf(stderr,">Z  %lu graphs converted from %s to %s.\n",
			   nin,infilename,outfilename);

	exit(0);
}
