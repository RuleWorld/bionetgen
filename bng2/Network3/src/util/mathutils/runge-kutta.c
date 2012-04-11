#include "mathutils.h"

/* Take a single Runge-Kutta quality-controlled step with a fractional */
/* accuracy eps.  Adapted from Numerical Recipes 2nd ed. in C p. 719. */

/* KEY */
/* y       array containing the intial values */
/* dydx    array containing the initial derivatives */
/* n       dimension of y */
/* x       initial value of the dependent variable. Updated on exit. */
/* htry    stepsize to attempt */
/* eps     the fractional accuracy required */
/* yscal   (optional) array containing values to scale error against--- see */
/*            reference above for discussion.  Set to default value if NULL */
/* hdid    size of step actually taken */
/* hnext   size of next step to take based on current error estimate */
/* derivs  function to evaluate the derivatives of y at x */

#define SAFETY 0.9
#define PGROW -0.2
#define PSHRNK -0.25
#define ERRCON 1.89e-4
#define TINY 1.0e-10

void RKQS(double y[], double dydx[], int n, double *x, double htry, 
	  double eps, double yscal[], double *hdid, double *hnext,
	  void (*derivs)(double, double [], double [])){
    register int i;
    double errmax, h, hnew, xnew, *yerr, *ytemp, *yscale;

    yerr=ALLOC_VECTOR(n);
    ytemp=ALLOC_VECTOR(n);
    /* allocate space for yscale if an array has not been passed--- allows */
    /* default value to be set */
    if (yscal == NULL)
	yscale = ALLOC_VECTOR(n);
    else
	yscale = yscal;
    h=htry;
    
    for (;;){
	RKCK(y,dydx,n,*x,h,ytemp,yerr,derivs);
	/* default value for yscale */
	if (yscale != yscal)
	    for(i=0; i<n; ++i)
		yscale[i] = fabs(y[i]) + fabs(dydx[i]*h) + TINY;
	/* determine maximum error from the step */
	errmax=0.0;
	for (i=0;i<n;i++) errmax=MAX(errmax,fabs(yerr[i]/yscale[i]));
	errmax /= eps;
	if (errmax > 1.0){
	    /* reject the step, error too large */
	    hnew=SAFETY*h*pow(errmax,PSHRNK); /* new step size */
	    if (hnew < 0.1*h) hnew = 0.1*h;   /* no more than 10x decrease */
	    h = hnew;
	    xnew=(*x)+h;
	    if (xnew == *x) MATHERR("stepsize underflow in RKQS",0);
	    continue;
	} else {
	    /* accept the step */
	    if (errmax > ERRCON) *hnext=SAFETY*h*pow(errmax,PGROW);
	    else *hnext=5.0*h; /* increase the stepsize by no more than 5x */
	    *x += (*hdid=h);
	    COPY_VECTOR( ytemp, y, n);
	    break;
	}
    }
    FREE_VECTOR(ytemp);
    FREE_VECTOR(yerr);
    if (yscale != yscal)
	FREE_VECTOR(yscale);
}
#undef SAFETY
#undef PGROW
#undef PSHRNK
#undef ERRCON

/* Cash-Karp Runge-Kutta 5th order integrator adapted from Numerical */
/* Recipes 2nd ed. in C p. 720.  Use as part of adaptive integration */
/* scheme with drive routine RKQS. */

/* KEY */
/* y       array containing the intial values */
/* dydx    array containing the initial derivatives */
/* n       dimension of y */
/* x       initial value of the dependent variable */
/* h       stepsize */
/* yout    array containing the final values--- may be the same as y */
/* yerr    (optional) array containing error estimates; used only if non-NULL */
/* derivs  function to evaluate the derivatives of y at x */

void RKCK(double *y, double *dydx, int n, double x, double h, double
	  *yout, double *yerr, void (*derivs)(double, double *, double *)){
    register int i;
    static double a2=0.2,a3=0.3,a4=0.6,a5=1.0,a6=0.875,b21=0.2,
	b31=3.0/40.0,b32=9.0/40.0,b41=0.3,b42 = -0.9,b43=1.2,
	b51 = -11.0/54.0, b52=2.5,b53 = -70.0/27.0,b54=35.0/27.0,
	b61=1631.0/55296.0,b62=175.0/512.0,b63=575.0/13824.0,
	b64=44275.0/110592.0,b65=253.0/4096.0,c1=37.0/378.0,
	c3=250.0/621.0,c4=125.0/594.0,c6=512.0/1771.0,
	dc5 = -277.0/14336.0;
    double dc1=c1-2825.0/27648.0,dc3=c3-18575.0/48384.0,
	dc4=c4-13525.0/55296.0,dc6=c6-0.25;
    double *ytemp;
    double *ak2,*ak3,*ak4,*ak5,*ak6;

    ak2=ALLOC_VECTOR(n);
    ak3=ALLOC_VECTOR(n);
    ak4=ALLOC_VECTOR(n);
    ak5=ALLOC_VECTOR(n);
    ak6=ALLOC_VECTOR(n);
    ytemp=ALLOC_VECTOR(n);

    for (i=0;i<n;i++)               /* first step */
	ytemp[i]=y[i]+b21*h*dydx[i];

    (*derivs)(x+a2*h,ytemp,ak2);   /* second step */
    for (i=0;i<n;i++)  
	ytemp[i]=y[i]+h*(b31*dydx[i]+b32*ak2[i]);

    (*derivs)(x+a3*h,ytemp,ak3);   /* third step */
    for (i=0;i<n;i++)
	ytemp[i]=y[i]+h*(b41*dydx[i]+b42*ak2[i]+b43*ak3[i]);
	
    (*derivs)(x+a4*h,ytemp,ak4);  /* fourth step */
    for (i=0;i<n;i++)
	ytemp[i]=y[i]+h*(b51*dydx[i]+b52*ak2[i]+b53*ak3[i]+b54*ak4[i]);
	
    (*derivs)(x+a5*h,ytemp,ak5);  /* fifth step */
    for (i=0;i<n;i++)
	ytemp[i]=y[i]+
	    h*(b61*dydx[i]+b62*ak2[i]+b63*ak3[i]+b64*ak4[i]+b65*ak5[i]);
  
    (*derivs)(x+a6*h,ytemp,ak6);  /* sixth step */
    for (i=0;i<n;i++)
	yout[i]=y[i]+h*(c1*dydx[i]+c3*ak3[i]+c4*ak4[i]+c6*ak6[i]);
  
    if (yerr != NULL)
	/* Estimate error as difference between 4th and */
	/* 5th order methods */
	for (i=0;i<n;i++)
	    yerr[i]=
		h*(dc1*dydx[i]+dc3*ak3[i]+dc4*ak4[i]+dc5*ak5[i]+dc6*ak6[i]);

    /* free allocated space */
    FREE_VECTOR(ak2);
    FREE_VECTOR(ak3);
    FREE_VECTOR(ak4);
    FREE_VECTOR(ak5);
    FREE_VECTOR(ak6);
    FREE_VECTOR(ytemp);
}













