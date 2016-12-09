/**
 * @file   Server.cpp
 * @brief  Server class implementation.
 * @author zer0
 * @date   2016-10-14
 */

#include <libtbag/network/socket/Server.hpp>
#include <libtbag/log/Log.hpp>
#include <libtbag/loop/UvEventDispatcher.hpp>

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
    closeClient(static_cast<uv_tcp_t*>(tcp.getNative()));
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

Server::Server(EventCallback * callback) : _write(util::UvType::WRITE), _read_buffer(), _callback(callback)
{
    using namespace server_details;
    TBAG_UV_EVENT_DEFAULT_REGISTER(_tcp.getNative(), this);
    TBAG_UV_EVENT_DEFAULT_REGISTER(_write.getNative(), this);
}

Server::Server() : Server(nullptr)
{
    // EMPTY.
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
    return _loop.runDefault();
}

void Server::close()
{
    server_details::close(_tcp);
}

bool Server::addClient(ClientKey key, ClientValue client)
{
    using namespace server_details;
    if (_clients.insert(ClientMap::value_type(key, client)).second == false) {
        __tbag_error_f("Server::addClient({}) Insert client error (size:{}).", key.get(), _clients.size());
        return false;
    }

    __tbag_debug_f("Server::addClient({}) Insert client (size:{}).", key.get(), _clients.size());
    TBAG_UV_EVENT_DEFAULT_REGISTER(key.get(), this);
    return true;
}

bool Server::addClient(ClientValue client)
{
    return addClient(ClientKey(client->getNative()), client);
}

bool Server::removeClient(ClientKey key)
{
    using namespace server_details;
    TBAG_UV_EVENT_DEFAULT_UNREGISTER(key.get());

    bool const ERASE_RESULT = _clients.erase(key) == 1U;
    if (ERASE_RESULT == false) {
        __tbag_error_f("Server::removeClient({}) Erase client error (size:{}).", key.get(), _clients.size());
        return false;
    }

    __tbag_debug_f("Server::removeClient({}) Erase client (size:{}).", key.get(), _clients.size());
    return true;
}

Server::ClientValue Server::getClient(ClientKey key)
{
    auto find_itr = _clients.find(key);
    if (find_itr == _clients.end()) {
        __tbag_error_f("Server::getClient({}) not found client key.", key.get());
        return ClientValue();
    }
    return find_itr->second;
}

void Server::closeClient(ClientKey key)
{
    auto find_itr = _clients.find(key);
    if (find_itr == _clients.end()) {
        __tbag_error_f("Server::closeClient({}) not found client key.", key.get());
        return;
    }

    ClientValue client = find_itr->second;
    if (static_cast<bool>(client) == false) {
        __tbag_error_f("Server::closeClient({}) invalidate client value.", key.get());
        return;
    }

    server_details::closeClient(*client.get());
}

bool Server::read(ClientKey key)
{
    ClientValue client = getClient(key);
    if (static_cast<bool>(client) == false) {
        __tbag_error_f("Server::read() not found client key ({})", key.get());
        return false;
    }
    return server_details::read(*client.get());
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
    ClientKey   key(client->getNative());
    client->init(_loop);

    server_details::accept(_tcp, *client.get());
    if (addClient(key, client) == false) {
        return;
    }

    if (_callback != nullptr) {
        if (_callback->onConnection(key, status) == false) {
            removeClient(key);
        }
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
    removeClient(ClientKey(handle));
}

void Server::onReadBufferAlloc(void * handle, size_t suggested_size, void * buf)
{
    __tbag_debug_f("Server::onReadBufferAlloc(suggested_size:{})", suggested_size);

    // Realloc with read buffer.
    if (_read_buffer.size() < suggested_size) {
        _read_buffer.resize(suggested_size);
    }

    uv_buf_t * uv_buf = static_cast<uv_buf_t*>(buf);
    uv_buf->base = &_read_buffer[0];
    uv_buf->len  =  _read_buffer.size();
}

void Server::onRead(void * client_stream, ssize_t nread, void const * buf)
{
    __tbag_debug_f("Server::onRead({})", client_stream);

    Code code = Code::FAILURE;

    if (nread == UV_EOF) {
        code = Code::END_OF_FILE;
    } else if (nread >= 0){
        code = Code::SUCCESS;
    }

    if (_callback != nullptr) {
        uv_buf_t const * uv_buf = static_cast<uv_buf_t const *>(buf);
        ClientKey from(client_stream);

        auto find_itr = _clients.find(from);
        if (find_itr == _clients.end()) {
            __tbag_error_f("Server::onRead({}) Not found client.", client_stream);
            return;
        }

        ClientValue client = find_itr->second;
        if (static_cast<bool>(client) == false) {
            __tbag_error_f("Server::onRead({}) Empty client.", client_stream);
            return;
        }

        __tbag_debug_f("Server::onRead() client name: {}", client->getSocketName());

        _callback->onRead(from, code, uv_buf->base, static_cast<std::size_t>(nread));
    }
}

void Server::onWrite(void * req, int status)
{
    __tbag_debug("Server::onWrite()");

    Code code = Code::FAILURE;
    if (status == 0) {
        code = Code::SUCCESS;
    }

    if (_callback != nullptr) {
        if (req != nullptr) {
            uv_write_t * write_request = static_cast<uv_write_t*>(req);
            _callback->onWrite(ClientKey(write_request->handle), code);
        } else {
            __tbag_error("Server::onWrite() write request is nullptr error.");
        }
    }
}

} // namespace socket
} // namespace network

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

