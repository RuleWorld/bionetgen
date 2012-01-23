#----!!!! Operating system specific variables.  Override these by making a
#         file named makeincl with definitions of the variables
#         listed below. 



# C compiler options
COPTS  = -O3 -ffast-math -w  # debug symbols: -g -pg
DEFINE = -DAPPEND_UNDERSCORE -DNOBLAS_E -DRUN_NETWORK_VERSION=\"${VERSION}\" 
CFLAGS = $(COPTS) -I${INCDIR} $(DEFINE)

# Linker options
LDFLAGS = -lm -L${LIBDIR}



# version
VERSION=2.0

# include directory
INCDIR = include
# library directory
LIBDIR = lib
# library archive directory
LIBSOURCE = ../libsource
# Output directory for executables
BINDIR = ../bin

# library prefixes
MATHUTILS = Mathutils
CVODE = sundials

# library files
MATHUTILS_LIB = ${LIBDIR}/libmathutils.a 
CVODE_LIB = ${LIBDIR}/libsundials_cvode.a ${LIBDIR}/libsundials_nvecserial.a ${LIBDIR}/libsundials_shared.a

# cvode archive
CVODE_ARCHIVE = ${LIBSOURCE}/cvode-2.1.1.tar.gz



# Optional include file to override default variables
-include makeincl

# recipes that do not create files
.PHONY: clean distclean


# run_network executable
run_network:  run_network.o network.o $(MATHUTILS_LIB) $(CVODE_LIB)
	mkdir -p $(BINDIR)
	$(CC) $(LDFLAGS) -o ${BINDIR}/run_network run_network.o network.o \
         $(MATHUTILS_LIB) $(CVODE_LIB)




# object files
network.o:  network.c network.h $(MATHUTILS_LIB) $(CVODE_LIB)
	$(CC) $(CFLAGS) -c network.c
	
run_network.o:  run_network.c network.h $(MATHUTILS_LIB) $(CVODE_LIB)
	$(CC) $(CFLAGS) -c run_network.c




# libraries
$(CVODE_LIB): $(CVODE_ARCHIVE)
	mkdir -p $(LIBDIR) $(INCDIR)
	rm -rf $(CVODE)
	tar -xzf $(CVODE_ARCHIVE)
	cd $(CVODE); ./configure --prefix=$(CURDIR) --disable-shared; $(MAKE); $(MAKE) install

$(MATHUTILS_LIB):  $(LIBSOURCE)/$(MATHUTILS).tar.gz
	mkdir -p $(LIBDIR) $(INCDIR)
	rm -rf $(MATHUTILS)
	tar -xzf $(LIBSOURCE)/$(MATHUTILS).tar.gz
	cd $(MATHUTILS); $(MAKE)



# clean scripts
clean:
	rm -f *.o *.a ;
	if test -d ${CVODE} ; then \
	    cd ${CVODE} ;          \
	    ${MAKE} clean ;        \
	fi ;
	if test -d ${MATHUTILS} ; then \
	    cd ${MATHUTILS} ;          \
	    ${MAKE} clean ;            \
	fi ;



#----!!!! For making distributions of source code
#FILES=
#LICENSE INSTALL Makefile  network.h network.cpp PLA.h PLA.cpp Mathutils/ run_network.cpp HOW_TO_INSTALL README cvode_ser.tar.gz
#SOURCES=LICENSE INSTALL Makefile network.h network.cpp PLA.h PLA.cpp run_network.cpp HOW_TO_INSTALL README

#dist: $(FILES)
#	tar czf dist.tgz $(FILES)

#ci: $(FILES)
#	ci -zLT -s- -u $(SOURCES)
#	rcs -U $(SOURCES)
#	chmod u+w $(SOURCES)

#co: $(FILES)
#	co -zLT -s- -u $(SOURCES)
