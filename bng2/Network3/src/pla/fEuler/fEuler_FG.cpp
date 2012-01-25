/*
 * fEuler_FG.cpp
 *
 *  Created on: Feb 14, 2011
 *      Author: Leonard Harris
 */

#include "fEuler.hh"
/*
fEuler_FG::fEuler_FG() : round(true), rxn(){
	if (MoMMA::debug)
		cout << "fEuler_FG constructor called." << endl;
}
*/
fEuler_FG::fEuler_FG(vector<Reaction*>& rxn) : round(true), rxn(rxn){
	if (debug)
		cout << "fEuler_FG constructor called." << endl;
	// Round all species populations (default)
	for (unsigned int u=0;u < rxn.size();u++){
		// Rate species
		for (unsigned int i=0;i < rxn[u]->rateSpecies.size();i++){
			rxn[u]->rateSpecies[i]->population = floor(rxn[u]->rateSpecies[i]->population + 0.5);
		}
		// Stoichiometric species
		map<SimpleSpecies*,int>::iterator iter;
		for (iter = rxn[u]->stoichSpecies.begin();iter != rxn[u]->stoichSpecies.end();iter++){
			(*iter).first->population = floor((*iter).first->population + 0.5);
		}
	}
}

fEuler_FG::fEuler_FG(vector<Reaction*>& rxn, bool round) : round(round), rxn(rxn){
	if (debug)
		cout << "fEuler_FG constructor called." << endl;
	// Round all species populations if round = true
	if (round){
		for (unsigned int u=0;u < rxn.size();u++){
			// Rate species
			for (unsigned int i=0;i < rxn[u]->rateSpecies.size();i++){
				rxn[u]->rateSpecies[i]->population = floor(rxn[u]->rateSpecies[i]->population + 0.5);
			}
			// Stoichiometric species
			map<SimpleSpecies*,int>::iterator iter;
			for (iter = rxn[u]->stoichSpecies.begin();iter != rxn[u]->stoichSpecies.end();iter++){
				(*iter).first->population = floor((*iter).first->population + 0.5);
			}
		}
	}
}

fEuler_FG::fEuler_FG(const fEuler_FG& fg) : FiringGenerator(fg), round(fg.round), rxn(fg.rxn){
	if (debug)
		cout << "fEuler_FG copy constructor called." << endl;
}

fEuler_FG::~fEuler_FG(){
	if (debug)
		cout << "fEuler_FG destructor called." << endl;
}

double fEuler_FG::fireRxn(unsigned int u, int classif, double tau){

	double a_tau = this->rxn[u]->getRate()*tau;
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
		cout << "Error in fEuler_FG::fireRxn(): Reaction classification for " << this->rxn[u]->toString()
			 << " (" << classif << ") not recognized." << endl;
		cout << "Only Poisson, Langevin and Deterministic classifications are supported. Exiting." << endl;
		exit(1);
	}
//	cout << "k = " << k << endl;
	this->rxn[u]->fire(k);
	return k;
}

void fEuler_FG::fireRxns(vector<double>& k, vector<int>& classif, double tau){
	// Error check
	if (classif.size() != this->rxn.size()){
		cout << "Error in fEuler_FG::fireRxns(): 'classif' vector must be same size as 'rxn' vector. Exiting.\n";
		exit(1);
	}
	if (k.size() != this->rxn.size()){
		cout << "Error in fEuler_FG::fireRxns(): 'k' vector must be same size as 'rxn' vector. Exiting.\n";
		exit(1);
	}
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
