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

#include <sstream>
#include <execinfo.h>
#include <stdlib/language/kite.h> 
#include "exception.h"

namespace kite
{
    namespace stdlib
    {
        namespace System
        {
            namespace exceptions
            {
                System::dynamic_object exception::class_object;
                
                void exception::throw_exception()
                {
                    #define NUM_TRACE 32
                    void *buf[NUM_TRACE];
                    char **syms;
                    
                    // TODO
                    backtrace(buf, NUM_TRACE);
                    syms = backtrace_symbols(buf, NUM_TRACE);
                    std::ostringstream ss;
                    for (int i = 0; i < NUM_TRACE; i++)
                    {
                        if (!syms[i] || !*syms[i]) break;
                        ss << syms[i] << std::endl;
                    }
                    free(syms);
                    properties["trace"] = new System::string(ss.str().c_str());
                    
                    if (language::kite::kite::exception_stack.size() > 0)
                    {
                        language::kite::kite::last_exception = this;
                        longjmp(*language::kite::kite::exception_stack.back(), 1);
                    }
                    else
                    {
                        // TODO
                        properties["message"]->print();
                        properties["trace"]->print();
                        exit(-1);
                    }
                }
                
                void exception::InitializeClass()
                {
                    class_object.properties["throw__o"] =
                        new System::method((void*)kite_exception_throw);
                    class_object.properties["__init____o"] =
                        new System::method((void*)kite_exception_init);
                }
            }
        }
    }
}

using namespace kite::stdlib;

void *kite_exception_get()
{
    return (void*)language::kite::kite::last_exception;
}

void *kite_exception_init(void *exc)
{
    System::exceptions::exception *exception = (System::exceptions::exception*)exc;
    exception->initialize();
    return exc;
}

void *kite_exception_throw(void *exc)
{
    System::exceptions::exception *exception = (System::exceptions::exception*)exc;
    exception->throw_exception();
    return NULL;
}

void kite_exception_stack_push(jmp_buf *buf)
{
    language::kite::kite::exception_stack.push_back(buf);
}

void kite_exception_stack_pop()
{
    language::kite::kite::exception_stack.pop_back();
    language::kite::kite::last_exception = NULL;
}