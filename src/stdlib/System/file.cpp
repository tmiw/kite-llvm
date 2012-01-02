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

#include <iostream>
#include <cstdio>
#include <sys/stat.h>
#include "file.h"
#include "boolean.h"

namespace kite
{
    namespace stdlib
    {
        namespace System
        {
            void file::check_file_open()
            {
                if (file_handle == NULL)
                {
                    exceptions::FileError *exc = new exceptions::FileError("File not open.");
                    exc->throw_exception();
                }
            }
            
            object *file::close()
            {
                check_file_open();
                fclose(file_handle);
                file_handle = NULL;
            }
            
            object *file::exists(string *path)
            {
                struct stat buf;
                int result = stat(path->string_val.c_str(), &buf);
                return new boolean(result);
            }
            
            object *file::read(integer *bytes)
            {
                char buf[bytes->val + 1];
                int ret;
                
                check_file_open();
                ret = fread(buf, 1, bytes->val, file_handle);
                if (ret == 0) return NULL;
                return new string(buf);
            }
            
            object *file::readline()
            {
                char buf[1024], *rval;
                std::string full_string;
                
                check_file_open();
                do
                {
                    rval = fgets(buf, 1024, file_handle);
                    if (rval == NULL) break;
                    
                    full_string += buf;
                } while (rval && full_string.size() > 1023);
                
                return new string(full_string.c_str());
            }
            
            object *file::seek(integer *pos)
            {
                check_file_open();
                fseek(file_handle, pos->val, SEEK_SET);
                return this;
            }
            
            object *file::tell()
            {
                check_file_open();
                return new integer(ftell(file_handle));
            }
            
            object *file::write(string *str)
            {
                check_file_open();
                return new integer(
                    fwrite(str->string_val.c_str(), 1, str->string_val.size(), file_handle)
                );
            }
        }
    }
}

REGISTER_KITE_CLASS(kite::stdlib::System::System, kite::stdlib::System::file);