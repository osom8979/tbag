/**
 * @file   Server.cpp
 * @brief  Server class implementation.
 * @author zer0
 * @date   2016-10-14
 */

#include <libtbag/network/socket/Server.hpp>
#include <libtbag/log/Log.hpp>
#include <libtbag/loop/UvEventDispatcher.hpp>
#include <libtbag/debug/UvError.hpp>

#include <cstdlib>
#include <cstring>
#include <vector>

#include <uv.h>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace network {
namespace socket  {

// ------------------------------
// Server details implementation.
// ------------------------------

namespace server_details {

// -------------------
// Server message map.
// -------------------

TBAG_UV_EVENT_DEFAULT_IMPLEMENT_OPEN(Server);
//{
    TBAG_UV_EVENT_CONNECTION(onConnection);
    TBAG_UV_EVENT_CLOSE(onClose);
    TBAG_UV_EVENT_CLOSE(onCloseClient);
    TBAG_UV_EVENT_ALLOC(onReadBufferAlloc);
    TBAG_UV_EVENT_READ(onRead);
    TBAG_UV_EVENT_WRITE(onWrite);
//}
TBAG_UV_EVENT_DEFAULT_IMPLEMENT_CLOSE;

static bool addr(std::string const & ip, int port, sockaddr_in * addr)
{
    int const ERROR_CODE = ::uv_ip4_addr(ip.c_str(), port, addr);
    if (ERROR_CODE != 0) {
        __tbag_error_f("socket::server_details ipv4 addr error: {}", ERROR_CODE);
        return false;
    }
    return true;
}

static bool addr(std::string const & ip, int port, sockaddr_in6 * addr)
{
    int const ERROR_CODE = ::uv_ip6_addr(ip.c_str(), port, addr);
    if (ERROR_CODE != 0) {
        __tbag_error_f("socket::server_details ipv6 addr error: {}", ERROR_CODE);
        return false;
    }
    return true;
}

static bool bind(uv_tcp_t * tcp, sockaddr const * address)
{
    unsigned int const BIND_FLAGS = 0;
    int const ERROR_CODE = ::uv_tcp_bind(tcp, address, BIND_FLAGS);
    if (ERROR_CODE != 0) {
        __tbag_error_f("socket::server_details bind error: {}", ERROR_CODE);
        return false;
    }
    return true;
}

static bool bind(Tcp & tcp, sockaddr_in const * address)
{
    return bind(static_cast<uv_tcp_t*>(tcp.getNative()), (sockaddr const *)address);
}

static bool bind(Tcp & tcp, sockaddr_in6 const * address)
{
    return bind(static_cast<uv_tcp_t*>(tcp.getNative()), (sockaddr const *)address);
}

static bool listen(uv_tcp_t * tcp)
{
    int const LISTEN_QUEUE_LIMIT = 128;
    int const ERROR_CODE = ::uv_listen((uv_stream_t*)tcp, LISTEN_QUEUE_LIMIT, TBAG_UV_EVENT_DEFAULT_CALLBACK_CONNECTION(onConnection));
    if (ERROR_CODE != 0) {
        __tbag_error_f("socket::server_details listen error: {}", ERROR_CODE);
        return false;
    }
    return true;
}

static bool listen(Tcp & tcp)
{
    return listen(static_cast<uv_tcp_t*>(tcp.getNative()));
}

static bool accept(uv_tcp_t * server, uv_tcp_t * client)
{
    int const ERROR_CODE = ::uv_accept((uv_stream_t*)server, (uv_stream_t*)client);
    if (ERROR_CODE != 0) {
        __tbag_error_f("socket::server_details accept error: {}", ERROR_CODE);
        return false;
    }
    return true;
}

static bool accept(Tcp & server, Tcp & client)
{
    return accept(static_cast<uv_tcp_t*>(server.getNative()), static_cast<uv_tcp_t*>(client.getNative()));
}

static void close(uv_tcp_t * tcp)
{
    ::uv_close((uv_handle_t*)tcp, TBAG_UV_EVENT_DEFAULT_CALLBACK_CLOSE(onClose));
}

static void close(Tcp & tcp)
{
    close(static_cast<uv_tcp_t*>(tcp.getNative()));
}

static void closeClient(uv_tcp_t * tcp)
{
    ::uv_close((uv_handle_t*)tcp, TBAG_UV_EVENT_DEFAULT_CALLBACK_CLOSE(onCloseClient));
}

static void closeClient(Tcp & tcp)
{
    close(static_cast<uv_tcp_t*>(tcp.getNative()));
}

static bool read(uv_tcp_t * tcp)
{
    int const ERROR_CODE = ::uv_read_start((uv_stream_t*)tcp,
                                           TBAG_UV_EVENT_DEFAULT_CALLBACK_ALLOC(onReadBufferAlloc),
                                           TBAG_UV_EVENT_DEFAULT_CALLBACK_READ(onRead));
    if (ERROR_CODE != 0) {
        __tbag_error_f("socket::server_details read error: {}", ERROR_CODE);
        return false;
    }
    return true;
}

static bool read(Tcp & tcp)
{
    return read(static_cast<uv_tcp_t*>(tcp.getNative()));
}

static bool write(uv_write_t * handle, uv_tcp_t * tcp, char const * buffer, std::size_t length)
{
    uv_buf_t buf = {0,};
    buf.base = const_cast<char*>(buffer);
    buf.len  = length;

    int const ERROR_CODE = ::uv_write(handle, (uv_stream_t*)tcp, &buf, 1, TBAG_UV_EVENT_DEFAULT_CALLBACK_WRITE(onWrite));
    if (ERROR_CODE != 0) {
        __tbag_error_f("socket::server_details write error: {}", ERROR_CODE);
        return false;
    }
    return true;
}

static bool write(Server::Handle & handle, Tcp & tcp, char const * buffer, std::size_t size)
{
    if (handle.getType() != util::UvType::WRITE) {
        __tbag_error("socket::server_details write handle error.");
        return false;
    }

    return write(static_cast<uv_write_t*>(handle.getNative()),
                 static_cast<uv_tcp_t*>(tcp.getNative()),
                 buffer, size);
}

} // namespace server_details

// ----------------------
// Server implementation.
// ----------------------

Server::Server() : _write(util::UvType::WRITE)
{
    using namespace server_details;
    TBAG_UV_EVENT_DEFAULT_REGISTER(_tcp.getNative(), this);
    TBAG_UV_EVENT_DEFAULT_REGISTER(_write.getNative(), this);
}

Server::~Server()
{
    using namespace server_details;
    TBAG_UV_EVENT_DEFAULT_UNREGISTER(_write.getNative());
    TBAG_UV_EVENT_DEFAULT_UNREGISTER(_tcp.getNative());
}

bool Server::run(std::string const & address, int port)
{
    // init -> addr -> bind -> listen -> onConnection -> accept.
    // TODO: Query DNS.
    if (Tcp::isIpv4(address)) {
        return runIpv4(address, port);
    } else {
        return runIpv6(address, port);
    }
}

bool Server::runIpv4(std::string const & address, int port)
{
    if (!_tcp.init(_loop)) {
        return false;
    }
    sockaddr_in addr;
    if (!server_details::addr(address, port, &addr)) {
        return false;
    }
    if (!server_details::bind(_tcp, &addr)) {
        return false;
    }
    if (!server_details::listen(_tcp)) {
        return false;
    }
    if (!server_details::read(_tcp)) {
        return false;
    }
    return _loop.runDefault();
}

bool Server::runIpv6(std::string const & address, int port)
{
    if (!_tcp.init(_loop)) {
        return false;
    }
    sockaddr_in6 addr;
    if (!server_details::addr(address, port, &addr)) {
        return false;
    }
    if (!server_details::bind(_tcp, &addr)) {
        return false;
    }
    if (!server_details::listen(_tcp)) {
        return false;
    }
    if (!server_details::read(_tcp)) {
        return false;
    }
    return _loop.runDefault();
}

void Server::close()
{
    server_details::close(_tcp);
}

void Server::closeClient(ClientKey key)
{
    auto find_itr = _clients.find(key);
    if (find_itr == _clients.end()) {
        __tbag_error("Server::closeClient() not found client key.");
        return;
    }

    ClientValue client = find_itr->second;
    if (static_cast<bool>(client) == false) {
        __tbag_error("Server::closeClient() invalidate client value.");
        return;
    }

    server_details::closeClient(*client.get());
}

bool Server::write(ClientKey key, char * buffer, std::size_t size)
{
    auto find_itr = _clients.find(key);
    if (find_itr == _clients.end()) {
        __tbag_error("Server::write() not found client key.");
        return false;
    }

    ClientValue client = find_itr->second;
    if (static_cast<bool>(client) == false) {
        __tbag_error("Server::write() invalidate client value.");
        return false;
    }

    return server_details::write(_write, *client.get(), buffer, size);
}

void Server::onConnection(void * server, int status)
{
    ClientValue client(new Tcp);
    client->init(_loop);

    bool is_accept = false;
    std::string client_address;

    // TODO: Obtain client address.

    if (_callback != nullptr) {
        is_accept = _callback->onConnection(client_address, status);
    }

    __tbag_debug_f("Server::onConnection: status({}), ip({})", status, client_address);

    if (is_accept && server_details::accept(_tcp, *client.get())) {
        _clients.insert(ClientMap::value_type(ClientKey(client->getNative()), client));
    }
}

void Server::onClose(void * handle)
{
    __tbag_debug("Server::onClose()");
    if (_callback != nullptr) {
        _callback->onClose();
    }
}

void Server::onCloseClient(void * handle)
{
    __tbag_debug_f("Server::onCloseClient({})", handle);
    if (_callback != nullptr) {
        _callback->onCloseClient(ClientKey(handle));
    }
    _clients.erase(ClientKey(handle));
}

void Server::onReadBufferAlloc(void * handle, size_t suggested_size, void * buf)
{
    // Realloc with read buffer.
    if (_read_buffer.size() < suggested_size) {
        _read_buffer.resize(suggested_size);
    }

    uv_buf_t * uv_buf = static_cast<uv_buf_t*>(buf);
    uv_buf->base = &_read_buffer[0];
    uv_buf->len  =  _read_buffer.size();
}

void Server::onRead(void * stream, ssize_t nread, void const * buf)
{
    Code code = Code::FAILURE;

    if (nread == UV_EOF) {
        code = Code::END_OF_FILE;
    } else if (nread >= 0){
        code = Code::SUCCESS;
    }

    if (_callback != nullptr) {
        uv_buf_t const * uv_buf = static_cast<uv_buf_t const *>(buf);
        ClientKey from;
        // TODO: FIND FROM KEY.
        _callback->onRead(from, code, uv_buf->base, static_cast<std::size_t>(nread));
    }
}

void Server::onWrite(void * req, int status)
{
    Code code = Code::FAILURE;
    if (status == 0) {
        code = Code::SUCCESS;
    }

    if (_callback != nullptr) {
        _callback->onWrite(ClientKey(req), code);
    }
}

} // namespace socket
} // namespace network

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

