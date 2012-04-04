#include "mathutils.h"

		    
/*==========================================================================*/

dcomplex Cadd(dcomplex a, dcomplex b)
{
  dcomplex c;
  
  c.r = a.r+b.r;
  c.i = a.i+b.i;
  return(c);
}
