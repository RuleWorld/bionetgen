#include "mathutils.h"

/* returns (a-b)^2 where a and b are n dimensional vectors */

double VECTOR_DIST( double *a, double *b, int dim){
    register double *aptr, *bptr;
    double diff;
    double sum=0.0;

    for (aptr=a, bptr=b; aptr<a+dim; ++aptr, ++bptr){
	diff = *aptr-*bptr;
	sum += diff*diff;
    }
    return (sum);
}
