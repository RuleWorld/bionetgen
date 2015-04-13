/* Numerical recipes routines for bracketing and finding the minimum of a 1d */
/* function */
#include "mathutils.h"

/* a and b, or a, b, and c, may be specified on input.  If only a and b are */
/* given (reasonable), the program makes a reasonable initial guess for c. */

#define GOLD 1.618034
#define GLIMIT 5.0
#define TINY 1.0e-20
#define SHFT(a,b,c,d) (a)=(b);(b)=(c);(c)=(d);
#define MAXTRY 100

void MIN_BRACKET(double *ax, double *bx, double *cx, double *fa,
		 double *fb, double *fc, double (*func)(double)){
    int n_trial=0;
    double ulim,u,r,q,fu,dum;
    
    /* make sure that b > a */
    if (*bx < *ax){
	MATHERR("MIN_BRACKET: b must be greater than a.\n", 00);
    }

    *fa=(*func)(*ax);
    *fb=(*func)(*bx);

    if (*cx<*bx || (*cx-*bx)>GLIMIT*(*bx-*ax)){
	/* make b the downhill direction from a */
	if (*fb > *fa) {
	    SWAP(*bx, *ax);
	    SWAP(*fb, *fa);
	}
	/* make a reasonable initial guess for c */
	*cx=(*bx)+GOLD*(*bx-*ax);
    }

    *fc=(*func)(*cx);
    /* make c the downhill direction from a */
    if (*fc > *fa) {
	SWAP(*cx, *ax);
	SWAP(*fc, *fa);
    }

    while (*fb > *fc) {
	r=(*bx-*ax)*(*fb-*fc);
	q=(*bx-*cx)*(*fb-*fa);
	u=(*bx)-((*bx-*cx)*q-(*bx-*ax)*r)/
	    (2.0*SIGN(MAX(fabs(q-r),TINY),q-r));
	ulim=(*bx)+GLIMIT*(*cx-*bx);
	if ((*bx-u)*(u-*cx) > 0.0) {
	    fu=(*func)(u);
	    if (fu < *fc) {
		*ax=(*bx);
		*bx=u;
		*fa=(*fb);
		*fb=fu;
		return;
	    } else if (fu > *fb) {
		*cx=u;
		*fc=fu;
		return;
	    }
	    u=(*cx)+GOLD*(*cx-*bx);
	    fu=(*func)(u);
	} else if ((*cx-u)*(u-ulim) > 0.0) {
	    fu=(*func)(u);
	    if (fu < *fc) {
		SHFT(*bx,*cx,u,*cx+GOLD*(*cx-*bx));
		SHFT(*fb,*fc,fu,(*func)(u));
	    }
	} else if ((u-ulim)*(ulim-*cx) >= 0.0) {
	    u=ulim;
	    fu=(*func)(u);
	} else {
	    u=(*cx)+GOLD*(*cx-*bx);
	    fu=(*func)(u);
	}
	SHFT(*ax,*bx,*cx,u);
	SHFT(*fa,*fb,*fc,fu);
	++n_trial;
	if (n_trial > MAXTRY){
	    MATHERR("BRACKET_MINIMUM failed.", 00);
	}
    }
}
#define ITMAX 100
#define CGOLD 0.3819660
#define ZEPS 1.0e-10
#define SHFT(a,b,c,d) (a)=(b);(b)=(c);(c)=(d);

double MIN_FIND(double ax, double bx, double cx, double
		(*f)(double), double tol, double *xmin){
    int iter;
    double a,b,d,etemp,fu,fv,fw,fx,p,q,r,tol1,tol2,u,v,w,x,xm;
    double e=0.0;
    
    a=(ax < cx ? ax : cx);
    b=(ax > cx ? ax : cx);
    x=w=v=bx;
    fw=fv=fx=(*f)(x);
    for (iter=1;iter<=ITMAX;iter++) {
	xm=0.5*(a+b);
	tol2=2.0*(tol1=tol*fabs(x)+ZEPS);
	if (fabs(x-xm) <= (tol2-0.5*(b-a))) {
	    *xmin=x;
	    return fx;
	}
	if (fabs(e) > tol1) {
	    r=(x-w)*(fx-fv);
	    q=(x-v)*(fx-fw);
	    p=(x-v)*q-(x-w)*r;
	    q=2.0*(q-r);
	    if (q > 0.0) p = -p;
	    q=fabs(q);
	    etemp=e;
	    e=d;
	    if (fabs(p) >= fabs(0.5*q*etemp) || p <= q*(a-x) || p >= q*(b-x))
		d=CGOLD*(e=(x >= xm ? a-x : b-x));
	    else {
		d=p/q;
		u=x+d;
		if (u-a < tol2 || b-u < tol2)
		    d=SIGN(tol1,xm-x);
	    }
	} else {
	    d=CGOLD*(e=(x >= xm ? a-x : b-x));
	}
	u=(fabs(d) >= tol1 ? x+d : x+SIGN(tol1,d));
	fu=(*f)(u);
	if (fu <= fx) {
	    if (u >= x) a=x; else b=x;
	    SHFT(v,w,x,u);
	    SHFT(fv,fw,fx,fu);
	} else {
	    if (u < x) a=u; else b=u;
	    if (fu <= fw || w == x) {
		v=w;
		w=u;
		fv=fw;
		fw=fu;
	    } else if (fu <= fv || v == x || v == w) {
		v=u;
		fv=fu;
	    }
	}
    }
    MATHERR("Too many iterations in MIN_FIND", 00);
    *xmin=x;
    return fx;
}
#undef ITMAX
#undef CGOLD
#undef ZEPS
#undef SHFT
#undef SIGN


