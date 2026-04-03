/* copyg.c version 1.0; B D McKay, Jan 1997 */

#define USAGE "copyg [-gsfp#:#qhx] [infile [outfile]]"

#define HELPTEXT \
"  Copy a file of graphs with possible format conversion.\n\
\n\
     -g  Use graph6 format for output\n\
     -s  Use sparse6 format for output\n\
         In the absence of -g and -s, the format depends on \n\
         the header or, if none, the first input line.\n\
\n\
     -p# -p#:#  \n\
         Specify range of input lines (first is 1)\n\
     -f  With -p, assume input lines of fixed length\n\
          (ignored if header or first line has sparse6 format).\n\
\n\
     -h  Write a header.\n\
     -x  Don't write a header.\n\
         In the absence of -h and -x, a header is written if\n\
         there is one in the input.\n\
\n\
     -q  Suppress auxiliary output.\n"

/***********************************************************************/

#include "gtools.h"


main(int argc, char *argv[])
{
        graph *g;
	int m,n,codetype;
	char *infilename,*outfilename;
	FILE *infile,*outfile;
	int outcode;
	long nin;
	int argnum,j;
	char *arg,sw;
	boolean sswitch,fswitch,pswitch,qswitch,gswitch;
	boolean hswitch,xswitch;
	boolean badargs;
	long pval1,pval2,maxin;

	HELP;

	sswitch = fswitch = pswitch = FALSE;
	gswitch = qswitch = xswitch = hswitch = FALSE;
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
		    else SWBOOLEAN('q',qswitch)
		    else SWBOOLEAN('f',fswitch)
		    else SWBOOLEAN('h',hswitch)
		    else SWBOOLEAN('x',xswitch)
		    else SWRANGE('p',":-",pswitch,pval1,pval2,"copyg -p")
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
            gt_abort(">E copyg: -s and -g are incompatible\n");
	if (hswitch && xswitch) 
            gt_abort(">E copyg: -h and -x are incompatible\n");

	if (badargs || argnum > 2)
	{
	    fprintf(stderr,">E Usage: %s\n",USAGE);
	    GETHELP;
	    exit(1);
	}

	if (!qswitch)
	{
	    fprintf(stderr,">A copyg");
	    if (sswitch || gswitch || fswitch || pswitch || xswitch || hswitch)
		fprintf(stderr," -");
	    if (sswitch) fprintf(stderr,"s");
	    if (gswitch) fprintf(stderr,"g");
	    if (hswitch) fprintf(stderr,"h");
	    if (xswitch) fprintf(stderr,"x");
	    if (fswitch) fprintf(stderr,"f");
	    if (pswitch) writerange(stderr,'p',pval1,pval2);
	    if (argnum > 0) fprintf(stderr," %s",infilename);
	    if (argnum > 1) fprintf(stderr," %s",outfilename);
	    fprintf(stderr,"\n");
	    fflush(stderr);
	}

	if (infilename && infilename[0] == '-') infilename = NULL;
	infile = opengraphfile(infilename,&codetype,fswitch,
			       pswitch ? pval1 : 1);
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

	if (hswitch || !xswitch && (codetype&HAS_HEADER))
	{
	    if (outcode == SPARSE6) writeline(outfile,SPARSE6_HEADER);
	    else    		    writeline(outfile,GRAPH6_HEADER);
	}

	nin = 0;
	if (!pswitch || pval2 == NOLIMIT)
	    maxin = NOLIMIT;
	else if (pval1 < 1) maxin = pval2;
	else                maxin = pval2 - pval1 + 1;
	while (nin < maxin || maxin == NOLIMIT)
	{
	    if ((g = readg(infile,NULL,0,&m,&n)) == NULL) break;
	    ++nin;
	    if (outcode == readg_code)   writelast(outfile);
	    else if (outcode == SPARSE6) writes6(outfile,g,m,n);
	    else                         writeg6(outfile,g,m,n);
	    FREES(g);
	}

	if (!qswitch) 
	    fprintf(stderr,">Z  %ld graphs copied from %s to %s\n",
		           nin,infilename,outfilename);

	exit(0);
}
