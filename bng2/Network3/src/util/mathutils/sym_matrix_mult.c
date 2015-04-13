#include "mathutils.h"

		    
/*==========================================================================*/
void SYM_MATRIX_MULT(double **a, double **b, double **c, int dim){
#ifdef APPEND_UNDERSCORE
#define DSYMM dsymm_
#else
#define DSYMM dsymm
#endif
    /* uses upper triangular region of matrix only */
    /* uses BLAS3 routine dsymm */
    int n=dim;
    double alpha=1.0;
    double beta=0.0;
    extern void DSYMM();
    
    /* transpose necessary because of opposite way Fortran references */
    /* arrays */
    MATRIX_TRANSPOSE(b,dim);
    DSYMM("l", "l", &n, &n, &alpha, a[0], &n, b[0], &n, &beta, c[0], &n);
    MATRIX_TRANSPOSE(b,dim);
    MATRIX_TRANSPOSE(c,dim);
}
