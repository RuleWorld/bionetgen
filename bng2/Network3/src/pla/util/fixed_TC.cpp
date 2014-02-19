/*
 * fixed_TC.cpp
 *
 *  Created on: Dec 10, 2013
 *      Author: Len
 */

#include "fixed_TC.hh"

Fixed_TC::Fixed_TC(double tau) : Preleap_TC(tau){
	if (debug)
		cout << "Fixed_TC constructor called." << endl;
	// Error check
	if (tau < 0.0){
		cout << "Error in Fixed_TC constructor: tau cannot be negative." << endl;
		exit(1);
	}
}

Fixed_TC::Fixed_TC(const Fixed_TC& tc) : Preleap_TC(tc){
	if (debug)
		cout << "Fixed_TC copy constructor called." << endl;
}

Fixed_TC::~Fixed_TC(){
	if (debug)
		cout << "Fixed_TC destructor called." << endl;
}
