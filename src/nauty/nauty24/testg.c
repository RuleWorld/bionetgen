/* testg.c : Find properties of graphs.  This is the source file for
   both pickg (select by property) and countg (count by property).
   Version of Nov 19, 2003. */
/* TODO - write a header if input has one */
/* TODO - USERDEF should be long, not int */

#define USAGE \
  "[pickg|countg] [-fp#:#q -V] [--keys] [-constraints -v] [ifile [ofile]]"

#define HELPTEXT \
" countg : Count graphs according to their properties.\n\
  pickg : Select graphs according to their properties.\n\
\n\
  ifile, ofile : Input and output files.\n\
        '-' and missing names imply stdin and stdout.\n\
\n\
  Miscellaneous switches:\n\
     -p# -p#:#   Specify range of input lines (first is 1)\n\
     -f          With -p, assume input lines of fixed length\n\
                        (only used with a file in graph6 format)\n\
     -v          Negate all constraints\n\
     -V          List properties of every input matching constraints.\n\
     -q          Suppress informative output.\n\
\n\
  Constraints:\n\
     Numerical constraints (shown here with following #) can take\n\
     a single integer value, or a range like #:#, #:, or :#.  Each\n\
     can also be preceded by '~', which negates it.   (For example,\n\
     -~D2:4 will match any maximum degree which is _not_ 2, 3, or 4.)\n\
     Constraints are applied to all input graphs, and only those\n\
     which match all constraints are counted or selected.\n\
\n\
     -n#  number of vertices     -e#  number of edges\n\
     -d#  minimum degree         -D#  maximum degree\n\
     -r   regular                -b   bipartite\n\
     -z#  radius                 -Z#  diameter\n\
     -g#  girth (0=acyclic)      -Y#  total number of cycles\n\
     -T#  number of triangles\n\
     -E   Eulerian (all degrees are even, connectivity not required)\n\
     -a#  group size  -o# orbits  -F# fixed points  -t vertex-transitive\n\
     -c#  connectivity (only implemented for 0,1,2).\n\
     -i#  min common nbrs of adjacent vertices;     -I# maximum\n\
     -j#  min common nbrs of non-adjacent vertices; -J# maximum\n\
\n\
  Sort keys:\n\
     Counts are made for all graphs passing the constraints.  Counts\n\
     are given separately for each combination of values occuring for\n\
     the properties listed as sort keys.  A sort key is introduced by\n\
     '--' and uses one of the letters known as constraints.  These can\n\
     be combined:  --n --e  --r  is the same as --ne --r and --ner.\n\
     The order of sort keys is significant.\n"


#include "gtools.h"
#include "gutils.h"

/*
How to add a new property:

 1. Add entries to constraint[], following the examples there.
    If several things are computed at the same time, link them
    together such as for z and Z.  It doesn't matter which is
    first provided the prereq field points to the first one.
   
 2. Add code to compute() to compute the value(s) of the parameter.
    Probably this means calling an external procedure then setting
    some VAL() and COMPUTED() values.

 3. Update HELPTEXT.

External user-defined parameters:
  A general integer-valued parameter can be compiled into this program
  if USERDEF is defined as the function name at compile time.  In this
  case the parameter is selected using the letter 'Q'.  The name of the
  parameter is "userdef" unless USERDEFNAME is defined.  The function
  is called with the parameters (graph *g, int m, int n) and must return
  an integer value.
*/

#ifdef USERDEF
int USERDEF(graph*,int,int);
#endif
#ifndef USERDEFNAME
#define USERDEFNAME "userdef"
#endif

/**********************************************************************/

#define BOOLTYPE 0
#define INTTYPE 1
#define GROUPSIZE 2
#define INTVECTOR 3

#undef CMASK
#define CMASK(i) (1L << (i))

