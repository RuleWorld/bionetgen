#include "mathutils.h"

main(){
    int i,n;
    int incx=1;
    double *x;

    /* read n */
    scanf("%d", &n);
    /* allocate space for x */
    x = (double *) malloc(n*sizeof(double));
    /* read x */
    if (n<0){ fprintf(stderr,"n must be greater than 0.\n"); exit(1);}
    for (i=0; i<n; ++i)
	scanf("%lf", x+i);
    /* print norm */
    printf("dnormsq=%#.16g\n", NORMSQ(x,n));
}
