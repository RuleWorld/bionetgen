/*
 * eRungeKutta.hh
 *
 *  Created on: May 6, 2011
 *      Author: Leonard Harris
 */

#ifndef ERUNGEKUTTA_HH_
#define ERUNGEKUTTA_HH_

#include "base/eRungeKutta_BASE.hh"

namespace network3{

	// Order of operations:
	// 1. Calculate tau
	// 2. Classify rxns
	// 3. Fire rxns
	// 4. Postleap check/correct

	class eRungeKutta_preTC_RC_FG_negPL : public eRungeKutta_TC_RC_FG_PL{
		public:
//		eRungeKutta_preTC_RC_FG_negPL();
		eRungeKutta_preTC_RC_FG_negPL(ButcherTableau bt, double eps, double approx1, double gg1, double p, Preleap_TC* ptc,
									  vector<SimpleSpecies*>& sp, vector<Reaction*>& rxn);
		eRungeKutta_preTC_RC_FG_negPL(ButcherTableau bt, double eps, double approx1, double gg1, double p, Preleap_TC* ptc,
									  vector<SimpleSpecies*>& sp, vector<Reaction*>& rxn, bool round);
		eRungeKutta_preTC_RC_FG_negPL(const eRungeKutta_preTC_RC_FG_negPL& tc_rc_fg_pl);
		virtual ~eRungeKutta_preTC_RC_FG_negPL();
		virtual void getNewTau(double& tau){
			this->ptc->getNewTau(tau); // No preleap check
			this->aCalc->calc_aEff(tau);
		}
		virtual bool check(){ return this->ch->check(); }
		virtual void update(){ this->aCalc->update(); } // Overloaded
	protected:
		NegPopChecker* ch;
	};

	class eRungeKutta_preTC_RC_FG_rbPL : public eRungeKutta_TC_RC_FG_rbPL{
	public:
//		eRungeKutta_preTC_RC_FG_rbPL();
		eRungeKutta_preTC_RC_FG_rbPL(ButcherTableau bt, double eps, double approx1, double gg1, double p, Preleap_TC* ptc,
									 vector<SimpleSpecies*>& sp, vector<Reaction*>& rxn);
		eRungeKutta_preTC_RC_FG_rbPL(ButcherTableau bt, double eps, double approx1, double gg1, double p, Preleap_TC* ptc,
							  	     vector<SimpleSpecies*>& sp, vector<Reaction*>& rxn, bool round);
		eRungeKutta_preTC_RC_FG_rbPL(const eRungeKutta_preTC_RC_FG_rbPL& tc_rc_fg_pl);
		virtual ~eRungeKutta_preTC_RC_FG_rbPL();
		//
		virtual void getNewTau(double& tau);
		virtual bool check(){ return this->ch->check(1.0,this->aCalc->a_eff,this->oldPop,true); }
	};

	class eRungeKutta_preTC_RC_FG_sbPL : public eRungeKutta_TC_RC_FG_sbPL{
	public:
//		eRungeKutta_preTC_RC_FG_sbPL();
		eRungeKutta_preTC_RC_FG_sbPL(ButcherTableau bt, double eps, double approx1, double gg1, double p, Preleap_TC* ptc,
									 vector<SimpleSpecies*>& sp, vector<Reaction*>& rxn);
		eRungeKutta_preTC_RC_FG_sbPL(ButcherTableau bt, double eps, double approx1, double gg1, double p, Preleap_TC* ptc,
							  	     vector<SimpleSpecies*>& sp, vector<Reaction*>& rxn, bool round);
		eRungeKutta_preTC_RC_FG_sbPL(const eRungeKutta_preTC_RC_FG_sbPL& tc_rc_fg_pl);
		virtual ~eRungeKutta_preTC_RC_FG_sbPL();
		//
		virtual void getNewTau(double& tau);
		virtual bool check(){ return this->ch->check(1.0,this->aCalc->X_eff,this->oldPop,this->old_g,true); }
	};

