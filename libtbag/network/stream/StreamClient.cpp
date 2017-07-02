/**
 * @file   StreamClient.cpp
 * @brief  StreamClient class implementation.
 * @author zer0
 * @date   2017-05-22
 */

#include <libtbag/network/stream/StreamClient.hpp>
#include <libtbag/network/stream/StreamClientBackend.hpp>
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
struct StreamClient::Internal : private Noncopyable
{
    using binf = StreamClient::binf;

    StreamClient * _parent;

    Internal(StreamClient * parent) : _parent(parent)
    {
        // EMPTY.
    }

    ~Internal()
    {
        // EMPTY.
    }

    static Err startTimer(uvpp::Timer & timer, uint64_t millisec)
    {
        if (timer.isActive()) {
            return Err::E_EBUSY;
        }
        return timer.start(millisec);
    }

    static Err stopTimer(uvpp::Timer & timer)
    {
        return timer.stop();
    }

    bool initHandles()
    {
        assert(_parent != nullptr);
        StreamClient & p = *_parent;

        assert(p._client != nullptr);
        Loop * loop = p._client->getLoop();
        assert(loop != nullptr);

        if (static_cast<bool>(p._async) == false) {
            p._async = loop->newHandle<SafetyAsync>(*loop);
        }
        p._close_timer    = loop->newHandle<FuncTimer>(*loop);
        p._shutdown_timer = loop->newHandle<FuncTimer>(*loop);

        assert(static_cast<bool>(p._async));
        assert(static_cast<bool>(p._close_timer));
        assert(static_cast<bool>(p._shutdown_timer));

        p._close_timer->setOnTimer([&](){
            tDLogN("StreamClient::Internal::initHandles() On close timer!");
            Guard const MUTEX_GUARD(p._mutex);
            closeAll();
        });
        p._shutdown_timer->setOnTimer([&](){
            tDLogN("StreamClient::Internal::initHandles() On shutdown timer!");
            Guard const MUTEX_GUARD(p._mutex);
            shutdownWrite(); // The return value is not used.
        });
        return true;
    }

    Err startCloseTimer(uint64_t millisec)
    {
        assert(_parent != nullptr);
        StreamClient & p = *_parent;

        if (static_cast<bool>(p._close_timer) == false) {
            return Err::E_ALREADY;
        }

        Err const CODE = startTimer(*p._close_timer, millisec);
        if (CODE != Err::E_SUCCESS) {
            tDLogE("StreamClient::Internal::startCloseTimer() {} error.", getErrName(CODE));
        }
        return CODE;
    }

    void stopCloseTimer()
    {
        assert(_parent != nullptr);
        StreamClient & p = *_parent;

        if (static_cast<bool>(p._close_timer) == false) {
            return;
        }

        Err const CODE = stopTimer(*p._close_timer);
        if (CODE != Err::E_SUCCESS) {
            tDLogE("StreamClient::Internal::stopCloseTimer() {} error.", getErrName(CODE));
        }
    }

    Err startShutdownTimer(uint64_t millisec)
    {
        assert(_parent != nullptr);
        StreamClient & p = *_parent;

        if (static_cast<bool>(p._shutdown_timer) == false) {
            return Err::E_ALREADY;
        }

        Err const CODE = startTimer(*p._shutdown_timer, millisec);
        if (CODE != Err::E_SUCCESS) {
            tDLogE("StreamClient::Internal::startShutdownTimer() {} error.", getErrName(CODE));
        }
        return CODE;
    }

    void stopShutdownTimer()
    {
        assert(_parent != nullptr);
        StreamClient & p = *_parent;

        if (static_cast<bool>(p._shutdown_timer) == false) {
            return;
        }

        Err const CODE = stopTimer(*p._shutdown_timer);
        if (CODE != Err::E_SUCCESS) {
            tDLogE("StreamClient::Internal::stopShutdownTimer() {} error.", getErrName(CODE));
        }
    }

    Err shutdownWrite()
    {
        assert(_parent != nullptr);
        StreamClient & p = *_parent;

        if (p._writer.status == WriteStatus::WS_ASYNC) {
            p._writer.status = WriteStatus::WS_ASYNC_CANCEL;
            return Err::E_SUCCESS;
        }

        if (p._writer.status != WriteStatus::WS_WRITE) {
            tDLogE("StreamClient::Internal::shutdownWrite() Illegal state error: {}",
                   getWriteStatusName(p._writer.status));
            return Err::E_ILLSTATE;
        }

        assert(p._writer.status == WriteStatus::WS_WRITE);

        Err const CODE = p._client->shutdown(p._writer.shutdown_req);
        if (CODE != Err::E_SUCCESS) {
            tDLogE("StreamClient::Internal::shutdownWrite() {} error", getErrName(CODE));
            return CODE;
        }

        p._writer.status = WriteStatus::WS_SHUTDOWN;
        return Err::E_SUCCESS;
    }

