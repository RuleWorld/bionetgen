/*
 * eRungeKutta_extra.hh
 *
 *  Created on: Dec 26, 2011
 *      Author: Leonard Harris
 */

#ifndef ERUNGEKUTTA_EXTRA_HH_
#define ERUNGEKUTTA_EXTRA_HH_

#include "../../base/firingGenerator.hh"
#include "../../base/rxnClassifier.hh"
#include "../../base/postleapChecker.hh"
#include "../../base/tauCalculator.hh"
#include "../../util/plCheckers.hh"
#include "../../util/preleap_TC.hh"
#include "../../util/g_Getter.hh"
#include "../util/butcherTableau.hh"
#include "../util/aEff_Calculator.hh"
#include "../util/BinomialCorrector_RK.hh"
#include "eRK_RC.hh"
#include "eRK_FG.hh"

namespace network3{

	class eRungeKutta_FG : public FiringGenerator{
	public:
//		eRungeKutta_FG();
		eRungeKutta_FG(ButcherTableau bt, vector<SimpleSpecies*>& sp, vector<Reaction*>& rxn);
		eRungeKutta_FG(ButcherTableau bt, vector<SimpleSpecies*>& sp, vector<Reaction*>& rxn, bool round);
		eRungeKutta_FG(const eRungeKutta_FG& fg);
		virtual ~eRungeKutta_FG();
		void setRounding(bool round){ this->fg->setRounding(round); }
		virtual void fireRxns(vector<double>& k, vector<int>& classif, double tau);
//		virtual FiringGenerator* clone() const{ return new eRungeKutta_FG(*this); }
	protected:
		aEff_Calculator* aCalc;
		eRK_FG* fg;
	};

	class eRungeKutta_RC_FG : public RxnClassifier, public FiringGenerator{
	public:
//		eRungeKutta_RC_FG();
		eRungeKutta_RC_FG(ButcherTableau bt, double approx1, double gg1, vector<SimpleSpecies*>& sp, vector<Reaction*>& rxn);
		eRungeKutta_RC_FG(ButcherTableau bt, double approx1, double gg1, vector<SimpleSpecies*>& sp, vector<Reaction*>& rxn,
						  bool round);
		eRungeKutta_RC_FG(const eRungeKutta_RC_FG& rc_fg);
		virtual ~eRungeKutta_RC_FG();
		//
		virtual void classifyRxns(vector<int>& classif, double tau, bool initial){
			this->aCalc->calc_aEff(tau);
			this->rc->classifyRxns(classif,tau,initial,this->aCalc->a_eff);
		}
		virtual void fireRxns(vector<double>& k, vector<int>& classif, double tau){
			// a_eff[] elements calculated in classifyRxns()
			this->fg->fireRxns(k,classif,tau,this->aCalc->a_eff);
		}
		//
		void setRounding(bool round){ this->fg->setRounding(round); }
	protected:
		aEff_Calculator* aCalc;
		eRK_RC* rc;
		eRK_FG* fg;
	};

	// Virtual class
	class eRungeKutta_RC_FG_PL : public RxnClassifier, public FiringGenerator, public PostleapChecker{
	public:
//		eRungeKutta_RC_FG_PL();
		eRungeKutta_RC_FG_PL(ButcherTableau bt, double approx1, double gg1, double p,
							 vector<SimpleSpecies*>& sp, vector<Reaction*>& rxn);
		eRungeKutta_RC_FG_PL(ButcherTableau bt, double approx1, double gg1, double p,
							 vector<SimpleSpecies*>& sp, vector<Reaction*>& rxn, bool round);
		eRungeKutta_RC_FG_PL(const eRungeKutta_RC_FG_PL& rc_fg_pl);
		virtual ~eRungeKutta_RC_FG_PL();
		//
		virtual void classifyRxns(vector<int>& classif, double tau, bool initial){
			this->aCalc->calc_aEff(tau);
			this->rc->classifyRxns(classif,tau,initial,this->aCalc->a_eff);
		}
		virtual void fireRxns(vector<double>& k, vector<int>& classif, double tau){
			this->fg->fireRxns(k,classif,tau,this->aCalc->a_eff);
		}
//		virtual bool check() = 0;
		virtual void correct(vector<double>& k, double& tau){
			bool ok = false;
			while (!ok){
				this->bc->correct(k,this->aCalc,tau);
				ok = this->check();
			}
		}
		//
		void setRounding(bool round){ this->fg->setRounding(round); }
	protected:
		aEff_Calculator* aCalc;
		eRK_RC* rc;
		eRK_FG* fg;
		BinomialCorrector_RK* bc;
	};

