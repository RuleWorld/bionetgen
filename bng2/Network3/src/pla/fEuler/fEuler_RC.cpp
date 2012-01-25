/*
 * fEuler_RC.cpp
 *
 *  Created on: Feb 14, 2011
 *      Author: Leonard Harris
 */

#include "fEuler.hh"
/*
fEuler_RC::fEuler_RC() : rxn(rxn){
	if (MoMMA::debug)
		cout << "fEuler_RC constructor called." << endl;
}
*/
fEuler_RC::fEuler_RC(double approx1, double gg1, vector<Reaction*>& rxn) : approx1(approx1), gg1(gg1), rxn(rxn){
	if (debug)
		cout << "fEuler_RC constructor called." << endl;
}

fEuler_RC::fEuler_RC(const fEuler_RC& tc) : approx1(tc.approx1), gg1(tc.gg1), rxn(tc.rxn){
	if (debug)
		cout << "fEuler_RC copy constructor called.\n";
}

fEuler_RC::~fEuler_RC(){
	if (debug)
		cout << "fEuler_RC destructor called." << endl;
}

void fEuler_RC::classifyRxns(vector<int>& classif, double tau, bool initial){
	// Error check
	if (classif.size() != this->rxn.size()){
		cout << "Error in fEuler_RC::classifyRxns(): 'classif' and 'rxn' vectors must be equal size. Exiting.\n";
		exit(1);
	}
	//
	double a_tau;
	for (unsigned int v=0;v < this->rxn.size();v++){
		if (initial || !initial && classif[v] != RxnClassifier::EXACT_STOCHASTIC){
			a_tau = this->rxn[v]->getRate()*tau;
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
