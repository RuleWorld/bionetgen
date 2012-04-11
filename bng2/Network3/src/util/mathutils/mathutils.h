/* Header file for math utilities and macros--- many of the routines */
/* call NAG and/or BLAS/LAPACK routines so the NAG library must be */
/* present to compile with these utilities */
/* VERSION 1.00  -jrf 12/16/93 */

/* only include this header information once */
#ifndef INCLUDED_MATHUTILS

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

/* macro(s) for defining FORTRAN subroutine names */
#ifdef APPEND_UNDERSCORE
#define FNAME(fun) fun##_
#else
#define FNAME(fun) fun
#endif


/* define complex data type--- compatible with fortran */
typedef struct{ double r, i;} dcomplex;

extern dcomplex COMPLEX_ZERO; /* defined in mathutils.c */

/* LAPACK scratch storage variable */
#define BLOCKSIZE 1 /* suitable for scalar machines */

/* structure containing lapack options */
typedef struct {
    char uplo;
    char jobz;
    int itype;
} LAPACK_OPTIONS;

/* Series of simple matrix utililites for real and complex square matrices 
                            -jrf 1/23/93       
                            revised -jrf 8/24/93       
			    revised 12/16/93 to use BLAS and/or NAG
			       routines 
			    
   Key:  a,b,c,... are matrices; x,y,z,... are vectors; dim is
           dimension of the system.

   Contents:                                             
   
   double *ALLOC_VECTOR(n)              allocates memory and returns
                                         pointer to n-length vector.
   double *REALLOC_VECTOR( vec, newdim) reallocates double vec to be of size newdim
                                        while preserving its contents.
   int  *IALLOC_VECTOR(n)               allocates memory and returns
                                         pointer to n-length int vector.
   int *REIALLOC_VECTOR( vec, newdim)   reallocates integer vec to be of size newdim
                                        while preserving its contents.
   dcomplex *ZALLOC_VECTOR(n)           allocates memory and returns
                                         pointer to n-length complex vector.
   FREE_VECTOR(x)                       frees memory for vector x.
   ZFREE_VECTOR(x)                      frees memory for complex vector x.
   INIT_VECTOR(a,val,dim)               initializes vector a with val. 
   IINIT_VECTOR(a,val,dim)              initializes int vector a with int val. 
   ZINIT_VECTOR(a,val,dim)              initializes complex vector a
                                         with complex val. 
   COPY_VECTOR(a,b,dim)                 copies vector a to b.
   ZCOPY_VECTOR(a,b,dim)                copies vector a to b (complex).
   VECTOR_ADD(a, lambda, b, c, dim)     compute c = a + lambda*b
   double DOTP(a,b,dim)                 returns a.b.
   dcomplex ZDOTC(a,b,dim)              returns conj(a).b for complex data.
   dcomplex ZDOTU(a,b,dim)              returns a.b for complex data.
   double VECTOR_DIST(a,b,dim)          returns (a-b)^2.
   double NORM(a,dim)                   returns sqrt of sum of squares
                                         of the elts of a
   double ZNORM(a,dim)                   returns sqrt of sum of squares
                                         of the elts of a (sqrt(conj(a).a))
   double NORMSQ(a,dim)                 returns sum of squares
                                         of the elts of a
   double ZNORMSQ(a,dim)                returns sum of squares
                                         of the elts of a (sqrt(conj(a).a))
   SCALE_VECTOR(alpha,d,dim)   		multiples a real vector by a
                                         scalar alpha  		
   ZSCALE_VECTOR(alpha,a,dim)           multiples a complex vector by a
                                         complex scalar alpha  		
   ZRSCALE_VECTOR(alpha,a,dim)          multiples a complex vector by a
                                         real scalar alpha  		
   double SUM_VECTOR(a,dim)             returns sum of elts of vector
   dcomplex ZSUM_VECTOR(a,dim)          returns sum of elts of complex vector
   double **ALLOC_MATRIX(n,m)           allocates memory and returns
                                         pointer to nXm matrix.
   int  **IALLOC_MATRIX(n,m)            allocates memory and returns
                                         pointer to nXm int matrix.
   dcomplex **ZALLOC_MATRIX(n,m)        allocates memory and returns
                                         pointer to nXm complex matrix.
   FREE_MATRIX(a)                       frees memory for matrix a
   IFREE_MATRIX(a)                      frees memory for matrix a
   ZFREE_MATRIX(a)                      frees memory for complex matrix a.
   double ***ALLOC_MATRIX_ARRAY(l,n,m)  allocates memory and returns
					 pointer to an array of l nXm
					 matrices.   
   int ***IALLOC_MATRIX_ARRAY(l,n,m)    allocates memory and returns
					 pointer to an int array of l nXm
					 matrices.   
   dcomplex ***ZALLOC_MATRIX_ARRAY(l,n,m)  allocates memory and returns
					 pointer to an array of l
					 complex nXm matrices.   
   FREE_MATRIX_ARRAY(A, n_mat)         frees memory for matrix array A.
   IFREE_MATRIX_ARRAY(A, n_mat)        frees memory for int matrix array A.
   ZFREE_MATRIX_ARRAY(A, n_mat)        frees memory for complex matrix
                                         array A.
   MATRIX_TRANSPOSE(a,dim)             compute at	  
   double **RECT_TRANSPOSE(A,m,n)      returns At of mxn matrix
   ZMATRIX_TRANSPOSE(a,dim)            compute at for complex a
   MATRIX_ADD(a,b,c,dim)               a+b=c
   ZMATRIX_ADD(a,b,c,dim)              a+b=c
   MATRIX_SUB(a,b,c,dim)               a-b=c
   ZMATRIX_SUB(a,b,c,dim)              a-b=c
   MATRIX_MULT(a,b,c,dim)              a*b=c
   ZMATRIX_MULT(a,b,c,dim)             a*b=c, complex matrices
   SYM_MATRIX_MULT(a,b,c,dim)          a*b=c, where a is symmetric.
   HERM_MATRIX_MULT(a,b,c,dim)         a*b=c, where a is hermitian.
   MATRIX_VECTOR_MULT(a,x,b,dim)        a*x=b
   ZMATRIX_VECTOR_MULT(a,x,b,dim)       a*x=b for complex data.
   SYM_MATRIX_VECTOR_MULT(a,x,b,dim)    a*x=b, where a is symmetric
   HERM_MATRIX_VECTOR_MULT(a,x,b,dim)   a*x=b, where a is hermitian.
   double MATRIX_ELT(x,a,y,dim)         returns x.a.y, where a is symmetric.
   SYM_EIGENSYSTEM(a,evals,dim,opt)         returns the eigenvalues and
                                         eigenvectors of real symmetric
					 matrix. 
                                         The eigenvectors are returned
					 by row in a. Only the upper
					 triangle is referenced.  Opt may be
					 used to change arguments to LAPACK
					 routine. 
   SYM_LINSOLVE(a,b,nrhs,dim)            Solve set of linear equations 
                                          a.x=b where a is symmetric.  a
					  is overwritten by an
					  appropriate factorization of a
					  and b is overwritten by x.
   ZSYM_LINSOLVE(a,b,nrhs,dim)           Solve COMPLEX set of linear equations 
                                          a.x=b where a is symmetric.  a
					  is overwritten by an
					  appropriate factorization of a
					  and b is overwritten by x.
   SYMPOS_LINSOLVE(a,b,nrhs,dim)         Solve set of linear eqns a.x=b
                                          where a is symmetric and
					  positive definite.  a
					  is overwritten by an
					  appropriate factorization of a
					  and b is overwritten by x.
   LINSOLVE(a,b,nrhs,dim)                Solve set of linear equations 
                                          a.x=b a is overwritten by an
					  appropriate factorization of a
					  and b is overwritten by x.
   SVD(A,m,n,w,V)                        Computes SVD of A (mxn matrix)
                                          overwriting A with U.  The SVD is
					  A = U.diag(w).Vt
   SVBKSB( u, w, v, m, n, b, x)          Solves A.x=b following SVD of A =
                                         U.diag(w).Vt.  u and v are the svd  
					 matrices returned by SVD, and w
					 contains the weights.  Values of w
					 below some user determined threshold
					 should be set to zero to remove the 
					 ill-conditioned parts.  No input
					 quantities are altered.
   GRAM_SCHMIDT(V, m, n)                 Orthonormalizes m n-dimensional
                                           vectors stored in V. m<=n.
					   Be careful using this!  SVD is a
					   more reliable way to do this (but
					   slower). 
   PROJECT(v, n, basis, m)               Calculates (I - P).v where I is a
                                           prection operator defined by the m
					   basis vectors.  basis is an mxn
					   matrix . v is a vector with n elts. 
   SEED_RANDOM(n)                        Seed random number generator.
   RANDOM(min,max)			 Generates a uniformly random
                                          number on the interval (min,
					  max)  
   SPLINE_INIT( x, y, n, yp1, ypn, y2)   Does a cubic spline fit to the
                                          function y(x) with the
					  derivatives yp1 and ypn on the
					  endpoints.  y2 contains the
					  value of the 2nd derivative of
					  the interpolant on the x grid,
					  which is all that's needed to
					  evaluate the function between
					  x[0] and x[n-1]
   SPLINE_EVAL( xa, ya, y2a, n, x, y, dy, d2y)  
                                         Evaluates the interpolating
					  function determined by
					  SPLINE_INIT and its first two
					  derivatives.  
   MIN_BRACKET( a, b, c, fa, fb, fc, f)	 Given initial points a and b,
                                          attempts to bracket minimum by
					  searching in the downhill direction.
					  Bracketing abscissas and function
					  values are returned.
   double MIN_FIND( a, b, c, f, tol, xmin)      
                                         Finds the minimum of the function f.
                                          a, b, and c are the abscissas which
					  bracket the min, tol is the
					  fractional accuracy desired, and
					  xmin is the abscissa of the returned
					  minimum.
   int ROOT_BRACKET(a, b, f)             Attempts to bracket a root by
                                          expanding a and b.  Returns 1 if
					  successful, zero otherwise.
   double ROOT_FIND(a, b, f, tol)        Finds the root bracketed by a and b.
                                          The root must be bracketed or the
					  function returns an error. tol is
					  the fractional accuracy.
   double INTEGRATE(f, a, b, tol)        Integrates the smooth, nonsingular
                                          function f using a Romberg algorithm
					  returning the value of the integral.
   RKCK(y, dydx, n, x, h, yout, yerr, derivs)
                                         Takes a Runge-Kutta 5th order step
					  and returns error estimate.  See
					  runge-kutta.c for more details.
   RKQS(y, dydx, n, x, htry, eps, yscal, hdid, hnext, derivs)
                                         Takes a Runge-Kutta 5th order step
					  with accuracy specified by eps.  See
					  runge-kutta.c for more details.
   MATHERR( string, flags)               Math library error handler.
                                          Exits to system with message
					  string to stderr.
*/

