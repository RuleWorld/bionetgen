/*
 * eRungeKutta.hh
 *
 *  Created on: May 6, 2011
 *      Author: Leonard Harris
 */

#ifndef ERUNGEKUTTA_HH_
#define ERUNGEKUTTA_HH_

#include "../base/firingGenerator.hh"
#include "../base/rxnClassifier.hh"
#include "../base/postleapChecker.hh"
#include "../base/tauCalculator.hh"
#include "../util/binomialCorrector_PL.hh"
#include "../util/preleap_TC.hh"
#include "../util/g_Getter.hh"
#include "core/butcherTableau.hh"
#include "core/aEff_Calculator.hh"
#include "core/eRK.hh"

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
		virtual FiringGenerator* clone() const{ return new eRungeKutta_FG(*this); }
	protected:
		eRK_FG* fg;
		aEff_Calculator* aCalc;
	};

	class eRungeKutta_RC_FG : public RxnClassifier, public eRungeKutta_FG{
	public:
//		eRungeKutta_RC_FG();
		eRungeKutta_RC_FG(ButcherTableau bt, double approx1, double gg1, vector<SimpleSpecies*>& sp, vector<Reaction*>& rxn);
		eRungeKutta_RC_FG(ButcherTableau bt, double approx1, double gg1, vector<SimpleSpecies*>& sp, vector<Reaction*>& rxn,
						  bool round);
		eRungeKutta_RC_FG(const eRungeKutta_RC_FG& rc_fg);
		virtual ~eRungeKutta_RC_FG();
		virtual void fireRxns(vector<double>& k, vector<int>& classif, double tau);
		virtual void classifyRxns(vector<int>& classif, double tau, bool initial);
	protected:
		eRK_RC* rc;
	};

	class eRungeKuttaRB_FG_PL : public eRungeKutta_FG, public BinomialCorrector_PL{
	public:
//		eRungeKuttaRB_FG_PL();
		eRungeKuttaRB_FG_PL(ButcherTableau bt, double eps, double p, vector<SimpleSpecies*>& sp, vector<Reaction*>& rxn);
		eRungeKuttaRB_FG_PL(ButcherTableau bt, double eps, double p, vector<SimpleSpecies*>& sp, vector<Reaction*>& rxn,
							bool round);
		eRungeKuttaRB_FG_PL(const eRungeKuttaRB_FG_PL& fg_pl);
		virtual ~eRungeKuttaRB_FG_PL();
		virtual bool check();
		virtual void update();
	protected:
		eRKrb_PL* pl;
		vector<double*> oldPop;
		void addRxn();
	private:
		vector<Reaction*>& rxn;
	};

	class eRungeKuttaSB_FG_PL : public eRungeKutta_FG, public BinomialCorrector_PL{
	public:
//		eRungeKuttaSB_FG_PL();
		eRungeKuttaSB_FG_PL(ButcherTableau bt, double eps, double p, vector<SimpleSpecies*>& sp, vector<Reaction*>& rxn);
		eRungeKuttaSB_FG_PL(ButcherTableau bt, double eps, double p, vector<SimpleSpecies*>& sp, vector<Reaction*>& rxn,
							bool round);
		eRungeKuttaSB_FG_PL(const eRungeKuttaSB_FG_PL& fg_pl);
		virtual ~eRungeKuttaSB_FG_PL();
		virtual bool check();
		virtual void update();
	protected:
		eRKsb_PL* pl;
		g_Getter* gGet;
		vector<double> oldPop;
		vector<double> old_g;
		void addSpecies();
	private:
		vector<SimpleSpecies*>& sp;
	};

	class eRungeKuttaRB_RC_FG_PL : public eRungeKutta_RC_FG, public BinomialCorrector_PL{
	public:
//		eRungeKuttaRB_RC_FG_PL();
		eRungeKuttaRB_RC_FG_PL(ButcherTableau bt, double eps, double p, double approx1, double gg1, vector<SimpleSpecies*>& sp,
							   vector<Reaction*>& rxn);
		eRungeKuttaRB_RC_FG_PL(ButcherTableau bt, double eps, double p, double approx1, double gg1, vector<SimpleSpecies*>& sp,
							   vector<Reaction*>& rxn, bool round);
		eRungeKuttaRB_RC_FG_PL(const eRungeKuttaRB_RC_FG_PL& rc_fg_pl);
		virtual ~eRungeKuttaRB_RC_FG_PL();
		virtual bool check();
		virtual void update();
	protected:
		eRKrb_PL* pl;
		vector<double*> oldPop;
		void addRxn();
	private:
		vector<Reaction*>& rxn;
	};

	class eRungeKuttaSB_RC_FG_PL : public eRungeKutta_RC_FG, public BinomialCorrector_PL{
	public:
//		eRungeKuttaSB_RC_FG_PL();
		eRungeKuttaSB_RC_FG_PL(ButcherTableau bt, double eps, double p, double approx1, double gg1, vector<SimpleSpecies*>& sp,
							   vector<Reaction*>& rxn);
		eRungeKuttaSB_RC_FG_PL(ButcherTableau bt, double eps, double p, double approx1, double gg1, vector<SimpleSpecies*>& sp,
							   vector<Reaction*>& rxn, bool round);
		eRungeKuttaSB_RC_FG_PL(const eRungeKuttaSB_RC_FG_PL& rc_fg_pl);
		virtual ~eRungeKuttaSB_RC_FG_PL();
		virtual bool check();
		virtual void update();
	protected:
		eRKsb_PL* pl;
		g_Getter* gGet;
		vector<double> oldPop;
		vector<double> old_g;
		void addSpecies();
	private:
		vector<SimpleSpecies*>& sp;
	};

	class eRungeKuttaRB_TC_FG_PL : public TauCalculator, public eRungeKutta_FG, public BinomialCorrector_PL{
	public:
//		eRungeKuttaRB_TC_FG_PL();
		eRungeKuttaRB_TC_FG_PL(ButcherTableau bt, double eps, double p, double pp, double q, double w,
							   vector<SimpleSpecies*>& sp, vector<Reaction*>& rxn, Preleap_TC& ptc);
		eRungeKuttaRB_TC_FG_PL(ButcherTableau bt, double eps, double p, double pp, double q, double w,
							   vector<SimpleSpecies*>& sp, vector<Reaction*>& rxn, Preleap_TC& ptc, bool round);
		eRungeKuttaRB_TC_FG_PL(const eRungeKuttaRB_TC_FG_PL& tc_fg_pl);
		virtual ~eRungeKuttaRB_TC_FG_PL();
		virtual void getNewTau(double& tau);
		virtual void fireRxns(vector<double>& k, vector<int>& classif, double tau);
		virtual bool check();
		virtual void update();
	protected:
		eRKrb_PL* pl;
		vector<double*> oldPop;
		vector<double*> projPop; // Projected species populations: For pre-checking
		double pp, q, w;  // p < pp < 1, q > 1, 0 < w < 1
		bool substantially; // TRUE: substantially accepted (tau *= pp); FALSE: barely accepted (tau *= q)
		bool preCalc;
		Preleap_TC& ptc;
		void addRxn();
	private:
		vector<Reaction*>& rxn;
	};

	class eRungeKuttaSB_TC_FG_PL : public TauCalculator, public eRungeKutta_FG, public BinomialCorrector_PL{
	public:
//		eRungeKuttaSB_TC_FG_PL();
		eRungeKuttaSB_TC_FG_PL(ButcherTableau bt, double eps, double p, double pp, double q, double w,
							   vector<SimpleSpecies*>& sp, vector<Reaction*>& rxn, Preleap_TC& ptc);
		eRungeKuttaSB_TC_FG_PL(ButcherTableau bt, double eps, double p, double pp, double q, double w,
							   vector<SimpleSpecies*>& sp, vector<Reaction*>& rxn, Preleap_TC& ptc, bool round);
		eRungeKuttaSB_TC_FG_PL(const eRungeKuttaSB_TC_FG_PL& tc_fg_pl);
		virtual ~eRungeKuttaSB_TC_FG_PL();
		virtual void getNewTau(double& tau);
		virtual void fireRxns(vector<double>& k, vector<int>& classif, double tau);
		virtual bool check();
		virtual void update();
	protected:
		eRKsb_PL* pl;
		g_Getter* gGet;
		vector<double> oldPop;
		vector<double> projPop;
		vector<double> old_g;
		double pp, q, w;  // p < pp < 1, q > 1, 0 < w < 1
		bool substantially; // TRUE: substantially accepted (tau *= pp); FALSE: barely accepted (tau *= q)
		bool preCalc;
		Preleap_TC& ptc;
		void addSpecies();
	private:
		vector<SimpleSpecies*>& sp;
	};

	class eRungeKuttaRB_TC_RC_FG_PL : public eRungeKuttaRB_TC_FG_PL, public RxnClassifier{
	public:
		eRungeKuttaRB_TC_RC_FG_PL(ButcherTableau bt, double eps, double approx1, double gg1, double p, double pp, double q,
								  double w, vector<SimpleSpecies*>& sp, vector<Reaction*>& rxn, Preleap_TC& ptc);
		eRungeKuttaRB_TC_RC_FG_PL(ButcherTableau bt, double eps, double approx1, double gg1, double p, double pp, double q,
								  double w, vector<SimpleSpecies*>& sp, vector<Reaction*>& rxn, Preleap_TC& ptc, bool round);
		eRungeKuttaRB_TC_RC_FG_PL(const eRungeKuttaRB_TC_RC_FG_PL& tc_rc_fg_pl);
		~eRungeKuttaRB_TC_RC_FG_PL();
		virtual void classifyRxns(vector<int>& classif, double tau, bool initial);
	protected:
		eRK_RC* rc;
	};

	class eRungeKuttaSB_TC_RC_FG_PL : public eRungeKuttaSB_TC_FG_PL, public RxnClassifier{
	public:
		eRungeKuttaSB_TC_RC_FG_PL(ButcherTableau bt, double eps, double approx1, double gg1, double p, double pp, double q,
								  double w, vector<SimpleSpecies*>& sp, vector<Reaction*>& rxn, Preleap_TC& ptc);
		eRungeKuttaSB_TC_RC_FG_PL(ButcherTableau bt, double eps, double approx1, double gg1, double p, double pp, double q,
								  double w, vector<SimpleSpecies*>& sp, vector<Reaction*>& rxn, Preleap_TC& ptc, bool round);
		eRungeKuttaSB_TC_RC_FG_PL(const eRungeKuttaSB_TC_RC_FG_PL& tc_rc_fg_pl);
		~eRungeKuttaSB_TC_RC_FG_PL();
		virtual void classifyRxns(vector<int>& classif, double tau, bool initial);
	protected:
		eRK_RC* rc;
	};
}

#endif /* ERUNGEKUTTA_HH_ */
