/*
 * checkers.hh
 *
 *  Created on: Mar 15, 2012
 *      Author: Len
 */

#ifndef CHECKERS_HH_
#define CHECKERS_HH_

#include "../../std_include.hh"
#include "../../model/reaction.hh"

namespace network3{

	class NegPopChecker{
	public:
	//	NegPopChecker();
		NegPopChecker(vector<SimpleSpecies*>& sp);
		NegPopChecker(const NegPopChecker& ch);
		virtual ~NegPopChecker();
		virtual bool check();
		unsigned int nSpecies(){ return this->sp.size(); }
//		virtual PostleapChecker* clone() const{ return new NegPopChecker(*this); }
	private:
		vector<SimpleSpecies*>& sp;
	};

	class RBChecker{
	public:
		RBChecker(double eps, vector<Reaction*>& rxn);
		RBChecker(const RBChecker& ch);
		~RBChecker();
		bool check(double w, vector<double>& a_check, vector<double*>& x_ref, bool postcheck);
		unsigned int nRxns(){ return this->rxn.size(); }
	protected:
		double eps;
	private:
		vector<Reaction*>& rxn;
	};

	class SBChecker{
	public:
		SBChecker(double eps, vector<SimpleSpecies*>& sp);
		SBChecker(const SBChecker& ch);
		~SBChecker();
		bool check(double w, vector<double>& x_check, vector<double>& x_ref, vector<double>& g_ref, bool postcheck);
		unsigned int nSpecies(){ return this->sp.size(); }
	protected:
		double eps;
	private:
		vector<SimpleSpecies*>& sp;
	};
}

#endif /* CHECKERS_HH_ */
