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

#include <assert.h>
#include "list.h"

namespace kite
{
    namespace stdlib
    {
        namespace System
        {
            System::dynamic_object list::class_object;
            
            void list::InitializeClass()
            {
                // TODO: operators
                class_object.add_method("append", 1, (void*)&list::append);
                class_object.add_method("count", 0, (void*)&list::count);
                // TODO: cur
                class_object.add_method("getIndex", 1, (void*)&list::get_index);
                class_object.add_method("head", 0, (void*)&list::head);
                // TODO: next
                class_object.add_method("prepend", 1, (void*)&list::prepend);
                class_object.add_method("removeAt", 1, (void*)&list::remove_at);
                // TODO: reset
                // TOOD: sort
                class_object.add_method("str", 0, (void*)&list::as_string);
                class_object.add_method("sublist", 1, (void*)&list::sublist);
                class_object.add_method("sublist", 2, (void*)&list::sublist_with_length);
                class_object.add_method("tail", 0, (void*)&list::tail);
            }
            
            System::object *list::append(System::list *list, System::object *item)
            {
                System::list *new_list = new System::list();
                new_list->list_contents = list->list_contents;
                new_list->list_contents.push_back(item);
                return new_list;
            }
            
            System::object *list::count(System::list *list)
            {
                return new System::integer(list->list_contents.size());
            }
            
            System::object *list::get_index(System::list *list, System::integer *index)
            {
                // TODO
                assert(index->type == semantics::INTEGER);
                return list->list_contents[index->val];
            }
            
            System::object *list::head(System::list *list)
            {
                return list->list_contents[0];
            }
            
            System::object *list::prepend(System::list *list, System::object *item)
            {
                System::list *new_list = new System::list();
                new_list->list_contents = list->list_contents;
                new_list->list_contents.push_front(item);
                return new_list;
            }
            
            System::object *list::remove_at(System::list *list, System::integer *index)
            {
                // TODO
                assert(index->type == semantics::INTEGER);
                
                System::list *new_list = new System::list();
                new_list->list_contents = list->list_contents;
                std::deque<System::object*>::iterator iter = new_list->list_contents.begin();
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
            
            System::object *list::as_string(System::list *list)
            {
                return NULL;
            }
            
            System::object *list::sublist(System::list *list, System::integer *index_from)
            {
                return NULL;
            }
            
            System::object *list::sublist_with_length(System::list *list, System::integer *index_from, System::integer *count)
            {
                return NULL;
            }
            
            System::object *list::tail(System::list *list)
            {
                System::list *new_list = new System::list();
                new_list->list_contents = list->list_contents;
                new_list->list_contents.pop_front();
                return new_list;
            }
        }
    }
}