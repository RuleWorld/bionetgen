/*
 * eRungeKutta_preTC_RC_FG_sbPL.cpp
 *
 *  Created on: Mar 19, 2012
 *      Author: Leonard Harris
 */

#include "eRungeKutta.hh"
/*
eRungeKutta_preTC_RC_FG_sbPL::eRungeKutta_preTC_RC_FG_sbPL() : rxn(){
	if (MoMMA::debug)
		cout << "eRungeKutta_preTC_RC_FG_sbPL constructor called." << endl;
}
*/
eRungeKutta_preTC_RC_FG_sbPL::eRungeKutta_preTC_RC_FG_sbPL(ButcherTableau bt, double eps, double approx1, double gg1,
		double p, Preleap_TC* ptc, vector<SimpleSpecies*>& sp, vector<Reaction*>& rxn)
		: eRungeKutta_TC_RC_FG_sbPL(bt,eps,approx1,gg1,p,ptc,sp,rxn){
	if (debug)
		cout << "eRungeKutta_preTC_RC_FG_sbPL constructor called." << endl;
}

eRungeKutta_preTC_RC_FG_sbPL::eRungeKutta_preTC_RC_FG_sbPL(ButcherTableau bt, double eps, double approx1, double gg1,
		double p, Preleap_TC* ptc, vector<SimpleSpecies*>& sp, vector<Reaction*>& rxn, bool round)
		: eRungeKutta_TC_RC_FG_sbPL(bt,eps,approx1,gg1,p,ptc,sp,rxn,round){
	if (debug)
		cout << "eRungeKutta_preTC_RC_FG_sbPL constructor called." << endl;
}

eRungeKutta_preTC_RC_FG_sbPL::eRungeKutta_preTC_RC_FG_sbPL(const eRungeKutta_preTC_RC_FG_sbPL& tc_rc_fg_pl)
		: eRungeKutta_TC_RC_FG_sbPL(tc_rc_fg_pl){
	if (debug)
		cout << "eRungeKutta_preTC_RC_FG_sbPL copy constructor called." << endl;
}

eRungeKutta_preTC_RC_FG_sbPL::~eRungeKutta_preTC_RC_FG_sbPL(){
	if (debug)
		cout << "eRungeKutta_preTC_RC_FG_sbPL destructor called." << endl;
}

void eRungeKutta_preTC_RC_FG_sbPL::getNewTau(double& tau){

	// Check for new species
	while (this->oldPop.size() != this->sp.size() || this->old_g.size() != this->sp.size()
			|| this->projPop.size() != this->sp.size()){
		this->addSpecies();
	}

	// Get new tau
	this->ptc->getNewTau(tau);

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
/*
bool eRungeKutta_preTC_RC_FG_sbPL::check(){
	return this->ch->check(1.0,this->aCalc->X_eff,this->oldPop,this->old_g,true);
}
*/
/*
void eRungeKutta_preTC_RC_FG_sbPL::update(){
	// Update aCalc
	this->aCalc->update();
	// Update oldPop[] and old_g[]
	for (unsigned int j=0;j < this->oldPop.size();j++){
		this->oldPop[j] = this->sp[j]->population;
		this->old_g[j] = this->gGet->get_g(j);
	}
	// Just in case
	if (this->oldPop.size() != this->sp.size()){
		cout << "Error in eRungeKutta_preTC_RC_FG_sbPL::update(): Sizes of 'oldPop' and 'sp' vectors not equal. "
			 << "Shouldn't happen. Exiting." << endl;
		exit(1);
	}
	if (this->old_g.size() != this->sp.size()){
		cout << "Error in eRungeKutta_preTC_RC_FG_sbPL::update(): Sizes of 'old_g' and 'sp' vectors not equal. "
			 << "Shouldn't happen. Exiting." << endl;
		exit(1);
	}
}

void eRungeKutta_preTC_RC_FG_sbPL::addSpecies(){
	// Add to oldPop[] and old_g[]
	if (this->oldPop.size() < this->sp.size() && this->oldPop.size() == this->old_g.size()){
		unsigned int i = this->oldPop.size();
		this->oldPop.push_back(this->sp[i]->population);
		this->old_g.push_back(this->gGet->get_g(i));;
	}
	else{ // Throw error if oldPop.size || old_g.size >= sp.size
		cout << "Error in eRungeKutta_preTC_RC_FG_sbPL::addSpecies(): No species to add (oldPop.size = "
			 << this->oldPop.size() << ", old_g.size = " << this->old_g.size() << ", sp.size = "
			 << this->sp.size() << ")." << endl;
		cout << "Shouldn't happen. Exiting." << endl;
		exit(1);
	}
}
*/
