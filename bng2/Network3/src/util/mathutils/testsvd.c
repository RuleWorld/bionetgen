#include "mathutils.h"

main(int argc, char *argv[]){
  int i,j,k;
  int nr, n_run;
  int n,m;
  double **A, **G, **Gt, **GtG, **U, **Ut, **UtU, *w, **V, norm;
  
  FILE *stream;
  if (argc <4){
    fprintf(stderr,"Usage: %s n m <datfile>\n", argv[0]);
    exit(EXIT_FAILURE);
  }
  m = atoi(argv[1]); 
  n = atoi(argv[2]);
  n_run = atoi(argv[3]);
  if (argc == 5){
    if ((stream = fopen(argv[4], "r")) == NULL){
      fprintf(stderr, "Can't open file %s.\n", argv[4]);
      exit(EXIT_FAILURE);
    }
  } else {
    stream = stdin;
  }
  
  A = ALLOC_MATRIX(m,n);
  for(i=0; i<m; ++i){
    for(j=0; j<n; ++j){
      if (fscanf(stream,"%lf", A[i]+j) != 1){
	fprintf(stderr,"Error occured in reading the data.\n");
	exit(EXIT_FAILURE);
      }
    }
  }

/*   for(i=0; i<m; ++i){  */
/*       for(j=0; j<n; ++j){  */
/* 	  A[i][j] = RANDOM(-10.0,10.0); */
/*       } */
/*   } */

  /* print A */
  U = ALLOC_MATRIX(m,n);
  COPY_VECTOR(A[0], U[0], m*n);
  for(i=0; i<m; ++i){
    for(j=0; j<n; ++j)
      printf("%8.2f ", U[i][j]);
    printf("\n");
  }

  /* perform svd */
  w = ALLOC_VECTOR(n); 
  V = ALLOC_MATRIX(n,n);
  for (nr=0; nr<n_run; ++nr){
    COPY_VECTOR(A[0], U[0], m*n);
    SVD(U, m, n, w, V);
  }

  printf("Singular values are \n");
  for (i=0; i<n; ++i)
    printf("%.6f ", w[i]);
  printf("\n");

  Ut = RECT_TRANSPOSE(U,m,n);
  printf("Orthogonal vectors are \n");
  for (i=0; i<n; ++i)
      if (w[i] > 1.0e-12){
	  for(j=0, norm=0.0; j<m; ++j){
	      norm += SQR(Ut[i][j]);
	      printf("%.6f ", Ut[i][j]);
	  }
	  printf(" %f\n",norm);
      }

  UtU = ALLOC_MATRIX(n,n);
  printf("S=\n");
  for(i=0; i<n; ++i){
      for(j=0; j<n; ++j){
	  UtU[i][j]=0.0;
	  for(k=0; k<m; ++k)
	      UtU[i][j]+=Ut[i][k]*U[k][j];
	  printf("%8.1e ", UtU[i][j]);
      }
      printf("\n");
  }

}





