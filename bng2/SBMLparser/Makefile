SHELL = /bin/sh
BUILD = ./build
DIST = ./dist

.PHONY: all install clean


all: 
ifeq ($(OS),Windows_NT)
ifeq ($(shell uname -o),Cygwin)
		python pyinstaller2/pyinstaller.py sbmlTranslator.spec;
else
		python pyinstaller2/pyinstaller.py sbmlTranslator_windows.spec ;
endif
else
	python pyinstaller2/pyinstaller.py sbmlTranslator.spec ;
endif


update:

	if ! test -d pyinstaller2 ; then \
		unzip pyinstaller2.zip;   \
	fi ;
	python updateDistribution.py -c;


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
	if test -d SBMLparser ; then \
	    rm -rf SBMLparser ;          \
	fi ;	
	if test -d pyinstaller2 ; then \
	    rm -rf pyinstaller2 ;          \
	fi ;	

	find . -name '*.pyc' -delete
	