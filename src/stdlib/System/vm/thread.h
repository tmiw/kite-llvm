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
 
#ifndef KITE_STDLIB__SYSTEM__VM__THREAD_H
#define KITE_STDLIB__SYSTEM__VM__THREAD_H

#include <pthread.h>
#include "stdlib/api.h"
#include "../vm.h"
#include "../list.h"
#include "../method.h"
#include "../exceptions/OsError.h"

namespace kite
{
    namespace stdlib
    {
        namespace System
        {
            namespace vm
            {
                BEGIN_KITE_BASE_CLASS(thread)
                private:
                    pthread_t thread_id;
                    method *start_method;
                    list *args;
                    
                    static object* s_exit(thread *t) { return t->exit_thread(); }
                    static object* s_getCurrent(thread *) { return thread::getCurrent(); }
                    static object *s_join(thread *t) { return t->join(); }
                    
                    static void *thread_start(void *arg)
                    {
                        thread *t = (thread*)arg;
                        t->start_method->invoke_with_arg_list(t->args);
                    }
                    
                    static object* s_initialize_2(thread *t, method *start_m, list *args)
                    {
                        t->start_method = start_m;
                        t->args = args;
                        int err = pthread_create(&t->thread_id, NULL, &thread::thread_start, (void*)t);
                        if (err != 0)
                        {
                            std::stringstream ss;

                            ss << "Could not create thread (errno " << err << ")";
                            kite::stdlib::System::exceptions::exception *exc = kite::stdlib::System::exceptions::OsError::Create(
                                1,
                                new kite::stdlib::System::string(
                                    ss.str().c_str()                                
                                ));
                            exc->throw_exception();
                        }
                        return t;
                    }
                    
                    static void s_destroy(thread *t)
                    {
                        pthread_cancel(t->thread_id);
                    }
                public:
                    object* exit_thread();
                    static object* getCurrent();
                    object *join();
                    
                    BEGIN_KITE_CLASS_INITIALIZER
                        KITE_CONSTRUCTOR_DEFINE(2, &thread::s_initialize_2);
                        KITE_DESTRUCTOR_DEFINE(&thread::s_destroy);
                        
                        KITE_METHOD_DEFINE(exit, 0, &thread::s_exit);
                        KITE_METHOD_DEFINE(getCurrent, 0, &thread::s_getCurrent);
                        KITE_METHOD_DEFINE(join, 2, &thread::s_join);
                    END_KITE_CLASS_INITIALIZER
                END_KITE_CLASS
            }
        }
    }
}

#endif