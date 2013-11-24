/* genrang.c  version 1.4; B D McKay, Aug 8, 2008 */

#define USAGE \
   "genrang [-P#|-P#/#|-e#|-r#|-R#] [-l#] [-m#] [-a] [-s|-g] [-S#] [-q] n num [outfile]"

#define HELPTEXT \
" Generate random graphs.\n\
     n  : number of vertices\n\
    num : number of graphs\n\
\n\
    -s  : Write in sparse6 format (default)\n\
    -g  : Write in graph6 format\n\
    -P#/# : Give edge probability; -P# means -P1/#.\n\
    -e# : Give the number of edges\n\
    -r# : Make regular of specified degree\n\
    -R# : Make regular of specified degree but output\n\
          as vertex count, edgecount, then list of edges\n\
    -l# : Maximum loop multiplicity (default 0)\n\
    -m# : Maximum multiplicity of non-loop edge (default and minimum 1)\n\
         -l and -m are only permitted with -R and -r without -g\n\
    -a  : Make invariant under a random permutation\n\
    -S# : Specify random generator seed (default nondeterministic)\n\
    -q  : suppress auxiliary output\n\
\n\
    Incompatible: -P,-e,-r,-R; -s,-g,-R; -R,-a; -s,-g & -l,-m.\n"

#define MAXREG 88   /* Max value for -r or -R switch */

/*************************************************************************

   Oct 27, 2004 : corrected handling of -P values
**************************************************************************/

#include "gtools.h"

/*************************************************************************/

static void
perminvar(graph *g, permutation *perm, int m, int n)
/* Add to g the least number of edges needed to make perm
   an automorphism. */
{
	int i,j,ii,jj;
	set *gi,*gpi;

	for (i = 0, gi = (set*)g; i < n; ++i, gi += m)
	{
	    gpi = g + m * 1L * perm[i];
	    for (j = -1; (j = nextelement(gi,m,j)) >= 0; )
		if (!ISELEMENT(gpi,perm[j]))
		{
		    ii = perm[i];
		    jj = perm[j];
		    while (ii != i || jj != j)
		    {
			ADDELEMENT(g+m*1L*ii,jj);
			ii = perm[ii];
			jj = perm[jj];
		    }
		}
	}
}

/**************************************************************************/

static void
ranedges(long e, graph *g, int m, int n)
/* Random graph with n vertices and e edges */
{
	unsigned long ln,li,nc2,ned,sofar;
	set *gi,*gj;
	int i,j;

	ln = n;
        nc2 = (ln&1) ? ln*((ln-1)/2) : (ln/2)*(ln-1);

	if (e + e > nc2) ned = nc2 - e;
        else             ned = e;
	sofar = 0;

	for (li = m*ln; --li != 0;) g[li] = 0;
	g[0] = 0;

	while (sofar < ned)
	{
	    i = KRAN(n);
	    do { j = KRAN(n); } while (i == j);
	    gi = GRAPHROW(g,i,m);
	    if (!ISELEMENT(gi,j))
	    {
		ADDELEMENT(gi,j);
		gj = GRAPHROW(g,j,m);
		ADDELEMENT(gj,i);
	 	++sofar;
	    }
	}

	if (ned != e) complement(g,m,n);
}

/**************************************************************************/

static void
makeranreg(int *cub, int degree, int multmax, int loopmax, int n)
/* Make a random regular graph in cub[].  Each consecutive degree
   entries of cub[] is set to the neighbours of one vertex.
   The length of cub had better be at least degree*n  */
{
        long i,j,k,v,w,nn,mult;
        boolean ok;
	set *gi;
#if MAXN
        int deg[MAXN],p[MAXREG*MAXN];
#else
	DYNALLSTAT(int,deg,deg_sz);
	DYNALLSTAT(int,p,p_sz);
	DYNALLSTAT(int,loops,loops_sz);

	DYNALLOC1(int,deg,deg_sz,n,"genrang");
	DYNALLOC2(int,p,p_sz,degree,n,"genrang");
	DYNALLOC1(int,loops,loops_sz,n,"genrang");
#endif

	nn = n;

        for (i = j = 0; i < nn; ++i)
	    for (k = 0; k < degree; ++k)
               p[j++] = i;

        do
        {
            ok = TRUE;

            for (j = degree*nn-1; j >= 1; j -= 2)
            {
                i = KRAN(j);
                k = p[j-1];
                p[j-1] = p[i];
                p[i] = k;
            }
            for (i = 0; i < nn; ++i) deg[i] = loops[i] = 0;

            for (j = degree*nn-1; j >= 1;)
            {
                v = p[j--];
                w = p[j--];
                if (v == w && ++loops[v] > loopmax)
		{
		    ok = FALSE;
                    break;
                }
		if (v != w && multmax < degree)
	 	{
		    mult = 0;
                    for (i = deg[w]; --i >= 0;)
                        if (cub[degree*w+i] == v && ++mult >= multmax) break;
                    if (i >= 0)
                    {
                        ok = FALSE;
                        break;
                    }
		}
                cub[degree*w+deg[w]++] = v;
                cub[degree*v+deg[v]++] = w;
            }
        }
        while (!ok);
}

