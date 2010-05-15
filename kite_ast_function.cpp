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
#include "kite_ast_function.h"

namespace kite
{
	namespace parse_tree
	{
		MethodValue::MethodValue(const char *name)
		: _name(name)
		{
			// empty
		}
		
		MethodValue::~MethodValue()
		{
			// empty
		}
		
		Value *MethodValue::codegen(CompilerState *state)
		{
			assert(state != NULL);
			
			Module *currentModule = state->current_module();
			FunctionType *FT = FunctionType::get(Type::getDoubleTy(getGlobalContext()), _parameters, false);
			Function *F = Function::Create(FT, Function::ExternalLinkage, _name, currentModule);
			
			// TODO: redefinition check and exception if needed.
			
			// Create arguments in named symbol table.
			unsigned int idx = 0;
			state->push_symbol_stack();
			for (Function::arg_iterator AI = F->arg_begin(); idx != _parameters.size();
				 ++AI, ++idx) 
			{
				AI->setName(_parameterNames[idx]);
				(state->current_symbol_stack())[_parameterNames[idx]] = AI;
			}
			
			// Create a new basic block to start insertion into.
			BasicBlock *BB = BasicBlock::Create(getGlobalContext(), "entry", F);
			IRBuilder<> &builder = state->module_builder();
			builder.SetInsertPoint(BB);
			
			if (Value *rv = this->iterate_all_instructions(state))
			{
				builder.CreateRet(rv);
			}
			
			state->pop_symbol_stack();
			verifyFunction(*F);
			return F;
		}
	}
}