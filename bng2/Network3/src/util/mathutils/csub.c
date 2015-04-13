#include "mathutils.h"

		    
/*==========================================================================*/

dcomplex Csub(dcomplex a, dcomplex b)
{
  dcomplex c;
  
  c.r = a.r-b.r;
  c.i = a.i-b.i;
  return(c);
}
