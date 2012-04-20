/*
 * PLA.hh
 *
 *  Created on: Feb 4, 2011
 *      Author: Leonard Harris
 */

#ifndef PLA_HH_
#define PLA_HH_

#include <fstream>
#include "../std_include.hh"
#include "../util/util.hh"
#include "../model/reaction.hh"
#include "base/tauCalculator.hh"
#include "base/rxnClassifier.hh"
#include "base/firingGenerator.hh"
#include "base/postleapChecker.hh"

namespace network3{

	class PLA{
	public:
		double tau;			 		// Timestep
		TauCalculator& tc;			// Tau calculator
		RxnClassifier& rc;			// Rxn classifier
		FiringGenerator& fg;		// Firing generator
		PostleapChecker& pl;		// Postleap checker
		vector<SimpleSpecies*> sp;	// Species
		vector<Reaction*> rxn;		// Reactions
		vector<double> k; 	 		// Rxn firings
		vector<int> classif; 		// Classifications

//		PLA();
		PLA(TauCalculator& tc, RxnClassifier& rc, FiringGenerator& fg, PostleapChecker& pl,
				vector<SimpleSpecies*>& sp, vector<Reaction*>& rxn);
		~PLA();

//		void addDependencyMap(map<Reaction*,vector<Reaction*> > dependency);
//		void addDependencyMap(vector<vector<Reaction*> > dependency);

		void addOutputFile(string directory, string filename){
			if ( directory.substr(directory.size()-1,directory.size()-1) != "/" ){ directory.append("/"); }
			this->addOutputFile((directory+filename));
		}
		void addOutputFile(string filePath);
		void setOutputInterval(double outInterval){ this->output_interval = outInterval; }
		void setSeed(unsigned long seed){ Util::SEED_RANDOM(seed); }
		void setSeed(long seed){ this->setSeed((unsigned long)seed); }
		void setSeed(int seed){ this->setSeed((unsigned long)seed); }

//		double calcSumRates();

//		virtual void initialize();
		double run(double t){ return this->run(0.0,t); }
		double run(double tStart, double tEnd);
		double run(int nSteps){ return this->run(0.0,nSteps); }
		double run(double tStart, int nSteps);
		void nextStep(); //{ return this->nextStep(INFINITY); }
//		double nextStep(double maxTau);

		double get_tau_ES(unsigned int v);
//		double get_tau_Gibson(unsigned int v);
		void perform_allES_step();

	protected:
//		map<Reaction*,vector<Reaction*> > dependency;
//		pair<Reaction*,double> ES_rxn; // <ES_rxn,min_tau_ES>
		//
		Reaction* ES_rxn;
		bool fire_ES;
//		double tau_ES;
		//
		double get_tau_FRM(unsigned int v);
//		double get_tau_Gibson(unsigned int v);
		//
		vector<ofstream*> output;
		double output_interval;
		void outputToFile(double time, int step);
	};
}

#endif /* PLA_HH_ */
