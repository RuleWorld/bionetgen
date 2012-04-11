#include "mathutils.h"
#include <time.h>

/* generates a uniformly random number on the interval (min, max) */ 

#define ONE_RAND_MAX 4.6566128752457969e-10
 
static int initflag=1;
    
double RANDOM( double min, double max){
    /* seed random number generator from the current time */
    if (initflag){
	/* default seed based on time */
	srandom( (int) time(NULL));
	initflag=0;
    }

    return( (max-min)*ONE_RAND_MAX*random() + min);
}

void SEED_RANDOM( int seed){
    srandom( seed);
    initflag = 0;
}
