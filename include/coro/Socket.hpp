/*
 * Copyright (c) 2013 Matt Fichman
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to
 * deal in the Software without restriction, including without limitation the
 * rights to use, copy, modify, merge, publish, distribute, sublicense, and/or
 * sell copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, APEXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
 * IN THE SOFTWARE.
 */

#pragma once

#include "coro/Common.hpp"

namespace coro {

class SocketAddr {
// Represents an IPv4 or IPv6 address, with the host being an IP address
// encoded the normal formats or a DNS name. 
public:
    SocketAddr(std::string const& host, short port) : host(host), port(port) {}
    struct sockaddr_in sockaddr() const;
    struct in_addr inaddr() const;
    std::string const host;
    short const port;
};

class Socket {
// Emulates Berkeley sockets using yields to block the coroutine when I/O
// operations cannot complete right away.  Actually, this class is more like
// Python's adaptation of sockets.  Failures throw exceptions rather than
// returning error codes, and the send/recv/read/write functions do not return
// until all data has been sent, or there is an error.
public:
    Socket(int type=SOCK_STREAM, int protocol=IPPROTO_TCP);
    ~Socket();
    Ptr<Socket> accept();
    void bind(SocketAddr const& addr);
    void close();
    void connect(SocketAddr const& addr);
    void listen(int backlog);
    void read(char* buf, size_t buflen, int flags=0);
    void write(char const* buf, size_t buflen, int flags=0);
    void shutdown(int how);
    int fileno();

private:
    Socket(int sd) : sd_(sd) {}

    int sd_;
};

}