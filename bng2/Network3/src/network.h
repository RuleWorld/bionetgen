/*
 * $Id: network.h,v 1.1 2005/09/19 17:57:58 faeder Exp $
 * $Log: network.h,v $
 * Revision 1.1  2005/09/19 17:57:58  faeder
 * Initial revision
 *
 * Revision 1.1  2005-08-09 13:59:21-06  kozdon
 * Initial revision
 *
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <sys/stat.h>
#include <unistd.h>
#include <errno.h>
#include <cstdlib> 
#include <cstring>
#include <cmath>
#include <string>
#include <algorithm>
#include <iostream>
#include <locale>
#include <list>
#include <limits>
#include <ios> 
#include <iomanip>
#include <fstream>
#include <vector>
#include <queue>

using namespace std;

#include <time.h>
#include "muParser.h"
#include "muParserInt.h"
#include "util/util.hh"

#include <cmath>

extern "C" int isnan(double);

// Reaction types
enum {ELEMENTARY, SATURATION, MICHAELIS_MENTEN, HILL, FUNCTIONAL};

/* Basic network structures */

/* Structure used for species and rate constants.  For species, val is the concentration in a consistent set of units
 * (up to user).*/
typedef struct ELT{
	char*			name;
    double      	val;
    int         	index;
    int         	fixed;
    struct ELT*		next;
} Elt;

/* Structure used to hold an array of Elt structures.  Used to speed up finding of an elt by index. */
typedef struct{
	int 			n_elt;
	int 			offset;
    Elt**			elt;
    Elt*			list;
    int 			n_fixed_elts;
    int*			fixed_elts;
} Elt_array;

/* Structure that contains a listing of elt indices that defines a group of elts */
typedef struct GROUP{
	char*			name;
	int         	index;
	int         	n_elt;
	int*			elt_index;
	double*			elt_factor;
	double 	 		total_val;
	struct GROUP*	next;
} Group;

/* Structure used to contain reactions. Indices of r_index and p_index refer to an Elt list (or Elt_array) containing species,
 * while rate_index refers to an Elt list or Elt_array containing rates. */
typedef struct RXN{
	int				index;
	int             n_reactants;
	int             n_products;
	int*			r_index;
	int*			p_index;
	int             rateLaw_type;
	int             n_rateLaw_params;
	double*			rateLaw_params;
	int*			rateLaw_indices;
	double          stat_factor;
	struct RXN*		next;
} Rxn;

/* Structure used to contain an array of reactions that can be quickly accessed by index. */
typedef struct{
	int 			n_rxn;
	int 			offset;
    Rxn**			rxn;
    Rxn*			list;
} Rxn_array;

/* I/O parsing routines */
extern char*  get_line(FILE* infile);
extern char** parse_line (char* buf, int* n_tok, char* comment_chars, char* sep_chars);
extern char*  chop_suffix(char* string, const char* suff);

/* Elt handling routines */
extern Elt* new_Elt(char* name, double val, int index);
extern void add_Elt(Elt** list, Elt* new_elt);
extern void free_Elt(Elt* elt);
extern Elt* lookup_Elt(char* name, Elt* list);
extern void print_Elt_list(FILE* outfile, Elt* elist);

/* Elt_array handling routines */
extern Elt_array* new_Elt_array(Elt* list);
extern void append_Elt_array(Elt_array* earray1, Elt_array* earray2);
extern void free_Elt_array(Elt_array* earray);
extern Elt_array* read_Elt_array(FILE* datfile, int* line_number, char* name, int* n_read, Elt_array* params);

/* Group handling routines */
extern Group* add_Group(Group* glist, char* name, int index, int n_elt, int* elt_index, double* elt_factor);
extern void free_Group(Group* group);
extern Group* read_Groups(Group* glist, FILE* datfile, Elt_array* earray, int* line_number, char* name, int* n_read);
extern void print_Groups(FILE* out, Group* list, Elt_array* earray);

/* Function handling routines */
extern map<string, double*> init_param_map(Elt_array*& rates, Group*& spec_groups);
extern map<string, int> init_param_index_map(Elt_array*& rates);
extern map<string, int> init_observ_index_map(Group*& spec_groups);
extern void process_function_names(string& a);
extern vector<mu::Parser> read_functions_array(const char* netfile, Group* spec_groups, Elt_array*& rates, Elt_array* species,
		vector<int>& var_parameters, map<string, double*>& param_map, map<string, int> param_index_map,
		map<string, int> observ_index_map, vector<vector<int> >& func_observ_depend, vector<vector<int> >& func_param_depend,
		map<string, bool>& is_func_map);
extern void remove_redundancies(vector<int>& vec);

/* Rxn handling routines */
Rxn* new_Rxn (int index, int n_reactants, int n_products, int* r_index, int* p_index, int rateLaw_type, int n_rateLaw_params,
		int* rateLaw_indices, double stat_factor, Elt_array* rates);
