/*
 * eRK.hh
 *
 *  Created on: May 1, 2011
 *      Author: Leonard Harris
 */

#ifndef ERK_HH_
#define ERK_HH_

#include "../../../model/reaction.hh"
#include "../../../util/util.hh"
#include "../../base/rxnClassifier.hh"

namespace network3{

	class eRK_FG{
	public:
		eRK_FG(vector<Reaction*>& rxn);
		eRK_FG(vector<Reaction*>& rxn, bool round);
		eRK_FG(const eRK_FG& fg);
		~eRK_FG();
		void setRounding(bool round){ this->round = round; }
		void fireRxns(vector<double>& k, vector<int>& classif, double tau, vector<double>& a_eff);
	protected:
		bool round;
		double fireRxn(unsigned int u, double a_tau, int classif);
		vector<Reaction*>& rxn;
	};

	class eRK_RC{
	public:
		eRK_RC(double approx1, double gg1);
		eRK_RC(const eRK_RC& rc);
		~eRK_RC();
		void classifyRxns(vector<int>& classif, double tau, bool initial, vector<double>& a_eff);
	protected:
		double approx1;
		double gg1;
	};

	class eRKrb_PL{
	public:
		eRKrb_PL(double eps, vector<Reaction*>& rxn);
		eRKrb_PL(const eRKrb_PL& pl);
		~eRKrb_PL();
//		bool check(double w, vector<double>& a_eff, vector<double*>& oldPop);
		bool check(double w, vector<double>& a_eff, vector<double*>& refPop, bool postleap);
	protected:
		double eps;
		vector<Reaction*>& rxn;
	};

	class eRKsb_PL{
	public:
		eRKsb_PL(double eps, vector<SimpleSpecies*>& sp);
		eRKsb_PL(const eRKsb_PL& pl);
		~eRKsb_PL();
//		bool check(double w, vector<double>& X_eff, vector<double>& oldPop, vector<double>& old_g);
		bool check(double w, vector<double>& X_eff, vector<double>& refPop, vector<double>& ref_g, bool postleap);
	protected:
		double eps;
		vector<SimpleSpecies*>& sp;
	};
}

#endif /* ERK_HH_ */
