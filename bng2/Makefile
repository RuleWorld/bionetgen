# CMake
CMAKELISTS_DIR = $(CURDIR)

BNG_VERSION = 2.7.2
DIST_DIR = BioNetGen-$(BNG_VERSION)
DIST_TARBALL = BioNetGen-$(BNG_VERSION).tgz
GROUP_BINDIR = bin

NETWORK_DIR = Network3
NETWORK_BIN = run_network

NFSIM_BIN = NFsim
NFSIM_DIR = nfsim_src

# recipes that do not create files
.PHONY: clean test


all: $(NFSIM_BIN) $(NETWORK_BIN)

dist: $(DIST_TARBALL)

$(DIST_TARBALL): $(DIST_DIR)
	tar czf $(DIST_TARBALL) $(DIST_DIR)

$(DIST_DIR): $(NFSIM_BIN) $(NETWORK_BIN)
	mkdir -p $(DIST_DIR)
	cd $(NETWORK_DIR); cp -fr lib include ../$(DIST_DIR)
	cp -fr bin BNG2.pl CHANGES.txt CREDITS.txt Models2 Perl2 Network3 Validate VERSION $(DIST_DIR)

$(NETWORK_BIN):
	cd $(NETWORK_DIR); cp Makefile.cmake Makefile; $(MAKE)

$(NFSIM_BIN):
	git submodule init; git submodule update
	mkdir -p $(NFSIM_DIR)/lib; cd $(NFSIM_DIR)/lib; cmake ..; make
	mkdir -p $(GROUP_BINDIR)
	cp $(NFSIM_DIR)/lib/$(NFSIM_BIN) $(GROUP_BINDIR)

# clean script
clean:
	rm -fr $(DIST_DIR)
	rm -fr $(DIST_TARBALL)
	cd $(NFSIM_DIR) ; $(MAKE) clean
	cd $(NETWORK_DIR) ; $(MAKE) clean
	cd $(GROUP_BINDIR); rm $(NETWORK_BIN); rm $(NFSIM_BIN)

test: $(NFSIM_BIN) $(NETWORK_BIN)
	cd Validate; perl validate_examples.pl
