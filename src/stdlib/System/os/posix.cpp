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

#include <errno.h>
#include <signal.h>
#include <unistd.h>
#include <sstream>
#include "posix.h"
#include "../exceptions/OsError.h"
#include "../boolean.h"
#include "../integer.h"

static int wrap_raise(int sig)
{
    return raise(sig);
}

static int wrap_pclose(FILE *fd)
{
    return pclose(fd);
}

static FILE *wrap_popen(const char *path, const char *mode)
{
    return popen(path, mode);
}

static void kite_handle_signal(int sig)
{
    if (kite::stdlib::System::os::posix::signal_handlers[sig] != NULL)
    {
        kite::stdlib::System::os::posix::signal_handlers[sig]->invoke(new kite::stdlib::System::integer(sig), NULL);
    }
}

namespace kite
{
    namespace stdlib
    {
        namespace System
        {
            namespace os
            {
                std::map<int, method*> posix::signal_handlers;
                
                object* posix::pclose(file *pipe)
                {
                    return new integer(wrap_pclose(pipe->stdio_handle()));
                }
                
                object* posix::popen(string *name, string *mode)
                {
                    FILE *fp = wrap_popen(name->string_val.c_str(), mode->string_val.c_str());
                    if (fp == NULL)
                    {
                        std::stringstream ss;
                        
                        ss << "Could not open connection to executable (errno " << errno << ")";
                        exceptions::exception *exc = exceptions::OsError::Create(
                            1,
                            new string(ss.str().c_str()));
                        exc->throw_exception();
                    }
                    
                    return file::create_from_pointer(fp);
                }
                
                object* posix::raise(integer *sig)
                {
                    return new integer(wrap_raise(sig->val));
                }
                
                object* posix::setSignalHandler(integer *sig, method *f)
                {
                    signal_handlers[sig->val] = f;
                    
                    if (f == NULL)
                    {
                        signal(sig->val, SIG_DFL);
                    }
                    else
                    {
                        signal(sig->val, kite_handle_signal);
                    }
                    
                    return new boolean(true);
                }
            }
        }
    }
}
REGISTER_KITE_CLASS(kite::stdlib::System::os::os, kite::stdlib::System::os::posix)