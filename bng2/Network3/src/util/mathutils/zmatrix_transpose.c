#include "mathutils.h"

void ZMATRIX_TRANSPOSE(dcomplex **a, int dim)
{
  register int i;
  register dcomplex *uptr, *lptr, *end;
  register dcomplex zsw;

  for(i=0; i<dim-1; ++i)
  {
    end = a[i+1];
    for(uptr=end-dim+i+1, lptr=end+i; uptr<end; ++uptr, lptr+=dim)
    {
      zsw = *lptr;
      *lptr = *uptr;
      *uptr = zsw;
    }
  }
}

