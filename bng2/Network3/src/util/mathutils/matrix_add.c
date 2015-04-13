#include "mathutils.h"

		    
/*==========================================================================*/
void MATRIX_ADD(double **a, double **b, double **c, int dim)
{
  register int i,j;
  double *aptr, *bptr, *cptr;
  
  for(i=0; i<dim; ++i)
  {
    for(j=0, aptr=a[i], bptr=b[i], cptr=c[i]; j<dim; ++j, ++aptr,
	++bptr, ++cptr)
      *cptr = *aptr + *bptr;
  }

}
