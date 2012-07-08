/*
 * rand2.cpp
 *
 *  Created on: Feb 14, 2011
 *      Author: Leonard Harris
 */

#include <cmath>
#include "rand2.hh"

#define PI 3.141592654

//using namespace Util;

// Returns the value ln[Gamma(xx)] for xx > 0.
double gammln(double xx){
	// Internal arithmetic will be done in double precision, a nicety that you can omit if five-figure accuracy is good enough.
	double x,y,tmp,ser;
	static double cof[6]={76.18009172947146,-86.50532032941677, 24.01409824083091,-1.231739572450155, 0.1208650973866179e-2,
						  -0.5395239384953e-5};
	int j;
	y=x=xx;
	tmp=x+5.5;
	tmp -= (x+0.5)*log(tmp);
	ser=1.000000000190015;
	for (j=0;j<=5;j++) ser += cof[j]/++y;
	return -tmp+log(2.5066282746310005*ser/x);
}

// Returns as a floating-point number an integer value that is a random deviate drawn from a
// Poisson distribution of mean xm, using ran1(idum) as a source of uniform random deviates.
//double poidev(double xm, long *idum){
double Util::RANDOM_POISSON(double xm){
//	double gammln(double xx);
	//float ran1(long *idum);
	static double sq,alxm,g,oldm=(-1.0); /// oldm is a flag for whether xm has changed since last call.
	double em,t,y;
	if (xm < 12.0) { // Use direct method.
		if (xm != oldm) {
			oldm=xm;
			g=exp(-xm); // If xm is new, compute the exponential.
		}
		em = -1;
		t=1.0;
		do { // Instead of adding exponential deviates it is equivalent to multiply uniform deviates. We never
			 // actually have to take the log, merely compare to the pre-computed exponential.
			++em;
//			t *= ran1(idum);
			t *= Util::RANDOM_CLOSED();
		} while (t > g);
	} else { // Use rejection method.
		if (xm != oldm) { // If xm has changed since the last call, then precompute some functions that occur below.
			oldm=xm;
			sq=sqrt(2.0*xm);
			alxm=log(xm);
			g=xm*alxm-gammln(xm+1.0);
			// The function gammln is the natural log of the gamma function, as given in ¤6.1.
		}
		do {
			do { // y is a deviate from a Lorentzian comparison function.
//				y=tan(PI*ran1(idum));
				y=tan(PI*Util::RANDOM_CLOSED());
				em=sq*y+xm; // em is y, shifted and scaled.
			} while (em < 0.0); // Reject if in regime of zero probability.
			em=floor(em); // The trick for integer-valued distributions.
			t=0.9*(1.0+y*y)*exp(em*alxm-gammln(em+1.0)-g);
			// The ratio of the desired distribution to the comparison function; we accept or
			// reject by comparing it to another uniform deviate. The factor 0.9 is chosen so
			// that t never exceeds 1.
//		} while (ran1(idum) > t);
		} while (Util::RANDOM_CLOSED() > t);
	}
	return em;
}

// Returns as a floating-point number an integer value that is a random deviate drawn from a binomial distribution of n trials
// each of probability pp, using ran1(idum) as a source of uniform random deviates.
//double Util::RANDOM_BINOMIAL(double pp, int n){
double Util::RANDOM_BINOMIAL(double pp, double n){
//	double gammln(double xx);
	//float ran1(long *idum);
	int j;
//	static int nold=(-1);
	static double nold=(-1);
	double am,em,g,angle,p,bnl,sq,t,y;
	static double pold=(-1.0),pc,plog,pclog,en,oldg;
	p=(pp <= 0.5 ? pp : 1.0-pp);
//	The binomial distribution is invariant under changing pp to 1-pp, if we also change the
//	answer to n minus itself; weÕll remember to do this below.
	am=n*p; // This is the mean of the deviate to be produced.
	if (n < 25) { // Use the direct method while n is not too large.
		bnl=0.0; // This can require up to 25 calls to ran1.
		for (j=1;j<=n;j++)
//			if (ran1(idum) < p) ++bnl;
			if (Util::RANDOM_CLOSED() < p) ++bnl;
	}
	else if (am < 1.0) { // If fewer than one event is expected out of 25 or more trials, then the
						 // distribution is quite accurately Poisson. Use direct Poisson method.
		g=exp(-am);
		t=1.0;
		for (j=0;j<=n;j++) {
//			t *= ran1(idum);
			t *= Util::RANDOM_CLOSED();
			if (t < g) break;
		}
		bnl=(j <= n ? j : n);
	}
	else { // Use the rejection method.
		if (n != nold) { // If n has changed, then compute useful quantities.
			en=n;
			oldg=gammln(en+1.0);
			nold=n;
		}
		if (p != pold) { // If p has changed, then compute useful quantities.
			pc=1.0-p;
			plog=log(p);
			pclog=log(pc);
			pold=p;
		}
		sq=sqrt(2.0*am*pc); // The following code should by now seem familiar:
							// Rejection method with a Lorentzian comparison function.
		do {
			do {
//				angle=PI*ran1(idum);
				angle=PI*Util::RANDOM_CLOSED();
				y=tan(angle);
				em=sq*y+am;
			} while (em < 0.0 || em >= (en+1.0)); // Reject.
			em=floor(em); // Trick for integer-valued distribution.
			t=1.2*sq*(1.0+y*y)*exp(oldg-gammln(em+1.0)-gammln(en-em+1.0)+em*plog+(en-em)*pclog);
//		} while (ran1(idum) > t); // Reject. This happens about 1.5 times per deviate, on average.
		} while (Util::RANDOM_CLOSED() > t);
		bnl=em;
	}
	if (p != pp) bnl=n-bnl; // Remember to undo the symmetry transformation.
	return bnl;
}
