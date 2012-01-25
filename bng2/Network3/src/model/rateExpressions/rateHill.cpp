/*
 * rateHill.cpp
 *
 *  Created on: Feb 25, 2011
 *      Author: Leonard Harris
 */

#include "../rateExpression.hh"
#include "../../util/util.hh"

RateHill::RateHill(double Vmax, double Kh, double h, vector<SimpleSpecies*> r, vector<int> rS) : Vmax(Vmax), Kh(Kh), h(h), rStoich(rS){
	if (debug)
		cout << "RateHill constructor called." << endl;
	// Error check
	if (r.size() != rS.size()){
		cout << "Error in RateHill constructor: 'r' and 'rS' vectors must be equal size. Exiting." << endl;
		exit(1);
	}
	for (unsigned int i=0;i < rS.size();i++){
		if (rS[i] >= 0.0){
			cout << "Error in RateHill constructor: Reactant stoichiometries must be negative. Exiting." << endl;
			cout << "(" << r[i]->name << ": stoich = " << rS[i] << ")" << endl;
			exit(1);
		}
	}
	for (unsigned int i=0;i < r.size();i++){
		for (unsigned int j=i+1;j < r.size();j++){
			if (r[i] == r[j]){
				cout << "Error in RateHill constructor: Duplicate reactant species not allowed (r[" << i << "] = r["
					 << j << "] = " << r[i]->name << "). Exiting." << endl;
				exit(1);
			}
		}
	}
	// Get type string
	this->type = "Hill:{";
	if (r.size() == 0){
		this->type += "* ->}{";
	}
	else{
		for (unsigned int i=0;i < r.size();i++){
			if (i != 0) this->type += " + ";
			if (rS[i] < -1) this->type += Util::toString(-rS[i]);
			this->type += r[i]->name;
		}
		this->type += " ->}{";
	}
	this->type += "Vmax=" + Util::toString(this->Vmax) + ",Kh=" + Util::toString(this->Kh) +
			",h=" + Util::toString(this->h) + "}";
}

RateHill::~RateHill(){
	if (debug)
		cout << "RateHill destructor called." << endl;
}

double RateHill::getRate(vector<double> X){
	// Error check
	if (X.size() != this->rStoich.size()){
		cout << "Error in RateHill::getRate(): Size of 'X' vector not equal to that of 'rStoich' vector. Exiting." << endl;
		exit(1);
	}
	// Rate calculation
	double rate = this->Vmax;
	// Loop over reactant species
	for (unsigned int i=0;i < X.size();i++){
		// Error check
//		if (X[i] < 0.0){
//			cout << "Error in RateHill::getRate(): Negative population detected (X[" << i << "] = " << X[i] << "). Exiting." << endl;
//			exit(1);
//		}
		// Loop over stoichiometry
		double X_mult = 1.0;
		for (int j=0;j < fabs(this->rStoich[i]);j++){
			X_mult *= (X[i]-(double)j)/((double)j+1.0);
		}
		// First species
		if (i == 0){
			double X_h = pow(X_mult,this->h);
			rate *= X_h/( pow(this->Kh,this->h) + X_h );
		}
		// Other species
		else{
			rate *= X_mult;
		}
	}
	return rate;
}

double RateHill::get_dRate_dX(unsigned int which, vector<double> X){
	// Error check
	if (X.size() != this->rStoich.size()){
		cout << "Error in RateHill::get_dRate_dX(): Size of 'X' vector not equal to that of 'rStoich' vector. Exiting." << endl;
		exit(1);
	}
	if (which >= X.size()){ // This will prevent zeroth-order rxns from being passed
		cout << "Error in RateHill::get_dRate_dX(): Parameter 'which' larger than size of 'X' vector. Exiting." << endl;
		exit(1);
	}
//	if (X[0] < 0.0){
//		cout << "Error in RateHill::get_dRate_dX(): Negative population detected (X[" << 0 << "] = " << X[0] << "). Exiting." << endl;
//		exit(1);
//	}
	//
	double dRate = this->Vmax;
	//
	double X0_mult = 1.0;
	for (int j=0;j < fabs(this->rStoich[0]);j++){
		X0_mult *= (X[0]-(double)j)/((double)j+1.0);
	}
	// First species
	if (which == 0){
		// Loop over other species
		for (unsigned int i=1;i < X.size();i++){
			// Error check
			if (X[i] < 0.0){
				cout << "Error in RateHill::get_dRate_dX(): Negative population detected (X[" << i << "] = " << X[i] << "). Exiting.\n";
				exit(1);
			}
			//
			for (int j=0;j < fabs(this->rStoich[i]);j++){
				dRate *= (X[i]-(double)j)/((double)j+1.0);
			}
		}
		// Back to the first species
		double denom = pow(X0_mult,this->h) + pow(this->Kh,this->h);
		dRate *= this->h*pow(X0_mult,this->h-1.0)/denom;
		dRate *= (1.0 - pow(X0_mult,this->h)/denom);
		// derivative
		double dX_0 = 0.0;
		for (unsigned int j=0;j < fabs(this->rStoich[0]);j++){
			double prod = 1.0/((double)j+1.0);
			for (unsigned int k=0;k < fabs(this->rStoich[0]);k++){
				if (k != j){
					prod *= (X[0]-(double)k)/((double)k+1.0);
				}
			}
			dX_0 += prod;
		}
		dRate *= dX_0;
	}
	// Other species
	else{
		double X0_h = pow(X0_mult,this->h);
		dRate *= X0_h/( pow(this->Kh,this->h) + X0_h );
		for (unsigned int i=1;i < X.size();i++){
			// Error check
//			if (X[i] < 0.0){
//				cout << "Error in RateHill::get_dRate_dX(): Negative population detected (X[" << i << "] = " << X[i] << "). Exiting.\n";
//				exit(1);
//			}
			//
			if (i == which){
				double dX_i = 0.0;
				for (unsigned int j=0;j < fabs(this->rStoich[i]);j++){
					double prod = 1.0/((double)j+1.0);
					for (unsigned int k=0;k < fabs(this->rStoich[i]);k++){
						if (k != j){
							prod *= (X[i]-(double)k)/((double)k+1.0);
						}
					}
					dX_i += prod;
				}
				dRate *= dX_i;
			}
			else{
				for (int j=0;j < fabs(this->rStoich[i]);j++){
					dRate *= (X[i]-(double)j)/((double)j+1.0);
				}
			}
		}
	}
	return dRate;
}
