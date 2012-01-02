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
#include <algorithm>
#include <assert.h>
#include "array.h"
#include "../boolean.h"
#include "../method.h"
#include "../exceptions/InvalidArgument.h"

namespace kite
{
    namespace stdlib
    {
        namespace System
        {
            namespace collections
            {
                object *array::append_item(object *item)
                {
                    array *old_array = (array*)item;
                    array *new_array = new array();
                    new_array->array_contents = old_array->array_contents;
                    new_array->array_contents.push_back(item);
                    return new_array;
                }
                
                object *array::append_item_in_place(object *item)
                {
                    this->array_contents.push_back(item);
                    return this;
                }
                
                object *array::capacity()
                {
                    return new integer(this->array_contents.capacity());
                }
                
                object *array::equals(array *right)
                {
                    bool retval = false;
                    if (right->type == semantics::OBJECT && ((dynamic_object*)right)->parent == this->parent)
                    {
                        retval = 
                            !(this->array_contents == right->array_contents);
                    }
                    return new boolean(retval);
                }
                
                object *array::get(integer *index)
                {
                    if (index->type != semantics::INTEGER)
                    {
                        (exceptions::InvalidArgument::Create(1, new string("Argument 1 must be an integer.")))->throw_exception();
                    }
                    
                    return this->array_contents[index->val];
                }
                
                object *array::greater_than(object *item)
                {
                    bool retval = false;
                    if (item->type == semantics::OBJECT && ((dynamic_object*)item)->parent == this->parent)
                    {
                        array *right_array = (array*)item;
                        retval = 
                            !(this->array_contents == right_array->array_contents) && 
                            !(this->array_contents < right_array->array_contents);
                    }
                    return new boolean(retval);
                }
                
                object *array::greater_than_or_equal(object *item)
                {
                    bool retval = false;
                    if (item->type == semantics::OBJECT && ((dynamic_object*)item)->parent == this->parent)
                    {
                        array *right_array = (array*)item;
                        retval = 
                            !(this->array_contents < right_array->array_contents);
                    }
                    return new boolean(retval);
                }
                
                object *array::index_of(object *item)
                {
                    int j = 0;
                    for (array_contents_type::iterator i = this->array_contents.begin();
                         i != this->array_contents.end();
                         i++, j++)
                    {
                        if (*i == item)
                        {
                            return new integer(j);
                        }    
                    }
                    return new integer(-1);
                }
                
                object *array::less_than(object *item)
                {
                    bool retval = false;
                    if (item->type == semantics::OBJECT && ((dynamic_object*)item)->parent == this->parent)
                    {
                        array *right_array = (array*)item;
                        retval = 
                            this->array_contents < right_array->array_contents;
                    }
                    return new boolean(retval);
                }
                
                object *array::less_than_or_equal(object *item)
                {
                    bool retval = false;
                    if (item->type == semantics::OBJECT && ((dynamic_object*)item)->parent == this->parent)
                    {
                        array *right_array = (array*)item;
                        retval = 
                            (this->array_contents == right_array->array_contents) || 
                            (this->array_contents < right_array->array_contents);
                    }
                    return new boolean(retval);
                }
                
                object *array::map(object *m)
                {
                    if (m->type != semantics::METHOD_TY)
                    {
                        (exceptions::InvalidArgument::Create(1, new string("Argument 1 must be a method object.")))->throw_exception();
                    }
                    
                    method *method_obj = (method*)m;
                    
                    if (method_obj->num_args != 2)
                    {
                        (exceptions::InvalidArgument::Create(1, new string("Argument 1: method takes the wrong number of arguments.")))->throw_exception();
                    }
                    
                    array *retval = new array();
                    for (array_contents_type::iterator i = this->array_contents.begin();
                         i != this->array_contents.end();
                         i++)
                    {
                        retval->array_contents.push_back(
                            method_obj->invoke(*i)
                        );
                    }
                    
                    return retval;
                }
                
                object *array::not_equals(array *right)
                {
                    bool retval = false;
                    if (right->type == semantics::OBJECT && ((dynamic_object*)right)->parent == this->parent)
                    {
                        retval = 
                            !(this->array_contents == right->array_contents);
                    }
                    return new boolean(retval);
                }
                
                object *array::obj()
                {
                    return this;
                }
                
                object *array::reduce(object *m)
                {
                    if (m->type != semantics::METHOD_TY)
                    {
                        (exceptions::InvalidArgument::Create(1, new string("Argument 1 must be a method object.")))->throw_exception();
                    }
                    
                    method *method_obj = (method*)m;
                    
                    if (method_obj->num_args != 3)
                    {
                        (exceptions::InvalidArgument::Create(1, new string("Argument 1: method takes the wrong number of arguments.")))->throw_exception();
                    }
                    
                    object *tmpval = NULL;
                    for (array_contents_type::iterator i = this->array_contents.begin();
                         i != this->array_contents.end();
                         i++)
                    {
                        object *(*methodptr)(object *thisobj, object *result, object *element) =
                            (object *(*)(object *, object *, object *))method_obj->method_ptr;
                        tmpval =
                            (*methodptr)(method_obj->this_ptr, tmpval, *i);
                    }
                    
                    return tmpval;
                }
                
                object *array::print()
                {
                    string *strval = (string*)str();
                    std::cout << strval->string_val << std::endl;
                    return this;
                }
                
                object *array::reverse()
                {
                    array *retval = new array();
                    for (array_contents_type::reverse_iterator i = this->array_contents.rbegin();
                         i != this->array_contents.rend();
                         i++)
                    {
                        retval->array_contents.push_back(*i);
                    }
                    return retval;
                }
                
                object *array::set(integer *index, object *item)
                {
                    if (index->type != semantics::INTEGER)
                    {
                        (exceptions::InvalidArgument::Create(1, new string("Argument 1 must be an integer.")))->throw_exception();
                    }
                    
                    this->array_contents[index->val] = item;
                    return this;
                }
                
                object *array::size()
                {
                    return new integer(this->array_contents.size());
                }
                
                object *array::sort(object *comparator)
                {
                    array *retval = new array();
                    retval->array_contents = this->array_contents;
                    std::sort(retval->array_contents.begin(), retval->array_contents.end());
                    return retval;
                }
                
                object *array::str()
                {
                    std::string retval("[");
                    int index = 0;
                    for (array_contents_type::iterator i = this->array_contents.begin();
                         i != this->array_contents.end();
                         i++, index++)
                    {
                        if (index > 0)
                        {
                            retval += ", ";
                        }
                        retval += (*i)->as_string();
                    }
                    retval += "]";
                    return new string(retval.c_str());
                }
            }
        }
    }
}

REGISTER_KITE_CLASS(kite::stdlib::System::collections::collections, kite::stdlib::System::collections::array);