/*
 * midpoint_FG.cpp
 *
 *  Created on: Feb 18, 2011
 *      Author: Leonard Harris
 */

#include "midpoint.hh"
#include "../../util/util.hh"
/*
Midpoint_FG::Midpoint_FG() : round(true), sp(), rxn(){
	if (MoMMA::debug)
		cout << "Midpoint_FG constructor called." << endl;
}
*/
Midpoint_FG::Midpoint_FG(vector<SimpleSpecies*>& sp, vector<Reaction*>& rxn) : round(true), sp(sp), rxn(rxn){
	if (debug)
		cout << "Midpoint_FG constructor called." << endl;
	// Round species populations (default)
	for (unsigned int i=0;i < this->sp.size();i++){
		this->sp[i]->population = floor(this->sp[i]->population + 0.5);
	}
	//
	this->m_rxn.resize(this->rxn.size());
	this->m_sp.resize(this->sp.size());
	this->a_mid.resize(this->rxn.size());
	//
	// Species that contribute to rxn rates
	this->rateSp.resize(this->rxn.size());
	for (unsigned int v=0;v < this->rxn.size();v++){
		this->rateSp[v].resize(this->rxn[v]->rateSpecies.size());
		for (unsigned int i=0;i < this->rxn[v]->rateSpecies.size();i++){
			bool found = false;
			for (unsigned int j=0;!found;j++){
//			for (unsigned int j=0;j < this->sp.size() && !found;j++){
				if (this->rxn[v]->rateSpecies[i] == this->sp.at(j)){ // at() checks for out-of-boundedness
					this->rateSp[v][i] = j;
					found = true;
				}
			}
		}
	}
	//
	// Rxns that species participate in
	this->spInRxn.resize(this->sp.size());
	this->stoich.resize(this->sp.size());
	map<SimpleSpecies*,int>::iterator it;
	for (unsigned int j=0;j < this->sp.size();j++){
		for (unsigned int v=0;v < this->rxn.size();v++){
			it = this->rxn[v]->stoichSpecies.find(this->sp[j]);
			if (it != this->rxn[v]->stoichSpecies.end()){ // Species j involved in Reaction v
				this->spInRxn[j].push_back(v);
				this->stoich[j].push_back(static_cast<double>((*it).second));
			}
		}
	}
}

Midpoint_FG::Midpoint_FG(vector<SimpleSpecies*>& sp, vector<Reaction*>& rxn, bool round) : round(round), sp(sp), rxn(rxn){
	if (debug)
		cout << "Midpoint_FG constructor called." << endl;
	// Round species populations if round = true
	if (round){
		for (unsigned int i=0;i < this->sp.size();i++){
			this->sp[i]->population = floor(this->sp[i]->population + 0.5);
		}
	}
	//
	this->m_rxn.resize(this->rxn.size());
	this->m_sp.resize(this->sp.size());
	this->a_mid.resize(this->rxn.size());
	//
	// Species that contribute to rxn rates
	this->rateSp.resize(this->rxn.size());
	for (unsigned int v=0;v < this->rxn.size();v++){
		this->rateSp[v].resize(this->rxn[v]->rateSpecies.size());
		for (unsigned int i=0;i < this->rxn[v]->rateSpecies.size();i++){
			bool found = false;
			for (unsigned int j=0;!found;j++){
//			for (unsigned int j=0;j < this->sp.size() && !found;j++){
				if (this->rxn[v]->rateSpecies[i] == this->sp.at(j)){ // at() checks for out-of-boundedness
					this->rateSp[v][i] = j;
					found = true;
				}
			}
		}
	}
	//
	// Rxns that species participate in
	this->spInRxn.resize(this->sp.size());
	this->stoich.resize(this->sp.size());
	map<SimpleSpecies*,int>::iterator it;
	for (unsigned int j=0;j < this->sp.size();j++){
		for (unsigned int v=0;v < this->rxn.size();v++){
			it = this->rxn[v]->stoichSpecies.find(this->sp[j]);
			if (it != this->rxn[v]->stoichSpecies.end()){ // Species j involved in Reaction v
				this->spInRxn[j].push_back(v);
				this->stoich[j].push_back(static_cast<double>((*it).second));
			}
		}
	}
}

Midpoint_FG::Midpoint_FG(const Midpoint_FG& fg) : round(fg.round), m_rxn(fg.m_rxn), m_sp(fg.m_sp), a_mid(fg.a_mid),
	rateSp(fg.rateSp), spInRxn(fg.spInRxn), stoich(fg.stoich), sp(fg.sp), rxn(fg.rxn){
	if (debug)
		cout << "Midpoint_FG copy constructor called." << endl;
}

Midpoint_FG::~Midpoint_FG(){
	if (debug)
		cout << "Midpoint_FG destructor called." << endl;
}

