/* This program prints generators for the automorphism group of an
   n-vertex polygon, where n is a number supplied by the user.
   It needs to be linked with nauty.c, nautil.c and nausparse.c.
   This version uses sparse form with dynamic allocation.
*/

#include "nausparse.h"    /* which includes nauty.h */

int
main(int argc, char *argv[])
{
    DYNALLSTAT(int,lab,lab_sz);
    DYNALLSTAT(int,ptn,ptn_sz);
    DYNALLSTAT(int,orbits,orbits_sz);
    DYNALLSTAT(setword,workspace,workspace_sz);
    static DEFAULTOPTIONS_SPARSEGRAPH(options);
    statsblk stats;
    sparsegraph sg;   /* Declare sparse graph structure */

    int n,m,i;

    options.writeautoms = TRUE;

 /* Initialise sparse graph structure. */

    SG_INIT(sg);

    while (1)
    {
        printf("\nenter n : ");
        if (scanf("%d",&n) == 1 && n > 0)
        {
            m = (n + WORDSIZE - 1) / WORDSIZE;
            nauty_check(WORDSIZE,m,n,NAUTYVERSIONID);

            DYNALLOC1(int,lab,lab_sz,n,"malloc");
            DYNALLOC1(int,ptn,ptn_sz,n,"malloc");
            DYNALLOC1(int,orbits,orbits_sz,n,"malloc");
            DYNALLOC1(setword,workspace,workspace_sz,2*m,"malloc");

         /* SG_ALLOC makes sure that the v,d,e fields of a sparse graph
         * structure point to arrays that are large enough.  This only
         * works if the structure has been initialised.             */

            SG_ALLOC(sg,n,2*n,"malloc");

            sg.nv = n;              /* Number of vertices */
            sg.nde = 2*n;           /* Number of directed edges */

            for (i = 0; i < n; ++i)
            {
                sg.v[i] = 2*i;
                sg.d[i] = 2;
                sg.e[2*i] = (i+n-1)%n;      /* edge i->i-1 */
                sg.e[2*i+1] = (i+n+1)%n;    /* edge i->i+1 */
            }

            printf("Generators for Aut(C[%d]):\n",n);
            nauty((graph*)&sg,lab,ptn,NULL,orbits,&options,&stats,
                                                  workspace,2*m,m,n,NULL);

            printf("Automorphism group size = %.0f",stats.grpsize1+0.1);
        }
        else
            break;
    }

    exit(0);
}
