#include "mathutils.h"

		    
/*==========================================================================*/
void SYM_MATRIX_VECTOR_MULT(double **a, double *x, double *y, int dim){
#ifdef APPEND_UNDERSCORE
#define DSYMV dsymv_
#else
#define DSYMV dsymv
#endif
    /* uses BLAS3 routine dsymv  */
    int n=dim;
    int incx=1;
    int incy=1;
    double alpha=1.0;
    double beta=0.0;
    extern void DSYMV();
    
    DSYMV("l", &n, &alpha, a[0], &n, x, &incx, &beta, y, &incy);
}
