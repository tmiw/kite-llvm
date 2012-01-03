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

#include <sys/socket.h>
#include <sys/un.h>
#include <sys/types.h>
#include <netdb.h>
#include <unistd.h>
#include <errno.h>
#include "../network.h"
#include "../exceptions/DnsError.h"
#include "../exceptions/SocketError.h"
#include "socket.h"
#include "../boolean.h"
#include "../integer.h"
#include "../string.h"
#include "../list.h"

namespace kite
{
    namespace stdlib
    {
        namespace System
        {
            namespace network
            {
                /*****************************************************************************
                 * Perform DNS lookup
                 ****************************************************************************/
                int 
                socket::dnsLookup(const char *host, int port, int mode)
                {
                    struct sockaddr *sa;
                    struct addrinfo hints;
                    struct addrinfo *res;
                    int ret = 0;
                    int err = 0;
                    int sock = socket_val;
                    size_t salen;
                    char buf[128];
                    int socktype;
                    socklen_t len = sizeof(socklen_t);
#ifndef WIN32
                    struct sockaddr_un local;
#endif /* WIN32 */

                    /* Unix domain sockets */
#ifndef WIN32
                    if (host[0] == '/')
                    {
                        properties["unix_socket"] = new boolean(true);

                        local.sun_family = AF_UNIX;
                        strcpy(local.sun_path, host);

                        if (mode)
                        {
                            unlink(local.sun_path);
                            ret = bind_sock(sock, (struct sockaddr*)&local, sizeof(local));
                        }
                        else
                        {
                            ret = connect_sock(sock, (struct sockaddr*)&local, sizeof(local));
                        }
                    }
                    else
                    {
#endif
                        properties["unix_socket"] = new boolean(false);

                        /* AF_INET[6] */
                        memset(&hints, 0, sizeof(hints));
                        sprintf(buf, "%d", port);

                        getsockopt(sock, SOL_SOCKET, SO_TYPE, (char*)&socktype, &len);

                        /* set-up hints structure */
                        hints.ai_family = AF_UNSPEC;
                        hints.ai_socktype = socktype;
                        if (mode) hints.ai_flags = AI_PASSIVE;
                        err = getaddrinfo(host, buf, &hints, &res);
                        if (err)
                        {
                            if (res) freeaddrinfo(res);
                            
                            exceptions::DnsError *exc = exceptions::DnsError::Create(
                                1,
                                new string((char*)gai_strerror(err))
                            );
                            exc->throw_exception();
                        }
                        else 
                        {
                            struct addrinfo *top = res;
                            while (res) {
                                sa = res->ai_addr;
                                salen = res->ai_addrlen;

                                if (mode)
                                {
                                    /* bind() */
                                    ret = bind_sock(sock, sa, salen);
                                    if (ret < 0) goto end_socket;
                                }
                                else
                                {
                                    /* connect() */
                                    ret = connect_sock(sock, sa, salen);
                                    if (ret == 0) goto end_socket;
                                }

                                res = res->ai_next;
                            }
end_socket:
                            freeaddrinfo(top);
                        }
#ifndef WIN32
                    }
#endif
                    return ret;
                }
                
                object *socket::accept()
                {
                    struct sockaddr_storage addr;
                    socklen_t len = sizeof(addr);
                    int ret;
                    char hostname[1024];
                    list *retval;
                    socket *sock;
                    
                    if (socket_val < 0)
                    {
                        exceptions::SocketError *exc = exceptions::SocketError::Create(
                            1,
                            new string("Socket already closed.")
                        );
                        exc->throw_exception();
                    }
                    
                    ret = accept_sock(socket_val, (struct sockaddr*)&addr, &len);
                    if (ret < 0)
                    {
                        exceptions::SocketError *exc = exceptions::SocketError::Create(
                            1,
                            new string("Error during socket accept.")
                        );
                        exc->throw_exception();
                    }
                    
                    if (!KITE_PROPERTY_EXISTS(this, unix_socket) || !KITE_GET_BOOLEAN_PROPERTY(this, unix_socket))
                    {
                        getnameinfo((struct sockaddr*)&addr, sizeof(addr), hostname, 1024, NULL, 0, NI_NUMERICHOST);
                    }
                    else
                    {
                        hostname[0] = 0;
                    }
                    
