#include "mathutils.h"

		    
/*==========================================================================*/
/* function allocates space for an NxM double matrix and returns a */
/* pointer to the first element of the pointer array defining the rows */
/* of a matrix  */ 
double **ALLOC_MATRIX ( int n, int m){
    register int i,j;  
    double **mat, **rowptr, *eltptr;
  
    /* allocate space for the main memory block */
    eltptr = (double *) malloc((unsigned) n*m*sizeof(double));
    if (!eltptr){
	perror("ALLOC_MATRIX: malloc");
	exit(EXIT_FAILURE);
    }

    /* allocate and set the row pointers */
    rowptr = mat = (double **) malloc((unsigned) n*sizeof(double*));
    if (!mat){
	perror("ALLOC_MATRIX: malloc");
	exit(EXIT_FAILURE);
    }
    for(i=0; i < n; ++i, ++rowptr, eltptr+=m)
	*rowptr = eltptr;

    return(mat);
}

double ***ALLOC_MATRIX_ARRAY( int n_mat, int n, int m){
    register int i,j;
    int mat_size;
    double ***marray, ***matptr, **mat, *eltptr;
    
    mat_size = n*m;
    /* allocate space for the matrix pointers */
    marray = (double ***) malloc((unsigned) n_mat * sizeof(double**));
    if (!marray){
	perror("ALLOC_MATRIX_ARRAY: malloc");
	exit(EXIT_FAILURE);
    }
    /* allocate the main memory block */
    eltptr = (double *) malloc(n_mat*mat_size*sizeof(double));
    if (!eltptr){
	perror("ALLOC_MATRIX_ARRAY: malloc");
	exit(EXIT_FAILURE);
    }
    
    /* set matrix and row pointers */
    for(i=0, matptr=marray; i < n_mat; ++i, ++matptr){ 
	/* allocate the row pointers */
	mat = *matptr = (double **) malloc(n*sizeof(double *));
	if (!mat){
	    perror("ALLOC_MATRIX_ARRAY: malloc");
	    exit(EXIT_FAILURE);
	}
	/* set the row pointers */
	for(j=0; j<n; ++j, ++mat, eltptr+=m)
	    *mat= eltptr;
    }

    return (marray);
}

double *ALLOC_VECTOR( int n)
{
  double *v;
  
  v = (double *) malloc((size_t) n*sizeof(double));
  if (!v)
  {
    perror("ALLOC_VECTOR: malloc");
    exit(EXIT_FAILURE);
  }
  return(v);
}
		    
double *REALLOC_VECTOR( double *vec, int newdim)
{
    double *new;
    
    new= (double *) realloc( vec, (size_t) newdim*sizeof(double));

    if (!new)
    {
	perror("REALLOC_VECTOR: realloc");
	MATHERR("", 00);
    }

    return(new);
}

/*==========================================================================*/
/* function allocates space for an NxM int matrix and returns a */
/* pointer to the first element of the pointer array defining the rows */
/* of a matrix  */ 

int **IALLOC_MATRIX ( int n, int m){
    register int i,j;  
    int **mat, **rowptr, *eltptr;
  
    /* allocate space for the main memory block */
    eltptr = (int *) malloc((unsigned) n*m*sizeof(int));
    if (!eltptr){
	perror("IALLOC_MATRIX: malloc");
	exit(EXIT_FAILURE);
    }

    /* allocate and set the row pointers */
    rowptr = mat = (int **) malloc((unsigned) n*sizeof(int*));
    if (!mat){
	perror("IALLOC_MATRIX: malloc");
	exit(EXIT_FAILURE);
    }
    for(i=0; i < n; ++i, ++rowptr, eltptr+=m)
	*rowptr = eltptr;

    return(mat);
}

int ***IALLOC_MATRIX_ARRAY( int n_mat, int n, int m){
    register int i,j;
    int mat_size;
    int ***marray, ***matptr, **mat, *eltptr;
    
    mat_size = n*m;
    /* allocate space for the matrix pointers */
    marray = (int ***) malloc((unsigned) n_mat * sizeof(int**));
    if (!marray){
	perror("IALLOC_MATRIX_ARRAY: malloc");
	exit(EXIT_FAILURE);
    }
    /* allocate the main memory block */
    eltptr = (int *) malloc(n_mat*mat_size*sizeof(int));
    if (!eltptr){
	perror("IALLOC_MATRIX_ARRAY: malloc");
	exit(EXIT_FAILURE);
    }
    
    /* set matrix and row pointers */
    for(i=0, matptr=marray; i < n_mat; ++i, ++matptr){ 
	/* allocate the row pointers */
	mat = *matptr = (int **) malloc(n*sizeof(int *));
	if (!mat){
	    perror("IALLOC_MATRIX_ARRAY: malloc");
	    exit(EXIT_FAILURE);
	}
	/* set the row pointers */
	for(j=0; j<n; ++j, ++mat, eltptr+=m)
	    *mat= eltptr;
    }

    return (marray);
}

