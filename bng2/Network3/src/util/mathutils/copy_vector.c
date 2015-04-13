#include "mathutils.h"

		    
/*==========================================================================*/
void COPY_VECTOR(double *a, double *b, int dim){
    memcpy((void *) b, (void *) a, dim*sizeof(double));
}
