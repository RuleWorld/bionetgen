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
    extern void DPOSV();
    
    Dim= dim;
    Nrhs= nrhs;
    DPOSV("l", &Dim, &Nrhs, a[0], &Dim, b[0], &Dim, &info);
    
    return(info);
}
