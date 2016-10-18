/**
 * @file   Server.cpp
 * @brief  Server class implementation.
 * @author zer0
 * @date   2016-10-14
 */

#include <libtbag/network/socket/Server.hpp>
#include <cstdlib>
#include <uv.h>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace network {
namespace socket  {

/**
 * Pointer to implementation of @c uv_tcp_t.
 *
 * @author zer0
 * @date   2016-10-18
 */
struct Server::TcpPimpl
{
private:
    Server & _parent;
    uv_tcp_t _server;

public:
    TcpPimpl(Server & parent) : _parent(parent)
    {
        ::memset(&_server, 0x00, sizeof(_server));
        _parent.add(&_server);
    }

    ~TcpPimpl()
    {
        _parent.remove(&_server);
    }

    inline uv_tcp_t * getNative() noexcept
    { return &_server; }
    inline uv_tcp_t const * getNative() const noexcept
    { return &_server; }
};

/**
 * Pointer to implementation of @c sockaddr_in.
 *
 * @author zer0
 * @date   2016-10-18
 */
struct Server::SockAddrPimpl
{
private:
    Server & _parent;
    struct sockaddr_in _sockaddr;

public:
    SockAddrPimpl(Server & parent) : _parent(parent)
    {
        ::memset(&_sockaddr, 0x00, sizeof(_sockaddr));
    }

    ~SockAddrPimpl()
    {
        // EMPTY.
    }

    inline sockaddr_in * getNative() noexcept
    { return &_sockaddr; }
    inline sockaddr_in const * getNative() const noexcept
    { return &_sockaddr; }
};

// ----------------------
// Server implementation.
// ----------------------

Server::Server() : _server(new TcpPimpl(*this)), _sockaddr(new SockAddrPimpl(*this))
{
    // EMPTY.
}

Server::~Server()
{
    // EMPTY.
}

bool Server::runIpv4(std::string const & ip, int port)
{
    unsigned int const BIND_FLAGS = 0;

    ::uv_tcp_init(static_cast<uv_loop_t*>(_loop.getNative()), _server->getNative());
    ::uv_ip4_addr(ip.c_str(), port, _sockaddr->getNative());
    ::uv_tcp_bind(_server->getNative(), (const struct sockaddr *)_sockaddr->getNative(), BIND_FLAGS);

    int code = ::uv_listen((uv_stream_t*)_server->getNative()
                         , LISTEN_QUEUE_LIMIT
                         , (uv_connection_cb)&libtbag::loop::event::uv::onConnection);
    if (code != 0) {
        return false;
    }

    return _loop.runDefault();
}

void Server::onAlloc(void * handle, size_t suggested_size, void * buf)
{
    uv_handle_t * uv_handle = static_cast<uv_handle_t*>(handle);
    uv_buf_t * uv_buf = static_cast<uv_buf_t*>(buf);

    uv_buf->base = (char*) malloc (suggested_size);
    uv_buf->len  = suggested_size;
}

void Server::onRead(void * stream, ssize_t nread, void const * buf)
{
    uv_stream_t * uv_client = static_cast<uv_stream_t*>(stream);
    uv_buf_t const * uv_buf = static_cast<uv_buf_t const*>(buf);

    if (nread < 0) {
        if (nread != UV_EOF) {
            fprintf(stderr, "Read error %s\n", uv_err_name(nread));
        }
        uv_close((uv_handle_t*) uv_client, NULL);
    } else if (nread > 0) {
        uv_write_t *req = (uv_write_t *) malloc(sizeof(uv_write_t));
        uv_buf_t wrbuf = uv_buf_init(uv_buf->base, nread);
        uv_write(req, uv_client, &wrbuf, 1, (uv_write_cb)&libtbag::loop::event::uv::onWrite);
        //printf("%.*s\n", wrbuf.len, wrbuf.base);
    }

    if (uv_buf->base) {
        free(uv_buf->base);
    }
}

void Server::onWrite(void * req, int status)
{
    uv_write_t * uv_write = static_cast<uv_write_t*>(req);

    if (status) {
        //fprintf(stderr, "Write error %s\n", uv_strerror(status));
    }
    free(uv_write);
}

void Server::onConnection(void * server, int status)
{
    uv_stream_t * uv_server = static_cast<uv_stream_t*>(server);

    if (status < 0) {
        fprintf(stderr, "New connection error %s\n", uv_strerror(status));
        // error!
        return;
    }

    uv_tcp_t * client = (uv_tcp_t*) ::malloc(sizeof(uv_tcp_t));
    uv_tcp_init(static_cast<uv_loop_t*>(_loop.getNative()), client);

    if (uv_accept((uv_stream_t*)_server->getNative(), (uv_stream_t*) client) == 0) {
        uv_read_start((uv_stream_t*) client
                    , (uv_alloc_cb)&libtbag::loop::event::uv::onAlloc
                    , (uv_read_cb)&libtbag::loop::event::uv::onRead);
    } else {
        uv_close((uv_handle_t*) client, NULL);
    }
}

} // namespace socket
} // namespace network

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

