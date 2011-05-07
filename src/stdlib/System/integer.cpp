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
                ("bool__i", function_semantics(semantics::BOOLEAN, (void*)&integer::to_boolean))
                ("int__i", function_semantics(semantics::INTEGER, (void*)&integer::to_integer))
                ("int__o", function_semantics(semantics::INTEGER, (void*)&integer::to_integer_obj))
                ("float__i", function_semantics(semantics::FLOAT, (void*)&integer::to_float))
                ("print__i", function_semantics(semantics::INTEGER, (void*)&integer::print))
                ("print__o", function_semantics(semantics::OBJECT, (void*)&integer::print_obj))
                ("obj__i", function_semantics(semantics::OBJECT, (void*)&integer::to_object));
            
            bool integer::to_boolean(int val)
            {
                return val > 0;
            }
            
            int integer::to_integer(int val)
            {
                return val;
            }
            
            double integer::to_float(int val)
            {
                return (double)val;
            }
            
            int integer::print(int val)
            {
                std::cout << val << std::endl;
                return val;
            }
            
            System::object *integer::to_object(int val)
            {
                return new integer(val);
            }
            
            System::object *integer::print_obj(System::object *val)
            {
                integer *integ = (integer*)val;
                std::cout << integ->val << std::endl;
                return val;
            }
            
            int integer::to_integer_obj(System::object *val)
            {
                integer *integ = (integer*)val;
                return integ->val;
            }
        }
    }
}

kite::stdlib::System::object *System__integer__obj__i(int val)
{
    return kite::stdlib::System::integer::to_object(val);
}

int System__integer__print__i(int val)
{
    return kite::stdlib::System::integer::print(val);
}