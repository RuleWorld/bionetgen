#include "mathutils.h"

		    
/*==========================================================================*/
/* Routine to Solve set of linear equations a.x=b where a is symmetric. */
/* a is overwritten by an appropriate factorization of a and b is       */
/* overwritten by x. Note that the rhs b are stored in the ROWS of b */
/* rather than the columns because of the way Fortran accesses arrays. */

int SYM_LINSOLVE(double **A, double **B, int nrhs, int dim){
#ifdef APPEND_UNDERSCORE
#define DSYSV dsysv_
#else
#define DSYSV dsysv
#endif
    int *ipiv;
    int Nrhs, Dim;
    int info;

    register int i;
    double det;
    double a,b,c,*x,*y,cx_ay;
    double detinv, ainv;
    
    static double *work;
    static int lwork;
    static int olddim;
    static int initflag=1;
    extern void DSYSV();

    if(dim==2){
	a= A[0][0];  c=A[0][1]; b = A[1][1];
	det= a*b - c*c;
	detinv=1.0/det;
	ainv= 1.0/a;
	for (i=0; i<nrhs; ++i){
	    x= B[i];
	    y= x+1;
	    cx_ay= c*(*x) - a*(*y);
	    *y = -cx_ay*detinv;
	    *x = (*x - c*(*y))*ainv;
	}
	return(0);
    }

    if (initflag){
	initflag = 0;
	olddim = dim;
	lwork = dim*64; /* a rather large blocksize */
	work =  ALLOC_VECTOR(lwork);
    }
  
    if ( dim > olddim){ /* dimension has increased */
	olddim = dim;
	FREE_VECTOR(work);
	lwork = dim*64; /* a rather large blocksize */
	work =  ALLOC_VECTOR(lwork);
    }
    
    Dim= dim;
    Nrhs= nrhs;
    ipiv = (int *) malloc (sizeof(int)*Dim);
    DSYSV("l", &Dim, &Nrhs, A[0], &Dim, ipiv, B[0], &Dim, work, &lwork,
	   &info);
    
    free(ipiv);
    return(info);
}


