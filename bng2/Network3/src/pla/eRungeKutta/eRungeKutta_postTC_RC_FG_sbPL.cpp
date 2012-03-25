/*
 * eRungeKutta_postTC_RC_FG_sbPL.cpp
 *
 *  Created on: May 19, 2011
 *      Author: Leonard Harris
 */

#include "eRungeKutta.hh"

eRungeKutta_postTC_RC_FG_sbPL::eRungeKutta_postTC_RC_FG_sbPL(ButcherTableau bt, double eps, double approx1, double gg1,
		double p, double pp, double q, double w, Preleap_TC* ptc, vector<SimpleSpecies*>& sp, vector<Reaction*>& rxn)
		: eRungeKutta_TC_RC_FG_sbPL(bt,eps,approx1,gg1,p,ptc,sp,rxn), preCalc(true), pp(pp), q(q), w(w){
	if (debug)
		cout << "eRungeKutta_postTC_RC_FG_sbPL constructor called." << endl;
	// Error check
	if (this->pp < this->p){
		cout << "Error in eRungeKutta_postTC_RC_FG_sbPL constructor: ";
		cout << "pp must be >= p; you have pp = " << this->pp << ", p = " << this->p << endl;
		exit(1);
	}
	if (this->q < 1.0){
		cout << "Error in eRungeKutta_postTC_RC_FG_sbPL constructor: ";
		cout << "q must be >= 1.0; your q = " << this->q << endl;
		exit(1);
	}
	if (this->w <= 0.0 || this->w >= 1.0){
		cout << "Error in eRungeKutta_postTC_RC_FG_sbPL constructor: ";
		cout << "w must be > 0.0 and < 1.0; your w = " << this->w << endl;
		exit(1);
	}
}

eRungeKutta_postTC_RC_FG_sbPL::eRungeKutta_postTC_RC_FG_sbPL(ButcherTableau bt, double eps, double approx1, double gg1, double p,
		double pp, double q, double w, Preleap_TC* ptc, vector<SimpleSpecies*>& sp, vector<Reaction*>& rxn, bool round)
		: eRungeKutta_TC_RC_FG_sbPL(bt,eps,approx1,gg1,p,ptc,sp,rxn,round), preCalc(true), pp(pp), q(q), w(w){
	if (debug)
		cout << "eRungeKutta_postTC_RC_FG_sbPL constructor called." << endl;
	// Error check
	if (this->pp < this->p){
		cout << "Error in eRungeKutta_postTC_RC_FG_sbPL constructor: ";
		cout << "pp must be >= p; you have pp = " << this->pp << ", p = " << this->p << endl;
		exit(1);
	}
	if (this->q < 1.0){
		cout << "Error in eRungeKutta_postTC_RC_FG_sbPL constructor: ";
		cout << "q must be >= 1.0; your q = " << this->q << endl;
		exit(1);
	}
	if (this->w <= 0.0 || this->w >= 1.0){
		cout << "Error in eRungeKutta_postTC_RC_FG_sbPL constructor: ";
		cout << "w must be > 0.0 and < 1.0; your w = " << this->w << endl;
		exit(1);
	}
}

eRungeKutta_postTC_RC_FG_sbPL::eRungeKutta_postTC_RC_FG_sbPL(const eRungeKutta_postTC_RC_FG_sbPL& tc_rc_fg_pl)
		: eRungeKutta_TC_RC_FG_sbPL(tc_rc_fg_pl), preCalc(true), pp(tc_rc_fg_pl.pp), q(tc_rc_fg_pl.q), w(tc_rc_fg_pl.w){
	if (debug)
		cout << "eRungeKutta_postTC_RC_FG_sbPL copy constructor called." << endl;
}

eRungeKutta_postTC_RC_FG_sbPL::~eRungeKutta_postTC_RC_FG_sbPL(){
	if (debug)
		cout << "eRungeKutta_postTC_RC_FG_sbPL destructor called." << endl;
}

