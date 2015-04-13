#include "mathutils.h"

void MATRIX_TRANSPOSE(double **a, int dim)
{
  register int i;
  register double *uptr, *lptr, *end;
  register double dsw;

  for(i=0; i<dim-1; ++i)
  {
    end = a[i+1];
    for(uptr=end-dim+i+1, lptr=end+i; uptr<end; ++uptr, lptr+=dim)
    {
      dsw = *lptr;
      *lptr = *uptr;
      *uptr = dsw;
    }
  }
}





