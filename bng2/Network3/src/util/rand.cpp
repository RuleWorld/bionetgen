/**
 * @file rand.cpp
 *
 * Implementation of the random number functions, uses the Mersenne Twister
 * pseudo-random number generator implementation
 * (http://www.bedaux.net/mtrand/) from the paper:
 *     M. Matsumoto and T. Nishimura,
 *     Mersenne Twister: A 623-Dimensionally Equidistributed
 *     Uniform Pseudo-Random Number Generator,
 *     ACM Transactions on Modeling and Computer Simulation,
 *     Vol. 8, No. 1, January 1998, pp. 3-30
 *
 * @date Oct 14th, 2009   last edited: Oct 14th, 2009
 *
 * @author Michael Sneddon
 */


#include "MTrand/mtrand.h"
#include "rand.hh"

/* files needed to seed and compute random numbers */
#include <time.h>
#include <cstdlib>
#include <math.h>

using namespace Util;

static int initflag=1;
static bool haveNextGaussian=false;
static double nextGaussian = 0;

static MTRand_int32 iRand;
static MTRand dRand;
static MTRand_closed dRandClosed;
static MTRand_open dRandOpen;



/* Return a random double on the range (0,max] */
double Util::RANDOM( double max )
{
	if (initflag) {
		iRand.seed( (int) time(NULL));
		initflag=0;
    }

	/* dRand() gives a uniform double on the interval [0,1).  But
	 * for our purposes, we want a double value (0,1] so that if a reaction class
	 * has propensity (a) equal to zero and is the first in the list, it
	 * can never be fired.  Thus, we always want something larger than 0, but
	 * it can equal 1.  This then is just 1-dRand().  To get the correct range,
	 * we multiply by the max value.  This is what I do here: */

	return ( (1-dRand()) * max );
}

/* Return a random double on the range (0,1) */
double Util::RANDOM_CLOSED()
{
	if (initflag) {
		iRand.seed( (int) time(NULL));
		initflag=0;
    }
	return dRandClosed();
}


/* Returns a random normally distributed number */
double Util::RANDOM_GAUSSIAN()
{
	if (initflag) {
		iRand.seed( (int) time(NULL));
		initflag=0;
    }
    if(haveNextGaussian)
    {
    	haveNextGaussian = false;
    	return nextGaussian;
    }

	double v1=0, v2=0, s=0;
	do {
		v1 = 2 * dRandOpen()-1;
		v2 = 2 * dRandOpen()-1;
		s=v1*v1 + v2*v2;
	} while (s>=1 || s==0);

	double multiplier = sqrt(-2*log(s)/s);
	nextGaussian = v2*multiplier;
	haveNextGaussian = true;
	return v1*multiplier;
}


/* Returns a random positive integer on the range [min, max) */
int Util::RANDOM_INT(unsigned long min, unsigned long max)
{
	if (initflag) {
		iRand.seed( (int) time(NULL));
		initflag=0;
    }
	return ( min+int((max-min)*dRand()) );
}


/* Seed the number generator with a positive 32 bit integer */
void Util::SEED_RANDOM( unsigned long seedInt ){
    iRand.seed(seedInt);
    initflag = 0;
}
