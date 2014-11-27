#include "mathutils.h"

		    
/*==========================================================================*/
/* returns sum of squares of the elts of a */

#define DNRSQ FNAME(dnrsq)

double NORMSQ( double *a, int dim)
{
    int n;
    int inca=1;
    double norm;
    extern double DNRSQ();

    n=dim;

    return(DNRSQ(&n, a, &inca));
}

#ifdef NOBLAS_E
double DNRSQ( int *n, double *a, int *inca)
{
    register int inc=*inca, len=*n;
    register double *x, *xlast, sum=0.0;

    xlast= a + inc*len;
    for(x=a; x<xlast; x+=inc)
        sum+= SQR(*x);
    return(sum);
}
#endif
