/*
 * eRK_RC.cpp
 *
 *  Created on: May 1, 2011
 *      Author: Leonard Harris
 */

#include "eRK_RC.hh"

eRK_RC::eRK_RC(double approx1, double gg1) : approx1(approx1), gg1(gg1){
	if (debug)
		cout << "eRK_RC constructor called." << endl;
}

eRK_RC::eRK_RC(const eRK_RC& rc) : approx1(rc.approx1), gg1(rc.gg1){
	if (debug)
		cout << "eRK_RC copy constructor called." << endl;
}

eRK_RC::~eRK_RC(){
	if (debug)
		cout << "eRK_RC destructor called." << endl;
}

void eRK_RC::classifyRxns(vector<int>& classif, double tau, bool initial, vector<double>& a_eff){
	// Error check
	if (classif.size() != a_eff.size()){
		cout << "Error in eRK_RC::classifyRxns(): 'classif' and 'a_eff' vectors must be equal size. Exiting.\n";
		exit(1);
	}
	//
	double a_tau;
	for (unsigned int v=0;v < classif.size();v++){
		if (initial || !initial && classif[v] != RxnClassifier::EXACT_STOCHASTIC){
			a_tau = a_eff[v]*tau;
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
