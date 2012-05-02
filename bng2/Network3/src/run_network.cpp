/*
 *    run_network : river for network propagation routines,
 *                    including ODE solvers and Gillespie simulators.
 *
 *                  Copyright (C) 2006,2010,2012 by
 *
 *      James R. Faeder    (faeder at pitt dot edu)
 *      Leonard A. Harris  (lharris at pitt dot edu)
 *      Ilya Korsunsky
 *      Justin S. Hogg
 *
 *    This program is free software: you can redistribute it and/or modify
 *    it under the terms of the GNU General Public License as published by
 *    the Free Software Foundation, either version 3 of the License, or
 *    (at your option) any later version.
 *
 *    This program is distributed in the hope that it will be useful,
 *    but WITHOUT ANY WARRANTY; without even the implied warranty of
 *    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *    GNU General Public License for more details.
 *
 *    You should have received a copy of the GNU General Public License
 *    along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

/* include files */
extern "C" {
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <sys/stat.h>
#include <unistd.h>
#include <errno.h>
#include <time.h>
#include <sys/times.h>
#include <limits.h>
#include "util/mathutils/mathutils.h"
}

#include <vector>
#include <map>
#include <list>

#include "network3.hh"

#ifndef RUN_NETWORK_VERSION
#define RUN_NETWORK_VERSION "3.0"
#endif

struct program_times{
	double cpu;
	double total_cpu;
	double total_real;
};

struct program_times t_elapsed(){
	static double t_last = 0.0;
	static double real_start_time;
	static int initflag = 1;
	double t_new;
	struct program_times t_elapsed;
	struct tms times_buffer;

	if (initflag) {
		real_start_time = (double)time(NULL);
		initflag = 0;
	}
	/* total elapsed time */
	t_elapsed.total_real = (double)time(NULL) - real_start_time;

	/* cpu time--- user + system */
	times(&times_buffer);
	t_elapsed.total_cpu = t_new = (times_buffer.tms_utime + times_buffer.tms_stime) / (double)sysconf(_SC_CLK_TCK);
	t_elapsed.cpu = t_new - t_last;
	t_last = t_new;

	return (t_elapsed);
}

void print_error(){
	char *usage="run_network  [-bcdefkmsvx] [-a atol] [-g groupfile] [-h seed] [-i start_time] [-o outprefix] [-r rtol] [-t tol] [-z iteration number]";
	fprintf(stderr,	"Usage:\n%s netfile sample_time n_sample\n",usage);
	fprintf(stderr, "or\n");
	fprintf(stderr,	"%s netfile t1 t2 ... tn\n", usage);
	exit(1);
}

