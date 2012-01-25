/*
 * hillRxn.cpp
 *
 *  Created on: Jul 14, 2011
 *      Author: Leonard Harris
 */

#include "bioNetGenRxn.hh"

HillRxn::HillRxn(double Vmax, double Kh, double h, vector<SimpleSpecies*> r, vector<int> rS, vector<SimpleSpecies*> p, vector<int> pS)
	: BioNetGenRxn(r,rS,p,pS){
	this->construct(Vmax,Kh,h);
}

HillRxn::HillRxn(double Vmax, double Kh, double h, SimpleSpecies* r, int rS, vector<SimpleSpecies*> p, vector<int> pS)
	: BioNetGenRxn(vector<SimpleSpecies*>(1,r),vector<int>(1,rS),p,pS){
	this->construct(Vmax,Kh,h);
}

HillRxn::HillRxn(double Vmax, double Kh, double h, vector<SimpleSpecies*> r, vector<int> rS, SimpleSpecies* p, int pS)
	: BioNetGenRxn(r,rS,vector<SimpleSpecies*>(1,p),vector<int>(1,pS)){
	this->construct(Vmax,Kh,h);
}

HillRxn::HillRxn(double Vmax, double Kh, double h, SimpleSpecies* r, int rS, SimpleSpecies* p, int pS)
	: BioNetGenRxn(vector<SimpleSpecies*>(1,r),vector<int>(1,rS),vector<SimpleSpecies*>(1,p),vector<int>(1,pS)){
	this->construct(Vmax,Kh,h);
}

HillRxn::~HillRxn(){
	if (debug)
		cout << "HillRxn destructor called." << endl;
	delete this->re;
}

void HillRxn::construct(double Vmax, double Kh, double h){
	if (debug)
		cout << "HillRxn constructor called." << endl;
	this->re = new RateHill(Vmax,Kh,h,this->rateSpecies,this->rStoich); // Don't forget to delete this in the destructor
	this->type = this->re->type;
	this->string_ID = Reaction::getStringID(this->stoichSpecies,this->re->toString());
}
