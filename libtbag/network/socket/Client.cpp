/**
 * @file   Client.cpp
 * @brief  Client class implementation.
 * @author zer0
 * @date   2016-10-14
 */

#include <libtbag/network/socket/Client.hpp>
#include <cstring>
#include <vector>
#include <uv.h>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace network {
namespace socket  {

/**
 * Pointer to implementation of @c socket.
 *
 * @author zer0
 * @date   2016-11-03
 *
 * @remarks
 *  Use the libuv.
 */
struct Client::SocketPimpl : public libtbag::loop::event::UvHandler
{
private:
    Client & _parent;

private:
    sockaddr_in  _sockaddr;
    uv_tcp_t     _tcp;
    uv_connect_t _connect;
    uv_write_t   _write;

private:
    std::vector<char> _buffer;
    uv_buf_t          _buffer_info;

public:
    SocketPimpl(Client & parent) : _parent(parent)
    {
        ::memset(&_sockaddr, 0x00, sizeof(_sockaddr));
        ::memset(&_tcp     , 0x00, sizeof(_tcp));
        ::memset(&_connect , 0x00, sizeof(_connect));
        ::memset(&_write   , 0x00, sizeof(_write));

        add(&_tcp);
        add(&_connect);
        add(&_write);
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

    bool connect()
    {
        if (uv_tcp_connect(&_connect, &_tcp, (sockaddr const *)&_sockaddr, TBAG_UV_EVENT_CALLBACK_CONNECT) != 0) {
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
    virtual void onConnect(/*uv_connect_t*/void * req, int status) override
    {
        if (status < 0) {
            fprintf(stderr, "Connection error %s\n", uv_strerror(status));
            // error!
            return;
        }

        reallocWithStreamBuffer(5);
        _buffer[0] = 'H';
        _buffer[1] = 'E';
        _buffer[2] = 'L';
        _buffer[3] = 'L';
        _buffer[4] = 'O';
        _buffer_info.len = 5;
        _buffer_info.base = &_buffer[0];

        uv_write(&_write
               , (uv_stream_t*)&_tcp
               , &_buffer_info
               , 1
               , TBAG_UV_EVENT_CALLBACK_WRITE);
    }

    virtual void onWrite(/*uv_write_t*/void * req, int status) override
    {
        fprintf(stdout, "Write!!\n");

        uv_write_t * uv_write = static_cast<uv_write_t*>(req);
        uv_close((uv_handle_t*)uv_write->handle, nullptr);
    }
};

// ----------------------
// Client implementation.
// ----------------------

Client::Client() : _socket(new SocketPimpl(*this))
{
    // EMPTY.
}

Client::~Client()
{
    // EMPTY.
}

bool Client::run(std::string const & ip, int port)
{
    if (_socket->init(ip, port) && _socket->connect()) {
        return _loop.runDefault();
    }
    return false;
}

} // namespace socket
} // namespace network

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

