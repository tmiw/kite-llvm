CC=g++
CPPFLAGS=-g
#-O2
LDFLAGS=-rdynamic
INC=-Isrc/ -I/opt/local/include
LLVM_CONFIG=llvm-config
LLVM_CPPFLAGS=`$(LLVM_CONFIG) --cppflags`
LLVM_LDFLAGS=`$(LLVM_CONFIG) --ldflags`
LLVM_LIBS=`$(LLVM_CONFIG) --libs` 
GC_LIBS=-lgc
GC_LDFLAGS=-L/usr/lib
MATH_LIBS=-lm
COMMON_OBJS=src/codegen/llvm_compile_state.o src/codegen/llvm_node_codegen.o \
	 src/codegen/syntax_tree_node_printer.o src/codegen/syntax_tree_printer.o \
	 src/stdlib/System.o src/stdlib/System/integer.o src/stdlib/System/string.o \
	 src/stdlib/System/boolean.o src/stdlib/System/float.o src/stdlib/System/object.o \
	 src/stdlib/System/dynamic_object.o src/stdlib/System/list.o src/stdlib/System/method.o \
	 src/stdlib/System/collections.o src/stdlib/System/collections/array.o \
	 src/stdlib/System/collections/queue.o src/stdlib/System/collections/stack.o \
	 src/stdlib/System/collections/binary_tree.o src/stdlib/System/date.o \
	 src/stdlib/language/kite.o src/stdlib/language/kite/syntax_tree.o \
	 src/stdlib/System/exceptions.o src/stdlib/System/exceptions/exception.o \
	 src/stdlib/System/exceptions/NotImplemented.o src/stdlib/System/exceptions/InvalidArgument.o \
	 src/stdlib/System/exceptions/TypeMismatch.o src/stdlib/System/exceptions/DivideByZero.o \
	 src/stdlib/System/exceptions/NullReference.o src/stdlib/System/exceptions/EmptyCollection.o \
	 src/parser/constants.o src/parser/make.o src/parser/assignment.o src/parser/bitwise.o \
	 src/parser/comparison.o src/parser/math.o src/parser/map_reduce.o src/parser/classes.o  \
	 src/parser/decide.o src/parser/deref.o src/parser/grouping.o \
	 src/parser/loop.o src/parser/method.o src/stdlib/api.o \
	 src/parser/statement.o src/parser/exceptions.o src/parser/constructor.o \
	 src/parser/destructor.o src/parser/parser.o src/semantics/constants.o

KITE_OBJS=src/apps/kite.o $(COMMON_OBJS)
IKT_OBJS=src/apps/ikt.o $(COMMON_OBJS)

.cpp.o: %.cpp
	$(CC) -c -o $@ $(CPPFLAGS) $(INC) $(LLVM_CPPFLAGS) $<

all: kite ikt

depend:
	makedepend `find src -name '*.cpp'` -Y -Isrc/ >/dev/null 2>&1

test: all 
	sh run-tests.sh

clean: 
	rm -rf Makefile.bak `find ./ -name '*.o'` kite ikt

kite: $(KITE_OBJS)
	$(CC) $(CPPFLAGS) $(LDFLAGS) -o kite $(KITE_OBJS) $(LLVM_LDFLAGS) $(LLVM_LIBS) $(GC_LDFLAGS) $(GC_LIBS) $(MATH_LIBS)

ikt: $(IKT_OBJS)
	$(CC) $(CPPFLAGS) $(LDFLAGS) -o ikt $(IKT_OBJS) $(LLVM_LDFLAGS) $(LLVM_LIBS) $(GC_LDFLAGS) $(GC_LIBS) $(MATH_LIBS)

# DO NOT DELETE

