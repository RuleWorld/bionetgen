/* gtnauty.c :  nauty-related routines for gtools.

   Jan 15, 2001 : Increased graph order limit from 2^16-1 to 2^22-1.
   Aug  9, 2001 : Added fgroup_inv() and fcanonise_inv()
   Sep 15, 2004 : Completed prototypes
   Oct 16, 2004 : DEAFULTOPTIONS_GRAPH
   Nov 17, 2005 : Added fcanonise_inv_sg()

**************************************************************************/

#include "gtools.h"   /* which includes naututil.h, nausparse.h, stdio.h */

static boolean issymm;
static set *g0;
static int gm;
static int fuzz2[] = {006532,070236,035523,062437};
#define FUZZ2(x) ((x) ^ fuzz2[(x)&3])

int gt_numorbits;

#ifdef REFINE
void REFINE(graph*,int*,int*,int,int*,permutation*,set*,int*,int,int);
#endif

/**************************************************************************/

static void                         /* sort k longwords */
lsort(long *x, int k)
{
        int i,j,h;
        long iw;

        j = k / 3;
        h = 1;
        do
            h = 3 * h + 1;
        while (h < j);

        do
        {
            for (i = h; i < k; ++i)
            {
                iw = x[i];
                for (j = i; x[j-h] > iw; )
                {
                    x[j] = x[j-h];
                    if ((j -= h) < h) break;
                }
                x[j] = iw;
            }
            h /= 3;
        }
        while (h > 0);
}

/**************************************************************************/

void
fcanonise(graph *g, int m, int n, graph *h, char *fmt, boolean digraph)
/*  canonise g under format fmt; result in h.
   fmt is either NULL (for no vertex classification) or is a string
   with char-valued colours for the vertices.  If it ends early, it
   is assumed to continue with the colour 'z' indefinitely. */
{
#if MAXN
        int lab[MAXN],ptn[MAXN],orbits[MAXN];
        long x[MAXN];
        permutation count[MAXN];
        set active[MAXM];
        setword workspace[4*MAXM];
#else
	DYNALLSTAT(int,lab,lab_sz);
	DYNALLSTAT(int,ptn,ptn_sz);
	DYNALLSTAT(int,orbits,orbits_sz);
	DYNALLSTAT(long,x,x_sz);
	DYNALLSTAT(permutation,count,count_sz);
	DYNALLSTAT(set,active,active_sz);
	DYNALLSTAT(setword,workspace,workspace_sz);
#endif
        int i;
	boolean endfmt;
        int numcells,code;
        statsblk stats;
        static DEFAULTOPTIONS_GRAPH(options);

#if MAXN
	if (n > MAXN || m > MAXM)
	{
	    fprintf(stderr,">E fcanonise: m or n too large\n");
	    ABORT(">E fcanonise");
	}
#else
	DYNALLOC1(int,lab,lab_sz,n,"fcanonise");
	DYNALLOC1(int,ptn,ptn_sz,n,"fcanonise");
	DYNALLOC1(int,orbits,orbits_sz,n,"fcanonise");
	DYNALLOC1(long,x,x_sz,n,"fcanonise");
	DYNALLOC1(permutation,count,count_sz,n,"fcanonise");
	DYNALLOC1(set,active,active_sz,m,"fcanonise");
	DYNALLOC1(setword,workspace,workspace_sz,4*m,"fcanonise");
#endif

        EMPTYSET(active,m);
        ADDELEMENT(active,0);
        numcells = 1;

	if (fmt != NULL)
	{
	    endfmt = FALSE;
            for (i = 0; i < n; ++i)
	    {
		if (!endfmt && fmt[i] != '\0') 
		    x[i] = ((long)fmt[i] << 22) | i;
		else
		{
		    endfmt = TRUE;
		    x[i] = ((long)'z' << 22) | i;
		}
	    }
            lsort(x,n);

            for (i = 0; i < n; ++i)
            {
                lab[i] = x[i] & 0x3fffffL;
                if (i == n-1)
                    ptn[i] = 0;
                else if ((x[i+1] >> 22) != (x[i] >> 22))
                {
                    ++numcells;
                    ADDELEMENT(active,i+1);
                    ptn[i] = 0;
                }
                else
                    ptn[i] = 1;
            }
	}
	else
	{
	    for (i = 0; i < n; ++i)
	    {
		lab[i] = i;
		ptn[i] = 1;
	    }
	    ptn[n-1] = 0;
	}

	if (m == 1)
            refine1(g,lab,ptn,0,&numcells,count,active,&code,1,n);
	else
            refine(g,lab,ptn,0,&numcells,count,active,&code,m,n);

        if (numcells == n || numcells == n-1 && !digraph) 
        {
            for (i = 0; i < n; ++i) count[i] = lab[i];
            updatecan(g,h,count,0,m,n);
	    gt_numorbits = numcells;
        }
        else
        {
            options.getcanon = TRUE;
            options.defaultptn = FALSE;
	    options.digraph = digraph;
#ifdef REFINE
	    options.userrefproc = REFINE;
#endif

            EMPTYSET(active,m);
            nauty(g,lab,ptn,active,orbits,&options,&stats,
                                                  workspace,4*m,m,n,h);
	    gt_numorbits = stats.numorbits;
        }
}