static struct constraint_st    /* Table of Constraints */
{
    char symbol;
    int needed;     /* 1 = sortkey, 2 = constraint; 3 = both */
    boolean computed;
    boolean inverse;
    unsigned long prereq;    /* Must be earlier, must be <= bits in long */
    long lo,hi;
    char *id;
    int valtype;
    long val;      /* Might be a cast pointer */
} constraint[] = {
#define I_n 0
   {'n',0,FALSE,FALSE,0,-NOLIMIT,NOLIMIT,"n",INTTYPE,0}, /* always known */
#define I_e 1
   {'e',0,FALSE,FALSE,0,-NOLIMIT,NOLIMIT,"e",INTTYPE,0},
#define I_d 2
   {'d',0,FALSE,FALSE,CMASK(I_e),-NOLIMIT,NOLIMIT,"mindeg",INTTYPE,0},
#define I_D 3
   {'D',0,FALSE,FALSE,CMASK(I_e),-NOLIMIT,NOLIMIT,"maxdeg",INTTYPE,0},
#define I_E 4
   {'E',0,FALSE,FALSE,CMASK(I_e),-NOLIMIT,NOLIMIT,"eulerian",BOOLTYPE,0},
#define I_r 5
   {'r',0,FALSE,FALSE,CMASK(I_e),-NOLIMIT,NOLIMIT,"regular",BOOLTYPE,0},
#define I_b 6
   {'b',0,FALSE,FALSE,0,-NOLIMIT,NOLIMIT,"bipartite",BOOLTYPE,0},
#define I_z 7
   {'z',0,FALSE,FALSE,0,-NOLIMIT,NOLIMIT,"radius",INTTYPE,0},
#define I_Z 8
   {'Z',0,FALSE,FALSE,CMASK(I_z),-NOLIMIT,NOLIMIT,"diameter",INTTYPE,0},
#define I_a 9
   {'a',0,FALSE,FALSE,0,-NOLIMIT,NOLIMIT,"groupsize",GROUPSIZE,0},
#define I_o 10
   {'o',0,FALSE,FALSE,CMASK(I_a),-NOLIMIT,NOLIMIT,"orbits",INTTYPE,0},
#define I_t 11
   {'t',0,FALSE,FALSE,CMASK(I_o),-NOLIMIT,NOLIMIT,"transitive",BOOLTYPE,0},
#define I_c 12
   {'c',0,FALSE,FALSE,0,-NOLIMIT,NOLIMIT,"connectivity",INTTYPE,0},
#define I_F 13
   {'F',0,FALSE,FALSE,CMASK(I_a),-NOLIMIT,NOLIMIT,"fixedpts",INTTYPE,0},
#define I_g 14
   {'g',0,FALSE,FALSE,0,-NOLIMIT,NOLIMIT,"girth",INTTYPE,0},
#define I_Y 15
   {'Y',0,FALSE,FALSE,0,-NOLIMIT,NOLIMIT,"cycles",INTTYPE,0},
#define I_i 16
   {'i',0,FALSE,FALSE,0,-NOLIMIT,NOLIMIT,"minadjcn",INTTYPE,0},
#define I_I 17
   {'I',0,FALSE,FALSE,CMASK(I_i),-NOLIMIT,NOLIMIT,"maxadjcn",INTTYPE,0},
#define I_j 18
   {'j',0,FALSE,FALSE,CMASK(I_i),-NOLIMIT,NOLIMIT,"minnoncn",INTTYPE,0},
#define I_J 19
   {'J',0,FALSE,FALSE,CMASK(I_i),-NOLIMIT,NOLIMIT,"maxnoncn",INTTYPE,0},
#define I_T 20
   {'T',0,FALSE,FALSE,0,-NOLIMIT,NOLIMIT,"triang",INTTYPE,0},
#define I_Q 21
#ifdef USERDEF
   {'Q',0,FALSE,FALSE,0,-NOLIMIT,NOLIMIT,USERDEFNAME,INTTYPE,0}
#else
   {' ',0,FALSE,FALSE,0,-NOLIMIT,NOLIMIT,USERDEFNAME,INTTYPE,0}
#endif
};

#define NUMCONSTRAINTS (sizeof(constraint)/sizeof(struct constraint_st))
#define SYMBOL(i) (constraint[i].symbol)
#define ISNEEDED(i) (constraint[i].needed > 0)
#define NEEDED(i) (constraint[i].needed)
#define ISKEY(i) ((constraint[i].needed & 1) != 0)
#define ISCONSTRAINT(i) (constraint[i].needed > 1)
#define INVERSE(i) (constraint[i].inverse)
#define COMPUTED(i) (constraint[i].computed)
#define PREREQ(i) (constraint[i].prereq)
#define LO(i) (constraint[i].lo)
#define HI(i) (constraint[i].hi)
#define VAL(i) (constraint[i].val)
#define VALTYPE(i) (constraint[i].valtype)
#define ID(i) (constraint[i].id)

