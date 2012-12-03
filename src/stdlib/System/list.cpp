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
#include <cstring>
#include <assert.h>
#include "list.h"
#include "exceptions/TypeMismatch.h"

namespace kite
{
    namespace stdlib
    {
        namespace System
        {   
            object *list::append(list *l, object *item)
            {
                list *new_list = new list();
                new_list->list_contents = l->list_contents;
                new_list->list_contents.push_back(item);
                return new_list;
            }
            
            object *list::count(list *l)
            {
                return new integer(l->list_contents.size());
            }
            
            struct sorter : public std::binary_function<object *, object *, bool>
            {
                bool operator()(object *l, object *r)
                {
                    object *res = l->invoke_operator(semantics::LESS_THAN, r);
                    if (res->type != semantics::BOOLEAN)
                    {
                        kite::stdlib::System::exceptions::exception *exc = kite::stdlib::System::exceptions::TypeMismatch::Create(
                            1,
                            new kite::stdlib::System::string("Could not compare objects.")
                        );
                        exc->throw_exception();
                    }
                    
                    return ((boolean*)res)->val;
                }
            };
            
            object *list::sort(list *l)
            {
                std::sort(l->list_contents.begin(), l->list_contents.end(), sorter());
                return l;
            }
            
            object *list::get_index(list *l, integer *index)
            {
                if (index->type != semantics::INTEGER)
                {
                    kite::stdlib::System::exceptions::exception *exc = kite::stdlib::System::exceptions::TypeMismatch::Create(
                        1,
                        new kite::stdlib::System::string("integer expected")
                    );
                    exc->throw_exception();
                }
                return l->list_contents[index->val];
            }
            
            object *list::head(list *l)
            {
                return l->list_contents[0];
            }
            
            object *list::prepend(list *l, object *item)
            {
                list *new_list = new list();
                list_contents_type::iterator i = l->list_contents.begin();
                for (; i != l->list_contents.end(); i++)
                {
                    new_list->list_contents.push_back(*i);
                }
                new_list->list_contents.push_front(item);
                return new_list;
            }
            
            object *list::print(list *l)
            {
                std::cout << list::as_string(l) << std::endl;
                return l;
            }
            
            object *list::remove_at(list *l, integer *index)
            {
                if (index->type != semantics::INTEGER)
                {
                    kite::stdlib::System::exceptions::exception *exc = kite::stdlib::System::exceptions::TypeMismatch::Create(
                        1,
                        new kite::stdlib::System::string("integer expected")
                    );
                    exc->throw_exception();
                }
                
                list *new_list = new list();
                new_list->list_contents = l->list_contents;
                list_contents_type::iterator iter = new_list->list_contents.begin();
                for (int i = 0; i < new_list->list_contents.size(); i++)
                {
                    if (i == index->val)
                    {
                        new_list->list_contents.erase(iter);
                        break;
                    }
                    iter++;
                }
                return new_list;
            }
            
            char *list::as_string(list *l)
            {
                std::string result = "[";
                int idx = 0;
                for(
                    list_contents_type::iterator i = l->list_contents.begin();
                    i != l->list_contents.end();
                    i++)
                {
                    result += (*i)->as_string();
                    if (idx < l->list_contents.size() - 1)
                    {
                        result += ", ";
                    }
                    idx++;
                }
                result += "]";
                
                char *ret = (char*)GC_MALLOC(result.size() + 1);
                strcpy(ret, result.c_str());
                return ret;
            }
            
            object *list::sublist(list *l, integer *index_from)
            {
                return sublist_with_length(l, index_from, new integer(l->list_contents.size()));
            }
            
            object *list::sublist_with_length(list *l, integer *index_from, integer *count)
            {
                if (index_from->type != semantics::INTEGER)
                {
                    kite::stdlib::System::exceptions::exception *exc = kite::stdlib::System::exceptions::TypeMismatch::Create(
                        1,
                        new kite::stdlib::System::string("arg 1: integer expected")
                    );
                    exc->throw_exception();
                }
                
