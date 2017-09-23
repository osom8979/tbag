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
 *  - Do not use a mutex if the function name starts with '_'.
 */
class StreamServer::Internal : private Noncopyable
{
public:
    using SharedClient = StreamServer::SharedClient;
    using ClientMap    = StreamServer::ClientMap;
    using AtomicBool   = std::atomic_bool;

    using key_type        = typename ClientMap::key_type;
    using mapped_type     = typename ClientMap::mapped_type;
    using hasher          = typename ClientMap::hasher;
    using key_equal       = typename ClientMap::key_equal;
    using allocator_type  = typename ClientMap::allocator_type;
    using value_type      = typename ClientMap::value_type;
    using reference       = typename ClientMap::reference;
    using const_reference = typename ClientMap::const_reference;
    using pointer         = typename ClientMap::pointer;
    using const_pointer   = typename ClientMap::const_pointer;
    using size_type       = typename ClientMap::size_type;
    using difference_type = typename ClientMap::difference_type;
    using iterator        = typename ClientMap::iterator;
    using const_iterator  = typename ClientMap::const_iterator;

private:
    StreamType const STREAM_TYPE;

private:
    StreamServer * _parent;

private:
    std::string _destination;
    int _port;

private:
    SharedServerBackend  _server;
    SharedSafetyAsync    _async;
    ClientMap            _clients;

private:
    mutable Mutex _mutex;
    AtomicBool _on_connection;

public:
    Internal(StreamServer * parent, Loop & loop, StreamType type)
            : STREAM_TYPE(type), _parent(parent), _destination(), _port(0), _on_connection(false)
    {
        using  TcpBackend = StreamServerBackend<uvpp::Tcp>;
        using PipeBackend = StreamServerBackend<uvpp::Pipe>;

        if (type == StreamType::TCP) {
            _server = loop.newHandle<TcpBackend>(loop, _parent);
        } else if (type == StreamType::PIPE) {
            _server = loop.newHandle<PipeBackend>(loop, _parent);
        } else {
            throw std::bad_alloc();
        }
        assert(static_cast<bool>(_server));
    }

    ~Internal()
    {
        // EMPTY.
    }

public:
    inline StreamType getStreamType() const TBAG_NOEXCEPT { return STREAM_TYPE; }

    inline Mutex       & atMutex()       TBAG_NOEXCEPT { return _mutex; }
    inline Mutex const & atMutex() const TBAG_NOEXCEPT { return _mutex; }

    WeakServerBackend getServer() { Guard g(_mutex); return WeakServerBackend(_server); }
    WeakSafetyAsync   getAsync () { Guard g(_mutex); return WeakSafetyAsync  ( _async); }

    inline std::string dest() const { Guard g(_mutex); return _destination; }
    inline int         port() const { Guard g(_mutex); return        _port; }

public:
    inline bool isOnConnection() const TBAG_NOEXCEPT_SP_OP(_on_connection.load())
    { return _on_connection.load(); }

public:
    inline       iterator    _begin()       TBAG_NOEXCEPT_SP_OP(_clients.   begin()) { return _clients.   begin(); }
    inline const_iterator    _begin() const TBAG_NOEXCEPT_SP_OP(_clients.   begin()) { return _clients.   begin(); }
    inline       iterator      _end()       TBAG_NOEXCEPT_SP_OP(_clients.     end()) { return _clients.     end(); }
    inline const_iterator      _end() const TBAG_NOEXCEPT_SP_OP(_clients.     end()) { return _clients.     end(); }
    inline const_iterator   _cbegin() const TBAG_NOEXCEPT_SP_OP(_clients.  cbegin()) { return _clients.  cbegin(); }
    inline const_iterator     _cend() const TBAG_NOEXCEPT_SP_OP(_clients.    cend()) { return _clients.    cend(); }
    inline           bool    _empty() const TBAG_NOEXCEPT_SP_OP(_clients.   empty()) { return _clients.   empty(); }
    inline      size_type     _size() const TBAG_NOEXCEPT_SP_OP(_clients.    size()) { return _clients.    size(); }
    inline      size_type _max_size() const TBAG_NOEXCEPT_SP_OP(_clients.max_size()) { return _clients.max_size(); }

public:
    inline      bool empty() const TBAG_NOEXCEPT_SP_OP(_clients.empty()) { Guard g(_mutex); return _clients.empty(); }
    inline size_type  size() const TBAG_NOEXCEPT_SP_OP(_clients. size()) { Guard g(_mutex); return _clients.size (); }

public:
    WeakClient getWeakClient(Id id)
    {
        Guard const LOCK(_mutex);
        auto itr = _clients.find(id);
        if (itr != _clients.end()) {
            return WeakClient(itr->second);
        }
        return WeakClient();
    }

    bool eraseClient(Id id)
    {
        Guard const LOCK(_mutex);
        return _clients.erase(id) == 1U;
    }