/**************************************************************************/

void
fcanonise_inv(graph *g, int m, int n, graph *h, char *fmt,
   void (*invarproc)(graph*,int*,int*,int,int,int,permutation*,int,
    boolean,int,int), int mininvarlevel, int maxinvarlevel,
    int invararg, boolean digraph)
/*  canonise g under format fmt; result in h.
   fmt is either NULL (for no vertex classification) or is a string
   with char-valued colours for the vertices.  If it ends early, it
   is assumed to continue with the colour 'z' indefinitely.
   This is like fcanonise() except that a invariant and its arguments
   can be specified. */
{
#if MAXN
        int lab[MAXN],ptn[MAXN],orbits[MAXN];
        long x[MAXN];
        permutation count[MAXN];
        set active[MAXM];
        setword workspace[4*MAXM];
#else
	DYNALLSTAT(int,lab,lab_sz);
	DYNALLSTAT(int,ptn,ptn_sz);
	DYNALLSTAT(int,orbits,orbits_sz);
	DYNALLSTAT(long,x,x_sz);
	DYNALLSTAT(permutation,count,count_sz);
	DYNALLSTAT(set,active,active_sz);
	DYNALLSTAT(setword,workspace,workspace_sz);
#endif
        int i;
	boolean endfmt;
        int numcells,code;
        statsblk stats;
        static DEFAULTOPTIONS_GRAPH(options);

#if MAXN
	if (n > MAXN || m > MAXM)
	{
	    fprintf(stderr,">E fcanonise: m or n too large\n");
	    ABORT(">E fcanonise");
	}
#else
	DYNALLOC1(int,lab,lab_sz,n,"fcanonise");
	DYNALLOC1(int,ptn,ptn_sz,n,"fcanonise");
	DYNALLOC1(int,orbits,orbits_sz,n,"fcanonise");
	DYNALLOC1(long,x,x_sz,n,"fcanonise");
	DYNALLOC1(permutation,count,count_sz,n,"fcanonise");
	DYNALLOC1(set,active,active_sz,m,"fcanonise");
	DYNALLOC1(setword,workspace,workspace_sz,4*m,"fcanonise");
#endif

        EMPTYSET(active,m);
        ADDELEMENT(active,0);
        numcells = 1;

	if (fmt != NULL)
	{
	    endfmt = FALSE;
            for (i = 0; i < n; ++i)
	    {
		if (!endfmt && fmt[i] != '\0') 
		    x[i] = ((long)fmt[i] << 22) | i;
		else
		{
		    endfmt = TRUE;
		    x[i] = ((long)'z' << 22) | i;
		}
	    }
            lsort(x,n);

            for (i = 0; i < n; ++i)
            {
                lab[i] = x[i] & 0x3fffffL;
                if (i == n-1)
                    ptn[i] = 0;
                else if ((x[i+1] >> 22) != (x[i] >> 22))
                {
                    ++numcells;
                    ADDELEMENT(active,i+1);
                    ptn[i] = 0;
                }
                else
                    ptn[i] = 1;
            }
	}
	else
	{
	    for (i = 0; i < n; ++i)
	    {
		lab[i] = i;
		ptn[i] = 1;
	    }
	    ptn[n-1] = 0;
	}

	if (m == 1)
            refine1(g,lab,ptn,0,&numcells,count,active,&code,1,n);
	else
            refine(g,lab,ptn,0,&numcells,count,active,&code,m,n);

        if (numcells == n || !digraph && numcells >= n-1)
        {
            for (i = 0; i < n; ++i) count[i] = lab[i];
            updatecan(g,h,count,0,m,n);
	    gt_numorbits = numcells;
        }
        else
        {
            options.getcanon = TRUE;
            options.digraph = digraph;
            options.defaultptn = FALSE;
	    if (invarproc)
	    {
		options.invarproc = invarproc;
		options.mininvarlevel = mininvarlevel;
		options.maxinvarlevel = maxinvarlevel;
		options.invararg = invararg;
	    }
#ifdef REFINE
	    options.userrefproc = REFINE;
#endif

            EMPTYSET(active,m);
            nauty(g,lab,ptn,active,orbits,&options,&stats,
                                                  workspace,4*m,m,n,h);
	    gt_numorbits = stats.numorbits;
        }
}

