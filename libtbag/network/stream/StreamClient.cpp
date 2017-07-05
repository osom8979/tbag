/**
 * @file   StreamClient.cpp
 * @brief  StreamClient class implementation.
 * @author zer0
 * @date   2017-05-22
 */

#include <libtbag/network/stream/StreamClient.hpp>
#include <libtbag/network/stream/StreamClientBackend.hpp>
#include <libtbag/container/CachedBuffers.hpp>
#include <libtbag/uvpp/Timer.hpp>
#include <libtbag/debug/Assert.hpp>
#include <libtbag/log/Log.hpp>
#include <libtbag/Type.hpp>
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

    using binf    = StreamClient::binf;
    using Buffers = container::CachedBuffers<binf::Base, binf::Size>;

    STATIC_ASSERT_CHECK_IS_SAME(binf, Buffers::binf);

    StreamClient * parent;

    bool owner_async;
    uint64_t write_timeout;

    SharedClientBackend client;
    SharedSafetyAsync   async;
    SharedUserTimer     user_timer;
    SharedShutdownTimer shutdown_timer;

    struct {
        WriteStatus      status;
        ConnectRequest   connect_req;
        WriteRequest     write_req;
        ShutdownRequest  shutdown_req;
        Buffers          buffers;
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
        if (writer.status == WriteStatus::WS_ASYNC) {
            writer.status = WriteStatus::WS_ASYNC_CANCEL;
            return Err::E_SUCCESS;
        }

        if (writer.status != WriteStatus::WS_WRITE) {
            tDLogE("StreamClient::Internal::shutdownWrite() Illegal state error: {}",
                   getWriteStatusName(writer.status));
            return Err::E_ILLSTATE;
        }

        assert(writer.status == WriteStatus::WS_WRITE);

        Err const CODE = client->shutdown(writer.shutdown_req);
        if (CODE != Err::E_SUCCESS) {
            tDLogE("StreamClient::Internal::shutdownWrite() {} error", getErrName(CODE));
            return CODE;
        }

        writer.status = WriteStatus::WS_SHUTDOWN;
        return Err::E_SUCCESS;
    }

    Err writeReal(binf const * buffer, std::size_t size)
    {
        assert(static_cast<bool>(client));
        return client->write(writer.write_req, buffer, size);
    }

    std::vector<binf> getWriteBufferInfo()
    {
        return writer.buffers.info();
        //std::size_t SIZE = writer.buffers.size();
        //std::vector<binf> result(SIZE);
        //for (std::size_t i = 0; i < SIZE; ++i) {
        //    result[i].buffer = &writer.buffers[i][0];
        //    result[i].size   =  writer.buffers[i].size();
        //}
        //return result;
    }

    Err autoWrite(binf const * buffer, std::size_t size)
    {
        if (writer.status != WriteStatus::WS_READY) {
            tDLogE("StreamClient::Internal::autoWrite() Illegal state error: {}",
                   getWriteStatusName(writer.status));
            return Err::E_ILLSTATE;
        }

        assert(static_cast<bool>(client));
        assert(static_cast<bool>(async));

        Loop & loop = getLoop();

        if (loop.isAliveAndThisThread() || static_cast<bool>(async) == false) {
            Err const CODE = writeReal(buffer, size);
            if (CODE != Err::E_SUCCESS) {
                tDLogE("StreamClient::Internal::autoWrite() Write {} error.", getErrName(CODE));
                return CODE;
            }
            writer.status = WriteStatus::WS_WRITE;

        } else {
            writer.buffers.assign(buffer, size);
            auto job = async->newSendJob<AsyncWrite>(parent);
            if (static_cast<bool>(job) == false) {
                tDLogE("StreamClient::Internal::autoWrite() New job error.");
                return Err::E_BADALLOC;
            }
            writer.status = WriteStatus::WS_ASYNC;
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
            tDLogD("StreamClient::Internal::autoWrite() No timeout.");
        }

        return Err::E_SUCCESS;
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
        _internal->writer.status = WriteStatus::WS_READY;
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

    _internal->writer.status = WriteStatus::WS_NOT_READY;
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

    if (_internal->writer.status == WriteStatus::WS_ASYNC_CANCEL) {
        tDLogN("StreamClient::onAsyncWrite() Cancel async write.");
        _internal->stopShutdownTimer();
        _internal->writer.status = WriteStatus::WS_READY;
        return;
    }

    if (_internal->writer.status != WriteStatus::WS_ASYNC) {
        tDLogE("StreamClient::onAsyncWrite() Error state: {}", getWriteStatusName(_internal->writer.status));
        _internal->stopShutdownTimer();
        _internal->writer.status = WriteStatus::WS_READY;
        return;
    }

    assert(_internal->writer.status == WriteStatus::WS_ASYNC);
    auto binfs = _internal->writer.buffers.info();
    if (binfs.empty()) {
        tDLogD("StreamClient::onAsyncWrite() Empty binfs.");
        _internal->writer.status = WriteStatus::WS_READY;
        return;
    }

    Err const CODE = _internal->writeReal(&binfs[0], binfs.size());
    if (CODE == Err::E_SUCCESS) {
        _internal->writer.status = WriteStatus::WS_WRITE;
    } else {
        tDLogE("StreamClient::onAsyncWrite() {} error.", getErrName(CODE));
        _internal->stopShutdownTimer();
        _internal->writer.status = WriteStatus::WS_READY;
    }
}

