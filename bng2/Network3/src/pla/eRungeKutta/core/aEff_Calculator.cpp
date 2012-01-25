/*
 * aEff_Calculator.cpp
 *
 *  Created on: Apr 30, 2011
 *      Author: Leonard Harris
 */

#include "aEff_Calculator.hh"
/*
aEff_Calculator::aEff_Calculator() : sp(), rxn(){
	if (MoMMA::debug)
		cout << "aEff_Calculator constructor called." << endl;
}
*/
aEff_Calculator::aEff_Calculator(ButcherTableau bt, vector<SimpleSpecies*>& sp, vector<Reaction*>& rxn) : bt(bt), sp(sp), rxn(rxn){
	if (debug)
		cout << "aEff_Calculator constructor called." << endl;
	// Add rxns
	for (unsigned int v=0;v < this->rxn.size();v++){
		this->addRxn();
	}
	// Add species
	for (unsigned int j=0;j < this->sp.size();j++){
		this->addSpecies();
	}
}

aEff_Calculator::aEff_Calculator(const aEff_Calculator& aCalc) : bt(aCalc.bt), a_eff(aCalc.a_eff), X_eff(aCalc.X_eff),
	rateSp(aCalc.rateSp), spInRxn(aCalc.spInRxn), stoich(aCalc.stoich), sp(aCalc.sp), rxn(aCalc.rxn){
	if (debug)
		cout << "aEff_Calculator copy constructor called." << endl;
}

aEff_Calculator::~aEff_Calculator(){
	if (debug)
		cout << "aEff_Calculator destructor called." << endl;
	for (unsigned int v=0;v < this->rateSp.size();v++){
		delete this->rateSp[v];
	}
}

void aEff_Calculator::calc_aEff(double tau){

	// Check if new rxns were added to system
	while (this->a_eff.size() != this->rxn.size()){
		this->addRxn();
	}
	// Check if new species were added to system
	while (this->X_eff.size() != this->sp.size()){
		this->addSpecies();
	}

	// Initial populations
	double X[this->sp.size()][this->bt.size()];
	for (unsigned int j=0;j < this->sp.size();j++){
		X[j][0] = this->sp[j]->population;
		this->X_eff[j] = this->bt.beta[0]*X[j][0];
	}
	// Initial rates
	double a[this->rxn.size()][this->bt.size()];
	for (unsigned int v=0;v < this->rxn.size();v++){
		a[v][0] = this->rxn[v]->getRate();
		this->a_eff[v] = this->bt.beta[0]*a[v][0];
	}
	// Loop over stages
	for (unsigned int s=1;s < this->bt.size();s++){
		// Populations
		for (unsigned int j=0;j < this->sp.size();j++){
			X[j][s] = 0.0;
			for (unsigned int r=0;r < s;r++){
				if (fabs(this->bt.alpha[s][r]) > TOL){ // Skip the sum if alpha[s][r] is zero.
					double m_j = 0.0;
					for (unsigned int v=0;v < this->spInRxn[j].size();v++){
						m_j += this->stoich[j][v]*a[this->spInRxn[j][v]][r];
					}
					X[j][s] += this->bt.alpha[s][r]*m_j;
				}
			}
			X[j][s] *= tau;
//			X[j][s] += X[j][0];
			X[j][s] += this->sp[j]->population;
			this->X_eff[j] += this->bt.beta[s]*X[j][s];
		}
		// Rates
		vector<double> Xs;
		for (unsigned int v=0;v < this->rxn.size();v++){
			Xs.resize(this->rxn[v]->rateSpecies.size());
			for (unsigned int j=0;j < this->rxn[v]->rateSpecies.size();j++){
				Xs[j] = X[this->rateSp[v][j]][s];
			}
			a[v][s] = this->rxn[v]->re->getRate(Xs); // Have to calculate this, even if beta[s] = 0.
			this->a_eff[v] += this->bt.beta[s]*a[v][s];
		}
	}
}

void aEff_Calculator::addRxn(){
	if (this->a_eff.size() < this->rxn.size()){
		unsigned int u = this->a_eff.size();
		// Add new element to a_eff[]
		this->a_eff.push_back(0.0);
		// Species that contribute to rxn rates
		this->rateSp.push_back(new unsigned int[this->rxn[u]->rateSpecies.size()]);
		for (unsigned int i=0;i < this->rxn[u]->rateSpecies.size();i++){
			bool found = false;
			for (unsigned int j=0;!found;j++){
	//			for (unsigned int j=0;j < this->sp.size() && !found;j++){
				if (this->rxn[u]->rateSpecies[i] == this->sp.at(j)){ // Use at() to check for out-of-boundedness
					this->rateSp[u][i] = j;
					found = true;
				}
			}
		}
		// See if existing species participate in new rxn
		map<SimpleSpecies*,int>::iterator it;
		for (unsigned int j=0;j < this->X_eff.size();j++){ // X_eff.size, not sp.size
			it = this->rxn[u]->stoichSpecies.find(this->sp[j]);
			if (it != this->rxn[u]->stoichSpecies.end()){ // Species j involved in Reaction u
				this->spInRxn[j].push_back(u);
				this->stoich[j].push_back(static_cast<double>((*it).second));
			}
		}
	}
	else{
		cout << "Error in aEff_Calculator::addRxn(): No rxns to add (a_eff.size = " << this->a_eff.size() << ", rxn.size = "
			 << this->rxn.size() << "). Shouldn't happen. Exiting." << endl;
		exit(1);
	}
}

void aEff_Calculator::addSpecies(){
	if (this->X_eff.size() < this->sp.size()){
		unsigned int i = this->X_eff.size();
		// Add new elements to X_eff[], spInRxn[] and stoich[]
		this->X_eff.push_back(0.0);
		this->spInRxn.push_back(vector<unsigned int>());
		this->stoich.push_back(vector<double>());
		// Rxns that species i participates in
		map<SimpleSpecies*,int>::iterator it;
		for (unsigned int v=0;v < this->a_eff.size();v++){ // a_eff.size, not rxn.size (should actually be the same)
			it = this->rxn[v]->stoichSpecies.find(this->sp[i]);
			if (it != this->rxn[v]->stoichSpecies.end()){ // Species i involved in Reaction v
				this->spInRxn[i].push_back(v);
				this->stoich[i].push_back(static_cast<double>((*it).second));
			}
		}
	}
	else{
		cout << "Error in aEff_Calculator::addSpecies(): No species to add (X_eff.size = " << this->X_eff.size()
			 << ", sp.size = " << this->sp.size() << "). Shouldn't happen. Exiting." << endl;
		exit(1);
	}
}