int main(int argc, char *argv[]){

	register int i/*, j*/;
    char *netfile_name, *network_name;
    char *group_input_file_name = NULL;
    char *save_file_name;
    FILE *netfile, *conc_file, *group_file, *func_file, *out, *flux_file, *species_stats_file;
    int line_number, n_read;
    Elt_array *species, *rates/*, *parameters*/;
    Group *spec_groups = NULL;
    Rxn_array *reactions;
    int n, n_sample;
    double t_start=0.0, t, dt, atol = 1.0e-8, rtol = 1.0e-8;
    double sample_time, *sample_times = 0x0, /* *st,*/ t1;
    char c, buf[1000], *outpre = NULL;
    int argleft, iarg = 1, error = 0;
    int save_file = 0;
    int check_steady_state = 0;
    int seed = -1;
    int remove_zero = 1;
    int print_flux = 0, print_end_net = 0, print_save_net = 0, enable_species_stats = 0;
    bool print_cdat = true, print_fdat = false;
    int gillespie_update_interval = 1;
    int verbose=0;
    int continuation=0;
    double *conc, *conc_last, *derivs;
    struct program_times ptimes;
//  extern char *optarg;
//  extern int optind, opterr;
    // Allowed propagator types
    enum {SSA, CVODE, EULER, RKCS, PLA};
    int propagator = CVODE;
    int SOLVER = DENSE;
    int outtime = -1;
    //
    double maxSteps = INFINITY;//LONG_MAX;//-1;
    double stepInterval = INFINITY;//LONG_MAX;// -1;
    string pla_config;// = "fEuler|sb|pre:neg|0.03,3,100,0.5"; // Default

    if (argc < 4) print_error();

    /* Process input options */
    while ( argv[iarg][0] == '-'  ){
    	c = argv[iarg++][1];
    	switch (c){
    	case 'a':
    		atol = atof(argv[iarg++]);
    		break;
    	case 'b':
    		if(SOLVER == DENSE) SOLVER = GMRES;
    		else SOLVER = GMRES_J;
    		break;
    	case 'c':
    		check_steady_state = 1;
    		break;
    	case 'd':
    		if (SOLVER == DENSE) SOLVER = DENSE_J;
    		else SOLVER = GMRES_J;
    		break;
    	case 'e':
    		print_end_net = 1;
    		break;
    	case 'f':
    		print_flux = 1;
    		break;
    	case 'g':
    		group_input_file_name = argv[iarg++];
    		break;
    	case 'h':
    		seed = atoi(argv[iarg++]);
    		if (seed == INT_MAX){
    			cout << "Warning in run_network: Your seed (" << seed  << ") equals INT_MAX." << endl;
    			cout << "Are you sure you didn't enter a seed larger than INT_MAX?" << endl;
    			cout << "If you did you could be getting duplicate results." << endl;
    		}
    		break;
    	case 'i':
    		t_start = atof(argv[iarg++]);
    		break;
    	case 'j':
    		enable_species_stats = 1;
    		break;
    	case 'k':
    		remove_zero = 0;
    		break;
    	case 'm':
    		propagator = SSA;
    		break;
    	case 'n':
    		print_save_net = 1;
    		break;
    	case 'o':
    		outpre = argv[iarg++];
    		break;
    	case 'p':
    		if (strcmp(argv[iarg],"ssa") == 0) propagator= SSA;
    		else if (strcmp(argv[iarg],"cvode") == 0) propagator= CVODE;
    		else if (strcmp(argv[iarg],"euler") == 0) propagator= EULER;
    		else if (strcmp(argv[iarg],"rkcs") == 0) propagator= RKCS;
    		else if (strcmp(argv[iarg],"pla") == 0){
    			propagator= PLA;
    			if (argv[iarg+1][0] != '-') pla_config = argv[++iarg];
    			else{
    				cout << "ERROR: To use the pla you must specify a simulation configuration. Please try again." << endl;
    				exit(1);
    			}
    		}
    		else{
    			fprintf(stderr, "ERROR: Unrecognized propagator type %s.\n", argv[iarg]);
    			exit(1);
    		}
    		iarg++;
    		break;
    	case 'r':
    		rtol = atof(argv[iarg++]);
    		break;
    	case 's':
    		save_file = 1;
    		break;
    	case 't':
    		atol = rtol = atof(argv[iarg++]);
    		break;
    	case 'u':
    		gillespie_update_interval = atoi(argv[iarg++]);
    		break;
    	case 'v':
    		verbose = 1;
    		break;
    	case 'x': /* continue ('extend') simulation */
    		continuation = 1;
    		break;
    	case 'z':
    		outtime = atoi(argv[iarg++]);
    		break;
    	case '?':
    		++error;
    		break;
    	case 'M':
    		if ((string)argv[iarg] == "INT_MAX") maxSteps = (double)INT_MAX;
    		else if ((string)argv[iarg] == "LONG_MAX") maxSteps = (double)LONG_MAX;
    		else if ((string)argv[iarg] == "INFINITY") maxSteps = INFINITY;//LONG_MAX;//-1L;
    		else maxSteps = floor(atof(argv[iarg])); //std::atol(argv[iarg++]);
    		if (maxSteps <= 0){
    			cout << "Warning: You set maxSteps = " << maxSteps << ". Simulation will not run." << endl;
    		}
    		iarg++;
    		break;
    	case 'I':
    		if ((string)argv[iarg] == "INT_MAX") stepInterval = (double)INT_MAX;
    		else if ((string)argv[iarg] == "LONG_MAX") stepInterval = (double)LONG_MAX;
    		else if ((string)argv[iarg] == "INFINITY") stepInterval = INFINITY;//LONG_MAX;//-1L;
    		else stepInterval = floor(atof(argv[iarg])); //std::atol(argv[iarg++]);
    		iarg++;
    		break;
    	case '-': // Process long options
    		//
//			cout << argv[iarg-1] << " ";
			string long_opt(argv[iarg-1]);
			long_opt = long_opt.substr(2); // remove '--'
			//
			// Output to .cdat
			if (long_opt == "cdat"){
				if (Util::convertToInt(argv[iarg]) <= 0){
					print_cdat = false;
				}
			}
			// Output to .fdat
			else if (long_opt == "fdat"){
				if (Util::convertToInt(argv[iarg]) > 0){
					print_fdat = true;
				}
			}
			else{
//				cout << endl;
				cout << "Sorry, don't recognize your long option " << argv[iarg-1] << ". Please try again." << endl;
			}
			iarg++;
    		//
    		break;
    	}
    }

    /* Check input options for consistency */

    /* Check for correct number of input args */
    argleft = argc - iarg;
    if (argleft < 3) print_error();

	/* Get net file name */
	netfile_name = strdup(argv[iarg++]);

	/* Is the trajectory a continuation of a previous simulation? */
//	if (t_start > 0) continuation = 1;
//	else continuation = 0;

	/* Process sample times */
	if ((argleft = argc - iarg) == 2) {
		/* input is sample_time n_sample */
		sample_time = atof(argv[iarg++]);
		n_sample = atoi(argv[iarg++]);
	}
	else {
		/* input is t1 t2 ... tn */
		n_sample = argleft;
		t1 = atof(argv[iarg++]);
		if (t1 == t_start){
			n_sample--;
			t1 = atof(argv[iarg++]);
		}
		sample_times = ALLOC_VECTOR(n_sample + 1);
		sample_times[0] = t_start;
		sample_times[1] = t1;
		for (int j=2;j < n_sample+1;j++){ // t_start is the extra sample
			sample_times[j] = atof(argv[iarg++]);
		}
		/*if (t0 != t_start){
			sample_times[1] = t0;
			st = &sample_times[2];
		}
		else {
			st = &sample_times[1];
		}
		for (iarg = iarg; iarg < argc; ++iarg, ++st) {
			*st = atof(argv[iarg]);
		}*/
		/* Check that final array is in ascending order with no negative elements */
		for (i = 0; i <= n_sample; ++i) {
			if (sample_times[i] < 0.0) {
				fprintf(stderr,"ERROR: Negative sample times are not allowed.\n");
				exit(1);
			}
			if (i == 0)	continue;
			if (sample_times[i] <= sample_times[i-1]) {
				fprintf(stderr,"ERROR: Sample times must be in ascending order.\n");
				exit(1);
			}
		}
	}

	if (!(netfile = fopen(netfile_name, "r"))) {
		fprintf(stderr, "ERROR: Couldn't open file %s.\n", netfile_name);
		exit(1);
	}

	/* Output message */
	fprintf(stdout, "run_network %s\n", RUN_NETWORK_VERSION);
	fflush(stdout);

	/* Assign network_name based on netfile_name */
	network_name = chop_suffix(netfile_name,".net");
	if (!outpre){
		outpre = network_name;
	}

	line_number = 0;

	/* Rate constants and concentration parameters should now be placed in the parameters block. */
	rates = read_Elt_array(netfile, &line_number, "parameters", &n_read, 0x0);
	fprintf(stdout, "Read %d parameters\n", n_read);
	if (n_read < 1) {
		fprintf(stderr,"ERROR: Reaction network must have parameters defined to be used as rate constants.\n");
		exit(1);
	}
	rewind(netfile);
	line_number = 0;

    /* Read species */
    if (!(species = read_Elt_array(netfile, &line_number, "species", &n_read, rates))){
    	fprintf(stderr,"ERROR: Couldn't read rates array.\n");
    	exit(1);
    }
    fprintf(stdout, "Read %d species\n", n_read);

	/* Read optional groups */
	if (group_input_file_name){
		if (!(group_file = fopen(group_input_file_name, "r"))) {
			fprintf(stderr, "ERROR: Couldn't open file %s.\n", group_input_file_name);
			exit(1);
		}
		line_number = 0;
		spec_groups = read_Groups(0x0, group_file, species, &line_number, "groups", &n_read);
		fprintf(stdout, "Read %d group(s) from %s\n", n_read, group_input_file_name);
		fclose(group_file);
    }

	/** Ilya Korsunsky 6/2/10: Global Functions */
    map<string, double*> param_map = init_param_map(rates, spec_groups);
    map<string, int> observ_index_map = init_observ_index_map(spec_groups);
    map<string, int> param_index_map = init_param_index_map(rates);
    vector<vector<int> > func_observ_depend; 
    vector<vector<int> > func_param_depend; 

    vector<mu::Parser> functions; 
    vector<int> variable_parameters;
    char* netfile_name_tmp = netfile_name; 
    sprintf(netfile_name_tmp, "%s%s", netfile_name_tmp, ".net");

    map<string, bool> is_func_map_temp; 
    functions = read_functions_array(netfile_name_tmp, spec_groups, rates, species, variable_parameters, param_map,
				param_index_map, observ_index_map, func_observ_depend, func_param_depend, is_func_map_temp);
    cout << "Read " << functions.size() << " function(s)" << endl;

    /* Read reactions */
	if (!(reactions = read_Rxn_array(netfile, &line_number, &n_read, species, rates, is_func_map_temp, remove_zero))){
		fprintf(stderr, "ERROR: No reactions in the network.\n");
		exit(1);
	}
	fprintf(stdout, "Read %d reaction(s)\n", n_read);
	if (remove_zero) {
		remove_zero_rate_rxns(&reactions, rates);
		fprintf(stdout, "%d reaction(s) have nonzero rate\n", reactions->n_rxn);
	}
	else 
		fprintf(stdout, "nonzero rate reactions were not removed\n"); 
	/* sort_Rxn_array( reactions, rates); */
    fclose(netfile);

	/* Should add check that reactions, rates, and species are defined */
	/* Also should check that definitions don't exceed array bounds */
	if (n_sample < 1) {
		fprintf(stderr, "ERROR: n_sample < 1\n");
		exit(1);
	}

	/* Initialize reaction network */
	init_network(reactions, rates, species, spec_groups, network_name, functions, variable_parameters,
				 func_observ_depend, func_param_depend, is_func_map_temp);

	// Round species populations if propagator is SSA or PLA
	if (propagator == SSA || propagator == PLA){
		for (int i=0;i < network.species->n_elt;i++) {
			network.species->elt[i]->val = floor(network.species->elt[i]->val + 0.5);
		}
	}

	/* Initialize SSA */
	if (propagator == SSA){
		init_gillespie_direct_network(gillespie_update_interval, seed);
	}

	/* Save network to file */
	if (save_file) {
		if (outpre) {
			sprintf(buf, "%s.net", outpre);
			save_file_name = strdup(buf);
		}
		else {
			save_file_name = strdup("save.net");
		}
		if ((out = fopen(save_file_name, "w"))) {
			print_network(out);
			fprintf(stdout, "Saved network to file %s.\n", save_file_name);
			fclose(out);
		}
	}
	fflush(stdout);

	/* timing for initialization */
	ptimes = t_elapsed();
	fprintf(stdout, "Initialization took %.2f CPU seconds\n", ptimes.total_cpu);

	t = t_start;

	/* space for concentration vector */
	if (check_steady_state) {
		conc = ALLOC_VECTOR(n_species_network());
		conc_last = ALLOC_VECTOR(n_species_network());
		derivs = ALLOC_VECTOR(n_species_network());
		get_conc_network(conc_last);
	}
	outpre = chop_suffix(outpre, ".net");

	/* Initialize and print initial concentrations */
	conc_file = NULL; // Just to be safe
	conc_file = init_print_concentrations_network(outpre, continuation);
	if (!continuation) print_concentrations_network(conc_file, t);
	if (!print_cdat) cout << "Suppressing concentrations (.cdat) output" << endl;

	/* Initialize and print initial group concentrations */
	group_file = NULL;
	if (spec_groups){
		group_file = init_print_group_concentrations_network(outpre, continuation);
		if (!continuation) print_group_concentrations_network(group_file, t);
	}

	// Initialize and print initial function values
	func_file = NULL;
	if (print_fdat && network.functions.size() > 0){
		func_file = init_print_function_values_network(outpre, continuation);
		if (!continuation) print_function_values_network(func_file, t);
		cout << "Activating functions (.fdat) output" << endl;
	}

	/* Initialize and print species stats (if enabled) */
	species_stats_file = NULL;
	if (enable_species_stats){
		species_stats_file = init_print_species_stats(outpre, continuation);
		if (!continuation) print_species_stats(species_stats_file, t);
	}

	/* Initialize flux printing (if selected) */
	flux_file = NULL;
	if (print_flux){
		flux_file = init_print_flux_network(outpre);
		print_flux_network(flux_file, t);
	}

	fflush(stdout);

	// *** Simulate ***
	double t_end;
	if (sample_times) t_end = sample_times[n_sample];
	else t_end = t_start + (double)n_sample*sample_time;

	// PLA simulator
	if (propagator == PLA)
	{
		cout << "Accelerated stochastic simulation using PLA" << endl;

		// Initialize Network3
		Network3::init_Network3(false);

		// Initialize PLA
		Network3::init_PLA(pla_config,verbose);
		if (seed >= 0)	Network3::PLA_SIM->setSeed(seed);

		// Initial output to stdout
		if (verbose){
			cout << "#" << "\t" << setw(8) << left << "time" << "\t" << "step" << endl;
			cout << "\t" << fixed << t_start; cout.unsetf(ios::fixed); cout << "\t" << 0 << endl;
		}

		// Run simulation
//		initTime = clock();
		if (!verbose) cout << "Running..." << flush;
		double step = 0, outputCounter = 0;
		pair<double,double> nSteps_Tau;
		if (sample_times){ // Sample times
			for (int i=1;i < n_sample+1 && step < maxSteps - network3::TOL;i++) // t_start is the extra sample (already output)
			{
				// Simulate to next output >>step<<
//				if ((step % stepInterval) != 0){
//					long stepsLeft = stepInterval - (step % stepInterval);
				if (outputCounter < stepInterval - network3::TOL){
					double stepsLeft = stepInterval - outputCounter;
//					cout << "(run: " << time << " --> " << sample_times[i] << ")" << endl;
//					cout << "(maxSteps: " << maxSteps << ")" << endl;
//					cout << "(step: " << step << ")" << endl;
//					cout << "(" << stepsLeft << " steps left until next output)" << endl;
					nSteps_Tau = Network3::run_PLA(t,sample_times[i],INFINITY,step,min(stepsLeft,maxSteps),
									 			   stepInterval,outpre,print_cdat,print_save_net,print_end_net,verbose);
					step += nSteps_Tau.first;
					t += nSteps_Tau.second;
					//
					outputCounter += nSteps_Tau.first;
					if (outputCounter >= stepInterval - network3::TOL) outputCounter = 0;
				}

				if (step < maxSteps - network3::TOL){
					// Error check
					if (outputCounter > network3::TOL){
						cout << "Uh oh, step < maxSteps && outputCounter > 0, shouldn't happen. Exiting." << endl;
						exit(1);
					}
					// Simulate to next output >>time<<
//					cout << "(run: " << time << " --> " << sample_times[i] << ")" << endl;
//					cout << "(maxSteps: " << maxSteps << ")" << endl;
//					cout << "(step: " << step << ")" << endl;
					nSteps_Tau = Network3::run_PLA(t,sample_times[i],INFINITY,step,maxSteps,
												   stepInterval,outpre,print_cdat,print_save_net,print_end_net,verbose);
					step += nSteps_Tau.first;
					t += nSteps_Tau.second;
					//
					outputCounter += nSteps_Tau.first;
					if (outputCounter >= stepInterval - network3::TOL) outputCounter = 0;
				}
			}
		}
		else{ // Sample interval
			nSteps_Tau = Network3::run_PLA(t_start,t_end,sample_time,step,maxSteps,
										   stepInterval,outpre,print_cdat,print_save_net,print_end_net,verbose);
			step += nSteps_Tau.first;
			t += nSteps_Tau.second;
		}
		if (!verbose) cout << "Done" << endl;
//		cout << "PLA simulation took " << (clock()-initTime)/(double)CLOCKS_PER_SEC << " CPU seconds" << endl;

		// Even if .cdat printing is suppressed, must output the last step
		if (!print_cdat){
			string filename(outpre);
			filename += ".cdat";
			FILE* cdatFile = fopen(filename.c_str(),"a");
			Network3::print_species_concentrations(cdatFile,t);
		}

		// Print total steps to stdout
		fprintf(stdout, "TOTAL STEPS: %d\n", (int)step);

		// Clean up
		Network3::close_Network3(false);
	}
	// ODE & SSA simulators
	else{
		/* Compute time course */
		long int /*n_steps = 0, n_steps_last = 0,*/ n_rate_calls_last = 0, n_deriv_calls_last = 0;
		double n_steps = 0, n_steps_last = 0;//, n_rate_calls_last = 0, n_deriv_calls_last = 0;
		//
		double stepLimit = min(stepInterval,maxSteps);
		bool maxStepsReached = false;
		double t_out = t_start;
		//
		bool first_step = true;
		int n_old = 0;
		for (n = 1; n <= n_sample && !maxStepsReached; ++n){
			if (n != n_old){
				if (sample_times) t_out = sample_times[n];
				else t_out += sample_time;
				n_old = n;
			}
			dt = t_out-t;
			dt = min(dt,t_end-t); // Don't go past end time
			if (first_step){
				first_step = false;
				switch (propagator) {
				case SSA:
					fprintf(stdout, "Stochastic simulation using direct Gillespie algorithm\n");
					if (verbose) fprintf(stdout, "%15s %8s %12s %7s %7s %10s %7s\n", "time", "n_steps", "n_rate_calls",
												 "% spec", "% rxn", "n_species", "n_rxns");
					break;
				case CVODE:
					fprintf(stdout, "Propagating with cvode");
					if (SOLVER == GMRES) fprintf(stdout, " using GMRES\n");
					else if (SOLVER == GMRES_J) fprintf(stdout, " using GMRES with specified Jacobian multiply\n");
					else if (SOLVER == DENSE_J) fprintf(stdout, " using dense LU with specified Jacobian\n");
					else fprintf(stdout, " using dense LU\n");
					if (verbose) fprintf(stdout, "%15s %16s %13s\n", "time", "n_steps", "n_deriv_calls");
					break;
				case EULER:
					fprintf(stdout,"Propagating with Euler method using fixed time step of %.15g\n",rtol);
					if (verbose) fprintf(stdout, "%15s %13s %13s\n", "time", "n_steps", "n_deriv_calls");
					break;
				case RKCS:
					fprintf(stdout, "Propagating with rkcs\n");
					if (verbose) fprintf(stdout, "%15s %13s %13s\n", "time", "n_steps", "n_deriv_calls");
					break;
				}
				if (verbose) fflush(stdout);
			}

			/* Do propagation */
			switch (propagator){
			case SSA:
				if (gillespie_n_steps() >= stepLimit - network3::TOL){
					// Error check
					if (gillespie_n_steps() > stepLimit + network3::TOL){
						cout << "Uh oh, step limit exceeded in SSA (step limit = " << stepLimit << ", current step = "
							 << gillespie_n_steps() << "). This shouldn't happen. Exiting." << endl;
						exit(1);
					}
					// Continue
					stepLimit = min(stepLimit+stepInterval,maxSteps);
				}
				error = gillespie_direct_network(&t, dt, 0x0, 0x0, stepLimit-network3::TOL);
				if (verbose){
//					fprintf(stdout, "%15.6f %8ld %12d %7.3f %7.3f %10d %7d",
					fprintf(stdout, "%15.6f %8.0f %12d %7.3f %7.3f %10d %7d",
							t,
							gillespie_n_steps() - n_steps_last,
							n_rate_calls_network() - (int)n_rate_calls_last,
							100 * gillespie_frac_species_active(),
							100 * gillespie_frac_rxns_active(),
							n_species_network(),
							n_rxns_network()
							);
				}
				n_steps_last = gillespie_n_steps();
				if (error == 1) n -= 1; // stepLimit reached in propagation
				if (gillespie_n_steps() >= maxSteps - network3::TOL) maxStepsReached = true;
				break;
			case CVODE:
				if (n_steps >= stepLimit - network3::TOL){
					// Error check
					if (n_steps > stepLimit + network3::TOL){
						cout << "Uh oh, step limit exceeded in CVODE (step limit = " << stepLimit << ", current step = "
							 << n_steps << "). This shouldn't happen. Exiting." << endl;
						exit(1);
					}
					// Continue
					stepLimit = min(stepLimit+stepInterval,maxSteps);
				}
				error = propagate_cvode_network(&t, dt, &n_steps, &rtol, &atol, SOLVER, stepLimit-network3::TOL);
//				if (verbose) fprintf(stdout, "%15.2f %13ld %13d", t, n_steps, n_deriv_calls_network());
				if (verbose) fprintf(stdout, "%15.2f %13.0f %13d", t, n_steps, n_deriv_calls_network());
				if (error == 1) n -= 1; // stepLimit reached in propagation
				if (n_steps >= maxSteps - network3::TOL) maxStepsReached = true;
				break;
			case EULER:
				if (n_steps >= stepLimit - network3::TOL){
					// Error check
					if (n_steps > stepLimit + network3::TOL){
						cout << "Uh oh, step limit exceeded in EULER (step limit = " << stepLimit << ", current step = "
							 << n_steps << "). This shouldn't happen. Exiting." << endl;
						exit(1);
					}
					// Continue
					stepLimit = min(stepLimit+stepInterval,maxSteps);
				}
				error = propagate_euler_network(&t, dt, &n_steps, rtol, stepLimit-network3::TOL);
//				if (verbose) fprintf(stdout, "%15.2f %13ld %13d", t, n_steps, n_deriv_calls_network());
				if (verbose) fprintf(stdout, "%15.2f %13.0f %13d", t, n_steps, n_deriv_calls_network());
				if (error == 1) n -= 1; // stepLimit reached in propagation
				if (n_steps >= maxSteps - network3::TOL) maxStepsReached = true;
				break;
			case RKCS:
				if (n_steps >= stepLimit - network3::TOL){
					// Error check
					if (n_steps > stepLimit + network3::TOL){
						cout << "Uh oh, step limit exceeded in RKCS (step limit = " << stepLimit << ", current step = "
							 << n_steps << "). This shouldn't happen. Exiting." << endl;
						exit(1);
					}
					// Continue
					stepLimit = min(stepLimit+stepInterval,maxSteps);
				}
				error = propagate_rkcs_network(&t, dt, &n_steps, rtol, stepLimit-network3::TOL);
//				if (verbose) fprintf(stdout, "%15.2f %13ld %13d", t, n_steps, n_deriv_calls_network());
				if (verbose) fprintf(stdout, "%15.2f %13.0f %13d", t, n_steps, n_deriv_calls_network());
				if (error == 1) n -= 1; // stepLimit reached in propagation
				if (n_steps >= maxSteps - network3::TOL) maxStepsReached = true;
				break;
			}
			n_rate_calls_last = n_rate_calls_network();
			n_deriv_calls_last = n_deriv_calls_network();
			if (error > 1) { // error = 1 means stepLimit was reached in propagation (not really an error)
				fprintf(stderr, "Stopping due to error in integration.\n");
				exit(1);
			} // End propagation

			// Print current properties of the system
			if (print_cdat) print_concentrations_network(conc_file, t);
			if (spec_groups) print_group_concentrations_network(group_file, t);
			if (network.functions.size() > 0 && print_fdat) print_function_values_network(func_file, t);
		    if (enable_species_stats) print_species_stats(species_stats_file, t);
			if (print_flux) print_flux_network(flux_file, t);
			if (print_save_net){
				if (outpre) sprintf(buf, "%s_save.net", outpre);
				else sprintf(buf, "save.net");
				out = fopen(buf, "w");
				print_network(out);
				fclose(out);
				fprintf(stdout, " Wrote NET file to %s", buf);
			}
			/* Check if steady state reached */
			if (check_steady_state) {
				double *a, delta, dx;
				get_conc_network(conc);
				delta = sqrt(VECTOR_DIST(conc, conc_last, n_species_network())) / n_species_network();
				fprintf(stdout, " RMS change in concentrations=%.1e.", delta);

				// Calculate derivatives
				derivs_network(t, conc, derivs);
				dx = NORM(derivs, n_species_network()) / n_species_network();
				fprintf(stdout, " |dx/dt|/dim(x)=%.1e.", dx);

				//if (delta<10*atol){
				if (dx < atol) {
					fprintf(stdout, "  Steady state reached.\n");
					break;
				}
				/* Swap conc and conc_last pointers */
				a = conc_last;
				conc_last = conc;
				conc = a;
			}
			if (verbose) printf("\n");
			if (n == outtime) {
				char buf[1000];
				FILE *outfile;
				sprintf(buf, "%s.m", outpre);
				outfile = fopen(buf, "w");
				init_sparse_matlab_file(outfile);
				sparse_jac_matlab(outfile);
				fclose(outfile);
				fprintf(stdout, "Jacobian written to %s after iteration %d\n", buf, outtime);
			}
			if (verbose) fflush(stdout);
		} // end for
	} // end else

	// Final print outs
	if (!print_cdat && propagator != PLA){ // Even if .cdat is suppressed, must print the last step (PLA handles this internally)
		print_concentrations_network(conc_file, t);
	}
	finish_print_concentrations_network(conc_file);
	if (spec_groups) finish_print_group_concentrations_network(group_file);
	if (network.functions.size() > 0 && print_fdat) finish_print_function_values_network(func_file);
	if (enable_species_stats) finish_print_species_stats(species_stats_file);

	// Clean up memory allocated for functions
	if (network.has_functions) delete[] network.rates->elt;
	if (propagator == SSA){
		// GSP.included added to GSP struct in code extension for functions
		// NOTE: GSP.included is created whether functions exist or not, so it must always be deleted
		delete_GSP_included();
	}

	// Screen outputs
	outpre = chop_suffix(outpre, ".net");
	if (propagator == SSA) fprintf(stdout, "TOTAL STEPS: %-16.0f\n", gillespie_n_steps());
	fprintf(stdout, "Time course of concentrations written to file %s.cdat.\n", outpre);
	if (n_groups_network()) fprintf(stdout, "Time course of groups written to file %s.gdat.\n", outpre);
	ptimes = t_elapsed();
	fprintf(stdout, "Propagation took %.2e CPU seconds\n", ptimes.cpu);

	/* Print final concentrations in species list */
	if (print_end_net){
		if (outpre) sprintf(buf, "%s_end.net", outpre);
		else sprintf(buf, "end.net");
		out = fopen(buf, "w");
		print_network(out);
		fclose(out);
		fprintf(stdout, "Final network file written to %s\n", buf);
	}
	ptimes = t_elapsed();
	fprintf(stdout, "Program times:  %.2f CPU s %.2f clock s \n", ptimes.total_cpu, ptimes.total_real);

//	exit:
	if (sample_times) free(sample_times);
	return (0);
}
