SUBDIRS = language System

AM_CXXFLAGS = $(LLVM_CPPFLAGS) $(BOOST_CPPFLAGS) -fexceptions -frtti -I../
AM_LDFLAGS = $(LLVM_LDFLAGS) $(BOOST_LDFLAGS) $(OPENSSL_LDFLAGS) $(BOOST_SYSTEM_LIB) $(BOOST_FILESYSTEM_LIB)

noinst_LTLIBRARIES = libkitestdlib.la
libkitestdlib_la_SOURCES = \
    api.cpp \
    api.h \
    System.cpp \
    System.h
libkitestdlib_la_LIBADD = System/libkitestdlib_System.la language/libkitestdlib_language.la