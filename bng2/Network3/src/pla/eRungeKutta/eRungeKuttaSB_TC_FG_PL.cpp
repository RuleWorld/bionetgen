/*
 * eRungeKuttaSB_TC_FG_PL.cpp
 *
 *  Created on: May 10, 2011
 *      Author: Leonard Harris
 */

#include "eRungeKutta.hh"

eRungeKuttaSB_TC_FG_PL::eRungeKuttaSB_TC_FG_PL(ButcherTableau bt, double eps, double p, double pp, double q, double w,
		vector<SimpleSpecies*>& sp, vector<Reaction*>& rxn, Preleap_TC& ptc): eRungeKutta_FG(bt,sp,rxn),
		BinomialCorrector_PL(p,rxn), pp(pp), q(q), w(w), preCalc(true), ptc(ptc), sp(sp){
	if (debug)
		cout << "eRungeKuttaSB_TC_FG_PL constructor called." << endl;
	// Error check
	if (this->pp < this->p){
		cout << "Error in eRungeKuttaSB_TC_FG_PL constructor: ";
		cout << "pp must be >= p; you have pp = " << this->pp << ", p = " << this->p << endl;
		exit(1);
	}
	if (this->q < 1.0){
		cout << "Error in eRungeKuttaSB_TC_FG_PL constructor: ";
		cout << "q must be >= 1.0; your q = " << this->q << endl;
		exit(1);
	}
	if (this->w <= 0.0 || this->w >= 1.0){
		cout << "Error in eRungeKuttaSB_TC_FG_PL constructor: ";
		cout << "w must be > 0.0 and < 1.0; your w = " << this->w << endl;
		exit(1);
	}
	this->pl = new eRKsb_PL(eps,this->sp);
	this->gGet = new g_Getter(this->sp,rxn);
	// Add species
	for (unsigned int j=0;j < this->sp.size();j++){
		this->addSpecies();
	}
}

eRungeKuttaSB_TC_FG_PL::eRungeKuttaSB_TC_FG_PL(ButcherTableau bt, double eps, double p, double pp, double q, double w,
		vector<SimpleSpecies*>& sp, vector<Reaction*>& rxn, Preleap_TC& ptc, bool round): eRungeKutta_FG(bt,sp,rxn,round),
		BinomialCorrector_PL(p,rxn), pp(pp), q(q), w(w), preCalc(true), ptc(ptc), sp(sp){
	if (debug)
		cout << "eRungeKuttaSB_TC_FG_PL constructor called." << endl;
	// Error check
	if (this->pp < this->p){
		cout << "Error in eRungeKuttaSB_TC_FG_PL constructor: ";
		cout << "pp must be >= p; you have pp = " << this->pp << ", p = " << this->p << endl;
		exit(1);
	}
	if (this->q < 1.0){
		cout << "Error in eRungeKuttaSB_TC_FG_PL constructor: ";
		cout << "q must be >= 1.0; your q = " << this->q << endl;
		exit(1);
	}
	if (this->w <= 0.0 || this->w >= 1.0){
		cout << "Error in eRungeKuttaSB_TC_FG_PL constructor: ";
		cout << "w must be > 0.0 and < 1.0; your w = " << this->w << endl;
		exit(1);
	}
	this->pl = new eRKsb_PL(eps,this->sp);
	this->gGet = new g_Getter(this->sp,rxn);
	// Add species
	for (unsigned int j=0;j < this->sp.size();j++){
		this->addSpecies();
	}
}

eRungeKuttaSB_TC_FG_PL::eRungeKuttaSB_TC_FG_PL(const eRungeKuttaSB_TC_FG_PL& tc_fg_pl): eRungeKutta_FG(tc_fg_pl),
		BinomialCorrector_PL(tc_fg_pl), pp(tc_fg_pl.pp), q(tc_fg_pl.q), w(tc_fg_pl.w), preCalc(true), ptc(tc_fg_pl.ptc),
		sp(tc_fg_pl.sp){
	if (debug)
		cout << "eRungeKuttaSB_TC_FG_PL copy constructor called." << endl;
	this->pl = new eRKsb_PL(*tc_fg_pl.pl);
	this->gGet = new g_Getter(*tc_fg_pl.gGet);
	// Add species
	for (unsigned int j=0;j < this->sp.size();j++){
		this->addSpecies();
	}
}

eRungeKuttaSB_TC_FG_PL::~eRungeKuttaSB_TC_FG_PL(){
	if (debug)
		cout << "eRungeKuttaSB_TC_FG_PL destructor called." << endl;
	delete this->pl;
	delete this->gGet;
}

