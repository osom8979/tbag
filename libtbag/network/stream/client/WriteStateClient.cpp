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

WriteStateClient::WriteStateClient(ClientInterface * parent, Loop & loop, StreamType type)
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

WriteStateClient::WriteStateClient(ClientInterface * parent,
                               Loop & loop,
                               StreamType type,
                               WriteReady const & UNUSED_PARAM(ready))
        : WriteStateClient(parent, loop, type)
{
    if (TBAG_ERR_FAILURE(initInternalHandles())) {
        throw std::bad_alloc();
    }
    _winfo.setReady();
}

WriteStateClient::~WriteStateClient()
{
    // EMPTY.
}

void * WriteStateClient::getUserData() TBAG_NOEXCEPT
{
    Guard g(_mutex);
    if (static_cast<bool>(_client)) {
        return _client->getUserData();
    }
    return nullptr;
}

void WriteStateClient::setWriteTimeout(uint64_t millisec) TBAG_NOEXCEPT
{
    Guard const LOCK(_mutex);
    _winfo.timeout = millisec;
}

WriteStateClient::Id WriteStateClient::getId() const
{
    Guard const LOCK(_mutex);
    if (static_cast<bool>(_client) == false) {
        tDLogW("WriteStateClient::getId() Expired client.");
        return id::UNKNOWN_ID;
    }
    return _client->id();
}

std::string WriteStateClient::dest() const
{
    Guard const LOCK(_mutex);
    if (static_cast<bool>(_client) == false) {
        tDLogW("WriteStateClient::dest() Expired client.");
        return std::string();
    }

    if (STREAM_TYPE == StreamType::TCP) {
        return std::static_pointer_cast<uvpp::Tcp>(_client)->getSockIp();
    } else if (STREAM_TYPE == StreamType::PIPE) {
        return std::static_pointer_cast<uvpp::Pipe>(_client)->getSockName();
    }

    TBAG_INACCESSIBLE_BLOCK_ASSERT();
    tDLogW("WriteStateClient::dest() Unknown stream type: {}", static_cast<int>(STREAM_TYPE));
    return std::string();
}

int WriteStateClient::port() const
{
    Guard const LOCK(_mutex);
    if (static_cast<bool>(_client) == false) {
        tDLogW("WriteStateClient::port() Expired client.");
        return 0;
    }

    if (STREAM_TYPE == StreamType::TCP) {
        return std::static_pointer_cast<uvpp::Tcp>(_client)->getSockPort();
    }
    return 0;
}

Err WriteStateClient::initClient(StreamType type, std::string const & destination, int port)
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

    tDLogA("WriteStateClient::initClient() Unknown stream type.");
    return Err::E_ILLARGS;
}

Err WriteStateClient::initInternalHandles()
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

Err WriteStateClient::startRead()
{
    Guard const LOCK(_mutex);
    Err const CODE = _client->startRead();
    if (TBAG_ERR_FAILURE(CODE)) {
        tDLogE("WriteStateClient::startRead() {} error", getErrName(CODE));
    }
    return CODE;
}

Err WriteStateClient::stopRead()
{
    Guard const LOCK(_mutex);
    Err const CODE = _client->stopRead();
    if (TBAG_ERR_FAILURE(CODE)) {
        tDLogE("WriteStateClient::stopRead() {} error", getErrName(CODE));
    }
    return CODE;
}

bool WriteStateClient::isActiveUserTimer() const
{
    Guard const LOCK(_mutex);
    return uvpp::isActiveTimer(_user_timer);
}

Err WriteStateClient::startUserTimer(uint64_t millisec)
{
    Guard const LOCK(_mutex);
    return uvpp::startTimer(_user_timer, millisec);
}

Err WriteStateClient::stopUserTimer()
{
    Guard const LOCK(_mutex);
    return uvpp::stopTimer(_user_timer);
}

