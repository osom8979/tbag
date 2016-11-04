/**
 * @file   Tcp.cpp
 * @brief  Tcp class implementation.
 * @author zer0
 * @date   2016-11-04
 */

#include <libtbag/network/socket/Tcp.hpp>
#include <libtbag/loop/event/UvHandler.hpp>
#include <libtbag/loop/UvEventLoop.hpp>

#include <cstring>
#include <vector>
#include <algorithm>

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
 * @date   2016-11-03
 *
 * @remarks
 *  Use the libuv.
 */
struct Tcp::StreamPimpl : public libtbag::loop::event::UvHandler
{
private:
    Tcp & _parent;

private:
    uv_tcp_t     _tcp;     // onRead, onAlloc (for read), onConnection (for server), onClose
    uv_write_t   _write;   // onWrite, onClose
    uv_connect_t _connect; // onConnect

public:
    std::vector<char> _read_buffer;
    uv_buf_t          _read_buffer_info;

public:
    StreamPimpl(Tcp & parent) : _parent(parent)
    {
        ::memset(&_tcp    , 0x00, sizeof(_tcp    ));
        ::memset(&_write  , 0x00, sizeof(_write  ));
        ::memset(&_connect, 0x00, sizeof(_connect));

        add(&_tcp);
        add(&_connect);
        add(&_write);
    }

    ~StreamPimpl()
    {
        // EMPTY.
    }

public:
    inline uv_tcp_t * getTcp()
    { return &_tcp; }
    inline uv_tcp_t const * getTcp() const
    { return &_tcp; }

public:
    bool init(uv_loop_t * loop)
    {
        return uv_tcp_init(loop, &_tcp) == 0;
    }

// CLIENT ONLY.
public:
    bool connect(sockaddr const * address)
    {
        return uv_tcp_connect(&_connect, &_tcp, address, TBAG_UV_EVENT_CALLBACK_CONNECT) == 0;
    }

    bool connect(sockaddr_in const * address)
    { return connect((sockaddr const *)address); }
    bool connect(sockaddr_in6 const * address)
    { return connect((sockaddr const *)address); }

// SERVER ONLY.
public:
    bool bind(sockaddr const * address)
    {
        unsigned int const BIND_FLAGS = 0;
        return uv_tcp_bind(&_tcp, address, BIND_FLAGS) == 0;
    }

    bool bind(sockaddr_in const * address)
    { return bind((sockaddr const *)address); }
    bool bind(sockaddr_in6 const * address)
    { return bind((sockaddr const *)address); }

    bool listen()
    {
        int const LISTEN_QUEUE_LIMIT = 128;
        return uv_listen((uv_stream_t*)&_tcp, LISTEN_QUEUE_LIMIT, TBAG_UV_EVENT_CALLBACK_CONNECTION) == 0;
    }

    /** bind -> listen -> onConnection -> accept */
    bool accept(uv_tcp_t * client)
    {
        return uv_accept((uv_stream_t*)&_tcp, (uv_stream_t*)client) == 0;
    }

public:
    void close(uv_handle_t * handle)
    {
        uv_close(handle, TBAG_UV_EVENT_CALLBACK_CLOSE);
    }

    void closeTcp()
    {
        close((uv_handle_t*)&_tcp); // with read handle.
    }

    void closeConnect()
    {
        close((uv_handle_t*)&_connect);
    }

    void closeWrite()
    {
        close((uv_handle_t*)&_write);
    }

public:
    bool read()
    {
        return uv_read_start((uv_stream_t*)&_tcp, TBAG_UV_EVENT_CALLBACK_ALLOC, TBAG_UV_EVENT_CALLBACK_READ) == 0;
    }

    bool write(char const * buffer, std::size_t length)
    {
        uv_buf_t buf;
        buf.base = const_cast<char*>(buffer);
        buf.len  = length;

        return uv_write(&_write, (uv_stream_t*)&_tcp, &buf, 1, TBAG_UV_EVENT_CALLBACK_WRITE) == 0;
    }

public:
    virtual void onConnect(/*uv_connect_t*/void * req, int status) override
    {
        if (_parent._callback != nullptr) {
            _parent._callback->onConnect(status);
        }
    }

    virtual void onConnection(/*uv_stream_t*/void * server, int status) override
    {
        if (_parent._callback != nullptr) {
            _parent._callback->onConnection(status);
        }
    }

