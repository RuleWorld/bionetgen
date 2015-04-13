#include "mathutils.h"

/* A general all-purpose integrator from Numerical Recipes (see p. 140 of the */
/* 2nd edition in C */

#define JMAX 20
#define JMAXP (JMAX+1)
#define K 5
static void POLINT(double xa[], double ya[], int n, double x, double *y,
		       double *dy); 
static double trapzd(double (*func)(double), double a, double b, int n);

double INTEGRATE(double (*func)(double), double a, double b, double tol){
    double ss,dss;
    double s[JMAXP+1],h[JMAXP+1];
    int j;

    h[1]=1.0;
    for (j=1;j<=JMAX;j++) {
	s[j]=trapzd(func,a,b,j);
	if (j >= K) {
	    POLINT(&h[j-K],&s[j-K],K,0.0,&ss,&dss);
	    if (fabs(dss) < tol*fabs(ss)) return ss;
	}
	s[j+1]=s[j];
	h[j+1]=0.25*h[j];
    }
    MATHERR("Too many steps in routine INTEGRATE", 00);
    return 0.0;
}
#undef JMAX
#undef JMAXP
#undef K

static void POLINT(double xa[], double ya[], int n, double x, double *y,
		   double *dy){  
    int i,m,ns=1;
    double den,dif,dift,ho,hp,w;
    double *c,*d;

    dif=fabs(x-xa[1]);
    c= ALLOC_VECTOR(n) - 1;
    d= ALLOC_VECTOR(n) - 1;
    for (i=1;i<=n;i++) {
	if ( (dift=fabs(x-xa[i])) < dif) {
	    ns=i;
	    dif=dift;
	}
	c[i]=ya[i];
	d[i]=ya[i];
    }
    *y=ya[ns--];
    for (m=1;m<n;m++) {
	for (i=1;i<=n-m;i++) {
	    ho=xa[i]-x;
	    hp=xa[i+m]-x;
	    w=c[i+1]-d[i];
	    if ( (den=ho-hp) == 0.0) MATHERR("Error in routine POLINT",00);
	    den=w/den;
	    d[i]=hp*den;
	    c[i]=ho*den;
	}
	*y += (*dy=(2*ns < (n-m) ? c[ns+1] : d[ns--]));
    }
    FREE_VECTOR(d+1);
    FREE_VECTOR(c+1);
}

#define FUNC(x) ((*func)(x))

static double trapzd(double (*func)(double), double a, double b, int n){
    double x,tnm,sum,del;
    static double s;
    int it,j;
    
    if (n == 1) {
	return (s=0.5*(b-a)*(FUNC(a)+FUNC(b)));
    } else {
	for (it=1,j=1;j<n-1;j++) it <<= 1;
	tnm=it;
	del=(b-a)/tnm;
	x=a+0.5*del;
	for (sum=0.0,j=1;j<=it;j++,x+=del) sum += FUNC(x);
	s=0.5*(s+(b-a)*sum/tnm);
	return s;
    }
}
#undef FUNC


