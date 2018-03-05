/* Library headers */
#include <stdlib.h>
#include <math.h>
#include <cvode/cvode.h>             /* prototypes for CVODE  */
#include <nvector/nvector_serial.h>  /* serial N_Vector       */
#include <cvode/cvode_dense.h>       /* prototype for CVDense */
#include <cvode/cvode_spgmr.h>       /* prototype for CVSpgmr */

/* Problem Dimensions */
#define __N_PARAMETERS__   2
#define __N_EXPRESSIONS__  2
#define __N_OBSERVABLES__  1
#define __N_RATELAWS__     2
#define __N_SPECIES__      1

/* core function declarations */
int   check_flag  ( void *flagvalue, char *funcname, int opt );
void  calc_expressions ( N_Vector expressions, double * parameters );
void  calc_observables ( N_Vector observables, N_Vector species, N_Vector expressions );
void  calc_ratelaws    ( N_Vector ratelaws,  N_Vector species, N_Vector expressions, N_Vector observables );
int   calc_species_deriv ( realtype time, N_Vector species, N_Vector Dspecies, void * f_data );
double* initialize_species(double* params,int nspecies);
double * integrate(double* timepoints, double* species_init, double* params, int num_eq, int n_timepoints);
/* user-defined function declarations */


/* user-defined function definitions  */


/* Calculate expressions */
void
calc_expressions ( N_Vector expressions, double * parameters )
{
    NV_Ith_S(expressions,0) = parameters[0];
    NV_Ith_S(expressions,1) = parameters[1];
   
}

/* Calculate observables */
void
calc_observables ( N_Vector observables, N_Vector species, N_Vector expressions )
{
    NV_Ith_S(observables,0) = NV_Ith_S(species,0);

}

/* Calculate ratelaws */
void
calc_ratelaws ( N_Vector ratelaws, N_Vector species, N_Vector expressions, N_Vector observables )
{  
    NV_Ith_S(ratelaws,0) = NV_Ith_S(expressions,0);
    NV_Ith_S(ratelaws,1) = NV_Ith_S(expressions,1)*NV_Ith_S(species,0);

}

/* Calculate species derivatives */
int
calc_species_deriv ( realtype time, N_Vector species, N_Vector Dspecies, void * f_data )
{
    N_Vector *  temp_data;
    
    N_Vector    expressions;
    N_Vector    observables;
    N_Vector    ratelaws;

    /* cast temp_data */
    temp_data = (N_Vector*)f_data;
     
    /* sget ratelaws Vector */
    expressions = temp_data[0];
    observables = temp_data[1];
    ratelaws    = temp_data[2];
       
    /* calculate observables */
    calc_observables( observables, species, expressions );
    
    /* calculate ratelaws */
    calc_ratelaws( ratelaws, species, expressions, observables );
                        
    /* calculate derivatives */
    NV_Ith_S(Dspecies,0) = NV_Ith_S(ratelaws,0) -NV_Ith_S(ratelaws,1);

    return(0);
}

