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
            object_method_map integer::method_map = map_list_of
                ("__op_plus____oo", function_semantics(semantics::OBJECT, (void*)&(PREFIX_INTEGER_METHOD_NAME(__op_plus____oo))))
                ("__op_minus____oo", function_semantics(semantics::OBJECT, (void*)&(PREFIX_INTEGER_METHOD_NAME(__op_minus____oo))))
                ("__op_multiply____oo", function_semantics(semantics::OBJECT, (void*)&(PREFIX_INTEGER_METHOD_NAME(__op_multiply____oo))))
                ("__op_divide____oo", function_semantics(semantics::OBJECT, (void*)&(PREFIX_INTEGER_METHOD_NAME(__op_divide____oo))))
                ("__op_mod____oo", function_semantics(semantics::OBJECT, (void*)&(PREFIX_INTEGER_METHOD_NAME(__op_mod____oo))))
                ("__op_unminus____o", function_semantics(semantics::OBJECT, (void*)&(PREFIX_INTEGER_METHOD_NAME(__op_unminus____o))))
                ("__op_unplus____o", function_semantics(semantics::OBJECT, (void*)&(PREFIX_INTEGER_METHOD_NAME(__op_unplus____o))))
                ("__op_equals____oo", function_semantics(semantics::OBJECT, (void*)&(PREFIX_INTEGER_METHOD_NAME(__op_equals____oo))))
                ("__op_nequals____oo", function_semantics(semantics::OBJECT, (void*)&(PREFIX_INTEGER_METHOD_NAME(__op_nequals____oo))))
                ("__op_lt____oo", function_semantics(semantics::OBJECT, (void*)&(PREFIX_INTEGER_METHOD_NAME(__op_lt____oo))))
                ("__op_gt____oo", function_semantics(semantics::OBJECT, (void*)&(PREFIX_INTEGER_METHOD_NAME(__op_gt____oo))))
                ("__op_leq____oo", function_semantics(semantics::OBJECT, (void*)&(PREFIX_INTEGER_METHOD_NAME(__op_leq____oo))))
                ("__op_geq____oo", function_semantics(semantics::OBJECT, (void*)&(PREFIX_INTEGER_METHOD_NAME(__op_geq____oo))))
                ("__op_and____oo", function_semantics(semantics::OBJECT, (void*)&(PREFIX_INTEGER_METHOD_NAME(__op_and____oo))))
                ("__op_or____oo", function_semantics(semantics::OBJECT, (void*)&(PREFIX_INTEGER_METHOD_NAME(__op_or____oo))))
                ("__op_not____o", function_semantics(semantics::OBJECT, (void*)&(PREFIX_INTEGER_METHOD_NAME(__op_not____o))))
                ("__op_xor____oo", function_semantics(semantics::OBJECT, (void*)&(PREFIX_INTEGER_METHOD_NAME(__op_xor____oo))))
                ("__op_lshift____oo", function_semantics(semantics::OBJECT, (void*)&(PREFIX_INTEGER_METHOD_NAME(__op_lshift____oo))))
                ("__op_rshift____oo", function_semantics(semantics::OBJECT, (void*)&(PREFIX_INTEGER_METHOD_NAME(__op_rshift____oo))))
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

static void verify_integer_type(System::integer *left, System::integer *right)
{
    if (left->type != right->type)
    {
        System::exceptions::exception *exc = new System::exceptions::TypeMismatch("integer expected");
        exc->throw_exception();
    }
}

static void verify_divisor_not_zero(System::integer *right)
{
    if (right->val == 0)
    {
        kite_exception_raise_div_by_zero();
    }
}

void *PREFIX_INTEGER_METHOD_NAME(__op_plus____oo)(void *lhs, void *rhs)
{
    System::integer *lhsInt = (System::integer*)lhs;
    System::integer *rhsInt = (System::integer*)rhs;
    verify_integer_type(lhsInt, rhsInt);
    return (void*)(new kite::stdlib::System::integer(lhsInt->val + rhsInt->val));
}

void *PREFIX_INTEGER_METHOD_NAME(__op_minus____oo)(void *lhs, void *rhs)
{
    System::integer *lhsInt = (System::integer*)lhs;
    System::integer *rhsInt = (System::integer*)rhs;
    verify_integer_type(lhsInt, rhsInt);
    return (void*)(new kite::stdlib::System::integer(lhsInt->val - rhsInt->val));
}

void *PREFIX_INTEGER_METHOD_NAME(__op_multiply____oo)(void *lhs, void *rhs)
{
    System::integer *lhsInt = (System::integer*)lhs;
    System::integer *rhsInt = (System::integer*)rhs;
    verify_integer_type(lhsInt, rhsInt);
    return (void*)(new kite::stdlib::System::integer(lhsInt->val * rhsInt->val));
}

void *PREFIX_INTEGER_METHOD_NAME(__op_divide____oo)(void *lhs, void *rhs)
{
    System::integer *lhsInt = (System::integer*)lhs;
    System::integer *rhsInt = (System::integer*)rhs;
    verify_integer_type(lhsInt, rhsInt);
    verify_divisor_not_zero(rhsInt);
    return (void*)(new kite::stdlib::System::integer(lhsInt->val / rhsInt->val));
}

