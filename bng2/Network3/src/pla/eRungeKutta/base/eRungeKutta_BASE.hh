/*
 * eRungeKutta_BASE.hh
 *
 *  Created on: Mar 20, 2012
 *      Author: Leonard Harris
 */

#ifndef ERUNGEKUTTA_BASE_HH_
#define ERUNGEKUTTA_BASE_HH_

#include "../../base/firingGenerator.hh"
#include "../../base/rxnClassifier.hh"
#include "../../base/postleapChecker.hh"
#include "../../base/tauCalculator.hh"
#include "../../util/plCheckers.hh"
#include "../../util/preleap_TC.hh"
#include "../../util/g_Getter.hh"
#include "../util/butcherTableau.hh"
#include "../util/aEff_Calculator.hh"
#include "../util/binomialCorrector_RK.hh"

namespace network3{

	// ---------------------------------------------------------------------------------------------------
	// VIRTUAL CLASSES -----------------------------------------------------------------------------------
	// ---------------------------------------------------------------------------------------------------

	class eRungeKutta_TC_RC_FG_PL : public TauCalculator, public RxnClassifier, public FiringGenerator,
									public PostleapChecker{
	public:
//		eRungeKutta_TC_RC_FG_PL();
		eRungeKutta_TC_RC_FG_PL(ButcherTableau bt, double eps, double approx1, double gg1, double p, Preleap_TC* ptc,
								vector<SimpleSpecies*>& sp, vector<Reaction*>& rxn);
		eRungeKutta_TC_RC_FG_PL(ButcherTableau bt, double eps, double approx1, double gg1, double p, Preleap_TC* ptc,
								vector<SimpleSpecies*>& sp, vector<Reaction*>& rxn, bool round);
		eRungeKutta_TC_RC_FG_PL(const eRungeKutta_TC_RC_FG_PL& tc_rc_fg_pl);
		virtual ~eRungeKutta_TC_RC_FG_PL();
		//
//		virtual void getNewTau(double& tau) = 0;
		virtual void classifyRxns(vector<int>& classif, double tau, bool reclassify_all);
		virtual void fireRxns(vector<double>& k, vector<int>& classif, double tau);
//		virtual bool check() = 0;
		virtual void correct(vector<double>& k, double& tau){
			bool ok = false;
			while (!ok){
				this->bc->correct(k,this->aCalc,tau);
				ok = this->check();
			}
		}
//		virtual void update(); // Default update is to do nothing
		//
		void setRounding(bool round){ this->round = round; }
	protected:
		bool round;
		double eps;
		double approx1;
		double gg1;
		Preleap_TC* ptc;
		aEff_Calculator* aCalc;
		BinomialCorrector_RK* bc;
	private:
		vector<Reaction*>& rxn;
	};


	class eRungeKutta_TC_RC_FG_rbPL : public eRungeKutta_TC_RC_FG_PL{
	public:
//		eRungeKutta_TC_RC_FG_rbPL();
		eRungeKutta_TC_RC_FG_rbPL(ButcherTableau bt, double eps, double approx1, double gg1, double p, Preleap_TC* ptc,
								  vector<SimpleSpecies*>& sp, vector<Reaction*>& rxn);
		eRungeKutta_TC_RC_FG_rbPL(ButcherTableau bt, double eps, double approx1, double gg1, double p, Preleap_TC* ptc,
							  	  vector<SimpleSpecies*>& sp, vector<Reaction*>& rxn, bool round);
		eRungeKutta_TC_RC_FG_rbPL(const eRungeKutta_TC_RC_FG_rbPL& tc_rc_fg_pl);
		virtual ~eRungeKutta_TC_RC_FG_rbPL();
		//
//		virtual void getNewTau(double& tau) = 0;
//		virtual bool check() = 0;
		virtual void update(); // Overloaded
	protected:
		double p;
		RBChecker* ch;
		vector<double*> oldPop;
		vector<double*> projPop; // Projected species populations: For preleap checking
		void calc_projPop(double tau);
		void addRxn();
		//
		vector<Reaction*>& rxn;
	};


	class eRungeKutta_TC_RC_FG_sbPL : public eRungeKutta_TC_RC_FG_PL{
	public:
//		eRungeKutta_TC_RC_FG_sbPL();
		eRungeKutta_TC_RC_FG_sbPL(ButcherTableau bt, double eps, double approx1, double gg1, double p, Preleap_TC* ptc,
								  vector<SimpleSpecies*>& sp, vector<Reaction*>& rxn);
		eRungeKutta_TC_RC_FG_sbPL(ButcherTableau bt, double eps, double approx1, double gg1, double p, Preleap_TC* ptc,
								  vector<SimpleSpecies*>& sp, vector<Reaction*>& rxn, bool round);
		eRungeKutta_TC_RC_FG_sbPL(const eRungeKutta_TC_RC_FG_sbPL& tc_rc_fg_pl);
		virtual ~eRungeKutta_TC_RC_FG_sbPL();
		//
//		virtual void getNewTau(double& tau) = 0;
//		virtual bool check() = 0;
		virtual void update(); // Overloaded
	protected:
		double p;
		SBChecker* ch;
		g_Getter* gGet;
		vector<double> oldPop;
		vector<double> projPop; // Projected species populations: For preleap checking
		vector<double> old_g;
		void calc_projPop(double tau);
		void addSpecies();
		//
		vector<SimpleSpecies*>& sp;
	};
}
#endif /* ERUNGEKUTTA_BASE_HH_ */
