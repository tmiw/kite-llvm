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

#include <sstream>
#include <errno.h>
#include <sys/stat.h>
#include <unistd.h>
#include "os.h"
#include "exceptions/FileError.h"
#include "exceptions/OsError.h"

static int wrap_chdir(const char *name)
{
    return chdir(name);
}

static int wrap_chmod(const char *name, int mode)
{
    return chmod(name, mode);
}

static int wrap_fork()
{
    return fork();
}

static char *wrap_getcwd(char *buf, size_t sz)
{
    return getcwd(buf, sz);
}

static int wrap_mkdir(const char *name, mode_t mode)
{
    return mkdir(name, mode);
}

static int wrap_rmdir(const char *name)
{
    return rmdir(name);
}

static int wrap_system(const char *cmdline)
{
    return system(cmdline);
}

namespace kite
{
    namespace stdlib
    {
        namespace System
        {
            namespace os
            {
                object* os::chdir(string *dirname)
                {
                    if (wrap_chdir(dirname->string_val.c_str()) != 0)
                    {
                        std::ostringstream ss;
                        ss << "Could not change current directory to " << dirname->string_val << " (errno " << errno << ")";
                        
                        exceptions::FileError *exc = exceptions::FileError::Create(1, new string(ss.str().c_str()));
                        exc->throw_exception();
                    }
                    
                    return NULL;
                }
                
                object* os::chmod(string *name, integer *mode)
                {
                    if (wrap_chmod(name->string_val.c_str(), mode->val) != 0)
                    {
                        std::ostringstream ss;
                        ss << "Could not change mode of " << name->string_val << " to " << mode->val 
                           << " (errno " << errno << ")";
                        
                        exceptions::FileError *exc = exceptions::FileError::Create(1, new string(ss.str().c_str()));
                        exc->throw_exception();
                    }
                    
                    return NULL;
                }
                
                object* os::envGet(string *name)
                {
                    char *retval;
                    
                    retval = getenv(name->string_val.c_str());
                    if (retval == NULL)
                    {
                        retval = "";
                    }
                    
                    return new string(retval);
                }
                
                object* os::envSet(string *name, string *value)
                {
                    if (setenv(name->string_val.c_str(), value->string_val.c_str(), 1) != 0)
                    {
                        std::ostringstream ss;
                        ss << "Could not set " << name->string_val << " to " << value->string_val 
                           << " (errno " << errno << ")";
                        
                        exceptions::exception *exc = exceptions::OsError::Create(1, new string(ss.str().c_str()));
                        exc->throw_exception();
                    }
                    
                    return NULL;
                    
                }
                
                object* os::fork()
                {
                    return new integer(wrap_fork());
                }
                
                object* os::getcwd()
                {
                    char buf[2048];
                    if (wrap_getcwd(buf, 2048) == NULL)
                    {
                        std::ostringstream ss;
                        ss << "Could not get current path "
                           << " (errno " << errno << ")";
                        
                        exceptions::FileError *exc = exceptions::FileError::Create(1, new string(ss.str().c_str()));
                        exc->throw_exception();
                    }
                    return new string(buf);
                }
                
                object* os::mkdir(string *name, integer *mode)
                {
                    if (wrap_mkdir(name->string_val.c_str(), mode->val) != 0)
                    {
                        std::ostringstream ss;
                        ss << "Could not create directory " << name->string_val  
                           << " (errno " << errno << ")";
                        
                        exceptions::FileError *exc = exceptions::FileError::Create(1, new string(ss.str().c_str()));
                        exc->throw_exception();
                    }
                    
                    return NULL;
                }
                
                object* os::rmdir(string *name)
                {
                    if (wrap_rmdir(name->string_val.c_str()) != 0)
                    {
                        std::ostringstream ss;
                        ss << "Could not remove directory " << name->string_val  
                           << " (errno " << errno << ")";
                        
                        exceptions::FileError *exc = exceptions::FileError::Create(1, new string(ss.str().c_str()));
                        exc->throw_exception();
                    }
                    
                    return NULL;
                }
                
                object* os::system(string *cmdline)
                {
                    return new integer(
                        wrap_system(cmdline->string_val.c_str()));
                }
            }
        }
    }
}