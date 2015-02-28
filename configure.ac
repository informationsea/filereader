#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.68])
AC_INIT([filereader], [0.0.1], [okamura@informationsea.info])
AC_CONFIG_AUX_DIR([build-aux])
AM_INIT_AUTOMAKE([-Wall])

AC_CONFIG_SRCDIR([src])
AC_CONFIG_HEADERS([src/config.h])

AC_CANONICAL_BUILD
AC_CANONICAL_HOST

# Checks for programs.
AM_PROG_AR
AC_CONFIG_MACRO_DIR([m4])
ACLOCAL_AMFLAGS="-I m4"
AM_SILENT_RULES([yes])
AM_PROG_LIBTOOL([shared])

m4_ifdef([AC_CHECK_CPPCUTTER], [AC_CHECK_CPPCUTTER], [ac_cv_use_cutter="no"])
AM_CONDITIONAL([WITH_CUTTER], [test "$ac_cv_use_cutter" != "no"])

AC_PROG_CXX
AC_PROG_AWK
AC_PROG_CC
AC_PROG_CPP
AC_PROG_INSTALL
AC_PROG_LN_S
AC_PROG_MAKE_SET
AC_PROG_RANLIB

# Checks for libraries.
PKG_CHECK_MODULES(zlib, zlib >= 0.28)

# Checks for header files.
AC_CHECK_HEADERS([fcntl.h string.h])

# Checks for typedefs, structures, and compiler characteristics.
#AC_CHECK_HEADER_STDBOOL
AC_TYPE_OFF_T
AC_TYPE_SIZE_T

# Checks for library functions.
AC_FUNC_FSEEKO
AC_FUNC_MALLOC
AC_FUNC_REALLOC
AC_FUNC_MMAP
AC_CHECK_FUNCS([strerror strtol munmap strndup])

AC_CONFIG_FILES([Makefile src/Makefile test/Makefile])
AC_OUTPUT
