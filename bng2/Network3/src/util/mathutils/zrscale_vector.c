#include "mathutils.h"

		    
/*==========================================================================*/
/* multiples a complex vector by a real scalar alpha*/
void ZRSCALE_VECTOR( double alpha, dcomplex *a, int dim){
#ifdef APPEND_UNDERSCORE
#define ZDSCAL zdscal_
#else
#define ZDSCAL zdscal
#endif
    int n=dim;
    int inca=1;
    double scalef=alpha;
    extern void ZDSCAL();
    
    scalef=alpha;
    ZDSCAL(&n, &scalef, a, &inca);
}
