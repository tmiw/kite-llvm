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

#include <iostream>
#include <algorithm>
#include <assert.h>
#include "stack.h"
#include "../integer.h"
#include "../exceptions/EmptyCollection.h"

namespace kite
{
    namespace stdlib
    {
        namespace System
        {
            namespace collections
            {
                object *stack::push(object *item)
                {
                    stack_contents.push_front(item);
                    return this;
                }
                
                object *stack::size()
                {
                    return new integer(stack_contents.size());
                }
                
                object *stack::pop()
                {
                    if (stack_contents.size() == 0)
                    {
                        exceptions::EmptyCollection *exc = exceptions::EmptyCollection::Create(
                            1,
                            new string("Stack is empty.")
                        );
                        exc->throw_exception();
                    }
                    
                    object *front_of_queue = stack_contents.front();
                    stack_contents.pop_front();
                    return front_of_queue;
                }   
            }
        }
    }
}


REGISTER_KITE_CLASS(kite::stdlib::System::collections::collections, kite::stdlib::System::collections::stack);