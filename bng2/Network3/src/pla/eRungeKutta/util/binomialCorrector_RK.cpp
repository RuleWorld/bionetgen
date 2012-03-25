/*
 * binomialCorrector_RK.cpp
 *
 *  Created on: Mar 22, 2012
 *      Author: Leonard Harris
 */

#include "binomialCorrector_RK.hh"
/*
BinomialCorrector_RK::BinomialCorrector_RK() : rxn(){
	if (MoMMA::debug)
		cout << "BinomialCorrector_RK constructor called." << endl;
}
*/
BinomialCorrector_RK::BinomialCorrector_RK(double p, vector<Reaction*>& rxn) : p(p), rxn(rxn){
	if (debug)
		cout << "BinomialCorrector_RK constructor called." << endl;
	// Error check
	if (this->p <= 0.0 || this->p >= 1.0){
		cout << "Error in BinomialCorrector_RK constructor: ";
		cout << "p must be > 0.0 and < 1.0; your p = " << this->p << ". Exiting. " << endl;
		exit(1);
	}
}

BinomialCorrector_RK::BinomialCorrector_RK(const BinomialCorrector_RK& bc) : p(bc.p), rxn(bc.rxn){
	if (debug)
		cout << "BinomialCorrector_RK copy constructor called." << endl;
}

BinomialCorrector_RK::~BinomialCorrector_RK(){
	if (debug)
		cout << "BinomialCorrector_RK destructor called." << endl;
}

void BinomialCorrector_RK::correct(vector<double>& k, aEff_Calculator* aCalc, double& tau){
//	cout << "Correcting." << endl;
	// Error check
	if (k.size() != this->rxn.size()){
		cout << "Error in BinomialCorrector_RK::correct(): Sizes of 'k' and 'rxn' vectors not equal. Exiting." << endl;
		exit(1);
	}
	// Correct
//	double tau_old = tau;
	tau *= this->p; // Reduce tau
	vector<double> aEff_old = aCalc->a_eff;
	aCalc->calc_aEff(tau); // Recalculate a_eff[]
	double k_old;
	bool stop = true;
	for (unsigned int v=0;v < this->rxn.size();v++){
		if (k[v] > 0.0){
			stop = false;
			k_old = k[v];
			// Reduce k[v]
			k[v] = Util::RANDOM_BINOMIAL(this->p*aCalc->a_eff[v]/aEff_old[v],floor(k_old + 0.5));
//			k[v] = Util::RANDOM_BINOMIAL(this->p,floor(k_old + 0.5));
			// Error check
			if (k[v] < 0.0){
				cout << "Error in BinomialCorrector_RK::correct(): Negative # of firings detected (k[" << v << "] = "
					 << k[v] << "). Shouldn't happen. Exiting." << endl;
				exit(1);
			}
			// Fire rxn k_new-k_old times (negative value)
			this->rxn[v]->fire(k[v]-k_old);
		}
	}
	if (stop){
		cout << "Error in BinomialCorrector_RK::correct(): All rxn firings <= 0.0. Shouldn't happen. Exiting." << endl;
		exit(1);
	}
/*/////////
	cout << "tau_new: " << tau << endl;
	vector<SimpleSpecies*> SP;
	for (unsigned int v=0;v < this->rxn.size();v++){
		for (unsigned int j=0;j < this->rxn[v]->rateSpecies.size();j++){
			bool already = false;
			for (unsigned int k=0;k < SP.size();k++){
				if (SP[k] == this->rxn[v]->rateSpecies[j]){
					already = true;
				}
			}
			if (!already){
				SP.push_back(this->rxn[v]->rateSpecies[j]);
				cout << this->rxn[v]->rateSpecies[j]->name << ": " << this->rxn[v]->rateSpecies[j]->population << endl;
			}
		}
	}
	for (unsigned int v=0;v < this->rxn.size();v++){
		cout << "a_" << v << ": " << this->rxn[v]->getRate() << endl;
	}
/////////*/
}