Err WriteStateClient::close()
{
    Guard const LOCK(_mutex);
    Loop & loop = _getLoop();

    if (_safety_async && loop.isAliveAndThisThread() == false) {
        tDLogD("WriteStateClient::close() Async request.");
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
        tDLogW("WriteStateClient::close() Async is expired.");
        result_code = Err::E_WARNING;
    }

    tDLogD("WriteStateClient::close() Synced request.");
    return result_code;
}

Err WriteStateClient::cancel()
{
    Guard const LOCK(_mutex);
    Loop & loop = _getLoop();

    if (_safety_async && loop.isAliveAndThisThread() == false) {
        tDLogD("WriteStateClient::cancel() Async request.");
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
        tDLogW("WriteStateClient::cancel() Async is expired.");
        result_code = Err::E_WARNING;
    }

    tDLogD("WriteStateClient::cancel() Synced request.");
    return result_code;
}

WriteStateClient::Loop & WriteStateClient::_getLoop()
{
    assert(static_cast<bool>(_client));
    Loop * loop = _client->getLoop();
    assert(loop != nullptr);
    return *loop;
}

Err WriteStateClient::_writeReal(char const * buffer, std::size_t size)
{
    assert(static_cast<bool>(_client));
    TBAG_INTERNAL_CLIENT_PRINT_BUFFER_IMPL("WriteStateClient::_writeReal", buffer, size);

    Err const WRITE_CODE = _client->write(_winfo.write_req, buffer, size);
    if (TBAG_ERR_SUCCESS(WRITE_CODE)) {
        if (_winfo.cur_fail_count > 0) {
            tDLogI("WriteStateClient::_writeReal() Restore fail count.");
            _winfo.cur_fail_count = 0;
        }
        return Err::E_SUCCESS;
    }

    ++_winfo.cur_fail_count;
    if (_winfo.cur_fail_count < _winfo.max_fail_count) {
        tDLogW("WriteStateClient::_writeReal() Write {} error (FAIL_COUNT:{}/{}).",
               getErrName(WRITE_CODE), _winfo.cur_fail_count, _winfo.max_fail_count);
    } else {
        tDLogE("WriteStateClient::_writeReal() Write {} error (FAIL_COUNT:{}/{}) => Force close!",
               getErrName(WRITE_CODE), _winfo.cur_fail_count, _winfo.max_fail_count);
        _closeClient();
    }
    return WRITE_CODE;
}

Err WriteStateClient::_autoWrite(char const * buffer, std::size_t size)
{
    if (_winfo.isReady() == false) {
        tDLogE("WriteStateClient::_autoWrite() Illegal state error: {}", _winfo.getStateName());
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
            tDLogE("WriteStateClient::_autoWrite() Async write {} error.", getErrName(result_code));
            return Err::E_ESEND;
        }

    } else {
        result_code = _writeReal(buffer, size);
        if (TBAG_ERR_FAILURE(result_code)) {
            tDLogE("WriteStateClient::_autoWrite() Direct write {} error.", getErrName(result_code));
            return result_code;
        }
        _winfo.setWrite();

        if (!_safety_async && loop.isAliveAndThisThread() == false) {
            tDLogW("WriteStateClient::_autoWrite() Async is expired.");
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
            tDLogW("WriteStateClient::_autoWrite() Timer job {} error!", getErrName(CODE));
        }
    }

    return result_code;
}