#define INBOUNDS0(i) ((LO(i) == -NOLIMIT || VAL(i) >= LO(i)) \
		  && (HI(i) == NOLIMIT || VAL(i) <= HI(i)))
#define INBOUNDS(i) (VALTYPE(i) == GROUPSIZE \
	      ? group_in_range((group_node*)VAL(i),LO(i),HI(i)) \
              : INBOUNDS0(i))

static boolean docount,dofilter;

#define MAXKEYS 20 /* Maximum number of keys to sort by */

/* splay_st is the generic structure of a splay tree node.  The
   data[] field has varying lengths according to need.  This program
   uses two splay trees: one for counts and one for large data items.
*/

typedef struct splay_st
{
    struct splay_st *left,*right,*parent;
    long data[1];
} splay_node;

typedef struct node_st     /* variant for count tree */
{
    struct splay_st *left,*right,*parent;
    unsigned long count;
    long val[MAXKEYS];
} count_node;

typedef struct value_st    /* variant for value tree */
{
    struct splay_st *left,*right,*parent;
    size_t size;
    long data[1];
} value_node;

#define TOSPLAY(p) ((SPLAYNODE*)(p))
#define TOVALUE(p) ((value_node*)(p))
#define TOCOUNT(p) ((count_node*)(p))
#define SPLAYNODE splay_node
#define SPLAYNODESIZE new_val_sz
#define SCAN_ARGS , FILE *f
#define ACTION(p) {fprintf(f,"%9ld graphs : ",TOCOUNT(p)->count); \
                   printkeyvals(f,TOCOUNT(p)->val); fprintf(f,"\n");}
#define INSERT_ARGS , boolean isvalue, SPLAYNODE *new_val, size_t new_val_sz
#define COMPARE(p) (isvalue ? \
              compare_value_node(TOVALUE(new_val),TOVALUE(p)) \
            : compare_count_node(TOCOUNT(new_val),TOCOUNT(p)))
#define PRESENT(p) {if (!isvalue) ++TOCOUNT(p)->count;}
#define NOT_PRESENT(p) {memcpy((void*)p,(void*)new_val,SPLAYNODESIZE); \
                    if (!isvalue) TOCOUNT(p)->count = 1;}

static void printkeyvals(FILE*,long*);
static int compare_count_node(count_node*,count_node*);
static int compare_value_node(value_node*,value_node*);

static splay_node *count_root = NULL;
static splay_node *value_root = NULL;
static int key[MAXKEYS];
static int numkeys;

#include "splay.c"     /* Procedures for splay tree management */

typedef struct grpsize_st
{
    struct splay_st *left,*right,*parent;
    size_t size;
    double groupsize1;
    long groupsize2;
} group_node;

/**********************************************************************/

static int
compare_count_node(count_node *a, count_node *b)
/* Usual type of comparison */
{
	int i;
	group_node *sza,*szb;

	for (i = 0; i < numkeys; ++i)
	{
	    if (VALTYPE(key[i]) == GROUPSIZE)
	    {
		sza = (group_node*)a->val[i];
		szb = (group_node*)b->val[i];
		if      (sza->groupsize2 < szb->groupsize2) return -1;
		else if (sza->groupsize2 > szb->groupsize2) return 1;
		else if (sza->groupsize1 < szb->groupsize1) return -1;
		else if (sza->groupsize1 > szb->groupsize1) return 1;
	    }
	    else if (a->val[i] < b->val[i]) return -1;
	    else if (a->val[i] > b->val[i]) return 1;
	}

	return 0;
}

/**********************************************************************/

static int
compare_value_node(value_node *a, value_node *b)
/* Usual type of comparison */
{
	size_t minsize;
	int cmp;

	if (a->size < b->size) minsize = a->size;
	else                   minsize = b->size;
	cmp = memcmp(a->data,b->data,minsize);
	if (cmp != 0) return cmp;

	if      (a->size < minsize) return -1;
	else if (a->size > minsize) return 1;
	else                        return 0;
}

