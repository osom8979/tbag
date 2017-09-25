/**
 * @file   InternalClient.cpp
 * @brief  InternalClient class implementation.
 * @author zer0
 * @date   2017-09-24
 */

#include <libtbag/network/stream/InternalClient.hpp>
#include <libtbag/log/Log.hpp>

#include <libtbag/network/stream/StreamClientBackend.hpp>
#include <libtbag/string/StringUtils.hpp>

#include <cassert>

//#define TBAG_INTERNAL_CLIENT_PRINT_BUFFER

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace network {
namespace stream  {

static void printBufferOfStreamBuffer(std::string const & prefix, uint8_t const * buffer, std::size_t size)
{
    tDLogD("printBufferOfStreamBuffer({}) SIZE:{}n{}",
           prefix, size, string::convertByteArrayToHexStringBox(std::vector<std::uint8_t>(buffer, buffer + size)));
}

#if defined(TBAG_INTERNAL_CLIENT_PRINT_BUFFER)
#define TBAG_INTERNAL_CLIENT_PRINT_BUFFER_IMPL(prefix, buffer, size)
    ::libtbag::network::stream::printBufferOfStreamBuffer(prefix, (uint8_t const *)buffer, size)
#else
#define TBAG_INTERNAL_CLIENT_PRINT_BUFFER_IMPL(prefix, buffer, size)
#endif

InternalClient::InternalClient(ClientInterface * parent, Loop & loop, StreamType type)
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

InternalClient::InternalClient(ClientInterface * parent,
                               Loop & loop,
                               StreamType type,
                               WriteReady const & UNUSED_PARAM(ready))
        : InternalClient(parent, loop, type)
{
    if (TBAG_ERR_FAILURE(initInternalHandles())) {
        throw std::bad_alloc();
    }
    _winfo.setReady();
}

InternalClient::~InternalClient()
{
    // EMPTY.
}

void * InternalClient::getUserData() TBAG_NOEXCEPT
{
    Guard g(_mutex);
    if (static_cast<bool>(_client)) {
        return _client->getUserData();
    }
    return nullptr;
}

void InternalClient::setWriteTimeout(uint64_t millisec) TBAG_NOEXCEPT
{
    Guard const LOCK(_mutex);
    _winfo.timeout = millisec;
}

InternalClient::Id InternalClient::getId() const
{
    Guard const LOCK(_mutex);
    if (static_cast<bool>(_client)) {
        return _client->id();
    }
    return id::UNKNOWN_ID;
}

std::string InternalClient::dest() const
{
    Guard const LOCK(_mutex);
    if (_client && STREAM_TYPE == StreamType::TCP) {
        return std::static_pointer_cast<uvpp::Tcp>(_client)->getPeerIp();
    }
    return std::string();
}

int InternalClient::port() const
{
    Guard const LOCK(_mutex);
    if (_client && STREAM_TYPE == StreamType::TCP) {
        return std::static_pointer_cast<uvpp::Tcp>(_client)->getPeerPort();
    }
    return 0;
}

Err InternalClient::initClient(StreamType type, std::string const & destination, int port)
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

    tDLogA("InternalClient::initClient() Unknown stream type.");
    return Err::E_ILLARGS;
}

Err InternalClient::initInternalHandles()
{
    Guard const LOCK(_mutex);
    Loop & loop = _getLoop();
    _safety_async   = loop.newInternalHandle<SafetyAsync  >(true, loop);
    _shutdown_timer = loop.newInternalHandle<ShutdownTimer>(true, loop, this);
    _user_timer     = loop.newInternalHandle<UserTimer    >(true, loop, this);

    if (_safety_async && _shutdown_timer && _user_timer) {
        return Err::E_SUCCESS;
    }
    return Err::E_BADALLOC;
}

Err InternalClient::startRead()
{
    Guard const LOCK(_mutex);
    Err const CODE = _client->startRead();
    if (TBAG_ERR_FAILURE(CODE)) {
        tDLogE("InternalClient::startRead() {} error", getErrName(CODE));
    }
    return CODE;
}

Err InternalClient::stopRead()
{
    Guard const LOCK(_mutex);
    Err const CODE = _client->stopRead();
    if (TBAG_ERR_FAILURE(CODE)) {
        tDLogE("InternalClient::stopRead() {} error", getErrName(CODE));
    }
    return CODE;
}

bool InternalClient::isActiveUserTimer() const
{
    Guard const LOCK(_mutex);
    return uvpp::isActiveTimer(_user_timer);
}

Err InternalClient::startUserTimer(uint64_t millisec)
{
    Guard const LOCK(_mutex);
    return uvpp::startTimer(_user_timer, millisec);
}

Err InternalClient::stopUserTimer()
{
    Guard const LOCK(_mutex);
    return uvpp::stopTimer(_user_timer);
}

Err InternalClient::close()
{
    Guard const LOCK(_mutex);
    Loop & loop = _getLoop();

    if (_safety_async && loop.isAliveAndThisThread() == false) {
        tDLogD("InternalClient::close() Async request.");
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
        tDLogW("InternalClient::close() Async is expired.");
        result_code = Err::E_WARNING;
    }

    tDLogD("InternalClient::close() Synced request.");
    return result_code;
}

Err InternalClient::cancel()
{
    Guard const LOCK(_mutex);
    Loop & loop = _getLoop();

    if (_safety_async && loop.isAliveAndThisThread() == false) {
        tDLogD("InternalClient::cancel() Async request.");
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
        tDLogW("InternalClient::cancel() Async is expired.");
        result_code = Err::E_WARNING;
    }

    tDLogD("InternalClient::cancel() Synced request.");
    return result_code;
}

InternalClient::Loop & InternalClient::_getLoop()
{
    assert(static_cast<bool>(_client));
    Loop * loop = _client->getLoop();
    assert(loop != nullptr);
    return *loop;
}

Err InternalClient::_writeReal(char const * buffer, std::size_t size)
{
    assert(static_cast<bool>(_client));
    TBAG_INTERNAL_CLIENT_PRINT_BUFFER_IMPL("InternalClient::writeReal", buffer, size);

    Err const WRITE_CODE = _client->write(_winfo.write_req, buffer, size);
    if (TBAG_ERR_SUCCESS(WRITE_CODE)) {
        if (_winfo.cur_fail_count > 0) {
            tDLogI("InternalClient::_writeReal() Restore fail count.");
            _winfo.cur_fail_count = 0;
        }
        return Err::E_SUCCESS;
    }

    ++_winfo.cur_fail_count;
    if (_winfo.cur_fail_count < _winfo.max_fail_count) {
        tDLogW("InternalClient::_writeReal() Write {} error (FAIL_COUNT:{}/{}).",
               getErrName(WRITE_CODE), _winfo.cur_fail_count, _winfo.max_fail_count);
    } else {
        tDLogE("InternalClient::_writeReal() Write {} error (FAIL_COUNT:{}/{}) => Force close!",
               getErrName(WRITE_CODE), _winfo.cur_fail_count, _winfo.max_fail_count);
        _closeClient();
    }
    return WRITE_CODE;
}

Err InternalClient::_autoWrite(char const * buffer, std::size_t size)
{
    if (_winfo.isReady() == false) {
        tDLogE("InternalClient::autoWrite() Illegal state error: {}", _winfo.getStateName());
        return Err::E_ILLSTATE;
    }

    assert(static_cast<bool>(_client));
    assert(static_cast<bool>(_safety_async));

    Loop & loop = _getLoop();
    Err result_code = Err::E_UNKNOWN;

    if (_safety_async && loop.isAliveAndThisThread() == false) {
        _winfo.buffer.assign(buffer, buffer + size);
        if (auto job = _safety_async->newSendJob<AsyncWrite>(this)) {
            _winfo.setAsync();
            result_code = Err::E_ENQASYNC;
        } else {
            tDLogE("InternalClient::autoWrite() Async write {} error.", getErrName(result_code));
            return Err::E_ESEND;
        }

    } else {
        result_code = _writeReal(buffer, size);
        if (TBAG_ERR_FAILURE(result_code)) {
            tDLogE("InternalClient::autoWrite() Direct write {} error.", getErrName(result_code));
            return result_code;
        }
        _winfo.setWrite();

        if (!_safety_async && loop.isAliveAndThisThread() == false) {
            tDLogW("InternalClient::autoWrite() Async is expired.");
            result_code = Err::E_WARNING;
        } else {
            result_code = Err::E_SUCCESS;
        }
    }

    if (_shutdown_timer && _winfo.timeout > 0) {
        if (_isActiveShutdownTimer()) {
            _stopShutdownTimer();
        }
        Err const CODE = _startShutdownTimer(_winfo.timeout);
        if (TBAG_ERR_FAILURE(CODE)) {
            tDLogW("InternalClient::_autoWrite() Timer job {} error!", getErrName(CODE));
        }
    }

    return result_code;
}

Err InternalClient::_writeFromOnAsync()
{
    if (_winfo.isAsyncCancel()) {
        tDLogW("InternalClient::onAsyncWrite() Cancel async write.");
        _stopShutdownTimer();
        _winfo.setReady();
        return Err::E_ECANCELED;
    }

    if (_winfo.isAsync() == false) {
        tDLogE("InternalClient::onAsyncWrite() Error state: {}", _winfo.getStateName());
        _stopShutdownTimer();
        _winfo.setReady();
        return Err::E_ILLSTATE;
    }

    assert(_winfo.isAsync());

    if (_winfo.buffer.empty()) {
        tDLogE("InternalClient::onAsyncWrite() Empty buffer");
        _stopShutdownTimer();
        _winfo.setReady();
        return Err::E_EBUFFER;
    }

    Err const CODE = _writeReal(_winfo.buffer.data(), _winfo.buffer.size());
    if (TBAG_ERR_FAILURE(CODE)) {
        tDLogE("InternalClient::onAsyncWrite() Write {} error.", getErrName(CODE));
        _stopShutdownTimer();
        _winfo.setReady();
        return CODE;
    }

    _winfo.setWrite();
    return Err::E_SUCCESS;
}

Err InternalClient::_shutdownWrite()
{
    if (_winfo.isAsync()) {
        _winfo.setAsyncCancel();
        return Err::E_SUCCESS;
    }

    if (_winfo.isWrite() == false) {
        tDLogE("InternalClient::_shutdownWrite() Illegal state error: {}", _winfo.getStateName());
        return Err::E_ILLSTATE;
    }

    assert(_winfo.isWrite());

    Err const CODE = _client->shutdown(_winfo.shutdown_req);
    if (TBAG_ERR_FAILURE(CODE)) {
        tDLogE("InternalClient::_shutdownWrite() {} error", getErrName(CODE));
        return CODE;
    }

    _winfo.setShutdown();
    return Err::E_SUCCESS;
}

bool InternalClient::_isActiveShutdownTimer() const
{
    return uvpp::isActiveTimer(_shutdown_timer);
}

Err InternalClient::_startShutdownTimer(uint64_t millisec)
{
    return uvpp::startTimer(_shutdown_timer, millisec);
}

Err InternalClient::_stopShutdownTimer()
{
    return uvpp::stopTimer(_shutdown_timer);
}

void InternalClient::_closeClient()
{
    assert(static_cast<bool>(_client));
    if (_client->isClosing() == false) {
        _client->close();
    }
    _winfo.setClosing();
}

void InternalClient::_closeInternal()
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

void InternalClient::onUserTimer()
{
    assert(_parent != nullptr);
    _parent->onTimer();
}

void InternalClient::onShutdownTimer()
{
    Guard const LOCK(_mutex);
    _shutdownWrite();
}

void InternalClient::onAsyncWrite()
{
    _mutex.lock();
    Err const WRITE_CODE = _writeFromOnAsync();
    _mutex.unlock();

    if (TBAG_ERR_FAILURE(WRITE_CODE)) {
        assert(_parent != nullptr);
        tDLogD("InternalClient::onAsyncWrite() ~~> onShutdown({}) event force call.",
               getErrName(WRITE_CODE));
        _parent->onShutdown(WRITE_CODE);
    }
}

void InternalClient::preConnect(Err code)
{
    Guard const LOCK(_mutex);
    if (_winfo.isClosing() || _winfo.isEnd()) {
        return;
    }
    _winfo.setReady();
}

void InternalClient::preShutdown(Err code)
{
    Guard const LOCK(_mutex);
    if (_winfo.isClosing() || _winfo.isEnd()) {
        return;
    }
    _winfo.setReady();
}

void InternalClient::preWrite(Err code)
{
    Guard const LOCK(_mutex);
    if (_winfo.isClosing() || _winfo.isEnd()) {
        return;
    }
    assert(_winfo.isWrite() || _winfo.isShutdown());
    _winfo.setReady();
    _stopShutdownTimer();
}

void InternalClient::preRead(Err code, ReadPacket const & packet)
{
    // EMPTY.
}

void InternalClient::preClose()
{
    Guard const LOCK(_mutex);
    _closeInternal();
    _winfo.setEnd();
}

} // namespace stream
} // namespace network

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