char const * StreamClient::getWriteStatusName(WriteStatus status) TBAG_NOEXCEPT
{
    switch (status) {
    case WriteStatus::WS_NOT_READY:     return "NOT_READY";
    case WriteStatus::WS_READY:         return "READY";
    case WriteStatus::WS_ASYNC:         return "ASYNC";
    case WriteStatus::WS_ASYNC_CANCEL:  return "ASYNC_CANCEL";
    case WriteStatus::WS_WRITE:         return "WRITE";
    case WriteStatus::WS_SHUTDOWN:      return "SHUTDOWN";
    case WriteStatus::WS_END:           return "END";
    }

    TBAG_INACCESSIBLE_BLOCK_ASSERT();
    return "UNKNOWN";
}

StreamClient::WriteStatus StreamClient::getWriteStatus() const
{
    assert(static_cast<bool>(_internal));
    Guard const MUTEX_GUARD(_mutex);
    return _internal->writer.status;
}

char const * StreamClient::getWriteStatusName() const
{
    assert(static_cast<bool>(_internal));
    Guard const MUTEX_GUARD(_mutex);
    return getWriteStatusName(_internal->writer.status);
}

StreamClient::WeakClientBackend StreamClient::getClient()
{
    assert(static_cast<bool>(_internal));
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
    assert(static_cast<bool>(_internal->client));
    Guard const MUTEX_GUARD(_mutex);
    return _internal->client->id();
}

void * StreamClient::udata()
{
    assert(static_cast<bool>(_internal->client));
    Guard const MUTEX_GUARD(_mutex);
    return _internal->client->getUserData();
}

std::string StreamClient::dest() const
{
    assert(static_cast<bool>(_internal->client));
    Guard const MUTEX_GUARD(_mutex);
    if (STREAM_TYPE == StreamType::TCP) {
        return std::static_pointer_cast<uvpp::Tcp>(_internal->client)->getPeerIp();
    }
    return std::string();
}

int StreamClient::port() const
{
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
    assert(static_cast<bool>(_internal->client));
    Guard const MUTEX_GUARD(_mutex);
    Err const CODE = _internal->client->stopRead();
    if (CODE != Err::E_SUCCESS) {
        tDLogE("StreamClient::stop() {} error", getErrName(CODE));
    }
    return CODE;
}

void StreamClient::close()
{
    assert(static_cast<bool>(_internal));
    assert(static_cast<bool>(_internal->client));

    Guard const MUTEX_GUARD_OUT(_mutex);
    Loop & loop = _internal->getLoop();

    if (loop.isAliveAndThisThread() || static_cast<bool>(_internal->async) == false) {
        tDLogD("StreamClient::close() request.");
        _internal->closeAll();

    } else {
        tDLogD("StreamClient::close() async request.");
        _internal->async->newSendFunc([&](){
            Guard const MUTEX_GUARD_IN(_mutex);
            _internal->closeAll();
        });
    }
}

void StreamClient::cancel()
{
    assert(static_cast<bool>(_internal));
    assert(static_cast<bool>(_internal->client));

    Guard const MUTEX_GUARD_OUT(_mutex);
    Loop & loop = _internal->getLoop();

    if (loop.isAliveAndThisThread() || static_cast<bool>(_internal->async) == false) {
        tDLogD("StreamClient::cancel() request.");
        _internal->shutdownWrite();

    } else {
        tDLogD("StreamClient::cancel() async request.");
        _internal->async->newSendFunc([&]() {
            Guard const MUTEX_GUARD_IN(_mutex);
            _internal->shutdownWrite();
        });
    }
}

Err StreamClient::write(binf const * buffer, std::size_t size)
{
    assert(static_cast<bool>(_internal));
    Guard const MUTEX_GUARD(_mutex);
    return _internal->autoWrite(buffer, size);
}

Err StreamClient::write(char const * buffer, std::size_t size)
{
    assert(static_cast<bool>(_internal));

    binf info;
    info.buffer = const_cast<char*>(buffer);
    info.size   = size;

    Guard const MUTEX_GUARD(_mutex);
    return _internal->autoWrite(&info, 1U);
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
    _internal->writer.status = WriteStatus::WS_READY;
    _mutex.unlock();

    onConnect(code);
}

void StreamClient::backShutdown(Err code)
{
    assert(static_cast<bool>(_internal));
    _mutex.lock();
    _internal->writer.status = WriteStatus::WS_READY;
    _mutex.unlock();

    onShutdown(code);
}

void StreamClient::backWrite(Err code)
{
    assert(static_cast<bool>(_internal));
    _mutex.lock();
    _internal->stopShutdownTimer();
    _internal->writer.status = WriteStatus::WS_READY;
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
    _internal->writer.status = WriteStatus::WS_END;
    _mutex.unlock();

    onClose();
}

} // namespace stream
} // namespace network

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

