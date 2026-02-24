# -----------------------------------------------------------------
# $Revision: 1.16 $
# $Date: 2006/11/29 00:05:01 $
# -----------------------------------------------------------------
# Programmer(s): Radu Serban and Aaron Collier @ LLNL
# -----------------------------------------------------------------
# Copyright (c) 2002, The Regents of the University of California.
# Produced at the Lawrence Livermore National Laboratory.
# All rights reserved.
# For details, see the LICENSE file.
# -----------------------------------------------------------------
# Main Makefile for SUNDIALS suite
#
# Makefile.  Generated from Makefile.in by configure.
# -----------------------------------------------------------------

SHELL = /bin/sh



srcdir       = .
builddir     = .
abs_builddir = /Users/faeder/Library/CloudStorage/OneDrive-UniversityofPittsburgh/Code/bionetgen/bng2/Network3/cvode-2.6.0
top_builddir = .
prefix       = /Users/faeder/Library/CloudStorage/OneDrive-UniversityofPittsburgh/Code/bionetgen/bng2/Network3
exec_prefix  = ${prefix}
includedir   = ${prefix}/include
libdir       = ${exec_prefix}/lib
bindir       = ${exec_prefix}/bin

INSTALL      = /usr/bin/install -c
INSTALL_PROG = ${INSTALL}

SLV_MODULES  = src/sundials src/nvec_ser src/cvode
EXS_MODULES  = 

ALL_MODULES  = src/sundials src/nvec_ser src/cvode 

EXS_INSTDIR  = 

top_srcdir = $(srcdir)

mkinstalldirs = $(SHELL) $(top_srcdir)/config/mkinstalldirs

# -----------------------------------------------------------------

all:
	@for i in ${ALL_MODULES} ; do       \
	   if test -d $${i} ; then          \
	     echo "----------------------"; \
	     echo "Make $${i}..." ;         \
	     echo "----------------------"; \
	     cd $${i} ;                     \
	     ${MAKE} ;                      \
	     cd ${abs_builddir} ;           \
	     echo "" ;                      \
	   fi ;                             \
	 done

install:
	$(mkinstalldirs) $(bindir)
	$(INSTALL_PROG)  $(top_builddir)/bin/sundials-config    $(bindir)/
	@if test -f $(top_builddir)/bin/fortran-update.sh ; then              \
	  $(INSTALL_PROG)  $(top_builddir)/bin/fortran-update.sh  $(bindir)/ ;\
	fi
	@for i in ${SLV_MODULES} ; do                                         \
	   if test -d $${i} ; then                                            \
	     echo "----------------------";                                   \
	     echo "Install $${i}..." ;                                        \
	     echo "----------------------";                                   \
	     cd $${i} ;                                                       \
	     ${MAKE} install ;                                                \
	     cd ${abs_builddir} ;                                             \
	     echo "" ;                                                        \
	   fi ;                                                               \
	 done
	@if test "X${EXS_MODULES}" = "X" ; then                               \
	   : ;                                                                \
	 else                                                                 \
	   if test "X${EXS_INSTDIR}" = "Xno"; then                            \
	     echo "" ;                                                        \
	     echo "" ;                                                        \
	     echo "Examples were built but installation was disabled." ;      \
	     echo "" ;                                                        \
	     echo "" ;                                                        \
	   else                                                               \
	     for i in ${EXS_MODULES} ; do                                     \
	       echo "----------------------";                                 \
	       echo "Install $${i}..." ;                                      \
	       echo "----------------------";                                 \
	       cd $${i} ;                                                     \
	       ${MAKE} install ;                                              \
	       cd ${abs_builddir} ;                                           \
	       echo "" ;                                                      \
	     done ;                                                           \
	   fi ;                                                               \
	 fi

clean:
	@for i in ${ALL_MODULES} ; do \
	   if test -d $${i} ; then    \
	     cd $${i} ;               \
	     ${MAKE} clean ;          \
	     cd ${abs_builddir} ;     \
	   fi ;                       \
	 done

uninstall:
	rm -f $(bindir)/sundials-config
	rm -f $(bindir)/fortran-update.sh
	@for i in ${ALL_MODULES} ; do       \
	   if test -d $${i} ; then          \
	     echo "----------------------"; \
	     echo "Uninstall $${i}..." ;    \
	     echo "----------------------"; \
	     cd $${i} ;                     \
	     ${MAKE} uninstall ;            \
	     cd ${abs_builddir} ;           \
	   fi ;                             \
	 done

# ---------------------------------------------------------------------

distclean:
	@for i in ${ALL_MODULES} ; do  \
	   if test -d $${i} ; then     \
	     cd $${i};                 \
	     ${MAKE} distclean;        \
	     cd ${abs_builddir} ;      \
	   fi ;                        \
	 done
	rm -f $(top_builddir)/config.log
	rm -f $(top_builddir)/config.status
	rm -f $(top_builddir)/config.cache
	rm -f $(top_builddir)/config.h
	rm -f $(top_builddir)/libtool
	rm -f $(top_builddir)/bin/sundials-config
	rm -f $(top_builddir)/bin/fortran-update.sh
	rm -f Makefile
