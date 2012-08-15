/*
 * PLA.cpp
 *
 *  Created on: Feb 4, 2011
 *      Author: Leonard Harris
 */

#include "PLA.hh"

using namespace std;
/*
PLA::PLA(){
	if (debug)
		cout << "PLA constructor called." << endl;
}
*/
PLA::PLA(TauCalculator& tc, RxnClassifier& rc, FiringGenerator& fg, PostleapChecker& pl, vector<SimpleSpecies*>& sp,
		vector<Reaction*>& rxn) : tau(0.0), tc(tc), rc(rc), fg(fg), pl(pl), sp(sp), rxn(rxn), ES_rxn(NULL), fire_ES(false),
		output_interval(INFINITY){
	if (debug)
		cout << "PLA constructor called." << endl;
	this->k.resize(this->rxn.size(),NAN);
	this->classif.resize(this->rxn.size(),-1);
}

PLA::~PLA(){
	if (debug)
		cout << "PLA destructor called." << endl;
	for (unsigned int i=0;i < this->output.size();i++){
		delete this->output[i];
	}
}

void PLA::addOutputFile(string filePath){
	this->output.push_back(new ofstream(filePath.c_str()));
	//
	// Initialize output file
	*this->output[this->output.size()-1] << "time\t" << "step";
	//
	if (this->output.size() == 1){ // Populations
		for (unsigned int i=0;i < this->sp.size();i++){
			*this->output[0] << "\t" << this->sp[i]->name;
		}
	}
	//
	if (this->output.size() == 2){ // Rates
		for (unsigned int v=0;v < this->rxn.size();v++){
			*this->output[1] << "\t" << "Rate_" << v;
		}
	}
	//
	if (this->output.size() == 3){ // tau's and classifications
		*this->output[2] << "\t" << "tau";
		for (unsigned int v=0;v < this->rxn.size();v++){
			*this->output[2] << "\t" << "Classif_" << v;
		}
	}
	//
	// ...
	//
	*this->output[this->output.size()-1] << endl;
}
/*
void PLA::initialize(){

}
*/
double PLA::run(double tStart, double tEnd){

	// Error check
	if (tEnd < tStart){
		cout << "Error in PLA::run(): Simulation time cannot be negative. Exiting." << endl;
		exit(1);
	}

	double time = tStart;
	int step = 0;

	double nextOutput = tStart + this->output_interval;
	bool lastOut = true;

	// Initial output to file
	this->outputToFile(time,step);
	// Output to stdout
	if (this->output_interval != INFINITY){
		cout << "time" << "\t" << "step";
		for (unsigned int i=0;i < this->sp.size();i++){
			cout << "\t" << this->sp[i]->name;
		}
		cout << endl;
		cout << time << "\t" << step;
		for (unsigned int i=0;i < this->sp.size();i++){
			cout << "\t" << this->sp[i]->population;
		}
		cout << endl;
	}

	// Simulation loop
	while (time < tEnd && this->tau < INFINITY){

		// Next step
		step++;
		this->nextStep();
		time += this->tau;

		// Output
		lastOut = false;
		if (time >= nextOutput){
			nextOutput += this->output_interval;
			lastOut = true;
			// Output to file
			this->outputToFile(time,step);
			// Output to stdout
			cout << time << "\t" << step;
			for (unsigned int i=0;i < this->sp.size();i++){
				cout << "\t" << this->sp[i]->population;
			}
			cout << endl;
		}
	}

	// Final output
	if (!lastOut){
		// Output to file
		this->outputToFile(time,step);
		// Output to stdout
		if (this->output_interval != INFINITY){
			cout << time << "\t" << step;
			for (unsigned int i=0;i < this->sp.size();i++){
				cout << "\t" << this->sp[i]->population;
			}
			cout << endl;
		}
	}

	return (time-tStart);
}

