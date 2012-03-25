/*
 * eRungeKutta_postTC_RC_FG_rbPL.cpp
 *
 *  Created on: May 19, 2011
 *      Author: Leonard Harris
 */

#include "eRungeKutta.hh"

eRungeKutta_postTC_RC_FG_rbPL::eRungeKutta_postTC_RC_FG_rbPL(ButcherTableau bt, double eps, double approx1, double gg1,
		double p, double pp, double q, double w, Preleap_TC* ptc, vector<SimpleSpecies*>& sp, vector<Reaction*>& rxn)
		: eRungeKutta_TC_RC_FG_rbPL(bt,eps,approx1,gg1,p,ptc,sp,rxn), preCalc(true), pp(pp), q(q), w(w){
	if (debug)
		cout << "eRungeKutta_postTC_RC_FG_rbPL constructor called." << endl;
	// Error check
	if (this->pp < this->p){
		cout << "Error in eRungeKutta_postTC_RC_FG_rbPL constructor: ";
		cout << "pp must be >= p; you have pp = " << this->pp << ", p = " << this->p << endl;
		exit(1);
	}
	if (this->q < 1.0){
		cout << "Error in eRungeKutta_postTC_RC_FG_rbPL constructor: ";
		cout << "q must be >= 1.0; your q = " << this->q << endl;
		exit(1);
	}
	if (this->w <= 0.0 || this->w >= 1.0){
		cout << "Error in eRungeKutta_postTC_RC_FG_rbPL constructor: ";
		cout << "w must be > 0.0 and < 1.0; your w = " << this->w << endl;
		exit(1);
	}
}

eRungeKutta_postTC_RC_FG_rbPL::eRungeKutta_postTC_RC_FG_rbPL(ButcherTableau bt, double eps, double approx1, double gg1,
		double p, double pp, double q, double w, Preleap_TC* ptc, vector<SimpleSpecies*>& sp, vector<Reaction*>& rxn,
		bool round) : eRungeKutta_TC_RC_FG_rbPL(bt,eps,approx1,gg1,p,ptc,sp,rxn,round), preCalc(true), pp(pp), q(q), w(w){
	if (debug)
		cout << "eRungeKutta_postTC_RC_FG_rbPL constructor called." << endl;
	// Error check
	if (this->pp < this->p){
		cout << "Error in eRungeKutta_postTC_RC_FG_rbPL constructor: ";
		cout << "pp must be >= p; you have pp = " << this->pp << ", p = " << this->p << endl;
		exit(1);
	}
	if (this->q < 1.0){
		cout << "Error in eRungeKutta_postTC_RC_FG_rbPL constructor: ";
		cout << "q must be >= 1.0; your q = " << this->q << endl;
		exit(1);
	}
	if (this->w <= 0.0 || this->w >= 1.0){
		cout << "Error in eRungeKutta_postTC_RC_FG_rbPL constructor: ";
		cout << "w must be > 0.0 and < 1.0; your w = " << this->w << endl;
		exit(1);
	}
}

eRungeKutta_postTC_RC_FG_rbPL::eRungeKutta_postTC_RC_FG_rbPL(const eRungeKutta_postTC_RC_FG_rbPL& tc_rc_fg_pl)
		: eRungeKutta_TC_RC_FG_rbPL(tc_rc_fg_pl), preCalc(true), pp(tc_rc_fg_pl.pp), q(tc_rc_fg_pl.q), w(tc_rc_fg_pl.w){
	if (debug)
		cout << "eRungeKutta_postTC_RC_FG_rbPL copy constructor called." << endl;
}

eRungeKutta_postTC_RC_FG_rbPL::~eRungeKutta_postTC_RC_FG_rbPL(){
	if (debug)
		cout << "eRungeKutta_postTC_RC_FG_rbPL destructor called." << endl;
}

void eRungeKutta_postTC_RC_FG_rbPL::getNewTau(double& tau){

	// Check for new rxns
	while (this->oldPop.size() != this->rxn.size() || this->projPop.size() != this->rxn.size()){
		this->addRxn();
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

bool eRungeKutta_postTC_RC_FG_rbPL::check(){
//	cout << "**Checking**" << endl;
	bool ok;
	this->substantially = this->ch->check(this->w,this->aCalc->a_eff,this->oldPop,true);
	if (this->substantially){
		ok = true;
//		cout << "**Substantially accepted**" << endl;
	}
	else{
		ok = this->ch->check(1.0,this->aCalc->a_eff,this->oldPop,true);
//		if (ok);// cout << "**Barely accepted**" << endl;
//		else cout << "**Rejected**" << endl;
	}
	return ok;
}
/*
void eRungeKutta_postTC_RC_FG_rbPL::update(){
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
		cout << "Error in eRungeKutta_postTC_RC_FG_rbPL::update(): Sizes of 'oldPop' and 'rxn' vectors not equal. "
			 << "Shouldn't happen. Exiting." << endl;
		exit(1);
	}
	if (this->projPop.size() != this->rxn.size()){
		cout << "Error in eRungeKutta_postTC_RC_FG_rbPL::update(): Sizes of 'projPop' and 'rxn' vectors not equal. "
			 << "Shouldn't happen. Exiting." << endl;
		exit(1);
	}
}

void eRungeKutta_postTC_RC_FG_rbPL::addRxn(){
	// Add to oldPop[]
	if (this->oldPop.size() < this->rxn.size()){
		unsigned int u = this->oldPop.size();
		this->oldPop.push_back(new double[this->rxn[u]->rateSpecies.size()]);
		for (unsigned int j=0;j < this->rxn[u]->rateSpecies.size();j++){
			this->oldPop[u][j] = this->rxn[u]->rateSpecies[j]->population;
		}
	}
	else if (this->oldPop.size() > this->rxn.size()){ // Only throw error if oldPop.size > rxn.size
		cout << "Error in eRungeKutta_postTC_RC_FG_rbPL::addRxn(): oldPop.size (" << this->oldPop.size() << ") > "
				<< "rxn.size (" << this->rxn.size() << ")." << endl;
		cout << "Shouldn't happen. Exiting." << endl;
		exit(1);
	}
	// Add to projPop[]
	if (this->projPop.size() < this->rxn.size()){
		unsigned int u = this->projPop.size();
		this->projPop.push_back(new double[this->rxn[u]->rateSpecies.size()]);
		for (unsigned int j=0;j < this->rxn[u]->rateSpecies.size();j++){
			this->projPop[u][j] = 0.0; // Initialize
		}
	}
	else{ // Throw error if projPop.size >= rxn.size
		cout << "Error in eRungeKutta_postTC_RC_FG_rbPL::addRxn(): No rxns to add (projPop.size = "
			 << this->projPop.size() << ", rxn.size = " << this->rxn.size() << ")." << endl;
		cout << "Shouldn't happen. Exiting." << endl;
		exit(1);
	}
}
*/