/**************************************************************************/

void
fcanonise_inv_sg(sparsegraph *g, int m, int n, sparsegraph *h, char *fmt,
   void (*invarproc)(graph*,int*,int*,int,int,int,permutation*,int,
    boolean,int,int), int mininvarlevel, int maxinvarlevel,
    int invararg, boolean digraph)
/*  canonise g under format fmt; result in h.
   fmt is either NULL (for no vertex classification) or is a string
   with char-valued colours for the vertices.  If it ends early, it
   is assumed to continue with the colour 'z' indefinitely.
   This is like fcanonise() except that a invariant and its arguments
   can be specified.  Version for sparse graphs. */
{
#if MAXN
        int lab[MAXN],ptn[MAXN],orbits[MAXN];
        long x[MAXN];
        permutation count[MAXN];
        set active[MAXM];
        setword workspace[4*MAXM];
#else
	DYNALLSTAT(int,lab,lab_sz);
	DYNALLSTAT(int,ptn,ptn_sz);
	DYNALLSTAT(int,orbits,orbits_sz);
	DYNALLSTAT(long,x,x_sz);
	DYNALLSTAT(permutation,count,count_sz);
	DYNALLSTAT(set,active,active_sz);
	DYNALLSTAT(setword,workspace,workspace_sz);
#endif
        int i;
	boolean endfmt;
        int numcells,code;
        statsblk stats;
        static DEFAULTOPTIONS_SPARSEGRAPH(options);

#if MAXN
	if (n > MAXN || m > MAXM)
	{
	    fprintf(stderr,">E fcanonise: m or n too large\n");
	    ABORT(">E fcanonise");
	}
#else
	DYNALLOC1(int,lab,lab_sz,n,"fcanonise");
	DYNALLOC1(int,ptn,ptn_sz,n,"fcanonise");
	DYNALLOC1(int,orbits,orbits_sz,n,"fcanonise");
	DYNALLOC1(long,x,x_sz,n,"fcanonise");
	DYNALLOC1(permutation,count,count_sz,n,"fcanonise");
	DYNALLOC1(set,active,active_sz,m,"fcanonise");
	DYNALLOC1(setword,workspace,workspace_sz,4*m,"fcanonise");
#endif

        EMPTYSET(active,m);
        ADDELEMENT(active,0);
        numcells = 1;

	if (fmt != NULL)
	{
	    endfmt = FALSE;
            for (i = 0; i < n; ++i)
	    {
		if (!endfmt && fmt[i] != '\0') 
		    x[i] = ((long)fmt[i] << 22) | i;
		else
		{
		    endfmt = TRUE;
		    x[i] = ((long)'z' << 22) | i;
		}
	    }
            lsort(x,n);

            for (i = 0; i < n; ++i)
            {
                lab[i] = x[i] & 0x3fffffL;
                if (i == n-1)
                    ptn[i] = 0;
                else if ((x[i+1] >> 22) != (x[i] >> 22))
                {
                    ++numcells;
                    ADDELEMENT(active,i+1);
                    ptn[i] = 0;
                }
                else
                    ptn[i] = 1;
            }
	}
	else
	{
	    for (i = 0; i < n; ++i)
	    {
		lab[i] = i;
		ptn[i] = 1;
	    }
	    ptn[n-1] = 0;
	}

        refine_sg((graph*)g,lab,ptn,0,&numcells,count,active,&code,1,n);

        if (numcells == n || !digraph && numcells == n-1)
        {
            for (i = 0; i < n; ++i) count[i] = lab[i];
            updatecan_sg((graph*)g,(graph*)h,count,0,m,n);
	    gt_numorbits = numcells;
        }
        else
        {
            options.getcanon = TRUE;
            options.digraph = digraph;
            options.defaultptn = FALSE;
	    if (invarproc)
	    {
		options.invarproc = invarproc;
		options.mininvarlevel = mininvarlevel;
		options.maxinvarlevel = maxinvarlevel;
		options.invararg = invararg;
	    }
#ifdef REFINE
	    options.userrefproc = REFINE;
#endif

            EMPTYSET(active,m);
            nauty((graph*)g,lab,ptn,active,orbits,&options,&stats,
                                             workspace,4*m,m,n,(graph*)h);
	    gt_numorbits = stats.numorbits;
        }
}

