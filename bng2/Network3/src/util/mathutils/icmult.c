#include "mathutils.h"

		    
/*==========================================================================*/

dcomplex ICmult(double im, dcomplex a)
{
  dcomplex c;
  
  c.r = -im*a.i;
  c.i = im*a.r;
  return(c);
}
