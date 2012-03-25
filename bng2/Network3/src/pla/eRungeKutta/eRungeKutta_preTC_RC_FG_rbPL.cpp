/*
 * eRungeKutta_preTC_RC_FG_rbPL.cpp
 *
 *  Created on: Mar 19, 2012
 *      Author: Leonard Harris
 */

#include "eRungeKutta.hh"
/*
eRungeKutta_preTC_RC_FG_rbPL::eRungeKutta_preTC_RC_FG_rbPL() : rxn(){
	if (MoMMA::debug)
		cout << "eRungeKutta_preTC_RC_FG_rbPL constructor called." << endl;
}
*/
eRungeKutta_preTC_RC_FG_rbPL::eRungeKutta_preTC_RC_FG_rbPL(ButcherTableau bt, double eps, double approx1, double gg1,
		double p, Preleap_TC* ptc, vector<SimpleSpecies*>& sp, vector<Reaction*>& rxn)
		: eRungeKutta_TC_RC_FG_rbPL(bt,eps,approx1,gg1,p,ptc,sp,rxn){
	if (debug)
		cout << "eRungeKutta_preTC_RC_FG_rbPL constructor called." << endl;
}

eRungeKutta_preTC_RC_FG_rbPL::eRungeKutta_preTC_RC_FG_rbPL(ButcherTableau bt, double eps, double approx1, double gg1,
		double p, Preleap_TC* ptc, vector<SimpleSpecies*>& sp, vector<Reaction*>& rxn, bool round)
		: eRungeKutta_TC_RC_FG_rbPL(bt,eps,approx1,gg1,p,ptc,sp,rxn,round){
	if (debug)
		cout << "eRungeKutta_preTC_RC_FG_rbPL constructor called." << endl;
}

eRungeKutta_preTC_RC_FG_rbPL::eRungeKutta_preTC_RC_FG_rbPL(const eRungeKutta_preTC_RC_FG_rbPL& tc_rc_fg_pl)
		: eRungeKutta_TC_RC_FG_rbPL(tc_rc_fg_pl){
	if (debug)
		cout << "eRungeKutta_preTC_RC_FG_rbPL copy constructor called." << endl;
}

eRungeKutta_preTC_RC_FG_rbPL::~eRungeKutta_preTC_RC_FG_rbPL(){
	if (debug)
		cout << "eRungeKutta_preTC_RC_FG_rbPL destructor called." << endl;
}

void eRungeKutta_preTC_RC_FG_rbPL::getNewTau(double& tau){

	// Check for new rxns
	while (this->oldPop.size() != this->rxn.size() || this->projPop.size() != this->rxn.size()){
		this->addRxn();
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
		// Calculate elements of projPop[][]
		unsigned int S_j;
		for (unsigned int v=0;v < this->projPop.size();v++){
			for (unsigned int j=0;j < this->rxn[v]->rateSpecies.size();j++){
				S_j = this->aCalc->rateSp.at(v)[j];
				this->projPop[v][j] = this->oldPop[v][j] + delta_X[S_j];
			}
		}
*/
		// Check against current rates
		if (!(ok = this->ch->check(1.0,this->aCalc->a_eff,this->projPop,false))){
			tau *= this->p; // Reduce
		}
	}
}
/*
bool eRungeKutta_preTC_RC_FG_rbPL::check(){
	return this->ch->check(1.0,this->aCalc->a_eff,this->oldPop,true);
}
*/
/*
void eRungeKutta_preTC_RC_FG_rbPL::update(){
	// Update aCalc
	this->aCalc->update();
	// Update oldPop[][]
	for (unsigned int v=0;v < this->oldPop.size();v++){
		for (unsigned int j=0;j < this->rxn[v]->rateSpecies.size();j++){
			this->oldPop[v][j] = this->rxn[v]->rateSpecies[j]->population;
		}
	}
	// Error check
	if (this->oldPop.size() != this->rxn.size()){
		cout << "Error in eRungeKutta_preTC_RC_FG_rbPL::update(): Sizes of 'oldPop' and 'rxn' vectors not equal. "
			 << "Shouldn't happen. Exiting." << endl;
		exit(1);
	}
}

void eRungeKutta_preTC_RC_FG_rbPL::addRxn(){
	// Add to oldPop[]
	if (this->oldPop.size() < this->rxn.size()){
		unsigned int u = this->oldPop.size();
		this->oldPop.push_back(new double[this->rxn[u]->rateSpecies.size()]);
		for (unsigned int j=0;j < this->rxn[u]->rateSpecies.size();j++){
			this->oldPop[u][j] = this->rxn[u]->rateSpecies[j]->population;
		}
	}
	else{ // Throw error if oldPop.size >= rxn.size
		cout << "Error in eRungeKutta_preTC_RC_FG_rbPL::addRxn(): No rxns to add (oldPop.size = "
			 << this->oldPop.size() << ", rxn.size = " << this->rxn.size() << ")." << endl;
		cout << "Shouldn't happen. Exiting." << endl;
		exit(1);
	}
}
*/
