AM_CXXFLAGS = $(LLVM_CPPFLAGS) $(BOOST_CPPFLAGS) -fexceptions -frtti -I../../../
AM_LDFLAGS = $(LLVM_LDFLAGS) $(BOOST_LDFLAGS) $(OPENSSL_LDFLAGS) $(BOOST_SYSTEM_LIB) $(BOOST_FILESYSTEM_LIB)

SUBDIRS = wrapper

noinst_LTLIBRARIES = libkitestdlib_System_network.la
libkitestdlib_System_network_la_SOURCES = \
    socket.cpp \
    socket.h \
    wrapper.cpp \
    wrapper.h
libkitestdlib_System_network_la_LIBADD = wrapper/libkitestdlib_System_network_wrapper.la