void *PREFIX_INTEGER_METHOD_NAME(__op_mod____oo)(void *lhs, void *rhs)
{
    System::integer *lhsInt = (System::integer*)lhs;
    System::integer *rhsInt = (System::integer*)rhs;
    verify_integer_type(lhsInt, rhsInt);
    verify_divisor_not_zero(rhsInt);
    return (void*)(new kite::stdlib::System::integer(lhsInt->val % rhsInt->val));
}

void *PREFIX_INTEGER_METHOD_NAME(__op_unminus____o)(void *rhs)
{
    System::integer *rhsInt = (System::integer*)rhs;
    return (void*)(new kite::stdlib::System::integer(-rhsInt->val));
}

void *PREFIX_INTEGER_METHOD_NAME(__op_unplus____o)(void *rhs)
{
    System::integer *rhsInt = (System::integer*)rhs;
    return (void*)(new kite::stdlib::System::integer(rhsInt->val));
}

void *PREFIX_INTEGER_METHOD_NAME(__op_equals____oo)(void *lhs, void *rhs)
{
    System::integer *lhsInt = (System::integer*)lhs;
    System::integer *rhsInt = (System::integer*)rhs;
    verify_integer_type(lhsInt, rhsInt);
    return (void*)(new kite::stdlib::System::boolean(lhsInt->val == rhsInt->val));
}

void *PREFIX_INTEGER_METHOD_NAME(__op_nequals____oo)(void *lhs, void *rhs)
{
    System::integer *lhsInt = (System::integer*)lhs;
    System::integer *rhsInt = (System::integer*)rhs;
    verify_integer_type(lhsInt, rhsInt);
    return (void*)(new kite::stdlib::System::boolean(lhsInt->val != rhsInt->val));
}

void *PREFIX_INTEGER_METHOD_NAME(__op_lt____oo)(void *lhs, void *rhs)
{
    System::integer *leftObject = (System::integer*)lhs;
    System::integer *rightObject = (System::integer*)rhs;
    verify_integer_type(leftObject, rightObject);
    return (void*)(new System::boolean(leftObject->val < rightObject->val));
}

void *PREFIX_INTEGER_METHOD_NAME(__op_gt____oo)(void *lhs, void *rhs)
{
    System::integer *leftObject = (System::integer*)lhs;
    System::integer *rightObject = (System::integer*)rhs;
    verify_integer_type(leftObject, rightObject);
    return (void*)(new System::boolean(leftObject->val > rightObject->val));
}

void *PREFIX_INTEGER_METHOD_NAME(__op_leq____oo)(void *lhs, void *rhs)
{
    System::integer *leftObject = (System::integer*)lhs;
    System::integer *rightObject = (System::integer*)rhs;
    verify_integer_type(leftObject, rightObject);
    return (void*)(new System::boolean(leftObject->val <= rightObject->val));
}

void *PREFIX_INTEGER_METHOD_NAME(__op_geq____oo)(void *lhs, void *rhs)
{
    System::integer *leftObject = (System::integer*)lhs;
    System::integer *rightObject = (System::integer*)rhs;
    verify_integer_type(leftObject, rightObject);
    return (void*)(new System::boolean(leftObject->val >= rightObject->val));
}

void *PREFIX_INTEGER_METHOD_NAME(__op_and____oo)(void *lhs, void *rhs)
{
    System::integer *leftObject = (System::integer*)lhs;
    System::integer *rightObject = (System::integer*)rhs;
    verify_integer_type(leftObject, rightObject);
    return (void*)(new System::integer(leftObject->val & rightObject->val));
}

void *PREFIX_INTEGER_METHOD_NAME(__op_or____oo)(void *lhs, void *rhs)
{
    System::integer *leftObject = (System::integer*)lhs;
    System::integer *rightObject = (System::integer*)rhs;
    verify_integer_type(leftObject, rightObject);
    return (void*)(new System::integer(leftObject->val | rightObject->val));
}

void *PREFIX_INTEGER_METHOD_NAME(__op_not____o)(void *rhs)
{
    System::integer *rhsInt = (System::integer*)rhs;
    return (void*)(new kite::stdlib::System::integer(~rhsInt->val));
}

void *PREFIX_INTEGER_METHOD_NAME(__op_xor____oo)(void *lhs, void *rhs)
{
    System::integer *leftObject = (System::integer*)lhs;
    System::integer *rightObject = (System::integer*)rhs;

    verify_integer_type(leftObject, rightObject);
    return (void*)(new System::integer(leftObject->val ^ rightObject->val));
}

void *PREFIX_INTEGER_METHOD_NAME(__op_lshift____oo)(void *lhs, void *rhs)
{
    System::integer *lhsInt = (System::integer*)lhs;
    System::integer *rhsInt = (System::integer*)rhs;
    verify_integer_type(lhsInt, rhsInt);
    return (void*)(new kite::stdlib::System::integer(lhsInt->val << rhsInt->val));
}

void *PREFIX_INTEGER_METHOD_NAME(__op_rshift____oo)(void *lhs, void *rhs)
{
    System::integer *lhsInt = (System::integer*)lhs;
    System::integer *rhsInt = (System::integer*)rhs;
    verify_integer_type(lhsInt, rhsInt);
    return (void*)(new kite::stdlib::System::integer(lhsInt->val >> rhsInt->val));
}

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

