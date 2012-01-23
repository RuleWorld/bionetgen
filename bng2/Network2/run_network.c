/*
 * run_network
 *
 * Driver for network propagation routines, including ODE solvers and Gillespie simulators */

/* include files */
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
#include "network.h"

#ifndef RUN_NETWORK_VERSION
#define RUN_NETWORK_VERSION 1.5
#endif

struct program_times {
	double cpu;
	double total_cpu;
	double total_real;
};

struct program_times t_elapsed() {
	static double t_last = 0.0;
	static double real_start_time;
	static int initflag = 1;
	double t_new;
	struct program_times t_elapsed;
	struct tms times_buffer;

	if (initflag) {
		real_start_time = (double) time(NULL);
		initflag = 0;
	}
	/* total elapsed time */
	t_elapsed.total_real = (double) time(NULL) - real_start_time;

	/* cpu time--- user + system */
	times(&times_buffer);
	t_elapsed.total_cpu = t_new = (times_buffer.tms_utime
			+ times_buffer.tms_stime) / (double) sysconf(_SC_CLK_TCK);
	t_elapsed.cpu = t_new - t_last;
	t_last = t_new;

	return (t_elapsed);
}

int main(int argc, char *argv[]) {
	register int i, j;
	char *netfile_name, *network_name;
	char *group_input_file_name = NULL;
	char *save_file_name;
	FILE *netfile, *conc_file, *group_file, *out, *flux_file, *species_stats_file;
	int line_number, n_read;
	Elt_array *species, *rates, *parameters;
	Group *spec_groups = NULL;
	Rxn_array *reactions;
	int n, n_sample;
	double t_start=0, t, dt, atol = 1.0e-8, rtol = 1.0e-8;
	double sample_time, *sample_times = 0x0, *st, t0;
	char c, buf[1000], *outpre = NULL;
	int argleft, iarg, error = 0;
	int save_file = 0;
	int check_steady_state = 0;
	int seed = -1;
	int remove_zero = 1;
	int print_flux = 0, print_end_net = 0, print_net = 0, enable_species_stats = 0;
	int gillespie_update_interval = 1;
	int verbose=0;
	int continuation=0;
	double *conc, *conc_last, *derivs;
	struct program_times ptimes;
	extern char *optarg;
	extern int optind, opterr;
	// Allowed propagator types
	enum {
		SSA, CVODE, EULER, RKCS
	};
	int propagator = CVODE;
	int SOLVER = DENSE;
	int outtime = -1;

	/* Process input options */
	while ((c = getopt(argc, argv, "a:bcdefg:h:i:jkmno:p:r:st:u:vxz:")) != EOF) {
		switch (c) {
		case 'a':
			atol = atof(optarg);
			break;
		case 'b':
			if (SOLVER == DENSE) {
				SOLVER = GMRES;
			} else {
				SOLVER = GMRES_J;
			}
			break;
		case 'c':
			check_steady_state = 1;
			break;
		case 'd':
			if (SOLVER == DENSE) {
				SOLVER = DENSE_J;
			} else {
				SOLVER = GMRES_J;
			}
			break;
		case 'e':
			print_end_net = 1;
			break;
		case 'f':
			print_flux = 1;
			break;
		case 'g':
			group_input_file_name = optarg;
			break;
		case 'h':
			seed = atoi(optarg);
			break;
		case 'i':
			t_start = atof(optarg);
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
			print_net = 1;
			break;
		case 'o':
			outpre = optarg;
			break;
		case 'p':
			if (strcmp(optarg, "ssa") == 0) {
				propagator = SSA;
			} else if (strcmp(optarg, "cvode") == 0) {
				propagator = CVODE;
			} else if (strcmp(optarg, "euler") == 0) {
				propagator = EULER;
			} else if (strcmp(optarg, "rkcs") == 0) {
				propagator = RKCS;
			} else {
				fprintf(stderr, "ERROR: Unrecognized propagator type %s.\n",
						optarg);
				exit(1);
			}
			break;
		case 'r':
			rtol = atof(optarg);
			break;
		case 's':
			save_file = 1;
			break;
		case 't':
			atol = rtol = atof(optarg);
			break;
		case 'u':
			gillespie_update_interval = atoi(optarg);
			break;
		case 'v': /* Verbose */
			verbose=1;
			break;
		case 'x': /* new flag: continue ('extend') simulation */
			continuation=1;
			break;			
		case 'z':
			outtime = atoi(optarg);
			break;
		case '?':
			++error;
			break;
		}
	}

	/* Check input options for consistency */

	/* Check for correct number of input args */
	iarg = optind;
	argleft = argc - iarg;
	if (argleft < 3)
		++error;

	if (error) {
		char *usage="run_network  [-bcdefkmsvx] [-a atol] [-g groupfile] [-h seed] [-i start_time] [-o outprefix] [-r rtol] [-t tol] [-z iteration number]";
		fprintf(stderr,	"Usage:\n%s netfile sample_time n_sample\n",usage);
		fprintf(stderr, "or\n");
		fprintf(stderr,	"%s netfile t1 t2 ... tn\n", usage);
		exit(1);
	}

	/* Get net file name */
	netfile_name = strdup(argv[iarg++]);

	/* Is the trajectory a continuation of a previous simulation? */
	/* CHANGES: this is now passed as an explicit flag */
	//continuation= (t_start>0) ? 1 : 0;

	/* Process sample times */
	if ((argleft = argc - iarg) == 2) {
		/* input is sample_time n_sample */
		sample_time = atof(argv[iarg++]);
		n_sample = atoi(argv[iarg++]);
	} else {
		/* input is t1 t2 ... tn */
		n_sample = argleft;
		t0 = atof(argv[iarg++]);
		if (t0 == 0)
			--n_sample;
		sample_times = ALLOC_VECTOR(n_sample + 1);
		sample_times[0] = 0.0;
		if (t0 != 0) {
			sample_times[1] = t0;
			st = &sample_times[2];
		} else {
			st = &sample_times[1];
		}
		for (iarg = iarg; iarg < argc; ++iarg, ++st) {
			*st = atof(argv[iarg]);
		}
		/* Check that final array is in ascending order with no negative
		 elements */
		for (i = 0; i <= n_sample; ++i) {
			if (sample_times[i] < 0) {
				fprintf(stderr,
						"ERROR: Negative sample times are not allowed.\n");
				exit(1);
			}
			if (i == 0)	continue;
			if (sample_times[i] <= sample_times[i - 1]) {
				fprintf(stderr,
						"ERROR: Sample times must be in ascending order.\n");
				exit(1);
			}
		}
	}

	if (!(netfile = fopen(netfile_name, "r"))) {
		fprintf(stderr, "Couldn't open file %s.\n", netfile_name);
		exit(1);
	}

	/* Output message */
	fprintf(stdout, "run_network %s\n", RUN_NETWORK_VERSION);
	fflush(stdout);

	/* Assign network_name based on netfile_name */
	network_name = chop_suffix(netfile_name, ".net");
	if (!outpre)
		outpre = network_name;

	line_number = 0;

	/* Rate constants and concentration parameters should now be placed in the
	 parameters block. */
	rates = read_Elt_array(netfile, &line_number, "parameters", &n_read, 0x0);
	fprintf(stdout, "Read %d parameters\n", n_read);
	if (n_read < 1) {
		fprintf(stderr,
				"Reaction network must have parameters defined to be used as rate constants.\n");
		exit(1);
	}
	rewind(netfile);
	
	line_number = 0;

	/* Read species */
	if (!(species = read_Elt_array(netfile, &line_number, "species", &n_read,
			rates)))
		exit(1);
	fprintf(stdout, "Read %d species\n", n_read);

	/* Read reactions */
	if (!(reactions = read_Rxn_array(netfile, &line_number, &n_read, species,
			rates))) {
		fprintf(stderr, "No reactions in the network.\n");
		exit(1);
	}
	fprintf(stdout, "Read %d reaction(s)\n", n_read);
	if (remove_zero)
		remove_zero_rate_rxns(&reactions, rates);
	fprintf(stdout, "%d reaction(s) have nonzero rate\n", reactions->n_rxn);
	/*     sort_Rxn_array( reactions, rates); */
	fclose(netfile);

	/* Read optional groups */
	if (group_input_file_name) {
		if (!(group_file = fopen(group_input_file_name, "r"))) {
			fprintf(stderr, "Couldn't open file %s.\n", group_input_file_name);
			exit(1);
		}
		line_number = 0;
		spec_groups = read_Groups(0x0, group_file, species, &line_number,
				"groups", &n_read);
		fprintf(stdout, "Read %d group(s) from %s\n", n_read,
				group_input_file_name);
		fclose(group_file);
	}

	/* Should add check that reactions, rates, and species are defined */
	/* Also should check that definitions don't exceed array bounds */
	if (n_sample < 1) {
		fprintf(stderr, "n_sample<1\n");
		goto exit;
	}

	/* Initialize reaction network */
	init_network(reactions, rates, species, spec_groups, network_name);
	if (propagator == SSA) {
		init_gillespie_direct_network(gillespie_update_interval, seed);
	}

	/* Save network to file */
	if (save_file) {
		if (outpre) {
			sprintf(buf, "%s.net", outpre);
			save_file_name = strdup(buf);
		} else {
			save_file_name = strdup("save.net");
		}
		if (out = fopen(save_file_name, "w")) {
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

	/* Initialize and print initial concentrations */
	conc_file = init_print_concentrations_network(outpre, continuation);
	if (!continuation) print_concentrations_network(conc_file, t);

	/* Initialize and print initial group concentrations */
	if (spec_groups) {
		group_file = init_print_group_concentrations_network(outpre, continuation);
		if (!continuation) print_group_concentrations_network(group_file, t);
	}

	/* Initialize and print species stats (if enabled) */
	if (enable_species_stats) {
		species_stats_file = init_print_species_stats(outpre, continuation);
		if (!continuation) print_species_stats(species_stats_file, t);
	}

	/* Initialize flux printing (if selected) */
	if (print_flux) {
		flux_file = init_print_flux_network(outpre);
		print_flux_network(flux_file, t);
	}

	fflush(stdout);

	/* Compute time course */
	long int n_steps = 0;
	long int n_steps_last = 0;
	long int n_rate_calls_last = 0;
	long int n_deriv_calls_last = 0;
	for (n = 1; n <= n_sample; ++n) {
		if (sample_times) {
			dt = sample_times[n] - sample_times[n - 1];
			if (dt<0) continue;
		} else {
			dt = sample_time;
		}


		if (n == 1) {
			switch (propagator) {
			case SSA:
				fprintf(stdout,
						"Stochastic simulation using direct Gillespie algorithm\n");
				if (verbose)
					fprintf(stdout, "%15s %8s %12s %7s %7s\n", "time", "n_steps",
						"n_rate_calls", "% spec", "% rxn");
				break;
			case CVODE:
				fprintf(stdout, "Propagating with cvode");
				if (SOLVER == GMRES) {
					fprintf(stdout, " using GMRES\n");
				} else if (SOLVER == GMRES_J) {
					fprintf(stdout,
							" using GMRES with specified Jacobian multiply\n");
				} else if (SOLVER == DENSE_J) {
					fprintf(stdout, " using dense LU with specified Jacobian\n");
				} else {
					fprintf(stdout, " using dense LU\n");
				}
				if (verbose)
					fprintf(stdout, "%15s %16s %13s\n", "time", "n_steps",
						"n_deriv_calls");
				break;
			case EULER:
				fprintf(
						stdout,
						"Propagating with Euler method using fixed time step of %.15g\n",
						rtol);
				if (verbose)
					fprintf(stdout, "%15s %13s %13s\n", "time", "n_steps",
						"n_deriv_calls");
				break;
			case RKCS:
				fprintf(stdout, "Propagating with rkcs\n");
				if (verbose)
					fprintf(stdout, "%15s %13s %13s\n", "time", "n_steps",
						"n_deriv_calls");
				break;
			}
			if (verbose) fflush(stdout);
		}

		/* Do propagation */
		switch (propagator) {
		case SSA:
			error = gillespie_direct_network(&t, dt, 0x0, 0x0);
			if (verbose)
				fprintf(stdout, "%15.2f %8ld %12ld %7.3f %7.3f %6d %6d", t,
					gillespie_n_steps() - n_steps_last, n_rate_calls_network()
							- n_rate_calls_last, 100
							* gillespie_frac_species_active(), 100
							* gillespie_frac_rxns_active(),
					n_species_network(), n_rxns_network());
			n_steps_last = gillespie_n_steps();
			break;
		case CVODE:
			error = propagate_cvode_network(&t, dt, &n_steps, &rtol, &atol,
					SOLVER);
			if (verbose)
				fprintf(stdout, "%15.2f %13ld %13d", t, n_steps,
					n_deriv_calls_network());
			break;
		case EULER:
			error = propagate_euler_network(&t, dt, &n_steps, rtol);
			if (verbose)
				fprintf(stdout, "%15.2f %13ld %13d", t, n_steps,
					n_deriv_calls_network());
			break;
		case RKCS:
			error = propagate_rkcs_network(&t, dt, &n_steps, rtol);
			if (verbose)
				fprintf(stdout, "%15.2f %13ld %13d", t, n_steps,
					n_deriv_calls_network());
			break;
		}
		n_rate_calls_last = n_rate_calls_network();
		n_deriv_calls_last = n_deriv_calls_network();

		if (error) {
			fprintf(stderr, "Stopping due to error in integration.\n");
			exit(1);
		}

		// Print current properties of the system
		print_concentrations_network(conc_file, t);
		if (spec_groups)
			print_group_concentrations_network(group_file, t);
		if (enable_species_stats)
			print_species_stats(species_stats_file, t);
		if (print_flux)
			print_flux_network(flux_file, t);
		if (print_net) {
			if (outpre)
				sprintf(buf, "%s.net", outpre);
			else
				sprintf(buf, "tmp.net");
			out = fopen(buf, "w");
			print_network(out);
			fclose(out);
			fprintf(stdout, " Wrote NET file to %s", buf);
		}

		/* Check if steady state reached */
		if (check_steady_state) {
			double *a, delta, dx;
			get_conc_network(conc);
			delta = sqrt(VECTOR_DIST(conc, conc_last, n_species_network()))
					/ n_species_network();
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
			fprintf(stdout, "Jacobian written to %s after iteration %d\n", buf,
					outtime);
		}
		if (verbose) fflush(stdout);
	}
	finish_print_concentrations_network(conc_file);
	if (spec_groups)
		finish_print_group_concentrations_network(group_file);
	if (enable_species_stats)
		finish_print_species_stats(species_stats_file);

	if (propagator == SSA)
		fprintf(stdout, "TOTAL STEPS: %ld\n", gillespie_n_steps());
	fprintf(stdout, "Time course of concentrations written to file %s.cdat.\n",
			outpre);
	if (n_groups_network()) {
		fprintf(stdout, "Time course of groups written to file %s.gdat.\n",
				outpre);
	}

	ptimes = t_elapsed();
	fprintf(stdout, "Propagation took %.2e CPU seconds\n", ptimes.cpu);

	/* Print final concentrations in species list */
	if (print_end_net) {
		if (outpre)
			sprintf(buf, "%s_end.net", outpre);
		else
			sprintf(buf, "end.net");
		out = fopen(buf, "w");
		print_network(out);
		fclose(out);
		fprintf(stdout, "Final network file written to %s\n", buf);
	}
	ptimes = t_elapsed();
	fprintf(stdout, "Program times:  %.2f CPU s %.2f clock s \n",
			ptimes.total_cpu, ptimes.total_real);

	exit: if (sample_times)
		free(sample_times);
	return (0);
}

