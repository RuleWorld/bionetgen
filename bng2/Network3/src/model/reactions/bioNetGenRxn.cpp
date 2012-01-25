/*
 * bioNetGenRxn.cpp
 *
 *  Created on: Jul 11, 2011
 *      Author: Leonard Harris
 */

#include "bioNetGenRxn.hh"
/*
BioNetGenRxn::BioNetGenRxn(){
	if (MoMMA::debug)
		cout << "BioNetGenRxn constructor called." << endl;
}
*/
BioNetGenRxn::BioNetGenRxn(vector<SimpleSpecies*> r, vector<int> rS, vector<SimpleSpecies*> p, vector<int> pS){
	if (debug)
		cout << "BioNetGenRxn constructor called." << endl;
	// Error check
	if (r.size() != rS.size()){
		cout << "Error in BioNetGenRxn constructor: 'r' and 'rS' vectors must be equal size. Exiting." << endl;
		exit(1);
	}
	if (p.size() != pS.size()){
		cout << "Error in BioNetGenRxn constructor: 'p' and 'pS' vectors must be equal size. Exiting." << endl;
		exit(1);
	}
	for (unsigned int i=0;i < rS.size();i++){
		if (rS[i] >= 0){
			cout << "Error in BioNetGenRxn constructor: Reactant " << r[i]->name << " has non-negative stoichiometry = " << rS[i]
				 << ". Exiting." << endl;
			exit(1);
		}
	}
	for (unsigned int i=0;i < pS.size();i++){
		if (pS[i] <= 0){
			cout << "Error in BioNetGenRxn constructor: Product " << p[i]->name << " has non-positive stoichiometry = " << pS[i]
			     << ". Exiting." << endl;
			exit(1);
		}
	}
	// Remove redundancies from reactants
	vector<pair<SimpleSpecies*,int> > reactants;
	vector<bool> skip; skip.resize(r.size(),false);
	for (unsigned int i=0;i < r.size();i++){
		if (!skip[i]){
			for (unsigned int j=i+1;j < r.size();j++){
				if (r[i] == r[j]){
					rS[i] += rS[j];
					skip[j] = true;
				}
			}
		}
	}
	for (unsigned int i=0;i < r.size();i++){
		if (!skip[i]){
			reactants.push_back( pair<SimpleSpecies*,int>(r[i],rS[i]) );
		}
	}
/*
	cout << "-----Reactants-----" << endl;
	for (unsigned int i=0;i != this->reactants.size();i++){
		cout << this->reactants[i].first->name << ": " << this->reactants[i].second << endl;
	}
//*/
	// Remove redundancies from products
	vector<pair<SimpleSpecies*,int> > products;
	skip.clear(); skip.resize(p.size(),false);
	for (unsigned int i=0;i < p.size();i++){
		if (!skip[i]){
			for (unsigned int j=i+1;j < p.size();j++){
				if (p[i] == p[j]){
					pS[i] += pS[j];
					skip[j] = true;
				}
			}
		}
	}
	for (unsigned int i=0;i < p.size();i++){
		if (!skip[i]){
			products.push_back( pair<SimpleSpecies*,int>(p[i],pS[i]) );
		}
	}
/*
	cout << "-----Products-----" << endl;
	for (unsigned int i = 0;i != this->products.size();i++){
		cout << this->products[i].first->name << ": " << this->products[i].second << endl;
	}
//*/
	// Set rateSpecies = reactant species and store reactant stoichiometries
	for (unsigned int i=0;i != reactants.size();i++){
		this->rateSpecies.push_back(reactants[i].first);
		this->rStoich.push_back(reactants[i].second);
	}
	//
/*	cout << "\t" << "------RateSpecies------" << endl;
	for (unsigned int i=0;i < this->rateSpecies.size();i++){
		cout << "\t" << this->rateSpecies[i]->name << ": " << this->rStoich[i] << endl;
	}
	//
*/	// Identify stoichSpecies
	vector<pair<SimpleSpecies*,int> > stoichTemp;
	skip.resize((reactants.size()+products.size()),false);
	for (unsigned int i=0;i != reactants.size();i++){
		stoichTemp.push_back(reactants[i]);
	}
	for (unsigned int i=0;i != products.size();i++){
		stoichTemp.push_back(products[i]);
	}
	for (unsigned int i=0;i < stoichTemp.size();i++){
		if (!skip[i]){
			for (unsigned int j=i+1;j < stoichTemp.size();j++){
				if (stoichTemp[i].first == stoichTemp[j].first){
					stoichTemp[i].second += stoichTemp[j].second;
					skip[j] = true;
				}
			}
		}
	}
	for (unsigned int i=0;i < stoichTemp.size();i++){
		if (stoichTemp[i].second != 0 && !skip[i]){
			this->stoichSpecies.insert( stoichTemp[i] );
		}
	}
	//
/*	cout << "\t" << "-----StoichSpecies-----" << endl;
	map<SimpleSpecies*,int>::iterator iter;
	for (iter = this->stoichSpecies.begin();iter != this->stoichSpecies.end();iter++){
		cout << "\t" << (*iter).first->name << ": " << (*iter).second << endl;
	}
	cout << "\t" << "-----------------------" << endl;
*/
}

BioNetGenRxn::~BioNetGenRxn(){
	if (debug)
		cout << "BioNetGenRxn destructor called." << endl;
}
