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
#include <sstream>
#include <boost/assign.hpp>
#include "string_type.h"
#include "boolean.h"
#include "exceptions/InvalidArgument.h"

using namespace boost::assign;
 
namespace kite
{
    namespace stdlib
    {
        namespace System
        {
            System::dynamic_object string::class_object;
            
            object_method_map string::method_map = map_list_of
                ("__op_plus____oo", function_semantics(semantics::OBJECT, (void*)&(PREFIX_STRING_METHOD_NAME(__op_plus____oo))))
                ("__op_equals____oo", function_semantics(semantics::OBJECT, (void*)&(PREFIX_STRING_METHOD_NAME(__op_equals____oo))))
                ("__op_nequals____oo", function_semantics(semantics::OBJECT, (void*)&(PREFIX_STRING_METHOD_NAME(__op_nequals____oo))))
                ("__op_lt____oo", function_semantics(semantics::OBJECT, (void*)&(PREFIX_STRING_METHOD_NAME(__op_lt____oo))))
                ("__op_gt____oo", function_semantics(semantics::OBJECT, (void*)&(PREFIX_STRING_METHOD_NAME(__op_gt____oo))))
                ("__op_leq____oo", function_semantics(semantics::OBJECT, (void*)&(PREFIX_STRING_METHOD_NAME(__op_leq____oo))))
                ("__op_geq____oo", function_semantics(semantics::OBJECT, (void*)&(PREFIX_STRING_METHOD_NAME(__op_geq____oo))))
                ("append__ss", function_semantics(semantics::STRING, (void*)&(PREFIX_STRING_METHOD_NAME(append__ss))))
                ("asc__s", function_semantics(semantics::INTEGER, (void*)&(PREFIX_STRING_METHOD_NAME(asc__s))))
                ("bool__s", function_semantics(semantics::BOOLEAN, (void*)&(PREFIX_STRING_METHOD_NAME(bool__s))))
                ("charAt__si", function_semantics(semantics::STRING, (void*)&(PREFIX_STRING_METHOD_NAME(charAt__si))))
                ("float__s", function_semantics(semantics::FLOAT, (void*)&(PREFIX_STRING_METHOD_NAME(float__s))))
                ("int__s", function_semantics(semantics::INTEGER, (void*)&(PREFIX_STRING_METHOD_NAME(int__s))))
                ("length__s", function_semantics(semantics::INTEGER, (void*)&(PREFIX_STRING_METHOD_NAME(length__s))))
                ("lower__s", function_semantics(semantics::STRING, (void*)&(PREFIX_STRING_METHOD_NAME(lower__s))))
                ("ltrim__s", function_semantics(semantics::STRING, (void*)&(PREFIX_STRING_METHOD_NAME(ltrim__s))))
                ("print__s", function_semantics(semantics::STRING, (void*)&(PREFIX_STRING_METHOD_NAME(print__s))))
                ("print__o", function_semantics(semantics::OBJECT, (void*)&(PREFIX_STRING_METHOD_NAME(print__o))))
                ("rtrim__s", function_semantics(semantics::STRING, (void*)&(PREFIX_STRING_METHOD_NAME(rtrim__s))))
                ("str__s", function_semantics(semantics::STRING, (void*)&(PREFIX_STRING_METHOD_NAME(str__s))))
                ("str__o", function_semantics(semantics::STRING, (void*)&(PREFIX_STRING_METHOD_NAME(str__o))))
                ("trim__s", function_semantics(semantics::STRING, (void*)&(PREFIX_STRING_METHOD_NAME(trim__s))))
                ("upper__s", function_semantics(semantics::STRING, (void*)&(PREFIX_STRING_METHOD_NAME(upper__s))))
                ("obj__s", function_semantics(semantics::OBJECT, (void*)&(PREFIX_STRING_METHOD_NAME(obj__s))))
                ("obj__o", function_semantics(semantics::OBJECT, (void*)&(PREFIX_STRING_METHOD_NAME(obj__o))));
                                
            int string::asc()
            {
                return PREFIX_STRING_METHOD_NAME(asc__s)(string_val.c_str());
            }
            
