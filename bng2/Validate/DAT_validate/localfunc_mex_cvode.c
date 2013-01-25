/*   
**   localfunc_mex_cvode.c
**	 
**   Cvode-Mex implementation of BioNetGen model 'localfunc'.
**
**   Code Adapted from templates provided by Mathworks and Sundials.
**   QUESTIONS about the code generator?  Email justinshogg@gmail.com
**
**   Requires the CVODE libraries:  sundials_cvode and sundials_nvecserial.
**   https://computation.llnl.gov/casc/sundials/main.html
**
**-----------------------------------------------------------------------------
**
**   COMPILE in MATLAB:
**   mex -L<path_to_cvode_libraries> -I<path_to_cvode_includes>  ...
**          -lsundials_nvecserial -lsundials_cvode -lm localfunc_mex_cvode.c
**
**   note1: if cvode is in your library path, you can omit path specifications.
**
**   note2: if linker complains about lib stdc++, try removing "-lstdc++"
**     from the mex configuration file "gccopts.sh".  This should be in the
**     matlab bin folder.
** 
**-----------------------------------------------------------------------------
**
**   EXECUTE in MATLAB:
**   [error_status, species_out, observables_out]
**        = localfunc_mex_cvode( timepoints, species_init, parameters )
**
**   timepoints      : column vector of time points returned by integrator.
**   parameters      : row vector of 4 parameters.
**   species_init    : row vector of 7 initial species populations.
**
**   error_status    : 0 if the integrator exits without error, non-zero otherwise.
**   species_out     : species population trajectories
**                        (columns correspond to states, rows correspond to time).
**   observables_out : observable trajectories
**                        (columns correspond to observables, rows correspond to time).
*/

/* Library headers */
#include "mex.h"
#include "matrix.h"
#include <stdlib.h>
#include <math.h>
#include <cvode/cvode.h>             /* prototypes for CVODE  */
#include <nvector/nvector_serial.h>  /* serial N_Vector       */
#include <cvode/cvode_dense.h>       /* prototype for CVDense */
#include <cvode/cvode_spgmr.h>       /* prototype for CVSpgmr */

/* Problem Dimensions */
#define __N_PARAMETERS__   4
#define __N_EXPRESSIONS__  4
#define __N_OBSERVABLES__  8
#define __N_RATELAWS__     11
#define __N_SPECIES__      7

/* core function declarations */
void  mexFunction ( int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[] );
int   check_flag  ( void *flagvalue, char *funcname, int opt );
void  calc_expressions ( N_Vector expressions, double * parameters );
void  calc_observables ( N_Vector observables, N_Vector species, N_Vector expressions );
void  calc_ratelaws    ( N_Vector ratelaws,  N_Vector species, N_Vector expressions, N_Vector observables );
int   calc_species_deriv ( realtype time, N_Vector species, N_Vector Dspecies, void * f_data );

/* user-defined function declarations */
double rateLaw1_1 ( N_Vector expressions, N_Vector observables );
double rateLaw1_2 ( N_Vector expressions, N_Vector observables );
double rateLaw1_3 ( N_Vector expressions, N_Vector observables );
double rateLaw1_4 ( N_Vector expressions, N_Vector observables );


/* user-defined function definitions  */
/* user-defined function rateLaw1_1 */
double rateLaw1_1 ( N_Vector expressions, N_Vector observables )
{
    return (NV_Ith_S(expressions,2)*pow(0.0,2.0));
}

/* user-defined function rateLaw1_2 */
double rateLaw1_2 ( N_Vector expressions, N_Vector observables )
{
    return (NV_Ith_S(expressions,2)*pow(1.0,2.0));
}

/* user-defined function rateLaw1_3 */
double rateLaw1_3 ( N_Vector expressions, N_Vector observables )
{
    return (NV_Ith_S(expressions,2)*pow(2.0,2.0));
}

/* user-defined function rateLaw1_4 */
double rateLaw1_4 ( N_Vector expressions, N_Vector observables )
{
    return (NV_Ith_S(expressions,2)*pow(3.0,2.0));
}



/* Calculate expressions */
void
calc_expressions ( N_Vector expressions, double * parameters )
{
    NV_Ith_S(expressions,0) = parameters[0];
    NV_Ith_S(expressions,1) = parameters[1];
    NV_Ith_S(expressions,2) = parameters[2];
    NV_Ith_S(expressions,3) = parameters[3];
   
}

