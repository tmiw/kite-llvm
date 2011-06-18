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
 
#include "llvm_compile_state.h"

namespace kite
{
    namespace codegen
    {
        llvm_compile_state::llvm_compile_state()
	    : _moduleBuilder(getGlobalContext()), _overrideOverloadedProperties(false)
	    {
	        // empty
	    }
	               
	    llvm_compile_state::~llvm_compile_state()
	    {
	        // empty
	    }
	               
	    Module *llvm_compile_state::pop_module()
	    {
	        Module *returnValue = _moduleStack.back();
	        _moduleStack.pop_back();
	        return returnValue;
	    }
	               
        void llvm_compile_state::push_module(Module *module)
	    {
	        _moduleStack.push_back(module);
	    }
	               
	    void llvm_compile_state::push_symbol_stack()
	    {
	        _symbolTableStack.push_back(new std::map<std::string, Value*>());
	    }
	               
	    void llvm_compile_state::pop_symbol_stack()
	    {
	        std::map<std::string, Value*> *ptr = _symbolTableStack.back();
	        delete ptr;
	        _symbolTableStack.pop_back();
	    }

        std::string llvm_compile_state::identifier_prefix() const
        {
            std::string ret;
            std::vector<std::string>::const_iterator i = _namespaceStack.begin();

            for (; i != _namespaceStack.end(); i++)
            {
                ret += (*i);
                ret += "__";
            }
            return ret;
        }
        
        std::string llvm_compile_state::full_class_name() const
        {
            std::string ret;
            std::vector<std::string>::const_iterator i = _namespaceStack.begin();

            for (; i != _namespaceStack.end(); i++)
            {
                ret += (*i);
                ret += ".";
            }
            return ret.erase(ret.size() - 1, 1);
        }
    }
}
