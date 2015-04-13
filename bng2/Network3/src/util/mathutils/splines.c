#include "mathutils.h"

/* Calculate second derivatives for a cubic spline interpolating */
/* function on x and y arrays.  yp1 and ypn are the first derivatives at */
/* the boundaries.  Natural spline boundary conditions (y''=0) may be */
/* imposed at either boundary by setting yp1 or ypn >= 1.0e30.  This routine */
/* was derived from Numerical Recipes in C. */

void SPLINE_INIT(double x[], double y[], int n, double yp1, double ypn, double
	    y2[]) 
{
  int i,k;
  double p,qn,sig,un,*u;
  
  u = ALLOC_VECTOR(n);
  if (yp1 > 0.99e30)
    y2[0]=u[0]=0.0;
  else {
    y2[0] = -0.5;
    u[0]=(3.0/(x[1]-x[0]))*((y[1]-y[0])/(x[1]-x[0])-yp1);
  }
  for (i=1;i<n-1;i++) 
  {
    sig=(x[i]-x[i-1])/(x[i+1]-x[i-1]);
    p=sig*y2[i-1]+2.0;
    y2[i]=(sig-1.0)/p;
    u[i]=(y[i+1]-y[i])/(x[i+1]-x[i]) - (y[i]-y[i-1])/(x[i]-x[i-1]);
    u[i]=(6.0*u[i]/(x[i+1]-x[i-1])-sig*u[i-1])/p;
  }
  if (ypn > 0.99e30)
    qn=un=0.0;
  else {
    qn=0.5;
    un=(3.0/(x[n-1]-x[n-2]))*(ypn-(y[n-1]-y[n-2])/(x[n-1]-x[n-2]));
  }
  y2[n-1]=(un-qn*u[n-2])/(qn*y2[n-2]+1.0);
  for (k=n-2;k>=0;k--)
    y2[k]=y2[k]*y2[k+1]+u[k];
  FREE_VECTOR(u);
}

/* calculates the value of the interpolant and its first two derivatives */
/* after the second derivatives have been calculated and stored in y2a */

void SPLINE_EVAL(double xa[], double ya[], double y2a[], int n, double x,
	    double *y, double *dy, double *d2y) 
{
  static int i;
  static double *xa_old;
  int ip1;
  double A, B, h, x_i, x_ip1, y_i, y_ip1, y2_i, y2_ip1;

  /* check to make sure that x is in the interpolation interval */
  if (x<xa[0] || x > xa[n-1])
    MATHERR("x value outside range of interpolation grid in splint", 0);

  /* start search for interpolation interval at i=0 if this is a new */
  /* interpolation grid */
  if (xa_old != xa)
  {
    i=0;
    xa_old = xa;
  }

  /* find the correct interpolation interval using the saved value of i */
  if (x < xa[i])
    do {--i;} while (x < xa[i]);
  else if (x> xa[i+1])
    do {++i;} while (x > xa[i+1]);

  ip1 = i+1;
  x_i = xa[i];
  x_ip1 = xa[ip1];
  y_i = ya[i];
  y_ip1 = ya[ip1];
  y2_i = y2a[i];
  y2_ip1 = y2a[ip1];
  
  h = x_ip1 - x_i;
  A = (x_ip1 - x)/h;
  B = 1.0 - A;

  if (y)
    *y= A*y_i + B*y_ip1 + (h*h/6.0)*((A*A*A-A)*y2_i + (B*B*B-B)*y2_ip1);
  if (dy)
    *dy = (y_ip1 - y_i)/h - (h/6.0)*((3*A*A-1.0)*y2_i -
				   (3*B*B-1.0)*y2_ip1);
  if (d2y)
    *d2y = A*y2_i + B*y2_ip1;
}
