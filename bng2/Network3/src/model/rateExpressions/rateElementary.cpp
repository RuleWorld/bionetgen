/*
 * rateElementary.cpp
 *
 *  Created on: Feb 20, 2011
 *      Author: Leonard Harris
 */

#include "../rateExpression.hh"
#include "../../util/util.hh"

RateElementary::RateElementary(double c, vector<SimpleSpecies*> r, vector<int> rS) : c(c), rStoich(rS){
	if (debug)
		cout << "RateElementary constructor called." << endl;
	// Error check
	if (r.size() != rS.size()){
		cout << "Error in RateElementary constructor: 'r' and 'rS' vectors must be equal size. Exiting." << endl;
		exit(1);
	}
	for (unsigned int i=0;i < rS.size();i++){
		if (rS[i] >= 0.0){
			cout << "Error in RateElementary constructor: Reactant stoichiometries must be negative. Exiting." << endl;
			cout << "(" << r[i]->name << ": stoich = " << rS[i] << ")" << endl;
			exit(1);
		}
	}
	for (unsigned int i=0;i < r.size();i++){
		for (unsigned int j=i+1;j < r.size();j++){
			if (r[i] == r[j]){
				cout << "Error in RateElementary constructor: Duplicate reactant species not allowed (r[" << i << "] = r["
					 << j << "] = " << r[i]->name << "). Exiting." << endl;
				exit(1);
			}
		}
	}
	// Get type string
	this->type = "ELEMENTARY:{";
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
	this->type += "k=" + Util::toString(this->c) + "}";
}

RateElementary::~RateElementary(){
	if (debug)
		cout << "RateElementary destructor called." << endl;
}

double RateElementary::getRate(vector<double> X){
	// Error check
	if (X.size() != this->rStoich.size()){
		cout << "Error in RateElementary::getRate(): Size of 'X' vector not equal to that of 'rStoich' vector. Exiting." << endl;
		exit(1);
	}
	// Rate calculation
	double rate = this->c;
	for (unsigned int i=0;i < X.size();i++){
		// Error check
//		if (X[i] < 0.0){
//			cout << "Error in RateElementary::getRate(): Negative population detected (X[" << i << "] = " << X[i] << "). Exiting." << endl;
//			exit(1);
//		}
		// Loop over stoichiometric factor
		for (int j=0;j < fabs(this->rStoich[i]);j++){
			rate *= (X[i]-(double)j)/((double)j+1.0);
		}
	}
	return rate;
}

double RateElementary::get_dRate_dX(unsigned int which, vector<double> X){
	// Error check
	if (X.size() != this->rStoich.size()){
		cout << "Error in RateElementary::get_dRate_dX(): Size of 'X' vector not equal to that of 'rStoich' vector. Exiting." << endl;
		exit(1);
	}
	if (which >= X.size()){ // This will prevent zeroth-order rxns from being passed
		cout << "Error in RateElementary::get_dRate_dX(): Parameter 'which' larger than size of 'X' vector. Exiting." << endl;
		exit(1);
	}
	//
	double dRate = this->c;
	for (unsigned int i=0;i < X.size();i++){
		// Error check
//		if (X[i] < 0.0){
//			cout << "Error in RateElementary::get_dRate_dX(): Negative population detected (X[" << i << "] = " << X[i] << "). Exiting.\n";
//			exit(1);
//		}
		//
		if (i != which){
			for (int j=0;j < fabs(this->rStoich[i]);j++){
				dRate *= (X[i]-(double)j)/((double)j+1.0);
			}
		}
		else{
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
		}
	}
	return dRate;
}
