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

// NOTE: lots of unit test failures on OSX and eval does not work on Linux
// with the below enabled. Wait for future LLVM release to reenable.
#define ENABLE_ENHANCED_JIT

#include <algorithm>
#include <sys/stat.h>
#include <dlfcn.h>
#include <parser/parser.h>
#include <stdlib/System/dynamic_object.h>
#include <stdlib/System/method.h>
#include <stdlib/System/os.h>
#include <stdlib/System/list.h>
#include <stdlib/System/regex.h>
#include <stdlib/System/vm/loader.h>
#include <stdlib/System/exceptions/FileError.h>
#include <codegen/syntax_tree_printer.h>
#include <codegen/llvm_node_codegen.h>
#include <llvm/IR/LLVMContext.h>
#include <llvm/IR/LegacyPassManager.h>
#include <llvm/ExecutionEngine/ExecutionEngine.h>
#include <llvm/Support/MemoryBuffer.h>
#include <llvm/Support/DynamicLibrary.h>
#ifdef ENABLE_ENHANCED_JIT
#include <llvm/ExecutionEngine/MCJIT.h>
#else
#include <llvm/ExecutionEngine/Interpreter.h>
#endif
#include <llvm/IR/PassManager.h>
#include <llvm/IR/Verifier.h>
#include <llvm/Analysis/Passes.h>
#include <llvm/Support/TargetSelect.h>
#include <llvm/Transforms/Scalar.h>
#include <llvm/Support/raw_ostream.h>
#include <llvm/Target/TargetOptions.h>
#include <llvm/Transforms/IPO/PassManagerBuilder.h>

