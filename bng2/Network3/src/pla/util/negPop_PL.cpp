/*
 * negPopPL.cpp
 *
 *  Created on: Feb 15, 2011
 *      Author: Leonard Harris
 */

#include "negPop_PL.hh"
/*
NegPop_PL::NegPop_PL() : sp(){
	if (MoMMA::debug)
		cout << "NegPop_PL constructor called." << endl;
}
*/
NegPop_PL::NegPop_PL(double p, vector<SimpleSpecies*>& sp, vector<Reaction*>& rxn) : BinomialCorrector_PL(p,rxn), sp(sp){
	if (debug)
		cout << "NegPop_PL constructor called." << endl;
}

NegPop_PL::NegPop_PL(const NegPop_PL& pl) : BinomialCorrector_PL(pl), sp(pl.sp){
	if (debug)
		cout << "NegPop_PL copy constructor called." << endl;
}

NegPop_PL::~NegPop_PL(){
	if (debug)
		cout << "NegPop_PL destructor called." << endl;
}

bool NegPop_PL::check(){
	for (unsigned int j=0;j < this->sp.size();j++){
		if (this->sp[j]->population < 0.0){
			cout << "Uh oh, species " << this->sp[j]->name << " has a negative population ("
				 << this->sp[j]->population << ").\n";
			return false;
		}
	}
	return true;
}
