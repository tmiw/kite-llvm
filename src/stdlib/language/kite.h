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
 
#ifndef KITE_STDLIB__LANGUAGE__KITE_H
#define KITE_STDLIB__LANGUAGE__KITE_H

#include <setjmp.h>
#include <map>
#include <stdlib/System/dynamic_object.h>
#include <codegen/llvm_compile_state.h>
#include <stdlib/language/kite/syntax_tree.h>

namespace llvm
{
    class Module;
    class ExecutionEngine;
}

namespace kite
{
    namespace stdlib
    {
        namespace language
        {
            namespace kite
            {
                struct kite : System::dynamic_object
                {
                    static System::dynamic_object *root() { return root_object; }

                    static void InitializeRuntimeSystem();
                    static System::object *ExecuteCode(syntax_tree &ast, System::object *context);
                    static System::object *ExecuteCode(syntax_tree &ast);
                    static void DumpCompiledCode();

                    static bool enable_optimizer;
                    static std::vector<jmp_buf*> exception_stack;
                    static System::dynamic_object *last_exception;
                    
                    private:
                        static llvm::Module *current_module;
                        static System::dynamic_object *root_object;
                        static codegen::llvm_compile_state state;
                        static ExecutionEngine *execution_engine;
                };
            }
        }
    }
}

#endif