/**********************************************************************/

static void
write_group_size(FILE *f, group_node *sz)
{
	double sz1;
	int sz2;

	sz1 = sz->groupsize1;
	sz2 = sz->groupsize2;

        if (sz2 == 0)
            fprintf(f,"%.0f",sz1+0.1);
        else
        {
            while (sz1 >= 10.0)
            {
                sz1 /= 10.0;
                ++sz2;
            }
            fprintf(f,"%12.10fe%d",sz1,sz2);
        }
}

/**********************************************************************/

static void
add_one(void)
/* Add current graph to count. */
{
	int i;
	count_node new_val;

	for (i = 0; i < numkeys; ++i)
	    new_val.val[i] = VAL(key[i]);

        splay_insert(&count_root,FALSE,TOSPLAY(&new_val),
	    	     sizeof(splay_node)+numkeys*sizeof(long));
}

/**********************************************************************/

static void
printthesevals(FILE *f)
{       
        int i,ki;
        
        for (i = 0; i < numkeys; ++i)
        {   
            ki = key[i];
            if (i > 0) fprintf(f,"; ");
            
            if (VALTYPE(ki) == BOOLTYPE)
	    {
                if (!VAL(ki)) fprintf(f,"not %s",ID(ki));
                else          fprintf(f,ID(ki));
	    }
	    else if (VALTYPE(ki) == GROUPSIZE)
	    {
		fprintf(f,"%s=",ID(ki));
		write_group_size(f,(group_node*)VAL(ki));
	    }
            else
                fprintf(f,"%s=%ld",ID(ki),VAL(ki));
        }
}

/**********************************************************************/

static void
printkeyvals(FILE *f, long *val)
{
	int i,ki;

	for (i = 0; i < numkeys; ++i)
	{
	    ki = key[i];
	    if (i > 0) fprintf(f,"; ");

	    if (VALTYPE(ki) == BOOLTYPE)
	    {
		if (!val[i]) fprintf(f,"not %s",ID(ki));
		else         fprintf(f,ID(ki));
	    }
	    else if (VALTYPE(ki) == GROUPSIZE)
	    {
		fprintf(f,"%s=",ID(ki));
		write_group_size(f,(group_node*)val[i]);
	    }
	    else
		fprintf(f,"%s=%ld",ID(ki),val[i]);
	}
}

/**********************************************************************/

static void
groupstats(graph *g, int m, int n, group_node *sz,
           int *numorbits, int *fixedpts)  
/* Find the automorphism group of the undirected graph g.
   Return the group size and number of orbits and fixed points. */
{
#if MAXN
        int lab[MAXN],ptn[MAXN],orbits[MAXN];
        permutation count[MAXN];
        set active[MAXM];
        setword workspace[4*MAXM];
#else
	DYNALLSTAT(int,lab,lab_sz);
	DYNALLSTAT(int,ptn,ptn_sz);
	DYNALLSTAT(int,orbits,orbits_sz);
	DYNALLSTAT(permutation,count,count_sz);
	DYNALLSTAT(set,active,active_sz);
	DYNALLSTAT(setword,workspace,workspace_sz);
#endif
        int i;
	int fixed;
        int numcells,code;
        statsblk stats;
        static DEFAULTOPTIONS_GRAPH(options);

#if !MAXN
	DYNALLOC1(int,lab,lab_sz,n,"groupstats");
	DYNALLOC1(int,ptn,ptn_sz,n,"groupstats");
	DYNALLOC1(int,orbits,orbits_sz,n,"groupstats");
	DYNALLOC1(permutation,count,count_sz,n,"groupstats");
	DYNALLOC1(set,active,active_sz,m,"groupstats");
	DYNALLOC1(setword,workspace,workspace_sz,4*m,"groupstats");
#endif

        EMPTYSET(active,m);
        ADDELEMENT(active,0);
        numcells = 1;

	for (i = 0; i < n; ++i)
	{
	    lab[i] = i;
	    ptn[i] = 1;
	}
	ptn[n-1] = 0;

	if (m == 1)
            refine1(g,lab,ptn,0,&numcells,count,active,&code,1,n);
	else
            refine(g,lab,ptn,0,&numcells,count,active,&code,m,n);

        if (numcells >= n-1)
        {
	    *numorbits = numcells;
	    *fixedpts = (numcells == n ? n : n-2);
	    sz->groupsize1 = n + 1.0 - numcells; 
	    sz->groupsize2 = 0;
        }
        else
        {
            options.getcanon = FALSE;
            options.defaultptn = FALSE;

            EMPTYSET(active,m);
            nauty(g,lab,ptn,active,orbits,&options,&stats,
                                             workspace,4*m,m,n,NULL);
	    *numorbits = stats.numorbits;
	    sz->groupsize1 = stats.grpsize1;
	    sz->groupsize2 = stats.grpsize2;
	    for (i = 0; i < n; ++i) count[i] = 0;
	    fixed = stats.numorbits;
	    for (i = 0; i < n; ++i)
		if (++count[orbits[i]] == 2) --fixed;
	    *fixedpts = fixed;
        }
}

