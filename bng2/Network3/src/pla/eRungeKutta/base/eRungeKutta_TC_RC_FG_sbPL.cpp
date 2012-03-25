/*
 * eRungeKutta_TC_RC_FG_sbPL.cpp
 *
 *  Created on: Apr 30, 2011
 *      Author: Leonard Harris
 */

#include "../eRungeKutta.hh"
/*
eRungeKutta_TC_RC_FG_sbPL::eRungeKutta_TC_RC_FG_sbPL() : p(){
	if (MoMMA::debug)
		cout << "eRungeKutta_TC_RC_FG_sbPL constructor called." << endl;
}
*/
eRungeKutta_TC_RC_FG_sbPL::eRungeKutta_TC_RC_FG_sbPL(ButcherTableau bt, double eps, double approx1, double gg1, double p,
		Preleap_TC* ptc, vector<SimpleSpecies*>& sp, vector<Reaction*>& rxn)
		: eRungeKutta_TC_RC_FG_PL(bt,approx1,gg1,p,ptc,sp,rxn), p(p), sp(sp){
	if (debug)
		cout << "eRungeKutta_TC_RC_FG_sbPL constructor called." << endl;
	this->ch = new SBChecker(eps,this->sp);
	this->gGet = new g_Getter(this->sp,rxn);
	for (unsigned int j=0;j < this->sp.size();j++){
		this->addSpecies();
	}
}

eRungeKutta_TC_RC_FG_sbPL::eRungeKutta_TC_RC_FG_sbPL(ButcherTableau bt, double eps, double approx1, double gg1, double p,
		Preleap_TC* ptc, vector<SimpleSpecies*>& sp, vector<Reaction*>& rxn, bool round)
		: eRungeKutta_TC_RC_FG_PL(bt,approx1,gg1,p,ptc,sp,rxn,round), p(p), sp(sp){
	if (debug)
		cout << "eRungeKutta_TC_RC_FG_sbPL constructor called." << endl;
	this->ch = new SBChecker(eps,this->sp);
	this->gGet = new g_Getter(this->sp,rxn);
	for (unsigned int j=0;j < this->sp.size();j++){
		this->addSpecies();
	}
}

eRungeKutta_TC_RC_FG_sbPL::eRungeKutta_TC_RC_FG_sbPL(const eRungeKutta_TC_RC_FG_sbPL& tc_rc_fg_pl)
		: eRungeKutta_TC_RC_FG_PL(tc_rc_fg_pl), p(tc_rc_fg_pl.p), oldPop(tc_rc_fg_pl.oldPop), projPop(tc_rc_fg_pl.projPop),
		  old_g(tc_rc_fg_pl.old_g), sp(tc_rc_fg_pl.sp){
	if (debug)
		cout << "eRungeKutta_TC_RC_FG_sbPL copy constructor called." << endl;
	this->ch = new SBChecker(*tc_rc_fg_pl.ch);
	this->gGet = new g_Getter(*tc_rc_fg_pl.gGet);
}

eRungeKutta_TC_RC_FG_sbPL::~eRungeKutta_TC_RC_FG_sbPL(){
	if (debug)
		cout << "eRungeKutta_TC_RC_FG_sbPL destructor called." << endl;
	delete this->ch;
	delete this->gGet;
}

void eRungeKutta_TC_RC_FG_sbPL::update(){
	// Update aCalc
	this->aCalc->update();
	// Update oldPop[] and old_g[]
	for (unsigned int j=0;j < this->oldPop.size();j++){
		this->oldPop[j] = this->sp[j]->population;
		this->old_g[j] = this->gGet->get_g(j);
	}
	// Just in case
	if (this->oldPop.size() != this->sp.size()){
		cout << "Error in eRungeKutta_TC_RC_FG_sbPL::update(): Sizes of 'oldPop' and 'sp' vectors not equal. "
			 << "Shouldn't happen. Exiting." << endl;
		exit(1);
	}
	if (this->projPop.size() != this->sp.size()){
		cout << "Error in eRungeKutta_TC_RC_FG_sbPL::update(): Sizes of 'projPop' and 'sp' vectors not equal. "
			 << "Shouldn't happen. Exiting." << endl;
		exit(1);
	}
	if (this->old_g.size() != this->sp.size()){
		cout << "Error in eRungeKutta_TC_RC_FG_sbPL::update(): Sizes of 'old_g' and 'sp' vectors not equal. "
			 << "Shouldn't happen. Exiting." << endl;
		exit(1);
	}
}

void eRungeKutta_TC_RC_FG_sbPL::calc_projPop(double tau){

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
	// Calculate elements of projPop[]
	for (unsigned int j=0;j < this->projPop.size();j++){
		this->projPop[j] = this->oldPop[j] + delta_X[j];
	}
}

void eRungeKutta_TC_RC_FG_sbPL::addSpecies(){
	// Add to oldPop[] and old_g[]
	if (this->oldPop.size() < this->sp.size() && this->oldPop.size() == this->old_g.size()){
		unsigned int i = this->oldPop.size();
		this->oldPop.push_back(this->sp[i]->population);
		this->old_g.push_back(this->gGet->get_g(i));;
	}
	else if (this->oldPop.size() > this->sp.size()){ // Throw error if oldPop.size > sp.size
		cout << "Error in eRungeKutta_TC_RC_FG_sbPL::addSpecies(): oldPop.size ("
			 << this->oldPop.size() << ") > sp.size (" << this->sp.size() << ")." << endl;
		cout << "Shouldn't happen. Exiting." << endl;
		exit(1);
	}
	else if (this->old_g.size() > this->sp.size()){ // Throw error if old_g.size > sp.size
		cout << "Error in eRungeKutta_TC_RC_FG_sbPL::addSpecies(): old_g.size ("
			 << this->old_g.size() << ") > sp.size (" << this->sp.size() << ")." << endl;
		cout << "Shouldn't happen. Exiting." << endl;
		exit(1);
	}
	// Add to projPop[]
	if (this->projPop.size() < this->sp.size()){
		this->projPop.push_back(0.0);
	}
	else{ // Throw error if projPop.size >= sp.size
		cout << "Error in eRungeKutta_TC_RC_FG_sbPL::addSpecies(): No species to add (projPop.size = "
			 << this->projPop.size() << ", sp.size = " << this->sp.size() << ")." << endl;
		cout << "Shouldn't happen. Exiting." << endl;
		exit(1);
	}
}
