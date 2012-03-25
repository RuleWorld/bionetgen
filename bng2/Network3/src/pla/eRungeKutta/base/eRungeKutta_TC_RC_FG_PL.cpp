/*
 * eRungeKutta_TC_RC_FG_PL.cpp
 *
 *  Created on: Mar 19, 2012
 *      Author: Leonard Harris
 */

#include "../eRungeKutta.hh"

eRungeKutta_TC_RC_FG_PL::eRungeKutta_TC_RC_FG_PL(ButcherTableau bt, double approx1, double gg1, double p, Preleap_TC* ptc,
		vector<SimpleSpecies*>& sp, vector<Reaction*>& rxn) : round(true), approx1(approx1), gg1(gg1), rxn(rxn){
	if (debug)
		cout << "eRungeKutta_TC_RC_FG_PL constructor called." << endl;
	//
	// Error check
	if (!bt.isExplicit()){
		cout << "Error in eRungeKutta_TC_RC_FG_PL constructor: Butcher tableau must be explicit. Exiting." << endl;
		exit(1);
	}
	// Round species populations (default)
	for (unsigned int i=0;i < sp.size();i++){
		sp[i]->population = floor(sp[i]->population + 0.5);
	}
	//
	this->ptc = ptc->clone();
	this->aCalc = new aEff_Calculator(bt,sp,rxn);
	this->bc = new BinomialCorrector_RK(p,rxn);
}

eRungeKutta_TC_RC_FG_PL::eRungeKutta_TC_RC_FG_PL(ButcherTableau bt, double approx1, double gg1, double p, Preleap_TC* ptc,
		vector<SimpleSpecies*>& sp, vector<Reaction*>& rxn, bool round) : round(round), approx1(approx1), gg1(gg1),
		rxn(rxn){
	if (debug)
		cout << "eRungeKutta_TC_RC_FG_PL constructor called." << endl;
	//
	// Error check
	if (!bt.isExplicit()){
		cout << "Error in eRungeKutta_TC_RC_FG_PL constructor: Butcher tableau must be explicit. Exiting." << endl;
		exit(1);
	}
	// Round species populations if round = true
	if (round){
		for (unsigned int i=0;i < sp.size();i++){
			sp[i]->population = floor(sp[i]->population + 0.5);
		}
	}
	//
	this->ptc = ptc->clone();
	this->aCalc = new aEff_Calculator(bt,sp,rxn);
	this->bc = new BinomialCorrector_RK(p,rxn);
}

eRungeKutta_TC_RC_FG_PL::eRungeKutta_TC_RC_FG_PL(const eRungeKutta_TC_RC_FG_PL& tc_rc_fg_pl) : TauCalculator(tc_rc_fg_pl),
		RxnClassifier(tc_rc_fg_pl), FiringGenerator(tc_rc_fg_pl), PostleapChecker(tc_rc_fg_pl),
		approx1(tc_rc_fg_pl.approx1), gg1(tc_rc_fg_pl.gg1), rxn(tc_rc_fg_pl.rxn){
	if (debug)
		cout << "eRungeKutta_TC_RC_FG_PL copy constructor called." << endl;
	//
	this->ptc = tc_rc_fg_pl.ptc->clone();
	this->aCalc = new aEff_Calculator(*tc_rc_fg_pl.aCalc);
	this->bc = new BinomialCorrector_RK(*tc_rc_fg_pl.bc);
}

eRungeKutta_TC_RC_FG_PL::~eRungeKutta_TC_RC_FG_PL(){
	if (debug)
		cout << "eRungeKutta_TC_RC_FG_PL destructor called." << endl;
	delete this->ptc;
	delete this->aCalc;
	delete this->bc;
}

void eRungeKutta_TC_RC_FG_PL::classifyRxns(vector<int>& classif, double tau, bool initial){
	// Error check
	if (classif.size() != this->aCalc->a_eff.size()){
		cout << "Error in eRungeKutta_TC_RC_FG_PL::classifyRxns(): 'classif' and 'a_eff' vectors must be equal size. "
			 << "Exiting." << endl;
		exit(1);
	}
	// Classify rxns
	double a_tau;
	for (unsigned int v=0;v < classif.size();v++){
		if (initial || !initial && classif[v] != RxnClassifier::EXACT_STOCHASTIC){
			a_tau = this->aCalc->a_eff[v]*tau;
			//
			if (sqrt(a_tau) > this->gg1){
				classif[v] = RxnClassifier::DETERMINISTIC;
			}
			else if (a_tau > this->gg1){
				classif[v] = RxnClassifier::LANGEVIN;
			}
			else if (a_tau > this->approx1){
				classif[v] = RxnClassifier::POISSON;
			}
			else{
				classif[v] = RxnClassifier::EXACT_STOCHASTIC;
			}
		}
	}
}
void eRungeKutta_TC_RC_FG_PL::fireRxns(vector<double>& k, vector<int>& classif, double tau){

	// Error check
	if (k.size() != this->rxn.size()){
		cout << "Error in eRungeKutta_TC_RC_FG_PL::fireRxns(): 'k' and 'rxn' vectors must be equal sizes. "
			 << "Exiting." << endl;
		exit(1);
	}
	if (classif.size() != this->rxn.size()){
		cout << "Error in eRungeKutta_TC_RC_FG_PL::fireRxns(): 'classif' and 'rxn' vectors must be equal sizes. "
			 << "Exiting." << endl;
		exit(1);
	}
	// Fire rxns
	for (unsigned int v=0;v < this->rxn.size();v++){
		if (classif[v] != RxnClassifier::EXACT_STOCHASTIC){ // ES rxn is fired in PLA::nextStep() after postleap check
			double a_tau = this->aCalc->a_eff[v]*tau;
			if (classif[v] == RxnClassifier::POISSON){
				k[v] = Util::RANDOM_POISSON(a_tau);
			}
			else if (classif[v] == RxnClassifier::LANGEVIN){
				k[v] = a_tau + sqrt(a_tau)*Util::RANDOM_GAUSSIAN();
				if (k[v] < 0.0) k[v] = 0.0; // Just to be safe
				else if (this->round) k[v] = floor(k[v] + 0.5);
			}
			else if (classif[v] == RxnClassifier::DETERMINISTIC){
				k[v] = a_tau;
				if (this->round) k[v] = floor(k[v] + 0.5);
			}
			else{
				cout << "Error in eRungeKutta_TC_RC_FG_PL::fireRxns(): Reaction classification for "
					 << this->rxn[v]->toString() << " (" << classif[v] << ") not recognized." << endl;
				cout << "Only Exact Stochastic (" << RxnClassifier::EXACT_STOCHASTIC << "), Poisson ("
					 << RxnClassifier::POISSON << "), Langevin (" << RxnClassifier::LANGEVIN
					 << ") and Deterministic (" << RxnClassifier::DETERMINISTIC << ") are supported. "
					 << "Exiting." << endl;
				exit(1);
			}
			this->rxn[v]->fire(k[v]);
		}
		else{
			k[v] = 0.0;
		}
	}
}
