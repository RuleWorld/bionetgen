#include "mathutils.h"

		    
/*==========================================================================*/

dcomplex Cconj (dcomplex a)
{
  dcomplex c;

  c.r = a.r;
  c.i = -a.i;
  return(c);
}
