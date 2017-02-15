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

TcpServer::TcpServer()
{
    _server = _loop.newHandle<ServerTcp>(_loop, this);
    _async  = _loop.newHandle<Async>(_loop);
}

TcpServer::~TcpServer()
{
    // EMPTY.
}

TcpServer::WeakClient TcpServer::acceptedNewClient()
{
    assert(static_cast<bool>(_server));

    auto client = _loop.newHandle<ClientTcp>(_loop, this);
    if (static_cast<bool>(client) == false) {
        return WeakClient();
    }
    assert(_loop.findChildHandle(*client).expired() == false);

    if (_server->accept(*client) != uerr::UVPP_SUCCESS) {
        _loop.eraseChildHandle(*client);
        return WeakClient();
    }

    return WeakClient(client);
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

bool TcpServer::asyncClose(ClientTcp & client)
{
    Guard guard(_async_mutex);
    if (std::this_thread::get_id() == _loop.getOwnerThreadId()) {
        client.close();
        return true;
    }

    WeakClient weak = std::static_pointer_cast<ClientTcp>(_loop.findChildHandle(client).lock());
    if (weak.expired()) {
        return false;
    }

    if (auto shared = _async->newPushJob<CloseJob>(weak)) {
        return _async->send() == uerr::UVPP_SUCCESS;
    }
    return false;
}

bool TcpServer::asyncWrite(ClientTcp & client, char const * buffer, std::size_t size)
{
    Guard guard(_async_mutex);
    client.pushWriteBuffer(buffer, size);

    if (std::this_thread::get_id() == _loop.getOwnerThreadId()) {
        return client.writeWithPushedBuffer() == uerr::UVPP_SUCCESS;
    }

    WeakClient weak = std::static_pointer_cast<ClientTcp>(_loop.findChildHandle(client).lock());
    if (weak.expired()) {
        return false;
    }

    if (auto shared = _async->newPushJob<WriteJob>(weak)) {
        return _async->send() == uerr::UVPP_SUCCESS;
    }
    return false;
}

// -----------------
// Callback methods.
// -----------------

void TcpServer::onConnection(BaseTcp & tcp, uerr code)
{
    if (tcp.getCsType() == CsType::SERVER && code == uerr::UVPP_SUCCESS) {
        ServerTcp & server = static_cast<ServerTcp&>(tcp);
        WeakClient client = acceptedNewClient();

        bool const IS_ACCEPT = onNewConnection(code, client);
        if (IS_ACCEPT == false) {
            _loop.eraseChildHandle(*(client.lock()));
        }
    } else {
        onNewConnection(code, WeakClient());
    }
}

TcpServer::binf TcpServer::onAlloc(BaseTcp & tcp, std::size_t suggested_size)
{
    if (tcp.getCsType() == CsType::CLIENT) {
        ClientTcp & client = static_cast<ClientTcp&>(tcp);
        return onClientAlloc(client, suggested_size);
    }
    return binf();
}

void TcpServer::onWrite(BaseTcp & tcp, WriteRequest & request, uerr code)
{
    if (tcp.getCsType() == CsType::CLIENT) {
        ClientTcp & client = static_cast<ClientTcp&>(tcp);
        onClientWrite(client, request, code);
    }
}

void TcpServer::onClose(BaseTcp & tcp)
{
    if (tcp.getCsType() == CsType::SERVER) {
        ServerTcp & server = static_cast<ServerTcp&>(tcp);
        onServerClose(server);
    } else if (tcp.getCsType() == CsType::CLIENT) {
        ClientTcp & client = static_cast<ClientTcp&>(tcp);
        onClientClose(client);
    }
}

void TcpServer::onReadEof(BaseTcp & tcp, uerr code, char const * buffer, std::size_t size)
{
    if (tcp.getCsType() == CsType::CLIENT) {
        ClientTcp & client = static_cast<ClientTcp&>(tcp);
        onClientReadEof(client, code, buffer, size);
    }
}

void TcpServer::onReadDatagram(BaseTcp & tcp, uerr code, char const * buffer, std::size_t size)
{
    if (tcp.getCsType() == CsType::CLIENT) {
        ClientTcp & client = static_cast<ClientTcp&>(tcp);
        onClientReadDatagram(client, code, buffer, size);
    }
}

void TcpServer::onReadError(BaseTcp & tcp, uerr code, char const * buffer, std::size_t size)
{
    if (tcp.getCsType() == CsType::CLIENT) {
        ClientTcp & client = static_cast<ClientTcp&>(tcp);
        onClientReadError(client, code, buffer, size);
    }
}

void TcpServer::onAsyncWrite(BaseTcp & tcp, uerr code)
{
    if (tcp.getCsType() == CsType::CLIENT) {
        ClientTcp & client = static_cast<ClientTcp&>(tcp);
        onClientAsyncWrite(client, code);
    }
}

// --------------
// Event methods.
// --------------

bool TcpServer::onNewConnection(uerr code, WeakClient client)
{
    __tbag_debug("TcpServer::onNewConnection() result code({})", static_cast<int>(code));
    return true;
}

TcpServer::binf TcpServer::onClientAlloc(ClientTcp & client, std::size_t suggested_size)
{
    return uvpp::defaultOnAlloc(client.atReadBuffer(), suggested_size);
}

void TcpServer::onClientWrite(ClientTcp & client, WriteRequest & request, uerr code)
{
    __tbag_debug("TcpServer::onClientWrite({}) result code({})", (void*)&client, static_cast<int>(code));
}

void TcpServer::onClientReadEof(ClientTcp & client, uerr code, char const * buffer, std::size_t size)
{
    __tbag_debug("TcpServer::onClientReadEof({}) result code({})", (void*)&client, static_cast<int>(code));
}

void TcpServer::onClientReadDatagram(ClientTcp & client, uerr code, char const * buffer, std::size_t size)
{
    __tbag_debug("TcpServer::onClientReadDatagram({}) result code({})", (void*)&client, static_cast<int>(code));
}

void TcpServer::onClientReadError(ClientTcp & client, uerr code, char const * buffer, std::size_t size)
{
    __tbag_debug("TcpServer::onClientReadError({}) result code({})", (void*)&client, static_cast<int>(code));
}

void TcpServer::onClientAsyncWrite(ClientTcp & client, uerr code)
{
    __tbag_debug("TcpServer::onClientAsyncWrite({}) result code({})", (void*)&client, static_cast<int>(code));
}

void TcpServer::onServerClose(ServerTcp & server)
{
    __tbag_debug("TcpServer::onServerClose({})", (void*)&server);
}

void TcpServer::onClientClose(ClientTcp & client)
{
    __tbag_debug("TcpServer::onClientClose({})", (void*)&client);
}

} // namespace network

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

