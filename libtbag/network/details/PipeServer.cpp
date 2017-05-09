/**
 * @file   PipeServer.cpp
 * @brief  PipeServer class implementation.
 * @author zer0
 * @date   2017-05-09
 */

#include <libtbag/network/details/PipeServer.hpp>
#include <libtbag/filesystem/Path.hpp>
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
// PipeRealNode implementation.
// ---------------------------

PipeClientNode::PipeClientNode(Loop & loop, PipeServer & parent)
        : PipeClient(loop), _parent(parent)
{
    // EMPTY.
}

PipeClientNode::~PipeClientNode()
{
    // EMPTY.
}

Server::WeakClient PipeClientNode::getWeakClient()
{
    Guard guard(_parent._mutex);
    return _parent.getWeakClient(getId());
}

void PipeClientNode::onShutdown(uerr code)
{
    _parent.onClientShutdown(getWeakClient(), code);
}

void PipeClientNode::onWrite(uerr code)
{
    _parent.onClientWrite(getWeakClient(), code);
}

void PipeClientNode::onRead(uerr code, char const * buffer, Size size)
{
    _parent.onClientRead(getWeakClient(), code, buffer, size);
}

void PipeClientNode::onClose()
{
    _parent.onClientClose(getWeakClient());
}

void * PipeClientNode::onUserDataAlloc()
{
    return _parent.onClientUserDataAlloc(getWeakClient());
}

void PipeClientNode::onUserDataDealloc(void * data)
{
    _parent.onClientUserDataDealloc(getWeakClient(), data);
}

// -----------------------------
// PipeRealServer implementation.
// -----------------------------

PipeRealServer::PipeRealServer(Loop & loop, PipeServer & parent) : Pipe(loop), _parent(parent)
{
    _on_connection.store(false);
    setUserData(_parent.onUserDataAlloc());
}

PipeRealServer::~PipeRealServer()
{
    _parent.onUserDataDealloc(getUserData());
}

bool PipeRealServer::init(String const & path, int UNUSED_PARAM(unused))
{
    uerr const BIND_CODE = bind(path.c_str());
    if (BIND_CODE != uerr::UVPP_SUCCESS) {
        tDLogE("PipeRealServer::init() pipe bind {} error.", getErrorName(BIND_CODE));
        return false;
    }

    uerr const LISTEN_CODE = listen();
    if (LISTEN_CODE != uerr::UVPP_SUCCESS) {
        tDLogE("PipeRealServer::init() pipe listen {} error.", getErrorName(LISTEN_CODE));
        return false;
    }

    return true;
}

void PipeRealServer::onConnection(uerr code)
{
    _on_connection.store(true);
    _parent.onConnection(code);
    _on_connection.store(false);
}

void PipeRealServer::onClose()
{
    _parent.onServerClose();

    {   // Update parent state.
        Guard guard(_parent._mutex);
        _parent.closeAll();
    }
}

// -------------------------
// PipeServer implementation.
// -------------------------

PipeServer::PipeServer(Loop & loop)
{
    _server = loop.newHandle<PipeRealServer>(loop, *this);
    _async  = loop.newHandle<SafetyWriteAsync>(loop);
    assert(static_cast<bool>(_server));
    assert(static_cast<bool>(_async));
}

PipeServer::~PipeServer()
{
    _server.reset();
    _async.reset();
}

PipeServer::SharedClient PipeServer::createClient(Loop & loop, PipeServer & server)
{
    SharedClient client(new (std::nothrow) PipeClientNode(loop, server));
    if (static_cast<bool>(client)) {
        return client;
    }
    return SharedClient();
}

PipeServer::SharedClient PipeServer::createClient()
{
    assert(static_cast<bool>(_server));

    Loop * loop = _server->getLoop();
    assert(loop != nullptr);

    return createClient(*loop, *this);
}

PipeServer::SharedClient PipeServer::getSharedClient(ClientKey key)
{
    auto itr = _clients.find(key);
    if (itr != _clients.end()) {
        return itr->second;
    }
    return SharedClient();
}

PipeServer::WeakClient PipeServer::getWeakClient(ClientKey key)
{
    auto itr = _clients.find(key);
    if (itr != _clients.end()) {
        return WeakClient(itr->second);
    }
    return WeakClient();
}

bool PipeServer::insertClient(SharedClient client)
{
    return _clients.insert(ClientPair(client->getId(), client)).second;
}

bool PipeServer::removeClient(ClientKey key)
{
    return _clients.erase(key) == 1U;
}

void PipeServer::closeAll()
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

bool PipeServer::init(String const & path, int UNUSED_PARAM(unused))
{
    assert(static_cast<bool>(_server));
    Guard guard(_mutex);
    return _server->init(path);
}

PipeServer::WeakClient PipeServer::accept()
{
    assert(static_cast<bool>(_server));
    if (_server->isOnConnection() == false) {
        tDLogE("PipeServer::accept() server is not a connection state.");
        return WeakClient();
    }

    Guard guard(_mutex);
    auto client = std::static_pointer_cast<PipeClient, Client>(createClient());
    if (auto shared = client->getClient().lock()) {
        uerr const CODE = _server->accept(*shared);
        if (CODE == uerr::UVPP_SUCCESS) {
            tDLogD("PipeServer::accept() client connect.");
            bool const INSERT_RESULT = insertClient(client);
            assert(INSERT_RESULT);
            return WeakClient(client);
        } else {
            tDLogE("PipeServer::accept() {} error.", uvpp::getErrorName(CODE));
        }
    } else {
        tDLogE("PipeServer::accept() client is nullptr.");
    }
    return WeakClient();
}

void PipeServer::close()
{
    assert(static_cast<bool>(_server));
    assert(static_cast<bool>(_async));

    Loop * loop = _server->getLoop();
    assert(loop != nullptr);

    if (loop->isAliveAndThisThread()) {
        tDLogD("PipeServer::close() sync request.");
        Guard guard(_mutex);
        closeAll();
    } else {
        tDLogD("PipeServer::close() async request.");
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

