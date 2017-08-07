/**
 * @file   StreamClient.cpp
 * @brief  StreamClient class implementation.
 * @author zer0
 * @date   2017-05-22
 */

#include <libtbag/network/stream/StreamClient.hpp>
#include <libtbag/network/stream/StreamClientBackend.hpp>
#include <libtbag/uvpp/Timer.hpp>
#include <libtbag/debug/Assert.hpp>
#include <libtbag/log/Log.hpp>
#include <libtbag/Type.hpp>
#include <libtbag/Noncopyable.hpp>

#include <cassert>
#include <cstring>

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
struct StreamClient::Internal : private Noncopyable
{
    using ConnectRequest  = uvpp::ConnectRequest;
    using ShutdownRequest = uvpp::ShutdownRequest;
    using WriteRequest    = uvpp::WriteRequest;

    struct UserTimer : public uvpp::Timer
    {
        StreamClient * parent;

        UserTimer(Loop & loop, StreamClient * p) : uvpp::Timer(loop), parent(p)
        { /* EMPTY. */ }
        virtual ~UserTimer()
        { /* EMPTY. */ }

        virtual void onTimer() override
        {
            assert(parent != nullptr);
            parent->onTimer();
        }
    };

    struct ShutdownTimer : public uvpp::Timer
    {
        StreamClient * parent;

        ShutdownTimer(Loop & loop, StreamClient * p) : uvpp::Timer(loop), parent(p)
        { /* EMPTY. */ }
        virtual ~ShutdownTimer()
        { /* EMPTY. */ }

        virtual void onTimer() override
        {
            assert(parent != nullptr);
            parent->onShutdownTimer();
        }
    };

    struct AsyncWrite : public SafetyAsync::JobInterface
    {
        StreamClient * parent;

        AsyncWrite(StreamClient * p) : parent(p)
        { /* EMPTY. */ }
        virtual ~AsyncWrite()
        { /* EMPTY. */ }

        virtual void run() override
        {
            assert(parent != nullptr);
            parent->onAsyncWrite();
        }
    };

    using SharedUserTimer     = std::shared_ptr<UserTimer>;
    using SharedShutdownTimer = std::shared_ptr<ShutdownTimer>;

    using binf   = StreamClient::binf;
    using Buffer = std::vector<char>;

    StreamClient * parent;

    bool owner_async;
    uint64_t write_timeout;

    SharedClientBackend client;
    SharedSafetyAsync   async;
    SharedUserTimer     user_timer;
    SharedShutdownTimer shutdown_timer;

    struct {
        WriteState      state;
        ConnectRequest   connect_req;
        WriteRequest     write_req;
        ShutdownRequest  shutdown_req;
        Buffer           buffer;
    } writer;

    Internal(StreamClient * parent) : parent(parent), owner_async(false), write_timeout(0)
    {
        // EMPTY.
    }

    virtual ~Internal()
    {
        // EMPTY.
    }

    Loop & getLoop()
    {
        assert(static_cast<bool>(client));
        Loop * loop = client->getLoop();
        assert(loop != nullptr);
        return *loop;
    }

    Err initClient(StreamType type, std::string const & destination, int port = 0)
    {
        if (static_cast<bool>(client) == false) {
            return Err::E_EXPIRED;
        }

        using TcpBackend  = StreamClientBackend<uvpp::Tcp>;
        using PipeBackend = StreamClientBackend<uvpp::Pipe>;

        if (type == StreamType::TCP) {
            auto backend = std::static_pointer_cast<TcpBackend>(client);
            return uvpp::initCommonClient(*backend, writer.connect_req, destination, port);
        } else if (type == StreamType::PIPE) {
            auto backend = std::static_pointer_cast<PipeBackend>(client);
            return uvpp::initPipeClient(*backend, writer.connect_req, destination);
        }

        tDLogA("StreamClient::Internal::initClient() Unknown stream type.");
        return Err::E_ILLARGS;
    }

    bool initHandles()
    {
        Loop & loop = getLoop();

        if (static_cast<bool>(async) == false) {
            async = loop.newHandle<SafetyAsync>(loop);
        }
        user_timer = loop.newHandle<UserTimer>(loop, parent);
        shutdown_timer = loop.newHandle<ShutdownTimer>(loop, parent);

        assert(static_cast<bool>(async));
        assert(static_cast<bool>(user_timer));
        assert(static_cast<bool>(shutdown_timer));

        return true;
    }

