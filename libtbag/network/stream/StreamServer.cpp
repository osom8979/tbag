/**
 * @file   StreamServer.cpp
 * @brief  StreamServer class implementation.
 * @author zer0
 * @date   2017-05-23
 */

#include <libtbag/network/stream/StreamServer.hpp>
#include <libtbag/network/stream/StreamClient.hpp>
#include <libtbag/network/stream/StreamServerBackend.hpp>
#include <libtbag/network/stream/StreamNode.hpp>
#include <libtbag/debug/Assert.hpp>
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
    assert(static_cast<bool>(_server));
}

StreamServer::~StreamServer()
{
    // EMPTY.
}

// ------------------
// PROTECTED SECTION.
// ------------------

Err StreamServer::_initInternalHandles()
{
    assert(static_cast<bool>(_server));
    Loop * loop = _server->getLoop();
    assert(loop != nullptr);

    _async = loop->newHandle<SafetyAsync>(*loop);
    assert(static_cast<bool>(_async));

    return Err::E_SUCCESS;
}

StreamServer::SharedClient StreamServer::_createClient()
{
    assert(static_cast<bool>(_server));
    Loop * loop = _server->getLoop();
    assert(loop != nullptr);

    return SharedClient(new (std::nothrow) StreamNode(*loop, STREAM_TYPE, _async, this));
}

StreamServer::SharedClient StreamServer::_getSharedClient(Id id)
{
    auto itr = _clients.find(id);
    if (itr != _clients.end()) {
        return itr->second;
    }
    return SharedClient();
}

bool StreamServer::_insertClient(SharedClient client)
{
    return _clients.insert(ClientPair(client->id(), client)).second;
}

bool StreamServer::_eraseClient(Id id)
{
    return _clients.erase(id) == 1U;
}

void StreamServer::_closeAll()
{
    {   // Close all clients.
        for (auto & cursor : _clients) {
            if (static_cast<bool>(cursor.second)) {
                cursor.second->close();
            }
        }
    }

    if (static_cast<bool>(_async)) {
        if (_async->isClosing() == false) {
            _async->close();
        }
        _async.reset();
    }

    assert(static_cast<bool>(_server));
    if (_server->isClosing() == false) {
        _server->close();
    }

    _destination.clear();
    _port = 0;
}

std::string StreamServer::dest() const
{
    Guard guard(_mutex);
    return _destination;
}

int StreamServer::port() const
{
    Guard guard(_mutex);
    return _port;
}

Err StreamServer::init(char const * destination, int port)
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
    } else {
        TBAG_INACCESSIBLE_BLOCK_ASSERT();
        return Err::E_UNKNOWN;
    }

    if (is_init) {
        return _initInternalHandles();
    }
    return Err::E_UNKNOWN;
}

void StreamServer::close()
{
    assert(static_cast<bool>(_server));
    Loop * loop = _server->getLoop();
    assert(loop != nullptr);

    Guard guard(_mutex);
    if (loop->isAliveAndThisThread() || static_cast<bool>(_async) == false) {
        tDLogD("StreamServer::close() sync request.");
        _closeAll();
    } else {
        tDLogD("StreamServer::close() async request.");
        _async->newSendFunc([&]() {
            Guard guard(_mutex);
            _closeAll();
        });
    }
}

StreamServer::WeakClient StreamServer::accept()
{
    assert(static_cast<bool>(_server));
    if (_on_connection.load() == false) {
        tDLogE("StreamServer::accept() server is not a connection state.");
        return WeakClient();
    }

    Guard guard(_mutex);
    auto client = std::static_pointer_cast<StreamNode, ClientInterface>(_createClient());

   if (auto shared = client->getClient().lock()) {
        Err const CODE = _server->accept(*shared);
        if (CODE == Err::E_SUCCESS) {
            tDLogD("StreamServer::accept() client connect.");
            bool const INSERT_RESULT = _insertClient(client);
            assert(INSERT_RESULT);

            auto weak_client = WeakClient(client);
            {   // Allocate User Data.
                shared->setUserData(onClientUdataAlloc(weak_client));
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

StreamServer::WeakClient StreamServer::get(Id id)
{
    Guard guard(_mutex);
    return WeakClient(_getSharedClient(id));
}

Err StreamServer::remove(Id id)
{
    Guard guard(_mutex);
    return _eraseClient(id) ? Err::E_SUCCESS : Err::E_UNKNOWN;
}

// --------------
// Event backend.
// --------------

void StreamServer::backConnection(Err code)
{
    _on_connection.store(true);
    onConnection(code);
    _on_connection.store(false);
}

void StreamServer::backClose()
{
    onClose();

    _mutex.lock();
    _closeAll();
    _mutex.unlock();
}

} // namespace stream
} // namespace network

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

