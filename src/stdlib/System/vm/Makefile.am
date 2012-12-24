AM_CXXFLAGS = $(LLVM_CPPFLAGS) $(BOOST_CPPFLAGS) -fexceptions -frtti -I../../../ -DDATADIR='"$(pkgdatadir)"'
AM_LDFLAGS = $(LLVM_LDFLAGS) $(BOOST_LDFLAGS) $(OPENSSL_LDFLAGS) $(BOOST_SYSTEM_LIB) $(BOOST_FILESYSTEM_LIB)

noinst_LTLIBRARIES = libkitestdlib_System_vm.la
libkitestdlib_System_vm_la_SOURCES = \
    ./compiler.cpp \
    ./compiler.h \
    ./loader.cpp \
    ./loader.h \
    ./thread.cpp \
    ./thread.h