/*
 * eRungeKutta_TC_RC_FG_rbPL.cpp
 *
 *  Created on: Apr 30, 2011
 *      Author: Leonard Harris
 */

#include "../eRungeKutta.hh"
/*
eRungeKutta_TC_RC_FG_rbPL::eRungeKutta_TC_RC_FG_rbPL() : p(){
	if (MoMMA::debug)
		cout << "eRungeKutta_TC_RC_FG_rbPL constructor called." << endl;
}
*/
eRungeKutta_TC_RC_FG_rbPL::eRungeKutta_TC_RC_FG_rbPL(ButcherTableau bt, double eps, double approx1, double gg1, double p,
		Preleap_TC* ptc, vector<SimpleSpecies*>& sp, vector<Reaction*>& rxn)
		: eRungeKutta_TC_RC_FG_PL(bt,approx1,gg1,p,ptc,sp,rxn), p(p), rxn(rxn){
	if (debug)
		cout << "eRungeKutta_TC_RC_FG_rbPL constructor called." << endl;
	this->ch = new RBChecker(eps,this->rxn);
	// Add rxns
	for (unsigned int v=0;v < this->rxn.size();v++){
		this->addRxn();
	}
}

eRungeKutta_TC_RC_FG_rbPL::eRungeKutta_TC_RC_FG_rbPL(ButcherTableau bt, double eps, double approx1, double gg1, double p,
		Preleap_TC* ptc, vector<SimpleSpecies*>& sp, vector<Reaction*>& rxn, bool round)
		: eRungeKutta_TC_RC_FG_PL(bt,approx1,gg1,p,ptc,sp,rxn,round), p(p), rxn(rxn){
	if (debug)
		cout << "eRungeKutta_TC_RC_FG_rbPL constructor called." << endl;
	this->ch = new RBChecker(eps,this->rxn);
	// Add rxns
	for (unsigned int v=0;v < this->rxn.size();v++){
		this->addRxn();
	}
}

eRungeKutta_TC_RC_FG_rbPL::eRungeKutta_TC_RC_FG_rbPL(const eRungeKutta_TC_RC_FG_rbPL& tc_rc_fg_pl)
		: eRungeKutta_TC_RC_FG_PL(tc_rc_fg_pl), p(tc_rc_fg_pl.p), rxn(tc_rc_fg_pl.rxn){
	if (debug)
		cout << "eRungeKutta_TC_RC_FG_rbPL copy constructor called." << endl;
	this->ch = new RBChecker(*tc_rc_fg_pl.ch);
	this->oldPop.resize(this->rxn.size());
	this->projPop.resize(this->rxn.size());
	for (unsigned int v=0;v < this->rxn.size();v++){
		this->oldPop[v] = new double[this->rxn[v]->rateSpecies.size()];
		this->projPop[v] = new double[this->rxn[v]->rateSpecies.size()];
		for (unsigned int j=0;j < this->rxn[v]->rateSpecies.size();j++){
			this->oldPop[v][j] = tc_rc_fg_pl.oldPop[v][j];
			this->projPop[v][j] = tc_rc_fg_pl.projPop[v][j];
		}
	}
}

eRungeKutta_TC_RC_FG_rbPL::~eRungeKutta_TC_RC_FG_rbPL(){
	if (debug)
		cout << "eRungeKutta_TC_RC_FG_rbPL destructor called." << endl;
	delete this->ch;
	for (unsigned int v=0;v < this->rxn.size();v++){
		delete[] this->oldPop[v];
		delete[] this->projPop[v];
	}
}

void eRungeKutta_TC_RC_FG_rbPL::update(){
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
		cout << "Error in eRungeKutta_TC_RC_FG_rbPL::update(): Sizes of 'oldPop' and 'rxn' vectors not equal. "
			 << "Shouldn't happen. Exiting." << endl;
		exit(1);
	}
	if (this->projPop.size() != this->rxn.size()){
		cout << "Error in eRungeKutta_TC_RC_FG_rbPL::update(): Sizes of 'projPop' and 'rxn' vectors not equal. "
			 << "Shouldn't happen. Exiting." << endl;
		exit(1);
	}
}

void eRungeKutta_TC_RC_FG_rbPL::calc_projPop(double tau){

	// Calculate projected species population changes
	double delta_X[this->aCalc->X_eff.size()];
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
}

void eRungeKutta_TC_RC_FG_rbPL::addRxn(){
	// Add to oldPop[]
	if (this->oldPop.size() < this->rxn.size()){
		unsigned int u = this->oldPop.size();
		this->oldPop.push_back(new double[this->rxn[u]->rateSpecies.size()]);
		for (unsigned int j=0;j < this->rxn[u]->rateSpecies.size();j++){
			this->oldPop[u][j] = this->rxn[u]->rateSpecies[j]->population;
		}
	}
	else if (this->oldPop.size() > this->rxn.size()){ // Only throw error if oldPop.size > rxn.size
		cout << "Error in eRungeKutta_TC_RC_FG_rbPL::addRxn(): oldPop.size (" << this->oldPop.size() << ") > "
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
		cout << "Error in eRungeKutta_TC_RC_FG_rbPL::addRxn(): No rxns to add (projPop.size = "
			 << this->projPop.size() << ", rxn.size = " << this->rxn.size() << ")." << endl;
		cout << "Shouldn't happen. Exiting." << endl;
		exit(1);
	}
}
