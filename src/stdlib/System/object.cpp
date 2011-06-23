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
#include <sstream>
#include <boost/assign.hpp>
#include "object.h"
#include "string.h"
#include "integer.h"
#include "float.h"
#include "boolean.h"
#include "method.h"
#include "dynamic_object.h"
#include "exceptions/NotImplemented.h"
#include "exceptions/InvalidArgument.h"

using namespace boost::assign;
 
namespace kite
{
    namespace stdlib
    {
        namespace System
        {
            object_method_map object::method_map = map_list_of
                ("bool__o", function_semantics(semantics::BOOLEAN, (void*)0))
                ("int__o", function_semantics(semantics::INTEGER, (void*)0))
                ("float__o", function_semantics(semantics::FLOAT, (void*)0))
                ("print__o", function_semantics(semantics::OBJECT, (void*)0))
                ("obj__o", function_semantics(semantics::OBJECT, (void*)0));

            void object::finalizer_setup()
            {
                // Taken from gc_cpp.h. This seems to be to take into account
                // operator[] new.
                GC_finalization_proc proc = (GC_finalization_proc)cleanup;
                GC_finalization_proc oldProc;
                void* oldData;
                void* base = GC_base( (void *) this );
                if ( base != 0 )
                {
                    GC_register_finalizer_ignore_self( base, proc, (void*) ((char*) this - (char*) base), &oldProc, &oldData );

                    if ( oldProc != 0 )
                    {
                        GC_register_finalizer_ignore_self( base, oldProc, oldData, 0, 0 );
                    }
                }
            }

            void object::cleanup( void* obj, void* displ )
            {
                System::object *real_this = (System::object*)((char*) obj + (ptrdiff_t) displ);
                if (real_this->type == semantics::OBJECT) // only do destruction for non-trivial objects
                {
                    void *(*deinit_method)(void *) = (void*(*)(void*))kite_find_funccall((int*)real_this, "__destruct__", 1);
                    if (deinit_method != NULL) (*deinit_method)((void*)real_this);    
                }
                GC_register_finalizer_ignore_self( GC_base(real_this), 0, 0, 0, 0 );
            }
        }
    }
}

using namespace kite::stdlib;

void System::object::print()
{
    switch(type)
    {
        case semantics::INTEGER:
            ((integer*)this)->print();
            break;
        case semantics::FLOAT:
            ((fpnum*)this)->print();
            break;
        case semantics::BOOLEAN:
            ((boolean*)this)->print();
            break;
        case semantics::STRING:
            ((string*)this)->print();
            break;
        case semantics::METHOD_TY:
            ((method*)this)->print();
            break;
        default:
            std::cout << "object" << std::endl;
            break;

    }
}

int *kite_find_funccall(int *obj, char *name, int numargs)
{
    System::object *obj_class = (System::object*)obj;
    std::string method_name = std::string(name) + "__";
    System::dynamic_object *dyn_object = NULL;
    
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
            goto failed_to_find_method;
        }
    }
    else
    {
        dyn_object = (System::dynamic_object*)obj;
        do
        {
            System::property_map::iterator item = dyn_object->properties.find(method_name);
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
                    std::ostringstream ss;
                    ss << name << " is not callable.";
                    System::exceptions::InvalidArgument *exception = new System::exceptions::InvalidArgument(ss.str());
                    exception->throw_exception();
                }
            }
            dyn_object = (System::dynamic_object*)dyn_object->parent;
        } while (dyn_object != NULL);
        
        // TODO
failed_to_find_method:
        std::ostringstream ss;
        ss << "Could not find method " << name << " that takes " << (numargs - 1) << " argument(s).";
        System::exceptions::NotImplemented *exception = new System::exceptions::NotImplemented(ss.str());
        exception->throw_exception();
        
        return NULL;
    }
}

bool kite_object_isof(void *lhs, void *rhs, bool type)
{
    System::dynamic_object *lhsObj = (System::dynamic_object*)lhs;
    System::dynamic_object *rhsObj = (System::dynamic_object*)rhs;
    bool ret = false;
    
    do
    {
        // TODO: handle built-in types.
        if (lhsObj->parent == rhsObj)
        {
            ret = true;
            break;
        }
        
        lhsObj = (System::dynamic_object*)lhsObj->parent;
    } while (type && lhsObj);
    
    return ret;
}