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
#include <cmath>
#include <boost/assign.hpp>
#include "float.h"
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
            System::dynamic_object fpnum::class_object;
            object_method_map fpnum::method_map = map_list_of
                ("__op_plus____oo", function_semantics(semantics::OBJECT, (void*)&(PREFIX_FLOAT_METHOD_NAME(__op_plus____oo))))
                ("__op_minus____oo", function_semantics(semantics::OBJECT, (void*)&(PREFIX_FLOAT_METHOD_NAME(__op_minus____oo))))
                ("__op_multiply____oo", function_semantics(semantics::OBJECT, (void*)&(PREFIX_FLOAT_METHOD_NAME(__op_multiply____oo))))
                ("__op_divide____oo", function_semantics(semantics::OBJECT, (void*)&(PREFIX_FLOAT_METHOD_NAME(__op_divide____oo))))
                ("__op_mod____oo", function_semantics(semantics::OBJECT, (void*)&(PREFIX_FLOAT_METHOD_NAME(__op_mod____oo))))
                ("__op_unminus____o", function_semantics(semantics::OBJECT, (void*)&(PREFIX_FLOAT_METHOD_NAME(__op_unminus____o))))
                ("__op_unplus____o", function_semantics(semantics::OBJECT, (void*)&(PREFIX_FLOAT_METHOD_NAME(__op_unplus____o))))
                ("__op_equals____oo", function_semantics(semantics::OBJECT, (void*)&(PREFIX_FLOAT_METHOD_NAME(__op_equals____oo))))
                ("__op_nequals____oo", function_semantics(semantics::OBJECT, (void*)&(PREFIX_FLOAT_METHOD_NAME(__op_nequals____oo))))
                ("__op_lt____oo", function_semantics(semantics::OBJECT, (void*)&(PREFIX_FLOAT_METHOD_NAME(__op_lt____oo))))
                ("__op_gt____oo", function_semantics(semantics::OBJECT, (void*)&(PREFIX_FLOAT_METHOD_NAME(__op_gt____oo))))
                ("__op_leq____oo", function_semantics(semantics::OBJECT, (void*)&(PREFIX_FLOAT_METHOD_NAME(__op_leq____oo))))
                ("__op_geq____oo", function_semantics(semantics::OBJECT, (void*)&(PREFIX_FLOAT_METHOD_NAME(__op_geq____oo))))
                ("__op_not____o", function_semantics(semantics::OBJECT, (void*)&(PREFIX_FLOAT_METHOD_NAME(__op_not____o))))
                ("bool__f", function_semantics(semantics::BOOLEAN, (void*)&(PREFIX_FLOAT_METHOD_NAME(bool__f))))
                ("bool__o", function_semantics(semantics::BOOLEAN, (void*)&(PREFIX_FLOAT_METHOD_NAME(bool__o))))
                ("int__f", function_semantics(semantics::INTEGER, (void*)&(PREFIX_FLOAT_METHOD_NAME(int__f))))
                ("float__f", function_semantics(semantics::FLOAT, (void*)&(PREFIX_FLOAT_METHOD_NAME(float__f))))
                ("str__f", function_semantics(semantics::FLOAT, (void*)&(PREFIX_FLOAT_METHOD_NAME(str__f))))
                ("str__o", function_semantics(semantics::FLOAT, (void*)&(PREFIX_FLOAT_METHOD_NAME(str__o))))
                ("print__f", function_semantics(semantics::FLOAT, (void*)&(PREFIX_FLOAT_METHOD_NAME(print__f))))
                ("print__o", function_semantics(semantics::OBJECT, (void*)&(PREFIX_FLOAT_METHOD_NAME(print__o))))
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
            
            System::object *fpnum::to_object()
            {
                return (System::object*)PREFIX_FLOAT_METHOD_NAME(obj__f)(val);
            }
            
            void fpnum::InitializeClass()
            {
                class_object.add_method("parse", 1, (void*)&fpnum::parse);
                class_object.properties["__name"] = new System::string("System.float");
            }
            
            System::object *fpnum::parse(System::object *t, System::string *str)
            {
                // TODO
                assert(str->type == semantics::STRING);
                return new System::fpnum(atof(str->string_val.c_str()));
            }
        }
    }
}

using namespace kite::stdlib;

bool PREFIX_FLOAT_METHOD_NAME(bool__f)(double val)
{
    return val != 0.0f;
}

