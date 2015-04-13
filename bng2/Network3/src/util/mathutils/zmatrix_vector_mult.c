#include "mathutils.h"

		    
/*==========================================================================*/
void ZMATRIX_VECTOR_MULT(dcomplex **a, dcomplex *x, dcomplex *y, int dim){
#ifdef APPEND_UNDERSCORE
#define ZGEMV zgemv_
#else
#define ZGEMV zgemv
#endif
    /* uses BLAS3 routine zgemv */
    int n=dim;
    int incx=1;
    int incy=1;
    dcomplex alpha={1.0,0.0};
    dcomplex beta={0.0,0.0};
    extern void ZGEMV();
    
    /* transpose because of difference between Fortran and C array */
    /* handling */
    ZGEMV("t", &n, &n, &alpha, a[0], &n, x, &incx, &beta, y, &incy);
}
