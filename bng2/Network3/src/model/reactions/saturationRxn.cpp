/*
 * saturationRxn.cpp
 *
 *  Created on: Jul 10, 2011
 *      Author: Leonard Harris
 */

#include "bioNetGenRxn.hh"
/*
SaturationRxn::SaturationRxn(){
	if (MoMMA::debug)
		cout << "SaturationRxn constructor called." << endl;
}
*/
SaturationRxn::SaturationRxn(double kcat, double Km, vector<SimpleSpecies*> r, vector<int> rS, vector<SimpleSpecies*> p, vector<int> pS)
	: BioNetGenRxn(r,rS,p,pS){
	this->construct(kcat,vector<double>(1,Km));
}

SaturationRxn::SaturationRxn(double kcat, double Km, SimpleSpecies* r, int rS, vector<SimpleSpecies*> p, vector<int> pS)
	: BioNetGenRxn(vector<SimpleSpecies*>(1,r),vector<int>(1,rS),p,pS){
	this->construct(kcat,vector<double>(1,Km));
}

SaturationRxn::SaturationRxn(double kcat, double Km, vector<SimpleSpecies*> r, vector<int> rS, SimpleSpecies* p, int pS)
	: BioNetGenRxn(r,rS,vector<SimpleSpecies*>(1,p),vector<int>(1,pS)){
	this->construct(kcat,vector<double>(1,Km));
}

SaturationRxn::SaturationRxn(double kcat, double Km, SimpleSpecies* r, int rS, SimpleSpecies* p, int pS)
	: BioNetGenRxn(vector<SimpleSpecies*>(1,r),vector<int>(1,rS),vector<SimpleSpecies*>(1,p),vector<int>(1,pS)){
	this->construct(kcat,vector<double>(1,Km));
}
//
SaturationRxn::SaturationRxn(double kcat, vector<double> Km, vector<SimpleSpecies*> r, vector<int> rS, vector<SimpleSpecies*> p, vector<int> pS)
	: BioNetGenRxn(r,rS,p,pS){
	this->construct(kcat,Km);
}

SaturationRxn::SaturationRxn(double kcat, vector<double> Km, SimpleSpecies* r, int rS, vector<SimpleSpecies*> p, vector<int> pS)
	: BioNetGenRxn(vector<SimpleSpecies*>(1,r),vector<int>(1,rS),p,pS){
	this->construct(kcat,Km);
}

SaturationRxn::SaturationRxn(double kcat, vector<double> Km, vector<SimpleSpecies*> r, vector<int> rS, SimpleSpecies* p, int pS)
	: BioNetGenRxn(r,rS,vector<SimpleSpecies*>(1,p),vector<int>(1,pS)){
	this->construct(kcat,Km);
}

SaturationRxn::SaturationRxn(double kcat, vector<double> Km, SimpleSpecies* r, int rS, SimpleSpecies* p, int pS)
	: BioNetGenRxn(vector<SimpleSpecies*>(1,r),vector<int>(1,rS),vector<SimpleSpecies*>(1,p),vector<int>(1,pS)){
	this->construct(kcat,Km);
}

SaturationRxn::~SaturationRxn(){
	if (debug)
		cout << "SaturationRxn destructor called." << endl;
	delete this->re;
}

void SaturationRxn::construct(double kcat, vector<double> Km){
	if (debug)
		cout << "SaturationRxn constructor called." << endl;
	this->re = new RateSaturation(kcat,Km,this->rateSpecies,this->rStoich); // Don't forget to delete this in the destructor
	this->type = this->re->type;
	this->string_ID = Reaction::getStringID(this->stoichSpecies,this->re->toString());

}
