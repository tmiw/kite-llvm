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

#ifndef KITE_AST_CONST_BASE_H
#define KITE_AST_CONST_BASE_H

#include <assert.h>
#include "kite_ast_base.h"

// LLVM-related files.
#ifdef __cplusplus
#include "llvm/DerivedTypes.h"
#include "llvm/LLVMContext.h"
#include "llvm/Module.h"
#include "llvm/Analysis/Verifier.h"
#include "llvm/Support/IRBuilder.h"

using namespace llvm;
#endif // __cplusplus

namespace kite
{
	namespace parse_tree
	{
		/*
		 * Kite abstract syntax tree -- constant value.
		 */
		template<typename T>
		class ConstantValue : public IAbstractTree
		{
		public:
			ConstantValue(T value) /*! Creates new instance of the ConstantValue object. */
			: _value(value)
			{
				// empty.
			}
			virtual ~ConstantValue() /*! Destroys instance of the ConstantValue object. */
			{
				// empty.
			}
		
			virtual Value *codegen() /*! Generates bytecode for constant value. */
			{
				// By default, we don't know how to generate for whatever type
				// the developer throws at us. Therefore, a simple assert(0) will
				// exit the program here.
				assert(0);
				return NULL;
			}
		private:
			T _value;
		};
	
		template<>
		Value *ConstantValue<int>::codegen()
		{
			return ConstantInt::get(getGlobalContext(), APInt(_value, 32, true));
		}
	};
};

#ifdef __cplusplus
extern "C"
{
#endif // __cplusplus
	
	IAbstractTreePtr GenerateConstantIntegerTreeNode(int);

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // KITE_AST_BASE_H