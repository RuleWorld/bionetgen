/*
 * michaelisMentenRxn.cpp
 *
 *  Created on: Jul 16, 2011
 *      Author: Leonard Harris
 */

#include "bioNetGenRxn.hh"

MichaelisMentenRxn::MichaelisMentenRxn(double kcat, double Km, vector<SimpleSpecies*> r, vector<int> rS, vector<SimpleSpecies*> p, vector<int> pS)
	: BioNetGenRxn(r,rS,p,pS){
	this->construct(kcat,Km);
}

MichaelisMentenRxn::MichaelisMentenRxn(double kcat, double Km, SimpleSpecies* r, int rS, vector<SimpleSpecies*> p, vector<int> pS)
	: BioNetGenRxn(vector<SimpleSpecies*>(1,r),vector<int>(1,rS),p,pS){
	this->construct(kcat,Km);
}

MichaelisMentenRxn::MichaelisMentenRxn(double kcat, double Km, vector<SimpleSpecies*> r, vector<int> rS, SimpleSpecies* p, int pS)
	: BioNetGenRxn(r,rS,vector<SimpleSpecies*>(1,p),vector<int>(1,pS)){
	this->construct(kcat,Km);
}

MichaelisMentenRxn::MichaelisMentenRxn(double kcat, double Km, SimpleSpecies* r, int rS, SimpleSpecies* p, int pS)
	: BioNetGenRxn(vector<SimpleSpecies*>(1,r),vector<int>(1,rS),vector<SimpleSpecies*>(1,p),vector<int>(1,pS)){
	this->construct(kcat,Km);
}

MichaelisMentenRxn::~MichaelisMentenRxn(){
	if (debug)
		cout << "MichaelisMentenRxn destructor called." << endl;
	delete this->re;
}

void MichaelisMentenRxn::construct(double kcat, double Km){
	if (debug)
		cout << "MichaelisMentenRxn constructor called." << endl;
	this->re = new RateMM(kcat,Km,this->rateSpecies,this->rStoich); // Don't forget to delete this in the destructor
	this->type = this->re->type;
	this->string_ID = Reaction::getStringID(this->stoichSpecies,this->re->toString());
}
