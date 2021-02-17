SUBDIRS = codegen parser semantics stdlib

lib_LTLIBRARIES = libkite.la
AM_CXXFLAGS = $(LLVM_CPPFLAGS) $(BOOST_CPPFLAGS) -fexceptions -frtti
AM_LDFLAGS = $(LLVM_LDFLAGS) $(BOOST_LDFLAGS) $(OPENSSL_LDFLAGS) $(BOOST_SYSTEM_LIB) $(BOOST_FILESYSTEM_LIB) $(BOOST_REGEX_LIB) $(OPENSSL_LIBS)

libkite_la_LIBADD = \
    codegen/libkitecodegen.la \
    parser/libkiteparser.la \
    semantics/libkitesemantics.la \
    stdlib/libkitestdlib.la
    
bin_PROGRAMS = kite ikt
kite_SOURCES = kite_app.cpp
kite_LDADD = libkite.la
ikt_SOURCES = ikt.cpp
ikt_LDADD = libkite.la
