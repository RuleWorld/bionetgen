/*
 * binomialCorrector_PL.hh
 *
 *  Created on: Apr 27, 2011
 *      Author: Leonard Harris
 */

#ifndef BINOMIALCORRECTOR_PL_HH_
#define BINOMIALCORRECTOR_PL_HH_

#include "../../std_include.hh"
#include "../base/postleapChecker.hh"
#include "../../model/reaction.hh"
#include "../../util/util.hh"

namespace network3{

	class BinomialCorrector_PL : public PostleapChecker{
	public:
	//	BinomialCorrector_PL();
		BinomialCorrector_PL(double p, vector<Reaction*>& rxn);
		BinomialCorrector_PL(const BinomialCorrector_PL& pl);
		virtual ~BinomialCorrector_PL();
		virtual void correct(vector<double>& k, double& tau);
		virtual PostleapChecker* clone() const{ return new BinomialCorrector_PL(*this); }
	protected:
		double p;
	private:
		vector<Reaction*>& rxn;
	};
}

#endif /* BINOMIALCORRECTOR_PL_HH_ */
