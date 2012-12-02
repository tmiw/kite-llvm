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
 
#ifndef KITE_STDLIB__SYSTEM__DATE_H
#define KITE_STDLIB__SYSTEM__DATE_H

#include <deque>
#include "stdlib/System.h"
#include "integer.h"
#include "boolean.h"

namespace kite
{
    namespace stdlib
    {
        namespace System
        {
            namespace date
            {
                BEGIN_KITE_BASE_CLASS(date)
                private:
                    static object *s_initialize(date *d)
                    {
                        d->properties["day_of_month"] = new integer(1);
                        d->properties["day_of_week"] = new integer(0);
                        d->properties["day_of_year"] = new integer(0);
                        d->properties["dst"] = new boolean(false);
                        d->properties["hour"] = new integer(0);
                        d->properties["min"] = new integer(0);
                        d->properties["month"] = new integer(1);
                        d->properties["sec"] = new integer(0);
                        d->properties["year"] = new integer(1900);
                    }
                
                    static object *s_initialize_with_params
                        (date *d, integer *sec, integer *min, integer *hour, 
                         integer *day_of_month, integer *month, integer *year, 
                         integer *day_of_week, integer *day_of_year, boolean *dst)
                    {
                        d->properties["day_of_month"] = day_of_month;
                        d->properties["day_of_week"] = day_of_week;
                        d->properties["day_of_year"] = day_of_year;
                        d->properties["dst"] = dst;
                        d->properties["hour"] = hour;
                        d->properties["min"] = min;
                        d->properties["month"] = month;
                        d->properties["sec"] = sec;
                        d->properties["year"] = year;
                    }
                
                    static object *s_format(date *d, string *format) { return d->format(format); }
                    static object *s_now_gmt(date *d) { return d->now_gmt(); }
                    static object *s_now_local(date *d) { return d->now_local(); }
                    static object *s_parse(date *, string *value, string *format) { return date::parse(value, format); }
                    static const char *s_str(date *d) { return d->str(); }
                    static object *s_timestamp(date *d) { return d->timestamp(); }
                   
                public:
                    object *format(string *format);
                    object *now_gmt();
                    object *now_local();
                    static object *parse(string *value, string *format);
                    const char *str();
                    object *timestamp();
                             
                    BEGIN_KITE_CLASS_INITIALIZER
                        KITE_CONSTRUCTOR_DEFINE(0, &date::s_initialize);
                        KITE_CONSTRUCTOR_DEFINE(9, &date::s_initialize_with_params);
                    
                        KITE_METHOD_DEFINE(format, 1, &date::s_format);
                        KITE_METHOD_DEFINE(now_gmt, 0, &date::s_now_gmt);
                        KITE_METHOD_DEFINE(now_local, 0, &date::s_now_local);
                        KITE_METHOD_DEFINE(parse, 2, &date::s_parse);
                        KITE_METHOD_DEFINE(str, 0, &date::s_str);
                        KITE_METHOD_DEFINE(timestamp, 0, &date::s_timestamp);
                    END_KITE_CLASS_INITIALIZER
                };
            }
        }
    }
}

REGISTER_KITE_CLASS(kite::stdlib::System::System, kite::stdlib::System::date::date);

#endif