bool PREFIX_FLOAT_METHOD_NAME(bool__o)(void *val)
{
    System::fpnum *fpobj = (System::fpnum*)val;
    return fpobj->val != 0.0f;
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

char *PREFIX_FLOAT_METHOD_NAME(str__f)(double val)
{
    char *retVal;
    retVal = new char[256];
    sprintf(retVal, "%f", &val);
    return retVal;
}

char *PREFIX_FLOAT_METHOD_NAME(str__o)(void *obj)
{
    System::fpnum *val = (System::fpnum*)obj;
    return PREFIX_FLOAT_METHOD_NAME(str__f)(val->val);
}

void *PREFIX_FLOAT_METHOD_NAME(print__o)(void *obj)
{
    System::fpnum *val = (System::fpnum*)obj;
    std::cout << val->val << std::endl;
    return obj;
}

void *PREFIX_FLOAT_METHOD_NAME(obj__f)(double val)
{
    return (void*)(new System::fpnum(val));
}

void *PREFIX_FLOAT_METHOD_NAME(__op_not____o)(void *rhs)
{
    System::fpnum *rhsFloat = (System::fpnum*)rhs;
    System::fpnum *ret;
    if (rhsFloat->val == 0.0f)
    {
        ret = new System::fpnum(1.0f);
    }
    else
    {
        ret = new System::fpnum(0.0f);
    }
    return (void*)ret;
}

static void verify_float_type(System::fpnum *left, System::fpnum *right)
{
    if (left->type != right->type)
    {
        System::exceptions::exception *exc = new System::exceptions::TypeMismatch("float expected");
        exc->throw_exception();
    }
}

static void verify_divisor_not_zero(System::fpnum *right)
{
    if (right->val == 0)
    {
        kite_exception_raise_div_by_zero();
    }
}

void *PREFIX_FLOAT_METHOD_NAME(__op_plus____oo)(void *lhs, void *rhs)
{
    System::fpnum *lhsInt = (System::fpnum*)lhs;
    System::fpnum *rhsInt = (System::fpnum*)rhs;
    verify_float_type(lhsInt, rhsInt);
    return (void*)(new kite::stdlib::System::fpnum(lhsInt->val + rhsInt->val));
}

void *PREFIX_FLOAT_METHOD_NAME(__op_minus____oo)(void *lhs, void *rhs)
{
    System::fpnum *lhsInt = (System::fpnum*)lhs;
    System::fpnum *rhsInt = (System::fpnum*)rhs;
    verify_float_type(lhsInt, rhsInt);
    return (void*)(new kite::stdlib::System::fpnum(lhsInt->val - rhsInt->val));
}

void *PREFIX_FLOAT_METHOD_NAME(__op_multiply____oo)(void *lhs, void *rhs)
{
    System::fpnum *lhsInt = (System::fpnum*)lhs;
    System::fpnum *rhsInt = (System::fpnum*)rhs;
    verify_float_type(lhsInt, rhsInt);
    return (void*)(new kite::stdlib::System::fpnum(lhsInt->val * rhsInt->val));
}

void *PREFIX_FLOAT_METHOD_NAME(__op_divide____oo)(void *lhs, void *rhs)
{
    System::fpnum *lhsInt = (System::fpnum*)lhs;
    System::fpnum *rhsInt = (System::fpnum*)rhs;
    verify_float_type(lhsInt, rhsInt);
    verify_divisor_not_zero(rhsInt);
    return (void*)(new kite::stdlib::System::fpnum(lhsInt->val / rhsInt->val));
}

void *PREFIX_FLOAT_METHOD_NAME(__op_mod____oo)(void *lhs, void *rhs)
{
    System::fpnum *lhsInt = (System::fpnum*)lhs;
    System::fpnum *rhsInt = (System::fpnum*)rhs;
    verify_float_type(lhsInt, rhsInt);
    verify_divisor_not_zero(rhsInt);
    return (void*)(new kite::stdlib::System::fpnum(fmod(lhsInt->val, rhsInt->val)));
}

void *PREFIX_FLOAT_METHOD_NAME(__op_unminus____o)(void *rhs)
{
    System::fpnum *rhsInt = (System::fpnum*)rhs;
    return (void*)(new kite::stdlib::System::fpnum(-rhsInt->val));
}

void *PREFIX_FLOAT_METHOD_NAME(__op_unplus____o)(void *rhs)
{
    System::fpnum *rhsInt = (System::fpnum*)rhs;
    return (void*)(new kite::stdlib::System::fpnum(rhsInt->val));
}

void *PREFIX_FLOAT_METHOD_NAME(__op_equals____oo)(void *lhs, void *rhs)
{
    System::fpnum *lhsInt = (System::fpnum*)lhs;
    System::fpnum *rhsInt = (System::fpnum*)rhs;
    verify_float_type(lhsInt, rhsInt);
    return (void*)(new kite::stdlib::System::boolean(lhsInt->val == rhsInt->val));
}

void *PREFIX_FLOAT_METHOD_NAME(__op_nequals____oo)(void *lhs, void *rhs)
{
    System::fpnum *lhsInt = (System::fpnum*)lhs;
    System::fpnum *rhsInt = (System::fpnum*)rhs;
    verify_float_type(lhsInt, rhsInt);
    return (void*)(new kite::stdlib::System::boolean(lhsInt->val != rhsInt->val));
}

void *PREFIX_FLOAT_METHOD_NAME(__op_lt____oo)(void *lhs, void *rhs)
{
    System::fpnum *leftObject = (System::fpnum*)lhs;
    System::fpnum *rightObject = (System::fpnum*)rhs;
    verify_float_type(leftObject, rightObject);
    return (void*)(new System::boolean(leftObject->val < rightObject->val));
}

void *PREFIX_FLOAT_METHOD_NAME(__op_gt____oo)(void *lhs, void *rhs)
{
    System::fpnum *leftObject = (System::fpnum*)lhs;
    System::fpnum *rightObject = (System::fpnum*)rhs;
    verify_float_type(leftObject, rightObject);
    return (void*)(new System::boolean(leftObject->val > rightObject->val));
}

void *PREFIX_FLOAT_METHOD_NAME(__op_leq____oo)(void *lhs, void *rhs)
{
    System::fpnum *leftObject = (System::fpnum*)lhs;
    System::fpnum *rightObject = (System::fpnum*)rhs;
    verify_float_type(leftObject, rightObject);
    return (void*)(new System::boolean(leftObject->val <= rightObject->val));
}

void *PREFIX_FLOAT_METHOD_NAME(__op_geq____oo)(void *lhs, void *rhs)
{
    System::fpnum *leftObject = (System::fpnum*)lhs;
    System::fpnum *rightObject = (System::fpnum*)rhs;
    verify_float_type(leftObject, rightObject);
    return (void*)(new System::boolean(leftObject->val >= rightObject->val));
}