/* Prototype declarations for utils in mathutils.c */

/* LINEAR ALGEBRA ROUTINES */
extern double *ALLOC_VECTOR( int n);
extern double *REALLOC_VECTOR( double *vec, int newdim);
extern int *IALLOC_VECTOR( int n);
extern int *REIALLOC_VECTOR( int *vec, int newdim);
extern dcomplex *ZALLOC_VECTOR( int n);
extern void FREE_VECTOR(double *v);
extern void ZFREE_VECTOR(dcomplex *v);
extern void INIT_VECTOR(double *a, double val, int dim);
extern void IINIT_VECTOR(int *a, int val, int dim);
extern void ZINIT_VECTOR(dcomplex *a, dcomplex val, int dim);
extern void COPY_VECTOR(double *a, double *b, int dim);
extern void ZCOPY_VECTOR(dcomplex *a, dcomplex *b, int dim);
extern void VECTOR_ADD( double *a, double lambda, double *b, double *c, int
			dim);
extern double DOTP(double *a, double *b, int dim); 
extern dcomplex ZDOTC(dcomplex *a, dcomplex *b, int dim);
extern dcomplex ZDOTU(dcomplex *a, dcomplex *b, int dim);
extern double VECTOR_DIST( double *a, double *b, int dim);
extern double NORM( double *a, int dim);
extern double ZNORM( dcomplex *a, int dim);
extern double NORMSQ( double *a, int dim);
extern double ZNORMSQ( dcomplex *a, int dim);
extern void SCALE_VECTOR( double alpha, double *a, int dim);
extern void ZSCALE_VECTOR( dcomplex alpha, dcomplex *a, int dim);
extern void ZRSCALE_VECTOR( double alpha, dcomplex *a, int dim);
extern double SUM_VECTOR(double *a, int dim);
extern dcomplex ZSUM_VECTOR(dcomplex *a, int dim);
extern double **ALLOC_MATRIX ( int n, int m);
extern int **IALLOC_MATRIX ( int n, int m);
extern dcomplex **ZALLOC_MATRIX ( int n, int m);
extern void FREE_MATRIX(double **h);
extern void IFREE_MATRIX(int **h);
extern void ZFREE_MATRIX(dcomplex **h);
extern double ***ALLOC_MATRIX_ARRAY( int num_matrices, int n, int m);
extern void FREE_MATRIX_ARRAY( double ***ma, int num_matrices);
extern int ***IALLOC_MATRIX_ARRAY( int n_mat, int n, int m);
extern void IFREE_MATRIX_ARRAY( int ***ma, int num_matrices);
extern dcomplex ***ZALLOC_MATRIX_ARRAY( int num_matrices, int n, int m);
extern void ZFREE_MATRIX_ARRAY( dcomplex ***ma, int num_matrices);
extern void MATRIX_TRANSPOSE(double **a, int dim);
extern double **RECT_TRANSPOSE(double **A, int m, int dim);
extern void ZMATRIX_TRANSPOSE(dcomplex **a, int dim);
extern void MATRIX_ADD(double **a, double **b, double **c, int dim);
extern void ZMATRIX_ADD(dcomplex **a, dcomplex **b, dcomplex **c, int
			dim);
