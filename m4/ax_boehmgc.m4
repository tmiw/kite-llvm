# SYNOPSIS
#
#   AX_BOEHMGC([llvm-libs])
#
# DESCRIPTION
#
#   Test for the existance of libgc.

#serial 12

AC_DEFUN([AX_BOEHMGC],
[
AC_ARG_WITH([gc],
   [AS_HELP_STRING([--with-gc@<:@=DIR|yes|no|check@:>@],
                   [enable Boehm garbage collection library
                   @<:@default=yes@:>@])],
   [],
   [with_gc="yes"])

AS_IF([test "x$with_gc" != "xno"],
[
 gc_save_LIBS="$LIBS"
 LIBS="$LIBS -lgc"
 if test "x$with_gc" != "xyes" -a "x$with_gc" != "xcheck"
 then
   gc_save_CFLAGS="$CFLAGS"
   gc_save_CPPFLAGS="$CPPFLAGS"
   gc_save_LDFLAGS="$LDFLAGS"
   CFLAGS="$CFLAGS -I$with_gc/include -DHAVE_GC_GC_H"
   CXXFLAGS="$CXXFLAGS -I$with_gc/include -DHAVE_GC_GC_H"
   LDFLAGS="$LDFLAGS -L$with_gc/lib"
 fi
 
 # check headers
 AC_CHECK_HEADER(gc/gc.h, ,
 [
   if test "x$with_gc" != "xcheck"
   then
     AC_MSG_ERROR([Could not find gc/gc.h. Required for Boehm GC support. Pass --with-gc=no to disable. (not recommended)])
   fi
 ], AC_INCLUDES_DEFAULT)

 # check library
 AC_CHECK_LIB([gc], [GC_malloc],
    [AC_SUBST([LIBGC], [-lgc])
     AC_DEFINE([HAVE_GC_H], [1],
               [Define if you have libgc])
    ],
    [if test "x$with_gc" != "xcheck"; then
        AC_MSG_FAILURE(
            [--with-gc was given, but test for libgc failed])
     else
        LIBS=$gc_save_LIBS
     fi
    ])
    
 AC_SUBST([HAVE_GC], [true])
], [
    AC_SUBST([HAVE_GC], [false])
])
])
