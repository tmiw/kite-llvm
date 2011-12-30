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
            
            object *list::get_index(list *l, integer *index)
            {
                // TODO
                assert(index->type == semantics::INTEGER);
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
                // TODO
                assert(index->type == semantics::INTEGER);
                
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
                // TODO
                assert(index_from->type == count->type == semantics::INTEGER);
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
        }
    }
}

REGISTER_KITE_CLASS(kite::stdlib::System::System, kite::stdlib::System::list);

using namespace kite::stdlib;

void *kite_list_new()
{
    return (void*)(new System::list());
}

void kite_list_append(void *list, void *item)
{
    System::list *listObj = (System::list*)list;
    System::object *itemObj = (System::object*)item;
    
    listObj->list_contents.push_back(itemObj);
}
