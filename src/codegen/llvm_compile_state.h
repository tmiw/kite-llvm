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
#include <llvm/Analysis/DIBuilder.h>
#include <llvm/Analysis/DebugInfo.h>
#include "../semantics/syntax_tree.h"

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
            
            void push_loop(BasicBlock *loop); /*! Pushes new loop onto stack. */
            inline BasicBlock *current_loop() { return _loopStack.back(); }
            BasicBlock *pop_loop(); /*! Pops loop from top of stack. */
            
            void push_loop_end(BasicBlock *loop); /*! Pushes new loop onto stack. */
            inline BasicBlock *current_loop_end() { return _loopEndStack.back(); }
            BasicBlock *pop_loop_end(); /*! Pops loop from top of stack. */
            
            void push_friendly_method_name(std::string name); /*! Pushes new method onto stack. */
            inline std::string current_friendly_method_name() { return _methodStack.back(); }
            std::string pop_friendly_method_name(); /*! Pops method from top of stack. */
            
            void push_c_method_name(std::string name); /*! Pushes new method onto stack. */
            inline std::string current_c_method_name() { return _cMethodStack.back(); }
            std::string pop_c_method_name(); /*! Pops method from top of stack. */
            
            void push_class_from(semantics::syntax_tree *tree) { _classInheritStack.push_back(tree); }
            void pop_class_from() { _classInheritStack.pop_back(); }
            semantics::syntax_tree *class_from() { return _classInheritStack.back(); }
            
            inline IRBuilder<> &module_builder() { return _moduleBuilder; }
            
            void push_symbol_stack();
            inline std::map<std::string, Value*> &current_symbol_stack() { return *_symbolTableStack.back(); }
            inline std::vector<std::map<std::string, Value*> *> &symbol_stack() { return _symbolTableStack; }
            void pop_symbol_stack();
           
            void push_namespace_stack(std::string name) { _namespaceStack.push_back(name); }
            void pop_namespace_stack() { _namespaceStack.pop_back(); }
            std::string identifier_prefix() const;
            std::string full_class_name() const;

            inline bool overrideOverloadedProperties() { return _overrideOverloadedProperties; }
            void overrideOverloadedProperties(bool val) { _overrideOverloadedProperties = val; }

            inline void skip_remaining(bool val) { _skipRemainingStatements = val; }
            inline bool get_skip_remaining() { return _skipRemainingStatements; }

            std::map<std::string, MDNode*> compileUnitCache;            
            std::map<std::string, DISubprogram> subroutineCache;
            std::map<std::string, DILexicalBlock> lexicalBlockCache;

            inline DIBuilder *current_debug_builder() { return _debugBuilderStack.back(); }
        private:
            void push_debug_builder(DIBuilder *builder); /*! Pushes new debug builder onto stack. */
            DIBuilder *pop_debug_builder(); /*! Pops debug builder from top of stack. */
            
            std::vector<DIBuilder*> _debugBuilderStack;
            std::vector<std::string> _namespaceStack;
            std::vector<Module*> _moduleStack;
            std::vector<BasicBlock*> _loopStack;
            std::vector<BasicBlock*> _loopEndStack;
            std::vector<std::string> _methodStack;
            std::vector<std::string> _cMethodStack;
            std::vector<semantics::syntax_tree*> _classInheritStack;
            
            std::vector<std::map<std::string, Value*> *> _symbolTableStack;
            IRBuilder<> _moduleBuilder;
            bool _overrideOverloadedProperties;
            bool _skipRemainingStatements;
        };
    }
}

#endif
