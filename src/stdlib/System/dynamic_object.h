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
 
#ifndef KITE_STDLIB__SYSTEM__DYNAMIC_OBJECT_H
#define KITE_STDLIB__SYSTEM__DYNAMIC_OBJECT_H

#include <map>
#include <semantics/constants.h>
#include "object.h"
#include "method.h"
#include <gc/gc_allocator.h>

extern "C"
{
    void *kite_dynamic_object_alloc();
    void kite_dynamic_object_set_parent(void *object, void *parent);
    void kite_dynamic_object_set_name(void *object, char *name);
    void **kite_dynamic_object_get_property(void *object, char *name, bool set);
    void *kite_dynamic_object_get_root();
}
 
namespace kite
{
    namespace stdlib
    {
        namespace System
        {
            typedef gc_allocator<std::pair<const std::string, object*> > map_allocator;
            typedef std::less<std::string> map_compare;
            typedef std::map<std::string, object*, map_compare, map_allocator> property_map;
            struct dynamic_object : object
            {
                object *parent;
                void *obj_alloc_method;
                property_map properties;
                
                dynamic_object() 
                : object(semantics::OBJECT), 
                  parent(NULL),
                  obj_alloc_method((void*)&kite_dynamic_object_alloc) { }
                dynamic_object(object *p) 
                : object(semantics::OBJECT), 
                  parent(p), 
                  obj_alloc_method((void*)&kite_dynamic_object_alloc) { }
                
                void add_method(const char *name, int numargs, void *ptr)
                {
                    std::string real_name = std::string(name) + "__o";
                    for (int i = numargs; i > 0; i--)
                    {
                        real_name += "o";
                    }
                    
                    System::method *method = new System::method(ptr);
                    method->num_args = numargs;
                    properties[real_name] = method;
                }
                
                void add_operator(semantics::code_operation op, void *ptr)
                {
                    assert(op < semantics::__END_OVERRIDABLE_OPS && op != semantics::__END_BINARY_OPS);
                    if (op < semantics::__END_BINARY_OPS)
                    {
                        add_method(semantics::operator_map[op].c_str(), 1, ptr);
                    }
                    else
                    {
                        add_method(semantics::operator_map[op].c_str(), 0, ptr);
                    }
                }
            };
        }
    }
}

#endif
