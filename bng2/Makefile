# version
VERSION=3.0

# CMake
CMAKELISTS_DIR = $(CURDIR)

FINAL_BINDIR = bin
NETWORK_DIR = Network3

NFSIM_BIN = NFsim
NFSIM_DIR = nfsim_src

SBML_TRANSLATOR_BIN = sbmlTranslator
SBML_TRANSLATOR_DIR = atomizer

# recipes that do not create files
.PHONY: clean 

all: $(NFSIM_BIN) $(SBML_TRANSLATOR_BIN)

$(NFSIM_BIN):
	git submodule init; git submodule update
	mkdir -p $(NFSIM_DIR)/lib; cd $(NFSIM_DIR)/lib; cmake ..; make
	cp $(NFSIM_DIR)/lib/$(NFSIM_BIN) $(FINAL_BINDIR)

$(SBML_TRANSLATOR_BIN):
	git submodule init; git submodule update
	cd $(SBML_TRANSLATOR_DIR); make
	cp $(SBML_TRANSLATOR_DIR)/dist/$(SBML_TRANSLATOR_BIN) $(FINAL_BINDIR)

# clean script
clean:
	cd $(NFSIM_DIR) ; $(MAKE) clean