void eRungeKuttaSB_TC_FG_PL::getNewTau(double& tau){
	// Check for new species
	while (this->oldPop.size() != this->sp.size() && this->old_g.size() != this->sp.size()
			&& this->projPop.size() != this->sp.size()){
		this->addSpecies();
	}
	// Get new tau
	if (this->preCalc){
		this->ptc.getNewTau(tau);
		this->preCalc = false;
	}
	else{
		if (this->substantially){ // Step was substantially accepted, increase tau
			tau *= this->q;
		}
		else{ // Step was barely accepted, reduce tau by a little bit
			tau *= this->pp;
		}
	}
	// Perform pre-check
	bool ok = false;
	while (!ok){
		//
		// Calculate a_eff[]
		this->aCalc->calc_aEff(tau);
		double mean_dX[this->aCalc->X_eff.size()];
		double sdev_dX[this->aCalc->X_eff.size()];
		//
		// Calculate projected species population changes
		for (unsigned int j=0;j < this->aCalc->X_eff.size();j++){
			mean_dX[j] = 0.0;
			sdev_dX[j] = 0.0;
			double z_vj;
			unsigned int R_v;
			for (unsigned int v=0;v < this->aCalc->spInRxn[j].size();v++){
				z_vj = this->aCalc->stoich[j][v];
				R_v = this->aCalc->spInRxn[j][v];
				mean_dX[j] += z_vj*this->aCalc->a_eff[R_v];
				sdev_dX[j] += z_vj*z_vj*this->aCalc->a_eff[R_v];
			}
			mean_dX[j] *= tau;
			sdev_dX[j] *= tau;
			sdev_dX[j] = sqrt(sdev_dX[j]);
			if (mean_dX[j] < 0.0){ // If the mean is negative, make the sdev negative
				sdev_dX[j] = -sdev_dX[j];
			}
		}
		// Calculate elements of projPop[]
		for (unsigned int j=0;j < this->projPop.size();j++){
			this->projPop[j] = this->oldPop[j] + mean_dX[j] + sdev_dX[j];

		}
		// Check against current rates
		vector<double>* curr_g = &this->old_g; // Since we haven't leapt yet, old_g[] is actually curr_g[]
		ok = this->pl->check(1.0,this->aCalc->X_eff,this->projPop,*curr_g,false);
		if (!ok){
			tau *= this->p; // Reduce
		}
	}
}

void eRungeKuttaSB_TC_FG_PL::fireRxns(vector<double>& k, vector<int>& classif, double tau){
	// a_eff[] elements have already been calculated in getNewTau()
	this->fg->fireRxns(k,classif,tau,this->aCalc->a_eff);
}

bool eRungeKuttaSB_TC_FG_PL::check(){
	// Check for new species
	while (this->oldPop.size() != this->sp.size() && this->old_g.size() != this->sp.size()
			&& this->projPop.size() != this->sp.size()){
		this->addSpecies();
	}
//	cout << "**Checking**" << endl;
	bool ok;
	this->substantially = this->pl->check(this->w,this->aCalc->X_eff,this->oldPop,this->old_g,true);
	if (this->substantially){
		ok = true;
//		cout << "**Substantially accepted**" << endl;
	}
	else{
		ok = this->pl->check(1.0,this->aCalc->X_eff,this->oldPop,this->old_g,true);
//		if (ok) cout << "**Barely accepted**" << endl;
//		else cout << "**Rejected**" << endl;
	}
	return ok;
}

void eRungeKuttaSB_TC_FG_PL::update(){
	// Update oldPop[] and old_g[]
	for (unsigned int j=0;j < this->oldPop.size();j++){
		this->oldPop[j] = this->sp[j]->population;
		this->old_g[j] = this->gGet->get_g(j);
	}
	// Just in case
	if (this->oldPop.size() != this->sp.size()){
		cout << "Error in eRungeKuttaSB_TC_FG_PL::update(): Sizes of 'oldPop' and 'sp' vectors not equal. Shouldn't happen. "
			 << "Exiting." << endl;
		exit(1);
	}
	if (this->old_g.size() != this->sp.size()){
		cout << "Error in eRungeKuttaSB_TC_FG_PL::update(): Sizes of 'old_g' and 'sp' vectors not equal. Shouldn't happen. "
			 << "Exiting." << endl;
		exit(1);
	}
	if (this->projPop.size() != this->sp.size()){
		cout << "Error in eRungeKuttaSB_TC_FG_PL::update(): Sizes of 'projPop' and 'sp' vectors not equal. Shouldn't happen. "
			 << "Exiting." << endl;
		exit(1);
	}
}

void eRungeKuttaSB_TC_FG_PL::addSpecies(){
	if (this->oldPop.size() < this->sp.size() && this->oldPop.size() == this->old_g.size()
			&& this->oldPop.size() == this->projPop.size()){
		unsigned int i = this->oldPop.size();
		this->oldPop.push_back(this->sp[i]->population);
		this->old_g.push_back(this->gGet->get_g(i));
		this->projPop.push_back(0.0);
	}
	else{
		cout << "Error in eRungeKuttaSB_TC_FG_PL::addSpecies(): No species to add (oldPop.size = " << this->oldPop.size()
			 << ", old_g.size = " << this->old_g.size() << ", projPop.size = " << this->projPop.size() << ", sp.size = "
			 << this->sp.size() << "). Shouldn't happen. Exiting." << endl;
		exit(1);
	}
}
