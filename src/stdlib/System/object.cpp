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
#include <boost/assign.hpp>
#include "object.h"
#include "string.h"
#include "integer.h"
#include "float.h"
#include "boolean.h"

using namespace boost::assign;
 
namespace kite
{
    namespace stdlib
    {
        namespace System
        {
            object_method_map object::method_map;
        }
    }
}

int *kite_find_funccall(int *obj, char *name, int numargs)
{
    kite::stdlib::System::object *object = (kite::stdlib::System::object*)obj;
    std::string method_name = std::string(name) + "__";
    
    for (int i = 0; i < numargs; i++)
    {
        method_name += "o";
    }
    
    switch(object->type)
    {
        case kite::semantics::STRING:
            return (int*)kite::stdlib::System::string::method_map[method_name].second;
        case kite::semantics::INTEGER:
            return (int*)kite::stdlib::System::integer::method_map[method_name].second;
        case kite::semantics::FLOAT:
            return (int*)kite::stdlib::System::fpnum::method_map[method_name].second;
        case kite::semantics::BOOLEAN:
            return (int*)kite::stdlib::System::boolean::method_map[method_name].second;
        default:
            // TODO
            assert(0);
    }
}