    bool isActiveUserTimer() const
    {
        if (static_cast<bool>(user_timer) == false) {
            return false;
        }
        return user_timer->isActive();
    }

    Err startUserTimer(uint64_t millisec)
    {
        if (static_cast<bool>(user_timer) == false) {
            return Err::E_ALREADY;
        }
        if (user_timer->isActive()) {
            return Err::E_EBUSY;
        }

        Err const CODE = user_timer->start(millisec);
        if (CODE != Err::E_SUCCESS) {
            tDLogE("StreamClient::Internal::startUserTimer() {} error.", getErrName(CODE));
        }
        return CODE;
    }

    void stopUserTimer()
    {
        if (static_cast<bool>(user_timer) == false) {
            return;
        }

        Err const CODE = user_timer->stop();
        if (CODE != Err::E_SUCCESS) {
            tDLogE("StreamClient::Internal::stopUserTimer() {} error.", getErrName(CODE));
        }
    }

    bool isActiveShutdownTimer() const
    {
        if (static_cast<bool>(shutdown_timer) == false) {
            return false;
        }
        return shutdown_timer->isActive();
    }

    Err startShutdownTimer(uint64_t millisec)
    {
        if (static_cast<bool>(shutdown_timer) == false) {
            return Err::E_ALREADY;
        }
        if (shutdown_timer->isActive()) {
            return Err::E_EBUSY;
        }

        Err const CODE = shutdown_timer->start(millisec);
        if (CODE != Err::E_SUCCESS) {
            tDLogE("StreamClient::Internal::startShutdownTimer() {} error.", getErrName(CODE));
        }
        return CODE;
    }

    void stopShutdownTimer()
    {
        if (static_cast<bool>(shutdown_timer) == false) {
            return;
        }

        Err const CODE = shutdown_timer->stop();
        if (CODE != Err::E_SUCCESS) {
            tDLogE("StreamClient::Internal::stopShutdownTimer() {} error.", getErrName(CODE));
        }
    }

    Err shutdownWrite()
    {
        if (writer.state == WriteState::WS_ASYNC) {
            writer.state = WriteState::WS_ASYNC_CANCEL;
            return Err::E_SUCCESS;
        }

        if (writer.state != WriteState::WS_WRITE) {
            tDLogE("StreamClient::Internal::shutdownWrite() Illegal state error: {}",
                   getWriteStateName(writer.state));
            return Err::E_ILLSTATE;
        }

        assert(writer.state == WriteState::WS_WRITE);

        Err const CODE = client->shutdown(writer.shutdown_req);
        if (CODE != Err::E_SUCCESS) {
            tDLogE("StreamClient::Internal::shutdownWrite() {} error", getErrName(CODE));
            return CODE;
        }

        writer.state = WriteState::WS_SHUTDOWN;
        return Err::E_SUCCESS;
    }

    Err writeReal(char const * buffer, std::size_t size)
    {
        assert(static_cast<bool>(client));
        return client->write(writer.write_req, buffer, size);
    }

    void assignWriteBuffer(char const * buffer, std::size_t size)
    {
        assert(buffer != nullptr);
        assert(size > 0);
        if (writer.buffer.size() < size) {
            writer.buffer.resize(size);
        }
        ::memcpy(writer.buffer.data(), buffer, size);
    }

    Err autoWrite(char const * buffer, std::size_t size)
    {
        if (writer.state != WriteState::WS_READY) {
            tDLogE("StreamClient::Internal::autoWrite() Illegal state error: {}",
                   getWriteStateName(writer.state));
            return Err::E_ILLSTATE;
        }

        assert(static_cast<bool>(client));
        assert(static_cast<bool>(async));

        Loop & loop = getLoop();
        Err result_code = Err::E_UNKNOWN;

        if (loop.isAliveAndThisThread() == false && static_cast<bool>(async)) {
            assignWriteBuffer(buffer, size);
            auto job = async->newSendJob<AsyncWrite>(parent);
            if (static_cast<bool>(job) == false) {
                tDLogE("StreamClient::Internal::autoWrite() New job error.");
                return Err::E_BADALLOC;
            }
            writer.state = WriteState::WS_ASYNC;
            result_code = Err::E_ASYNCREQ;

        } else {
            Err const CODE = writeReal(buffer, size);
            if (CODE != Err::E_SUCCESS) {
                tDLogE("StreamClient::Internal::autoWrite() Write {} error.", getErrName(CODE));
                return CODE;
            }

            writer.state = WriteState::WS_WRITE;
            if (loop.isAliveAndThisThread() == false && static_cast<bool>(async) == false) {
                tDLogW("StreamClient::Internal::autoWrite() Async is expired.");
                result_code = Err::E_WARNING;
            } else {
                result_code = Err::E_SUCCESS;
            }
        }

        if (write_timeout > 0) {
            if (isActiveShutdownTimer()) {
                stopShutdownTimer();
            }

            Err const CODE = startShutdownTimer(write_timeout);
            if (CODE != Err::E_SUCCESS) {
                tDLogW("StreamClient::Internal::autoWrite() Timer job {} error!", getErrName(CODE));
            }
        } else {
            // tDLogD("StreamClient::Internal::autoWrite() No timeout.");
        }

        return result_code;
    }