/* Calculate observables */
void
calc_observables ( N_Vector observables, N_Vector species, N_Vector expressions )
{
    NV_Ith_S(observables,0) = NV_Ith_S(species,0) +NV_Ith_S(species,4) +NV_Ith_S(species,5) +NV_Ith_S(species,6);
    NV_Ith_S(observables,1) = NV_Ith_S(species,1) +NV_Ith_S(species,4) +2*NV_Ith_S(species,5) +3*NV_Ith_S(species,6);
    NV_Ith_S(observables,2) = NV_Ith_S(species,2);
    NV_Ith_S(observables,3) = NV_Ith_S(species,0);
    NV_Ith_S(observables,4) = NV_Ith_S(species,4);
    NV_Ith_S(observables,5) = NV_Ith_S(species,5);
    NV_Ith_S(observables,6) = NV_Ith_S(species,6);
    NV_Ith_S(observables,7) = NV_Ith_S(species,4) +2*NV_Ith_S(species,5) +3*NV_Ith_S(species,6);

}

/* Calculate ratelaws */
void
calc_ratelaws ( N_Vector ratelaws, N_Vector species, N_Vector expressions, N_Vector observables )
{  
    NV_Ith_S(ratelaws,0) = rateLaw1_1(expressions,observables)*NV_Ith_S(species,0);
    NV_Ith_S(ratelaws,1) = 3.0*NV_Ith_S(expressions,0)*NV_Ith_S(species,0)*NV_Ith_S(species,1);
    NV_Ith_S(ratelaws,2) = NV_Ith_S(expressions,3)*NV_Ith_S(species,2);
    NV_Ith_S(ratelaws,3) = rateLaw1_2(expressions,observables)*NV_Ith_S(species,4);
    NV_Ith_S(ratelaws,4) = 2.0*NV_Ith_S(expressions,0)*NV_Ith_S(species,4)*NV_Ith_S(species,1);
    NV_Ith_S(ratelaws,5) = NV_Ith_S(expressions,1)*NV_Ith_S(species,4);
    NV_Ith_S(ratelaws,6) = rateLaw1_3(expressions,observables)*NV_Ith_S(species,5);
    NV_Ith_S(ratelaws,7) = NV_Ith_S(expressions,0)*NV_Ith_S(species,5)*NV_Ith_S(species,1);
    NV_Ith_S(ratelaws,8) = 2.0*NV_Ith_S(expressions,1)*NV_Ith_S(species,5);
    NV_Ith_S(ratelaws,9) = rateLaw1_4(expressions,observables)*NV_Ith_S(species,6);
    NV_Ith_S(ratelaws,10) = 3.0*NV_Ith_S(expressions,1)*NV_Ith_S(species,6);

}


/* Calculate species derivates */
int
calc_species_deriv ( realtype time, N_Vector species, N_Vector Dspecies, void * f_data )
{
    int         return_val;
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
                        
    /* calculate derivates */
    NV_Ith_S(Dspecies,0) = NV_Ith_S(ratelaws,5) -NV_Ith_S(ratelaws,1);
    NV_Ith_S(Dspecies,1) = -NV_Ith_S(ratelaws,7) +NV_Ith_S(ratelaws,5) +NV_Ith_S(ratelaws,10) +NV_Ith_S(ratelaws,8) -NV_Ith_S(ratelaws,1) -NV_Ith_S(ratelaws,4);
    NV_Ith_S(Dspecies,2) = NV_Ith_S(ratelaws,3) +NV_Ith_S(ratelaws,0) -NV_Ith_S(ratelaws,2) +NV_Ith_S(ratelaws,6) +NV_Ith_S(ratelaws,9);
    NV_Ith_S(Dspecies,3) = 0;
    NV_Ith_S(Dspecies,4) = -NV_Ith_S(ratelaws,5) +NV_Ith_S(ratelaws,8) +NV_Ith_S(ratelaws,1) -NV_Ith_S(ratelaws,4);
    NV_Ith_S(Dspecies,5) = -NV_Ith_S(ratelaws,7) +NV_Ith_S(ratelaws,10) -NV_Ith_S(ratelaws,8) +NV_Ith_S(ratelaws,4);
    NV_Ith_S(Dspecies,6) = NV_Ith_S(ratelaws,7) -NV_Ith_S(ratelaws,10);


    return(0);
}


