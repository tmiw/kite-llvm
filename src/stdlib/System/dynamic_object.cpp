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

#include "dynamic_object.h"
#include "string_type.h"
#include "method.h"
#include "integer.h"
#include "../language/kite.h"
#include "exceptions/NullReference.h"

namespace kite
{
    namespace stdlib
    {
        namespace System
        {
            dynamic_object dynamic_object::t_class_object;
            
            void dynamic_object::add_method(const char *name, int numargs, void *ptr)
            {
                std::string real_name = std::string(name) + "__o";
                for (int i = numargs; i > 0; i--)
                {
                    real_name += "o";
                }
                
                System::method *method = new System::method(ptr);
                method->num_args = numargs;
                properties[real_name.c_str()] = method;
            }
            
            void dynamic_object::add_operator(semantics::code_operation op, void *ptr)
            {
                assert(op < semantics::__END_OVERRIDABLE_OPS && op != semantics::__END_BINARY_OPS);
                if (op < semantics::__END_BINARY_OPS)
                {
                    add_method(semantics::Constants::Get().operator_map[op].c_str(), 1, ptr);
                }
                else
                {
                    add_method(semantics::Constants::Get().operator_map[op].c_str(), 0, ptr);
                }
            }
            
            void dynamic_object::InitializeClass()
            {
                t_class_object.properties["__name"] = new System::string("System.object");
                t_class_object.add_method("str", 0, (void*)&s_as_string);
                
                // Operator management.
                t_class_object.properties["operators"] = new System::dynamic_object();
                dynamic_object *operator_obj = (dynamic_object*)t_class_object.properties["operators"];
                operator_obj->properties["__name"] = new System::string("System.object.operators");
                operator_obj->properties["number_of_operators"] = new System::integer(semantics::__END_OVERRIDABLE_OPS);
                operator_obj->properties["to_str__oo"] = new System::method((void*)&kite_dynamic_object_get_op_name);
            }
            
            const char *dynamic_object::s_as_string(dynamic_object *obj)
            {
                System::string *str = (System::string*)obj->properties["__name"];
                return str->string_val.c_str();
            }
        }
    }
}
using namespace kite::stdlib;

void *kite_dynamic_object_alloc()
{
    return (void*)(new System::dynamic_object());
}

void kite_dynamic_object_set_parent(void *object, void *parent)
{
    ((System::dynamic_object*)object)->parent = (System::object*)parent;
    
    // Enable finalizer if needed.
    void **ptr = kite_dynamic_object_get_property(object, "__destruct____o", false);
    if (*ptr != NULL)
    {
        System::method *method = (System::method*)*ptr;
        kite_dynamic_object_enable_finalizer(object, method->method_ptr);
    }
    else
    {
        // Erase the item get_property created.
        ((System::dynamic_object*)object)->properties.erase("__destruct____o");
    }
}

void kite_dynamic_object_set_name(void *object, char *name)
{
    ((System::dynamic_object*)object)->properties["__name"] = new System::string(name);
}

void **kite_dynamic_object_get_property(void *object, char *name, bool set)
{
    System::dynamic_object *castedObj = (System::dynamic_object*)object;
    System::dynamic_object *cur = castedObj;
    void **ret = NULL;

    if (object == NULL)
    {
        System::exceptions::NullReference *nre = System::exceptions::NullReference::Create(0);
        nre->throw_exception();
    }
    
    if (!set)
    {
        do
        {
            if (cur->properties.find(name) != cur->properties.end())
            {
                ret = (void**)&cur->properties[name];
                break;
            }
            cur = (System::dynamic_object*)cur->parent;
        } while (cur);

        if (!ret)
        {
            // Look in root object for the desired symbol.
            cur = language::kite::kite::root();
            if (cur->properties.find(name) != cur->properties.end())
            {
                ret = (void**)&cur->properties[name];
            }
        }
        
        if (!ret)
        {
            castedObj->properties[name] = NULL;
            ret = (void**)&castedObj->properties[name];
        }
    }
    else
    {
        ret = (void**)&castedObj->properties[name];
    }
    return ret;
}

void *kite_dynamic_object_get_root()
{
    return language::kite::kite::root_object;
}

void kite_dynamic_object_enable_finalizer(void *object, void *func)
{
    // Taken from gc_cpp.h. This seems to be to take into account
    // operator[] new.
    GC_finalization_proc proc = (GC_finalization_proc)&System::object::cleanup;
    GC_finalization_proc oldProc;
    void* oldData;
    void* base = GC_base( (void *) object );
    if ( base != 0 )
    {
        GC_register_finalizer_ignore_self( base, proc, (void*) ((char*) object - (char*) base), &oldProc, &oldData );

        if ( oldProc != 0 )
        {
            GC_register_finalizer_ignore_self( base, oldProc, oldData, 0, 0 );
        }
    }
}

void *kite_dynamic_object_get_op_name(void *object, void *idx)
{
    System::integer *opId = (System::integer*)idx;
    kite::semantics::code_operation op = (kite::semantics::code_operation)opId->val;
    kite::semantics::Constants &constants = kite::semantics::Constants::Get();
    std::string &funcName = constants.operator_map[op];
    
    for (
        kite::semantics::OperatorMethodNameMap::iterator i = constants.operator_name_map.begin();
        i != constants.operator_name_map.end();
        i++)
    {
        if (i->second == funcName)
        {
            return new System::string(i->first.c_str());
        }
    }
    
    return NULL;
}