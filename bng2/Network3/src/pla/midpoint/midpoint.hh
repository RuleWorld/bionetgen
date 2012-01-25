/*
 * midpoint.hh
 *
 *  Created on: May 6, 2011
 *      Author: Leonard Harris
 */

#ifndef MIDPOINT_HH_
#define MIDPOINT_HH_

#include "../base/firingGenerator.hh"
#include "../base/rxnClassifier.hh"

namespace network3{

	class Midpoint_FG : public FiringGenerator{
	public:
//		Midpoint_FG();
		Midpoint_FG(vector<SimpleSpecies*>& sp, vector<Reaction*>& rxn);
		Midpoint_FG(vector<SimpleSpecies*>& sp, vector<Reaction*>& rxn, bool round);
		Midpoint_FG(const Midpoint_FG& fg);
		virtual ~Midpoint_FG();
		void setRounding(bool round){ this->round = round; }
		double fireRxn(unsigned int u, int classif, double tau);
		virtual void fireRxns(vector<double>& k, vector<int>& classif, double tau);
		virtual FiringGenerator* clone() const{ return new Midpoint_FG(*this); }
	protected:
		bool round;
		vector<double> m_rxn; // For reactions
		vector<double> m_sp;  // For species
		vector<double> a_mid; // Use vectors in case new rxns need to be added (on-the-fly network generation)
		vector<vector<unsigned int> > rateSp;	// [rxn.size()][rateSpecies.size()]
		vector<vector<unsigned int> > spInRxn; 	// [sp.size()][#rxns sp involved in]
		vector<vector<double> > stoich; 		// [sp.size()][#rxns sp involved in]
		void calc_aMid(double tau, bool rxnBased);
		void calc_mRxn();
		void calc_mSp();
	private:
		vector<SimpleSpecies*>& sp;
		vector<Reaction*>& rxn;
	};

	class Midpoint_RC_FG : public RxnClassifier, public Midpoint_FG{
	public:
//		Midpoint_RC_FG();
		Midpoint_RC_FG(double approx1, double gg1, vector<SimpleSpecies*>& sp, vector<Reaction*>& rxn);
		Midpoint_RC_FG(double approx1, double gg1, vector<SimpleSpecies*>& sp, vector<Reaction*>& rxn, bool round);
		virtual ~Midpoint_RC_FG();
		virtual void fireRxns(vector<double>& k, vector<int>& classif, double tau);
		virtual void classifyRxns(vector<int>& classif, double tau, bool initial);
	protected:
		double approx1;
		double gg1;
	private:
		vector<Reaction*>& rxn;
	};
}

#endif /* MIDPOINT_HH_ */
