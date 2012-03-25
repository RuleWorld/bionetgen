/*
 * eRungeKutta_FG_rbPL.cpp
 *
 *  Created on: Apr 26, 2011
 *      Author: Leonard Harris
 */

#include "eRungeKutta_EXTRA.hh"
#include "../../../util/util.hh"
/*
eRungeKuttaRB_FG_PL::eRungeKuttaRB_FG_PL() : rxn(){
	if (MoMMA::debug)
		cout << "eRungeKuttaRB_FG_PL constructor called." << endl;
}
*/
eRungeKutta_FG_rbPL::eRungeKutta_FG_rbPL(ButcherTableau bt, double eps, double p, vector<SimpleSpecies*>& sp,
		vector<Reaction*>& rxn) : eRungeKutta_FG(bt,sp,rxn), rxn(rxn){
	if (debug)
		cout << "eRungeKutta_FG_rbPL constructor called." << endl;
	this->ch = new RBChecker(eps,this->rxn);
	this->bc = new BinomialCorrector_RK(p,this->rxn);
	// Add rxns
	for (unsigned int v=0;v < this->rxn.size();v++){
		this->addRxn();
	}
}

eRungeKutta_FG_rbPL::eRungeKutta_FG_rbPL(ButcherTableau bt, double eps, double p, vector<SimpleSpecies*>& sp,
		vector<Reaction*>& rxn, bool round) : eRungeKutta_FG(bt,sp,rxn,round), rxn(rxn){
	if (debug)
		cout << "eRungeKutta_FG_rbPL constructor called." << endl;
	this->ch = new RBChecker(eps,this->rxn);
	this->bc = new BinomialCorrector_RK(p,this->rxn);
	// Add rxns
	for (unsigned int v=0;v < this->rxn.size();v++){
		this->addRxn();
	}
}

eRungeKutta_FG_rbPL::eRungeKutta_FG_rbPL(const eRungeKutta_FG_rbPL& fg_pl) : eRungeKutta_FG(fg_pl), rxn(fg_pl.rxn){
	if (debug)
		cout << "eRungeKutta_FG_rbPL copy constructor called." << endl;
	this->ch = new RBChecker(*fg_pl.ch);
	this->bc = new BinomialCorrector_RK(*fg_pl.bc);
	// Initialize oldPop[]
	this->oldPop.resize(fg_pl.oldPop.size());
	for (unsigned int v=0;v < this->oldPop.size();v++){
		this->oldPop[v] = new double[this->rxn[v]->rateSpecies.size()];
		for (unsigned int j=0;j < this->rxn[v]->rateSpecies.size();j++){
			this->oldPop[v][j] = fg_pl.oldPop[v][j];
		}
	}
}

eRungeKutta_FG_rbPL::~eRungeKutta_FG_rbPL(){
	if (debug)
		cout << "eRungeKutta_FG_rbPL destructor called." << endl;
	delete this->ch;
	delete this->bc;
	for (unsigned int v=0;v < this->oldPop.size();v++){
		delete[] this->oldPop[v];
	}
}

bool eRungeKutta_FG_rbPL::check(){
	// Check for new rxns
	while (this->oldPop.size() != this->rxn.size()){
		this->addRxn();
	}
	// a_eff[] elements have already been calculated in fireRxns()
	return this->ch->check(1.0,this->aCalc->a_eff,this->oldPop,true);
}

void eRungeKutta_FG_rbPL::update(){
	// Update oldPop[][]
	for (unsigned int v=0;v < this->oldPop.size();v++){
		for (unsigned int j=0;j < this->rxn[v]->rateSpecies.size();j++){
			this->oldPop[v][j] = this->rxn[v]->rateSpecies[j]->population;
		}
	}
	// Just in case
	if (this->oldPop.size() != this->rxn.size()){
		cout << "Error in eRungeKutta_FG_rbPL::update(): Sizes of 'oldPop' and 'rxn' vectors not equal. "
			 <<	"Shouldn't happen. Exiting." << endl;
		exit(1);
	}
}

void eRungeKutta_FG_rbPL::addRxn(){
	if (this->oldPop.size() < this->rxn.size()){
		unsigned int u = this->oldPop.size();
		this->oldPop.push_back(new double[this->rxn[u]->rateSpecies.size()]);
		for (unsigned int j=0;j < this->rxn[u]->rateSpecies.size();j++){
			this->oldPop[u][j] = this->rxn[u]->rateSpecies[j]->population;
		}
	}
	else{
		cout << "Error in eRungeKutta_FG_rbPL::addRxn(): No rxns to add (oldPop.size = " << this->oldPop.size()
			 << ", rxn.size = " << this->rxn.size() << "). Shouldn't happen. Exiting." << endl;
		exit(1);
	}
}
