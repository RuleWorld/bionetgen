/*
 * network3.hh
 *
 *  Created on: Jul 27, 2011
 *      Author: Leonard Harris
 */

#ifndef NETWORK3_HH_
#define NETWORK3_HH_

#include <climits>
#include "network.h"
#include "model/reactions/bioNetGenRxn.hh"
#include "model/observable.hh"
#include "pla/fEuler/fEuler.hh"
#include "pla/eRungeKutta/eRungeKutta.hh"
#include "pla/PLA.hh"

namespace network3{

	class Network3{
	public:
		static vector<SimpleSpecies*> SPECIES;
		static vector<pair<Observable*,double>*> OBSERVABLE;
		static vector<pair<Function*,double>*> FUNCTION;
		static vector<Reaction*> REACTION;
		static PLA* PLA_SIM;
		static void init_Network3(bool verbose);
		static void init_PLA(string config, bool verbose);
		static pair<long,double> run_PLA(double tStart, double maxTime, double sampleTime, long startStep, long maxSteps,
				long stepInterval, char* prefix, bool print_cdat, bool print_net, bool print_end_net, bool verbose);
//		static bool all_inactive();
		static void print_species_concentrations(FILE* out, double t);
		static void print_observable_concentrations(FILE* out, double t);
		static void print_function_values(FILE* out, double t);
		static void close_Network3(bool verbose);
	};
}

using namespace network3;

#endif /* NETWORK3_HH_ */
