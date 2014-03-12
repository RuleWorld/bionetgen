SHELL = /bin/sh
BUILD = ./build
DIST = ./dist

.PHONY: all install clean


all: 
	python pyinstaller2/pyinstaller.py sbmlTranslator.spec ;
	cp -r ./config ${DIST} ;

install:
	cp -r ${DIST}/config ../bin ;
	cp  ${DIST}/sbmlTranslator ../bin ;

clean:
	if test -d ${BUILD} ; then \
	    rm -rf ${BUILD} ;          \
	fi ;
	if test -d ${DIST} ; then \
	    rm -rf ${DIST} ;          \
	fi ;
	rm -rf *pyc