	class eRungeKutta_FG_rbPL : public eRungeKutta_FG, public PostleapChecker{
	public:
//		eRungeKutta_FG_rbPL();
		eRungeKutta_FG_rbPL(ButcherTableau bt, double eps, double p, vector<SimpleSpecies*>& sp, vector<Reaction*>& rxn);
		eRungeKutta_FG_rbPL(ButcherTableau bt, double eps, double p, vector<SimpleSpecies*>& sp, vector<Reaction*>& rxn,
							bool round);
		eRungeKutta_FG_rbPL(const eRungeKutta_FG_rbPL& fg_pl);
		virtual ~eRungeKutta_FG_rbPL();
		virtual bool check();
		virtual void correct(vector<double>& k, double& tau){
			bool ok = false;
			while (!ok){
				this->bc->correct(k,this->aCalc,tau);
				ok = this->check();
			}
		}
		virtual void update();
	protected:
		RBChecker* ch;
		BinomialCorrector_RK* bc;
		vector<double*> oldPop;
		void addRxn();
	private:
		vector<Reaction*>& rxn;
	};

	class eRungeKutta_FG_sbPL : public eRungeKutta_FG, public PostleapChecker{
	public:
//		eRungeKutta_FG_sbPL();
		eRungeKutta_FG_sbPL(ButcherTableau bt, double eps, double p, vector<SimpleSpecies*>& sp, vector<Reaction*>& rxn);
		eRungeKutta_FG_sbPL(ButcherTableau bt, double eps, double p, vector<SimpleSpecies*>& sp, vector<Reaction*>& rxn,
							bool round);
		eRungeKutta_FG_sbPL(const eRungeKutta_FG_sbPL& fg_pl);
		virtual ~eRungeKutta_FG_sbPL();
		virtual bool check();
		virtual void correct(vector<double>& k, double& tau){
			bool ok = false;
			while (!ok){
				this->bc->correct(k,this->aCalc,tau);
				ok = this->check();
			}
		}
		virtual void update();
	protected:
		SBChecker* ch;
		BinomialCorrector_RK* bc;
		g_Getter* gGet;
		vector<double> oldPop;
		vector<double> old_g;
		void addSpecies();
	private:
		vector<SimpleSpecies*>& sp;
	};

	class eRungeKutta_TC_FG_rbPL : public TauCalculator, public eRungeKutta_FG, public PostleapChecker{
	public:
//		eRungeKutta_TC_FG_rbPL();
		eRungeKutta_TC_FG_rbPL(ButcherTableau bt, double eps, double p, double pp, double q, double w,
							   vector<SimpleSpecies*>& sp, vector<Reaction*>& rxn, Preleap_TC& ptc);
		eRungeKutta_TC_FG_rbPL(ButcherTableau bt, double eps, double p, double pp, double q, double w,
							   vector<SimpleSpecies*>& sp, vector<Reaction*>& rxn, Preleap_TC& ptc, bool round);
		eRungeKutta_TC_FG_rbPL(const eRungeKutta_TC_FG_rbPL& tc_fg_pl);
		virtual ~eRungeKutta_TC_FG_rbPL();
		virtual void getNewTau(double& tau);
		virtual void fireRxns(vector<double>& k, vector<int>& classif, double tau);
		virtual bool check();
		virtual void correct(vector<double>& k, double& tau){
			bool ok = false;
			while (!ok){
				this->bc->correct(k,this->aCalc,tau);
				ok = this->check();
			}
		}
		virtual void update();
	protected:
		RBChecker* ch;
		BinomialCorrector_RK* bc;
		vector<double*> oldPop;
		vector<double*> projPop; // Projected species populations: For pre-checking
		double p, pp, q, w;  // p < pp < 1, q > 1, 0 < w < 1
		bool substantially; // TRUE: substantially accepted (tau *= pp); FALSE: barely accepted (tau *= q)
		bool preCalc;
		Preleap_TC& ptc;
		void addRxn();
	private:
		vector<Reaction*>& rxn;
	};

	class eRungeKutta_TC_FG_sbPL : public TauCalculator, public eRungeKutta_FG, public PostleapChecker{
	public:
//		eRungeKutta_TC_FG_sbPL();
		eRungeKutta_TC_FG_sbPL(ButcherTableau bt, double eps, double p, double pp, double q, double w,
							   vector<SimpleSpecies*>& sp, vector<Reaction*>& rxn, Preleap_TC& ptc);
		eRungeKutta_TC_FG_sbPL(ButcherTableau bt, double eps, double p, double pp, double q, double w,
							   vector<SimpleSpecies*>& sp, vector<Reaction*>& rxn, Preleap_TC& ptc, bool round);
		eRungeKutta_TC_FG_sbPL(const eRungeKutta_TC_FG_sbPL& tc_fg_pl);
		virtual ~eRungeKutta_TC_FG_sbPL();
		virtual void getNewTau(double& tau);
		virtual void fireRxns(vector<double>& k, vector<int>& classif, double tau);
		virtual bool check();
		virtual void update();
	protected:
		SBChecker* ch;
		BinomialCorrector_RK* bc;
		g_Getter* gGet;
		vector<double> oldPop;
		vector<double> projPop;
		vector<double> old_g;
		double p, pp, q, w;  // p < pp < 1, q > 1, 0 < w < 1
		bool substantially; // TRUE: substantially accepted (tau *= pp); FALSE: barely accepted (tau *= q)
		bool preCalc;
		Preleap_TC& ptc;
		void addSpecies();
	private:
		vector<SimpleSpecies*>& sp;
	};
}

#endif /* ERUNGEKUTTA_EXTRA_HH_ */