extern void MATRIX_SUB(double **a, double **b, double **c, int dim);
extern void ZMATRIX_SUB(dcomplex **a, dcomplex **b, dcomplex **c, int
			dim);
extern void MATRIX_MULT(double **a, double **b, double **c, int dim);
extern void ZMATRIX_MULT(dcomplex **a, dcomplex **b, dcomplex **c, int
			dim);
extern void SYM_MATRIX_MULT(double **a, double **b, double **c, int dim);
extern void HERM_MATRIX_MULT(dcomplex **a, dcomplex **b, dcomplex **c,
			     int dim); 
extern void MATRIX_VECTOR_MULT(double **a, double *x, double *y, int dim);
extern void ZMATRIX_VECTOR_MULT(dcomplex **a, dcomplex *x, dcomplex *y,
				int dim);
extern void SYM_MATRIX_VECTOR_MULT(double **a, double *x, double *y, int dim);
extern void HERM_MATRIX_VECTOR_MULT(dcomplex **a, dcomplex *x, dcomplex
				    *y, int dim);
extern double MATRIX_ELT (double *a, double **H, double *b, int dim);
extern int SYM_EIGENSYSTEM(double **a, double *evals, int dim, LAPACK_OPTIONS
			   *options);
extern int GEN_SYM_EIGENSYSTEM(double **a, double **b, double *evals, int dim,
			       LAPACK_OPTIONS *options);
