#include "mathutils.h"

main(int argc, char *argv[]){
  int i,j,k;
  int nr, n_run;
  int n,m;
  double **A, **At, **G, **Gt, **GtG, **U, **Ut, **UtU, *w, **V, norm;
  double *v;
  
  FILE *stream;
  if (argc <4){
    fprintf(stderr,"Usage: %s m n n_run <datfile>\n", argv[0]);
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
/*   for(i=0; i<m; ++i){ */
/*     for(j=0; j<n; ++j){ */
/*       if (fscanf(stream,"%lf", A[i]+j) != 1){ */
/* 	fprintf(stderr,"Error occured in reading the data.\n"); */
/* 	exit(EXIT_FAILURE); */
/*       } */
/*     } */
/*   } */

  for(i=0; i<m; ++i){  
      for(j=0; j<n; ++j){ 
	  A[i][j] = RANDOM(-10.0,10.0);
      }
  }

  /* print A */
  U = ALLOC_MATRIX(m,n);
  COPY_VECTOR(A[0], U[0], m*n);
  for(i=0; i<m; ++i){
    for(j=0; j<n; ++j)
      printf("%8.2f ", U[i][j]);
    printf("\n");
  }

  printf("Using Gram-Schmidt\n");
  /* gram-schmidt */
  At = RECT_TRANSPOSE(A, m,n);
  Gt = ALLOC_MATRIX(n,m);
  for(i=0; i<n_run; ++i){
      COPY_VECTOR(At[0], Gt[0], n*m);
      GRAM_SCHMIDT(Gt,n,m);
  }

  printf("Orthogonal vectors are \n");
  for (i=0; i<n; ++i){
      for(j=0, norm=0.0; j<m; ++j){
	  norm += SQR(Gt[i][j]);
	  printf("%.6f ", Gt[i][j]);
      }
      printf(" %f\n",norm);
  }

  G = RECT_TRANSPOSE(Gt,n,m);
  GtG = ALLOC_MATRIX(n,n);
  printf("S=\n");
  for(i=0; i<n; ++i){
      for(j=0; j<n; ++j){
	  GtG[i][j]=0.0;
	  for(k=0; k<m; ++k)
	      GtG[i][j]+=Gt[i][k]*G[k][j];
	  printf("%8.1e ", GtG[i][j]);
      }
      printf("\n");
  }

  /* project random vector against orthogonal basis */
  v = ALLOC_VECTOR(m);
  for(i=0; i<m; ++i)
      v[i] = RANDOM(-10,10);
  PROJECT(v, m, Gt, n);
  for(i=0; i<n; ++i)
      printf("v.G[%2d] = %9.2e\n", i, DOTP(v,Gt[i],m));
}





