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
#include "list.h"
#include "dynamic_object.h"
#include "exceptions/NotImplemented.h"
#include "exceptions/InvalidArgument.h"
#include "exceptions/NullReference.h"

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
                ("str__o", function_semantics(semantics::STRING, (void*)0))
                ("obj__o", function_semantics(semantics::OBJECT, (void*)&obj__o))
                ("doc__o", function_semantics(semantics::OBJECT, (void*)&doc__o))
                ("type__o", function_semantics(semantics::OBJECT, (void*)&type__o))
                ("is_class__o", function_semantics(semantics::OBJECT, (void*)&is_class__o))
                ("get_base_object__o", function_semantics(semantics::OBJECT, (void*)&get_base_object__o))
                ("list_properties__o", function_semantics(semantics::OBJECT, (void*)&list_properties__o))
                ("list_methods__o", function_semantics(semantics::OBJECT, (void*)&list_methods__o))
                ("get_method__ooo", function_semantics(semantics::OBJECT, (void*)&get_method__ooo))
                ("get_property__oo", function_semantics(semantics::OBJECT, (void*)&get_property__oo))
                ("get_property__os", function_semantics(semantics::OBJECT, (void*)&get_property__os))
                ("get_property_string__oo", function_semantics(semantics::OBJECT, (void*)&get_property_string__oo))
                ("get_property_string__os", function_semantics(semantics::OBJECT, (void*)&get_property_string__os));
                
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
                    void *(*deinit_method)(void *) = (void*(*)(void*))kite_find_funccall((void*)real_this, "__destruct__", 1);
                    if (deinit_method != NULL) (*deinit_method)((void*)real_this);    
                }
                GC_register_finalizer_ignore_self( GC_base(real_this), 0, 0, 0, 0 );
            }
            
            object *object::invoke_operator(semantics::code_operation op)
            {
                std::string method_name = semantics::Constants::Get().operator_map[op].c_str();
                object *(*func)(object *) = (object *(*)(object *))kite_find_funccall(this, method_name.c_str(), 1);
                return (*func)(this);
            }
            
            object *object::invoke_operator(semantics::code_operation op, object *rhs)
            {
                std::string method_name = semantics::Constants::Get().operator_map[op].c_str();
                object *(*func)(object *, object *) = (object *(*)(object *, object *))kite_find_funccall(this, method_name.c_str(), 2);
                return (*func)(this, rhs);
            }
            
            std::string object::as_string()
            {
                std::ostringstream res;
                switch(type)
                {
                    case semantics::INTEGER:
                        res << ((integer*)this)->val;
                        break;
                    case semantics::FLOAT:
                        res << ((fpnum*)this)->val;
                        break;
                    case semantics::BOOLEAN:
                        if (((boolean*)this)->val)
                            res << "true";
                        else
                            res << "false";
                        break;
                    case semantics::STRING:
                        res << ((string*)this)->string_val;
                        break;
                    case semantics::METHOD_TY:
                        res << "method";
                        break;
                    default:
                        char *(*funcptr)(object *) = (char *(*)(object *))kite_find_funccall((void*)this, "str", 1);
                        res << (*funcptr)(this);
                        break;
                }
                
                return res.str();
            }
        }
    }
}

using namespace kite::stdlib;

void System::object::print()
{
    std::cout << this->as_string() << std::endl;
}

void *kite_null_string(void *obj)
{
    assert(obj == NULL);
    return (void*)"null";
}

void *kite_print_null(void *obj)
{
    assert(obj == NULL);
    std::cout << "null" << std::endl;
    return obj;
}

void *kite_find_funccall(void *obj, const char *name, int numargs)
{
    System::object *obj_class = (System::object*)obj;
    std::string method_name = std::string(name) + "__";
    System::dynamic_object *dyn_object = NULL;
    
    if (obj == NULL && strncmp("str", name, 3) != 0 && strncmp("print", name, 5) != 0)
    {
        System::exceptions::NullReference *nre = System::exceptions::NullReference::Create(0);
        nre->throw_exception();
    }
    else if (obj == NULL && numargs == 1)
    {
        if (strncmp("str", name, 3) == 0)
            return (void*)&kite_null_string;
        else
            return (void*)&kite_print_null;
    }
    
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
            case kite::semantics::METHOD_TY:
                method_map = &System::method::method_map;
                break;
            default:
                assert(0);
        }
        
        object_method_map::iterator iter = method_map->find(method_name);
        if (iter != method_map->end())
        {
            return (void*)((*iter).second.second);
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
                    return (void*)met->method_ptr;
                }
                else
                {
                    std::ostringstream ss;
                    ss << name << " is not callable.";
                    System::exceptions::InvalidArgument *exception = System::exceptions::InvalidArgument::Create(
                        1,
                        new System::string(ss.str().c_str())
                    );
                    exception->throw_exception();
                }
            }
            dyn_object = (System::dynamic_object*)dyn_object->parent;
        } while (dyn_object != NULL);
        
failed_to_find_method:
        std::string theName(name);
        if ((theName == "__init__" && numargs > 1) || (theName != "__init__" && theName != "__destruct__"))
        {
            std::ostringstream ss;
            ss << "Could not find method " << name << " that takes " << (numargs - 1) << " argument(s).";
            System::exceptions::NotImplemented *exception = System::exceptions::NotImplemented::Create(
                1,
                new System::string(ss.str().c_str())
            );
            exception->throw_exception();
        }
        return NULL;
    }
}

