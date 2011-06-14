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
 
#ifndef KITE_STDLIB__SYSTEM__STRING_H
#define KITE_STDLIB__SYSTEM__STRING_H

#include "object.h"

#define STRING_METHOD_PREFIX System__string__
#define PREFIX_STRING_METHOD_NAME(name) System__string__ ## name
#define STRING_METHOD_PREFIX_AS_STRING "System__string__"

namespace kite
{
    namespace stdlib
    {
        namespace System
        {
            struct string : System::object
            {
                std::string string_val;
                
                string() : System::object(semantics::STRING), string_val("") { }
                string(std::string val) : System::object(semantics::STRING), string_val(val) { }
                
                static object_method_map method_map;
                int asc();
                char* append(char* rhs);
                bool to_boolean();
                char* charAt(int index);
                double to_float();
                int to_integer();
                int length();
                char* lower();
                char* ltrim();
                char* print();
                char* rtrim();
                char* str();
                char* trim();
                char* upper();
                System::object *to_object();
            };
        }
    }
}

extern "C"
{
    int PREFIX_STRING_METHOD_NAME(asc__s)(const char *val);
    char* PREFIX_STRING_METHOD_NAME(append__ss)(const char* val, char* rhs);
    bool PREFIX_STRING_METHOD_NAME(bool__s)(const char* val);
    char* PREFIX_STRING_METHOD_NAME(charAt__si)(const char *val, int index);
    double PREFIX_STRING_METHOD_NAME(float__s)(const char* val);
    int PREFIX_STRING_METHOD_NAME(int__s)(const char* val);
    int PREFIX_STRING_METHOD_NAME(length__s)(const char* val);
    char* PREFIX_STRING_METHOD_NAME(lower__s)(const char *val);
    char* PREFIX_STRING_METHOD_NAME(ltrim__s)(const char *val);
    void* PREFIX_STRING_METHOD_NAME(obj__s)(const char *val);
    void* PREFIX_STRING_METHOD_NAME(obj__o)(void *val);
    void* PREFIX_STRING_METHOD_NAME(print__o)(void *val);
    char* PREFIX_STRING_METHOD_NAME(print__s)(const char *val);
    char* PREFIX_STRING_METHOD_NAME(rtrim__s)(const char *val);
    char* PREFIX_STRING_METHOD_NAME(str__s)(const char *val);
    char* PREFIX_STRING_METHOD_NAME(trim__s)(const char *val);
    char* PREFIX_STRING_METHOD_NAME(upper__s)(const char *val);
}

#endif
