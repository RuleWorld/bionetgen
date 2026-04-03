CXX = g++
INCLUDE = -I. -I../nauty 
# `perl -MConfig -e 'print join(" ", @Config{qw(ccflags optimize cccdlflags)}, "-I$Config{archlib}/CORE")'`
PERL_CXXFLAGS = $(shell perl -MConfig -e 'print join(" ", @Config{qw(ccflags optimize cccdlflags)}, "-I$$Config{archlib}/CORE")' )
# `perl -MConfig -e 'print $Config{lddlflags}'`
PERL_LDOPTS = $(shell perl -MConfig -e 'print $$Config{lddlflags}' )
DEPS = BNGcore.hpp ../nauty/nauty.h ../nauty/nausparse.h
OBJ = StateType.o State.o NodeType.o Node.o PatternGraph.o Map.o Transformation.o BNGcore_wrap.o
NAUTYDIR = ../nauty
NAUTYOBJ = $(NAUTYDIR)/nauty.o $(NAUTYDIR)/nausparse.o $(NAUTYDIR)/nautil.o

BNGcore.so: $(OBJ)
	$(CXX) $(PERL_LDOPTS) $(OBJ) $(NAUTYOBJ) -o $@

BNGcore_wrap.o: BNGcore_wrap.cxx
	$(CXX) $(PERL_CXXFLAGS) $(INCLUDE) -c BNGcore_wrap.cxx

%.o: %.cpp $(DEPS)
	$(CXX) $(PERL_CXXFLAGS) $(INCLUDE) -c $<

BNGcore_wrap.cxx: BNGcore.i
	swig -c++ -perl5 BNGcore.i

