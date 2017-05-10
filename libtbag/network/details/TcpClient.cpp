/**
 * @file   TcpClient.cpp
 * @brief  TcpClient class implementation.
 * @author zer0
 * @date   2017-05-06
 */

#include <libtbag/network/details/TcpClient.hpp>
#include <libtbag/log/Log.hpp>
#include <cassert>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace network {
namespace details {

// -----------------------------
// TcpRealClient implementation.
// -----------------------------

TcpRealClient::TcpRealClient(Loop & loop, TcpClient & parent) : Tcp(loop), _parent(parent)
{
    setUserData(_parent.onUserDataAlloc());
}

TcpRealClient::~TcpRealClient()
{
    _parent.onUserDataDealloc(getUserData());
}

bool TcpRealClient::init(String const & ip, int port)
{
    return uvpp::initCommonClient(*this, _connect_req, ip, port);
}

void TcpRealClient::onConnect(ConnectRequest & request, uerr code)
{
    tDLogD("TcpRealClient::onConnect({})", uvpp::getErrorName(code));

    {   // Update parent state.
        Guard guard(_parent._mutex);
        _parent.cancelTimeoutClose();
    }

    _parent.onConnect(code);
}

void TcpRealClient::onShutdown(ShutdownRequest & request, uerr code)
{
    tDLogD("TcpRealClient::onShutdown({})", uvpp::getErrorName(code));

    {   // Update parent state.
        Guard guard(_parent._mutex);
        _parent.cancelTimeoutShutdown();
        _parent._last_writer.reset();
    }

    _parent.onShutdown(code);
}

void TcpRealClient::onWrite(WriteRequest & request, uerr code)
{
    tDLogD("TcpRealClient::onWrite({})", uvpp::getErrorName(code));

    {   // Update parent state.
        Guard guard(_parent._mutex);
        _parent.cancelTimeoutShutdown();
        _parent._last_writer.reset();
    }

    _parent.onWrite(code);
}

TcpRealClient::binf TcpRealClient::onAlloc(std::size_t suggested_size)
{
    return uvpp::defaultOnAlloc(_buffer, suggested_size);
}

void TcpRealClient::onRead(uerr code, char const * buffer, std::size_t size)
{
    tDLogD("TcpRealClient::onRead({})", uvpp::getErrorName(code));
    _parent.onRead(code, buffer, size);
}

void TcpRealClient::onClose()
{
    tDLogD("TcpRealClient::onClose()");
    _parent.onClose();

    {   // Update parent state.
        Guard guard(_parent._mutex);
        _parent.closeAll();
    }
}

// -------------------------
// TcpClient implementation.
// -------------------------

TcpClient::TcpClient(Loop & loop)
{
    _client   = loop.newHandle<TcpRealClient>(loop, *this);
    _async    = loop.newHandle<SafetyWriteAsync>(loop);
    _close    = loop.newHandle<TimeoutClose>(loop, _client.get(), false);
    _shutdown = loop.newHandle<TimeoutShutdown>(loop, _client.get(), false);
    assert(static_cast<bool>(_client));
    assert(static_cast<bool>(_async));
    assert(static_cast<bool>(_close));
    assert(static_cast<bool>(_shutdown));
}

TcpClient::~TcpClient()
{
    _client.reset();
    _async.reset();
    _close.reset();
    _shutdown.reset();
    _last_writer.reset();
}

void TcpClient::startTimeoutShutdown(Milliseconds const & millisec)
{
    assert(static_cast<bool>(_shutdown));
    uerr const CODE = _shutdown->start(static_cast<uint64_t>(millisec.count()));
    tDLogD("TcpClient::startTimeoutShutdown({}) result code: {}", millisec.count(), uvpp::getErrorName(CODE));
}

void TcpClient::startTimeoutClose(Milliseconds const & millisec)
{
    assert(static_cast<bool>(_close));
    uerr const CODE = _close->start(static_cast<uint64_t>(millisec.count()));
    tDLogD("TcpClient::startTimeoutClose({}) result code: {}", millisec.count(), uvpp::getErrorName(CODE));
}

void TcpClient::cancelTimeoutShutdown()
{
    assert(static_cast<bool>(_shutdown));
    if (_shutdown->isActive()) {
        _shutdown->cancel();
    }
    tDLogD("TcpClient::cancelTimeoutShutdown().");
}

void TcpClient::cancelTimeoutClose()
{
    assert(static_cast<bool>(_close));
    if (_close->isActive()) {
        _close->cancel();
    }
    tDLogD("TcpClient::cancelTimeoutClose().");
}

bool TcpClient::write(SafetyWriteAsync::SharedWriter writer, uint64_t millisec)
{
    assert(static_cast<bool>(_client));
    assert(static_cast<bool>(_async));

    if (static_cast<bool>(_last_writer)) {
        tDLogD("TcpClient::write() busy state.");
        return false;
    }

    Loop * loop = _client->getLoop();
    assert(loop != nullptr);

    if (millisec >= 1U) {
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

void TcpClient::closeAll()
{
    assert(static_cast<bool>(_client));
    assert(static_cast<bool>(_async));
    assert(static_cast<bool>(_close));
    assert(static_cast<bool>(_shutdown));

    _close->stop();
    if (_close->isClosing() == false) {
        _close->close();
    }

    _shutdown->stop();
    if (_shutdown->isClosing() == false) {
        _shutdown->close();
    }

    if (_client->isClosing() == false) {
        _client->close();
    }

    if (_async->isClosing() == false) {
        _async->close();
    }

    _last_writer.reset();
}

bool TcpClient::init(String const & ip, int port, uint64_t millisec)
{
    assert(static_cast<bool>(_client));
    Guard guard(_mutex);
    if (static_cast<bool>(_client) && _client->init(ip, port)) {
        if (millisec >= 1U) {
            startTimeoutClose(Milliseconds(millisec));
        }
        return true;
    }
    return false;
}

bool TcpClient::start()
{
    assert(static_cast<bool>(_client));
    Guard guard(_mutex);
    return _client->startRead() == uerr::UVPP_SUCCESS;
}

bool TcpClient::stop()
{
    assert(static_cast<bool>(_client));
    Guard guard(_mutex);
    return _client->stopRead() == uerr::UVPP_SUCCESS;
}

void TcpClient::close()
{
    assert(static_cast<bool>(_client));
    assert(static_cast<bool>(_async));

    Loop * loop = _client->getLoop();
    assert(loop != nullptr);

    if (loop->isAliveAndThisThread()) {
        tDLogD("TcpServer::close() sync request.");
        Guard guard(_mutex);
        closeAll();
    } else {
        tDLogD("TcpServer::close() async request.");
        _async->newSendFunc([&](SafetyAsync * UNUSED_PARAM(async)) {
            Guard guard(_mutex);
            closeAll();
        });
    }
}

void TcpClient::cancel()
{
    assert(static_cast<bool>(_client));
    assert(static_cast<bool>(_async));

    Loop * loop = _client->getLoop();
    assert(loop != nullptr);

    if (loop->isAliveAndThisThread()) {
        tDLogD("TcpServer::cancel() sync request.");
        Guard guard(_mutex);
        startTimeoutShutdown(Milliseconds(0U));
    } else {
        tDLogD("TcpServer::cancel() async request.");
        _async->newSendFunc([&](SafetyAsync * UNUSED_PARAM(async)) {
            Guard guard(_mutex);
            startTimeoutShutdown(Milliseconds(0U));
        });
    }
}

bool TcpClient::write(binf const * buffer, Size size, uint64_t millisec)
{
    assert(static_cast<bool>(_client));
    Guard guard(_mutex);
    return write(SafetyWriteAsync::createWrite(SafetyWriteAsync::WeakStream(_client), buffer, size), millisec);
}

bool TcpClient::write(char const * buffer, Size size, uint64_t millisec)
{
    assert(static_cast<bool>(_client));
    Guard guard(_mutex);
    return write(SafetyWriteAsync::createWrite(SafetyWriteAsync::WeakStream(_client), buffer, size), millisec);
}

} // namespace details
} // namespace network

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