/*==========================================================================*/
int *IALLOC_VECTOR( int n){
    int *v;
    
    v = (int *) malloc((size_t) n*sizeof(int));
    if (!v){
	perror("IALLOC_VECTOR: malloc");
	exit(EXIT_FAILURE);
    }
    return(v);
}
		
int *REIALLOC_VECTOR( int *vec, int newdim)
{
    int *new;
    
    new= (int *) realloc( vec, (size_t) newdim*sizeof(int));

    if (!new)
    {
	perror("REIALLOC_VECTOR: realloc");
	MATHERR("", 00);
    }

    return(new);
}

    
/*==========================================================================*/
/* function allocates space for an NxM dcomplex matrix and returns a */
/* pointer to the first element of the pointer array defining the rows */
/* of a matrix  */ 

dcomplex **ZALLOC_MATRIX ( int n, int m){
    register int i,j;  
    dcomplex **mat, **rowptr, *eltptr;
  
    /* allocate space for the main memory block */
    eltptr = (dcomplex *) malloc((unsigned) n*m*sizeof(dcomplex));
    if (!eltptr){
	perror("ZALLOC_MATRIX: malloc");
	exit(EXIT_FAILURE);
    }

    /* allocate and set the row pointers */
    rowptr = mat = (dcomplex **) malloc((unsigned) n*sizeof(dcomplex*));
    if (!mat){
	perror("ZALLOC_MATRIX: malloc");
	exit(EXIT_FAILURE);
    }
    for(i=0; i < n; ++i, ++rowptr, eltptr+=m)
	*rowptr = eltptr;

    return(mat);
}

/*==========================================================================*/
dcomplex ***ZALLOC_MATRIX_ARRAY( int n_mat, int n, int m){
    register int i,j;
    int mat_size;
    dcomplex ***marray, ***matptr, **mat, *eltptr;
    
    mat_size = n*m;
    /* allocate space for the matrix pointers */
    marray = (dcomplex ***) malloc((unsigned) n_mat * sizeof(dcomplex**));
    if (!marray){
	perror("ZALLOC_MATRIX_ARRAY: malloc");
	exit(EXIT_FAILURE);
    }
    /* allocate the main memory block */
    eltptr = (dcomplex *) malloc(n_mat*mat_size*sizeof(dcomplex));
    if (!eltptr){
	perror("ZALLOC_MATRIX_ARRAY: malloc");
	exit(EXIT_FAILURE);
    }
    
    /* set matrix and row pointers */
    for(i=0, matptr=marray; i < n_mat; ++i, ++matptr){ 
	/* allocate the row pointers */
	mat = *matptr = (dcomplex **) malloc(n*sizeof(dcomplex *));
	if (!mat){
	    perror("ZALLOC_MATRIX_ARRAY: malloc");
	    exit(EXIT_FAILURE);
	}
	/* set the row pointers */
	for(j=0; j<n; ++j, ++mat, eltptr+=m)
	    *mat= eltptr;
    }

    return (marray);
}

/*==========================================================================*/
dcomplex *ZALLOC_VECTOR( int n)
{
  dcomplex *v;
  
  v = (dcomplex *) malloc((size_t) n*sizeof(dcomplex));
  if (!v)
  {
    perror("ALLOC_VECTOR: malloc");
    exit(EXIT_FAILURE);
  }
  return(v);
}
		    
/*==========================================================================*/
/* function frees a matrix pointed to by pointer h */
void FREE_MATRIX(double **h)
{
  free(h[0]);
  free(h);
}
void FREE_MATRIX_ARRAY( double ***ma, int num_matrices){
    register int i;
    
    /* free main data space */
    free(ma[0][0]);
    /* free row pointers */
    for(i=0;i<num_matrices;++i)
	free(ma[i]);
    /* free matrix pointers */
    free(ma);
}
		    
/*==========================================================================*/
void FREE_VECTOR(double *v)
{
  free((void *)v);
}
		    
/*==========================================================================*/
/* function frees an int  matrix pointed to by pointer h */
void IFREE_MATRIX(int **h){
    free(h[0]);
    free(h);
}
		    
void IFREE_MATRIX_ARRAY( int ***ma, int num_matrices){
    register int i;
    
    /* free main data space */
    free(ma[0][0]);
    /* free row pointers */
    for(i=0;i<num_matrices;++i)
	free(ma[i]);
    /* free matrix pointers */
    free(ma);
}

/*==========================================================================*/
/* function frees a matrix pointed to by pointer h */
void ZFREE_MATRIX(dcomplex **h)
{
  free(h[0]);
  free(h);
}

void ZFREE_MATRIX_ARRAY( dcomplex ***ma, int num_matrices){
    register int i;
    
    /* free main data space */
    free(ma[0][0]);
    /* free row pointers */
    for(i=0;i<num_matrices;++i)
	free(ma[i]);
    /* free matrix pointers */
    free(ma);
}

/*==========================================================================*/
void ZFREE_VECTOR(dcomplex *v)
{
  free((void *)v);
}
