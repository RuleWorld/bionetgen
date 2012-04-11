#include "mathutils.h"

		    
/*==========================================================================*/
/* multiples a complex vector by a complex scalar alpha*/
void ZSCALE_VECTOR( dcomplex alpha, dcomplex *a, int dim)
{
  int n;
  int inca=1;
  dcomplex scalef;
  extern void zscal_();

  scalef=alpha;
  n=dim;
  zscal_(&n, &scalef, a, &inca);
}
