/*
 * eRungeKutta_RC_FG.cpp
 *
 *  Created on: Apr 4, 2011
 *      Author: Leonard Harris
 */

#include "eRungeKutta.hh"
/*
eRungeKutta_RC_FG::eRungeKutta_RC_FG(){
	if (MoMMA::debug)
		cout << "eRungeKutta_RC_FG constructor called." << endl;
}
*/
eRungeKutta_RC_FG::eRungeKutta_RC_FG(ButcherTableau bt, double approx1, double gg1, vector<SimpleSpecies*>& sp,
		vector<Reaction*>& rxn) : eRungeKutta_FG(bt,sp,rxn){
	if (debug)
		cout << "eRungeKutta_RC_FG constructor called." << endl;
	this->rc = new eRK_RC(approx1,gg1);
}

eRungeKutta_RC_FG::eRungeKutta_RC_FG(ButcherTableau bt, double approx1, double gg1, vector<SimpleSpecies*>& sp,
		vector<Reaction*>& rxn, bool round) : eRungeKutta_FG(bt,sp,rxn,round){
	if (debug)
		cout << "eRungeKutta_RC_FG constructor called." << endl;
	this->rc = new eRK_RC(approx1,gg1);
}

eRungeKutta_RC_FG::eRungeKutta_RC_FG(const eRungeKutta_RC_FG& rc_fg) : RxnClassifier(rc_fg), eRungeKutta_FG(rc_fg){
	if (debug)
		cout << "eRungeKutta_RC_FG copy constructor called." << endl;
	this->rc = new eRK_RC(*rc_fg.rc);
}

eRungeKutta_RC_FG::~eRungeKutta_RC_FG(){
	if (debug)
		cout << "eRungeKutta_RC_FG destructor called." << endl;
	delete this->rc;
}

void eRungeKutta_RC_FG::fireRxns(vector<double>& k, vector<int>& classif, double tau){
	// a_eff[] elements have already been calculated in classifyRxns()
	this->fg->fireRxns(k,classif,tau,this->aCalc->a_eff);
}

void eRungeKutta_RC_FG::classifyRxns(vector<int>& classif, double tau, bool initial){
	this->aCalc->calc_aEff(tau);
	this->rc->classifyRxns(classif,tau,initial,this->aCalc->a_eff);
}
