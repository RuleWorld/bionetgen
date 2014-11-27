#include "mathutils.h"

		    
/*==========================================================================*/
/* Perform computation conj(a).b = c where a and b are complex.             */
/* Home grown because of difficulty of passing complex function between */
/* C and Fortran */
dcomplex ZDOTC (dcomplex *a, dcomplex *b, int dim)
{
  register int i;
  dcomplex *aptr, *bptr, dotpr;
  
  aptr=a; 
  bptr=b;
  dotpr=COMPLEX_ZERO;
  
  for(i=0; i<dim; ++i, ++aptr, ++bptr)
  {
    dotpr = Cadd(dotpr, Cmult(Cconj(*aptr),*bptr));
  }
  return(dotpr);
}
