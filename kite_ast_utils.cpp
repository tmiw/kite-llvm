/*****************************************************************************
 * Copyright (c) 2010, Mooneer Salem
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

#include "kite_ast_base.h"

namespace kite
{
	namespace parse_tree
	{
		
		CompilerState::CompilerState()
		: _moduleBuilder(getGlobalContext())
		{
			// empty
		}
		
		CompilerState::~CompilerState()
		{
			// empty
		}
		
		Module *CompilerState::pop_module()
		{
			Module *returnValue = _moduleStack.back();
			_moduleStack.pop_back();
			return returnValue;
		}
		
		void CompilerState::push_module(Module *module)
		{
			_moduleStack.push_back(module);
		}
		
		void CompilerState::push_symbol_stack()
		{
			_symbolTableStack.push_back(new std::map<const char*, Value*>());
		}
		
		void CompilerState::pop_symbol_stack()
		{
			std::map<const char*, Value*> *ptr = _symbolTableStack.back();
			delete ptr;
			_symbolTableStack.pop_back();
		}
		
		void CompilerState::push_child_tree(IAbstractTree *tree)
		{
			_childStack.push_back(tree);
		}
		
		void CompilerState::pop_child_tree()
		{
			_childStack.pop_back();
		}
		
		MultipleChildTrees::~MultipleChildTrees()
		{
			std::vector<IAbstractTree*>::iterator i = _instructionList.begin();
			for(; i != _instructionList.end(); i++)
			{
				delete *i;
			}
		}
		
		Value *MultipleChildTrees::iterate_all_instructions(CompilerState *state)
		{
			Value *returnValue = NULL;
			std::vector<IAbstractTree*>::iterator i = _instructionList.begin();
			for(; i != _instructionList.end(); i++)
			{
				returnValue = (*i)->codegen(state);
			}
			return returnValue;
		}
	}
}