    Err initServer(StreamType type, std::string const & destination, int port = 0)
    {
        Guard const LOCK(_mutex);
        if (static_cast<bool>(_server) == false) {
            return Err::E_EXPIRED;
        }

        using  TcpBackend = StreamServerBackend<uvpp::Tcp>;
        using PipeBackend = StreamServerBackend<uvpp::Pipe>;

        Err code = Err::E_UNKNOWN;
        if (type == StreamType::TCP) {
            auto backend = std::static_pointer_cast<TcpBackend>(_server);
            code = uvpp::initCommonServer(*backend, destination, port);
            _destination = backend->getSockIp();
            _port = backend->getSockPort();

        } else if (type == StreamType::PIPE) {
            auto backend = std::static_pointer_cast<PipeBackend>(_server);
            code = uvpp::initPipeServer(*backend, destination);
            _destination = destination;
            _port = 0;

        } else {
            code = Err::E_ILLARGS;
            _destination.clear();
            _port = 0;

            TBAG_INACCESSIBLE_BLOCK_ASSERT();
            tDLogA("StreamServer::Internal::initServer() Unknown stream type.");
        }

        return code;
    }

    Err initInternalHandles()
    {
        Guard const LOCK(_mutex);
        Loop & loop = _getLoop();
        _async = loop.newInternalHandle<SafetyAsync>(true, loop);
        assert(static_cast<bool>(_async));
        return Err::E_SUCCESS;
    }

    Err close()
    {
        Guard const LOCK(_mutex);
        Loop & loop = _getLoop();

        if (_async && loop.isAliveAndThisThread() == false) {
            tDLogD("StreamServer::Internal::close() Async request.");
            auto job = _async->newSendFunc([&](){
                _closeServer();
            });
            if (static_cast<bool>(job)) {
                return Err::E_ASYNCREQ;
            }
            return Err::E_ESEND;
        }

        _closeServer();

        Err result_code = Err::E_SUCCESS;
        if (!_async && loop.isAliveAndThisThread() == false) {
            tDLogW("StreamServer::Internal::close() Async is expired.");
            result_code = Err::E_WARNING;
        }

        tDLogD("StreamServer::Internal::close() Synced request.");
        return result_code;
    }

    WeakClient accept()
    {
        if (_on_connection == false) {
            tDLogE("StreamServer::Internal::accept() server is not a connection state.");
            return WeakClient();
        }

        Guard const LOCK(_mutex);
        assert(_parent != nullptr);
        assert(static_cast<bool>(_server));

        SharedStreamNode client = _parent->createClient(STREAM_TYPE, _getLoop(), _server);

        if (StreamClient::SharedClientBackend shared = client->getClient().lock()) {
            STATIC_ASSERT_CHECK_IS_BASE_OF(typename StreamClient::SharedClientBackend::element_type, uvpp::Stream);
            uvpp::Stream & uv_stream = *shared;

            Err const CODE = _server->accept(uv_stream);
            if (CODE == Err::E_SUCCESS) {
                tDLogD("StreamServer::Internal::accept() client connect.");
                bool const INSERT_RESULT = _clients.insert(value_type(client->id(), client)).second;
                assert(INSERT_RESULT);

                WeakClient weak_client(client);
                uv_stream.setUserData(_parent->onClientUdataAlloc(weak_client)); // Allocate User Data.
                return weak_client;
            } else {
                tDLogE("StreamServer::Internal::accept() {} error.", getErrName(CODE));
            }
        } else {
            tDLogE("StreamServer::Internal::accept() client is nullptr.");
        }
        return WeakClient();
    }

private:
    Loop & _getLoop()
    {
        assert(static_cast<bool>(_server));
        Loop * loop = _server->getLoop();
        assert(loop != nullptr);
        return *loop;
    }

    void _closeServer()
    {
        // Close all clients.
        for (auto & cursor : _clients) {
            if (static_cast<bool>(cursor.second)) {
                cursor.second->close();
            }
        }

        assert(static_cast<bool>(_server));
        if (_server->isClosing() == false) {
            _server->close();
        }
    }

    void _closeInternal()
    {
        if (_async && _async->isClosing() == false) {
            _async->close();
        }
    }

public:
    void preConnection(Err code)
    {
        _on_connection.store(true);
    }

    void postConnection(Err code)
    {
        _on_connection.store(false);
    }

    void preClose()
    {
        Guard const LOCK(_mutex);
        _closeInternal();
    }

    void postClose()
    {
        // EMPTY.
    }
};

// -----------------------------------
// ClientIteratorGuard implementation.
// -----------------------------------

StreamServer::ClientIteratorGuard::ClientIteratorGuard(StreamServer * parent) : _parent(parent)
{
    assert(_parent != nullptr);
    assert(static_cast<bool>(_parent->_internal));
    _parent->_internal->atMutex().lock();
}

StreamServer::ClientIteratorGuard::~ClientIteratorGuard()
{
    assert(_parent != nullptr);
    assert(static_cast<bool>(_parent->_internal));
    _parent->_internal->atMutex().unlock();
}

