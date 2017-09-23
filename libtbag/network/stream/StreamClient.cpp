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
#include <libtbag/string/StringUtils.hpp>

#include <cassert>
#include <cstring>
#include <mutex>

//#define TBAG_STREAM_CLIENT_PRINT_BUFFER

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace network {
namespace stream  {

static void printBufferOfStreamBuffer(std::string const & prefix, uint8_t const * buffer, std::size_t size)
{
    tDLogD("printBufferOfStreamBuffer({}) SIZE:{}\n{}",
           prefix, size, string::convertByteArrayToHexStringBox(std::vector<std::uint8_t>(buffer, buffer + size)));
}

#if defined(TBAG_STREAM_CLIENT_PRINT_BUFFER)
#define TBAG_STREAM_CLIENT_PRINT_BUFFER_IMPL(prefix, buffer, size) \
    ::libtbag::network::stream::printBufferOfStreamBuffer(prefix, (uint8_t const *)buffer, size)
#else
#define TBAG_STREAM_CLIENT_PRINT_BUFFER_IMPL(prefix, buffer, size)
#endif

/**
 * Stream internal helper class.
 *
 * @author zer0
 * @date   2017-06-16
 *
 * @warning
 *  - Do not use a mutex if the function name starts with '_'.
 */
class StreamClient::Internal : private Noncopyable
{
public:
    struct UserTimer final : public uvpp::Timer
    {
        Internal * parent;
        UserTimer(Loop & loop, Internal * p) : uvpp::Timer(loop), parent(p) { /* EMPTY. */ }
        virtual ~UserTimer() { /* EMPTY. */ }
        virtual void onTimer() override { parent->onUserTimer(); }
    };

    struct ShutdownTimer final : public uvpp::Timer
    {
        Internal * parent;
        ShutdownTimer(Loop & loop, Internal * p) : uvpp::Timer(loop), parent(p) { /* EMPTY. */ }
        virtual ~ShutdownTimer() { /* EMPTY. */ }
        virtual void onTimer() override { parent->onShutdownTimer(); }
    };

    struct AsyncWrite final : public SafetyAsync::JobInterface
    {
        Internal * parent;
        AsyncWrite(Internal * p) : parent(p) { /* EMPTY. */ }
        virtual ~AsyncWrite() { /* EMPTY. */ }
        virtual void run() override { parent->onAsyncWrite(); }
    };

public:
    using SharedUserTimer     = std::shared_ptr<UserTimer>;
    using SharedShutdownTimer = std::shared_ptr<ShutdownTimer>;

    using binf   = StreamClient::binf;
    using Buffer = std::vector<char>;

    using Mutex = std::mutex;
    using Guard = std::lock_guard<Mutex>;

private:
    StreamType const STREAM_TYPE;

private:
    StreamClient * _parent;

private:
    mutable Mutex _mutex;

private:
    SharedClientBackend _client;
    SharedSafetyAsync   _safety_async;
    SharedUserTimer     _user_timer;
    SharedShutdownTimer _shutdown_timer;

private:
    WriteInfo _winfo;

public:
    Internal(StreamClient * parent, Loop & loop, StreamType type)
            : STREAM_TYPE(type), _parent(parent)
    {
        using  TcpBackend = StreamClientBackend<uvpp::Tcp>;
        using PipeBackend = StreamClientBackend<uvpp::Pipe>;

        if (type == StreamType::TCP) {
            _client = loop.newHandle<TcpBackend>(loop, _parent);
        } else if (type == StreamType::PIPE) {
            _client = loop.newHandle<PipeBackend>(loop, _parent);
        } else {
            throw std::bad_alloc();
        }

        assert(static_cast<bool>(_client));
        _winfo.setNotReady();
    }

    Internal(StreamClient * parent, Loop & loop, StreamType type, WriteReady const & UNUSED_PARAM(ready))
            : Internal(parent, loop, type)
    {
        if (initInternalHandles()) {
            _winfo.setReady();
        }
    }

    ~Internal()
    {
        // EMPTY.
    }

public:
    inline StreamType getStreamType() const TBAG_NOEXCEPT { return STREAM_TYPE; }

public:
    inline WriteState   getState    () const TBAG_NOEXCEPT { Guard g(_mutex); return _winfo.getState    (); }
    inline char const * getStateName() const TBAG_NOEXCEPT { Guard g(_mutex); return _winfo.getStateName(); }

    WeakClientBackend   getClient        () { Guard g(_mutex); return WeakClientBackend  (        _client); }
    WeakSafetyAsync     getAsync         () { Guard g(_mutex); return WeakSafetyAsync    (  _safety_async); }
    SharedUserTimer     getUserTimer     () { Guard g(_mutex); return SharedUserTimer    (    _user_timer); }
    SharedShutdownTimer getShutdownTimer () { Guard g(_mutex); return SharedShutdownTimer(_shutdown_timer); }

public:
    void * getUserData() TBAG_NOEXCEPT
    {
        Guard g(_mutex);
        if (static_cast<bool>(_client)) {
            return _client->getUserData();
        }
        return nullptr;
    }

    void setWriteTimeout(uint64_t millisec) TBAG_NOEXCEPT
    {
        Guard const LOCK(_mutex);
        _winfo.timeout = millisec;
    }

    Id getId() const
    {
        Guard const LOCK(_mutex);
        if (static_cast<bool>(_client)) {
            return _client->id();
        }
        return id::UNKNOWN_ID;
    }

    std::string dest() const
    {
        Guard const LOCK(_mutex);
        if (_client && STREAM_TYPE == StreamType::TCP) {
            return std::static_pointer_cast<uvpp::Tcp>(_client)->getPeerIp();
        }
        return std::string();
    }

    int port() const
    {
        Guard const LOCK(_mutex);
        if (_client && STREAM_TYPE == StreamType::TCP) {
            return std::static_pointer_cast<uvpp::Tcp>(_client)->getPeerPort();
        }
        return 0;
    }

public:
    Err initClient(StreamType type, std::string const & destination, int port = 0)
    {
        Guard const LOCK(_mutex);
        if (static_cast<bool>(_client) == false) {
            return Err::E_EXPIRED;
        }

        using TcpBackend  = StreamClientBackend<uvpp::Tcp>;
        using PipeBackend = StreamClientBackend<uvpp::Pipe>;

        if (type == StreamType::TCP) {
            auto backend = std::static_pointer_cast<TcpBackend>(_client);
            return uvpp::initCommonClient(*backend, _winfo.connect_req, destination, port);

        } else if (type == StreamType::PIPE) {
            auto backend = std::static_pointer_cast<PipeBackend>(_client);
            return uvpp::initPipeClient(*backend, _winfo.connect_req, destination);
        }

        tDLogA("StreamClient::Internal::initClient() Unknown stream type.");
        return Err::E_ILLARGS;
    }

    bool initInternalHandles()
    {
        Guard const LOCK(_mutex);
        Loop & loop = _getLoop();
        _safety_async   = loop.newInternalHandle<SafetyAsync  >(true, loop);
        _shutdown_timer = loop.newInternalHandle<ShutdownTimer>(true, loop, this);
        _user_timer     = loop.newInternalHandle<UserTimer    >(true, loop, this);
        return _safety_async && _shutdown_timer && _user_timer;
    }

    Err startRead()
    {
        Guard const LOCK(_mutex);
        Err const CODE = _client->startRead();
        if (TBAG_ERR_FAILURE(CODE)) {
            tDLogE("StreamClient::Internal::startRead() {} error", getErrName(CODE));
        }
        return CODE;
    }

    Err stopRead()
    {
        Guard const LOCK(_mutex);
        Err const CODE = _client->stopRead();
        if (TBAG_ERR_FAILURE(CODE)) {
            tDLogE("StreamClient::Internal::stopRead() {} error", getErrName(CODE));
        }
        return CODE;
    }

    bool isActiveUserTimer() const
    {
        Guard const LOCK(_mutex);
        return uvpp::isActiveTimer(_user_timer);
    }

    Err startUserTimer(uint64_t millisec)
    {
        Guard const LOCK(_mutex);
        return uvpp::startTimer(_user_timer, millisec);
    }

    Err stopUserTimer()
    {
        Guard const LOCK(_mutex);
        return uvpp::stopTimer(_user_timer);
    }

    Err autoWrite(char const * buffer, std::size_t size)
    {
        Guard const LOCK(_mutex);
        if (_winfo.isReady() == false) {
            tDLogE("StreamClient::Internal::autoWrite() Illegal state error: {}", _winfo.getStateName());
            return Err::E_ILLSTATE;
        }

        assert(static_cast<bool>(_client));
        assert(static_cast<bool>(_safety_async));

        Loop & loop = _getLoop();
        Err result_code = Err::E_UNKNOWN;

        if (_safety_async && loop.isAliveAndThisThread() == false) {
            result_code = _asyncWrite(buffer, size);
            if (TBAG_ERR_FAILURE(result_code)) {
                tDLogE("StreamClient::Internal::autoWrite() Async write {} error.", getErrName(result_code));
                return result_code;
            }
            result_code = Err::E_ENQASYNC;

        } else {
            result_code = _writeReal(buffer, size);
            if (TBAG_ERR_FAILURE(result_code)) {
                tDLogE("StreamClient::Internal::autoWrite() Direct write {} error.", getErrName(result_code));
                return result_code;
            }
            _winfo.setWrite();

            if (!_safety_async && loop.isAliveAndThisThread() == false) {
                tDLogW("StreamClient::Internal::autoWrite() Async is expired.");
                result_code = Err::E_WARNING;
            } else {
                result_code = Err::E_SUCCESS;
            }
        }

        if (_shutdown_timer && _winfo.timeout > 0) {
            if (uvpp::isActiveTimer(_shutdown_timer)) {
                uvpp::stopTimer(_shutdown_timer);
            }
            Err const CODE = uvpp::startTimer(_shutdown_timer, _winfo.timeout);
            if (TBAG_ERR_FAILURE(CODE)) {
                tDLogW("StreamClient::Internal::_autoWrite() Timer job {} error!", getErrName(CODE));
            }
        }

        return result_code;
    }

    Err close()
    {
        Guard const LOCK(_mutex);
        Loop & loop = _getLoop();

        if (_safety_async && loop.isAliveAndThisThread() == false) {
            tDLogD("StreamClient::Internal::close() Async request.");
            auto job = _safety_async->newSendFunc([&](){
                _closeClient();
            });
            if (static_cast<bool>(job)) {
                return Err::E_ASYNCREQ;
            }
            return Err::E_ESEND;
        }

        _closeClient();

        Err result_code = Err::E_SUCCESS;
        if (!_safety_async && loop.isAliveAndThisThread() == false) {
            tDLogW("StreamClient::Internal::close() Async is expired.");
            result_code = Err::E_WARNING;
        }

        tDLogD("StreamClient::Internal::close() Synced request.");
        return result_code;
    }

    Err cancel()
    {
        Guard const LOCK(_mutex);
        Loop & loop = _getLoop();

        if (_safety_async && loop.isAliveAndThisThread() == false) {
            tDLogD("StreamClient::Internal::cancel() Async request.");
            auto job = _safety_async->newSendFunc([&](){
                _shutdownWrite();
            });
            if (static_cast<bool>(job)) {
                return Err::E_ASYNCREQ;
            }
            return Err::E_ESEND;
        }

        _shutdownWrite();

        Err result_code = Err::E_SUCCESS;
        if (!_safety_async && loop.isAliveAndThisThread() == false) {
            tDLogW("StreamClient::Internal::cancel() Async is expired.");
            result_code = Err::E_WARNING;
        }

        tDLogD("StreamClient::Internal::cancel() Synced request.");
        return result_code;
    }

private:
    Loop & _getLoop()
    {
        assert(static_cast<bool>(_client));
        Loop * loop = _client->getLoop();
        assert(loop != nullptr);
        return *loop;
    }

    Err _writeReal(char const * buffer, std::size_t size)
    {
        assert(static_cast<bool>(_client));
        TBAG_STREAM_CLIENT_PRINT_BUFFER_IMPL("StreamClient::Internal::writeReal", buffer, size);

        Err const WRITE_CODE = _client->write(_winfo.write_req, buffer, size);
        if (TBAG_ERR_SUCCESS(WRITE_CODE)) {
            if (_winfo.cur_fail_count > 0) {
                tDLogI("StreamClient::Internal::_writeReal() Restore fail count.");
                _winfo.cur_fail_count = 0;
            }
            return Err::E_SUCCESS;
        }

        ++_winfo.cur_fail_count;
        if (_winfo.cur_fail_count < _winfo.max_fail_count) {
            tDLogW("StreamClient::Internal::_writeReal() Write {} error (FAIL_COUNT:{}/{}).",
                   getErrName(WRITE_CODE), _winfo.cur_fail_count, _winfo.max_fail_count);
        } else {
            tDLogE("StreamClient::Internal::_writeReal() Write {} error (FAIL_COUNT:{}/{}) => Force close!",
                   getErrName(WRITE_CODE), _winfo.cur_fail_count, _winfo.max_fail_count);
            _closeClient();
        }
        return WRITE_CODE;
    }

    Err _asyncWrite(char const * buffer, std::size_t size)
    {
        _winfo.buffer.assign(buffer, buffer + size);
        if (auto job = _safety_async->newSendJob<AsyncWrite>(this)) {
            _winfo.setAsync();
            return Err::E_SUCCESS;
        }
        return Err::E_ESEND;
    }

    Err _writeFromOnAsync()
    {
        assert(_parent != nullptr);
        if (_winfo.isAsyncCancel()) {
            tDLogW("StreamClient::Internal::onAsyncWrite() Cancel async write.");
            uvpp::stopTimer(_shutdown_timer);
            _winfo.setReady();
            return Err::E_ECANCELED;
        }

        if (_winfo.isAsync() == false) {
            tDLogE("StreamClient::Internal::onAsyncWrite() Error state: {}", _winfo.getStateName());
            uvpp::stopTimer(_shutdown_timer);
            _winfo.setReady();
            return Err::E_ILLSTATE;
        }

        assert(_winfo.isAsync());

        if (_winfo.buffer.empty()) {
            tDLogE("StreamClient::Internal::onAsyncWrite() Empty buffer");
            uvpp::stopTimer(_shutdown_timer);
            _winfo.setReady();
            return Err::E_EBUFFER;
        }

        Err const CODE = _writeReal(_winfo.buffer.data(), _winfo.buffer.size());
        if (TBAG_ERR_FAILURE(CODE)) {
            tDLogE("StreamClient::Internal::onAsyncWrite() Write {} error.", getErrName(CODE));
            uvpp::stopTimer(_shutdown_timer);
            _winfo.setReady();
            return CODE;
        }

        _winfo.setWrite();
        return Err::E_SUCCESS;
    }

    Err _shutdownWrite()
    {
        if (_winfo.isAsync()) {
            _winfo.setAsyncCancel();
            return Err::E_SUCCESS;
        }

        if (_winfo.isWrite() == false) {
            tDLogE("StreamClient::Internal::_shutdownWrite() Illegal state error: {}", _winfo.getStateName());
            return Err::E_ILLSTATE;
        }

        assert(_winfo.isWrite());

        Err const CODE = _client->shutdown(_winfo.shutdown_req);
        if (TBAG_ERR_FAILURE(CODE)) {
            tDLogE("StreamClient::Internal::_shutdownWrite() {} error", getErrName(CODE));
            return CODE;
        }

        _winfo.setShutdown();
        return Err::E_SUCCESS;
    }

    void _closeClient()
    {
        assert(static_cast<bool>(_client));
        if (_client->isClosing() == false) {
            _client->close();
        }
        _winfo.setClosing();
    }

    void _closeInternal()
    {
        // Close all internal handles.
        if (_safety_async && _safety_async->isClosing() == false) {
            _safety_async->close();
        }
        if (_user_timer && _user_timer->isClosing() == false) {
            _user_timer->close();
        }
        if (_shutdown_timer && _shutdown_timer->isClosing() == false) {
            _shutdown_timer->close();
        }
    }

protected:
    void onUserTimer()
    {
        assert(_parent != nullptr);
        _parent->onTimer();
    }

    void onShutdownTimer()
    {
        Guard const LOCK(_mutex);
        _shutdownWrite();
    }

    void onAsyncWrite()
    {
        _mutex.lock();
        Err const WRITE_CODE = _writeFromOnAsync();
        _mutex.unlock();

        if (TBAG_ERR_FAILURE(WRITE_CODE)) {
            assert(_parent != nullptr);
            tDLogD("StreamClient::Internal::onAsyncWrite() ~~> onWrite({}) event force call.", getErrName(WRITE_CODE));
            _parent->onWrite(WRITE_CODE);
        }
    }

public:
    void preConnect(Err code)
    {
        Guard const LOCK(_mutex);
        _winfo.setReady();
    }

    void preShutdown(Err code)
    {
        Guard const LOCK(_mutex);
        _winfo.setReady();
    }

    void preWrite(Err code)
    {
        Guard const LOCK(_mutex);
        uvpp::stopTimer(_shutdown_timer);
        assert(_winfo.isWrite() || _winfo.isShutdown());
        _winfo.setReady();
    }

    void preRead(Err code, ReadPacket const & packet)
    {
        // EMPTY.
    }

    void preClose()
    {
        Guard const LOCK(_mutex);
        _closeInternal();
        _winfo.setEnd();
    }
};

// ----------------------------
// StreamClient implementation.
// ----------------------------

StreamClient::StreamClient(Loop & loop, StreamType type)
        : _internal(new Internal(this, loop, type))
{
    assert(static_cast<bool>(_internal));
}

StreamClient::StreamClient(Loop & loop, StreamType type, WriteReady const & ready)
        : _internal(new Internal(this, loop, type, ready))
{
    assert(static_cast<bool>(_internal));
}

StreamClient::~StreamClient()
{
    assert(static_cast<bool>(_internal));
    _internal.reset();
}

StreamClient::WriteState StreamClient::getWriteState() const
{
    assert(static_cast<bool>(_internal));
    return _internal->getState();
}

char const * StreamClient::getWriteStateName() const
{
    assert(static_cast<bool>(_internal));
    return _internal->getStateName();
}

StreamClient::WeakClientBackend StreamClient::getClient()
{
    assert(static_cast<bool>(_internal));
    return _internal->getClient();
}

StreamClient::WeakSafetyAsync StreamClient::getAsync()
{
    assert(static_cast<bool>(_internal));
    return _internal->getAsync();
}

StreamClient::Id StreamClient::id() const
{
    assert(static_cast<bool>(_internal));
    return _internal->getId();
}

void * StreamClient::udata()
{
    assert(static_cast<bool>(_internal));
    return _internal->getUserData();
}

std::string StreamClient::dest() const
{
    assert(static_cast<bool>(_internal));
    return _internal->dest();
}

int StreamClient::port() const
{
    assert(static_cast<bool>(_internal));
    return _internal->port();
}

Err StreamClient::init(char const * destination, int port)
{
    assert(static_cast<bool>(_internal));

    Err const INIT_CODE = _internal->initClient(_internal->getStreamType(), std::string(destination), port);
    if (INIT_CODE != Err::E_SUCCESS) {
        tDLogE("StreamClient::init() Initialize fail.");
        return INIT_CODE;
    }

    if (_internal->initInternalHandles() == false) {
        tDLogE("StreamClient::init() Initialize fail (internal handles).");
        return Err::E_UNKNOWN;
    }

    return Err::E_SUCCESS;
}

Err StreamClient::start()
{
    assert(static_cast<bool>(_internal));
    return _internal->startRead();
}

Err StreamClient::stop()
{
    assert(static_cast<bool>(_internal));
    return _internal->stopRead();
}

Err StreamClient::close()
{
    assert(static_cast<bool>(_internal));
    return _internal->close();
}

Err StreamClient::cancel()
{
    assert(static_cast<bool>(_internal));
    return _internal->cancel();
}

Err StreamClient::write(char const * buffer, std::size_t size)
{
    assert(static_cast<bool>(_internal));
    return _internal->autoWrite(buffer, size);
}

void StreamClient::setWriteTimeout(uint64_t millisec)
{
    assert(static_cast<bool>(_internal));
    _internal->setWriteTimeout(millisec);
}

bool StreamClient::isActiveTimer()
{
    assert(static_cast<bool>(_internal));
    return _internal->isActiveUserTimer();
}

Err StreamClient::startTimer(uint64_t millisec)
{
    assert(static_cast<bool>(_internal));
    return _internal->startUserTimer(millisec);
}

void StreamClient::stopTimer()
{
    assert(static_cast<bool>(_internal));
    _internal->stopUserTimer();
}

// --------------
// Event backend.
// --------------

void StreamClient::backConnect(Err code)
{
    assert(static_cast<bool>(_internal));
    _internal->preConnect(code);
    onConnect(code);
}

void StreamClient::backShutdown(Err code)
{
    assert(static_cast<bool>(_internal));
    _internal->preShutdown(code);
    onShutdown(code);
}

void StreamClient::backWrite(Err code)
{
    assert(static_cast<bool>(_internal));
    _internal->preWrite(code);
    onWrite(code);
}

void StreamClient::backRead(Err code, ReadPacket const & packet)
{
    assert(static_cast<bool>(_internal));
    _internal->preRead(code, packet);
    onRead(code, packet);
}

void StreamClient::backClose()
{
    assert(static_cast<bool>(_internal));
    _internal->preClose();
    onClose();
}

} // namespace stream
} // namespace network

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

