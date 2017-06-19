/**
 * @file   UdpSender.cpp
 * @brief  UdpSender class implementation.
 * @author zer0
 * @date   2017-06-17
 */

#include <libtbag/network/udp/UdpSender.hpp>
#include <libtbag/debug/Assert.hpp>
#include <libtbag/log/Log.hpp>
#include <libtbag/Noncopyable.hpp>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace network {
namespace udp     {

/**
 * UdpSender internal helper class.
 *
 * @author zer0
 * @date   2017-06-17
 *
 * @warning
 *  Don't mutex guard in this class. (However, callbacks are an exception)
 */
struct UdpSender::Internal : private Noncopyable
{
    using binf = UdpSender::binf;

    UdpSender * _parent;

    Internal(UdpSender * parent) : _parent(parent)
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
        UdpSender & p = *_parent;

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
        UdpSender & p = *_parent;

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

UdpSender::UdpSender(Loop & loop, SharedSafetyAsync async, ServerInterface * parent)
        : _parent(parent)
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

    _sender.status = SendStatus::SS_READY;
}

UdpSender::~UdpSender()
{
    // EMPTY.
}

char const * UdpSender::getSendStatusName(SendStatus status) TBAG_NOEXCEPT
{
    switch (status) {
    case SendStatus::SS_READY:         return "READY";
    case SendStatus::SS_ASYNC:         return "ASYNC";
    case SendStatus::SS_ASYNC_CANCEL:  return "ASYNC_CANCEL";
    case SendStatus::SS_SEND:          return "SEND";
    case SendStatus::SS_END:           return "END";
    }

    TBAG_INACCESSIBLE_BLOCK_ASSERT();
    return "UNKNOWN";
}

void UdpSender::onAsyncSend()
{
}

UdpSender::Id UdpSender::id() const
{
    assert(static_cast<bool>(_client));
    return _client->id();
}

std::string UdpSender::dest() const
{
    if (_addr.ipv4.sin_family == AF_INET) {
        return uvpp::getIpName(&_addr.ipv4);
    } else if (_addr.ipv6.sin6_family == AF_INET6) {
        return uvpp::getIpName(&_addr.ipv6);
    }
    return std::string(); // Unsupported family.
}

int UdpSender::port() const
{
    if (_addr.ipv4.sin_family == AF_INET) {
        return uvpp::getPortNumber(&_addr.ipv4);
    } else if (_addr.ipv6.sin6_family == AF_INET6) {
        return uvpp::getPortNumber(&_addr.ipv6);
    }
    return 0;
}

void * UdpSender::udata()
{
    assert(static_cast<bool>(_client));
    return _client->getUserData();
}

Err UdpSender::init(char const * destination, int port, uint64_t millisec)
{
    // TODO...
}

Err UdpSender::start()
{
    return Err::E_UNSUPOP;
}

Err UdpSender::stop()
{
    return Err::E_UNSUPOP;
}

void UdpSender::close()
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

void UdpSender::cancel()
{
    // TODO...
}

Err UdpSender::write(binf const * buffer, std::size_t size, uint64_t millisec)
{
    // TODO...
}

Err UdpSender::write(char const * buffer, std::size_t size, uint64_t millisec)
{
    // TODO...
}

// --------------
// Event backend.
// --------------

void UdpSender::backConnect(Err code)
{
    TBAG_INACCESSIBLE_BLOCK_ASSERT();
}

void UdpSender::backShutdown(Err code)
{
    TBAG_INACCESSIBLE_BLOCK_ASSERT();
}

void UdpSender::backWrite(Err code)
{
    assert(static_cast<bool>(_internal));
    _mutex.lock();
    _sender.status = SendStatus::SS_READY;
    _mutex.unlock();

    onWrite(code);
}

void UdpSender::backRead(Err code, ReadPacket const & packet)
{
    TBAG_INACCESSIBLE_BLOCK_ASSERT();
}

void UdpSender::backClose()
{
    assert(static_cast<bool>(_internal));
    _mutex.lock();
    _internal->closeAll();
    _sender.status = SendStatus::SS_END;
    _mutex.unlock();

    onClose();
}

} // namespace udp
} // namespace network

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

