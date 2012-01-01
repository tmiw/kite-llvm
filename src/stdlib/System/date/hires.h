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
 
#ifndef KITE_STDLIB__SYSTEM__DATE__HIRES_H
#define KITE_STDLIB__SYSTEM__DATE__HIRES_H

#include <deque>
#include "stdlib/System.h"
#include "../integer.h"
#include "../boolean.h"
#include "../date.h"

namespace kite
{
    namespace stdlib
    {
        namespace System
        {
            namespace date
            {
                BEGIN_KITE_BASE_CLASS(hires)
                private:
                    static object *s_initialize(hires *d)
                    {
                        d->properties["total_sec"] = new integer(0);
                        d->properties["total_usec"] = new integer(0);
                    }
                
                    static object *s_subtract(hires *d1, hires *d2) { return d1->subtract(d2); }
                    static object *s_add(hires *d1, hires *d2) { return d1->add(d2); }
                    static object *s_day_of_month(hires *d) { return d->day_of_month(); }
                    static object *s_format(hires *d, string *format) { return d->format(format); }
                    static object *s_hour(hires *d) { return d->hour(); }
                    static object *s_min(hires *d) { return d->min(); }
                    static object *s_month(hires *d) { return d->month(); }
                    static object *s_nowGmt(hires *d) { return d->nowGmt(); }
                    static object *s_nowLocal(hires *d) { return d->nowLocal(); }
                    static object *s_parse(hires *, string *val, string *format) { return hires::parse(val, format); }
                    static object *s_sec(hires *d) { return d->sec(); }
                    static const char *s_str(hires *d) { return d->str(); }
                    static object *s_timestamp(hires *d) { return d->timestamp(); }
                    static object *s_usec(hires *d) { return d->usec(); }
                    static object *s_year(hires *d) { return d->sec(); }
                   
                public:
                    object *subtract(hires *d2);
                    object *add(hires *d2);
                    object *day_of_month();
                    object *format(string *format);
                    object *hour();
                    object *min();
                    object *month();
                    object *nowGmt();
                    object *nowLocal();
                    static object *parse(string *val, string *format);
                    object *sec();
                    const char *str();
                    object *timestamp();
                    object *usec();
                    object *year();
                             
                    BEGIN_KITE_CLASS_INITIALIZER
                        KITE_CONSTRUCTOR_DEFINE(0, &hires::s_initialize);
                    
                        KITE_OPERATOR_DEFINE(semantics::ADD, &hires::s_add);
                        KITE_OPERATOR_DEFINE(semantics::SUBTRACT, &hires::s_subtract);
                    
                        KITE_METHOD_DEFINE(day_of_month, 0, &hires::s_day_of_month);
                        KITE_METHOD_DEFINE(format, 1, &hires::s_format);
                        KITE_METHOD_DEFINE(hour, 0, &hires::s_hour);
                        KITE_METHOD_DEFINE(min, 0, &hires::s_min);
                        KITE_METHOD_DEFINE(month, 0, &hires::s_month);
                        KITE_METHOD_DEFINE(nowGmt, 0, &hires::s_nowGmt);
                        KITE_METHOD_DEFINE(nowLocal, 0, &hires::s_nowLocal);
                        KITE_METHOD_DEFINE(parse, 2, &hires::s_parse);
                        KITE_METHOD_DEFINE(sec, 0, &hires::s_sec);
                        KITE_METHOD_DEFINE(str, 0, &hires::s_str);
                        KITE_METHOD_DEFINE(timestamp, 0, &hires::s_timestamp);
                        KITE_METHOD_DEFINE(usec, 0, &hires::s_usec);
                        KITE_METHOD_DEFINE(year, 0, &hires::s_year);
                    END_KITE_CLASS_INITIALIZER
                END_KITE_CLASS
            }
        }
    }
}

#endif