Err WriteStateClient::_writeFromOnAsync()
{
    if (_winfo.isClosing()) {
        tDLogW("WriteStateClient::_writeFromOnAsync() Closing ...");
        return Err::E_CLOSING;
    } else if (_winfo.isEnd()) {
        tDLogW("WriteStateClient::_writeFromOnAsync() Client END.");
        return Err::E_CLOSED;
    }

    if (_winfo.isAsyncCancel()) {
        tDLogW("WriteStateClient::_writeFromOnAsync() Cancel async write.");
        _stopShutdownTimer();
        _winfo.setReady();
        return Err::E_ECANCELED;
    }

    if (_winfo.isAsync() == false) {
        tDLogE("WriteStateClient::_writeFromOnAsync() Error state: {}", _winfo.getStateName());
        _stopShutdownTimer();
        _winfo.setReady();
        return Err::E_ILLSTATE;
    }

    assert(_winfo.isAsync());

    if (_winfo.buffer.empty()) {
        tDLogE("WriteStateClient::_writeFromOnAsync() Empty buffer");
        _stopShutdownTimer();
        _winfo.setReady();
        return Err::E_EBUFFER;
    }

    Err const CODE = _writeReal(_winfo.buffer.data(), _winfo.buffer.size());
    if (TBAG_ERR_FAILURE(CODE)) {
        tDLogE("WriteStateClient::_writeFromOnAsync() Write {} error.", getErrName(CODE));
        _stopShutdownTimer();
        _winfo.setReady();
        return CODE;
    }

    _winfo.setWrite();
    return Err::E_SUCCESS;
}

Err WriteStateClient::_shutdownWrite()
{
    if (_winfo.isAsync()) {
        _winfo.setAsyncCancel();
        return Err::E_SUCCESS;
    }

    if (_winfo.isWrite() == false) {
        tDLogE("WriteStateClient::_shutdownWrite() Illegal state error: {}", _winfo.getStateName());
        return Err::E_ILLSTATE;
    }

    assert(_winfo.isWrite());

    Err const CODE = _client->shutdown(_winfo.shutdown_req);
    if (TBAG_ERR_FAILURE(CODE)) {
        tDLogE("WriteStateClient::_shutdownWrite() {} error", getErrName(CODE));
        return CODE;
    }

    _winfo.setShutdown();
    return Err::E_SUCCESS;
}

bool WriteStateClient::_isActiveShutdownTimer() const
{
    return uvpp::isActiveTimer(_shutdown_timer);
}

Err WriteStateClient::_startShutdownTimer(uint64_t millisec)
{
    return uvpp::startTimer(_shutdown_timer, millisec);
}

Err WriteStateClient::_stopShutdownTimer()
{
    return uvpp::stopTimer(_shutdown_timer);
}

void WriteStateClient::_closeClient()
{
    assert(static_cast<bool>(_client));
    if (_client->isClosing() == false) {
        _client->close();
    }
    _winfo.setClosing();
}

void WriteStateClient::_closeInternal()
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

void WriteStateClient::onUserTimer()
{
    assert(_parent != nullptr);
    _parent->onTimer();
}

void WriteStateClient::onShutdownTimer()
{
    Guard const LOCK(_mutex);
    _shutdownWrite();
}

void WriteStateClient::onAsyncWrite()
{
    _mutex.lock();
    Err const WRITE_CODE = _writeFromOnAsync();
    _mutex.unlock();

    if (TBAG_ERR_FAILURE(WRITE_CODE)) {
        assert(_parent != nullptr);
        tDLogD("WriteStateClient::onAsyncWrite() ~~> onShutdown({}) event force call.",
               getErrName(WRITE_CODE));
        _parent->onShutdown(WRITE_CODE);
    }
}

void WriteStateClient::preConnect(Err code)
{
    Guard const LOCK(_mutex);
    if (_winfo.isClosing() || _winfo.isEnd()) {
        return;
    }
    _winfo.setReady();
}

void WriteStateClient::preShutdown(Err code)
{
    Guard const LOCK(_mutex);
    if (_winfo.isClosing() || _winfo.isEnd()) {
        return;
    }
    _winfo.setReady();
}

void WriteStateClient::preWrite(Err code)
{
    Guard const LOCK(_mutex);
    if (_winfo.isClosing() || _winfo.isEnd()) {
        return;
    }
    assert(_winfo.isWrite() || _winfo.isShutdown());
    _winfo.setReady();
    _stopShutdownTimer();
}

void WriteStateClient::preRead(Err code, ReadPacket const & packet)
{
    // EMPTY.
}

void WriteStateClient::preClose()
{
    Guard const LOCK(_mutex);
    _closeInternal();
    _winfo.setEnd();
}

} // namespace client
} // namespace stream
} // namespace network

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

