SUBDIRS = collections date exceptions math network os regex vm

AM_CXXFLAGS = $(LLVM_CPPFLAGS) $(BOOST_CPPFLAGS) -fexceptions -frtti -I../../
AM_LDFLAGS = $(LLVM_LDFLAGS) $(BOOST_LDFLAGS) $(OPENSSL_LDFLAGS) $(BOOST_SYSTEM_LIB) $(BOOST_FILESYSTEM_LIB)

noinst_LTLIBRARIES = libkitestdlib_System.la

libkitestdlib_System_la_SOURCES = \
    ./boolean.cpp \
    ./boolean.h \
    ./date.cpp \
    ./date.h \
    ./digest.cpp \
    ./digest.h \
    ./directory.cpp \
    ./directory.h \
    ./dynamic_object.cpp \
    ./dynamic_object.h \
    ./exceptions.cpp \
    ./exceptions.h \
    ./file.cpp \
    ./file.h \
    ./float.cpp \
    ./float.h \
    ./integer.cpp \
    ./integer.h \
    ./list.cpp \
    ./list.h \
    ./math_type.cpp \
    ./math_type.h \
    ./method.cpp \
    ./method.h \
    ./network.cpp \
    ./network.h \
    ./object.cpp \
    ./object.h \
    ./os.cpp \
    ./os.h \
    ./regex.cpp \
    ./regex.h \
    ./string_type.cpp \
    ./string_type.h \
    ./vm.cpp \
    ./vm.h

libkitestdlib_System_la_LIBADD = \
    collections/libkitestdlib_System_collections.la \
    date/libkitestdlib_System_date.la \
    exceptions/libkitestdlib_System_exceptions.la \
    math/libkitestdlib_System_math.la \
    network/libkitestdlib_System_network.la \
    os/libkitestdlib_System_os.la \
    regex/libkitestdlib_System_regex.la \
    vm/libkitestdlib_System_vm.la