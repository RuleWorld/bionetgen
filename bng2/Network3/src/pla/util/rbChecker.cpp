/*
 * rbChecker.cpp
 *
 *  Created on: Mar 15, 2012
 *      Author: Leonard Harris
 */

#include "plCheckers.hh"

RBChecker::RBChecker(double eps, vector<Reaction*>& rxn) : eps(eps), rxn(rxn){
	if (debug)
		cout << "RBChecker constructor called." << endl;
	// Error check
	if (eps < 0.0 || eps > 1.0){
		cout << "Error in RBChecker constructor: ";
		cout << "epsilon must be >= 0.0 and <= 1.0; your eps = " << eps << ". Exiting." << endl;
		exit(1);
	}
}

RBChecker::RBChecker(const RBChecker& ch) : eps(ch.eps), rxn(ch.rxn){
	if (debug)
		cout << "RBChecker copy constructor called." << endl;
}

RBChecker::~RBChecker(){
	if (debug)
		cout << "RBChecker destructor called." << endl;
}

bool RBChecker::check(double w, vector<double>& a_check, vector<double*>& x_ref, bool postcheck){

	// Error check
	if (a_check.size() != this->rxn.size()){
		cerr << "Error in RBChecker::check(): 'a_eff' and 'rxn' vectors must be equal sizes. Exiting.\n";
		exit(1);
	}
	if (x_ref.size() != this->rxn.size()){
		cerr << "Error in RBChecker::check(): 'refPop' and 'rxn' vectors must be equal sizes. Exiting.\n";
		exit(1);
	}
//	cout << "Checking" << endl;
	bool skip;
	double X_vj, a_v;
	for (unsigned int v=0;v < this->rxn.size();v++){
		skip = true;
		//
//		for (unsigned int j=0;j < this->rxn[v]->rateSpecies.size() && skip;j++){
		for (unsigned int j=0;j < this->rxn[v]->rateSpecies.size();j++){ // Loop over all rateSpecies to check for neg pops
			X_vj = this->rxn[v]->rateSpecies[j]->population;
			if (X_vj < 0.0 || x_ref[v][j] < 0.0){
/*				if (X_vj < 0.0){
					cout << "Uh oh, species " << this->rxn[v]->rateSpecies[j]->name
						 << " has a negative population (" << X_vj << ")." << endl;
				}*/
				return false;
			}
			// If species population change <= 1.0, skip
//			if (fabs(X_vj - x_ref[v][j]) > (1.0 + TOL)){
			if (skip && fabs(X_vj - x_ref[v][j]) > (1.0 + TOL)){
				skip = false;
			}
		}
		//
		if (!skip){
			a_v = this->rxn[v]->getRate();
			double aScale;
			if (postcheck) aScale = a_check[v];
			else aScale = a_v;
			if ( fabs(a_v - a_check[v]) > w*this->eps*aScale ){
//				cout << "**Rejected**" << endl;
/*				cout << "a_eff[v] = " << a_eff[v]
				     << ", a_target = " << a_eff[v]*(1+this->eps)
				     << ", a[v] = " << a_v
				     << ", abs(av-a_eff)/a_eff = " << fabs(a_v-a_eff[v])/a_eff[v]
				     << ", eps = " << this->eps << endl;*/
				return false;
			}
		}
	}
	//
//	cout << "**Accepted**" << endl;
	return true;
}
