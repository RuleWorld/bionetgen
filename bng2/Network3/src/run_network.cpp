/*
 *    run_network : river for network propagation routines,
 *                    including ODE solvers and stochastic simulators.
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
#include "mathutils.h"
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
	char *usage=(char*)"run_network  [-bcdefkmsvx] [-a atol] [-g groupfile] [-h seed] [-i start_time] [-o outprefix] [-r rtol] [-t tol] [-z iteration number]";
	fprintf(stderr,	"Usage:\n%s netfile sample_time n_sample\n",usage);
	fprintf(stderr, "or\n");
	fprintf(stderr,	"%s netfile t1 t2 ... tn\n", usage);
	exit(1);
}

int main(int argc, char *argv[]){
	/* Output message */
	fprintf(stdout, "run_network %s\n", RUN_NETWORK_VERSION);
	fflush(stdout);
	// Variables //
	register int i/*, j*/;
    char *netfile_name, *network_name;
    char *group_input_file_name = NULL;
    char *save_file_name;
    FILE *netfile, *conc_file, *group_file/*, *func_file*/, *out, *flux_file, *species_stats_file;
    int net_line_number, group_line_number, n_read;
    Elt_array *species, *rates/*, *parameters*/;
    Group *spec_groups = NULL;
    Rxn_array *reactions;
    int n, n_sample;
    double t_start=0.0, t, dt, atol = 1.0e-8, rtol = 1.0e-8;
    double sample_time, *sample_times = 0x0/*, *st, t1*/;
    char c, buf[1000], *outpre = NULL;
    int argleft, iarg = 1, error = 0;
    int save_file = 0;
    int check_steady_state = 0;
    int seed = -1;
    int remove_zero = 1;
    int print_flux = 0, print_end_net = 0, print_save_net = 0, enable_species_stats = 0;
    int gillespie_update_interval = 1;
    int verbose=0;
    int continuation=0;
    double *conc, *conc_last, *derivs;
    struct program_times ptimes;
//  extern char *optarg;
//  extern int optind, opterr;
    //
    // Allowed propagator types
    enum {SSA, CVODE, EULER, RKCS, PLA};
    int propagator = CVODE;
    int SOLVER = DENSE;
    int outtime = -1;
    //
    double maxSteps = INFINITY;//LONG_MAX;//-1;
    double stepInterval = INFINITY;//LONG_MAX;// -1;
    string pla_config; // No default
    bool print_cdat = true, print_func = false;
    bool additional_pla_output = false; // Print PLA-specific data (e.g., rxn classifications)
    bool print_on_stop = true; // Print to file if stopping condition met?
    string stop_string = "0";
    mu::Parser stop_condition;

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
//			cout << argv[iarg-1] << " ";
			string long_opt(argv[iarg-1]);
			long_opt = long_opt.substr(2); // remove '--'
			//
			// Print to .cdat
			if (long_opt == "cdat"){
				if (atoi(argv[iarg]) <= 0){
					print_cdat = false;
					cout << "Suppressing concentrations (.cdat) output" << endl;
				}
			}
			// Print to .fdat
			else if (long_opt == "fdat"){
				if (atoi(argv[iarg]) > 0){
					print_func = true;
					cout << "Activating functions output (to .gdat)" << endl;
				}
			}
			// Print additional PLA data (e.g., rxn classifications)
			else if (long_opt == "pla_output"){
				if (atoi(argv[iarg]) > 0){
					additional_pla_output = true;
				}
			}
			else if (long_opt == "stop_cond"){
				stop_string = (string)argv[iarg++];
				cout << "Stopping condition specified: " << stop_string;
				if (atoi(argv[iarg]) <= 0){
					print_on_stop = false;
					cout << " (print-on-stop disabled)";
				}
				cout << endl;
//				cout << stop_string << endl;
			}
			//...
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

	/* Process sample times */
	if ((argleft = argc - iarg) == 2) {
		/* input is sample_time n_sample */
		sample_time = atof(argv[iarg++]);
		n_sample = (int)atof(argv[iarg++]); // Read as float and cast to int to allow for exponential format
	}
	else {
		/* input is t1 t2 ... tn */
		n_sample = argleft;
		vector<double> st;
		vector<bool> keep;
		st.push_back(t_start);
		keep.push_back(true);

		// Collect all sample times
		for (int j=0;j < n_sample;j++){
			st.push_back(atof(argv[iarg++]));
			keep.push_back(true);
		}
		if (t_start > st[st.size()-1]){ // BNG appends t_end to the sample_times array
			cout << "WARNING: t_start > t_end. Setting t_end = t_start, simulation will not run." << endl;
			st[st.size()-1] = t_start;
		}
		double t_end = st[st.size()-1];

		// Flag sample times <= t_start and >= t_end for removal
		for (unsigned int j=1;j < st.size()-1;j++){
			if (st[j] <= t_start || st[j] >= t_end){
//				cout << ": ERASE";
				keep.at(j) = false;
				n_sample--;
			}
//			cout << endl;
		}

		// Fill up sample_times array
		sample_times = ALLOC_VECTOR(n_sample+1); // t_start is the extra sample
		int k=0;
		for (unsigned int j=0;j < st.size();j++){
			if (keep.at(j)){
				sample_times[k] = st[j];
				k++;
			}
		}
		// Error check
		if (k != n_sample+1){
			cout << "Oops, something went wrong while processing sample_times." << endl;
			exit(1);
		}

		// Make sure there are at least 2 elements (t_start and t_end)
		if (n_sample < 1){
			fprintf(stderr,"ERROR: There must be at least one sample time (t_end).\n");
			exit(1);
		}

		// Check that final array is in ascending order with no negative elements
		for (i = 0; i <= n_sample; ++i) {
			if (sample_times[i] < 0.0) {
				fprintf(stderr,"ERROR: Negative sample times are not allowed.\n");
				exit(1);
			}
			if (i == 0)	continue;
//			if (sample_times[i] <= sample_times[i-1]) {
			if (sample_times[i] < sample_times[i-1]) { // Handle case where n_sample=2 and t_start=t_end
				fprintf(stderr,"ERROR: Sample times must be in ascending order.\n");
				exit(1);
			}
		}
	}

	// Initialize time
	t = t_start;

	// Find NET file
	if (!(netfile = fopen(netfile_name, "r"))) {
		fprintf(stderr, "ERROR: Couldn't open file %s.\n", netfile_name);
		exit(1);
	}

	/* Assign network_name based on netfile_name */
	network_name = chop_suffix(netfile_name,".net");
	if (!outpre){
		outpre = network_name;
	}

	/* Rate constants and concentration parameters should now be placed in the parameters block. */
	net_line_number = 0;
	rates = read_Elt_array(netfile, &net_line_number, (char*)"parameters", &n_read, 0x0);
	fprintf(stdout, "Read %d parameters\n", n_read);
