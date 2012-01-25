/*
 * eRKsb_PL.cpp
 *
 *  Created on: May 1, 2011
 *      Author: Leonard Harris
 */

#include "eRK.hh"

eRKsb_PL::eRKsb_PL(double eps, vector<SimpleSpecies*>& sp) : eps(eps), sp(sp){
	if (debug)
		cout << "eRKsb_PL constructor called." << endl;
	// Error check
	if (eps < 0.0 || eps > 1.0){
		cout << "Error in eRKsb_PL constructor: ";
		cout << "epsilon must be >= 0.0 and <= 1.0; your eps = " << eps << ". Exiting." << endl;
		exit(1);
	}
}

eRKsb_PL::eRKsb_PL(const eRKsb_PL& pl) : eps(pl.eps), sp(pl.sp){
	if (debug)
		cout << "eRKsb_PL copy constructor called." << endl;
}

eRKsb_PL::~eRKsb_PL(){
	if (debug)
		cout << "eRKsb_PL destructor called." << endl;
}

bool eRKsb_PL::check(double w, vector<double>& X_eff, vector<double>& refPop, vector<double>& ref_g, bool postleap){
	// Error check
	if (X_eff.size() != this->sp.size()){
		cout << "Error in eRKsb_PL::check(): 'X_eff' and 'sp' vectors must be equal sizes. Exiting.\n";
		exit(1);
	}
	if (refPop.size() != this->sp.size()){
		cout << "Error in eRKsb_PL::check(): 'refPop' and 'sp' vectors must be equal sizes. Exiting.\n";
		exit(1);
	}
	if (ref_g.size() != this->sp.size()){
		cout << "Error in eRKsb_PL::check(): 'ref_g' and 'sp' vectors must be equal sizes. Exiting.\n";
		exit(1);
	}
	double X_j, dX_j, dXeff_j;
	for (unsigned int j=0;j < this->sp.size();j++){
		X_j = this->sp[j]->population;
		if (X_j < 0.0 || refPop[j] < 0.0){
			if (X_j < 0.0){
				cout << "Uh oh, species " << this->sp[j]->name << " has a negative population (" << X_j << ").\n";
			}
			return false;
		}
		dX_j = fabs(X_j - refPop[j]);
		dXeff_j = fabs(X_j - X_eff[j]);
		double xScale;
		if (postleap) xScale = X_eff[j];
		else xScale = X_j;
		if ( dXeff_j > w*this->eps*xScale/ref_g[j] && dX_j > (1.0 + TOL) ){
			cout << this->sp[j]->name << ": "
				 << "X_old = " << refPop[j]
				 << ", Xeff[" << j << "] = " << X_eff[j]
			     << ", X_target = " << X_eff[j]*(1+this->eps/ref_g[j])
			     << ", X[" << j << "] = " << X_j
			     << ", abs(Xj-Xeff)/Xeff = " << fabs(X_j-X_eff[j])/X_eff[j]
			     << ", eps/g_j = " << this->eps/ref_g[j] << endl;
			return false;
		}
	}
	return true;
}
/*
bool eRKsb_PL::check(double w, vector<double>& X_eff, vector<double>& oldPop, vector<double>& old_g){
	// Error check
	if (X_eff.size() != this->sp.size()){
		cout << "Error in eRKsb_PL::check(): 'X_eff' and 'sp' vectors must be equal sizes. Exiting.\n";
		exit(1);
	}
	if (oldPop.size() != this->sp.size()){
		cout << "Error in eRKsb_PL::check(): 'oldPop' and 'sp' vectors must be equal sizes. Exiting.\n";
		exit(1);
	}
	if (old_g.size() != this->sp.size()){
		cout << "Error in eRKsb_PL::check(): 'old_g' and 'sp' vectors must be equal sizes. Exiting.\n";
		exit(1);
	}
	double X_j, dX_j, dXeff_j;
	for (unsigned int j=0;j < this->sp.size();j++){
		X_j = this->sp[j]->population;
		if (X_j < 0.0){
			cout << "Uh oh, species " << this->sp[j]->name << " has a negative population ("
				 << this->sp[j]->population << ").\n";
			return false;
		}
		dX_j = fabs(X_j - oldPop[j]);
		dXeff_j = fabs(X_j - X_eff[j]);
		if ( dXeff_j > w*this->eps*X_eff[j]/old_g[j] && dX_j > (1.0 + MoMMA::TOL) ){
			return false;
		}
	}
	return true;
}
*/
