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

    UdpReceiver * parent;

    bool owner_async;
    unsigned int bind_flags;

    SharedClientBackend client;
    SharedSafetyAsync   async;

    Internal(UdpReceiver * parent) : parent(parent), owner_async(false), bind_flags(0)
    {
        // EMPTY.
    }

    ~Internal()
    {
        // EMPTY.
    }

    Loop & getLoop()
    {
        assert(static_cast<bool>(client));
        Loop * loop = client->getLoop();
        assert(loop != nullptr);
        return *loop;
    }

    bool initHandles()
    {
        Loop & loop = getLoop();
        if (static_cast<bool>(async) == false) {
            async = loop.newHandle<SafetyAsync>(loop);
        }
        assert(static_cast<bool>(async));
        return true;
    }

    void closeAll()
    {
        assert(static_cast<bool>(client));
        if (client->isClosing() == false) {
            client->close();
        }

        if (owner_async && static_cast<bool>(async)) {
            if (async->isClosing() == false) {
                async->close();
            }
            async.reset();
        }
    }
};

// ---------------------------
// UdpReceiver implementation.
// ---------------------------

UdpReceiver::UdpReceiver(Loop & loop, ServerInterface * parent) : UdpReceiver(loop, SharedSafetyAsync(), parent)
{
    // EMPTY.
}

UdpReceiver::UdpReceiver(Loop & loop, SharedSafetyAsync async, ServerInterface * interface)
        : _interface(interface), _internal(new Internal(this))
{
    assert(static_cast<bool>(_internal));
    _internal->client = loop.newHandle<UdpNodeBackend>(loop, this);
    assert(static_cast<bool>(_internal->client));

    if (static_cast<bool>(async) && loop.id() == async->getLoop()->id()) {
        _internal->owner_async = false;
        _internal->async = async;
    } else {
        _internal->owner_async = true;
        _internal->async.reset();
    }
}

UdpReceiver::~UdpReceiver()
{
    // EMPTY.
}

void UdpReceiver::setBindFlags(unsigned int flags) TBAG_NOEXCEPT
{
    assert(static_cast<bool>(_internal));
    Guard const MUTEX_GUARD(_mutex);
    _internal->bind_flags = flags;
}

unsigned int UdpReceiver::getBindFlags() const TBAG_NOEXCEPT
{
    assert(static_cast<bool>(_internal));
    Guard const MUTEX_GUARD(_mutex);
    return _internal->bind_flags;
}

UdpReceiver::WeakClientBackend UdpReceiver::getClient()
{
    assert(static_cast<bool>(_internal));
    assert(static_cast<bool>(_internal->client));
    Guard const MUTEX_GUARD(_mutex);
    return WeakClientBackend(_internal->client);
}

UdpReceiver::WeakSafetyAsync UdpReceiver::getAsync()
{
    assert(static_cast<bool>(_internal));
    assert(static_cast<bool>(_internal->client));
    Guard const MUTEX_GUARD(_mutex);
    return WeakSafetyAsync(_internal->async);
}

UdpReceiver::Id UdpReceiver::id() const
{
    assert(static_cast<bool>(_internal));
    assert(static_cast<bool>(_internal->client));
    Guard const MUTEX_GUARD(_mutex);
    return _internal->client->id();
}

std::string UdpReceiver::dest() const
{
    assert(static_cast<bool>(_internal));
    assert(static_cast<bool>(_internal->client));
    Guard const MUTEX_GUARD(_mutex);
    return _internal->client->getSockIp();
}

int UdpReceiver::port() const
{
    assert(static_cast<bool>(_internal));
    assert(static_cast<bool>(_internal->client));
    Guard const MUTEX_GUARD(_mutex);
    return _internal->client->getSockPort();
}

void * UdpReceiver::udata()
{
    assert(static_cast<bool>(_internal));
    assert(static_cast<bool>(_internal->client));
    Guard const MUTEX_GUARD(_mutex);
    return _internal->client->getUserData();
}

