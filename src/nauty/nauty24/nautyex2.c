/* This program prints generators for the automorphism group of an
   n-vertex polygon, where n is a number supplied by the user.
   It needs to be linked with nauty.c, nautil.c and naugraph.c.

   This version uses dynamic allocation.
*/

#include "nauty.h"    /* which includes <stdio.h> */
/* MAXN=0 is defined by nauty.h, which implies dynamic allocation */

int
main(int argc, char *argv[])
{
  /* DYNALLSTAT declares a pointer variable (to hold an array when it
  * is allocated) and a size variable to remember how big the array is.
  * Nothing is allocated yet.                                        */
 
    DYNALLSTAT(graph,g,g_sz);
    DYNALLSTAT(int,lab,lab_sz);
    DYNALLSTAT(int,ptn,ptn_sz);
    DYNALLSTAT(int,orbits,orbits_sz);
    DYNALLSTAT(setword,workspace,workspace_sz);
    static DEFAULTOPTIONS_GRAPH(options);
    statsblk stats;

    int n,m,v;
    set *gv;

/* Default options are set by the DEFAULTOPTIONS_GRAPH macro above.
 * Here we change those options that we want to be different from the
 * defaults.  writeautoms=TRUE causes automorphisms to be written.     */

    options.writeautoms = TRUE;

    while (1)
    {
        printf("\nenter n : ");
        if (scanf("%d",&n) == 1 && n > 0)
        {

     /* The nauty parameter m is a value such that an array of
      * m setwords is sufficient to hold n bits.  The type setword
      * is defined in nauty.h.  The number of bits in a setword is
      * WORDSIZE, which is 16, 32 or 64.  Here we calculate
      * m = ceiling(n/WORDSIZE).                                  */

            m = (n + WORDSIZE - 1) / WORDSIZE;

         /* The following optional call verifies that we are linking
          * to compatible versions of the nauty routines.            */

            nauty_check(WORDSIZE,m,n,NAUTYVERSIONID);

         /* Now that we know how big the graph will be, we allocate
          * space for the graph and the other arrays we need.   */

            DYNALLOC2(graph,g,g_sz,m,n,"malloc");
            DYNALLOC1(setword,workspace,workspace_sz,5*m,"malloc");
            DYNALLOC1(int,lab,lab_sz,n,"malloc");
            DYNALLOC1(int,ptn,ptn_sz,n,"malloc");
            DYNALLOC1(int,orbits,orbits_sz,n,"malloc");

            for (v = 0; v < n; ++v)
            {
                gv = GRAPHROW(g,v,m);

                EMPTYSET(gv,m);
                ADDELEMENT(gv,(v+n-1)%n);
                ADDELEMENT(gv,(v+1)%n);
            }

            printf("Generators for Aut(C[%d]):\n",n);
            nauty(g,lab,ptn,NULL,orbits,&options,&stats,
                                            workspace,5*m,m,n,NULL);

    /* The size of the group is returned in stats.grpsize1 and
     * stats.grpsize2. See dreadnaut.c for code that will write the
     * value in a sensible format; here we will take advantage of
     * knowing that the size cannot be very large.  Adding 0.1 is
     * just in case the floating value is truncated instead of rounded,
     * but that shouldn't be.                                     */

        printf("Automorphism group size = %.0f",stats.grpsize1+0.1);
 
        }
        else
            break;
    }

    exit(0);
}
