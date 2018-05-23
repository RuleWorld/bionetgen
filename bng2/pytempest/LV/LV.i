%module LV
%include "cpointer.i"
%pointer_class(int,intp);
%pointer_functions(int, intp1);
%pointer_cast(int *, unsigned int *, int_to_uint);
%include "carrays.i"
%array_functions(double, doubleArray);
%array_class(double, doubleArrayClass);
%{
/* Library headers */
#include <stdlib.h>
#include <math.h>
#include <cvode/cvode.h>             /* prototypes for CVODE  */
#include <nvector/nvector_serial.h>  /* serial N_Vector       */
#include <cvode/cvode_dense.h>       /* prototype for CVDense */
#include <cvode/cvode_spgmr.h>       /* prototype for CVSpgmr */

/* Problem Dimensions */
#define __N_PARAMETERS__   3
#define __N_EXPRESSIONS__  3
#define __N_OBSERVABLES__  2
#define __N_RATELAWS__     3
#define __N_SPECIES__      2

/* core function declarations */
int   check_flag  ( void *flagvalue, char *funcname, int opt );
void  calc_expressions ( N_Vector expressions, double * parameters );
void  calc_observables ( N_Vector observables, N_Vector species, N_Vector expressions );
void  calc_ratelaws    ( N_Vector ratelaws,  N_Vector species, N_Vector expressions, N_Vector observables );
int   calc_species_deriv ( realtype time, N_Vector species, N_Vector Dspecies, void * f_data );
double* initialize_species(double* params,int nspecies);
double * integrate(double* timepoints, double* species_init, double* params, int num_eq, int n_timepoints);
double* bng_protocol(double* params);
%}
/* Library headers */
#include <stdlib.h>
#include <math.h>
#include <cvode/cvode.h>             /* prototypes for CVODE  */
#include <nvector/nvector_serial.h>  /* serial N_Vector       */
#include <cvode/cvode_dense.h>       /* prototype for CVDense */
#include <cvode/cvode_spgmr.h>       /* prototype for CVSpgmr */

/* Problem Dimensions */
#define __N_PARAMETERS__   3
#define __N_EXPRESSIONS__  3
#define __N_OBSERVABLES__  2
#define __N_RATELAWS__     3
#define __N_SPECIES__      2

/* core function declarations */
int   check_flag  ( void *flagvalue, char *funcname, int opt );
void  calc_expressions ( N_Vector expressions, double * parameters );
void  calc_observables ( N_Vector observables, N_Vector species, N_Vector expressions );
void  calc_ratelaws    ( N_Vector ratelaws,  N_Vector species, N_Vector expressions, N_Vector observables );
int   calc_species_deriv ( realtype time, N_Vector species, N_Vector Dspecies, void * f_data );
double* initialize_species(double* params,int nspecies);
double * integrate(double* timepoints, double* species_init, double* params, int num_eq, int n_timepoints);
double* bng_protocol(double* params);