//	if (n_read < 1) {
//		fprintf(stderr,"ERROR: Reaction network must have parameters defined to be used as rate constants.\n");
//		exit(1);
//	}
	rewind(netfile);
	net_line_number = 0;

    /* Read species */
    if (!(species = read_Elt_array(netfile, &net_line_number, (char*)"species", &n_read, rates))){
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
		group_line_number = 0;
		spec_groups = read_Groups(0x0, group_file, species, &group_line_number, (char*)"groups", &n_read);
		fprintf(stdout, "Read %d group(s) from %s\n", n_read, group_input_file_name);
		fclose(group_file);
    }

	/** Ilya Korsunsky 6/2/10: Global Functions */
    map<string, double*> param_map = init_param_map(rates,spec_groups);
    map<string, int> observ_index_map = init_observ_index_map(spec_groups);
    map<string, int> param_index_map = init_param_index_map(rates);
//    vector<vector<int> > func_observ_depend;
//    vector<vector<int> > func_param_depend;

    char* netfile_name_tmp = netfile_name; 
    sprintf(netfile_name_tmp, "%s%s", netfile_name_tmp, ".net");
    read_functions_array(netfile_name_tmp,rates,param_map,param_index_map,observ_index_map,&t);
    int n_func = network.functions.size();
    if (n_func > 0) n_func--; // Subtract off 'time' function
    cout << "Read " << n_func << " function(s)" << endl;
	if (!rates){ // Error if the 'rates' array doesn't exist (means 0 parameters, 0 functions)
		fprintf(stderr,"ERROR: Reaction network must have parameters and/or functions defined to be used as rate laws.\n");
		exit(1);
	}

    // Create stop condition
	process_function_names(stop_string); // Remove parentheses from variable names
	vector<string> variable_names = find_variables(stop_string); // Extract variable names
