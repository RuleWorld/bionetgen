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
aEff_Calculator::aEff_Calculator(ButcherTableau bt, vector<SimpleSpecies*>& sp, vector<Reaction*>& rxn) : bt(bt), sp(sp),
	rxn(rxn){
	if (debug)
		cout << "aEff_Calculator constructor called." << endl;
	// Error check
	if (this->bt.size() == 0){
		/*cerr*/cout << "Error in aEff_Calculator constructor: Butcher tableau has no elements (size=0). Exiting." << endl;
		exit(1);
	}
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
	// Error check
	if (this->bt.size() == 0){
		cout << "Error in aEff_Calculator copy constructor: Butcher tableau has no elements (size=0). Exiting." << endl;
		exit(1);
	}
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
	// # of stages
	unsigned int nStages = this->bt.size();
	// Initial populations
	double X[this->sp.size()][nStages];
	for (unsigned int j=0;j < this->sp.size();j++){
//		X[j][0] = this->sp[j]->population;
		X[j][0] = this->x_curr[j];
		this->X_eff[j] = this->bt.beta[0]*X[j][0];
	}
	// Initial rates
	double a[this->rxn.size()][nStages];
	for (unsigned int v=0;v < this->rxn.size();v++){
//		a[v][0] = this->rxn[v]->getRate();
		a[v][0] = this->a_curr[v];
//		this->a_eff[v] = this->bt.beta[0]*a[v][0];
	}
	// Loop over stages
	for (unsigned int s=1;s < nStages;s++){
		// Populations
		for (unsigned int j=0;j < this->sp.size();j++){
			X[j][s] = 0.0;
			for (unsigned int ss=0;ss < s;ss++){
				if (fabs(this->bt.alpha[s][ss]) > TOL){ // Skip the sum if alpha[s][ss] is zero.
					double m_j = 0.0;
					for (unsigned int v=0;v < this->spInRxn[j].size();v++){
						m_j += this->stoich[j][v]*a[this->spInRxn[j][v]][ss];
					}
					X[j][s] += this->bt.alpha[s][ss]*m_j;
				}
			}
			X[j][s] *= tau;
			X[j][s] += X[j][0];
//			X[j][s] += this->sp[j]->population;
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
//			this->a_eff[v] += this->bt.beta[s]*a[v][s];
		}
	}
	// Calculate a_eff[v] = getRate(X_eff)
	vector<double> xEff;
	for (unsigned int v=0;v < this->rxn.size();v++){
		xEff.resize(this->rxn[v]->rateSpecies.size());
		for (unsigned int j=0;j < this->rxn[v]->rateSpecies.size();j++){
			xEff[j] = this->X_eff[this->rateSp[v][j]];
		}
		this->a_eff[v] = this->rxn[v]->re->getRate(xEff);
	}
/*
	//////////////
	cout << endl;
	cout << "tau: " << tau << endl;
	cout << endl;
	for (unsigned int j=0;j < this->sp.size();j++){
		cout << "x_" << j << ": " << this->sp[j]->population << endl;
	}
	cout << endl;
	for (unsigned int v=0;v < this->rxn.size();v++){
		cout << "a_" << v << ": " << this->rxn[v]->getRate() << endl;
	}
	cout << endl;
	if (this->X_eff.size() != this->sp.size()){
		cout << "X_eff.size != sp.size. Exiting." << endl; exit(1);
	}
	if (this->a_eff.size() != this->rxn.size()){
		cout << "a_eff.size != rxn.size. Exiting." << endl; exit(1);
	}
	for (unsigned int j=0;j < this->X_eff.size();j++){
		cout << "xEff_" << j << ": " << this->X_eff[j] << endl;
	}
	cout << endl;
	for (unsigned int v=0;v < this->a_eff.size();v++){
		cout << "aEff_" << v << ": " << this->a_eff[v] << endl;
	}
	cout << endl;
//	vector<double> xEff;
	for (unsigned int v=0;v < this->rxn.size();v++){
		xEff.resize(this->rxn[v]->rateSpecies.size());
		for (unsigned int j=0;j < this->rxn[v]->rateSpecies.size();j++){
			xEff[j] = this->X_eff[this->rateSp[v][j]];
		}
		cout << "a(xEff)_" << v << ": " << this->rxn[v]->re->getRate(xEff) << endl;
	}
	exit(1);
	cout << endl;
	//////////////
//*/
}

void aEff_Calculator::update(){
	for (unsigned int j=0;j < this->sp.size();j++){
		x_curr.at(j) = this->sp[j]->population;
	}
	for (unsigned int v=0;v < this->rxn.size();v++){
		a_curr.at(v) = this->rxn[v]->getRate();
	}
	// Error check
	if (this->x_curr.size() != this->sp.size()){
		cout << "Error in aEff_Calculator::update(): Sizes of 'x_curr' and 'sp' vectors not equal. "
			 << "Shouldn't happen. Exiting." << endl;
		exit(1);
	}
	if (this->a_curr.size() != this->rxn.size()){
		cout << "Error in aEff_Calculator::update(): Sizes of 'a_curr' and 'rxn' vectors not equal. "
			 << "Shouldn't happen. Exiting." << endl;
		exit(1);
	}
}

void aEff_Calculator::addRxn(){
	// Error check
	if (this->a_eff.size() != this->a_curr.size()){
		cout << "Error in aEff_Calculator::addRxn(): Sizes of 'a_eff' and 'a_curr' vectors not equal. "
			 << "Shouldn't happen. Exiting." << endl;
		exit(1);
	}
	// Continue
	if (this->a_eff.size() < this->rxn.size()){
		unsigned int u = this->a_eff.size();
		// Add new elements to a_eff[] and a_curr[]
		this->a_eff.push_back(0.0);
		this->a_curr.push_back(this->rxn[u]->getRate());
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
		cout << "Error in aEff_Calculator::addRxn(): No rxns to add (a_eff.size = " << this->a_eff.size()
			 << ", a_curr.size = " << this->a_curr.size() << ", rxn.size = " << this->rxn.size() << ")." << endl;
		cout << "Shouldn't happen. Exiting." << endl;
		exit(1);
	}
}

void aEff_Calculator::addSpecies(){
	// Error check
	if (this->X_eff.size() != this->x_curr.size()){
		cout << "Error in aEff_Calculator::addRxn(): Sizes of 'X_eff' and 'x_curr' vectors not equal. "
			 << "Shouldn't happen. Exiting." << endl;
		exit(1);
	}
	// Continue
	if (this->X_eff.size() < this->sp.size()){
		unsigned int i = this->X_eff.size();
		// Add new elements to X_eff[], x_curr[], spInRxn[] and stoich[]
		this->X_eff.push_back(0.0);
		this->x_curr.push_back(this->sp[i]->population);
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
