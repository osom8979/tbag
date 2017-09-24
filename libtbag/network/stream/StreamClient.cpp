/**
 * @file   StreamClient.cpp
 * @brief  StreamClient class implementation.
 * @author zer0
 * @date   2017-05-22
 */

#include <libtbag/network/stream/StreamClient.hpp>
#include <libtbag/network/stream/StreamClientBackend.hpp>
#include <libtbag/uvpp/Timer.hpp>
#include <libtbag/debug/Assert.hpp>
#include <libtbag/log/Log.hpp>
#include <libtbag/Type.hpp>
#include <libtbag/Noncopyable.hpp>
#include <libtbag/string/StringUtils.hpp>

#include <cstring>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace network {
namespace stream  {

// ----------------------------
// StreamClient implementation.
// ----------------------------

StreamClient::StreamClient(Loop & loop, StreamType type)
        : _internal(this, loop, type)
{
    // EMPTY.
}

StreamClient::StreamClient(Loop & loop, StreamType type, WriteReady const & ready)
        : _internal(this, loop, type, ready)
{
    // EMPTY.
}

StreamClient::~StreamClient()
{
    // EMPTY.
}

StreamClient::WriteState StreamClient::getWriteState() const
{
    return _internal.getState();
}

char const * StreamClient::getWriteStateName() const
{
    return _internal.getStateName();
}

StreamClient::WeakClientBackend StreamClient::getClient()
{
    return _internal.getClient();
}

StreamClient::WeakSafetyAsync StreamClient::getAsync()
{
    return _internal.getAsync();
}

StreamClient::Id StreamClient::id() const
{
    return _internal.getId();
}

void * StreamClient::udata()
{
    return _internal.getUserData();
}

std::string StreamClient::dest() const
{
    return _internal.dest();
}

int StreamClient::port() const
{
    return _internal.port();
}

Err StreamClient::init(char const * destination, int port)
{
    Err const INIT_CODE = _internal.initClient(_internal.getStreamType(), std::string(destination), port);
    if (INIT_CODE != Err::E_SUCCESS) {
        tDLogE("StreamClient::init() Initialize fail.");
        return INIT_CODE;
    }

    if (TBAG_ERR_FAILURE(_internal.initInternalHandles())) {
        tDLogE("StreamClient::init() Initialize fail (internal handles).");
        return Err::E_UNKNOWN;
    }
    return Err::E_SUCCESS;
}

Err StreamClient::start()
{
    return _internal.startRead();
}

Err StreamClient::stop()
{
    return _internal.stopRead();
}

Err StreamClient::close()
{
    return _internal.close();
}

Err StreamClient::cancel()
{
    return _internal.cancel();
}

Err StreamClient::write(char const * buffer, std::size_t size)
{
    return _internal.autoWrite(buffer, size);
}

void StreamClient::setWriteTimeout(uint64_t millisec)
{
    _internal.setWriteTimeout(millisec);
}

Err StreamClient::startTimer(uint64_t millisec)
{
    return _internal.startUserTimer(millisec);
}

void StreamClient::stopTimer()
{
    _internal.stopUserTimer();
}

bool StreamClient::isActiveTimer()
{
    return _internal.isActiveUserTimer();
}

// --------------
// Event backend.
// --------------

void StreamClient::backConnect(Err code)
{
    _internal.preConnect(code);
    onConnect(code);
}

void StreamClient::backShutdown(Err code)
{
    _internal.preShutdown(code);
    onShutdown(code);
}

void StreamClient::backWrite(Err code)
{
    _internal.preWrite(code);
    onWrite(code);
}

void StreamClient::backRead(Err code, ReadPacket const & packet)
{
    _internal.preRead(code, packet);
    onRead(code, packet);
}

void StreamClient::backClose()
{
    _internal.preClose();
    onClose();
}

} // namespace stream
} // namespace network

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

