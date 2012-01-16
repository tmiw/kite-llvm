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
 
#ifndef KITE_STDLIB__SYSTEM__OBJECT_H
#define KITE_STDLIB__SYSTEM__OBJECT_H

#include <gc.h>
#include <string>
#include <map>
#include <semantics/constants.h>

namespace kite
{
    namespace stdlib
    {
        // TODO
        typedef std::pair<semantics::builtin_types, void*> function_semantics;
        typedef std::map<std::string, function_semantics> object_method_map;
        
        namespace System
        {
            struct object
            {
                semantics::builtin_types type;
                static object_method_map method_map;
                
                object() : type(semantics::OBJECT) { }
                object(semantics::builtin_types type) : type(type) { }

                void print();
                std::string as_string();
                
                // For Boehm GC. Not using gc_cleanup to avoid the memory
                // penalty that virtual methods cause.
                void finalizer_setup();
                static void cleanup( void* obj, void* displ );
                inline void* operator new (std::size_t size)
                {
                    System::object *obj = (System::object*)GC_MALLOC(size);
                    obj->finalizer_setup();
                    return (void*)obj;
                }
            };
        }
    }
}

extern "C"
{
    void *kite_find_funccall(void *obj, const char *name, int numargs);
    bool kite_object_isof(void *lhs, void *rhs, bool type);
    void *obj__o(void *obj);
}

#endif