/*
**   ========
**   main MEX
**   ========
*/
void mexFunction( int nlhs, mxArray * plhs[], int nrhs, const mxArray * prhs[] )
{
    /* variables */
    double *  return_status;
    double *  species_out;
    double *  observables_out;    
    double *  parameters;
    double *  species_init;
    double *  timepoints; 
    size_t    n_timepoints;
    size_t    i;
    size_t    j;

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

    /* check number of input/output arguments */
    if (nlhs != 3)
    {  mexErrMsgTxt("syntax: [err_flag, species_out, obsv_out] = network_mex( timepoints, species_init, params )");  }
    if (nrhs != 3)
    {  mexErrMsgTxt("syntax: [err_flag, species_out, obsv_out] = network_mex( timepoints, species_init, params )");  }


    /* make sure timepoints has correct dimensions */
    if ( (mxGetM(prhs[0]) < 2)  ||  (mxGetN(prhs[0]) != 1) )
    {  mexErrMsgTxt("TIMEPOINTS must be a column vector with 2 or more elements.");  }

    /* make sure species_init has correct dimensions */
    if ( (mxGetM(prhs[1]) != 1)  ||  (mxGetN(prhs[1]) != __N_SPECIES__) )
    {  mexErrMsgTxt("SPECIES_INIT must be a row vector with 7 elements.");  } 

    /* make sure params has correct dimensions */
    if ( (mxGetM(prhs[2]) != 1)  ||  (mxGetN(prhs[2]) != __N_PARAMETERS__) )
    {  mexErrMsgTxt("PARAMS must be a column vector with 4 elements.");  }

    /* get pointers to input arrays */
    timepoints   = mxGetPr(prhs[0]);
    species_init = mxGetPr(prhs[1]);
    parameters   = mxGetPr(prhs[2]);

    /* get number of timepoints */
    n_timepoints = mxGetM(prhs[0]);

    /* Create an mxArray for output trajectories */
    plhs[0] = mxCreateDoubleMatrix(1, 1, mxREAL );
    plhs[1] = mxCreateDoubleMatrix(n_timepoints, __N_SPECIES__, mxREAL);
    plhs[2] = mxCreateDoubleMatrix(n_timepoints, __N_OBSERVABLES__, mxREAL);

    /* get pointers to output arrays */
    return_status   = mxGetPr(plhs[0]);
    species_out     = mxGetPr(plhs[1]);
    observables_out = mxGetPr(plhs[2]);    
   
    /* initialize intermediate data vectors */
    expressions  = NULL;
    expressions = N_VNew_Serial(__N_EXPRESSIONS__);
    if (check_flag((void *)expressions, "N_VNew_Serial", 0))
    {
        return_status[0] = 1;
        return;
    }

    observables = NULL;
    observables = N_VNew_Serial(__N_OBSERVABLES__);
    if (check_flag((void *)observables, "N_VNew_Serial", 0))
    {
        N_VDestroy_Serial(expressions);
        return_status[0] = 1;
        return;
    }

    ratelaws    = NULL; 
    ratelaws = N_VNew_Serial(__N_RATELAWS__);
    if (check_flag((void *)ratelaws, "N_VNew_Serial", 0))
    {   
        N_VDestroy_Serial(expressions);
        N_VDestroy_Serial(observables);        
        return_status[0] = 1;
        return;
    }
    
    /* set up pointers to intermediate data vectors */
    temp_data[0] = expressions;
    temp_data[1] = observables;
    temp_data[2] = ratelaws;

    /* calculate expressions (expressions are constant, so only do this once!) */
    calc_expressions( expressions, parameters );

        
    /* SOLVE model equations! */
    species   = NULL;
    cvode_mem = NULL;

    /* Set the scalar relative tolerance */
    reltol = 1e-06;
    abstol = 1e-06;

    /* Create serial vector for Species */
    species = N_VNew_Serial(__N_SPECIES__);
    if (check_flag((void *)species, "N_VNew_Serial", 0))
    {  
        N_VDestroy_Serial(expressions);
        N_VDestroy_Serial(observables);
        N_VDestroy_Serial(ratelaws);
        return_status[0] = 1;
        return;
    }
    for ( i = 0; i < __N_SPECIES__; i++ )
    {   NV_Ith_S(species,i) = species_init[i];   }
    
    /* write initial species populations into species_out */
    for ( i = 0; i < __N_SPECIES__; i++ )
    {   species_out[i*n_timepoints] = species_init[i];   }
    
    /* write initial observables populations into species_out */ 
    calc_observables( observables, species, expressions );  
    for ( i = 0; i < __N_OBSERVABLES__; i++ )
    {   observables_out[i*n_timepoints] = NV_Ith_S(observables,i);   }

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
        return_status[0] = 1;          
        return;                        
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
        return_status[0] = 1;          
        return;                        
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
        return_status[0] = 1;          
        return;                        
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
        return_status[0] = 1;          
        return;                        
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
        return_status[0] = 1;          
        return;                        
    }                                  
    
    flag = CVodeSetMaxNumSteps(cvode_mem, 2000);
    if (check_flag(&flag, "CVodeSetMaxNumSteps", 1))
    {                                  
        N_VDestroy_Serial(expressions);
        N_VDestroy_Serial(observables);
        N_VDestroy_Serial(ratelaws);   
        N_VDestroy_Serial(species);    
        CVodeFree(&cvode_mem);         
        return_status[0] = 1;          
        return;                        
    }                                  

    flag = CVodeSetMaxErrTestFails(cvode_mem, 7);
    if (check_flag(&flag, "CVodeSetMaxErrTestFails", 1))
    {                                  
        N_VDestroy_Serial(expressions);
        N_VDestroy_Serial(observables);
        N_VDestroy_Serial(ratelaws);   
        N_VDestroy_Serial(species);    
        CVodeFree(&cvode_mem);         
        return_status[0] = 1;          
        return;                        
    }                                  

    flag = CVodeSetMaxConvFails(cvode_mem, 10);
    if (check_flag(&flag, "CVodeSetMaxConvFails", 1))
    {                                  
        N_VDestroy_Serial(expressions);
        N_VDestroy_Serial(observables);
        N_VDestroy_Serial(ratelaws);   
        N_VDestroy_Serial(species);    
        CVodeFree(&cvode_mem);         
        return_status[0] = 1;          
        return;                        
    }                                  

    flag = CVodeSetMaxStep(cvode_mem, 0.0);
    if (check_flag(&flag, "CVodeSetMaxStep", 1))
    {                                  
        N_VDestroy_Serial(expressions);
        N_VDestroy_Serial(observables);
        N_VDestroy_Serial(ratelaws);   
        N_VDestroy_Serial(species);    
        CVodeFree(&cvode_mem);         
        return_status[0] = 1;          
        return;                        
    }                                  

    /* integrate to each timepoint */
    for ( i=1;  i < n_timepoints;  i++ )
    {
        flag = CVode(cvode_mem, timepoints[i], species, &time, CV_NORMAL);
        if (check_flag(&flag, "CVode", 1))
        {
            N_VDestroy_Serial(expressions);
            N_VDestroy_Serial(observables);           
            N_VDestroy_Serial(ratelaws);
            N_VDestroy_Serial(species);
            CVodeFree(&cvode_mem);
            return_status[0] = 1; 
            return;
        }

        /* copy species output from nvector to matlab array */
        for ( j = 0; j < __N_SPECIES__; j++ )
        {   species_out[j*n_timepoints + i] = NV_Ith_S(species,j);   }
        
        /* copy observables output from nvector to matlab array */
        calc_observables( observables, species, expressions );         
        for ( j = 0; j < __N_OBSERVABLES__; j++ )
        {   observables_out[j*n_timepoints + i] = NV_Ith_S(observables,j);   }      
    }
 
    /* Free vectors */
    N_VDestroy_Serial(expressions);
    N_VDestroy_Serial(observables);  
    N_VDestroy_Serial(ratelaws);        
    N_VDestroy_Serial(species);

    /* Free integrator memory */
    CVodeFree(&cvode_mem);

    return;
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
        mexPrintf( "\nSUNDIALS_ERROR: %s() failed - returned NULL pointer\n", funcname );    
        return(1);
    }

    /* Check if flag < 0 */
    else if (opt == 1)
    {
        errflag = (int *) flagvalue;
        if (*errflag < 0)
        {
            mexPrintf( "\nSUNDIALS_ERROR: %s() failed with flag = %d\n", funcname, *errflag );
            return(1);
        }
    }

    /* Check if function returned NULL pointer - no memory allocated */
    else if (opt == 2 && flagvalue == NULL)
    {
        mexPrintf( "\nMEMORY_ERROR: %s() failed - returned NULL pointer\n", funcname );
        return(1);
    }

    return(0);
}