    void closeAll()
    {
        assert(static_cast<bool>(client));
        if (client->isClosing() == false) {
            client->close();
        }

        if (owner_async && static_cast<bool>(async)) {
            if (async->isClosing() == false) {
                async->close();
            }
            async.reset();
        }

        if (static_cast<bool>(user_timer)) {
            if (user_timer->isActive()) {
                user_timer->stop();
            }
            if (user_timer->isClosing() == false) {
                user_timer->close();
            }
            user_timer.reset();
        }

        if (static_cast<bool>(shutdown_timer)) {
            if (shutdown_timer->isActive()) {
                shutdown_timer->stop();
            }
            if (shutdown_timer->isClosing() == false) {
                shutdown_timer->close();
            }
            shutdown_timer.reset();
        }
    }
};

// ----------------------------
// StreamClient implementation.
// ----------------------------

StreamClient::StreamClient(Loop & loop, StreamType type)
        : StreamClient(loop, type, SharedSafetyAsync())
{
    assert(static_cast<bool>(_internal));
}

StreamClient::StreamClient(Loop & loop,
                           StreamType type,
                           SharedSafetyAsync async,
                           WriteReady const & UNUSED_PARAM(ready))
        : StreamClient(loop, type, async)
{
    assert(static_cast<bool>(_internal));
    if (_internal->initHandles()) {
        _internal->writer.state = WriteState::WS_READY;
    }
}

StreamClient::StreamClient(Loop & loop, StreamType type, SharedSafetyAsync async)
         : STREAM_TYPE(type), _internal(new Internal(this))
{
    assert(static_cast<bool>(_internal));

    using  TcpBackend = StreamClientBackend<uvpp::Tcp>;
    using PipeBackend = StreamClientBackend<uvpp::Pipe>;

    if (type == StreamType::TCP) {
        _internal->client = loop.newHandle<TcpBackend>(loop, this);
    } else if (type == StreamType::PIPE) {
        _internal->client = loop.newHandle<PipeBackend>(loop, this);
    } else {
        throw std::bad_alloc();
    }
    assert(static_cast<bool>(_internal->client));

    if (static_cast<bool>(async) && loop.id() == async->getLoop()->id()) {
        _internal->owner_async = false;
        _internal->async = async;
    } else {
        _internal->owner_async = true;
        _internal->async.reset();
    }

    _internal->writer.state = WriteState::WS_NOT_READY;
}

StreamClient::~StreamClient()
{
    // EMPTY.
}

void StreamClient::onShutdownTimer()
{
    assert(static_cast<bool>(_internal));
    Guard const MUTEX_GUARD(_mutex);
    _internal->shutdownWrite();
}

void StreamClient::onAsyncWrite()
{
    assert(static_cast<bool>(_internal));
    Guard const MUTEX_GUARD(_mutex);

    if (_internal->writer.state == WriteState::WS_ASYNC_CANCEL) {
        tDLogN("StreamClient::onAsyncWrite() Cancel async write.");
        _internal->stopShutdownTimer();
        _internal->writer.state = WriteState::WS_READY;
        return;
    }

    if (_internal->writer.state != WriteState::WS_ASYNC) {
        tDLogE("StreamClient::onAsyncWrite() Error state: {}", getWriteStateName(_internal->writer.state));
        _internal->stopShutdownTimer();
        _internal->writer.state = WriteState::WS_READY;
        return;
    }

    assert(_internal->writer.state == WriteState::WS_ASYNC);

    if (_internal->writer.buffer.empty()) {
        tDLogD("StreamClient::onAsyncWrite() Empty writer buffer.");
        _internal->writer.state = WriteState::WS_READY;
        return;
    }

    Err const CODE = _internal->writeReal(_internal->writer.buffer.data(), _internal->writer.buffer.size());
    if (CODE == Err::E_SUCCESS) {
        _internal->writer.state = WriteState::WS_WRITE;
    } else {
        tDLogE("StreamClient::onAsyncWrite() {} error.", getErrName(CODE));
        _internal->stopShutdownTimer();
        _internal->writer.state = WriteState::WS_READY;
    }
}