/**************************************************************************/

void
fgroup(graph *g, int m, int n, char *fmt, int *orbits, int *numorbits)  
/* Find the orbits of undirected graph g stabilised by format fmt.
   The orbits are put into orbits[] and the number of them into *numorbits
   fmt is either NULL (for no vertex classification) or is a string
   with char-valued colours for the vertices.  If it ends early, it
   is assumed to continue with the colour 'z' indefinitely. */
{
#if MAXN
        int lab[MAXN],ptn[MAXN];
        long x[MAXN];
        permutation count[MAXN];
        set active[MAXM];
        setword workspace[4*MAXM];
#else
	DYNALLSTAT(int,lab,lab_sz);
	DYNALLSTAT(int,ptn,ptn_sz);
	DYNALLSTAT(long,x,x_sz);
	DYNALLSTAT(permutation,count,count_sz);
	DYNALLSTAT(set,active,active_sz);
	DYNALLSTAT(setword,workspace,workspace_sz);
#endif
        int i,j;
	int orbrep;
	boolean endfmt;
        int numcells,code;
        statsblk stats;
        static DEFAULTOPTIONS_GRAPH(options);

#if MAXN
	if (n > MAXN || m > MAXM)
	{
	    fprintf(stderr,">E fcanonise: m or n too large\n");
	    ABORT(">E fcanonise");
	}
#else
	DYNALLOC1(int,lab,lab_sz,n,"fcanonise");
	DYNALLOC1(int,ptn,ptn_sz,n,"fcanonise");
	DYNALLOC1(long,x,x_sz,n,"fcanonise");
	DYNALLOC1(permutation,count,count_sz,n,"fcanonise");
	DYNALLOC1(set,active,active_sz,m,"fcanonise");
	DYNALLOC1(setword,workspace,workspace_sz,4*m,"fcanonise");
#endif

        EMPTYSET(active,m);
        ADDELEMENT(active,0);
        numcells = 1;

	if (fmt != NULL)
	{
	    endfmt = FALSE;
            for (i = 0; i < n; ++i)
	    {
		if (!endfmt && fmt[i] != '\0') 
		    x[i] = ((long)fmt[i] << 22) | i;
		else
		{
		    endfmt = TRUE;
		    x[i] = ((long)'z' << 22) | i;
		}
	    }
            lsort(x,n);

            for (i = 0; i < n; ++i)
            {
                lab[i] = x[i] & 0xffff;
                if (i == n-1) ptn[i] = 0;
                else if ((x[i+1] >> 22) != (x[i] >> 22))
                {
                    ++numcells;
                    ADDELEMENT(active,i+1);
                    ptn[i] = 0;
                }
                else
                    ptn[i] = 1;
            }
	}
	else
	{
	    for (i = 0; i < n; ++i)
	    {
		lab[i] = i;
		ptn[i] = 1;
	    }
	    ptn[n-1] = 0;
	}

	if (m == 1)
            refine1(g,lab,ptn,0,&numcells,count,active,&code,1,n);
	else
            refine(g,lab,ptn,0,&numcells,count,active,&code,m,n);

        if (cheapautom(ptn,0,FALSE,n))
        {
	    for (i = 0; i < n; )
	    {
		if (ptn[i] == 0)
		{
		    orbits[lab[i]] = lab[i];
		    ++i;
		}
		else
		{
		    orbrep = n;
		    j = i;
		    do
		    {
		        if (lab[j] < orbrep) orbrep = lab[j];
		    } while (ptn[j++] != 0);

		    for (; i < j; ++i) orbits[lab[i]] = orbrep;
		}
	    }
	    *numorbits = gt_numorbits = numcells;
        }
        else
        {
            options.getcanon = FALSE;
            options.defaultptn = FALSE;
#ifdef REFINE
	    options.userrefproc = REFINE;
#endif

            EMPTYSET(active,m);
            nauty(g,lab,ptn,active,orbits,&options,&stats,
                                             workspace,4*m,m,n,NULL);
	    *numorbits = gt_numorbits = stats.numorbits;
        }
}

