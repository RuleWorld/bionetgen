#----!!!! Operating system specific variables.  Override these by making a
#         file named makeincl with definitions of the variables
#         listed below. 

# version
VERSION=3.0

# CMake
CMAKELISTS_DIR = $(CURDIR)

# include directory
INCDIR = include

# library directory
LIBDIR = lib

# library archive directory
LIBSOURCE = ../libsource

# Output directories for executables
NETWORK_BINDIR = bin
BNG_BINDIR = ../bin

# library prefixes
MATHUTILS = Mathutils
CVODE = cvode-2.6.0
#GSL = gsl-1.9
MUPARSER = muparser_v134

# library files
MATHUTILS_LIB = ${LIBDIR}/libmathutils.a 
CVODE_LIB = ${LIBDIR}/libsundials_cvode.a ${LIBDIR}/libsundials_nvecserial.a
#GSL_LIB = ${LIBDIR}/libgsl.a ${LIBDIR}/libgslcblas.a
MUPARSER_LIB = ${LIBDIR}/libmuparser.a

# Optional include file to override default variables
-include makeincl

# recipes that do not create files
.PHONY: clean distclean

# run_network executable
#run_network: $(MATHUTILS_LIB) $(CVODE_LIB) $(GSL_LIB) $(MUPARSER_LIB)
run_network: $(MATHUTILS_LIB) $(CVODE_LIB) $(MUPARSER_LIB)
	mkdir -p $(NETWORK_BINDIR)
	cd $(NETWORK_BINDIR); cmake $(CMAKELISTS_DIR); make;
	mkdir -p $(CURDIR)/$(BNG_BINDIR)
	cp -f $(NETWORK_BINDIR)/run_network $(CURDIR)/$(BNG_BINDIR)

# libraries
$(CVODE_LIB):  $(LIBSOURCE)/$(CVODE).tar.gz
	mkdir -p $(LIBDIR) $(INCDIR)
	rm -rf $(CVODE)
	tar -xzf $(LIBSOURCE)/$(CVODE).tar.gz
	cd $(CVODE);  ./configure --prefix=$(CURDIR) --disable-shared;  make;  make install

#$(GSL_LIB):  $(LIBSOURCE)/$(GSL).tar.gz
#	mkdir -p $(LIBDIR) $(INCDIR)
#	rm -rf $(GSL)
#	tar -xzf $(LIBSOURCE)/$(GSL).tar.gz
#	cd $(GSL);  ./configure --prefix=$(CURDIR) --disable-shared;  make;  make install

$(MUPARSER_LIB):  $(LIBSOURCE)/$(MUPARSER).tar.gz
	mkdir -p $(LIBDIR) $(INCDIR)
	rm -rf $(MUPARSER)
	tar -xzf $(LIBSOURCE)/$(MUPARSER).tar.gz
	cd $(MUPARSER); ./configure --prefix=$(CURDIR) --disable-shared;  make;  make install

$(MATHUTILS_LIB):  $(LIBSOURCE)/$(MATHUTILS).tar.gz
	mkdir -p $(LIBDIR) $(INCDIR)
	rm -rf $(MATHUTILS)
	tar -xzf $(LIBSOURCE)/$(MATHUTILS).tar.gz
	cd $(MATHUTILS); make

# clean scripts
clean:
	rm -f *.o *.a
    rm $(NETWORK_BINDIR)/run_network $(NETWORK_BINDIR)/CMakeCache.txt $(NETWORK_BINDIR)/cmake_install.cmake $(NETWORK_BINDIR)/Makefile
    rm -r $(NETWORK_BINDIR)/CMakeFiles
	if test -d ${CVODE} ; then \
	    cd ${CVODE} ;          \
	    ${MAKE} clean ;        \
	fi;
	if test -d ${GSL} ; then \
	    cd ${GSL} ;          \
	    ${MAKE} clean ;      \
	fi;
	if test -d ${MUPARSER} ; then \
	    cd ${MUPARSER} ;          \
	    ${MAKE} clean ;           \
	fi;
	if test -d ${MATHUTILS} ; then \
	    cd ${MATHUTILS} ;          \
	    ${MAKE} clean ;            \
	fi;

distclean:
	if test -d ${CVODE} ; then \
	    cd ${CVODE} ;          \
	    ${MAKE} distclean ;    \
	fi;
	if test -d ${GSL} ; then \
	    cd ${GSL} ;          \
	    ${MAKE} distclean ;  \
	fi;
	if test -d ${MUPARSER} ; then \
	    cd ${MUPARSER} ;          \
	    ${MAKE} distclean ;       \
	fi;


#----!!!! For making distributions of source code

#FILES=LICENSE INSTALL Makefile  network.h network.cpp PLA.h PLA.cpp Mathutils/ run_network.cpp HOW_TO_INSTALL README cvode_ser.tar.gz
#SOURCES=LICENSE INSTALL Makefile network.h network.cpp PLA.h PLA.cpp run_network.cpp HOW_TO_INSTALL README

#dist: $(FILES)
#	tar czf dist.tgz $(FILES)

#ci: $(FILES)
#	ci -zLT -s- -u $(SOURCES)
#	rcs -U $(SOURCES)
#	chmod u+w $(SOURCES)

#co: $(FILES)
#	co -zLT -s- -u $(SOURCES)
