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
}

TcpRealClient::~TcpRealClient()
{
}

bool TcpRealClient::init(String const & ip, int port)
{
    return uvpp::initCommonClient(*this, _connect_req, ip, port);
}

void TcpRealClient::onConnect(ConnectRequest & request, uerr code)
{
    _parent.removeTimeoutToClose();
}

void TcpRealClient::onWrite(WriteRequest & request, uerr code)
{
    _parent.onWrite(code);
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
}

// -------------------------
// TcpClient implementation.
// -------------------------

TcpClient::TcpClient(Loop & loop)
{
    _client   = loop.newHandle<TcpRealClient>(loop, *this);
    _async    = loop.newHandle<SafetyAsync>(loop);
    _close    = loop.newHandle<TimeoutToClose>(loop, _client.get(), false);
    _shutdown = loop.newHandle<TimeoutToShutdown>(loop, _client.get(), false);
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
}

bool TcpClient::start()
{
    return _client->startRead() == uerr::UVPP_SUCCESS;
}

bool TcpClient::stop()
{
    return _client->stopRead() == uerr::UVPP_SUCCESS;
}

bool TcpClient::close()
{
    _close->stop();
    _close->close();

    _shutdown->stop();
    _shutdown->close();

    _client->close();
    _async->close();

    return true;
}

bool TcpClient::cancel()
{
    return false;
}

bool TcpClient::syncWrite(char const * buffer, Size * size)
{
    return false;
}

bool TcpClient::asyncWrite(char const * buffer, Size * size)
{
    return false;
}

bool TcpClient::tryWrite(char const * buffer, Size * size)
{
    return false;
}

bool TcpClient::init(String const & ip, int port, int timeout)
{
    if (static_cast<bool>(_client) && _client->init(ip, port)) {
        startTimeoutToClose(std::chrono::milliseconds(timeout));
        return true;
    }
    return false;
}

void TcpClient::startTimeoutToClose(std::chrono::milliseconds const & millisec)
{
    _close->start(static_cast<uint64_t>(millisec.count()));
}

void TcpClient::removeTimeoutToClose()
{
    _close->close();
}

} // namespace details
} // namespace network

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

