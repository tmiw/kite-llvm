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
#include "string.h"
using namespace boost::assign;
 
namespace kite
{
    namespace stdlib
    {
        namespace System
        {
            object_method_map string::method_map = map_list_of
                ("append__ss", function_semantics(semantics::STRING, (void*)&string::append))
                ("asc__s", function_semantics(semantics::INTEGER, (void*)&string::asc))
                ("bool__s", function_semantics(semantics::BOOLEAN, (void*)&string::to_boolean))
                ("charAt__si", function_semantics(semantics::STRING, (void*)&string::charAt))
                ("float__s", function_semantics(semantics::FLOAT, (void*)&string::to_float))
                ("int__s", function_semantics(semantics::INTEGER, (void*)&string::to_integer))
                ("length__s", function_semantics(semantics::INTEGER, (void*)&string::length))
                ("lower__s", function_semantics(semantics::STRING, (void*)&string::lower))
                ("ltrim__s", function_semantics(semantics::STRING, (void*)&string::ltrim))
                ("print__s", function_semantics(semantics::STRING, (void*)&string::print))
                ("rtrim__s", function_semantics(semantics::STRING, (void*)&string::rtrim))
                ("str__s", function_semantics(semantics::STRING, (void*)&string::str))
                ("trim__s", function_semantics(semantics::STRING, (void*)&string::trim))
                ("upper__s", function_semantics(semantics::STRING, (void*)&string::upper))
                ("obj__s", function_semantics(semantics::OBJECT, (void*)&string::to_object));
                
            int string::asc(char* val)
            {
                return *val;
            }
            
            char* string::append(char* val, char *rhs)
            {
                char *ret = (char*)malloc(strlen(val) + strlen(rhs) + 1);
                strcpy(ret, val);
                strcat(ret, rhs);
                return ret;
            }
            
            bool string::to_boolean(char* val)
            {
                return val != NULL && length(val) > 0;
            }
            
            char* string::charAt(char *val, int index)
            {
                // TODO: exception for invalid input
                char *ret = (char*)malloc(2);
                ret[0] = val[index];
                ret[1] = 0;
                return ret;
            }
            
            double string::to_float(char* val)
            {
                return (double)atof(val);
            }
            
            int string::to_integer(char* val)
            {
                return atoi(val);
            }
            
            int string::length(char* val)
            {
                return strlen(val);
            }
            
            char* string::lower(char *val)
            {
                char *ret = (char*)malloc(length(val) + 1);
                for (char *tmp = ret; *val != 0; val++, tmp++)
                {
                    *tmp = tolower(*val);
                }
                return ret;
            }
            
            char* string::ltrim(char *val)
            {
                while (isspace(*val)) { val++; }
                
                char *ret = (char*)malloc(length(val) + 1);
                strcpy(ret, val);
                return ret;
            }
            
            char* string::print(char *val)
            {
                std::cout << val << std::endl;
                return val;
            }
            
            char* string::rtrim(char *val)
            {
                char *ret = (char*)calloc(1, strlen(val) + 1);
                char *tmp = ret + strlen(val) - 1;
                
                strcpy(ret, val);
                while (isspace(*tmp)) { *tmp = 0; tmp--; }
                return ret;
            }
            
            char* string::str(char *val)
            {
                return val;
            }
            
            char* string::trim(char *val)
            {
                return ltrim(rtrim(val));
            }
            
            char* string::upper(char *val)
            {
                char *ret = (char*)malloc(length(val) + 1);
                for (char *tmp = ret; *val != 0; val++, tmp++)
                {
                    *tmp = toupper(*val);
                }
                return ret;
            }
            
            System::object *string::to_object(char *val)
            {
                return new string(val);
            }
        }
    }
}