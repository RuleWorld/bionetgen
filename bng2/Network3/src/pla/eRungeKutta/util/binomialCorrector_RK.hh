/*
 * binomialCorrector_RK.hh
 *
 *  Created on: Mar 22, 2012
 *      Author: Leonard Harris
 */

#ifndef BINOMIALCORRECTOR_RK_HH_
#define BINOMIALCORRECTOR_RK_HH_

#include "../../../model/reaction.hh"
#include "../../../util/util.hh"
#include "aEff_Calculator.hh"

namespace network3{

	class BinomialCorrector_RK{
	public:
	//	BinomialCorrector_RK();
		BinomialCorrector_RK(double p, vector<Reaction*>& rxn);
		BinomialCorrector_RK(const BinomialCorrector_RK& bc);
		~BinomialCorrector_RK();
		void correct(vector<double>& k, aEff_Calculator* aCalc, double& tau);
	protected:
		double p;
	private:
		vector<Reaction*>& rxn;
	};
}

#endif /* BINOMIALCORRECTOR_RK_HH_ */
