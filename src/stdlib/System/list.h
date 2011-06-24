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
#include "dynamic_object.h"
#include "integer.h"

namespace kite
{
    namespace stdlib
    {
        namespace System
        {
            struct list : dynamic_object
            {
                static System::dynamic_object class_object;
                std::deque<object*> list_contents;
                
                list() : dynamic_object(&class_object)
                {
                    // empty
                }
                
                static void InitializeClass();
                static System::object *append(System::list *list, System::object *item);
                static System::object *count(System::list *list);
                static System::object *get_index(System::list *list, System::integer *index);
                static System::object *head(System::list *list);
                static System::object *prepend(System::list *list, System::object *item);
                static System::object *remove_at(System::list *list, System::integer *index);
                static System::object *as_string(System::list *list);
                static System::object *sublist(System::list *list, System::integer *index_from);
                static System::object *sublist_with_length(System::list *list, System::integer *index_from, System::integer *count);
                static System::object *tail(System::list *list);
            };
        }
    }
}

#endif