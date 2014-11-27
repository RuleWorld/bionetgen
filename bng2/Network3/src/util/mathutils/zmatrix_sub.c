#include "mathutils.h"
		    
/*==========================================================================*/
/* Compute c=a-b, where a, b, and c are double complex matrices */

void ZMATRIX_SUB(dcomplex **a, dcomplex **b, dcomplex **c, int dim){
    register int i,j;
    register double *aptr, *bptr, *cptr;
    
    for(i=0; i<dim; ++i){
	aptr=(double *) a[i];
	bptr=(double *) b[i];
	cptr=(double *) c[i];
	for(j=0; j<2*dim; ++j, ++aptr, ++bptr, ++cptr)
	    *cptr = *aptr - *bptr;
    }
}

#ifdef TESTMAIN
#include <stdlib.h>                
#include <stdio.h>                
main(){
    int i,j,n;
    int incx=1;
    dcomplex **a, **b, *x;

    /* read n */
    scanf("%d", &n);
    /* allocate space for x */
    a = ZALLOC_MATRIX(n,n);
    b = ZALLOC_MATRIX(n,n);
    /* read a and b */
    if (n<0){ fprintf(stderr,"n must be greater than 0.\n"); exit(1);}
    x= a[0];
    for (i=0; i<n*n; ++i)
	scanf("%lf %lf", &x[i].r, &x[i].i);
    x= b[0];
    for (i=0; i<n*n; ++i)
	scanf("%lf %lf", &x[i].r, &x[i].i);

    /* call matrix subtraction */
    ZMATRIX_SUB(a,b,a,n);
    for(i=0; i<n;++i){
	for(j=0;j<n;++j){
	    printf("%#.16g %#.16g\n", a[i][j].r, a[i][j].i);
	}
    }
}
#endif
