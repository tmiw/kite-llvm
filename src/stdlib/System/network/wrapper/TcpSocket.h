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
 
#ifndef KITE_STDLIB__SYSTEM__NETWORK__WRAPPER__TCPSOCKET_H
#define KITE_STDLIB__SYSTEM__NETWORK__WRAPPER__TCPSOCKET_H

#include <stdlib/api.h>
#include <sys/socket.h>
#include "../socket.h"

namespace kite
{
    namespace stdlib
    {
        namespace System
        {
            namespace network
            {
                namespace wrapper
                {
                    BEGIN_KITE_CHILD_CLASS(TcpSocket, socket)
                    private:
                        int socket_val;
                
                        static object *s_initialize(socket *sock, string *host, integer *port)
                        {
                            socket::s_initialize(sock, new integer(AF_INET), new integer(SOCK_STREAM), new integer(0));
                            socket::s_bind(sock, host, port);
                            return sock;
                        }

                    public:
                        BEGIN_KITE_CLASS_INITIALIZER
                            KITE_CONSTRUCTOR_DEFINE(2, &TcpSocket::s_initialize);
                        END_KITE_CLASS_INITIALIZER
                    END_KITE_CLASS
                }
            }
        }
    }
}

#endif