//function to return pointer to integrated species output
double * integrate(double* timepoints, double* species_init, double* params, int num_eq, int n_timepoints)
{
    size_t i;
    size_t j;
    double *species_out = (double *)malloc((n_timepoints+1)*(num_eq+1)* sizeof(double));
    int counter = 0;
    //[n_timepoints][num_eq+1];
    /* intermediate data vectors */
    N_Vector  expressions;
    N_Vector  observables;
    N_Vector  ratelaws;
    /* array to hold pointers to data vectors */
    N_Vector  temp_data[3];
    /* CVODE specific variables */
    realtype  reltol;
    realtype  abstol;
    realtype  time;
    N_Vector  species;
    void *    cvode_mem;
    int       flag; 
    /* initialize intermediate data vectors */
    expressions  = NULL;
    expressions = N_VNew_Serial(__N_EXPRESSIONS__);
    /*if (check_flag((void *)expressions, "N_VNew_Serial", 0))
    {
        return 1;
    }*/
    observables = NULL;
    observables = N_VNew_Serial(__N_OBSERVABLES__);
    
    /*if (check_flag((void *)observables, "N_VNew_Serial", 0))
    {
        N_VDestroy_Serial(expressions);
        return 1;
    }*/
    ratelaws    = NULL; 
    ratelaws = N_VNew_Serial(__N_RATELAWS__);
    /*
    if (check_flag((void *)ratelaws, "N_VNew_Serial", 0))
    {   
        N_VDestroy_Serial(expressions);
        N_VDestroy_Serial(observables);        
        return 1;
    } */ 
    /* set up pointers to intermediate data vectors */
    temp_data[0] = expressions;
    temp_data[1] = observables;
    temp_data[2] = ratelaws;
    /* calculate expressions (expressions are constant, so only do this once!) */
    calc_expressions( expressions, params );       
    /* SOLVE model equations! */
    species   = NULL;
    cvode_mem = NULL;
    /* Set the scalar relative tolerance */
    reltol = 1e-08;
    abstol = 1e-06;
    /* Create serial vector for Species */
    species = N_VNew_Serial(__N_SPECIES__);
    if (check_flag((void *)species, "N_VNew_Serial", 0))
    {  
        N_VDestroy_Serial(expressions);
        N_VDestroy_Serial(observables);
        N_VDestroy_Serial(ratelaws);
        //return 1;
    }
    for ( i = 0; i < __N_SPECIES__; i++ )
    {   NV_Ith_S(species,i) = species_init[i];   }

    /*   Call CVodeCreate to create the solver memory:    
     *   CV_ADAMS or CV_BDF is the linear multistep method
     *   CV_FUNCTIONAL or CV_NEWTON is the nonlinear solver iteration
     *   A pointer to the integrator problem memory is returned and stored in cvode_mem.
     */
    cvode_mem = CVodeCreate(CV_BDF, CV_NEWTON);
    if (check_flag((void *)cvode_mem, "CVodeCreate", 0))
    {                                  
        N_VDestroy_Serial(expressions);
        N_VDestroy_Serial(observables);
        N_VDestroy_Serial(ratelaws);   
        N_VDestroy_Serial(species);    
        CVodeFree(&cvode_mem);         
        //return 1;                      
    }                                  
    /*   Call CVodeInit to initialize the integrator memory:     
     *   cvode_mem is the pointer to the integrator memory returned by CVodeCreate
     *   rhs_func  is the user's right hand side function in y'=f(t,y)
     *   T0        is the initial time
     *   y         is the initial dependent variable vector
     */
    flag = CVodeInit(cvode_mem, calc_species_deriv, timepoints[0], species);
    if (check_flag(&flag, "CVodeInit", 1))
    {                                  
        N_VDestroy_Serial(expressions);
        N_VDestroy_Serial(observables);
        N_VDestroy_Serial(ratelaws);   
        N_VDestroy_Serial(species);    
        CVodeFree(&cvode_mem);         
       // return 1;                      
    }                                    
    /* Set scalar relative and absolute tolerances */
    flag = CVodeSStolerances(cvode_mem, reltol, abstol);
    if (check_flag(&flag, "CVodeSStolerances", 1))
    {                                  
        N_VDestroy_Serial(expressions);
        N_VDestroy_Serial(observables);
        N_VDestroy_Serial(ratelaws);   
        N_VDestroy_Serial(species);    
        CVodeFree(&cvode_mem);         
    //    return 1;                      
    }                                        
    /* pass params to rhs_func */
    flag = CVodeSetUserData(cvode_mem, &temp_data);
    if (check_flag(&flag, "CVodeSetFdata", 1))
    {                                  
        N_VDestroy_Serial(expressions);
        N_VDestroy_Serial(observables);
        N_VDestroy_Serial(ratelaws);   
        N_VDestroy_Serial(species);    
        CVodeFree(&cvode_mem);         
        //return 1;                      
    }                                     
    /* select linear solver */
    flag = CVDense(cvode_mem, __N_SPECIES__);
    if (check_flag(&flag, "CVDense", 1))
    {                                  
        N_VDestroy_Serial(expressions);
        N_VDestroy_Serial(observables);
        N_VDestroy_Serial(ratelaws);   
        N_VDestroy_Serial(species);    
        CVodeFree(&cvode_mem);         
        //return 1;                      
    }                                     
    flag = CVodeSetMaxNumSteps(cvode_mem, 2000);
    if (check_flag(&flag, "CVodeSetMaxNumSteps", 1))
    {                                  
        N_VDestroy_Serial(expressions);
        N_VDestroy_Serial(observables);
        N_VDestroy_Serial(ratelaws);   
        N_VDestroy_Serial(species);    
        CVodeFree(&cvode_mem);         
       // return 1;                      
    }                                  
    flag = CVodeSetMaxErrTestFails(cvode_mem, 7);
    if (check_flag(&flag, "CVodeSetMaxErrTestFails", 1))
    {                                  
        N_VDestroy_Serial(expressions);
        N_VDestroy_Serial(observables);
        N_VDestroy_Serial(ratelaws);   
        N_VDestroy_Serial(species);    
        CVodeFree(&cvode_mem);         
        //return 1;                      
    }                                  
    flag = CVodeSetMaxConvFails(cvode_mem, 10);
    if (check_flag(&flag, "CVodeSetMaxConvFails", 1))
    {                                  
        N_VDestroy_Serial(expressions);
        N_VDestroy_Serial(observables);
        N_VDestroy_Serial(ratelaws);   
        N_VDestroy_Serial(species);    
        CVodeFree(&cvode_mem);         
        //return 1;                      
    }                                  
    flag = CVodeSetMaxStep(cvode_mem, 0.0);
    if (check_flag(&flag, "CVodeSetMaxStep", 1))
    {                                  
        N_VDestroy_Serial(expressions);
        N_VDestroy_Serial(observables);
        N_VDestroy_Serial(ratelaws);   
        N_VDestroy_Serial(species);    
        CVodeFree(&cvode_mem);         
       // return 1;                      
    }                   
    //initializing output species array. First column will be time.               
    species_out[counter] = timepoints[0]; 
    //printf("%.16e\t",species_out[counter]);
    counter++;
    for(j=0;j<__N_SPECIES__;j++)
    {
        species_out[counter] = NV_Ith_S(species,j);
        //printf("%.16e\t",species_out[counter]);
        counter++;
    }
    //printf("\n");
    /* integrate to each timepoint */
    for ( i=1;  i <= n_timepoints;  i++ )
    {
        flag = CVode(cvode_mem, timepoints[i], species, &time, CV_NORMAL);
        if (check_flag(&flag, "CVode", 1))
        {
            N_VDestroy_Serial(expressions);
            N_VDestroy_Serial(observables);           
            N_VDestroy_Serial(ratelaws);
            N_VDestroy_Serial(species);
            CVodeFree(&cvode_mem);
            //return 1;
        }  
        species_out[counter] = timepoints[i];
        //printf("%.16e\t",species_out[counter]);
        counter++;
        for(j=0;j<__N_SPECIES__;j++)
        {
            species_out[counter] = NV_Ith_S(species,j);
            //printf("%.16e\t",species_out[counter]);
            counter++;
        }
        //printf("\n");
    }
    /* Free vectors */
    N_VDestroy_Serial(expressions);
    N_VDestroy_Serial(observables);  
    N_VDestroy_Serial(ratelaws);        
    N_VDestroy_Serial(species);
    /* Free integrator memory */
    CVodeFree(&cvode_mem);
    return species_out;
}

