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
 
#ifndef KITE_STDLIB__SYSTEM__OS__POSIX_H
#define KITE_STDLIB__SYSTEM__OS__POSIX_H

#include <map>
#include "stdlib/api.h"
#include "../method.h"
#include "../file.h"
#include "../os.h"

namespace kite
{
    namespace stdlib
    {
        namespace System
        {
            namespace os
            {
                BEGIN_KITE_BASE_CLASS(posix)
                private:
                    static object* s_pclose(posix *, file *pipe) { return posix::pclose(pipe); }
                    static object* s_popen(posix *, string *name, string *mode) { return posix::popen(name, mode); }
                    static object* s_raise(posix *, integer *sig) { return posix::raise(sig); }
                    static object* s_setSignalHandler(os *, integer *sig, method *f) { return posix::setSignalHandler(sig, f); }
                                        
                public:
                    static std::map<int, method*> signal_handlers;
                    
                    static object* pclose(file *pipe);
                    static object* popen(string *name, string *mode);
                    static object* raise(integer *sig);
                    static object* setSignalHandler(integer *sig, method *f);
                    
                    BEGIN_KITE_CLASS_INITIALIZER
                        KITE_METHOD_DEFINE(pclose, 1, &posix::s_pclose);
                        KITE_METHOD_DEFINE(popen, 2, &posix::s_popen);
                        KITE_METHOD_DEFINE(raise, 1, &posix::s_raise);
                        KITE_METHOD_DEFINE(setSignalHandler, 2, &posix::s_setSignalHandler);
        
                        class_obj.properties["NSIG"] = new integer(NSIG);
                        class_obj.properties["SIGABRT"] = new integer(SIGABRT);
                        class_obj.properties["SIGALRM"] = new integer(SIGALRM);
                        class_obj.properties["SIGBUS"] = new integer(SIGBUS);
                        class_obj.properties["SIGCHLD"] = new integer(SIGCHLD);
                        class_obj.properties["SIGCONT"] = new integer(SIGCONT);
                        class_obj.properties["SIGFPE"] = new integer(SIGFPE);
                        class_obj.properties["SIGHUP"] = new integer(SIGHUP);
                        class_obj.properties["SIGILL"] = new integer(SIGILL);
                        class_obj.properties["SIGINT"] = new integer(SIGINT);
                        class_obj.properties["SIGIO"] = new integer(SIGIO);
                        class_obj.properties["SIGKILL"] = new integer(SIGKILL);
                        class_obj.properties["SIGPIPE"] = new integer(SIGPIPE);
                        class_obj.properties["SIGPROF"] = new integer(SIGPROF);
                        class_obj.properties["SIGQUIT"] = new integer(SIGQUIT);
                        class_obj.properties["SIGSEGV"] = new integer(SIGSEGV);
                        class_obj.properties["SIGSTOP"] = new integer(SIGSTOP);
                        class_obj.properties["SIGSYS"] = new integer(SIGSYS);
                        class_obj.properties["SIGTERM"] = new integer(SIGTERM);
                        class_obj.properties["SIGTRAP"] = new integer(SIGTRAP);
                        class_obj.properties["SIGTSTP"] = new integer(SIGTSTP);
                        class_obj.properties["SIGTTIN"] = new integer(SIGTTIN);
                        class_obj.properties["SIGTTOU"] = new integer(SIGTTOU);
                        class_obj.properties["SIGURG"] = new integer(SIGURG);
                        class_obj.properties["SIGUSR1"] = new integer(SIGUSR1);
                        class_obj.properties["SIGUSR2"] = new integer(SIGUSR2);
                        class_obj.properties["SIGVTALRM"] = new integer(SIGVTALRM);
                        class_obj.properties["SIGWINCH"] = new integer(SIGWINCH);
                        class_obj.properties["SIGXCPU"] = new integer(SIGXCPU);
                        class_obj.properties["SIGXFSZ"] = new integer(SIGXFSZ);
                    END_KITE_CLASS_INITIALIZER
                END_KITE_CLASS
            }
        }
    }
}

#endif