char const * StreamClient::getWriteStateName(WriteState state) TBAG_NOEXCEPT
{
    switch (state) {
    case WriteState::WS_NOT_READY:     return "NOT_READY";
    case WriteState::WS_READY:         return "READY";
    case WriteState::WS_ASYNC:         return "ASYNC";
    case WriteState::WS_ASYNC_CANCEL:  return "ASYNC_CANCEL";
    case WriteState::WS_WRITE:         return "WRITE";
    case WriteState::WS_SHUTDOWN:      return "SHUTDOWN";
    case WriteState::WS_END:           return "END";
    }

    TBAG_INACCESSIBLE_BLOCK_ASSERT();
    return "UNKNOWN";
}

StreamClient::WriteState StreamClient::getWriteState() const
{
    assert(static_cast<bool>(_internal));
    Guard const MUTEX_GUARD(_mutex);
    return _internal->writer.state;
}

char const * StreamClient::getWriteStateName() const
{
    assert(static_cast<bool>(_internal));
    Guard const MUTEX_GUARD(_mutex);
    return getWriteStateName(_internal->writer.state);
}

StreamClient::WeakClientBackend StreamClient::getClient()
{
    assert(static_cast<bool>(_internal));
    assert(static_cast<bool>(_internal->client));
    Guard const MUTEX_GUARD(_mutex);
    return WeakClientBackend(_internal->client);
}

StreamClient::WeakSafetyAsync StreamClient::getAsync()
{
    assert(static_cast<bool>(_internal));
    Guard const MUTEX_GUARD(_mutex);
    return WeakSafetyAsync(_internal->async);
}

StreamClient::Id StreamClient::id() const
{
    assert(static_cast<bool>(_internal));
    assert(static_cast<bool>(_internal->client));
    Guard const MUTEX_GUARD(_mutex);
    return _internal->client->id();
}

void * StreamClient::udata()
{
    assert(static_cast<bool>(_internal));
    assert(static_cast<bool>(_internal->client));
    Guard const MUTEX_GUARD(_mutex);
    return _internal->client->getUserData();
}

std::string StreamClient::dest() const
{
    assert(static_cast<bool>(_internal));
    assert(static_cast<bool>(_internal->client));
    Guard const MUTEX_GUARD(_mutex);
    if (STREAM_TYPE == StreamType::TCP) {
        return std::static_pointer_cast<uvpp::Tcp>(_internal->client)->getPeerIp();
    }
    return std::string();
}

int StreamClient::port() const
{
    assert(static_cast<bool>(_internal));
    assert(static_cast<bool>(_internal->client));
    Guard const MUTEX_GUARD(_mutex);
    if (STREAM_TYPE == StreamType::TCP) {
        return std::static_pointer_cast<uvpp::Tcp>(_internal->client)->getPeerPort();
    }
    return 0;
}

Err StreamClient::init(char const * destination, int port)
{
    assert(static_cast<bool>(_internal));
    Guard const MUTEX_GUARD(_mutex);

    Err const INIT_CODE = _internal->initClient(STREAM_TYPE, std::string(destination), port);
    if (INIT_CODE != Err::E_SUCCESS) {
        tDLogE("StreamClient::init() Initialize fail.");
        return INIT_CODE;
    }

    if (_internal->initHandles() == false) {
        tDLogE("StreamClient::init() Initialize fail (internal handles).");
        return Err::E_UNKNOWN;
    }

    return Err::E_SUCCESS;
}

Err StreamClient::start()
{
    assert(static_cast<bool>(_internal));
    assert(static_cast<bool>(_internal->client));
    Guard const MUTEX_GUARD(_mutex);
    Err const CODE = _internal->client->startRead();
    if (CODE != Err::E_SUCCESS) {
        tDLogE("StreamClient::start() {} error", getErrName(CODE));
    }
    return CODE;
}

