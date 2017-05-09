/**
 * @file   TcpServer.cpp
 * @brief  TcpServer class implementation.
 * @author zer0
 * @date   2017-05-06
 */

#include <libtbag/network/details/TcpServer.hpp>
#include <libtbag/id/generator/TimeId.hpp>
#include <libtbag/log/Log.hpp>

#include <cassert>
#include <thread>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace network {
namespace details {

// -----------------------------
// TcpRealServer implementation.
// -----------------------------

TcpRealServer::TcpRealServer(Loop & loop, TcpServer & parent) : Tcp(loop), _parent(parent)
{
    _on_connection.store(false);
}

TcpRealServer::~TcpRealServer()
{
    // EMPTY.
}

bool TcpRealServer::init(String const & ip, int port)
{
    return uvpp::initCommonServer(*this, ip, port);
}

void TcpRealServer::onConnection(uerr code)
{
    _on_connection.store(true);
    _parent.onConnection(code);
    _on_connection.store(false);
}

void TcpRealServer::onClose()
{
    _parent.onServerClose();

    {   // Update parent state.
        Guard guard(_parent._mutex);
        _parent.closeAll();
    }
}

// -------------------------
// TcpServer implementation.
// -------------------------

TcpServer::TcpServer(Loop & loop)
{
    _server = loop.newHandle<TcpRealServer>(loop, *this);
    _async  = loop.newHandle<SafetyWriteAsync>(loop);
    assert(static_cast<bool>(_server));
    assert(static_cast<bool>(_async));
}

TcpServer::~TcpServer()
{
    _server.reset();
    _async.reset();
}

TcpServer::SharedClient TcpServer::createClient(Loop & loop)
{
    SharedClient client(new (std::nothrow) TcpClient(loop));
    if (static_cast<bool>(client)) {
        return client;
    }
    return SharedClient();
}

bool TcpServer::insertClient(SharedClient client)
{
    return _clients.insert(ClientPair(client->getId(), client)).second;
}

bool TcpServer::removeClient(ClientKey key)
{
    return _clients.erase(key) == 1U;
}

void TcpServer::closeAll()
{
    {   // Close all clients.
        for (auto & cursor : _clients) {
            if (static_cast<bool>(cursor.second)) {
                cursor.second->close();
            }
        }
    }

    assert(static_cast<bool>(_server));
    assert(static_cast<bool>(_async));

    if (_async->isClosing() == false) {
        _async->close();
    }

    if (_server->isClosing() == false) {
        _server->close();
    }
}

bool TcpServer::init(String const & ip, int port)
{
    assert(static_cast<bool>(_server));
    Guard guard(_mutex);
    return _server->init(ip, port);
}

TcpServer::WeakClient TcpServer::accept()
{
    assert(static_cast<bool>(_server));
    if (_server->isOnConnection() == false) {
        __tbag_error("TcpServer::accept() server is not a connection state.");
        return WeakClient();
    }

    Loop * loop = _server->getLoop();
    assert(loop != nullptr);

    Guard guard(_mutex);
    auto client = std::static_pointer_cast<TcpClient, Client>(TcpServer::createClient(*loop));
    if (auto shared = client->getClient().lock()) {
        uerr const CODE = _server->accept(*shared);
        if (CODE == uerr::UVPP_SUCCESS) {
            __tbag_debug("TcpServer::accept() client connect Sock({}:{})/Peer({}:{})",
                         shared->getSockIp(), shared->getSockPort(),
                         shared->getPeerIp(), shared->getPeerPort());
            bool const INSERT_RESULT = insertClient(client);
            assert(INSERT_RESULT);
            return WeakClient(client);
        } else {
            __tbag_error("TcpServer::accept() {} error.", uvpp::getErrorName(CODE));
        }
    } else {
        __tbag_error("TcpServer::accept() client is nullptr.");
    }
    return WeakClient();
}

void TcpServer::close()
{
    assert(static_cast<bool>(_server));
    assert(static_cast<bool>(_async));

    Loop * loop = _server->getLoop();
    assert(loop != nullptr);

    if (loop->isAliveAndThisThread()) {
        __tbag_debug("TcpServer::close() sync request.");
        Guard guard(_mutex);
        closeAll();
    } else {
        __tbag_debug("TcpServer::close() async request.");
        _async->newSendFunc([&](SafetyAsync * UNUSED_PARAM(async)) {
            Guard guard(_mutex);
            closeAll();
        });
    }
}

} // namespace details
} // namespace network

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

