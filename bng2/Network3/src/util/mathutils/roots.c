#include "mathutils.h"

#define FACTOR 1.6
#define NTRY 50

int ROOT_BRACKET(double *x1, double *x2, double (*func)(double)){
    int j;
    double f1,f2;

    if (*x1 == *x2) MATHERR("Bad initial range in ROOT_BRACKET", 00);
    f1=(*func)(*x1);
    f2=(*func)(*x2);
    for (j=1;j<=NTRY;j++) {
	if (f1*f2 < 0.0) return (1);
	if (fabs(f1) < fabs(f2))
	    f1=(*func)(*x1 += FACTOR*(*x1-*x2));
	else
	    f2=(*func)(*x2 += FACTOR*(*x2-*x1));
    }
    return (0);
}
#undef FACTOR
#undef NTRY

#define NRANSI
#define EPS 2.0e-16 /* number close to machine precision to set */
		    /* tolerance in root finding functions */
#define ITMAX 100

double ROOT_FIND(double x1, double x2, double (*func)(double), double tol){ 
    int iter;
    double a=x1,b=x2,c=x2,d,e,min1,min2;
    double fa=(*func)(a),fb=(*func)(b),fc,p,q,r,s,tol1,xm;
    
    if ((fa > 0.0 && fb > 0.0) || (fa < 0.0 && fb < 0.0))
	MATHERR("Root must be bracketed in ROOT_FIND", 00);
    fc=fb;
    for (iter=1;iter<=ITMAX;iter++){
	if ((fb > 0.0 && fc > 0.0) || (fb < 0.0 && fc < 0.0)){
	    c=a;
	    fc=fa;
	    e=d=b-a;
	}
	if (fabs(fc) < fabs(fb)) {
	    a=b;
	    b=c;
	    c=a;
	    fa=fb;
	    fb=fc;
	    fc=fa;
	}
	tol1=2.0*EPS*fabs(b)+0.5*tol;
	xm=0.5*(c-b);
	if (fabs(xm) <= tol1 || fb == 0.0) return b;
	if (fabs(e) >= tol1 && fabs(fa) > fabs(fb)) {
	    s=fb/fa;
	    if (a == c) {
		p=2.0*xm*s;
		q=1.0-s;
	    } else {
		q=fa/fc;
		r=fb/fc;
		p=s*(2.0*xm*q*(q-r)-(b-a)*(r-1.0));
		q=(q-1.0)*(r-1.0)*(s-1.0);
	    }
	    if (p > 0.0) q = -q;
	    p=fabs(p);
	    min1=3.0*xm*q-fabs(tol1*q);
	    min2=fabs(e*q);
	    if (2.0*p < (min1 < min2 ? min1 : min2)) {
		e=d;
		d=p/q;
	    } else {
		d=xm;
		e=d;
	    }
	} else {
	    d=xm;
	    e=d;
	}
	a=b;
	fa=fb;
	if (fabs(d) > tol1)
	    b += d;
	else
	    b += SIGN(tol1,xm);
	fb=(*func)(b);
    }
    MATHERR("Maximum number of iterations exceeded in zbrent", 00);
    return 0.0;
}
#undef SIGN
#undef EPS
#undef ITMAX
#undef NRANSI
