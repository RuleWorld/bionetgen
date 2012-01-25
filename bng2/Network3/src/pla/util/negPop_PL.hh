/*
 * negPop_PL.hh
 *
 *  Created on: Feb 15, 2011
 *      Author: Leonard Harris
 */

#ifndef NEGPOP_PL_HH_
#define NEGPOP_PL_HH_

#include "binomialCorrector_PL.hh"

namespace network3{

	class NegPop_PL : public BinomialCorrector_PL{
	public:
	//	NegPop_PL();
		NegPop_PL(double p, vector<SimpleSpecies*>& sp, vector<Reaction*>& rxn);
		NegPop_PL(const NegPop_PL& pl);
		virtual ~NegPop_PL();
		virtual bool check();
		virtual PostleapChecker* clone() const{ return new NegPop_PL(*this); }
	private:
		vector<SimpleSpecies*>& sp;
	};
}

#endif /* NEGPOP_PL_HH_ */
