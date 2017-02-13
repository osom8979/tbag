/**
 * @file   TcpClient.cpp
 * @brief  TcpClient class implementation.
 * @author zer0
 * @date   2016-12-29
 */

#include <libtbag/network/TcpClient.hpp>
#include <libtbag/log/Log.hpp>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace network {

// ----------------------
// Client implementation.
// ----------------------

TcpClient::Client::Client(Loop & loop, TcpClient * client) : Tcp(loop), parent(client)
{
    write_ready.store(true);
}

TcpClient::Client::~Client()
{
    // EMPTY.
}

TcpClient::uerr TcpClient::Client::write(char const * buffer, std::size_t size)
{
    bool IS_READY = true;
    if (write_ready.compare_exchange_weak(IS_READY, false)) {
        return Tcp::write(write_req, buffer, size);
    }
    return uerr::UVPP_NREADY;
}

void TcpClient::Client::onConnect(ConnectRequest & request, uerr code)
{
    if (parent != nullptr) {
        parent->onConnect(request, code);
    }
}

TcpClient::binf TcpClient::Client::onAlloc(std::size_t suggested_size)
{
    if (parent != nullptr) {
        return parent->onAlloc(suggested_size);
    }
    return binf();
}

void TcpClient::Client::onRead(uerr code, char const * buffer, std::size_t size)
{
    if (parent != nullptr) {
        parent->onRead(code, buffer, size);
    }
}

void TcpClient::Client::onWrite(WriteRequest & request, uerr code)
{
    if (parent != nullptr) {
        parent->onWrite(request, code);
    }
    assert(write_ready.load() == false);
    write_ready.store(true);
}

void TcpClient::Client::onClose()
{
    if (parent != nullptr) {
        parent->onClose();
    }
}

// ------------------------
// WriteJob implementation.
// ------------------------

TcpClient::WriteJob::WriteJob(WeakClient c, char const * data, std::size_t size) : client(c)
{
    DatagramInterface::Size const DATAGRAM_HEADER_SIZE = sizeof(uint32_t);

    Guard guard(mutex);
    buffer.resize(DATAGRAM_HEADER_SIZE + size);
    uint32_t    host_byte_size = static_cast<uint32_t>(size);
    uint32_t network_byte_size = DatagramInterface::toNetwork(host_byte_size);
    ::memcpy(&buffer[0], (char*)&network_byte_size, DATAGRAM_HEADER_SIZE);
    ::memcpy(&buffer[DATAGRAM_HEADER_SIZE], data, size);
}

TcpClient::WriteJob::~WriteJob()
{
    // EMPTY.
}

void TcpClient::WriteJob::run(Async * handle)
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
// TcpClient implementation.
// -------------------------

TcpClient::TcpClient()
{
    _client = _loop.newHandle<Client>(_loop, this);
    _async  = _loop.newHandle<Async >(_loop);
}

TcpClient::~TcpClient()
{
    // EMPTY.
}

bool TcpClient::initIpv4(std::string const & ip, int port)
{
    if (static_cast<bool>(_client) == false) {
        return false;
    }

    if (_client->isInit() == false) {
        return false;
    }

    sockaddr_in addr;
    if (_client->initAddress(ip, port, &addr) != uvpp::uerr::UVPP_SUCCESS) {
        return false;
    }

    if (_client->bind((sockaddr const *)&addr) != uvpp::uerr::UVPP_SUCCESS) {
        return false;
    }

    if (_client->listen() != uerr::UVPP_SUCCESS) {
        return false;
    }

    return true;
}

bool TcpClient::initIpv6(std::string const & ip, int port)
{
    return false;
}

bool TcpClient::init(std::string const & ip, int port)
{
    if (Tcp::isIpv4(ip)) {
        return initIpv4(ip, port);
    } else if (Tcp::isIpv6(ip)) {
        return initIpv6(ip, port);
    }
    return false;
}

bool TcpClient::run()
{
    return _loop.run() == uerr::UVPP_SUCCESS;
}

bool TcpClient::asyncClose()
{
    return false;
}

bool TcpClient::asyncWrite(char const * buffer, std::size_t size)
{
    if (std::this_thread::get_id() == _loop.getOwnerThreadId()) {
        return _client->write(buffer, size) == uerr::UVPP_SUCCESS;
    } else if (static_cast<bool>(_async)) {
        WeakClient weak = std::static_pointer_cast<Client>(_loop.findChildHandle(*_client).lock());
        auto shared = _async->safeNewPush<WriteJob>(weak, buffer, size);
        return static_cast<bool>(shared);
    }
    return false;
}

// --------------
// Event methods.
// --------------

void TcpClient::onConnect(ConnectRequest & request, uerr code)
{
    __tbag_debug("TcpClient::onConnect() result code({})", static_cast<int>(code));
}

TcpClient::binf TcpClient::onAlloc(std::size_t suggested_size)
{
    if (static_cast<bool>(_client)) {
        return uvpp::defaultOnAlloc(_client->buffer, suggested_size);
    }
    return binf();
}

void TcpClient::onRead(uerr code, char const * buffer, std::size_t size)
{
    __tbag_debug("TcpClient::onRead() result code({})", static_cast<int>(code));
}

void TcpClient::onWrite(WriteRequest & request, uerr code)
{
    __tbag_debug("TcpClient::onWrite() result code({})", static_cast<int>(code));
}

void TcpClient::onClose()
{
    __tbag_debug("TcpClient::onClose()");
}

} // namespace network

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

