/* program to test mathutilities */
#include "mathutils.h"

#define TOL 1.0e-14

main (int argc, char *argv[])
{
  int i,j,k;
  int dim;
  int seed;
  int errcount;
  double *x, *y;
  double r;
  dcomplex z;
  dcomplex z1;
  dcomplex *zx, *zy;
  double **a, **b, **c;
  dcomplex **za, **zb, **zc;

  if (argc < 2)
  {
    fprintf(stderr,"Usage: %s dim\n", argv[0]);
    exit(EXIT_FAILURE);
  }

  dim = atoi(argv[1]);

  /* turn off buffering of output */
  setbuf(stdout, NULL);

  /* allocation and initialization of vector */
  printf("Testing ALLOC_VECTOR...");
  x = ALLOC_VECTOR( dim);
  printf("OK.\n");

  printf("Testing INIT_VECTOR...");
  INIT_VECTOR( x, 1.0, dim);
  for (i=0, errcount=0; i<dim; ++i)
    if (x[i] != 1.0) ++errcount;
  if (errcount)
    printf("%d errors encountered.\n", errcount);
  else printf ("OK.\n");

  printf("Testing FREE_VECTOR...");
  FREE_VECTOR(x);
  printf("OK.\n");

  printf("Testing ZALLOC_VECTOR...");
  zx = ZALLOC_VECTOR( dim);
  printf("OK.\n");

  printf("Testing ZINIT_VECTOR...");
  z.r = 1.0; z.i = 1.0;
  ZINIT_VECTOR( zx, z, dim);
  for (i=0, errcount=0; i<dim; ++i)
    if ((zx[i].r != z.r) || (zx[i].i != z.i)) ++errcount;
  if (errcount)
    printf("%d errors encountered.\n", errcount);
  else printf ("OK.\n");

  printf("Testing ZFREE_VECTOR...");
  ZFREE_VECTOR(zx);
  printf("OK.\n");

  /* vector copy */
  printf("Testing COPY_VECTOR...");
  x = ALLOC_VECTOR( dim);
  y = ALLOC_VECTOR( dim);
  INIT_VECTOR( x, 1.0, dim);
  COPY_VECTOR( x, y, dim);
  for (i=0, errcount=0; i<dim; ++i)
    if (y[i] != 1.0) ++errcount;
  if (errcount)
    printf("%d errors encountered.\n", errcount);
  else printf ("OK.\n");
  FREE_VECTOR(x); 
  FREE_VECTOR(y);

  
  printf("Testing ZCOPY_VECTOR...");
  zx = ZALLOC_VECTOR( dim);
  zy = ZALLOC_VECTOR( dim);
  z.r = 1.0; z.i = 1.0;
  ZINIT_VECTOR( zx, z, dim);
  ZCOPY_VECTOR( zx, zy, dim);
  for (i=0, errcount=0; i<dim; ++i)
    if ((zy[i].r != z.r) || (zy[i].i != z.i)) ++errcount;
  if (errcount)
    printf("%d errors encountered.\n", errcount);
  else printf ("OK.\n");
  ZFREE_VECTOR(zx); 
  ZFREE_VECTOR(zy);
  
  /* test dot products */
  printf("Testing DOTP...");
  x = ALLOC_VECTOR( dim);
  INIT_VECTOR( x, 1.0, dim);
  y = ALLOC_VECTOR( dim);
  INIT_VECTOR( y, 1.0, dim);
  r = DOTP(x,y,dim);
  if (fabs(r-dim)<TOL) printf("OK\n");
  else printf("Failed\n");
  FREE_VECTOR(x);
  FREE_VECTOR(y);

  printf("Testing ZDOTC...");
  z.r = 1.0; z.i = 1.0;
  zx = ZALLOC_VECTOR( dim);
  zy = ZALLOC_VECTOR( dim);
  ZINIT_VECTOR( zx, z, dim);
  ZINIT_VECTOR( zy, z, dim);
  z= ZDOTC(zx,zy,dim);
  if (fabs(z.r-2*dim)<TOL) printf("OK\n");
  else printf("Failed, returned (%.2f, %.2f)\n", z.r, z.i);
  ZFREE_VECTOR(zx); 
  ZFREE_VECTOR(zy);

  printf("Testing ZDOTU...");
  z.r = 1.0; z.i = 1.0;
  zx = ZALLOC_VECTOR( dim);
  zy = ZALLOC_VECTOR( dim);
  ZINIT_VECTOR( zx, z, dim);
  ZINIT_VECTOR( zy, z, dim);
  z = ZDOTU(zx,zy,dim);
  if (fabs(z.i-2*dim)<TOL) printf("OK\n");
  else printf("Failed, returned (%.2f, %.2f)\n", z.r, z.i);
  ZFREE_VECTOR(zx); 
  ZFREE_VECTOR(zy);

  /* testing norms */
  printf("Testing NORM...");
  x = ALLOC_VECTOR(dim);
  INIT_VECTOR(x,1.0,dim);
  r = NORM(x,dim);
  if (fabs(r-sqrt((double)dim))<TOL) printf("OK\n");
  else printf("Failed %.4f\n",r-sqrt((double)dim));
  FREE_VECTOR(x);

  printf("Testing ZNORM...");
  z.r = 1.0; z.i = 1.0;
  zx = ZALLOC_VECTOR( dim);
  ZINIT_VECTOR( zx, z, dim);
  r = ZNORM(zx,dim);
  if (fabs(r-sqrt(2.0*dim))<TOL) printf("OK\n");
  else printf("Failed %.4f\n",r-sqrt(2.0*dim));
  ZFREE_VECTOR(zx);

  /* testing norms */
  x = ALLOC_VECTOR(dim);
  INIT_VECTOR(x,1.0,dim);
  printf("Testing NORMSQ...");
  r = NORMSQ(x,dim);
  if (fabs(r-dim)<TOL) printf("OK\n");
  else printf("Failed %f\n",r);
  FREE_VECTOR(x);

  printf("Testing ZNORMSQ...");
  z.r = 1.0; z.i = 1.0;
  zx = ZALLOC_VECTOR( dim);
  ZINIT_VECTOR( zx, z, dim);
  r = ZNORMSQ(zx,dim);
  if (fabs(r-2.0*dim)<TOL) printf("OK\n");
  else printf("Failed\n");
  ZFREE_VECTOR(zx); 
  
  printf("Testing SCALE_VECTOR...");
  x = ALLOC_VECTOR(dim);
  INIT_VECTOR(x,1.0,dim);
  SCALE_VECTOR(2.0,x,dim);
  for (i=0, errcount=0; i<dim; ++i)
    if (x[i] != 2.0) ++errcount;
  if (errcount)
    printf("%d errors encountered.\n", errcount);
  else printf ("OK.\n");
  FREE_VECTOR(x);

  printf("Testing ZSCALE_VECTOR...");
  z.r=1.0; z.i=1.0;
  zx = ZALLOC_VECTOR( dim);
  ZINIT_VECTOR( zx, z, dim);
  ZSCALE_VECTOR(z,zx,dim);
  for (i=0, errcount=0; i<dim; ++i)
    if ((zx[i].r != 0.0) || (zx[i].i != 2.0)) ++errcount;
  if (errcount)
    printf("%d errors encountered.\n", errcount);
  else printf ("OK.\n");
  ZFREE_VECTOR(zx); 

  printf("Testing ZRSCALE_VECTOR...");
  z.r=1.0; z.i=1.0;
  zx = ZALLOC_VECTOR( dim);
  ZINIT_VECTOR( zx, z, dim);
  ZRSCALE_VECTOR(2.0,zx,dim);
  for (i=0, errcount=0; i<dim; ++i)
    if ((zx[i].r != 2.0) || (zx[i].i != 2.0)) ++errcount;
  if (errcount)
    printf("%d errors encountered.\n", errcount);
  else printf ("OK.\n");
  ZFREE_VECTOR(zx); 


  printf("Testing SUM_VECTOR...");
  x = ALLOC_VECTOR(dim);
  INIT_VECTOR(x,1.0,dim);
  r = SUM_VECTOR(x,dim);
  if (fabs(r-dim)<TOL) printf("OK\n");
  else printf("Failed\n");
  FREE_VECTOR(x);

  printf("Testing ZSUM_VECTOR...");
  zx = ZALLOC_VECTOR(dim);
  ZINIT_VECTOR( zx, z, dim);
  z = ZSUM_VECTOR(zx,dim);
  if ((fabs(z.r-dim)<TOL) && (fabs(z.i-dim)<TOL)) printf("OK\n");
  else printf("Failed, returned (%.2f, %.2f)\n", z.r, z.i);
  ZFREE_VECTOR(zx);
  
  printf("Testing ALLOC_MATRIX...");
  a = ALLOC_MATRIX(dim,dim);
  INIT_VECTOR(a[0],1.0,dim*dim);
  for (i=0, errcount=0; i<dim; ++i)
    for (j=0; j<dim; ++j)
      if (a[i][j] != 1.0) ++errcount;
  if (!errcount) printf("OK.\n");
  else printf("Failed.\n");

  printf("Testing FREE_MATRIX...");
  FREE_MATRIX(a);
  printf("OK.\n");

  printf("Testing ZALLOC_MATRIX...");
  za = ZALLOC_MATRIX(dim,dim);
  z.r=1.0; z.i=1.0;
  ZINIT_VECTOR(za[0],z,dim*dim);
  for (i=0, errcount=0; i<dim; ++i)
    for (j=0; j<dim; ++j)
      if ((za[i][j].r != 1.0) || (za[i][j].i != 1.0)) ++errcount;
  if (!errcount) printf("OK.\n");
  else printf("Failed.\n");

  printf("Testing ZFREE_MATRIX...");
  ZFREE_MATRIX(za);
  printf("OK.\n");

  printf("Testing MATRIX_ADD...");
  a= ALLOC_MATRIX(dim,dim);
  b= ALLOC_MATRIX(dim,dim);
  c= ALLOC_MATRIX(dim,dim);
  INIT_VECTOR(a[0],1.0,dim*dim);
  INIT_VECTOR(b[0],1.0,dim*dim);
  MATRIX_ADD(a,b,c,dim);
  for (i=0, errcount=0; i<dim; ++i)
    for (j=0; j<dim; ++j)
      if (c[i][j] != 2.0) ++errcount;
  if (!errcount) printf("OK.\n");
  else printf("Failed.\n");
  FREE_MATRIX(a);
  FREE_MATRIX(b);
  FREE_MATRIX(c);

  printf("Testing ZMATRIX_ADD...");
  za = ZALLOC_MATRIX(dim,dim);
  zb = ZALLOC_MATRIX(dim,dim);
  zc = ZALLOC_MATRIX(dim,dim);
  z.r=1.0; z.i=1.0;
  ZINIT_VECTOR(za[0],z,dim*dim);
  ZINIT_VECTOR(zb[0],z,dim*dim);
  ZMATRIX_ADD(za,zb,zc,dim); 
  for (i=0, errcount=0; i<dim; ++i)
    for (j=0; j<dim; ++j)
      if ((zc[i][j].r != 2.0) || (zc[i][j].i != 2.0)) ++errcount;
  if (!errcount) printf("OK.\n");
  else printf("Failed.\n");
  ZFREE_MATRIX(za);
  ZFREE_MATRIX(zb);
  ZFREE_MATRIX(zc);

  printf("Testing MATRIX_SUB...");
  a= ALLOC_MATRIX(dim,dim);
  b= ALLOC_MATRIX(dim,dim);
  c= ALLOC_MATRIX(dim,dim);
  INIT_VECTOR(a[0],1.0,dim*dim);
  INIT_VECTOR(b[0],1.0,dim*dim);
  MATRIX_SUB(a,b,c,dim);
  for (i=0, errcount=0; i<dim; ++i)
    for (j=0; j<dim; ++j)
      if (c[i][j] != 0.0) ++errcount;
  if (!errcount) printf("OK.\n");
  else printf("Failed.\n");
  FREE_MATRIX(a);
  FREE_MATRIX(b);
  FREE_MATRIX(c);
  
  printf("Testing ZMATRIX_SUB...");
  za = ZALLOC_MATRIX(dim,dim);
  zb = ZALLOC_MATRIX(dim,dim);
  zc = ZALLOC_MATRIX(dim,dim);
  z.r=1.0; z.i=1.0;
  ZINIT_VECTOR(za[0],z,dim*dim);
  ZINIT_VECTOR(zb[0],z,dim*dim);
  ZMATRIX_SUB(za,zb,zc,dim); 
  for (i=0, errcount=0; i<dim; ++i)
    for (j=0; j<dim; ++j)
      if ((zc[i][j].r != 0.0) || (zc[i][j].i != 0.0)) ++errcount;
  if (!errcount) printf("OK.\n");
  else printf("Failed.\n");
  ZFREE_MATRIX(za);
  ZFREE_MATRIX(zb);
  ZFREE_MATRIX(zc);

  printf("Testing MATRIX_MULT...");
  a= ALLOC_MATRIX(dim,dim);
  b= ALLOC_MATRIX(dim,dim);
  c= ALLOC_MATRIX(dim,dim);
  INIT_VECTOR(a[0],1.0,dim*dim);
  INIT_VECTOR(b[0],1.0,dim*dim);
  MATRIX_MULT(a,b,c,dim);
  for (i=0, errcount=0; i<dim; ++i)
    for (j=0; j<dim; ++j)
      if (c[i][j] != dim) ++errcount;
  if (!errcount) printf("OK.\n");
  else printf("Failed.\n");
  FREE_MATRIX(a);
  FREE_MATRIX(b);
  FREE_MATRIX(c);
  
  printf("Testing ZMATRIX_MULT...");
  za = ZALLOC_MATRIX(dim,dim);
  zb = ZALLOC_MATRIX(dim,dim);
  zc = ZALLOC_MATRIX(dim,dim);
  z.r=1.0; z.i=1.0;
  ZINIT_VECTOR(za[0],z,dim*dim);
  ZINIT_VECTOR(zb[0],z,dim*dim);
  ZMATRIX_MULT(za,zb,zc,dim); 
  for (i=0, errcount=0; i<dim; ++i)
    for (j=0; j<dim; ++j)
      if ((zc[i][j].r != 0.0) || (zc[i][j].i != 2.0*dim)) ++errcount;
  if (!errcount) printf("OK.\n");
  else printf("Failed.\n");
  ZFREE_MATRIX(za);
  ZFREE_MATRIX(zb);
  ZFREE_MATRIX(zc);

  printf("Testing SYM_MATRIX_MULT...");
  a= ALLOC_MATRIX(dim,dim);
  b= ALLOC_MATRIX(dim,dim);
  c= ALLOC_MATRIX(dim,dim);
  INIT_VECTOR(a[0],1.0,dim*dim);
  INIT_VECTOR(b[0],1.0,dim*dim);
  SYM_MATRIX_MULT(a,b,c,dim);
  for (i=0, errcount=0; i<dim; ++i)
    for (j=0; j<dim; ++j)
      if (c[i][j] != dim) ++errcount;
  if (!errcount) printf("OK.\n");
  else printf("Failed.\n");
  FREE_MATRIX(a);
  FREE_MATRIX(b);
  FREE_MATRIX(c);

  printf("Testing HERM_MATRIX_MULT...");
  za = ZALLOC_MATRIX(dim,dim);
  zb = ZALLOC_MATRIX(dim,dim);
  zc = ZALLOC_MATRIX(dim,dim);
  ZINIT_VECTOR(za[0],COMPLEX_ZERO, dim*dim);
  z.r=1.0; z.i=0.0;
  for(i=0; i<dim; ++i) za[i][i] = z;
  ZINIT_VECTOR(zb[0],COMPLEX_ZERO, dim*dim);
  z.r=0.0; z.i=1.0;
  for (i=0; i<dim; ++i)
    for(j=0; j<dim; ++j)
    {
      if (i<j) zb[i][j]= z;
      else if (i>j) zb[i][j] = Cconj(z);
      else zb[i][j]=COMPLEX_ZERO;
    }
  HERM_MATRIX_MULT(za,zb,zc,dim); 
  for (i=0, errcount=0; i<dim; ++i)
    for (j=0; j<dim; ++j)
    {
      if (i<j) 
      {
	if ((zc[i][j].r != z.r) || (zc[i][j].i != z.i)) ++errcount;
      }
      else if (i>j) 
      {
	if ((zc[i][j].r != z.r) || (zc[i][j].i != -z.i)) ++errcount;
      }
      else 
      {
	if ((zc[i][j].r != 0.0) || (zc[i][j].i != 0.0)) ++errcount;
      }
    }
  if (!errcount) printf("OK.\n");
  else printf("Failed.\n");
  ZFREE_MATRIX(za);
  ZFREE_MATRIX(zb);
  ZFREE_MATRIX(zc);

  printf("Testing MATRIX_VECTOR_MULT...");
  x = ALLOC_VECTOR(dim);
  y = ALLOC_VECTOR(dim);
  a= ALLOC_MATRIX(dim,dim);
  INIT_VECTOR(x,1.0,dim);
  INIT_VECTOR(y,1.0,dim);
  INIT_VECTOR(a[0],1.0,dim*dim);
  MATRIX_VECTOR_MULT(a,x,y,dim);
  for (i=0, errcount=0; i<dim; ++i)
    if (y[i] != dim) ++errcount;
  if (!errcount) printf("OK.\n");
  else printf("Failed.\n");
  FREE_VECTOR(x); 
  FREE_VECTOR(y);
  FREE_MATRIX(a);

  printf("Testing ZMATRIX_VECTOR_MULT...");
  za = ZALLOC_MATRIX(dim,dim);
  zx = ZALLOC_VECTOR(dim);
  zy = ZALLOC_VECTOR(dim);
  z.r=1.0; z.i=1.0;
  ZINIT_VECTOR(za[0],z,dim*dim);
  z.r=1.0; z.i=0.0;
  ZINIT_VECTOR(zx,z,dim);
  ZINIT_VECTOR(zy,z,dim);
  ZMATRIX_VECTOR_MULT(za,zx,zy,dim); 
  for (i=0, errcount=0; i<dim; ++i)
    if ((zy[i].r != dim) || (zy[i].i != dim)) ++errcount;
  if (!errcount) printf("OK.\n");
  else printf("Failed.\n");
  ZFREE_MATRIX(za);
  ZFREE_VECTOR(zx);
  ZFREE_VECTOR(zy);

  printf("Testing SYM_MATRIX_VECTOR_MULT...");
  x = ALLOC_VECTOR(dim);
  y = ALLOC_VECTOR(dim);
  a= ALLOC_MATRIX(dim,dim);
  INIT_VECTOR(x,1.0,dim);
  INIT_VECTOR(y,1.0,dim);
  INIT_VECTOR(a[0],1.0,dim*dim);
  SYM_MATRIX_VECTOR_MULT(a,x,y,dim);
  for (i=0, errcount=0; i<dim; ++i)
    if (y[i] != dim) ++errcount;
  if (!errcount) printf("OK.\n");
  else printf("Failed.\n");
  FREE_VECTOR(x); 
  FREE_VECTOR(y);
  FREE_MATRIX(a);

  printf("Testing HERM_MATRIX_VECTOR_MULT...");
  za = ZALLOC_MATRIX(dim,dim);
  zx = ZALLOC_VECTOR(dim);
  zy = ZALLOC_VECTOR(dim);
  ZINIT_VECTOR(za[0],COMPLEX_ZERO,dim*dim);
  for(i=0; i<dim; ++i)
    for(j=0; j<dim; ++j)
    {
      if (i<j) za[i][j].i=1.0;
      else if (i>j) za[i][j].i=-1.0;
    }
  z.r=1.0; z.i=0.0;
  ZINIT_VECTOR(zx,z,dim);
  ZINIT_VECTOR(zy,z,dim);
  HERM_MATRIX_VECTOR_MULT(za,zx,zy,dim); 
  for (i=0, errcount=0; i<dim; ++i)
    if ((zy[i].r != 0.0) || (zy[i].i != dim -1 - 2*i)) ++errcount;
  if (!errcount) printf("OK.\n");
  else printf("Failed.\n");
  ZFREE_MATRIX(za);
  ZFREE_VECTOR(zx);
  ZFREE_VECTOR(zy);

  printf("Testing MATRIX_ELT...");
  x = ALLOC_VECTOR(dim);
  y = ALLOC_VECTOR(dim);
  a= ALLOC_MATRIX(dim,dim);
  INIT_VECTOR(x,1.0,dim);
  INIT_VECTOR(y,1.0,dim);
  INIT_VECTOR(a[0],1.0,dim*dim);
  r=MATRIX_ELT(x,a, y,dim);
  if (fabs(r-dim*dim)<TOL) printf("OK.\n");
  else printf("Failed.\n");
  FREE_VECTOR(x); 
  FREE_VECTOR(y);
  FREE_MATRIX(a);

  printf("Testing SYM_EIGENSYSTEM...");
  a= ALLOC_MATRIX(2,2);
  x=ALLOC_VECTOR(2);
  b=ALLOC_MATRIX(2,2);
  INIT_VECTOR(a[0],0.0,2*2);
  a[1][0] = a[0][1] = 1.0;
  SYM_EIGENSYSTEM(a,x,2,0x0);
  if ( (fabs(x[0]+1.0)<TOL) && (fabs(x[1]-1.0)<TOL))
    printf("OK.\n");
  else
    printf("Failed.\n");
  FREE_VECTOR(x); 
  FREE_MATRIX(a);
  FREE_MATRIX(b);
  
  printf("Testing SYM_LINSOLVE...");
  a= ALLOC_MATRIX(2,2);
  b=ALLOC_MATRIX(2,2);
  c=ALLOC_MATRIX(2,2);
  INIT_VECTOR(a[0],0.0,4);
  a[0][0]=5; a[0][1] = 1; a[1][1]=3;
  INIT_VECTOR(b[0],0.0,4);
  b[0][0] = b[1][1] = 1;
  COPY_VECTOR(a[0],c[0],4);
  SYM_LINSOLVE(c,b,2,2);
  SYM_MATRIX_MULT(a,b,c,2);
  for(i=0, errcount=0; i<2; ++i){
    for(j=0; j<2; ++j){
      if(i==j && fabs(c[i][i]-1.0)>TOL) ++errcount;
      else if (fabs(c[i][i]-1.0)>TOL) ++errcount;
    }
  }
  if (errcount ==0) printf("OK.\n");
  else printf("Failed.\n");
  FREE_MATRIX(a);
  FREE_MATRIX(b);
  FREE_MATRIX(c);

  printf("Testing SYMPOS_LINSOLVE...");
  a= ALLOC_MATRIX(2,2);
  b=ALLOC_MATRIX(2,2);
  c=ALLOC_MATRIX(2,2);
  INIT_VECTOR(a[0],0.0,4);
  a[0][0]=1; a[0][1] = .5; a[1][1]=1;
  INIT_VECTOR(b[0],0.0,4);
  b[0][0] = b[1][1] = 1;
  COPY_VECTOR(a[0],c[0],4);
  SYMPOS_LINSOLVE(c,b,2,2);
  SYM_MATRIX_MULT(a,b,c,2);
  for(i=0, errcount=0; i<2; ++i){
    for(j=0; j<2; ++j){
      if(i==j && fabs(c[i][i]-1.0)>TOL) ++errcount;
      else if (fabs(c[i][i]-1.0)>TOL) ++errcount;
    }
  }
  if (errcount ==0) printf("OK.\n");
  else printf("Failed.\n");
  FREE_MATRIX(a);
  FREE_MATRIX(b);
  FREE_MATRIX(c);
}



