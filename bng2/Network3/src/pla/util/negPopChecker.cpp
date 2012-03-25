/*
 * negPopChecker.cpp
 *
 *  Created on: Feb 15, 2011
 *      Author: Leonard Harris
 */
#include "plCheckers.hh"
/*
NegPopChecker::NegPopChecker() : sp(){
	if (MoMMA::debug)
		cout << "NegPopChecker constructor called." << endl;
}
*/
NegPopChecker::NegPopChecker(vector<SimpleSpecies*>& sp) : sp(sp){
	if (debug)
		cout << "NegPopChecker constructor called." << endl;
}

NegPopChecker::NegPopChecker(const NegPopChecker& ch) : sp(ch.sp){
	if (debug)
		cout << "NegPopChecker copy constructor called." << endl;
}

NegPopChecker::~NegPopChecker(){
	if (debug)
		cout << "NegPopChecker destructor called." << endl;
}

bool NegPopChecker::check(){
	for (unsigned int j=0;j < this->sp.size();j++){
		if (this->sp[j]->population < 0.0){
///*
			cout << "Uh oh, species " << this->sp[j]->name << " has a negative population ("
				 << this->sp[j]->population << ").\n";
//*/
			return false;
		}
	}
	return true;
}
