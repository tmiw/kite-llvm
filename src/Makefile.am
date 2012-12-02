# TODO: split into multiple Makefile.am folders.

lib_LTLIBRARIES = libkite.la
libkite_la_CPPFLAGS = $(LLVM_CPPFLAGS) $(BOOST_CPPFLAGS) -fexceptions -frtti
libkite_la_LDFLAGS = $(LLVM_LDFLAGS) $(BOOST_LDFLAGS) $(BOOST_SYSTEM_LIB) $(BOOST_FILESYSTEM_LIB) $(BOOST_REGEX_LIB)

libkite_la_SOURCES = codegen/llvm_compile_state.cpp codegen/llvm_node_codegen.cpp \
	 codegen/syntax_tree_node_printer.cpp codegen/syntax_tree_printer.cpp \
	 stdlib/System.cpp stdlib/System/integer.cpp stdlib/System/string.cpp \
	 stdlib/System/boolean.cpp stdlib/System/float.cpp stdlib/System/object.cpp \
	 stdlib/System/dynamic_object.cpp stdlib/System/list.cpp stdlib/System/method.cpp \
	 stdlib/System/collections.cpp stdlib/System/collections/array.cpp \
	 stdlib/System/collections/queue.cpp stdlib/System/collections/stack.cpp \
	 stdlib/System/collections/binary_tree.cpp stdlib/System/date.cpp stdlib/System/date/hires.cpp \
	 stdlib/System/directory.cpp stdlib/System/file.cpp stdlib/System/math.cpp stdlib/System/math/range.cpp \
	 stdlib/System/network.cpp stdlib/System/network/socket.cpp stdlib/System/network/wrapper.cpp \
	 stdlib/System/network/wrapper/TcpSocket.cpp stdlib/System/network/wrapper/UdpSocket.cpp \
	 stdlib/language/kite.cpp stdlib/language/kite/syntax_tree.cpp \
	 stdlib/System/exceptions.cpp stdlib/System/exceptions/exception.cpp \
	 stdlib/System/exceptions/NotImplemented.cpp stdlib/System/exceptions/InvalidArgument.cpp \
	 stdlib/System/exceptions/TypeMismatch.cpp stdlib/System/exceptions/DivideByZero.cpp \
	 stdlib/System/exceptions/NullReference.cpp stdlib/System/exceptions/EmptyCollection.cpp \
	 stdlib/System/exceptions/FileError.cpp stdlib/System/exceptions/SocketError.cpp \
	 stdlib/System/exceptions/DnsError.cpp stdlib/System/os.cpp stdlib/System/os/posix.cpp \
	 stdlib/System/regex.cpp stdlib/System/regex/match_result.cpp \
	 stdlib/System/vm.cpp stdlib/System/vm/compiler.cpp stdlib/System/vm/loader.cpp \
	 stdlib/System/vm/thread.cpp stdlib/System/digest.cpp stdlib/api.cpp \
	 parser/parser.cpp semantics/constants.cpp \
	 codegen/llvm_compile_state.h codegen/llvm_node_codegen.h codegen/syntax_tree_printer.h \
	 parser/grammar.h parser/parser.h semantics/constants.h semantics/syntax_tree.h \
	 stdlib/api.h stdlib/language/kite.h stdlib/language/kite/syntax_tree.h stdlib/System.h \
	 stdlib/System/boolean.h stdlib/System/collections/array.h stdlib/System/collections/binary_tree.h \
     stdlib/System/collections/queue.h stdlib/System/collections/stack.h stdlib/System/collections.h \
     stdlib/System/date/hires.h stdlib/System/date.h stdlib/System/digest.h stdlib/System/directory.h \
     stdlib/System/dynamic_object.h stdlib/System/exceptions/DivideByZero.h stdlib/System/exceptions/DnsError.h \
     stdlib/System/exceptions/EmptyCollection.h stdlib/System/exceptions/exception.h \
     stdlib/System/exceptions/FileError.h stdlib/System/exceptions/InvalidArgument.h \
     stdlib/System/exceptions/NotImplemented.h stdlib/System/exceptions/NullReference.h \
     stdlib/System/exceptions/SocketError.h stdlib/System/exceptions/TypeMismatch.h \
     stdlib/System/exceptions.h stdlib/System/file.h stdlib/System/float.h \
     stdlib/System/integer.h stdlib/System/list.h stdlib/System/math/range.h \
     stdlib/System/math.h stdlib/System/method.h stdlib/System/network/socket.h \
     stdlib/System/network/wrapper/TcpSocket.h stdlib/System/network/wrapper/UdpSocket.h \
     stdlib/System/network/wrapper.h stdlib/System/network.h \
     stdlib/System/object.h stdlib/System/os/posix.h stdlib/System/os.h \
     stdlib/System/regex/match_result.h stdlib/System/regex.h \
     stdlib/System/string.h stdlib/System/vm/compiler.h \
     stdlib/System/vm/loader.hstdlib/System/vm/thread.h stdlib/System/vm.h
     
bin_PROGRAMS = kite ikt

kite_SOURCES = apps/kite_app.cpp
kite_LDADD = libkite.la
kite_CPPFLAGS = $(LLVM_CPPFLAGS) -frtti

ikt_SOURCES = apps/ikt.cpp
ikt_LDADD = libkite.la
ikt_CPPFLAGS = $(LLVM_CPPFLAGS) -frtti