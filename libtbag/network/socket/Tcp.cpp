/**
 * @file   Tcp.cpp
 * @brief  Tcp class implementation.
 * @author zer0
 * @date   2016-11-04
 */

#include <libtbag/network/socket/Tcp.hpp>
#include <libtbag/loop/UvEventLoop.hpp>

#include <cassert>
#include <cstring>

#include <uv.h>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace network {
namespace socket  {

//struct Tcp::StreamPimpl : public libtbag::loop::event::UvHandler
//{
//private:
//    Tcp & _parent;
//
//private:
//    uv_tcp_t     _tcp;     // onRead, onAlloc (for read), onConnection (for server), onClose
//    uv_write_t   _write;   // onWrite, onClose
//    uv_connect_t _connect; // onConnect
//
//public:
//    std::vector<char> _read_buffer;
//
//public:
//    StreamPimpl(Tcp & parent) : _parent(parent)
//    {
//        ::memset(&_tcp    , 0x00, sizeof(_tcp    ));
//        ::memset(&_write  , 0x00, sizeof(_write  ));
//        ::memset(&_connect, 0x00, sizeof(_connect));
//
//        add(&_tcp);
//        add(&_connect);
//        add(&_write);
//    }
//
//    ~StreamPimpl()
//    {
//        // EMPTY.
//    }
//
//public:
//    inline uv_tcp_t * getTcp()
//    { return &_tcp; }
//    inline uv_tcp_t const * getTcp() const
//    { return &_tcp; }
//
//public:
//    bool init(uv_loop_t * loop)
//    {
//        return uv_tcp_init(loop, &_tcp) == 0;
//    }
//
//// CLIENT ONLY.
//public:
//    bool connect(sockaddr const * address)
//    {
//        return uv_tcp_connect(&_connect, &_tcp, address, TBAG_UV_EVENT_CALLBACK_CONNECT) == 0;
//    }
//
//    bool connect(sockaddr_in const * address)
//    { return connect((sockaddr const *)address); }
//    bool connect(sockaddr_in6 const * address)
//    { return connect((sockaddr const *)address); }
//
//// SERVER ONLY.
//public:
//    bool bind(sockaddr const * address)
//    {
//        unsigned int const BIND_FLAGS = 0;
//        return uv_tcp_bind(&_tcp, address, BIND_FLAGS) == 0;
//    }
//
//    bool bind(sockaddr_in const * address)
//    { return bind((sockaddr const *)address); }
//    bool bind(sockaddr_in6 const * address)
//    { return bind((sockaddr const *)address); }
//
//    bool listen()
//    {
//        int const LISTEN_QUEUE_LIMIT = 128;
//        return uv_listen((uv_stream_t*)&_tcp, LISTEN_QUEUE_LIMIT, TBAG_UV_EVENT_CALLBACK_CONNECTION) == 0;
//    }
//
//    /** bind -> listen -> onConnection -> accept */
//    bool accept(uv_tcp_t * client)
//    {
//        return uv_accept((uv_stream_t*)&_tcp, (uv_stream_t*)client) == 0;
//    }
//
//public:
//    void close()
//    {
//        uv_close((uv_handle_t*)&_tcp, TBAG_UV_EVENT_CALLBACK_CLOSE);
//    }
//
//public:
//    bool read()
//    {
//        return uv_read_start((uv_stream_t*)&_tcp, TBAG_UV_EVENT_CALLBACK_ALLOC, TBAG_UV_EVENT_CALLBACK_READ) == 0;
//    }
//
//    bool write(char const * buffer, std::size_t length)
//    {
//        uv_buf_t buf;
//        buf.base = const_cast<char*>(buffer);
//        buf.len  = length;
//
//        return uv_write(&_write, (uv_stream_t*)&_tcp, &buf, 1, TBAG_UV_EVENT_CALLBACK_WRITE) == 0;
//    }


//public:
//    virtual void onConnect(/*uv_connect_t*/void * req, int status) override
//    {
//        if (_parent._callback != nullptr) {
//            _parent._callback->onConnect(status);
//        }
//    }
//
//    virtual void onConnection(/*uv_stream_t*/void * server, int status) override
//    {
//        if (_parent._callback != nullptr) {
//            _parent._callback->onConnection(status);
//        }
//    }
//
//    virtual void onAlloc(/*uv_handle_t*/void * handle, size_t suggested_size, /*uv_buf_t*/void * buf) override
//    {
//        // Realloc with read buffer.
//        if (_read_buffer.size() < suggested_size) {
//            _read_buffer.resize(suggested_size);
//        }
//
//        uv_buf_t * uv_buf = static_cast<uv_buf_t*>(buf);
//        uv_buf->base = &this->_read_buffer[0];
//        uv_buf->len  =  this->_read_buffer.size();
//    }
//
//    virtual void onRead(/*uv_stream_t*/void * stream, ssize_t nread, /*uv_buf_t*/void const * buf) override
//    {
//        Tcp::ReadErrorCode code;
//        if (nread == UV_EOF) {
//            code = Tcp::ReadErrorCode::END_OF_FILE;
//        } else if (nread < 0){
//            code = Tcp::ReadErrorCode::UNKNOWN_ERROR;
//        } else {
//            code = Tcp::ReadErrorCode::SUCCESS;
//        }
//
//        if (_parent._callback != nullptr) {
//            uv_buf_t const * uv_buf = static_cast<uv_buf_t const *>(buf);
//            _parent._callback->onRead(code, uv_buf->base, static_cast<std::size_t>(nread));
//        }
//    }
//
//    virtual void onWrite(/*uv_write_t*/void * req, int status) override
//    {
//        Tcp::WriteErrorCode code;
//        if (status == 0) {
//            code = Tcp::WriteErrorCode::SUCCESS;
//        } else {
//            code = Tcp::WriteErrorCode::UNKNOWN_ERROR;
//        }
//
//        if (_parent._callback != nullptr) {
//            _parent._callback->onWrite(code);
//        }
//    }
//
//    virtual void onClose(/*uv_handle_t*/void * handle) override
//    {
//        if (_parent._callback != nullptr) {
//            _parent._callback->onClose();
//        }
//    }
//};

//bool Tcp::connectIpv4(std::string const & ip, int port)
//{
//    sockaddr_in sockaddr;
//    if (uv_ip4_addr(ip.c_str(), port, &sockaddr) == 0) {
//        return _stream->connect(&sockaddr);
//    }
//    return false;
//}
//
//bool Tcp::connectIpv6(std::string const & ip, int port)
//{
//    sockaddr_in6 sockaddr;
//    if (uv_ip6_addr(ip.c_str(), port, &sockaddr) == 0) {
//        return _stream->connect(&sockaddr);
//    }
//    return false;
//}
//
//bool Tcp::bindIpv4(std::string const & ip, int port)
//{
//    sockaddr_in sockaddr;
//    if (uv_ip4_addr(ip.c_str(), port, &sockaddr) == 0) {
//        return _stream->bind(&sockaddr);
//    }
//    return false;
//}
//
//bool Tcp::bindIpv6(std::string const & ip, int port)
//{
//    sockaddr_in6 sockaddr;
//    if (uv_ip6_addr(ip.c_str(), port, &sockaddr) == 0) {
//        return _stream->bind(&sockaddr);
//    }
//    return false;
//}
//
//bool Tcp::listen()
//{
//    return _stream->listen();
//}
//
//bool Tcp::accept(Tcp & client)
//{
//    return _stream->accept((uv_tcp_t*)client.getTcp());
//}
//
//std::string Tcp::getSocketName() const
//{
//    return _stream->getSocketName();
//}
//
//std::string Tcp::getPeerName() const
//{
//    return _stream->getPeerName();
//}
//
//bool Tcp::read()
//{
//    return _stream->read();
//}
//
//bool Tcp::write(char const * buffer, std::size_t length)
//{
//    return _stream->write(buffer, length);
//}
//
//void Tcp::close()
//{
//    return _stream->close();
//}
//
//bool Tcp::isIpv4(std::string const & ip)
//{
//    return std::count(ip.begin(), ip.end(), '.') == 3;
//}

// ------------------------
// TcpPimpl implementation.
// ------------------------

/**
 * Pointer to implementation of @c uv_tcp_t.
 *
 * @author zer0
 * @date   2016-11-03
 * @date   2016-11-07 (Refactoring this class)
 *
 * @remarks
 *  Use the libuv.
 */
struct Tcp::TcpPimpl
{
    uv_tcp_t tcp;

