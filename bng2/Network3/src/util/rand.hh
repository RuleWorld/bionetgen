/**
 * @file rand.hh
 *
 * A library to generate random numbers.  The functions here use the MTrand
 * library, a fast and small c++ Mersene Twister random number generator.
 *
 * @date Oct 14th, 2009   last edited: Oct 14th, 2009
 *
 * @author Michael Sneddon
 */

#ifndef RAND_HH_
#define RAND_HH_


namespace Util {

	//!  Seeds the random number generator used in all simulations
	/*!
	 * Seed the random number generator with a positive 32bit integer
	 * If you don't call this function, the current time will be used
	 * as a seed so each run will be different.
	 * @param seed the number to seed the random generator
	 * @author Michael Sneddon
	 */
	void SEED_RANDOM( unsigned long  seed );


	//!  Uniform random number on the interval (0,max]
	/*!
	 * This function returns a random double on the half open interval (0,max]
	 * @param max the max value that can be generated, inclusive
	 * @return double the random deviate
	 * @author Michael Sneddon
	*/
	double RANDOM( double max );


	//!  Uniform random number on the interval (0,1)
	/*!
	 * This function returns a random double on the closed interval (0,1)
	 * @return double the random deviate
	 * @author Michael Sneddon
	*/
	double RANDOM_CLOSED();


	//!  Uniform random number on the interval [min,max]
	/*!
	 * This is a multipurpose function that selects a random number
	 * on the half open interval [min, max).  This allows you to quickly
	 * select a random element from a list of known size.  The function
	 * call would look something like: RANDOM_INT(0,sizeOfList).
	 * @param min the minimum number that can be generated, inclusive
	 * @param max the maximum number that can be generated, exclusive
	 * @return int the random integer
	 * @author Michael Sneddon
	*/
	int RANDOM_INT(unsigned long min, unsigned long max);


	//!  Normally distributed random number with mean 0 and variance 1.
	/*!
	 * Returns a normally distributed random number.
	 * Based on the java implementation of Random.nextGaussian, this method uses the
	 * polar method of G. E. P. Box, M. E. Muller, and G. Marsaglia, as described by
	 * Donald E. Knuth in The Art of Computer Programming, Volume 2: Seminumerical
	 * Algorithms, section 3.4.1, subsection C, algorithm P.
	 * @return double normally distributed random number
	 * @author Michael Sneddon
	*/
	double RANDOM_GAUSSIAN();

};


#endif /* RAND_HH_ */
