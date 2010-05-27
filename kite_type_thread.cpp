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

#include <vector>
#include "kite_type_thread.h"

using namespace std;
namespace kite
{
	namespace types
	{
		const Type *kite_thread_t::GetStructureType()
		{
			vector<const Type*> structureTypes;
			
			structureTypes.push_back(PointerType::getUnqual(Type::getVoidTy(getGlobalContext())));
			structureTypes.push_back(PointerType::getUnqual(Type::getVoidTy(getGlobalContext())));

			return StructType::get(getGlobalContext(), structureTypes);
		}

		const Type *kite_thread_t::GetPointerType()
		{
			return PointerType::getUnqual(GetStructureType());
		}
	}
}

using namespace kite::types;

void KitePushRuntimeValue(kite::types::kite_thread_t *thd, const char *name, void *value)
{
	(*thd->runtime_stack)[name].push(value);
}

void **KiteGetRuntimeValue(kite::types::kite_thread_t *thd, const char *name)
{
	return &(*thd->runtime_stack)[name].top();
}

void KitePopRuntimeValue(kite::types::kite_thread_t *thd, const char *name)
{
	(*thd->runtime_stack)[name].pop();
}
