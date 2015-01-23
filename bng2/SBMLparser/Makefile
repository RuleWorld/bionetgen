SHELL = /bin/sh
BUILD = ./build
DIST = ./dist

.PHONY: all install clean


all: 
	python pyinstaller2/pyinstaller.py sbmlTranslator.spec ;
	cp -r ./config ${DIST} ;

install:
ifeq ($(OS),Windows_NT)
	cp  ${DIST}/sbmlTranslator ../bin/sbmlTranslator.exe;
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
	rm -rf *pyc