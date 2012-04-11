#include "mathutils.h"
		    
void IINIT_VECTOR(int *a, int val, int dim){
    register int *aptr;
    for(aptr=a; aptr<a+dim; ++aptr)
	*aptr=val;
}
		    
void INIT_VECTOR(double *a, double val, int dim){
    register double *aptr;
    
    for(aptr=a; aptr<a+dim; ++aptr)
	*aptr=val;
}

void ZINIT_VECTOR(dcomplex *a, dcomplex val, int dim){
    register dcomplex *aptr;
    
    for(aptr=a; aptr< a+dim;  ++aptr)
	*aptr=val;
}
