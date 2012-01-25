/*
 * rateExpression.cpp
 *
 *  Created on: Oct 5, 2010
 *      Author: Leonard Harris
 */

#include "rateExpression.hh"

RateExpression::RateExpression() : type("UNKNOWN"){
	if (debug)
		cout << "RateExpression constructor called." << endl;
}

RateExpression::~RateExpression(){
	if (debug)
		cout << "RateExpression destructor called: " << this->type << endl;
}

double RateExpression::getRate(vector<double> X){
	cout << "Error: Cannot use base RateExpression::getRate(). Choose a standard one or create your own. Exiting." << endl;
	exit(1);
	return NAN;
}

double RateExpression::get_dRate_dX(unsigned int which, vector<double> X){
	// Error check
	if (which >= X.size()){
		cout << "Error in RateExpression::get_dRate_dX(): Parameter 'which' larger than size of X vector. Exiting." << endl;
		exit(1);
	}
	// In the absence of an overloaded method, call the static numerical_deriv() method
	if (debug)
		cout << "Warning: Using default RateExpression::numerical_deriv() method to calculate derivative." << endl;
	return RateExpression::numerical_deriv(which,X,this);
}

double RateExpression::numerical_deriv(unsigned int which, vector<double> X, RateExpression* re){
	double dRate;
	X[which] += 1.0;
	double r_plus = re->getRate(X);
	if (X[which] >= 10){ // Use a 3-point approximation: [f(x+h)-f(x-h)]/2h
		X[which] -= 2.0;
		dRate = 0.5*(r_plus - re->getRate(X));
	}
	else{ // Otherwise, use a 2-point approximation: [f(x+h)-f(x)]/h
		X[which] -= 1.0;
		dRate = r_plus - re->getRate(X);
	}
	return dRate;
}

