#include "mathutils.h"


/*==========================================================================*/
/* returns sqrt of sum of squares of the elts of a */
double NORM( double *a, int dim){
#ifdef APPEND_UNDERSCORE
#define DNRM2 dnrm2_
#else
#define DNRM2 dnrm2
#endif
    int n;
    int inca=1;
    double norm;
    extern double DNRM2();

    n=dim;
    return(DNRM2(&n, a, &inca));
}

double DNRM2(int *n, double *x, int *incx)
{
  long int ix, nn, iincx;
  double norm, scale, absxi, ssq, temp;

/*  DNRM2 returns the euclidean norm of a vector via the function
    name, so that

       DNRM2 := sqrt( x'*x )

    -- This version written on 25-October-1982.
       Modified on 14-October-1993 to inline the call to SLASSQ.
       Sven Hammarling, Nag Ltd.   */

  /* Dereference inputs */
  nn = *n;
  iincx = *incx;

  if( nn > 0 && iincx > 0 )
  {
    if (nn == 1)
    {
      norm = fabs(x[0]);
    }
    else
    {
      scale = 0.0;
      ssq = 1.0;

      /* The following loop is equivalent to this call to the LAPACK
         auxiliary routine:   CALL SLASSQ( N, X, INCX, SCALE, SSQ ) */

      for (ix=(nn-1)*iincx; ix>=0; ix-=iincx)
      {
        if (x[ix] != 0.0)
        {
          absxi = fabs(x[ix]);
          if (scale < absxi)
          {
            temp = scale / absxi;
            ssq = ssq * (temp * temp) + 1.0;
            scale = absxi;
          }
          else
          {
            temp = absxi / scale;
            ssq += temp * temp;
          }
        }
      }
      norm = scale * sqrt(ssq);
    }
  }
  else
    norm = 0.0;

  return norm;

} /* dnrm2_ */
