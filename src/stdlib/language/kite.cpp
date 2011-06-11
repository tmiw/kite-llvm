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

#include "kite.h"

#include <parser/parser.h>
#include <stdlib/System/dynamic_object.h>
#include <stdlib/System/exceptions/exception.h>
#include <codegen/syntax_tree_printer.h>
#include <codegen/llvm_node_codegen.h>
#include <llvm/LLVMContext.h>
#include <llvm/Target/TargetSelect.h>
#include <llvm/ExecutionEngine/ExecutionEngine.h>
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

namespace kite
{
    namespace stdlib
    {
        namespace language
        {
            namespace kite
            {
                Module *kite::current_module = NULL;
                System::dynamic_object *kite::root_object = NULL;
                codegen::llvm_compile_state kite::state;
                ExecutionEngine *kite::execution_engine = NULL;
                bool kite::enable_optimizer = false;
                std::vector<jmp_buf*> kite::exception_stack;
                System::dynamic_object *kite::last_exception = NULL;
                
                void kite::InitializeRuntimeSystem()
                {
                    InitializeNativeTarget();
                    llvm_start_multithreaded();

                    current_module = new Module("__root_module", getGlobalContext());
                    root_object = new System::dynamic_object();
                    state.push_module(current_module);
                    execution_engine = EngineBuilder(current_module).create();
                    
                    // TODO
                    System::dynamic_object *system_obj = new System::dynamic_object();
                    System::dynamic_object *exceptions_obj = new System::dynamic_object();
                    root_object->properties["System"] = system_obj;
                    system_obj->properties["exceptions"] = exceptions_obj;
                    exceptions_obj->properties["exception"] = &System::exceptions::exception::class_object;
                    System::exceptions::exception::InitializeClass();
                }

                System::object *kite::ExecuteCode(syntax_tree &ast)
                {
                    return ExecuteCode(ast, root_object);
                }

                System::object *kite::ExecuteCode(syntax_tree &ast, System::object *context)
                {
                    codegen::llvm_node_codegen cg(state);
                    std::vector<std::string> argNames;
                    Function *function = (Function*)cg.generate_llvm_method("__static_init__", argNames, ast.ast);

                    if (enable_optimizer)
                    {
                        FunctionPassManager FPM(current_module);

                        // Set up the optimizer pipeline.  Start with registering info about how the
                        // target lays out data structures.
                        FPM.add(new TargetData(*execution_engine->getTargetData()));
                        // Provide basic AliasAnalysis support for GVN.
                        FPM.add(createBasicAliasAnalysisPass());
                        // Do simple "peephole" optimizations and bit-twiddling optzns.
                        FPM.add(createInstructionCombiningPass());
                        // Reassociate expressions.
                        FPM.add(createReassociatePass());
                        // Eliminate Common SubExpressions.
                        FPM.add(createGVNPass());
                        // Simplify the control flow graph (deleting unreachable blocks, etc).
                        FPM.add(createCFGSimplificationPass());

                        FPM.doInitialization();
                        FPM.run(*function);
                    }

                    //current_module->dump();
                    void *fptr = execution_engine->getPointerToFunction(function);
                    System::object *(*FP)(System::object *) = (System::object*(*)(System::object*))fptr;
                    return (*FP)(context);
                }

                void kite::DumpCompiledCode()
                {
                    current_module->dump();
                }
            }
        }
    }
}

#if 0
            // bitcode caching disabled for now.
            if (fileNameCompiled.size() > 0)
            {
                raw_fd_ostream os(fileNameCompiled.c_str(), errorInfo);
                WriteBitcodeToFile(currentModule, os);
                os.close();
            }

            {
                std::string error;
                currentModule = ParseBitcodeFile(MemoryBuffer::getFile(fileNameCompiled.c_str()), context, &error);
                engine = EngineBuilder(currentModule).create();
                F = engine->FindFunctionNamed("__static_init____o");
            }
#endif
