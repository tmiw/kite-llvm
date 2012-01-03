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
 
#ifndef KITE_STDLIB__SYSTEM__MATH__RANGE_H
#define KITE_STDLIB__SYSTEM__MATH__RANGE_H

#include "../math.h"
#include "../integer.h"

namespace kite
{
    namespace stdlib
    {
        namespace System
        {
            namespace math
            {
                BEGIN_KITE_BASE_CLASS(range)
                private:
                    static object *s_initialize_2(range *r, integer *f, integer *t)
                    {
                        return s_initialize_3(r, f, t, new integer(1));
                    }
                    
                    static object *s_initialize_3(range *r, integer *f, integer *t, integer *s)
                    {
                        r->properties["fromnumber"] = f;
                        r->properties["tonumber"] = t;
                        r->properties["step"] = s;
                        return r;
                    }
                    
                    static object *s_list(range *r) { return r->list(); }

                public:
                    object *list();
                    
                    BEGIN_KITE_CLASS_INITIALIZER
                        KITE_CONSTRUCTOR_DEFINE(2, &range::s_initialize_2);
                        KITE_CONSTRUCTOR_DEFINE(3, &range::s_initialize_3);
                        
                        KITE_METHOD_DEFINE(list, 0, &range::s_list);
                    END_KITE_CLASS_INITIALIZER
                };
            }
        }
    }
}

#endif