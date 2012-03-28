/*
 * eRungeKutta_FG_sbPL.cpp
 *
 *  Created on: Apr 26, 2011
 *      Author: Leonard Harris
 */

#include "eRungeKutta_EXTRA.hh"
#include "../../../util/util.hh"
/*
eRungeKuttaSB_FG_PL::eRungeKuttaSB_FG_PL() : sp(){
	if (MoMMA::debug)
		cout << "eRungeKuttaSB_FG_PL constructor called." << endl;
}
*/
eRungeKutta_FG_sbPL::eRungeKutta_FG_sbPL(ButcherTableau bt, double eps, double p, vector<SimpleSpecies*>& sp,
		vector<Reaction*>& rxn) : eRungeKutta_FG(bt,sp,rxn), sp(sp){
	if (debug)
		cout << "eRungeKutta_FG_sbPL constructor called." << endl;
	this->ch = new SBChecker(eps,this->sp);
	this->bc = new BinomialCorrector_RK(p,rxn);
	this->gGet = new g_Getter(this->sp,rxn);
	// Add species
	for (unsigned int j=0;j < this->sp.size();j++){
		this->addSpecies();
	}
}

eRungeKutta_FG_sbPL::eRungeKutta_FG_sbPL(ButcherTableau bt, double eps, double p, vector<SimpleSpecies*>& sp,
		vector<Reaction*>& rxn, bool round) : eRungeKutta_FG(bt,sp,rxn,round), sp(sp){
	if (debug)
		cout << "eRungeKutta_FG_sbPL constructor called." << endl;
	this->ch = new SBChecker(eps,this->sp);
	this->bc = new BinomialCorrector_RK(p,rxn);
	this->gGet = new g_Getter(this->sp,rxn);
	// Add species
	for (unsigned int j=0;j < this->sp.size();j++){
		this->addSpecies();
	}
}

eRungeKutta_FG_sbPL::eRungeKutta_FG_sbPL(const eRungeKutta_FG_sbPL& fg_pl) : eRungeKutta_FG(fg_pl), oldPop(fg_pl.oldPop),
		old_g(fg_pl.old_g), sp(fg_pl.sp){
	if (debug)
		cout << "eRungeKutta_FG_sbPL copy constructor called." << endl;
	this->ch = new SBChecker(*fg_pl.ch);
	this->bc = new BinomialCorrector_RK(*fg_pl.bc);
	this->gGet = new g_Getter(*fg_pl.gGet);
}

eRungeKutta_FG_sbPL::~eRungeKutta_FG_sbPL(){
	if (debug)
		cout << "eRungeKutta_FG_sbPL destructor called." << endl;
	delete this->ch;
	delete this->bc;
	delete this->gGet;
}

bool eRungeKutta_FG_sbPL::check(){
	// Check for new species
	while (this->oldPop.size() != this->sp.size() && this->old_g.size() != this->sp.size()){
		this->addSpecies();
	}
	// X_eff[] elements have already been calculated in fireRxns()
	return this->ch->check(1.0,this->aCalc->X_eff,this->oldPop,this->old_g,true);
}

void eRungeKutta_FG_sbPL::update(){
	// Update oldPop[] and old_g[]
	for (unsigned int j=0;j < this->oldPop.size();j++){
		this->oldPop[j] = this->sp[j]->population;
		this->old_g[j] = this->gGet->get_g(j);
	}
	// Just in case
	if (this->oldPop.size() != this->sp.size()){
		cout << "Error in eRungeKutta_FG_sbPL::update(): Sizes of 'oldPop' and 'sp' vectors not equal. "
			 << "Shouldn't happen. Exiting." << endl;
		exit(1);
	}
	if (this->old_g.size() != this->sp.size()){
		cout << "Error in eRungeKutta_FG_sbPL::update(): Sizes of 'old_g' and 'sp' vectors not equal. "
			 << "Shouldn't happen. Exiting." << endl;
		exit(1);
	}
}

void eRungeKutta_FG_sbPL::addSpecies(){
	if (this->oldPop.size() < this->sp.size()  && this->old_g.size() < this->sp.size()
			&& this->oldPop.size() == this->old_g.size()){
		unsigned int i = this->oldPop.size();
		this->oldPop.push_back(this->sp[i]->population);
		this->old_g.push_back(this->gGet->get_g(i));;
	}
	else{
		cout << "Error in eRungeKutta_FG_sbPL::addSpecies(): No species to add (oldPop.size = " << this->oldPop.size()
			 << ", old_g.size = " << old_g.size() << ", sp.size = " << this->sp.size() << "). Shouldn't happen. Exiting.\n";
		exit(1);
	}
}
