#include "mathutils.h"

		    
/*==========================================================================*/
void MATRIX_VECTOR_MULT(double **a, double *x, double *y, int dim){
    /* uses BLAS3 routine dgemv  */
#ifdef APPEND_UNDERSCORE
#define DGEMV dgemv_
#else
#define DGEMV dgemv
#endif
    int n=dim;
    int incx=1;
    int incy=1;
    double alpha=1.0;
    double beta=0.0;
    extern void DGEMV();
    
    /* take the transpose of the multiplying matrix because matrices */
    /* referenced opposite in Fortran */
    DGEMV("t", &n, &n, &alpha, a[0], &n, x, &incx, &beta, y, &incy);
}
