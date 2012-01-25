/*
 * fEulerSB_PL.cpp
 *
 *  Created on: Mar 8, 2011
 *      Author: Leonard Harris
 */

#include "fEuler.hh"
/*
fEulerSB_PL::fEulerSB_PL() : sp(), rxn(){
	if (MoMMA::debug)
		cout << "fEulerSB_PL constructor called." << endl;
}
*/
fEulerSB_PL::fEulerSB_PL(double eps, double p, vector<SimpleSpecies*>& sp, vector<Reaction*>& rxn)
	: BinomialCorrector_PL(p,rxn), eps(eps), sp(sp), rxn(rxn){
	if (debug)
		cout << "fEulerSB_PL constructor called." << endl;
	// Error check
	if (eps < 0.0 || eps > 1.0){
		cout << "Error in fEulerSB_PL constructor: ";
		cout << "epsilon must be >= 0.0 and <= 1.0; your eps = " << this->eps << endl;
		exit(1);
	}
	//
	// Create g_Getter
	this->gGet = new g_Getter(this->sp,this->rxn);
	//
	// Initialize oldPop[] and old_g[]
	this->oldPop.resize(this->sp.size());
	this->old_g.resize(this->sp.size());
	for (unsigned int j=0;j < this->sp.size();j++){
		this->oldPop[j] = this->sp[j]->population;
		this->old_g[j] = this->gGet->get_g(j);
	}
}

fEulerSB_PL::fEulerSB_PL(const fEulerSB_PL& pl)
	: BinomialCorrector_PL(pl), eps(pl.eps), oldPop(pl.oldPop), old_g(pl.old_g), sp(pl.sp), rxn(pl.rxn){
	if (debug)
		cout << "fEulerSB_PL copy constructor called." << endl;
	this->gGet = new g_Getter(this->sp,this->rxn);
}

fEulerSB_PL::~fEulerSB_PL(){
	if (debug)
		cout << "fEulerSB_PL destructor called." << endl;
	delete this->gGet;
}

bool fEulerSB_PL::wCheck(double w){
//	cout << "**Checking**" << endl;
	// Error check
	if (w < 0.0){
		cout << "Error in fEulerSB_PL::check(): w must be >= 0.0; your w = " << w << endl;
		exit(1);
	}
	//
	double X_j, dX_j;
	for (unsigned int j=0;j < this->sp.size();j++){
		X_j = this->sp[j]->population;
		if (X_j < 0.0){
			cout << "Uh oh, species " << this->sp[j]->name << " has a negative population (" << this->sp[j]->population << ").\n";
			return false;
		}
		dX_j = fabs(X_j - this->oldPop[j]);
		if ( dX_j > w*this->eps*this->oldPop[j]/this->old_g[j] && dX_j > (1.0 + TOL) ){
			return false;
		}
	}
	return true;
}

void fEulerSB_PL::update(){
	// Update oldPop[] and old_g[]
	for (unsigned int j=0;j < this->sp.size();j++){
		this->oldPop[j] = this->sp[j]->population;
		this->old_g[j] = this->gGet->get_g(j);
	}
}
