MAKEINCL = make.include
#LIBDIR  = ../lib
LIBDIR = .
#INCDIR  = ../include
INCDIR = .
MATHLIB = ${LIBDIR}/libmathutils.a

TARFILE= mathutils.tar
AUX= Makefile make.include.alpha make.include.irix make.include.linux
HEADERS= mathutils.h
SRC =    allocate.c cabs.c cadd.c cconj.c cmult.c complex_zero.c copy_vector.c \
         cprob.c csub.c dotp.c eigensystem.c expi.c gram_schmidt.c \
         herm_matrix_mult.c herm_matrix_vector_mult.c icmult.c init.c \
         integration.c linsolve.c matherr.c matrix_add.c matrix_elt.c \
         matrix_mult.c matrix_sub.c matrix_transpose.c matrix_vector_mult.c \
         matrixexp.c minimum.c norm.c normsq.c project.c random.c rcmult.c \
         rect_transpose.c roots.c runge-kutta.c scale_vector.c splines.c \
         sum_vector.c svd.c sym_linsolve.c sym_matrix_mult.c \
         sym_matrix_vector_mult.c sympos_linsolve.c test_transpose.c \
         testdummy.c testgram.c testspline.c testsvd.c testutils.c \
         vector_add.c vector_dist.c zcopy_vector.c zdotc.c zdotu.c \
         zmatrix_add.c zmatrix_mult.c zmatrix_sub.c zmatrix_transpose.c \
         zmatrix_vector_mult.c znorm.c znormsq.c zrscale_vector.c \
         zscale_vector.c zsum_vector.c zsym_linsolve.c

OBJ = allocate.o cabs.o cadd.o cconj.o cmult.o complex_zero.o \
         copy_vector.o cprob.o csub.o dotp.o eigensystem.o expi.o \
         gram_schmidt.o herm_matrix_mult.o herm_matrix_vector_mult.o icmult.o \
         init.o integration.o linsolve.o matherr.o matrix_add.o matrix_elt.o \
         matrix_mult.o matrix_sub.o matrix_transpose.o matrix_vector_mult.o \
         matrixexp.o minimum.o norm.o normsq.o project.o random.o rcmult.o \
         rect_transpose.o roots.o runge-kutta.o scale_vector.o splines.o \
         sum_vector.o svd.o sym_linsolve.o sym_matrix_mult.o \
         sym_matrix_vector_mult.o sympos_linsolve.o vector_add.o vector_dist.o \
         zcopy_vector.o zdotc.o zdotu.o zmatrix_mult.o zmatrix_transpose.o \
         zmatrix_vector_mult.o znorm.o znormsq.o zrscale_vector.o \
         zscale_vector.o zsum_vector.o zsym_linsolve.o \
         zmatrix_add.o zmatrix_sub.o

EXEOBJ = test_transpose.o testdummy.o testgram.o testspline.o testsvd.o \
         testutils.o 

LIBOBJ= $(MATHLIB)($(OBJ))

EXEC=  test_transpose testdummy testgram testspline testsvd \
         testutils 

.PHONY: $(EXEC) clean tar co library

library: $(LIBOBJ) $(OBJ) $(HEADERS)
#	mkdir -p ${LIBDIR} ${INCDIR}
	ranlib $(MATHLIB)
#	cp $(HEADERS) ${INCDIR}

all: $(EXEC)

$(EXEC): %: %.o library
	$(LD) $(LDFLAGS) -o $@ $@.o $(MATHLIBS)

tar: 
	tar cf $(TARFILE) $(AUX) $(HEADERS) $(SRC)

tar_rcs: 
	tar cf $(TARFILE) $(AUX) $(HEADERS) $(SRC) RCS

clean:
	rm -f *.o a.out $(EXEC)

co:
	$(CO) $(AUX) $(HEADERS) $(SRC)    

$(LIBOBJ): $(HEADERS)

# is there a better way to do this??
%.o: %.c $(HEADERS)
	$(CC) $(CFLAGS) -c -o $@ $<

$(EXEOBJ): $(HEADERS)

