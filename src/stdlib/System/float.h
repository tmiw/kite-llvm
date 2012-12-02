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
 
#ifndef KITE_STDLIB__SYSTEM__FLOAT_H
#define KITE_STDLIB__SYSTEM__FLOAT_H

#include "object.h"
#include "dynamic_object.h"
#include "string_type.h"

#define FLOAT_METHOD_PREFIX System__float__
#define PREFIX_FLOAT_METHOD_NAME(name) System__float__ ## name
#define FLOAT_METHOD_PREFIX_AS_STRING "System__float__"

namespace kite
{
    namespace stdlib
    {
        namespace System
        {
            struct fpnum : System::object
            {
                static System::dynamic_object class_object;
                double val;
                
                fpnum() : System::object(semantics::FLOAT), val(0) { }
                fpnum(double val) : System::object(semantics::FLOAT), val(val) { }
                
                static object_method_map method_map;
                bool to_boolean();
                int to_integer();
                double to_float();
                System::object *to_object();
                
                static void InitializeClass();
                static object *parse(object *t, string *str);
            };
        }
    }
}

extern "C"
{
    bool PREFIX_FLOAT_METHOD_NAME(bool__f)(double val);
    bool PREFIX_FLOAT_METHOD_NAME(bool__o)(void *val);
    int PREFIX_FLOAT_METHOD_NAME(int__f)(double val);
    int PREFIX_FLOAT_METHOD_NAME(int__o)(void *val);
    double PREFIX_FLOAT_METHOD_NAME(float__f)(double val);
    double PREFIX_FLOAT_METHOD_NAME(float__o)(void *val);
    double PREFIX_FLOAT_METHOD_NAME(print__f)(double val);
    void *PREFIX_FLOAT_METHOD_NAME(print__o)(void *obj);
    char *PREFIX_FLOAT_METHOD_NAME(str__f)(double val);
    char *PREFIX_FLOAT_METHOD_NAME(str__o)(void *obj);
    void *PREFIX_FLOAT_METHOD_NAME(obj__f)(double val);
    
    void *PREFIX_FLOAT_METHOD_NAME(__op_plus____oo)(void *lhs, void *rhs);
    void *PREFIX_FLOAT_METHOD_NAME(__op_minus____oo)(void *lhs, void *rhs);
    void *PREFIX_FLOAT_METHOD_NAME(__op_multiply____oo)(void *lhs, void *rhs);
    void *PREFIX_FLOAT_METHOD_NAME(__op_divide____oo)(void *lhs, void *rhs);
    void *PREFIX_FLOAT_METHOD_NAME(__op_mod____oo)(void *lhs, void *rhs);
    void *PREFIX_FLOAT_METHOD_NAME(__op_unminus____o)(void *rhs);
    void *PREFIX_FLOAT_METHOD_NAME(__op_unplus____o)(void *rhs);
    void *PREFIX_FLOAT_METHOD_NAME(__op_equals____oo)(void *lhs, void *rhs);
    void *PREFIX_FLOAT_METHOD_NAME(__op_nequals____oo)(void *lhs, void *rhs);
    void *PREFIX_FLOAT_METHOD_NAME(__op_lt____oo)(void *lhs, void *rhs);
    void *PREFIX_FLOAT_METHOD_NAME(__op_gt____oo)(void *lhs, void *rhs);
    void *PREFIX_FLOAT_METHOD_NAME(__op_leq____oo)(void *lhs, void *rhs);
    void *PREFIX_FLOAT_METHOD_NAME(__op_geq____oo)(void *lhs, void *rhs);
    void *PREFIX_FLOAT_METHOD_NAME(__op_not____o)(void *rhs);
}

#endif
