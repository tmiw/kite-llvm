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
#include <unistd.h>
#include <sys/time.h>
#include <assert.h>
#include "hires.h"

namespace kite
{
    namespace stdlib
    {
        namespace System
        {
            namespace date
            {
                object *hires::subtract(hires *d2)
                {
                    struct timeval tv;
                    
                    tv.tv_sec = KITE_GET_INTEGER_PROPERTY(this, total_sec) - KITE_GET_INTEGER_PROPERTY(d2, total_sec);
                    if (KITE_GET_INTEGER_PROPERTY(d2, total_usec) < KITE_GET_INTEGER_PROPERTY(this, total_usec))
                    {
                        tv.tv_usec--;
                    }
                    tv.tv_usec = KITE_GET_INTEGER_PROPERTY(this, total_usec) - KITE_GET_INTEGER_PROPERTY(d2, total_usec);
                    
                    hires *result = hires::Create(0);
                    result->properties["total_usec"] = new integer(tv.tv_usec);
                    result->properties["total_sec"] = new integer(tv.tv_sec);
                    return result;
                }
                
                object *hires::add(hires *d2)
                {
                    struct timeval tv;
                    
                    tv.tv_sec = KITE_GET_INTEGER_PROPERTY(this, total_sec) + KITE_GET_INTEGER_PROPERTY(d2, total_sec);
                    tv.tv_usec = KITE_GET_INTEGER_PROPERTY(this, total_usec) + KITE_GET_INTEGER_PROPERTY(d2, total_usec);
                    if (tv.tv_usec >= 1000000)
                    {
                        tv.tv_sec += tv.tv_usec / 1000000;
                        tv.tv_usec %= 1000000;
                    }
                    
                    hires *result = hires::Create(0);
                    result->properties["total_usec"] = new integer(tv.tv_usec);
                    result->properties["total_sec"] = new integer(tv.tv_sec);
                    return result;
                }
                
                object *hires::day_of_month()
                {
                    time_t t = KITE_GET_INTEGER_PROPERTY(this, total_sec);
                    struct tm *thetime;
                    
                    thetime = gmtime(&t);
                    return new integer(thetime->tm_mday);
                }
                
                object *hires::format(string *format)
                {
                    struct tm *thistime;
                    time_t curtime;
                    char buf[2048];
                    
                    curtime = KITE_GET_INTEGER_PROPERTY(this, total_sec);
                    thistime = gmtime(&curtime);
                    strftime(buf, 2048, format->string_val.c_str(), thistime);
                    return new string(buf);
                }
                
                object *hires::hour()
                {
                    time_t t = KITE_GET_INTEGER_PROPERTY(this, total_sec);
                    struct tm *thetime;
                    
                    thetime = gmtime(&t);
                    return new integer(thetime->tm_hour);
                }
                
                object *hires::min()
                {
                    time_t t = KITE_GET_INTEGER_PROPERTY(this, total_sec);
                    struct tm *thetime;
                    
                    thetime = gmtime(&t);
                    return new integer(thetime->tm_min);
                }
                
                object *hires::month()
                {
                    time_t t = KITE_GET_INTEGER_PROPERTY(this, total_sec);
                    struct tm *thetime;
                    
                    thetime = gmtime(&t);
                    return new integer(thetime->tm_mon);
                }
                
                object *hires::nowGmt()
                {
                    hires *result = hires::Create(0);
                    struct timeval tv;
                    struct timezone tz;
                    
                    gettimeofday(&tv, &tz);
                    result->properties["total_sec"] = new integer(tv.tv_sec + (tz.tz_minuteswest * 60));
                    result->properties["total_usec"] = new integer(tv.tv_usec);
                    return result;
                }
                
                object *hires::nowLocal()
                {
                    hires *result = hires::Create(0);
                    struct timeval tv;
                    
                    gettimeofday(&tv, NULL);
                    result->properties["total_sec"] = new integer(tv.tv_sec);
                    result->properties["total_usec"] = new integer(tv.tv_usec);
                    return result;
                }
                
                object *hires::parse(string *val, string *format)
                {
                    struct tm current;
                    hires *result = hires::Create(0);
                    
                    strptime(val->string_val.c_str(), format->string_val.c_str(), &current);
                    time_t t = mktime(&current);
                    result->properties["total_sec"] = new integer(t);
                    return result;
                }
                
                object *hires::sec()
                {
                    int total_sec = KITE_GET_INTEGER_PROPERTY(this, total_sec);
                    return new integer(total_sec % 60);
                }
                
                const char *hires::str()
                {
                    time_t t = KITE_GET_INTEGER_PROPERTY(this, total_sec);
                    return ctime(&t);
                }
                
                object *hires::timestamp()
                {
                    return properties["total_sec"];
                }
                
                object *hires::usec()
                {
                    return properties["total_usec"];
                }
                
                object *hires::year()
                {
                    time_t t = KITE_GET_INTEGER_PROPERTY(this, total_sec);
                    struct tm *thetime;
                    
                    thetime = gmtime(&t);
                    return new integer(thetime->tm_year);
                }
            }
        }
    }
}

REGISTER_KITE_CLASS(kite::stdlib::System::date::date, kite::stdlib::System::date::hires);