//	size_t found;
//	while ((found = stop_string.find("&&")) != string::npos) stop_string.replace(found,2,"and"); // Replace && with 'and'
//	while ((found = stop_string.find("||")) != string::npos) stop_string.replace(found,2,"or");  // Replace || with 'or'
	for (unsigned int i=0;i < variable_names.size();i++){
		// Error check
		if (param_map.find(variable_names[i]) == param_map.end()) {
			cout << "Error in parsing stop condition: \"" << stop_string << "\". Could not find variable "
				 << variable_names[i] << ". Exiting." << endl;
			exit(1);
		}
		// Define variable
		else {
//			cout << variable_names[i] << " = " << *param_map[variable_names[i]] << endl;
			stop_condition.DefineVar(_T(variable_names[i]),param_map[variable_names[i]]);
		}
	}
	stop_condition.SetExpr(stop_string);

    /* Read reactions */
//	if (!(reactions = read_Rxn_array(netfile,&net_line_number,&n_read,species,rates,network.is_func_map,remove_zero))){
	if (!(reactions = read_Rxn_array(netfile,&net_line_number,&n_read,species,rates,network.is_func_map))){
		fprintf(stderr, "ERROR: No reactions in the network.\n");
		exit(1);
	}
	fprintf(stdout, "Read %d reaction(s)\n", n_read);
	if (remove_zero) {
		remove_zero_rate_rxns(&reactions, rates);
		int n_rxn = 0;
		if (reactions){
			n_rxn = reactions->n_rxn;
		}
		fprintf(stdout, "%d reaction(s) have nonzero rate\n", n_rxn);
	}
	else{
		fprintf(stdout, "nonzero rate reactions were not removed\n"); 
	}
	/* sort_Rxn_array( reactions, rates); */
    fclose(netfile);

	/* Should add check that reactions, rates, and species are defined */
	/* Also should check that definitions don't exceed array bounds */
	if (n_sample < 1) {
		fprintf(stderr, "ERROR: n_sample < 1\n");
		exit(1);
	}

	/* Initialize reaction network */
	init_network(reactions, rates, species, spec_groups, network_name);

	// Round species populations if propagator is SSA or PLA
	if (propagator == SSA || propagator == PLA){
		for (int i=0;i < network.species->n_elt;i++) {
			network.species->elt[i]->val = floor(network.species->elt[i]->val + 0.5);
		}
	}

	/* Initialize SSA */
	if (propagator == SSA){
		init_gillespie_direct_network(gillespie_update_interval,seed);
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

//	t = t_start;

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
	conc_file = init_print_concentrations_network(outpre,continuation);
	if (!continuation) print_concentrations_network(conc_file, t);

	/* Initialize and print initial group concentrations and function values */
	group_file = NULL;
	if (spec_groups || (print_func && network.functions.size() > 0)){
		group_file = init_print_group_concentrations_network(outpre,continuation,print_func);
		if (print_func & !continuation) init_print_function_values_network(group_file);
		if (!continuation){
			print_group_concentrations_network(group_file,t,print_func);
			if (print_func) print_function_values_network(group_file,t);
		}
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
		int discrete = 0;
		if (propagator == SSA || propagator == PLA) discrete = 1;
		print_flux_network(flux_file,t,discrete);
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
		Network3::init_Network3(&t,false);

		// Stop condition
		mu::Parser pla_stop_condition;
		map<string,double*> var = stop_condition.GetUsedVar();
		// Search observables
		for (unsigned int j=0;j < Network3::OBSERVABLE.size();j++){
			if (var.find(Network3::OBSERVABLE[j]->first->name) != var.end()){
//				cout << "\t" << Network3::OBSERVABLE[j]->first->name << " = " << Network3::OBSERVABLE[j]->second << endl;
				pla_stop_condition.DefineVar(Network3::OBSERVABLE[j]->first->name,&Network3::OBSERVABLE[j]->second);
			}
		}
		// Search parameters
		for (Elt* elt=network.rates->list;elt != NULL;elt=elt->next){
			if (var.find(elt->name) != var.end()){
//				cout << "\t" << "rates[" << elt->index << "] = " << elt->name << " (";
				bool func = false;
				// Is it a function?
				for (unsigned int j=0;j < network.var_parameters.size() && !func;j++){
					if (elt->index == network.var_parameters[j]){
						// YES
//						cout << "function[" << j <<"] = " << network.functions[j].GetExpr() << ")" << endl;
						func = true;
						bool found = false;
						// Which one?
						for (unsigned int k=0;k < Network3::FUNCTION.size() && !found;k++){
							if (network.functions[j].GetExpr() == Network3::FUNCTION[k]->first->GetExpr()){
								found = true;
								pla_stop_condition.DefineVar(elt->name,&Network3::FUNCTION[k]->second);
							}
						}
						// Error check
						if (!found){
							cout << "Error constructing PLA stop condition in run_network: "
								 << "Couldn't find function " << network.functions[j].GetExpr()
								 << ". Exiting." << endl;
							exit(1);
						}
					}
				}
				// NO, it's a constant
				if (!func){
//					cout << "constant)" << endl;
					pla_stop_condition.DefineConst(elt->name,elt->val);
				}
			}
		}
		// Set expression
		string expr = stop_condition.GetExpr();
		expr.erase(expr.size()-1); // Trim last character (muParser adds a null to the end)
		pla_stop_condition.SetExpr(expr);
//		cout << pla_stop_condition.GetExpr() << "= " << pla_stop_condition.Eval() << endl;

		// Initialize PLA
		Network3::init_PLA(pla_config,verbose);
		if (seed >= 0)	Network3::PLA_SIM->setSeed(seed);

		// PLA-specific output
		if (additional_pla_output){
			cout << "Activating classifications output (to _classif.pla)" << endl;
			if (!continuation){ // Print header
				FILE* outfile = NULL;
				outfile = fopen(((string)outpre+"_classif.pla").c_str(),"w");
				fprintf(outfile, "#");
				fprintf(outfile, "%18s", "time");
				fprintf(outfile, " %19s", "step");
				for (unsigned int v=0;v < Network3::REACTION.size();v++){
					fprintf(outfile," %10s",("R_"+Util::toString((int)v+1)).c_str());
				}
				fprintf(outfile,"\n");
				fclose(outfile);
			}
		}

		// Initial output to stdout
		if (verbose){
			printf("# \t time \t\t step\n");
			printf("\t %f \t 0\n",t_start);
			fflush(stdout);
		}

		// Run simulation
//		initTime = clock();
		if (!verbose) cout << "Running..." << endl;
		double step = 0;
		if (sample_times){ // Sample times
			double nextOutputStep = stepInterval;
			bool forceQuit = false;
			for (int n=1;n <= n_sample && step < maxSteps - network3::TOL && !forceQuit;n++) // t_start is the extra sample (already output)
			{
				error = Network3::run_PLA(t,sample_times[n],INFINITY,
										  step,min(nextOutputStep,maxSteps),stepInterval,
										  pla_stop_condition,print_on_stop,
										  outpre,
										  print_cdat,print_func,print_save_net,print_end_net,
										  additional_pla_output,
										  verbose);
				if (error == -1){ // stepLimit reached in propagation
					n--;
					nextOutputStep += stepInterval;
				}
				if (error == -2){ // Stop condition satisfied
					forceQuit = true;
//					cout << "\nStopping condition " << pla_stop_condition.GetExpr() << "met in "
//						 <<	"PLA simulation." << endl;
				}
			}
		}
		else{ // Sample interval
			error = Network3::run_PLA(t,t_end,sample_time,
									  step,maxSteps,stepInterval,
									  pla_stop_condition,print_on_stop,
									  outpre,
									  print_cdat,print_func,print_save_net,print_end_net,
									  additional_pla_output,
									  verbose);
		}
		if (step >= maxSteps){ // maxSteps limit reached
			cout << "Maximum step limit (" << maxSteps << ") reached in PLA simulation." << endl;
		}
		if (!verbose) cout << "Done." << endl;
//		cout << "PLA simulation took " << (clock()-initTime)/(double)CLOCKS_PER_SEC << " CPU seconds" << endl;

		// Even if .cdat printing is suppressed, must output the last step
		if (step > 0.5 && !print_cdat){ // Only print if the simulation ran (i.e., step > 0)
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
		long int /*n_steps = 0, n_steps_last = 0,*/ n_rate_calls_last = 0, n_deriv_calls_last = 0;
		double n_steps = 0, n_steps_last = 0;//, n_rate_calls_last = 0, n_deriv_calls_last = 0;
		//
		double stepLimit = min(stepInterval,maxSteps);
		bool forceQuit = false;
		string forceQuit_message;
		double t_out = t_start;

		// Initial screen outputs
		switch (propagator) {
		case SSA:
			fprintf(stdout, "Stochastic simulation using direct Gillespie algorithm\n");
			if (verbose){
				fprintf(stdout, "%15s %8s %12s %7s %7s %10s %7s\n", "time", "n_steps", "n_rate_calls",
								 "% spec", "% rxn", "n_species", "n_rxns");
				fprintf(stdout, "%15.6f %8.0f %12d %7.3f %7.3f %10d %7d\n",
						t,
						gillespie_n_steps() - n_steps_last,
						n_rate_calls_network() - (int)n_rate_calls_last,
						100 * gillespie_frac_species_active(),
						100 * gillespie_frac_rxns_active(),
						n_species_network(),
						n_rxns_network()
						);
			}
		break;
		case CVODE:
			fprintf(stdout, "Propagating with cvode");
			if (SOLVER == GMRES) fprintf(stdout, " using GMRES\n");
			else if (SOLVER == GMRES_J) fprintf(stdout, " using GMRES with specified Jacobian multiply\n");
			else if (SOLVER == DENSE_J) fprintf(stdout, " using dense LU with specified Jacobian\n");
			else fprintf(stdout, " using dense LU\n");
			if (verbose){
				fprintf(stdout, "%15s %13s %13s\n", "time", "n_steps", "n_deriv_calls");
				fprintf(stdout, "%15.2f %13.0f %13d\n", t, n_steps, n_deriv_calls_network());
			}
		break;
		case EULER:
			fprintf(stdout,"Propagating with Euler method using fixed time step of %.15g\n",rtol);
			if (verbose){
				fprintf(stdout, "%15s %13s %13s\n", "time", "n_steps", "n_deriv_calls");
				fprintf(stdout, "%15.2f %13.0f %13d\n", t, n_steps, n_deriv_calls_network());
			}
		break;
		case RKCS:
			fprintf(stdout, "Propagating with rkcs\n");
			if (verbose){
				fprintf(stdout, "%15s %13s %13s\n", "time", "n_steps", "n_deriv_calls");
				fprintf(stdout, "%15.2f %13.0f %13d\n", t, n_steps, n_deriv_calls_network());
			}
		break;
		}
		if (verbose) fflush(stdout);

		// Initial check of stopping condition before starting propagation
		if (stop_condition.Eval()){
			cout << "Stopping condition " << stop_condition.GetExpr() << "already met prior "
				 <<	"to simulation. Quitting." << endl;
			forceQuit = true;
		}

		// Do propagation
		int n_old = 0;
		for (n = 1; n <= n_sample && t < t_end-network3::TOL && !forceQuit; ++n){
			if (n != n_old){
				if (sample_times) t_out = sample_times[n];
				else t_out += sample_time;
				n_old = n;
			}
			if (t_end < t_out) t_out = t_end; // Don't go past end time
			dt = t_out-t;
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
				error = gillespie_direct_network(&t, dt, 0x0, 0x0, stepLimit-network3::TOL,stop_condition);
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
				if (error == -1) n -= 1; // stepLimit reached in propagation
				if (error == -2){ // Stop condition satisfied
					forceQuit = true;
					forceQuit_message = "Stopping condition " + stop_condition.GetExpr() +
							"met in Gillespie simulation.";
				}
				if (gillespie_n_steps() >= maxSteps - network3::TOL){ // maxSteps limit reached
					forceQuit = true;
					forceQuit_message = "Maximum step limit (" + Util::toString(maxSteps) +
							") reached in Gillespie simulation.";
				}
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
				error = propagate_cvode_network(&t, dt, &n_steps, &rtol, &atol, SOLVER, stepLimit-network3::TOL,stop_condition);
//				if (verbose) fprintf(stdout, "%15.2f %13ld %13d", t, n_steps, n_deriv_calls_network());
				if (verbose) fprintf(stdout, "%15.2f %13.0f %13d", t, n_steps, n_deriv_calls_network());
				if (error == -1) n -= 1; // stepLimit reached in propagation
				if (error == -2){ // Stop condition satisfied
					forceQuit = true;
					forceQuit_message = "Stopping condition " + stop_condition.GetExpr() +
							"met in CVODE simulation.";
				}
				if (n_steps >= maxSteps - network3::TOL){ // maxSteps limit reached
					forceQuit = true;
					forceQuit_message = "Maximum step limit (" + Util::toString(maxSteps) +
							") reached in CVODE simulation.";
				}
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
				error = propagate_euler_network(&t, dt, &n_steps, rtol, stepLimit-network3::TOL, stop_condition);
//				if (verbose) fprintf(stdout, "%15.2f %13ld %13d", t, n_steps, n_deriv_calls_network());
				if (verbose) fprintf(stdout, "%15.2f %13.0f %13d", t, n_steps, n_deriv_calls_network());
				if (error == -1) n -= 1; // stepLimit reached in propagation
				if (error == -2){ // Stop condition satisfied
					forceQuit = true;
					forceQuit_message = "Stopping condition " + stop_condition.GetExpr() +
							"met in EULER simulation.";
				}
				if (n_steps >= maxSteps - network3::TOL){ // maxSteps limit reached
					forceQuit = true;
					forceQuit_message = "Maximum step limit (" + Util::toString(maxSteps) +
							") reached in EULER simulation.";
				}
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
				error = propagate_rkcs_network(&t, dt, &n_steps, rtol, stepLimit-network3::TOL, stop_condition);
//				if (verbose) fprintf(stdout, "%15.2f %13ld %13d", t, n_steps, n_deriv_calls_network());
				if (verbose) fprintf(stdout, "%15.2f %13.0f %13d", t, n_steps, n_deriv_calls_network());
				if (error == -1) n -= 1; // stepLimit reached in propagation
				if (error == -2){ // Stop condition satisfied
					forceQuit = true;
					forceQuit_message = "Stopping condition " + stop_condition.GetExpr() +
							"met in RKCS simulation.";
				}
				if (n_steps >= maxSteps - network3::TOL){ // maxSteps limit reached
					forceQuit = true;
					forceQuit_message = "Maximum step limit (" + Util::toString(maxSteps) +
							") reached in RKCS simulation.";
				}
				break;
			}
			n_rate_calls_last = n_rate_calls_network();
			n_deriv_calls_last = n_deriv_calls_network();
			if (error > 0) { // error codes < 0 (e.g., stepLimit reached = -1) are not really errors
				fprintf(stderr, "Stopping due to error in integration.\n");
				exit(1);
			} // End propagation

			// Print current properties of the system
			if (print_cdat) print_concentrations_network(conc_file,t);
			// Don't print if stopping condition met and !print_on_stop (must print to CDAT)
			// NOTE: Sometimes forceQuit happens at an output step. In this case print.
			if (!(forceQuit && !print_on_stop && t < t_out-network3::TOL)){
				if (group_file) print_group_concentrations_network(group_file,t,print_func);
				if (group_file && print_func) print_function_values_network(group_file,t);
				if (enable_species_stats) print_species_stats(species_stats_file,t);
				if (print_flux){
					int discrete = 0;
					if (propagator == SSA || propagator == PLA) discrete = 1;
					print_flux_network(flux_file,t,discrete);
				}
				if (print_save_net){
					if (outpre) sprintf(buf, "%s_save.net", outpre);
					else sprintf(buf, "save.net");
					out = fopen(buf, "w");
					print_network(out);
					fclose(out);
					if (verbose) fprintf(stdout, " Wrote NET file to %s", buf);
				}
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

			// Screen output if forceQuit = true
			if (forceQuit) cout << forceQuit_message << endl;

		} // end for
	} // end else

	// Final printouts
	if (t > t_start+network3::TOL && !print_cdat && propagator != PLA){ // If simulation ran t > t_start
		// Even if .cdat is suppressed, must print the last step (PLA handles this internally)
		print_concentrations_network(conc_file, t);
	}
	finish_print_concentrations_network(conc_file);
	if (group_file) finish_print_group_concentrations_network(group_file,print_func);
	if (group_file && print_func) finish_print_function_values_network(group_file);
	if (enable_species_stats) finish_print_species_stats(species_stats_file);

	// Screen outputs
	outpre = chop_suffix(outpre, ".net");
	if (propagator == SSA) fprintf(stdout, "TOTAL STEPS: %-16.0f\n", gillespie_n_steps());
	fprintf(stdout, "Time course of concentrations written to file %s.cdat.\n", outpre);
	if (n_groups_network()) fprintf(stdout, "Time course of groups written to file %s.gdat.\n", outpre);
	if (print_func && network.functions.size() > 0) fprintf(stdout, "Time course of functions written to file %s.gdat.\n", outpre);
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

//	exit:
	// Clean up memory allocated for functions
	if (network.has_functions) delete[] network.rates->elt;
	if (propagator == SSA){
		// GSP.included added to GSP struct in code extension for functions
		// NOTE: GSP.included is created whether functions exist or not, so it must always be deleted
		delete_GSP_included();
	}
	// Delete sample_times if exists
	if (sample_times) free(sample_times);

	// Note that "/^Program times:/" must be last message sent from Network3 (see BNGAction.pm)
	ptimes = t_elapsed();
	fprintf(stdout, "Program times:  %.2f CPU s %.2f clock s \n", ptimes.total_cpu, ptimes.total_real);

	return (0);
}
