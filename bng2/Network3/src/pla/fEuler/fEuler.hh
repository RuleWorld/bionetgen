/*
 * fEuler.hh
 *
 *  Created on: May 6, 2011
 *      Author: Leonard Harris
 */

#ifndef FEULER_HH_
#define FEULER_HH_

#include "../base/firingGenerator.hh"
#include "../base/rxnClassifier.hh"
#include "../base/postleapChecker.hh"
#include "../base/tauCalculator.hh"
#include "../util/binomialCorrector_PL.hh"
#include "../util/g_Getter.hh"
#include "../util/preleap_TC.hh"

namespace network3{

	class fEuler_FG : public FiringGenerator{
	public:
	//	fEuler_FG();
		fEuler_FG(vector<Reaction*>& rxn);
		fEuler_FG(vector<Reaction*>& rxn, bool round);
		fEuler_FG(const fEuler_FG& fg);
		virtual ~fEuler_FG();
		void setRounding(bool round){ this->round = round; }
		double fireRxn(unsigned int u, int classif, double tau);
		virtual void fireRxns(vector<double>& k, vector<int>& classif, double tau);
		virtual FiringGenerator* clone() const{ return new fEuler_FG(*this); }
	protected:
		bool round;
	private:
		vector<Reaction*>& rxn;
	};

	class fEuler_RC : public RxnClassifier{
	public:
	//	fEuler_RC();
		fEuler_RC(double approx1, double gg1, vector<Reaction*>& rxn);
		fEuler_RC(const fEuler_RC& tc);
		virtual ~fEuler_RC();
		virtual void classifyRxns(vector<int>& classif, double tau, bool initial);
		virtual RxnClassifier* clone() const{ return new fEuler_RC(*this); }
	protected:
		double approx1;
		double gg1;
	private:
		vector<Reaction*>& rxn;
	};

	class fEulerRB_PL : public BinomialCorrector_PL{
	public:
	//	fEulerRB_PL();
		fEulerRB_PL(double eps, double p, vector<Reaction*>& rxn);
		fEulerRB_PL(const fEulerRB_PL& pl);
		virtual ~fEulerRB_PL();
		bool wCheck(double w);
		virtual bool check(){ return this->wCheck(1.0); }
		virtual void update();
		virtual PostleapChecker* clone() const{ return new fEulerRB_PL(*this); }
	protected:
		double eps;
		vector<double>  oldRate;
		vector<double*> oldPop;
	private:
		vector<Reaction*>& rxn;
	};

	class fEulerSB_PL : public BinomialCorrector_PL{
	public:
	//	fEulerSB_PL();
		fEulerSB_PL(double eps, double p, vector<SimpleSpecies*>& sp, vector<Reaction*>& rxn);
		fEulerSB_PL(const fEulerSB_PL& pl);
		virtual ~fEulerSB_PL();
		bool wCheck(double w);
		virtual bool check(){ return this->wCheck(1.0); }
		virtual void update();
		virtual PostleapChecker* clone() const{ return new fEulerSB_PL(*this); }
	protected:
		double eps;
		g_Getter* gGet;
		vector<double> oldPop;
		vector<double> old_g;
	private:
		vector<SimpleSpecies*>& sp;
		vector<Reaction*>& rxn;
	};

	class fEulerRB_TC_PL : public fEulerRB_PL, public TauCalculator{
	public:
	//	fEulerRB_TC_PL();
		fEulerRB_TC_PL(double eps, double p, double pp, double q, double w, vector<Reaction*>& rxn, Preleap_TC& ptc);
		virtual ~fEulerRB_TC_PL();
		virtual bool check();
		virtual void getNewTau(double& tau);
	protected:
		double pp, q, w;  // p < pp < 1, q > 1, 0 < w < 1
		bool substantially; // TRUE: substantially accepted (tau *= pp); FALSE: barely accepted (tau *= q)
		bool preCalc;
		Preleap_TC& ptc;
	};

	class fEulerSB_TC_PL : public fEulerSB_PL, public TauCalculator{
	public:
	//	fEulerSB_TC_PL();
		fEulerSB_TC_PL(double eps, double p, double pp, double q, double w, vector<SimpleSpecies*>& sp, vector<Reaction*>& rxn,
					   Preleap_TC& ptc);
		virtual ~fEulerSB_TC_PL();
		virtual bool check();
		virtual void getNewTau(double& tau);
	protected:
		double pp, q, w;  // p < pp < 1, q > 1, 0 < w < 1
		bool substantially; // TRUE: substantially accepted (tau *= pp); FALSE: barely accepted (tau *= q)
		bool preCalc;
		Preleap_TC& ptc;
	};

	class fEulerPreleapRB_TC : public Preleap_TC{
	public:
	//	fEulerPreleapRB_TC();
		fEulerPreleapRB_TC(double eps, vector<Reaction*>& rxn);
		fEulerPreleapRB_TC(const fEulerPreleapRB_TC& tc);
		virtual ~fEulerPreleapRB_TC();
		virtual void getNewTau(double& tau);
		virtual TauCalculator* clone() const{ return new fEulerPreleapRB_TC(*this); }
	private:
		vector<Reaction*>& rxn;
	};

	class fEulerPreleapSB_TC : public Preleap_TC{
	public:
	//	fEulerPreleapSB_TC();
		fEulerPreleapSB_TC(double eps, vector<SimpleSpecies*>& sp, vector<Reaction*>& rxn);
		fEulerPreleapSB_TC(const fEulerPreleapSB_TC& tc);
		virtual ~fEulerPreleapSB_TC();
		virtual void getNewTau(double& tau);
		virtual TauCalculator* clone() const{ return new fEulerPreleapSB_TC(*this); }
	protected:
		g_Getter* gGet;
	private:
		vector<SimpleSpecies*>& sp;
		vector<Reaction*>& rxn;
	};
}



#endif /* FEULER_HH_ */
