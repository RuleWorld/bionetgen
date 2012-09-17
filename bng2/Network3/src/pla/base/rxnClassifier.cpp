/*
 * rxnClassifier.cpp
 *
 *  Created on: Feb 13, 2011
 *      Author: Leonard Harris
 */

#include "rxnClassifier.hh"

using namespace std;

RxnClassifier::RxnClassifier() : force(-1){
	if (debug)
		cout << "RxnClassifier constructor called.\n";
}

RxnClassifier::RxnClassifier(const RxnClassifier& rc) : force(rc.force){
	if (debug)
		cout << "RxnClassifier copy constructor called.\n";
}

RxnClassifier::~RxnClassifier(){
	if (debug)
		cout << "RxnClassifier destructor called.\n";
}
/*
void RxnClassifier::classifyRxns(vector<int>& classif, double tau, bool initial){
	cout << "Error in RxnClassifier::classifyRxns(): Cannot use base method, must overload. Exiting." << endl;
	exit(1);
}
*/
