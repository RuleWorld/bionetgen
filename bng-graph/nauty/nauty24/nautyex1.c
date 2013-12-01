/* This program prints generators for the automorphism group of an
   n-vertex polygon, where n is a number supplied by the user.
   It needs to be linked with nauty.c, nautil.c and naugraph.c.

   This version uses a fixed limit for MAXN.
*/

#define MAXN 1000    /* Define this before including nauty.h */
#include "nauty.h"   /* which includes <stdio.h> and other system files */

int
main(int argc, char *argv[])
{
    graph g[MAXN*MAXM];
    int lab[MAXN],ptn[MAXN],orbits[MAXN];
    static DEFAULTOPTIONS_GRAPH(options);
    statsblk stats;
    setword workspace[5*MAXM];

    int n,m,v;
    set *gv;

 /* Default options are set by the DEFAULTOPTIONS_GRAPH macro above.
 *  Here we change those options that we want to be different from the
 *  defaults.  writeautoms=TRUE causes automorphisms to be written.     */

    options.writeautoms = TRUE;

    while (1)
    {
        printf("\nenter n : ");
        if (scanf("%d",&n) != 1 || n <= 0)    /* Exit if EOF or bad number */
            break;

        if (n > MAXN)
        {
            printf("n must be in the range 1..%d\n",MAXN);
            exit(1);
        }

     /* The nauty parameter m is a value such that an array of
      * m setwords is sufficient to hold n bits.  The type setword
      * is defined in nauty.h.  The number of bits in a setword is
      * WORDSIZE, which is 16, 32 or 64.  Here we calculate
      * m = ceiling(n/WORDSIZE).                                  */

        m = (n + WORDSIZE - 1) / WORDSIZE;

     /* The following optional call verifies that we are linking
      * to compatible versions of the nauty routines.            */

        nauty_check(WORDSIZE,m,n,NAUTYVERSIONID);

     /* Now we create the cycle.  For each v, we add the edges
      * (v,v+1) and (v,v-1), where values are mod n.  gv is set to
      * the position in g[] where row v starts, EMPTYSET zeros it,
      * then ADDELEMENT adds one bit (a directed edge) to it.  */

        for (v = 0; v < n; ++v)
        {
            gv = GRAPHROW(g,v,m);

            EMPTYSET(gv,m);
            ADDELEMENT(gv,(v+n-1)%n);
            ADDELEMENT(gv,(v+1)%n);
        }

        printf("Generators for Aut(C[%d]):\n",n);

     /* Since we are not requiring a canonical labelling, the last
      * parameter to nauty() is noit required and can be NULL. 
      * Similarly, we are not using a fancy active[] value, so we
      * can pass NULL to ask nauty() to use the default.          */

        nauty(g,lab,ptn,NULL,orbits,&options,&stats,
                                            workspace,5*MAXM,m,n,NULL);

     /* The size of the group is returned in stats.grpsize1 and
      * stats.grpsize2. See dreadnaut.c for code that will write the
      * value in a sensible format; here we will take advantage of
      * knowing that the size cannot be very large.  Adding 0.1 is
      * just in case the floating value is truncated instead of rounded,
      * but that shouldn't be.                                     */

        printf("Automorphism group size = %.0f",stats.grpsize1+0.1);
    }

    exit(0);
}
