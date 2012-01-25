/*
 * eRungeKuttaRB_TC_RC_FG_PL.cpp
 *
 *  Created on: May 19, 2011
 *      Author: Leonard Harris
 */

#include "eRungeKutta.hh"

eRungeKuttaRB_TC_RC_FG_PL::eRungeKuttaRB_TC_RC_FG_PL(ButcherTableau bt, double eps, double approx1, double gg1, double p,
		double pp, double q, double w, vector<SimpleSpecies*>& sp, vector<Reaction*>& rxn, Preleap_TC& ptc)
		: eRungeKuttaRB_TC_FG_PL(bt,eps,p,pp,q,w,sp,rxn,ptc){
	if (debug)
		cout << "eRungeKuttaRB_TC_RC_FG_PL constructor called." << endl;
	this->rc = new eRK_RC(approx1,gg1);
}

eRungeKuttaRB_TC_RC_FG_PL::eRungeKuttaRB_TC_RC_FG_PL(ButcherTableau bt, double eps, double approx1, double gg1, double p,
		double pp, double q, double w, vector<SimpleSpecies*>& sp, vector<Reaction*>& rxn, Preleap_TC& ptc, bool round)
		: eRungeKuttaRB_TC_FG_PL(bt,eps,p,pp,q,w,sp,rxn,ptc,round){
	if (debug)
		cout << "eRungeKuttaRB_TC_RC_FG_PL constructor called." << endl;
	this->rc = new eRK_RC(approx1,gg1);
}

eRungeKuttaRB_TC_RC_FG_PL::eRungeKuttaRB_TC_RC_FG_PL(const eRungeKuttaRB_TC_RC_FG_PL& tc_rc_fg_pl)
		: eRungeKuttaRB_TC_FG_PL(tc_rc_fg_pl){
	if (debug)
		cout << "eRungeKuttaRB_TC_RC_FG_PL copy constructor called." << endl;
	this->rc = new eRK_RC(*tc_rc_fg_pl.rc);

}

eRungeKuttaRB_TC_RC_FG_PL::~eRungeKuttaRB_TC_RC_FG_PL(){
	if (debug)
		cout << "eRungeKuttaRB_TC_RC_FG_PL destructor called." << endl;
	delete this->rc;
}

void eRungeKuttaRB_TC_RC_FG_PL::classifyRxns(vector<int>& classif, double tau, bool initial){
	// a_eff[] elements have already been calculated in getNewTau()
	this->rc->classifyRxns(classif,tau,initial,this->aCalc->a_eff);
}
