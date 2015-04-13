#include "mathutils.h"

/*  Calculates (I - P).v where I is a prection operator defined by the m basis */
/*  vectors.  basis is an mxn matrix . v is a vector with n elts */

void PROJECT( double *v, int n, double **basis, int m){
    register int p;
    double *P_v, *e_p;

    P_v = ALLOC_VECTOR(n);
    INIT_VECTOR(P_v, 0.0, n);
    /* calculate P.v, the projection of P onto the orthormal basis */
    for(p=0; p<m; ++p){
	e_p = basis[p];
	VECTOR_ADD(P_v, DOTP(e_p, v, n), e_p, P_v, n);
    }
    /* Subtract the projection P.v from v to obtain a new vector orthogonal to */
    /* the basis */
    VECTOR_ADD(v, -1.0, P_v, v, n);

    FREE_VECTOR(P_v);
}

