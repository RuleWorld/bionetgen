#include "mathutils.h"

		    
/*==========================================================================*/
/* returns sqrt of sum of squares of the elts of complex a */
double ZNORM( dcomplex *a, int dim){
#ifdef APPEND_UNDERSCORE
#define DZNRM2 dznrm2_
#else
#define DZNRM2 dznrm2
#endif
    int n;
    int inca=1;
    extern double DZNRM2();
    
    n=dim;
    return (DZNRM2(&n, a, &inca));
}
