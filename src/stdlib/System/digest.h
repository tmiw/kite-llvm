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
 
#ifndef KITE_STDLIB__SYSTEM_DIGEST_H
#define KITE_STDLIB__SYSTEM_DIGEST_H

#include "stdlib/api.h"
#include "../System.h"
#include "string.h"

namespace kite
{
    namespace stdlib
    {
        namespace System
        {
            BEGIN_KITE_BASE_CLASS(digest)
            private:
                static object* s_md5(digest *, string *message) { return digest::md5(message); }
                static object* s_md5file(digest *, string *file) { return digest::md5file(file); }
                
            public:
                static object* md5(string *message);
                static object* md5file(string *file);
                
                BEGIN_KITE_CLASS_INITIALIZER
                    KITE_METHOD_DEFINE(md5, 1, &digest::s_md5);
                    KITE_METHOD_DEFINE(md5file, 1, &digest::s_md5file);
                END_KITE_CLASS_INITIALIZER
            END_KITE_CLASS
        }
    }
}

#endif