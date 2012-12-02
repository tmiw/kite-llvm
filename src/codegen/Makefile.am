AM_CXXFLAGS = $(LLVM_CPPFLAGS) $(BOOST_CPPFLAGS) -fexceptions -frtti -I../
AM_LDFLAGS = $(LLVM_LDFLAGS) $(BOOST_LDFLAGS) $(OPENSSL_LDFLAGS) $(BOOST_SYSTEM_LIB) $(BOOST_FILESYSTEM_LIB)

noinst_LTLIBRARIES = libkitecodegen.la
libkitecodegen_la_SOURCES = \
    ./llvm_compile_state.cpp \
    ./llvm_compile_state.h \
    ./llvm_node_codegen.cpp \
    ./llvm_node_codegen.h \
    ./syntax_tree_node_printer.cpp \
    ./syntax_tree_printer.cpp \
    ./syntax_tree_printer.h