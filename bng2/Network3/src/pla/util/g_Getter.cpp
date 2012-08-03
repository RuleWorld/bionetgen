/*
 * g_Getter.cpp
 *
 *  Created on: Apr 26, 2011
 *      Author: Leonard Harris
 */

#include "g_Getter.hh"
/*
g_Getter::g_Getter() : sp(), rxn(){
	if (MoMMA::debug)
		cout << "g_Getter constructor called." << endl;
}
*/
g_Getter::g_Getter(vector<SimpleSpecies*>& sp, vector<Reaction*>& rxn) : nRxns(0), sp(sp), rxn(rxn){
	if (debug)
		cout << "g_Getter constructor called." << endl;
	// Add rxns
	for (unsigned int v=0;v < this->rxn.size();v++){
		this->addRxn();
	}
	// Add species
	for (unsigned int j=0;j < this->sp.size();j++){
		this->addSpecies();
	}
/*
for (unsigned int i=0;i < this->g.size();i++){
	cout << "g[" << i << "][0]: " << this->g[i][0];
	if (this->g[i].size() > 1){
		for (unsigned int j=1;j < this->g[i].size();j++){
			cout << " ; g[" << i << "][" << j << "]: " << this->g[i][j];
		}
	}
	cout << endl;
}
//*/
	// Set elements of g[]
/*	this->g.resize(this->sp.size());
	double var_gi;
	for (unsigned int i=0;i < this->sp.size();i++){
		this->g[i].push_back(this->get_const_g(i)); // Constant part of g_i only needs to be determined once
		var_gi = this->get_var_g(i); // If S_i in non-standard rxn type, size of g[i] will be increased to 2 in get_var_g()
		if (this->g[i].size() == 2){
			this->g[i][1] = var_gi;
		}
*/
/*		cout << "[" << i << "]:";
		for (unsigned int j=0;j < this->g[i].size();j++){
			cout << "\t" << this->g[i][j];
		}
		cout << endl;
*/
//	}
}

g_Getter::g_Getter(const g_Getter& gGet) : g(gGet.g), nRxns(gGet.nRxns), sp(gGet.sp), rxn(gGet.rxn){
	if (debug)
		cout << "g_Getter copy constructor called." << endl;
}

g_Getter::~g_Getter(){
	if (debug)
		cout << "g_Getter destructor called." << endl;
}

double g_Getter::get_g(unsigned int i){
	// Check for new rxns
	while (this->rxn.size() != this->nRxns){
		this->addRxn();
	}
	// Check for new species
	while (this->sp.size() != this->g.size()){
		this->addSpecies();
	}
	// Get g[i]
	if (this->g[i].size() == 1){
		return this->g[i][0];
	}
	else if (this->g[i].size() == 2){
		this->g[i][1] = this->get_var_g(i);
		return max(this->g[i][0],this->g[i][1]);
	}
	else{
		cout << "Error in g_Getter::get_g(): Vector g[" << i << "] has a length of " << this->g[i].size() << ". Exiting.\n";
		exit(1);
	}
}

double g_Getter::get_const_g(unsigned int i){
	double g_const_i = 0.0; // Constant part of g_i
	// Loop over reactions
	for (unsigned int v=0;v < this->nRxns;v++){
		g_const_i = max(g_const_i,this->get_const_g(v,i));
	}
	return g_const_i;
}

double g_Getter::get_const_g(unsigned int u, unsigned int i){
	double g_ui = 0.0; // Initialize to zero
	SimpleSpecies* S_i = this->sp[i];
	Reaction* R_u = this->rxn[u];
	// Loop over reactant species in reaction R_u
	bool found = false;
	for (unsigned int j=0;j < R_u->rateSpecies.size() && !found;j++){
//cout << "S_" << i << ": " << S_i->name << "; R_" << u << "->rateSpecies[" << j << "]: " << R_u->rateSpecies[j]->name << endl;
		if (S_i == R_u->rateSpecies[j]){ // Species S_i is a reactant in reaction R_u
/*cout << "MATCH" << endl;
cout << R_u->toString() << endl;*/
			found = true;
			// Constant cases
			if (R_u->type.find("ELEMENTARY:UNIMOLECULAR") != string::npos){
				g_ui = 1.0;
			}
			else if (R_u->type.find("ELEMENTARY:BIMOLECULAR_AA") != string::npos){
				g_ui = 3.0;
			}
			else if (R_u->type.find("ELEMENTARY:BIMOLECULAR_AB") != string::npos){
				g_ui = 2.0;
			}
			else if (R_u->type.find("ELEMENTARY:TRIMOLECULAR_AAA") != string::npos){
				g_ui = 5.5;
			}
			else if (R_u->type.find("ELEMENTARY:TRIMOLECULAR_AAB") != string::npos){
				if (j == 0){	  // 2S_i + S_j -> products
					g_ui = 4.5;
				}
				else if (j == 1){ // S_i + 2S_j -> products
					g_ui = 3.0;
				}
				else{ // Something wrong, there are only two reactant species
					cout << "Error in g_Getter::get_const_g(u,i): Species " << S_i->name << " identified as a ";
					cout << "reactant in elementary reaction " << R_u->toString() << ". Exiting." << endl;
					exit(1);
				}
			}
			else if (R_u->type.find("ELEMENTARY:TRIMOLECULAR_ABC") != string::npos){
				g_ui = 3.0;
			}
/*				else if (R_u->type == "SATURATION"){
				if (R_u->rateSpecies.size() == 1){
					g_ui = 1.0;
//					cout << S_i->name << ", " << R_u->toString() << ", 1st-order" << endl;
				}
				if (R_v->rateSpecies.size() == 2){
					g_ui = 2.0;
//					cout << S_i->name << ", " << R_u->toString() << ", 2nd-order" << endl;
				}
			}
*/
			// else if (R_u->type == "..."){
			// ...
		}
	}
	return g_ui;
}

