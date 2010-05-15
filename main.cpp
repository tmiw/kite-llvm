#include <iostream>

#include <llvm/LLVMContext.h>
#include <llvm/Target/TargetSelect.h>
#include <llvm/Bitcode/ReaderWriter.h>
#include <llvm/ExecutionEngine/ExecutionEngine.h>
#include <llvm/ModuleProvider.h>
#include <llvm/Support/MemoryBuffer.h>
#include <llvm/ExecutionEngine/JIT.h>

#include "kite_ast_base.h"
#include "kite_ast_function.h"
#include "kite_ast_const_base.h"

using namespace kite::parse_tree;

int main (int argc, char * const argv[]) {
    // TODO: kite compiler driver.
	InitializeNativeTarget();
	llvm_start_multithreaded();
	
	ConstantValue<int> *constant = new ConstantValue<int>(42);
	MethodValue *method = new MethodValue("life_universe_everything");
	method->push_instruction(constant);
	CompilerState *state = new CompilerState();
	
	LLVMContext &context = getGlobalContext();
	state->push_module(new Module("test_module", context));
	Value *v = method->codegen(state);
	
	Module *mod = state->pop_module();
	
	ExecutionEngine *engine = EngineBuilder(mod).create();
	void *fptr = engine->getPointerToFunction(static_cast<Function*>(v));
	double (*FP)() = (double (*)())(intptr_t)fptr;
	std::cout << "Evaluates to: " << (*FP)() << std::endl;
	mod->dump();
	
    return 0;
}
