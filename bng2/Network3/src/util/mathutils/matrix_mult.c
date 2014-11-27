#include "mathutils.h"

		    
/*==========================================================================*/
void MATRIX_MULT(double **a, double **b, double **c, int dim){
  /* uses BLAS3 routine dgemm (NAG f06yaf) */
#ifdef APPEND_UNDERSCORE
#define DGEMM dgemm_
#else
#define DGEMM dgemm
#endif
  int n=dim;
  double alpha=1.0;
  double beta=0.0;
  extern void DGEMM();

  /* need to call dgemm_ with transposed matrices because C and Fortran */
  /* reference matrices in the opposite sense */
  DGEMM("t", "t", &n, &n, &n, &alpha, a[0], &n, b[0], &n, &beta, c[0], &n);
  /* tranpose result */
  MATRIX_TRANSPOSE(c, dim);
}
