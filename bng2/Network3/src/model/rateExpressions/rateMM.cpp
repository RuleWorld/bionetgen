/*
 * rateMM.cpp
 *
 *  Created on: Jul 15, 2011
 *      Author: Leonard Harris
 */

#include "../rateExpression.hh"
#include "../../util/util.hh"

RateMM::RateMM(double kcat, double Km, vector<SimpleSpecies*> r, vector<int> rS) : kcat(kcat), Km(Km){
	// Error check
	if (r.size() != 2){
		cout << "Error in RateMM constructor: There must be exactly 2 reactant species (r.size = " << r.size() << "). Exiting." << endl;
		exit(1);
	}
	if (rS.size() != 2){
		cout << "Error in RateMM constructor: There must be exactly 2 reactant species (rS.size = " << rS.size() << "). Exiting." << endl;
		exit(1);
	}
	if (r[0] == r[1]){
		cout << "Error in RateMM constructor: Substrate and enzyme species must be different (r[0] = r[1] = ";
		cout << r[0]->name << "). Exiting." << endl;
		exit(1);
	}
	if (rS[0] != -1){
		cout << "Error in RateMM constructor: Substrate stoichiometry must be exactly -1. Exiting." << endl;
		cout << "(" << r[0]->name << ": stoich = " << rS[0] << ")" << endl;
		exit(1);
	}
	if (rS[1] != -1){
		cout << "Error in RateMM constructor: Enzyme stoichiometry must be exactly -1. Exiting." << endl;
		cout << "(" << r[1]->name << ": stoich = " << rS[1] << ")" << endl;
		exit(1);
	}
	// Get type string
	this->type = "MICHAELIS_MENTEN:{";
	this->type += r[0]->name + " + " + r[1]->name + " ->}{";
	this->type += "kcat=" + Util::toString(this->kcat) + ",Km=" + Util::toString(this->Km) + "}";
}

RateMM::~RateMM(){
	if (debug)
	cout << "RateMM destructor called." << endl;
}

double RateMM::getRate(vector<double> X){
	// Error check
	if (X.size() != 2){
		cout << "Error in RateMM::getRate(): Size of 'X' vector must be exactly 2. Exiting." << endl;
		exit(1);
	}
//	for (unsigned int i=0;i < X.size();i++){
//		// Error check
//		if (X[i] < 0.0){
//			cout << "Error in RateMM::getRate(): Negative population detected (X[" << i << "] = " << X[i] << "). Exiting." << endl;
//			exit(1);
//		}
//	}
	// Rate calculation
	double St = X[0];
	double Et = X[1];
	double b = St - Et - this->Km;
	double S = 0.5*(b + sqrt(b*b + 4.0*St*this->Km));
	double rate = this->kcat*Et*S/(this->Km + S);
	return rate;
}

double RateMM::get_dRate_dX(unsigned int which, vector<double> X){
	// Error check
	if (X.size() != 2){
		cout << "Error in RateMM::get_dRate_dX(): Size of 'X' vector must be exactly 2. Exiting." << endl;
		exit(1);
	}
	if (which > 1){
		cout << "Error in RateMM::get_dRate_dX(): Parameter 'which' cannot be greater than 1. Exiting." << endl;
		exit(1);
	}
//	for (unsigned int i=0;i < X.size();i++){
//		// Error check
//		if (X[i] < 0.0){
//			cout << "Error in RateMM::get_dRate_dX(): Negative population detected (X[" << i << "] = " << X[i] << "). Exiting." << endl;
//			exit(1);
//		}
//	}
	// Rate calculation
	double St = X[0];
	double Et = X[1];
	double b = St - Et - this->Km;
	double sqrt_bb4ac = sqrt(b*b + 4.0*St*this->Km);
	double S = 0.5*(b + sqrt_bb4ac);
	//
	double dRate = this->kcat/(this->Km + S);
	//
	if (which == 0){ // Substrate
//		double dS_dSt = (this->Km + S)/sqrt_;
//		dRate *= this->Km*Et/(this->Km + S)*dS_dSt;
		dRate *= this->Km*Et/sqrt_bb4ac;
	}
	else{ // Enzyme
		double dS_dEt = -S/sqrt_bb4ac;
		dRate *= (S + this->Km*Et/(this->Km + S)*dS_dEt);
	}
	return dRate;
}
