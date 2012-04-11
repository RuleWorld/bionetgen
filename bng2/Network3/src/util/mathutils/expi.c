#include "mathutils.h"

		    
/*==========================================================================*/

dcomplex ExpI (double x)
{
  dcomplex c;
  
  c.r = cos(x);
  c.i = sin(x);
  return(c);
}
