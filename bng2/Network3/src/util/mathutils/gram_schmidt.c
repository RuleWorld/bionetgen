#include "mathutils.h"

/* orthogonalize m n-dimensional vectors using the Gram-Schmidt algorithm. */
/* This method is not very stable so it's much better to use another method */
/* e.g. SVD if m is large or there is any hint of linear dependence.  N.B. the */
/* returned vectors are ORTHONORMAL. */

void GRAM_SCHMIDT(double **V, int m, int n){
    register int p, pp;
    double *e_p, *e_p0, *e_pp;
    double norm;

    if(m > n){
	MATHERR("GRAM_SCHMIDT: Number of rows must be <= number columns.\n",
		00);
    }
    e_p = ALLOC_VECTOR(n);
    for(p=0; p<m; ++p){
	e_p0 = V[p];
	/* initial value of e_p */
	COPY_VECTOR(e_p0, e_p, n);
	/* make e_p orthogonal with previous basis vectors */
	for(pp=0; pp<p; ++pp){
	    e_pp=V[pp];
	    VECTOR_ADD(e_p, -DOTP(e_pp, e_p0, n), e_pp, e_p, n);
	}
	/* normalize e_p */
	norm = NORM(e_p,n);
	SCALE_VECTOR(1.0/norm, e_p, n);
	/* copy e_p back into V */
	COPY_VECTOR(e_p, e_p0, n);
    }
    FREE_VECTOR(e_p);
}


