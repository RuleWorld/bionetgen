#include "mathutils.h"
/* returns the transpose a rectangular matrix */

double **RECT_TRANSPOSE(double **A, int m, int n){
    double **At, *Aptr, *Atptr;
    register int i,j;

    At= ALLOC_MATRIX(n,m);
    for(i=0; i<m; ++i){
	for(j=0; j<n; ++j)
	    At[j][i] = A[i][j];
    }
   
    return(At);
}


/*	for(Aptr=A[i], Atptr=*At+i; Aptr<A[i]+n; ++Aptr, Atptr+=m)
	    *Atptr = *Aptr;*/


