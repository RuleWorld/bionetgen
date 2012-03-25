/*
 * firingGenerator.cpp
 *
 *  Created on: Feb 13, 2011
 *      Author: Leonard Harris
 */

#include "firingGenerator.hh"

using namespace std;

FiringGenerator::FiringGenerator(){
	if (debug)
		cout << "FiringGenerator constructor called.\n";
}

FiringGenerator::FiringGenerator(const FiringGenerator& fg){
	if (debug)
		cout << "FiringGenerator copy constructor called." << endl;
}

FiringGenerator::~FiringGenerator(){
	if (debug)
		cout << "FiringGenerator destructor called.\n";
}
/*
void FiringGenerator::fireRxns(vector<double>& k, vector<int>& classif, double tau){
	cout << "Error in FiringGenerator::fireRxns(): Cannot use base method, must overload. Exiting." << endl;
	exit(1);
}
*/