bool kite_object_isof(void *lhs, void *rhs, bool type)
{
    System::dynamic_object *lhsObj = (System::dynamic_object*)lhs;
    System::dynamic_object *rhsObj = (System::dynamic_object*)rhs;
    bool ret = false;
    
    if (lhsObj == rhsObj ||
       (lhsObj != NULL && rhsObj != NULL && lhsObj->type == rhsObj->type && lhsObj->type != kite::semantics::OBJECT))
    {
        return true;
    }
    
    do
    {
        if (lhsObj->parent == rhsObj)
        {
            ret = true;
            break;
        }
        
        lhsObj = (System::dynamic_object*)lhsObj->parent;
    } while (type && lhsObj);
    
    return ret;
}

void *obj__o(void *obj)
{
    return obj;
}

void *get_property_string__oo(void *obj, void *prop)
{
    System::dynamic_object *object = (System::dynamic_object*)obj;
    if (object->type != kite::semantics::OBJECT)
    {
        return new System::string("");
    }
    
    System::string *key = (System::string*)prop;
    std::string val;
    do
    {
        val = object->property_docs[key->string_val.c_str()];
        object = (System::dynamic_object*)object->parent;
    } while (object && val.size() == 0);
    
    return new System::string(val.c_str());
}

void *get_property_string__os(void *obj, char *prop)
{
    System::dynamic_object *object = (System::dynamic_object*)obj;
    if (object->type != kite::semantics::OBJECT)
    {
        return new System::string("");
    }
    
    std::string val;
    do
    {
        val = object->property_docs[prop];
        object = (System::dynamic_object*)object->parent;
    } while (object && val.size() == 0);
    
    return new System::string(val.c_str());
}

void *get_property__oo(void *obj, void *prop)
{
    System::dynamic_object *object = (System::dynamic_object*)obj;
    if (object->type != kite::semantics::OBJECT)
    {
        return new System::string("");
    }
    
    System::string *key = (System::string*)prop;
    
    System::object *val;
    do
    {
        val = object->properties[key->string_val.c_str()];
        object = (System::dynamic_object*)object->parent;
    } while (object && object->properties.find(key->string_val.c_str()) != object->properties.end());
    
    return val;
}

void *get_property__os(void *obj, const char *prop)
{
    System::dynamic_object *object = (System::dynamic_object*)obj;
    if (object->type != kite::semantics::OBJECT)
    {
        return new System::string("");
    }
    
    System::object *val;
    do
    {
        val = object->properties[prop];
        object = (System::dynamic_object*)object->parent;
    } while (object && object->properties.find(prop) != object->properties.end());
    
    return val;
}

void *kite_set_docstring(void *obj, const char *str)
{
    ((System::dynamic_object*)obj)->doc_string = str;
    return obj;
}

void *kite_set_docstring_prop(void *obj, const char *name, const char *str)
{
    System::dynamic_object *objPtr = (System::dynamic_object*)obj;
    objPtr->property_docs[name] = str;
    return obj;
}

void *doc__o(void *obj)
{
    System::dynamic_object *object = (System::dynamic_object*)obj;
    return new System::string(object->doc_string.c_str());
}

void *get_base_object__o(void *obj)
{
    System::dynamic_object *object = (System::dynamic_object*)obj;
    return object->parent;
}

void *type__o(void *obj)
{
    System::dynamic_object *object = (System::dynamic_object*)obj;
    return *kite_dynamic_object_get_property(obj, "__name", false);
}

void *list_properties__o(void *obj)
{
    System::dynamic_object *object = (System::dynamic_object*)obj;
    System::list *retValue = System::list::Create(0);
    
    for (System::property_map::iterator i = object->properties.begin();
         i != object->properties.end();
         i++)
    {
        if (i->second == NULL || i->second->type != kite::semantics::METHOD_TY)
        {
            retValue->list_contents.push_back(new System::string(i->first.c_str()));
        }
    }
    
    return retValue;
}

void *list_methods__o(void *obj)
{
    System::dynamic_object *object = (System::dynamic_object*)obj;
    System::list *retValue = System::list::Create(0);
    std::map<std::string, std::vector<int> > funcMap;
    
    for (System::property_map::iterator i = object->properties.begin();
         i != object->properties.end();
         i++)
    {
        if (i->second != NULL && i->second->type == kite::semantics::METHOD_TY)
        {
            size_t pos = i->first.rfind("__");
            if (pos != std::string::npos)
            {
                std::string fName = i->first.substr(0, pos);
                int numArgs = i->first.substr(pos + 2).size() - 1;
                std::vector<int> &argList = funcMap[fName];
                if (std::find(argList.begin(), argList.end(), numArgs) == argList.end())
                {
                    argList.push_back(numArgs);
                }
            }
        }
    }
    
    for (std::map<std::string, std::vector<int> >::iterator i = funcMap.begin();
         i != funcMap.end();
         i++)
    {
        retValue->list_contents.push_back(new System::string(i->first.c_str()));
        System::list *argCounts = System::list::Create(0);
        for (std::vector<int>::iterator j = i->second.begin();
             j != i->second.end();
             j++)
        {
            argCounts->list_contents.push_back(new System::integer(*j));
        }
        retValue->list_contents.push_back(argCounts);
    }
    
    return retValue;
}

void *is_class__o(void *obj)
{
    System::dynamic_object *object = (System::dynamic_object*)obj;
    return new System::boolean(
        object != NULL &&
        object->type == kite::semantics::OBJECT &&
        object->properties.find("__name") != object->properties.end());
}

void *get_method__ooo(void *obj, void *prop, void *numargs)
{
    System::string *funcName = (System::string*)prop;
    System::integer *numArgs = (System::integer*)numargs;
    
    std::string fullName(funcName->string_val.c_str());
    fullName += "__";
    fullName += std::string(numArgs->val + 1, 'o');
    
    return get_property__os(obj, fullName.c_str());
}