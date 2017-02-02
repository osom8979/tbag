/**
 * @file   TcpServer.cpp
 * @brief  TcpServer class implementation.
 * @author zer0
 * @date   2016-12-30
 */

#include <libtbag/network/TcpServer.hpp>
#include <libtbag/log/Log.hpp>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace network {

TcpServer::binf TcpServer::Client::onAlloc(std::size_t suggested_size)
{
    return server.onClientAlloc(*this, suggested_size);
}

void TcpServer::Client::onRead(Err code, char const * buffer, std::size_t size)
{
    server.onClientRead(*this, code, buffer, size);
}

void TcpServer::Client::onWrite(WriteRequest & request, Err code)
{
    server.onClientWrite(*this, request, code);
    releaseWriteRequest(&request);
}

void TcpServer::Client::onClose()
{
    server.onClientClose(*this);
    auto const SAFETY_SHARED = shared_from_this();
    server.eraseClient(*this);
}

// -------------------------
// TcpServer implementation.
// -------------------------

TcpServer::TcpServer()
{
    // EMPTY.
}

TcpServer::~TcpServer()
{
    // EMPTY.
}

TcpServer::SharedClient TcpServer::createClient()
{
    return SharedClient(new Client(*this));
}

bool TcpServer::insertClient(SharedClient const & client)
{
    return _clients.insert(ClientMap::value_type(ClientKey(client.get()), client)).second;
}

void TcpServer::eraseClient(Client & client)
{
    _clients.erase(ClientKey(&client));
}

TcpServer::ClientKey TcpServer::getClientKey(Client & client) const
{
    return ClientKey(&client);
}

TcpServer::WeakClient TcpServer::getWeakClient(ClientKey const & client)
{
    auto itr = _clients.find(client);
    if (itr == _clients.end()) {
        return WeakClient();
    }
    return WeakClient(itr->second);
}

TcpServer::WeakClient TcpServer::createAcceptedClient()
{
    auto client = createClient();
    if (atTcp()->accept(*client->atTcp()) && insertClient(client)) {
        return WeakClient(client);
    }
    return WeakClient();
}

bool TcpServer::initIpv4(std::string const & ip, int port)
{
    auto & TCP = atTcp();
    if (TCP->isInit() == false) {
        return false;
    }

    sockaddr_in addr;
    if (TCP->initAddress(ip, port, &addr) == false) {
        return false;
    }

    if (TCP->bind((sockaddr const *)&addr) == false) {
        return false;
    }

    if (TCP->listen() == false) {
        return false;
    }

    return true;
}

bool TcpServer::run(std::string const & ip, int port)
{
    if (uvpp::Tcp::isIpv4(ip) && initIpv4(ip, port)) {
        return atLoop().run() == uvpp::uerr::UVPP_SUCCESS;
    }
    return false;
}

// ------------------------
// Extension event methods.
// ------------------------

TcpServer::binf TcpServer::onClientAlloc(Client & client, std::size_t suggested_size)
{
    return uvpp::defaultOnAlloc(client.atReadBuffer(), suggested_size);
}

void TcpServer::onClientRead(Client & client, Err code, char const * buffer, std::size_t size)
{
    __tbag_debug("TcpServer::onClientRead({}) size({}), result code({})", (void*)&client, size, static_cast<int>(code));
}

void TcpServer::onClientWrite(Client & client, WriteRequest & request, Err code)
{
    __tbag_debug("TcpServer::onClientWrite({}) result code: {}", (void*)&client, static_cast<int>(code));
}

void TcpServer::onClientClose(Client & client)
{
    __tbag_debug("TcpServer::onClientClose({}) erase client (size:{})", (void*)&client, _clients.size());
}

} // namespace network

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

