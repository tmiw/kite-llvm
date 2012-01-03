/*****************************************************************************
 * Copyright (c) 2012, Mooneer Salem
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
 
#ifndef KITE_STDLIB__SYSTEM__MATH_H
#define KITE_STDLIB__SYSTEM__MATH_H

#include <cmath>
#include "../System.h"
#include "exceptions/TypeMismatch.h"
#include "float.h"
#include "integer.h"
#include "boolean.h"

extern "C" int isnan(double);

#define DEFINE_ZERO_PARAM_MATH_FUNCTION(name) KITE_METHOD_DEFINE(name, 0, &math::s_ ## name);
#define DEFINE_ONE_PARAM_MATH_FUNCTION(name) KITE_METHOD_DEFINE(name, 1, &math::s_ ## name);
#define DEFINE_TWO_PARAM_MATH_FUNCTION(name) KITE_METHOD_DEFINE(name, 2, &math::s_ ## name);

#define ZERO_PARAM_MATH_FUNCTION(name) \
    static object *s_ ## name(math *) \
    { \
        return new fpnum(name()); \
    }

#define ONE_PARAM_MATH_FUNCTION(name) \
    static object *s_ ## name(math *, fpnum *p1) \
    { \
        if (p1->type != semantics::FLOAT) \
        { \
            exceptions::exception *exc = exceptions::TypeMismatch::Create( \
                1, \
                new string("float expected.") \
            ); \
            exc->throw_exception(); \
        } \
        \
        return new fpnum(name(p1->val)); \
    }

#define ONE_PARAM_MATH_FUNCTION_BOOL_RET(name) \
    static object *s_ ## name(math *, fpnum *p1) \
    { \
        if (p1->type != semantics::FLOAT) \
        { \
            exceptions::exception *exc = exceptions::TypeMismatch::Create( \
                1, \
                new string("float expected.") \
            ); \
            exc->throw_exception(); \
        } \
        \
        return new boolean(name(p1->val)); \
    }

#define ONE_PARAM_VOID_MATH_FUNCTION_INT(name) \
    static object *s_ ## name(math *, integer *p1) \
    { \
        if (p1->type != semantics::INTEGER) \
        { \
            exceptions::exception *exc = exceptions::TypeMismatch::Create( \
                1, \
                new string("integer expected.") \
            ); \
            exc->throw_exception(); \
        } \
        \
        name(p1->val); \
        return NULL; \
    }
    
#define TWO_PARAM_MATH_FUNCTION(name) \
    static object *s_ ## name(math *, fpnum *p1, fpnum *p2) \
    { \
        if (p1->type != semantics::FLOAT || p2->type != semantics::FLOAT) \
        { \
            exceptions::exception *exc = exceptions::TypeMismatch::Create( \
                1, \
                new string("float expected.") \
            ); \
            exc->throw_exception(); \
        } \
        \
        return new fpnum(name(p1->val, p2->val)); \
    }

namespace kite
{
    namespace stdlib
    {
        namespace System
        {
            namespace math
            {
                BEGIN_KITE_BASE_CLASS(math)
                private:
                    ONE_PARAM_MATH_FUNCTION(acos)
                    ONE_PARAM_MATH_FUNCTION(acosh)
                    ONE_PARAM_MATH_FUNCTION(asin)
                    ONE_PARAM_MATH_FUNCTION(asinh)
                    ONE_PARAM_MATH_FUNCTION(atan)
                    TWO_PARAM_MATH_FUNCTION(atan2)
                    ONE_PARAM_MATH_FUNCTION(atanh)
                    ONE_PARAM_MATH_FUNCTION(cbrt)
                    ONE_PARAM_MATH_FUNCTION(ceil)
                    ONE_PARAM_MATH_FUNCTION(cos)
                    ONE_PARAM_MATH_FUNCTION(cosh)
                    ONE_PARAM_MATH_FUNCTION(erf)
                    ONE_PARAM_MATH_FUNCTION(erfc)
                    ONE_PARAM_MATH_FUNCTION(exp)
                    ONE_PARAM_MATH_FUNCTION(fabs)
                    ONE_PARAM_MATH_FUNCTION(floor)
                    TWO_PARAM_MATH_FUNCTION(fmod) // TODO: verify division by zero
                    TWO_PARAM_MATH_FUNCTION(hypot)
                    ONE_PARAM_MATH_FUNCTION(ilogb)
                    ONE_PARAM_MATH_FUNCTION_BOOL_RET(isnan)
                    ONE_PARAM_MATH_FUNCTION(j0)
                    ONE_PARAM_MATH_FUNCTION(j1)
                    ONE_PARAM_MATH_FUNCTION(lgamma)
                    ONE_PARAM_MATH_FUNCTION(log)
                    ONE_PARAM_MATH_FUNCTION(log10)
                    ONE_PARAM_MATH_FUNCTION(logb)
                    TWO_PARAM_MATH_FUNCTION(nextafter)
                    TWO_PARAM_MATH_FUNCTION(pow)
                    ZERO_PARAM_MATH_FUNCTION(random)
                    TWO_PARAM_MATH_FUNCTION(remainder)
                    ONE_PARAM_MATH_FUNCTION(rint)
                    TWO_PARAM_MATH_FUNCTION(scalb)
                    ONE_PARAM_MATH_FUNCTION(sin)
                    ONE_PARAM_MATH_FUNCTION(sinh)
                    ONE_PARAM_MATH_FUNCTION(sqrt)
                    ONE_PARAM_VOID_MATH_FUNCTION_INT(srandom)
                    ONE_PARAM_MATH_FUNCTION(tan)
                    ONE_PARAM_MATH_FUNCTION(tanh)
                    ONE_PARAM_MATH_FUNCTION(y0)
                    ONE_PARAM_MATH_FUNCTION(y1)
                    
                public:
                    BEGIN_KITE_CLASS_INITIALIZER
                        DEFINE_ONE_PARAM_MATH_FUNCTION(acos)
                        DEFINE_ONE_PARAM_MATH_FUNCTION(acosh)
                        DEFINE_ONE_PARAM_MATH_FUNCTION(asin)
                        DEFINE_ONE_PARAM_MATH_FUNCTION(asinh)
                        DEFINE_ONE_PARAM_MATH_FUNCTION(atan)
                        DEFINE_TWO_PARAM_MATH_FUNCTION(atan2)
                        DEFINE_ONE_PARAM_MATH_FUNCTION(atanh)
                        DEFINE_ONE_PARAM_MATH_FUNCTION(cbrt)
                        DEFINE_ONE_PARAM_MATH_FUNCTION(ceil)
                        DEFINE_ONE_PARAM_MATH_FUNCTION(cos)
                        DEFINE_ONE_PARAM_MATH_FUNCTION(cosh)
                        DEFINE_ONE_PARAM_MATH_FUNCTION(erf)
                        DEFINE_ONE_PARAM_MATH_FUNCTION(erfc)
                        DEFINE_ONE_PARAM_MATH_FUNCTION(exp)
                        DEFINE_ONE_PARAM_MATH_FUNCTION(fabs)
                        DEFINE_ONE_PARAM_MATH_FUNCTION(floor)
                        DEFINE_TWO_PARAM_MATH_FUNCTION(fmod) // TODO: verify division by zero
                        DEFINE_TWO_PARAM_MATH_FUNCTION(hypot)
                        DEFINE_ONE_PARAM_MATH_FUNCTION(ilogb)
                        DEFINE_ONE_PARAM_MATH_FUNCTION(isnan)
                        DEFINE_ONE_PARAM_MATH_FUNCTION(j0)
                        DEFINE_ONE_PARAM_MATH_FUNCTION(j1)
                        DEFINE_ONE_PARAM_MATH_FUNCTION(lgamma)
                        DEFINE_ONE_PARAM_MATH_FUNCTION(log)
                        DEFINE_ONE_PARAM_MATH_FUNCTION(log10)
                        DEFINE_ONE_PARAM_MATH_FUNCTION(logb)
                        DEFINE_TWO_PARAM_MATH_FUNCTION(nextafter)
                        DEFINE_TWO_PARAM_MATH_FUNCTION(pow)
                        DEFINE_ZERO_PARAM_MATH_FUNCTION(random)
                        DEFINE_TWO_PARAM_MATH_FUNCTION(remainder)
                        DEFINE_ONE_PARAM_MATH_FUNCTION(rint)
                        DEFINE_TWO_PARAM_MATH_FUNCTION(scalb)
                        DEFINE_ONE_PARAM_MATH_FUNCTION(sin)
                        DEFINE_ONE_PARAM_MATH_FUNCTION(sinh)
                        DEFINE_ONE_PARAM_MATH_FUNCTION(sqrt)
                        DEFINE_ONE_PARAM_MATH_FUNCTION(srandom)
                        DEFINE_ONE_PARAM_MATH_FUNCTION(tan)
                        DEFINE_ONE_PARAM_MATH_FUNCTION(tanh)
                        DEFINE_ONE_PARAM_MATH_FUNCTION(y0)
                        DEFINE_ONE_PARAM_MATH_FUNCTION(y1)
                        
                        // Define constants
                        class_obj.properties["E"] = new fpnum(M_E);
                        class_obj.properties["LN10"] = new fpnum(M_LN10);
                        class_obj.properties["LN2"] = new fpnum(M_LN2);
                        class_obj.properties["LOG10E"] = new fpnum(M_LOG10E);
                        class_obj.properties["LOG2E"] = new fpnum(M_LOG2E);
                        class_obj.properties["ONE_PI"] = new fpnum(M_1_PI);
                        class_obj.properties["PI"] = new fpnum(M_PI);
                        class_obj.properties["PI_2"] = new fpnum(M_PI_2);
                        class_obj.properties["PI_4"] = new fpnum(M_PI_4);
                        class_obj.properties["SQRT1_2"] = new fpnum(M_SQRT1_2);
                        class_obj.properties["SQRT2"] = new fpnum(M_SQRT2);
                        class_obj.properties["TWO_PI"] = new fpnum(M_2_PI);
                        class_obj.properties["TWO_SQRTPI"] = new fpnum(M_2_SQRTPI);
                    END_KITE_CLASS_INITIALIZER
                };
            }
        }
    }
}

#endif