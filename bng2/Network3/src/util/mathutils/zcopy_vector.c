#include "mathutils.h"

		    
/*==========================================================================*/
void ZCOPY_VECTOR(dcomplex *a, dcomplex *b, int dim){
    memcpy((void *) b, (void *) a, dim*sizeof(dcomplex));
}
