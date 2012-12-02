AM_CXXFLAGS = $(LLVM_CPPFLAGS) $(BOOST_CPPFLAGS) -fexceptions -frtti -I../../../
AM_LDFLAGS = $(LLVM_LDFLAGS) $(BOOST_LDFLAGS) $(OPENSSL_LDFLAGS) $(BOOST_SYSTEM_LIB) $(BOOST_FILESYSTEM_LIB)

noinst_LTLIBRARIES = libkitestdlib_System_regex.la
libkitestdlib_System_regex_la_SOURCES = \
    match_result.cpp \
    match_result.h