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
    // EMPTY.
}

TcpRealClient::~TcpRealClient()
{
    // EMPTY.
}

bool TcpRealClient::init(String const & ip, int port)
{
    return uvpp::initCommonClient(*this, _connect_req, ip, port);
}

void TcpRealClient::onShutdown(ShutdownRequest & request, uerr code)
{
    _parent.cancelTimeoutToShutdown();
    _parent._last_writer.reset();
}

void TcpRealClient::onConnect(ConnectRequest & request, uerr code)
{
    _parent.cancelTimeoutToClose();
}

void TcpRealClient::onWrite(WriteRequest & request, uerr code)
{
    _parent.cancelTimeoutToShutdown();
    _parent.onWrite(code);
    _parent._last_writer.reset();
}

TcpRealClient::binf TcpRealClient::onAlloc(std::size_t suggested_size)
{
    return uvpp::defaultOnAlloc(_buffer, suggested_size);
}

void TcpRealClient::onRead(uerr code, char const * buffer, std::size_t size)
{
    _parent.onRead(code, buffer, size);
}

void TcpRealClient::onClose()
{
    _parent.onClose();
    _parent.close();
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

void TcpClient::startTimeoutToShutdown(milliseconds const & millisec)
{
    uerr const CODE = _shutdown->start(static_cast<uint64_t>(millisec.count()));
    __tbag_debug("TcpClient::startTimeoutToShutdown({}) result code: {}", millisec.count(), uvpp::getErrorName(CODE));
}

void TcpClient::cancelTimeoutToShutdown()
{
    _shutdown->cancel();
}

void TcpClient::startTimeoutToClose(milliseconds const & millisec)
{
    uerr const CODE = _close->start(static_cast<uint64_t>(millisec.count()));
    __tbag_debug("TcpClient::startTimeoutToClose({}) result code: {}", millisec.count(), uvpp::getErrorName(CODE));
}

void TcpClient::cancelTimeoutToClose()
{
    _close->cancel();
}

bool TcpClient::write(SafetyWriteAsync::SharedWriter writer, uint64_t millisec)
{
    Loop * loop = _client->getLoop();
    assert(loop != nullptr);

    if (millisec >= 1U) {
        startTimeoutToShutdown(milliseconds(millisec));
    }

    if (loop->isAliveAndThisThread()) {
        _last_writer = writer;
        _last_writer->run(nullptr);
    }
    return _async->asyncWrite(writer);
}

bool TcpClient::init(String const & ip, int port, uint64_t millisec)
{
    Guard guard(_mutex);
    if (static_cast<bool>(_client) && _client->init(ip, port)) {
        startTimeoutToClose(milliseconds(millisec));
        return true;
    }
    return false;
}

bool TcpClient::start()
{
    Guard guard(_mutex);
    return _client->startRead() == uerr::UVPP_SUCCESS;
}

bool TcpClient::stop()
{
    Guard guard(_mutex);
    return _client->stopRead() == uerr::UVPP_SUCCESS;
}

bool TcpClient::close()
{
    Guard guard(_mutex);

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
    return true;
}

bool TcpClient::cancel()
{
    Guard guard(_mutex);
    startTimeoutToShutdown(milliseconds(0));
    return true;
}

bool TcpClient::write(binf const * buffer, Size size, uint64_t millisec)
{
    Guard guard(_mutex);
    if (static_cast<bool>(_last_writer) == false) {
        return false; // BUSY.
    }
    return write(_async->createWrite(SafetyWriteAsync::WeakStream(_client), buffer, size), millisec);
}

bool TcpClient::write(char const * buffer, Size size, uint64_t millisec)
{
    Guard guard(_mutex);
    if (static_cast<bool>(_last_writer) == false) {
        return false; // BUSY.
    }
    return write(_async->createWrite(SafetyWriteAsync::WeakStream(_client), buffer, size), millisec);
}

} // namespace details
} // namespace network

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