                if (count->type != semantics::INTEGER)
                {
                    kite::stdlib::System::exceptions::exception *exc = kite::stdlib::System::exceptions::TypeMismatch::Create(
                        1,
                        new kite::stdlib::System::string("arg 2: integer expected")
                    );
                    exc->throw_exception();
                }
                
                int length = count->val;
                int start = index_from->val;
                list_contents_type::iterator iter = l->list_contents.begin();
                list *ret = new list();
                
                for(; iter != l->list_contents.end(); iter++)
                {
                    if (length > 0 && start <= 0)
                    {
                        ret->list_contents.push_back(*iter);
                    }
                    length--;
                    start--;
                }
                
                return ret;
            }
            
            object *list::tail(list *l)
            {
                list *new_list = new list();
                new_list->list_contents = l->list_contents;
                new_list->list_contents.pop_front();
                return new_list;
            }
            
            object *list::equals(list *lhs, list *rhs)
            {
                bool ret = lhs->list_contents.size() == rhs->list_contents.size();
                if (ret)
                {
                    for (int i = 0; i < lhs->list_contents.size(); i++)
                    {
                        object *l_obj = lhs->list_contents[i];
                        object *r_obj = rhs->list_contents[i];
                        object *ret_obj = l_obj->invoke_operator(semantics::EQUALS, r_obj);
                        
                        ret &= ((boolean*)ret_obj)->val;
                        if (!ret) break;
                    }
                }
                return new boolean(ret);
            }
            
            object *list::not_equals(list *lhs, list *rhs)
            {
                object *ret = equals(lhs, rhs);
                return new boolean(!((boolean*)ret)->val);
            }

            object *list::less_than(list *lhs, list *rhs)
            {
                bool ret = lhs->list_contents.size() == rhs->list_contents.size();
                if (ret)
                {
                    for (int i = 0; i < lhs->list_contents.size(); i++)
                    {
                        object *l_obj = lhs->list_contents[i];
                        object *r_obj = rhs->list_contents[i];
                        object *ret_obj = l_obj->invoke_operator(semantics::LESS_THAN, r_obj);
                        
                        ret &= ((boolean*)ret_obj)->val;
                        if (!ret) break;
                    }
                }
                return new boolean(ret);
            }
            
            object *list::less_than_or_equals(list *lhs, list *rhs)
            {
                bool ret_val;
                boolean *lt_val = (boolean*)less_than(lhs, rhs);
                ret_val = lt_val->val;
                if (!ret_val)
                {
                    boolean *eq_val = (boolean*)equals(lhs, rhs);
                    ret_val = eq_val->val;
                }
                return new boolean(ret_val);
            }
            
            object *list::greater_than(list *lhs, list *rhs)
            {
                object *ret = less_than_or_equals(lhs, rhs);
                return new boolean(!((boolean*)ret)->val);
            }
            
            object *list::greater_than_or_equals(list *lhs, list *rhs)
            {
                object *ret = less_than(lhs, rhs);
                return new boolean(!((boolean*)ret)->val);
            }
            
            object *list::append_in_place(list *lhs, object *rhs)
            {
                lhs->list_contents.push_back(rhs);
                return lhs;
            }
            
            object *list::map(list *lhs, method *m)
            {
                list *new_list = new list();
                for (int i = 0; i < lhs->list_contents.size(); i++)
                {
                    new_list->list_contents.push_back(m->invoke(lhs->list_contents[i]));
                }
                return new_list;
            }
            
            object *list::reduce(list *lhs, method *m)
            {
                object *prev = NULL;
                for (int i = 0; i < lhs->list_contents.size(); i++)
                {
                    prev = m->invoke(prev, lhs->list_contents[i]);
                }
                return prev;
            }
        }
    }
}

REGISTER_KITE_CLASS(kite::stdlib::System::System, kite::stdlib::System::list);

using namespace kite::stdlib;

void *kite_list_new()
{
    return (void*)System::list::Create(0);
}

void kite_list_append(void *list, void *item)
{
    System::list *listObj = (System::list*)list;
    System::object *itemObj = (System::object*)item;
    
    listObj->list_contents.push_back(itemObj);
}
