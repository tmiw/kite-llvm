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

#include <algorithm>
#include <sys/stat.h>
#include <parser/parser.h>
#include <stdlib/System/dynamic_object.h>
#include <stdlib/System/method.h>
#include <stdlib/System/os.h>
#include <stdlib/System/list.h>
#include <stdlib/System/regex.h>
#include <stdlib/System/vm/loader.h>
#include <codegen/syntax_tree_printer.h>
#include <codegen/llvm_node_codegen.h>
#include <llvm/LLVMContext.h>
#include <llvm/ExecutionEngine/ExecutionEngine.h>
#include <llvm/Support/MemoryBuffer.h>
//#include <llvm/ExecutionEngine/MCJIT.h>
#include <llvm/ExecutionEngine/JIT.h>
#include <llvm/PassManager.h>
#include <llvm/Analysis/Verifier.h>
#include <llvm/Analysis/Passes.h>
#include <llvm/Target/TargetData.h>
#include <llvm/Support/TargetSelect.h>
#include <llvm/Transforms/Scalar.h>
#include <llvm/Support/raw_ostream.h>
#include <llvm/Target/TargetOptions.h>
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
                
                void kite::InitializeRuntimeSystem(int argc, char **argv)
                {
                    InitializeNativeTarget();
                    InitializeNativeTargetAsmPrinter();
                    llvm_start_multithreaded();

                    current_module = new Module("__root_module", getGlobalContext());
                    state.push_module(current_module);
                    
                    System::dynamic_object *system_obj = (System::dynamic_object*)root()->properties["System"];
                    system_obj->properties["float"] = &System::fpnum::class_object;
                    system_obj->properties["integer"] = &System::integer::class_object;
                    system_obj->properties["boolean"] = &System::boolean::class_object;
                    system_obj->properties["method"] = &System::method::class_object;
                    system_obj->properties["string"] = &System::string::class_object;
                    System::fpnum::InitializeClass();
                    System::integer::InitializeClass();
                    System::boolean::InitializeClass();
                    System::method::InitializeClass();
                    System::string::InitializeClass();
                    
                    // Initialize args array
                    System::list *arg_list = System::list::Create(0);
                    for (int index = 0; index < argc; index++)
                    {
                        arg_list->list_contents.push_back(new System::string(argv[index]));
                    }
                    System::os::os::class_object().properties["args"] = arg_list;
                }

                System::object *kite::ImportModule(std::string &module_name)
                {
                    System::dynamic_object *context = root_object;

                    // Generate module load list.
                    std::vector<std::string> module_load_list;
                    std::vector<std::string> module_name_list;
                    size_t split_pos = module_name.find(".");
                    size_t begin_pos = 0;
                    while (split_pos != std::string::npos)
                    {
                        std::string tmp_string(module_name, 0, split_pos - begin_pos);
                        std::replace(tmp_string.begin(), tmp_string.end(), '.', '/');
                        module_load_list.push_back(tmp_string);
                        module_name_list.push_back(std::string(module_name, begin_pos, split_pos - begin_pos));
                        begin_pos = split_pos + 1;
                        split_pos = module_name.find(".", split_pos + 1);
                    }
                    std::string tmp_string(module_name);
                    std::replace(tmp_string.begin(), tmp_string.end(), '.', '/');
                    module_load_list.push_back(tmp_string);
                    module_name_list.push_back(std::string(module_name, begin_pos, std::string::npos));
                    
                    // Split search path on ':'.
                    System::regex::regex *rgx = System::regex::regex::Create(1, new System::string(":"));
                    System::list *search_path = (System::list*)rgx->split((System::string*)System::vm::loader::class_object().properties["searchPath"]);
                    
                    // Load each module in the list.
                    int size = module_name_list.size();
                    for (int index = 0; index < size; index++)
                    {
                        state.push_namespace_stack(module_name_list[index].c_str());
                        
                        std::string full_path;
                        System::dynamic_object *the_object = new System::dynamic_object();
                        the_object->properties["__name"] = new System::string(state.full_class_name().c_str());
                        
                        for (std::deque<System::object*>::iterator pos = search_path->list_contents.begin(); pos != search_path->list_contents.end(); pos++)
                        {
                            struct stat st;
                            
                            // TODO: support C based Kite modules.
                            full_path = std::string(((System::string*)(*pos))->string_val.c_str()) + module_load_list[index] + ".kt";
                            if (stat(full_path.c_str(), &st) == 0)
                            {
                                break;
                            }
                            full_path = "";
                        }
                        
                        if (context->properties[module_name_list[index]] != NULL) 
                        {
                            context = (System::dynamic_object*)context->properties[module_name_list[index]];
                            continue;
                        }
                        
                        if (full_path.size() == 0)
                        {
                            if (index == size - 1)
                            {
                                // TODO: throw exception for file not found.
                                for (; index > 0; index--)
                                {
                                    state.pop_namespace_stack();
                                }
                                return NULL;
                            }
                        }
                        else
                        {
                            language::kite::syntax_tree ast;
                            if (ast.from_file(full_path))
                            {
                                ExecuteCode(ast, the_object);
                            }
                            else
                            {
                                // TODO: throw exception for parse error.
                                for (; index > 0; index--)
                                {
                                    state.pop_namespace_stack();
                                }
                                return NULL;
                            }
                        }
                        
                        context->properties[module_name_list[index]] = the_object;
                        context = the_object;
                    }
                    
                    return context;
                }
                
                System::object *kite::ExecuteCode(syntax_tree &ast, bool suppressExec)
                {
                    return ExecuteCode(ast, root_object, suppressExec);
                }

                System::object *kite::ExecuteCode(syntax_tree &ast, System::object *context, bool suppressExec)
                {
                    codegen::llvm_node_codegen cg(state);
                    std::vector<std::string> argNames;

                    semantics::syntax_tree fake_ast;
                    fake_ast.position.line = 1;
                    fake_ast.position.column = 1;
                    fake_ast.position.file = ast.ast.position.file;
                    Function *function = (Function*)cg.generate_llvm_method("__static_init__", argNames, ast.ast, fake_ast);

#ifdef LLVM3_1
                    llvm::TargetOptions targetOptions;
                    targetOptions.JITEmitDebugInfo = true;
#else
                    llvm::JITEmitDebugInfo = true; // for not-weird stack traces in gdb
#endif

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
                    verifyFunction(*function);
                    
                    if (!suppressExec)
                    {
                        if (execution_engine == NULL)
                        {
                            state.debugBuilder->finalize();
                            state.debugBuilder = NULL; // TODO: proper cleanup.
                            EngineBuilder engineBuilder(current_module);
#ifdef LLVM3_1
                            engineBuilder.setTargetOptions(targetOptions);
#endif
                            //engineBuilder.setUseMCJIT(true);
                            execution_engine = engineBuilder.create();
                        }

                        void *fptr = execution_engine->getPointerToFunction(function);
                        System::object *(*FP)(System::object *) = (System::object*(*)(System::object*))fptr;
                        return (*FP)(context);
                    }
                    else
                    {
                        return NULL;
                    }
                }

                void kite::DumpCompiledCode()
                {
                    current_module->dump();
                }
                
                static bool sort_pointers(std::pair<void *, Function *> i, std::pair<void *, Function *> j)
                {
                    return i.first > j.first;
                }
                
                std::string kite::GetMethodNameFromPointer(void *ptr, void **beginPointer)
                {
                    // We need to go through every function object in
                    // the LLVM Module object to find the correct function
                    // name. This might cause performance issues if called often.
                    // TODO: evaluate for possible performance improvement.
                    // TODO: this method will break once we begin compiling to ELF binary files.
                    std::string retValue;
                    std::vector<std::pair<void *, Function *> > methods;
                    for (Module::iterator i = current_module->begin();
                         i != current_module->end();
                         i++)
                    {
                        Function *fxn = i;
                        //if (std::string(fxn->getName()).compare(0, 4, "kite") == 0) continue;
                        void *fxn_begin_ptr = execution_engine->getPointerToFunction(fxn);
                        methods.push_back(std::pair<void*, Function*>(fxn_begin_ptr, fxn));
                    }
                    std::sort(methods.begin(), methods.end(), sort_pointers);
                    
                    for (std::vector<std::pair<void *, Function *> >::iterator i = methods.begin();
                         i != methods.end();
                         i++)
                    {
                        if (ptr >= i->first /*&& std::string(i->second->getName()).compare(0, 4, "kite") != 0*/)
                        {
                            retValue = i->second->getName();
                            *beginPointer = i->first;
                            break;
                        }
                    }
                    
                    if (retValue.size() > 0)
                    {
                        // Found method name. Process and create 
                        // user-friendly name for return.
                        size_t idx = retValue.rfind("__");
                        if (idx != std::string::npos)
                        {
                            retValue.erase(idx);
                        }
                        idx = retValue.rfind("__");
                        if (idx == retValue.size() - 2)
                        {
                            // Operator methods (__init__, etc).
                            // Skip both __ entirely.
                            idx = retValue.rfind("__", idx - 2);
                            idx -= 2;
                        }
                        bool isFirst = true;
                        while (idx != std::string::npos && idx < retValue.size())
                        {
                            if (isFirst)
                            {
                                retValue.replace(idx, 2, "|");
                                isFirst = false;
                            }
                            else
                            {
                                retValue.replace(idx, 2, ".");
                            }
                            idx = retValue.rfind("__", idx);
                        }
                    }
                    
                    return retValue;
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