                    retval = list::Create(0);
                    sock = new socket();
                    sock->socket_val = ret;
                    
                    retval->list_contents.push_back(retval);
                    retval->list_contents.push_back(
                        new string(hostname)
                    );
                    
                    if (!KITE_PROPERTY_EXISTS(this, unix_socket) || !KITE_GET_BOOLEAN_PROPERTY(this, unix_socket))
                    {
                        int port = 0;
                        
                        switch(addr.ss_family)
                        {
                            case AF_INET:
                            {
                                struct sockaddr_in *sin = (struct sockaddr_in*)&addr;
                                port = sin->sin_port;
                                break;
                            }
                            case AF_INET6:
                            {
                                struct sockaddr_in6 *sin = (struct sockaddr_in6*)&addr;
                                port = sin->sin6_port;
                                break;
                            }
                            default:
                            {
                                assert(0);
                            }
                        }
                        
                        retval->list_contents.push_back(new integer(port));
                    }
                    else
                    {
                        retval->list_contents.push_back(new integer(0));
                    }
                    
                    return retval;
                }
                
                object *socket::bind(string *host, integer *port)
                {
                    if (socket_val < 0)
                    {
                        exceptions::SocketError *exc = exceptions::SocketError::Create(
                            1,
                            new string("Socket already closed.")
                        );
                        exc->throw_exception();
                    }
                    
                    int ret = dnsLookup(host->string_val.c_str(), port->val, 1);
                    if (ret < 0)
                    {
                        exceptions::SocketError *exc = exceptions::SocketError::Create(
                            1,
                            new string("Could not look up host.")
                        );
                        exc->throw_exception();
                    }
                    
                    return new boolean(true);
                }
                
                object *socket::close()
                {
                    if (socket_val < 0)
                    {
                        exceptions::SocketError *exc = exceptions::SocketError::Create(
                            1,
                            new string("Socket already closed.")
                        );
                        exc->throw_exception();
                    }
                    
                    int ret = close_sock(socket_val);
                    if (ret < 0)
                    {
                        exceptions::SocketError *exc = exceptions::SocketError::Create(
                            1,
                            new string("Error while closing socket.")
                        );
                        exc->throw_exception();
                    }
                    socket_val = -1;
                    
                    return new boolean(true);
                }
                
                object *socket::connect(string *host, integer *port)
                {
                    if (socket_val < 0)
                    {
                        exceptions::SocketError *exc = exceptions::SocketError::Create(
                            1,
                            new string("Socket already closed.")
                        );
                        exc->throw_exception();
                    }
                    
                    if (dnsLookup(host->string_val.c_str(), port->val, 0) < 0)
                    {
                        exceptions::SocketError *exc = exceptions::SocketError::Create(
                            1,
                            new string("Could not open connection.")
                        );
                        exc->throw_exception();
                    }
                    
                    return new boolean(true);
                }
                
                object *socket::eof()
                {
                    return eof(new integer(1000));
                }
                
                object *socket::eof(integer *timeout)
                {
                    struct timeval timeout_struct;
                    fd_set socklist;
                    int can_read;
                    
                    timeout_struct.tv_sec = timeout->val / 1000;
                    timeout_struct.tv_usec = (float)(timeout->val % 1000) / 1000.0f;
                    
                    if (socket_val < 0)
                    {
                        exceptions::SocketError *exc = exceptions::SocketError::Create(
                            1,
                            new string("Socket already closed.")
                        );
                        exc->throw_exception();
                    }
                    
                    FD_ZERO(&socklist);
                    FD_SET(socket_val, &socklist);
                    can_read = select(FD_SETSIZE, &socklist, NULL, NULL, &timeout_struct);
                    if (can_read <= 0)
                    {
                        return new boolean(true);
                    }
                    else
                    {
                        return new boolean(false);
                    }
                }
                
                object *socket::listen(integer *queue)
                {
                    if (socket_val < 0)
                    {
                        exceptions::SocketError *exc = exceptions::SocketError::Create(
                            1,
                            new string("Socket already closed.")
                        );
                        exc->throw_exception();
                    }
                    
