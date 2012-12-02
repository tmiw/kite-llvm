AM_CXXFLAGS = $(LLVM_CPPFLAGS) $(BOOST_CPPFLAGS) -fexceptions -frtti -I../../../../
AM_LDFLAGS = $(LLVM_LDFLAGS) $(BOOST_LDFLAGS) $(OPENSSL_LDFLAGS) $(BOOST_SYSTEM_LIB) $(BOOST_FILESYSTEM_LIB)

noinst_LTLIBRARIES = libkitestdlib_System_network_wrapper.la
libkitestdlib_System_network_wrapper_la_SOURCES = \
    TcpSocket.cpp \
    TcpSocket.h \
    UdpSocket.cpp \
    UdpSocket.h