            char* string::append(char *rhs)
            {
                return PREFIX_STRING_METHOD_NAME(append__ss)(string_val.c_str(), rhs);
            }
            
            bool string::to_boolean()
            {
                return PREFIX_STRING_METHOD_NAME(bool__s)(string_val.c_str());
            }
            
            char* string::charAt(int index)
            {
                return PREFIX_STRING_METHOD_NAME(charAt__si)(string_val.c_str(), index);
            }
            
            double string::to_float()
            {
                return PREFIX_STRING_METHOD_NAME(float__s)(string_val.c_str());
            }
            
            int string::to_integer()
            {
                return PREFIX_STRING_METHOD_NAME(int__s)(string_val.c_str());
            }
            
            int string::length()
            {
                return PREFIX_STRING_METHOD_NAME(length__s)(string_val.c_str());
            }
            
            char* string::lower()
            {
                return PREFIX_STRING_METHOD_NAME(lower__s)(string_val.c_str());
            }
            
            char* string::ltrim()
            {
                return PREFIX_STRING_METHOD_NAME(ltrim__s)(string_val.c_str());
            }
            
            char* string::rtrim()
            {
                return PREFIX_STRING_METHOD_NAME(rtrim__s)(string_val.c_str());
            }
            
            char* string::str()
            {
                return PREFIX_STRING_METHOD_NAME(str__s)(string_val.c_str());
            }
            
            char* string::trim()
            {
                return PREFIX_STRING_METHOD_NAME(trim__s)(string_val.c_str());
            }
            
            char* string::upper()
            {
                return PREFIX_STRING_METHOD_NAME(upper__s)(string_val.c_str());
            }
            
            System::object *string::to_object()
            {
                return (System::object*)(PREFIX_STRING_METHOD_NAME(obj__s)(string_val.c_str()));
            }
            
            void string::InitializeClass()
            {
                class_object.properties["__name"] = new System::string("System.string");
            }
        }
    }
}

using namespace kite::stdlib;

int PREFIX_STRING_METHOD_NAME(asc__s)(const char *val)
{
    return *val;
}

char* PREFIX_STRING_METHOD_NAME(append__ss)(const char* val, char* rhs)
{
    char *ret = (char*)malloc(strlen(val) + strlen(rhs) + 1);
    strcpy(ret, val);
    strcat(ret, rhs);
    return ret;
}

bool PREFIX_STRING_METHOD_NAME(bool__s)(const char* val)
{
    return val != NULL && strlen(val) > 0;
}

char* PREFIX_STRING_METHOD_NAME(charAt__si)(const char *val, int index)
{
    if (val == NULL || index >= strlen(val) || index < 0)
    {
        std::stringstream ss;

        ss << "Index " << index << " is outside the range [0, " << strlen(val) << ")";
        kite::stdlib::System::exceptions::exception *exc = kite::stdlib::System::exceptions::InvalidArgument::Create(
            1,
            new kite::stdlib::System::string(
                ss.str().c_str()                                
            ));
        exc->throw_exception();
    }
    
    char *ret = (char*)malloc(2);
    ret[0] = val[index];
    ret[1] = 0;
    return ret;
}

double PREFIX_STRING_METHOD_NAME(float__s)(const char* val)
{
    return (double)atof(val);
}

int PREFIX_STRING_METHOD_NAME(int__s)(const char* val)
{
    return atoi(val);
}

int PREFIX_STRING_METHOD_NAME(length__s)(const char* val)
{
    return strlen(val);
}

char* PREFIX_STRING_METHOD_NAME(lower__s)(const char *val)
{
    char *ret = (char*)malloc(strlen(val) + 1);
    char *tmp = ret;
    for ( ; *val != 0; val++, tmp++)
    {
        *tmp = tolower(*val);
    }
    *tmp = 0;
    return ret;
}

char* PREFIX_STRING_METHOD_NAME(ltrim__s)(const char *val)
{
    while (isspace(*val)) { val++; }
    
    char *ret = (char*)malloc(strlen(val) + 1);
    strcpy(ret, val);
    return ret;
}

