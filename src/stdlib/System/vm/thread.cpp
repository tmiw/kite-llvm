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
#include "thread.h"
#include "../exceptions/OsError.h"

namespace kite
{
    namespace stdlib
    {
        namespace System
        {
            namespace vm
            {
                object* thread::exit_thread()
                {
                    int err = pthread_cancel(thread_id);
                    if (err != 0)
                    {
                        std::stringstream ss;
                        
                        ss << "Could not exit thread (errno " << err << ")";
                        kite::stdlib::System::exceptions::exception *exc = kite::stdlib::System::exceptions::OsError::Create(
                            1,
                            new kite::stdlib::System::string(
                                ss.str().c_str()                                
                            ));
                        exc->throw_exception();
                    }
                    return this;
                }
                
                object* thread::getCurrent()
                {
                    thread *t = new thread();
                    t->thread_id = pthread_self();
                    return t;
                }
                
                object *thread::join()
                {
                    int err = pthread_join(thread_id, NULL);
                    if (err != 0)
                    {
                        std::stringstream ss;
                        
                        ss << "Could not join thread (errno " << err << ")";
                        kite::stdlib::System::exceptions::exception *exc = kite::stdlib::System::exceptions::OsError::Create(
                            1,
                            new kite::stdlib::System::string(
                                ss.str().c_str()                                
                            ));
                        exc->throw_exception();
                    }
                    return this;
                }
            }
        }
    }
}

REGISTER_KITE_CLASS(kite::stdlib::System::vm::vm, kite::stdlib::System::vm::thread)