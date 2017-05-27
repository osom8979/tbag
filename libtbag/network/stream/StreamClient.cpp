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

StreamClient::StreamClient(Loop & loop, StreamType type) : STREAM_TYPE(type)
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

    _async    = loop.newHandle<SafetyWriteAsync>(loop);
    _close    = loop.newHandle<TimeoutClose>(loop, _client.get(), IS_AUTO_CLOSE);
    _shutdown = loop.newHandle<TimeoutShutdown>(loop, _client.get(), IS_AUTO_CLOSE);

    assert(static_cast<bool>(_client));
    assert(static_cast<bool>(_async));
    assert(static_cast<bool>(_close));
    assert(static_cast<bool>(_shutdown));
}

StreamClient::~StreamClient()
{
    _client.reset();
    _async.reset();
    _close.reset();
    _shutdown.reset();
    _last_writer.reset();
}

StreamClient::WeakClientBackend StreamClient::getClient()
{
    Guard g(_mutex);
    return WeakClientBackend(_client);
}

StreamClient::WeakSafetyWriteAsync StreamClient::getAsync()
{
    Guard g(_mutex);
    return WeakSafetyWriteAsync(_async);
}

StreamClient::WeakTimeoutClose StreamClient::getClose()
{
    Guard g(_mutex);
    return WeakTimeoutClose(_close);
}

StreamClient::WeakTimeoutShutdown StreamClient::getShutdown()
{
    Guard g(_mutex);
    return WeakTimeoutShutdown(_shutdown);
}

bool StreamClient::isWriting() const
{
    Guard g(_mutex);
    return static_cast<bool>(_last_writer);
}

void StreamClient::startTimeoutShutdown(Milliseconds const & millisec)
{
    assert(static_cast<bool>(_shutdown));
    Err const CODE = _shutdown->start(static_cast<uint64_t>(millisec.count()));
    tDLogD("StreamClient::startTimeoutShutdown({}) result code: {}", millisec.count(), getErrName(CODE));
}

void StreamClient::startTimeoutClose(Milliseconds const & millisec)
{
    assert(static_cast<bool>(_close));
    Err const CODE = _close->start(static_cast<uint64_t>(millisec.count()));
    tDLogD("StreamClient::startTimeoutClose({}) result code: {}", millisec.count(), getErrName(CODE));
}

void StreamClient::cancelTimeoutShutdown()
{
    assert(static_cast<bool>(_shutdown));
    if (_shutdown->isActive()) {
        _shutdown->cancel();
        _shutdown->stop();
    }
    tDLogD("StreamClient::cancelTimeoutShutdown()");
}

void StreamClient::cancelTimeoutClose()
{
    assert(static_cast<bool>(_close));
    if (_close->isActive()) {
        _close->cancel();
        _close->stop();
    }
    tDLogD("StreamClient::cancelTimeoutClose()");
}

bool StreamClient::write(SharedSafetyWriter writer, uint64_t millisec)
{
    assert(static_cast<bool>(_client));
    assert(static_cast<bool>(_async));

    if (static_cast<bool>(writer) == false) {
        tDLogD("StreamClient::write() writer is nullptr.");
        return false;
    }

    if (static_cast<bool>(_last_writer)) {
        tDLogD("StreamClient::write() busy state.");
        return false;
    }

    Loop * loop = _client->getLoop();
    assert(loop != nullptr);

    if (millisec > 0) {
        startTimeoutShutdown(Milliseconds(millisec));
    }

    if (loop->isAliveAndThisThread()) {
        _last_writer = writer;
        _last_writer->run(nullptr);
    } else {
        _last_writer = writer;
        if (_async->asyncWrite(writer) == false) {
            _last_writer.reset();
        }
    }

    return true;
}

void StreamClient::closeAll()
{
    assert(static_cast<bool>(_close));
    if (_close->isActive()) {
        _close->cancel();
        _close->stop();
    }
    if (_close->isClosing() == false) {
        _close->close();
    }

    assert(static_cast<bool>(_shutdown));
    if (_shutdown->isActive()) {
        _shutdown->cancel();
        _shutdown->stop();
    }
    if (_shutdown->isClosing() == false) {
        _shutdown->close();
    }

    assert(static_cast<bool>(_client));
    if (_client->isClosing() == false) {
        _client->close();
    }

    assert(static_cast<bool>(_async));
    if (_async->isClosing() == false) {
        _async->close();
    }

    _last_writer.reset();
}

