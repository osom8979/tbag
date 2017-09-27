/**
 * @file   WriteStateClient.cpp
 * @brief  WriteStateClient class implementation.
 * @author zer0
 * @date   2017-09-24
 */

#include <libtbag/network/stream/client/WriteStateClient.hpp>
#include <libtbag/log/Log.hpp>
#include <libtbag/Type.hpp>

#include <libtbag/network/stream/StreamClientBackend.hpp>
#include <libtbag/string/StringUtils.hpp>

#include <cassert>

//#define TBAG_INTERNAL_CLIENT_PRINT_BUFFER

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace network {
namespace stream  {
namespace client  {

static void printBuffer(std::string const & name, uint8_t const * buffer, std::size_t size)
{
    std::string const BUFFER_STR = string::convertAddressToString(buffer);
    std::vector<std::uint8_t> const BUFFER_TO_VEC(buffer, buffer + size);
    std::string const MEMORY_STR = string::convertByteArrayToHexStringBox(BUFFER_TO_VEC);
    tDLogD("printBuffer(NAME:{}, BUFFER:{}, SIZE:{})\n", name, BUFFER_STR, MEMORY_STR);
}

#if defined(TBAG_INTERNAL_CLIENT_PRINT_BUFFER)
#define TBAG_INTERNAL_CLIENT_PRINT_BUFFER_IMPL(prefix, buffer, size) \
    ::libtbag::network::stream::client::printBuffer(prefix, (uint8_t const *)buffer, size)
#else
#define TBAG_INTERNAL_CLIENT_PRINT_BUFFER_IMPL(prefix, buffer, size)
#endif

// ------------------------------------------
// WriteStateClient::Internal implementation.
// ------------------------------------------

struct WriteStateClient::Internal : public BasicClientProperty
{
    StreamType const STREAM_TYPE;
    WriteStateClient * parent;
    Loop & loop;

    Internal(WriteStateClient * p, ClientInterface * interface, Loop & l, StreamType t) : parent(p), loop(l), STREAM_TYPE(t)
    {

        using  TcpBackend = StreamClientBackend<uvpp::Tcp>;
        using PipeBackend = StreamClientBackend<uvpp::Pipe>;

        if (STREAM_TYPE == StreamType::TCP) {
            client_stream = loop.newHandle<TcpBackend>(loop, interface);
        } else if (STREAM_TYPE == StreamType::PIPE) {
            client_stream = loop.newHandle<PipeBackend>(loop, interface);
        } else {
            throw std::bad_alloc();
        }

        assert(state == WriteState::WS_NOT_READY);
        assert(static_cast<bool>(client_stream));
    }

    ~Internal()
    {
        // EMPTY.
    }

    inline void setNotReady   () TBAG_NOEXCEPT { state = WriteState::WS_NOT_READY;    }
    inline void setReady      () TBAG_NOEXCEPT { state = WriteState::WS_READY;        }
    inline void setAsync      () TBAG_NOEXCEPT { state = WriteState::WS_ASYNC;        }
    inline void setAsyncCancel() TBAG_NOEXCEPT { state = WriteState::WS_ASYNC_CANCEL; }
    inline void setWrite      () TBAG_NOEXCEPT { state = WriteState::WS_WRITE;        }
    inline void setShutdown   () TBAG_NOEXCEPT { state = WriteState::WS_SHUTDOWN;     }
    inline void setClosing    () TBAG_NOEXCEPT { state = WriteState::WS_CLOSING;      }
    inline void setEnd        () TBAG_NOEXCEPT { state = WriteState::WS_END;          }

    inline bool isNotReady   () const TBAG_NOEXCEPT { return state == WriteState::WS_NOT_READY;    }
    inline bool isReady      () const TBAG_NOEXCEPT { return state == WriteState::WS_READY;        }
    inline bool isAsync      () const TBAG_NOEXCEPT { return state == WriteState::WS_ASYNC;        }
    inline bool isAsyncCancel() const TBAG_NOEXCEPT { return state == WriteState::WS_ASYNC_CANCEL; }
    inline bool isWrite      () const TBAG_NOEXCEPT { return state == WriteState::WS_WRITE;        }
    inline bool isShutdown   () const TBAG_NOEXCEPT { return state == WriteState::WS_SHUTDOWN;     }
    inline bool isClosing    () const TBAG_NOEXCEPT { return state == WriteState::WS_CLOSING;      }
    inline bool isEnd        () const TBAG_NOEXCEPT { return state == WriteState::WS_END;          }

    inline WriteState getState() const TBAG_NOEXCEPT { return state; }

    inline char const * getStateName() const TBAG_NOEXCEPT
    {
        return getWriteStateName(state);
    }

    inline bool isDisconnectedState() const TBAG_NOEXCEPT
    {
        return isNotReady() || isClosing() || isEnd();
    }

    Err initClient(std::string const & destination, int port)
    {
        if (static_cast<bool>(client_stream) == false) {
            return Err::E_EXPIRED;
        }

        using TcpBackend  = StreamClientBackend<uvpp::Tcp>;
        using PipeBackend = StreamClientBackend<uvpp::Pipe>;

        if (STREAM_TYPE == StreamType::TCP) {
            auto backend = std::static_pointer_cast<TcpBackend>(client_stream);
            return uvpp::initCommonClient(*backend, connect_req, destination, port);

        } else if (STREAM_TYPE == StreamType::PIPE) {
            auto backend = std::static_pointer_cast<PipeBackend>(client_stream);
            return uvpp::initPipeClient(*backend, connect_req, destination);
        }

        TBAG_INACCESSIBLE_BLOCK_ASSERT();
        tDLogA("WriteStateClient::Internal::initClient() Unknown stream type: {}", static_cast<int>(STREAM_TYPE));
        return Err::E_ILLARGS;
    }

    Err initInternalHandles()
    {
        safety_async   = loop.newInternalHandle<SafetyAsync  >(true, loop);
        shutdown_timer = loop.newInternalHandle<ShutdownTimer>(true, loop, parent);
        user_timer     = loop.newInternalHandle<UserTimer    >(true, loop, parent);

        if (safety_async && shutdown_timer && user_timer) {
            return Err::E_SUCCESS;
        }
        return Err::E_BADALLOC;
    }

    bool isActiveUserTimer() const
    {
        return uvpp::isActiveTimer(user_timer);
    }

    Err startUserTimer(uint64_t millisec)
    {
        return uvpp::startTimer(user_timer, millisec);
    }

    Err stopUserTimer()
    {
        return uvpp::stopTimer(user_timer);
    }

    bool isActiveShutdownTimer() const
    {
        if (isDisconnectedState()) {
            return false;
        }
        return uvpp::isActiveTimer(shutdown_timer);
    }

    Err startShutdownTimer(uint64_t millisec)
    {
        if (isDisconnectedState()) {
            return Err::E_ILLSTATE;
        }
        return uvpp::startTimer(shutdown_timer, millisec);
    }

    Err stopShutdownTimer()
    {
        if (isDisconnectedState()) {
            return Err::E_ILLSTATE;
        }
        return uvpp::stopTimer(shutdown_timer);
    }


    Err shutdownWrite()
    {
        if (isAsync()) {
            setAsyncCancel();
            return Err::E_SUCCESS;
        }

        if (isWrite() == false) {
            tDLogE("WriteStateClient::Internal::shutdownWrite() Illegal state error: {}", getStateName());
            return Err::E_ILLSTATE;
        }

        assert(isWrite());

        Err const CODE = client_stream->shutdown(shutdown_req);
        if (TBAG_ERR_FAILURE(CODE)) {
            tDLogE("WriteStateClient::Internal::shutdownWrite() {} error", getErrName(CODE));
            return CODE;
        }

        setShutdown();
        return Err::E_SUCCESS;
    }

    Err writeReal(char const * buffer, std::size_t size)
    {
        assert(static_cast<bool>(client_stream));
        TBAG_INTERNAL_CLIENT_PRINT_BUFFER_IMPL("WriteStateClient::_writeReal", buffer, size);

        Err const WRITE_CODE = client_stream->write(write_req, buffer, size);
        if (TBAG_ERR_SUCCESS(WRITE_CODE)) {
            if (cur_fail_count > 0) {
                tDLogI("WriteStateClient::Internal::writeReal() Restore fail count.");
                cur_fail_count = 0;
            }
            return Err::E_SUCCESS;
        }

        ++cur_fail_count;
        if (cur_fail_count < max_fail_count) {
            tDLogW("WriteStateClient::Internal::writeReal() Write {} error (FAIL_COUNT:{}/{}).",
                   getErrName(WRITE_CODE), cur_fail_count, max_fail_count);
        } else {
            tDLogE("WriteStateClient::Internal::writeReal() Write {} error (FAIL_COUNT:{}/{}) => Force close!",
                   getErrName(WRITE_CODE), cur_fail_count, max_fail_count);
            closeClient();
        }
        return WRITE_CODE;
    }

    Err writeFromQueue()
    {
        if (isReady() == false || queue.empty()) {
            return Err::E_ILLSTATE;
        }

        SharedBuffer buffer;
        Err const POP_CODE = queue.frontAndPop(buffer);
        if (TBAG_ERR_FAILURE(POP_CODE)) {
            tDLogE("WriteStateClient::Internal::writeFromQueue() Dequeue {} error", getErrName(POP_CODE));
            return POP_CODE;
        }

        tDLogD("WriteStateClient::Internal::writeFromQueue() Remaining queue size: {}", queue.size());

        if (static_cast<bool>(buffer) == false) {
            tDLogE("WriteStateClient::Internal::writeFromQueue() Expired buffer");
            return Err::E_EXPIRED;
        }

        if (buffer->empty()) {
            tDLogE("WriteStateClient::Internal::writeFromQueue() Empty current buffer");
            return Err::E_EBUFFER;
        }

        return autoWrite(buffer->data(), buffer->size());
    }

    Err writeFromOnAsync()
    {
        if (isClosing()) {
            tDLogW("WriteStateClient::Internal::writeFromOnAsync() Closing ...");
            return Err::E_CLOSING;
        } else if (isEnd()) {
            tDLogW("WriteStateClient::Internal::writeFromOnAsync() Client END.");
            return Err::E_CLOSED;
        }

        if (isAsyncCancel()) {
            tDLogW("WriteStateClient::Internal::writeFromOnAsync() Cancel async write.");
            stopShutdownTimer();
            setReady();
            return Err::E_ECANCELED;
        }

        if (isAsync() == false) {
            tDLogE("WriteStateClient::Internal::writeFromOnAsync() Error state: {}", getStateName());
            stopShutdownTimer();
            setReady();
            return Err::E_ILLSTATE;
        }

        assert(isAsync());

        if (wbuffer.empty()) {
            tDLogE("WriteStateClient::Internal::writeFromOnAsync() Empty buffer");
            stopShutdownTimer();
            setReady();
            return Err::E_EBUFFER;
        }

        Err const CODE = writeReal(wbuffer.data(), wbuffer.size());
        if (TBAG_ERR_FAILURE(CODE)) {
            tDLogE("WriteStateClient::Internal::writeFromOnAsync() Write {} error.", getErrName(CODE));
            stopShutdownTimer();
            setReady();
            return CODE;
        }

        setWrite();
        return Err::E_SUCCESS;
    }

    Err autoWrite(char const * buffer, std::size_t size)
    {
        if (isReady() == false) {
            tDLogE("WriteStateClient::Internal::autoWrite() Illegal state error: {}", getStateName());
            return Err::E_ILLSTATE;
        }

        assert(static_cast<bool>(client_stream));
        assert(static_cast<bool>(safety_async));

        Err result_code = Err::E_UNKNOWN;

        if (safety_async && loop.isAliveAndThisThread() == false) {
            wbuffer.assign(buffer, buffer + size);
            if (auto job = safety_async->newSendJob<AsyncWrite>(parent)) {
                setAsync();
                result_code = Err::E_ENQASYNC;
            } else {
                tDLogE("WriteStateClient::Internal::autoWrite() Async write {} error.", getErrName(result_code));
                return Err::E_ESEND;
            }

        } else {
            result_code = writeReal(buffer, size);
            if (TBAG_ERR_FAILURE(result_code)) {
                tDLogE("WriteStateClient::Internal::autoWrite() Direct write {} error.", getErrName(result_code));
                return result_code;
            }
            setWrite();

            if (!safety_async && loop.isAliveAndThisThread() == false) {
                tDLogW("WriteStateClient::Internal::autoWrite() Async is expired.");
                result_code = Err::E_WARNING;
            } else {
                result_code = Err::E_SUCCESS;
            }
        }

        if (shutdown_timer && timeout > 0) {
            if (isActiveShutdownTimer()) {
                stopShutdownTimer();
            }
            Err const CODE = startShutdownTimer(timeout);
            if (TBAG_ERR_FAILURE(CODE)) {
                tDLogW("WriteStateClient::Internal::autoWrite() Timer job {} error!", getErrName(CODE));
            }
        }

        return result_code;
    }

    Err writeOrEnqueueOrAsync(char const * buffer, std::size_t size)
    {
        if (isDisconnectedState()) {
            return Err::E_ILLSTATE;
        }

        if (isReady()) {
            return autoWrite(buffer, size);
        }

        if (queue.size() >= max_queue_size) {
            tDLogE("WriteStateClient::Internal::write() The queue is full ({})", max_queue_size);
            return Err::E_EBUSY;
        }

        auto & back = queue.push();
        if (static_cast<bool>(back)) {
            back->assign(buffer, buffer + size);
        } else {
            try {
                back.reset(new Buffer(buffer, buffer + size));
            } catch (...) {
                tDLogE("WriteStateClient::Internal::write() Queue alloc fail.");
                return Err::E_BADALLOC;
            }
        }
        return Err::E_ENQASYNC;
    }

    Err closeClient()
    {
        assert(static_cast<bool>(client_stream));
        if (client_stream->isClosing() == false) {
            client_stream->close();
        }
        if (isEnd() == false) {
            setClosing();
        }
        return Err::E_SUCCESS;
    }

    Err closeInternal()
    {
        // Close all internal handles.
        if (safety_async && safety_async->isClosing() == false) {
            safety_async->close();
        }
        if (user_timer && user_timer->isClosing() == false) {
            user_timer->close();
        }
        if (shutdown_timer && shutdown_timer->isClosing() == false) {
            shutdown_timer->close();
        }
        return Err::E_SUCCESS;
    }
};

// --------------------------------
// WriteStateClient implementation.
// --------------------------------

WriteStateClient::WriteStateClient(ClientInterface * parent, Loop & loop, StreamType type)
        : _internal(new Internal(this, parent, loop, type)), _parent(parent), _user_data(nullptr)
{
    assert(static_cast<bool>(_internal));
}

WriteStateClient::WriteStateClient(ClientInterface * parent, Loop & loop, StreamType type, UpdateReady const & UNUSED_PARAM(ready))
        : WriteStateClient(parent, loop, type)
{
    assert(static_cast<bool>(_internal));
    if (TBAG_ERR_FAILURE(initInternalHandles())) {
        throw std::bad_alloc();
    }
    _internal->setReady();
}

WriteStateClient::~WriteStateClient()
{
    // EMPTY.
}

WriteStateClient::StreamType WriteStateClient::getStreamType() const TBAG_NOEXCEPT
{
    return _internal->STREAM_TYPE;
}

WriteState WriteStateClient::getState() const
{
    Guard const LOCK(_mutex);
    return _internal->getState();
}

char const * WriteStateClient::getStateName() const
{
    Guard const LOCK(_mutex);
    return _internal->getStateName();
}

uint64_t WriteStateClient::getWriteTimeout(uint64_t millisec) const
{
    Guard const LOCK(_mutex);
    return _internal->timeout;
}

void WriteStateClient::setWriteTimeout(uint64_t millisec)
{
    Guard const LOCK(_mutex);
    _internal->timeout = millisec;
}

// @formatter:off
WriteStateClient::WeakStream          WriteStateClient::getClient       () { Guard g(_mutex); return _internal->client_stream;  }
WriteStateClient::WeakSafetyAsync     WriteStateClient::getAsync        () { Guard g(_mutex); return _internal->safety_async;   }
WriteStateClient::SharedUserTimer     WriteStateClient::getUserTimer    () { Guard g(_mutex); return _internal->user_timer;     }
WriteStateClient::SharedShutdownTimer WriteStateClient::getShutdownTimer() { Guard g(_mutex); return _internal->shutdown_timer; }
// @formatter:on

WriteStateClient::Id WriteStateClient::id() const
{
    Guard const LOCK(_mutex);
    if (static_cast<bool>(_internal->client_stream) == false) {
        tDLogW("WriteStateClient::id() Expired client.");
        return id::UNKNOWN_ID;
    }
    return _internal->client_stream->id();
}

std::string WriteStateClient::dest() const
{
    Guard const LOCK(_mutex);
    if (static_cast<bool>(_internal->client_stream) == false) {
        tDLogW("WriteStateClient::dest() Expired client.");
        return std::string();
    }

    if (_internal->STREAM_TYPE == StreamType::TCP) {
        return std::static_pointer_cast<uvpp::Tcp>(_internal->client_stream)->getSockIp();
    } else if (_internal->STREAM_TYPE == StreamType::PIPE) {
        return std::static_pointer_cast<uvpp::Pipe>(_internal->client_stream)->getSockName();
    }

    TBAG_INACCESSIBLE_BLOCK_ASSERT();
    tDLogW("WriteStateClient::dest() Unknown stream type: {}", static_cast<int>(_internal->STREAM_TYPE));
    return std::string();
}

int WriteStateClient::port() const
{
    Guard const LOCK(_mutex);
    if (static_cast<bool>(_internal->client_stream) == false) {
        tDLogW("WriteStateClient::port() Expired client.");
        return 0;
    }

    if (_internal->STREAM_TYPE == StreamType::TCP) {
        return std::static_pointer_cast<uvpp::Tcp>(_internal->client_stream)->getSockPort();
    }
    return 0;
}

Err WriteStateClient::initClient(std::string const & destination, int port)
{
    Guard const LOCK(_mutex);
    if (_internal->isNotReady()) {
        return _internal->initClient(destination, port);
    }
    return Err::E_ILLSTATE;
}

Err WriteStateClient::initInternalHandles()
{
    Guard const LOCK(_mutex);
    if (_internal->isNotReady()) {
        return _internal->initInternalHandles();
    }
    return Err::E_ILLSTATE;
}

Err WriteStateClient::startRead()
{
    Guard const LOCK(_mutex);
    if (_internal->isDisconnectedState()) {
        return Err::E_ILLSTATE;
    }
    Err const CODE = _internal->client_stream->startRead();
    if (TBAG_ERR_FAILURE(CODE)) {
        tDLogE("WriteStateClient::startRead() Start {} error", getErrName(CODE));
    }
    return CODE;
}

Err WriteStateClient::stopRead()
{
    Guard const LOCK(_mutex);
    if (_internal->isDisconnectedState()) {
        return Err::E_ILLSTATE;
    }
    Err const CODE = _internal->client_stream->stopRead();
    if (TBAG_ERR_FAILURE(CODE)) {
        tDLogE("WriteStateClient::stopRead() Stop {} error", getErrName(CODE));
    }
    return CODE;
}

bool WriteStateClient::isActiveUserTimer() const
{
    Guard const LOCK(_mutex);
    return _internal->isActiveUserTimer();
}

Err WriteStateClient::startUserTimer(uint64_t millisec)
{
    Guard const LOCK(_mutex);
    return _internal->startUserTimer(millisec);
}

Err WriteStateClient::stopUserTimer()
{
    Guard const LOCK(_mutex);
    return _internal->stopUserTimer();
}

Err WriteStateClient::close()
{
    Guard const LOCK(_mutex);
    if (_internal->safety_async && _internal->loop.isAliveAndThisThread() == false) {
        auto job = _internal->safety_async->newSendFunc([&](){
            Guard const ASYNC_LOCK(_mutex);
            _internal->closeClient();
        });
        return static_cast<bool>(job) ? Err::E_ASYNCREQ : Err::E_ESEND;
    }
    return _internal->closeClient();
}

Err WriteStateClient::cancel()
{
    Guard const LOCK(_mutex);
    if (_internal->isDisconnectedState()) {
        return Err::E_ILLSTATE;
    }
    if (_internal->safety_async && _internal->loop.isAliveAndThisThread() == false) {
        auto job = _internal->safety_async->newSendFunc([&](){
            Guard const ASYNC_LOCK(_mutex);
            _internal->shutdownWrite();
        });
        return static_cast<bool>(job) ? Err::E_ASYNCREQ : Err::E_ESEND;
    }
    return _internal->shutdownWrite();
}

Err WriteStateClient::write(char const * buffer, std::size_t size)
{
    Guard const LOCK(_mutex);
    return _internal->writeOrEnqueueOrAsync(buffer, size);
}

void WriteStateClient::onUserTimer()
{
    assert(_parent != nullptr);
    _parent->onTimer();
}

void WriteStateClient::onShutdownTimer()
{
    Guard const LOCK(_mutex);
    _internal->shutdownWrite();
}

void WriteStateClient::onAsyncWrite()
{
    _mutex.lock();
    Err const WRITE_CODE = _internal->writeFromOnAsync();
    _mutex.unlock();

    if (TBAG_ERR_FAILURE(WRITE_CODE)) {
        assert(_parent != nullptr);
        tDLogD("WriteStateClient::onAsyncWrite() ~~> onShutdown({}) event force call.", getErrName(WRITE_CODE));
        _parent->onShutdown(WRITE_CODE);
    }
}

void WriteStateClient::preConnect(Err code)
{
    Guard const LOCK(_mutex);
    _internal->setReady();
}

void WriteStateClient::preShutdown(Err code)
{
    Guard const LOCK(_mutex);
    if (_internal->isDisconnectedState()) {
        return;
    }

    _internal->setReady();
    if (_internal->queue.empty() == false) {
        _internal->writeFromQueue();
    }
}

void WriteStateClient::preWrite(Err code)
{
    Guard const LOCK(_mutex);
    if (code == Err::E_EOF) {
        tDLogD("WriteStateClient::preWrite() END OF FILE.");
        return;
    }

    if (_internal->isDisconnectedState()) {
        return;
    }

    assert(_internal->isWrite() || _internal->isShutdown());
    _internal->setReady();
    _internal->stopShutdownTimer();
    if (_internal->queue.empty() == false) {
        _internal->writeFromQueue();
    }
}

void WriteStateClient::preRead(Err code, ReadPacket const & packet)
{
    // EMPTY.
}

void WriteStateClient::preClose()
{
    Guard const LOCK(_mutex);
    _internal->closeInternal();
    _internal->setEnd();
}

} // namespace client
} // namespace stream
} // namespace network

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