/**************************************************************************/

static void
ranregR(FILE *f, int degree, int multmax, int loopmax, int n)
/* Make a random regular graph of order n and degree d and write
   it in f, as number of vertices, number of edges, list of edges */
{
    	long i,j,k,l;
	set *gi;
	int loops;
#if MAXN
        int cub[MAXREG*MAXN];
#else
	DYNALLSTAT(int,cub,cub_sz);
	DYNALLOC2(int,cub,cub_sz,degree,n,"genrang");
#endif

	makeranreg(cub,degree,multmax,loopmax,n);

	fprintf(f,"%d %ld\n",n,n*(long)degree/2);
	l = j = 0;
	for (i = 0; i < n; ++i)
	{
	    loops = 0;
	    for (k = 0; k < degree; ++k, ++j)
		if (i < cub[j] || i == cub[j] && (++loops & 1) == 0)
		{
		    if (l > 0 && l % 5 == 0) fprintf(f,"\n");
		    fprintf(f," %ld %d",i,cub[j]);
		    ++l;
		}
	}
	fprintf(f,"\n");
}
 
/**************************************************************************/

static void
ranreg(int degree, graph *g, int m, int n)
/* Make a random simple regular graph of order n and degree d and return
   it in g. */
{
    	int i,j,k;
	set *gi;
#if MAXN
        int cub[MAXREG*MAXN];
#else
	DYNALLSTAT(int,cub,cub_sz);
	DYNALLOC1(int,cub,cub_sz,degree*n,"genrang");
#endif

	makeranreg(cub,degree,1,0,n);

	j = 0;
	for (i = 0, gi = (set*)g; i < n; ++i, gi += m)
	{
	    EMPTYSET(gi,m);
	    for (k = 0; k < degree; ++k)
	    {
	        ADDELEMENT(gi,cub[j]);
	        j++;
	    }
	}
}

/**************************************************************************/

static void
ranreg_sg(int degree, sparsegraph *sg, int multmax, int loopmax, int n)
/* Make a sparse random regular graph of order n and degree d
 * and return it in sg. */
{
    	int i,j,k,k0,deg,loops,nde;
#if MAXN
        int cub[MAXREG*MAXN];
#else
	DYNALLSTAT(int,cub,cub_sz);
	DYNALLOC1(int,cub,cub_sz,degree*n,"genrang");
#endif

	makeranreg(cub,degree,multmax,loopmax,n);

	SG_ALLOC(*sg,n,degree*n,"genrang");

	sg->nv = n;
	j = nde = 0;
	for (i = 0; i < n; ++i)
	{
	    sg->v[i] = k0 = i*degree;
	    loops = deg = 0;
	    for (k = 0; k < degree; ++k, ++j)
	    {
		if (cub[j] == i)
		{
		    /* Loops are in cub twice but sg once */
		    ++loops;
		    if ((loops&1)) sg->e[k0+deg++] = i;
		}
		else
	            sg->e[k0+deg++] = cub[j];
	    }
	    sg->d[i] = deg;
	    nde += deg;
	}
	sg->nde = nde;
}

/**************************************************************************/
/**************************************************************************/

