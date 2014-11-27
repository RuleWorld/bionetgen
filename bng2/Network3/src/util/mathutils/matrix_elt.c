#include "mathutils.h"

		    
/*==========================================================================*/
/* Return product a.H.b where a and b are vectors, H is a real symmetric */
/* matrix.  */

double MATRIX_ELT (double *a, double **H, double *b, int dim)
{
  double aHb;
  double *Hb;
  
  Hb = ALLOC_VECTOR(dim);
  SYM_MATRIX_VECTOR_MULT(H,b,Hb,dim);
  aHb = DOTP(a,Hb,dim);
  FREE_VECTOR(Hb);
  return(aHb);
}
