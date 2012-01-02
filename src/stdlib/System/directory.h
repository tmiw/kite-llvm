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
 
#ifndef KITE_STDLIB__SYSTEM__DICTIONARY_H
#define KITE_STDLIB__SYSTEM__DICTIONARY_H

#include <dirent.h>
#include "../System.h"
#include "exceptions/FileError.h"
#include "integer.h"

namespace kite
{
    namespace stdlib
    {
        namespace System
        {
            BEGIN_KITE_BASE_CLASS(directory)
            private:
                static object *s_initialize(directory *d, string *name)
                {
                    d->dir_handle = opendir(name->string_val.c_str());
                    if (d->dir_handle == NULL)
                    {
                        exceptions::FileError *exc = new exceptions::FileError("Problem opening directory.");
                        exc->throw_exception();
                    }
                    return d;
                }
            
                static object *s_map(directory *d, method *m) { return d->map(m); }
                static object *s_close(directory *d) { return d->close(); }
                static object *s_read(directory *d) { return d->read(); }
                static object *s_seek(directory *d, integer *pos) { return d->seek(pos); }
                static object *s_tell(directory *d) { return d->tell(); }
               
                void check_directory_open();
                
                DIR *dir_handle;
            public:
                object *map(method *m);
                object *close();
                object *read();
                object *seek(integer *pos);
                object *tell();
                         
                BEGIN_KITE_CLASS_INITIALIZER
                    KITE_CONSTRUCTOR_DEFINE(1, &directory::s_initialize);
                
                    KITE_OPERATOR_DEFINE(semantics::MAP, &directory::s_map);
                
                    KITE_METHOD_DEFINE(map, 1, &directory::s_map);
                    KITE_METHOD_DEFINE(close, 0, &directory::s_close);
                    KITE_METHOD_DEFINE(read, 0, &directory::s_read);
                    KITE_METHOD_DEFINE(seek, 1, &directory::s_seek);
                    KITE_METHOD_DEFINE(tell, 0, &directory::s_tell);
                END_KITE_CLASS_INITIALIZER
            END_KITE_CLASS
        }
    }
}

#endif