double PLA::run(double tStart, int nSteps){

	// Error check
	if (nSteps < 0){
		cout << "Error in PLA::run(): Number of simulation steps cannot be negative. Exiting." << endl;
		exit(1);
	}

	double time = tStart;
	int step = 0;

	double nextOutput = tStart + this->output_interval;
	bool lastOut = true;

	// Initial output to file
	this->outputToFile(time,step);
	// Output to stdout
	if (this->output_interval != INFINITY){
		cout << "time" << "\t" << "step";
		for (unsigned int i=0;i < this->sp.size();i++){
			cout << "\t" << this->sp[i]->name;
		}
		cout << endl;
		cout << time << "\t" << step;
		for (unsigned int i=0;i < this->sp.size();i++){
			cout << "\t" << this->sp[i]->population;
		}
		cout << endl;
	}

	// Simulation loop
	while (step < nSteps && this->tau < INFINITY){

		// Next step
		step++;
		this->nextStep();
		time += this->tau;

		// Output
		lastOut = false;
		if (time >= nextOutput){
			nextOutput += this->output_interval;
			lastOut = true;
			// Output to file
			this->outputToFile(time,step);
			// Output to stdout
			cout << time << "\t" << step;
			for (unsigned int i=0;i < this->sp.size();i++){
				cout << "\t" << this->sp[i]->population;
			}
			cout << endl;
		}
	}

	// Final output
	if (!lastOut){
		// Output to file
		this->outputToFile(time,step);
		// Output to stdout
		if (this->output_interval != INFINITY){
			cout << time << "\t" << step;
			for (unsigned int i=0;i < this->sp.size();i++){
				cout << "\t" << this->sp[i]->population;
			}
			cout << endl;
		}
	}

	return (time-tStart);
}

void PLA::nextStep(){

	// Step 1: Calculate initial tau
	this->tc.getNewTau(this->tau);

	// Step 2: Get initial reaction classifications
	this->rc.classifyRxns(this->classif,this->tau,true);

	// Initialize ES rxn variables
	this->ES_rxn = NULL; // Don't know if there are any ES rxns yet
	this->fire_ES = false;
	double tau_ES = INFINITY;
/*
	// If all rxns are ES, do a simple ES step
	bool allES = true;
	for (unsigned int v=0;v < this->classif.size() && allES;v++){
		if (this->classif[v] != RxnClassifier::EXACT_STOCHASTIC){
			allES = false;
		}
	}
	if (allES){
		this->perform_allES_step();
	}
	// Otherwise, do PLA step
	else{
*/
		// Step 3: Iterate until a consistent tau and set of classifications are found
		vector<bool> already_ES(this->rxn.size(),false); // Tracks whether a rxn has already been flagged as ES
		bool done = false;
		bool allES = false;
		//
		while (!done){
			done = true;
			allES = true;
			for (unsigned int v=0;v < this->classif.size();v++){

				// Consider ES rxns
				if (this->classif[v] == RxnClassifier::EXACT_STOCHASTIC){
					// Is it newly ES?
					if (!already_ES[v]){ // Yes
						already_ES[v] = true;
						double tau_ESv = this->get_tau_ES(v);
						if (tau_ESv < tau_ES){ // Remember that tau_ES = INFINITY initially
							this->ES_rxn = this->rxn[v];
							tau_ES = tau_ESv;
						}
					}
				}
				else{ // There are non-ES rxns present
					allES = false;
				}
			}

			// If tau_ES < tau or allES = true, set tau = tau_ES and fire_ES = true
			if (tau_ES < this->tau || allES){
				this->tau = tau_ES;
				this->fire_ES = true;
				// If allES = false, set done = false and reclassify all non-ES rxns
				if (!allES){
					done = false;
					// Reclassify non-ES rxns
					this->rc.classifyRxns(this->classif,this->tau,false);
				}
			}
		}

		// Step 4: Fire reactions
		// If all rxns are ES, just fire the one with min{tau_ES_v} and move on (no need to postleap check)
		if (allES){
//			cout << "allES" << endl;
			if (this->ES_rxn){ // Don't fire if all rxns are inactive (ES_rxn = NULL initially)
//				cout << "firing rxn " << this->ES_rxn->toString() << endl;
//				printf("rate: %26.18f\n",this->ES_rxn->getRate());
				this->ES_rxn->fire(1.0);
			}
//			cout << "All ES step" << endl;
		}
		// Otherwise...
		else{
			/*
			for (unsigned int i=0;i < this->sp.size();i++) cout << this->sp[i]->name << ": " << this->sp[i]->population << endl;
			cout << endl;*/
			// Fire all non-ES rxns
			this->fg.fireRxns(this->k,this->classif,this->tau);
			/*for (unsigned int i=0;i < this->k.size();i++) cout << "k[" << i << "]: " << this->k[i] << endl;
			cout << endl;
			for (unsigned int i=0;i < this->sp.size();i++) cout << this->sp[i]->name << ": " << this->sp[i]->population << endl;*/

			// Step 5: Perform postleap check (before firing ES rxn)
			if (!this->pl.check()){
				this->pl.correct(this->k, this->tau);
				this->fire_ES = false; // tau is reduced, so don't fire ES rxn anymore
			}

			// Now fire ES rxn if tau = tau_ES
//			if (this->tau == tau_ES){
			if (this->fire_ES){
				if (this->ES_rxn->getRate() > 0.0){ // Make sure rxn is still active (not always caught in PL check)
					this->ES_rxn->fire(1.0);
//					cout << "Firing ES rxn" << endl;
//					cout << "tau = " << tau << ", tau_ES = " << tau_ES << endl;
				}
				/*else{
					cout << "Warning: Rxn " << this->ES_rxn->toString() << " was scheduled to fire (once) but didn't ";
					cout << "due to rate becoming " << this->ES_rxn->getRate() << "." << endl;
				}*/
			}
		}
//	}
	// Update postleap checker
	this->pl.update();
}

