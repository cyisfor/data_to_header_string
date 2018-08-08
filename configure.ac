AC_PREREQ([2.69])
AC_INIT([data_to_header_string], [1.0], [zaroboogs])
AC_CONFIG_SRCDIR([d2h_convert.c])
AC_CONFIG_AUX_DIR([build-aux])
AC_CONFIG_MACRO_DIR([m4])
AC_CONFIG_HEADERS([config.h])
AM_INIT_AUTOMAKE([foreign])
LT_INIT([disable-shared])

# Checks for programs.
AC_PROG_CC

# Checks for libraries.

# Checks for header files.
AC_CHECK_HEADERS([stdlib.h string.h unistd.h])

# Checks for typedefs, structures, and compiler characteristics.
AC_CHECK_HEADER_STDBOOL
AC_TYPE_SIZE_T
AC_TYPE_SSIZE_T

# Checks for library functions.
AC_FUNC_MMAP
AC_CHECK_FUNCS([strpbrk])

AC_CONFIG_FILES([Makefile])
AC_OUTPUT
