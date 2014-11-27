#include "mathutils.h"

		    
/*==========================================================================*/
/* returns sum of squares of the elts of complex a */
#define DZNRSQ FNAME(dznrsq)

double ZNORMSQ( dcomplex *a, int dim)
{
    int n;
    int inca=1;
    extern double DZNRSQ();
    
    n=dim;
    return (DZNRSQ(&n, a, &inca));
}

#ifdef NOBLAS_E
double DZNRSQ( int *n, dcomplex *a, int *inca)
{
    register int inc, len;
    register double *x, *xmax, sum=0.0;

    len=*n;
    inc=2*(*inca);
    xmax= (double *)(a) + inc*len;

    x= (double *) a;
    for(; x<xmax; x+=inc)
        sum+= SQR(*x);
    
    x= ((double *) a)+1;
    for(; x<xmax; x+=inc)
        sum+= SQR(*x);

    return(sum);
}
#endif