double g_Getter::get_var_g(unsigned int i){
	double g_var_i = 0.0; // Variable part of g_i
	// Loop over reactions
	for (unsigned int v=0;v < this->nRxns;v++){
		g_var_i = max(g_var_i,this->get_var_g(v,i));
	}
	return g_var_i;
}

double g_Getter::get_var_g(unsigned int u, unsigned int i){
	double g_ui = 0.0; // Initialize to zero
	SimpleSpecies* S_i = this->sp[i];
	Reaction* R_u = this->rxn[u];
	// Loop over reactant species in reaction R_u
	bool found = false;
	for (unsigned int j=0;j < R_u->rateSpecies.size() && !found;j++){
		if (S_i == R_u->rateSpecies[j]){ // Species S_i is a reactant in reaction R_v
			found = true;
			// Skip over known reaction types
			if (R_u->type.find("ELEMENTARY:SYNTHESIS")        == string::npos &&
				R_u->type.find("ELEMENTARY:UNIMOLECULAR")     == string::npos &&
				R_u->type.find("ELEMENTARY:BIMOLECULAR_AA")   == string::npos &&
				R_u->type.find("ELEMENTARY:BIMOLECULAR_AB")   == string::npos &&
				R_u->type.find("ELEMENTARY:TRIMOLECULAR_AAA") == string::npos &&
				R_u->type.find("ELEMENTARY:TRIMOLECULAR_AAB") == string::npos &&
				R_u->type.find("ELEMENTARY:TRIMOLECULAR_ABC") == string::npos //&&
//				R_u->type != "SATURATION"					//&&
// 				R_u->type != "..."
				)
			{
				if (this->g[i].size() == 1) this->g[i].push_back(0.0);
				double dau_dXi = R_u->get_dRate_dX(j);
				double X_i = S_i->population;
				double a_u = R_u->getRate();
				double zeta_ui = static_cast<double>(R_u->rateSpecies.size()); // Give equal weights to each term
				double alpha_ui;
				if (dau_dXi*X_i < network3::TOL) alpha_ui = 0.0;
				else alpha_ui = dau_dXi*X_i/a_u;
				g_ui = zeta_ui*alpha_ui;
				//
				// Example: S1 + S2 -> products; a_u = c*X1*X2
				// Delta_a_u/a_u = (dau_dX1*X1/a_u)*[Delta_X1/X1] + (dau_dX2*X2/a_u)*[Delta_X2/X2] = eps
				// *** g_ui = zeta_ui*alpha_ui ***
				// zeta_u1 = zeta_u2 = 2
				// alpha_u1 = dau_dX1*X1/a_u = (c*X2)*X1/(c*X1*X2) = 1
				// alpha_u2 = dau_dX2*X2/a_u = (c*X1)*X2/(c*X1*X2) = 1
				// g_u1 = g_u2 = 2
/*
				cout << "-----" << endl;
				cout << "Species " << i << ": " << S_i->name << ", Rxn " << u << ": " << R_u->toString() << endl;
				cout << "dau_dXi = " << dau_dXi << ", X_i = " << X_i << ", a_u = " << a_u << ", zeta_ui = " << zeta_ui
					 << ", alpha_ui = " << alpha_ui << ", g_ui = " << g_ui << endl;
//*/
			}
		}
	}
	return g_ui;
}

void g_Getter::addRxn(){
	if (this->nRxns < this->rxn.size()){
		unsigned int u = this->nRxns;
		// Loop over species
		double var_g_uj;
		for (unsigned int j=0;j < this->g.size();j++){ // g.size(), not sp.size(), in case species have been added
			this->g[j][0] = max(this->g[j][0],this->get_const_g(u,j)); // Constant part
			var_g_uj = this->get_var_g(u,j);
			if (this->g[j].size() == 2){
				this->g[j][1] = max(this->g[j][1],var_g_uj);
			}
		}
		// Increase nRxns
		this->nRxns++;
	}
	else{
		cout << "Error in g_Getter::addRxn(): No rxns to add (nRxns = " << this->nRxns << ", rxn.size = " << this->rxn.size()
			 << "). Shouldn't happen. Exiting." << endl;
		exit(1);
	}
}

void g_Getter::addSpecies(){
	if (this->g.size() < this->sp.size()){
		unsigned int i = this->g.size();
		this->g.push_back(vector<double>());
		double var_gi;
		this->g[i].push_back(this->get_const_g(i)); // Constant part of g_i only needs to be determined once
		var_gi = this->get_var_g(i); // If S_i in non-standard rxn type, size of g[i] will be increased to 2 in get_var_g()
		if (this->g[i].size() == 2){
			this->g[i][1] = var_gi;
		}
	}
	else{
		cout << "Error in g_Getter::addSpecies(): No species to add (g.size = " << this->g.size() << ", sp.size = "
			 << this->sp.size() << "). Shouldn't happen. Exiting." << endl;
		exit(1);
	}
}
