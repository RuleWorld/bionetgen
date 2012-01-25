/*
 * eRK_Base_FG.cpp
 *
 *  Created on: Apr 30, 2011
 *      Author: Leonard Harris
 */

#include "eRK.hh"

eRK_FG::eRK_FG(vector<Reaction*>& rxn) : round(true), rxn(rxn){
	if (debug)
		cout << "eRK_FG constructor called." << endl;
}

eRK_FG::eRK_FG(vector<Reaction*>& rxn, bool round) : round(round), rxn(rxn){
	if (debug)
		cout << "eRK_FG constructor called." << endl;
}

eRK_FG::eRK_FG(const eRK_FG& fg) : round(fg.round), rxn(fg.rxn){
	if (debug)
			cout << "eRK_FG copy constructor called." << endl;
}

eRK_FG::~eRK_FG(){
	if (debug)
		cout << "eRK_FG destructor called." << endl;
}

void eRK_FG::fireRxns(vector<double>& k, vector<int>& classif, double tau, vector<double>& a_eff){
	// Error check
	if (k.size() != this->rxn.size()){
		cout << "Error in eRK_FG::fireRxns(): 'k' and 'rxn' vectors must be equal sizes. Exiting.\n";
		exit(1);
	}
	if (classif.size() != this->rxn.size()){
		cout << "Error in eRK_FG::fireRxns(): 'classif' and 'rxn' vectors must be equal sizes. Exiting.\n";
		exit(1);
	}
	if (a_eff.size() != this->rxn.size()){
		cout << "Error in eRK_FG::fireRxns(): 'a_eff' and 'rxn' vectors must be equal sizes. Exiting.\n";
		exit(1);
	}
	//
	for (unsigned int v=0;v < this->rxn.size();v++){
		if (classif[v] != RxnClassifier::EXACT_STOCHASTIC){
			k[v] = this->fireRxn(v,a_eff[v]*tau,classif[v]);
		}
		else{
			k[v] = 0.0;
		}
	}
}

double eRK_FG::fireRxn(unsigned int u, double a_tau, int classif){

//	double a_tau = this->a_eff[u]*tau;
	double k = 0.0;
	if (classif == RxnClassifier::POISSON){
		k = Util::RANDOM_POISSON(a_tau);
	}
	else if (classif == RxnClassifier::LANGEVIN){
		k = a_tau + sqrt(a_tau)*Util::RANDOM_GAUSSIAN();
		if (k < 0.0){ // Just to be safe
			k = 0.0;
		}
		else if (this->round){
			k = floor(k + 0.5);
		}
	}
	else if (classif == RxnClassifier::DETERMINISTIC){
		k = a_tau;
		if (this->round){
			k = floor(k + 0.5);
		}
	}
	else{
		cout << "Error in eRK_FG::fireRxn(): Reaction classification for " << this->rxn[u]->toString()
			 << " (" << classif << ") not recognized." << endl;
		cout << "Only Poisson, Langevin and Deterministic classifications are supported. Exiting." << endl;
		exit(1);
	}
//	cout << "k = " << k << endl;
	this->rxn[u]->fire(k);
	return k;
}
