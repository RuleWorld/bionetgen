#include "mathutils.h"

		    
/*==========================================================================*/
void ZMATRIX_MULT(dcomplex **a, dcomplex **b, dcomplex **c, int dim){
    /* uses BLAS3 routine zgema */
#ifdef APPEND_UNDERSCORE
#define ZGEMM zgemm_
#else
#define ZGEMM zgemm
#endif
    int n=dim;
    dcomplex alpha={1.0,0.0};
    dcomplex beta={0.0,0.0};
    extern void ZGEMM();
    
    /* need to call dgemm_ with transposed matrices because C and Fortran */
    /* reference matrices in the opposite sense */
    ZGEMM("t", "t", &n, &n, &n, &alpha, a[0], &n, b[0], &n, &beta, c[0], &n);
    ZMATRIX_TRANSPOSE(c, dim);
}

