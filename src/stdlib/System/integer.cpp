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
#include "integer.h"
using namespace boost::assign;
 
namespace kite
{
    namespace stdlib
    {
        namespace System
        {
            object_method_map integer::method_map = map_list_of
                ("bool__i", function_semantics(semantics::BOOLEAN, (void*)&(PREFIX_INTEGER_METHOD_NAME(bool__i))))
                ("bool__o", function_semantics(semantics::BOOLEAN, (void*)&(PREFIX_INTEGER_METHOD_NAME(bool__o))))
                ("int__i", function_semantics(semantics::INTEGER, (void*)&(PREFIX_INTEGER_METHOD_NAME(int__i))))
                ("int__o", function_semantics(semantics::INTEGER, (void*)&(PREFIX_INTEGER_METHOD_NAME(int__o))))
                ("float__i", function_semantics(semantics::FLOAT, (void*)&(PREFIX_INTEGER_METHOD_NAME(float__i))))
                ("float__o", function_semantics(semantics::FLOAT, (void*)&(PREFIX_INTEGER_METHOD_NAME(float__o))))
                ("print__i", function_semantics(semantics::INTEGER, (void*)&(PREFIX_INTEGER_METHOD_NAME(print__i))))
                ("print__o", function_semantics(semantics::OBJECT, (void*)&(PREFIX_INTEGER_METHOD_NAME(print__o))))
                ("obj__i", function_semantics(semantics::OBJECT, (void*)&(PREFIX_INTEGER_METHOD_NAME(obj__i))))
                ("obj__o", function_semantics(semantics::OBJECT, (void*)&(PREFIX_INTEGER_METHOD_NAME(obj__o))));
            
            bool integer::to_boolean()
            {
                return PREFIX_INTEGER_METHOD_NAME(bool__i)(val);
            }
            
            int integer::to_integer()
            {
                return PREFIX_INTEGER_METHOD_NAME(int__i)(val);
            }
            
            double integer::to_float()
            {
                return PREFIX_INTEGER_METHOD_NAME(float__i)(val);
            }
            
            int integer::print()
            {
                return PREFIX_INTEGER_METHOD_NAME(print__i)(val);
            }
            
            System::object *integer::to_object()
            {
                return (System::object*)PREFIX_INTEGER_METHOD_NAME(obj__i)(val);
            }
        }
    }
}

using namespace kite::stdlib;

bool PREFIX_INTEGER_METHOD_NAME(bool__i)(int val)
{
    return val != 0;
}

bool PREFIX_INTEGER_METHOD_NAME(bool__o)(void *val)
{
    return PREFIX_INTEGER_METHOD_NAME(bool__i)(((System::integer*)val)->val);
}

int PREFIX_INTEGER_METHOD_NAME(int__i)(int val)
{
    return val;
}

int PREFIX_INTEGER_METHOD_NAME(int__o)(void *val)
{
    return PREFIX_INTEGER_METHOD_NAME(int__i)(((System::integer*)val)->val);
}

double PREFIX_INTEGER_METHOD_NAME(float__i)(int val)
{
    return (double)val;
}

double PREFIX_INTEGER_METHOD_NAME(float__o)(void *val)
{
    return PREFIX_INTEGER_METHOD_NAME(float__i)(((System::integer*)val)->val);
}

int PREFIX_INTEGER_METHOD_NAME(print__i)(int val)
{
    std::cout << val << std::endl;
    return val;
}

void *PREFIX_INTEGER_METHOD_NAME(print__o)(void *val)
{
    PREFIX_INTEGER_METHOD_NAME(print__i)(((System::integer*)val)->val);
    return val;
}

void *PREFIX_INTEGER_METHOD_NAME(obj__i)(int val)
{
    return (void*)(new kite::stdlib::System::integer(val));
}

void *PREFIX_INTEGER_METHOD_NAME(obj__o)(void *val)
{
    return val;
}

