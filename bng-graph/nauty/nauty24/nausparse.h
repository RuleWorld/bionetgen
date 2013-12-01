/* nausparse.h : header file for sparse digraphs, nauty 2.4 */
/* This version allows only simple graphs with loops but
 * contains the data structures for weights on the edges
 * even though they aren't implemented yet. */

#ifndef  _NAUSPARSE_H_    /* only process this file once */
#define  _NAUSPARSE_H_

#include "nauty.h"

#ifndef SG_WEIGHT
#define SG_WEIGHT int
#endif

typedef struct
{
	int nv,nde;
	int *v,*d,*e;
	SG_WEIGHT *w;      /* Not implemented, should be NULL. */
	size_t vlen,dlen,elen,wlen;
} sparsegraph;

/* tip: loops only contribute 1 to nde */

#define SG_VDE(sgp,vv,dd,ee) do { vv = ((sparsegraph*)(sgp))->v; \
  dd = ((sparsegraph*)(sgp))->d; ee = ((sparsegraph*)(sgp))->e; } while(0)
#define SG_INIT(sg) do { (sg).v = (sg).d = (sg).e = (sg).w = NULL; \
   (sg).vlen = (sg).dlen = (sg).elen = (sg).wlen = 0; } while(0)
#define SG_ALLOC(sg,nlen,ndelen,msg) do { \
   DYNALLOC1(int,(sg).v,(sg).vlen,nlen,msg); \
   DYNALLOC1(int,(sg).d,(sg).dlen,nlen,msg); \
   DYNALLOC1(int,(sg).e,(sg).elen,ndelen,msg); } while (0)
#define SG_FREE(sg) do { \
   DYNFREE((sg).v,(sg).vlen); \
   DYNFREE((sg).d,(sg).dlen); \
   DYNFREE((sg).e,(sg).elen); } while (0)

#define SG_DECL(sg) sparsegraph sg = {0,0,NULL,NULL,NULL,NULL,0,0,0,0}

#define DEFAULTOPTIONS_SPARSEGRAPH(options) optionblk options = \
 {0,FALSE,FALSE,FALSE,TRUE,FALSE,CONSOLWIDTH, \
  NULL,NULL,NULL,NULL,NULL,NULL,100,0,1,0,&dispatch_sparse,NULL}

#ifdef __cplusplus
extern "C" {
#endif

extern dispatchvec dispatch_sparse;

extern int targetcell_sg(graph*,int*,int*,int,int,boolean,int,int,int);
extern boolean cheapautom_sg(int*,int,boolean,int);
extern boolean isautom_sg(graph*,permutation*,boolean,int,int);
extern void
    refine_sg(graph*,int*,int*,int,int*,permutation*,set*,int*,int,int);
extern int testcanlab_sg(graph*,graph*,int*,int*,int,int);
extern void updatecan_sg(graph*,graph*,permutation*,int,int,int);
extern void init_sg(graph*,graph**,graph*,graph**,int*,int*,set*,
	                   struct optionstruct*,int*,int,int);
extern void nausparse_freedyn(void);
extern void nausparse_check(int,int,int,int);

extern sparsegraph *nauty_to_sg(graph*,sparsegraph*,int,int);
extern graph* sg_to_nauty(sparsegraph*,graph*,int,int*);
extern void sortlists_sg(sparsegraph*);
extern boolean aresame_sg(sparsegraph*,sparsegraph*);
extern void put_sg(FILE*,sparsegraph*,boolean,int);

extern void adjacencies_sg(graph*,int*,int*,int,int,int,
	                 permutation*,int,boolean,int,int);
extern void distances_sg(graph*,int*,int*,int,int,int,
	                 permutation*,int,boolean,int,int);

#ifdef __cplusplus
}
#endif

#endif
