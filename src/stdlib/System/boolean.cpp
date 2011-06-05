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
#include "boolean.h"
using namespace boost::assign;
 
namespace kite
{
    namespace stdlib
    {
        namespace System
        {
            object_method_map boolean::method_map = map_list_of
                ("bool__b", function_semantics(semantics::BOOLEAN, (void*)&(PREFIX_BOOLEAN_METHOD_NAME(bool__b))))
                ("int__b", function_semantics(semantics::INTEGER, (void*)&(PREFIX_BOOLEAN_METHOD_NAME(int__b))))
                ("float__b", function_semantics(semantics::FLOAT, (void*)&(PREFIX_BOOLEAN_METHOD_NAME(float__b))))
                ("print__b", function_semantics(semantics::BOOLEAN, (void*)&(PREFIX_BOOLEAN_METHOD_NAME(print__b))))
                ("obj__b", function_semantics(semantics::OBJECT, (void*)&(PREFIX_BOOLEAN_METHOD_NAME(obj__b))));
            
            bool boolean::to_boolean()
            {
                return PREFIX_BOOLEAN_METHOD_NAME(bool__b)(val);
            }
            
            int boolean::to_integer()
            {
                return PREFIX_BOOLEAN_METHOD_NAME(int__b)(val);
            }
            
            double boolean::to_float()
            {
                return PREFIX_BOOLEAN_METHOD_NAME(float__b)(val);
            }
            
            bool boolean::print()
            {
                return PREFIX_BOOLEAN_METHOD_NAME(print__b)(val);
            }
            
            System::object *boolean::to_object()
            {
                return (System::object*)PREFIX_BOOLEAN_METHOD_NAME(obj__b)(val);
            }
        }
    }
}

bool PREFIX_BOOLEAN_METHOD_NAME(bool__b)(bool val)
{
    return val;
}

int PREFIX_BOOLEAN_METHOD_NAME(int__b)(bool val)
{
    return (int)val;
}

double PREFIX_BOOLEAN_METHOD_NAME(float__b)(bool val)
{
    return (double)val;
}

bool PREFIX_BOOLEAN_METHOD_NAME(print__b)(bool val)
{
    if (val) std::cout << "true" << std::endl;
    else std::cout << "false" << std::endl;
    return val;
}

void *PREFIX_BOOLEAN_METHOD_NAME(obj__b)(bool val)
{
    return (void*)(new kite::stdlib::System::boolean(val));
}