    Err writeReal(binf const * buffer, std::size_t size)
    {
        assert(_parent != nullptr);
        StreamClient & p = *_parent;

        assert(static_cast<bool>(p._client));
        return p._client->write(p._writer.write_req, buffer, size);
    }

    void copyToWriteBuffer(binf const * buffer, std::size_t size)
    {
        assert(_parent != nullptr);
        StreamClient & p = *_parent;

        // COPY TO TEMP BUFFER ...
        p._writer.buffers.resize(size);
        for (std::size_t i = 0; i < size; ++i) {
            char const * b = (buffer + i)->buffer;
            std::size_t bs = (buffer + i)->size;
            p._writer.buffers[i].assign(b, b + bs);
        }
    }

    std::vector<binf> getWriteBufferInfo()
    {
        assert(_parent != nullptr);
        StreamClient & p = *_parent;

        std::size_t SIZE = p._writer.buffers.size();
        std::vector<binf> result(SIZE);
        for (std::size_t i = 0; i < SIZE; ++i) {
            result[i].buffer = &p._writer.buffers[i][0];
            result[i].size   =  p._writer.buffers[i].size();
        }
        return result;
    }

    Err autoWrite(binf const * buffer, std::size_t size, uint64_t millisec)
    {
        assert(_parent != nullptr);
        StreamClient & p = *_parent;

        if (p._writer.status != WriteStatus::WS_READY) {
            tDLogE("StreamClient::Internal::autoWrite() Illegal state error: {}",
                   getWriteStatusName(p._writer.status));
            return Err::E_ILLSTATE;
        }

        assert(static_cast<bool>(p._client));
        assert(static_cast<bool>(p._async));

        Loop * loop = p._client->getLoop();
        assert(loop != nullptr);

        if (loop->isAliveAndThisThread() || static_cast<bool>(p._async) == false) {
            Err const CODE = writeReal(buffer, size);
            if (CODE != Err::E_SUCCESS) {
                tDLogE("StreamClient::Internal::autoWrite() Write {} error.", getErrName(CODE));
                return CODE;
            }
            p._writer.status = WriteStatus::WS_WRITE;

        } else {
            copyToWriteBuffer(buffer, size);
            auto job = p._async->newSendFunc(std::bind(&StreamClient::onAsyncWrite, _parent));
            if (static_cast<bool>(job) == false) {
                tDLogE("StreamClient::Internal::autoWrite() New job error.");
                return Err::E_BADALLOC;
            }
            p._writer.status = WriteStatus::WS_ASYNC;
        }

        if (millisec > 0) {
            Err const CODE = startShutdownTimer(millisec);
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
        assert(_parent != nullptr);
        StreamClient & p = *_parent;

        assert(static_cast<bool>(p._client));
        if (p._client->isClosing() == false) {
            p._client->close();
        }

        if (p._owner_async && static_cast<bool>(p._async)) {
            if (p._async->isClosing() == false) {
                p._async->close();
            }
            p._async.reset();
        }

        if (static_cast<bool>(p._close_timer)) {
            if (p._close_timer->isActive()) {
                p._close_timer->stop();
            }
            if (p._close_timer->isClosing() == false) {
                p._close_timer->close();
            }
            p._close_timer.reset();
        }

        if (static_cast<bool>(p._shutdown_timer)) {
            if (p._shutdown_timer->isActive()) {
                p._shutdown_timer->stop();
            }
            if (p._shutdown_timer->isClosing() == false) {
                p._shutdown_timer->close();
            }
            p._shutdown_timer.reset();
        }
    }
};

// ----------------------------
// StreamClient implementation.
// ----------------------------

StreamClient::StreamClient(Loop & loop, StreamType type)
        : StreamClient(loop, type, SharedSafetyAsync())
{
    // EMPTY.
}

StreamClient::StreamClient(Loop & loop,
                           StreamType type,
                           SharedSafetyAsync async,
                           WriteReady const & UNUSED_PARAM(ready))
        : StreamClient(loop, type, async)
{
    assert(static_cast<bool>(_internal));
    if (_internal->initHandles()) {
        _writer.status = WriteStatus::WS_READY;
    }
}

StreamClient::StreamClient(Loop & loop, StreamType type, SharedSafetyAsync async)
         : STREAM_TYPE(type), _owner_async(false), _skip_timeout(false), _internal(new Internal(this))
{
    using  TcpBackend = StreamClientBackend<uvpp::Tcp>;
    using PipeBackend = StreamClientBackend<uvpp::Pipe>;

    if (type == StreamType::TCP) {
        _client = loop.newHandle<TcpBackend>(loop, this);
    } else if (type == StreamType::PIPE) {
        _client = loop.newHandle<PipeBackend>(loop, this);
    } else {
        throw std::bad_alloc();
    }
    assert(static_cast<bool>(_client));

    if (static_cast<bool>(async) && loop.id() == async->getLoop()->id()) {
        _owner_async = false;
        _async = async;
    } else {
        _owner_async = true;
        _async.reset();
    }

    _writer.status = WriteStatus::WS_NOT_READY;
}

StreamClient::~StreamClient()
{
    // EMPTY.
}

// -----------------------
// Private static methods.
// -----------------------

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

// ------------------
// PROTECTED SECTION.
// ------------------

void StreamClient::onAsyncWrite()
{
    assert(static_cast<bool>(_internal));
    Guard const MUTEX_GUARD(_mutex);

    if (_writer.status == WriteStatus::WS_ASYNC_CANCEL) {
        tDLogN("StreamClient::onAsyncWrite() Cancel async write.");
        _internal->stopShutdownTimer();
        _writer.status = WriteStatus::WS_READY;
        return;
    }

    if (_writer.status != WriteStatus::WS_ASYNC) {
        tDLogE("StreamClient::onAsyncWrite() Error state: {}", getWriteStatusName(_writer.status));
        _internal->stopShutdownTimer();
        _writer.status = WriteStatus::WS_READY;
        return;
    }

    assert(_writer.status == WriteStatus::WS_ASYNC);
    auto binfs = _internal->getWriteBufferInfo();
    Err const CODE = _internal->writeReal(&binfs[0], binfs.size());

    if (CODE == Err::E_SUCCESS) {
        _writer.status = WriteStatus::WS_WRITE;
    } else {
        tDLogE("StreamClient::onAsyncWrite() {} error.", getErrName(CODE));
        _internal->stopShutdownTimer();
        _writer.status = WriteStatus::WS_READY;
    }
}

StreamClient::Id StreamClient::id() const
{
    assert(static_cast<bool>(_client));
    Guard const MUTEX_GUARD(_mutex);
    return _client->id();
}

void * StreamClient::udata()
{
    assert(static_cast<bool>(_client));
    Guard const MUTEX_GUARD(_mutex);
    return _client->getUserData();
}

std::string StreamClient::dest() const
{
    assert(static_cast<bool>(_client));
    Guard const MUTEX_GUARD(_mutex);
    if (STREAM_TYPE == StreamType::TCP) {
        return std::static_pointer_cast<uvpp::Tcp>(_client)->getPeerIp();
    }
    return std::string();
}

int StreamClient::port() const
{
    assert(static_cast<bool>(_client));
    Guard const MUTEX_GUARD(_mutex);
    if (STREAM_TYPE == StreamType::TCP) {
        return std::static_pointer_cast<uvpp::Tcp>(_client)->getPeerPort();
    }
    return 0;
}

Err StreamClient::init(char const * destination, int port, uint64_t millisec)
{
    assert(static_cast<bool>(_client));
    assert(static_cast<bool>(_internal));

    Guard const MUTEX_GUARD(_mutex);
    if (static_cast<bool>(_client) == false) {
        return Err::E_EXPIRED;
    }

    using TcpBackend  = StreamClientBackend<uvpp::Tcp>;
    using PipeBackend = StreamClientBackend<uvpp::Pipe>;

    bool is_init = false;
    if (STREAM_TYPE == StreamType::TCP) {
        auto backend = std::static_pointer_cast<TcpBackend>(_client);
        is_init = uvpp::initCommonClient(*backend, _writer.connect_req, destination, port);
    } else if (STREAM_TYPE == StreamType::PIPE) {
        auto backend = std::static_pointer_cast<PipeBackend>(_client);
        is_init = uvpp::initPipeClient(*backend, _writer.connect_req, destination);
    } else {
        tDLogA("StreamClient::init() Unknown stream type.");
        return Err::E_ILLARGS;
    }

    if (is_init == false) {
        tDLogE("StreamClient::init() Initialize fail.");
        return Err::E_UNKNOWN;
    }

    if (_internal->initHandles() == false) {
        tDLogE("StreamClient::init() Initialize fail (internal handles).");
        return Err::E_UNKNOWN;
    }

    if (millisec > 0) {
        _internal->startCloseTimer(millisec);
    } else {
        tDLogD("StreamClient::init() No timeout.");
    }

    return Err::E_SUCCESS;
}

Err StreamClient::start()
{
    assert(static_cast<bool>(_client));
    Guard const MUTEX_GUARD(_mutex);
    Err const CODE = _client->startRead();
    if (CODE != Err::E_SUCCESS) {
        tDLogE("StreamClient::start() {} error", getErrName(CODE));
    }
    return CODE;
}

Err StreamClient::stop()
{
    assert(static_cast<bool>(_client));
    Guard const MUTEX_GUARD(_mutex);
    Err const CODE = _client->stopRead();
    if (CODE != Err::E_SUCCESS) {
        tDLogE("StreamClient::stop() {} error", getErrName(CODE));
    }
    return CODE;
}

void StreamClient::close()
{
    assert(static_cast<bool>(_client));
    assert(static_cast<bool>(_internal));
    Loop * loop = _client->getLoop();
    assert(loop != nullptr);

    Guard const MUTEX_GUARD(_mutex);
    if (loop->isAliveAndThisThread() || static_cast<bool>(_async) == false) {
        tDLogD("StreamClient::close() request.");
        _internal->closeAll();
    } else {
        tDLogD("StreamClient::close() async request.");
        _async->newSendFunc([&](){
            Guard const MUTEX_GUARD(_mutex);
            _internal->closeAll();
        });
    }
}

void StreamClient::cancel()
{
    assert(static_cast<bool>(_client));
    assert(static_cast<bool>(_internal));
    Loop * loop = _client->getLoop();
    assert(loop != nullptr);

    Guard const MUTEX_GUARD(_mutex);
    if (loop->isAliveAndThisThread() || static_cast<bool>(_async) == false) {
        tDLogD("StreamClient::cancel() request.");
        _internal->shutdownWrite();
    } else {
        tDLogD("StreamClient::cancel() async request.");
        _async->newSendFunc([&]() {
            Guard const MUTEX_GUARD(_mutex);
            _internal->shutdownWrite();
        });
    }
}

Err StreamClient::write(binf const * buffer, std::size_t size, uint64_t millisec)
{
    assert(static_cast<bool>(_internal));
    Guard const MUTEX_GUARD(_mutex);
    return _internal->autoWrite(buffer, size, millisec);
}

Err StreamClient::write(char const * buffer, std::size_t size, uint64_t millisec)
{
    assert(static_cast<bool>(_internal));

    binf info;
    info.buffer = const_cast<char*>(buffer);
    info.size   = size;

    Guard const MUTEX_GUARD(_mutex);
    return _internal->autoWrite(&info, 1U, millisec);
}

Err StreamClient::startTimeout(uint64_t millisec)
{
    assert(static_cast<bool>(_internal));
    return _internal->startCloseTimer(millisec);
}

void StreamClient::stopTimeout()
{
    assert(static_cast<bool>(_internal));
    _internal->stopCloseTimer();
}

// --------------
// Event backend.
// --------------

void StreamClient::backConnect(Err code)
{
    assert(static_cast<bool>(_internal));

    _mutex.lock();
    if (_skip_timeout == false) {
        _internal->stopCloseTimer();
    }
    _writer.status = WriteStatus::WS_READY;
    _mutex.unlock();

    onConnect(code);
}

void StreamClient::backShutdown(Err code)
{
    _mutex.lock();
    _writer.status = WriteStatus::WS_READY;
    _mutex.unlock();

    onShutdown(code);
}

void StreamClient::backWrite(Err code)
{
    assert(static_cast<bool>(_internal));
    _mutex.lock();
    _internal->stopShutdownTimer();
    _writer.status = WriteStatus::WS_READY;
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
    _writer.status = WriteStatus::WS_END;
    _mutex.unlock();

    onClose();
}

} // namespace stream
} // namespace network

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

