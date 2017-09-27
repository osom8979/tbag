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
        : _client(this, loop, type)
{
    // EMPTY.
}

StreamClient::StreamClient(Loop & loop, StreamType type, UpdateReady const & ready)
        : _client(this, loop, type, ready)
{
    // EMPTY.
}

StreamClient::~StreamClient()
{
    // EMPTY.
}

StreamClient::WriteState StreamClient::getWriteState() const
{
    return _client.getState();
}

char const * StreamClient::getWriteStateName() const
{
    return _client.getStateName();
}

StreamClient::WeakStream StreamClient::getClient()
{
    return _client.getClient();
}

StreamClient::WeakSafetyAsync StreamClient::getAsync()
{
    return _client.getAsync();
}

StreamClient::Id StreamClient::id() const
{
    return _client.id();
}

void * StreamClient::udata(void * data)
{
    return _client.udata(data);
}

std::string StreamClient::dest() const
{
    return _client.dest();
}

int StreamClient::port() const
{
    return _client.port();
}

Err StreamClient::init(char const * destination, int port)
{
    Err const INIT_CODE = _client.initClient(std::string(destination), port);
    if (TBAG_ERR_FAILURE(INIT_CODE)) {
        tDLogE("StreamClient::init() Initialize {} error.", getErrName(INIT_CODE));
        return INIT_CODE;
    }

    if (TBAG_ERR_FAILURE(_client.initInternalHandles())) {
        tDLogE("StreamClient::init() Initialize fail (internal handles).");
        return Err::E_UNKNOWN;
    }
    return Err::E_SUCCESS;
}

Err StreamClient::start()
{
    return _client.startRead();
}

Err StreamClient::stop()
{
    return _client.stopRead();
}

Err StreamClient::close()
{
    return _client.close();
}

Err StreamClient::cancel()
{
    return _client.cancel();
}

Err StreamClient::write(char const * buffer, std::size_t size)
{
    return _client.write(buffer, size);
}

void StreamClient::setWriteTimeout(uint64_t millisec)
{
    _client.setWriteTimeout(millisec);
}

Err StreamClient::startTimer(uint64_t millisec)
{
    return _client.startUserTimer(millisec);
}

void StreamClient::stopTimer()
{
    _client.stopUserTimer();
}

bool StreamClient::isActiveTimer()
{
    return _client.isActiveUserTimer();
}

// --------------
// Event backend.
// --------------

void StreamClient::backConnect(Err code)
{
    _client.preConnect(code);
    onConnect(code);
}

void StreamClient::backShutdown(Err code)
{
    _client.preShutdown(code);
    onShutdown(code);
}

void StreamClient::backWrite(Err code)
{
    _client.preWrite(code);
    onWrite(code);
}

void StreamClient::backRead(Err code, ReadPacket const & packet)
{
    _client.preRead(code, packet);
    onRead(code, packet);
}

void StreamClient::backClose()
{
    _client.preClose();
    onClose();
}

} // namespace stream
} // namespace network

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