using namespace llvm;
using namespace kite::codegen;

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
                semantics::gc_vector<jmp_buf*>::type kite::exception_stack;
                System::dynamic_object *kite::last_exception = NULL;
                char *kite::app_name = NULL;
                
                void kite::InitializeRuntimeSystem(char *appname, int argc, char **argv)
                {
                    app_name = appname;
                    
                    InitializeNativeTarget();
                    InitializeNativeTargetAsmPrinter();
                    assert(llvm::llvm_is_multithreaded());

                    // Ensure that the standard library can be seen by LLVM during JIT compilation.
                    llvm::sys::DynamicLibrary::LoadLibraryPermanently(nullptr);

                    current_module = new Module("__root_module", KiteGlobalContext);
                    state.push_module(current_module);
                    
                    System::dynamic_object *system_obj = (System::dynamic_object*)root()->properties["System"];
                    system_obj->properties["float"] = &System::fpnum::class_object;
                    system_obj->properties["integer"] = &System::integer::class_object;
                    system_obj->properties["boolean"] = &System::boolean::class_object;
                    system_obj->properties["method"] = &System::method::class_obj;
                    system_obj->properties["object"] = &System::dynamic_object::class_object();
                    system_obj->properties["string"] = &System::string::class_object;
                    System::fpnum::InitializeClass();
                    System::integer::InitializeClass();
                    System::boolean::InitializeClass();
                    System::method::InitializeClass();
                    System::dynamic_object::InitializeClass();
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
                        std::string tmp_string(module_name, 0, split_pos);
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
                    state.push_import_namespace_stack();
                    int size = module_name_list.size();
                    for (int index = 0; index < size; index++)
                    {
                        state.push_namespace_stack(module_name_list[index].c_str());
                        
                        std::string full_path;
                        System::dynamic_object *the_object = new System::dynamic_object();
                        the_object->properties["__name"] = new System::string(state.full_class_name().c_str());
                        
                        for (System::list_contents_type::iterator pos = search_path->list_contents.begin(); pos != search_path->list_contents.end(); pos++)
                        {
                            struct stat st;
                            
                            // TODO: support C based Kite modules.
                            full_path = std::string(((System::string*)(*pos))->string_val.c_str()) + "/" + module_load_list[index] + ".kt";
                            if (stat(full_path.c_str(), &st) == 0)
                            {
                                break;
                            }
                            full_path = "";
                        }
                        
                        if (context->properties[module_name_list[index].c_str()] != NULL) 
                        {
                            context = (System::dynamic_object*)context->properties[module_name_list[index].c_str()];
                            continue;
                        }
                        
                        if (full_path.size() == 0)
                        {
                            if (index == size - 1)
                            {        
                                for (; index > 0; index--)
                                {
                                    state.pop_namespace_stack();
                                }
                                state.pop_import_namespace_stack();
                                System::exceptions::exception *exc = 
                                    System::exceptions::FileError::Create(
                                        1,
                                        new System::string(
                                            (std::string("Could not find ") + 
                                             module_load_list[index] + 
                                             " in load path.").c_str())
                                    );
                                exc->throw_exception();
                                return NULL;
                            }
                            context->properties[module_name_list[index].c_str()] = the_object;
                        }
                        else
                        {
                            language::kite::syntax_tree ast;
                            if (ast.from_file(full_path))
                            {
                                context->properties[module_name_list[index].c_str()] = the_object;
                                ExecuteCode(ast, the_object);
                            }
                            else
                            {
                                for (; index > 0; index--)
                                {
                                    state.pop_namespace_stack();
                                }
                                state.pop_import_namespace_stack();
                                return NULL;
                            }
                        }
                        
                        context = the_object;
                    }
                    
                    for (; size > 0; size--)
                    {
                        state.pop_namespace_stack();
                    }
                    state.pop_import_namespace_stack();
                    return context;
                }
                
                // Returns a pointer to a function.
                void *kite::GenerateEvalMethod(System::object *code, int numArgs, va_list vl)
                {
                    codegen::llvm_node_codegen cg(state);
                    System::string *codeString = (System::string*)code;

                    // Create list of argument names.
                    std::vector<std::string> argNames;
                    for (int i = 0; i < numArgs; i++)
                    {
                        argNames.push_back(va_arg(vl, char*));
                    }
                    
                    // Throws exception on parse error.
                    syntax_tree st;
                    std::string codeStr(codeString->string_val.c_str());
                    st.from_string(codeStr);
                    
                    // Generate LLVM Function* object.
                    semantics::syntax_tree fake_ast;
                    fake_ast.position.line = 1;
                    fake_ast.position.column = 1;
                    fake_ast.position.file = st.ast->position.file;
                    Function *function = (Function*)cg.generate_llvm_eval_method(argNames, *st.ast, fake_ast);
                    
                    //current_module->dump();
                    return (void*)execution_engine->getFunctionAddress(function->getName().str());
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
                    fake_ast.position.file = ast.ast->position.file;
                    Function *function = (Function*)cg.generate_llvm_method("__static_init__", argNames, *ast.ast, fake_ast);

                    if (enable_optimizer)
                    {
                        llvm::legacy::FunctionPassManager FPM(current_module);
                        llvm::PassManagerBuilder builder;
                        builder.OptLevel = 2;
                        builder.populateFunctionPassManager(FPM);
                        ///  Builder.populateModulePassManager(MPM);

                        FPM.doInitialization();
                        FPM.run(*function);
                    }

                    //current_module->dump();
                    verifyFunction(*function);
                    
                    if (!suppressExec)
                    {
                        if (state.identifier_prefix().size() > 0)
                        {
                            // Write LLVM code to manually call the generated function
                            // to ensure correct initialization.
                            Value *val = ConstantInt::get(KiteGlobalContext, APInt(sizeof(void*)*8, (uint64_t)context, false));
                            val = state.module_builder().CreateIntToPtr(val, PointerType::getUnqual(Type::getInt32Ty(KiteGlobalContext)));
                            state.module_builder().CreateCall(function, val);
                        }
                        else
                        {
                            if (execution_engine == NULL)
                            {
                                state.current_debug_builder()->finalize();
                                llvm::EngineBuilder engineBuilder((std::unique_ptr<Module>(current_module)));
                                std::string error;
                                engineBuilder.setErrorStr(&error);
#ifdef ENABLE_ENHANCED_JIT
                                engineBuilder.setEngineKind(EngineKind::Either);
#else
                                engineBuilder.setEngineKind(EngineKind::Interpreter);
#endif // ENABLE_ENHANCED_JIT
                                execution_engine = engineBuilder.create();
                                if (execution_engine == nullptr)
                                {
                                    std::cerr << "LLVM engine creation error: " << error << std::endl;
                                    assert(0);
                                }
                            }

                            void *fptr = (void*)execution_engine->getFunctionAddress(function->getName().str());
                            System::object *(*FP)(System::object *) = (System::object*(*)(System::object*))fptr;
                            return (*FP)(context);
                        }
                    }
                    else
                    {
                        return NULL;
                    }
                }

                void kite::DumpCompiledCode()
                {
                    current_module->print(llvm::errs(), nullptr);
                }
                
                static bool sort_pointers(std::pair<void *, Function*> i, std::pair<void *, Function*> j)
                {
                    return i.first > j.first;
                }
                
                std::string kite::GetMethodNameFromPointer(void *ptr, void **beginPointer)
                {
#ifndef ENABLE_ENHANCED_JIT
                    if (execution_engine == NULL)
                    {
                        // most likely thrown exception while trying to load
                        // a module. Can't get any method names in that case.
                        return "";
                    }
                    
                    // We need to go through every function object in
                    // the LLVM Module object to find the correct function
                    // name. This might cause performance issues if called often.
                    // TODO: evaluate for possible performance improvement.
                    // TODO: this method will break once we begin compiling to ELF binary files.
                    std::string retValue;
                    std::vector<std::pair<void *, Function*> > methods;
                    for (Module::iterator i = current_module->begin();
                         i != current_module->end();
                         i++)
                    {
                        //if (std::string(fxn->getName()).compare(0, 4, "kite") == 0) continue;
                        void *fxn_begin_ptr = (void*)execution_engine->getFunctionAddress(i->getName().str()); 
                        methods.push_back(std::pair<void*, Function*>(fxn_begin_ptr, &*i));
                    }
                    std::sort(methods.begin(), methods.end(), sort_pointers);
                    
                    for (std::vector<std::pair<void *, Function*> >::iterator i = methods.begin();
                         i != methods.end();
                         i++)
                    {
                        if (ptr >= i->first /*&& std::string(i->second->getName()).compare(0, 4, "kite") != 0*/)
                        {
                            retValue = i->second->getName().str();
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
#else
                    return "";
#endif
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
