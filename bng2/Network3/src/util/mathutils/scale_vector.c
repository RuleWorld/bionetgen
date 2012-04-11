#include "mathutils.h"

		    
/*==========================================================================*/
/* multiples a real vector by a scalar alpha*/
void SCALE_VECTOR( double scale, double *a, int dim)
{
  register int i;
  double *dptr;

  for(i=0, dptr=a; i<dim; ++i, ++dptr)
    *dptr *= scale;
}