Err UdpReceiver::init(char const * destination, int port)
{
    assert(static_cast<bool>(_internal));
    assert(static_cast<bool>(_internal->client));
    Guard const MUTEX_GUARD(_mutex);

    if (static_cast<bool>(_internal->client) == false) {
        return Err::E_EXPIRED;
    }

    if (uvpp::initRecvUdp(*(_internal->client), destination, port, _internal->bind_flags) == false) {
        tDLogE("UdpReceiver::init() Initialize fail.");
        return Err::E_UNKNOWN;
    }

    if (_internal->initHandles() == false) {
        tDLogE("UdpReceiver::init() Initialize fail (internal handles).");
        return Err::E_UNKNOWN;
    }

    return _internal->client->startRecv();
}

Err UdpReceiver::start()
{
    assert(static_cast<bool>(_internal));
    assert(static_cast<bool>(_internal->client));
    Guard const MUTEX_GUARD(_mutex);

    Err const CODE = _internal->client->startRecv();
    if (CODE != Err::E_SUCCESS) {
        tDLogE("UdpReceiver::start() {} error", getErrName(CODE));
    }
    return CODE;
}

Err UdpReceiver::stop()
{
    assert(static_cast<bool>(_internal));
    assert(static_cast<bool>(_internal->client));
    Guard const MUTEX_GUARD(_mutex);

    Err const CODE = _internal->client->stopRecv();
    if (CODE != Err::E_SUCCESS) {
        tDLogE("UdpReceiver::stop() {} error", getErrName(CODE));
    }
    return CODE;
}

Err UdpReceiver::close()
{
    assert(static_cast<bool>(_internal));
    assert(static_cast<bool>(_internal->client));
    Guard const MUTEX_GUARD(_mutex);
    Loop & loop = _internal->getLoop();

    if (loop.isAliveAndThisThread() == false && static_cast<bool>(_internal->async)) {
        tDLogD("UdpReceiver::close() Async request.");
        _internal->async->newSendFunc([&](){
            Guard const MUTEX_GUARD_ASYNC(_mutex);
            _internal->closeAll();
        });
        return Err::E_ASYNCREQ;
    }

    Err code = Err::E_SUCCESS;
    if (loop.isAliveAndThisThread() == false && static_cast<bool>(_internal->async) == false) {
        tDLogW("UdpReceiver::close() Async is expired.");
        code = Err::E_WARNING;
    }

    tDLogD("UdpReceiver::close() Synced request.");
    _internal->closeAll();
    return code;
}

Err UdpReceiver::cancel()
{
    return Err::E_UNSUPOP;
}

Err UdpReceiver::write(char const * buffer, std::size_t size)
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
    assert(_interface != nullptr);
    _interface->onClientRead(_interface->get(id()), code, packet);
}

void UdpReceiver::backClose()
{
    assert(static_cast<bool>(_internal));
    _mutex.lock();
    _internal->closeAll();
    _mutex.unlock();

    assert(_interface != nullptr);
    _interface->onClientClose(_interface->get(id()));
}

// ---------------
// Event callback.
// ---------------

void UdpReceiver::onConnect(Err code)
{
    TBAG_INACCESSIBLE_BLOCK_ASSERT();
}

void UdpReceiver::onShutdown(Err code)
{
    TBAG_INACCESSIBLE_BLOCK_ASSERT();
}

void UdpReceiver::onWrite(Err code)
{
    TBAG_INACCESSIBLE_BLOCK_ASSERT();
}

void UdpReceiver::onRead(Err code, ReadPacket const & packet)
{
    TBAG_INACCESSIBLE_BLOCK_ASSERT();
}

void UdpReceiver::onClose()
{
    TBAG_INACCESSIBLE_BLOCK_ASSERT();
}

} // namespace udp
} // namespace network

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

