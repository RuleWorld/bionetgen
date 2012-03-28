/*
 * preleap_TC.cpp
 *
 *  Created on: Jul 29, 2011
 *      Author: Leonard Harris
 */

#include "preleap_TC.hh"

Preleap_TC::Preleap_TC(double eps) : eps(eps){
	if (debug)
		cout << "Preleap_TC constructor called." << endl;
}

Preleap_TC::Preleap_TC(const Preleap_TC& ptc) : TauCalculator(ptc), eps(ptc.eps){
	if (debug)
		cout << "Preleap_TC copy constructor called." << endl;
}

Preleap_TC::~Preleap_TC(){
	if (debug)
		cout << "Preleap_TC destructor called." << endl;
}
