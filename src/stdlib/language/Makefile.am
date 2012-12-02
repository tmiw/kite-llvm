SUBDIRS = kite

AM_CXXFLAGS = $(LLVM_CPPFLAGS) $(BOOST_CPPFLAGS) -fexceptions -frtti -I../..
AM_LDFLAGS = $(LLVM_LDFLAGS) $(BOOST_LDFLAGS) $(OPENSSL_LDFLAGS) $(BOOST_SYSTEM_LIB) $(BOOST_FILESYSTEM_LIB)

noinst_LTLIBRARIES = libkitestdlib_language.la

libkitestdlib_language_la_SOURCES = \
    kite.cpp \
    kite.h
libkitestdlib_language_la_LIBADD = kite/libkitestdlib_language_kite.la