/**********************************************************************/

static void
compute(graph *g, int m, int n, int code)
/* Compute property i assuming the prerequisites are known. */
{
	int mind,maxd,mincount,maxcount;
	int rad,diam;
	unsigned long ned;
	boolean eul;
	group_node sz;
	int norbs,fixedpts;
	int minadj,maxadj,minnon,maxnon;

	switch (code)
	{
	    case I_e:
		degstats(g,m,n,&ned,&mind,&mincount,&maxd,&maxcount,&eul);
		VAL(I_e) = ned;
		VAL(I_d) = mind;
		VAL(I_D) = maxd;
		VAL(I_E) = eul;
		VAL(I_r) = mind == maxd;
		COMPUTED(I_e) = COMPUTED(I_d) = COMPUTED(I_D) = TRUE;
	        COMPUTED(I_E) = COMPUTED(I_r) = TRUE;
		break;

	    case I_b:
		VAL(I_b) = isbipartite(g,m,n);
		COMPUTED(I_b) = TRUE;
		break;

	    case I_g:
		VAL(I_g) = girth(g,m,n);
		COMPUTED(I_g) = TRUE;
		break;

	    case I_z:
	    case I_Z:
		diamstats(g,m,n,&rad,&diam);
		VAL(I_z) = rad;
		VAL(I_Z) = diam;
		COMPUTED(I_z) = COMPUTED(I_Z) = TRUE;
		break;		

	    case I_a:
		groupstats(g,m,n,&sz,&norbs,&fixedpts);
		sz.size = sizeof(long) + sizeof(double);
		splay_insert(&value_root,TRUE,TOSPLAY(&sz),sizeof(group_node));
		VAL(I_a) = (long)value_root;
		VAL(I_o) = norbs;
		VAL(I_t) = norbs == 1;
		VAL(I_F) = fixedpts;
		COMPUTED(I_a) = COMPUTED(I_o) = TRUE;
		COMPUTED(I_F) = COMPUTED(I_t) = TRUE;
	        break;

	    case I_c:
		if (isbiconnected(g,m,n)) VAL(I_c) = 2;
		else if (isconnected(g,m,n)) VAL(I_c) = 1;
		else VAL(I_c) = 0;
		COMPUTED(I_c) = TRUE;
		break;

	    case I_n:
	    case I_d:
	    case I_D:
	    case I_E:
	    case I_r:
	    case I_o:
	    case I_t:
		fprintf(stderr,">E Property %d should be known already\n",code);
		exit(1);

	    case I_Y:
		VAL(I_Y) = cyclecount(g,m,n);
		COMPUTED(I_Y) = TRUE;
		break;

	    case I_T:
		VAL(I_T) = numtriangles(g,m,n);
		COMPUTED(I_T) = TRUE;
		break;

	    case I_i:
	    case I_I:
	    case I_j:
	    case I_J:
		commonnbrs(g,&minadj,&maxadj,&minnon,&maxnon,m,n);
		VAL(I_i) = minadj;
		VAL(I_I) = maxadj;
		VAL(I_j) = minnon;
		VAL(I_J) = maxnon;
		COMPUTED(I_i) = COMPUTED(I_I) = TRUE;
                COMPUTED(I_j) = COMPUTED(I_J) = TRUE;
                break;

#ifdef USERDEF
	    case I_Q:
		VAL(I_Q) = USERDEF(g,m,n);
		COMPUTED(I_Q) = TRUE;
		break;
#endif

	    default:
		fprintf(stderr,">E Property %d is uncomputable\n",code);
		exit(1);
	}
}

