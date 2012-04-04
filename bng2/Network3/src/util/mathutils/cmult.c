#include "mathutils.h"

		    
/*==========================================================================*/

dcomplex Cmult(dcomplex a, dcomplex b)
{
  dcomplex c;
  
  c.r = a.r*b.r - a.i*b.i;
  c.i = a.r*b.i + a.i*b.r;
  return(c);
}
