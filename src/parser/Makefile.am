AM_CXXFLAGS = $(LLVM_CPPFLAGS) $(BOOST_CPPFLAGS) -fexceptions -frtti -I../
AM_LDFLAGS = $(LLVM_LDFLAGS) $(BOOST_LDFLAGS) $(OPENSSL_LDFLAGS) $(BOOST_SYSTEM_LIB) $(BOOST_FILESYSTEM_LIB)

noinst_LTLIBRARIES = libkiteparser.la
libkiteparser_la_SOURCES = \
    kite-parser.y \
    kite-lexer.l \
    kite-driver.hh \
    kite-driver.cpp \
    parser.cpp \
    parser.h

EXTRA_DIST = \
    ./assignment.cpp \
    ./bitwise.cpp \
    ./classes.cpp \
    ./comparison.cpp \
    ./constants_decl.cpp \
    ./constructor.cpp \
    ./decide.cpp \
    ./deref.cpp \
    ./destructor.cpp \
    ./exceptions_decl.cpp \
    ./grouping.cpp \
    ./loop.cpp \
    ./make.cpp \
    ./map_reduce.cpp \
    ./math_ops.cpp \
    ./method_decl.cpp \
    ./statement.cpp