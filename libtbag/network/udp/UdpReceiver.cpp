/**
 * @file   UdpReceiver.cpp
 * @brief  UdpReceiver class implementation.
 * @author zer0
 * @date   2017-06-17
 */

#include <libtbag/network/udp/UdpReceiver.hpp>
#include <libtbag/debug/Assert.hpp>
#include <libtbag/log/Log.hpp>
#include <libtbag/Noncopyable.hpp>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace network {
namespace udp     {

/**
 * UdpReceiver internal helper class.
 *
 * @author zer0
 * @date   2017-06-17
 *
 * @warning
 *  Don't mutex guard in this class. (However, callbacks are an exception)
 */
struct UdpReceiver::Internal : private Noncopyable
{
    using binf = UdpReceiver::binf;

    UdpReceiver * _parent;

    Internal(UdpReceiver * parent) : _parent(parent)
    {
        // EMPTY.
    }

    ~Internal()
    {
        // EMPTY.
    }

    bool initHandles()
    {
        assert(_parent != nullptr);
        UdpReceiver & p = *_parent;

        assert(p._client != nullptr);
        Loop * loop = p._client->getLoop();
        assert(loop != nullptr);

        if (static_cast<bool>(p._async) == false) {
            p._async = loop->newHandle<SafetyAsync>(*loop);
        }
        assert(static_cast<bool>(p._async));
        return true;
    }

    void closeAll()
    {
        assert(_parent != nullptr);
        UdpReceiver & p = *_parent;

        assert(static_cast<bool>(p._client));
        if (p._client->isClosing() == false) {
            p._client->close();
        }

        if (p._owner_async && static_cast<bool>(p._async)) {
            if (p._async->isClosing() == false) {
                p._async->close();
            }
            p._async.reset();
        }
    }
};

// ---------------------------
// UdpReceiver implementation.
// ---------------------------

UdpReceiver::UdpReceiver(Loop & loop, SharedSafetyAsync async, ServerInterface * parent) : _parent(parent)
{
    _client = loop.newHandle<UdpNodeBackend>(loop, this);
    assert(static_cast<bool>(_client));

    if (static_cast<bool>(async) && loop.id() == async->getLoop()->id()) {
        _owner_async = false;
        _async = async;
    } else {
        _owner_async = true;
        _async.reset();
    }
}

UdpReceiver::~UdpReceiver()
{
    // EMPTY.
}

UdpReceiver::Id UdpReceiver::id() const
{
    assert(static_cast<bool>(_client));
    return _client->id();
}

std::string UdpReceiver::dest() const
{
    assert(static_cast<bool>(_client));
    return _client->getSockIp();
}

int UdpReceiver::port() const
{
    assert(static_cast<bool>(_client));
    return _client->getSockPort();
}

void * UdpReceiver::udata()
{
    assert(static_cast<bool>(_client));
    return _client->getUserData();
}

Err UdpReceiver::init(char const * destination, int port, uint64_t millisec)
{
    assert(static_cast<bool>(_client));
    assert(static_cast<bool>(_internal));

    Guard const MUTEX_GUARD(_mutex);
    if (static_cast<bool>(_client) == false) {
        return Err::E_EXPIRED;
    }

    if (uvpp::initRecvUdp(*_client, destination, port) == false) {
        tDLogE("UdpReceiver::init() Initialize fail.");
        return Err::E_UNKNOWN;
    }

    if (_internal->initHandles() == false) {
        tDLogE("UdpReceiver::init() Initialize fail (internal handles).");
        return Err::E_UNKNOWN;
    }

    return Err::E_SUCCESS;
}

Err UdpReceiver::start()
{
    assert(static_cast<bool>(_client));
    Guard const MUTEX_GUARD(_mutex);
    Err const CODE = _client->startRecv();
    if (CODE != Err::E_SUCCESS) {
        tDLogE("UdpReceiver::start() {} error", getErrName(CODE));
    }
    return CODE;
}

Err UdpReceiver::stop()
{
    assert(static_cast<bool>(_client));
    Guard const MUTEX_GUARD(_mutex);
    Err const CODE = _client->stopRecv();
    if (CODE != Err::E_SUCCESS) {
        tDLogE("UdpReceiver::stop() {} error", getErrName(CODE));
    }
    return CODE;
}

void UdpReceiver::close()
{
    assert(static_cast<bool>(_client));
    assert(static_cast<bool>(_internal));
    Loop * loop = _client->getLoop();
    assert(loop != nullptr);

    Guard const MUTEX_GUARD(_mutex);
    if (loop->isAliveAndThisThread() || static_cast<bool>(_async) == false) {
        tDLogD("UdpReceiver::close() request.");
        _internal->closeAll();
    } else {
        tDLogD("UdpReceiver::close() async request.");
        _async->newSendFunc([&](){
            Guard const MUTEX_GUARD(_mutex);
            _internal->closeAll();
        });
    }
}

void UdpReceiver::cancel()
{
    // EMPTY.
}

Err UdpReceiver::write(binf const * buffer, std::size_t size, uint64_t millisec)
{
    return Err::E_UNSUPOP;
}

Err UdpReceiver::write(char const * buffer, std::size_t size, uint64_t millisec)
{
    return Err::E_UNSUPOP;
}

// --------------
// Event backend.
// --------------

void UdpReceiver::backConnect(Err code)
{
    TBAG_INACCESSIBLE_BLOCK_ASSERT();
}

void UdpReceiver::backShutdown(Err code)
{
    TBAG_INACCESSIBLE_BLOCK_ASSERT();
}

void UdpReceiver::backWrite(Err code)
{
    TBAG_INACCESSIBLE_BLOCK_ASSERT();
}

void UdpReceiver::backRead(Err code, ReadPacket const & packet)
{
    onRead(code, packet);
}

void UdpReceiver::backClose()
{
    assert(static_cast<bool>(_internal));
    _mutex.lock();
    _internal->closeAll();
    _mutex.unlock();

    onClose();
}

} // namespace udp
} // namespace network

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

