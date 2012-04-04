/*
 * eRungeKutta_preTC_RC_FG_negPL.cpp
 *
 *  Created on: Mar 15, 2012
 *      Author: Leonard Harris
 */

#include "eRungeKutta.hh"
/*
eRungeKutta_preTC_RC_FG_negPL::eRungeKutta_preTC_RC_FG_negPL(){
	if (MoMMA::debug)
		cout << "eRungeKutta_preTC_RC_FG_negPL constructor called." << endl;
}
*/
eRungeKutta_preTC_RC_FG_negPL::eRungeKutta_preTC_RC_FG_negPL(ButcherTableau bt, double eps, double approx1, double gg1,
		double p, Preleap_TC* ptc, vector<SimpleSpecies*>& sp, vector<Reaction*>& rxn)
		: eRungeKutta_TC_RC_FG_PL(bt,eps,approx1,gg1,p,ptc,sp,rxn){
	if (debug)
		cout << "eRungeKutta_preTC_RC_FG_negPL constructor called." << endl;
	this->ch = new NegPopChecker(sp);
}

eRungeKutta_preTC_RC_FG_negPL::eRungeKutta_preTC_RC_FG_negPL(ButcherTableau bt, double eps, double approx1, double gg1,
		double p, Preleap_TC* ptc, vector<SimpleSpecies*>& sp, vector<Reaction*>& rxn, bool round)
		: eRungeKutta_TC_RC_FG_PL(bt,eps,approx1,gg1,p,ptc,sp,rxn,round){
	if (debug)
		cout << "eRungeKutta_preTC_RC_FG_negPL constructor called." << endl;
	this->ch = new NegPopChecker(sp);
}

eRungeKutta_preTC_RC_FG_negPL::eRungeKutta_preTC_RC_FG_negPL(const eRungeKutta_preTC_RC_FG_negPL& tc_rc_fg_pl)
		: eRungeKutta_TC_RC_FG_PL(tc_rc_fg_pl){
	if (debug)
		cout << "eRungeKutta_preTC_RC_FG_negPL copy constructor called." << endl;
	this->ch = new NegPopChecker(*tc_rc_fg_pl.ch);
}

eRungeKutta_preTC_RC_FG_negPL::~eRungeKutta_preTC_RC_FG_negPL(){
	if (debug)
		cout << "eRungeKutta_preTC_RC_FG_negPL destructor called." << endl;
	delete this->ch;
}
