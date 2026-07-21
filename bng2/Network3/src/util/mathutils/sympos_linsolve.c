#include "mathutils.h"

		    
/*==========================================================================*/
/* Routine to Solve set of linear equations a.x=b where a is symmetric */
/* and positive definite. */
/* a is overwritten by an appropriate factorization of a and b is       */
/* overwritten by x.  Note that the rhs b are stored in the ROWS of b */
/* rather than the columns because of the way Fortran accesses arrays. */

int SYMPOS_LINSOLVE(double **a, double **b, int nrhs, int dim){
#ifdef APPEND_UNDERSCORE
#define DPOSV dposv_
#else
#define DPOSV dposv
#endif
    int Nrhs, Dim;
    int info;
#ifdef __cplusplus
extern "C" {
#endif
    /* Fortran dposv: SUBROUTINE DPOSV( UPLO, N, NRHS, A, LDA, B, LDB, INFO ) */
    void DPOSV(char *uplo, int *n, int *nrhs, double *a, int *lda, double *b, int *ldb, int *info);
#ifdef __cplusplus
}
#endif
    
    Dim= dim;
    Nrhs= nrhs;
    DPOSV("l", &Dim, &Nrhs, a[0], &Dim, b[0], &Dim, &info);
    
    return(info);
}
