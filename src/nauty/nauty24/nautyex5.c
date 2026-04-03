/* This program demonstrates how an isomorphism is found between
   graphs of the form in the figure above, for general size.
   It needs to be linked with nauty.c, nautil.c and nausparse.c.
   This version uses sparse form with dynamic allocation.
*/

#include "nausparse.h"    /* which includes nauty.h */

int
main(int argc, char *argv[])
{
    DYNALLSTAT(int,lab1,lab1_sz);
    DYNALLSTAT(int,lab2,lab2_sz);
    DYNALLSTAT(int,ptn,ptn_sz);
    DYNALLSTAT(int,orbits,orbits_sz);
    DYNALLSTAT(setword,workspace,workspace_sz);
    DYNALLSTAT(int,map,map_sz);
    static DEFAULTOPTIONS_SPARSEGRAPH(options);
    statsblk stats;
    sparsegraph sg1,sg2,cg1,cg2;   /* Declare sparse graph structures */

    int n,m,i;

 /* Select option for canonical labelling */

    options.getcanon = TRUE;
 
 /* Initialise sparse graph structure. */

    SG_INIT(sg1); SG_INIT(sg2);
    SG_INIT(cg1); SG_INIT(cg2);

    while (1)
    {
        printf("\nenter n : ");
        if (scanf("%d",&n) == 1 && n > 0)
        {
            if (n%2 != 0)
            {
                fprintf(stderr,"Sorry, n must be even\n");
                continue;
            }

            m = (n + WORDSIZE - 1) / WORDSIZE;
            nauty_check(WORDSIZE,m,n,NAUTYVERSIONID);

            DYNALLOC1(int,lab1,lab1_sz,n,"malloc");
            DYNALLOC1(int,lab2,lab2_sz,n,"malloc");
            DYNALLOC1(int,ptn,ptn_sz,n,"malloc");
            DYNALLOC1(int,orbits,orbits_sz,n,"malloc");
            DYNALLOC1(setword,workspace,workspace_sz,2*m,"malloc");
            DYNALLOC1(int,map,map_sz,n,"malloc");

         /* Now make the first graph */

            SG_ALLOC(sg1,n,3*n,"malloc");
            sg1.nv = n;              /* Number of vertices */
            sg1.nde = 3*n;          /* Number of directed edges */

            for (i = 0; i < n; ++i)
            {
                sg1.v[i] = 3*i;
                sg1.d[i] = 3;
            }
             
            for (i = 0; i < n; i += 2) sg1.e[sg1.v[i]] = i+1;
            for (i = 1; i < n; i += 2) sg1.e[sg1.v[i]] = i-1;
            for (i = 0; i < n; ++i)
            {
                sg1.e[sg1.v[i]+1] = i+2;
                sg1.e[sg1.v[i]+2] = i-2;
            }
            sg1.e[sg1.v[0]+2] = n-1;
            sg1.e[sg1.v[1]+2] = n-2;
            sg1.e[sg1.v[n-2]+1] = 1;
            sg1.e[sg1.v[n-1]+1] = 0;

         /* Now make the second graph */

            SG_ALLOC(sg2,n,3*n,"malloc");
            sg2.nv = n;              /* Number of vertices */
            sg2.nde = 3*n;          /* Number of directed edges */

            for (i = 0; i < n; ++i)
            {
                sg2.v[i] = 3*i;
                sg2.d[i] = 3;
            }

            for (i = 0; i < n; ++i)
            {
                sg2.v[i] = 3*i;
                sg2.d[i] = 3;
                sg2.e[sg2.v[i]] = (i+1) % n;
                sg2.e[sg2.v[i]+1] = (i+n-1) % n;
                sg2.e[sg2.v[i]+2] = (i+n/2) % n;
            }

         /* Label sg1, result in cg1 and labelling in lab1; similarly sg1.
            It is not necessary to pre-allocate space in cg1 and cg2, but
            they have to be initialised as we did above.  */
            
            nauty((graph*)&sg1,lab1,ptn,NULL,orbits,&options,&stats,
                                              workspace,2*m,m,n,(graph*)&cg1);
            nauty((graph*)&sg2,lab2,ptn,NULL,orbits,&options,&stats,
                                              workspace,2*m,m,n,(graph*)&cg2);

         /* Compare canonically labelled graphs */

            if (aresame_sg(&cg1,&cg2))
            {
                printf("Isomorphic.\n");
                if (n <= 1000)
                {
                 /* Write the isomorphism.  For each i, vertex lab1[i]
                    of sg1 maps onto vertex lab2[i] of sg2.  We compute
                    the map in order of labelling because it looks better. */

                    for (i = 0; i < n; ++i) map[lab1[i]] = lab2[i];
                    for (i = 0; i < n; ++i)
                    {
                        printf(" %d-%d",i,map[i]);
                    }
                    printf("\n");
                }
            }
            else
                printf("Not isomorphic.\n");
        }
        else
            break;
    }

    exit(0);
}
