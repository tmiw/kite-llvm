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

#ifndef KITE_TYPE_THREAD_H
#define KITE_TYPE_THREAD_H

#include <pthread.h>
#include <map>
#include <stack>
using namespace std;

#include "llvm/DerivedTypes.h"
#include "llvm/LLVMContext.h"
#include "llvm/Module.h"
#include "llvm/Analysis/Verifier.h"
#include "llvm/Support/IRBuilder.h"
using namespace llvm;

#ifndef __cplusplus
struct kite_thread_t;
#endif // !__cplusplus

#ifdef __cplusplus
namespace kite
{
	namespace types
	{
		struct kite_thread_t
		{
			pthread_t *thread_pointer;
			map<const char*, stack<void*> > *runtime_stack;
			
			kite_thread_t() : runtime_stack(new map<const char*, stack<void*> >()) { }
			
			static const Type* GetStructureType();
			static const Type* GetPointerType();
		};
	}
}

extern "C"
{
#endif // __cplusplus

	void KitePushRuntimeValue(struct kite_thread_t *thd, const char *name, void *value);
	void **KiteGetRuntimeValue(struct kite_thread_t *thd, const char *name);
	void KitePopRuntimeValue(struct kite_thread_t *thd, const char *name);

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // KITE_TYPE_THREAD_H