extern Rxn* copy_Rxn (Rxn* rxn, int index, Elt_array* rates);
extern void add_Rxn(Rxn** list, Rxn* new_elt);
extern void free_Rxn(Rxn* rxn);
extern void print_Rxn_text(FILE* out, Rxn* rxn, Elt_array* species, Elt_array* rates);

/* Rxn_array handling routines */
extern Rxn_array* new_Rxn_array(Rxn* list);
extern void free_Rxn_array(Rxn_array* rarray);
extern void sort_Rxn_array(Rxn_array* rarray, Elt_array* rates);
extern void set_Rxn_rates(Rxn_array* reactions, Elt_array* rates);
extern void remove_zero_rate_rxns(Rxn_array** reactions, Elt_array* rates);
extern Rxn_array* read_Rxn_array(FILE* datfile, int* line_number, int* n_read, Elt_array* species, Elt_array* rates,
		map<string,bool>, int&);
extern void print_Rxn_array(FILE* outfile, Rxn_array* reactions, Elt_array* species, Elt_array* rates);

/* Network interface */
struct NETWORK{
	char*					name;
	Rxn_array*				reactions;
	Elt_array*				rates;
	Elt_array*				species;
	int       	 			n_groups;
	Group*					spec_groups;
	vector<GROUP*> 			spec_groups_vec;
	int 					n_deriv_calls;
	int 					n_rate_calls;
	map<string, bool> 		is_func_map;
	void (*derivs)(double, double*, double*);
	vector<mu::Parser> 		functions;
	vector<int> 			var_parameters;
	vector<vector<int> > 	func_observ_depend;
	vector<vector<int> > 	func_param_depend;
	bool has_functions; // true iff at least 1 rxn with (user defined) functional rate law
};

extern struct NETWORK network;
enum {DENSE, GMRES, DENSE_J, GMRES_J};
extern void  sparse_jac_matlab(FILE* outfile);
extern void  init_sparse_matlab_file(FILE* outfile);
extern int   init_network(Rxn_array* reactions, Elt_array* rates, Elt_array* species, Group* spec_groups, char* name,
						  vector<mu::Parser> new_functions, vector<int> new_var_parameters, vector<vector<int> > func_observ_depend,
						  vector<vector<int> > func_param_depend, map<string, bool>);
extern int   n_rate_calls_network();
extern int   n_deriv_calls_network();
extern int   n_rxns_network();
extern int   n_species_network();
extern int   n_species_active();
extern int   n_species_ever_populated();
extern int   n_rate_constants_network();
extern int   n_groups_network();
extern void  c_code_network();
extern int   get_conc_network(double* conc);
extern int   set_conc_network(double* conc);
extern double* get_group_concentrations_network();
extern void  derivs_network(double t, double* conc, double* derivs);
extern int   rxn_rates_network(double* rxn_rates);
extern int   derivs_byrxn_network(double* conc, double** derivs);
extern int   print_derivs_network(FILE* out);
extern int   print_derivs_species_network(FILE* out);
extern int   print_rates_network(FILE* out);
extern int   print_derivs_byrxn_network(FILE* out);
extern int   propagate_cvode_network(double* t, double delta_t, long int* n_steps, double* rtol, double* atol,
									 int SOLVER_TYPE, long maxStep);
extern int   propagate_euler_network(double* t, double delta_t, long int* n_steps, double h, long maxStep);
extern int   propagate_rkcs_network (double* t, double delta_t, long int* n_steps, double tol, long maxStep);

extern FILE* init_print_concentrations_network(char* prefix, int append);
extern int   print_concentrations_network(FILE* out, double t);
extern int   finish_print_concentrations_network(FILE* out);

extern FILE* init_print_group_concentrations_network(char* prefix, int append);
extern int   print_group_concentrations_network(FILE* out, double t);
extern int   finish_print_group_concentrations_network(FILE* out);

extern FILE* init_print_function_values_network(char* prefix, int append);
extern int   print_function_values_network(FILE* out, double t);
extern int   finish_print_function_values_network(FILE* out);

extern int   print_network(FILE* out);
extern void  print_species_network(FILE* out);

extern FILE* init_print_species_stats(char* prefix, int append);
extern int   print_species_stats(FILE* out, double t);
extern int   finish_print_species_stats(FILE* out);

extern void  connectivity_Rxn_array(Rxn_array* reactions, Elt_array* species, int** as_reactant, int** as_product);
extern FILE* init_print_flux_network(char* filename);
extern int   print_flux_network(FILE* out, double t);
extern FILE* init_print_pc_network(char* filename);
extern int   print_pc_network(FILE* out, double t);
extern FILE* init_print_jac_network(char* filename);
extern int   print_jac_network(FILE* out, double t);
extern FILE* init_print_pca_network(char* filename);
extern int   print_pca_network(FILE* out, double t);

/* Gillespie Monte Carlo functions */
extern int      init_gillespie_direct_network(int update_interval, int seed);
extern int      gillespie_direct_network(double* t, double delta_t, double* C_avg, double* C_sig, long maxStep);
extern long int gillespie_n_steps();
extern double   gillespie_frac_species_active();
extern double   gillespie_frac_rxns_active();
extern void		delete_GSP_included();