    TcpPimpl()
    {
        ::memset(&tcp, 0x00, sizeof(tcp));
    }

    ~TcpPimpl()
    {
        // EMPTY.
    }

    inline uv_tcp_t * get() TBAG_NOEXCEPT
    { return &tcp; }
    inline uv_tcp_t const * get() const TBAG_NOEXCEPT
    { return &tcp; }

    inline std::string getPeerName() const
    { return getPeerName(&tcp); }
    inline std::string getSocketName() const
    { return getSocketName(&tcp); }

public:
    static std::string getIpName(sockaddr_in * address)
    {
        char name[16] = {0,}; // e.g. 255.255.255.255
        if (::uv_ip4_name(address, name, sizeof(name)) == 0) {
            return std::string(name);
        }
        return std::string();
    }

    static std::string getIpName(sockaddr_in6 * address)
    {
        char name[40] = {0,}; // e.g. 2001:0db8:85a3:08d3:1319:8a2e:0370:7334
        if (::uv_ip6_name(address, name, sizeof(name)) == 0) {
            return std::string(name);
        }
        return std::string();
    }

    static std::string getPeerName(uv_tcp_t const * handle)
    {
        sockaddr addr = {0,};
        int length = sizeof(addr);

        if (::uv_tcp_getpeername(handle, &addr, &length) == 0) {
            if (addr.sa_family == AF_INET) {
                return getIpName((sockaddr_in*)&addr);
            } else if (addr.sa_family == AF_INET6) {
                return getIpName((sockaddr_in6*)&addr);
            }
        }
        return std::string();
    }

    static std::string getSocketName(uv_tcp_t const * handle)
    {
        sockaddr addr = {0,};
        int length = sizeof(addr);

        if (::uv_tcp_getsockname(handle, &addr, &length) == 0) {
            if (addr.sa_family == AF_INET) {
                return getIpName((sockaddr_in*)&addr);
            } else if (addr.sa_family == AF_INET6) {
                return getIpName((sockaddr_in6*)&addr);
            }
        }
        return std::string();
    }

};

// -------------------
// Tcp implementation.
// -------------------

Tcp::Tcp() : _tcp(new TcpPimpl())
{
    assert(_tcp != nullptr);
}

Tcp::~Tcp()
{
    // EMPTY.
}

bool Tcp::init(libtbag::loop::UvEventLoop & loop)
{
    return ::uv_tcp_init(static_cast<uv_loop_t*>(loop.getNative()), _tcp->get()) == 0;
}

void * Tcp::getNative()
{
    return _tcp->get();
}

void const * Tcp::getNative() const
{
    return _tcp->get();
}

std::string Tcp::getPeerName() const
{
    return _tcp->getPeerName();
}

std::string Tcp::getSocketName() const
{
    return _tcp->getSocketName();
}

} // namespace socket
} // namespace network

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

