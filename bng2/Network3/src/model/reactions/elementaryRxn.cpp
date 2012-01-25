/*
 * elementaryRxn.cpp
 *
 *  Created on: Feb 20, 2011
 *      Author: Leonard Harris
 */

#include "bioNetGenRxn.hh"
/*
ElementaryRxn::ElementaryRxn(){
	if (MoMMA::debug)
		cout << "ElementaryRxn constructor called." << endl;
}
*/
ElementaryRxn::ElementaryRxn(double c, vector<SimpleSpecies*> r, vector<int> rS, vector<SimpleSpecies*> p, vector<int> pS)
	: BioNetGenRxn(r,rS,p,pS){
	this->construct(c);
}

ElementaryRxn::ElementaryRxn(double c, SimpleSpecies* r, int rS, vector<SimpleSpecies*> p, vector<int> pS)
	: BioNetGenRxn(vector<SimpleSpecies*>(1,r),vector<int>(1,rS),p,pS){
	this->construct(c);
}

ElementaryRxn::ElementaryRxn(double c, vector<SimpleSpecies*> r, vector<int> rS, SimpleSpecies* p, int pS)
	: BioNetGenRxn(r,rS,vector<SimpleSpecies*>(1,p),vector<int>(1,pS)){
	this->construct(c);
}

ElementaryRxn::ElementaryRxn(double c, SimpleSpecies* r, int rS, SimpleSpecies* p, int pS)
	: BioNetGenRxn(vector<SimpleSpecies*>(1,r),vector<int>(1,rS),vector<SimpleSpecies*>(1,p),vector<int>(1,pS)){
	this->construct(c);
}

ElementaryRxn::~ElementaryRxn(){
	if (debug)
		cout << "ElementaryRxn destructor called." << endl;
	delete this->re;
}

void ElementaryRxn::construct(double c){
	if (debug)
		cout << "ElementaryRxn constructor called." << endl;
	this->re = new RateElementary(c,this->rateSpecies,this->rStoich); // Don't forget to delete this in the destructor
	this->type = this->re->type;
	this->string_ID = Reaction::getStringID(this->stoichSpecies,this->re->toString());
}
