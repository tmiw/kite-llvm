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
 
#ifndef KITE_STDLIB__SYSTEM_OS_H
#define KITE_STDLIB__SYSTEM_OS_H

#include "stdlib/api.h"
#include "../System.h"
#include "integer.h"
#include "string.h"

namespace kite
{
    namespace stdlib
    {
        namespace System
        {
            namespace os
            {
                BEGIN_KITE_BASE_CLASS(os)
                private:
                    static object* s_chdir(os *, string *dirname) { return os::chdir(dirname); }
                    static object* s_chmod(os *, string *name, integer *mode) { return os::chmod(name, mode); }
                    static object* s_envGet(os *, string *name) { return os::envGet(name); }
                    static object* s_envSet(os *, string *name, string *value) { return os::envSet(name, value); }
                    static object* s_fork(os *) { return os::fork(); }
                    static object* s_getcwd(os *) { return os::getcwd(); }
                    static object* s_mkdir(os *, string *name, integer *mode) { return os::mkdir(name, mode); }
                    static object* s_rmdir(os *, string *name) { return os::rmdir(name); }
                    static object* s_system(os *, string *cmdline) { return os::system(cmdline); }
                    
                public:
                    static object* chdir(string *dirname);
                    static object* chmod(string *name, integer *mode);
                    static object* envGet(string *name);
                    static object* envSet(string *name, string *value);
                    static object* fork();
                    static object* getcwd();
                    static object* mkdir(string *name, integer *mode);
                    static object* rmdir(string *name);
                    static object* system(string *cmdline);
                    
                    BEGIN_KITE_CLASS_INITIALIZER
                        KITE_METHOD_DEFINE(chdir, 1, &os::s_chdir);
                        KITE_METHOD_DEFINE(chmod, 2, &os::s_chmod);
                        KITE_METHOD_DEFINE(envGet, 1, &os::s_envGet);
                        KITE_METHOD_DEFINE(envSet, 2, &os::s_envSet);
                        KITE_METHOD_DEFINE(fork, 0, &os::s_fork);
                        KITE_METHOD_DEFINE(getcwd, 0, &os::s_getcwd);
                        KITE_METHOD_DEFINE(mkdir, 2, &os::s_mkdir);
                        KITE_METHOD_DEFINE(rmdir, 1, &os::s_rmdir);
                        KITE_METHOD_DEFINE(system, 1, &os::s_system);

                        // TODO: Windows support
#ifdef __apple__
                        class_obj.properties["dylib_extension"] = new string(".dylib");
#else
                        class_obj.properties["dylib_extension"] = new string(".so");
#endif
                    END_KITE_CLASS_INITIALIZER
                END_KITE_CLASS
            }
        }
    }
}

REGISTER_KITE_CLASS(kite::stdlib::System::System, kite::stdlib::System::os::os)

#endif