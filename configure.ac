#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.69])
AC_INIT(kite-llvm, 0.1.0, mooneer@kite-language.org)
LT_INIT
LT_LANG([C++])
AC_CONFIG_SRCDIR([src/apps/ikt.cpp])
AC_CONFIG_HEADERS([config.h])
AC_CONFIG_MACRO_DIR([m4])
AM_INIT_AUTOMAKE

# Checks for programs.
AC_PROG_CC

# Checks for libraries.
AX_BOOST_BASE([1.49])
AX_BOOST_FILESYSTEM
AX_BOOST_REGEX
AX_BOOST_SYSTEM
AX_CHECK_OPENSSL
AX_LLVM
AX_BOEHMGC

# Checks for header files.
AC_CHECK_HEADERS([fcntl.h netdb.h sys/socket.h sys/time.h unistd.h])

# Checks for typedefs, structures, and compiler characteristics.
AC_CHECK_HEADER_STDBOOL
AC_C_INLINE
AC_TYPE_MODE_T
AC_TYPE_SIZE_T
AC_TYPE_UINT64_T
AC_CHECK_TYPES([ptrdiff_t])

# Checks for library functions.
AC_FUNC_FORK
AC_FUNC_MALLOC
AC_FUNC_MKTIME
AC_FUNC_MMAP
AC_CHECK_FUNCS([getcwd gettimeofday memset mkdir munmap rmdir select setenv socket strtol])

AC_OUTPUT(Makefile src/Makefile)
