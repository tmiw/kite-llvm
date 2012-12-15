#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.69])
AC_INIT(kite-llvm, 0.1.0, mooneer@kite-language.org)
LT_INIT
LT_LANG([C++])
AC_CONFIG_SRCDIR([src/ikt.cpp])
AC_CONFIG_HEADERS([config.h])
AC_CONFIG_MACRO_DIR([m4])
AM_INIT_AUTOMAKE

# Checks for programs.
AC_PROG_CC
AC_PROG_YACC
AC_PROG_LEX

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

# We need both of these due to ffi.h being in different locations on different OSes.
AC_CHECK_HEADER(ffi/ffi.h, AC_DEFINE(FFI_HEADER, ["ffi/ffi.h"], [Path to ffi.h]))
AC_CHECK_HEADER(ffi.h, AC_DEFINE(FFI_HEADER, ["ffi.h"], [Path to ffi.h]))

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

AC_OUTPUT(
_testdeps/Makefile
Makefile
src/codegen/Makefile
src/Makefile
src/parser/Makefile
src/semantics/Makefile
src/stdlib/language/kite/Makefile
src/stdlib/language/Makefile
src/stdlib/Makefile
src/stdlib/System/collections/Makefile
src/stdlib/System/date/Makefile
src/stdlib/System/exceptions/Makefile
src/stdlib/System/Makefile
src/stdlib/System/math/Makefile
src/stdlib/System/network/Makefile
src/stdlib/System/network/wrapper/Makefile
src/stdlib/System/os/Makefile
src/stdlib/System/regex/Makefile
src/stdlib/System/vm/Makefile
stress/Makefile
tests/algorithms/Makefile
tests/Makefile
tests/semantics/class/Makefile
tests/semantics/deref/array/Makefile
tests/semantics/deref/Makefile
tests/semantics/deref/var/Makefile
tests/semantics/eval/Makefile
tests/semantics/exceptions/Makefile
tests/semantics/import/Makefile
tests/semantics/loop/Makefile
tests/semantics/Makefile
tests/semantics/method/Makefile
tests/semantics/ops/boolean/Makefile
tests/semantics/ops/is/Makefile
tests/semantics/ops/isof/Makefile
tests/semantics/ops/Makefile
tests/semantics/ops/map/Makefile
tests/semantics/ops/reduce/Makefile
tests/stdlib/boolean/Makefile
tests/stdlib/collections/Makefile
tests/stdlib/collections/binary_tree/Makefile
tests/stdlib/float/Makefile
tests/stdlib/int/Makefile
tests/stdlib/list/Makefile
tests/stdlib/Makefile
tests/stdlib/math/Makefile
tests/stdlib/null/Makefile
tests/stdlib/string/Makefile
)
