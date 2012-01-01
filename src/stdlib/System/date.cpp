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
#include <cstring>
#include <ctime>
#include <assert.h>
#include "date.h"

namespace kite
{
    namespace stdlib
    {
        namespace System
        {
            namespace date
            {
                static void datePropertiesToStructTm(date *d, struct tm *t)
                {
                    t->tm_sec = KITE_GET_INTEGER_PROPERTY(d, sec);
                    t->tm_min = KITE_GET_INTEGER_PROPERTY(d, min);
                    t->tm_hour = KITE_GET_INTEGER_PROPERTY(d, hour);
                    t->tm_mday = KITE_GET_INTEGER_PROPERTY(d, day_of_month);
                    t->tm_mon = KITE_GET_INTEGER_PROPERTY(d, month);
                    t->tm_year = KITE_GET_INTEGER_PROPERTY(d, year);
                    t->tm_wday = KITE_GET_INTEGER_PROPERTY(d, day_of_week);
                    t->tm_yday = KITE_GET_INTEGER_PROPERTY(d, day_of_year);
                    t->tm_isdst = KITE_GET_BOOLEAN_PROPERTY(d, dst);
                }
            
                object *date::format(string *format)
                {
                    struct tm thistime;
                    char buf[2048];
                
                    datePropertiesToStructTm(this, &thistime);
                    strftime(buf, 2048, format->string_val.c_str(), &thistime);
                    return new string(buf);
                }
            
                object *date::now_gmt()
                {
                    time_t t = time(NULL);
                    struct tm *current;                
                    current = gmtime(&t);
                
                    return date::Create(9, 
                        new integer(current->tm_sec),
                        new integer(current->tm_min),
                        new integer(current->tm_hour),
                        new integer(current->tm_mday),
                        new integer(current->tm_mon),
                        new integer(current->tm_year),
                        new integer(current->tm_wday),
                        new integer(current->tm_yday),
                        new boolean(current->tm_isdst));
                }
            
                object *date::now_local()
                {
                    time_t t = time(NULL);
                    struct tm *current;                
                    current = localtime(&t);
                
                    return date::Create(9, 
                        new integer(current->tm_sec),
                        new integer(current->tm_min),
                        new integer(current->tm_hour),
                        new integer(current->tm_mday),
                        new integer(current->tm_mon),
                        new integer(current->tm_year),
                        new integer(current->tm_wday),
                        new integer(current->tm_yday),
                        new boolean(current->tm_isdst));
                }
            
                object *date::parse(string *value, string *format)
                {
                    struct tm t;
                    strptime(value->string_val.c_str(), format->string_val.c_str(), &t);
                    return date::Create(9, 
                        new integer(t.tm_sec),
                        new integer(t.tm_min),
                        new integer(t.tm_hour),
                        new integer(t.tm_mday),
                        new integer(t.tm_mon),
                        new integer(t.tm_year),
                        new integer(t.tm_wday),
                        new integer(t.tm_yday),
                        new boolean(t.tm_isdst));
                }
            
                const char *date::str()
                {
                    struct tm thistime;
                    datePropertiesToStructTm(this, &thistime);
                    time_t t = mktime(&thistime);
                    return ctime(&t);
                }
            
                object *date::timestamp()
                {
                    struct tm thistime;
                    datePropertiesToStructTm(this, &thistime);
                    return new integer(mktime(&thistime));
                }
            }
        }
    }
}

REGISTER_KITE_CLASS(kite::stdlib::System::System, kite::stdlib::System::date::date);