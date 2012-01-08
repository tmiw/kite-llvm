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

#include <stdio.h>
#include "../System.h"
#include "exceptions/FileError.h"
#include "integer.h"

namespace kite
{
    namespace stdlib
    {
        namespace System
        {
            BEGIN_KITE_BASE_CLASS(file)
            private:
                file(FILE *fhandle)
                    : dynamic_object(&class_object(true))
                {
                    file_handle = fhandle;
                }
                
                static object *s_initialize(file *f, string *name, string *mode)
                {
                    f->file_handle = fopen(name->string_val.c_str(), mode->string_val.c_str());
                    if (f->file_handle == NULL)
                    {
                        exceptions::FileError *exc = exceptions::FileError::Create(
                            1,
                            new string("Problem opening file.")
                        );
                        exc->throw_exception();
                    }
                    return f;
                }
            
                static object *s_close(file *f) { return f->close(); }
                static object *s_exists(file *f, string *path) { return file::exists(path); }
                static object *s_read(file *f, integer *bytes) { return f->read(bytes); }
                static object *s_readline(file *f) { return f->readline(); }
                static object *s_seek(file *f, integer *pos) { return f->seek(pos); }
                static object *s_tell(file *f) { return f->tell(); }
                static object *s_write(file *f, string *str) { return f->write(str); }
                void check_file_open();
                
                FILE *file_handle;
            public:
                static file *create_from_pointer(FILE *fptr) { return new file(fptr); }
                FILE *stdio_handle() { return file_handle; }
                
                object *close();
                static object *exists(string *path);
                object *read(integer *bytes);
                object *readline();
                object *seek(integer *pos);
                object *tell();
                object *write(string *str);
                         
                BEGIN_KITE_CLASS_INITIALIZER
                    KITE_CONSTRUCTOR_DEFINE(2, &file::s_initialize);
                
                    KITE_METHOD_DEFINE(close, 0, &file::s_close);
                    KITE_METHOD_DEFINE(exists, 1, &file::s_exists);
                    KITE_METHOD_DEFINE(read, 1, &file::s_read);
                    KITE_METHOD_DEFINE(readline, 0, &file::s_readline);
                    KITE_METHOD_DEFINE(seek, 1, &file::s_seek);
                    KITE_METHOD_DEFINE(tell, 0, &file::s_tell);
                    KITE_METHOD_DEFINE(write, 1, &file::s_write);
                    
                    class_obj.properties["stderr"] = new file(stderr);
                    class_obj.properties["stdout"] = new file(stdout);
                    class_obj.properties["stdin"] = new file(stdin);
                END_KITE_CLASS_INITIALIZER
            END_KITE_CLASS
        }
    }
}

#endif