/**********************************************************************/

static boolean
group_in_range(group_node *sz, long lo, long hi)
/* Test if the group size is in the given range */
{
	double sz1;
	int sz2;

	if (lo != -NOLIMIT)
	{
	    sz1 = sz->groupsize1;
	    sz2 = sz->groupsize2;

            while (sz2 >= 0 && sz1 < lo)
	    {
		--sz2;
		sz1 *= 10.0;
	    }
	    if (sz2 < 0) return FALSE;
	}

	if (hi != NOLIMIT)
	{
	    sz1 = sz->groupsize1;
	    sz2 = sz->groupsize2;
	   
	    while (sz2 >= 0 && sz1 <= hi)
	    {
		--sz2;
		sz1 *= 10.0;
	    }
	    if (sz2 >= 0) return FALSE;
	}

	return TRUE;
}

/**********************************************************************/

static boolean
selected(graph *g, int m, int n)
/* See if g is selected by the constraints */
{
	int i;

	VAL(I_n) = n;
	COMPUTED(I_n) = TRUE;

	for (i = 0; i < NUMCONSTRAINTS; ++i)
	if (ISNEEDED(i))
	{
	    if (!COMPUTED(i)) compute(g,m,n,i);

	    if (ISCONSTRAINT(i))
	    {
	        if (INBOUNDS(i))
	        {
		    if (INVERSE(i)) return FALSE;
	        }
	        else
	        {
		    if (!INVERSE(i)) return FALSE;
	        }
	    }
	}

	return TRUE;
}

/**********************************************************************/

static void
decodekeys(char *s)
/* Extract key symbols from -- string */
{
	int i,j,k;

	for (i = 0; s[i] != '\0'; ++i)
	{
	    for (j = 0; j < NUMCONSTRAINTS; ++j)
		if (s[i] == SYMBOL(j)) break;
	    if (j == NUMCONSTRAINTS)
	    {
		fprintf(stderr,">E unknown sort key %c\n",s[i]);
		exit(1);
	    }

	    for (k = 0; k < numkeys; ++k) if (key[k] == j) break;

	    if (k == numkeys)
	    {
		if (numkeys == MAXKEYS)
		{
		    fprintf(stderr,
			    ">E too many sort keys, increase MAXKEYS\n");
		    exit(1);
		}
		key[numkeys++] = j;
		NEEDED(j) |= 1;
	    }
	}
}

/**********************************************************************/

