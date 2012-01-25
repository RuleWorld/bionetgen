/*
 * midpoint_RC_FG.cpp
 *
 *  Created on: Feb 18, 2011
 *      Author: Leonard Harris
 */

#include "midpoint.hh"
#include "../../util/util.hh"
/*
Midpoint_RC_FG::Midpoint_RC_FG() : rxn(){
	if (MoMMA::debug)
		cout << "Midpoint_RC_FG constructor called." << endl;
}
*/
Midpoint_RC_FG::Midpoint_RC_FG(double approx1, double gg1, vector<SimpleSpecies*>& sp, vector<Reaction*>& rxn)
		: Midpoint_FG(sp,rxn), approx1(approx1), gg1(gg1), rxn(rxn){
	if (debug)
		cout << "Midpoint_RC_FG constructor called." << endl;
}

Midpoint_RC_FG::Midpoint_RC_FG(double approx1, double gg1, vector<SimpleSpecies*>& sp, vector<Reaction*>& rxn, bool round)
		: Midpoint_FG(sp,rxn,round), approx1(approx1), gg1(gg1), rxn(rxn){
	if (debug)
		cout << "Midpoint_RC_FG constructor called." << endl;
}

Midpoint_RC_FG::~Midpoint_RC_FG(){
	if (debug)
		cout << "Midpoint_RC_FG destructor called." << endl;
}

void Midpoint_RC_FG::fireRxns(vector<double>& k, vector<int>& classif, double tau){
	// Error check
	if (classif.size() != this->rxn.size()){
		cout << "Error in Midpoint_RC_FG::fireRxns(): 'classif' vector must be same size as 'rxn' vector. Exiting.\n";
		exit(1);
	}
	if (k.size() != this->rxn.size()){
		cout << "Error in Midpoint_RC_FG::fireRxns(): 'k' vector must be same size as 'rxn' vector. Exiting.\n";
		exit(1);
	}
	//
	// a_mid[] elements already calculated in classifyRxns(). Just move on to firing rxns.
	//
	for (unsigned int v=0;v < this->rxn.size();v++){
		if (classif[v] != RxnClassifier::EXACT_STOCHASTIC){
			k[v] = this->fireRxn(v,classif[v],tau);
		}
		else{
			k[v] = 0.0;
		}
	}
}

void Midpoint_RC_FG::classifyRxns(vector<int>& classif, double tau, bool initial){
	// Error check
	if (classif.size() != this->rxn.size()){
		cout << "Error in Midpoint_RC_FG::classifyRxns(): 'classif' and 'rxn' vectors must be equal size. Exiting.\n";
		exit(1);
	}
	//
	this->calc_aMid(tau,true); // TRUE: Rxn-based; FALSE: Species-based
	//
	// Loop over rxns
	for (unsigned int v=0;v < this->rxn.size();v++){
		if (initial || !initial && classif[v] != RxnClassifier::EXACT_STOCHASTIC){
			double a_tau = this->a_mid[v]*tau; // Estimated midpoint propensity * tau
			if (sqrt(a_tau) > this->gg1){
				classif[v] = this->DETERMINISTIC;
			}
			else if (a_tau > this->gg1){
				classif[v] = this->LANGEVIN;
			}
			else if (a_tau > this->approx1){
				classif[v] = this->POISSON;
			}
			else{
				classif[v] = this->EXACT_STOCHASTIC;
			}
		}
	}
}
