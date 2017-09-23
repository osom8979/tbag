/**
 * @file   StreamServer.cpp
 * @brief  StreamServer class implementation.
 * @author zer0
 * @date   2017-05-23
 */

#include <libtbag/network/stream/StreamServer.hpp>
#include <libtbag/network/stream/StreamClient.hpp>
#include <libtbag/network/stream/StreamServerBackend.hpp>
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

    using ClientMap  = StreamServer::ClientMap;
    using ClientPair = StreamServer::ClientPair;

    using AtomicBool = std::atomic_bool;

    StreamServer * _parent;

    std::string destination;
    int port;

    SharedServerBackend  server;
    SharedSafetyAsync    async;
    ClientMap            clients;

    AtomicBool on_connection;

    Internal(StreamServer * parent) : _parent(parent), destination(), port(0)
    {
        on_connection.store(false);
    }

    virtual ~Internal()
    {
        // EMPTY.
    }

    Loop & getLoop()
    {
        assert(static_cast<bool>(server));
        Loop * loop = server->getLoop();
        assert(loop != nullptr);
        return *loop;
    }

    Err initServer(StreamType type, std::string const & destination, int port = 0)
    {
        if (static_cast<bool>(server) == false) {
            return Err::E_EXPIRED;
        }

        using  TcpBackend = StreamServerBackend<uvpp::Tcp>;
        using PipeBackend = StreamServerBackend<uvpp::Pipe>;

        Err code = Err::E_UNKNOWN;
        if (type == StreamType::TCP) {
            auto backend = std::static_pointer_cast<TcpBackend>(server);
            code = uvpp::initCommonServer(*backend, destination, port);

            this->destination = backend->getSockIp();
            this->port = backend->getSockPort();

        } else if (type == StreamType::PIPE) {
            auto backend = std::static_pointer_cast<PipeBackend>(server);
            code = uvpp::initPipeServer(*backend, destination);

            this->destination = destination;
            this->port = 0;

        } else {
            code = Err::E_ILLARGS;
            this->destination.clear();
            this->port = 0;
            TBAG_INACCESSIBLE_BLOCK_ASSERT();
            tDLogA("StreamServer::Internal::initServer() Unknown stream type.");
        }

        return code;
    }

    Err initHandles()
    {
        Loop & loop = getLoop();
        async = loop.newHandle<SafetyAsync>(loop);
        assert(static_cast<bool>(async));
        return Err::E_SUCCESS;
    }

    SharedClient getSharedClient(Id id)
    {
        auto itr = clients.find(id);
        if (itr != clients.end()) {
            return itr->second;
        }
        return SharedClient();
    }

    bool insertClient(SharedClient client)
    {
        return clients.insert(ClientPair(client->id(), client)).second;
    }

    bool eraseClient(Id id)
    {
        return clients.erase(id) == 1U;
    }

    void closeAll()
    {
        // Close all clients.
        for (auto & cursor : clients) {
            if (static_cast<bool>(cursor.second)) {
                cursor.second->close();
            }
        }

        if (static_cast<bool>(async)) {
            if (async->isClosing() == false) {
                async->close();
            }
            async.reset();
        }

        assert(static_cast<bool>(server));
        if (server->isClosing() == false) {
            server->close();
        }

        destination.clear();
        port = 0;
    }
};

// -----------------------------------
// ClientIteratorGuard implementation.
// -----------------------------------

StreamServer::ClientIteratorGuard::ClientIteratorGuard(StreamServer & server)
        : _mutex(server._mutex),
          begin(server._internal->clients.begin()),
          end(server._internal->clients.end())
{
    _mutex.lock();
}

StreamServer::ClientIteratorGuard::~ClientIteratorGuard()
{
    _mutex.unlock();
}

// ----------------------------
// StreamServer implementation.
// ----------------------------

StreamServer::StreamServer(Loop & loop, StreamType type)
        : STREAM_TYPE(type), _internal(new Internal(this))
{
    assert(static_cast<bool>(_internal));

    using  TcpBackend = StreamServerBackend<uvpp::Tcp>;
    using PipeBackend = StreamServerBackend<uvpp::Pipe>;

    if (type == StreamType::TCP) {
        _internal->server = loop.newHandle<TcpBackend>(loop, this);
    } else if (type == StreamType::PIPE) {
        _internal->server = loop.newHandle<PipeBackend>(loop, this);
    } else {
        throw std::bad_alloc();
    }
    assert(static_cast<bool>(_internal->server));
}

StreamServer::~StreamServer()
{
    // EMPTY.
}

bool StreamServer::isOnConnection() const
{
    assert(static_cast<bool>(_internal));
    return _internal->on_connection.load();
}

bool StreamServer::emptyClients() const
{
    assert(static_cast<bool>(_internal));
    Guard const MUTEX_GUARD(_mutex);
    return _internal->clients.empty();
}

