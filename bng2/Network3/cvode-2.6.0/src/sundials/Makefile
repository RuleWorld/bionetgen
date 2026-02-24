# -----------------------------------------------------------------
# $Revision: 1.12 $
# $Date: 2009/02/17 02:52:53 $
# -----------------------------------------------------------------
# Programmer(s): Radu Serban and Aaron Collier @ LLNL
# -----------------------------------------------------------------
# Copyright (c) 2002, The Regents of the University of California.
# Produced at the Lawrence Livermore National Laboratory.
# All rights reserved.
# For details, see the LICENSE file.
# -----------------------------------------------------------------
# Makefile for SHARED module
#
# src/sundials/Makefile.  Generated from Makefile.in by configure.
# -----------------------------------------------------------------

SHELL = /bin/sh

srcdir       = .
builddir     = .
abs_builddir = /Users/faeder/Library/CloudStorage/OneDrive-UniversityofPittsburgh/Code/bionetgen/bng2/Network3/cvode-2.6.0/src/sundials
top_builddir = ../..
prefix       = /Users/faeder/Library/CloudStorage/OneDrive-UniversityofPittsburgh/Code/bionetgen/bng2/Network3
exec_prefix  = ${prefix}
includedir   = ${prefix}/include
libdir       = ${exec_prefix}/lib
bindir       = ${exec_prefix}/bin

INSTALL        = /usr/bin/install -c
INSTALL_PROG   = ${INSTALL}
INSTALL_HEADER = ${INSTALL} -m 644

LIBTOOL      = $(SHELL) $(top_builddir)/libtool
LIBTOOL_DEPS = config/ltmain.sh

CPP      = cc -E
CPPFLAGS = 
CC       = cc
CFLAGS   = -g -O2
LDFLAGS  = 
LIBS     = -lm 

LAPACK_ENABLED = no

top_srcdir = $(srcdir)/../..

INCLUDES = -I$(top_srcdir)/include -I$(top_builddir)/include

SHARED_SRC_FILES = sundials_direct.c sundials_band.c sundials_dense.c sundials_iterative.c sundials_math.c sundials_nvector.c sundials_spgmr.c sundials_spbcgs.c sundials_sptfqmr.c

SHARED_OBJ_FILES = $(SHARED_SRC_FILES:.c=.o)

SHARED_LIB_FILES = $(SHARED_SRC_FILES:.c=.lo)

mkinstalldirs = $(SHELL) $(top_srcdir)/config/mkinstalldirs
rminstalldirs = $(SHELL) $(top_srcdir)/config/rminstalldirs

# ----------------------------------------------------------------------------------------------------------------------


all:
	@if test "X${LAPACK_ENABLED}" = "Xyes"; then \
	  make lib_with_bl;                          \
	else                                         \
	  make lib_without_bl;                       \
	fi

lib_without_bl: $(SHARED_LIB_FILES)

lib_with_bl: $(SHARED_LIB_FILES)

install:
	$(mkinstalldirs)  $(includedir)/sundials
	$(INSTALL_HEADER) $(top_srcdir)/include/sundials/sundials_direct.h     $(includedir)/sundials/
	$(INSTALL_HEADER) $(top_srcdir)/include/sundials/sundials_band.h       $(includedir)/sundials/
	$(INSTALL_HEADER) $(top_srcdir)/include/sundials/sundials_dense.h      $(includedir)/sundials/
	$(INSTALL_HEADER) $(top_srcdir)/include/sundials/sundials_iterative.h  $(includedir)/sundials/
	$(INSTALL_HEADER) $(top_srcdir)/include/sundials/sundials_spgmr.h      $(includedir)/sundials/
	$(INSTALL_HEADER) $(top_srcdir)/include/sundials/sundials_spbcgs.h     $(includedir)/sundials/
	$(INSTALL_HEADER) $(top_srcdir)/include/sundials/sundials_sptfqmr.h    $(includedir)/sundials/
	$(INSTALL_HEADER) $(top_srcdir)/include/sundials/sundials_math.h       $(includedir)/sundials/
	$(INSTALL_HEADER) $(top_srcdir)/include/sundials/sundials_types.h      $(includedir)/sundials/
	$(INSTALL_HEADER) $(top_srcdir)/include/sundials/sundials_nvector.h    $(includedir)/sundials/
	$(INSTALL_HEADER) $(top_srcdir)/include/sundials/sundials_fnvector.h   $(includedir)/sundials/
	$(INSTALL_HEADER) $(top_builddir)/include/sundials/sundials_config.h   $(includedir)/sundials/
	@if test "X${LAPACK_ENABLED}" = "Xyes"; then \
	  $(INSTALL_HEADER) $(top_srcdir)/include/sundials/sundials_lapack.h     $(includedir)/sundials/ ; \
	fi