double PLA::get_tau_ES(unsigned int v){
	double tau_ESv = this->get_tau_FRM(v);
	// Error check
	if (tau_ESv <= 0.0){
		cout << "Error in PLA::get_tau_ES(): tauES_" << v << " = " << tau_ESv << ". Shouldn't happen. Exiting." << endl;
		cout << this->rxn[v]->toString() << endl;
		for (unsigned int j=0;j < this->rxn[v]->rateSpecies.size();j++){
			cout << this->rxn[v]->rateSpecies[j]->name << ": " << this->rxn[v]->rateSpecies[j]->population << endl;
		}
		exit(1);
	}
	return tau_ESv;
}

double PLA::get_tau_FRM(unsigned int v){
	double rate_v = this->rxn[v]->getRate();
	if (rate_v == 0.0) return INFINITY; // Do this to prevent getting -INFINITY if rate_v = -0.0 (yes, negative zero)
	else{
		double r = Util::RANDOM_CLOSED();
		while (r == 0.0 || r == 1.0) r = Util::RANDOM_CLOSED();
		double tau = -log(r)/rate_v;
		// Error check
		if (tau <= 0.0){
			cout << "Error in PLA::get_tau_FRM(): tau_" << v << " = " << tau << ". Shouldn't happen. Exiting." << endl;
			cout << "(r = " << r << ", log(" << r << ") = " << log(r) << ", rate_" << v << " = " << rate_v << " )" << endl;
			exit(1);
		}
		return tau;
	}
}
/*
double PLA::get_tau_Gibson(unsigned int v){

}
*/
void PLA::perform_allES_step(){
	// Calculate sumRates
	double sumRates = 0.0;
	for (unsigned int v=0;v < this->rxn.size();v++){
		sumRates += this->rxn[v]->getRate();
	}
	// Get firing time
	this->tau = -log(Util::RANDOM_CLOSED())/sumRates;
	// Find which rxn to fire
	double r2_sumRates = Util::RANDOM_CLOSED()*sumRates;
	double partialSum = this->rxn[0]->getRate();
	unsigned int which = 0;
	while (partialSum < r2_sumRates){
		which++;
		partialSum += this->rxn[which]->getRate();
	}
	// Fire rxn
	this->rxn[which]->fire(1.0);
}

void PLA::outputToFile(double time, int step){
	for (unsigned int f=0;f < this->output.size();f++){
		if (this->output[f]->is_open()){
			*this->output[f] << time << "\t" << step;
			//
			if (f == 0){
				for (unsigned int j=0;j < this->sp.size();j++){
					*this->output[0] << "\t" << this->sp[j]->population;
				}
			}
			//
			if (f == 1){
				for (unsigned int v=0;v < this->rxn.size();v++){
					*this->output[1] << "\t" << this->rxn[v]->getRate();
				}
			}
			//
			if (f == 2){
				*this->output[2] << "\t" << this->tau;
				for (unsigned int v=0;v < this->rxn.size();v++){
					*this->output[2] << "\t" << this->classif[v];
				}
			}
			//
			*this->output[f] << endl;
		}
	}
}