int
main(int argc, char *argv[])
{
	int m,n,codetype;
	int argnum,j;
	char *arg,sw;
	boolean badargs;
	boolean gswitch,sswitch,qswitch,Sswitch,Rswitch,lswitch;
	boolean aswitch,P1switch,P2switch,eswitch,rswitch,mswitch;
	long numgraphs,nout,P1value,P2value,evalue,rvalue;
	nauty_counter ln,nc2;
	int Svalue,loopmax,multmax;
        static FILE *outfile;
	char *outfilename;
	sparsegraph sg;
	boolean usesparse;

#if MAXN
	graph g[MAXM*1L*MAXN];
	permutation perm[MAXN];
#else
	DYNALLSTAT(graph,g,g_sz);
	DYNALLSTAT(permutation,perm,perm_sz);
#endif

	HELP;

	gswitch = sswitch = qswitch = Sswitch = Rswitch = FALSE;
	aswitch = P1switch = P2switch = eswitch = rswitch = FALSE;
	lswitch = mswitch = FALSE;
	outfilename = NULL;

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
			 SWBOOLEAN('g',gswitch)
		    else SWBOOLEAN('s',sswitch)
		    else SWBOOLEAN('a',aswitch)
		    else SWBOOLEAN('q',qswitch)
		    else SWLONG('P',P1switch,P1value,"genrang -P")
		    else SWLONG('/',P2switch,P2value,"genrang -P")
		    else SWLONG('e',eswitch,evalue,"genrang -e")
		    else SWLONG('r',rswitch,rvalue,"genrang -r")
		    else SWLONG('R',Rswitch,rvalue,"genrang -R")
		    else SWINT('S',Sswitch,Svalue,"genrang -S")
		    else SWINT('l',lswitch,loopmax,"genrang -l")
		    else SWINT('m',mswitch,multmax,"genrang -m")
		    else badargs = TRUE;
		}
	    }
	    else
	    {
		++argnum;
		if      (argnum == 1)
		{
		    if (sscanf(arg,"%d",&n) != 1 || n < 1) badargs = TRUE;
		}
		else if (argnum == 2)
		{
		    if (sscanf(arg,"%ld",&numgraphs) != 1 || numgraphs < 1)
			badargs = TRUE;
		}
		else if (argnum == 3) outfilename = arg;
		else                  badargs = TRUE;
	    }
	}

	if ((gswitch!=0) + (sswitch!=0) > 1)
	    gt_abort(">E genrang: -gs are incompatible\n");

	if (gswitch) codetype = GRAPH6;
	else         codetype = SPARSE6;

	if (P1switch && !P2switch)
	{
	    P2value = P1value;
	    P1value = 1;
	}
	else if (P2switch && !P1switch)
	{
	    P1value = 1;
	    P1switch = TRUE;
	}

	if (P1switch && (P1value < 0 || P2value <= 0 || P1value > P2value))
	    gt_abort(">E genrang: bad value for -P switch\n");

	if ((P1switch!=0) + (eswitch!=0) + (rswitch!=0) + (Rswitch!=0) > 1)
	    gt_abort(">E genrang: -Per are incompatible\n");

	if ((sswitch!=0) + (gswitch!=0) + (Rswitch!=0) > 1)
	    gt_abort(">E genrang: -sgR are incompatible\n");

	if ((aswitch!=0) + (Rswitch!=0) > 1)
	    gt_abort(">E genrang: -aR are incompatible\n");

	if (!lswitch) loopmax = 0;
	if (!mswitch) multmax = 1;

	if ((loopmax>0 || multmax>1) && !(Rswitch || rswitch && !gswitch))
	    gt_abort(">E genrang: -l,-m need -R or -r without -g\n");

	if (multmax < 1 || loopmax < 0)
	    gt_abort(">E genrang: bad value for -l or -m\n");

	if (argnum < 2 || argnum > 3) badargs = TRUE;

	if (badargs)
	{
	    fprintf(stderr,">E Usage: %s\n",USAGE);
	    GETHELP;
	    exit(1);
	}

	if (!Sswitch)
	{
#ifdef INITSEED
	    INITSEED;
	    ran_init(seed);
#endif
	}
	else
	    ran_init(Svalue);

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

	m = (n + WORDSIZE + 1) / WORDSIZE;
	usesparse = rswitch && !aswitch && codetype==SPARSE6;
#if !MAXN
	if (!Rswitch && !usesparse)
	{
	    DYNALLOC2(graph,g,g_sz,n,m,"genrang");
	    if (aswitch) DYNALLOC1(permutation,perm,perm_sz,n,"genrang");
	}
#endif

	rswitch = rswitch || Rswitch;

	if (rswitch && rvalue > MAXREG)
	{
	    fprintf(stderr,
		    ">E -r/-R is only implemented for degree <= %d\n",MAXREG);
	    exit(1);
	}

	ln = n;
        nc2 = ln*loopmax + ln*(ln-1)/2*multmax;

	if (eswitch && evalue > nc2)
	{   
            fprintf(stderr,
                 ">E There are no graphs of order %d and %ld edges\n",
                 n,evalue);
            exit(1);
        }

	if (rswitch && ((n&1) != 0 && (rvalue&1) != 0
	    || rvalue > (n-1)*multmax+2*loopmax))
	{
	    fprintf(stderr, 	
		 ">E There are no such graphs of order %d and degree %ld\n",
		 n,rvalue);
	    exit(1);
	}

	if (!P1switch)
        {
	    P1value = 1;
	    P2value = 2;
	}

	SG_INIT(sg);

	for (nout = 1; nout <= numgraphs; ++nout)
	{
	    if (eswitch) ranedges(evalue,g,m,n);
	    else if (Rswitch) ranregR(outfile,rvalue,multmax,loopmax,n);
	    else if (rswitch && usesparse)
		ranreg_sg(rvalue,&sg,multmax,loopmax,n);
	    else if (rswitch && !usesparse) ranreg(rvalue,g,m,n);
            else rangraph2(g,FALSE,P1value,P2value,m,n);

	    if (Rswitch) continue;

	    if (aswitch)
	    {
		ranperm(perm,n);
		perminvar(g,perm,m,n);
	    }
	    if (codetype == SPARSE6)
		if (usesparse)
		{
		    sortlists_sg(&sg);
		    writes6_sg(outfile,&sg);
		}
	        else
		    writes6(outfile,g,m,n);
	    else 
		writeg6(outfile,g,m,n);
	}

	exit(0);
}
