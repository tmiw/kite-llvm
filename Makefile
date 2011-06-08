CC=g++
CPPFLAGS=-g -O2
INC=-Isrc/ -I/opt/local/include
LLVM_CPPFLAGS=`llvm-config --cppflags`
LLVM_LDFLAGS=`llvm-config --ldflags`
LLVM_LIBS=`llvm-config --libs`
OBJS=src/apps/kite.o src/codegen/llvm_compile_state.o src/codegen/llvm_node_codegen.o \
	 src/codegen/syntax_tree_node_printer.o src/codegen/syntax_tree_printer.o \
	 src/parser/constants.o src/parser/assignment.o src/parser/bitwise.o \
	 src/parser/comparison.o src/parser/math.o src/parser/map_reduce.o src/parser/deref.o \
	 src/parser/grouping.o src/parser/loop.o src/parser/decide.o src/parser/method.o src/parser/classes.o \
	 src/parser/statement.o src/parser/parser.o src/stdlib/System/integer.o src/stdlib/System/string.o \
	 src/stdlib/System/boolean.o src/stdlib/System/float.o src/stdlib/System/object.o

.cpp.o: %.cpp
	$(CC) -c -o $@ $(CPPFLAGS) $(INC) $(LLVM_CPPFLAGS) $<

all: kite

depend:
	makedepend `find src -name '*.cpp'` -Isrc/ -I/usr/include/c++/4.2.1

clean:
	rm -rf `find ./ -name '*.o'` `find ./ -name 'kite'`

kite: $(OBJS)
	$(CC) $(CPPFLAGS) $(LLVM_LDFLAGS) $(LLVM_LIBS) -o kite $(OBJS)

# DO NOT DELETE