/**************************************************************************/

void
fgroup_inv(graph *g, int m, int n, char *fmt, int *orbits, int *numorbits,
      void (*invarproc)(graph*,int*,int*,int,int,int,permutation*,int,
       boolean,int,int), int mininvarlevel, int maxinvarlevel, int invararg)  
/* Find the orbits of undirected graph g stabilised by format fmt.
   The orbits are put into orbits[] and the number of them into *numorbits
   fmt is either NULL (for no vertex classification) or is a string
   with char-valued colours for the vertices.  If it ends early, it
   is assumed to continue with the colour 'z' indefinitely.
   This is like fgroup() except that a invariant and its arguments
   can be specified. */
{
#if MAXN
        int lab[MAXN],ptn[MAXN];
        long x[MAXN];
        permutation count[MAXN];
        set active[MAXM];
        setword workspace[4*MAXM];
#else
	DYNALLSTAT(int,lab,lab_sz);
	DYNALLSTAT(int,ptn,ptn_sz);
	DYNALLSTAT(long,x,x_sz);
	DYNALLSTAT(permutation,count,count_sz);
	DYNALLSTAT(set,active,active_sz);
	DYNALLSTAT(setword,workspace,workspace_sz);
#endif
        int i,j;
	int orbrep;
	boolean endfmt;
        int numcells,code;
        statsblk stats;
        static DEFAULTOPTIONS_GRAPH(options);

#if MAXN
	if (n > MAXN || m > MAXM)
	{
	    fprintf(stderr,">E fcanonise: m or n too large\n");
	    ABORT(">E fcanonise");
	}
#else
	DYNALLOC1(int,lab,lab_sz,n,"fcanonise");
	DYNALLOC1(int,ptn,ptn_sz,n,"fcanonise");
	DYNALLOC1(long,x,x_sz,n,"fcanonise");
	DYNALLOC1(permutation,count,count_sz,n,"fcanonise");
	DYNALLOC1(set,active,active_sz,m,"fcanonise");
	DYNALLOC1(setword,workspace,workspace_sz,4*m,"fcanonise");
#endif

        EMPTYSET(active,m);
        ADDELEMENT(active,0);
        numcells = 1;

	if (fmt != NULL)
	{
	    endfmt = FALSE;
            for (i = 0; i < n; ++i)
	    {
		if (!endfmt && fmt[i] != '\0') 
		    x[i] = ((long)fmt[i] << 22) | i;
		else
		{
		    endfmt = TRUE;
		    x[i] = ((long)'z' << 22) | i;
		}
	    }
            lsort(x,n);

            for (i = 0; i < n; ++i)
            {
                lab[i] = x[i] & 0xffff;
                if (i == n-1) ptn[i] = 0;
                else if ((x[i+1] >> 22) != (x[i] >> 22))
                {
                    ++numcells;
                    ADDELEMENT(active,i+1);
                    ptn[i] = 0;
                }
                else
                    ptn[i] = 1;
            }
	}
	else
	{
	    for (i = 0; i < n; ++i)
	    {
		lab[i] = i;
		ptn[i] = 1;
	    }
	    ptn[n-1] = 0;
	}

	if (m == 1)
            refine1(g,lab,ptn,0,&numcells,count,active,&code,1,n);
	else
            refine(g,lab,ptn,0,&numcells,count,active,&code,m,n);

        if (cheapautom(ptn,0,FALSE,n))
        {
	    for (i = 0; i < n; )
	    {
		if (ptn[i] == 0)
		{
		    orbits[lab[i]] = lab[i];
		    ++i;
		}
		else
		{
		    orbrep = n;
		    j = i;
		    do
		    {
		        if (lab[j] < orbrep) orbrep = lab[j];
		    } while (ptn[j++] != 0);

		    for (; i < j; ++i) orbits[lab[i]] = orbrep;
		}
	    }
	    *numorbits = gt_numorbits = numcells;
        }
        else
        {
            options.getcanon = FALSE;
            options.defaultptn = FALSE;
            if (invarproc)
            {
                options.invarproc = invarproc;
                options.mininvarlevel = mininvarlevel;
                options.maxinvarlevel = maxinvarlevel;
                options.invararg = invararg;
            }
#ifdef REFINE
	    options.userrefproc = REFINE;
#endif

            EMPTYSET(active,m);
            nauty(g,lab,ptn,active,orbits,&options,&stats,
                                             workspace,4*m,m,n,NULL);
	    *numorbits = gt_numorbits = stats.numorbits;
        }
}

