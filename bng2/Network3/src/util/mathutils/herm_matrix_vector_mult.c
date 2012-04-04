#include "mathutils.h"

		    
/*==========================================================================*/
void HERM_MATRIX_VECTOR_MULT(dcomplex **a, dcomplex *x, dcomplex *y, int
			     dim){
    /* uses BLAS3 routine zhemv */
#ifdef APPEND_UNDERSCORE
#define ZHEMV zhemv_
#else
#define ZHEMV zhemv
#endif
    int n=dim;
    int incx=1;
    int incy=1;
    dcomplex alpha={1.0,0.0};
    dcomplex beta={0.0,0.0};
    extern void ZHEMV();
    
    ZMATRIX_TRANSPOSE(a,dim);
    ZHEMV("u", &n, &alpha, a[0], &n, x, &incx, &beta, y, &incy);
    ZMATRIX_TRANSPOSE(a,dim);
}
