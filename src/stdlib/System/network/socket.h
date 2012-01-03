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
 
#ifndef KITE_STDLIB__SYSTEM__NETWORK__SOCKET_H
#define KITE_STDLIB__SYSTEM__NETWORK__SOCKET_H

#include <sys/socket.h>
#include "../exceptions/SocketError.h"
#include "../integer.h"

namespace kite
{
    namespace stdlib
    {
        namespace System
        {
            namespace network
            {
                // Wrapper methods since we're using the same names 
                // as the system socket API calls.
                static int open_sock(int domain, int type, int protocol)
                {
                    return socket(domain, type, protocol);
                }
                
                static int close_sock(int sock)
                {
                    return close(sock);
                }
                
                static int bind_sock(int sock, struct sockaddr *addr, size_t sz)
                {
                    return bind(sock, addr, sz);
                }
                
                static int connect_sock(int sock, struct sockaddr *addr, size_t sz)
                {
                    return connect(sock, addr, sz);
                }
                
                static int listen_sock(int sock, int queue)
                {
                    return listen(sock, queue);
                }
                
                static int accept_sock(int sock, struct sockaddr *addr, socklen_t *sz)
                {
                    return accept(sock, addr, sz);
                }
                
                static int read_sock(int fd, void *buf, size_t count)
                {
                    return read(fd, buf, count);
                }
                
                static int write_sock(int fd, void *buf, size_t count)
                {
                    return write(fd, buf, count);
                }
                
                BEGIN_KITE_BASE_CLASS(socket)
                protected:
                    static object *s_initialize(socket *sock, integer *domain, integer *type, integer *protocol)
                    {
                        sock->socket_val = open_sock(domain->val, type->val, protocol->val);
                        if (sock->socket_val < 0)
                        {
                            exceptions::SocketError *exc = exceptions::SocketError::Create(0);
                            exc->throw_exception();
                        }
                        return sock;
                    }
                    
                    static object *s_destruct(socket *sock)
                    {
                        if (sock->socket_val >= 0) close_sock(sock->socket_val);
                        return NULL;
                    }
                    
                    static object *s_bind(socket *sock, string *host, integer *port) { return sock->bind(host, port); }
                private:
                    int socket_val;
                    static object *s_accept(socket *sock) { return sock->accept(); }
                    static object *s_close(socket *sock) { return sock->close(); }
                    static object *s_connect(socket *sock, string *host, integer *port) { return sock->connect(host, port); }
                    static object *s_eof(socket *sock) { return sock->eof(); }
                    static object *s_eof_timeout(socket *sock, integer *timeout) { return sock->eof(timeout); }
                    static object *s_listen(socket *sock, integer *queue) { return sock->listen(queue); }
                    static object *s_read(socket *sock, integer *size) { return sock->read(size); }
                    static object *s_read_timeout(socket *sock, integer *size, integer *timeout) { return sock->read(size, timeout); }
                    static object *s_readline(socket *sock) { return sock->readline(); }
                    static object *s_write(socket *sock, string *val) { return sock->write(val); }
                    
                    int dnsLookup(const char *host, int port, int mode);
                    
                public:
                    object *accept();
                    object *bind(string *host, integer *port);
                    object *close();
                    object *connect(string *host, integer *port);
                    object *eof();
                    object *eof(integer *timeout);
                    object *listen(integer *queue);
                    object *read(integer *size);
                    object *read(integer *size, integer *timeout);
                    object *readline();
                    object *write(string *val);
                    
                    BEGIN_KITE_CLASS_INITIALIZER
                        KITE_CONSTRUCTOR_DEFINE(3, &socket::s_initialize);
                        KITE_DESTRUCTOR_DEFINE(&socket::s_destruct);
                    
                        KITE_METHOD_DEFINE(accept, 0, &socket::s_accept);
                        KITE_METHOD_DEFINE(bind, 2, &socket::s_bind);
                        KITE_METHOD_DEFINE(close, 0, &socket::s_close);
                        KITE_METHOD_DEFINE(connect, 2, &socket::s_connect);
                        KITE_METHOD_DEFINE(eof, 0, &socket::s_eof);
                        KITE_METHOD_DEFINE(eof, 1, &socket::s_eof_timeout);
                        KITE_METHOD_DEFINE(listen, 1, &socket::s_listen);
                        KITE_METHOD_DEFINE(read, 1, &socket::s_read);
                        KITE_METHOD_DEFINE(read, 2, &socket::s_read_timeout);
                        KITE_METHOD_DEFINE(readline, 0, &socket::s_readline);
                        KITE_METHOD_DEFINE(write, 1, &socket::s_write);
                        
                        // Define constants
                        class_obj.properties["AF_INET"] = new integer(AF_INET);
                        class_obj.properties["AF_INET6"] = new integer(AF_INET6);
                        class_obj.properties["AF_UNIX"] = new integer(AF_UNIX);
                        class_obj.properties["AF_UNSPEC"] = new integer(AF_UNSPEC);
                        class_obj.properties["SOCK_DGRAM"] = new integer(SOCK_DGRAM);
                        class_obj.properties["SOCK_SEQPACKET"] = new integer(SOCK_SEQPACKET);
                        class_obj.properties["SOCK_STREAM"] = new integer(SOCK_STREAM);
                    END_KITE_CLASS_INITIALIZER
                END_KITE_CLASS
            }
        }
    }
}

#endif