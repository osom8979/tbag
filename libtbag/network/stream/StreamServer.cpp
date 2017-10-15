/**
 * @file   StreamServer.cpp
 * @brief  StreamServer class implementation.
 * @author zer0
 * @date   2017-05-23
 */

#include <libtbag/network/stream/StreamServer.hpp>
#include <libtbag/log/Log.hpp>
#include <libtbag/debug/Assert.hpp>
#include <libtbag/Type.hpp>

#include <libtbag/network/stream/StreamClient.hpp>
#include <libtbag/network/stream/StreamServerBackend.hpp>

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
 */
struct StreamServer::Internal : public details::ServerProperty
{
    StreamType const STREAM_TYPE;
    StreamServer * parent;
    Loop & loop;

    Internal(StreamServer * p, Loop & l, StreamType type)
            : STREAM_TYPE(type), parent(p), loop(l)
    {
        using  TcpBackend = StreamServerBackend<uvpp::Tcp>;
        using PipeBackend = StreamServerBackend<uvpp::Pipe>;

        if (type == StreamType::TCP) {
            server_stream = loop.newHandle<TcpBackend>(loop, parent);
        } else if (type == StreamType::PIPE) {
            server_stream = loop.newHandle<PipeBackend>(loop, parent);
        } else {
            throw std::bad_alloc();
        }

        assert(static_cast<bool>(server_stream));
    }

    ~Internal()
    {
        // EMPTY.
    }

public:
    // @formatter:off
    inline       iterator    begin()       TBAG_NOEXCEPT_SP_OP(clients.   begin()) { return clients.   begin(); }
    inline const_iterator    begin() const TBAG_NOEXCEPT_SP_OP(clients.   begin()) { return clients.   begin(); }
    inline       iterator      end()       TBAG_NOEXCEPT_SP_OP(clients.     end()) { return clients.     end(); }
    inline const_iterator      end() const TBAG_NOEXCEPT_SP_OP(clients.     end()) { return clients.     end(); }
    inline const_iterator   cbegin() const TBAG_NOEXCEPT_SP_OP(clients.  cbegin()) { return clients.  cbegin(); }
    inline const_iterator     cend() const TBAG_NOEXCEPT_SP_OP(clients.    cend()) { return clients.    cend(); }
    inline           bool    empty() const TBAG_NOEXCEPT_SP_OP(clients.   empty()) { return clients.   empty(); }
    inline      size_type     size() const TBAG_NOEXCEPT_SP_OP(clients.    size()) { return clients.    size(); }
    inline      size_type max_size() const TBAG_NOEXCEPT_SP_OP(clients.max_size()) { return clients.max_size(); }
    // @formatter:on

public:
    WeakClient getWeakClient(Id id)
    {
        auto itr = clients.find(id);
        if (itr != clients.end()) {
            return WeakClient(itr->second);
        }
        return WeakClient();
    }

    bool eraseClient(Id id)
    {
        return clients.erase(id) == 1U;
    }

public:
    Err initServer(std::string const & destination, int port = 0)
    {
        if (static_cast<bool>(server_stream) == false) {
            return Err::E_EXPIRED;
        }

        using  TcpBackend = StreamServerBackend<uvpp::Tcp>;
        using PipeBackend = StreamServerBackend<uvpp::Pipe>;

        if (STREAM_TYPE == StreamType::TCP) {
            auto backend = std::static_pointer_cast<TcpBackend>(server_stream);
            return uvpp::initCommonServer(*backend, destination, port);

        } else if (STREAM_TYPE == StreamType::PIPE) {
            auto backend = std::static_pointer_cast<PipeBackend>(server_stream);
            return uvpp::initPipeServer(*backend, destination);
        }

        tDLogA("StreamServer::Internal::initServer() Unknown stream type: {}", static_cast<int>(STREAM_TYPE));
        TBAG_INACCESSIBLE_BLOCK_ASSERT();
        return Err::E_ILLARGS;
    }

    Err initInternalHandles()
    {
        safety_async = loop.newInternalHandle<SafetyAsync>(true, loop);
        if (safety_async) {
            return Err::E_SUCCESS;
        }
        return Err::E_BADALLOC;
    }

