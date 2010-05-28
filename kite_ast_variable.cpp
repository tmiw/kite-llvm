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
#include "kite_ast_variable.h"
#include "kite_type_object.h"

using namespace kite::types;

namespace kite
{
	namespace parse_tree
	{
		VariableValue:: VariableValue(const char *name)
		: _name(name)
		{
			// empty
		}
		
		VariableValue::~VariableValue()
		{
			// empty
		}
		
		Value *VariableValue::codegen(CompilerState *state)
		{
			assert(state != NULL);
			
			// find variable in state object
			// TODO: find from this and add dynamic lookup from shadow stack.
			for (std::vector<std::map<const char *, Value*> *>::reverse_iterator i = state->symbol_stack().rbegin(); i != state->symbol_stack().rend(); i++)
			{
				if ((*i)->count(_name) > 0)
				{
					return (*(*i))[_name];
				}
			}

			// allocate new value in stack and initialize to null.
			IRBuilder<> &builder = state->module_builder();
			Value *result = builder.CreateAlloca(kite_object_t::GetPointerType());
			std::map<const char *, Value*> &current_stack = state->current_symbol_stack();
			
			builder.CreateStore(ConstantPointerNull::get(static_cast<const PointerType*>(kite_object_t::GetPointerType())), result);
			current_stack[_name] = result;
			return current_stack[_name];
		}
	}
}