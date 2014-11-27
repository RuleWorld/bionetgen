/* function to take exponential of a Real Symmetric matrix */
/* in the form exp( I c A), where c is a double constant and A */
/* is a double precision matrix of dimension dim.  The eigenvalues and */
/* eigenvectors (by rows) must be passed to the routine. */
#include "mathutils.h"

void MATRIX_EXP( double c, dcomplex **result, double *evals, double
		**evecs, int dim)   
{
  int i, j, alpha;
  int n;
  dcomplex *rptr;
  dcomplex *expIca;
  
  n = dim;
  expIca = ZALLOC_VECTOR(n);

  /* Store exp(c*evals) in vector expIca */
  for(i=0; i<n; ++i)
    expIca[i] = ExpI(c*evals[i]); 
  
  /* Compute result matrix: S*exp(Ica)*St, where St is the matrix */
  /* obtained from the diagonalization above.*/
  for (i=0; i<n; ++i)
    for (j=0; j<=i; ++j)
    {
      rptr= &result[i][j];
      rptr->r = 0.0;
      rptr->i = 0.0;
      for (alpha=0; alpha<n; ++alpha)
      { 
	rptr->r += evecs[alpha][i]*expIca[alpha].r*evecs[alpha][j];
	rptr->i += evecs[alpha][i]*expIca[alpha].i*evecs[alpha][j];
      }
      result[j][i]= *rptr;
    }      
  /* Free storage space */
  ZFREE_VECTOR(expIca);

}
