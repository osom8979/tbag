/**
 * @file   Client.cpp
 * @brief  Client class implementation.
 * @author zer0
 * @date   2016-10-14
 */

#include <libtbag/network/socket/Client.hpp>
#include <libtbag/log/Log.hpp>
#include <libtbag/loop/UvEventDispatcher.hpp>
#include <libtbag/debug/UvError.hpp>

#include <cstring>
#include <vector>

#include <uv.h>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace network {
namespace socket  {

// ------------------------------
// Client details implementation.
// ------------------------------

namespace client_details {

// -------------------
// Client message map.
// -------------------

TBAG_UV_EVENT_DEFAULT_IMPLEMENT_OPEN(Client);
//{
    TBAG_UV_EVENT_CONNECT(onConnect);
    TBAG_UV_EVENT_CLOSE(onClose);
    TBAG_UV_EVENT_ALLOC(onReadBufferAlloc);
    TBAG_UV_EVENT_READ(onRead);
    TBAG_UV_EVENT_WRITE(onWrite);
//}
TBAG_UV_EVENT_DEFAULT_IMPLEMENT_CLOSE;

static bool addr(std::string const & ip, int port, sockaddr_in * addr)
{
    int const ERROR_CODE = ::uv_ip4_addr(ip.c_str(), port, addr);
    if (ERROR_CODE != 0) {
        __tbag_error_f("socket::client_details ipv4 addr error: {}", ERROR_CODE);
        return false;
    }
    return true;
}

static bool addr(std::string const & ip, int port, sockaddr_in6 * addr)
{
    int const ERROR_CODE = ::uv_ip6_addr(ip.c_str(), port, addr);
    if (ERROR_CODE != 0) {
        __tbag_error_f("socket::client_details ipv6 addr error: {}", ERROR_CODE);
        return false;
    }
    return true;
}

static bool connect(uv_connect_t * req, uv_tcp_t * tcp, sockaddr const * address)
{
    int const ERROR_CODE = ::uv_tcp_connect(req, tcp, address, TBAG_UV_EVENT_DEFAULT_CALLBACK_CONNECT(onConnect));
    if (ERROR_CODE != 0) {
        __tbag_error_f("socket::client_details connect error: {}", ERROR_CODE);
        return false;
    }
    return true;
}

static bool connect(Client::Handle & req, Tcp & tcp, sockaddr_in const * address)
{
    return connect(static_cast<uv_connect_t*>(req.getNative()),
                   static_cast<uv_tcp_t*>(tcp.getNative()),
                   (sockaddr const *)address);
}

static bool connect(Client::Handle & req, Tcp & tcp, sockaddr_in6 const * address)
{
    return connect(static_cast<uv_connect_t*>(req.getNative()),
                   static_cast<uv_tcp_t*>(tcp.getNative()),
                   (sockaddr const *)address);
}

static void close(uv_tcp_t * tcp)
{
    ::uv_close((uv_handle_t*)tcp, TBAG_UV_EVENT_DEFAULT_CALLBACK_CLOSE(onClose));
}

static void close(Tcp & tcp)
{
    close(static_cast<uv_tcp_t*>(tcp.getNative()));
}

static bool read(uv_tcp_t * tcp)
{
    int const ERROR_CODE = ::uv_read_start((uv_stream_t*)tcp,
                                           TBAG_UV_EVENT_DEFAULT_CALLBACK_ALLOC(onReadBufferAlloc),
                                           TBAG_UV_EVENT_DEFAULT_CALLBACK_READ(onRead));
    if (ERROR_CODE != 0) {
        __tbag_error_f("socket::client_details read error: {}", ERROR_CODE);
        return false;
    }
    return true;
}

static bool read(Tcp & tcp)
{
    return read(static_cast<uv_tcp_t*>(tcp.getNative()));
}

static bool write(uv_write_t * handle, uv_tcp_t * tcp, char const * buffer, std::size_t size)
{
    uv_buf_t buf = {0,};
    buf.base = const_cast<char*>(buffer);
    buf.len  = size;

    int const ERROR_CODE = ::uv_write(handle, (uv_stream_t*)tcp, &buf, 1, TBAG_UV_EVENT_DEFAULT_CALLBACK_WRITE(onWrite));
    if (ERROR_CODE != 0) {
        __tbag_error_f("socket::client_details write error: {}", ERROR_CODE);
        return false;
    }
    return true;
}

static bool write(Client::Handle & handle, Tcp & tcp, char const * buffer, std::size_t size)
{
    if (handle.getType() != util::UvType::WRITE) {
        __tbag_error("socket::client_details write handle error.");
        return false;
    }

    return write(static_cast<uv_write_t*>(handle.getNative()),
                 static_cast<uv_tcp_t*>(tcp.getNative()),
                 buffer, size);
}

} // namespace client_details

// ----------------------
// Client implementation.
// ----------------------

Client::Client() : _connect(util::UvType::CONNECT), _write(util::UvType::WRITE)
{
    using namespace client_details;
    TBAG_UV_EVENT_DEFAULT_REGISTER(_tcp.getNative(), this);
    TBAG_UV_EVENT_DEFAULT_REGISTER(_connect.getNative(), this);
    TBAG_UV_EVENT_DEFAULT_REGISTER(_write.getNative(), this);
}

Client::~Client()
{
    using namespace client_details;
    TBAG_UV_EVENT_DEFAULT_UNREGISTER(_write.getNative());
    TBAG_UV_EVENT_DEFAULT_UNREGISTER(_connect.getNative());
    TBAG_UV_EVENT_DEFAULT_UNREGISTER(_tcp.getNative());
}

bool Client::run(std::string const & address, int port)
{
    // init -> addr -> connect.
    // TODO: Query DNS.
    if (Tcp::isIpv4(address)) {
        return runIpv4(address, port);
    } else {
        return runIpv6(address, port);
    }
}

bool Client::runIpv4(std::string const & address, int port)
{
    if (!_tcp.init(_loop)) {
        return false;
    }
    sockaddr_in addr;
    if (!client_details::addr(address, port, &addr)) {
        return false;
    }
    if (!client_details::connect(_connect, _tcp, &addr)) {
        return false;
    }
    return _loop.runDefault();
}

bool Client::runIpv6(std::string const & address, int port)
{
    return _loop.runDefault();
}

void Client::close()
{
    client_details::close(_tcp);
}

bool Client::write(char const * buffer, std::size_t size)
{
    return client_details::write(_write, _tcp, buffer, size);
}

void Client::onConnect(void * req, int status)
{
    bool is_read_start = false;
    if (_callback != nullptr) {
        is_read_start = _callback->onConnect(status);
    }

    if (is_read_start) {
        client_details::read(_tcp);
    }
}

void Client::onClose(void * handle)
{
    if (_callback != nullptr) {
        _callback->onClose();
    }
}

void Client::onReadBufferAlloc(void * handle, size_t suggested_size, void * buf)
{
    // Realloc with read buffer.
    if (_read_buffer.size() < suggested_size) {
        _read_buffer.resize(suggested_size);
    }

    uv_buf_t * uv_buf = static_cast<uv_buf_t*>(buf);
    uv_buf->base = &_read_buffer[0];
    uv_buf->len  =  _read_buffer.size();
}

void Client::onRead(void * stream, ssize_t nread, void const * buf)
{
    Code code = Code::FAILURE;
    if (nread == UV_EOF) {
        code = Code::END_OF_FILE;
    } else if (nread >= 0){
        code = Code::SUCCESS;
    }

    if (_callback != nullptr) {
        uv_buf_t const * uv_buf = static_cast<uv_buf_t const *>(buf);
        _callback->onRead(code, uv_buf->base, static_cast<std::size_t>(nread));
    }
}

void Client::onWrite(void * req, int status)
{
    Code code = Code::FAILURE;
    if (status == 0) {
        code = Code::SUCCESS;
    }

    if (_callback != nullptr) {
        _callback->onWrite(code);
    }
}

} // namespace socket
} // namespace network

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

