################################################################################
# Automatically-generated file. Do not edit!
################################################################################
RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include src/core/subdir.mk
-include src/gnuplot-cpp/subdir.mk
-include src/subdir.mk
-include subdir.mk
-include objects.mk

LIBDIR = lib
INCDIR = include
LIBSOURCE = libsource 
MPIDIR = mpibuild

GSL = gsl-1.16
OPENMPI = openmpi-1.8.1
BINDIR = $(CURDIR)/bin

GSL_LIB = ${LIBDIR}/libgsl.a ${LIBDIR}/libgslcblas.a
MPI_EXE =$(CURDIR)/$(OPENMPI)/$(MPIDIR)/mpic++ 
BNG_EXE = ../../Network3/bin/run_network


$(GSL_LIB): $(GSL).tar.gz
	mkdir -p $(LIBDIR) $(INCDIR)
	rm -rf $(GSL)
	tar -xzf $(GSL).tar.gz
	cd $(GSL);  ./configure --prefix=$(CURDIR) --disable-shared;  make;  make install

$(MPI_EXE): $(OPENMPI).tar.gz
	rm -rf $(OPENMPI)
	tar -zxvf $(OPENMPI).tar.gz
	mkdir -p $(OPENMPI)/$(MPIDIR)
	cd $(OPENMPI); ./configure --prefix=$(CURDIR)/$(OPENMPI)/$(MPIDIR) --disable-mpi-f90 --disable-mpi-f77;  make all install

#CFLAGS = -I$(CURDIR)/$(INCDIR) -I$(CURDIR)/$(INCDIR)/gsl -I$(CURDIR)/$(INCDIR)/openmpi/ompi -std=c++0x
CFLAGS = -I$(CURDIR)/$(INCDIR) -I$(CURDIR)/$(INCDIR)/gsl -std=c++0x
CXX = $(CURDIR)/$(OPENMPI)/$(MPIDIR)/bin/mpic++

# All Target
all: work_manager
work_manager: $(GSL_LIB) $(MPI_EXE) $(OBJS) 
	cp $(CURDIR)/$(OPENMPI)/$(MPIDIR)/bin/mpirun ../bin/
	@echo 'Building target: $@'
	@echo 'Invoking: GCC C++ Linker'
	$(CXX) $(CFLAGS) -o "work_manager" $(OBJS) $(GSL_LIB)  
	cp CMakeListsBNG.txt ../../Network3/CMakeLists.txt
	cd ../../Network3; cp Makefile.cmake Makefile; make clean; make 
	cp ../../Network3/bin/run_network ../bin/
	g++ run.cpp -o run 
	@echo 'Finished building target: $@'
	@echo ' '



# Other Targets
clean:
	-$(RM) $(OBJS) work_manager run run_network 
	-@echo ' '

distclean: 
	-$(RM) -rf $(OBJS) work_manager run run_network mpirun $(GSL) $(OPENMPI) $(INCDIR) $(LIBDIR) bin share

.PHONY: all clean dependents
.SECONDARY:

-include ../makefile.targets
