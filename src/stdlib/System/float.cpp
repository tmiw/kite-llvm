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
#include "float.h"
using namespace boost::assign;
 
namespace kite
{
    namespace stdlib
    {
        namespace System
        {
            object_method_map fpnum::method_map = map_list_of
                ("bool__f", function_semantics(semantics::BOOLEAN, (void*)&(PREFIX_FLOAT_METHOD_NAME(bool__f))))
                ("int__f", function_semantics(semantics::INTEGER, (void*)&(PREFIX_FLOAT_METHOD_NAME(int__f))))
                ("float__f", function_semantics(semantics::FLOAT, (void*)&(PREFIX_FLOAT_METHOD_NAME(float__f))))
                ("print__f", function_semantics(semantics::FLOAT, (void*)&(PREFIX_FLOAT_METHOD_NAME(print__f))))
                ("obj__f", function_semantics(semantics::OBJECT, (void*)&(PREFIX_FLOAT_METHOD_NAME(obj__f))));
            
            bool fpnum::to_boolean()
            {
                return PREFIX_FLOAT_METHOD_NAME(bool__f)(val);
            }
            
            int fpnum::to_integer()
            {
                return PREFIX_FLOAT_METHOD_NAME(int__f)(val);
            }
            
            double fpnum::to_float()
            {
                return PREFIX_FLOAT_METHOD_NAME(float__f)(val);
            }
            
            double fpnum::print()
            {
                return PREFIX_FLOAT_METHOD_NAME(print__f)(val);
            }
            
            System::object *fpnum::to_object()
            {
                return (System::object*)PREFIX_FLOAT_METHOD_NAME(obj__f)(val);
            }
        }
    }
}

bool PREFIX_FLOAT_METHOD_NAME(bool__f)(double val)
{
    return val != 0.0f;
}

int PREFIX_FLOAT_METHOD_NAME(int__f)(double val)
{
    return (int)val;
}

double PREFIX_FLOAT_METHOD_NAME(float__f)(double val)
{
    return val;
}

double PREFIX_FLOAT_METHOD_NAME(print__f)(double val)
{
    std::cout << val << std::endl;
    return val;
}

void *PREFIX_FLOAT_METHOD_NAME(obj__f)(double val)
{
    return (void*)(new kite::stdlib::System::fpnum(val));
}