    WeakClient accept()
    {
        if (on_connection == false) {
            tDLogE("StreamServer::Internal::accept() server is not a connection state.");
            return WeakClient();
        }

        if (cur_connection_count + 1 >= max_connection_count) {
            tDLogE("StreamServer::Internal::accept() The connection is full ({})", max_connection_count);
            return WeakClient();
        }

        assert(parent != nullptr);
        assert(static_cast<bool>(server_stream));

        SharedStreamNode client = parent->createClient(STREAM_TYPE, loop, server_stream);
        auto shared = client->getClient().lock();

        if (static_cast<bool>(shared) == false) {
            tDLogE("StreamServer::Internal::accept() Expired client.");
            return WeakClient();
        }

        STATIC_ASSERT_CHECK_IS_BASE_OF(typename StreamClient::SharedStream::element_type, uvpp::Stream);
        uvpp::Stream & uv_stream = *shared;

        Err const CODE = server_stream->accept(uv_stream);
        if (isFailure(CODE)) {
            tDLogE("StreamServer::Internal::accept() Accept {} error.", getErrName(CODE));
            return WeakClient();
        }

        tDLogD("StreamServer::Internal::accept() client connect.");
        bool const INSERT_RESULT = clients.insert(value_type(client->id(), client)).second;
        if (INSERT_RESULT == false) {
            tDLogA("StreamServer::Internal::accept() Insert failure.");
        }

        ++cur_connection_count;
        tDLogI("StreamServer::Internal::accept() New accept ({}/{})", cur_connection_count, max_connection_count);
        return WeakClient(client);
    }

    Err closeServer()
    {
        // Close all clients.
        for (auto & cursor : clients) {
            if (static_cast<bool>(cursor.second)) {
                cursor.second->close();
            }
        }

        assert(static_cast<bool>(server_stream));
        if (server_stream->isClosing() == false) {
            server_stream->close();
        }
        return Err::E_SUCCESS;
    }

    Err closeInternal()
    {
        // Close all internal handles.
        if (safety_async && safety_async->isClosing() == false) {
            safety_async->close();
        }
        return Err::E_SUCCESS;
    }
};

// -----------------------------------
// ClientIteratorGuard implementation.
// -----------------------------------

StreamServer::ClientGuard::ClientGuard(StreamServer * parent) : _parent(parent)
{
    assert(_parent != nullptr);
    _parent->_mutex.lock();
}

StreamServer::ClientGuard::~ClientGuard()
{
    assert(_parent != nullptr);
    _parent->_mutex.unlock();
}

StreamServer::iterator StreamServer::ClientGuard::begin()
{
    assert(_parent != nullptr);
    assert(static_cast<bool>(_parent->_internal));
    return _parent->_internal->begin();
}

StreamServer::iterator StreamServer::ClientGuard::end()
{
    assert(_parent != nullptr);
    assert(static_cast<bool>(_parent->_internal));
    return _parent->_internal->end();
}

StreamServer::const_iterator StreamServer::ClientGuard::begin() const
{
    assert(_parent != nullptr);
    assert(static_cast<bool>(_parent->_internal));
    return _parent->_internal->begin();
}

StreamServer::const_iterator StreamServer::ClientGuard::end() const
{
    assert(_parent != nullptr);
    assert(static_cast<bool>(_parent->_internal));
    return _parent->_internal->end();
}

StreamServer::const_iterator StreamServer::ClientGuard::cbegin() const
{
    assert(_parent != nullptr);
    assert(static_cast<bool>(_parent->_internal));
    return _parent->_internal->cbegin();
}

StreamServer::const_iterator StreamServer::ClientGuard::cend() const
{
    assert(_parent != nullptr);
    assert(static_cast<bool>(_parent->_internal));
    return _parent->_internal->cend();
}

bool StreamServer::ClientGuard::empty() const
{
    assert(_parent != nullptr);
    assert(static_cast<bool>(_parent->_internal));
    return _parent->_internal->empty();
}

StreamServer::size_type StreamServer::ClientGuard::size() const
{
    assert(_parent != nullptr);
    assert(static_cast<bool>(_parent->_internal));
    return _parent->_internal->size();
}

