/**
 * @file   TcpServer.cpp
 * @brief  TcpServer class implementation.
 * @author zer0
 * @date   2016-12-30
 */

#include <libtbag/network/TcpServer.hpp>
#include <libtbag/log/Log.hpp>
#include <cassert>
#include <thread>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace network {

// ----------------------
// Server implementation.
// ----------------------

TcpServer::Server::Server(Loop & loop, TcpServer * server) : Tcp(loop), parent(server)
{
    // EMPTY.
}

TcpServer::Server::~Server()
{
    // EMPTY.
}

void TcpServer::Server::onConnection(uerr code)
{
    if (parent != nullptr) {
        parent->onConnection(code);
    }
}

void TcpServer::Server::onClose()
{
    if (parent != nullptr) {
        parent->onClose();
    }
}

// ----------------------
// Client implementation.
// ----------------------

TcpServer::Client::Client(Loop & loop, TcpServer * server) : Tcp(loop), parent(server)
{
    write_ready.store(true);
}

TcpServer::Client::~Client()
{
    // EMPTY.
}

TcpServer::binf TcpServer::Client::onAlloc(std::size_t suggested_size)
{
    if (parent != nullptr) {
        return parent->onClientAlloc(*this, suggested_size);
    }
    return binf();
}

void TcpServer::Client::onRead(uerr code, char const * buffer, std::size_t size)
{
    if (parent != nullptr) {
        parent->onClientRead(*this, code, buffer, size);
    }
}

void TcpServer::Client::onWrite(WriteRequest & request, uerr code)
{
    if (parent != nullptr) {
        parent->onClientWrite(*this, request, code);
    }
    assert(write_ready.load() == false);
    write_ready.store(true);
}

void TcpServer::Client::onClose()
{
    if (parent != nullptr) {
        parent->onClientClose(*this);
    }
}

TcpServer::uerr TcpServer::Client::write(char const * buffer, std::size_t size)
{
    bool IS_READY = true;
    if (write_ready.compare_exchange_weak(IS_READY, false)) {
        return Tcp::write(write_req, buffer, size);
    }
    return uerr::UVPP_NREADY;
}

// ------------------------
// WriteJob implementation.
// ------------------------

TcpServer::WriteJob::WriteJob(WeakClient c, char const * data, std::size_t size) : client(c)
{
    DatagramInterface::Size const DATAGRAM_HEADER_SIZE = sizeof(uint32_t);

    Guard guard(mutex);
    buffer.resize(DATAGRAM_HEADER_SIZE + size);
    uint32_t    host_byte_size = static_cast<uint32_t>(size);
    uint32_t network_byte_size = DatagramInterface::toNetwork(host_byte_size);
    ::memcpy(&buffer[0], (char*)&network_byte_size, DATAGRAM_HEADER_SIZE);
    ::memcpy(&buffer[DATAGRAM_HEADER_SIZE], data, size);
}

TcpServer::WriteJob::~WriteJob()
{
    // EMPTY.
}

void TcpServer::WriteJob::run(Async * handle)
{
    if (auto shared = client.lock()) {
        mutex.lock();
        result = shared->write(&buffer[0], buffer.size());
        mutex.unlock();
    } else {
        result = uerr::UVPP_NULLPTR;
    }
}

// -------------------------
// TcpServer implementation.
// -------------------------

TcpServer::TcpServer()
{
    _server = _loop.newHandle<Server>(_loop, this);
    _async  = _loop.newHandle<Async >(_loop);
}

TcpServer::~TcpServer()
{
    // EMPTY.
}

TcpServer::WeakClient TcpServer::acceptedNewClient()
{
    if (static_cast<bool>(_server)) {
        auto client = _loop.newHandle<Client>(_loop, this);
        if (_server->accept(*client) == uerr::UVPP_SUCCESS) {
            assert(_loop.findChildHandle(*client).expired() == false);
            return WeakClient(client);
        }
        _loop.eraseChildHandle(*client);
    }
    return WeakClient();
}

bool TcpServer::initIpv4(std::string const & ip, int port)
{
    SharedServer server = _server;
    if (static_cast<bool>(server) == false) {
        return false;
    }

    if (server->isInit() == false) {
        return false;
    }

    sockaddr_in addr;
    if (server->initAddress(ip, port, &addr) != uvpp::uerr::UVPP_SUCCESS) {
        return false;
    }

    if (server->bind((sockaddr const *)&addr) != uvpp::uerr::UVPP_SUCCESS) {
        return false;
    }

    if (server->listen() != uerr::UVPP_SUCCESS) {
        return false;
    }

    return true;
}

bool TcpServer::initIpv6(std::string const & ip, int port)
{
    return false;
}

bool TcpServer::init(std::string const & ip, int port)
{
    if (Tcp::isIpv4(ip)) {
        return initIpv4(ip, port);
    } else if (Tcp::isIpv6(ip)) {
        return initIpv6(ip, port);
    }
    return false;
}

bool TcpServer::run()
{
    return _loop.run() == uerr::UVPP_SUCCESS;
}

bool TcpServer::asyncClose()
{
    return false;
}

bool TcpServer::asyncWrite(Client & client, char const * buffer, std::size_t size)
{
    if (std::this_thread::get_id() == _loop.getOwnerThreadId()) {
        return client.write(buffer, size) == uerr::UVPP_SUCCESS;
    } else if (static_cast<bool>(_async)) {
        WeakClient weak = std::static_pointer_cast<Client>(_loop.findChildHandle(client).lock());
        auto shared = _async->safeNewPush<WriteJob>(weak, buffer, size);
        return static_cast<bool>(shared);
    }
    return false;
}

// ------------------------
// Extension event methods.
// ------------------------

void TcpServer::onConnection(uerr code)
{
    __tbag_debug("TcpServer::onConnection() result code({})", static_cast<int>(code));
}

void TcpServer::onClose()
{
    __tbag_debug("TcpServer::onClose()");
}

TcpServer::binf TcpServer::onClientAlloc(Client & client, std::size_t suggested_size)
{
    return uvpp::defaultOnAlloc(client.buffer, suggested_size);
}

void TcpServer::onClientRead(Client & client, uerr code, char const * buffer, std::size_t size)
{
    __tbag_debug("TcpServer::onClientRead({}) result code({})", (void*)&client, static_cast<int>(code));
}

void TcpServer::onClientWrite(Client & client, WriteRequest & request, uerr code)
{
    __tbag_debug("TcpServer::onClientWrite({}) result code({})", (void*)&client, static_cast<int>(code));
}

void TcpServer::onClientClose(Client & client)
{
    __tbag_debug("TcpServer::onClientClose({})", (void*)&client);
}

} // namespace network

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