uninstall:
	rm -f $(includedir)/sundials/sundials_direct.h
	rm -f $(includedir)/sundials/sundials_band.h
	rm -f $(includedir)/sundials/sundials_dense.h
	rm -f $(includedir)/sundials/sundials_lapack.h
	rm -f $(includedir)/sundials/sundials_iterative.h
	rm -f $(includedir)/sundials/sundials_spgmr.h
	rm -f $(includedir)/sundials/sundials_spbcgs.h
	rm -f $(includedir)/sundials/sundials_sptfqmr.h
	rm -f $(includedir)/sundials/sundials_math.h
	rm -f $(includedir)/sundials/sundials_types.h
	rm -f $(includedir)/sundials/sundials_nvector.h
	rm -f $(includedir)/sundials/sundials_fnvector.h
	rm -f $(includedir)/sundials/sundials_config.h
	$(rminstalldirs) $(includedir)/sundials

clean:
	rm -f $(SHARED_LIB_FILES)
	rm -f $(SHARED_OBJ_FILES)
	rm -rf .libs

distclean: clean
	rm -f $(top_builddir)/include/sundials/sundials_config.h
	rm -f Makefile

sundials_direct.lo: $(srcdir)/sundials_direct.c
	$(LIBTOOL) --mode=compile $(CC) $(CPPFLAGS) $(INCLUDES) $(CFLAGS) -c $(srcdir)/sundials_direct.c
sundials_band.lo: $(srcdir)/sundials_band.c
	$(LIBTOOL) --mode=compile $(CC) $(CPPFLAGS) $(INCLUDES) $(CFLAGS) -c $(srcdir)/sundials_band.c
sundials_dense.lo: $(srcdir)/sundials_dense.c
	$(LIBTOOL) --mode=compile $(CC) $(CPPFLAGS) $(INCLUDES) $(CFLAGS) -c $(srcdir)/sundials_dense.c
sundials_iterative.lo: $(srcdir)/sundials_iterative.c
	$(LIBTOOL) --mode=compile $(CC) $(CPPFLAGS) $(INCLUDES) $(CFLAGS) -c $(srcdir)/sundials_iterative.c
sundials_spgmr.lo: $(srcdir)/sundials_spgmr.c
	$(LIBTOOL) --mode=compile $(CC) $(CPPFLAGS) $(INCLUDES) $(CFLAGS) -c $(srcdir)/sundials_spgmr.c
sundials_spbcgs.lo: $(srcdir)/sundials_spbcgs.c
	$(LIBTOOL) --mode=compile $(CC) $(CPPFLAGS) $(INCLUDES) $(CFLAGS) -c $(srcdir)/sundials_spbcgs.c
sundials_sptfqmr.lo: $(srcdir)/sundials_sptfqmr.c
	$(LIBTOOL) --mode=compile $(CC) $(CPPFLAGS) $(INCLUDES) $(CFLAGS) -c $(srcdir)/sundials_sptfqmr.c
sundials_math.lo: $(srcdir)/sundials_math.c
	$(LIBTOOL) --mode=compile $(CC) $(CPPFLAGS) $(INCLUDES) $(CFLAGS) -c $(srcdir)/sundials_math.c
sundials_nvector.lo: $(srcdir)/sundials_nvector.c
	$(LIBTOOL) --mode=compile $(CC) $(CPPFLAGS) $(INCLUDES) $(CFLAGS) -c $(srcdir)/sundials_nvector.c

libtool: $(top_builddir)/$(LIBTOOL_DEPS)
	@cd ${top_builddir} ;                 \
	 ${SHELL} ./config.status --recheck ; \
	 cd ${abs_builddir}
