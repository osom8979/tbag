/**
 * @file   Client.cpp
 * @brief  Client class implementation.
 * @author zer0
 * @date   2016-10-14
 */

#include <libtbag/network/socket/Client.hpp>
#include <uv.h>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace network {
namespace socket  {

Client::Client() : _tcp(this)
{
    ::memset(&_sockaddr, 0x00, sizeof(_sockaddr));
}

Client::~Client()
{
    // EMPTY.
}

bool Client::connect(std::string const & ip, int port)
{
    ::uv_tcp_init(static_cast<uv_loop_t*>(_loop.getNative()), TBAG_TCP_NATIVE_CASTING(_tcp));

    uv_connect_t * connect = (uv_connect_t*)malloc(sizeof(uv_connect_t));

    ::uv_ip4_addr(ip.c_str(), port, &_sockaddr);

    int r = ::uv_tcp_connect(connect, TBAG_TCP_NATIVE_CASTING(_tcp), (sockaddr const *)&_sockaddr
            , (uv_connect_cb)&libtbag::loop::event::uv::onConnect);

    if (r) {
        fprintf(stderr, "Listen error %s\n", uv_strerror(r));
        return 1;
    }

    return _loop.runDefault();
}

void Client::onWrite(void * req, int status)
{
    uv_write_t * uv_write = static_cast<uv_write_t*>(req);
    uv_close((uv_handle_t*) uv_write->handle, nullptr);
}

uv_write_t write_request;

void Client::onConnect(void * req, int status)
{
    if (status < 0) {
        fprintf(stderr, "Connection error %s\n", uv_strerror(status));
        // error!
        return;
    }

    static char ECHO_STRING[] = "HELLO";
    static uv_buf_t buf;
    buf.len = 4;
    buf.base = ECHO_STRING;

    ::uv_write(&write_request
            , (uv_stream_t *) _tcp.getNative()
            , &buf
            , 1
            , (uv_write_cb)&libtbag::loop::event::uv::onWrite);
}

} // namespace socket
} // namespace network

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

