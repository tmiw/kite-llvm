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
#include "exceptions/TypeMismatch.h"
#include "exceptions/DivideByZero.h"
using namespace boost::assign;
 
namespace kite
{
    namespace stdlib
    {
        namespace System
        {
            System::dynamic_object boolean::class_object;
            
            object_method_map boolean::method_map = map_list_of
                ("__op_equals____oo", function_semantics(semantics::OBJECT, (void*)&(PREFIX_BOOLEAN_METHOD_NAME(__op_equals____oo))))
                ("__op_nequals____oo", function_semantics(semantics::OBJECT, (void*)&(PREFIX_BOOLEAN_METHOD_NAME(__op_nequals____oo))))
                ("__op_and____oo", function_semantics(semantics::OBJECT, (void*)&(PREFIX_BOOLEAN_METHOD_NAME(__op_and____oo))))
                ("__op_or____oo", function_semantics(semantics::OBJECT, (void*)&(PREFIX_BOOLEAN_METHOD_NAME(__op_or____oo))))
                ("__op_not____o", function_semantics(semantics::OBJECT, (void*)&(PREFIX_BOOLEAN_METHOD_NAME(__op_not____o))))
                ("__op_xor____oo", function_semantics(semantics::OBJECT, (void*)&(PREFIX_BOOLEAN_METHOD_NAME(__op_xor____oo))))            
                ("bool__b", function_semantics(semantics::BOOLEAN, (void*)&(PREFIX_BOOLEAN_METHOD_NAME(bool__b))))
                ("int__b", function_semantics(semantics::INTEGER, (void*)&(PREFIX_BOOLEAN_METHOD_NAME(int__b))))
                ("float__b", function_semantics(semantics::FLOAT, (void*)&(PREFIX_BOOLEAN_METHOD_NAME(float__b))))
                ("print__b", function_semantics(semantics::BOOLEAN, (void*)&(PREFIX_BOOLEAN_METHOD_NAME(print__b))))
                ("print__o", function_semantics(semantics::OBJECT, (void*)&(PREFIX_BOOLEAN_METHOD_NAME(print__o))))
                ("str__b", function_semantics(semantics::STRING, (void*)&(PREFIX_BOOLEAN_METHOD_NAME(str__b))))
                ("str__o", function_semantics(semantics::STRING, (void*)&(PREFIX_BOOLEAN_METHOD_NAME(str__o))))
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
            
            System::object *boolean::to_object()
            {
                return (System::object*)PREFIX_BOOLEAN_METHOD_NAME(obj__b)(val);
            }
            
            void boolean::InitializeClass()
            {
                class_object.properties["__name"] = new System::string("System.boolean");
            }
        }
    }
}

using namespace kite::stdlib;

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

void *PREFIX_BOOLEAN_METHOD_NAME(print__o)(void *val)
{
    System::boolean *objVal = (System::boolean*)val;

    if (objVal->val) std::cout << "true" << std::endl;
    else std::cout << "false" << std::endl;
    return val;
}

char *PREFIX_BOOLEAN_METHOD_NAME(str__b)(bool val)
{
    char *retVal;
    
    if (val)
    {
        retVal = new char[5];
        strcpy(retVal, "true");
    }
    else
    {
        retVal = new char[6];
        strcpy(retVal, "false");
    }
    
    return retVal;
}

char *PREFIX_BOOLEAN_METHOD_NAME(str__o)(void *val)
{
    System::boolean *objVal = (System::boolean*)val;
    return PREFIX_BOOLEAN_METHOD_NAME(str__b)(objVal->val);
}

void *PREFIX_BOOLEAN_METHOD_NAME(obj__b)(bool val)
{
    return (void*)(new System::boolean(val));
}

static void verify_boolean_type(System::boolean *left, System::boolean *right)
{
    if (left->type != right->type)
    {
        System::exceptions::exception *exc = System::exceptions::TypeMismatch::Create(
            1,
            new System::string("boolean expected")
        );
        exc->throw_exception();
    }
}

void *PREFIX_BOOLEAN_METHOD_NAME(__op_equals____oo)(void *lhs, void *rhs)
{
    System::boolean *lhsInt = (System::boolean*)lhs;
    System::boolean *rhsInt = (System::boolean*)rhs;
    verify_boolean_type(lhsInt, rhsInt);
    return (void*)(new kite::stdlib::System::boolean(lhsInt->val == rhsInt->val));
}

void *PREFIX_BOOLEAN_METHOD_NAME(__op_nequals____oo)(void *lhs, void *rhs)
{
    System::boolean *lhsInt = (System::boolean*)lhs;
    System::boolean *rhsInt = (System::boolean*)rhs;
    verify_boolean_type(lhsInt, rhsInt);
    return (void*)(new kite::stdlib::System::boolean(lhsInt->val != rhsInt->val));
}

void *PREFIX_BOOLEAN_METHOD_NAME(__op_and____oo)(void *lhs, void *rhs)
{
    System::boolean *leftObject = (System::boolean*)lhs;
    System::boolean *rightObject = (System::boolean*)rhs;
    verify_boolean_type(leftObject, rightObject);
    return (void*)(new System::boolean(leftObject->val && rightObject->val));
}

void *PREFIX_BOOLEAN_METHOD_NAME(__op_or____oo)(void *lhs, void *rhs)
{
    System::boolean *leftObject = (System::boolean*)lhs;
    System::boolean *rightObject = (System::boolean*)rhs;
    verify_boolean_type(leftObject, rightObject);
    return (void*)(new System::boolean(leftObject->val || rightObject->val));
}

void *PREFIX_BOOLEAN_METHOD_NAME(__op_not____o)(void *val)
{
    System::boolean *objVal = (System::boolean*)val;
    return (void*)(new System::boolean(!objVal->val));
}

void *PREFIX_BOOLEAN_METHOD_NAME(__op_xor____oo)(void *lhs, void *rhs)
{
    System::boolean *leftObject = (System::boolean*)lhs;
    System::boolean *rightObject = (System::boolean*)rhs;

    verify_boolean_type(leftObject, rightObject);
    return (void*)(new System::boolean(leftObject->val ^ rightObject->val));
}