StreamClient::Id StreamClient::getId() const
{
    return _client->id();
}

bool StreamClient::init(char const * destination, int port, uint64_t millisec)
{
    using TcpBackend = StreamClientBackend<uvpp::Tcp>;
    using PipeBackend = StreamClientBackend<uvpp::Pipe>;

    assert(static_cast<bool>(_client));
    Guard guard(_mutex);
    if (static_cast<bool>(_client) == false) {
        return false;
    }

    bool is_init = false;
    if (STREAM_TYPE == StreamType::TCP) {
        auto backend = std::static_pointer_cast<TcpBackend>(_client);
        is_init = uvpp::initCommonClient(*backend, backend->atConnectReq(), destination, port);
    } else if (STREAM_TYPE == StreamType::PIPE) {
        auto backend = std::static_pointer_cast<PipeBackend>(_client);
        is_init = uvpp::initPipeClient(*backend, backend->atConnectReq(), destination);
    } else {
        return false;
    }

    if (is_init == false) {
        return false;
    }

    if (millisec >= 1U) {
        startTimeoutClose(Milliseconds(millisec));
    }
    return true;
}

bool StreamClient::start()
{
    assert(static_cast<bool>(_client));
    Guard guard(_mutex);

    Err const CODE = _client->startRead();
    tDLogD("StreamClient::start() result code: {}", getErrName(CODE));
    return CODE == Err::E_SUCCESS;
}

bool StreamClient::stop()
{
    assert(static_cast<bool>(_client));
    Guard guard(_mutex);

    Err const CODE = _client->stopRead();
    tDLogD("StreamClient::stop() result code: {}", getErrName(CODE));
    return CODE == Err::E_SUCCESS;
}

void StreamClient::close()
{
    assert(static_cast<bool>(_client));
    assert(static_cast<bool>(_async));

    Loop * loop = _client->getLoop();
    assert(loop != nullptr);

    if (loop->isAliveAndThisThread()) {
        tDLogD("StreamClient::close() sync request.");
        Guard guard(_mutex);
        closeAll();
    } else {
        tDLogD("StreamClient::close() async request.");
        _async->newSendFunc([&](SafetyAsync * UNUSED_PARAM(async)) {
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

    if (loop->isAliveAndThisThread()) {
        tDLogD("StreamClient::cancel() sync request.");
        Guard guard(_mutex);
        startTimeoutShutdown(Milliseconds(0U));
    } else {
        tDLogD("StreamClient::cancel() async request.");
        _async->newSendFunc([&](SafetyAsync * UNUSED_PARAM(async)) {
            Guard guard(_mutex);
            startTimeoutShutdown(Milliseconds(0U));
        });
    }
}

bool StreamClient::write(binf const * buffer, std::size_t size, uint64_t millisec)
{
    assert(static_cast<bool>(_client));
    Guard guard(_mutex);
    return write(SafetyWriteAsync::createWrite(SafetyWriteAsync::WeakStream(_client), buffer, size), millisec);
}

bool StreamClient::write(char const * buffer, std::size_t size, uint64_t millisec)
{
    assert(static_cast<bool>(_client));
    Guard guard(_mutex);
    return write(SafetyWriteAsync::createWrite(SafetyWriteAsync::WeakStream(_client), buffer, size), millisec);
}

void * StreamClient::getUserData()
{
    assert(static_cast<bool>(_client));
    return _client->getUserData();
}

std::string StreamClient::getDestination() const
{
    if (STREAM_TYPE == StreamType::TCP) {
        return std::static_pointer_cast<uvpp::Tcp>(_client)->getPeerIp();
    }
    return std::string();
}

int StreamClient::getPort() const
{
    if (STREAM_TYPE == StreamType::TCP) {
        return std::static_pointer_cast<uvpp::Tcp>(_client)->getPeerPort();
    }
    return 0;
}

void StreamClient::runBackendConnect(Err code)
{
    _mutex.lock();
    cancelTimeoutClose();
    _mutex.unlock();

    onConnect(code);
}

void StreamClient::runBackendShutdown(Err code)
{
    _mutex.lock();
    cancelTimeoutShutdown();
    _last_writer.reset();
    _mutex.unlock();

    onShutdown(code);
}

void StreamClient::runBackendWrite(Err code)
{
    _mutex.lock();
    cancelTimeoutShutdown();
    _last_writer.reset();
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
    _mutex.unlock();
}

} // namespace stream
} // namespace network

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