/**************************************************************************/

static void
userlevel(int *lab, int *ptn, int level, int *orbits, statsblk *stats,
          int tv, int index, int tcellsize, int numcells, int cc, int n)
{
	int i0,i;

	if (level != 2) return;

	issymm = TRUE;

	i0 = nextelement(g0,gm,-1);
	if (i0 >= 0)
	    for (i = i0; (i = nextelement(g0,gm,i)) >= 0;)
		if (orbits[i] != i0)
		{
		    issymm = FALSE;
		    return;
		}
}
 
/*******************************************************************/

/* istransitive(g,m,n,h)

   g   is an input graph with
   m,n of standard meaning.  
   h   is a place to put an output graph.

   If g is transitive, return 1 or 2 and put a canonically labelled
       version of g into h.  The value is 2 for symmetric graphs, 
       and 1 for other transitive graphs.
   If g is not transitive, return 0.  In that case h may or 
       may not have something in it.
*/

int
istransitive(graph *g, int m, int n, graph *h)
{
        int i,inv;
        set *gw;
        short wt;
        int d,inv0,v,w;
        statsblk stats; 
        static DEFAULTOPTIONS_GRAPH(options);
#if MAXN
        int lab[MAXN],ptn[MAXN],orbits[MAXN];
        long x[MAXN];
        permutation count[MAXN];
        setword workspace[4*MAXM];
        set workset[MAXM];
        set sofar[MAXM],frontier[MAXM];
#else
	DYNALLSTAT(int,lab,lab_sz);
	DYNALLSTAT(int,ptn,ptn_sz);
	DYNALLSTAT(int,orbits,orbits_sz);
	DYNALLSTAT(long,x,x_sz);
	DYNALLSTAT(permutation,count,count_sz);
	DYNALLSTAT(setword,workspace,workspace_sz);
	DYNALLSTAT(set,workset,workset_sz);
	DYNALLSTAT(set,sofar,sofar_sz);
	DYNALLSTAT(set,frontier,frontier_sz);
#endif

#if MAXN
	if (m > MAXM || n > MAXN)
	{
	    fprintf(stderr,
                ">E istransitive: bad input parameters (n=%d m=%d)\n",n,m);
	    exit(1);
	}
#else
	DYNALLOC1(int,lab,lab_sz,n,"istransitive");
	DYNALLOC1(int,ptn,ptn_sz,n,"istransitive");
	DYNALLOC1(int,orbits,orbits_sz,n,"istransitive");
	DYNALLOC1(long,x,x_sz,n,"istransitive");
	DYNALLOC1(permutation,count,count_sz,n,"istransitive");
	DYNALLOC1(setword,workspace,workspace_sz,4*m,"istransitive");
	DYNALLOC1(set,workset,workset_sz,m,"istransitive");
	DYNALLOC1(set,sofar,sofar_sz,m,"istransitive");
	DYNALLOC1(set,frontier,frontier_sz,m,"istransitive");
#endif

	for (v = 0; v < n; ++v)
        {
            inv = 0;
            EMPTYSET(sofar,m);
            ADDELEMENT(sofar,v);
            EMPTYSET(frontier,m);
            ADDELEMENT(frontier,v);
            for (d = 1; d < n; ++d)
            {
                EMPTYSET(workset,m);
                wt = 0;
                for (w = -1; (w = nextelement(frontier,m,w)) >= 0;)
                {
                    ++wt;
                    gw = GRAPHROW(g,w,m);
                    for (i = m; --i >= 0;) workset[i] |= gw[i];
                }
                if (wt == 0) break;
                wt += 0x73 ^ d;
                wt = FUZZ2(wt);
                inv += wt;
                for (i = m; --i >= 0;)
                {
                    frontier[i] = workset[i] & ~sofar[i];
                    sofar[i] |= frontier[i];
                }
            }
            if (v == 0) inv0 = inv;
            else if (inv != inv0) return 0;
        }

	options.getcanon = TRUE;
	options.userlevelproc = userlevel;
#ifdef REFINE
	options.userrefproc = REFINE;
#endif
 
	issymm = TRUE;
	g0 = (set*) g;
	gm = m;

        nauty(g,lab,ptn,NULL,orbits,&options,&stats,workspace,4*m,m,n,h);

	if (stats.numorbits != 1) return 0;
	else if (!issymm)         return 1;
	else                      return 2;
}

