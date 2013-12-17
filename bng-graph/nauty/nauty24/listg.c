/* listg.c  version 1.4; B D McKay, June 2007 */

#define USAGE "listg [-fp#:#l#o#Ftq] [-a|-A|-c|-d|-e|-M|-s] [infile [outfile]]"

#define HELPTEXT \
" Write graphs in human-readable format.\n\
\n\
    -f  : assume inputs have same size (only used from a file\n\
           and only if -p is given)\n\
    -p#, -p#:#, -p#-# : only display one graph or a sequence of\n\
           graphs.  The first graph is number 1.  A second number\n\
           which is empty or zero means infinity.\n\
    -a  : write as adjacency matrix, not as list of adjacencies\n\
    -A  : same as -a with a space between entries\n\
    -l# : specify screen width limit (default 78, 0 means no limit)\n\
          This is not currently implemented with -a or -A.\n\
    -o# : specify number of first vertex (default is 0).\n\
    -d  : write output to satisfy dreadnaut \n\
    -c  : write ascii form with minimal line-breaks\n\
    -e  : write a list of edges, preceded by the order and the\n\
          number of edges\n\
    -M  : write in Magma format\n\
    -W  : write matrix in Maple format\n\
    -t  : write upper triangle only (affects -a, -A, -d and default)\n\
    -s  : write only the numbers of vertices and edges\n\
    -F  : write a form-feed after each graph except the last\n\
    -q  : suppress auxiliary output\n\
\n\
    -a, -A, -c, -d, -M, -W and -e are incompatible.\n"

#define MAPLE_MATRIX 1  /* 1 for Matrix(..), 0 for array(..) */

/*************************************************************************

    June 26, 2007 : Fix error in putve() reported by Evan Heidtmann

*************************************************************************/

#include "gtools.h"
#define LABELORG 0   /* number of first vertex (any integer >= 0) */
#define LINELEN CONSOLWIDTH   /* max characters per line (0 = no limit) */

static FILE *infile,*outfile;
static unsigned long nin;
extern int labelorg;

/*****************************************************************************
*                                                                            *
*  putsetx(f,set1,curlenp,linelength,m,compress,start)   writes the set      *
*  set1 to file f using at most linelength characters per line (excluding    *
*  '\n').   Set elements less than or equal to start are ignored.            *
*  *curlenp is the number of characters on the line so far; it is updated.   *
*  A range j1,j1+1,...,j2 for j2-j1>=2 is written as "j1:j2" if compress     *
*  is nonzero (eg. TRUE); otherwise each element is written separately.      *
*  No final '\n' is written.  labelorg is used.                              *
*                                                                            *
*  FUNCTIONS CALLED: nextelement(),itos()                                    *
*                                                                            *
*****************************************************************************/

void
putsetx(FILE *f, set *set1, int *curlenp, int linelength, int m,
        boolean compress, int start)
{
        int slen,j1,j2;
        char s[40];
	boolean first;

	first = TRUE;
        j1 = start;
        while ((j1 = nextelement(set1,m,j1)) >= 0)
        {
            j2 = j1;
            if (compress)
            {
                while (nextelement(set1,m,j2) == j2 + 1)
                    ++j2;
                if (j2 == j1+1)
                    j2 = j1;
            }
            slen = itos(j1 + labelorg,s);
            if (j2 >= j1 + 2)
            {
                s[slen] = ':';
                slen += 1 + itos(j2 + labelorg,&s[slen+1]);
            }

            if (*curlenp + slen + 1 >= linelength)
            {
                fprintf(f,"\n ");
                *curlenp = 1;
            }
	    if (first)
	    {
                fprintf(f,"%s",s);
                *curlenp += slen;
		first = FALSE;
	    }
	    else
            {    
                fprintf(f," %s",s);
                *curlenp += slen + 1;
            }
            j1 = j2;
        }
}

/*****************************************************************************
*                                                                            *
*  STOLEN FROM naututil.c                                                    *
*  putgraphx(f,g,linelength,m,n) writes a list of the edges of g to f        *
*  using at most linelength characters per line (excluding '\n').            *
*  If triang, only write the upper triangle.                                 *
*  labelorg is used.                                                         *
*                                                                            *
*****************************************************************************/

void
putgraphx(FILE *f, graph *g, int linelength, boolean triang, int m, int n)
{
        int i,curlen;
        set *pg;

        for (i = 0, pg = g; i < n; ++i, pg += m)
        {
            fprintf(f,"%3d : ",i + labelorg);
            curlen = 7;
            putsetx(f,pg,&curlen,linelength,m,FALSE,triang ? i-1 : -1);
            fprintf(f,";\n");
        }
}

/***************************************************************************/

