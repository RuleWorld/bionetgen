/*
 * eRungeKuttaSB_FG_PL.cpp
 *
 *  Created on: Apr 26, 2011
 *      Author: Leonard Harris
 */

#include "eRungeKutta.hh"
#include "../../util/util.hh"
/*
eRungeKuttaSB_FG_PL::eRungeKuttaSB_FG_PL() : sp(), rxn(){
	if (MoMMA::debug)
		cout << "eRungeKuttaSB_FG_PL constructor called." << endl;
}
*/
eRungeKuttaSB_FG_PL::eRungeKuttaSB_FG_PL(ButcherTableau bt, double eps, double p, vector<SimpleSpecies*>& sp,
		vector<Reaction*>& rxn) : eRungeKutta_FG(bt,sp,rxn), BinomialCorrector_PL(p,rxn), sp(sp){//, rxn(rxn){
	if (debug)
		cout << "eRungeKuttaSB_FG_PL constructor called." << endl;
	this->pl = new eRKsb_PL(eps,this->sp);
	this->gGet = new g_Getter(this->sp,rxn);
	// Add species
	for (unsigned int j=0;j < this->sp.size();j++){
		this->addSpecies();
	}
}

eRungeKuttaSB_FG_PL::eRungeKuttaSB_FG_PL(ButcherTableau bt, double eps, double p, vector<SimpleSpecies*>& sp,
		vector<Reaction*>& rxn, bool round) : eRungeKutta_FG(bt,sp,rxn,round), BinomialCorrector_PL(p,rxn), sp(sp){//, rxn(rxn){
	if (debug)
		cout << "eRungeKuttaSB_FG_PL constructor called." << endl;
	this->pl = new eRKsb_PL(eps,this->sp);
	this->gGet = new g_Getter(this->sp,rxn);
	// Add species
	for (unsigned int j=0;j < this->sp.size();j++){
		this->addSpecies();
	}
}

eRungeKuttaSB_FG_PL::eRungeKuttaSB_FG_PL(const eRungeKuttaSB_FG_PL& fg_pl) : eRungeKutta_FG(fg_pl), BinomialCorrector_PL(fg_pl),
		oldPop(fg_pl.oldPop), old_g(fg_pl.old_g), sp(fg_pl.sp){//, rxn(fg_pl.rxn){
	if (debug)
		cout << "eRungeKuttaSB_FG_PL copy constructor called." << endl;
	this->pl = new eRKsb_PL(*fg_pl.pl);
	this->gGet = new g_Getter(*fg_pl.gGet);
}

eRungeKuttaSB_FG_PL::~eRungeKuttaSB_FG_PL(){
	if (debug)
		cout << "eRungeKuttaSB_FG_PL destructor called." << endl;
	delete this->pl;
	delete this->gGet;
}

bool eRungeKuttaSB_FG_PL::check(){
	// Check for new species
	while (this->oldPop.size() != this->sp.size() && this->old_g.size() != this->sp.size()){
		this->addSpecies();
	}
	// X_eff[] elements have already been calculated in fireRxns()
	return this->pl->check(1.0,this->aCalc->X_eff,this->oldPop,this->old_g,true);
}

void eRungeKuttaSB_FG_PL::update(){
	// Update oldPop[] and old_g[]
	for (unsigned int j=0;j < this->oldPop.size();j++){
		this->oldPop[j] = this->sp[j]->population;
		this->old_g[j] = this->gGet->get_g(j);
	}
	// Just in case
	if (this->oldPop.size() != this->sp.size()){
		cout << "Error in eRungeKuttaSB_FG_PL::update(): Sizes of 'oldPop' and 'sp' vectors not equal. Shouldn't happen. "
			 << "Exiting." << endl;
		exit(1);
	}
	if (this->old_g.size() != this->sp.size()){
		cout << "Error in eRungeKuttaSB_FG_PL::update(): Sizes of 'old_g' and 'sp' vectors not equal. Shouldn't happen. "
			 << "Exiting." << endl;
		exit(1);
	}
}

void eRungeKuttaSB_FG_PL::addSpecies(){
	if (this->oldPop.size() < this->sp.size()  && this->old_g.size() < this->sp.size()
			&& this->oldPop.size() == this->old_g.size()){
		unsigned int i = this->oldPop.size();
		this->oldPop.push_back(this->sp[i]->population);
		this->old_g.push_back(this->gGet->get_g(i));;
	}
	else{
		cout << "Error in eRungeKuttaSB_FG_PL::addSpecies(): No species to add (oldPop.size = " << this->oldPop.size()
			 << ", old_g.size = " << old_g.size() << ", sp.size = " << this->sp.size() << "). Shouldn't happen. Exiting.\n";
		exit(1);
	}
}