/**************************************************************************/

void 
tg_canonise(graph *g, graph *h, int m, int n)
/* Canonise vertex-transitive graph */
{
        int i;
#if MAXN
        int lab[MAXN],ptn[MAXN],orbits[MAXN];
        set active[MAXM];
        setword workspace[4*MAXM];
#else
	DYNALLSTAT(int,lab,lab_sz);
	DYNALLSTAT(int,ptn,ptn_sz);
	DYNALLSTAT(int,orbits,orbits_sz);
	DYNALLSTAT(set,active,active_sz);
	DYNALLSTAT(setword,workspace,workspace_sz);
#endif
        statsblk stats;
        static DEFAULTOPTIONS_GRAPH(options);

#if MAXN
	if (n > MAXN || m > MAXM)
	{
	    fprintf(stderr,">E tg_canonise: m or n too large\n");
	    ABORT(">E tg_canonise");
	}
#else
	DYNALLOC1(int,lab,lab_sz,n,"tg_canonise");
	DYNALLOC1(int,ptn,ptn_sz,n,"tg_canonise");
	DYNALLOC1(int,orbits,orbits_sz,n,"tg_canonise");
	DYNALLOC1(set,active,active_sz,m,"tg_canonise");
	DYNALLOC1(setword,workspace,workspace_sz,4*m,"tg_canonise");
#endif

        options.getcanon = TRUE;
        options.defaultptn = FALSE;
#ifdef REFINE
	options.userrefproc = REFINE;
#endif

        for (i = 0; i < n; ++i)
        {
            lab[i] = i;
            ptn[i] = 1;
        }
        ptn[0] = ptn[n-1] = 0;

        EMPTYSET(active,m);
        ADDELEMENT(active,0);

        nauty(g,lab,ptn,active,orbits,&options,&stats,workspace,4*m,m,n,h);
}
