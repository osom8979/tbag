/**
 * @file   StreamServer.cpp
 * @brief  StreamServer class implementation.
 * @author zer0
 * @date   2017-05-23
 */

#include <libtbag/network/stream/StreamServer.hpp>
#include <libtbag/network/stream/StreamClient.hpp>
#include <libtbag/network/stream/StreamServerBackend.hpp>
#include <libtbag/network/stream/StreamServerNode.hpp>
#include <libtbag/log/Log.hpp>

#include <cassert>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace network {
namespace stream  {

StreamServer::StreamServer(Loop & loop, StreamType type) : STREAM_TYPE(type), _port(0)
{
    _on_connection.store(false);

    using  TcpBackend = StreamServerBackend<uvpp::Tcp>;
    using PipeBackend = StreamServerBackend<uvpp::Pipe>;

    if (type == StreamType::TCP) {
        _server = loop.newHandle<TcpBackend>(loop, this);
    } else if (type == StreamType::PIPE) {
        _server = loop.newHandle<PipeBackend>(loop, this);
    } else {
        throw std::bad_alloc();
    }

    _async = loop.newHandle<SafetyWriteAsync>(loop);

    assert(static_cast<bool>(_server));
    assert(static_cast<bool>(_async));
}

StreamServer::~StreamServer()
{
    _server.reset();
    _async.reset();
}

StreamServer::WeakServerBackend StreamServer::getServer()
{
    Guard g(_mutex);
    return WeakServerBackend(_server);
}

StreamServer::WeakSafetyWriteAsync StreamServer::getAsync()
{
    Guard g(_mutex);
    return WeakSafetyWriteAsync(_async);
}

StreamServer::SharedClient StreamServer::createClient()
{
    assert(static_cast<bool>(_server));

    Loop * loop = _server->getLoop();
    assert(loop != nullptr);

    return SharedClient(new (std::nothrow) StreamServerNode(*loop, STREAM_TYPE, this));
}

StreamServer::SharedClient StreamServer::getSharedClient(Id id)
{
    auto itr = _clients.find(id);
    if (itr != _clients.end()) {
        return itr->second;
    }
    return SharedClient();
}

bool StreamServer::insertClient(SharedClient client)
{
    return _clients.insert(ClientPair(client->getId(), client)).second;
}

bool StreamServer::removeClient(Id id)
{
    return _clients.erase(id) == 1U;
}

void StreamServer::closeAll()
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

    _destination.clear();
    _port = 0;
}

bool StreamServer::init(char const * destination, int port)
{
    assert(static_cast<bool>(_server));
    Guard guard(_mutex);

    using  TcpBackend = StreamServerBackend<uvpp::Tcp>;
    using PipeBackend = StreamServerBackend<uvpp::Pipe>;

    bool is_init = false;
    if (STREAM_TYPE == StreamType::TCP) {
        auto backend = std::static_pointer_cast<TcpBackend>(_server);
        is_init = uvpp::initCommonServer(*backend, destination, port);

        _destination = backend->getSockIp();
        _port = backend->getSockPort();

    } else if (STREAM_TYPE == StreamType::PIPE) {
        auto backend = std::static_pointer_cast<PipeBackend>(_server);
        is_init = uvpp::initPipeServer(*backend, destination);

        _destination = destination;
        _port = 0;
    }

    return is_init;
}

std::string StreamServer::getDestination() const
{
    return _destination;
}

int StreamServer::getPort() const
{
    return _port;
}

void StreamServer::close()
{
    assert(static_cast<bool>(_server));
    assert(static_cast<bool>(_async));

    Loop * loop = _server->getLoop();
    assert(loop != nullptr);

    if (loop->isAliveAndThisThread()) {
        tDLogD("StreamServer::close() sync request.");
        Guard guard(_mutex);
        closeAll();
    } else {
        tDLogD("StreamServer::close() async request.");
        _async->newSendFunc([&]() {
            Guard guard(_mutex);
            closeAll();
        });
    }
}

StreamServer::WeakClient StreamServer::accept()
{
    assert(static_cast<bool>(_server));
    if (isOnConnection() == false) {
        tDLogE("StreamServer::accept() server is not a connection state.");
        return WeakClient();
    }

    Guard guard(_mutex);
    auto client = std::static_pointer_cast<StreamServerNode, ClientInterface>(createClient());

    if (auto shared = client->getClient().lock()) {
        Err const CODE = _server->accept(*shared);
        if (CODE == Err::E_SUCCESS) {
            tDLogD("StreamServer::accept() client connect.");
            bool const INSERT_RESULT = insertClient(client);
            assert(INSERT_RESULT);

            auto weak_client = WeakClient(client);
            {   // Allocate User Data.
                shared->setUserData(this->onClientUserDataAlloc(weak_client));
            }
            return weak_client;
        } else {
            tDLogE("StreamServer::accept() {} error.", getErrName(CODE));
        }
    } else {
        tDLogE("StreamServer::accept() client is nullptr.");
    }
    return WeakClient();
}

StreamServer::WeakClient StreamServer::getClient(Id id)
{
    return WeakClient(getSharedClient(id));
}

void StreamServer::runBackendConnection(Err code)
{
    _on_connection.store(true);
    onConnection(code);
    _on_connection.store(false);
}

void StreamServer::runBackendClose()
{
    onServerClose();

    _mutex.lock();
    closeAll();
    _mutex.unlock();
}

} // namespace stream
} // namespace network

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

