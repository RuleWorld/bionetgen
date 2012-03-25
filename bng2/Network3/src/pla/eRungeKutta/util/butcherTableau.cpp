/*
 * butcherTableau.cpp
 *
 *  Created on: Mar 22, 2011
 *      Author: Leonard Harris
 */

#include "butcherTableau.hh"
/*
ButcherTableau::ButcherTableau() : s(0){
	if (debug)
		cout << "ButcherTableau constructor called." << endl;
}
*/
ButcherTableau::ButcherTableau(vector<vector<double> > alpha, vector<double> beta) : alpha(alpha), beta(beta),
	uint_size(alpha.size()){
	if (debug)
		cout << "ButcherTableau constructor called." << endl;
	// Error check
	if (alpha.size() == 0){
		/*cerr*/cout << "Error in ButcherTableau constructor: Matrix 'alpha' has no elements. Exiting." << endl;
		exit(1);
	}
	if (beta.size() == 0){
		/*cerr*/cout << "Error in ButcherTableau constructor: Vector 'beta' has no elements. Exiting." << endl;
		exit(1);
	}
	if (alpha.size() != beta.size()){
		/*cerr*/cout << "Error in ButcherTableau constructor: Matrix 'alpha' and vector 'beta' must be of equal lengths. Exiting." << endl;
		exit(1);
	}
	for (unsigned int i=0;i < alpha.size();i++){
		if (alpha[i].size() != alpha.size()){
			/*cerr*/cout << "Error in ButcherTableau constructor: Matrix 'alpha' must be square. Exiting." << endl;
			exit(1);
		}
	}
	double sum = 0.0;
	for (unsigned int i=0;i < beta.size();i++){
		sum += beta[i];
	}
	if (sum > 1.0 + TOL || sum < 1.0 - TOL){
		/*cerr*/cout << "Error in ButcherTableau constructor: Sum of elements of 'beta' must equal 1.0. Exiting." << endl;
		exit(1);
	}
	// Calculate elements of gamma[]: gamma_i = sum_j alpha_ij
	this->gamma.resize(this->size(),0.0);
	for (unsigned int i=0;i < this->gamma.size();i++){
		for (unsigned int j=0;j < this->alpha[i].size();j++){
			this->gamma[i] += this->alpha[i][j];
		}
	}
	// Error check
	for (unsigned int i=0;i < this->gamma.size();i++){
		if (this->gamma[i] < -TOL || this->gamma[i] > 1.0 + TOL){
			/*cerr*/cout << "Error in ButcherTableau constructor: All elements of 'gamma' vector must be >= 0.0 and <= 1.0. Exiting.\n";
			exit(1);
		}
	}
	// Explicit or implicit?
	this->is_explicit = true;
	for (unsigned int i=0;i < this->size() && this->is_explicit;i++){
		for (unsigned int j=i;j < this->size() && this->is_explicit;j++){
			if (fabs(this->alpha[i][j]) > TOL){
				this->is_explicit = false;
			}
		}
	}
	//
//	if (this->explicit_) cout << "Explicit Butcher tableau" << endl;
//	else cout << "Implicit Butcher tableau" << endl;
}

string ButcherTableau::toString(){
	string out;
	//
	for (unsigned int i=0;i < this->size(); i++){
		out += Util::toString(this->gamma[i]) + "\t|";
		for (unsigned int j=0;j < this->size();j++){
			out += "\t" + Util::toString(this->alpha[i][j]);
		}
		out += "\n";
	}
	out += "-----\t-----";
	for (unsigned int i=0;i < this->size();i++){
		out += "\t-----";
	}
	out += "\n";
	out += "\t|";
	for (unsigned int i=0;i < this->beta.size();i++){
		out += "\t" + Util::toString(this->beta[i]);
	}
	//
	return out;
}