void
putedges(FILE *f, graph *g, boolean ptn, int linelength, int m, int n)
/* Write list of edges, preceded by the numbers of vertices and
   edges and optionally by "1" if "ptn" is TRUE.  Use labelorg */
{
	int i,j,curlen,ne;
	char s[20];
	set *pg;

	ne = 0;
        for (i = 0, pg = g; i < n; ++i, pg += m)
	{
	    for (j = i-1; (j = nextelement(pg,m,j)) >= 0;)
		++ne;
	}

	if (ptn) fprintf(f,"%d %d 1\n",n,ne);
	else     fprintf(f,"%d %d\n",n,ne);

	curlen = 0;
        for (i = 0, pg = g; i < n; ++i, pg += m)
	{
	    for (j = i-1; (j = nextelement(pg,m,j)) >= 0;)
	    { 
		if (curlen > linelength - 10 && linelength > 0)
		{
		    fprintf(f,"\n");
		    curlen = 0;
		}
		if (curlen > 0)
		{
		    fprintf(f,"  ");
		    curlen += 2;
		}
		curlen += itos(i+labelorg,s);
		fprintf(f,s);
		fprintf(f," ");
		curlen += 1 + itos(j+labelorg,s);
		fprintf(f,s);
	    }
	}
	fprintf(f,"\n");
}

/***************************************************************************/

void
putcgraph(FILE *f, graph *g, int linelength, int m, int n)
/* write compressed form, using labelorg */
{
        int i,curlen;
	int semicolons;
	char s[20];
        set *pg;

	curlen = itos(n,s)+2;
	fprintf(f,";n%sg",s);

	semicolons = 0;
        for (i = 0, pg = g; i < n; ++i, pg += m)
        {
	    if (nextelement(pg,m,i-1) >= 0)
	    {
		while (semicolons > 0)
		{
		    if (curlen >= linelength-1 && linelength > 0)
		    {
			fprintf(f,"\n ");
			curlen = 1;
		    }
		    fprintf(f,";");
		    ++curlen;
		    --semicolons;
		}
                putsetx(f,pg,&curlen,linelength,m,FALSE,i-1);
                semicolons = 1;
	    }
	    else
		++semicolons;
        }
	fprintf(f,".\n");
}

/**************************************************************************/

static void
putve(FILE *f, unsigned long id, graph *g, int m, int n)
/* Write the numbers of vertices and edges */
{
	unsigned long ne;
	setword x,*pg;

	ne = 0;
	for (pg = g + m*(long)n; --pg >= g;)
	    if ((x = *pg) != 0) ne += POPCOUNT(x);

        fprintf(f,"Graph %lu has %d vertices and %lu edges.\n",id,n,ne/2);
}

/**************************************************************************/

static void
putam(FILE *f, graph *g, int linelength, boolean space,
	boolean triang, int m, int n)
/* write adjacency matrix */
{
 	set *gi;
	int i,j;
	boolean first;

	for (i = 0, gi = (set*)g; i < n - (triang!=0); ++i, gi += m)
	{
	    first = TRUE;
	    for (j = triang ? i+1 : 0; j < n; ++j)
	    {
		if (!first && space) putc(' ',f);
		else                 first = FALSE;
		if (ISELEMENT(gi,j)) putc('1',f);
		else                 putc('0',f);
	    }
	    putc('\n',f);
	}
}

/**************************************************************************/

static void
putMagma(FILE *outfile, graph *g, int linelength, int m, int n, long index)
{
	int i,j;
	set *gi;
	boolean first;

	fprintf(outfile,"g%ld := Graph<%d|[\n",index,n);

	for (i = 0, gi = (set*)g; i < n; ++i, gi += m)
	{
	    fprintf(outfile,"{");
	    first = TRUE;
	    for (j = i; (j = nextelement(gi,m,j)) >= 0; )
	    {
		if (!first) fprintf(outfile,",");
		first = FALSE;
		fprintf(outfile,"%d",j+1);
	    }
	    fprintf(outfile,"}");
	    if (i != n-1) fprintf(outfile,",\n");
	}
	fprintf(outfile,"]>;\n");
}

/**************************************************************************/

static void
putMaple(FILE *outfile, graph *g, int linelength, int m, int n, long index)
{
	int i,j;
	set *gi;
	boolean first;

#if MAPLE_MATRIX
	fprintf(outfile,"f%ld := Matrix(%d,%d,[\n",index,n,n);
#else
	fprintf(outfile,"f%ld := array(1..%d,1..%d,[\n",index,n,n);
#endif

	for (i = 0, gi = (set*)g; i < n; ++i, gi += m)
        {
            fprintf(outfile,"[");
            first = TRUE;
            for (j = 0; j < n; ++j)
            {
                if (!first) fprintf(outfile,",");
                first = FALSE;
                fprintf(outfile,"%d",(ISELEMENT(gi,j)?1:0));
            }
            fprintf(outfile,"]");
            if (i != n-1) fprintf(outfile,",\n");
        }
        fprintf(outfile,"]);\n");
}

