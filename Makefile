CC=g++
CPPFLAGS=-g -O2
LDFLAGS=-rdynamic
INC=-Isrc/ -I/opt/local/include
LLVM_CONFIG=llvm-config-2.8
LLVM_CPPFLAGS=`$(LLVM_CONFIG) --cppflags`
LLVM_LDFLAGS=`$(LLVM_CONFIG) --ldflags`
LLVM_LIBS=`$(LLVM_CONFIG) --libs` 
OBJS=src/apps/kite.o src/codegen/llvm_compile_state.o src/codegen/llvm_node_codegen.o \
	 src/codegen/syntax_tree_node_printer.o src/codegen/syntax_tree_printer.o \
	 src/parser/parser.o src/stdlib/System/integer.o src/stdlib/System/string.o \
	 src/stdlib/System/boolean.o src/stdlib/System/float.o src/stdlib/System/object.o

.cpp.o: %.cpp
	$(CC) -c -o $@ $(CPPFLAGS) $(INC) $(LLVM_CPPFLAGS) $<

all: kite

depend:
	makedepend `find src -name '*.cpp'` -Y -Isrc/ >/dev/null 2>&1

clean:
	rm -rf `find ./ -name '*.o'` `find ./ -name 'kite'`

kite: $(OBJS)
	$(CC) $(CPPFLAGS) $(LDFLAGS) -o kite $(OBJS) $(LLVM_LDFLAGS) $(LLVM_LIBS)

# DO NOT DELETE

src/apps/kite.o: src/parser/parser.h src/semantics/syntax_tree.h
src/apps/kite.o: src/semantics/constants.h src/codegen/syntax_tree_printer.h
src/apps/kite.o: src/codegen/llvm_node_codegen.h src/stdlib/System/integer.h
src/apps/kite.o: src/stdlib/System/object.h src/semantics/constants.h
src/apps/kite.o: src/stdlib/System/string.h src/stdlib/System/boolean.h
src/apps/kite.o: src/stdlib/System/float.h src/codegen/llvm_compile_state.h
src/parser/method.o: src/parser/grammar.h src/semantics/syntax_tree.h
src/parser/method.o: src/semantics/constants.h
src/parser/deref.o: src/parser/grammar.h src/semantics/syntax_tree.h
src/parser/deref.o: src/semantics/constants.h
src/parser/map_reduce.o: src/parser/grammar.h src/semantics/syntax_tree.h
src/parser/map_reduce.o: src/semantics/constants.h
src/parser/assignment.o: src/parser/grammar.h src/semantics/syntax_tree.h
src/parser/assignment.o: src/semantics/constants.h
src/parser/bitwise.o: src/parser/grammar.h src/semantics/syntax_tree.h
src/parser/bitwise.o: src/semantics/constants.h
src/parser/classes.o: src/parser/grammar.h src/semantics/syntax_tree.h
src/parser/classes.o: src/semantics/constants.h
src/parser/parser.o: src/parser/grammar.h src/semantics/syntax_tree.h
src/parser/parser.o: src/semantics/constants.h src/parser/parser.h
src/parser/parser.o: src/parser/assignment.cpp src/parser/bitwise.cpp
src/parser/parser.o: src/parser/classes.cpp src/parser/comparison.cpp
src/parser/parser.o: src/parser/constants.cpp src/parser/decide.cpp
src/parser/parser.o: src/parser/deref.cpp src/parser/grouping.cpp
src/parser/parser.o: src/parser/loop.cpp src/parser/map_reduce.cpp
src/parser/parser.o: src/parser/math.cpp src/parser/method.cpp
src/parser/parser.o: src/parser/statement.cpp
src/parser/grouping.o: src/parser/grammar.h src/semantics/syntax_tree.h
src/parser/grouping.o: src/semantics/constants.h
src/parser/math.o: src/parser/grammar.h src/semantics/syntax_tree.h
src/parser/math.o: src/semantics/constants.h
src/parser/loop.o: src/parser/grammar.h src/semantics/syntax_tree.h
src/parser/loop.o: src/semantics/constants.h
src/parser/statement.o: src/parser/grammar.h src/semantics/syntax_tree.h
src/parser/statement.o: src/semantics/constants.h
src/parser/decide.o: src/parser/grammar.h src/semantics/syntax_tree.h
src/parser/decide.o: src/semantics/constants.h
src/parser/comparison.o: src/parser/grammar.h src/semantics/syntax_tree.h
src/parser/comparison.o: src/semantics/constants.h
src/parser/constants.o: src/parser/grammar.h src/semantics/syntax_tree.h
src/parser/constants.o: src/semantics/constants.h
src/codegen/syntax_tree_node_printer.o: src/codegen/syntax_tree_printer.h
src/codegen/syntax_tree_node_printer.o: src/semantics/syntax_tree.h
src/codegen/syntax_tree_node_printer.o: src/semantics/constants.h
src/codegen/llvm_compile_state.o: src/codegen/llvm_compile_state.h
src/codegen/syntax_tree_printer.o: src/codegen/syntax_tree_printer.h
src/codegen/syntax_tree_printer.o: src/semantics/syntax_tree.h
src/codegen/syntax_tree_printer.o: src/semantics/constants.h
src/codegen/llvm_node_codegen.o: src/codegen/llvm_node_codegen.h
src/codegen/llvm_node_codegen.o: src/semantics/syntax_tree.h
src/codegen/llvm_node_codegen.o: src/semantics/constants.h
src/codegen/llvm_node_codegen.o: src/stdlib/System/integer.h
src/codegen/llvm_node_codegen.o: src/stdlib/System/object.h
src/codegen/llvm_node_codegen.o: src/semantics/constants.h
src/codegen/llvm_node_codegen.o: src/stdlib/System/string.h
src/codegen/llvm_node_codegen.o: src/stdlib/System/boolean.h
src/codegen/llvm_node_codegen.o: src/stdlib/System/float.h
src/codegen/llvm_node_codegen.o: src/codegen/llvm_compile_state.h
src/stdlib/System/string.o: src/stdlib/System/string.h
src/stdlib/System/string.o: src/stdlib/System/object.h
src/stdlib/System/string.o: src/semantics/constants.h
src/stdlib/System/boolean.o: src/stdlib/System/boolean.h
src/stdlib/System/boolean.o: src/stdlib/System/object.h
src/stdlib/System/boolean.o: src/semantics/constants.h
src/stdlib/System/object.o: src/stdlib/System/object.h
src/stdlib/System/object.o: src/semantics/constants.h
src/stdlib/System/object.o: src/stdlib/System/string.h
src/stdlib/System/object.o: src/stdlib/System/integer.h
src/stdlib/System/object.o: src/stdlib/System/float.h
src/stdlib/System/object.o: src/stdlib/System/boolean.h
src/stdlib/System/object.o: src/stdlib/System/method.h
src/stdlib/System/object.o: src/stdlib/System/dynamic_object.h
src/stdlib/System/float.o: src/stdlib/System/float.h
src/stdlib/System/float.o: src/stdlib/System/object.h
src/stdlib/System/float.o: src/semantics/constants.h
src/stdlib/System/integer.o: src/stdlib/System/integer.h
src/stdlib/System/integer.o: src/stdlib/System/object.h
src/stdlib/System/integer.o: src/semantics/constants.h
