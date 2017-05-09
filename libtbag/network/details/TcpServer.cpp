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

// ---------------------------
// TcpRealNode implementation.
// ---------------------------

TcpClientNode::TcpClientNode(Loop & loop, TcpServer & parent)
        : TcpClient(loop), _parent(parent)
{
    // EMPTY.
}

TcpClientNode::~TcpClientNode()
{
    // EMPTY.
}

Server::WeakClient TcpClientNode::getWeakClient()
{
    Guard guard(_parent._mutex);
    return _parent.getWeakClient(getId());
}

void TcpClientNode::onShutdown(uerr code)
{
    _parent.onClientShutdown(getWeakClient(), code);
}

void TcpClientNode::onWrite(uerr code)
{
    _parent.onClientWrite(getWeakClient(), code);
}

void TcpClientNode::onRead(uerr code, char const * buffer, Size size)
{
    _parent.onClientRead(getWeakClient(), code, buffer, size);
}

void TcpClientNode::onClose()
{
    _parent.onClientClose(getWeakClient());
}

void * TcpClientNode::onUserDataAlloc()
{
    return _parent.onClientUserDataAlloc(getWeakClient());
}

void TcpClientNode::onUserDataDealloc(void * data)
{
    _parent.onClientUserDataDealloc(getWeakClient(), data);
}

// -----------------------------
// TcpRealServer implementation.
// -----------------------------

TcpRealServer::TcpRealServer(Loop & loop, TcpServer & parent) : Tcp(loop), _parent(parent)
{
    _on_connection.store(false);
    setUserData(_parent.onUserDataAlloc());
}

TcpRealServer::~TcpRealServer()
{
    _parent.onUserDataDealloc(getUserData());
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

TcpServer::SharedClient TcpServer::createClient(Loop & loop, TcpServer & server)
{
    SharedClient client(new (std::nothrow) TcpClientNode(loop, server));
    if (static_cast<bool>(client)) {
        return client;
    }
    return SharedClient();
}

TcpServer::SharedClient TcpServer::createClient()
{
    assert(static_cast<bool>(_server));

    Loop * loop = _server->getLoop();
    assert(loop != nullptr);

    return createClient(*loop, *this);
}

TcpServer::SharedClient TcpServer::getSharedClient(ClientKey key)
{
    auto itr = _clients.find(key);
    if (itr != _clients.end()) {
        return itr->second;
    }
    return SharedClient();
}

TcpServer::WeakClient TcpServer::getWeakClient(ClientKey key)
{
    auto itr = _clients.find(key);
    if (itr != _clients.end()) {
        return WeakClient(itr->second);
    }
    return WeakClient();
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
        tDLogE("TcpServer::accept() server is not a connection state.");
        return WeakClient();
    }

    Guard guard(_mutex);
    auto client = std::static_pointer_cast<TcpClient, Client>(createClient());
    if (auto shared = client->getClient().lock()) {
        uerr const CODE = _server->accept(*shared);
        if (CODE == uerr::UVPP_SUCCESS) {
            tDLogD("TcpServer::accept() client connect Sock({}:{})/Peer({}:{})",
                         shared->getSockIp(), shared->getSockPort(),
                         shared->getPeerIp(), shared->getPeerPort());
            bool const INSERT_RESULT = insertClient(client);
            assert(INSERT_RESULT);
            return WeakClient(client);
        } else {
            tDLogE("TcpServer::accept() {} error.", uvpp::getErrorName(CODE));
        }
    } else {
        tDLogE("TcpServer::accept() client is nullptr.");
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
        tDLogD("TcpServer::close() sync request.");
        Guard guard(_mutex);
        closeAll();
    } else {
        tDLogD("TcpServer::close() async request.");
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

