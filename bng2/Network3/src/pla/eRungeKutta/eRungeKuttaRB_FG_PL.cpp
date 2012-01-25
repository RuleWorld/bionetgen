/*
 * eRungeKuttaRB_FG_PL.cpp
 *
 *  Created on: Apr 26, 2011
 *      Author: Leonard Harris
 */

#include "eRungeKutta.hh"
#include "../../util/util.hh"
/*
eRungeKuttaRB_FG_PL::eRungeKuttaRB_FG_PL() : rxn(){
	if (MoMMA::debug)
		cout << "eRungeKuttaRB_FG_PL constructor called." << endl;
}
*/
eRungeKuttaRB_FG_PL::eRungeKuttaRB_FG_PL(ButcherTableau bt, double eps, double p, vector<SimpleSpecies*>& sp,
		vector<Reaction*>& rxn) : eRungeKutta_FG(bt,sp,rxn), BinomialCorrector_PL(p,rxn), rxn(rxn){
	if (debug)
		cout << "eRungeKuttaRB_FG_PL constructor called." << endl;
	this->pl = new eRKrb_PL(eps,this->rxn);
	// Add rxns
	for (unsigned int v=0;v < this->rxn.size();v++){
		this->addRxn();
	}
}

eRungeKuttaRB_FG_PL::eRungeKuttaRB_FG_PL(ButcherTableau bt, double eps, double p, vector<SimpleSpecies*>& sp,
		vector<Reaction*>& rxn, bool round) : eRungeKutta_FG(bt,sp,rxn,round), BinomialCorrector_PL(p,rxn), rxn(rxn){
	if (debug)
		cout << "eRungeKuttaRB_FG_PL constructor called." << endl;
	this->pl = new eRKrb_PL(eps,this->rxn);
	// Add rxns
	for (unsigned int v=0;v < this->rxn.size();v++){
		this->addRxn();
	}
}

eRungeKuttaRB_FG_PL::eRungeKuttaRB_FG_PL(const eRungeKuttaRB_FG_PL& fg_pl) : eRungeKutta_FG(fg_pl),
		BinomialCorrector_PL(fg_pl), rxn(fg_pl.rxn){
	if (debug)
		cout << "eRungeKuttaRB_FG_PL copy constructor called." << endl;
	this->pl = new eRKrb_PL(*fg_pl.pl);
	// Initialize oldPop[]
	this->oldPop.resize(fg_pl.oldPop.size());
	for (unsigned int v=0;v < this->oldPop.size();v++){
		this->oldPop[v] = new double[this->rxn[v]->rateSpecies.size()];
		for (unsigned int j=0;j < this->rxn[v]->rateSpecies.size();j++){
			this->oldPop[v][j] = fg_pl.oldPop[v][j];
		}
	}
}

eRungeKuttaRB_FG_PL::~eRungeKuttaRB_FG_PL(){
	if (debug)
		cout << "eRungeKuttaRB_FG_PL destructor called." << endl;
	delete this->pl;
	for (unsigned int v=0;v < this->oldPop.size();v++){
		delete[] this->oldPop[v];
	}
}

bool eRungeKuttaRB_FG_PL::check(){
	// Check for new rxns
	while (this->oldPop.size() != this->rxn.size()){
		this->addRxn();
	}
	// a_eff[] elements have already been calculated in fireRxns()
	return this->pl->check(1.0,this->aCalc->a_eff,this->oldPop,true);
}

void eRungeKuttaRB_FG_PL::update(){
	// Update oldPop[][]
	for (unsigned int v=0;v < this->oldPop.size();v++){
		for (unsigned int j=0;j < this->rxn[v]->rateSpecies.size();j++){
			this->oldPop[v][j] = this->rxn[v]->rateSpecies[j]->population;
		}
	}
	// Just in case
	if (this->oldPop.size() != this->rxn.size()){
		cout << "Error in eRungeKuttaRB_FG_PL::update(): Sizes of 'oldPop' and 'rxn' vectors not equal. Shouldn't happen. "
			 << "Exiting." << endl;
		exit(1);
	}
}

void eRungeKuttaRB_FG_PL::addRxn(){
	if (this->oldPop.size() < this->rxn.size()){
		unsigned int u = this->oldPop.size();
		this->oldPop.push_back(new double[this->rxn[u]->rateSpecies.size()]);
		for (unsigned int j=0;j < this->rxn[u]->rateSpecies.size();j++){
			this->oldPop[u][j] = this->rxn[u]->rateSpecies[j]->population;
		}
	}
	else{
		cout << "Error in eRungeKuttaRB_FG_PL::addRxn(): No rxns to add (oldPop.size = " << this->oldPop.size()
			 << ", rxn.size = " << this->rxn.size() << "). Shouldn't happen. Exiting." << endl;
		exit(1);
	}
}
