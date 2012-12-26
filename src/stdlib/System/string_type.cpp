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
#include "integer.h"
#include "float.h"
#include "exceptions/InvalidArgument.h"
#include "exceptions/TypeMismatch.h"
#include "list.h"
#include "config.h"

using namespace boost::assign;

#define CHECK_TYPE(object, t) \
    if (object == NULL) { \
        exc = kite::stdlib::System::exceptions::InvalidArgument::Create(1, \
            new kite::stdlib::System::string( \
                "Not enough objects have been passed in for the format string given.")); \
        exc->throw_exception(); \
        return NULL; \
    } \
    if (object->type != t) { \
        exc = kite::stdlib::System::exceptions::TypeMismatch::Create(1, \
            new kite::stdlib::System::string( \
                "Object passed to format is of invalid type for the format " \
                "specifier provided.")); \
        exc->throw_exception(); \
        return NULL; \
    }

#define FORMAT(out, type, formatted_object, val) \
    fmtstr = (char*)GC_malloc_atomic(length + 1); \
    strncpy(fmtstr, cur - length + 1, length); \
    asprintf(&out, fmtstr, \
        (type)formatted_object->val); \
    GC_free(fmtstr);

#ifndef HAVE_ASPRINTF
void asprintf(char **out, char *fmt, ...)
{
    va_list ap;
    char *ret;

    va_start(ap, fmt);
    switch(*(fmt+1))
    {    
        case 's':
        {
            char *ptr = va_arg(ap, char*);
            ret = (char*)GC_malloc_atomic(strlen(ptr) + 1);
            strcpy(ret, ptr);
            break;
        }
        default:
        {
            /* Really, we need to do this better to avoid buffer overflows. */
            ret = (char*)GC_malloc_atomic(256);
            vsprintf(ret, fmt, ap);
            break;
        }
    }

    va_end(ap);
    *out = ret;
}
#endif

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
                ("format__so", function_semantics(semantics::STRING, (void*)&(PREFIX_STRING_METHOD_NAME(format__so))))
                ("format__oo", function_semantics(semantics::OBJECT, (void*)&(PREFIX_STRING_METHOD_NAME(format__oo))))
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
            
            char* string::format(list *format_list)
            {
                char *output = NULL, *ret_string = NULL, *cur, *fmtstr;
                int length = 0;
                kite::stdlib::System::exceptions::exception *exc;

                if (format_list == NULL)
                {
                    format_list = list::Create(0);
                }

                cur = const_cast<char*>(string_val.c_str());
                list_contents_type::iterator cur_fmt = format_list->list_contents.begin();
                while(*cur && cur_fmt != format_list->list_contents.end()) {
                    if (*cur == '%') {
                        cur++;
                        length++;
cont_parse:
                        while(*cur && *cur != '%' && !isalpha(*cur)) {
                            cur++;
                            length++;
                        }
                        if (*cur == '%') goto cont_parse;

                        length++;
                        switch(*cur) {
                            case 'd':
                            case 'i':
                            case 'o':
                            case 'u':
                            case 'x':
                            case 'X':
                            {
                                /* Integer conversion. */
                                kite::stdlib::System::integer *intVal = (kite::stdlib::System::integer*)*cur_fmt;
                                CHECK_TYPE(intVal, kite::semantics::INTEGER);
                                FORMAT(output, int, intVal, val);
                                break;
                            }
                            case 'D':
                            case 'O':
                            case 'U':
                            {
                                /* Long conversion. */
                                kite::stdlib::System::integer *intVal = (kite::stdlib::System::integer*)*cur_fmt;
                                CHECK_TYPE(intVal, kite::semantics::INTEGER);
                                FORMAT(output, long, intVal, val);
                                break;
                            }
                            case 'e':
                            case 'E':
                            case 'F':
                            case 'f':
                            case 'G':
                            case 'g':
                            case 'A':
                            case 'a':
                            {
                                /* Double conversion. */
                                kite::stdlib::System::fpnum *fp = (kite::stdlib::System::fpnum*)*cur_fmt;
                                CHECK_TYPE(fp, kite::semantics::FLOAT);
                                FORMAT(output, double, fp, val);
                                break;
                            }
                            case 's':
                            {
                                /* String conversion */
                                kite::stdlib::System::string *str = (kite::stdlib::System::string*)*cur_fmt;
                                CHECK_TYPE(str, kite::semantics::STRING);
                                FORMAT(output, char*, str, string_val.c_str());
                                break;
                            }
                            case 'c':
                            {
                                /* Character conversion */
                                kite::stdlib::System::integer *intVal = (kite::stdlib::System::integer*)*cur_fmt;
                                CHECK_TYPE(intVal, kite::semantics::INTEGER);
                                FORMAT(output, int, intVal, val);
                                break;
                            }
                            default:
                                /* Invalid format string. */
                                exc = kite::stdlib::System::exceptions::InvalidArgument::Create(
                                    1,
                                    new kite::stdlib::System::string("Provided format string is invalid."));
                                exc->throw_exception();
                                return NULL;
                        }

                        length = 0;
                        if (ret_string == NULL) {
                            ret_string = GC_strdup(output);
                            GC_free(output);
                        } else {
                            ret_string = (char*)GC_realloc(ret_string, strlen(ret_string) + 
                                                 strlen(output) + 1);
                            strcat(ret_string, output);
                            GC_free(output);
                        }
                        cur++;
                        cur_fmt++;
                        continue;
                    }

                    cur++;
                    length++;
                }

                if (cur_fmt != format_list->list_contents.end() && !*cur) {
                    exc = kite::stdlib::System::exceptions::InvalidArgument::Create(
                        1,
                        new kite::stdlib::System::string(
                                        "Provided format string has fewer format "
                                        "specifiers than required."));
                    exc->throw_exception();
                    return NULL;
                } else if (length > 0 || *cur) {
                    asprintf(&output, cur - length);
                    if (ret_string != NULL) {
                        ret_string = 
                            (char*)GC_realloc(ret_string, strlen(ret_string) + strlen(output) + 1);
                        strcat(ret_string, output);
                        GC_free(output);
                    } else {
                        ret_string = GC_strdup(output);
                        GC_free(output);
                    }
                } else if (!ret_string) {
                    ret_string = (char*)GC_malloc_atomic(1);
                    ret_string[0] = 0;
                }

                return ret_string;
            }
            
            System::object *string::to_object()
            {
                return (System::object*)(PREFIX_STRING_METHOD_NAME(obj__s)(string_val.c_str()));
            }
            
            void string::InitializeClass()
            {
                class_object.properties["__name"] = new kite::stdlib::System::string("System.string");
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
    char *ret = (char*)GC_malloc_atomic(strlen(val) + strlen(rhs) + 1);
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
    
    char *ret = (char*)GC_malloc_atomic(2);
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
    char *ret = (char*)GC_malloc_atomic(strlen(val) + 1);
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
    
    char *ret = (char*)GC_malloc_atomic(strlen(val) + 1);
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
    char *ret = (char*)GC_malloc_atomic(strlen(val) + 1);
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
    char *ret = (char*)GC_malloc_atomic(lhs_str->string_val.size() + 1);
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
    char *ret = (char*)GC_malloc_atomic(strlen(val) + 1);
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
    
    return new System::string((lhs_str->string_val + rhs_str->string_val).c_str());
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

void* PREFIX_STRING_METHOD_NAME(format__oo)(void *val, void *args)
{
    System::list *argList = (System::list*)args;
    System::string *stringVal = (System::string*)val;
    return new System::string(stringVal->format(argList));
}

char* PREFIX_STRING_METHOD_NAME(format__so)(const char *val, void *args)
{
    System::string *stringVal = (System::string*)PREFIX_STRING_METHOD_NAME(format__oo)(new System::string(val), args);
    return GC_strdup(stringVal->string_val.c_str());
}