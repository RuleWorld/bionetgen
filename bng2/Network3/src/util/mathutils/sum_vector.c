#include "mathutils.h"

		    
/*==========================================================================*/
#define DSUM FNAME(dsum)

double SUM_VECTOR( double *a, int dim)
{
    int n=dim;
    int inca=1;
    extern double DSUM();
    
    return(DSUM(&n, a, &inca));
}

#ifdef NOBLAS_E
double DSUM( int *n, double *a, int *inca)
{
    register int inc=*inca, len=*n;
    register double *x, *xlast, sum=0.0;

    xlast= a + len*inc;
    for(x=a; x<xlast; x+=inc)
        sum+= *x;
    return(sum);
}
#endif
