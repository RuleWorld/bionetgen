#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.60])
AC_INIT([run_network], [3.0], [bionetgen.help@gmail.com])
AM_INIT_AUTOMAKE([-Wall -Werror foreign])
AC_CONFIG_SRCDIR([src/run_network.cpp])
AC_CONFIG_HEADERS([config.h])

# Checks for programs.
AC_PROG_LIBTOOL
AC_PROG_CXX
AC_PROG_AWK
AC_PROG_CC
AC_PROG_CPP
AC_PROG_INSTALL
AC_PROG_LN_S
AC_PROG_MAKE_SET
AM_PROG_CC_C_O

# Checks for libraries.

# Checks for header files.
AC_CHECK_HEADERS([limits.h stdlib.h string.h unistd.h])

# Checks for typedefs, structures, and compiler characteristics.
AC_HEADER_STDBOOL
AC_C_INLINE
AC_TYPE_SIZE_T

# Checks for library functions.
AC_FUNC_MALLOC
AC_FUNC_REALLOC
AC_CHECK_FUNCS([floor pow rint sqrt strchr strdup strpbrk])

AC_CONFIG_FILES([
 Makefile
 src/Makefile
])
#AC_CONFIG_FILES([
# Makefile
# mathutils-1.0/Makefile
# src/Makefile
#])
AC_CONFIG_SUBDIRS([
 cvode-2.6.0
 muparser_v134
])
#AC_CONFIG_SUBDIRS([
# mathutils-1.0
# cvode-2.6.0
# gsl-1.9
# muparser_v134
#])
AC_OUTPUT

