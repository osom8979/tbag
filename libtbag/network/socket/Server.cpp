/**
 * @file   Server.cpp
 * @brief  Server class implementation.
 * @author zer0
 * @date   2016-10-14
 */

#include <libtbag/network/socket/Server.hpp>
#include <cstdlib>
#include <cstring>
#include <vector>
#include <uv.h>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace network {
namespace socket  {

// ----------------------
// Server implementation.
// ----------------------

/**
 * Pointer to implementation of @c socket.
 *
 * @author zer0
 * @date   2016-11-03
 *
 * @remarks
 *  Use the libuv.
 */
struct Server::SocketPimpl : public libtbag::loop::event::UvHandler
{
private:
    Server & _parent;

private:
    sockaddr_in _sockaddr;
    uv_tcp_t    _tcp;

private:
    std::vector<char> _buffer;

public:
    SocketPimpl(Server & parent) : _parent(parent)
    {
        ::memset(&_sockaddr, 0x00, sizeof(_sockaddr));
        ::memset(&_tcp, 0x00, sizeof(_tcp));

        add(&_tcp);
    }

    ~SocketPimpl()
    {
        // EMPTY.
    }

public:
    bool init(std::string const & ip, int port)
    {
        uv_loop_t * loop = static_cast<uv_loop_t*>(_parent._loop.getNative());
        if (uv_tcp_init(loop, &_tcp) != 0) {
            return false;
        }
        if (uv_ip4_addr(ip.c_str(), port, &_sockaddr) != 0) {
            return false;
        }
        return true;
    }

    bool bindAndListen()
    {
        unsigned int const BIND_FLAGS = 0;
        int const LISTEN_QUEUE_LIMIT = 128;

        if (uv_tcp_bind(&_tcp, (sockaddr const *)&_sockaddr, BIND_FLAGS) != 0) {
            return false;
        }
        if (uv_listen((uv_stream_t*)&_tcp, LISTEN_QUEUE_LIMIT, TBAG_UV_EVENT_CALLBACK_CONNECTION) != 0) {
            return false;
        }

        return true;
    }

public:
    void reallocWithStreamBuffer(std::size_t size)
    {
        if (_buffer.size() < size) {
            _buffer.resize(size);
        }
    }

public:
    virtual void onAlloc(/*uv_handle_t*/void * handle, size_t suggested_size, /*uv_buf_t*/void * buf) override
    {
        reallocWithStreamBuffer(suggested_size);

        uv_buf_t * uv_buf = static_cast<uv_buf_t*>(buf);
        uv_buf->base = &this->_buffer[0];
        uv_buf->len  =  this->_buffer.size();
    }

    virtual void onRead(/*uv_stream_t*/void * stream, ssize_t nread, /*uv_buf_t*/void const * buf) override
    {
        fprintf(stdout, "onRead!!\n");

        uv_stream_t * uv_client = static_cast<uv_stream_t*>(stream);
        uv_buf_t const * uv_buf = static_cast<uv_buf_t const*>(buf);

        if (nread == UV_EOF) {
            // END_OF_FILE
            uv_close((uv_handle_t*)uv_client, NULL);
        } else if (nread < 0){
            // UNKNOWN_ERROR
            fprintf(stderr, "Read error %s\n", uv_err_name(nread));
            uv_close((uv_handle_t*)uv_client, NULL);
        } else {
            // SUCCESS
            uv_write_t *req = (uv_write_t *) malloc(sizeof(uv_write_t));
            uv_buf_t wrbuf = uv_buf_init(uv_buf->base, nread);
            uv_write(req, uv_client, &wrbuf, 1, TBAG_UV_EVENT_CALLBACK_WRITE);
            //printf("%.*s\n", wrbuf.len, wrbuf.base);
        }
    }

    virtual void onWrite(/*uv_write_t*/void * req, int status) override
    {
        uv_write_t * uv_write = static_cast<uv_write_t*>(req);
        if (status) {
            //fprintf(stderr, "Write error %s\n", uv_strerror(status));
        }
        free(uv_write); // ?? WHY ??
    }

    virtual void onConnection(/*uv_stream_t*/void * server, int status) override
    {
        uv_stream_t * uv_server = static_cast<uv_stream_t*>(server);

        if (status < 0) {
            fprintf(stderr, "New connection error %s\n", uv_strerror(status));
            // error!
            return;
        } else {
            fprintf(stdout, "New connection!!\n");
        }

        uv_loop_t * loop = static_cast<uv_loop_t*>(_parent._loop.getNative());

        uv_tcp_t * client = (uv_tcp_t*) ::malloc(sizeof(uv_tcp_t));
        uv_tcp_init(loop, client);

        if (uv_accept((uv_stream_t*)&_tcp, (uv_stream_t*) client) == 0) {
            fprintf(stdout, "Accept!!\n");
            uv_read_start((uv_stream_t*) client
                        , TBAG_UV_EVENT_CALLBACK_ALLOC
                        , TBAG_UV_EVENT_CALLBACK_READ);
        } else {
            fprintf(stderr, "Accept error!!\n");
            uv_close((uv_handle_t*) client, NULL);
        }
    }
};

// ----------------------
// Server implementation.
// ----------------------

Server::Server() : _socket(new SocketPimpl(*this))
{
}

Server::~Server()
{
}

bool Server::run(std::string const & ip, int port)
{
    if (_socket->init(ip, port) && _socket->bindAndListen()) {
        return _loop.runDefault();
    }
    return false;
}

} // namespace socket
} // namespace network

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

