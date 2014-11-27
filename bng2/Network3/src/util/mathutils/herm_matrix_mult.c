#include "mathutils.h"

		    
/*==========================================================================*/
void HERM_MATRIX_MULT(dcomplex **a, dcomplex **b, dcomplex **c, int dim){
  /* uses upper triangular region of matrix only */
  /* uses BLAS3 routine zhemm */
#ifdef APPEND_UNDERSCORE
#define ZHEMM zhemm_
#else
#define ZHEMM zhemm
#endif
    int n=dim;
    dcomplex alpha={1.0,0.0};
    dcomplex beta={0.0,0.0};
    extern void ZHEMM();
    
    /* transpose necessary because of opposite way Fortran references */
    /* arrays */
    ZMATRIX_TRANSPOSE(a,dim);
    ZMATRIX_TRANSPOSE(b,dim);
    ZHEMM("l", "u", &n, &n, &alpha, a[0], &n, b[0], &n, &beta, c[0], &n);
    ZMATRIX_TRANSPOSE(a,dim);
    ZMATRIX_TRANSPOSE(b,dim);
    ZMATRIX_TRANSPOSE(c,dim);
}