	class eRungeKutta_postTC_RC_FG_rbPL : public eRungeKutta_TC_RC_FG_rbPL{
	public:
//		eRungeKutta_postTC_RC_FG_rbPL();
		eRungeKutta_postTC_RC_FG_rbPL(ButcherTableau bt, double eps, double approx1, double gg1, double p, double pp,
									  double q, double w, Preleap_TC* ptc, vector<SimpleSpecies*>& sp,
									  vector<Reaction*>& rxn);
		eRungeKutta_postTC_RC_FG_rbPL(ButcherTableau bt, double eps, double approx1, double gg1, double p, double pp,
									  double q, double w, Preleap_TC* ptc, vector<SimpleSpecies*>& sp,
									  vector<Reaction*>& rxn, bool round);
		eRungeKutta_postTC_RC_FG_rbPL(const eRungeKutta_postTC_RC_FG_rbPL& tc_rc_fg_pl);
		~eRungeKutta_postTC_RC_FG_rbPL();
		virtual void getNewTau(double& tau);
		virtual bool check();
	protected:
		bool preCalc;
		double pp, q, w;  // p < pp < 1, q > 1, 0 < w < 1
		bool substantially; // TRUE: substantially accepted (tau *= q); FALSE: barely accepted (tau *= pp)
	};