void eRungeKutta_postTC_RC_FG_sbPL::getNewTau(double& tau){

	// Check for new species
	while (this->oldPop.size() != this->sp.size() || this->old_g.size() != this->sp.size()
			|| this->projPop.size() != this->sp.size()){
		this->addSpecies();
	}

	// Get new tau
	if (this->preCalc){
		this->ptc->getNewTau(tau);
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

	// Perform preleap check
	bool ok = false;
	while (!ok){

		// Calculate a_eff[]
		this->aCalc->calc_aEff(tau);

		// Calculate projected species population changes
		this->calc_projPop(tau);
/*		double delta_X[this->aCalc->X_eff.size()];
		for (unsigned int j=0;j < this->aCalc->X_eff.size();j++){
			delta_X[j] = 0.0;
			double z_vj;
			unsigned int R_v;
			for (unsigned int v=0;v < this->aCalc->spInRxn[j].size();v++){
				z_vj = this->aCalc->stoich.at(j).at(v);
				R_v = this->aCalc->spInRxn.at(j).at(v);
				delta_X[j] += z_vj*this->aCalc->a_eff.at(R_v);
			}
			delta_X[j] *= tau;
		}
		// Calculate elements of projPop[]
		for (unsigned int j=0;j < this->projPop.size();j++){
			this->projPop[j] = this->oldPop[j] + delta_X[j];
		}
*/
		// Check against current rates
		vector<double>* curr_g = &this->old_g; // Since we haven't leapt yet, old_g[] is actually curr_g[]
		if (!(ok = this->ch->check(1.0,this->aCalc->X_eff,this->projPop,*curr_g,false))){
			tau *= this->p; // Reduce
		}
	}
}

bool eRungeKutta_postTC_RC_FG_sbPL::check(){
//	cout << "**Checking**" << endl;
	bool ok;
	this->substantially = this->ch->check(this->w,this->aCalc->X_eff,this->oldPop,this->old_g,true);
	if (this->substantially){
		ok = true;
//		cout << "**Substantially accepted**" << endl;
	}
	else{
		ok = this->ch->check(1.0,this->aCalc->X_eff,this->oldPop,this->old_g,true);
//		if (ok) cout << "**Barely accepted**" << endl;
//		else cout << "**Rejected**" << endl;
	}
	return ok;
}
/*
void eRungeKutta_postTC_RC_FG_sbPL::update(){
	// Update aCalc
	this->aCalc->update();
	// Update oldPop[] and old_g[]
	for (unsigned int j=0;j < this->oldPop.size();j++){
		this->oldPop[j] = this->sp[j]->population;
		this->old_g[j] = this->gGet->get_g(j);
	}
	// Just in case
	if (this->oldPop.size() != this->sp.size()){
		cout << "Error in eRungeKutta_postTC_RC_FG_sbPL::update(): Sizes of 'oldPop' and 'sp' vectors not equal. "
			 << "Shouldn't happen. Exiting." << endl;
		exit(1);
	}
	if (this->old_g.size() != this->sp.size()){
		cout << "Error in eRungeKutta_postTC_RC_FG_sbPL::update(): Sizes of 'old_g' and 'sp' vectors not equal. "
			 << "Shouldn't happen. Exiting." << endl;
		exit(1);
	}
	if (this->projPop.size() != this->sp.size()){
		cout << "Error in eRungeKutta_postTC_RC_FG_sbPL::update(): Sizes of 'projPop' and 'sp' vectors not equal. "
			 << "Shouldn't happen. Exiting." << endl;
		exit(1);
	}
}

void eRungeKutta_postTC_RC_FG_sbPL::addSpecies(){
	// Add to oldPop[] and old_g[]
	if (this->oldPop.size() < this->sp.size() && this->oldPop.size() == this->old_g.size()){
		unsigned int i = this->oldPop.size();
		this->oldPop.push_back(this->sp[i]->population);
		this->old_g.push_back(this->gGet->get_g(i));;
	}
	else if (this->oldPop.size() > this->sp.size()){ // Throw error if oldPop.size > sp.size
		cout << "Error in eRungeKutta_postTC_RC_FG_sbPL::addSpecies(): oldPop.size ("
			 << this->oldPop.size() << ") > sp.size (" << this->sp.size() << ")." << endl;
		cout << "Shouldn't happen. Exiting." << endl;
		exit(1);
	}
	else if (this->old_g.size() > this->sp.size()){ // Throw error if old_g.size > sp.size
		cout << "Error in eRungeKutta_postTC_RC_FG_sbPL::addSpecies(): old_g.size ("
			 << this->old_g.size() << ") > sp.size (" << this->sp.size() << ")." << endl;
		cout << "Shouldn't happen. Exiting." << endl;
		exit(1);
	}
	// Add to projPop[]
	if (this->projPop.size() < this->sp.size()){
		this->projPop.push_back(0.0);
	}
	else{ // Throw error if projPop.size >= sp.size
		cout << "Error in eRungeKutta_postTC_RC_FG_sbPL::addSpecies(): No species to add (projPop.size = "
			 << this->projPop.size() << ", sp.size = " << this->sp.size() << ")." << endl;
		cout << "Shouldn't happen. Exiting." << endl;
		exit(1);
	}
}
*/
