#include "mathutils.h"

main()
{
  double dat[]={1.0, 2.0, 3.0, 4.0, 5.0, 6.0, 7.0, 8.0};
  dcomplex **m;

  m = ZALLOC_MATRIX(2,2);
  COPY_VECTOR(dat, (double *) m[0], 8);
  ZMATRIX_TRANSPOSE(m, 2);
}
