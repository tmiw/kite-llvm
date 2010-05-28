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
#include "kite_ast_binaryop.h"
#include "kite_ast_variable.h"

using namespace kite::parse_tree;

int main (int argc, char * const argv[]) {
    // TODO: kite compiler driver.
	InitializeNativeTarget();
	llvm_start_multithreaded();
	
	ConstantValue<int> *constant1 = new ConstantValue<int>(32);
	//ConstantValue<int> *constant2 = new ConstantValue<int>(10);
	VariableValue *var = new VariableValue("param");
	
	BinaryOperation *constant = new BinaryOperation(ADDITION, constant1, var);
	MethodValue *method = new MethodValue("life_universe_everything");
	method->push_parameter("param", Type::getInt32Ty(getGlobalContext()));
	method->push_instruction(new VariableValue("p2"));
	method->push_instruction(constant);
	CompilerState *state = new CompilerState();
	
	LLVMContext &context = getGlobalContext();
	state->push_module(new Module("test_module", context));
	Value *v = method->codegen(state);
	
	Module *mod = state->pop_module();
	
	ExecutionEngine *engine = EngineBuilder(mod).create();
	Function *f = static_cast<Function*>(v);
	void *fptr = engine->getPointerToFunction(f);
	
	if (f->getReturnType()->isFloatingPoint())
	{
		double (*FP)(int*,...) = (double (*)(int*,...))(intptr_t)fptr;
		std::cout << "Evaluates to: " << (*FP)(NULL, 10) << std::endl;
	}
	else
	{
		int (*IP)(int*,...) = (int (*)(int*,...))(intptr_t)fptr;
		std::cout << "Evaluates to: " << (*IP)(NULL, 10) << std::endl;
	}

	mod->dump();
	
    return 0;
}