std::size_t StreamServer::sizeClients() const
{
    assert(static_cast<bool>(_internal));
    Guard const MUTEX_GUARD(_mutex);
    return _internal->clients.size();
}

StreamServer::WeakServerBackend StreamServer::getServer()
{
    assert(static_cast<bool>(_internal));
    Guard const MUTEX_GUARD(_mutex);
    return WeakServerBackend(_internal->server);
}

StreamServer::WeakSafetyAsync StreamServer::getAsync()
{
    assert(static_cast<bool>(_internal));
    Guard const MUTEX_GUARD(_mutex);
    return WeakSafetyAsync(_internal->async);
}

StreamServer::UniqueClientIteratorGuard StreamServer::getIterators()
{
    return UniqueClientIteratorGuard(new ClientIteratorGuard(*this));
}

std::string StreamServer::dest() const
{
    assert(static_cast<bool>(_internal));
    Guard const MUTEX_GUARD(_mutex);
    return _internal->destination;
}

int StreamServer::port() const
{
    assert(static_cast<bool>(_internal));
    Guard const MUTEX_GUARD(_mutex);
    return _internal->port;
}

Err StreamServer::init(char const * destination, int port)
{
    assert(static_cast<bool>(_internal));
    Guard const MUTEX_GUARD(_mutex);
    Err const CODE = _internal->initServer(STREAM_TYPE, destination, port);
    if (CODE != Err::E_SUCCESS) {
        return CODE;
    }
    return _internal->initHandles();
}

Err StreamServer::close()
{
    assert(static_cast<bool>(_internal));
    Guard const MUTEX_GUARD(_mutex);
    Loop & loop = _internal->getLoop();

    if (loop.isAliveAndThisThread() == false && static_cast<bool>(_internal->async)) {
        tDLogD("StreamServer::close() Async request.");
        _internal->async->newSendFunc([&]() {
            assert(static_cast<bool>(_internal));
            Guard const MUTEX_GUARD_ASYNC(_mutex);
            _internal->closeAll();
        });
        return Err::E_ASYNCREQ;
    }

    Err code = Err::E_SUCCESS;
    if (loop.isAliveAndThisThread() == false && static_cast<bool>(_internal->async) == false) {
        tDLogW("StreamServer::close() Async is expired.");
        code = Err::E_WARNING;
    }

    tDLogD("StreamServer::close() Synced request.");
    _internal->closeAll();
    return code;
}

StreamServer::WeakClient StreamServer::accept()
{
    assert(static_cast<bool>(_internal));
    assert(static_cast<bool>(_internal->server));
    if (_internal->on_connection.load() == false) {
        tDLogE("StreamServer::accept() server is not a connection state.");
        return WeakClient();
    }

    Guard const MUTEX_GUARD(_mutex);
    SharedStreamNode client = createClient(STREAM_TYPE, _internal->getLoop(), _internal->server);

    if (StreamClient::SharedClientBackend shared = client->getClient().lock()) {
        STATIC_ASSERT_CHECK_IS_BASE_OF(typename StreamClient::SharedClientBackend::element_type, uvpp::Stream);
        uvpp::Stream & uv_stream = *shared;

        Err const CODE = _internal->server->accept(uv_stream);
        if (CODE == Err::E_SUCCESS) {
            tDLogD("StreamServer::accept() client connect.");
            bool const INSERT_RESULT = _internal->insertClient(client);
            assert(INSERT_RESULT);

            WeakClient weak_client(client);
            {   // Allocate User Data.
                uv_stream.setUserData(onClientUdataAlloc(weak_client));
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
    Guard const MUTEX_GUARD(_mutex);
    return WeakClient(_internal->getSharedClient(id));
}

Err StreamServer::remove(Id id)
{
    assert(static_cast<bool>(_internal));
    Guard const MUTEX_GUARD(_mutex);
    return _internal->eraseClient(id) ? Err::E_SUCCESS : Err::E_UNKNOWN;
}

// --------------
// Event backend.
// --------------

void StreamServer::backConnection(Err code)
{
    assert(static_cast<bool>(_internal));
    _internal->on_connection.store(true);
    onConnection(code);
    _internal->on_connection.store(false);
}

void StreamServer::backClose()
{
    onServerClose();

    assert(static_cast<bool>(_internal));
    _mutex.lock();
    _internal->closeAll();
    _mutex.unlock();
}

// -----------------------
// StreamServer extension.
// -----------------------

StreamServer::SharedStreamNode StreamServer::createClient(StreamType type,
                                                          Loop & loop,
                                                          SharedServerBackend & server)
{
    return SharedStreamNode(new (std::nothrow) StreamNode(loop, type, this));
}

} // namespace stream
} // namespace network

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

