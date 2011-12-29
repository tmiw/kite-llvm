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
 
#ifndef KITE_STDLIB__SYSTEM_COLLECTIONS__ARRAY_H
#define KITE_STDLIB__SYSTEM_COLLECTIONS__ARRAY_H

#include <vector>
#include "stdlib/System/collections.h"
#include "../integer.h"

namespace kite
{
    namespace stdlib
    {
        namespace System
        {
            namespace collections
            {
                typedef std::vector<object*, gc_allocator<object*> > array_contents_type;
            
                BEGIN_KITE_BASE_CLASS(array)
                    private:
                        static object *s_append_item(array *list, object *item) { return list->append_item(item); }
                        static object *s_append_item_in_place(array *list, object *item) { return list->append_item_in_place(item); }
                        static object *s_capacity(array *list) { return list->capacity(); }
                        static object *s_equals(array *left, array *right) { return left->equals(right); }
                        static object *s_not_equals(array *left, array *right) { return left->not_equals(right); }
                        static object *s_get(array *list, integer *index) { return list->get(index); }
                        static object *s_greater_than(array *list, object *item) { return list->less_than(item); }
                        static object *s_greater_than_or_equal(array *list, object *item) { return list->less_than_or_equal(item); }
                        static object *s_index_of(array *list, object *item) { return list->index_of(item); }
                        static object *s_less_than(array *list, object *item) { return list->less_than(item); }
                        static object *s_less_than_or_equal(array *list, object *item) { return list->less_than_or_equal(item); }
                        static object *s_map(array *list, object *method) { return list->map(method); }
                        static object *s_obj(array *list) { return list->obj(); }
                        static object *s_print(array *list) { return list->print(); }
                        static object *s_reduce(array *list, object *method) { return list->reduce(method); }
                        static object *s_reverse(array *list) { return list->reverse(); }
                        static object *s_set(array *list, integer *index, object *item) { return list->set(index, item); }
                        static object *s_size(array *list) { return list->size(); }
                        static object *s_sort(array *list, object *comparator) { return list->sort(comparator); }
                        static object *s_str(array *list) { return list->str(); }
                        
                    public:
                        object *append_item(object *item);
                        object *append_item_in_place(object *item);
                        object *capacity();
                        object *equals(array *right);
                        object *get(integer *index);
                        object *greater_than(object *item);
                        object *greater_than_or_equal(object *item);
                        object *index_of(object *item);
                        object *less_than(object *item);
                        object *less_than_or_equal(object *item);
                        object *map(object *method);
                        object *not_equals(array *right);
                        object *obj();
                        object *reduce(object *method);
                        object *print();
                        object *reverse();
                        object *set(integer *index, object *item);
                        object *size();
                        object *sort(object *comparator);
                        object *str();
                        
                    BEGIN_KITE_CLASS_INITIALIZER
                        KITE_OPERATOR_DEFINE(semantics::ADD, &array::s_append_item);
                        KITE_OPERATOR_DEFINE(semantics::EQUALS, &array::s_equals);
                        KITE_OPERATOR_DEFINE(semantics::NOT_EQUALS, &array::s_not_equals);
                        KITE_OPERATOR_DEFINE(semantics::LESS_THAN, &array::s_less_than);
                        KITE_OPERATOR_DEFINE(semantics::LESS_OR_EQUALS, &array::s_less_than_or_equal);
                        KITE_OPERATOR_DEFINE(semantics::GREATER_THAN, &array::s_greater_than);
                        KITE_OPERATOR_DEFINE(semantics::GREATER_OR_EQUALS, &array::s_greater_than_or_equal);
                        KITE_OPERATOR_DEFINE(semantics::MAP, &array::s_map);
                        KITE_OPERATOR_DEFINE(semantics::REDUCE, &array::s_reduce);
                        KITE_OPERATOR_DEFINE(semantics::LEFT_SHIFT, &array::s_append_item_in_place);
                        KITE_OPERATOR_DEFINE(semantics::DEREF_ARRAY, &array::s_get);
                    
                        KITE_METHOD_DEFINE(append_item, 1, &array::s_append_item);
                        KITE_METHOD_DEFINE(capacity, 0, &array::s_capacity);
                        KITE_METHOD_DEFINE(get, 1, &array::s_get);
                        KITE_METHOD_DEFINE(index_of, 1, &array::s_index_of);
                        KITE_METHOD_DEFINE(map, 1, &array::s_map);
                        KITE_METHOD_DEFINE(obj, 0, &array::s_obj);
                        KITE_METHOD_DEFINE(print, 0, &array::s_print);
                        KITE_METHOD_DEFINE(reverse, 0, &array::s_reverse);
                        KITE_METHOD_DEFINE(set, 2, &array::s_set);
                        KITE_METHOD_DEFINE(size, 0, &array::s_size);
                        KITE_METHOD_DEFINE(sort, 1, &array::s_sort);
                        KITE_METHOD_DEFINE(str, 0, &array::s_str);
                    END_KITE_CLASS_INITIALIZER
                
                    array_contents_type array_contents;
                END_KITE_CLASS
            }
        }
    }
}

extern "C"
{
    void *kite_list_new();
    void kite_list_append(void *list, void *item);
}

#endif