extern void MATRIX_EXP ( double c, dcomplex **result, double *evals, double
		       **evecs, int dim);
extern int SYM_LINSOLVE(double **a, double **b, int nrhs, int dim);
extern int ZSYM_LINSOLVE(dcomplex **a, dcomplex **b, int nrhs, int dim);
extern int SYMPOS_LINSOLVE(double **a, double **b, int nrhs, int dim);
extern int SYMPOS_LINSOLVE(double **a, double **b, int nrhs, int dim);
extern int LINSOLVE(double **a, double **b, int nrhs, int dim);
extern void SVD(double **aa, int m, int n, double ww[], double **vv);
extern void SVBKSB( double **u, double w[], double **v, int m, int n,
                    double b[], double x[]);
extern void GRAM_SCHMIDT(double **V, int m, int n);
extern void PROJECT( double *v, int n, double **basis, int m);

/* COMPLEX ARITMETIC ROUTINES */
extern dcomplex Cadd(dcomplex a, dcomplex b);
extern dcomplex Csub(dcomplex a, dcomplex b);
extern dcomplex Cmult(dcomplex a, dcomplex b);
extern double Cabs(dcomplex a);
extern double Cprob(dcomplex a);
extern dcomplex RCmult(double r, dcomplex a);
extern dcomplex ICmult(double im, dcomplex a);
extern dcomplex ExpI (double x);
extern dcomplex Cconj (dcomplex a);

