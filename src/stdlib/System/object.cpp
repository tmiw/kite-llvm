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
#include <boost/assign.hpp>
#include "object.h"
#include "string.h"
#include "integer.h"
#include "float.h"
#include "boolean.h"
#include "method.h"
#include "dynamic_object.h"

using namespace boost::assign;
 
namespace kite
{
    namespace stdlib
    {
        namespace System
        {
            object_method_map object::method_map;
        }
    }
}

using namespace kite::stdlib;

int *kite_find_funccall(int *obj, char *name, int numargs)
{
    System::object *obj_class = (System::object*)obj;
    std::string method_name = std::string(name) + "__";
    
    for (int i = 0; i < numargs; i++)
    {
        method_name += "o";
    }
    
    if (obj_class->type != kite::semantics::OBJECT)
    {
        object_method_map *method_map;
        switch(obj_class->type)
        {
            case kite::semantics::STRING:
                method_map = &System::string::method_map;
                break;
            case kite::semantics::INTEGER:
                method_map = &System::integer::method_map;
                break;
            case kite::semantics::FLOAT:
                method_map = &System::fpnum::method_map;
                break;
            case kite::semantics::BOOLEAN:
                method_map = &System::boolean::method_map;
                break;
            default:
                assert(0);
        }
        
        object_method_map::iterator iter = method_map->find(method_name);
        if (iter != method_map->end())
        {
            return (int*)((*iter).second.second);
        }
        else
        {
            // TODO
            assert(0);
        }
    }
    else
    {
        System::dynamic_object *dyn_object = (System::dynamic_object*)obj;
        do
        {
            System::property_map::iterator item = dyn_object->properties.find(name);
            if (item != dyn_object->properties.end())
            {
                System::object *method_obj = (*item).second;
                if (method_obj->type == kite::semantics::METHOD_TY)
                {
                    System::method *met = (System::method*)method_obj;
                    return (int*)met->method_ptr;
                }
                else
                {
                    // TODO
                    assert(0);
                }
            }
            dyn_object = (System::dynamic_object*)dyn_object->parent;
        } while (dyn_object != NULL);
        
        // TODO
        assert(0);
    }
}