int
main(int argc, char *argv[])
{
	graph *g;
	int m,n,codetype;
	char *infilename,*outfilename;
	FILE *infile,*outfile,*countfile;
	int outcode;
	long nin,nout;
	int argnum,i,j;
	char *arg,sw,*baseptr,*bp;
	boolean badargs;
	long pval1,pval2,maxin;
	boolean fswitch,pswitch,Vswitch,vswitch,qswitch;
	unsigned long cmask;
	boolean havecon,neg;
	double t;

	HELP;

	if (sizeof(void*) > sizeof(long))
	{
	    fprintf(stderr,">E %s cannot run on this machine.\n",argv[0]);
	    exit(1);
	}
	vswitch = qswitch = fswitch = pswitch = FALSE;
	Vswitch = FALSE;
	infilename = outfilename = NULL;
	numkeys = 0;
	havecon = FALSE;

	baseptr = argv[0];
	for (bp = baseptr; *bp != '\0'; ++bp)
	    if (*bp == '/' || *bp == '\\') baseptr = bp+1;

	docount = strncmp("countg",baseptr,6) == 0;
	dofilter = !docount;

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
		         SWBOOLEAN('q',qswitch)
		    else SWBOOLEAN('f',fswitch)
		    else SWBOOLEAN('v',vswitch)
		    else SWBOOLEAN('V',Vswitch)
		    else SWRANGE('p',":-",pswitch,pval1,pval2,"-p")
		    else if (sw == '-')
		    {
			docount = TRUE;
			decodekeys(arg);
			while (*arg != '\0') ++arg;
		    }
		    else
		    {
			if (sw == '~')
			{
			    neg = TRUE;
			    sw = *arg++;
			}
			    else neg = FALSE;

			for (i = 0; i < NUMCONSTRAINTS; ++i)
			    if (sw == SYMBOL(i))
			    {
				NEEDED(i) |= 2;
				if (VALTYPE(i) == INTTYPE
				 || VALTYPE(i) == GROUPSIZE)
				    arg_range(&arg,":-",&LO(i),&HI(i),ID(i));
				else
				    LO(i) = HI(i) = 1;
				if (neg) INVERSE(i) = TRUE;
				havecon = TRUE;
			        break;
			    }
			if (i == NUMCONSTRAINTS) badargs = TRUE;
		    }
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

	if (badargs || argnum > 2)
	{
	    fprintf(stderr,">E Usage: %s\n",USAGE);
	    GETHELP;
	    exit(1);
	}

	if (vswitch && !havecon)
	{
	    fprintf(stderr,">E -v is illegal with no constraints\n");
	    exit(1);
	}

	for (j = NUMCONSTRAINTS; --j >= 0;)
	if (ISNEEDED(j))
	{
	     cmask = PREREQ(j);
	     for (i = 0; cmask != 0; ++i, cmask >>= 1)
		 if (cmask & 1) NEEDED(i) |= 1;
	}

	if (vswitch)
	{
	    for (j = 0; j < NUMCONSTRAINTS; ++j)
		if (ISCONSTRAINT(j)) INVERSE(j) = !INVERSE(j);
	}

	if (!qswitch)
	{
	    fprintf(stderr,">A %s",argv[0]);
	    if (fswitch || pswitch)
		fprintf(stderr," -");
	    if (fswitch) fprintf(stderr,"f");
	    if (pswitch) writerange(stderr,'p',pval1,pval2);

	    if (numkeys > 0)
	    {
		fprintf(stderr," --");
		for (j = 0; j < numkeys; ++j)
		    fprintf(stderr,"%c",SYMBOL(key[j]));
	    }

	    if (havecon) fprintf(stderr," -");
	    for (j = 0; j < NUMCONSTRAINTS; ++j)
	    if (ISCONSTRAINT(j))
	    {
		if (INVERSE(j)) fprintf(stderr,"~");
		if (VALTYPE(j) == BOOLTYPE)
		    fprintf(stderr,"%c",SYMBOL(j));
		else
		    writerange(stderr,(int)SYMBOL(j),LO(j),HI(j));
	    }

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

	if (dofilter) countfile = stderr;
	else          countfile = outfile;

	if (codetype&SPARSE6) outcode = SPARSE6;
	else                  outcode = GRAPH6;

	nin = nout = 0;
	if (!pswitch || pval2 == NOLIMIT) maxin = NOLIMIT;
	else if (pval1 < 1)               maxin = pval2;
	else                              maxin = pval2 - pval1 + 1;
	t = CPUTIME;
	while (nin < maxin || maxin == NOLIMIT)
	{
	    if ((g = readg(infile,NULL,0,&m,&n)) == NULL) break;
	    ++nin;

	    for (j = 0; j < NUMCONSTRAINTS; ++j) COMPUTED(j) = FALSE;

	    if (selected(g,m,n))
	    {
	        if (dofilter) writelast(outfile);
		if (Vswitch)
		{
		    fprintf(countfile,"Graph %6ld : ",nin);
		    printthesevals(countfile);
		    fprintf(countfile,"\n");
	        }
		else if (docount)
		    add_one();

	        ++nout;
	    }
	    FREES(g);
	}
	t = CPUTIME - t;

	if (docount && !Vswitch)
	{
	    splay_scan(count_root,countfile);
	    if (qswitch || !dofilter)
	    {
		fprintf(countfile,"%9ld graphs altogether",nout);
		if (nin != nout) fprintf(countfile," from %ld read",nin);
		fprintf(countfile,"; cpu=%.3f sec\n",t);
	    }
	}

	if (!qswitch && dofilter) 
	    fprintf(stderr,
		">Z  %ld graphs read from %s; %ld written to %s; %.3f sec\n",
	        nin,infilename,nout,outfilename,t);

	exit(0);
}
