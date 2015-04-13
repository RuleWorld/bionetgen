#include "mathutils.h"

/* compute c = a + lambda*b */
void VECTOR_ADD( double *a, double lambda, double *b, double *c, int dim){
    register int i;
    register double *aptr, *bptr, *cptr;

    aptr = a;
    bptr = b;
    cptr = c;

    if (lambda == 0.0){
	for (i=0; i<dim; ++i, ++aptr, ++bptr, ++cptr)
	    *cptr = *aptr;
    } else if (lambda == 1.0){
	for (i=0; i<dim; ++i, ++aptr, ++bptr, ++cptr)
	    *cptr = *aptr + *bptr;
    } else if (lambda == -1.0){
	for (i=0; i<dim; ++i, ++aptr, ++bptr, ++cptr)
	    *cptr = *aptr - *bptr;
    } else {
	for (i=0; i<dim; ++i, ++aptr, ++bptr, ++cptr)
	    *cptr = *aptr + lambda*(*bptr);
    }

    return;
}
