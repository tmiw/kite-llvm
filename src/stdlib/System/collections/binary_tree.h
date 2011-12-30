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
 
#ifndef KITE_STDLIB__SYSTEM_COLLECTIONS__binary_tree_H
#define KITE_STDLIB__SYSTEM_COLLECTIONS__binary_tree_H

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
                typedef std::map<std::string, object*, std::less<std::string>, gc_allocator<object*> > tree_contents_type;
            
                BEGIN_KITE_BASE_CLASS(binary_tree)
                    private:
                        // TODO: iterator
                        static object *s_count(binary_tree *list) { return list->count(); }
                        static object *s_set(binary_tree *list, object *index, object *item) { return list->set(index, item); }
                        static object *s_get(binary_tree *list, object *index) { return list->get(index); }
                        static object *s_remove(binary_tree *list, object *index) { return list->remove(index); }
                        static object *s_keys(binary_tree *list) { return list->keys(); }
                        
                    public:
                        object *count();
                        object *set(object *index, object *item);
                        object *get(object *index);
                        object *remove(object *index);
                        object *keys();
                                                
                    BEGIN_KITE_CLASS_INITIALIZER
                        KITE_OPERATOR_DEFINE(semantics::DEREF_ARRAY, &binary_tree::s_get);

                        KITE_METHOD_DEFINE(count, 0, &binary_tree::s_count);
                        KITE_METHOD_DEFINE(set, 2, &binary_tree::s_set);
                        KITE_METHOD_DEFINE(get, 1, &binary_tree::s_get);
                        KITE_METHOD_DEFINE(remove, 1, &binary_tree::s_remove);
                        KITE_METHOD_DEFINE(keys, 0, &binary_tree::s_keys);
                    END_KITE_CLASS_INITIALIZER
                
                    tree_contents_type tree_contents;
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