/* INTERPOLATION ROUTINES */
extern void SPLINE_INIT(double *x, double *y, int n, double yp1, double
			ypn, double *y2);
extern void SPLINE_EVAL(double *xa, double *ya, double *y2a, int n,
			double x, double *y, double *dy, double *d2y);

/* MINIMUM FINDING ROUTINES */
void MIN_BRACKET(double *ax, double *bx, double *cx, double *fa, double *fb,
		 double *fc, double (*func)(double)); 
double MIN_FIND(double ax, double bx, double cx, double (*f)(double), double
		tol, double *xmin);

/* ROOT FINDING ROUTINES */
int ROOT_BRACKET(double *x1, double *x2, double (*func)(double));
double ROOT_FIND(double x1, double x2, double (*func)(double), double tol);

/* INTEGRATION ROUTINES */
double INTEGRATE(double (*func)(double), double a, double b, double tol);

/* DIFFERENTIAL EQUATION ROUTINES */
void RKCK(double *y, double *dydx, int n, double x, double h, double
	  *yout, double *yerr, void (*derivs)(double, double *, double *));
void RKQS(double y[], double dydx[], int n, double *x, double htry, 
	  double eps, double yscal[], double *hdid, double *hnext,
	  void (*derivs)(double, double [], double []));

/* MISCELLANEOUS ROUTINES */
extern double RANDOM(double min, double max);
extern void SEED_RANDOM(int);

/* INTERNAL ROUTINES */
extern void MATHERR(char *error_text, unsigned int flags);

/* Math macros  */
static double sqrarg;
#define SQR(a) ((sqrarg=(a))==0.0?0.0:sqrarg*sqrarg)
static int isqrarg;
#define ISQR(a) ((isqrarg=(a))==0?0:isqrarg*isqrarg)
static double marg1, marg2;
#define MIN(a,b) (marg1=(a), marg2=(b), (marg1)<(marg2) ? (marg1) : (marg2))
#define MAX(a,b) (marg1=(a), marg2=(b), (marg1)>(marg2) ? (marg1) : (marg2))
static int imarg1, imarg2;
#define IMIN(a,b) (imarg1=(a), imarg2=(b), (imarg1)<(imarg2) ? (imarg1) :\
		  (imarg2)) 
#define IMAX(a,b) (imarg1=(a), imarg2=(b), (imarg1)>(imarg2) ? (imarg1) :\
		  (imarg2)) 
static double dswap;
#define SWAP(a,b) dswap=(a); (a)=(b); (b)=dswap;
#define SIGN(a,b) ((b)>0.0 ? fabs(a): -fabs(a))

#define INCLUDED_MATHUTILS
#endif