    virtual void onAlloc(/*uv_handle_t*/void * handle, size_t suggested_size, /*uv_buf_t*/void * buf) override
    {
        // Realloc with read buffer.
        if (_read_buffer.size() < suggested_size) {
            _read_buffer.resize(suggested_size);
        }

        uv_buf_t * uv_buf = static_cast<uv_buf_t*>(buf);
        uv_buf->base = &this->_read_buffer[0];
        uv_buf->len  =  this->_read_buffer.size();
    }

    virtual void onRead(/*uv_stream_t*/void * stream, ssize_t nread, /*uv_buf_t*/void const * buf) override
    {
        Tcp::ReadErrorCode code;
        if (nread == UV_EOF) {
            code = Tcp::ReadErrorCode::END_OF_FILE;
        } else if (nread < 0){
            code = Tcp::ReadErrorCode::UNKNOWN_ERROR;
        } else {
            code = Tcp::ReadErrorCode::SUCCESS;
        }

        if (_parent._callback != nullptr) {
            uv_buf_t const * uv_buf = static_cast<uv_buf_t const *>(buf);
            _parent._callback->onRead(code, uv_buf->base, static_cast<std::size_t>(nread));
        }
    }

    virtual void onWrite(/*uv_write_t*/void * req, int status) override
    {
        Tcp::WriteErrorCode code;
        if (status == 0) {
            code = Tcp::WriteErrorCode::SUCCESS;
        } else {
            code = Tcp::WriteErrorCode::UNKNOWN_ERROR;
        }

        if (_parent._callback != nullptr) {
            _parent._callback->onWrite(code);
        }
    }

    virtual void onClose(/*uv_handle_t*/void * handle) override
    {
        if (_parent._callback != nullptr) {
            if (handle == &_tcp) {
                _parent._callback->onCloseTcp();
            } else if (handle == &_connect) {
                _parent._callback->onCloseConnect();
            } else if (handle == &_write) {
                _parent._callback->onCloseWrite();
            }
        }
    }
};

// -------------------
// Tcp implementation.
// -------------------

Tcp::Tcp(Callback * callback) : _stream(new StreamPimpl(*this)), _callback(callback)
{
    // EMPTY.
}

Tcp::Tcp() : Tcp(nullptr)
{
    // EMPTY.
}

Tcp::~Tcp()
{
    // EMPTY.
}

void * Tcp::getTcp()
{
    return _stream->getTcp();
}

void const * Tcp::getTcp() const
{
    return _stream->getTcp();
}

bool Tcp::init(libtbag::loop::UvEventLoop & loop)
{
    return _stream->init(static_cast<uv_loop_t*>(loop.getNative()));
}

bool Tcp::connectIpv4(std::string const & ip, int port)
{
    sockaddr_in sockaddr;
    if (uv_ip4_addr(ip.c_str(), port, &sockaddr) == 0) {
        return _stream->connect(&sockaddr);
    }
    return false;
}

bool Tcp::connectIpv6(std::string const & ip, int port)
{
    sockaddr_in6 sockaddr;
    if (uv_ip6_addr(ip.c_str(), port, &sockaddr) == 0) {
        return _stream->connect(&sockaddr);
    }
    return false;
}

bool Tcp::bindIpv4(std::string const & ip, int port)
{
    sockaddr_in sockaddr;
    if (uv_ip4_addr(ip.c_str(), port, &sockaddr) == 0) {
        return _stream->bind(&sockaddr);
    }
    return false;
}

bool Tcp::bindIpv6(std::string const & ip, int port)
{
    sockaddr_in6 sockaddr;
    if (uv_ip6_addr(ip.c_str(), port, &sockaddr) == 0) {
        return _stream->bind(&sockaddr);
    }
    return false;
}

bool Tcp::listen()
{
    return _stream->listen();
}

bool Tcp::accept(Tcp & client)
{
    return _stream->accept((uv_tcp_t*)client.getTcp());
}

bool Tcp::read()
{
    return _stream->read();
}

bool Tcp::write(char const * buffer, std::size_t length)
{
    return _stream->write(buffer, length);
}

void Tcp::closeTcp()
{
    return _stream->closeTcp();
}

void Tcp::closeConnect()
{
    return _stream->closeConnect();
}

void Tcp::closeWrite()
{
    return _stream->closeWrite();
}

bool Tcp::isIpv4(std::string const & ip)
{
    return std::count(ip.begin(), ip.end(), '.') == 3;
}

} // namespace socket
} // namespace network

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

