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
 
#ifndef KITE_STDLIB__SYSTEM_REGEX_H
#define KITE_STDLIB__SYSTEM_REGEX_H

#include <boost/regex.hpp>
#include "stdlib/api.h"
#include "../System.h"
#include "integer.h"
#include "string.h"
#include "boolean.h"

namespace kite
{
    namespace stdlib
    {
        namespace System
        {
            namespace regex
            {
                BEGIN_KITE_BASE_CLASS(regex)
                private:
                    boost::regex *regex_obj;
                    
                    static object* s_match_1(regex *r, string *str) { return r->match(str); }
                    static object* s_match_2(regex *r, string *str, integer *start) { return r->match(str, start); }
                    static object* s_replace_2(regex *r, string *str, string *replace_with) { return r->replace(str, replace_with); }
                    static object* s_replace_3(regex *r, string *str, string *replace_with, integer *max) { return r->replace(str, replace_with, max); }
                    static object* s_split_1(regex *r, string *str) { return r->split(str); }
                    static object* s_split_2(regex *r, string *str, integer *max) { return r->split(str, max); }
                    
                    static object* s_initialize_1(regex *r, string *regex)
                    {
                        return s_initialize_2(r, regex, new boolean(false));
                    }
                    
                    static object* s_initialize_2(regex *r, string *regex, boolean *icase)
                    {
                        if (icase->val)
                        {
                            r->regex_obj = new boost::regex(regex->string_val.c_str(), boost::regex::ECMAScript | boost::regex::icase);
                        }
                        else
                        {
                            r->regex_obj = new boost::regex(regex->string_val.c_str(), boost::regex::ECMAScript);                            
                        }
                        
                        return r;
                    }
                    
                    static object* s_destroy(regex *r)
                    {
                        delete r->regex_obj;
                        return r;
                    }
                    
                public:
                    object* match(string *str);
                    object* match(string *str, integer *start);
                    object* replace(string *str, string *replace_with);
                    object* replace(string *str, string *replace_with, integer *max);
                    object* split(string *str);
                    object* split(string *str, integer *max);
                    
                    BEGIN_KITE_CLASS_INITIALIZER
                        KITE_CONSTRUCTOR_DEFINE(1, &regex::s_initialize_1);
                        KITE_CONSTRUCTOR_DEFINE(2, &regex::s_initialize_2);
                        KITE_DESTRUCTOR_DEFINE(&regex::s_destroy);
                        
                        KITE_METHOD_DEFINE(match, 1, &regex::s_match_1);
                        KITE_METHOD_DEFINE(match, 2, &regex::s_match_2);
                        KITE_METHOD_DEFINE(replace, 2, &regex::s_replace_2);
                        KITE_METHOD_DEFINE(replace, 3, &regex::s_replace_3);
                        KITE_METHOD_DEFINE(split, 1, &regex::s_split_1);
                        KITE_METHOD_DEFINE(split, 2, &regex::s_split_2);
                    END_KITE_CLASS_INITIALIZER
                END_KITE_CLASS
            }
        }
    }
}

#endif