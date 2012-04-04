#include "mathutils.h"

		    
/*==========================================================================*/
/* Routine to Solve COMPLEX  set of linear equations a.x=b where a is
symmetric.  a is overwritten by an appropriate factorization of a
and b is overwritten by x. Note that the rhs b are stored in the
ROWS of b rather than the columns because of the way Fortran
accesses arrays. */

int ZSYM_LINSOLVE(dcomplex **a, dcomplex **b, int nrhs, int dim){
#ifdef APPEND_UNDERSCORE
#define ZSYSV zsysv_
#else
#define ZSYSV zsysv
#endif
    int *ipiv;
    int Nrhs, Dim;
    int info;
    static dcomplex *work;
    static int lwork;
    static int olddim;
    static int initflag=1;
    extern void ZSYSV();
    
    if (initflag){
	initflag = 0;
	olddim = dim;
	lwork = dim*64; /* a rather large blocksize */
	work =  ZALLOC_VECTOR(lwork);
    }
    
    if ( dim > olddim){ /* dimension has increased */
	olddim = dim;
	ZFREE_VECTOR(work);
	lwork = dim*64; /* a rather large blocksize */
	work =  ZALLOC_VECTOR(lwork);
    }
  
    Dim= dim;
    Nrhs= nrhs;
    ipiv = (int *) malloc (sizeof(int)*Dim);
    ZSYSV("l", &Dim, &Nrhs, a[0], &Dim, ipiv, b[0], &Dim, work, &lwork,
	   &info);
    
    free(ipiv);
    return(info);
}

