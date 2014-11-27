#include "mathutils.h"

		    
/*==========================================================================*/
/* Perform computation a.b = c where a and b are real vectors.              */
double DOTP (double *a, double *b, int dim){
#ifdef APPEND_UNDERSCORE
#define DDOT ddot_
#else
#define DDOT ddot
#endif
    int n;
    int inca=1;
    int incb=1;
    extern double DDOT();
    
    n=dim;
    return(DDOT(&n, a, &inca, b, &incb));
}
