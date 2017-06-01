/**
 * @file   StreamClient.cpp
 * @brief  StreamClient class implementation.
 * @author zer0
 * @date   2017-05-22
 */

#include <libtbag/network/stream/StreamClient.hpp>
#include <libtbag/network/stream/StreamClientBackend.hpp>
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

StreamClient::StreamClient(Loop & loop, StreamType type, SharedSafetyAsync async)
         : STREAM_TYPE(type)
{
    bool const IS_AUTO_CLOSE = false;

    using  TcpBackend = StreamClientBackend<uvpp::Tcp>;
    using PipeBackend = StreamClientBackend<uvpp::Pipe>;

    if (type == StreamType::TCP) {
        _client = loop.newHandle<TcpBackend>(loop, this);
    } else if (type == StreamType::PIPE) {
        _client = loop.newHandle<PipeBackend>(loop, this);
    } else {
        throw std::bad_alloc();
    }

    if (static_cast<bool>(async) && loop.id() == async->getLoop()->id()) {
        _owner_async = false;
        _async = async;
    } else {
        _owner_async = true;
        _async = loop.newHandle<SafetyAsync>(loop);
    }

    _close_timer    = loop.newHandle<FuncTimer>(loop);
    _shutdown_timer = loop.newHandle<FuncTimer>(loop);

    assert(static_cast<bool>(_client));
    assert(static_cast<bool>(_async));
    assert(static_cast<bool>(_close_timer));
    assert(static_cast<bool>(_shutdown_timer));

    _close_timer->setOnTimer([&](){
        tDLogN("StreamClient::StreamClient() On close timer!");
        Guard guard(_mutex);
        closeAll();
    });

    _shutdown_timer->setOnTimer([&](){
        tDLogN("StreamClient::StreamClient() On shutdown timer!");
        Guard guard(_mutex);
        shutdownWrite(); // The return value is not used.
    });

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

char const * StreamClient::getWriteStatusName(WriteStatus status) TBAG_NOEXCEPT
{
    switch (status) {
    case WriteStatus::WS_NOT_READY:     return "NOT_READY";
    case WriteStatus::WS_READY:         return "READY";
    case WriteStatus::WS_ASYNC:         return "ASYNC";
    case WriteStatus::WS_ASYNC_CANCEL:  return "ASYNC_CANCEL";
    case WriteStatus::WS_WRITE:         return "WRITE";
    case WriteStatus::WS_SHUTDOWN:      return "SHUTDOWN";
    default:                            return "UNKNOWN";
    }
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

// ------------------
// PROTECTED SECTION.
// ------------------

Err StreamClient::startCloseTimer(uint64_t millisec)
{
    assert(static_cast<bool>(_close_timer));
    Err const CODE = startTimer(*_close_timer, millisec);
    if (CODE != Err::E_SUCCESS) {
        tDLogE("StreamClient::startCloseTimer() {} error.", getErrName(CODE));
    }
    return CODE;
}

Err StreamClient::startShutdownTimer(uint64_t millisec)
{
    assert(static_cast<bool>(_shutdown_timer));
    Err const CODE = startTimer(*_shutdown_timer, millisec);
    if (CODE != Err::E_SUCCESS) {
        tDLogE("StreamClient::startShutdownTimer() {} error.", getErrName(CODE));
    }
    return CODE;
}

void StreamClient::stopCloseTimer()
{
    assert(static_cast<bool>(_close_timer));
    Err const CODE = stopTimer(*_close_timer);
    if (CODE != Err::E_SUCCESS) {
        tDLogE("StreamClient::stopCloseTimer() {} error.", getErrName(CODE));
    }
}

void StreamClient::stopShutdownTimer()
{
    assert(static_cast<bool>(_shutdown_timer));
    Err const CODE = stopTimer(*_shutdown_timer);
    if (CODE != Err::E_SUCCESS) {
        tDLogE("StreamClient::stopShutdownTimer() {} error.", getErrName(CODE));
    }
}

void StreamClient::updateWriteStatusToReady()
{
    _writer.status = WriteStatus::WS_READY;
}

void StreamClient::updateWriteStatusToNotReady()
{
    _writer.status = WriteStatus::WS_NOT_READY;
}

Err StreamClient::shutdownWrite()
{
    if (_writer.status == WriteStatus::WS_ASYNC) {
        _writer.status = WriteStatus::WS_ASYNC_CANCEL;
        return Err::E_SUCCESS;
    }

    if (_writer.status != WriteStatus::WS_WRITE) {
        tDLogE("StreamClient::shutdownWrite() Illegal state error: {}", getWriteStatusName(_writer.status));
        return Err::E_ILLSTATE;
    }

    assert(_writer.status == WriteStatus::WS_WRITE);

    Err const CODE = _client->shutdown(_writer.shutdown_req);
    if (CODE != Err::E_SUCCESS) {
        tDLogE("StreamClient::shutdownWrite() {} error", getErrName(CODE));
        return CODE;
    }

    _writer.status = WriteStatus::WS_SHUTDOWN;
    return Err::E_SUCCESS;
}

Err StreamClient::writeReal(binf const * buffer, std::size_t size)
{
    assert(static_cast<bool>(_client));
    return _client->write(_writer.write_req, buffer, size);
}

void StreamClient::copyToWriteBuffer(binf const * buffer, std::size_t size)
{
    // COPY TO TEMP BUFFER ...
    _writer.buffers.resize(size);
    for (std::size_t i = 0; i < size; ++i) {
        char const * b = (buffer + i)->buffer;
        std::size_t bs = (buffer + i)->size;
        _writer.buffers[i].assign(b, b + bs);
    }
}

std::vector<StreamClient::binf> StreamClient::getWriteBufferInfo()
{
    std::size_t SIZE = _writer.buffers.size();
    std::vector<binf> result(SIZE);
    for (std::size_t i = 0; i < SIZE; ++i) {
        result[i].buffer = &_writer.buffers[i][0];
        result[i].size   =  _writer.buffers[i].size();
    }
    return result;
}

Err StreamClient::autoWrite(binf const * buffer, std::size_t size, uint64_t millisec)
{
    if (_writer.status != WriteStatus::WS_READY) {
        tDLogE("StreamClient::autoWrite() Illegal state error: {}", getWriteStatusName(_writer.status));
        return Err::E_ILLSTATE;
    }

    assert(static_cast<bool>(_client));
    assert(static_cast<bool>(_async));

    Loop * loop = _client->getLoop();
    assert(loop != nullptr);

    if (loop->isAliveAndThisThread()) {
        Err const CODE = writeReal(buffer, size);
        if (CODE != Err::E_SUCCESS) {
            tDLogE("StreamClient::autoWrite() Write {} error.", getErrName(CODE));
            return CODE;
        }
        _writer.status = WriteStatus::WS_WRITE;

    } else {
        copyToWriteBuffer(buffer, size);
        auto job = _async->newSendFunc(std::bind(&StreamClient::onAsyncWrite, this));
        if (static_cast<bool>(job) == false) {
            tDLogE("StreamClient::autoWrite() New job error.");
            return Err::E_BADALLOC;
        }
        _writer.status = WriteStatus::WS_ASYNC;
    }

    if (millisec > 0) {
        Err const CODE = startShutdownTimer(millisec);
        if (CODE != Err::E_SUCCESS) {
            tDLogW("StreamClient::autoWrite() Timer job {} error!", getErrName(CODE));
        }
    } else {
        tDLogD("StreamClient::autoWrite() No timeout.");
    }

    return Err::E_SUCCESS;
}

void StreamClient::onAsyncWrite()
{
    Guard guard(_mutex);

    if (_writer.status == WriteStatus::WS_ASYNC_CANCEL) {
        tDLogN("StreamClient::onAsyncWrite() Cancel async write.");
        stopShutdownTimer();
        _writer.status = WriteStatus::WS_READY;
        return;
    }

    if (_writer.status != WriteStatus::WS_ASYNC) {
        tDLogE("StreamClient::onAsyncWrite() Error state: {}", static_cast<int>(_writer.status));
        stopShutdownTimer();
        _writer.status = WriteStatus::WS_READY;
        return;
    }

    assert(_writer.status == WriteStatus::WS_ASYNC);
    auto binfs = getWriteBufferInfo();
    Err const CODE = writeReal(&binfs[0], binfs.size());

    if (CODE == Err::E_SUCCESS) {
        _writer.status = WriteStatus::WS_WRITE;
    } else {
        tDLogE("StreamClient::onAsyncWrite() {} error.", getErrName(CODE));
        stopShutdownTimer();
        _writer.status = WriteStatus::WS_READY;
    }
}

void StreamClient::closeAll()
{
    assert(static_cast<bool>(_client));
    if (_client->isClosing() == false) {
        _client->close();
    }

    if (_owner_async) {
        assert(static_cast<bool>(_async));
        if (_async->isClosing() == false) {
            _async->close();
        }
    }

    assert(static_cast<bool>(_close_timer));
    if (_close_timer->isActive()) {
        _close_timer->stop();
    }
    if (_close_timer->isClosing() == false) {
        _close_timer->close();
    }

    assert(static_cast<bool>(_shutdown_timer));
    if (_shutdown_timer->isActive()) {
        _shutdown_timer->stop();
    }
    if (_shutdown_timer->isClosing() == false) {
        _shutdown_timer->close();
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

    if (millisec > 0) {
        startCloseTimer(millisec);
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
    assert(static_cast<bool>(_async));

    Loop * loop = _client->getLoop();
    assert(loop != nullptr);

    Guard guard(_mutex);
    if (loop->isAliveAndThisThread()) {
        tDLogD("StreamClient::close() request.");
        closeAll();
    } else {
        tDLogD("StreamClient::close() async request.");
        _async->newSendFunc([&](){
            Guard guard(_mutex);
            closeAll();
        });
    }
}

void StreamClient::cancel()
{
    assert(static_cast<bool>(_client));
    assert(static_cast<bool>(_async));

    Loop * loop = _client->getLoop();
    assert(loop != nullptr);

    Guard guard(_mutex);
    if (loop->isAliveAndThisThread()) {
        tDLogD("StreamClient::cancel() request.");
        shutdownWrite();
    } else {
        tDLogD("StreamClient::cancel() async request.");
        _async->newSendFunc([&]() {
            Guard guard(_mutex);
            shutdownWrite();
        });
    }
}

bool StreamClient::write(binf const * buffer, std::size_t size, uint64_t millisec)
{
    Guard guard(_mutex);
    return autoWrite(buffer, size, millisec) == Err::E_SUCCESS;
}

bool StreamClient::write(char const * buffer, std::size_t size, uint64_t millisec)
{
    binf info;
    info.buffer = const_cast<char*>(buffer);
    info.size   = size;

    Guard guard(_mutex);
    return autoWrite(&info, 1U, millisec) == Err::E_SUCCESS;
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
    stopCloseTimer();
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
    stopShutdownTimer();
    _writer.status = WriteStatus::WS_READY;
    _mutex.unlock();

    onWrite(code);
}

void StreamClient::runBackendRead(Err code, char const * buffer, std::size_t size)
{
    onRead(code, buffer, size);
}

void StreamClient::runBackendClose()
{
    onClose();

    _mutex.lock();
    closeAll();
    _writer.status = WriteStatus::WS_NOT_READY;
    _mutex.unlock();
}

} // namespace stream
} // namespace network

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