StreamServer::size_type StreamServer::ClientGuard::max_size() const
{
    assert(_parent != nullptr);
    assert(static_cast<bool>(_parent->_internal));
    return _parent->_internal->max_size();
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

bool StreamServer::empty() const
{
    Guard const LOCK(_mutex);
    return _internal->empty();
}

std::size_t StreamServer::size() const
{
    Guard const LOCK(_mutex);
    return _internal->size();
}

StreamServer::WeakStream StreamServer::getServer()
{
    Guard const LOCK(_mutex);
    return _internal->server_stream;
}

StreamServer::WeakSafetyAsync StreamServer::getAsync()
{
    Guard const LOCK(_mutex);
    return _internal->safety_async;
}

Err StreamServer::sendJob(SharedJob job)
{
    Guard const LOCK(_mutex);
    if (static_cast<bool>(_internal->safety_async) == false) {
        return Err::E_EXPIRED;
    }
    return _internal->safety_async->sendJob(job);
}

StreamServer::UniqueClientGuard StreamServer::getIterators()
{
    return UniqueClientGuard(new ClientGuard(this));
}

std::string StreamServer::dest() const
{
    Guard const LOCK(_mutex);
    if (static_cast<bool>(_internal->server_stream) == false) {
        tDLogW("StreamServer::dest() Expired client.");
        return std::string();
    }

    if (_internal->STREAM_TYPE == StreamType::TCP) {
        return std::static_pointer_cast<uvpp::Tcp>(_internal->server_stream)->getSockIp();
    } else if (_internal->STREAM_TYPE == StreamType::PIPE) {
        return std::static_pointer_cast<uvpp::Pipe>(_internal->server_stream)->getSockName();
    }

    TBAG_INACCESSIBLE_BLOCK_ASSERT();
    tDLogW("StreamServer::dest() Unknown stream type: {}", static_cast<int>(_internal->STREAM_TYPE));
    return std::string();
}

int StreamServer::port() const
{
    Guard const LOCK(_mutex);
    if (static_cast<bool>(_internal->server_stream) == false) {
        tDLogW("StreamServer::port() Expired client.");
        return 0;
    }

    if (_internal->STREAM_TYPE == StreamType::TCP) {
        return std::static_pointer_cast<uvpp::Tcp>(_internal->server_stream)->getSockPort();
    }
    return 0;
}

Err StreamServer::init(char const * destination, int port)
{
    Guard const LOCK(_mutex);
    Err const INIT_CODE = _internal->initServer(destination, port);
    if (isFailure(INIT_CODE)) {
        tDLogE("StreamClient::init() Initialize {} error.", getErrName(INIT_CODE));
        return INIT_CODE;
    }

    Err const INIT_INTERNAL_CODE = _internal->initInternalHandles();
    if (isFailure(INIT_INTERNAL_CODE)) {
        tDLogE("StreamClient::init() Initialize {} error (internal handles).", getErrName(INIT_INTERNAL_CODE));
        return INIT_INTERNAL_CODE;
    }
    return Err::E_SUCCESS;
}

Err StreamServer::close()
{
    Guard const LOCK(_mutex);
    if (_internal->safety_async && _internal->loop.isAliveAndThisThread() == false) {
        auto job = _internal->safety_async->newSendFunc([&](){
            Guard const ASYNC_LOCK(_mutex);
            _internal->closeServer();
        });
        return static_cast<bool>(job) ? Err::E_ASYNCREQ : Err::E_ESEND;
    }
    return _internal->closeServer();
}

StreamServer::WeakClient StreamServer::accept()
{
    Guard const LOCK(_mutex);
    return _internal->accept();
}

StreamServer::WeakClient StreamServer::get(Id id)
{
    Guard const LOCK(_mutex);
    return _internal->getWeakClient(id);
}

Err StreamServer::remove(Id id)
{
    Guard const LOCK(_mutex);
    return _internal->eraseClient(id) ? Err::E_SUCCESS : Err::E_UNKNOWN;
}

// --------------
// Event backend.
// --------------

void StreamServer::backConnection(Err code)
{
    _internal->on_connection.store(true);
    onConnection(code);
    _internal->on_connection.store(false);
}

void StreamServer::backClose()
{
    {
        Guard const LOCK(_mutex);
        _internal->closeInternal();
    }
    onServerClose();
}

// -----------------------
// StreamServer extension.
// -----------------------

StreamServer::SharedStreamNode StreamServer::createClient(StreamType type, Loop & loop, SharedStream & server)
{
    return SharedStreamNode(new (std::nothrow) StreamNode(loop, type, this));
}

} // namespace stream
} // namespace network

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

