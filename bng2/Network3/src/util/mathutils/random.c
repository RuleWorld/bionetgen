#include "mathutils.h"
#include <time.h>
#include <stdlib.h>

/* generates a uniformly random number on the interval (min, max) */ 

static int initflag=1;
    
double RANDOM( double min, double max){
    if (initflag){
	srand( (unsigned int) time(NULL));
	initflag=0;
    }
    return( (max-min)*((double)rand()/RAND_MAX) + min);
}

void SEED_RANDOM( int seed){
    srand( (unsigned int)seed);
    initflag = 0;
}