src/apps/ikt.o: src/stdlib/language/kite.h src/stdlib/System/dynamic_object.h
src/apps/ikt.o: src/semantics/constants.h src/stdlib/System/object.h
src/apps/ikt.o: src/stdlib/System/method.h src/codegen/llvm_compile_state.h
src/apps/ikt.o: src/stdlib/language/kite/syntax_tree.h
src/apps/ikt.o: src/semantics/syntax_tree.h src/semantics/constants.h
src/apps/ikt.o: src/stdlib/System/exceptions/exception.h
src/apps/ikt.o: src/stdlib/System/string.h
src/apps/kite.o: src/stdlib/language/kite.h
src/apps/kite.o: src/stdlib/System/dynamic_object.h src/semantics/constants.h
src/apps/kite.o: src/stdlib/System/object.h src/stdlib/System/method.h
src/apps/kite.o: src/codegen/llvm_compile_state.h
src/apps/kite.o: src/stdlib/language/kite/syntax_tree.h
src/apps/kite.o: src/semantics/syntax_tree.h src/semantics/constants.h
src/codegen/llvm_compile_state.o: src/codegen/llvm_compile_state.h
src/codegen/llvm_node_codegen.o: src/codegen/llvm_node_codegen.h
src/codegen/llvm_node_codegen.o: src/semantics/syntax_tree.h
src/codegen/llvm_node_codegen.o: src/semantics/constants.h
src/codegen/llvm_node_codegen.o: src/stdlib/System/integer.h
src/codegen/llvm_node_codegen.o: src/stdlib/System/object.h
src/codegen/llvm_node_codegen.o: src/semantics/constants.h
src/codegen/llvm_node_codegen.o: src/stdlib/System/dynamic_object.h
src/codegen/llvm_node_codegen.o: src/stdlib/System/method.h
src/codegen/llvm_node_codegen.o: src/stdlib/System/string.h
src/codegen/llvm_node_codegen.o: src/stdlib/System/string.h
src/codegen/llvm_node_codegen.o: src/stdlib/System/boolean.h
src/codegen/llvm_node_codegen.o: src/stdlib/System/float.h
src/codegen/llvm_node_codegen.o: src/codegen/llvm_compile_state.h
src/codegen/llvm_node_codegen.o: src/stdlib/language/kite.h
src/codegen/llvm_node_codegen.o: src/stdlib/System/dynamic_object.h
src/codegen/llvm_node_codegen.o: src/codegen/llvm_compile_state.h
src/codegen/llvm_node_codegen.o: src/stdlib/language/kite/syntax_tree.h
src/codegen/syntax_tree_node_printer.o: src/codegen/syntax_tree_printer.h
src/codegen/syntax_tree_node_printer.o: src/semantics/syntax_tree.h
src/codegen/syntax_tree_node_printer.o: src/semantics/constants.h
src/codegen/syntax_tree_printer.o: src/codegen/syntax_tree_printer.h
src/codegen/syntax_tree_printer.o: src/semantics/syntax_tree.h
src/codegen/syntax_tree_printer.o: src/semantics/constants.h
src/parser/assignment.o: src/parser/grammar.h src/semantics/syntax_tree.h
src/parser/assignment.o: src/semantics/constants.h
src/parser/bitwise.o: src/parser/grammar.h src/semantics/syntax_tree.h
src/parser/bitwise.o: src/semantics/constants.h
src/parser/classes.o: src/parser/grammar.h src/semantics/syntax_tree.h
src/parser/classes.o: src/semantics/constants.h
src/parser/comparison.o: src/parser/grammar.h src/semantics/syntax_tree.h
src/parser/comparison.o: src/semantics/constants.h
src/parser/constants.o: src/parser/grammar.h src/semantics/syntax_tree.h
src/parser/constants.o: src/semantics/constants.h
src/parser/constructor.o: src/parser/grammar.h src/semantics/syntax_tree.h
src/parser/constructor.o: src/semantics/constants.h
src/parser/decide.o: src/parser/grammar.h src/semantics/syntax_tree.h
src/parser/decide.o: src/semantics/constants.h
src/parser/deref.o: src/parser/grammar.h src/semantics/syntax_tree.h
src/parser/deref.o: src/semantics/constants.h
src/parser/destructor.o: src/parser/grammar.h src/semantics/syntax_tree.h
src/parser/destructor.o: src/semantics/constants.h
src/parser/exceptions.o: src/parser/grammar.h src/semantics/syntax_tree.h
src/parser/exceptions.o: src/semantics/constants.h
src/parser/grouping.o: src/parser/grammar.h src/semantics/syntax_tree.h
src/parser/grouping.o: src/semantics/constants.h
src/parser/loop.o: src/parser/grammar.h src/semantics/syntax_tree.h
src/parser/loop.o: src/semantics/constants.h
src/parser/make.o: src/parser/grammar.h src/semantics/syntax_tree.h
src/parser/make.o: src/semantics/constants.h
src/parser/map_reduce.o: src/parser/grammar.h src/semantics/syntax_tree.h
src/parser/map_reduce.o: src/semantics/constants.h
src/parser/math.o: src/parser/grammar.h src/semantics/syntax_tree.h
src/parser/math.o: src/semantics/constants.h
src/parser/method.o: src/parser/grammar.h src/semantics/syntax_tree.h
src/parser/method.o: src/semantics/constants.h
src/parser/parser.o: src/parser/grammar.h src/semantics/syntax_tree.h
src/parser/parser.o: src/semantics/constants.h src/parser/parser.h
src/parser/statement.o: src/parser/grammar.h src/semantics/syntax_tree.h
src/parser/statement.o: src/semantics/constants.h
src/semantics/constants.o: src/semantics/constants.h
src/stdlib/api.o: src/stdlib/api.h src/stdlib/System/string.h
src/stdlib/api.o: src/stdlib/System/object.h src/semantics/constants.h
src/stdlib/api.o: src/stdlib/System/dynamic_object.h
src/stdlib/api.o: src/stdlib/System/method.h src/stdlib/language/kite.h
src/stdlib/api.o: src/stdlib/System/dynamic_object.h
src/stdlib/api.o: src/codegen/llvm_compile_state.h
src/stdlib/api.o: src/stdlib/language/kite/syntax_tree.h
src/stdlib/api.o: src/semantics/syntax_tree.h src/semantics/constants.h
src/stdlib/language/kite/syntax_tree.o: src/stdlib/language/kite/syntax_tree.h
src/stdlib/language/kite/syntax_tree.o: src/stdlib/System/dynamic_object.h
src/stdlib/language/kite/syntax_tree.o: src/semantics/constants.h
src/stdlib/language/kite/syntax_tree.o: src/stdlib/System/object.h
src/stdlib/language/kite/syntax_tree.o: src/stdlib/System/method.h
src/stdlib/language/kite/syntax_tree.o: src/semantics/syntax_tree.h
src/stdlib/language/kite/syntax_tree.o: src/semantics/constants.h
src/stdlib/language/kite/syntax_tree.o: src/parser/parser.h
src/stdlib/language/kite/syntax_tree.o: src/codegen/syntax_tree_printer.h
src/stdlib/language/kite.o: src/stdlib/language/kite.h
src/stdlib/language/kite.o: src/stdlib/System/dynamic_object.h
src/stdlib/language/kite.o: src/semantics/constants.h
src/stdlib/language/kite.o: src/stdlib/System/object.h
src/stdlib/language/kite.o: src/stdlib/System/method.h
src/stdlib/language/kite.o: src/codegen/llvm_compile_state.h
src/stdlib/language/kite.o: src/stdlib/language/kite/syntax_tree.h
src/stdlib/language/kite.o: src/semantics/syntax_tree.h
src/stdlib/language/kite.o: src/semantics/constants.h src/parser/parser.h
src/stdlib/language/kite.o: src/stdlib/System/exceptions/exception.h
src/stdlib/language/kite.o: src/stdlib/System/string.h
src/stdlib/language/kite.o: src/stdlib/System/exceptions/NotImplemented.h
src/stdlib/language/kite.o: src/stdlib/System/exceptions/exception.h
src/stdlib/language/kite.o: src/stdlib/System/exceptions/InvalidArgument.h
src/stdlib/language/kite.o: src/stdlib/System/exceptions/TypeMismatch.h
src/stdlib/language/kite.o: src/stdlib/System/exceptions/DivideByZero.h
src/stdlib/language/kite.o: src/stdlib/System/exceptions/NullReference.h
src/stdlib/language/kite.o: src/stdlib/System/exceptions/EmptyCollection.h
src/stdlib/language/kite.o: src/codegen/syntax_tree_printer.h
src/stdlib/language/kite.o: src/codegen/llvm_node_codegen.h
src/stdlib/language/kite.o: src/stdlib/System/integer.h
src/stdlib/language/kite.o: src/stdlib/System/dynamic_object.h
src/stdlib/language/kite.o: src/stdlib/System/string.h
src/stdlib/language/kite.o: src/stdlib/System/boolean.h
src/stdlib/language/kite.o: src/stdlib/System/float.h
src/stdlib/language/kite.o: src/codegen/llvm_compile_state.h
src/stdlib/System/boolean.o: src/stdlib/System/boolean.h
src/stdlib/System/boolean.o: src/stdlib/System/object.h
src/stdlib/System/boolean.o: src/semantics/constants.h
src/stdlib/System/boolean.o: src/stdlib/System/exceptions/TypeMismatch.h
src/stdlib/System/boolean.o: src/stdlib/System/exceptions/exception.h
src/stdlib/System/boolean.o: src/stdlib/System/exceptions/DivideByZero.h
src/stdlib/System/collections/array.o: src/stdlib/System/collections/array.h
src/stdlib/System/collections/array.o: src/stdlib/System/collections.h
src/stdlib/System/collections/array.o: src/stdlib/api.h
src/stdlib/System/collections/array.o: src/stdlib/System/string.h
src/stdlib/System/collections/array.o: src/stdlib/System/object.h
src/stdlib/System/collections/array.o: src/semantics/constants.h
src/stdlib/System/collections/array.o: src/stdlib/System/dynamic_object.h
src/stdlib/System/collections/array.o: src/stdlib/System/method.h
src/stdlib/System/collections/array.o: src/stdlib/language/kite.h
src/stdlib/System/collections/array.o: src/stdlib/System/dynamic_object.h
src/stdlib/System/collections/array.o: src/codegen/llvm_compile_state.h
src/stdlib/System/collections/array.o: src/stdlib/language/kite/syntax_tree.h
src/stdlib/System/collections/array.o: src/semantics/syntax_tree.h
src/stdlib/System/collections/array.o: src/semantics/constants.h
src/stdlib/System/collections/array.o: src/stdlib/System.h
src/stdlib/System/collections/array.o: src/stdlib/System/integer.h
src/stdlib/System/collections/array.o: src/stdlib/System/dynamic_object.h
src/stdlib/System/collections/array.o: src/stdlib/System/string.h
src/stdlib/System/collections/array.o: src/stdlib/System/boolean.h
src/stdlib/System/collections/array.o: src/stdlib/System/exceptions/InvalidArgument.h
src/stdlib/System/collections/array.o: src/stdlib/System/exceptions/exception.h
src/stdlib/System/collections/binary_tree.o: src/stdlib/System/collections/binary_tree.h
src/stdlib/System/collections/binary_tree.o: src/stdlib/System/collections.h
src/stdlib/System/collections/binary_tree.o: src/stdlib/api.h
src/stdlib/System/collections/binary_tree.o: src/stdlib/System/string.h
src/stdlib/System/collections/binary_tree.o: src/stdlib/System/object.h
src/stdlib/System/collections/binary_tree.o: src/semantics/constants.h
src/stdlib/System/collections/binary_tree.o: src/stdlib/System/dynamic_object.h
src/stdlib/System/collections/binary_tree.o: src/stdlib/System/method.h
src/stdlib/System/collections/binary_tree.o: src/stdlib/language/kite.h
src/stdlib/System/collections/binary_tree.o: src/stdlib/System/dynamic_object.h
src/stdlib/System/collections/binary_tree.o: src/codegen/llvm_compile_state.h
src/stdlib/System/collections/binary_tree.o: src/stdlib/language/kite/syntax_tree.h
src/stdlib/System/collections/binary_tree.o: src/semantics/syntax_tree.h
src/stdlib/System/collections/binary_tree.o: src/semantics/constants.h
src/stdlib/System/collections/binary_tree.o: src/stdlib/System.h
src/stdlib/System/collections/binary_tree.o: src/stdlib/System/integer.h
src/stdlib/System/collections/binary_tree.o: src/stdlib/System/dynamic_object.h
src/stdlib/System/collections/binary_tree.o: src/stdlib/System/string.h
src/stdlib/System/collections/binary_tree.o: src/stdlib/System/list.h
src/stdlib/System/collections/binary_tree.o: src/stdlib/System.h
src/stdlib/System/collections/binary_tree.o: src/stdlib/System/integer.h
src/stdlib/System/collections/binary_tree.o: src/stdlib/System/exceptions/InvalidArgument.h
src/stdlib/System/collections/binary_tree.o: src/stdlib/System/exceptions/exception.h
src/stdlib/System/collections/queue.o: src/stdlib/System/collections/queue.h
src/stdlib/System/collections/queue.o: src/stdlib/System/collections.h
src/stdlib/System/collections/queue.o: src/stdlib/api.h
src/stdlib/System/collections/queue.o: src/stdlib/System/string.h
src/stdlib/System/collections/queue.o: src/stdlib/System/object.h
src/stdlib/System/collections/queue.o: src/semantics/constants.h
src/stdlib/System/collections/queue.o: src/stdlib/System/dynamic_object.h
src/stdlib/System/collections/queue.o: src/stdlib/System/method.h
src/stdlib/System/collections/queue.o: src/stdlib/language/kite.h
src/stdlib/System/collections/queue.o: src/stdlib/System/dynamic_object.h
src/stdlib/System/collections/queue.o: src/codegen/llvm_compile_state.h
src/stdlib/System/collections/queue.o: src/stdlib/language/kite/syntax_tree.h
src/stdlib/System/collections/queue.o: src/semantics/syntax_tree.h
src/stdlib/System/collections/queue.o: src/semantics/constants.h
src/stdlib/System/collections/queue.o: src/stdlib/System.h
src/stdlib/System/collections/queue.o: src/stdlib/System/integer.h
src/stdlib/System/collections/queue.o: src/stdlib/System/dynamic_object.h
src/stdlib/System/collections/queue.o: src/stdlib/System/string.h
src/stdlib/System/collections/queue.o: src/stdlib/System/exceptions/EmptyCollection.h
src/stdlib/System/collections/queue.o: src/stdlib/System/exceptions/exception.h
src/stdlib/System/collections/stack.o: src/stdlib/System/collections/stack.h
src/stdlib/System/collections/stack.o: src/stdlib/System/collections.h
src/stdlib/System/collections/stack.o: src/stdlib/api.h
src/stdlib/System/collections/stack.o: src/stdlib/System/string.h
src/stdlib/System/collections/stack.o: src/stdlib/System/object.h
src/stdlib/System/collections/stack.o: src/semantics/constants.h
src/stdlib/System/collections/stack.o: src/stdlib/System/dynamic_object.h
src/stdlib/System/collections/stack.o: src/stdlib/System/method.h
src/stdlib/System/collections/stack.o: src/stdlib/language/kite.h
src/stdlib/System/collections/stack.o: src/stdlib/System/dynamic_object.h
src/stdlib/System/collections/stack.o: src/codegen/llvm_compile_state.h
src/stdlib/System/collections/stack.o: src/stdlib/language/kite/syntax_tree.h
src/stdlib/System/collections/stack.o: src/semantics/syntax_tree.h
src/stdlib/System/collections/stack.o: src/semantics/constants.h
src/stdlib/System/collections/stack.o: src/stdlib/System.h
src/stdlib/System/collections/stack.o: src/stdlib/System/integer.h
src/stdlib/System/collections/stack.o: src/stdlib/System/dynamic_object.h
src/stdlib/System/collections/stack.o: src/stdlib/System/string.h
src/stdlib/System/collections/stack.o: src/stdlib/System/exceptions/EmptyCollection.h
src/stdlib/System/collections/stack.o: src/stdlib/System/exceptions/exception.h
src/stdlib/System/collections.o: src/stdlib/System/collections.h
src/stdlib/System/collections.o: src/stdlib/api.h src/stdlib/System/string.h
src/stdlib/System/collections.o: src/stdlib/System/object.h
src/stdlib/System/collections.o: src/semantics/constants.h
src/stdlib/System/collections.o: src/stdlib/System/dynamic_object.h
src/stdlib/System/collections.o: src/stdlib/System/method.h
src/stdlib/System/collections.o: src/stdlib/language/kite.h
src/stdlib/System/collections.o: src/stdlib/System/dynamic_object.h
src/stdlib/System/collections.o: src/codegen/llvm_compile_state.h
src/stdlib/System/collections.o: src/stdlib/language/kite/syntax_tree.h
src/stdlib/System/collections.o: src/semantics/syntax_tree.h
src/stdlib/System/collections.o: src/semantics/constants.h
src/stdlib/System/collections.o: src/stdlib/System.h
src/stdlib/System/date.o: src/stdlib/System/date.h src/stdlib/System.h
src/stdlib/System/date.o: src/stdlib/System/integer.h
src/stdlib/System/date.o: src/stdlib/System/boolean.h
src/stdlib/System/date.o: src/stdlib/System/object.h
src/stdlib/System/date.o: src/semantics/constants.h
src/stdlib/System/dynamic_object.o: src/stdlib/System/dynamic_object.h
src/stdlib/System/dynamic_object.o: src/semantics/constants.h
src/stdlib/System/dynamic_object.o: src/stdlib/System/object.h
src/stdlib/System/dynamic_object.o: src/stdlib/System/method.h
src/stdlib/System/dynamic_object.o: src/stdlib/System/string.h
src/stdlib/System/dynamic_object.o: src/stdlib/language/kite.h
src/stdlib/System/dynamic_object.o: src/stdlib/System/dynamic_object.h
src/stdlib/System/dynamic_object.o: src/codegen/llvm_compile_state.h
src/stdlib/System/dynamic_object.o: src/stdlib/language/kite/syntax_tree.h
src/stdlib/System/dynamic_object.o: src/semantics/syntax_tree.h
src/stdlib/System/dynamic_object.o: src/semantics/constants.h
src/stdlib/System/dynamic_object.o: src/stdlib/System/exceptions/NullReference.h
src/stdlib/System/dynamic_object.o: src/stdlib/System/exceptions/exception.h
src/stdlib/System/exceptions/DivideByZero.o: src/stdlib/System/exceptions/DivideByZero.h
src/stdlib/System/exceptions/DivideByZero.o: src/stdlib/System/exceptions/exception.h
src/stdlib/System/exceptions/EmptyCollection.o: src/stdlib/System/exceptions/EmptyCollection.h
src/stdlib/System/exceptions/EmptyCollection.o: src/stdlib/System/exceptions/exception.h
src/stdlib/System/exceptions/exception.o: src/stdlib/language/kite.h
src/stdlib/System/exceptions/exception.o: src/stdlib/System/dynamic_object.h
src/stdlib/System/exceptions/exception.o: src/semantics/constants.h
src/stdlib/System/exceptions/exception.o: src/stdlib/System/object.h
src/stdlib/System/exceptions/exception.o: src/stdlib/System/method.h
src/stdlib/System/exceptions/exception.o: src/codegen/llvm_compile_state.h
src/stdlib/System/exceptions/exception.o: src/stdlib/language/kite/syntax_tree.h
src/stdlib/System/exceptions/exception.o: src/semantics/syntax_tree.h
src/stdlib/System/exceptions/exception.o: src/semantics/constants.h
src/stdlib/System/exceptions/exception.o: src/stdlib/System/exceptions/exception.h
src/stdlib/System/exceptions/InvalidArgument.o: src/stdlib/System/exceptions/InvalidArgument.h
src/stdlib/System/exceptions/InvalidArgument.o: src/stdlib/System/exceptions/exception.h
src/stdlib/System/exceptions/NotImplemented.o: src/stdlib/System/exceptions/NotImplemented.h
src/stdlib/System/exceptions/NotImplemented.o: src/stdlib/System/exceptions/exception.h
src/stdlib/System/exceptions/NullReference.o: src/stdlib/System/exceptions/NullReference.h
src/stdlib/System/exceptions/NullReference.o: src/stdlib/System/exceptions/exception.h
src/stdlib/System/exceptions/TypeMismatch.o: src/stdlib/System/exceptions/TypeMismatch.h
src/stdlib/System/exceptions/TypeMismatch.o: src/stdlib/System/exceptions/exception.h
src/stdlib/System/exceptions.o: src/stdlib/System.h src/stdlib/api.h
src/stdlib/System/exceptions.o: src/stdlib/System/string.h
src/stdlib/System/exceptions.o: src/stdlib/System/object.h
src/stdlib/System/exceptions.o: src/semantics/constants.h
src/stdlib/System/exceptions.o: src/stdlib/System/dynamic_object.h
src/stdlib/System/exceptions.o: src/stdlib/System/method.h
src/stdlib/System/exceptions.o: src/stdlib/language/kite.h
src/stdlib/System/exceptions.o: src/stdlib/System/dynamic_object.h
src/stdlib/System/exceptions.o: src/codegen/llvm_compile_state.h
src/stdlib/System/exceptions.o: src/stdlib/language/kite/syntax_tree.h
src/stdlib/System/exceptions.o: src/semantics/syntax_tree.h
src/stdlib/System/exceptions.o: src/semantics/constants.h
src/stdlib/System/exceptions.o: src/stdlib/System/exceptions.h
src/stdlib/System/float.o: src/stdlib/System/float.h
src/stdlib/System/float.o: src/stdlib/System/object.h
src/stdlib/System/float.o: src/semantics/constants.h
src/stdlib/System/float.o: src/stdlib/System/dynamic_object.h
src/stdlib/System/float.o: src/stdlib/System/method.h
src/stdlib/System/float.o: src/stdlib/System/string.h
src/stdlib/System/float.o: src/stdlib/System/boolean.h
src/stdlib/System/float.o: src/stdlib/System/exceptions/TypeMismatch.h
src/stdlib/System/float.o: src/stdlib/System/exceptions/exception.h
src/stdlib/System/float.o: src/stdlib/System/exceptions/DivideByZero.h
src/stdlib/System/integer.o: src/stdlib/System/integer.h
src/stdlib/System/integer.o: src/stdlib/System/boolean.h
src/stdlib/System/integer.o: src/stdlib/System/object.h
src/stdlib/System/integer.o: src/semantics/constants.h
src/stdlib/System/integer.o: src/stdlib/System/exceptions/TypeMismatch.h
src/stdlib/System/integer.o: src/stdlib/System/exceptions/exception.h
src/stdlib/System/integer.o: src/stdlib/System/exceptions/DivideByZero.h
src/stdlib/System/list.o: src/stdlib/System/list.h src/stdlib/System.h
src/stdlib/System/list.o: src/stdlib/System/integer.h
src/stdlib/System/method.o: src/stdlib/System/method.h
src/stdlib/System/method.o: src/stdlib/System/object.h
src/stdlib/System/method.o: src/semantics/constants.h
src/stdlib/System/object.o: src/stdlib/System/object.h
src/stdlib/System/object.o: src/semantics/constants.h
src/stdlib/System/object.o: src/stdlib/System/string.h
src/stdlib/System/object.o: src/stdlib/System/integer.h
src/stdlib/System/object.o: src/stdlib/System/float.h
src/stdlib/System/object.o: src/stdlib/System/dynamic_object.h
src/stdlib/System/object.o: src/stdlib/System/method.h
src/stdlib/System/object.o: src/stdlib/System/boolean.h
src/stdlib/System/object.o: src/stdlib/System/exceptions/NotImplemented.h
src/stdlib/System/object.o: src/stdlib/System/exceptions/exception.h
src/stdlib/System/object.o: src/stdlib/System/exceptions/InvalidArgument.h
src/stdlib/System/object.o: src/stdlib/System/exceptions/NullReference.h
src/stdlib/System/string.o: src/stdlib/System/string.h
src/stdlib/System/string.o: src/stdlib/System/object.h
src/stdlib/System/string.o: src/semantics/constants.h
src/stdlib/System.o: src/stdlib/System.h src/stdlib/api.h
src/stdlib/System.o: src/stdlib/System/string.h src/stdlib/System/object.h
src/stdlib/System.o: src/semantics/constants.h
src/stdlib/System.o: src/stdlib/System/dynamic_object.h
src/stdlib/System.o: src/stdlib/System/method.h src/stdlib/language/kite.h
src/stdlib/System.o: src/stdlib/System/dynamic_object.h
src/stdlib/System.o: src/codegen/llvm_compile_state.h
src/stdlib/System.o: src/stdlib/language/kite/syntax_tree.h
src/stdlib/System.o: src/semantics/syntax_tree.h src/semantics/constants.h