                    int ret = listen_sock(socket_val, queue->val);
                    if (ret < 0)
                    {
                        exceptions::SocketError *exc = exceptions::SocketError::Create(
                            1,
                            new string("Error while listening on socket.")
                        );
                        exc->throw_exception();
                    }

                    return new boolean(true);
                }
                
                object *socket::read(integer *size)
                {
                    if (socket_val < 0)
                    {
                        exceptions::SocketError *exc = exceptions::SocketError::Create(
                            1,
                            new string("Socket already closed.")
                        );
                        exc->throw_exception();
                    }
                    
                    char buf[size->val + 1];
                    int ret = read_sock(socket_val, buf, size->val);
                    if (ret < 0 && errno != EAGAIN)
                    {
                        exceptions::SocketError *exc = exceptions::SocketError::Create(
                            1,
                            new string("Error encountered while reading data.")
                        );
                        exc->throw_exception();
                    }
                    else if (ret == 0)
                    {
                        return NULL;
                    }
                    else
                    {
                        buf[ret] = 0;
                    }
                    
                    return new string(buf);
                }
                
                object *socket::read(integer *size, integer *timeout)
                {
                    struct timeval timeout_struct;
                    fd_set socklist;
                    int ret, can_read;
                    char buf[size->val + 1];
                    
                    timeout_struct.tv_sec = timeout->val / 1000;
                    timeout_struct.tv_usec = (float)(timeout->val % 1000) / 1000.0f;
                    
                    if (socket_val < 0)
                    {
                        exceptions::SocketError *exc = exceptions::SocketError::Create(
                            1,
                            new string("Socket already closed.")
                        );
                        exc->throw_exception();
                    }
                    
                    FD_ZERO(&socklist);
                    FD_SET(socket_val, &socklist);
                    can_read = select(FD_SETSIZE, &socklist, NULL, NULL, &timeout_struct);
                    if (can_read <= 0)
                    {
                        return NULL;
                    }
                    
                    ret = read_sock(socket_val, buf, size->val);
                    if (ret < 0 && errno != EAGAIN)
                    {
                        exceptions::SocketError *exc = exceptions::SocketError::Create(
                            1,
                            new string("Error encountered while reading data.")
                        );
                        exc->throw_exception();
                    }
                    else if (ret == 0)
                    {
                        return NULL;
                    }
                    else
                    {
                        buf[ret] = 0;
                    }
                    
                    return new string(buf);
                }
                
                object *socket::readline()
                {
                    std::string buf;
                    int ret, idx = 0;
                    string *rval;

                    if (socket_val < 0)
                    {
                        exceptions::SocketError *exc = exceptions::SocketError::Create(
                            1,
                            new string("Socket already closed.")
                        );
                        exc->throw_exception();
                    }

                    do 
                    {
                        char c[2];
                        c[1] = 0;
                        ret = read_sock(socket_val, c, 1);
                        if (ret < 0 && errno != EAGAIN) 
                        {
                            exceptions::SocketError *exc = exceptions::SocketError::Create(
                                1,
                                new string("Problem reading data.")
                            );
                            exc->throw_exception();
                        } 
                        else if (ret == 0 || errno == EAGAIN) 
                        {
                            break;
                        }
                        buf += c;
                    } while(buf[buf.size() - 1] != '\n');

                    return new string(buf.c_str());
                }
                
                object *socket::write(string *val)
                {
                    if (socket_val < 0)
                    {
                        exceptions::SocketError *exc = exceptions::SocketError::Create(
                            1,
                            new string("Socket already closed.")
                        );
                        exc->throw_exception();
                    }
                    
                    int ret = write_sock(socket_val, (void*)val->string_val.c_str(), val->string_val.size());
                    if (ret < 0)
                    {
                        exceptions::SocketError *exc = exceptions::SocketError::Create(
                            1,
                            new string("Error encountered while writing to socket.")
                        );
                        exc->throw_exception();
                    }
                    
                    return new integer(ret);
                }
            }
        }
    }
}

REGISTER_KITE_CLASS(kite::stdlib::System::network::network, kite::stdlib::System::network::socket);