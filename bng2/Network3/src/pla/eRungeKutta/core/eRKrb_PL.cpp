/*
 * eRKrb_PL.cpp
 *
 *  Created on: May 1, 2011
 *      Author: Leonard Harris
 */

#include "eRK.hh"

eRKrb_PL::eRKrb_PL(double eps, vector<Reaction*>& rxn) : eps(eps), rxn(rxn){
	if (debug)
		cout << "eRKrb_PL constructor called." << endl;
	// Error check
	if (eps < 0.0 || eps > 1.0){
		cout << "Error in eRKrb_PL constructor: ";
		cout << "epsilon must be >= 0.0 and <= 1.0; your eps = " << eps << ". Exiting." << endl;
		exit(1);
	}
}

eRKrb_PL::eRKrb_PL(const eRKrb_PL& pl) : eps(pl.eps), rxn(pl.rxn){
	if (debug)
		cout << "eRKrb_PL copy constructor called." << endl;
}

eRKrb_PL::~eRKrb_PL(){
	if (debug)
		cout << "eRKrb_PL destructor called." << endl;
}

bool eRKrb_PL::check(double w, vector<double>& a_eff, vector<double*>& refPop, bool postleap){
	// Error check
	if (a_eff.size() != this->rxn.size()){
		cout << "Error in eRKrb_PL::check(): 'a_eff' and 'rxn' vectors must be equal sizes. Exiting.\n";
		exit(1);
	}
	if (refPop.size() != this->rxn.size()){
		cout << "Error in eRKrb_PL::check(): 'refPop' and 'rxn' vectors must be equal sizes. Exiting.\n";
		exit(1);
	}
//	cout << "Checking" << endl;
	bool skip;
	double X_vj, a_v;
	for (unsigned int v=0;v < this->rxn.size();v++){
		skip = true;
		//
		for (unsigned int j=0;j < this->rxn[v]->rateSpecies.size() && skip;j++){
			X_vj = this->rxn[v]->rateSpecies[j]->population;
			if (X_vj < 0.0 || refPop[v][j] < 0.0){
				if (X_vj < 0.0){
					cout << "Uh oh, species " << this->rxn[v]->rateSpecies[j]->name << " has a negative population ("
						 << X_vj << ")." << endl;
				}
				return false;
			}
			// If species population change <= 1.0, skip
			if ( fabs(X_vj - refPop[v][j]) > (1.0 + TOL) ){
				skip = false;
			}
		}
		//
		if (!skip){
			a_v = this->rxn[v]->getRate();
			double aScale;
			if (postleap) aScale = a_eff[v];
			else aScale = a_v;
			if ( fabs(a_v - a_eff[v]) > w*this->eps*aScale ){
//				cout << "**Rejected**" << endl;
//				cout << "a_eff[v] = " << a_eff[v]
//				     << ", a_target = " << a_eff[v]*(1+this->eps)
//				     << ", a[v] = " << a_v
//				     << ", abs(av-a_eff)/a_eff = " << fabs(a_v-a_eff[v])/a_eff[v]
//				     << ", eps = " << this->eps << endl;
				return false;
			}
		}
	}
	//
//	cout << "**Accepted**" << endl;
	return true;
}
/*
bool eRKrb_PL::check(double w, vector<double>& a_eff, vector<double*>& oldPop){
	// Error check
	if (a_eff.size() != this->rxn.size()){
		cout << "Error in eRKrb_PL::check(): 'a_eff' and 'rxn' vectors must be equal sizes. Exiting.\n";
		exit(1);
	}
	if (oldPop.size() != this->rxn.size()){
		cout << "Error in eRKrb_PL::check(): 'oldPop' and 'rxn' vectors must be equal sizes. Exiting.\n";
		exit(1);
	}
//	cout << "Checking" << endl;
	bool skip;
	double X_vj, a_v;
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
			// If species population change <= 1.0, skip
			else if ( fabs(X_vj - oldPop[v][j]) > (1.0 + MoMMA::TOL) ){
				skip = false;
			}
		}
		//
		if (!skip){
			a_v = this->rxn[v]->getRate();
			if ( fabs(a_v - a_eff[v]) > w*this->eps*a_eff[v] ){
//				cout << "**Rejected**" << endl;
				return false;
			}
		}
	}
	//
//	cout << "**Accepted**" << endl;
	return true;
}
*/
