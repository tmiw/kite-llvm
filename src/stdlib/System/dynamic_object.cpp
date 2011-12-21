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

#include "dynamic_object.h"
#include "string.h"
#include "../language/kite.h"
#include "exceptions/NullReference.h"

using namespace kite::stdlib;

void *kite_dynamic_object_alloc()
{
    return (void*)(new System::dynamic_object());
}

void kite_dynamic_object_set_parent(void *object, void *parent)
{
    ((System::dynamic_object*)object)->parent = (System::object*)parent;
}

void kite_dynamic_object_set_name(void *object, char *name)
{
    ((System::dynamic_object*)object)->properties["__name"] = new System::string(name);
}

void **kite_dynamic_object_get_property(void *object, char *name, bool set)
{
    System::dynamic_object *castedObj = (System::dynamic_object*)object;
    System::dynamic_object *cur = castedObj;
    void **ret = NULL;

    if (object == NULL)
    {
        System::exceptions::NullReference *nre = new System::exceptions::NullReference();
        nre->throw_exception();
    }
    
    if (!set)
    {
        do
        {
            if (cur->properties.find(name) != cur->properties.end())
            {
                ret = (void**)&cur->properties[name];
                break;
            }
            cur = (System::dynamic_object*)cur->parent;
        } while (cur);
    
        if (!ret)
        {
            castedObj->properties[name] = NULL;
            ret = (void**)&castedObj->properties[name];
        }
    }
    else
    {
        ret = (void**)&castedObj->properties[name];
    }
    return ret;
}

void *kite_dynamic_object_get_root()
{
    return language::kite::kite::root_object;
}
