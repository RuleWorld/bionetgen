/*
 * fEulerRB_TC_PL.cpp
 *
 *  Created on: Feb 18, 2011
 *      Author: Leonard Harris
 */

#include "fEuler.hh"
/*
fEulerRB_TC_PL::fEulerRB_TC_PL() : preCalc(true), ptc(){
	if (MoMMA::debug)
		cout << "fEulerRB_TC_PL constructor called." << endl;
}
*/
fEulerRB_TC_PL::fEulerRB_TC_PL(double eps, double p, double pp, double q, double w, vector<Reaction*>& rxn, Preleap_TC& ptc)
		: fEulerRB_PL(eps,p,rxn), pp(pp), q(q), w(w), preCalc(true), ptc(ptc){
	if (debug)
		cout << "fEulerRB_TC_PL constructor called." << endl;
	// Error check
//	if (this->pp < this->p){
//		cout << "Error in fEulerRB_TC_PL constructor: ";
//		cout << "pp must be >= p; you have pp = " << this->pp << ", p = " << this->p << endl;
//		exit(1);
//	}
	if (this->q < 1.0){
		cout << "Error in fEulerRB_TC_PL constructor: ";
		cout << "q must be >= 1.0; your q = " << this->q << endl;
		exit(1);
	}
	if (this->w <= 0.0 || this->w >= 1.0){
		cout << "Error in fEulerRB_TC_PL constructor: ";
		cout << "w must be > 0.0 and < 1.0; your w = " << this->w << endl;
		exit(1);
	}
}

fEulerRB_TC_PL::~fEulerRB_TC_PL(){
	if (debug)
		cout << "fEulerRB_TC_PL destructor called." << endl;
}

bool fEulerRB_TC_PL::check(){
//	cout << "**Checking**" << endl;
	bool ok;
	this->substantially = this->wCheck(this->w);
	if (this->substantially){
		ok = true;
//		cout << "**Substantially accepted**" << endl;
	}
	else{
		ok = this->wCheck(1.0);
//		if (ok) cout << "**Barely accepted**" << endl;
//		else cout << "**Rejected**" << endl;
	}
	return ok;
}

void fEulerRB_TC_PL::getNewTau(double& tau){
	if (this->preCalc){
		this->ptc.getNewTau(tau);
		this->preCalc = false;
	}
	else{
		if (this->substantially){ // Step was substantially accepted, increase tau
			tau *= this->q;
		}
		else{ // Step was barely accepted, reduce tau by a little bit
			tau *= this->pp;
		}
	}
}