double Midpoint_FG::fireRxn(unsigned int u, int classif, double tau){

	double a_tau = this->a_mid[u]*tau; // Estimated midpoint propensity * tau
	double k = 0.0;
	if (classif == RxnClassifier::POISSON){
		k = Util::RANDOM_POISSON(a_tau);
	}
	else if (classif == RxnClassifier::LANGEVIN){
		k = a_tau + sqrt(a_tau)*Util::RANDOM_GAUSSIAN();
		if (k < 0.0){ // Just to be safe
			k = 0.0;
		}
		else if (this->round){
			k = floor(k + 0.5);
		}
	}
	else if (classif == RxnClassifier::DETERMINISTIC){
		k = a_tau;
		if (this->round){
			k = floor(k + 0.5);
		}
	}
	else{
		cout << "Error in Midpoint_FG::fireRxn(): Reaction classification for " << this->rxn[u]->toString()
			 << " (" << classif << ") not recognized." << endl;
		cout << "Only Poisson, Langevin and Deterministic classifications are supported. Exiting." << endl;
		exit(1);
	}
//	cout << "k = " << k << endl;
	this->rxn[u]->fire(k);
	return k;
}

void Midpoint_FG::fireRxns(vector<double>& k, vector<int>& classif, double tau){
	// Error check
	if (classif.size() != this->rxn.size()){
		cout << "Error in Midpoint_FG::fireRxns(): 'classif' vector must be same size as 'rxn' vector. Exiting.\n";
		exit(1);
	}
	if (k.size() != this->rxn.size()){
		cout << "Error in Midpoint_FG::fireRxns(): 'k' vector must be same size as 'rxn' vector. Exiting.\n";
		exit(1);
	}
	//
	this->calc_aMid(tau,true); // TRUE: Rxn-based; FALSE: Species-based
	//
	for (unsigned int v=0;v < this->rxn.size();v++){
		if (classif[v] != RxnClassifier::EXACT_STOCHASTIC){
			k[v] = this->fireRxn(v,classif[v],tau);
		}
		else{
			k[v] = 0.0;
		}
	}
}

void Midpoint_FG::calc_aMid(double tau, bool rxnBased){

	if (rxnBased){
		// METHOD 1: a_mid[u] = a[u] + tau/2*m[u]
		this->calc_mRxn();
		for (unsigned int v=0;v < this->rxn.size();v++){
			this->a_mid[v] = this->rxn[v]->getRate() + 0.5*tau*this->m_rxn[v];
		}
	}
	else{
		// METHOD 2: X_mid[j] = X[j] + tau/2*mm[j]
		//			 a_mid[u] = rxn[u]->re->getRate(X_mid)
		this->calc_mSp();
		double X_mid[this->sp.size()];
		for (unsigned int j=0;j < this->sp.size();j++){
			X_mid[j] = this->sp[j]->population + 0.5*tau*this->m_sp[j];
		}
		// Calculate midpoint rates
		vector<double> X;
		for (unsigned int v=0;v < this->rxn.size();v++){
			X.resize(this->rxn[v]->rateSpecies.size());
			for (unsigned int j=0;j < this->rxn[v]->rateSpecies.size();j++){
				X[j] = X_mid[this->rateSp[v][j]];
			}
			this->a_mid[v] = this->rxn[v]->re->getRate(X);
		}
	}
}

void Midpoint_FG::calc_mRxn(){

	// Collect rates for all reactions at the outset
	double rate[this->rxn.size()];
	for (unsigned int v=0;v < this->rxn.size();v++){
		rate[v] = this->rxn[v]->getRate();
	}

	// Loop over reactions
	for (unsigned int u=0;u < this->rxn.size();u++){

		// Calculate derivatives dau/dXj
		vector<double> dau_dX;
		for (unsigned int j=0;j < this->rxn[u]->rateSpecies.size();j++){
			dau_dX.push_back(this->rxn[u]->get_dRate_dX(j));
		}

		// Calculate elements of f_u[v]
		double f_u[this->rxn.size()];
		for (unsigned int v=0;v < this->rxn.size();v++){
			f_u[v] = 0.0;
			SimpleSpecies* rateSp;
			map<SimpleSpecies*,int>::iterator stoichSp;
			// Loop over reactant species in reaction R_u
			for (unsigned int j=0;j < this->rxn[u]->rateSpecies.size();j++){
				rateSp = this->rxn[u]->rateSpecies[j];
				stoichSp = this->rxn[v]->stoichSpecies.find(rateSp);
				if (stoichSp != this->rxn[v]->stoichSpecies.end()){ // Species j is created/consumed in reaction R_v
					f_u[v] += dau_dX[j] * static_cast<double>((*stoichSp).second);
				}
			}
		}

		// Calculate m_u
		this->m_rxn[u] = 0.0;
		for (unsigned int v=0;v < this->rxn.size();v++){
			this->m_rxn[u] += f_u[v]*rate[v];
		}
	}
}

void Midpoint_FG::calc_mSp(){

	// Collect initial rates
	double rate[this->rxn.size()];
	for (unsigned int v=0;v < this->rxn.size();v++){
		rate[v] = this->rxn[v]->getRate();
	}
	// Midpoint populations
	for (unsigned int j=0;j < this->sp.size();j++){
		this->m_sp[j] = 0.0;
		for (unsigned int v=0;v < this->spInRxn[j].size();v++){
			this->m_sp[j] += this->stoich[j][v]*rate[this->spInRxn[j][v]];
		}
	}
}
