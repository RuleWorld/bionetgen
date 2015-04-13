#include "mathutils.h"

		    
/*==========================================================================*/
/* Routine to Solve set of linear equations a.x=b where a is symmetric. */
/* Note that the rhs b are stored in the ROWS of b */
/* rather than the columns because of the way Fortran accesses arrays. */


int LINSOLVE(double **a, double **b, int nrhs, int dim){
#ifdef APPEND_UNDERSCORE
#define DGESV dgesv_
#else
#define DGESV dgesv
#endif
    register int i,j;
    int *ipiv;
    int Nrhs, Dim;
    int info;
    extern void DGESV();
    
    /* calculate transpose of a-- this is done because matrices in Fortran */
    /* and C have row and column indices interchanged */
    MATRIX_TRANSPOSE(a,dim);
    Dim= dim;
    Nrhs= nrhs;
    ipiv = (int *) malloc (sizeof(int)*Dim);
    DGESV(&Dim, &Nrhs, a[0], &Dim, ipiv, b[0], &Dim, &info);
    free(ipiv);
    return(info);
}


