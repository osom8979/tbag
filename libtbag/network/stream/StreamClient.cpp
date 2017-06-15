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

#include <cassert>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace network {
namespace stream  {

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
    if (_initInternalHandles()) {
        _writer.status = WriteStatus::WS_READY;
    }
}

StreamClient::StreamClient(Loop & loop, StreamType type, SharedSafetyAsync async)
         : STREAM_TYPE(type)
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

StreamClient::WeakClientBackend StreamClient::getClient()
{
    Guard g(_mutex);
    return WeakClientBackend(_client);
}

StreamClient::WeakSafetyAsync StreamClient::getAsync()
{
    Guard g(_mutex);
    return WeakSafetyAsync(_async);
}

Err StreamClient::startTimer(uvpp::Timer & timer, uint64_t millisec)
{
    if (timer.isActive()) {
        return Err::E_EBUSY;
    }
    return timer.start(millisec);
}

Err StreamClient::stopTimer(uvpp::Timer & timer)
{
    return timer.stop();
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

// ------------------
// PROTECTED SECTION.
// ------------------

bool StreamClient::_initInternalHandles()
{
    assert(_client != nullptr);
    Loop * loop = _client->getLoop();
    assert(loop != nullptr);

    if (static_cast<bool>(_async) == false) {
        _async = loop->newHandle<SafetyAsync>(*loop);
    }
    _close_timer    = loop->newHandle<FuncTimer>(*loop);
    _shutdown_timer = loop->newHandle<FuncTimer>(*loop);

    assert(static_cast<bool>(_async));
    assert(static_cast<bool>(_close_timer));
    assert(static_cast<bool>(_shutdown_timer));

    _close_timer->setOnTimer([&](){
        tDLogN("StreamClient::initInternalHandles() On close timer!");
        Guard guard(_mutex);
        _closeAll();
    });

    _shutdown_timer->setOnTimer([&](){
        tDLogN("StreamClient::initInternalHandles() On shutdown timer!");
        Guard guard(_mutex);
        _shutdownWrite(); // The return value is not used.
    });

    return true;
}

Err StreamClient::_startCloseTimer(uint64_t millisec)
{
    if (static_cast<bool>(_close_timer) == false) {
        return Err::E_ALREADY;
    }

    Err const CODE = startTimer(*_close_timer, millisec);
    if (CODE != Err::E_SUCCESS) {
        tDLogE("StreamClient::_startCloseTimer() {} error.", getErrName(CODE));
    }
    return CODE;
}

Err StreamClient::_startShutdownTimer(uint64_t millisec)
{
    if (static_cast<bool>(_shutdown_timer) == false) {
        return Err::E_ALREADY;
    }

    Err const CODE = startTimer(*_shutdown_timer, millisec);
    if (CODE != Err::E_SUCCESS) {
        tDLogE("StreamClient::_startShutdownTimer() {} error.", getErrName(CODE));
    }
    return CODE;
}

void StreamClient::_stopCloseTimer()
{
    if (static_cast<bool>(_close_timer) == false) {
        return;
    }

    Err const CODE = stopTimer(*_close_timer);
    if (CODE != Err::E_SUCCESS) {
        tDLogE("StreamClient::_stopCloseTimer() {} error.", getErrName(CODE));
    }
}

void StreamClient::_stopShutdownTimer()
{
    if (static_cast<bool>(_shutdown_timer) == false) {
        return;
    }

    Err const CODE = stopTimer(*_shutdown_timer);
    if (CODE != Err::E_SUCCESS) {
        tDLogE("StreamClient::_stopShutdownTimer() {} error.", getErrName(CODE));
    }
}

Err StreamClient::_shutdownWrite()
{
    if (_writer.status == WriteStatus::WS_ASYNC) {
        _writer.status = WriteStatus::WS_ASYNC_CANCEL;
        return Err::E_SUCCESS;
    }

    if (_writer.status != WriteStatus::WS_WRITE) {
        tDLogE("StreamClient::_shutdownWrite() Illegal state error: {}", getWriteStatusName(_writer.status));
        return Err::E_ILLSTATE;
    }

    assert(_writer.status == WriteStatus::WS_WRITE);

    Err const CODE = _client->shutdown(_writer.shutdown_req);
    if (CODE != Err::E_SUCCESS) {
        tDLogE("StreamClient::_shutdownWrite() {} error", getErrName(CODE));
        return CODE;
    }

    _writer.status = WriteStatus::WS_SHUTDOWN;
    return Err::E_SUCCESS;
}

Err StreamClient::_writeReal(binf const * buffer, std::size_t size)
{
    assert(static_cast<bool>(_client));
    return _client->write(_writer.write_req, buffer, size);
}

void StreamClient::_copyToWriteBuffer(binf const * buffer, std::size_t size)
{
    // COPY TO TEMP BUFFER ...
    _writer.buffers.resize(size);
    for (std::size_t i = 0; i < size; ++i) {
        char const * b = (buffer + i)->buffer;
        std::size_t bs = (buffer + i)->size;
        _writer.buffers[i].assign(b, b + bs);
    }
}

std::vector<StreamClient::binf> StreamClient::_getWriteBufferInfo()
{
    std::size_t SIZE = _writer.buffers.size();
    std::vector<binf> result(SIZE);
    for (std::size_t i = 0; i < SIZE; ++i) {
        result[i].buffer = &_writer.buffers[i][0];
        result[i].size   =  _writer.buffers[i].size();
    }
    return result;
}

Err StreamClient::_autoWrite(binf const * buffer, std::size_t size, uint64_t millisec)
{
    if (_writer.status != WriteStatus::WS_READY) {
        tDLogE("StreamClient::_autoWrite() Illegal state error: {}", getWriteStatusName(_writer.status));
        return Err::E_ILLSTATE;
    }

    assert(static_cast<bool>(_client));
    assert(static_cast<bool>(_async));

    Loop * loop = _client->getLoop();
    assert(loop != nullptr);

    if (loop->isAliveAndThisThread() || static_cast<bool>(_async) == false) {
        Err const CODE = _writeReal(buffer, size);
        if (CODE != Err::E_SUCCESS) {
            tDLogE("StreamClient::_autoWrite() Write {} error.", getErrName(CODE));
            return CODE;
        }
        _writer.status = WriteStatus::WS_WRITE;

    } else {
        _copyToWriteBuffer(buffer, size);
        auto job = _async->newSendFunc(std::bind(&StreamClient::onAsyncWrite, this));
        if (static_cast<bool>(job) == false) {
            tDLogE("StreamClient::_autoWrite() New job error.");
            return Err::E_BADALLOC;
        }
        _writer.status = WriteStatus::WS_ASYNC;
    }

    if (millisec > 0) {
        Err const CODE = _startShutdownTimer(millisec);
        if (CODE != Err::E_SUCCESS) {
            tDLogW("StreamClient::_autoWrite() Timer job {} error!", getErrName(CODE));
        }
    } else {
        tDLogD("StreamClient::_autoWrite() No timeout.");
    }

    return Err::E_SUCCESS;
}

void StreamClient::_closeAll()
{
    assert(static_cast<bool>(_client));
    if (_client->isClosing() == false) {
        _client->close();
    }

    if (_owner_async && static_cast<bool>(_async)) {
        if (_async->isClosing() == false) {
            _async->close();
        }
        _async.reset();
    }

    if (static_cast<bool>(_close_timer)) {
        if (_close_timer->isActive()) {
            _close_timer->stop();
        }
        if (_close_timer->isClosing() == false) {
            _close_timer->close();
        }
        _close_timer.reset();
    }

    if (static_cast<bool>(_shutdown_timer)) {
        if (_shutdown_timer->isActive()) {
            _shutdown_timer->stop();
        }
        if (_shutdown_timer->isClosing() == false) {
            _shutdown_timer->close();
        }
        _shutdown_timer.reset();
    }
}

void StreamClient::onAsyncWrite()
{
    Guard guard(_mutex);

    if (_writer.status == WriteStatus::WS_ASYNC_CANCEL) {
        tDLogN("StreamClient::onAsyncWrite() Cancel async write.");
        _stopShutdownTimer();
        _writer.status = WriteStatus::WS_READY;
        return;
    }

    if (_writer.status != WriteStatus::WS_ASYNC) {
        tDLogE("StreamClient::onAsyncWrite() Error state: {}", static_cast<int>(_writer.status));
        _stopShutdownTimer();
        _writer.status = WriteStatus::WS_READY;
        return;
    }

    assert(_writer.status == WriteStatus::WS_ASYNC);
    auto binfs = _getWriteBufferInfo();
    Err const CODE = _writeReal(&binfs[0], binfs.size());

    if (CODE == Err::E_SUCCESS) {
        _writer.status = WriteStatus::WS_WRITE;
    } else {
        tDLogE("StreamClient::onAsyncWrite() {} error.", getErrName(CODE));
        _stopShutdownTimer();
        _writer.status = WriteStatus::WS_READY;
    }
}

StreamClient::Id StreamClient::getId() const
{
    return _client->id();
}

bool StreamClient::init(char const * destination, int port, uint64_t millisec)
{
    Guard guard(_mutex);

    assert(static_cast<bool>(_client));
    if (static_cast<bool>(_client) == false) {
        return false;
    }

    using TcpBackend  = StreamClientBackend<uvpp::Tcp>;
    using PipeBackend = StreamClientBackend<uvpp::Pipe>;

    bool is_init = false;
    if (STREAM_TYPE == StreamType::TCP) {
        auto backend = std::static_pointer_cast<TcpBackend>(_client);
        is_init = uvpp::initCommonClient(*backend, backend->atConnectReq(), destination, port);
    } else if (STREAM_TYPE == StreamType::PIPE) {
        auto backend = std::static_pointer_cast<PipeBackend>(_client);
        is_init = uvpp::initPipeClient(*backend, backend->atConnectReq(), destination);
    } else {
        tDLogA("StreamClient::init() Unknown stream type.");
        return false;
    }

    if (is_init == false) {
        tDLogE("StreamClient::init() Initialize fail.");
        return false;
    }

    if (_initInternalHandles() == false) {
        tDLogE("StreamClient::init() Initialize fail (internal handles).");
        return false;
    }

    if (millisec > 0) {
        _startCloseTimer(millisec);
    } else {
        tDLogD("StreamClient::init() No timeout.");
    }

    return true;
}

bool StreamClient::start()
{
    assert(static_cast<bool>(_client));
    Guard guard(_mutex);

    Err const CODE = _client->startRead();
    if (CODE != Err::E_SUCCESS) {
        tDLogE("StreamClient::start() {} error", getErrName(CODE));
        return false;
    }
    return true;
}

bool StreamClient::stop()
{
    assert(static_cast<bool>(_client));
    Guard guard(_mutex);

    Err const CODE = _client->stopRead();
    if (CODE != Err::E_SUCCESS) {
        tDLogE("StreamClient::stop() {} error", getErrName(CODE));
        return false;
    }
    return true;
}

void StreamClient::close()
{
    assert(static_cast<bool>(_client));
    Loop * loop = _client->getLoop();
    assert(loop != nullptr);

    Guard guard(_mutex);
    if (loop->isAliveAndThisThread() || static_cast<bool>(_async) == false) {
        tDLogD("StreamClient::close() request.");
        _closeAll();
    } else {
        tDLogD("StreamClient::close() async request.");
        _async->newSendFunc([&](){
            Guard guard(_mutex);
            _closeAll();
        });
    }
}

void StreamClient::cancel()
{
    assert(static_cast<bool>(_client));
    Loop * loop = _client->getLoop();
    assert(loop != nullptr);

    Guard guard(_mutex);
    if (loop->isAliveAndThisThread() || static_cast<bool>(_async) == false) {
        tDLogD("StreamClient::cancel() request.");
        _shutdownWrite();
    } else {
        tDLogD("StreamClient::cancel() async request.");
        _async->newSendFunc([&]() {
            Guard guard(_mutex);
            _shutdownWrite();
        });
    }
}

bool StreamClient::write(binf const * buffer, std::size_t size, uint64_t millisec)
{
    Guard guard(_mutex);
    return _autoWrite(buffer, size, millisec) == Err::E_SUCCESS;
}

bool StreamClient::write(char const * buffer, std::size_t size, uint64_t millisec)
{
    binf info;
    info.buffer = const_cast<char*>(buffer);
    info.size   = size;

    Guard guard(_mutex);
    return _autoWrite(&info, 1U, millisec) == Err::E_SUCCESS;
}

void * StreamClient::getUserData()
{
    assert(static_cast<bool>(_client));

    Guard guard(_mutex);
    return _client->getUserData();
}

std::string StreamClient::getDestination() const
{
    Guard guard(_mutex);
    if (STREAM_TYPE == StreamType::TCP) {
        return std::static_pointer_cast<uvpp::Tcp>(_client)->getPeerIp();
    }
    return std::string();
}

int StreamClient::getPort() const
{
    Guard guard(_mutex);
    if (STREAM_TYPE == StreamType::TCP) {
        return std::static_pointer_cast<uvpp::Tcp>(_client)->getPeerPort();
    }
    return 0;
}

// --------------
// Event backend.
// --------------

void StreamClient::runBackendConnect(Err code)
{
    _mutex.lock();
    _stopCloseTimer();
    _writer.status = WriteStatus::WS_READY;
    _mutex.unlock();

    onConnect(code);
}

void StreamClient::runBackendShutdown(Err code)
{
    _mutex.lock();
    _writer.status = WriteStatus::WS_READY;
    _mutex.unlock();

    onShutdown(code);
}

void StreamClient::runBackendWrite(Err code)
{
    _mutex.lock();
    _stopShutdownTimer();
    _writer.status = WriteStatus::WS_READY;
    _mutex.unlock();

    onWrite(code);
}

void StreamClient::runBackendRead(Err code, char const * buffer, std::size_t size,
                                  sockaddr const * UNUSED_PARAM(addr), unsigned int UNUSED_PARAM(flags))
{
    onRead(code, buffer, size);
}

void StreamClient::runBackendClose()
{
    _mutex.lock();
    _closeAll();
    _writer.status = WriteStatus::WS_END;
    _mutex.unlock();

    onClose();
}

} // namespace stream
} // namespace network

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

