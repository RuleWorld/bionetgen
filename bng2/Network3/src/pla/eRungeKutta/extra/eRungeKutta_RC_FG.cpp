/*
 * eRungeKutta_RC_FG.cpp
 *
 *  Created on: Apr 4, 2011
 *      Author: Leonard Harris
 */

#include "eRungeKutta_EXTRA.hh"
/*
eRungeKutta_RC_FG::eRungeKutta_RC_FG(){
	if (MoMMA::debug)
		cout << "eRungeKutta_RC_FG constructor called." << endl;
}
*/
eRungeKutta_RC_FG::eRungeKutta_RC_FG(ButcherTableau bt, double approx1, double gg1,
		vector<SimpleSpecies*>& sp, vector<Reaction*>& rxn){
	if (debug)
		cout << "eRungeKutta_RC_FG constructor called." << endl;
	//
	// Error check
	if (!bt.isExplicit()){
		cout << "Error in eRungeKutta_RC_FG constructor: Butcher tableau must be explicit. Exiting." << endl;
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
}

eRungeKutta_RC_FG::eRungeKutta_RC_FG(ButcherTableau bt, double approx1, double gg1,
		vector<SimpleSpecies*>& sp, vector<Reaction*>& rxn, bool round){
	if (debug)
		cout << "eRungeKutta_RC_FG constructor called." << endl;
	//
	// Error check
	if (!bt.isExplicit()){
		cout << "Error in eRungeKutta_RC_FG constructor: Butcher tableau must be explicit. Exiting." << endl;
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
}

eRungeKutta_RC_FG::eRungeKutta_RC_FG(const eRungeKutta_RC_FG& rc_fg) : RxnClassifier(rc_fg), FiringGenerator(rc_fg){
	if (debug)
		cout << "eRungeKutta_RC_FG copy constructor called." << endl;
	//
	this->aCalc = new aEff_Calculator(*rc_fg.aCalc);
	this->rc = new eRK_RC(*rc_fg.rc);
	this->fg = new eRK_FG(*rc_fg.fg);
}

eRungeKutta_RC_FG::~eRungeKutta_RC_FG(){
	if (debug)
		cout << "eRungeKutta_RC_FG destructor called." << endl;
	delete this->aCalc;
	delete this->rc;
	delete this->fg;
}
