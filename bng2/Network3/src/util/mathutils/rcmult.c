#include "mathutils.h"

		    
/*==========================================================================*/

dcomplex RCmult(double r, dcomplex a)
{
  dcomplex c;
  
  c.r = r*a.r;
  c.i = r*a.i;
  return(c);
}
