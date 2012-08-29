/*
 * rateSaturation.cpp
 *
 *  Created on: Feb 25, 2011
 *      Author: Leonard Harris
 */

#include "../rateExpression.hh"
#include "../../util/util.hh"

RateSaturation::RateSaturation(double kcat, double Km, vector<SimpleSpecies*> r, vector<int> rS) : kcat(kcat), Km(vector<double>(1,Km)),
		rStoich(rS){
	if (debug)
		cout << "RateSaturation constructor called." << endl;
	//
	cout << "Km.size = " << this->Km.size() << endl;
	//
	this->construct(r,rS);
}

RateSaturation::RateSaturation(double kcat, vector<double> Km, vector<SimpleSpecies*> r, vector<int> rS) : kcat(kcat), Km(Km), rStoich(rS){
	if (debug)
		cout << "RateSaturation constructor called." << endl;
	//
	this->construct(r,rS);
}

void RateSaturation::construct(vector<SimpleSpecies*> r, vector<int> rS){
	// Error check
	if (r.size() != rS.size()){
		cout << "Error in RateSaturation constructor: 'r' and 'rS' vectors must be equal size. Exiting." << endl;
		exit(1);
	}
	for (unsigned int i=0;i < rS.size();i++){
		if (rS[i] >= 0.0){
			cout << "Error in RateSaturation constructor: Reactant stoichiometries must be negative. Exiting." << endl;
			cout << "(" << r[i]->name << ": stoich = " << rS[i] << ")" << endl;
			exit(1);
		}
	}
	for (unsigned int i=0;i < r.size();i++){
		for (unsigned int j=i+1;j < r.size();j++){
			if (r[i] == r[j]){
				cout << "Error in RateSaturation constructor: Duplicate reactant species not allowed (r[" << i << "] = r[" << j << "] = ";
				cout << r[i]->name << "). Exiting." << endl;
				exit(1);
			}
		}
	}
	// Get type string
	this->type = "SATURATION:{";
	if (r.size() == 0){
		this->type += "* ->}{";
	}
	else{
		for (unsigned int i=0;i < r.size();i++){
			if (i != 0) this->type += " + ";
			if (rS[i] < -1) this->type += Util::toString(-rS[i]) + "*";
			this->type += r[i]->name;
		}
		this->type += " ->}{";
	}
	this->type += "kcat=" + Util::toString(this->kcat);
	for (unsigned int i=0;i < this->Km.size();i++){
		this->type += ",Km" + Util::toString((int)i) + "=" + Util::toString(this->Km[i]);
	}
	this->type += "}";
}

RateSaturation::~RateSaturation(){
	if (debug)
		cout << "RateSaturation destructor called." << endl;
}

double RateSaturation::getRate(vector<double> X){
	// Error check
	if (X.size() != this->rStoich.size()){
		cout << "Error in RateSaturation::getRate(): Size of 'X' vector not equal to that of 'rStoich' vector. Exiting." << endl;
		exit(1);
	}
	// Rate calculation
	double rate = this->kcat;
	// Loop over reactant species
	for (unsigned int i=0;i < X.size();i++){
		// Error check
//		if (X[i] < 0.0){
//			cout << "Error in RateSaturation::getRate(): Negative population detected (X[" << i << "] = " << X[i] << "). Exiting." << endl;
//			exit(1);
//		}
		// Loop over stoichiometry
		double X_mult = 1.0;
		for (int j=0;j < fabs(this->rStoich[i]);j++){
			X_mult *= (X[i]-(double)j)/((double)j+1.0);
		}
		rate *= X_mult;
		if (i < this->Km.size()){ // A substrate
			rate /= (this->Km[i] + X_mult);
		}
	}
	return rate;
}

double RateSaturation::get_dRate_dX(unsigned int which, vector<double> X){
	// Error check
	if (X.size() != this->rStoich.size()){
		cout << "Error in RateSaturation::get_dRate_dX(): Size of 'X' vector not equal to that of 'rStoich' vector. Exiting." << endl;
		exit(1);
	}
	if (which >= X.size()){ // This will prevent zeroth-order rxns from being passed
		cout << "Error in RateSaturation::get_dRate_dX(): Parameter 'which' larger than size of 'X' vector. Exiting." << endl;
		exit(1);
	}
	//
	double dRate = this->kcat;
	for (unsigned int i=0;i < X.size();i++){
		// Error check
//		if (X[i] < 0.0){
//			cout << "Error in RateSaturation::get_dRate_dX(): Negative population detected (X[" << i << "] = " << X[i] << "). Exiting.\n";
//			exit(1);
//		}
		// Calculate X_mult = X * (X-1)/2 * (X-2)/3 *...
		double X_mult = 1.0;
		for (int j=0;j < fabs(this->rStoich[i]);j++){
			X_mult *= (X[i]-(double)j)/((double)j+1.0);
		}
		// Non-derivative terms
		if (i != which){
			// Common term
			dRate *= X_mult;
			// Extra term for substrates
			if (i < this->Km.size()){ // A substrate
				dRate /= (this->Km[i] + X_mult);
			}
		}
		// Derivative term
		else{
			// Common term
			double dX_which = 0.0;
			for (int k=0;k < fabs(this->rStoich[i]);k++){ // # of terms in summation
				double prod = 1.0;
				for (int j=0;j < fabs(this->rStoich[i]);j++){ // # of terms in each product of the summation
					if (j == k){
						prod *= 1.0/((double)j+1.0);
					}
					else{
						prod *= (X[i]-(double)j)/((double)j+1.0);
					}
				}
				dX_which += prod;
			}
			dRate *= dX_which;
			// Extra term for substrates
			if (i < this->Km.size()){
				double div = X_mult + this->Km[i];
				dRate *= this->Km[i]/div/div;
			}
		}
	}
	return dRate;
}
