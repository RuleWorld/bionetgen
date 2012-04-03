SHELL = /bin/sh
NETDIR2 = ./Network2
NETDIR3 = ./Network3

.PHONY: install network2 network3 clean distclean

install: 
	cd $(NETDIR3); $(MAKE)

network2:
	cd $(NETDIR2); $(MAKE)

network3:
	cd $(NETDIR3); $(MAKE)

clean:
	if test -d ${NETDIR2} ; then \
	    cd ${NETDIR2} ;          \
	    ${MAKE} clean ;          \
	fi ;
	if test -d ${NETDIR3} ; then \
	    cd ${NETDIR3} ;          \
	    ${MAKE} clean ;          \
	fi ;

distclean:
	if test -d ${NETDIR3} ; then \
	    cd ${NETDIR3} ;          \
	    ${MAKE} distclean ;      \
	fi ;
