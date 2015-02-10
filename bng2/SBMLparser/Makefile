SHELL = /bin/sh
BUILD = ./build
DIST = ./dist

.PHONY: all install clean


all: 
ifeq ($(OS),Windows_NT)
    ifeq ($(shell uname -o),Cygwin)
    	python pyinstaller2/pyinstaller.py sbmlTranslator.spec ;
    else
	    python pyinstaller2/pyinstaller.py sbmlTranslator_windows.spec ;
    endif
else
	python pyinstaller2/pyinstaller.py sbmlTranslator.spec ;
endif


update:
	python updateDistribution.py -c		


install:
ifeq ($(OS),Windows_NT)
    ifeq ($(shell uname -o),Cygwin)
	    cp  ${DIST}/sbmlTranslator ../bin/sbmlTranslator.exe;
    else
	    cp  ${DIST}/sbmlTranslator.exe ../bin/sbmlTranslator.exe;
    endif
else
	cp  ${DIST}/sbmlTranslator ../bin/sbmlTranslator;
endif

	
clean:
	if test -d ${BUILD} ; then \
	    rm -rf ${BUILD} ;          \
	fi ;
	if test -d ${DIST} ; then \
	    rm -rf ${DIST} ;          \
	fi ;
	find . -name '*.pyc' -delete
	python updateDistribution.py -r	
