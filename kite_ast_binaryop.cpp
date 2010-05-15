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
#include "kite_ast_binaryop.h"

namespace kite
{
	namespace parse_tree
	{
		BinaryOperation::BinaryOperation(SupportedBinaryOps op, IAbstractTree *left, IAbstractTree *right)
		: _left(left), _right(right), _op(op)
		{
			// empty
		}

		BinaryOperation::~BinaryOperation()
		{
			delete _left;
			delete _right;
		}
		
		Value *BinaryOperation::codegen(CompilerState *state)
		{
			Value *retVal;
			Value *lhs = _left->codegen(state), *rhs = _right->codegen(state);
			IRBuilder<> &builder = state->module_builder();
			bool integer = lhs->getType()->isInteger();
			
			if (lhs->getType() == rhs->getType())
			{
				switch(_op)
				{
					case ADDITION:
						if (integer) retVal = builder.CreateAdd(lhs, rhs);
						else retVal = builder.CreateFAdd(lhs, rhs);
						break;
					case SUBTRACTION:
						if (integer) retVal = builder.CreateSub(lhs, rhs);
						else retVal = builder.CreateFSub(lhs, rhs);
						break;
					case MULTIPLICATION:
						if (integer) retVal = builder.CreateMul(lhs, rhs);
						else retVal = builder.CreateFMul(lhs, rhs);
						break;
					case DIVISION:
						if (integer) retVal = builder.CreateSDiv(lhs, rhs);
						else retVal = builder.CreateFDiv(lhs, rhs);
						break;
					default:
						assert(0);
				}
			}
			else 
			{
				assert(0);
			}

			return retVal;
		}
	}
}