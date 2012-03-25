/*
 * eRungeKutta_RC_FG_PL.cpp
 *
 *  Created on: Mar 15, 2012
 *      Author: Leonard Harris
 */

#include "eRungeKutta_EXTRA.hh"

eRungeKutta_RC_FG_PL::eRungeKutta_RC_FG_PL(ButcherTableau bt, double approx1, double gg1, double p,
		vector<SimpleSpecies*>& sp, vector<Reaction*>& rxn){
	if (debug)
		cout << "eRungeKutta_RC_FG_PL constructor called." << endl;
	//
	// Error check
	if (!bt.isExplicit()){
		cout << "Error in eRungeKutta_RC_FG_PL constructor: Butcher tableau must be explicit. Exiting." << endl;
		exit(1);
	}
	// Round species populations (default)
	for (unsigned int i=0;i < sp.size();i++){
		sp[i]->population = floor(sp[i]->population + 0.5);
	}
	//
	this->aCalc = new aEff_Calculator(bt,sp,rxn);
	this->rc = new eRK_RC(approx1,gg1);
	this->fg = new eRK_FG(rxn);
	this->bc = new BinomialCorrector_RK(p,rxn);
}

eRungeKutta_RC_FG_PL::eRungeKutta_RC_FG_PL(ButcherTableau bt, double approx1, double gg1, double p,
		vector<SimpleSpecies*>& sp, vector<Reaction*>& rxn, bool round){
	if (debug)
		cout << "eRungeKutta_RC_FG_PL constructor called." << endl;
	//
	// Error check
	if (!bt.isExplicit()){
		cout << "Error in eRungeKutta_RC_FG_PL constructor: Butcher tableau must be explicit. Exiting." << endl;
		exit(1);
	}
	// Round species populations if round = true
	if (round){
		for (unsigned int i=0;i < sp.size();i++){
			sp[i]->population = floor(sp[i]->population + 0.5);
		}
	}
	//
	this->aCalc = new aEff_Calculator(bt,sp,rxn);
	this->rc = new eRK_RC(approx1,gg1);
	this->fg = new eRK_FG(rxn,round);
	this->bc = new BinomialCorrector_RK(p,rxn);
}

eRungeKutta_RC_FG_PL::eRungeKutta_RC_FG_PL(const eRungeKutta_RC_FG_PL& rc_fg_pl) : RxnClassifier(rc_fg_pl),
		FiringGenerator(rc_fg_pl), PostleapChecker(rc_fg_pl){
	if (debug)
		cout << "eRungeKutta_RC_FG_PL copy constructor called." << endl;
	//
	this->aCalc = new aEff_Calculator(*rc_fg_pl.aCalc);
	this->rc = new eRK_RC(*rc_fg_pl.rc);
	this->fg = new eRK_FG(*rc_fg_pl.fg);
	this->bc = new BinomialCorrector_RK(*rc_fg_pl.bc);
}

eRungeKutta_RC_FG_PL::~eRungeKutta_RC_FG_PL(){
	if (debug)
		cout << "eRungeKutta_RC_FG_PL destructor called." << endl;
	delete this->aCalc;
	delete this->rc;
	delete this->fg;
	delete this->bc;
}
