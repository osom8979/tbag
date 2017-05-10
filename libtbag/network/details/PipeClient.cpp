/**
 * @file   PipeClient.cpp
 * @brief  PipeClient class implementation.
 * @author zer0
 * @date   2017-05-09
 */

#include <libtbag/network/details/PipeClient.hpp>
#include <libtbag/filesystem/Path.hpp>
#include <libtbag/log/Log.hpp>
#include <cassert>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace network {
namespace details {

// -----------------------------
// PipeRealClient implementation.
// -----------------------------

PipeRealClient::PipeRealClient(Loop & loop, PipeClient & parent) : Pipe(loop), _parent(parent)
{
    setUserData(_parent.onUserDataAlloc());
}

PipeRealClient::~PipeRealClient()
{
    _parent.onUserDataDealloc(getUserData());
}

bool PipeRealClient::init(String const & path, int UNUSED_PARAM(unused))
{
    if (filesystem::Path(path).exists() == false) {
        return false;
    }

    connect(_connect_req, path.c_str());
    return true;
}

void PipeRealClient::onConnect(ConnectRequest & request, uerr code)
{
    tDLogD("PipeRealClient::onConnect({})", uvpp::getErrorName(code));

    {   // Update parent state.
        Guard guard(_parent._mutex);
        _parent.cancelTimeoutClose();
    }

    _parent.onConnect(code);
}

void PipeRealClient::onShutdown(ShutdownRequest & request, uerr code)
{
    tDLogD("PipeRealClient::onShutdown({})", uvpp::getErrorName(code));

    {   // Update parent state.
        Guard guard(_parent._mutex);
        _parent.cancelTimeoutShutdown();
        _parent._last_writer.reset();
    }

    _parent.onShutdown(code);
}

void PipeRealClient::onWrite(WriteRequest & request, uerr code)
{
    tDLogD("PipeRealClient::onWrite({})", uvpp::getErrorName(code));

    {   // Update parent state.
        Guard guard(_parent._mutex);
        _parent.cancelTimeoutShutdown();
        _parent._last_writer.reset();
    }

    _parent.onWrite(code);
}

PipeRealClient::binf PipeRealClient::onAlloc(std::size_t suggested_size)
{
    return uvpp::defaultOnAlloc(_buffer, suggested_size);
}

void PipeRealClient::onRead(uerr code, char const * buffer, std::size_t size)
{
    tDLogD("PipeRealClient::onRead({})", uvpp::getErrorName(code));
    _parent.onRead(code, buffer, size);
}

void PipeRealClient::onClose()
{
    tDLogD("PipeRealClient::onClose()");
    _parent.onClose();

    {   // Update parent state.
        Guard guard(_parent._mutex);
        _parent.closeAll();
    }
}

// -------------------------
// PipeClient implementation.
// -------------------------

PipeClient::PipeClient(Loop & loop)
{
    _client   = loop.newHandle<PipeRealClient>(loop, *this);
    _async    = loop.newHandle<SafetyWriteAsync>(loop);
    _close    = loop.newHandle<TimeoutClose>(loop, _client.get(), false);
    _shutdown = loop.newHandle<TimeoutShutdown>(loop, _client.get(), false);
    assert(static_cast<bool>(_client));
    assert(static_cast<bool>(_async));
    assert(static_cast<bool>(_close));
    assert(static_cast<bool>(_shutdown));
}

PipeClient::~PipeClient()
{
    _client.reset();
    _async.reset();
    _close.reset();
    _shutdown.reset();
    _last_writer.reset();
}

void PipeClient::startTimeoutShutdown(Milliseconds const & millisec)
{
    assert(static_cast<bool>(_shutdown));
    uerr const CODE = _shutdown->start(static_cast<uint64_t>(millisec.count()));
    tDLogD("PipeClient::startTimeoutShutdown({}) result code: {}", millisec.count(), uvpp::getErrorName(CODE));
}

void PipeClient::startTimeoutClose(Milliseconds const & millisec)
{
    assert(static_cast<bool>(_close));
    uerr const CODE = _close->start(static_cast<uint64_t>(millisec.count()));
    tDLogD("PipeClient::startTimeoutClose({}) result code: {}", millisec.count(), uvpp::getErrorName(CODE));
}

void PipeClient::cancelTimeoutShutdown()
{
    assert(static_cast<bool>(_shutdown));
    if (_shutdown->isActive()) {
        _shutdown->cancel();
    }
    tDLogD("PipeClient::cancelTimeoutShutdown().");
}

void PipeClient::cancelTimeoutClose()
{
    assert(static_cast<bool>(_close));
    if (_close->isActive()) {
        _close->cancel();
    }
    tDLogD("PipeClient::cancelTimeoutClose().");
}

bool PipeClient::write(SafetyWriteAsync::SharedWriter writer, uint64_t millisec)
{
    assert(static_cast<bool>(_client));
    assert(static_cast<bool>(_async));

    if (static_cast<bool>(_last_writer)) {
        tDLogD("PipeClient::write() busy state.");
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

void PipeClient::closeAll()
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

bool PipeClient::init(String const & path, int unused, uint64_t millisec)
{
    assert(static_cast<bool>(_client));
    Guard guard(_mutex);
    if (static_cast<bool>(_client) && _client->init(path)) {
        if (millisec >= 1U) {
            startTimeoutClose(Milliseconds(millisec));
        }
        return true;
    }
    return false;
}

bool PipeClient::start()
{
    assert(static_cast<bool>(_client));
    Guard guard(_mutex);
    return _client->startRead() == uerr::UVPP_SUCCESS;
}

bool PipeClient::stop()
{
    assert(static_cast<bool>(_client));
    Guard guard(_mutex);
    return _client->stopRead() == uerr::UVPP_SUCCESS;
}

void PipeClient::close()
{
    assert(static_cast<bool>(_client));
    assert(static_cast<bool>(_async));

    Loop * loop = _client->getLoop();
    assert(loop != nullptr);

    if (loop->isAliveAndThisThread()) {
        tDLogD("PipeServer::close() sync request.");
        Guard guard(_mutex);
        closeAll();
    } else {
        tDLogD("PipeServer::close() async request.");
        _async->newSendFunc([&](SafetyAsync * UNUSED_PARAM(async)) {
            Guard guard(_mutex);
            closeAll();
        });
    }
}

void PipeClient::cancel()
{
    assert(static_cast<bool>(_client));
    assert(static_cast<bool>(_async));

    Loop * loop = _client->getLoop();
    assert(loop != nullptr);

    if (loop->isAliveAndThisThread()) {
        tDLogD("PipeServer::cancel() sync request.");
        Guard guard(_mutex);
        startTimeoutShutdown(Milliseconds(0U));
    } else {
        tDLogD("PipeServer::cancel() async request.");
        _async->newSendFunc([&](SafetyAsync * UNUSED_PARAM(async)) {
            Guard guard(_mutex);
            startTimeoutShutdown(Milliseconds(0U));
        });
    }
}

bool PipeClient::write(binf const * buffer, Size size, uint64_t millisec)
{
    assert(static_cast<bool>(_client));
    Guard guard(_mutex);
    return write(SafetyWriteAsync::createWrite(SafetyWriteAsync::WeakStream(_client), buffer, size), millisec);
}

bool PipeClient::write(char const * buffer, Size size, uint64_t millisec)
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

