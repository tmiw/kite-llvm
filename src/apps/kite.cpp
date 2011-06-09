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
#include <fstream>
#include <unistd.h>
#include <sys/stat.h>
#include <parser/parser.h>
#include <codegen/syntax_tree_printer.h>
#include <codegen/llvm_node_codegen.h>
#include <stdlib/System/dynamic_object.h>
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
#include <llvm/Support/raw_ostream.h>
using namespace llvm;
using namespace kite;
using namespace std;

void usage(char *app_name)
{
    printf("Usage: %s [-haodx] [file]\n", app_name);
    printf("       -h: this message\n");
    printf("       -a: output parse tree\n");
    printf("       -o: optimize compiled code\n");
    printf("       -d: dump compiled code to stdout\n");
    printf("       -x: do not execute compiled code\n");
    printf("     file: the Kite file to execute (default: standard input)\n");
    exit(0);
}

int main(int argc, char **argv)
{
    int ch;
    bool output_ast = false;
    bool optimize_code = false;
    bool dump_code = false;
    bool inhibit_exec = false;
    
    while ((ch = getopt(argc, argv, "haodx")) != -1)
    {
        switch(ch)
        {
            case 'a':
                output_ast = true;
                break;
            case 'o':
                optimize_code = true;
                break;
            case 'd':
                dump_code = true;
                break;
            case 'x':
                inhibit_exec = true;
                break;
            case 'h':
            default:
                usage(argv[0]);
        }
    }
    
    argc -= optind;
    argv += optind;
         
    InitializeNativeTarget();
    llvm_start_multithreaded();
    
    string storage = "";
    char buf[1024];
    semantics::syntax_tree ast;
    //string storage = "x = 0; while (x < 100) [ x = x + 1; decide [ (x < 10) [ (x - 100)|print; ], true [ x|print; ] ] ]";
    //string storage = "x = 41|int; y = 1|int; (x + y)|print;";
    //string storage = "x = 1; while (x > 0) [ x = x - 1; decide [ (x == 1) [ (x)|print; ], true [ 0|print; ] ] ]";
    //string storage = "x = 1; (x)|print;";
    //string storage = "(1 or 2 or 3)|print;";
    
    bool bypass_parse = false;
    std::string fileNameCompiled;
    if (argc > 0)
    {
        struct stat compiledSt;
        struct stat rawSt;
        std::string fileName = argv[0];
        fileNameCompiled = std::string(argv[0]) + std::string("o");
        
        if (stat(fileNameCompiled.c_str(), &compiledSt) == 0)
        {
            stat(fileName.c_str(), &rawSt);
            if (rawSt.st_mtime <= compiledSt.st_mtime)
            {
                bypass_parse = true;
            }
        }
        
        if (bypass_parse == false)
        {
            std::ifstream handle(argv[0]);
            while (!handle.eof())
            {
                handle.getline(buf, 1024);
                storage += buf;
            }
            handle.close();
        }
    }
    else
    {
        // Standard input.
        while (!cin.eof())
        {
            cin.getline(buf, 1024);
            storage += buf;
        }
    }
    bool r;
    
    if (bypass_parse == false) 
        r = parser::kite_parser().parse(storage, ast);
    else
        r = bypass_parse;
        
    if (r)
    {
        if (output_ast && !bypass_parse)
        {
            codegen::syntax_tree_printer printer;
            printer(ast);
        }
        
        Module *currentModule;
        LLVMContext &context = getGlobalContext();
        ExecutionEngine *engine;
        Function *F;
        if (bypass_parse == false)
        {
            codegen::llvm_compile_state state;
            state.push_symbol_stack();
            state.push_module(new Module("test_module", context));
            currentModule = state.current_module();
            FunctionType *FT = FunctionType::get(Type::getVoidTy(context), false);
            F = Function::Create(FT, Function::ExternalLinkage, "begin", currentModule);
            BasicBlock *BB = BasicBlock::Create(getGlobalContext(), "entry", F);
            IRBuilder<> &builder = state.module_builder();
            builder.SetInsertPoint(BB);

            std::map<std::string, Value*> &sym_stack = state.current_symbol_stack();
            // TODO
            const Type *ptrType = codegen::llvm_node_codegen::kite_type_to_llvm_type(semantics::OBJECT);
            sym_stack["this"] = builder.CreateAlloca(ptrType);
            Value *intObj = ConstantInt::get(getGlobalContext(), APInt(sizeof(void*) << 3, (uint64_t)kite_dynamic_object_alloc(), true));
            Value *ptrObj = builder.CreateIntToPtr(intObj, ptrType);
            builder.CreateStore(ptrObj, sym_stack["this"]);
            codegen::llvm_node_codegen cg(state);
            cg(ast);
            builder.CreateRetVoid();
            verifyFunction(*F);

            engine = EngineBuilder(currentModule).create();
            if (optimize_code)
            {
                FunctionPassManager OurFPM(currentModule);
            
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
            }
            
            std::string errorInfo;
            if (fileNameCompiled.size() > 0)
            {
                raw_fd_ostream os(fileNameCompiled.c_str(), errorInfo);
                WriteBitcodeToFile(currentModule, os);
                os.close();
            }
        }
        else
        {
            std::string error;
            currentModule = ParseBitcodeFile(MemoryBuffer::getFile(fileNameCompiled.c_str()), context, &error);
            engine = EngineBuilder(currentModule).create();
            F = engine->FindFunctionNamed("begin");
        }
        
        if (dump_code) currentModule->dump();
                
        if (!inhibit_exec)
        {
            void *fptr = engine->getPointerToFunction(F);
            void(*FP)() = (void(*)())fptr;
            (*FP)();
        }
        
        return 0;
    }
    else
    {
        std::cout << "Parsing failed\n";
        return 1;
    }
}
