AM_CXXFLAGS = $(LLVM_CPPFLAGS) $(BOOST_CPPFLAGS) -fexceptions -frtti -I../
AM_LDFLAGS = $(LLVM_LDFLAGS) $(BOOST_LDFLAGS) $(OPENSSL_LDFLAGS) $(BOOST_SYSTEM_LIB) $(BOOST_FILESYSTEM_LIB)

noinst_LTLIBRARIES = libkiteparser.la
libkiteparser_la_SOURCES = \
    kite-parser.ypp \
    kite-lexer.lpp \
    kite-driver.hh \
    kite-driver.cpp \
    parser.cpp \
    parser.h