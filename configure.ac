#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.69])
AC_INIT([hashtable], [0.1], [eric@ericlagergren.com])
AM_INIT_AUTOMAKE
AC_CONFIG_SRCDIR([main.c])
AC_CONFIG_HEADERS([config.h])
LT_INIT

# Checks for header files.
AC_CHECK_HEADERS([inttypes.h stdint.h stdlib.h string.])

# Checks for typedefs, structures, and compiler characteristics.
AC_CHECK_HEADER_STDBOOL
AC_TYPE_INT64_T
AC_TYPE_SIZE_T
AC_TYPE_UINT64_T

# Checks for library functions.
AC_FUNC_MALLOC

AC_PROG_CPP
AC_LANG([C])

AC_CONFIG_FILES([Makefile
				src/Makefile])

AC_CONFIG_MACRO_DIRS([m4])
AC_OUTPUT
