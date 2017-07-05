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
#include <libtbag/Noncopyable.hpp>

#include <cassert>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace network {
namespace stream  {

/**
 * Stream internal helper class.
 *
 * @author zer0
 * @date   2017-06-16
 *
 * @warning
 *  Don't mutex guard in this class. (However, callbacks are an exception)
 */
struct StreamServer::Internal : private Noncopyable
{
    using SharedClient = StreamServer::SharedClient;

    StreamServer * _parent;

    Internal(StreamServer * parent) : _parent(parent)
    {
        // EMPTY.
    }

    ~Internal()
    {
        // EMPTY.
    }

    Err initHandles()
    {
        assert(_parent != nullptr);
        StreamServer & p = *_parent;

        assert(static_cast<bool>(p._server));
        Loop * loop = p._server->getLoop();
        assert(loop != nullptr);

        p._async = loop->newHandle<SafetyAsync>(*loop);
        assert(static_cast<bool>(p._async));

        return Err::E_SUCCESS;
    }

    SharedClient createClient()
    {
        assert(_parent != nullptr);
        StreamServer & p = *_parent;

        assert(static_cast<bool>(p._server));
        Loop * loop = p._server->getLoop();
        assert(loop != nullptr);

        return SharedClient(new (std::nothrow) StreamNode(*loop, p.STREAM_TYPE, p._async, _parent));
    }

    SharedClient getSharedClient(Id id)
    {
        assert(_parent != nullptr);
        StreamServer & p = *_parent;

        auto itr = p._clients.find(id);
        if (itr != p._clients.end()) {
            return itr->second;
        }
        return SharedClient();
    }

    bool insertClient(SharedClient client)
    {
        assert(_parent != nullptr);
        StreamServer & p = *_parent;
        return p._clients.insert(ClientPair(client->id(), client)).second;
    }

    bool eraseClient(Id id)
    {
        assert(_parent != nullptr);
        StreamServer & p = *_parent;
        return p._clients.erase(id) == 1U;
    }

    void closeAll()
    {
        assert(_parent != nullptr);
        StreamServer & p = *_parent;

        {   // Close all clients.
            for (auto & cursor : p._clients) {
                if (static_cast<bool>(cursor.second)) {
                    cursor.second->close();
                }
            }
        }

        if (static_cast<bool>(p._async)) {
            if (p._async->isClosing() == false) {
                p._async->close();
            }
            p._async.reset();
        }

        assert(static_cast<bool>(p._server));
        if (p._server->isClosing() == false) {
            p._server->close();
        }

        p._destination.clear();
        p._port = 0;
    }
};

// ----------------------------
// StreamServer implementation.
// ----------------------------

StreamServer::StreamServer(Loop & loop, StreamType type)
        : STREAM_TYPE(type), _internal(new Internal(this)), _destination(), _port(0)
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
    assert(static_cast<bool>(_internal));
    Guard guard(_mutex);

    using  TcpBackend = StreamServerBackend<uvpp::Tcp>;
    using PipeBackend = StreamServerBackend<uvpp::Pipe>;

    Err code = Err::E_UNKNOWN;
    if (STREAM_TYPE == StreamType::TCP) {
        auto backend = std::static_pointer_cast<TcpBackend>(_server);
        code = uvpp::initCommonServer(*backend, destination, port);

        _destination = backend->getSockIp();
        _port = backend->getSockPort();

    } else if (STREAM_TYPE == StreamType::PIPE) {
        auto backend = std::static_pointer_cast<PipeBackend>(_server);
        code = uvpp::initPipeServer(*backend, destination);

        _destination = destination;
        _port = 0;
    } else {
        TBAG_INACCESSIBLE_BLOCK_ASSERT();
        _destination.clear();
        _port = 0;
        return Err::E_UNKNOWN;
    }

    if (code == Err::E_SUCCESS) {
        return _internal->initHandles();
    }
    return Err::E_UNKNOWN;
}

void StreamServer::close()
{
    assert(static_cast<bool>(_server));
    assert(static_cast<bool>(_internal));
    Loop * loop = _server->getLoop();
    assert(loop != nullptr);

    Guard guard(_mutex);
    if (loop->isAliveAndThisThread() || static_cast<bool>(_async) == false) {
        tDLogD("StreamServer::close() sync request.");
        _internal->closeAll();
    } else {
        tDLogD("StreamServer::close() async request.");
        _async->newSendFunc([&]() {
            Guard const MUTEX_GUARD(_mutex);
            _internal->closeAll();
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
    auto client = std::static_pointer_cast<StreamNode, ClientInterface>(_internal->createClient());

   if (auto shared = client->getClient().lock()) {
        Err const CODE = _server->accept(*shared);
        if (CODE == Err::E_SUCCESS) {
            tDLogD("StreamServer::accept() client connect.");
            bool const INSERT_RESULT = _internal->insertClient(client);
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
    assert(static_cast<bool>(_internal));
    Guard guard(_mutex);
    return WeakClient(_internal->getSharedClient(id));
}

Err StreamServer::remove(Id id)
{
    assert(static_cast<bool>(_internal));
    Guard guard(_mutex);
    return _internal->eraseClient(id) ? Err::E_SUCCESS : Err::E_UNKNOWN;
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
    assert(static_cast<bool>(_internal));
    onClose();

    _mutex.lock();
    _internal->closeAll();
    _mutex.unlock();
}

} // namespace stream
} // namespace network

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

