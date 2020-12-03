#include "mathutils.h"

/* Find eigenvalues and optionally eigenvectors (default) of a real symmetric
   matrix using LAPACK routine dsyev.  On exit, A is overwritten with the
   eigenvectors (stored in rows) if they are requested, or, if not, A is
   destroyed. By default, only the upper triangle of A is referenced. */

#define DSYEV FNAME(dsyev)
#define DSYGV FNAME(dsygv)

int SYM_EIGENSYSTEM( double **A, double *evals, int dim, 
		     LAPACK_OPTIONS *options){ 
    int N=dim;
    char uplo, jobz;
    int info;
    int lwork;
    double *work;
    LAPACK_OPTIONS *opt, local_options={'L', 'V'};
    void DSYEV(char*,char*,int*,double*,int*,double*,double*,int*,int*);

    /* set options to LAPACK routine */
    opt= (options) ? options : &local_options;
    uplo=opt->uplo;
    jobz=opt->jobz;

    lwork = (BLOCKSIZE+2)*dim; 
    work =  ALLOC_VECTOR(lwork);

    /* call eigensolver */
    N = dim;
    DSYEV(&jobz, &uplo, &N, A[0], &N, evals, work, &lwork, &info);

    /* free space */
    FREE_VECTOR(work);

    return(info);
}

/* General symmetric eigensolver routine using LAPACK dsygv */
/* only the upper triangle of A is referenced.  On exit, A is overwritten and B
   is replaced with its Cholesky decomposition.*/

int GEN_SYM_EIGENSYSTEM( double **A, double **B, double *evals, int dim,
			 LAPACK_OPTIONS *options){
    int N=dim;
    int info, itype;
    char uplo, jobz;
    int lwork;
    double *work;
    LAPACK_OPTIONS *opt, local_options={'L', 'V', 1};
    void DSYGV(int*,char*,char*,int*,double*,int*,double*,int*,double*,double*,int*,int*);

    /* set options to LAPACK routine */
    opt= (options) ? options : &local_options;
    uplo=opt->uplo;
    jobz=opt->jobz;
    itype=opt->itype;

    lwork = (BLOCKSIZE+2)*dim; 
    work =  ALLOC_VECTOR(lwork);

    /* call eigensolver */
    DSYGV(&itype, &jobz, &uplo, &N, A[0], &N, B[0], &N, evals, work,
		 &lwork, &info); 

    /* free space */
    FREE_VECTOR(work);

    return(info);
}


