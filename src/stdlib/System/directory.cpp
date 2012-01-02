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
#include <cstring>
#include <unistd.h>
#include <sys/time.h>
#include <assert.h>
#include "directory.h"
#include "list.h"

namespace kite
{
    namespace stdlib
    {
        namespace System
        {
            void directory::check_directory_open()
            {
                if (dir_handle == NULL)
                {
                    exceptions::FileError *exc = exceptions::FileError::Create(
                        1,
                        new string("Directory not open.")
                    );
                    exc->throw_exception();
                }
            }
            
            object *directory::map(method *m)
            {
                check_directory_open();
                
                list *l = list::Create(0);
                int pos = telldir(dir_handle);
                rewinddir(dir_handle);
                
                struct dirent *ent = readdir(dir_handle);
                while (ent != 0)
                {
                    string *dirname = new string(ent->d_name);
                    l->list_contents.push_back(
                        m->invoke(dirname)
                    );
                    ent = readdir(dir_handle);
                }
                seekdir(dir_handle, pos);
                return l;
            }
            
            object *directory::close()
            {
                check_directory_open();
                closedir(dir_handle);
                dir_handle = NULL;
            }
            
            object *directory::read()
            {
                struct dirent *ent;
                
                check_directory_open();
                ent = readdir(dir_handle);
                if (ent == NULL) return NULL;
                
                return new string(ent->d_name);
            }
            
            object *directory::seek(integer *pos)
            {
                check_directory_open();
                seekdir(dir_handle, pos->val);
                return this;
            }
            
            object *directory::tell()
            {
                check_directory_open();
                return new integer(telldir(dir_handle));
            }
        }
    }
}

REGISTER_KITE_CLASS(kite::stdlib::System::System, kite::stdlib::System::directory);