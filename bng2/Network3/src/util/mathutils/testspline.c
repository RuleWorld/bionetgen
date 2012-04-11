#include "mathutils.h"

#define NPTS 20
#define XMIN (-3.0)
#define XMAX  3.0

main()
{
  int i;
  double *xa, *ya, *y2a, x;
  double y, dy, d2y;
  double delta_x;

  xa = ALLOC_VECTOR(NPTS);
  ya = ALLOC_VECTOR(NPTS);
  y2a = ALLOC_VECTOR(NPTS);

  delta_x = (XMAX-XMIN)/(NPTS-1);
    
  for(i=0; i<NPTS; ++i)
  {
    x = XMIN + i*delta_x;
    xa[i] = x;
    y = exp(-x);
    ya[i] = 50.0*(y*y - 2*y);
  }
  
  SPLINE_INIT(xa, ya, NPTS, 1.0e99, 1.0e99, y2a);

  delta_x = (XMAX-XMIN)/(4*NPTS);
  for(i=0; i<4*NPTS+1; ++i)
  {
    x = XMIN + i*delta_x;
    SPLINE_EVAL(xa, ya, y2a, NPTS, x, &y, &dy, &d2y);
/*    printf("%.8e %.8e\n", x, y);*/
    printf("%.8e %.8e %.8e %.8e\n", x, y, dy, d2y);
  }
}




