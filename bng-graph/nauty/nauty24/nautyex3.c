/* This program prints the entire automorphism group of an n-vertex
   polygon, where n is a number supplied by the user.  It needs to
   be linked with nauty.c, nautil.c, naugraph.c and naugroup.c.
*/

#include "nauty.h"    /* which includes <stdio.h> */
#include "naugroup.h"

/**************************************************************************/

void
writeautom(permutation *p, int n)
/* Called by allgroup.  Just writes the permutation p. */
{
    int i;

    for (i = 0; i < n; ++i) printf(" %2d",p[i]); printf("\n");
}

/**************************************************************************/

int
main(int argc, char *argv[])
{
    DYNALLSTAT(graph,g,g_sz);
    DYNALLSTAT(int,lab,lab_sz);
    DYNALLSTAT(int,ptn,ptn_sz);
    DYNALLSTAT(int,orbits,orbits_sz);
    DYNALLSTAT(setword,workspace,workspace_sz);
    static DEFAULTOPTIONS_GRAPH(options);
    statsblk stats;

    int n,m,v;
    set *gv;
    grouprec *group;

 /* The following cause nauty to call two procedures which
        store the group information as nauty runs. */
        
    options.userautomproc = groupautomproc;
    options.userlevelproc = grouplevelproc;

    while (1)
    {
        printf("\nenter n : ");
        if (scanf("%d",&n) == 1 && n > 0)
        {
            m = (n + WORDSIZE - 1) / WORDSIZE;
            nauty_check(WORDSIZE,m,n,NAUTYVERSIONID);

            DYNALLOC2(graph,g,g_sz,m,n,"malloc");
            DYNALLOC1(int,lab,lab_sz,n,"malloc");
            DYNALLOC1(int,ptn,ptn_sz,n,"malloc");
            DYNALLOC1(int,orbits,orbits_sz,n,"malloc");
            DYNALLOC1(setword,workspace,workspace_sz,5*m,"malloc");

            for (v = 0; v < n; ++v)
            {
                gv = GRAPHROW(g,v,m);
                EMPTYSET(gv,m);
                ADDELEMENT(gv,(v+n-1)%n);
                ADDELEMENT(gv,(v+1)%n);
            }

            printf("Automorphisms of C[%d]:\n",n);
            nauty(g,lab,ptn,NULL,orbits,&options,&stats,
                                            workspace,5*m,m,n,NULL);

         /* Get a pointer to the structure in which the group information
                has been stored.  If you use TRUE as an argument, the
                structure will be "cut loose" so that it won't be used
                again the next time nauty() is called.  Otherwise, as
                here, the same structure is used repeatedly. */
                
            group = groupptr(FALSE);

         /* Expand the group structure to include a full set of coset
                representatives at every level.  This step is necessary
                if allgroup() is to be called. */
                
            makecosetreps(group);

         /* Call the procedure writeautom() for every element of the group.
                The first call is always for the identity. */
                
            allgroup(group,writeautom);
        }
        else
            break;
    }
    exit(0);
}
