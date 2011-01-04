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

#ifndef KITE_CODEGEN__LLVM_COMPILE_STATE_H
#define KITE_CODEGEN__LLVM_COMPILE_STATE_H

#include <map>
#include <vector>
#include <llvm/DerivedTypes.h>
#include <llvm/LLVMContext.h>
#include <llvm/Module.h>
#include <llvm/Analysis/Verifier.h>
#include <llvm/Support/IRBuilder.h>
using namespace llvm;

namespace kite
{
    namespace codegen
    {
        class llvm_compile_state
        {
        public:
            llvm_compile_state();
            virtual ~llvm_compile_state();
            
            void push_module(Module *module); /*! Pushes new module onto stack. */
            inline Module *current_module() { return _moduleStack.back(); }
            Module *pop_module(); /*! Pops module from top of stack. */
            
            inline IRBuilder<> &module_builder() { return _moduleBuilder; }
            
            void push_symbol_stack();
            inline std::map<std::string, Value*> &current_symbol_stack() { return *_symbolTableStack.back(); }
            inline std::vector<std::map<std::string, Value*> *> &symbol_stack() { return _symbolTableStack; }
            void pop_symbol_stack();
            
        private:
            std::vector<Module*> _moduleStack;
            std::vector<std::map<std::string, Value*> *> _symbolTableStack;
            IRBuilder<> _moduleBuilder;        
        };
    }
}

#endif