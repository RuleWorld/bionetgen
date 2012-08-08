/*
 * fEulerPreleapSB_TC.cpp
 *
 *  Created on: Feb 7, 2011
 *      Author: Leonard Harris
 */

#include "fEuler.hh"
/*
fEulerPreleapSB_TC::fEulerPreleapSB_TC() : sp(), rxn(){
	if (MoMMA::debug)
		cout << "fEulerPreleapSB_TC constructor called." << endl;
}
*/
fEulerPreleapSB_TC::fEulerPreleapSB_TC(double eps, vector<SimpleSpecies*>& sp, vector<Reaction*>& rxn) : Preleap_TC(eps), sp(sp), rxn(rxn){
	if (debug)
		cout << "fEulerPreleapSB_TC constructor called." << endl;
	// Error check
	if (eps < 0.0 || eps > 1.0){
		cout << "Error in fEulerPreleapSB_TC constructor: ";
		cout << "epsilon must be >= 0.0 and <= 1.0; your eps = " << this->eps << endl;
		exit(1);
	}
	// Create g_Getter
	this->gGet = new g_Getter(this->sp,this->rxn);
}

fEulerPreleapSB_TC::fEulerPreleapSB_TC(const fEulerPreleapSB_TC& tc) : Preleap_TC(tc), sp(tc.sp), rxn(tc.rxn){
	if (debug)
		cout << "fEulerPreleapSB_TC copy constructor called.\n";
	this->gGet = new g_Getter(this->sp,this->rxn);
}

fEulerPreleapSB_TC::~fEulerPreleapSB_TC(){
	if (debug)
		cout << "fEulerPreleapSB_TC destructor called." << endl;
	delete this->gGet;
}

void fEulerPreleapSB_TC::getNewTau(double& tau){

	// *** SB TAU-SELECTION FORMULA ***
	// tau = min{tee[j]}
	// tee[i] = min{tee_m,tee_sig}
	// tee_m = (e_i/2) / |m_i|
	// tee_sig = (e_i^2/4) / sig_i2
	// m_i = SUM_v {a_v * z_vi}
	// sig_i2 = SUM_v {a_v * z_vi^2}
	// e_i = max{eps*X_i/g_i,1}

	// Make sure g[i] values have been calculated
	if (this->gGet->size() != this->sp.size()){
		cout << "Error in fEulerPreleapSB_TC::getNewTau(): Elements of g[] not initialized. Exiting." << endl;
		exit(1);
	}

//	double tauLeap; // Return value
	double e_i, m_i, sig_i2, tee_m, tee_sig;
	double tee[this->sp.size()];  // One for each species

	// Collect rates for all reactions at the outset
	double rate[this->rxn.size()];
	for (unsigned int v=0;v < this->rxn.size();v++){
		rate[v] = this->rxn[v]->getRate();
	}

	// Loop over species calculating values of tee[i]
	for (unsigned int i=0;i < this->sp.size();i++){

		// Get e_i
		if (this->sp[i]->population < network3::TOL) e_i = 1.0;
		else e_i = max(this->eps*this->sp[i]->population/this->gGet->get_g(i),1.0);
/*
cout << "g_" << i << ": " << this->gGet->get_g(i) << endl;
cout << "e_" << i << ": " << e_i << endl;
//*/
		// Calculate m_i and sig_i2
		m_i = 0.0;
		sig_i2 = 0.0;
		double z_vi;
		map<SimpleSpecies*,int>::iterator stoichSp;
		for (unsigned int v=0;v < this->rxn.size();v++){
			stoichSp = this->rxn[v]->stoichSpecies.find(this->sp[i]);
			if (stoichSp != this->rxn[v]->stoichSpecies.end()){ // Species i created/consumed in reaction R_v
				z_vi = static_cast<double>((*stoichSp).second);
				m_i += z_vi*rate[v];
				sig_i2 += z_vi*z_vi*rate[v];
			}
		}
/*
cout << "m_" << i << ": " << m_i << endl;
cout << "sig2_" << i << ": " << sig_i2 << endl;
//*/
		// Calculate tee_m and tee_sig
		tee_m = 0.5*e_i/fabs(m_i);
		tee_sig = 0.25*e_i*e_i/sig_i2;
//		tee_m = e_i/fabs(m_i);
//		tee_sig = e_i*e_i/sig_i2;
/*
cout << "tee_m_" << i << ": " << tee_m << endl;
cout << "tee_sig_" << i << ": " << tee_sig << endl;
//*/
		// Set tee[i] = min{tee_m,tee_sig}
		tee[i] = min(tee_m,tee_sig);
//cout << "tee[" << i << "]: " << tee[i] << endl;
	}

	// Find min{tee[j]}
	tau = INFINITY;
	for (unsigned int j=0;j < this->sp.size();j++){
		if (tee[j] < tau){
			tau = tee[j];
		}
	}

	// Error check
	if (tau == INFINITY){
		cout << "Error in fEulerPreleapSB_TC::getNewTau(): tau = INFINITY. Shouldn't happen. Exiting." << endl;
		exit(1);
	}
}
