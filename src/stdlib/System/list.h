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
 
#ifndef KITE_STDLIB__SYSTEM__LIST_H
#define KITE_STDLIB__SYSTEM__LIST_H

#include <deque>
#include "stdlib/System.h"
#include "integer.h"

namespace kite
{
    namespace stdlib
    {
        namespace System
        {
            typedef std::deque<object*, gc_allocator<object*> > list_contents_type;
            
            BEGIN_KITE_BASE_CLASS(list)
            public:
                static object *append(list *list, object *item);
                static object *count(list *list);
                static object *get_index(list *list, integer *index);
                static object *head(list *list);
                static object *print(list *list);
                static object *prepend(list *list, object *item);
                static object *remove_at(list *list, integer *index);
                static char *as_string(list *list);
                static object *sublist(list *list, integer *index_from);
                static object *sublist_with_length(list *list, integer *index_from, integer *count);
                static object *tail(list *list);
                
                BEGIN_KITE_CLASS_INITIALIZER
                    // TODO: operators
                    KITE_METHOD_DEFINE(append, 1, &list::append);
                    KITE_METHOD_DEFINE(count, 0, &list::count);
                    // TODO: cur
                    KITE_METHOD_DEFINE(getIndex, 1, &list::get_index);
                    KITE_METHOD_DEFINE(head, 0, &list::head);
                    // TODO: next
                    KITE_METHOD_DEFINE(prepend, 1, &list::prepend);
                    KITE_METHOD_DEFINE(print, 0, &list::print);
                    KITE_METHOD_DEFINE(removeAt, 1, &list::remove_at);
                    // TODO: reset
                    // TOOD: sort
                    KITE_METHOD_DEFINE(str, 0, &list::as_string);
                    KITE_METHOD_DEFINE(sublist, 1, &list::sublist);
                    KITE_METHOD_DEFINE(sublist, 2, &list::sublist_with_length);
                    KITE_METHOD_DEFINE(tail, 0, &list::tail);
                END_KITE_CLASS_INITIALIZER
                
                list_contents_type list_contents;
            };
        }
    }
}

extern "C"
{
    void *kite_list_new();
    void kite_list_append(void *list, void *item);
}

#endif