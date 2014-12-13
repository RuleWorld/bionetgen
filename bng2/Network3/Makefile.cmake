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

# library source directories
MATHUTILS = src/util/mathutils
CVODE = cvode-2.6.0
MUPARSER = muparser_v2_2_3

# library files
MATHUTILS_LIB = $(LIBDIR)/libmathutils.a 
CVODE_LIB = $(LIBDIR)/libsundials_cvode.a $(LIBDIR)/libsundials_nvecserial.a
MUPARSER_LIB = $(LIBDIR)/libmuparser.a

# recipes that do not create files
.PHONY: clean 

# run_network executable
run_network: $(MATHUTILS_LIB) $(CVODE_LIB) $(MUPARSER_LIB)
	mkdir -p $(NETWORK_BINDIR)
	cd $(NETWORK_BINDIR); cmake $(CMAKELISTS_DIR); make;
	mkdir -p $(BNG_BINDIR)
	cp -f $(NETWORK_BINDIR)/run_network $(BNG_BINDIR)

# libraries
$(CVODE_LIB):  
	mkdir -p $(LIBDIR) $(INCDIR)
	if test -d $(LIBSOURCE) ; then \
	    rm -rf $(CVODE) ; \
	    tar -xzf $(LIBSOURCE)/$(CVODE).tar.gz ; \
	fi;
	cd $(CVODE); ./configure --prefix=$(CURDIR) --disable-shared;  make;  make install

$(MUPARSER_LIB):  
	mkdir -p $(LIBDIR) $(INCDIR)
	if test -d $(LIBSOURCE) ; then \
	    rm -rf $(MUPARSER) ; \
	    tar -xzf $(LIBSOURCE)/$(MUPARSER).tar.gz ; \
	fi;
	cd $(MUPARSER); ./configure --prefix=$(CURDIR) --disable-shared;  make;  make install	

$(MATHUTILS_LIB):  
	mkdir -p $(LIBDIR) $(INCDIR)
	cd $(MATHUTILS); make; \
	mv libmathutils.a $(CMAKELISTS_DIR)/$(LIBDIR); \
	cp mathutils.h $(CMAKELISTS_DIR)/$(INCDIR)

# clean script
clean:
	rm -rf $(CVODE) $(MUPARSER) $(NETWORK_BINDIR) $(LIBDIR) $(INCDIR)
	cd $(MATHUTILS) ; $(MAKE) clean
