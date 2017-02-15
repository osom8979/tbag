/**
 * @file   BaseTcp.hpp
 * @brief  BaseTcp class prototype.
 * @author zer0
 * @date   2017-02-15
 */

#include <libtbag/network/BaseTcp.hpp>
#include <libtbag/log/Log.hpp>

#include <cassert>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace network {

// -------------------------
// ClientTcp implementation.
// -------------------------

ClientTcp::ClientTcp(Loop & loop, TcpCallback * callback) : BaseTcp(loop, CsType::CLIENT, callback)
{
    _connect_ready.store(true);
    _write_ready.store(true);
}

ClientTcp::~ClientTcp()
{
    // EMPTY.
}

ClientTcp::uerr ClientTcp::connect(sockaddr const * address)
{
    bool IS_READY = true;
    if (_connect_ready.compare_exchange_weak(IS_READY, false)) {
        return Tcp::connect(_connect_request, address);
    }
    return uerr::UVPP_NREADY;
}

ClientTcp::uerr ClientTcp::write(char const * buffer, std::size_t size)
{
    bool IS_READY = true;
    if (_write_ready.compare_exchange_weak(IS_READY, false)) {
        return Tcp::write(_write_request, buffer, size);
    }
    return uerr::UVPP_NREADY;
}

bool ClientTcp::pushWriteBuffer(char const * buffer, std::size_t size)
{
    return _datagram.pushWriteBuffer(buffer, size);
}

ClientTcp::uerr ClientTcp::writeWithPushedBuffer()
{
    uerr result = uerr::UVPP_UNKNOWN;
    bool const POP_RESULT = _datagram.popWriteBuffer([&result, this](char const * buffer, std::size_t size){
        result = write(buffer, size);
    });
    if (POP_RESULT == false) {
        result = uerr::UVPP_EQUEUE;
    }
    return result;
}

void ClientTcp::onConnect(ConnectRequest & request, uerr code)
{
    getCallback()->onConnect(*this, request, code);
    assert(_connect_ready.load() == false);
    _connect_ready.store(true);
}

ClientTcp::binf ClientTcp::onAlloc(std::size_t suggested_size)
{
    return getCallback()->onAlloc(*this, suggested_size);
}

void ClientTcp::onRead(uerr code, char const * buffer, std::size_t size)
{
    if (getCallback()->onRead(*this, code, buffer, size)) {
        return;
    }

    if (code == uerr::UVPP_SUCCESS) {
        _datagram.push(buffer, size);

        binf info;
        while (_datagram.next(&info)) {
            getCallback()->onReadDatagram(*this, code, info.buffer, info.size);
        }
    } else if (code == uerr::UVPP_EOF) {
        getCallback()->onReadEof(*this, code, buffer, size);
    } else {
        getCallback()->onReadError(*this, code, buffer, size);
    }
}

void ClientTcp::onWrite(WriteRequest & request, uerr code)
{
    getCallback()->onWrite(*this, request, code);
    assert(_write_ready.load() == false);
    _write_ready.store(true);
}

void ClientTcp::onClose()
{
    getCallback()->onClose(*this);
}

// -------------------------
// ServerTcp implementation.
// -------------------------

ServerTcp::ServerTcp(Loop & loop, TcpCallback * callback) : BaseTcp(loop, CsType::SERVER, callback)
{
    // EMPTY.
}

ServerTcp::~ServerTcp()
{
    // EMPTY.
}

void ServerTcp::onConnection(uerr code)
{
    getCallback()->onConnection(*this, code);
}

void ServerTcp::onClose()
{
    getCallback()->onClose(*this);
}

// -------------------------
// WriteJob implementation.
// -------------------------

WriteJob::WriteJob(WeakTcp tcp) : _tcp(tcp)
{
    // EMPTY.
}

WriteJob::~WriteJob()
{
    // EMPTY.
}

void WriteJob::run(Async * handle)
{
    if (auto shared = _tcp.lock()) {
        TcpCallback * callback = shared->getCallback();

        if (shared->getCsType() == CsType::CLIENT) {
            auto client = std::static_pointer_cast<ClientTcp>(shared);
            callback->onAsyncWrite(*shared, client->writeWithPushedBuffer());
        } else /* CsType::SERVER */ {
            callback->onAsyncWrite(*shared, uerr::UVPP_UNSUPOP);
        }
    } else {
        // Not found tcp.
        // uerr::UVPP_NULLPTR;
    }
}

// -------------------------
// CloseJob implementation.
// -------------------------

CloseJob::CloseJob(WeakTcp tcp) : _tcp(tcp)
{
    // EMPTY.
}

CloseJob::~CloseJob()
{
    // EMPTY.
}

void CloseJob::run(Async * handle)
{
    if (auto shared = _tcp.lock()) {
        if (shared->isInit()) {
            shared->close();
        }
    }
}

// ----------------------------
// CloseSelfJob implementation.
// ----------------------------

CloseSelfJob::CloseSelfJob()
{
    // EMPTY.
}

CloseSelfJob::~CloseSelfJob()
{
    // EMPTY.
}

void CloseSelfJob::run(Async * handle)
{
    assert(handle != nullptr);
    if (handle->isInit()) {
        handle->clearJob();
        handle->close();
    }
}

} // namespace network

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

