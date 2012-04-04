/*
 * fEulerPreleapRB_TC.cpp
 *
 *  Created on: Feb 7, 2011
 *      Author: Leonard Harris
 */

#include "fEuler.hh"
/*
fEulerPreleapRB_TC::fEulerPreleapRB_TC() : rxn(){
	if (MoMMA::debug)
		cout << "fEulerPreleapRB_TC constructor called." << endl;
}
*/
fEulerPreleapRB_TC::fEulerPreleapRB_TC(double eps, vector<Reaction*>& rxn) : Preleap_TC(eps), rxn(rxn){
	if (debug)
		cout << "fEulerPreleapRB_TC constructor called." << endl;
	// Error check
	if (eps < 0.0 || eps > 1.0){
		cout << "Error in fEulerPreleapRB_TC constructor: ";
		cout << "epsilon must be >= 0.0 and <= 1.0; your eps = " << this->eps << endl;
		exit(1);
	}
}

fEulerPreleapRB_TC::fEulerPreleapRB_TC(const fEulerPreleapRB_TC& tc) : Preleap_TC(tc), rxn(tc.rxn){
	if (debug)
		cout << "fEulerPreleapRB_TC copy constructor called.\n";
}

fEulerPreleapRB_TC::~fEulerPreleapRB_TC(){
	if (debug)
		cout << "fEulerPreleapRB_TC destructor called." << endl;
}

void fEulerPreleapRB_TC::getNewTau(double& tau){

	// *** RB TAU-SELECTION FORMULA ***
	// tauLeap = min{tau[v]}
	// tau[u] = min{tau_m,tau_sig}
	// tau_m = (eps_u/2) / |m_u|
	// tau_sig = (eps_u^2/4) / sig_u^2
	// m_u = SUM_v {a_v * f_uv}
	// sig_u^2 = SUM_v {a_v * f_uv^2}
	// f_uv = SUM_j {z_vj * da_u/dX_j}
	// eps_u = max{eps*a_u,beta_u}
	// beta_u = min{da_u/dX_j}>0 or a_u^MIN

//	double tauLeap; // Return value
	double beta_u, eps_u, m_u, sig_u2, tau_m, tau_sig;
	vector<double> dau_dX;
	double tau_[this->rxn.size()];  // One for each reaction
	double f_u[this->rxn.size()];  // One for each reaction at given u

	// Collect rates for all reactions at the outset
	double rate[this->rxn.size()];
	for (unsigned int v=0;v < this->rxn.size();v++){
		rate[v] = this->rxn[v]->getRate();
	}

	// Loop over reactions calculating values of tau[u]
	for (unsigned int u=0;u < this->rxn.size();u++){

		// Calculate derivatives dau/dXj
		dau_dX.clear(); // Make sure it's empty
		for (unsigned int j=0;j < this->rxn[u]->rateSpecies.size();j++){
			dau_dX.push_back(this->rxn[u]->get_dRate_dX(j));
		}

		// Get beta_u
		beta_u = 0.0;
		for (unsigned int j=0;j < dau_dX.size();j++){
//			if (dau_dX[j] != 0.0 && (beta_u == 0.0 || dau_dX[j] < beta_u)){
			if (fabs(dau_dX[j]) > network3::TOL && (beta_u < network3::TOL || fabs(dau_dX[j]) < beta_u)){
				beta_u = fabs(dau_dX[j]);
			}
		}
//		if (dau_dX.size() != 0 && beta_u == 0.0){
		if (dau_dX.size() != 0 && beta_u < network3::TOL){
			vector<double> X;
			for (unsigned int j=0;j < this->rxn[u]->rateSpecies.size();j++){
				X.push_back(1.0); // a_u^MIN is rate when all reactant pops = 1
			}
			beta_u = this->rxn[u]->re->getRate(X);
		}

		// Get eps_u
		eps_u = max(this->eps*rate[u],beta_u);

		// Calculate elements of f_u[v]
		for (unsigned int v=0;v < this->rxn.size();v++){
			f_u[v] = 0.0;
			SimpleSpecies* rateSp;
			map<SimpleSpecies*,int>::iterator stoichSp;
			// Loop over reactant species in reaction R_u
			for (unsigned int j=0;j < this->rxn[u]->rateSpecies.size();j++){
				rateSp = this->rxn[u]->rateSpecies[j];
				stoichSp = this->rxn[v]->stoichSpecies.find(rateSp);
				if (stoichSp != this->rxn[v]->stoichSpecies.end()){ // Species j is created/consumed in reaction R_v
					f_u[v] += dau_dX[j] * static_cast<double>((*stoichSp).second);
				}
			}
		}

		// Calculate m_u and sig_u2
		m_u = 0.0;
		sig_u2 = 0.0;
		for (unsigned int v=0;v < this->rxn.size();v++){
			m_u += f_u[v]*rate[v];
			sig_u2 += f_u[v]*f_u[v]*rate[v];
		}

		// Calculate tau_m and tau_sig
		tau_m = 0.5*eps_u/fabs(m_u);
		tau_sig = 0.25*eps_u*eps_u/sig_u2;

		// Set tau[u] = min{tau_m,tau_sig}
		tau_[u] = min(tau_m,tau_sig);
//		tau_[u] = tau_m; // ORIGINAL TAU-SELECTION METHOD
	}
	// Find min{tau[v]}
	tau = INFINITY;
	for (unsigned int v=0;v < this->rxn.size();v++){
//		cout << "tau[" << v << "] = " << tau_[v] << endl;
		if (tau_[v] < tau){
			tau = tau_[v];
		}
	}
//	return tauLeap;
}
