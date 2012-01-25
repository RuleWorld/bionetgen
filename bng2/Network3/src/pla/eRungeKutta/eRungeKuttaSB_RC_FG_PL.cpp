/*
 * eRungeKuttaSB_RC_FG_PL.cpp
 *
 *  Created on: Apr 30, 2011
 *      Author: Leonard Harris
 */

#include "eRungeKutta.hh"
/*
eRungeKuttaSB_RC_FG_PL::eRungeKuttaSB_RC_FG_PL() : sp(), rxn(){
	if (MoMMA::debug)
		cout << "eRungeKuttaSB_RC_FG_PL constructor called." << endl;
}
*/
eRungeKuttaSB_RC_FG_PL::eRungeKuttaSB_RC_FG_PL(ButcherTableau bt, double eps, double p, double approx1, double gg1,
		vector<SimpleSpecies*>& sp, vector<Reaction*>& rxn) : eRungeKutta_RC_FG(bt,approx1,gg1,sp,rxn),
		BinomialCorrector_PL(p,rxn), sp(sp){//, rxn(rxn){
	if (debug)
		cout << "eRungeKuttaSB_RC_FG_PL constructor called." << endl;
	this->pl = new eRKsb_PL(eps,this->sp);
	this->gGet = new g_Getter(this->sp,rxn);
	for (unsigned int j=0;j < this->sp.size();j++){
		this->addSpecies();
	}
}

eRungeKuttaSB_RC_FG_PL::eRungeKuttaSB_RC_FG_PL(ButcherTableau bt, double eps, double p, double approx1, double gg1,
		vector<SimpleSpecies*>& sp, vector<Reaction*>& rxn, bool round) : eRungeKutta_RC_FG(bt,approx1,gg1,sp,rxn,round),
		BinomialCorrector_PL(p,rxn), sp(sp){//, rxn(rxn){
	if (debug)
		cout << "eRungeKuttaSB_RC_FG_PL constructor called." << endl;
	this->pl = new eRKsb_PL(eps,this->sp);
	this->gGet = new g_Getter(this->sp,rxn);
	for (unsigned int j=0;j < this->sp.size();j++){
		this->addSpecies();
	}
}

eRungeKuttaSB_RC_FG_PL::eRungeKuttaSB_RC_FG_PL(const eRungeKuttaSB_RC_FG_PL& rc_fg_pl) : eRungeKutta_RC_FG(rc_fg_pl),
		BinomialCorrector_PL(rc_fg_pl), oldPop(rc_fg_pl.oldPop), old_g(rc_fg_pl.old_g), sp(rc_fg_pl.sp){
	if (debug)
		cout << "eRungeKuttaSB_RC_FG_PL copy constructor called." << endl;
	this->pl = new eRKsb_PL(*rc_fg_pl.pl);
	this->gGet = new g_Getter(*rc_fg_pl.gGet);
}

eRungeKuttaSB_RC_FG_PL::~eRungeKuttaSB_RC_FG_PL(){
	if (debug)
		cout << "eRungeKuttaSB_RC_FG_PL destructor called." << endl;
	delete this->pl;
	delete this->gGet;
}

bool eRungeKuttaSB_RC_FG_PL::check(){
	// Check for new species
	while (this->oldPop.size() != this->sp.size() && this->old_g.size() != this->sp.size()){
		this->addSpecies();
	}
	// X_eff[] elements have already been calculated in classifyRxns()
	return this->pl->check(1.0,this->aCalc->X_eff,this->oldPop,this->old_g,true);
}

void eRungeKuttaSB_RC_FG_PL::update(){
	// Update oldPop[] and old_g[]
	for (unsigned int j=0;j < this->oldPop.size();j++){
		this->oldPop[j] = this->sp[j]->population;
		this->old_g[j] = this->gGet->get_g(j);
	}
	// Just in case
	if (this->oldPop.size() != this->sp.size()){
		cout << "Error in eRungeKuttaSB_RC_FG_PL::update(): Sizes of 'oldPop' and 'sp' vectors not equal. Shouldn't happen. "
			 << "Exiting." << endl;
		exit(1);
	}
	if (this->old_g.size() != this->sp.size()){
		cout << "Error in eRungeKuttaSB_RC_FG_PL::update(): Sizes of 'old_g' and 'sp' vectors not equal. Shouldn't happen. "
			 << "Exiting." << endl;
		exit(1);
	}
}

void eRungeKuttaSB_RC_FG_PL::addSpecies(){
	if (this->oldPop.size() < this->sp.size() && this->oldPop.size() == this->old_g.size()){
		unsigned int i = this->oldPop.size();
		this->oldPop.push_back(this->sp[i]->population);
		this->old_g.push_back(this->gGet->get_g(i));;
	}
	else{
		cout << "Error in eRungeKuttaSB_RC_FG_PL::addSpecies(): No species to add (oldPop.size = " << this->oldPop.size()
			 << ", old_g.size = " << this->old_g.size() << ", sp.size = " << this->sp.size()
			 << "). Shouldn't happen. Exiting." << endl;
		exit(1);
	}
}
