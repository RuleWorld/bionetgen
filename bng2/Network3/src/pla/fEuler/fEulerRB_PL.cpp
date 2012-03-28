/*
 * fEulerRB_PL.cpp
 *
 *  Created on: Mar 8, 2011
 *      Author: Leonard Harris
 */

#include "fEuler.hh"
/*
fEulerRB_PL::fEulerRB_PL() : rxn(){
	if (MoMMA::debug)
		cout << "fEulerRB_PL constructor called." << endl;
}
*/
fEulerRB_PL::fEulerRB_PL(double eps, double p, vector<Reaction*>& rxn) : eps(eps), rxn(rxn){
//: BinomialCorrector_PL(p,rxn), eps(eps), rxn(rxn){
	if (debug)
		cout << "fEulerRB_PL constructor called." << endl;
	// Error check
	if (eps < 0.0 || eps > 1.0){
		cout << "Error in fEulerRB_PL constructor: ";
		cout << "epsilon must be >= 0.0 and <= 1.0; your eps = " << this->eps << endl;
		exit(1);
	}
	// Initialize oldRate[]
	this->oldRate.resize(this->rxn.size());
	for (unsigned int v=0;v < this->rxn.size();v++){
		this->oldRate[v] = this->rxn[v]->getRate();
	}
	// Initialize oldPop[]
	this->oldPop.resize(this->rxn.size());
	for (unsigned int v=0;v < this->rxn.size();v++){
		this->oldPop[v] = new double[this->rxn[v]->rateSpecies.size()];
		for (unsigned int j=0;j < this->rxn[v]->rateSpecies.size();j++){
			this->oldPop[v][j] = this->rxn[v]->rateSpecies[j]->population;
		}
	}
}

fEulerRB_PL::fEulerRB_PL(const fEulerRB_PL& pl) : eps(pl.eps), oldRate(pl.oldRate), rxn(pl.rxn){
//: BinomialCorrector_PL(pl), eps(pl.eps), oldRate(pl.oldRate), rxn(pl.rxn){
	if (debug)
		cout << "fEulerRB_PL copy constructor called." << endl;
	// Copy oldPop[]
	this->oldPop.resize(this->rxn.size());
	for (unsigned int v=0;v < this->rxn.size();v++){
		this->oldPop[v] = new double[this->rxn[v]->rateSpecies.size()];
		for (unsigned int j=0;j < this->rxn[v]->rateSpecies.size();j++){
			this->oldPop[v][j] = pl.oldPop[v][j];
		}
	}
}

fEulerRB_PL::~fEulerRB_PL(){
	if (debug)
		cout << "fEulerRB_PL destructor called." << endl;
	for (unsigned int v=0;v < this->rxn.size();v++){
		delete[] this->oldPop[v];
	}
}

bool fEulerRB_PL::wCheck(double w){
	// Error check
	if (w < 0.0){
		cout << "Error in fEulerRB_PL::check(): w must be >= 0.0; your w = " << w << ". Exiting." << endl;
		exit(1);
	}
	//
	bool skip;
	double X_vj;
	double a_v;
	for (unsigned int v=0;v < this->rxn.size();v++){
		skip = true;
		//
		for (unsigned int j=0;j < this->rxn[v]->rateSpecies.size() && skip;j++){
			X_vj = this->rxn[v]->rateSpecies[j]->population;
			if (X_vj < 0.0){
				cout << "Uh oh, species " << this->rxn[v]->rateSpecies[j]->name << " has a negative population ("
					 << this->rxn[v]->rateSpecies[j]->population << ").\n";
				return false;
			}
			else if ( fabs(X_vj - this->oldPop[v][j]) > (1.0 + TOL)){
				skip = false;
			}
		}
		//
		if (!skip){
			a_v = this->rxn[v]->getRate();
			if ( fabs(a_v - this->oldRate[v]) > w*this->eps*this->oldRate[v] ){
//				cout << "**Rejected**" << endl;
				return false;
			}
		}
	}
	//
///	cout << "**Accepted**" << endl;
	return true;
}

void fEulerRB_PL::update(){
	// Update oldRate[] and oldPop[]
	for (unsigned int v=0;v < this->rxn.size();v++){
		this->oldRate[v] = this->rxn[v]->getRate();
		for (unsigned int j=0;j < this->rxn[v]->rateSpecies.size();j++){
			this->oldPop[v][j] = this->rxn[v]->rateSpecies[j]->population;
		}
	}
}
