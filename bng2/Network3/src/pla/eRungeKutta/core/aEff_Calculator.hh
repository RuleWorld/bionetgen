/*
 * aEff_Calculator.hh
 *
 *  Created on: Apr 30, 2011
 *      Author: Leonard Harris
 */

#ifndef AEFF_CALCULATOR_HH_
#define AEFF_CALCULATOR_HH_

#include "../../../model/reaction.hh"
#include "butcherTableau.hh"

namespace network3{

	class aEff_Calculator{
	public:
		ButcherTableau bt;
		vector<double> a_eff; // Effective rates
		vector<double> X_eff; // Effective species populations
		vector<unsigned int*> rateSp;			// [rxn.size()][rxn[v].rateSpecies.size()]
		vector<vector<unsigned int> > spInRxn; 	// [sp.size()][#rxns sp involved in]
		vector<vector<double> > stoich; 		// [sp.size()][#rxns sp involved in]
//		aEff_Calculator();
		aEff_Calculator(ButcherTableau bt, vector<SimpleSpecies*>& sp, vector<Reaction*>& rxn);
		aEff_Calculator(const aEff_Calculator& aCalc);
		~aEff_Calculator();
		void calc_aEff(double tau);
	private:
		vector<SimpleSpecies*>& sp;
		vector<Reaction*>& rxn;
		void addRxn();
		void addSpecies();
	};
}

#endif /* AEFF_CALCULATOR_HH_ */
