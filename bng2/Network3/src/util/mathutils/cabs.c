#include "mathutils.h"

		    
/*==========================================================================*/


/* COMPLEX ARITHMETIC UTILITIES */

/*==========================================================================*/

double Cabs(dcomplex a)
{
  return(sqrt(a.r*a.r + a.i*a.i));
}
