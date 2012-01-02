/*****************************************************************************
 * Copyright (c) 2012, Mooneer Salem
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
#include <dlfcn.h>
#include <stdlib/language/kite.h> 
#include "../exceptions.h"
#include "exception.h"

namespace kite
{
    namespace stdlib
    {
        namespace System
        {
            namespace exceptions
            {
                void exception::throw_exception()
                {
                    #define NUM_TRACE 32
                    void *buf[NUM_TRACE];
                    
                    // TODO
                    int num_traces = backtrace(buf, NUM_TRACE);
                    std::ostringstream ss;
                    for (int i = 0; i < num_traces; i++)
                    {
                        // Get friendly name based on LLVM info. If no friendly name,
                        // most likely from a C/C++ function.
                        void *methodPointer = NULL;
                        Dl_info sym_info;
                        dladdr(buf[i], &sym_info);
                        if (sym_info.dli_sname && sym_info.dli_sname[0] != '_')
                        {
                            std::string friendlyName = language::kite::kite::GetMethodNameFromPointer(buf[i], &methodPointer);
                            if (friendlyName.size() > 0)
                            {
                                // TODO: clean up method names for Kite operators.
                                bool breakOut = false;
                                if (friendlyName == "__static_init__")
                                {
                                    friendlyName = "(main program)";
                                }
                                else if (friendlyName.find("kite_") == 0) continue;
                                else friendlyName = std::string("method ") + friendlyName;
                                ss << "    in " << friendlyName << " + 0x" << std::hex << ((size_t)buf[i] - (size_t)methodPointer) << std::dec << std::endl;
                                if (breakOut) break;
                            }
/*                            else
                            {
                                ss << sym_info.dli_sname << "(" << sym_info.dli_fbase << ")" << std::endl;
                            }
                        }
                        else
                        {
                            ss << sym_info.dli_sname << "(" << sym_info.dli_fbase << ")" << std::endl;*/
                        }
                    }
                    properties["trace"] = new System::string(ss.str().c_str());
                    
                    if (language::kite::kite::exception_stack.size() > 0)
                    {
                        language::kite::kite::last_exception = this;
                        longjmp(*language::kite::kite::exception_stack.back(), 1);
                    }
                    else
                    {
                        // TODO
                        System::dynamic_object *real_exc_class = (System::dynamic_object*)this->parent;
                        std::cout << ((System::string*)real_exc_class->properties["__name"])->string_val << ": ";
                        properties["message"]->print();
                        properties["trace"]->print();
                        exit(-1);
                    }
                }
            }
        }
    }
}

REGISTER_KITE_CLASS(kite::stdlib::System::exceptions::exceptions, kite::stdlib::System::exceptions::exception)

using namespace kite::stdlib;

void *kite_exception_get()
{
    return (void*)language::kite::kite::last_exception;
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
