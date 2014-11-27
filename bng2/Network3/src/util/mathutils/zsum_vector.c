#include "mathutils.h"

		    
/*==========================================================================*/
#define DSUM FNAME(dsum)

dcomplex ZSUM_VECTOR( dcomplex *a, int dim){
    int n=dim;
    int inca=2;
    dcomplex result=COMPLEX_ZERO;
    extern double DSUM();
    
    result.r = DSUM(&n, (double *) a, &inca);
    result.i = DSUM(&n, (double *) a + 1, &inca);
    return (result);
}
