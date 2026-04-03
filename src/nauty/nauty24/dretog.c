/* dretog.c  version 1.0; B D McKay, Jan 1997. */

#define USAGE "dretog [-n#o#sghq] [infile [outfile]]"

#define HELPTEXT \
" Read graphs in dreadnaut format.\n\
\n\
   -o#   Label vertices starting at # (default 0).  \n\
         This can be overridden in the input.\n\
   -n#   Set the initial graph order to # (no default).  \n\
         This can be overridden in the input.\n\
   -g    Use graph6 format (default).\n\
   -s    Use sparse6 format.\n\
   -h    Write a header (according to -g or -s).\n\
\n\
  Input consists of a sequence of dreadnaut commands restricted to:\n\
\n\
   n=#   set number of vertices (no default)\n\
         The = is optional.\n\
   $=#   set label of first vertex (default 0)\n\
         The = is optional.\n\
   $$    return origin to initial value (see -o#)\n\
   \"..\" and !..\\n   comments to ignore\n\
   g     specify graph to follow (as dreadnaut format)\n\
         Can be omitted if first character of graph is a digit or ';'.\n\
   q     exit (optional)\n"

/*************************************************************************/

#include "gtools.h"  /* which includes nauty.h and stdio.h */

extern int labelorg;

/**************************************************************************/
/**************************************************************************/

int
main(argc,argv)
int argc;
char *argv[];
{
	int m,n,c;
	int argnum,j,outcode,initorg;
	char *arg,sw;
	boolean badargs,prompt;
	boolean sswitch,gswitch,oswitch,nswitch,hswitch,qswitch;
	char *infilename,*outfilename;
	FILE *infile,*outfile;
	long nin;
	char s[10];
#if MAXN
	graph g[MAXN*MAXM];
#else
	DYNALLSTAT(graph,g,g_sz);
#endif

	HELP;

	sswitch = gswitch = oswitch = FALSE;
	qswitch = nswitch = hswitch = FALSE;
	infilename = outfilename = NULL;
	initorg = 0;
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
		    else SWINT('o',oswitch,initorg,">E dretog -o")
		    else SWINT('n',nswitch,n,">E dretog -n")
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
            gt_abort(">E dretog: -s and -g are incompatible\n");

	if (labelorg < 0) gt_abort(">E dretog: negative origin forbidden\n");

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

#if HAVE_ISATTY
        prompt = isatty(fileno(infile)) && isatty(fileno(outfile));
#else
	prompt = (infile == stdin);
#endif

     /* perform scanning required */

	labelorg = initorg;
	nin = 0;

	while (fscanf(infile,"%1s",s) == 1)
	{
	    if (s[0] == 'n')
	    {
		if (fscanf(infile,"%1s",s) == 1 && s[0] != '=')
		    ungetc(s[0],infile);
		if (fscanf(infile,"%d",&n) != 1)
		{
		    fprintf(stderr,">E dretog: invalid n=# command\n");
		    gt_abort(NULL);
		}
		if (n <= 0)
		    gt_abort(">E dretog: n can't be <= 0\n");
	    } 
	    else if (s[0] == '"')
	    {
		while ((c = getc(infile)) != '"' && c != EOF) {}
	    }
	    else if (s[0] == '!')
	    {
		while ((c = getc(infile)) != '\n' && c != EOF) {}
	    }
	    else if (s[0] == '$')
	    {
		if ((s[0] = getc(infile)) == '$')
		    labelorg = initorg;
		else
		{
		    if (s[0] != '=') ungetc(s[0],infile);
		    if (fscanf(infile,"%d",&labelorg) != 1)
                        gt_abort(">E dretog: invalid $=# command\n");
                    if (labelorg < 0)
                        gt_abort(">E dretog: must have labelorg >= 0\n");
		}
            }
	    else if (s[0] == 'g' || s[0] >= '0' && s[0] <= '9' || s[0] == ';')
	    {
		if (n < 0)
		    gt_abort(">E dretog: g command before n is defined\n");
		if (s[0] != 'g') ungetc(s[0],infile);
		m = (n + WORDSIZE - 1) / WORDSIZE;
#if MAXN
		if (n > MAXN || m > MAXM)
		    gt_abort(">E n or m too big\n");
#else
		DYNALLOC2(graph,g,g_sz,n,m,"dretog");
#endif
		++nin;
		readgraph(infile,g,FALSE,prompt,FALSE,78,m,n);
		if (outcode == SPARSE6) writes6(outfile,g,m,n);
		else                    writeg6(outfile,g,m,n);
	    }
	    else if (s[0] == 'q')
		exit(0);
	    else
	    {
	 	fprintf(stderr,">E dretog: invalid command \"%c\"\n",s[0]);
		gt_abort(NULL);
	    }
	}

	if (!qswitch)
	    fprintf(stderr,">Z  %ld graphs converted from %s to %s\n",
			    nin,infilename,outfilename);

	exit(0);
}