Err StreamClient::stop()
{
    assert(static_cast<bool>(_internal));
    assert(static_cast<bool>(_internal->client));
    Guard const MUTEX_GUARD(_mutex);
    Err const CODE = _internal->client->stopRead();
    if (CODE != Err::E_SUCCESS) {
        tDLogE("StreamClient::stop() {} error", getErrName(CODE));
    }
    return CODE;
}

Err StreamClient::close()
{
    assert(static_cast<bool>(_internal));
    Guard const MUTEX_GUARD(_mutex);
    Loop & loop = _internal->getLoop();

    if (loop.isAliveAndThisThread() == false && static_cast<bool>(_internal->async)) {
        tDLogD("StreamClient::close() Async request.");
        _internal->async->newSendFunc([&](){
            Guard const MUTEX_GUARD_ASYNC(_mutex);
            _internal->closeAll();
        });
        return Err::E_ASYNCREQ;
    }

    Err code = Err::E_SUCCESS;
    if (loop.isAliveAndThisThread() == false && static_cast<bool>(_internal->async) == false) {
        tDLogW("StreamClient::close() Async is expired.");
        code = Err::E_WARNING;
    }

    tDLogD("StreamClient::close() Synced request.");
    _internal->closeAll();
    return code;
}

Err StreamClient::cancel()
{
    assert(static_cast<bool>(_internal));
    Guard const MUTEX_GUARD(_mutex);
    Loop & loop = _internal->getLoop();

    if (loop.isAliveAndThisThread() == false && static_cast<bool>(_internal->async)) {
        tDLogD("StreamClient::cancel() Async request.");
        _internal->async->newSendFunc([&]() {
            Guard const MUTEX_GUARD_ASYNC(_mutex);
            _internal->shutdownWrite();
        });
        return Err::E_ASYNCREQ;
    }

    if (loop.isAliveAndThisThread() == false && static_cast<bool>(_internal->async) == false) {
        tDLogW("StreamClient::cancel() Async is expired.");
    }
    tDLogD("StreamClient::cancel() Synced request.");
    return _internal->shutdownWrite();
}

Err StreamClient::write(char const * buffer, std::size_t size)
{
    assert(static_cast<bool>(_internal));
    Guard const MUTEX_GUARD(_mutex);
    return _internal->autoWrite(buffer, size);
}

void StreamClient::setWriteTimeout(uint64_t millisec)
{
    assert(static_cast<bool>(_internal));
    Guard const MUTEX_GUARD(_mutex);
    _internal->write_timeout = millisec;
}

bool StreamClient::isActiveTimer()
{
    assert(static_cast<bool>(_internal));
    Guard const MUTEX_GUARD(_mutex);
    return _internal->isActiveUserTimer();
}

Err StreamClient::startTimer(uint64_t millisec)
{
    assert(static_cast<bool>(_internal));
    Guard const MUTEX_GUARD(_mutex);
    return _internal->startUserTimer(millisec);
}

void StreamClient::stopTimer()
{
    assert(static_cast<bool>(_internal));
    Guard const MUTEX_GUARD(_mutex);
    _internal->stopUserTimer();
}

// --------------
// Event backend.
// --------------

void StreamClient::backConnect(Err code)
{
    assert(static_cast<bool>(_internal));
    _mutex.lock();
    _internal->writer.state = WriteState::WS_READY;
    _mutex.unlock();

    onConnect(code);
}

void StreamClient::backShutdown(Err code)
{
    assert(static_cast<bool>(_internal));
    _mutex.lock();
    _internal->writer.state = WriteState::WS_READY;
    _mutex.unlock();

    onShutdown(code);
}

void StreamClient::backWrite(Err code)
{
    assert(static_cast<bool>(_internal));
    _mutex.lock();
    _internal->stopShutdownTimer();
    assert(/**/_internal->writer.state == WriteState::WS_WRITE ||
               _internal->writer.state == WriteState::WS_SHUTDOWN);
    _internal->writer.state = WriteState::WS_READY;
    _mutex.unlock();

    onWrite(code);
}

void StreamClient::backRead(Err code, ReadPacket const & packet)
{
    onRead(code, packet);
}

void StreamClient::backClose()
{
    assert(static_cast<bool>(_internal));
    _mutex.lock();
    _internal->closeAll();
    _internal->writer.state = WriteState::WS_END;
    _mutex.unlock();

    onClose();
}

} // namespace stream
} // namespace network

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

