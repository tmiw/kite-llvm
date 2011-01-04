/*****************************************************************************
 * Copyright (c) 2011, Mooneer Salem
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *     * Redistributions of source code must retain the above copyright
 *       notice, this list of conditions and the following disclaimer.
 *     * Redistributions in binary form must reproduce the above copyright
 *       notice, this list of conditions and the following disclaimer in the
 *       documentation and/or other materials provided with the distribution.
 *     * Neither the name of the Kite Language organization nor the
 *       names of its contributors may be used to endorse or promote products
 *       derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY MOONEER SALEM ``AS IS'' AND ANY
 * EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL MOONEER SALEM BE LIABLE FOR ANY
 * DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
 * THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 ****************************************************************************/

#include <iostream>
#include <parser/parser.h>
#include <codegen/syntax_tree_printer.h>
#include <codegen/llvm_node_codegen.h>
#include <llvm/LLVMContext.h>
#include <llvm/Target/TargetSelect.h>
#include <llvm/Bitcode/ReaderWriter.h>
#include <llvm/ExecutionEngine/ExecutionEngine.h>
//#include <llvm/ModuleProvider.h>
#include <llvm/Support/MemoryBuffer.h>
#include <llvm/ExecutionEngine/JIT.h>
#include <llvm/PassManager.h>
#include <llvm/Analysis/Verifier.h>
#include <llvm/Analysis/Passes.h>
#include <llvm/Target/TargetData.h>
#include <llvm/Target/TargetSelect.h>
#include <llvm/Transforms/Scalar.h>
using namespace llvm;
using namespace kite;
using namespace std;

int main()
{
    InitializeNativeTarget();
    llvm_start_multithreaded();
    
    semantics::syntax_tree ast;
    string storage = "x = 0; while (x < 100) [ x = x + 1; decide [ (x < 10) [ (x - 100)|print; ] (true) [ x|print; ] ]]";
    //x = 41|int; y = 1|int; (x + y)|print;";
    bool r = parser::kite_parser().parse(storage, ast);
    
    if (r)
    {
        std::cout << "-------------------------\n";
        std::cout << "Parsing succeeded\n";
        std::cout << "-------------------------\n";
        codegen::syntax_tree_printer printer;
        printer(ast);
        
        LLVMContext &context = getGlobalContext();
        codegen::llvm_compile_state state;
        state.push_symbol_stack();
        state.push_module(new Module("test_module", context));
        Module *currentModule = state.current_module();
        FunctionType *FT = FunctionType::get(Type::getVoidTy(context), false);
        Function *F = Function::Create(FT, Function::ExternalLinkage, "begin", currentModule);
        BasicBlock *BB = BasicBlock::Create(getGlobalContext(), "entry", F);
        IRBuilder<> &builder = state.module_builder();
        builder.SetInsertPoint(BB);

        codegen::llvm_node_codegen cg(state);
        
        cg(ast);
        builder.CreateRetVoid();
        std::cout << "Verifying code..." << std::endl;
        verifyFunction(*F);
        std::cout << "Optimizing code..." << std::endl;
        FunctionPassManager OurFPM(currentModule);

        ExecutionEngine *engine = EngineBuilder(currentModule).create();
        // Set up the optimizer pipeline.  Start with registering info about how the
        // target lays out data structures.
        OurFPM.add(new TargetData(*engine->getTargetData()));
        // Provide basic AliasAnalysis support for GVN.
        OurFPM.add(createBasicAliasAnalysisPass());
        // Do simple "peephole" optimizations and bit-twiddling optzns.
        OurFPM.add(createInstructionCombiningPass());
        // Reassociate expressions.
        OurFPM.add(createReassociatePass());
        // Eliminate Common SubExpressions.
        OurFPM.add(createGVNPass());
        // Simplify the control flow graph (deleting unreachable blocks, etc).
        OurFPM.add(createCFGSimplificationPass());

        OurFPM.doInitialization();
        OurFPM.run(*F);
        
        std::cout << "Dumping code..." << std::endl;
        currentModule->dump();
        std::cout << "Executing code..." << std::endl;
        void *fptr = engine->getPointerToFunction(F);
        void(*FP)() = (void(*)())fptr;
        (*FP)();
        
        return 0;
    }
    else
    {
        //std::string::const_iterator some = iter+30;
        //std::string context(iter, (some>end)?end:some);
        std::cout << "-------------------------\n";
        std::cout << "Parsing failed\n";
        //std::cout << "stopped at: \": " << context << "...\"\n";
        std::cout << "-------------------------\n";
        return 1;
    }
}