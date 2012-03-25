/*
 * sbChecker.cpp
 *
 *  Created on: Mar 15, 2012
 *      Author: Leonard Harris
 */

#include "plCheckers.hh"

SBChecker::SBChecker(double eps, vector<SimpleSpecies*>& sp) : eps(eps), sp(sp){
	if (debug)
		cout << "SBChecker constructor called." << endl;
	// Error check
	if (eps < 0.0 || eps > 1.0){
		cout << "Error in SBChecker constructor: ";
		cout << "epsilon must be >= 0.0 and <= 1.0; your eps = " << eps << ". Exiting." << endl;
		exit(1);
	}
}

SBChecker::SBChecker(const SBChecker& ch) : eps(ch.eps), sp(ch.sp){
	if (debug)
		cout << "SBChecker copy constructor called." << endl;
}

SBChecker::~SBChecker(){
	if (debug)
		cout << "SBChecker destructor called." << endl;
}

bool SBChecker::check(double w, vector<double>& x_check, vector<double>& x_ref, vector<double>& g_ref, bool postcheck){
	// Error check
	if (x_check.size() != this->sp.size()){
		cout << "Error in SBChecker::check(): 'X_eff' and 'sp' vectors must be equal sizes. Exiting.\n";
		exit(1);
	}
	if (x_ref.size() != this->sp.size()){
		cout << "Error in SBChecker::check(): 'refPop' and 'sp' vectors must be equal sizes. Exiting.\n";
		exit(1);
	}
	if (g_ref.size() != this->sp.size()){
		cout << "Error in SBChecker::check(): 'ref_g' and 'sp' vectors must be equal sizes. Exiting.\n";
		exit(1);
	}
	double X_j, dX_j, dXcheck_j;
	for (unsigned int j=0;j < this->sp.size();j++){
		X_j = this->sp[j]->population;
		if (X_j < 0.0 || x_ref[j] < 0.0){
			if (X_j < 0.0){
				cout << "Uh oh, species " << this->sp[j]->name << " has a negative population (" << X_j << ").\n";
			}
			return false;
		}
		dX_j = fabs(X_j - x_ref[j]);
		dXcheck_j = fabs(X_j - x_check[j]);
		double xScale;
		if (postcheck) xScale = x_check[j];
		else xScale = X_j;
		if ( dXcheck_j > w*this->eps*xScale/g_ref[j] && dX_j > (1.0 + TOL) ){
/*			cout << this->sp[j]->name << ": "
				 << "X_old = " << x_ref[j]
				 << ", Xeff[" << j << "] = " << x_check[j]
			     << ", X_target = " << x_check[j]*(1+this->eps/g_ref[j])
			     << ", X[" << j << "] = " << X_j
			     << ", abs(Xj-Xeff)/Xeff = " << fabs(X_j-x_check[j])/x_check[j]
			     << ", eps/g_j = " << this->eps/g_ref[j] << endl; //*/
			return false;
		}
	}
	return true;
}