int main(int argc,char** argv)
{
    size_t    i;
    //size_t    j;   
    /* user (command line input) variables */
    /*
    double t0 = atof(argv[1]);
    double tend = atof(argv[2]);
    size_t n_timepoints = atoi(argv[3]);
    size_t num_eq = atoi(argv[4]);  
    size_t counter=5;
    double species_init_array[num_eq];
    for(i=0;i<num_eq;i++)
    {
    	species_init_array[i]=atof(argv[5+i]);
    	counter++;
    }
    double *  species_init = species_init_array;
    size_t num_par = argc-1-4-num_eq;
    double parameters_array[num_par];
    for(i=0;i<num_par;i++)
    {
        parameters_array[i] = atof(argv[counter+i]);
    }
    double *  parameters = parameters_array;   
    double timepoints_array[n_timepoints];
    timepoints_array[0] = t0;
    for(i=1;i<=n_timepoints;i++)
    {
    	timepoints_array[i] = i*tend/(n_timepoints-1);
    }
    double *  timepoints = timepoints_array; 
    */
    double params[2] = {1,1};
    double t0 = 0;
    double tend = 10;
    size_t n_timepoints = 20;
    double timepoints[n_timepoints];
    int nspecies = 1;
    for(i=1;i<=n_timepoints;i++)
    {
        timepoints[i] = t0+i*tend/(n_timepoints);
    }
    double *species_init = initialize_species(params,nspecies);
    double *species_out = integrate(timepoints, species_init, params,nspecies,n_timepoints);
    int j;
    int counter=0;
    for(i=0;i<n_timepoints+1;i++)
    {
        for(j=0;j<nspecies+1;j++)
        {
            printf("%.16e\t",species_out[counter]);
            counter++;
        }
        printf("\n");
    }
    return 0;
}

double* initialize_species(double* params,int nspecies)
{
    double* species_init = malloc(nspecies*sizeof(*species_init));
    species_init[0] = 0;
    return species_init;
}

/*  Check function return value...
 *   opt == 0 means SUNDIALS function allocates memory so check if
 *            returned NULL pointer
 *   opt == 1 means SUNDIALS function returns a flag so check if
 *            flag >= 0
 *   opt == 2 means function allocates memory so check if returned
 *            NULL pointer 
 */
int check_flag(void *flagvalue, char *funcname, int opt)
{
    int *errflag;

    /* Check if SUNDIALS function returned NULL pointer - no memory allocated */
    if (opt == 0 && flagvalue == NULL)
    {
        printf( "\nSUNDIALS_ERROR: %s() failed - returned NULL pointer\n", funcname );    
        return(1);
    }

    /* Check if flag < 0 */
    else if (opt == 1)
    {
        errflag = (int *) flagvalue;
        if (*errflag < 0)
        {
            printf( "\nSUNDIALS_ERROR: %s() failed with flag = %d\n", funcname, *errflag );
            return(1);
        }
    }

    /* Check if function returned NULL pointer - no memory allocated */
    else if (opt == 2 && flagvalue == NULL)
    {
        printf( "\nMEMORY_ERROR: %s() failed - returned NULL pointer\n", funcname );
        return(1);
    }

    return(0);
}