void* PREFIX_STRING_METHOD_NAME(obj__s)(const char *val)
{
    return (void*)(new kite::stdlib::System::string(val));
}

void* PREFIX_STRING_METHOD_NAME(obj__o)(void *val)
{
    return val;
}

char* PREFIX_STRING_METHOD_NAME(print__s)(const char *val)
{
    std::cout << val << std::endl;
    return const_cast<char*>(val);
}

void* PREFIX_STRING_METHOD_NAME(print__o)(void *val)
{
    System::string *stringObj = (System::string*)val;
    std::cout << stringObj->string_val << std::endl;
    return val;
}

char* PREFIX_STRING_METHOD_NAME(rtrim__s)(const char *val)
{
    char *ret = (char*)calloc(1, strlen(val) + 1);
    char *tmp = ret + strlen(val) - 1;
    
    strcpy(ret, val);
    while (isspace(*tmp)) { *tmp = 0; tmp--; }
    return ret;
}

char* PREFIX_STRING_METHOD_NAME(str__s)(const char *val)
{
    return const_cast<char*>(val);
}

char* PREFIX_STRING_METHOD_NAME(str__o)(void *val)
{
    System::string *lhs_str = (System::string*)val;
    char *ret = (char*)calloc(1, lhs_str->string_val.size() + 1);
    strcpy(ret, lhs_str->string_val.c_str());
    return ret;
}

char* PREFIX_STRING_METHOD_NAME(trim__s)(const char *val)
{
    char *rtrimmed_str = PREFIX_STRING_METHOD_NAME(rtrim__s)(val);
    return PREFIX_STRING_METHOD_NAME(ltrim__s)(rtrimmed_str);
}

char* PREFIX_STRING_METHOD_NAME(upper__s)(const char *val)
{
    char *ret = (char*)malloc(strlen(val) + 1);
    char *tmp = ret;
    for ( ; *val != 0; val++, tmp++)
    {
        *tmp = toupper(*val);
    }
    *tmp = 0;
    return ret;
}

void *PREFIX_STRING_METHOD_NAME(__op_plus____oo)(void *lhs, void *rhs)
{
    System::string *lhs_str = (System::string*)lhs;
    System::string *rhs_str = (System::string*)rhs;
    
    return new System::string(lhs_str->string_val + rhs_str->string_val);
}

void *PREFIX_STRING_METHOD_NAME(__op_equals____oo)(void *lhs, void *rhs)
{
    System::string *lhs_str = (System::string*)lhs;
    System::string *rhs_str = (System::string*)rhs;
    
    return new System::boolean(lhs_str->string_val == rhs_str->string_val);
}

void *PREFIX_STRING_METHOD_NAME(__op_nequals____oo)(void *lhs, void *rhs)
{
    System::string *lhs_str = (System::string*)lhs;
    System::string *rhs_str = (System::string*)rhs;
    
    return new System::boolean(lhs_str->string_val != rhs_str->string_val);
}

void *PREFIX_STRING_METHOD_NAME(__op_lt____oo)(void *lhs, void *rhs)
{
    System::string *lhs_str = (System::string*)lhs;
    System::string *rhs_str = (System::string*)rhs;
    
    return new System::boolean(lhs_str->string_val < rhs_str->string_val);
}

void *PREFIX_STRING_METHOD_NAME(__op_gt____oo)(void *lhs, void *rhs)
{
    System::string *lhs_str = (System::string*)lhs;
    System::string *rhs_str = (System::string*)rhs;
    
    return new System::boolean(lhs_str->string_val > rhs_str->string_val);
}

void *PREFIX_STRING_METHOD_NAME(__op_leq____oo)(void *lhs, void *rhs)
{
    System::string *lhs_str = (System::string*)lhs;
    System::string *rhs_str = (System::string*)rhs;
    
    return new System::boolean(lhs_str->string_val <= rhs_str->string_val);
}

void *PREFIX_STRING_METHOD_NAME(__op_geq____oo)(void *lhs, void *rhs)
{
    System::string *lhs_str = (System::string*)lhs;
    System::string *rhs_str = (System::string*)rhs;
    
    return new System::boolean(lhs_str->string_val >= rhs_str->string_val);
}