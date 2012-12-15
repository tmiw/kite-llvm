AM_CXXFLAGS = $(LLVM_CPPFLAGS) $(BOOST_CPPFLAGS) -fexceptions -frtti -I../
AM_LDFLAGS = $(LLVM_LDFLAGS) $(BOOST_LDFLAGS) $(OPENSSL_LDFLAGS) $(BOOST_SYSTEM_LIB) $(BOOST_FILESYSTEM_LIB)

noinst_LTLIBRARIES = libkiteparser.la
libkiteparser_la_SOURCES = \
    kite-parser.cpp \
    kite-lexer.lpp \
    kite-driver.hh \
    kite-driver.cpp \
    parser.cpp \
    parser.h

# automake >= 1.12 uses a different file extension, breaking everything.
kite-parser.cpp kite-parser.hpp: kite-parser.ypp
	$(YACC) -v -o kite-parser.cpp --defines=kite-parser.hpp $<

BUILT_SOURCES = kite-parser.hpp kite-parser.cpp
CLEANFILES = $(BUILT_SOURCES)