/**************************************************************************/
/**************************************************************************/

int
main(int argc, char *argv[])
{
	graph *g;
	int m,n,codetype;
	int argnum,j;
	char *arg,sw;
	boolean badargs;
	unsigned long maxin;
	long pval1,pval2;
	boolean fswitch,pswitch,cswitch,dswitch;
	boolean aswitch,lswitch,oswitch,Fswitch;
	boolean Aswitch,eswitch,tswitch,qswitch;
	boolean sswitch,Mswitch,Wswitch,Eswitch;
	int linelength;
	char *infilename,*outfilename;

	HELP;

	fswitch = pswitch = cswitch = dswitch = FALSE;
	aswitch = lswitch = oswitch = Fswitch = FALSE;
	Aswitch = eswitch = tswitch = qswitch = FALSE;
	sswitch = Mswitch = Wswitch = Eswitch = FALSE;
	infilename = outfilename = NULL;
	linelength = LINELEN;
	labelorg = 0;

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
			 SWBOOLEAN('a',aswitch)
		    else SWBOOLEAN('A',Aswitch)
		    else SWBOOLEAN('c',cswitch)
		    else SWBOOLEAN('d',dswitch)
		    else SWBOOLEAN('e',eswitch)
		    else SWBOOLEAN('E',Eswitch)
		    else SWBOOLEAN('f',fswitch)
		    else SWBOOLEAN('F',Fswitch)
		    else SWBOOLEAN('t',tswitch)
		    else SWBOOLEAN('q',qswitch)
		    else SWBOOLEAN('M',Mswitch)
		    else SWBOOLEAN('W',Wswitch)
		    else SWBOOLEAN('s',sswitch)
		    else SWRANGE('p',":-",pswitch,pval1,pval2,"listg -p")
		    else SWINT('l',lswitch,linelength,"listg -l")
		    else SWINT('o',oswitch,labelorg,"listg -o")
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

	if (labelorg < 0) gt_abort(">E listg: negative origin forbidden.\n");

	if ((aswitch!=0) + (Aswitch!=0) + (eswitch!=0) + (Mswitch!=0) +
	    (Wswitch!=0) + (sswitch!=0) + (dswitch!=0) + (cswitch!=0) +
	    (Eswitch!=0) > 1)
	    gt_abort(">E listg: -aAMWeEcds are incompatible\n");

	if (badargs)
	{
	    fprintf(stderr,">E Usage: %s\n",USAGE);
	    fprintf(stderr,"  Try listg -help for more detailed help.\n");
	    exit(1);
	}

	if (!pswitch || pval1 < 1) pval1 = 1;

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

	nin = 0;
	if (!pswitch || pval2 == NOLIMIT)
	    maxin = NOLIMIT;
	else if (pval1 < 1) maxin = pval2;
	else                maxin = pval2 - pval1 + 1;
	while (nin < maxin || maxin == NOLIMIT)
	{
	    if ((g = readg(infile,NULL,0,&m,&n)) == NULL) break;
	    ++nin;

	    if (Fswitch && nin > 1) fprintf(outfile,"\f");

	    if (cswitch)
		putcgraph(outfile,g,linelength,m,n);
	    else if (dswitch)
	    {
		if (qswitch)
		    fprintf(outfile,"%d\n",n);
		else
		{
		    fprintf(outfile,"\n!Graph %lu.\n",pval1+nin-1);
		    fprintf(outfile,"n=%d $=%d g\n",n,labelorg);
		}
		putgraphx(outfile,g,linelength,tswitch,m,n);
		if (!qswitch) fprintf(outfile,"$$\n");
	    }
	    else if (Mswitch)
	        putMagma(outfile,g,linelength,m,n,pval1+nin-1);
	    else if (Wswitch)
	        putMaple(outfile,g,linelength,m,n,pval1+nin-1);
	    else if (sswitch)
		putve(outfile,pval1+nin-1,g,m,n);
	    else
            {
		if (qswitch)
		{
		    if (!eswitch && !Eswitch) fprintf(outfile,"%d\n",n);
		}
                else fprintf(outfile,"\nGraph %lu, order %d.\n",
				     pval1+nin-1,n);
	        if (aswitch|Aswitch)
		    putam(outfile,g,linelength,Aswitch,tswitch,m,n);
		else if (eswitch || Eswitch)
		    putedges(outfile,g,Eswitch,linelength,m,n);
	        else
	            putgraphx(outfile,g,linelength,tswitch,m,n);
            }
	    FREES(g);
	}

	exit(0);
}
