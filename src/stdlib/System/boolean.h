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
 
#ifndef KITE_STDLIB__SYSTEM__BOOLEAN_H
#define KITE_STDLIB__SYSTEM__BOOLEAN_H

#include "dynamic_object.h"
#include "object.h"

#define BOOLEAN_METHOD_PREFIX System__boolean__
#define PREFIX_BOOLEAN_METHOD_NAME(name) System__boolean__ ## name
#define BOOLEAN_METHOD_PREFIX_AS_STRING "System__boolean__"

namespace kite
{
    namespace stdlib
    {
        namespace System
        {
            struct boolean : System::object
            {
                static System::dynamic_object class_object;
                bool val;
                
                boolean() : System::object(semantics::BOOLEAN), val(0) { }
                boolean(bool val) : System::object(semantics::BOOLEAN), val(val) { }
                
                static object_method_map method_map;
                bool to_boolean();
                int to_integer();
                double to_float();
                System::object *to_object();
                
                static void InitializeClass();
            };
        }
    }
}

extern "C"
{
    bool PREFIX_BOOLEAN_METHOD_NAME(bool__b)(bool val);
    bool PREFIX_BOOLEAN_METHOD_NAME(bool__o)(void *val);
    int PREFIX_BOOLEAN_METHOD_NAME(int__b)(bool val);
    double PREFIX_BOOLEAN_METHOD_NAME(float__b)(bool val);
    bool PREFIX_BOOLEAN_METHOD_NAME(print__b)(bool val);
    void *PREFIX_BOOLEAN_METHOD_NAME(print__o)(void *val);
    void *PREFIX_BOOLEAN_METHOD_NAME(obj__b)(bool val);
    char *PREFIX_BOOLEAN_METHOD_NAME(str__o)(void *val);
    char *PREFIX_BOOLEAN_METHOD_NAME(str__b)(bool val);
    
    void *PREFIX_BOOLEAN_METHOD_NAME(__op_equals____oo)(void *lhs, void *rhs);
    void *PREFIX_BOOLEAN_METHOD_NAME(__op_nequals____oo)(void *lhs, void *rhs);
    void *PREFIX_BOOLEAN_METHOD_NAME(__op_and____oo)(void *lhs, void *rhs);
    void *PREFIX_BOOLEAN_METHOD_NAME(__op_or____oo)(void *lhs, void *rhs);
    void *PREFIX_BOOLEAN_METHOD_NAME(__op_not____o)(void *rhs);
    void *PREFIX_BOOLEAN_METHOD_NAME(__op_xor____oo)(void *lhs, void *rhs);
    
    bool kite_object_is_boolean(void *obj);
}

#endif
