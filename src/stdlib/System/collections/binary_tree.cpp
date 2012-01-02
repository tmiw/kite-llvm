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
#include "binary_tree.h"
#include "../integer.h"
#include "../list.h"
#include "../exceptions/InvalidArgument.h"

namespace kite
{
    namespace stdlib
    {
        namespace System
        {
            namespace collections
            {
                object *binary_tree::count()
                {
                    return new integer(tree_contents.size());
                }
                
                object *binary_tree::set(object *index, object *item)
                {
                    if (index->type != semantics::STRING)
                    {
                        (exceptions::InvalidArgument::Create(
                            1,
                            new string("Argument 1 must be a string.")
                        ))->throw_exception();
                    }
                    string *str = (string*)index;
                    tree_contents[str->string_val.c_str()] = item;
                    return this;
                }
                
                object *binary_tree::get(object *index)
                {
                    if (index->type != semantics::STRING)
                    {
                        (exceptions::InvalidArgument::Create(
                            1,
                            new string("Argument 1 must be a string.")
                        ))->throw_exception();
                    }
                    string *str = (string*)index;
                    return tree_contents[str->string_val.c_str()];
                }
                
                object *binary_tree::remove(object *index)
                {
                    if (index->type != semantics::STRING)
                    {
                        (exceptions::InvalidArgument::Create(
                            1,
                            new string("Argument 1 must be a string.")
                        ))->throw_exception();
                    }
                    string *str = (string*)index;
                    tree_contents.erase(str->string_val.c_str());
                    return this;
                }
                
                object *binary_tree::keys()
                {
                    list *key_list = list::Create(0);
                    for (tree_contents_type::iterator i = tree_contents.begin();
                         i != tree_contents.end();
                         i++)
                    {
                        key_list->list_contents.push_back(
                            new string(i->first.c_str())
                        );
                    }
                    return key_list;
                }
            }
        }
    }
}

REGISTER_KITE_CLASS(kite::stdlib::System::collections::collections, kite::stdlib::System::collections::binary_tree);