	class eRungeKutta_postTC_RC_FG_sbPL : public eRungeKutta_TC_RC_FG_sbPL{
	public:
//		eRungeKutta_postTC_RC_FG_sbPL();
		eRungeKutta_postTC_RC_FG_sbPL(ButcherTableau bt, double eps, double approx1, double gg1, double p, double pp,
									  double q, double w, Preleap_TC* ptc, vector<SimpleSpecies*>& sp,
									  vector<Reaction*>& rxn);
		eRungeKutta_postTC_RC_FG_sbPL(ButcherTableau bt, double eps, double approx1, double gg1, double p, double pp,
									  double q, double w, Preleap_TC* ptc, vector<SimpleSpecies*>& sp,
									  vector<Reaction*>& rxn, bool round);
		eRungeKutta_postTC_RC_FG_sbPL(const eRungeKutta_postTC_RC_FG_sbPL& tc_rc_fg_pl);
		~eRungeKutta_postTC_RC_FG_sbPL();
		virtual void getNewTau(double& tau);
		virtual bool check();
	protected:
		bool preCalc;
		double pp, q, w;  // p < pp < 1, q > 1, 0 < w < 1
		bool substantially; // TRUE: substantially accepted (tau *= q); FALSE: barely accepted (tau *= pp)
	};


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/*
	class eRungeKutta_RC_FG_negPL : public eRungeKutta_RC_FG_PL{
	public:
//		eRungeKutta_RC_FG_negPL();
		eRungeKutta_RC_FG_negPL(ButcherTableau bt, double eps, double approx1, double gg1, double p,
				vector<SimpleSpecies*>& sp, vector<Reaction*>& rxn);
		eRungeKutta_RC_FG_negPL(ButcherTableau bt, double eps, double approx1, double gg1, double p,
				vector<SimpleSpecies*>& sp, vector<Reaction*>& rxn, bool round);
		eRungeKutta_RC_FG_negPL(const eRungeKutta_RC_FG_negPL& rc_fg_pl);
		virtual ~eRungeKutta_RC_FG_negPL();
		virtual bool check(){
			// a_eff[] elements already calculated in classifyRxns()
			return this->ch->check();
		}
		virtual void update(){ this->aCalc->update(); } // Overloaded
	protected:
		NegPopChecker* ch;
	};

	class eRungeKutta_RC_FG_rbPL : public eRungeKutta_RC_FG_PL{
	public:
//		eRungeKutta_RC_FG_rbPL();
		eRungeKutta_RC_FG_rbPL(ButcherTableau bt, double eps, double approx1, double gg1, double p,
				vector<SimpleSpecies*>& sp, vector<Reaction*>& rxn);
		eRungeKutta_RC_FG_rbPL(ButcherTableau bt, double eps, double approx1, double gg1, double p,
				vector<SimpleSpecies*>& sp, vector<Reaction*>& rxn, bool round);
		eRungeKutta_RC_FG_rbPL(const eRungeKutta_RC_FG_rbPL& rc_fg_pl);
		virtual ~eRungeKutta_RC_FG_rbPL();
		virtual bool check();
		virtual void update(); // Overloaded
	protected:
		RBChecker* ch;
		vector<double*> oldPop;
		void addRxn();
	private:
		vector<Reaction*>& rxn;
	};

	class eRungeKutta_RC_FG_sbPL : public eRungeKutta_RC_FG_PL{
	public:
//		eRungeKutta_RC_FG_sbPL();
		eRungeKutta_RC_FG_sbPL(ButcherTableau bt, double eps, double approx1, double gg1, double p,
				vector<SimpleSpecies*>& sp, vector<Reaction*>& rxn);
		eRungeKutta_RC_FG_sbPL(ButcherTableau bt, double eps, double approx1, double gg1, double p,
				vector<SimpleSpecies*>& sp, vector<Reaction*>& rxn, bool round);
		eRungeKutta_RC_FG_sbPL(const eRungeKutta_RC_FG_sbPL& rc_fg_pl);
		virtual ~eRungeKutta_RC_FG_sbPL();
		virtual bool check();
		virtual void update(); // Overloaded
	protected:
		SBChecker* ch;
		g_Getter* gGet;
		vector<double> oldPop;
		vector<double> old_g;
		void addSpecies();
	private:
		vector<SimpleSpecies*>& sp;
	};

	class eRungeKutta_TC_RC_FG_rbPL : public TauCalculator, public eRungeKutta_RC_FG_rbPL{
	public:
//		eRungeKutta_TC_RC_FG_rbPL();
		eRungeKutta_TC_RC_FG_rbPL(ButcherTableau bt, double eps, double approx1, double gg1, double p, double pp, double q,
				double w, vector<SimpleSpecies*>& sp, vector<Reaction*>& rxn, Preleap_TC& ptc);
		eRungeKutta_TC_RC_FG_rbPL(ButcherTableau bt, double eps, double approx1, double gg1, double p, double pp, double q,
				double w, vector<SimpleSpecies*>& sp, vector<Reaction*>& rxn, Preleap_TC& ptc, bool round);
		eRungeKutta_TC_RC_FG_rbPL(const eRungeKutta_TC_RC_FG_rbPL& tc_rc_fg_pl);
		~eRungeKutta_TC_RC_FG_rbPL();
		virtual void getNewTau(double& tau);
		virtual void classifyRxns(vector<int>& classif, double tau, bool initial){ // Overloaded
			// a_eff[] elements calculated in getNewTau()
			this->rc->classifyRxns(classif,tau,initial,this->aCalc->a_eff);
		}
		virtual bool check(); // Overloaded
		virtual void update(); // Overloaded
	protected:
		Preleap_TC& ptc;
		bool preCalc;
		double p, pp, q, w;  // p < pp < 1, q > 1, 0 < w < 1
		bool substantially; // TRUE: substantially accepted (tau *= pp); FALSE: barely accepted (tau *= q)
		vector<double*> projPop; // Projected species populations: For pre-checking
		void addRxn();
	private:
		vector<Reaction*>& rxn;
	};

	class eRungeKutta_TC_RC_FG_sbPL : public TauCalculator, public eRungeKutta_RC_FG_sbPL{
	public:
//		eRungeKutta_TC_RC_FG_sbPL();
		eRungeKutta_TC_RC_FG_sbPL(ButcherTableau bt, double eps, double approx1, double gg1, double p, double pp, double q,
				double w, vector<SimpleSpecies*>& sp, vector<Reaction*>& rxn, Preleap_TC& ptc);
		eRungeKutta_TC_RC_FG_sbPL(ButcherTableau bt, double eps, double approx1, double gg1, double p, double pp, double q,
				double w, vector<SimpleSpecies*>& sp, vector<Reaction*>& rxn, Preleap_TC& ptc, bool round);
		eRungeKutta_TC_RC_FG_sbPL(const eRungeKutta_TC_RC_FG_sbPL& tc_rc_fg_pl);
		~eRungeKutta_TC_RC_FG_sbPL();
		virtual void getNewTau(double& tau);
		virtual void classifyRxns(vector<int>& classif, double tau, bool initial){ // overloaded
			// a_eff[] elements calculated in getNewTau()
			this->rc->classifyRxns(classif,tau,initial,this->aCalc->a_eff);
		}
		virtual bool check(); // Overloaded
		virtual void update(); // Overloaded
	protected:
		Preleap_TC& ptc;
		bool preCalc;
		double p, pp, q, w;  // p < pp < 1, q > 1, 0 < w < 1
		bool substantially; // TRUE: substantially accepted (tau *= pp); FALSE: barely accepted (tau *= q)
		vector<double> projPop;
		void addSpecies();
	private:
		vector<SimpleSpecies*>& sp;
	};
*/
}
#endif /* ERUNGEKUTTA_HH_ */
