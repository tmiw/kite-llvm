AM_CXXFLAGS = $(LLVM_CPPFLAGS) $(BOOST_CPPFLAGS) -fexceptions -frtti -I../../../
AM_LDFLAGS = $(LLVM_LDFLAGS) $(BOOST_LDFLAGS) $(OPENSSL_LDFLAGS) $(BOOST_SYSTEM_LIB) $(BOOST_FILESYSTEM_LIB)

noinst_LTLIBRARIES = libkitestdlib_System_exceptions.la
libkitestdlib_System_exceptions_la_SOURCES = \
    ./DivideByZero.cpp \
    ./DivideByZero.h \
    ./DnsError.cpp \
    ./DnsError.h \
    ./EmptyCollection.cpp \
    ./EmptyCollection.h \
    ./exception.cpp \
    ./exception.h \
    ./FileError.cpp \
    ./FileError.h \
    ./InvalidArgument.cpp \
    ./InvalidArgument.h \
    ./NotImplemented.cpp \
    ./NotImplemented.h \
    ./NullReference.cpp \
    ./NullReference.h \
    ./SocketError.cpp \
    ./SocketError.h \
    ./TypeMismatch.cpp \
    ./TypeMismatch.h
    