StreamServer::iterator StreamServer::ClientIteratorGuard::begin()
{
    assert(_parent != nullptr);
    assert(static_cast<bool>(_parent->_internal));
    return _parent->_internal->_begin();
}

StreamServer::iterator StreamServer::ClientIteratorGuard::end()
{
    assert(_parent != nullptr);
    assert(static_cast<bool>(_parent->_internal));
    return _parent->_internal->_end();
}

StreamServer::const_iterator StreamServer::ClientIteratorGuard::begin() const
{
    assert(_parent != nullptr);
    assert(static_cast<bool>(_parent->_internal));
    return _parent->_internal->_begin();
}

StreamServer::const_iterator StreamServer::ClientIteratorGuard::end() const
{
    assert(_parent != nullptr);
    assert(static_cast<bool>(_parent->_internal));
    return _parent->_internal->_end();
}

StreamServer::const_iterator StreamServer::ClientIteratorGuard::cbegin() const
{
    assert(_parent != nullptr);
    assert(static_cast<bool>(_parent->_internal));
    return _parent->_internal->_cbegin();
}

StreamServer::const_iterator StreamServer::ClientIteratorGuard::cend() const
{
    assert(_parent != nullptr);
    assert(static_cast<bool>(_parent->_internal));
    return _parent->_internal->_cend();
}

bool StreamServer::ClientIteratorGuard::empty() const
{
    assert(_parent != nullptr);
    assert(static_cast<bool>(_parent->_internal));
    return _parent->_internal->_empty();
}

StreamServer::size_type StreamServer::ClientIteratorGuard::size() const
{
    assert(_parent != nullptr);
    assert(static_cast<bool>(_parent->_internal));
    return _parent->_internal->_size();
}

StreamServer::size_type StreamServer::ClientIteratorGuard::max_size() const
{
    assert(_parent != nullptr);
    assert(static_cast<bool>(_parent->_internal));
    return _parent->_internal->_max_size();
}

// ----------------------------
// StreamServer implementation.
// ----------------------------

StreamServer::StreamServer(Loop & loop, StreamType type)
        : _internal(new Internal(this, loop, type))
{
    assert(static_cast<bool>(_internal));
}

StreamServer::~StreamServer()
{
    // EMPTY.
}

bool StreamServer::isOnConnection() const
{
    assert(static_cast<bool>(_internal));
    return _internal->isOnConnection();
}

bool StreamServer::isEmptyOfClients() const
{
    assert(static_cast<bool>(_internal));
    return _internal->empty();
}

std::size_t StreamServer::sizeOfClients() const
{
    assert(static_cast<bool>(_internal));
    return _internal->size();
}

StreamServer::WeakServerBackend StreamServer::getServer()
{
    assert(static_cast<bool>(_internal));
    return _internal->getServer();
}

StreamServer::WeakSafetyAsync StreamServer::getAsync()
{
    assert(static_cast<bool>(_internal));
    return _internal->getAsync();
}

StreamServer::UniqueClientIteratorGuard StreamServer::getIterators()
{
    return UniqueClientIteratorGuard(new ClientIteratorGuard(this));
}

std::string StreamServer::dest() const
{
    assert(static_cast<bool>(_internal));
    return _internal->dest();
}

int StreamServer::port() const
{
    assert(static_cast<bool>(_internal));
    return _internal->port();
}

Err StreamServer::init(char const * destination, int port)
{
    assert(static_cast<bool>(_internal));
    Err const CODE = _internal->initServer(_internal->getStreamType(), destination, port);
    if (TBAG_ERR_FAILURE(CODE)) {
        return CODE;
    }
    return _internal->initInternalHandles();
}

Err StreamServer::close()
{
    assert(static_cast<bool>(_internal));
    return _internal->close();
}

StreamServer::WeakClient StreamServer::accept()
{
    assert(static_cast<bool>(_internal));
    return _internal->accept();
}

StreamServer::WeakClient StreamServer::get(Id id)
{
    assert(static_cast<bool>(_internal));
    return _internal->getWeakClient(id);
}

Err StreamServer::remove(Id id)
{
    assert(static_cast<bool>(_internal));
    return _internal->eraseClient(id) ? Err::E_SUCCESS : Err::E_UNKNOWN;
}

// --------------
// Event backend.
// --------------

void StreamServer::backConnection(Err code)
{
    assert(static_cast<bool>(_internal));
    _internal->preConnection(code);
    onConnection(code);
    _internal->postConnection(code);
}

void StreamServer::backClose()
{
    assert(static_cast<bool>(_internal));
    _internal->preClose();
    onServerClose();
    _internal->postClose();
}

// -----------------------
// StreamServer extension.
// -----------------------

StreamServer::SharedStreamNode StreamServer::createClient(StreamType type, Loop & loop, SharedServerBackend & server)
{
    return SharedStreamNode(new (std::nothrow) StreamNode(loop, type, this));
}

} // namespace stream
} // namespace network

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

