/**
 * @file   TcpClient.cpp
 * @brief  TcpClient class implementation.
 * @author zer0
 * @date   2016-12-29
 * @date   2017-02-15 (Apply BasicTcp class)
 */

#include <libtbag/network/TcpClient.hpp>
#include <libtbag/log/Log.hpp>
#include <cassert>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace network {

TcpClient::TcpClient()
{
    _client = _loop.newHandle<ClientTcp>(_loop, this);
    _async  = _loop.newHandle<Async >(_loop);
}

TcpClient::~TcpClient()
{
    // EMPTY.
}

bool TcpClient::initIpv4(std::string const & ip, int port)
{
    if (static_cast<bool>(_client) == false) {
        return false;
    }

    if (_client->isInit() == false) {
        return false;
    }

    sockaddr_in addr;
    if (_client->initAddress(ip, port, &addr) != uvpp::uerr::UVPP_SUCCESS) {
        return false;
    }

    if (_client->connect((sockaddr const *)&addr) != uvpp::uerr::UVPP_SUCCESS) {
        return false;
    }

    return true;
}

bool TcpClient::initIpv6(std::string const & ip, int port)
{
    return false;
}

bool TcpClient::init(std::string const & ip, int port)
{
    if (Tcp::isIpv4(ip)) {
        return initIpv4(ip, port);
    } else if (Tcp::isIpv6(ip)) {
        return initIpv6(ip, port);
    }
    return false;
}

bool TcpClient::run()
{
    return _loop.run() == uerr::UVPP_SUCCESS;
}

bool TcpClient::run(std::string const & ip, int port)
{
    return init(ip, port) && run();
}

bool TcpClient::asyncClose()
{
    Guard guard(_async_mutex);
    if (std::this_thread::get_id() == _loop.getOwnerThreadId()) {
        _client->close();
        return true;
    }

    WeakClient weak = std::static_pointer_cast<ClientTcp>(_loop.findChildHandle(*_client).lock());
    if (weak.expired()) {
        return false;
    }

    auto close_client_shared = _async->newPushJob<CloseJob>(weak);
    auto close_async_shared  = _async->newPushJob<CloseSelfJob>();

    if (static_cast<bool>(close_client_shared) && static_cast<bool>(close_async_shared)) {
        return _async->send() == uerr::UVPP_SUCCESS;
    }
    return false;
}

bool TcpClient::asyncWrite(char const * buffer, std::size_t size)
{
    if (buffer == nullptr || size == 0) {
        return false;
    }

    Guard guard(_async_mutex);
    _client->pushWriteBuffer(buffer, size);
    if (std::this_thread::get_id() == _loop.getOwnerThreadId()) {
        return _client->writeWithPushedBuffer() == uerr::UVPP_SUCCESS;
    }

    WeakClient weak = std::static_pointer_cast<ClientTcp>(_loop.findChildHandle(*_client).lock());
    if (weak.expired()) {
        return false;
    }

    if (auto shared = _async->newPushJob<WriteJob>(weak)) {
        return _async->send() == uerr::UVPP_SUCCESS;
    }
    return false;
}

// -----------------
// Callback methods.
// -----------------

void TcpClient::onConnect(BaseTcp & tcp, ConnectRequest & request, uerr code)
{
    assert(tcp.getCsType() == CsType::CLIENT);
    return onClientConnect(request, code);
}

TcpClient::binf TcpClient::onAlloc(BaseTcp & tcp, std::size_t suggested_size)
{
    assert(tcp.getCsType() == CsType::CLIENT);
    return onClientAlloc(suggested_size);
}

void TcpClient::onWrite(BaseTcp & tcp, WriteRequest & request, uerr code)
{
    assert(tcp.getCsType() == CsType::CLIENT);
    onClientWrite(request, code);
}

bool TcpClient::onRead(BaseTcp & tcp, uerr code, char const * buffer, std::size_t size)
{
    assert(tcp.getCsType() == CsType::CLIENT);
    return onClientRead(code, buffer, size);
}

void TcpClient::onClose(BaseTcp & tcp)
{
    assert(tcp.getCsType() == CsType::CLIENT);
    onClientClose();
}

void TcpClient::onReadEof(BaseTcp & tcp, uerr code, char const * buffer, std::size_t size)
{
    assert(tcp.getCsType() == CsType::CLIENT);
    onClientReadEof(code, buffer, size);
}

void TcpClient::onReadDatagram(BaseTcp & tcp, uerr code, char const * buffer, std::size_t size)
{
    assert(tcp.getCsType() == CsType::CLIENT);
    onClientReadDatagram(code, buffer, size);
}

void TcpClient::onReadError(BaseTcp & tcp, uerr code, char const * buffer, std::size_t size)
{
    assert(tcp.getCsType() == CsType::CLIENT);
    onClientReadError(code, buffer, size);
}

void TcpClient::onAsyncWrite(BaseTcp & tcp, uerr code)
{
    assert(tcp.getCsType() == CsType::CLIENT);
    onClientAsyncWrite(code);
}

// --------------
// Event methods.
// --------------

void TcpClient::onClientConnect(ConnectRequest & request, uerr code)
{
    __tbag_debug("TcpClient::onClientConnect() result code({})", static_cast<int>(code));
}

TcpClient::binf TcpClient::onClientAlloc(std::size_t suggested_size)
{
    return uvpp::defaultOnAlloc(_client->atReadBuffer(), suggested_size);
}

void TcpClient::onClientWrite(WriteRequest & request, uerr code)
{
    __tbag_debug("TcpClient::onClientWrite() result code({})", static_cast<int>(code));
}

bool TcpClient::onClientRead(uerr code, char const * buffer, std::size_t size)
{
    __tbag_debug("TcpClient::onClientRead() result code({})", static_cast<int>(code));
    return false;
}

void TcpClient::onClientReadEof(uerr code, char const * buffer, std::size_t size)
{
    __tbag_debug("TcpClient::onClientReadEof() result code({})", static_cast<int>(code));
    _client->close();
}

void TcpClient::onClientReadDatagram(uerr code, char const * buffer, std::size_t size)
{
    __tbag_debug("TcpClient::onClientReadDatagram() result code({})", static_cast<int>(code));
}

void TcpClient::onClientReadError(uerr code, char const * buffer, std::size_t size)
{
    __tbag_debug("TcpClient::onClientReadError() result code({})", static_cast<int>(code));
    _client->close();
}

void TcpClient::onClientAsyncWrite(uerr code)
{
    __tbag_debug("TcpClient::onClientAsyncWrite() result code({})", static_cast<int>(code));
}

void TcpClient::onClientClose()
{
    __tbag_debug("TcpClient::onClientClose()");
    _async->close();
}

} // namespace network

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