src/apps/kite.o: /usr/include/c++/4.2.1/iostream
src/apps/kite.o: /usr/include/c++/4.2.1/ostream /usr/include/c++/4.2.1/ios
src/apps/kite.o: /usr/include/c++/4.2.1/iosfwd /usr/include/c++/4.2.1/cctype
src/apps/kite.o: /usr/include/ctype.h /usr/include/runetype.h
src/apps/kite.o: /usr/include/_types.h /usr/include/sys/_types.h
src/apps/kite.o: /usr/include/sys/cdefs.h /usr/include/machine/_types.h
src/apps/kite.o: /usr/include/i386/_types.h
src/apps/kite.o: /usr/include/c++/4.2.1/bits/stringfwd.h
src/apps/kite.o: /usr/include/c++/4.2.1/bits/postypes.h
src/apps/kite.o: /usr/include/c++/4.2.1/cwchar /usr/include/c++/4.2.1/cstddef
src/apps/kite.o: /usr/include/stddef.h /usr/include/c++/4.2.1/ctime
src/apps/kite.o: /usr/include/time.h /usr/include/_structs.h
src/apps/kite.o: /usr/include/sys/_structs.h
src/apps/kite.o: /usr/include/c++/4.2.1/bits/functexcept.h
src/apps/kite.o: /usr/include/c++/4.2.1/exception_defines.h
src/apps/kite.o: /usr/include/c++/4.2.1/exception
src/apps/kite.o: /usr/include/c++/4.2.1/bits/char_traits.h
src/apps/kite.o: /usr/include/c++/4.2.1/cstring /usr/include/string.h
src/apps/kite.o: /usr/include/secure/_string.h /usr/include/secure/_common.h
src/apps/kite.o: /usr/include/c++/4.2.1/bits/stl_algobase.h
src/apps/kite.o: /usr/include/c++/4.2.1/climits /usr/include/limits.h
src/apps/kite.o: /usr/include/machine/limits.h /usr/include/i386/limits.h
src/apps/kite.o: /usr/include/i386/_limits.h /usr/include/sys/syslimits.h
src/apps/kite.o: /usr/include/c++/4.2.1/cstdlib
src/apps/kite.o: /usr/include/c++/4.2.1/bits/stl_pair.h
src/apps/kite.o: /usr/include/c++/4.2.1/bits/cpp_type_traits.h
src/apps/kite.o: /usr/include/c++/4.2.1/ext/type_traits.h
src/apps/kite.o: /usr/include/c++/4.2.1/utility
src/apps/kite.o: /usr/include/c++/4.2.1/bits/stl_relops.h
src/apps/kite.o: /usr/include/c++/4.2.1/bits/stl_iterator_base_types.h
src/apps/kite.o: /usr/include/c++/4.2.1/bits/stl_iterator_base_funcs.h
src/apps/kite.o: /usr/include/c++/4.2.1/bits/concept_check.h
src/apps/kite.o: /usr/include/c++/4.2.1/bits/stl_iterator.h
src/apps/kite.o: /usr/include/c++/4.2.1/debug/debug.h
src/apps/kite.o: /usr/include/c++/4.2.1/cstdio /usr/include/stdio.h
src/apps/kite.o: /usr/include/secure/_stdio.h
src/apps/kite.o: /usr/include/c++/4.2.1/bits/localefwd.h
src/apps/kite.o: /usr/include/c++/4.2.1/bits/ios_base.h
src/apps/kite.o: /usr/include/c++/4.2.1/ext/atomicity.h
src/apps/kite.o: /usr/include/c++/4.2.1/bits/locale_classes.h
src/apps/kite.o: /usr/include/c++/4.2.1/string /usr/include/c++/4.2.1/memory
src/apps/kite.o: /usr/include/c++/4.2.1/bits/allocator.h
src/apps/kite.o: /usr/include/c++/4.2.1/bits/stl_construct.h
src/apps/kite.o: /usr/include/c++/4.2.1/new
src/apps/kite.o: /usr/include/c++/4.2.1/bits/stl_uninitialized.h
src/apps/kite.o: /usr/include/c++/4.2.1/bits/stl_raw_storage_iter.h
src/apps/kite.o: /usr/include/c++/4.2.1/limits
src/apps/kite.o: /usr/include/c++/4.2.1/bits/ostream_insert.h
src/apps/kite.o: /usr/include/c++/4.2.1/bits/stl_function.h
src/apps/kite.o: /usr/include/c++/4.2.1/bits/basic_string.h
src/apps/kite.o: /usr/include/c++/4.2.1/algorithm
src/apps/kite.o: /usr/include/c++/4.2.1/bits/stl_algo.h
src/apps/kite.o: /usr/include/c++/4.2.1/bits/stl_heap.h
src/apps/kite.o: /usr/include/c++/4.2.1/bits/stl_tempbuf.h
src/apps/kite.o: /usr/include/c++/4.2.1/bits/basic_string.tcc
src/apps/kite.o: /usr/include/c++/4.2.1/streambuf
src/apps/kite.o: /usr/include/c++/4.2.1/bits/streambuf.tcc
src/apps/kite.o: /usr/include/c++/4.2.1/bits/basic_ios.h
src/apps/kite.o: /usr/include/c++/4.2.1/bits/streambuf_iterator.h
src/apps/kite.o: /usr/include/c++/4.2.1/bits/locale_facets.h
src/apps/kite.o: /usr/include/c++/4.2.1/cwctype
src/apps/kite.o: /usr/include/c++/4.2.1/bits/codecvt.h
src/apps/kite.o: /usr/include/c++/4.2.1/bits/basic_ios.tcc
src/apps/kite.o: /usr/include/c++/4.2.1/bits/ostream.tcc
src/apps/kite.o: /usr/include/c++/4.2.1/locale
src/apps/kite.o: /usr/include/c++/4.2.1/bits/locale_facets.tcc
src/apps/kite.o: /usr/include/c++/4.2.1/typeinfo
src/apps/kite.o: /usr/include/c++/4.2.1/istream
src/apps/kite.o: /usr/include/c++/4.2.1/bits/istream.tcc
src/apps/kite.o: /usr/include/c++/4.2.1/fstream
src/apps/kite.o: /usr/include/c++/4.2.1/bits/fstream.tcc
src/apps/kite.o: /usr/include/unistd.h /usr/include/sys/unistd.h
src/apps/kite.o: /usr/include/sys/select.h /usr/include/sys/appleapiopts.h
src/apps/kite.o: /usr/include/sys/_select.h src/parser/parser.h
src/apps/kite.o: src/semantics/syntax_tree.h /usr/include/c++/4.2.1/deque
src/apps/kite.o: /usr/include/c++/4.2.1/bits/stl_deque.h
src/apps/kite.o: /usr/include/c++/4.2.1/bits/deque.tcc
src/apps/kite.o: src/semantics/constants.h src/codegen/syntax_tree_printer.h
src/apps/kite.o: src/codegen/llvm_node_codegen.h src/stdlib/System/integer.h
src/apps/kite.o: src/stdlib/System/object.h /usr/include/c++/4.2.1/map
src/apps/kite.o: /usr/include/c++/4.2.1/bits/stl_tree.h
src/apps/kite.o: /usr/include/c++/4.2.1/bits/stl_map.h
src/apps/kite.o: /usr/include/c++/4.2.1/bits/stl_multimap.h
src/apps/kite.o: src/semantics/constants.h src/stdlib/System/string.h
src/apps/kite.o: src/stdlib/System/boolean.h src/stdlib/System/float.h
src/apps/kite.o: src/codegen/llvm_compile_state.h
src/apps/kite.o: /usr/include/c++/4.2.1/vector
src/apps/kite.o: /usr/include/c++/4.2.1/bits/stl_vector.h
src/apps/kite.o: /usr/include/c++/4.2.1/bits/stl_bvector.h
src/apps/kite.o: /usr/include/c++/4.2.1/bits/vector.tcc
src/codegen/llvm_compile_state.o: src/codegen/llvm_compile_state.h
src/codegen/llvm_compile_state.o: /usr/include/c++/4.2.1/map
src/codegen/llvm_compile_state.o: /usr/include/c++/4.2.1/bits/stl_tree.h
src/codegen/llvm_compile_state.o: /usr/include/c++/4.2.1/bits/stl_algobase.h
src/codegen/llvm_compile_state.o: /usr/include/c++/4.2.1/cstring
src/codegen/llvm_compile_state.o: /usr/include/c++/4.2.1/cstddef
src/codegen/llvm_compile_state.o: /usr/include/stddef.h /usr/include/_types.h
src/codegen/llvm_compile_state.o: /usr/include/sys/_types.h
src/codegen/llvm_compile_state.o: /usr/include/sys/cdefs.h
src/codegen/llvm_compile_state.o: /usr/include/machine/_types.h
src/codegen/llvm_compile_state.o: /usr/include/i386/_types.h
src/codegen/llvm_compile_state.o: /usr/include/string.h
src/codegen/llvm_compile_state.o: /usr/include/secure/_string.h
src/codegen/llvm_compile_state.o: /usr/include/secure/_common.h
src/codegen/llvm_compile_state.o: /usr/include/c++/4.2.1/climits
src/codegen/llvm_compile_state.o: /usr/include/limits.h
src/codegen/llvm_compile_state.o: /usr/include/machine/limits.h
src/codegen/llvm_compile_state.o: /usr/include/i386/limits.h
src/codegen/llvm_compile_state.o: /usr/include/i386/_limits.h
src/codegen/llvm_compile_state.o: /usr/include/sys/syslimits.h
src/codegen/llvm_compile_state.o: /usr/include/c++/4.2.1/cstdlib
src/codegen/llvm_compile_state.o: /usr/include/c++/4.2.1/iosfwd
src/codegen/llvm_compile_state.o: /usr/include/c++/4.2.1/cctype
src/codegen/llvm_compile_state.o: /usr/include/ctype.h
src/codegen/llvm_compile_state.o: /usr/include/runetype.h
src/codegen/llvm_compile_state.o: /usr/include/c++/4.2.1/bits/stringfwd.h
src/codegen/llvm_compile_state.o: /usr/include/c++/4.2.1/bits/postypes.h
src/codegen/llvm_compile_state.o: /usr/include/c++/4.2.1/cwchar
src/codegen/llvm_compile_state.o: /usr/include/c++/4.2.1/ctime
src/codegen/llvm_compile_state.o: /usr/include/time.h /usr/include/_structs.h
src/codegen/llvm_compile_state.o: /usr/include/sys/_structs.h
src/codegen/llvm_compile_state.o: /usr/include/c++/4.2.1/bits/functexcept.h
src/codegen/llvm_compile_state.o: /usr/include/c++/4.2.1/exception_defines.h
src/codegen/llvm_compile_state.o: /usr/include/c++/4.2.1/bits/stl_pair.h
src/codegen/llvm_compile_state.o: /usr/include/c++/4.2.1/bits/cpp_type_traits.h
src/codegen/llvm_compile_state.o: /usr/include/c++/4.2.1/ext/type_traits.h
src/codegen/llvm_compile_state.o: /usr/include/c++/4.2.1/utility
src/codegen/llvm_compile_state.o: /usr/include/c++/4.2.1/bits/stl_relops.h
src/codegen/llvm_compile_state.o: /usr/include/c++/4.2.1/bits/stl_iterator_base_types.h
src/codegen/llvm_compile_state.o: /usr/include/c++/4.2.1/bits/stl_iterator_base_funcs.h
src/codegen/llvm_compile_state.o: /usr/include/c++/4.2.1/bits/concept_check.h
src/codegen/llvm_compile_state.o: /usr/include/c++/4.2.1/bits/stl_iterator.h
src/codegen/llvm_compile_state.o: /usr/include/c++/4.2.1/debug/debug.h
src/codegen/llvm_compile_state.o: /usr/include/c++/4.2.1/bits/allocator.h
src/codegen/llvm_compile_state.o: /usr/include/c++/4.2.1/bits/stl_construct.h
src/codegen/llvm_compile_state.o: /usr/include/c++/4.2.1/new
src/codegen/llvm_compile_state.o: /usr/include/c++/4.2.1/exception
src/codegen/llvm_compile_state.o: /usr/include/c++/4.2.1/bits/stl_function.h
src/codegen/llvm_compile_state.o: /usr/include/c++/4.2.1/bits/stl_map.h
src/codegen/llvm_compile_state.o: /usr/include/c++/4.2.1/bits/stl_multimap.h
src/codegen/llvm_compile_state.o: /usr/include/c++/4.2.1/vector
src/codegen/llvm_compile_state.o: /usr/include/c++/4.2.1/bits/stl_uninitialized.h
src/codegen/llvm_compile_state.o: /usr/include/c++/4.2.1/bits/stl_vector.h
src/codegen/llvm_compile_state.o: /usr/include/c++/4.2.1/bits/stl_bvector.h
src/codegen/llvm_compile_state.o: /usr/include/c++/4.2.1/bits/vector.tcc
src/codegen/llvm_node_codegen.o: src/codegen/llvm_node_codegen.h
src/codegen/llvm_node_codegen.o: src/semantics/syntax_tree.h
src/codegen/llvm_node_codegen.o: /usr/include/c++/4.2.1/deque
src/codegen/llvm_node_codegen.o: /usr/include/c++/4.2.1/bits/functexcept.h
src/codegen/llvm_node_codegen.o: /usr/include/c++/4.2.1/exception_defines.h
src/codegen/llvm_node_codegen.o: /usr/include/c++/4.2.1/bits/stl_algobase.h
src/codegen/llvm_node_codegen.o: /usr/include/c++/4.2.1/cstring
src/codegen/llvm_node_codegen.o: /usr/include/c++/4.2.1/cstddef
src/codegen/llvm_node_codegen.o: /usr/include/stddef.h /usr/include/_types.h
src/codegen/llvm_node_codegen.o: /usr/include/sys/_types.h
src/codegen/llvm_node_codegen.o: /usr/include/sys/cdefs.h
src/codegen/llvm_node_codegen.o: /usr/include/machine/_types.h
src/codegen/llvm_node_codegen.o: /usr/include/i386/_types.h
src/codegen/llvm_node_codegen.o: /usr/include/string.h
src/codegen/llvm_node_codegen.o: /usr/include/secure/_string.h
src/codegen/llvm_node_codegen.o: /usr/include/secure/_common.h
src/codegen/llvm_node_codegen.o: /usr/include/c++/4.2.1/climits
src/codegen/llvm_node_codegen.o: /usr/include/limits.h
src/codegen/llvm_node_codegen.o: /usr/include/machine/limits.h
src/codegen/llvm_node_codegen.o: /usr/include/i386/limits.h
src/codegen/llvm_node_codegen.o: /usr/include/i386/_limits.h
src/codegen/llvm_node_codegen.o: /usr/include/sys/syslimits.h
src/codegen/llvm_node_codegen.o: /usr/include/c++/4.2.1/cstdlib
src/codegen/llvm_node_codegen.o: /usr/include/c++/4.2.1/iosfwd
src/codegen/llvm_node_codegen.o: /usr/include/c++/4.2.1/cctype
src/codegen/llvm_node_codegen.o: /usr/include/ctype.h /usr/include/runetype.h
src/codegen/llvm_node_codegen.o: /usr/include/c++/4.2.1/bits/stringfwd.h
src/codegen/llvm_node_codegen.o: /usr/include/c++/4.2.1/bits/postypes.h
src/codegen/llvm_node_codegen.o: /usr/include/c++/4.2.1/cwchar
src/codegen/llvm_node_codegen.o: /usr/include/c++/4.2.1/ctime
src/codegen/llvm_node_codegen.o: /usr/include/time.h /usr/include/_structs.h
src/codegen/llvm_node_codegen.o: /usr/include/sys/_structs.h
src/codegen/llvm_node_codegen.o: /usr/include/c++/4.2.1/bits/stl_pair.h
src/codegen/llvm_node_codegen.o: /usr/include/c++/4.2.1/bits/cpp_type_traits.h
src/codegen/llvm_node_codegen.o: /usr/include/c++/4.2.1/ext/type_traits.h
src/codegen/llvm_node_codegen.o: /usr/include/c++/4.2.1/utility
src/codegen/llvm_node_codegen.o: /usr/include/c++/4.2.1/bits/stl_relops.h
src/codegen/llvm_node_codegen.o: /usr/include/c++/4.2.1/bits/stl_iterator_base_types.h
src/codegen/llvm_node_codegen.o: /usr/include/c++/4.2.1/bits/stl_iterator_base_funcs.h
src/codegen/llvm_node_codegen.o: /usr/include/c++/4.2.1/bits/concept_check.h
src/codegen/llvm_node_codegen.o: /usr/include/c++/4.2.1/bits/stl_iterator.h
src/codegen/llvm_node_codegen.o: /usr/include/c++/4.2.1/debug/debug.h
src/codegen/llvm_node_codegen.o: /usr/include/c++/4.2.1/bits/allocator.h
src/codegen/llvm_node_codegen.o: /usr/include/c++/4.2.1/bits/stl_construct.h
src/codegen/llvm_node_codegen.o: /usr/include/c++/4.2.1/new
src/codegen/llvm_node_codegen.o: /usr/include/c++/4.2.1/exception
src/codegen/llvm_node_codegen.o: /usr/include/c++/4.2.1/bits/stl_uninitialized.h
src/codegen/llvm_node_codegen.o: /usr/include/c++/4.2.1/bits/stl_deque.h
src/codegen/llvm_node_codegen.o: /usr/include/c++/4.2.1/bits/deque.tcc
src/codegen/llvm_node_codegen.o: /usr/include/c++/4.2.1/string
src/codegen/llvm_node_codegen.o: /usr/include/c++/4.2.1/bits/char_traits.h
src/codegen/llvm_node_codegen.o: /usr/include/c++/4.2.1/memory
src/codegen/llvm_node_codegen.o: /usr/include/c++/4.2.1/bits/stl_raw_storage_iter.h
src/codegen/llvm_node_codegen.o: /usr/include/c++/4.2.1/limits
src/codegen/llvm_node_codegen.o: /usr/include/c++/4.2.1/bits/ostream_insert.h
src/codegen/llvm_node_codegen.o: /usr/include/c++/4.2.1/bits/stl_function.h
src/codegen/llvm_node_codegen.o: /usr/include/c++/4.2.1/bits/basic_string.h
src/codegen/llvm_node_codegen.o: /usr/include/c++/4.2.1/ext/atomicity.h
src/codegen/llvm_node_codegen.o: /usr/include/c++/4.2.1/algorithm
src/codegen/llvm_node_codegen.o: /usr/include/c++/4.2.1/bits/stl_algo.h
src/codegen/llvm_node_codegen.o: /usr/include/c++/4.2.1/bits/stl_heap.h
src/codegen/llvm_node_codegen.o: /usr/include/c++/4.2.1/bits/stl_tempbuf.h
src/codegen/llvm_node_codegen.o: /usr/include/c++/4.2.1/bits/basic_string.tcc
src/codegen/llvm_node_codegen.o: src/semantics/constants.h
src/codegen/llvm_node_codegen.o: src/stdlib/System/integer.h
src/codegen/llvm_node_codegen.o: src/stdlib/System/object.h
src/codegen/llvm_node_codegen.o: /usr/include/c++/4.2.1/map
src/codegen/llvm_node_codegen.o: /usr/include/c++/4.2.1/bits/stl_tree.h
src/codegen/llvm_node_codegen.o: /usr/include/c++/4.2.1/bits/stl_map.h
src/codegen/llvm_node_codegen.o: /usr/include/c++/4.2.1/bits/stl_multimap.h
src/codegen/llvm_node_codegen.o: src/semantics/constants.h
src/codegen/llvm_node_codegen.o: src/stdlib/System/string.h
src/codegen/llvm_node_codegen.o: src/stdlib/System/boolean.h
src/codegen/llvm_node_codegen.o: src/stdlib/System/float.h
src/codegen/llvm_node_codegen.o: src/codegen/llvm_compile_state.h
src/codegen/llvm_node_codegen.o: /usr/include/c++/4.2.1/vector
src/codegen/llvm_node_codegen.o: /usr/include/c++/4.2.1/bits/stl_vector.h
src/codegen/llvm_node_codegen.o: /usr/include/c++/4.2.1/bits/stl_bvector.h
src/codegen/llvm_node_codegen.o: /usr/include/c++/4.2.1/bits/vector.tcc
src/codegen/syntax_tree_node_printer.o: /usr/include/c++/4.2.1/iostream
src/codegen/syntax_tree_node_printer.o: /usr/include/c++/4.2.1/ostream
src/codegen/syntax_tree_node_printer.o: /usr/include/c++/4.2.1/ios
src/codegen/syntax_tree_node_printer.o: /usr/include/c++/4.2.1/iosfwd
src/codegen/syntax_tree_node_printer.o: /usr/include/c++/4.2.1/cctype
src/codegen/syntax_tree_node_printer.o: /usr/include/ctype.h
src/codegen/syntax_tree_node_printer.o: /usr/include/runetype.h
src/codegen/syntax_tree_node_printer.o: /usr/include/_types.h
src/codegen/syntax_tree_node_printer.o: /usr/include/sys/_types.h
src/codegen/syntax_tree_node_printer.o: /usr/include/sys/cdefs.h
src/codegen/syntax_tree_node_printer.o: /usr/include/machine/_types.h
src/codegen/syntax_tree_node_printer.o: /usr/include/i386/_types.h
src/codegen/syntax_tree_node_printer.o: /usr/include/c++/4.2.1/bits/stringfwd.h
src/codegen/syntax_tree_node_printer.o: /usr/include/c++/4.2.1/bits/postypes.h
src/codegen/syntax_tree_node_printer.o: /usr/include/c++/4.2.1/cwchar
src/codegen/syntax_tree_node_printer.o: /usr/include/c++/4.2.1/cstddef
src/codegen/syntax_tree_node_printer.o: /usr/include/stddef.h
src/codegen/syntax_tree_node_printer.o: /usr/include/c++/4.2.1/ctime
src/codegen/syntax_tree_node_printer.o: /usr/include/time.h
src/codegen/syntax_tree_node_printer.o: /usr/include/_structs.h
src/codegen/syntax_tree_node_printer.o: /usr/include/sys/_structs.h
src/codegen/syntax_tree_node_printer.o: /usr/include/c++/4.2.1/bits/functexcept.h
src/codegen/syntax_tree_node_printer.o: /usr/include/c++/4.2.1/exception_defines.h
src/codegen/syntax_tree_node_printer.o: /usr/include/c++/4.2.1/exception
src/codegen/syntax_tree_node_printer.o: /usr/include/c++/4.2.1/bits/char_traits.h
src/codegen/syntax_tree_node_printer.o: /usr/include/c++/4.2.1/cstring
src/codegen/syntax_tree_node_printer.o: /usr/include/string.h
src/codegen/syntax_tree_node_printer.o: /usr/include/secure/_string.h
src/codegen/syntax_tree_node_printer.o: /usr/include/secure/_common.h
src/codegen/syntax_tree_node_printer.o: /usr/include/c++/4.2.1/bits/stl_algobase.h
src/codegen/syntax_tree_node_printer.o: /usr/include/c++/4.2.1/climits
src/codegen/syntax_tree_node_printer.o: /usr/include/limits.h
src/codegen/syntax_tree_node_printer.o: /usr/include/machine/limits.h
src/codegen/syntax_tree_node_printer.o: /usr/include/i386/limits.h
src/codegen/syntax_tree_node_printer.o: /usr/include/i386/_limits.h
src/codegen/syntax_tree_node_printer.o: /usr/include/sys/syslimits.h
src/codegen/syntax_tree_node_printer.o: /usr/include/c++/4.2.1/cstdlib
src/codegen/syntax_tree_node_printer.o: /usr/include/c++/4.2.1/bits/stl_pair.h
src/codegen/syntax_tree_node_printer.o: /usr/include/c++/4.2.1/bits/cpp_type_traits.h
src/codegen/syntax_tree_node_printer.o: /usr/include/c++/4.2.1/ext/type_traits.h
src/codegen/syntax_tree_node_printer.o: /usr/include/c++/4.2.1/utility
src/codegen/syntax_tree_node_printer.o: /usr/include/c++/4.2.1/bits/stl_relops.h
src/codegen/syntax_tree_node_printer.o: /usr/include/c++/4.2.1/bits/stl_iterator_base_types.h
src/codegen/syntax_tree_node_printer.o: /usr/include/c++/4.2.1/bits/stl_iterator_base_funcs.h
src/codegen/syntax_tree_node_printer.o: /usr/include/c++/4.2.1/bits/concept_check.h
src/codegen/syntax_tree_node_printer.o: /usr/include/c++/4.2.1/bits/stl_iterator.h
src/codegen/syntax_tree_node_printer.o: /usr/include/c++/4.2.1/debug/debug.h
src/codegen/syntax_tree_node_printer.o: /usr/include/c++/4.2.1/cstdio
src/codegen/syntax_tree_node_printer.o: /usr/include/stdio.h
src/codegen/syntax_tree_node_printer.o: /usr/include/secure/_stdio.h
src/codegen/syntax_tree_node_printer.o: /usr/include/c++/4.2.1/bits/localefwd.h
src/codegen/syntax_tree_node_printer.o: /usr/include/c++/4.2.1/bits/ios_base.h
src/codegen/syntax_tree_node_printer.o: /usr/include/c++/4.2.1/ext/atomicity.h
src/codegen/syntax_tree_node_printer.o: /usr/include/c++/4.2.1/bits/locale_classes.h
src/codegen/syntax_tree_node_printer.o: /usr/include/c++/4.2.1/string
src/codegen/syntax_tree_node_printer.o: /usr/include/c++/4.2.1/memory
src/codegen/syntax_tree_node_printer.o: /usr/include/c++/4.2.1/bits/allocator.h
src/codegen/syntax_tree_node_printer.o: /usr/include/c++/4.2.1/bits/stl_construct.h
src/codegen/syntax_tree_node_printer.o: /usr/include/c++/4.2.1/new
src/codegen/syntax_tree_node_printer.o: /usr/include/c++/4.2.1/bits/stl_uninitialized.h
src/codegen/syntax_tree_node_printer.o: /usr/include/c++/4.2.1/bits/stl_raw_storage_iter.h
src/codegen/syntax_tree_node_printer.o: /usr/include/c++/4.2.1/limits
src/codegen/syntax_tree_node_printer.o: /usr/include/c++/4.2.1/bits/ostream_insert.h
src/codegen/syntax_tree_node_printer.o: /usr/include/c++/4.2.1/bits/stl_function.h
src/codegen/syntax_tree_node_printer.o: /usr/include/c++/4.2.1/bits/basic_string.h
src/codegen/syntax_tree_node_printer.o: /usr/include/c++/4.2.1/algorithm
src/codegen/syntax_tree_node_printer.o: /usr/include/c++/4.2.1/bits/stl_algo.h
src/codegen/syntax_tree_node_printer.o: /usr/include/c++/4.2.1/bits/stl_heap.h
src/codegen/syntax_tree_node_printer.o: /usr/include/c++/4.2.1/bits/stl_tempbuf.h
src/codegen/syntax_tree_node_printer.o: /usr/include/c++/4.2.1/bits/basic_string.tcc
src/codegen/syntax_tree_node_printer.o: /usr/include/c++/4.2.1/streambuf
src/codegen/syntax_tree_node_printer.o: /usr/include/c++/4.2.1/bits/streambuf.tcc
src/codegen/syntax_tree_node_printer.o: /usr/include/c++/4.2.1/bits/basic_ios.h
src/codegen/syntax_tree_node_printer.o: /usr/include/c++/4.2.1/bits/streambuf_iterator.h
src/codegen/syntax_tree_node_printer.o: /usr/include/c++/4.2.1/bits/locale_facets.h
src/codegen/syntax_tree_node_printer.o: /usr/include/c++/4.2.1/cwctype
src/codegen/syntax_tree_node_printer.o: /usr/include/c++/4.2.1/bits/codecvt.h
src/codegen/syntax_tree_node_printer.o: /usr/include/c++/4.2.1/bits/basic_ios.tcc
src/codegen/syntax_tree_node_printer.o: /usr/include/c++/4.2.1/bits/ostream.tcc
src/codegen/syntax_tree_node_printer.o: /usr/include/c++/4.2.1/locale
src/codegen/syntax_tree_node_printer.o: /usr/include/c++/4.2.1/bits/locale_facets.tcc
src/codegen/syntax_tree_node_printer.o: /usr/include/c++/4.2.1/typeinfo
src/codegen/syntax_tree_node_printer.o: /usr/include/c++/4.2.1/istream
src/codegen/syntax_tree_node_printer.o: /usr/include/c++/4.2.1/bits/istream.tcc
src/codegen/syntax_tree_node_printer.o: src/codegen/syntax_tree_printer.h
src/codegen/syntax_tree_node_printer.o: src/semantics/syntax_tree.h
src/codegen/syntax_tree_node_printer.o: /usr/include/c++/4.2.1/deque
src/codegen/syntax_tree_node_printer.o: /usr/include/c++/4.2.1/bits/stl_deque.h
src/codegen/syntax_tree_node_printer.o: /usr/include/c++/4.2.1/bits/deque.tcc
src/codegen/syntax_tree_node_printer.o: src/semantics/constants.h
src/codegen/syntax_tree_printer.o: /usr/include/c++/4.2.1/iostream
src/codegen/syntax_tree_printer.o: /usr/include/c++/4.2.1/ostream
src/codegen/syntax_tree_printer.o: /usr/include/c++/4.2.1/ios
src/codegen/syntax_tree_printer.o: /usr/include/c++/4.2.1/iosfwd
src/codegen/syntax_tree_printer.o: /usr/include/c++/4.2.1/cctype
src/codegen/syntax_tree_printer.o: /usr/include/ctype.h
src/codegen/syntax_tree_printer.o: /usr/include/runetype.h
src/codegen/syntax_tree_printer.o: /usr/include/_types.h
src/codegen/syntax_tree_printer.o: /usr/include/sys/_types.h
src/codegen/syntax_tree_printer.o: /usr/include/sys/cdefs.h
src/codegen/syntax_tree_printer.o: /usr/include/machine/_types.h
src/codegen/syntax_tree_printer.o: /usr/include/i386/_types.h
src/codegen/syntax_tree_printer.o: /usr/include/c++/4.2.1/bits/stringfwd.h
src/codegen/syntax_tree_printer.o: /usr/include/c++/4.2.1/bits/postypes.h
src/codegen/syntax_tree_printer.o: /usr/include/c++/4.2.1/cwchar
src/codegen/syntax_tree_printer.o: /usr/include/c++/4.2.1/cstddef
src/codegen/syntax_tree_printer.o: /usr/include/stddef.h
src/codegen/syntax_tree_printer.o: /usr/include/c++/4.2.1/ctime
src/codegen/syntax_tree_printer.o: /usr/include/time.h
src/codegen/syntax_tree_printer.o: /usr/include/_structs.h
src/codegen/syntax_tree_printer.o: /usr/include/sys/_structs.h
src/codegen/syntax_tree_printer.o: /usr/include/c++/4.2.1/bits/functexcept.h
src/codegen/syntax_tree_printer.o: /usr/include/c++/4.2.1/exception_defines.h
src/codegen/syntax_tree_printer.o: /usr/include/c++/4.2.1/exception
src/codegen/syntax_tree_printer.o: /usr/include/c++/4.2.1/bits/char_traits.h
src/codegen/syntax_tree_printer.o: /usr/include/c++/4.2.1/cstring
src/codegen/syntax_tree_printer.o: /usr/include/string.h
src/codegen/syntax_tree_printer.o: /usr/include/secure/_string.h
src/codegen/syntax_tree_printer.o: /usr/include/secure/_common.h
src/codegen/syntax_tree_printer.o: /usr/include/c++/4.2.1/bits/stl_algobase.h
src/codegen/syntax_tree_printer.o: /usr/include/c++/4.2.1/climits
src/codegen/syntax_tree_printer.o: /usr/include/limits.h
src/codegen/syntax_tree_printer.o: /usr/include/machine/limits.h
src/codegen/syntax_tree_printer.o: /usr/include/i386/limits.h
src/codegen/syntax_tree_printer.o: /usr/include/i386/_limits.h
src/codegen/syntax_tree_printer.o: /usr/include/sys/syslimits.h
src/codegen/syntax_tree_printer.o: /usr/include/c++/4.2.1/cstdlib
src/codegen/syntax_tree_printer.o: /usr/include/c++/4.2.1/bits/stl_pair.h
src/codegen/syntax_tree_printer.o: /usr/include/c++/4.2.1/bits/cpp_type_traits.h
src/codegen/syntax_tree_printer.o: /usr/include/c++/4.2.1/ext/type_traits.h
src/codegen/syntax_tree_printer.o: /usr/include/c++/4.2.1/utility
src/codegen/syntax_tree_printer.o: /usr/include/c++/4.2.1/bits/stl_relops.h
src/codegen/syntax_tree_printer.o: /usr/include/c++/4.2.1/bits/stl_iterator_base_types.h
src/codegen/syntax_tree_printer.o: /usr/include/c++/4.2.1/bits/stl_iterator_base_funcs.h
src/codegen/syntax_tree_printer.o: /usr/include/c++/4.2.1/bits/concept_check.h
src/codegen/syntax_tree_printer.o: /usr/include/c++/4.2.1/bits/stl_iterator.h
src/codegen/syntax_tree_printer.o: /usr/include/c++/4.2.1/debug/debug.h
src/codegen/syntax_tree_printer.o: /usr/include/c++/4.2.1/cstdio
src/codegen/syntax_tree_printer.o: /usr/include/stdio.h
src/codegen/syntax_tree_printer.o: /usr/include/secure/_stdio.h
src/codegen/syntax_tree_printer.o: /usr/include/c++/4.2.1/bits/localefwd.h
src/codegen/syntax_tree_printer.o: /usr/include/c++/4.2.1/bits/ios_base.h
src/codegen/syntax_tree_printer.o: /usr/include/c++/4.2.1/ext/atomicity.h
src/codegen/syntax_tree_printer.o: /usr/include/c++/4.2.1/bits/locale_classes.h
src/codegen/syntax_tree_printer.o: /usr/include/c++/4.2.1/string
src/codegen/syntax_tree_printer.o: /usr/include/c++/4.2.1/memory
src/codegen/syntax_tree_printer.o: /usr/include/c++/4.2.1/bits/allocator.h
src/codegen/syntax_tree_printer.o: /usr/include/c++/4.2.1/bits/stl_construct.h
src/codegen/syntax_tree_printer.o: /usr/include/c++/4.2.1/new
src/codegen/syntax_tree_printer.o: /usr/include/c++/4.2.1/bits/stl_uninitialized.h
src/codegen/syntax_tree_printer.o: /usr/include/c++/4.2.1/bits/stl_raw_storage_iter.h
src/codegen/syntax_tree_printer.o: /usr/include/c++/4.2.1/limits
src/codegen/syntax_tree_printer.o: /usr/include/c++/4.2.1/bits/ostream_insert.h
src/codegen/syntax_tree_printer.o: /usr/include/c++/4.2.1/bits/stl_function.h
src/codegen/syntax_tree_printer.o: /usr/include/c++/4.2.1/bits/basic_string.h
src/codegen/syntax_tree_printer.o: /usr/include/c++/4.2.1/algorithm
src/codegen/syntax_tree_printer.o: /usr/include/c++/4.2.1/bits/stl_algo.h
src/codegen/syntax_tree_printer.o: /usr/include/c++/4.2.1/bits/stl_heap.h
src/codegen/syntax_tree_printer.o: /usr/include/c++/4.2.1/bits/stl_tempbuf.h
src/codegen/syntax_tree_printer.o: /usr/include/c++/4.2.1/bits/basic_string.tcc
src/codegen/syntax_tree_printer.o: /usr/include/c++/4.2.1/streambuf
src/codegen/syntax_tree_printer.o: /usr/include/c++/4.2.1/bits/streambuf.tcc
src/codegen/syntax_tree_printer.o: /usr/include/c++/4.2.1/bits/basic_ios.h
src/codegen/syntax_tree_printer.o: /usr/include/c++/4.2.1/bits/streambuf_iterator.h
src/codegen/syntax_tree_printer.o: /usr/include/c++/4.2.1/bits/locale_facets.h
src/codegen/syntax_tree_printer.o: /usr/include/c++/4.2.1/cwctype
src/codegen/syntax_tree_printer.o: /usr/include/c++/4.2.1/bits/codecvt.h
src/codegen/syntax_tree_printer.o: /usr/include/c++/4.2.1/bits/basic_ios.tcc
src/codegen/syntax_tree_printer.o: /usr/include/c++/4.2.1/bits/ostream.tcc
src/codegen/syntax_tree_printer.o: /usr/include/c++/4.2.1/locale
src/codegen/syntax_tree_printer.o: /usr/include/c++/4.2.1/bits/locale_facets.tcc
src/codegen/syntax_tree_printer.o: /usr/include/c++/4.2.1/typeinfo
src/codegen/syntax_tree_printer.o: /usr/include/c++/4.2.1/istream
src/codegen/syntax_tree_printer.o: /usr/include/c++/4.2.1/bits/istream.tcc
src/codegen/syntax_tree_printer.o: src/codegen/syntax_tree_printer.h
src/codegen/syntax_tree_printer.o: src/semantics/syntax_tree.h
src/codegen/syntax_tree_printer.o: /usr/include/c++/4.2.1/deque
src/codegen/syntax_tree_printer.o: /usr/include/c++/4.2.1/bits/stl_deque.h
src/codegen/syntax_tree_printer.o: /usr/include/c++/4.2.1/bits/deque.tcc
src/codegen/syntax_tree_printer.o: src/semantics/constants.h
src/parser/assignment.o: src/parser/grammar.h src/semantics/syntax_tree.h
src/parser/assignment.o: /usr/include/c++/4.2.1/deque
src/parser/assignment.o: /usr/include/c++/4.2.1/bits/functexcept.h
src/parser/assignment.o: /usr/include/c++/4.2.1/exception_defines.h
src/parser/assignment.o: /usr/include/c++/4.2.1/bits/stl_algobase.h
src/parser/assignment.o: /usr/include/c++/4.2.1/cstring
src/parser/assignment.o: /usr/include/c++/4.2.1/cstddef /usr/include/stddef.h
src/parser/assignment.o: /usr/include/_types.h /usr/include/sys/_types.h
src/parser/assignment.o: /usr/include/sys/cdefs.h
src/parser/assignment.o: /usr/include/machine/_types.h
src/parser/assignment.o: /usr/include/i386/_types.h /usr/include/string.h
src/parser/assignment.o: /usr/include/secure/_string.h
src/parser/assignment.o: /usr/include/secure/_common.h
src/parser/assignment.o: /usr/include/c++/4.2.1/climits /usr/include/limits.h
src/parser/assignment.o: /usr/include/machine/limits.h
src/parser/assignment.o: /usr/include/i386/limits.h
src/parser/assignment.o: /usr/include/i386/_limits.h
src/parser/assignment.o: /usr/include/sys/syslimits.h
src/parser/assignment.o: /usr/include/c++/4.2.1/cstdlib
src/parser/assignment.o: /usr/include/c++/4.2.1/iosfwd
src/parser/assignment.o: /usr/include/c++/4.2.1/cctype /usr/include/ctype.h
src/parser/assignment.o: /usr/include/runetype.h
src/parser/assignment.o: /usr/include/c++/4.2.1/bits/stringfwd.h
src/parser/assignment.o: /usr/include/c++/4.2.1/bits/postypes.h
src/parser/assignment.o: /usr/include/c++/4.2.1/cwchar
src/parser/assignment.o: /usr/include/c++/4.2.1/ctime /usr/include/time.h
src/parser/assignment.o: /usr/include/_structs.h /usr/include/sys/_structs.h
src/parser/assignment.o: /usr/include/c++/4.2.1/bits/stl_pair.h
src/parser/assignment.o: /usr/include/c++/4.2.1/bits/cpp_type_traits.h
src/parser/assignment.o: /usr/include/c++/4.2.1/ext/type_traits.h
src/parser/assignment.o: /usr/include/c++/4.2.1/utility
src/parser/assignment.o: /usr/include/c++/4.2.1/bits/stl_relops.h
src/parser/assignment.o: /usr/include/c++/4.2.1/bits/stl_iterator_base_types.h
src/parser/assignment.o: /usr/include/c++/4.2.1/bits/stl_iterator_base_funcs.h
src/parser/assignment.o: /usr/include/c++/4.2.1/bits/concept_check.h
src/parser/assignment.o: /usr/include/c++/4.2.1/bits/stl_iterator.h
src/parser/assignment.o: /usr/include/c++/4.2.1/debug/debug.h
src/parser/assignment.o: /usr/include/c++/4.2.1/bits/allocator.h
src/parser/assignment.o: /usr/include/c++/4.2.1/bits/stl_construct.h
src/parser/assignment.o: /usr/include/c++/4.2.1/new
src/parser/assignment.o: /usr/include/c++/4.2.1/exception
src/parser/assignment.o: /usr/include/c++/4.2.1/bits/stl_uninitialized.h
src/parser/assignment.o: /usr/include/c++/4.2.1/bits/stl_deque.h
src/parser/assignment.o: /usr/include/c++/4.2.1/bits/deque.tcc
src/parser/assignment.o: /usr/include/c++/4.2.1/string
src/parser/assignment.o: /usr/include/c++/4.2.1/bits/char_traits.h
src/parser/assignment.o: /usr/include/c++/4.2.1/memory
src/parser/assignment.o: /usr/include/c++/4.2.1/bits/stl_raw_storage_iter.h
src/parser/assignment.o: /usr/include/c++/4.2.1/limits
src/parser/assignment.o: /usr/include/c++/4.2.1/bits/ostream_insert.h
src/parser/assignment.o: /usr/include/c++/4.2.1/bits/stl_function.h
src/parser/assignment.o: /usr/include/c++/4.2.1/bits/basic_string.h
src/parser/assignment.o: /usr/include/c++/4.2.1/ext/atomicity.h
src/parser/assignment.o: /usr/include/c++/4.2.1/algorithm
src/parser/assignment.o: /usr/include/c++/4.2.1/bits/stl_algo.h
src/parser/assignment.o: /usr/include/c++/4.2.1/bits/stl_heap.h
src/parser/assignment.o: /usr/include/c++/4.2.1/bits/stl_tempbuf.h
src/parser/assignment.o: /usr/include/c++/4.2.1/bits/basic_string.tcc
src/parser/assignment.o: src/semantics/constants.h
src/parser/bitwise.o: src/parser/grammar.h src/semantics/syntax_tree.h
src/parser/bitwise.o: /usr/include/c++/4.2.1/deque
src/parser/bitwise.o: /usr/include/c++/4.2.1/bits/functexcept.h
src/parser/bitwise.o: /usr/include/c++/4.2.1/exception_defines.h
src/parser/bitwise.o: /usr/include/c++/4.2.1/bits/stl_algobase.h
src/parser/bitwise.o: /usr/include/c++/4.2.1/cstring
src/parser/bitwise.o: /usr/include/c++/4.2.1/cstddef /usr/include/stddef.h
src/parser/bitwise.o: /usr/include/_types.h /usr/include/sys/_types.h
src/parser/bitwise.o: /usr/include/sys/cdefs.h /usr/include/machine/_types.h
src/parser/bitwise.o: /usr/include/i386/_types.h /usr/include/string.h
src/parser/bitwise.o: /usr/include/secure/_string.h
src/parser/bitwise.o: /usr/include/secure/_common.h
src/parser/bitwise.o: /usr/include/c++/4.2.1/climits /usr/include/limits.h
src/parser/bitwise.o: /usr/include/machine/limits.h
src/parser/bitwise.o: /usr/include/i386/limits.h /usr/include/i386/_limits.h
src/parser/bitwise.o: /usr/include/sys/syslimits.h
src/parser/bitwise.o: /usr/include/c++/4.2.1/cstdlib
src/parser/bitwise.o: /usr/include/c++/4.2.1/iosfwd
src/parser/bitwise.o: /usr/include/c++/4.2.1/cctype /usr/include/ctype.h
src/parser/bitwise.o: /usr/include/runetype.h
src/parser/bitwise.o: /usr/include/c++/4.2.1/bits/stringfwd.h
src/parser/bitwise.o: /usr/include/c++/4.2.1/bits/postypes.h
src/parser/bitwise.o: /usr/include/c++/4.2.1/cwchar
src/parser/bitwise.o: /usr/include/c++/4.2.1/ctime /usr/include/time.h
src/parser/bitwise.o: /usr/include/_structs.h /usr/include/sys/_structs.h
src/parser/bitwise.o: /usr/include/c++/4.2.1/bits/stl_pair.h
src/parser/bitwise.o: /usr/include/c++/4.2.1/bits/cpp_type_traits.h
src/parser/bitwise.o: /usr/include/c++/4.2.1/ext/type_traits.h
src/parser/bitwise.o: /usr/include/c++/4.2.1/utility
src/parser/bitwise.o: /usr/include/c++/4.2.1/bits/stl_relops.h
src/parser/bitwise.o: /usr/include/c++/4.2.1/bits/stl_iterator_base_types.h
src/parser/bitwise.o: /usr/include/c++/4.2.1/bits/stl_iterator_base_funcs.h
src/parser/bitwise.o: /usr/include/c++/4.2.1/bits/concept_check.h
src/parser/bitwise.o: /usr/include/c++/4.2.1/bits/stl_iterator.h
src/parser/bitwise.o: /usr/include/c++/4.2.1/debug/debug.h
src/parser/bitwise.o: /usr/include/c++/4.2.1/bits/allocator.h
src/parser/bitwise.o: /usr/include/c++/4.2.1/bits/stl_construct.h
src/parser/bitwise.o: /usr/include/c++/4.2.1/new
src/parser/bitwise.o: /usr/include/c++/4.2.1/exception
src/parser/bitwise.o: /usr/include/c++/4.2.1/bits/stl_uninitialized.h
src/parser/bitwise.o: /usr/include/c++/4.2.1/bits/stl_deque.h
src/parser/bitwise.o: /usr/include/c++/4.2.1/bits/deque.tcc
src/parser/bitwise.o: /usr/include/c++/4.2.1/string
src/parser/bitwise.o: /usr/include/c++/4.2.1/bits/char_traits.h
src/parser/bitwise.o: /usr/include/c++/4.2.1/memory
src/parser/bitwise.o: /usr/include/c++/4.2.1/bits/stl_raw_storage_iter.h
src/parser/bitwise.o: /usr/include/c++/4.2.1/limits
src/parser/bitwise.o: /usr/include/c++/4.2.1/bits/ostream_insert.h
src/parser/bitwise.o: /usr/include/c++/4.2.1/bits/stl_function.h
src/parser/bitwise.o: /usr/include/c++/4.2.1/bits/basic_string.h
src/parser/bitwise.o: /usr/include/c++/4.2.1/ext/atomicity.h
src/parser/bitwise.o: /usr/include/c++/4.2.1/algorithm
src/parser/bitwise.o: /usr/include/c++/4.2.1/bits/stl_algo.h
src/parser/bitwise.o: /usr/include/c++/4.2.1/bits/stl_heap.h
src/parser/bitwise.o: /usr/include/c++/4.2.1/bits/stl_tempbuf.h
src/parser/bitwise.o: /usr/include/c++/4.2.1/bits/basic_string.tcc
src/parser/bitwise.o: src/semantics/constants.h
src/parser/classes.o: src/parser/grammar.h src/semantics/syntax_tree.h
src/parser/classes.o: /usr/include/c++/4.2.1/deque
src/parser/classes.o: /usr/include/c++/4.2.1/bits/functexcept.h
src/parser/classes.o: /usr/include/c++/4.2.1/exception_defines.h
src/parser/classes.o: /usr/include/c++/4.2.1/bits/stl_algobase.h
src/parser/classes.o: /usr/include/c++/4.2.1/cstring
src/parser/classes.o: /usr/include/c++/4.2.1/cstddef /usr/include/stddef.h
src/parser/classes.o: /usr/include/_types.h /usr/include/sys/_types.h
src/parser/classes.o: /usr/include/sys/cdefs.h /usr/include/machine/_types.h
src/parser/classes.o: /usr/include/i386/_types.h /usr/include/string.h
src/parser/classes.o: /usr/include/secure/_string.h
src/parser/classes.o: /usr/include/secure/_common.h
src/parser/classes.o: /usr/include/c++/4.2.1/climits /usr/include/limits.h
src/parser/classes.o: /usr/include/machine/limits.h
src/parser/classes.o: /usr/include/i386/limits.h /usr/include/i386/_limits.h
src/parser/classes.o: /usr/include/sys/syslimits.h
src/parser/classes.o: /usr/include/c++/4.2.1/cstdlib
src/parser/classes.o: /usr/include/c++/4.2.1/iosfwd
src/parser/classes.o: /usr/include/c++/4.2.1/cctype /usr/include/ctype.h
src/parser/classes.o: /usr/include/runetype.h
src/parser/classes.o: /usr/include/c++/4.2.1/bits/stringfwd.h
src/parser/classes.o: /usr/include/c++/4.2.1/bits/postypes.h
src/parser/classes.o: /usr/include/c++/4.2.1/cwchar
src/parser/classes.o: /usr/include/c++/4.2.1/ctime /usr/include/time.h
src/parser/classes.o: /usr/include/_structs.h /usr/include/sys/_structs.h
src/parser/classes.o: /usr/include/c++/4.2.1/bits/stl_pair.h
src/parser/classes.o: /usr/include/c++/4.2.1/bits/cpp_type_traits.h
src/parser/classes.o: /usr/include/c++/4.2.1/ext/type_traits.h
src/parser/classes.o: /usr/include/c++/4.2.1/utility
src/parser/classes.o: /usr/include/c++/4.2.1/bits/stl_relops.h
src/parser/classes.o: /usr/include/c++/4.2.1/bits/stl_iterator_base_types.h
src/parser/classes.o: /usr/include/c++/4.2.1/bits/stl_iterator_base_funcs.h
src/parser/classes.o: /usr/include/c++/4.2.1/bits/concept_check.h
src/parser/classes.o: /usr/include/c++/4.2.1/bits/stl_iterator.h
src/parser/classes.o: /usr/include/c++/4.2.1/debug/debug.h
src/parser/classes.o: /usr/include/c++/4.2.1/bits/allocator.h
src/parser/classes.o: /usr/include/c++/4.2.1/bits/stl_construct.h
src/parser/classes.o: /usr/include/c++/4.2.1/new
src/parser/classes.o: /usr/include/c++/4.2.1/exception
src/parser/classes.o: /usr/include/c++/4.2.1/bits/stl_uninitialized.h
src/parser/classes.o: /usr/include/c++/4.2.1/bits/stl_deque.h
src/parser/classes.o: /usr/include/c++/4.2.1/bits/deque.tcc
src/parser/classes.o: /usr/include/c++/4.2.1/string
src/parser/classes.o: /usr/include/c++/4.2.1/bits/char_traits.h
src/parser/classes.o: /usr/include/c++/4.2.1/memory
src/parser/classes.o: /usr/include/c++/4.2.1/bits/stl_raw_storage_iter.h
src/parser/classes.o: /usr/include/c++/4.2.1/limits
src/parser/classes.o: /usr/include/c++/4.2.1/bits/ostream_insert.h
src/parser/classes.o: /usr/include/c++/4.2.1/bits/stl_function.h
src/parser/classes.o: /usr/include/c++/4.2.1/bits/basic_string.h
src/parser/classes.o: /usr/include/c++/4.2.1/ext/atomicity.h
src/parser/classes.o: /usr/include/c++/4.2.1/algorithm
src/parser/classes.o: /usr/include/c++/4.2.1/bits/stl_algo.h
src/parser/classes.o: /usr/include/c++/4.2.1/bits/stl_heap.h
src/parser/classes.o: /usr/include/c++/4.2.1/bits/stl_tempbuf.h
src/parser/classes.o: /usr/include/c++/4.2.1/bits/basic_string.tcc
src/parser/classes.o: src/semantics/constants.h
src/parser/comparison.o: src/parser/grammar.h src/semantics/syntax_tree.h
src/parser/comparison.o: /usr/include/c++/4.2.1/deque
src/parser/comparison.o: /usr/include/c++/4.2.1/bits/functexcept.h
src/parser/comparison.o: /usr/include/c++/4.2.1/exception_defines.h
src/parser/comparison.o: /usr/include/c++/4.2.1/bits/stl_algobase.h
src/parser/comparison.o: /usr/include/c++/4.2.1/cstring
src/parser/comparison.o: /usr/include/c++/4.2.1/cstddef /usr/include/stddef.h
src/parser/comparison.o: /usr/include/_types.h /usr/include/sys/_types.h
src/parser/comparison.o: /usr/include/sys/cdefs.h
src/parser/comparison.o: /usr/include/machine/_types.h
src/parser/comparison.o: /usr/include/i386/_types.h /usr/include/string.h
src/parser/comparison.o: /usr/include/secure/_string.h
src/parser/comparison.o: /usr/include/secure/_common.h
src/parser/comparison.o: /usr/include/c++/4.2.1/climits /usr/include/limits.h
src/parser/comparison.o: /usr/include/machine/limits.h
src/parser/comparison.o: /usr/include/i386/limits.h
src/parser/comparison.o: /usr/include/i386/_limits.h
src/parser/comparison.o: /usr/include/sys/syslimits.h
src/parser/comparison.o: /usr/include/c++/4.2.1/cstdlib
src/parser/comparison.o: /usr/include/c++/4.2.1/iosfwd
src/parser/comparison.o: /usr/include/c++/4.2.1/cctype /usr/include/ctype.h
src/parser/comparison.o: /usr/include/runetype.h
src/parser/comparison.o: /usr/include/c++/4.2.1/bits/stringfwd.h
src/parser/comparison.o: /usr/include/c++/4.2.1/bits/postypes.h
src/parser/comparison.o: /usr/include/c++/4.2.1/cwchar
src/parser/comparison.o: /usr/include/c++/4.2.1/ctime /usr/include/time.h
src/parser/comparison.o: /usr/include/_structs.h /usr/include/sys/_structs.h
src/parser/comparison.o: /usr/include/c++/4.2.1/bits/stl_pair.h
src/parser/comparison.o: /usr/include/c++/4.2.1/bits/cpp_type_traits.h
src/parser/comparison.o: /usr/include/c++/4.2.1/ext/type_traits.h
src/parser/comparison.o: /usr/include/c++/4.2.1/utility
src/parser/comparison.o: /usr/include/c++/4.2.1/bits/stl_relops.h
src/parser/comparison.o: /usr/include/c++/4.2.1/bits/stl_iterator_base_types.h
src/parser/comparison.o: /usr/include/c++/4.2.1/bits/stl_iterator_base_funcs.h
src/parser/comparison.o: /usr/include/c++/4.2.1/bits/concept_check.h
src/parser/comparison.o: /usr/include/c++/4.2.1/bits/stl_iterator.h
src/parser/comparison.o: /usr/include/c++/4.2.1/debug/debug.h
src/parser/comparison.o: /usr/include/c++/4.2.1/bits/allocator.h
src/parser/comparison.o: /usr/include/c++/4.2.1/bits/stl_construct.h
src/parser/comparison.o: /usr/include/c++/4.2.1/new
src/parser/comparison.o: /usr/include/c++/4.2.1/exception
src/parser/comparison.o: /usr/include/c++/4.2.1/bits/stl_uninitialized.h
src/parser/comparison.o: /usr/include/c++/4.2.1/bits/stl_deque.h
src/parser/comparison.o: /usr/include/c++/4.2.1/bits/deque.tcc
src/parser/comparison.o: /usr/include/c++/4.2.1/string
src/parser/comparison.o: /usr/include/c++/4.2.1/bits/char_traits.h
src/parser/comparison.o: /usr/include/c++/4.2.1/memory
src/parser/comparison.o: /usr/include/c++/4.2.1/bits/stl_raw_storage_iter.h
src/parser/comparison.o: /usr/include/c++/4.2.1/limits
src/parser/comparison.o: /usr/include/c++/4.2.1/bits/ostream_insert.h
src/parser/comparison.o: /usr/include/c++/4.2.1/bits/stl_function.h
src/parser/comparison.o: /usr/include/c++/4.2.1/bits/basic_string.h
src/parser/comparison.o: /usr/include/c++/4.2.1/ext/atomicity.h
src/parser/comparison.o: /usr/include/c++/4.2.1/algorithm
src/parser/comparison.o: /usr/include/c++/4.2.1/bits/stl_algo.h
src/parser/comparison.o: /usr/include/c++/4.2.1/bits/stl_heap.h
src/parser/comparison.o: /usr/include/c++/4.2.1/bits/stl_tempbuf.h
src/parser/comparison.o: /usr/include/c++/4.2.1/bits/basic_string.tcc
src/parser/comparison.o: src/semantics/constants.h
src/parser/constants.o: src/parser/grammar.h src/semantics/syntax_tree.h
src/parser/constants.o: /usr/include/c++/4.2.1/deque
src/parser/constants.o: /usr/include/c++/4.2.1/bits/functexcept.h
src/parser/constants.o: /usr/include/c++/4.2.1/exception_defines.h
src/parser/constants.o: /usr/include/c++/4.2.1/bits/stl_algobase.h
src/parser/constants.o: /usr/include/c++/4.2.1/cstring
src/parser/constants.o: /usr/include/c++/4.2.1/cstddef /usr/include/stddef.h
src/parser/constants.o: /usr/include/_types.h /usr/include/sys/_types.h
src/parser/constants.o: /usr/include/sys/cdefs.h
src/parser/constants.o: /usr/include/machine/_types.h
src/parser/constants.o: /usr/include/i386/_types.h /usr/include/string.h
src/parser/constants.o: /usr/include/secure/_string.h
src/parser/constants.o: /usr/include/secure/_common.h
src/parser/constants.o: /usr/include/c++/4.2.1/climits /usr/include/limits.h
src/parser/constants.o: /usr/include/machine/limits.h
src/parser/constants.o: /usr/include/i386/limits.h
src/parser/constants.o: /usr/include/i386/_limits.h
src/parser/constants.o: /usr/include/sys/syslimits.h
src/parser/constants.o: /usr/include/c++/4.2.1/cstdlib
src/parser/constants.o: /usr/include/c++/4.2.1/iosfwd
src/parser/constants.o: /usr/include/c++/4.2.1/cctype /usr/include/ctype.h
src/parser/constants.o: /usr/include/runetype.h
src/parser/constants.o: /usr/include/c++/4.2.1/bits/stringfwd.h
src/parser/constants.o: /usr/include/c++/4.2.1/bits/postypes.h
src/parser/constants.o: /usr/include/c++/4.2.1/cwchar
src/parser/constants.o: /usr/include/c++/4.2.1/ctime /usr/include/time.h
src/parser/constants.o: /usr/include/_structs.h /usr/include/sys/_structs.h
src/parser/constants.o: /usr/include/c++/4.2.1/bits/stl_pair.h
src/parser/constants.o: /usr/include/c++/4.2.1/bits/cpp_type_traits.h
src/parser/constants.o: /usr/include/c++/4.2.1/ext/type_traits.h
src/parser/constants.o: /usr/include/c++/4.2.1/utility
src/parser/constants.o: /usr/include/c++/4.2.1/bits/stl_relops.h
src/parser/constants.o: /usr/include/c++/4.2.1/bits/stl_iterator_base_types.h
src/parser/constants.o: /usr/include/c++/4.2.1/bits/stl_iterator_base_funcs.h
src/parser/constants.o: /usr/include/c++/4.2.1/bits/concept_check.h
src/parser/constants.o: /usr/include/c++/4.2.1/bits/stl_iterator.h
src/parser/constants.o: /usr/include/c++/4.2.1/debug/debug.h
src/parser/constants.o: /usr/include/c++/4.2.1/bits/allocator.h
src/parser/constants.o: /usr/include/c++/4.2.1/bits/stl_construct.h
src/parser/constants.o: /usr/include/c++/4.2.1/new
src/parser/constants.o: /usr/include/c++/4.2.1/exception
src/parser/constants.o: /usr/include/c++/4.2.1/bits/stl_uninitialized.h
src/parser/constants.o: /usr/include/c++/4.2.1/bits/stl_deque.h
src/parser/constants.o: /usr/include/c++/4.2.1/bits/deque.tcc
src/parser/constants.o: /usr/include/c++/4.2.1/string
src/parser/constants.o: /usr/include/c++/4.2.1/bits/char_traits.h
src/parser/constants.o: /usr/include/c++/4.2.1/memory
src/parser/constants.o: /usr/include/c++/4.2.1/bits/stl_raw_storage_iter.h
src/parser/constants.o: /usr/include/c++/4.2.1/limits
src/parser/constants.o: /usr/include/c++/4.2.1/bits/ostream_insert.h
src/parser/constants.o: /usr/include/c++/4.2.1/bits/stl_function.h
src/parser/constants.o: /usr/include/c++/4.2.1/bits/basic_string.h
src/parser/constants.o: /usr/include/c++/4.2.1/ext/atomicity.h
src/parser/constants.o: /usr/include/c++/4.2.1/algorithm
src/parser/constants.o: /usr/include/c++/4.2.1/bits/stl_algo.h
src/parser/constants.o: /usr/include/c++/4.2.1/bits/stl_heap.h
src/parser/constants.o: /usr/include/c++/4.2.1/bits/stl_tempbuf.h
src/parser/constants.o: /usr/include/c++/4.2.1/bits/basic_string.tcc
src/parser/constants.o: src/semantics/constants.h
src/parser/decide.o: src/parser/grammar.h src/semantics/syntax_tree.h
src/parser/decide.o: /usr/include/c++/4.2.1/deque
src/parser/decide.o: /usr/include/c++/4.2.1/bits/functexcept.h
src/parser/decide.o: /usr/include/c++/4.2.1/exception_defines.h
src/parser/decide.o: /usr/include/c++/4.2.1/bits/stl_algobase.h
src/parser/decide.o: /usr/include/c++/4.2.1/cstring
src/parser/decide.o: /usr/include/c++/4.2.1/cstddef /usr/include/stddef.h
src/parser/decide.o: /usr/include/_types.h /usr/include/sys/_types.h
src/parser/decide.o: /usr/include/sys/cdefs.h /usr/include/machine/_types.h
src/parser/decide.o: /usr/include/i386/_types.h /usr/include/string.h
src/parser/decide.o: /usr/include/secure/_string.h
src/parser/decide.o: /usr/include/secure/_common.h
src/parser/decide.o: /usr/include/c++/4.2.1/climits /usr/include/limits.h
src/parser/decide.o: /usr/include/machine/limits.h /usr/include/i386/limits.h
src/parser/decide.o: /usr/include/i386/_limits.h /usr/include/sys/syslimits.h
src/parser/decide.o: /usr/include/c++/4.2.1/cstdlib
src/parser/decide.o: /usr/include/c++/4.2.1/iosfwd
src/parser/decide.o: /usr/include/c++/4.2.1/cctype /usr/include/ctype.h
src/parser/decide.o: /usr/include/runetype.h
src/parser/decide.o: /usr/include/c++/4.2.1/bits/stringfwd.h
src/parser/decide.o: /usr/include/c++/4.2.1/bits/postypes.h
src/parser/decide.o: /usr/include/c++/4.2.1/cwchar
src/parser/decide.o: /usr/include/c++/4.2.1/ctime /usr/include/time.h
src/parser/decide.o: /usr/include/_structs.h /usr/include/sys/_structs.h
src/parser/decide.o: /usr/include/c++/4.2.1/bits/stl_pair.h
src/parser/decide.o: /usr/include/c++/4.2.1/bits/cpp_type_traits.h
src/parser/decide.o: /usr/include/c++/4.2.1/ext/type_traits.h
src/parser/decide.o: /usr/include/c++/4.2.1/utility
src/parser/decide.o: /usr/include/c++/4.2.1/bits/stl_relops.h
src/parser/decide.o: /usr/include/c++/4.2.1/bits/stl_iterator_base_types.h
src/parser/decide.o: /usr/include/c++/4.2.1/bits/stl_iterator_base_funcs.h
src/parser/decide.o: /usr/include/c++/4.2.1/bits/concept_check.h
src/parser/decide.o: /usr/include/c++/4.2.1/bits/stl_iterator.h
src/parser/decide.o: /usr/include/c++/4.2.1/debug/debug.h
src/parser/decide.o: /usr/include/c++/4.2.1/bits/allocator.h
src/parser/decide.o: /usr/include/c++/4.2.1/bits/stl_construct.h
src/parser/decide.o: /usr/include/c++/4.2.1/new
src/parser/decide.o: /usr/include/c++/4.2.1/exception
src/parser/decide.o: /usr/include/c++/4.2.1/bits/stl_uninitialized.h
src/parser/decide.o: /usr/include/c++/4.2.1/bits/stl_deque.h
src/parser/decide.o: /usr/include/c++/4.2.1/bits/deque.tcc
src/parser/decide.o: /usr/include/c++/4.2.1/string
src/parser/decide.o: /usr/include/c++/4.2.1/bits/char_traits.h
src/parser/decide.o: /usr/include/c++/4.2.1/memory
src/parser/decide.o: /usr/include/c++/4.2.1/bits/stl_raw_storage_iter.h
src/parser/decide.o: /usr/include/c++/4.2.1/limits
src/parser/decide.o: /usr/include/c++/4.2.1/bits/ostream_insert.h
src/parser/decide.o: /usr/include/c++/4.2.1/bits/stl_function.h
src/parser/decide.o: /usr/include/c++/4.2.1/bits/basic_string.h
src/parser/decide.o: /usr/include/c++/4.2.1/ext/atomicity.h
src/parser/decide.o: /usr/include/c++/4.2.1/algorithm
src/parser/decide.o: /usr/include/c++/4.2.1/bits/stl_algo.h
src/parser/decide.o: /usr/include/c++/4.2.1/bits/stl_heap.h
src/parser/decide.o: /usr/include/c++/4.2.1/bits/stl_tempbuf.h
src/parser/decide.o: /usr/include/c++/4.2.1/bits/basic_string.tcc
src/parser/decide.o: src/semantics/constants.h
src/parser/deref.o: src/parser/grammar.h src/semantics/syntax_tree.h
src/parser/deref.o: /usr/include/c++/4.2.1/deque
src/parser/deref.o: /usr/include/c++/4.2.1/bits/functexcept.h
src/parser/deref.o: /usr/include/c++/4.2.1/exception_defines.h
src/parser/deref.o: /usr/include/c++/4.2.1/bits/stl_algobase.h
src/parser/deref.o: /usr/include/c++/4.2.1/cstring
src/parser/deref.o: /usr/include/c++/4.2.1/cstddef /usr/include/stddef.h
src/parser/deref.o: /usr/include/_types.h /usr/include/sys/_types.h
src/parser/deref.o: /usr/include/sys/cdefs.h /usr/include/machine/_types.h
src/parser/deref.o: /usr/include/i386/_types.h /usr/include/string.h
src/parser/deref.o: /usr/include/secure/_string.h
src/parser/deref.o: /usr/include/secure/_common.h
src/parser/deref.o: /usr/include/c++/4.2.1/climits /usr/include/limits.h
src/parser/deref.o: /usr/include/machine/limits.h /usr/include/i386/limits.h
src/parser/deref.o: /usr/include/i386/_limits.h /usr/include/sys/syslimits.h
src/parser/deref.o: /usr/include/c++/4.2.1/cstdlib
src/parser/deref.o: /usr/include/c++/4.2.1/iosfwd
src/parser/deref.o: /usr/include/c++/4.2.1/cctype /usr/include/ctype.h
src/parser/deref.o: /usr/include/runetype.h
src/parser/deref.o: /usr/include/c++/4.2.1/bits/stringfwd.h
src/parser/deref.o: /usr/include/c++/4.2.1/bits/postypes.h
src/parser/deref.o: /usr/include/c++/4.2.1/cwchar
src/parser/deref.o: /usr/include/c++/4.2.1/ctime /usr/include/time.h
src/parser/deref.o: /usr/include/_structs.h /usr/include/sys/_structs.h
src/parser/deref.o: /usr/include/c++/4.2.1/bits/stl_pair.h
src/parser/deref.o: /usr/include/c++/4.2.1/bits/cpp_type_traits.h
src/parser/deref.o: /usr/include/c++/4.2.1/ext/type_traits.h
src/parser/deref.o: /usr/include/c++/4.2.1/utility
src/parser/deref.o: /usr/include/c++/4.2.1/bits/stl_relops.h
src/parser/deref.o: /usr/include/c++/4.2.1/bits/stl_iterator_base_types.h
src/parser/deref.o: /usr/include/c++/4.2.1/bits/stl_iterator_base_funcs.h
src/parser/deref.o: /usr/include/c++/4.2.1/bits/concept_check.h
src/parser/deref.o: /usr/include/c++/4.2.1/bits/stl_iterator.h
src/parser/deref.o: /usr/include/c++/4.2.1/debug/debug.h
src/parser/deref.o: /usr/include/c++/4.2.1/bits/allocator.h
src/parser/deref.o: /usr/include/c++/4.2.1/bits/stl_construct.h
src/parser/deref.o: /usr/include/c++/4.2.1/new
src/parser/deref.o: /usr/include/c++/4.2.1/exception
src/parser/deref.o: /usr/include/c++/4.2.1/bits/stl_uninitialized.h
src/parser/deref.o: /usr/include/c++/4.2.1/bits/stl_deque.h
src/parser/deref.o: /usr/include/c++/4.2.1/bits/deque.tcc
src/parser/deref.o: /usr/include/c++/4.2.1/string
src/parser/deref.o: /usr/include/c++/4.2.1/bits/char_traits.h
src/parser/deref.o: /usr/include/c++/4.2.1/memory
src/parser/deref.o: /usr/include/c++/4.2.1/bits/stl_raw_storage_iter.h
src/parser/deref.o: /usr/include/c++/4.2.1/limits
src/parser/deref.o: /usr/include/c++/4.2.1/bits/ostream_insert.h
src/parser/deref.o: /usr/include/c++/4.2.1/bits/stl_function.h
src/parser/deref.o: /usr/include/c++/4.2.1/bits/basic_string.h
src/parser/deref.o: /usr/include/c++/4.2.1/ext/atomicity.h
src/parser/deref.o: /usr/include/c++/4.2.1/algorithm
src/parser/deref.o: /usr/include/c++/4.2.1/bits/stl_algo.h
src/parser/deref.o: /usr/include/c++/4.2.1/bits/stl_heap.h
src/parser/deref.o: /usr/include/c++/4.2.1/bits/stl_tempbuf.h
src/parser/deref.o: /usr/include/c++/4.2.1/bits/basic_string.tcc
src/parser/deref.o: src/semantics/constants.h
src/parser/grouping.o: src/parser/grammar.h src/semantics/syntax_tree.h
src/parser/grouping.o: /usr/include/c++/4.2.1/deque
src/parser/grouping.o: /usr/include/c++/4.2.1/bits/functexcept.h
src/parser/grouping.o: /usr/include/c++/4.2.1/exception_defines.h
src/parser/grouping.o: /usr/include/c++/4.2.1/bits/stl_algobase.h
src/parser/grouping.o: /usr/include/c++/4.2.1/cstring
src/parser/grouping.o: /usr/include/c++/4.2.1/cstddef /usr/include/stddef.h
src/parser/grouping.o: /usr/include/_types.h /usr/include/sys/_types.h
src/parser/grouping.o: /usr/include/sys/cdefs.h /usr/include/machine/_types.h
src/parser/grouping.o: /usr/include/i386/_types.h /usr/include/string.h
src/parser/grouping.o: /usr/include/secure/_string.h
src/parser/grouping.o: /usr/include/secure/_common.h
src/parser/grouping.o: /usr/include/c++/4.2.1/climits /usr/include/limits.h
src/parser/grouping.o: /usr/include/machine/limits.h
src/parser/grouping.o: /usr/include/i386/limits.h /usr/include/i386/_limits.h
src/parser/grouping.o: /usr/include/sys/syslimits.h
src/parser/grouping.o: /usr/include/c++/4.2.1/cstdlib
src/parser/grouping.o: /usr/include/c++/4.2.1/iosfwd
src/parser/grouping.o: /usr/include/c++/4.2.1/cctype /usr/include/ctype.h
src/parser/grouping.o: /usr/include/runetype.h
src/parser/grouping.o: /usr/include/c++/4.2.1/bits/stringfwd.h
src/parser/grouping.o: /usr/include/c++/4.2.1/bits/postypes.h
src/parser/grouping.o: /usr/include/c++/4.2.1/cwchar
src/parser/grouping.o: /usr/include/c++/4.2.1/ctime /usr/include/time.h
src/parser/grouping.o: /usr/include/_structs.h /usr/include/sys/_structs.h
src/parser/grouping.o: /usr/include/c++/4.2.1/bits/stl_pair.h
src/parser/grouping.o: /usr/include/c++/4.2.1/bits/cpp_type_traits.h
src/parser/grouping.o: /usr/include/c++/4.2.1/ext/type_traits.h
src/parser/grouping.o: /usr/include/c++/4.2.1/utility
src/parser/grouping.o: /usr/include/c++/4.2.1/bits/stl_relops.h
src/parser/grouping.o: /usr/include/c++/4.2.1/bits/stl_iterator_base_types.h
src/parser/grouping.o: /usr/include/c++/4.2.1/bits/stl_iterator_base_funcs.h
src/parser/grouping.o: /usr/include/c++/4.2.1/bits/concept_check.h
src/parser/grouping.o: /usr/include/c++/4.2.1/bits/stl_iterator.h
src/parser/grouping.o: /usr/include/c++/4.2.1/debug/debug.h
src/parser/grouping.o: /usr/include/c++/4.2.1/bits/allocator.h
src/parser/grouping.o: /usr/include/c++/4.2.1/bits/stl_construct.h
src/parser/grouping.o: /usr/include/c++/4.2.1/new
src/parser/grouping.o: /usr/include/c++/4.2.1/exception
src/parser/grouping.o: /usr/include/c++/4.2.1/bits/stl_uninitialized.h
src/parser/grouping.o: /usr/include/c++/4.2.1/bits/stl_deque.h
src/parser/grouping.o: /usr/include/c++/4.2.1/bits/deque.tcc
src/parser/grouping.o: /usr/include/c++/4.2.1/string
src/parser/grouping.o: /usr/include/c++/4.2.1/bits/char_traits.h
src/parser/grouping.o: /usr/include/c++/4.2.1/memory
src/parser/grouping.o: /usr/include/c++/4.2.1/bits/stl_raw_storage_iter.h
src/parser/grouping.o: /usr/include/c++/4.2.1/limits
src/parser/grouping.o: /usr/include/c++/4.2.1/bits/ostream_insert.h
src/parser/grouping.o: /usr/include/c++/4.2.1/bits/stl_function.h
src/parser/grouping.o: /usr/include/c++/4.2.1/bits/basic_string.h
src/parser/grouping.o: /usr/include/c++/4.2.1/ext/atomicity.h
src/parser/grouping.o: /usr/include/c++/4.2.1/algorithm
src/parser/grouping.o: /usr/include/c++/4.2.1/bits/stl_algo.h
src/parser/grouping.o: /usr/include/c++/4.2.1/bits/stl_heap.h
src/parser/grouping.o: /usr/include/c++/4.2.1/bits/stl_tempbuf.h
src/parser/grouping.o: /usr/include/c++/4.2.1/bits/basic_string.tcc
src/parser/grouping.o: src/semantics/constants.h
src/parser/loop.o: src/parser/grammar.h src/semantics/syntax_tree.h
src/parser/loop.o: /usr/include/c++/4.2.1/deque
src/parser/loop.o: /usr/include/c++/4.2.1/bits/functexcept.h
src/parser/loop.o: /usr/include/c++/4.2.1/exception_defines.h
src/parser/loop.o: /usr/include/c++/4.2.1/bits/stl_algobase.h
src/parser/loop.o: /usr/include/c++/4.2.1/cstring
src/parser/loop.o: /usr/include/c++/4.2.1/cstddef /usr/include/stddef.h
src/parser/loop.o: /usr/include/_types.h /usr/include/sys/_types.h
src/parser/loop.o: /usr/include/sys/cdefs.h /usr/include/machine/_types.h
src/parser/loop.o: /usr/include/i386/_types.h /usr/include/string.h
src/parser/loop.o: /usr/include/secure/_string.h
src/parser/loop.o: /usr/include/secure/_common.h
src/parser/loop.o: /usr/include/c++/4.2.1/climits /usr/include/limits.h
src/parser/loop.o: /usr/include/machine/limits.h /usr/include/i386/limits.h
src/parser/loop.o: /usr/include/i386/_limits.h /usr/include/sys/syslimits.h
src/parser/loop.o: /usr/include/c++/4.2.1/cstdlib
src/parser/loop.o: /usr/include/c++/4.2.1/iosfwd
src/parser/loop.o: /usr/include/c++/4.2.1/cctype /usr/include/ctype.h
src/parser/loop.o: /usr/include/runetype.h
src/parser/loop.o: /usr/include/c++/4.2.1/bits/stringfwd.h
src/parser/loop.o: /usr/include/c++/4.2.1/bits/postypes.h
src/parser/loop.o: /usr/include/c++/4.2.1/cwchar /usr/include/c++/4.2.1/ctime
src/parser/loop.o: /usr/include/time.h /usr/include/_structs.h
src/parser/loop.o: /usr/include/sys/_structs.h
src/parser/loop.o: /usr/include/c++/4.2.1/bits/stl_pair.h
src/parser/loop.o: /usr/include/c++/4.2.1/bits/cpp_type_traits.h
src/parser/loop.o: /usr/include/c++/4.2.1/ext/type_traits.h
src/parser/loop.o: /usr/include/c++/4.2.1/utility
src/parser/loop.o: /usr/include/c++/4.2.1/bits/stl_relops.h
src/parser/loop.o: /usr/include/c++/4.2.1/bits/stl_iterator_base_types.h
src/parser/loop.o: /usr/include/c++/4.2.1/bits/stl_iterator_base_funcs.h
src/parser/loop.o: /usr/include/c++/4.2.1/bits/concept_check.h
src/parser/loop.o: /usr/include/c++/4.2.1/bits/stl_iterator.h
src/parser/loop.o: /usr/include/c++/4.2.1/debug/debug.h
src/parser/loop.o: /usr/include/c++/4.2.1/bits/allocator.h
src/parser/loop.o: /usr/include/c++/4.2.1/bits/stl_construct.h
src/parser/loop.o: /usr/include/c++/4.2.1/new
src/parser/loop.o: /usr/include/c++/4.2.1/exception
src/parser/loop.o: /usr/include/c++/4.2.1/bits/stl_uninitialized.h
src/parser/loop.o: /usr/include/c++/4.2.1/bits/stl_deque.h
src/parser/loop.o: /usr/include/c++/4.2.1/bits/deque.tcc
src/parser/loop.o: /usr/include/c++/4.2.1/string
src/parser/loop.o: /usr/include/c++/4.2.1/bits/char_traits.h
src/parser/loop.o: /usr/include/c++/4.2.1/memory
src/parser/loop.o: /usr/include/c++/4.2.1/bits/stl_raw_storage_iter.h
src/parser/loop.o: /usr/include/c++/4.2.1/limits
src/parser/loop.o: /usr/include/c++/4.2.1/bits/ostream_insert.h
src/parser/loop.o: /usr/include/c++/4.2.1/bits/stl_function.h
src/parser/loop.o: /usr/include/c++/4.2.1/bits/basic_string.h
src/parser/loop.o: /usr/include/c++/4.2.1/ext/atomicity.h
src/parser/loop.o: /usr/include/c++/4.2.1/algorithm
src/parser/loop.o: /usr/include/c++/4.2.1/bits/stl_algo.h
src/parser/loop.o: /usr/include/c++/4.2.1/bits/stl_heap.h
src/parser/loop.o: /usr/include/c++/4.2.1/bits/stl_tempbuf.h
src/parser/loop.o: /usr/include/c++/4.2.1/bits/basic_string.tcc
src/parser/loop.o: src/semantics/constants.h
src/parser/map_reduce.o: src/parser/grammar.h src/semantics/syntax_tree.h
src/parser/map_reduce.o: /usr/include/c++/4.2.1/deque
src/parser/map_reduce.o: /usr/include/c++/4.2.1/bits/functexcept.h
src/parser/map_reduce.o: /usr/include/c++/4.2.1/exception_defines.h
src/parser/map_reduce.o: /usr/include/c++/4.2.1/bits/stl_algobase.h
src/parser/map_reduce.o: /usr/include/c++/4.2.1/cstring
src/parser/map_reduce.o: /usr/include/c++/4.2.1/cstddef /usr/include/stddef.h
src/parser/map_reduce.o: /usr/include/_types.h /usr/include/sys/_types.h
src/parser/map_reduce.o: /usr/include/sys/cdefs.h
src/parser/map_reduce.o: /usr/include/machine/_types.h
src/parser/map_reduce.o: /usr/include/i386/_types.h /usr/include/string.h
src/parser/map_reduce.o: /usr/include/secure/_string.h
src/parser/map_reduce.o: /usr/include/secure/_common.h
src/parser/map_reduce.o: /usr/include/c++/4.2.1/climits /usr/include/limits.h
src/parser/map_reduce.o: /usr/include/machine/limits.h
src/parser/map_reduce.o: /usr/include/i386/limits.h
src/parser/map_reduce.o: /usr/include/i386/_limits.h
src/parser/map_reduce.o: /usr/include/sys/syslimits.h
src/parser/map_reduce.o: /usr/include/c++/4.2.1/cstdlib
src/parser/map_reduce.o: /usr/include/c++/4.2.1/iosfwd
src/parser/map_reduce.o: /usr/include/c++/4.2.1/cctype /usr/include/ctype.h
src/parser/map_reduce.o: /usr/include/runetype.h
src/parser/map_reduce.o: /usr/include/c++/4.2.1/bits/stringfwd.h
src/parser/map_reduce.o: /usr/include/c++/4.2.1/bits/postypes.h
src/parser/map_reduce.o: /usr/include/c++/4.2.1/cwchar
src/parser/map_reduce.o: /usr/include/c++/4.2.1/ctime /usr/include/time.h
src/parser/map_reduce.o: /usr/include/_structs.h /usr/include/sys/_structs.h
src/parser/map_reduce.o: /usr/include/c++/4.2.1/bits/stl_pair.h
src/parser/map_reduce.o: /usr/include/c++/4.2.1/bits/cpp_type_traits.h
src/parser/map_reduce.o: /usr/include/c++/4.2.1/ext/type_traits.h
src/parser/map_reduce.o: /usr/include/c++/4.2.1/utility
src/parser/map_reduce.o: /usr/include/c++/4.2.1/bits/stl_relops.h
src/parser/map_reduce.o: /usr/include/c++/4.2.1/bits/stl_iterator_base_types.h
src/parser/map_reduce.o: /usr/include/c++/4.2.1/bits/stl_iterator_base_funcs.h
src/parser/map_reduce.o: /usr/include/c++/4.2.1/bits/concept_check.h
src/parser/map_reduce.o: /usr/include/c++/4.2.1/bits/stl_iterator.h
src/parser/map_reduce.o: /usr/include/c++/4.2.1/debug/debug.h
src/parser/map_reduce.o: /usr/include/c++/4.2.1/bits/allocator.h
src/parser/map_reduce.o: /usr/include/c++/4.2.1/bits/stl_construct.h
src/parser/map_reduce.o: /usr/include/c++/4.2.1/new
src/parser/map_reduce.o: /usr/include/c++/4.2.1/exception
src/parser/map_reduce.o: /usr/include/c++/4.2.1/bits/stl_uninitialized.h
src/parser/map_reduce.o: /usr/include/c++/4.2.1/bits/stl_deque.h
src/parser/map_reduce.o: /usr/include/c++/4.2.1/bits/deque.tcc
src/parser/map_reduce.o: /usr/include/c++/4.2.1/string
src/parser/map_reduce.o: /usr/include/c++/4.2.1/bits/char_traits.h
src/parser/map_reduce.o: /usr/include/c++/4.2.1/memory
src/parser/map_reduce.o: /usr/include/c++/4.2.1/bits/stl_raw_storage_iter.h
src/parser/map_reduce.o: /usr/include/c++/4.2.1/limits
src/parser/map_reduce.o: /usr/include/c++/4.2.1/bits/ostream_insert.h
src/parser/map_reduce.o: /usr/include/c++/4.2.1/bits/stl_function.h
src/parser/map_reduce.o: /usr/include/c++/4.2.1/bits/basic_string.h
src/parser/map_reduce.o: /usr/include/c++/4.2.1/ext/atomicity.h
src/parser/map_reduce.o: /usr/include/c++/4.2.1/algorithm
src/parser/map_reduce.o: /usr/include/c++/4.2.1/bits/stl_algo.h
src/parser/map_reduce.o: /usr/include/c++/4.2.1/bits/stl_heap.h
src/parser/map_reduce.o: /usr/include/c++/4.2.1/bits/stl_tempbuf.h
src/parser/map_reduce.o: /usr/include/c++/4.2.1/bits/basic_string.tcc
src/parser/map_reduce.o: src/semantics/constants.h
src/parser/math.o: src/parser/grammar.h src/semantics/syntax_tree.h
src/parser/math.o: /usr/include/c++/4.2.1/deque
src/parser/math.o: /usr/include/c++/4.2.1/bits/functexcept.h
src/parser/math.o: /usr/include/c++/4.2.1/exception_defines.h
src/parser/math.o: /usr/include/c++/4.2.1/bits/stl_algobase.h
src/parser/math.o: /usr/include/c++/4.2.1/cstring
src/parser/math.o: /usr/include/c++/4.2.1/cstddef /usr/include/stddef.h
src/parser/math.o: /usr/include/_types.h /usr/include/sys/_types.h
src/parser/math.o: /usr/include/sys/cdefs.h /usr/include/machine/_types.h
src/parser/math.o: /usr/include/i386/_types.h /usr/include/string.h
src/parser/math.o: /usr/include/secure/_string.h
src/parser/math.o: /usr/include/secure/_common.h
src/parser/math.o: /usr/include/c++/4.2.1/climits /usr/include/limits.h
src/parser/math.o: /usr/include/machine/limits.h /usr/include/i386/limits.h
src/parser/math.o: /usr/include/i386/_limits.h /usr/include/sys/syslimits.h
src/parser/math.o: /usr/include/c++/4.2.1/cstdlib
src/parser/math.o: /usr/include/c++/4.2.1/iosfwd
src/parser/math.o: /usr/include/c++/4.2.1/cctype /usr/include/ctype.h
src/parser/math.o: /usr/include/runetype.h
src/parser/math.o: /usr/include/c++/4.2.1/bits/stringfwd.h
src/parser/math.o: /usr/include/c++/4.2.1/bits/postypes.h
src/parser/math.o: /usr/include/c++/4.2.1/cwchar /usr/include/c++/4.2.1/ctime
src/parser/math.o: /usr/include/time.h /usr/include/_structs.h
src/parser/math.o: /usr/include/sys/_structs.h
src/parser/math.o: /usr/include/c++/4.2.1/bits/stl_pair.h
src/parser/math.o: /usr/include/c++/4.2.1/bits/cpp_type_traits.h
src/parser/math.o: /usr/include/c++/4.2.1/ext/type_traits.h
src/parser/math.o: /usr/include/c++/4.2.1/utility
src/parser/math.o: /usr/include/c++/4.2.1/bits/stl_relops.h
src/parser/math.o: /usr/include/c++/4.2.1/bits/stl_iterator_base_types.h
src/parser/math.o: /usr/include/c++/4.2.1/bits/stl_iterator_base_funcs.h
src/parser/math.o: /usr/include/c++/4.2.1/bits/concept_check.h
src/parser/math.o: /usr/include/c++/4.2.1/bits/stl_iterator.h
src/parser/math.o: /usr/include/c++/4.2.1/debug/debug.h
src/parser/math.o: /usr/include/c++/4.2.1/bits/allocator.h
src/parser/math.o: /usr/include/c++/4.2.1/bits/stl_construct.h
src/parser/math.o: /usr/include/c++/4.2.1/new
src/parser/math.o: /usr/include/c++/4.2.1/exception
src/parser/math.o: /usr/include/c++/4.2.1/bits/stl_uninitialized.h
src/parser/math.o: /usr/include/c++/4.2.1/bits/stl_deque.h
src/parser/math.o: /usr/include/c++/4.2.1/bits/deque.tcc
src/parser/math.o: /usr/include/c++/4.2.1/string
src/parser/math.o: /usr/include/c++/4.2.1/bits/char_traits.h
src/parser/math.o: /usr/include/c++/4.2.1/memory
src/parser/math.o: /usr/include/c++/4.2.1/bits/stl_raw_storage_iter.h
src/parser/math.o: /usr/include/c++/4.2.1/limits
src/parser/math.o: /usr/include/c++/4.2.1/bits/ostream_insert.h
src/parser/math.o: /usr/include/c++/4.2.1/bits/stl_function.h
src/parser/math.o: /usr/include/c++/4.2.1/bits/basic_string.h
src/parser/math.o: /usr/include/c++/4.2.1/ext/atomicity.h
src/parser/math.o: /usr/include/c++/4.2.1/algorithm
src/parser/math.o: /usr/include/c++/4.2.1/bits/stl_algo.h
src/parser/math.o: /usr/include/c++/4.2.1/bits/stl_heap.h
src/parser/math.o: /usr/include/c++/4.2.1/bits/stl_tempbuf.h
src/parser/math.o: /usr/include/c++/4.2.1/bits/basic_string.tcc
src/parser/math.o: src/semantics/constants.h
src/parser/method.o: src/parser/grammar.h src/semantics/syntax_tree.h
src/parser/method.o: /usr/include/c++/4.2.1/deque
src/parser/method.o: /usr/include/c++/4.2.1/bits/functexcept.h
src/parser/method.o: /usr/include/c++/4.2.1/exception_defines.h
src/parser/method.o: /usr/include/c++/4.2.1/bits/stl_algobase.h
src/parser/method.o: /usr/include/c++/4.2.1/cstring
src/parser/method.o: /usr/include/c++/4.2.1/cstddef /usr/include/stddef.h
src/parser/method.o: /usr/include/_types.h /usr/include/sys/_types.h
src/parser/method.o: /usr/include/sys/cdefs.h /usr/include/machine/_types.h
src/parser/method.o: /usr/include/i386/_types.h /usr/include/string.h
src/parser/method.o: /usr/include/secure/_string.h
src/parser/method.o: /usr/include/secure/_common.h
src/parser/method.o: /usr/include/c++/4.2.1/climits /usr/include/limits.h
src/parser/method.o: /usr/include/machine/limits.h /usr/include/i386/limits.h
src/parser/method.o: /usr/include/i386/_limits.h /usr/include/sys/syslimits.h
src/parser/method.o: /usr/include/c++/4.2.1/cstdlib
src/parser/method.o: /usr/include/c++/4.2.1/iosfwd
src/parser/method.o: /usr/include/c++/4.2.1/cctype /usr/include/ctype.h
src/parser/method.o: /usr/include/runetype.h
src/parser/method.o: /usr/include/c++/4.2.1/bits/stringfwd.h
src/parser/method.o: /usr/include/c++/4.2.1/bits/postypes.h
src/parser/method.o: /usr/include/c++/4.2.1/cwchar
src/parser/method.o: /usr/include/c++/4.2.1/ctime /usr/include/time.h
src/parser/method.o: /usr/include/_structs.h /usr/include/sys/_structs.h
src/parser/method.o: /usr/include/c++/4.2.1/bits/stl_pair.h
src/parser/method.o: /usr/include/c++/4.2.1/bits/cpp_type_traits.h
src/parser/method.o: /usr/include/c++/4.2.1/ext/type_traits.h
src/parser/method.o: /usr/include/c++/4.2.1/utility
src/parser/method.o: /usr/include/c++/4.2.1/bits/stl_relops.h
src/parser/method.o: /usr/include/c++/4.2.1/bits/stl_iterator_base_types.h
src/parser/method.o: /usr/include/c++/4.2.1/bits/stl_iterator_base_funcs.h
src/parser/method.o: /usr/include/c++/4.2.1/bits/concept_check.h
src/parser/method.o: /usr/include/c++/4.2.1/bits/stl_iterator.h
src/parser/method.o: /usr/include/c++/4.2.1/debug/debug.h
src/parser/method.o: /usr/include/c++/4.2.1/bits/allocator.h
src/parser/method.o: /usr/include/c++/4.2.1/bits/stl_construct.h
src/parser/method.o: /usr/include/c++/4.2.1/new
src/parser/method.o: /usr/include/c++/4.2.1/exception
src/parser/method.o: /usr/include/c++/4.2.1/bits/stl_uninitialized.h
src/parser/method.o: /usr/include/c++/4.2.1/bits/stl_deque.h
src/parser/method.o: /usr/include/c++/4.2.1/bits/deque.tcc
src/parser/method.o: /usr/include/c++/4.2.1/string
src/parser/method.o: /usr/include/c++/4.2.1/bits/char_traits.h
src/parser/method.o: /usr/include/c++/4.2.1/memory
src/parser/method.o: /usr/include/c++/4.2.1/bits/stl_raw_storage_iter.h
src/parser/method.o: /usr/include/c++/4.2.1/limits
src/parser/method.o: /usr/include/c++/4.2.1/bits/ostream_insert.h
src/parser/method.o: /usr/include/c++/4.2.1/bits/stl_function.h
src/parser/method.o: /usr/include/c++/4.2.1/bits/basic_string.h
src/parser/method.o: /usr/include/c++/4.2.1/ext/atomicity.h
src/parser/method.o: /usr/include/c++/4.2.1/algorithm
src/parser/method.o: /usr/include/c++/4.2.1/bits/stl_algo.h
src/parser/method.o: /usr/include/c++/4.2.1/bits/stl_heap.h
src/parser/method.o: /usr/include/c++/4.2.1/bits/stl_tempbuf.h
src/parser/method.o: /usr/include/c++/4.2.1/bits/basic_string.tcc
src/parser/method.o: src/semantics/constants.h
src/parser/parser.o: src/parser/grammar.h src/semantics/syntax_tree.h
src/parser/parser.o: /usr/include/c++/4.2.1/deque
src/parser/parser.o: /usr/include/c++/4.2.1/bits/functexcept.h
src/parser/parser.o: /usr/include/c++/4.2.1/exception_defines.h
src/parser/parser.o: /usr/include/c++/4.2.1/bits/stl_algobase.h
src/parser/parser.o: /usr/include/c++/4.2.1/cstring
src/parser/parser.o: /usr/include/c++/4.2.1/cstddef /usr/include/stddef.h
src/parser/parser.o: /usr/include/_types.h /usr/include/sys/_types.h
src/parser/parser.o: /usr/include/sys/cdefs.h /usr/include/machine/_types.h
src/parser/parser.o: /usr/include/i386/_types.h /usr/include/string.h
src/parser/parser.o: /usr/include/secure/_string.h
src/parser/parser.o: /usr/include/secure/_common.h
src/parser/parser.o: /usr/include/c++/4.2.1/climits /usr/include/limits.h
src/parser/parser.o: /usr/include/machine/limits.h /usr/include/i386/limits.h
src/parser/parser.o: /usr/include/i386/_limits.h /usr/include/sys/syslimits.h
src/parser/parser.o: /usr/include/c++/4.2.1/cstdlib
src/parser/parser.o: /usr/include/c++/4.2.1/iosfwd
src/parser/parser.o: /usr/include/c++/4.2.1/cctype /usr/include/ctype.h
src/parser/parser.o: /usr/include/runetype.h
src/parser/parser.o: /usr/include/c++/4.2.1/bits/stringfwd.h
src/parser/parser.o: /usr/include/c++/4.2.1/bits/postypes.h
src/parser/parser.o: /usr/include/c++/4.2.1/cwchar
src/parser/parser.o: /usr/include/c++/4.2.1/ctime /usr/include/time.h
src/parser/parser.o: /usr/include/_structs.h /usr/include/sys/_structs.h
src/parser/parser.o: /usr/include/c++/4.2.1/bits/stl_pair.h
src/parser/parser.o: /usr/include/c++/4.2.1/bits/cpp_type_traits.h
src/parser/parser.o: /usr/include/c++/4.2.1/ext/type_traits.h
src/parser/parser.o: /usr/include/c++/4.2.1/utility
src/parser/parser.o: /usr/include/c++/4.2.1/bits/stl_relops.h
src/parser/parser.o: /usr/include/c++/4.2.1/bits/stl_iterator_base_types.h
src/parser/parser.o: /usr/include/c++/4.2.1/bits/stl_iterator_base_funcs.h
src/parser/parser.o: /usr/include/c++/4.2.1/bits/concept_check.h
src/parser/parser.o: /usr/include/c++/4.2.1/bits/stl_iterator.h
src/parser/parser.o: /usr/include/c++/4.2.1/debug/debug.h
src/parser/parser.o: /usr/include/c++/4.2.1/bits/allocator.h
src/parser/parser.o: /usr/include/c++/4.2.1/bits/stl_construct.h
src/parser/parser.o: /usr/include/c++/4.2.1/new
src/parser/parser.o: /usr/include/c++/4.2.1/exception
src/parser/parser.o: /usr/include/c++/4.2.1/bits/stl_uninitialized.h
src/parser/parser.o: /usr/include/c++/4.2.1/bits/stl_deque.h
src/parser/parser.o: /usr/include/c++/4.2.1/bits/deque.tcc
src/parser/parser.o: /usr/include/c++/4.2.1/string
src/parser/parser.o: /usr/include/c++/4.2.1/bits/char_traits.h
src/parser/parser.o: /usr/include/c++/4.2.1/memory
src/parser/parser.o: /usr/include/c++/4.2.1/bits/stl_raw_storage_iter.h
src/parser/parser.o: /usr/include/c++/4.2.1/limits
src/parser/parser.o: /usr/include/c++/4.2.1/bits/ostream_insert.h
src/parser/parser.o: /usr/include/c++/4.2.1/bits/stl_function.h
src/parser/parser.o: /usr/include/c++/4.2.1/bits/basic_string.h
src/parser/parser.o: /usr/include/c++/4.2.1/ext/atomicity.h
src/parser/parser.o: /usr/include/c++/4.2.1/algorithm
src/parser/parser.o: /usr/include/c++/4.2.1/bits/stl_algo.h
src/parser/parser.o: /usr/include/c++/4.2.1/bits/stl_heap.h
src/parser/parser.o: /usr/include/c++/4.2.1/bits/stl_tempbuf.h
src/parser/parser.o: /usr/include/c++/4.2.1/bits/basic_string.tcc
src/parser/parser.o: src/semantics/constants.h src/parser/parser.h
src/parser/statement.o: src/parser/grammar.h src/semantics/syntax_tree.h
src/parser/statement.o: /usr/include/c++/4.2.1/deque
src/parser/statement.o: /usr/include/c++/4.2.1/bits/functexcept.h
src/parser/statement.o: /usr/include/c++/4.2.1/exception_defines.h
src/parser/statement.o: /usr/include/c++/4.2.1/bits/stl_algobase.h
src/parser/statement.o: /usr/include/c++/4.2.1/cstring
src/parser/statement.o: /usr/include/c++/4.2.1/cstddef /usr/include/stddef.h
src/parser/statement.o: /usr/include/_types.h /usr/include/sys/_types.h
src/parser/statement.o: /usr/include/sys/cdefs.h
src/parser/statement.o: /usr/include/machine/_types.h
src/parser/statement.o: /usr/include/i386/_types.h /usr/include/string.h
src/parser/statement.o: /usr/include/secure/_string.h
src/parser/statement.o: /usr/include/secure/_common.h
src/parser/statement.o: /usr/include/c++/4.2.1/climits /usr/include/limits.h
src/parser/statement.o: /usr/include/machine/limits.h
src/parser/statement.o: /usr/include/i386/limits.h
src/parser/statement.o: /usr/include/i386/_limits.h
src/parser/statement.o: /usr/include/sys/syslimits.h
src/parser/statement.o: /usr/include/c++/4.2.1/cstdlib
src/parser/statement.o: /usr/include/c++/4.2.1/iosfwd
src/parser/statement.o: /usr/include/c++/4.2.1/cctype /usr/include/ctype.h
src/parser/statement.o: /usr/include/runetype.h
src/parser/statement.o: /usr/include/c++/4.2.1/bits/stringfwd.h
src/parser/statement.o: /usr/include/c++/4.2.1/bits/postypes.h
src/parser/statement.o: /usr/include/c++/4.2.1/cwchar
src/parser/statement.o: /usr/include/c++/4.2.1/ctime /usr/include/time.h
src/parser/statement.o: /usr/include/_structs.h /usr/include/sys/_structs.h
src/parser/statement.o: /usr/include/c++/4.2.1/bits/stl_pair.h
src/parser/statement.o: /usr/include/c++/4.2.1/bits/cpp_type_traits.h
src/parser/statement.o: /usr/include/c++/4.2.1/ext/type_traits.h
src/parser/statement.o: /usr/include/c++/4.2.1/utility
src/parser/statement.o: /usr/include/c++/4.2.1/bits/stl_relops.h
src/parser/statement.o: /usr/include/c++/4.2.1/bits/stl_iterator_base_types.h
src/parser/statement.o: /usr/include/c++/4.2.1/bits/stl_iterator_base_funcs.h
src/parser/statement.o: /usr/include/c++/4.2.1/bits/concept_check.h
src/parser/statement.o: /usr/include/c++/4.2.1/bits/stl_iterator.h
src/parser/statement.o: /usr/include/c++/4.2.1/debug/debug.h
src/parser/statement.o: /usr/include/c++/4.2.1/bits/allocator.h
src/parser/statement.o: /usr/include/c++/4.2.1/bits/stl_construct.h
src/parser/statement.o: /usr/include/c++/4.2.1/new
src/parser/statement.o: /usr/include/c++/4.2.1/exception
src/parser/statement.o: /usr/include/c++/4.2.1/bits/stl_uninitialized.h
src/parser/statement.o: /usr/include/c++/4.2.1/bits/stl_deque.h
src/parser/statement.o: /usr/include/c++/4.2.1/bits/deque.tcc
src/parser/statement.o: /usr/include/c++/4.2.1/string
src/parser/statement.o: /usr/include/c++/4.2.1/bits/char_traits.h
src/parser/statement.o: /usr/include/c++/4.2.1/memory
src/parser/statement.o: /usr/include/c++/4.2.1/bits/stl_raw_storage_iter.h
src/parser/statement.o: /usr/include/c++/4.2.1/limits
src/parser/statement.o: /usr/include/c++/4.2.1/bits/ostream_insert.h
src/parser/statement.o: /usr/include/c++/4.2.1/bits/stl_function.h
src/parser/statement.o: /usr/include/c++/4.2.1/bits/basic_string.h
src/parser/statement.o: /usr/include/c++/4.2.1/ext/atomicity.h
src/parser/statement.o: /usr/include/c++/4.2.1/algorithm
src/parser/statement.o: /usr/include/c++/4.2.1/bits/stl_algo.h
src/parser/statement.o: /usr/include/c++/4.2.1/bits/stl_heap.h
src/parser/statement.o: /usr/include/c++/4.2.1/bits/stl_tempbuf.h
src/parser/statement.o: /usr/include/c++/4.2.1/bits/basic_string.tcc
src/parser/statement.o: src/semantics/constants.h
src/stdlib/System/boolean.o: /usr/include/c++/4.2.1/iostream
src/stdlib/System/boolean.o: /usr/include/c++/4.2.1/ostream
src/stdlib/System/boolean.o: /usr/include/c++/4.2.1/ios
src/stdlib/System/boolean.o: /usr/include/c++/4.2.1/iosfwd
src/stdlib/System/boolean.o: /usr/include/c++/4.2.1/cctype
src/stdlib/System/boolean.o: /usr/include/ctype.h /usr/include/runetype.h
src/stdlib/System/boolean.o: /usr/include/_types.h /usr/include/sys/_types.h
src/stdlib/System/boolean.o: /usr/include/sys/cdefs.h
src/stdlib/System/boolean.o: /usr/include/machine/_types.h
src/stdlib/System/boolean.o: /usr/include/i386/_types.h
src/stdlib/System/boolean.o: /usr/include/c++/4.2.1/bits/stringfwd.h
src/stdlib/System/boolean.o: /usr/include/c++/4.2.1/bits/postypes.h
src/stdlib/System/boolean.o: /usr/include/c++/4.2.1/cwchar
src/stdlib/System/boolean.o: /usr/include/c++/4.2.1/cstddef
src/stdlib/System/boolean.o: /usr/include/stddef.h
src/stdlib/System/boolean.o: /usr/include/c++/4.2.1/ctime /usr/include/time.h
src/stdlib/System/boolean.o: /usr/include/_structs.h
src/stdlib/System/boolean.o: /usr/include/sys/_structs.h
src/stdlib/System/boolean.o: /usr/include/c++/4.2.1/bits/functexcept.h
src/stdlib/System/boolean.o: /usr/include/c++/4.2.1/exception_defines.h
src/stdlib/System/boolean.o: /usr/include/c++/4.2.1/exception
src/stdlib/System/boolean.o: /usr/include/c++/4.2.1/bits/char_traits.h
src/stdlib/System/boolean.o: /usr/include/c++/4.2.1/cstring
src/stdlib/System/boolean.o: /usr/include/string.h
src/stdlib/System/boolean.o: /usr/include/secure/_string.h
src/stdlib/System/boolean.o: /usr/include/secure/_common.h
src/stdlib/System/boolean.o: /usr/include/c++/4.2.1/bits/stl_algobase.h
src/stdlib/System/boolean.o: /usr/include/c++/4.2.1/climits
src/stdlib/System/boolean.o: /usr/include/limits.h
src/stdlib/System/boolean.o: /usr/include/machine/limits.h
src/stdlib/System/boolean.o: /usr/include/i386/limits.h
src/stdlib/System/boolean.o: /usr/include/i386/_limits.h
src/stdlib/System/boolean.o: /usr/include/sys/syslimits.h
src/stdlib/System/boolean.o: /usr/include/c++/4.2.1/cstdlib
src/stdlib/System/boolean.o: /usr/include/c++/4.2.1/bits/stl_pair.h
src/stdlib/System/boolean.o: /usr/include/c++/4.2.1/bits/cpp_type_traits.h
src/stdlib/System/boolean.o: /usr/include/c++/4.2.1/ext/type_traits.h
src/stdlib/System/boolean.o: /usr/include/c++/4.2.1/utility
src/stdlib/System/boolean.o: /usr/include/c++/4.2.1/bits/stl_relops.h
src/stdlib/System/boolean.o: /usr/include/c++/4.2.1/bits/stl_iterator_base_types.h
src/stdlib/System/boolean.o: /usr/include/c++/4.2.1/bits/stl_iterator_base_funcs.h
src/stdlib/System/boolean.o: /usr/include/c++/4.2.1/bits/concept_check.h
src/stdlib/System/boolean.o: /usr/include/c++/4.2.1/bits/stl_iterator.h
src/stdlib/System/boolean.o: /usr/include/c++/4.2.1/debug/debug.h
src/stdlib/System/boolean.o: /usr/include/c++/4.2.1/cstdio
src/stdlib/System/boolean.o: /usr/include/stdio.h
src/stdlib/System/boolean.o: /usr/include/secure/_stdio.h
src/stdlib/System/boolean.o: /usr/include/c++/4.2.1/bits/localefwd.h
src/stdlib/System/boolean.o: /usr/include/c++/4.2.1/bits/ios_base.h
src/stdlib/System/boolean.o: /usr/include/c++/4.2.1/ext/atomicity.h
src/stdlib/System/boolean.o: /usr/include/c++/4.2.1/bits/locale_classes.h
src/stdlib/System/boolean.o: /usr/include/c++/4.2.1/string
src/stdlib/System/boolean.o: /usr/include/c++/4.2.1/memory
src/stdlib/System/boolean.o: /usr/include/c++/4.2.1/bits/allocator.h
src/stdlib/System/boolean.o: /usr/include/c++/4.2.1/bits/stl_construct.h
src/stdlib/System/boolean.o: /usr/include/c++/4.2.1/new
src/stdlib/System/boolean.o: /usr/include/c++/4.2.1/bits/stl_uninitialized.h
src/stdlib/System/boolean.o: /usr/include/c++/4.2.1/bits/stl_raw_storage_iter.h
src/stdlib/System/boolean.o: /usr/include/c++/4.2.1/limits
src/stdlib/System/boolean.o: /usr/include/c++/4.2.1/bits/ostream_insert.h
src/stdlib/System/boolean.o: /usr/include/c++/4.2.1/bits/stl_function.h
src/stdlib/System/boolean.o: /usr/include/c++/4.2.1/bits/basic_string.h
src/stdlib/System/boolean.o: /usr/include/c++/4.2.1/algorithm
src/stdlib/System/boolean.o: /usr/include/c++/4.2.1/bits/stl_algo.h
src/stdlib/System/boolean.o: /usr/include/c++/4.2.1/bits/stl_heap.h
src/stdlib/System/boolean.o: /usr/include/c++/4.2.1/bits/stl_tempbuf.h
src/stdlib/System/boolean.o: /usr/include/c++/4.2.1/bits/basic_string.tcc
src/stdlib/System/boolean.o: /usr/include/c++/4.2.1/streambuf
src/stdlib/System/boolean.o: /usr/include/c++/4.2.1/bits/streambuf.tcc
src/stdlib/System/boolean.o: /usr/include/c++/4.2.1/bits/basic_ios.h
src/stdlib/System/boolean.o: /usr/include/c++/4.2.1/bits/streambuf_iterator.h
src/stdlib/System/boolean.o: /usr/include/c++/4.2.1/bits/locale_facets.h
src/stdlib/System/boolean.o: /usr/include/c++/4.2.1/cwctype
src/stdlib/System/boolean.o: /usr/include/c++/4.2.1/bits/codecvt.h
src/stdlib/System/boolean.o: /usr/include/c++/4.2.1/bits/basic_ios.tcc
src/stdlib/System/boolean.o: /usr/include/c++/4.2.1/bits/ostream.tcc
src/stdlib/System/boolean.o: /usr/include/c++/4.2.1/locale
src/stdlib/System/boolean.o: /usr/include/c++/4.2.1/bits/locale_facets.tcc
src/stdlib/System/boolean.o: /usr/include/c++/4.2.1/typeinfo
src/stdlib/System/boolean.o: /usr/include/c++/4.2.1/istream
src/stdlib/System/boolean.o: /usr/include/c++/4.2.1/bits/istream.tcc
src/stdlib/System/boolean.o: src/stdlib/System/boolean.h
src/stdlib/System/boolean.o: src/stdlib/System/object.h
src/stdlib/System/boolean.o: /usr/include/c++/4.2.1/map
src/stdlib/System/boolean.o: /usr/include/c++/4.2.1/bits/stl_tree.h
src/stdlib/System/boolean.o: /usr/include/c++/4.2.1/bits/stl_map.h
src/stdlib/System/boolean.o: /usr/include/c++/4.2.1/bits/stl_multimap.h
src/stdlib/System/boolean.o: src/semantics/constants.h
src/stdlib/System/float.o: /usr/include/c++/4.2.1/iostream
src/stdlib/System/float.o: /usr/include/c++/4.2.1/ostream
src/stdlib/System/float.o: /usr/include/c++/4.2.1/ios
src/stdlib/System/float.o: /usr/include/c++/4.2.1/iosfwd
src/stdlib/System/float.o: /usr/include/c++/4.2.1/cctype /usr/include/ctype.h
src/stdlib/System/float.o: /usr/include/runetype.h /usr/include/_types.h
src/stdlib/System/float.o: /usr/include/sys/_types.h /usr/include/sys/cdefs.h
src/stdlib/System/float.o: /usr/include/machine/_types.h
src/stdlib/System/float.o: /usr/include/i386/_types.h
src/stdlib/System/float.o: /usr/include/c++/4.2.1/bits/stringfwd.h
src/stdlib/System/float.o: /usr/include/c++/4.2.1/bits/postypes.h
src/stdlib/System/float.o: /usr/include/c++/4.2.1/cwchar
src/stdlib/System/float.o: /usr/include/c++/4.2.1/cstddef
src/stdlib/System/float.o: /usr/include/stddef.h /usr/include/c++/4.2.1/ctime
src/stdlib/System/float.o: /usr/include/time.h /usr/include/_structs.h
src/stdlib/System/float.o: /usr/include/sys/_structs.h
src/stdlib/System/float.o: /usr/include/c++/4.2.1/bits/functexcept.h
src/stdlib/System/float.o: /usr/include/c++/4.2.1/exception_defines.h
src/stdlib/System/float.o: /usr/include/c++/4.2.1/exception
src/stdlib/System/float.o: /usr/include/c++/4.2.1/bits/char_traits.h
src/stdlib/System/float.o: /usr/include/c++/4.2.1/cstring
src/stdlib/System/float.o: /usr/include/string.h
src/stdlib/System/float.o: /usr/include/secure/_string.h
src/stdlib/System/float.o: /usr/include/secure/_common.h
src/stdlib/System/float.o: /usr/include/c++/4.2.1/bits/stl_algobase.h
src/stdlib/System/float.o: /usr/include/c++/4.2.1/climits
src/stdlib/System/float.o: /usr/include/limits.h
src/stdlib/System/float.o: /usr/include/machine/limits.h
src/stdlib/System/float.o: /usr/include/i386/limits.h
src/stdlib/System/float.o: /usr/include/i386/_limits.h
src/stdlib/System/float.o: /usr/include/sys/syslimits.h
src/stdlib/System/float.o: /usr/include/c++/4.2.1/cstdlib
src/stdlib/System/float.o: /usr/include/c++/4.2.1/bits/stl_pair.h
src/stdlib/System/float.o: /usr/include/c++/4.2.1/bits/cpp_type_traits.h
src/stdlib/System/float.o: /usr/include/c++/4.2.1/ext/type_traits.h
src/stdlib/System/float.o: /usr/include/c++/4.2.1/utility
src/stdlib/System/float.o: /usr/include/c++/4.2.1/bits/stl_relops.h
src/stdlib/System/float.o: /usr/include/c++/4.2.1/bits/stl_iterator_base_types.h
src/stdlib/System/float.o: /usr/include/c++/4.2.1/bits/stl_iterator_base_funcs.h
src/stdlib/System/float.o: /usr/include/c++/4.2.1/bits/concept_check.h
src/stdlib/System/float.o: /usr/include/c++/4.2.1/bits/stl_iterator.h
src/stdlib/System/float.o: /usr/include/c++/4.2.1/debug/debug.h
src/stdlib/System/float.o: /usr/include/c++/4.2.1/cstdio /usr/include/stdio.h
src/stdlib/System/float.o: /usr/include/secure/_stdio.h
src/stdlib/System/float.o: /usr/include/c++/4.2.1/bits/localefwd.h
src/stdlib/System/float.o: /usr/include/c++/4.2.1/bits/ios_base.h
src/stdlib/System/float.o: /usr/include/c++/4.2.1/ext/atomicity.h
src/stdlib/System/float.o: /usr/include/c++/4.2.1/bits/locale_classes.h
src/stdlib/System/float.o: /usr/include/c++/4.2.1/string
src/stdlib/System/float.o: /usr/include/c++/4.2.1/memory
src/stdlib/System/float.o: /usr/include/c++/4.2.1/bits/allocator.h
src/stdlib/System/float.o: /usr/include/c++/4.2.1/bits/stl_construct.h
src/stdlib/System/float.o: /usr/include/c++/4.2.1/new
src/stdlib/System/float.o: /usr/include/c++/4.2.1/bits/stl_uninitialized.h
src/stdlib/System/float.o: /usr/include/c++/4.2.1/bits/stl_raw_storage_iter.h
src/stdlib/System/float.o: /usr/include/c++/4.2.1/limits
src/stdlib/System/float.o: /usr/include/c++/4.2.1/bits/ostream_insert.h
src/stdlib/System/float.o: /usr/include/c++/4.2.1/bits/stl_function.h
src/stdlib/System/float.o: /usr/include/c++/4.2.1/bits/basic_string.h
src/stdlib/System/float.o: /usr/include/c++/4.2.1/algorithm
src/stdlib/System/float.o: /usr/include/c++/4.2.1/bits/stl_algo.h
src/stdlib/System/float.o: /usr/include/c++/4.2.1/bits/stl_heap.h
src/stdlib/System/float.o: /usr/include/c++/4.2.1/bits/stl_tempbuf.h
src/stdlib/System/float.o: /usr/include/c++/4.2.1/bits/basic_string.tcc
src/stdlib/System/float.o: /usr/include/c++/4.2.1/streambuf
src/stdlib/System/float.o: /usr/include/c++/4.2.1/bits/streambuf.tcc
src/stdlib/System/float.o: /usr/include/c++/4.2.1/bits/basic_ios.h
src/stdlib/System/float.o: /usr/include/c++/4.2.1/bits/streambuf_iterator.h
src/stdlib/System/float.o: /usr/include/c++/4.2.1/bits/locale_facets.h
src/stdlib/System/float.o: /usr/include/c++/4.2.1/cwctype
src/stdlib/System/float.o: /usr/include/c++/4.2.1/bits/codecvt.h
src/stdlib/System/float.o: /usr/include/c++/4.2.1/bits/basic_ios.tcc
src/stdlib/System/float.o: /usr/include/c++/4.2.1/bits/ostream.tcc
src/stdlib/System/float.o: /usr/include/c++/4.2.1/locale
src/stdlib/System/float.o: /usr/include/c++/4.2.1/bits/locale_facets.tcc
src/stdlib/System/float.o: /usr/include/c++/4.2.1/typeinfo
src/stdlib/System/float.o: /usr/include/c++/4.2.1/istream
src/stdlib/System/float.o: /usr/include/c++/4.2.1/bits/istream.tcc
src/stdlib/System/float.o: src/stdlib/System/float.h
src/stdlib/System/float.o: src/stdlib/System/object.h
src/stdlib/System/float.o: /usr/include/c++/4.2.1/map
src/stdlib/System/float.o: /usr/include/c++/4.2.1/bits/stl_tree.h
src/stdlib/System/float.o: /usr/include/c++/4.2.1/bits/stl_map.h
src/stdlib/System/float.o: /usr/include/c++/4.2.1/bits/stl_multimap.h
src/stdlib/System/float.o: src/semantics/constants.h
src/stdlib/System/integer.o: /usr/include/c++/4.2.1/iostream
src/stdlib/System/integer.o: /usr/include/c++/4.2.1/ostream
src/stdlib/System/integer.o: /usr/include/c++/4.2.1/ios
src/stdlib/System/integer.o: /usr/include/c++/4.2.1/iosfwd
src/stdlib/System/integer.o: /usr/include/c++/4.2.1/cctype
src/stdlib/System/integer.o: /usr/include/ctype.h /usr/include/runetype.h
src/stdlib/System/integer.o: /usr/include/_types.h /usr/include/sys/_types.h
src/stdlib/System/integer.o: /usr/include/sys/cdefs.h
src/stdlib/System/integer.o: /usr/include/machine/_types.h
src/stdlib/System/integer.o: /usr/include/i386/_types.h
src/stdlib/System/integer.o: /usr/include/c++/4.2.1/bits/stringfwd.h
src/stdlib/System/integer.o: /usr/include/c++/4.2.1/bits/postypes.h
src/stdlib/System/integer.o: /usr/include/c++/4.2.1/cwchar
src/stdlib/System/integer.o: /usr/include/c++/4.2.1/cstddef
src/stdlib/System/integer.o: /usr/include/stddef.h
src/stdlib/System/integer.o: /usr/include/c++/4.2.1/ctime /usr/include/time.h
src/stdlib/System/integer.o: /usr/include/_structs.h
src/stdlib/System/integer.o: /usr/include/sys/_structs.h
src/stdlib/System/integer.o: /usr/include/c++/4.2.1/bits/functexcept.h
src/stdlib/System/integer.o: /usr/include/c++/4.2.1/exception_defines.h
src/stdlib/System/integer.o: /usr/include/c++/4.2.1/exception
src/stdlib/System/integer.o: /usr/include/c++/4.2.1/bits/char_traits.h
src/stdlib/System/integer.o: /usr/include/c++/4.2.1/cstring
src/stdlib/System/integer.o: /usr/include/string.h
src/stdlib/System/integer.o: /usr/include/secure/_string.h
src/stdlib/System/integer.o: /usr/include/secure/_common.h
src/stdlib/System/integer.o: /usr/include/c++/4.2.1/bits/stl_algobase.h
src/stdlib/System/integer.o: /usr/include/c++/4.2.1/climits
src/stdlib/System/integer.o: /usr/include/limits.h
src/stdlib/System/integer.o: /usr/include/machine/limits.h
src/stdlib/System/integer.o: /usr/include/i386/limits.h
src/stdlib/System/integer.o: /usr/include/i386/_limits.h
src/stdlib/System/integer.o: /usr/include/sys/syslimits.h
src/stdlib/System/integer.o: /usr/include/c++/4.2.1/cstdlib
src/stdlib/System/integer.o: /usr/include/c++/4.2.1/bits/stl_pair.h
src/stdlib/System/integer.o: /usr/include/c++/4.2.1/bits/cpp_type_traits.h
src/stdlib/System/integer.o: /usr/include/c++/4.2.1/ext/type_traits.h
src/stdlib/System/integer.o: /usr/include/c++/4.2.1/utility
src/stdlib/System/integer.o: /usr/include/c++/4.2.1/bits/stl_relops.h
src/stdlib/System/integer.o: /usr/include/c++/4.2.1/bits/stl_iterator_base_types.h
src/stdlib/System/integer.o: /usr/include/c++/4.2.1/bits/stl_iterator_base_funcs.h
src/stdlib/System/integer.o: /usr/include/c++/4.2.1/bits/concept_check.h
src/stdlib/System/integer.o: /usr/include/c++/4.2.1/bits/stl_iterator.h
src/stdlib/System/integer.o: /usr/include/c++/4.2.1/debug/debug.h
src/stdlib/System/integer.o: /usr/include/c++/4.2.1/cstdio
src/stdlib/System/integer.o: /usr/include/stdio.h
src/stdlib/System/integer.o: /usr/include/secure/_stdio.h
src/stdlib/System/integer.o: /usr/include/c++/4.2.1/bits/localefwd.h
src/stdlib/System/integer.o: /usr/include/c++/4.2.1/bits/ios_base.h
src/stdlib/System/integer.o: /usr/include/c++/4.2.1/ext/atomicity.h
src/stdlib/System/integer.o: /usr/include/c++/4.2.1/bits/locale_classes.h
src/stdlib/System/integer.o: /usr/include/c++/4.2.1/string
src/stdlib/System/integer.o: /usr/include/c++/4.2.1/memory
src/stdlib/System/integer.o: /usr/include/c++/4.2.1/bits/allocator.h
src/stdlib/System/integer.o: /usr/include/c++/4.2.1/bits/stl_construct.h
src/stdlib/System/integer.o: /usr/include/c++/4.2.1/new
src/stdlib/System/integer.o: /usr/include/c++/4.2.1/bits/stl_uninitialized.h
src/stdlib/System/integer.o: /usr/include/c++/4.2.1/bits/stl_raw_storage_iter.h
src/stdlib/System/integer.o: /usr/include/c++/4.2.1/limits
src/stdlib/System/integer.o: /usr/include/c++/4.2.1/bits/ostream_insert.h
src/stdlib/System/integer.o: /usr/include/c++/4.2.1/bits/stl_function.h
src/stdlib/System/integer.o: /usr/include/c++/4.2.1/bits/basic_string.h
src/stdlib/System/integer.o: /usr/include/c++/4.2.1/algorithm
src/stdlib/System/integer.o: /usr/include/c++/4.2.1/bits/stl_algo.h
src/stdlib/System/integer.o: /usr/include/c++/4.2.1/bits/stl_heap.h
src/stdlib/System/integer.o: /usr/include/c++/4.2.1/bits/stl_tempbuf.h
src/stdlib/System/integer.o: /usr/include/c++/4.2.1/bits/basic_string.tcc
src/stdlib/System/integer.o: /usr/include/c++/4.2.1/streambuf
src/stdlib/System/integer.o: /usr/include/c++/4.2.1/bits/streambuf.tcc
src/stdlib/System/integer.o: /usr/include/c++/4.2.1/bits/basic_ios.h
src/stdlib/System/integer.o: /usr/include/c++/4.2.1/bits/streambuf_iterator.h
src/stdlib/System/integer.o: /usr/include/c++/4.2.1/bits/locale_facets.h
src/stdlib/System/integer.o: /usr/include/c++/4.2.1/cwctype
src/stdlib/System/integer.o: /usr/include/c++/4.2.1/bits/codecvt.h
src/stdlib/System/integer.o: /usr/include/c++/4.2.1/bits/basic_ios.tcc
src/stdlib/System/integer.o: /usr/include/c++/4.2.1/bits/ostream.tcc
src/stdlib/System/integer.o: /usr/include/c++/4.2.1/locale
src/stdlib/System/integer.o: /usr/include/c++/4.2.1/bits/locale_facets.tcc
src/stdlib/System/integer.o: /usr/include/c++/4.2.1/typeinfo
src/stdlib/System/integer.o: /usr/include/c++/4.2.1/istream
src/stdlib/System/integer.o: /usr/include/c++/4.2.1/bits/istream.tcc
src/stdlib/System/integer.o: src/stdlib/System/integer.h
src/stdlib/System/integer.o: src/stdlib/System/object.h
src/stdlib/System/integer.o: /usr/include/c++/4.2.1/map
src/stdlib/System/integer.o: /usr/include/c++/4.2.1/bits/stl_tree.h
src/stdlib/System/integer.o: /usr/include/c++/4.2.1/bits/stl_map.h
src/stdlib/System/integer.o: /usr/include/c++/4.2.1/bits/stl_multimap.h
src/stdlib/System/integer.o: src/semantics/constants.h
src/stdlib/System/object.o: /usr/include/c++/4.2.1/iostream
src/stdlib/System/object.o: /usr/include/c++/4.2.1/ostream
src/stdlib/System/object.o: /usr/include/c++/4.2.1/ios
src/stdlib/System/object.o: /usr/include/c++/4.2.1/iosfwd
src/stdlib/System/object.o: /usr/include/c++/4.2.1/cctype
src/stdlib/System/object.o: /usr/include/ctype.h /usr/include/runetype.h
src/stdlib/System/object.o: /usr/include/_types.h /usr/include/sys/_types.h
src/stdlib/System/object.o: /usr/include/sys/cdefs.h
src/stdlib/System/object.o: /usr/include/machine/_types.h
src/stdlib/System/object.o: /usr/include/i386/_types.h
src/stdlib/System/object.o: /usr/include/c++/4.2.1/bits/stringfwd.h
src/stdlib/System/object.o: /usr/include/c++/4.2.1/bits/postypes.h
src/stdlib/System/object.o: /usr/include/c++/4.2.1/cwchar
src/stdlib/System/object.o: /usr/include/c++/4.2.1/cstddef
src/stdlib/System/object.o: /usr/include/stddef.h
src/stdlib/System/object.o: /usr/include/c++/4.2.1/ctime /usr/include/time.h
src/stdlib/System/object.o: /usr/include/_structs.h
src/stdlib/System/object.o: /usr/include/sys/_structs.h
src/stdlib/System/object.o: /usr/include/c++/4.2.1/bits/functexcept.h
src/stdlib/System/object.o: /usr/include/c++/4.2.1/exception_defines.h
src/stdlib/System/object.o: /usr/include/c++/4.2.1/exception
src/stdlib/System/object.o: /usr/include/c++/4.2.1/bits/char_traits.h
src/stdlib/System/object.o: /usr/include/c++/4.2.1/cstring
src/stdlib/System/object.o: /usr/include/string.h
src/stdlib/System/object.o: /usr/include/secure/_string.h
src/stdlib/System/object.o: /usr/include/secure/_common.h
src/stdlib/System/object.o: /usr/include/c++/4.2.1/bits/stl_algobase.h
src/stdlib/System/object.o: /usr/include/c++/4.2.1/climits
src/stdlib/System/object.o: /usr/include/limits.h
src/stdlib/System/object.o: /usr/include/machine/limits.h
src/stdlib/System/object.o: /usr/include/i386/limits.h
src/stdlib/System/object.o: /usr/include/i386/_limits.h
src/stdlib/System/object.o: /usr/include/sys/syslimits.h
src/stdlib/System/object.o: /usr/include/c++/4.2.1/cstdlib
src/stdlib/System/object.o: /usr/include/c++/4.2.1/bits/stl_pair.h
src/stdlib/System/object.o: /usr/include/c++/4.2.1/bits/cpp_type_traits.h
src/stdlib/System/object.o: /usr/include/c++/4.2.1/ext/type_traits.h
src/stdlib/System/object.o: /usr/include/c++/4.2.1/utility
src/stdlib/System/object.o: /usr/include/c++/4.2.1/bits/stl_relops.h
src/stdlib/System/object.o: /usr/include/c++/4.2.1/bits/stl_iterator_base_types.h
src/stdlib/System/object.o: /usr/include/c++/4.2.1/bits/stl_iterator_base_funcs.h
src/stdlib/System/object.o: /usr/include/c++/4.2.1/bits/concept_check.h
src/stdlib/System/object.o: /usr/include/c++/4.2.1/bits/stl_iterator.h
src/stdlib/System/object.o: /usr/include/c++/4.2.1/debug/debug.h
src/stdlib/System/object.o: /usr/include/c++/4.2.1/cstdio
src/stdlib/System/object.o: /usr/include/stdio.h /usr/include/secure/_stdio.h
src/stdlib/System/object.o: /usr/include/c++/4.2.1/bits/localefwd.h
src/stdlib/System/object.o: /usr/include/c++/4.2.1/bits/ios_base.h
src/stdlib/System/object.o: /usr/include/c++/4.2.1/ext/atomicity.h
src/stdlib/System/object.o: /usr/include/c++/4.2.1/bits/locale_classes.h
src/stdlib/System/object.o: /usr/include/c++/4.2.1/string
src/stdlib/System/object.o: /usr/include/c++/4.2.1/memory
src/stdlib/System/object.o: /usr/include/c++/4.2.1/bits/allocator.h
src/stdlib/System/object.o: /usr/include/c++/4.2.1/bits/stl_construct.h
src/stdlib/System/object.o: /usr/include/c++/4.2.1/new
src/stdlib/System/object.o: /usr/include/c++/4.2.1/bits/stl_uninitialized.h
src/stdlib/System/object.o: /usr/include/c++/4.2.1/bits/stl_raw_storage_iter.h
src/stdlib/System/object.o: /usr/include/c++/4.2.1/limits
src/stdlib/System/object.o: /usr/include/c++/4.2.1/bits/ostream_insert.h
src/stdlib/System/object.o: /usr/include/c++/4.2.1/bits/stl_function.h
src/stdlib/System/object.o: /usr/include/c++/4.2.1/bits/basic_string.h
src/stdlib/System/object.o: /usr/include/c++/4.2.1/algorithm
src/stdlib/System/object.o: /usr/include/c++/4.2.1/bits/stl_algo.h
src/stdlib/System/object.o: /usr/include/c++/4.2.1/bits/stl_heap.h
src/stdlib/System/object.o: /usr/include/c++/4.2.1/bits/stl_tempbuf.h
src/stdlib/System/object.o: /usr/include/c++/4.2.1/bits/basic_string.tcc
src/stdlib/System/object.o: /usr/include/c++/4.2.1/streambuf
src/stdlib/System/object.o: /usr/include/c++/4.2.1/bits/streambuf.tcc
src/stdlib/System/object.o: /usr/include/c++/4.2.1/bits/basic_ios.h
src/stdlib/System/object.o: /usr/include/c++/4.2.1/bits/streambuf_iterator.h
src/stdlib/System/object.o: /usr/include/c++/4.2.1/bits/locale_facets.h
src/stdlib/System/object.o: /usr/include/c++/4.2.1/cwctype
src/stdlib/System/object.o: /usr/include/c++/4.2.1/bits/codecvt.h
src/stdlib/System/object.o: /usr/include/c++/4.2.1/bits/basic_ios.tcc
src/stdlib/System/object.o: /usr/include/c++/4.2.1/bits/ostream.tcc
src/stdlib/System/object.o: /usr/include/c++/4.2.1/locale
src/stdlib/System/object.o: /usr/include/c++/4.2.1/bits/locale_facets.tcc
src/stdlib/System/object.o: /usr/include/c++/4.2.1/typeinfo
src/stdlib/System/object.o: /usr/include/c++/4.2.1/istream
src/stdlib/System/object.o: /usr/include/c++/4.2.1/bits/istream.tcc
src/stdlib/System/object.o: src/stdlib/System/object.h
src/stdlib/System/object.o: /usr/include/c++/4.2.1/map
src/stdlib/System/object.o: /usr/include/c++/4.2.1/bits/stl_tree.h
src/stdlib/System/object.o: /usr/include/c++/4.2.1/bits/stl_map.h
src/stdlib/System/object.o: /usr/include/c++/4.2.1/bits/stl_multimap.h
src/stdlib/System/object.o: src/semantics/constants.h
src/stdlib/System/object.o: src/stdlib/System/integer.h
src/stdlib/System/object.o: src/stdlib/System/float.h
src/stdlib/System/object.o: src/stdlib/System/boolean.h
src/stdlib/System/object.o: src/stdlib/System/method.h
src/stdlib/System/object.o: src/stdlib/System/dynamic_object.h
src/stdlib/System/string.o: /usr/include/c++/4.2.1/iostream
src/stdlib/System/string.o: /usr/include/c++/4.2.1/ostream
src/stdlib/System/string.o: /usr/include/c++/4.2.1/ios
src/stdlib/System/string.o: /usr/include/c++/4.2.1/iosfwd
src/stdlib/System/string.o: /usr/include/c++/4.2.1/cctype
src/stdlib/System/string.o: /usr/include/ctype.h /usr/include/runetype.h
src/stdlib/System/string.o: /usr/include/_types.h /usr/include/sys/_types.h
src/stdlib/System/string.o: /usr/include/sys/cdefs.h
src/stdlib/System/string.o: /usr/include/machine/_types.h
src/stdlib/System/string.o: /usr/include/i386/_types.h
src/stdlib/System/string.o: /usr/include/c++/4.2.1/bits/stringfwd.h
src/stdlib/System/string.o: /usr/include/c++/4.2.1/bits/postypes.h
src/stdlib/System/string.o: /usr/include/c++/4.2.1/cwchar
src/stdlib/System/string.o: /usr/include/c++/4.2.1/cstddef
src/stdlib/System/string.o: /usr/include/stddef.h
src/stdlib/System/string.o: /usr/include/c++/4.2.1/ctime /usr/include/time.h
src/stdlib/System/string.o: /usr/include/_structs.h
src/stdlib/System/string.o: /usr/include/sys/_structs.h
src/stdlib/System/string.o: /usr/include/c++/4.2.1/bits/functexcept.h
src/stdlib/System/string.o: /usr/include/c++/4.2.1/exception_defines.h
src/stdlib/System/string.o: /usr/include/c++/4.2.1/exception
src/stdlib/System/string.o: /usr/include/c++/4.2.1/bits/char_traits.h
src/stdlib/System/string.o: /usr/include/c++/4.2.1/cstring
src/stdlib/System/string.o: /usr/include/string.h
src/stdlib/System/string.o: /usr/include/secure/_string.h
src/stdlib/System/string.o: /usr/include/secure/_common.h
src/stdlib/System/string.o: /usr/include/c++/4.2.1/bits/stl_algobase.h
src/stdlib/System/string.o: /usr/include/c++/4.2.1/climits
src/stdlib/System/string.o: /usr/include/limits.h
src/stdlib/System/string.o: /usr/include/machine/limits.h
src/stdlib/System/string.o: /usr/include/i386/limits.h
src/stdlib/System/string.o: /usr/include/i386/_limits.h
src/stdlib/System/string.o: /usr/include/sys/syslimits.h
src/stdlib/System/string.o: /usr/include/c++/4.2.1/cstdlib
src/stdlib/System/string.o: /usr/include/c++/4.2.1/bits/stl_pair.h
src/stdlib/System/string.o: /usr/include/c++/4.2.1/bits/cpp_type_traits.h
src/stdlib/System/string.o: /usr/include/c++/4.2.1/ext/type_traits.h
src/stdlib/System/string.o: /usr/include/c++/4.2.1/utility
src/stdlib/System/string.o: /usr/include/c++/4.2.1/bits/stl_relops.h
src/stdlib/System/string.o: /usr/include/c++/4.2.1/bits/stl_iterator_base_types.h
src/stdlib/System/string.o: /usr/include/c++/4.2.1/bits/stl_iterator_base_funcs.h
src/stdlib/System/string.o: /usr/include/c++/4.2.1/bits/concept_check.h
src/stdlib/System/string.o: /usr/include/c++/4.2.1/bits/stl_iterator.h
src/stdlib/System/string.o: /usr/include/c++/4.2.1/debug/debug.h
src/stdlib/System/string.o: /usr/include/c++/4.2.1/cstdio
src/stdlib/System/string.o: /usr/include/stdio.h /usr/include/secure/_stdio.h
src/stdlib/System/string.o: /usr/include/c++/4.2.1/bits/localefwd.h
src/stdlib/System/string.o: /usr/include/c++/4.2.1/bits/ios_base.h
src/stdlib/System/string.o: /usr/include/c++/4.2.1/ext/atomicity.h
src/stdlib/System/string.o: /usr/include/c++/4.2.1/bits/locale_classes.h
src/stdlib/System/string.o: /usr/include/c++/4.2.1/string
src/stdlib/System/string.o: /usr/include/c++/4.2.1/memory
src/stdlib/System/string.o: /usr/include/c++/4.2.1/bits/allocator.h
src/stdlib/System/string.o: /usr/include/c++/4.2.1/bits/stl_construct.h
src/stdlib/System/string.o: /usr/include/c++/4.2.1/new
src/stdlib/System/string.o: /usr/include/c++/4.2.1/bits/stl_uninitialized.h
src/stdlib/System/string.o: /usr/include/c++/4.2.1/bits/stl_raw_storage_iter.h
src/stdlib/System/string.o: /usr/include/c++/4.2.1/limits
src/stdlib/System/string.o: /usr/include/c++/4.2.1/bits/ostream_insert.h
src/stdlib/System/string.o: /usr/include/c++/4.2.1/bits/stl_function.h
src/stdlib/System/string.o: /usr/include/c++/4.2.1/bits/basic_string.h
src/stdlib/System/string.o: /usr/include/c++/4.2.1/algorithm
src/stdlib/System/string.o: /usr/include/c++/4.2.1/bits/stl_algo.h
src/stdlib/System/string.o: /usr/include/c++/4.2.1/bits/stl_heap.h
src/stdlib/System/string.o: /usr/include/c++/4.2.1/bits/stl_tempbuf.h
src/stdlib/System/string.o: /usr/include/c++/4.2.1/bits/basic_string.tcc
src/stdlib/System/string.o: /usr/include/c++/4.2.1/streambuf
src/stdlib/System/string.o: /usr/include/c++/4.2.1/bits/streambuf.tcc
src/stdlib/System/string.o: /usr/include/c++/4.2.1/bits/basic_ios.h
src/stdlib/System/string.o: /usr/include/c++/4.2.1/bits/streambuf_iterator.h
src/stdlib/System/string.o: /usr/include/c++/4.2.1/bits/locale_facets.h
src/stdlib/System/string.o: /usr/include/c++/4.2.1/cwctype
src/stdlib/System/string.o: /usr/include/c++/4.2.1/bits/codecvt.h
src/stdlib/System/string.o: /usr/include/c++/4.2.1/bits/basic_ios.tcc
src/stdlib/System/string.o: /usr/include/c++/4.2.1/bits/ostream.tcc
src/stdlib/System/string.o: /usr/include/c++/4.2.1/locale
src/stdlib/System/string.o: /usr/include/c++/4.2.1/bits/locale_facets.tcc
src/stdlib/System/string.o: /usr/include/c++/4.2.1/typeinfo
src/stdlib/System/string.o: /usr/include/c++/4.2.1/istream
src/stdlib/System/string.o: /usr/include/c++/4.2.1/bits/istream.tcc
