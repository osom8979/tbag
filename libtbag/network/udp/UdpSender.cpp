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

#include <cassert>
#include <cstring>
#include "UdpSender.hpp"

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

    UdpSender * parent;

    bool owner_async;

    SharedClientBackend client;
    SharedSafetyAsync   async;

    SocketAddress addr;
    struct {
        SendState      state;
        UdpSendRequest send_req;
        Buffer         buffer;
    } sender;

    Internal(UdpSender * parent) : parent(parent), owner_async(false)
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

    Err sendReal(char const * buffer, std::size_t size)
    {
        assert(static_cast<bool>(client));
        return client->send(sender.send_req, buffer, size, addr.getCommon());
    }

    void assignSendBuffer(char const * buffer, std::size_t size)
    {
        assert(buffer != nullptr);
        assert(size > 0);
        if (sender.buffer.size() < size) {
            sender.buffer.resize(size);
        }
        ::memcpy(sender.buffer.data(), buffer, size);
    }

    Err autoSend(char const * buffer, std::size_t size)
    {
        if (sender.state != SendState::SS_READY) {
            tDLogE("UdpSender::Internal::autoSend() Illegal state error: {}",
                   getSendStateName(sender.state));
            return Err::E_ILLSTATE;
        }

        assert(static_cast<bool>(client));
        assert(static_cast<bool>(async));

        Loop & loop = getLoop();
        assert(loop != nullptr);

        if (loop.isAliveAndThisThread() == false || static_cast<bool>(async)) {
            assignSendBuffer(buffer, size);
            auto job = async->newSendFunc(std::bind(&UdpSender::onAsyncSend, parent));
            if (static_cast<bool>(job) == false) {
                tDLogE("UdpSender::Internal::autoSend() New job error.");
                return Err::E_BADALLOC;
            }
            sender.state = SendState::SS_ASYNC;

        } else {
            Err const CODE = sendReal(buffer, size);
            if (CODE != Err::E_SUCCESS) {
                tDLogE("UdpSender::Internal::autoSend() Write {} error.", getErrName(CODE));
                return CODE;
            }
            sender.state = SendState::SS_SEND;
        }

        return Err::E_SUCCESS;
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

// -------------------------
// UdpSender implementation.
// -------------------------

UdpSender::UdpSender(Loop & loop, SharedSafetyAsync async, ServerInterface * interface)
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
    _internal->sender.state = SendState::SS_READY;
}

UdpSender::~UdpSender()
{
    // EMPTY.
}

void UdpSender::onAsyncSend()
{
    assert(static_cast<bool>(_internal));
    Guard const MUTEX_GUARD(_mutex);

    if (_internal->sender.state == SendState::SS_ASYNC_CANCEL) {
        tDLogN("UdpSender::onAsyncSend() Cancel async write.");
        _internal->sender.state = SendState::SS_READY;
        return;
    }

    if (_internal->sender.state != SendState::SS_ASYNC) {
        tDLogE("UdpSender::onAsyncSend() Error state: {}", getSendStateName(_internal->sender.state));
        _internal->sender.state = SendState::SS_READY;
        return;
    }

    assert(_internal->sender.state == SendState::SS_ASYNC);

    if (_internal->sender.buffer.empty()) {
        tDLogD("StreamClient::onAsyncWrite() Empty writer buffer.");
        _internal->sender.state = SendState::SS_READY;
        return;
    }

    Err const CODE = _internal->sendReal(_internal->sender.buffer.data(), _internal->sender.buffer.size());
    if (CODE == Err::E_SUCCESS) {
        _internal->sender.state = SendState::SS_SEND;
    } else {
        tDLogE("UdpSender::onAsyncSend() {} error.", getErrName(CODE));
        _internal->sender.state = SendState::SS_READY;
    }
}

char const * UdpSender::getSendStateName(SendState state) TBAG_NOEXCEPT
{
    switch (state) {
    case SendState::SS_READY:         return "READY";
    case SendState::SS_ASYNC:         return "ASYNC";
    case SendState::SS_ASYNC_CANCEL:  return "ASYNC_CANCEL";
    case SendState::SS_SEND:          return "SEND";
    case SendState::SS_END:           return "END";
    }

    TBAG_INACCESSIBLE_BLOCK_ASSERT();
    return "UNKNOWN";
}

UdpSender::SendState UdpSender::getSendState() const TBAG_NOEXCEPT
{
    assert(static_cast<bool>(_internal));
    Guard const MUTEX_GUARD(_mutex);
    return _internal->sender.state;
}

char const * UdpSender::getSendStateName() const TBAG_NOEXCEPT
{
    assert(static_cast<bool>(_internal));
    Guard const MUTEX_GUARD(_mutex);
    return getSendStateName(_internal->sender.state);
}

UdpSender::WeakClientBackend UdpSender::getClient()
{
    assert(static_cast<bool>(_internal));
    Guard const MUTEX_GUARD(_mutex);
    return WeakClientBackend(_internal->client);
}

UdpSender::WeakSafetyAsync UdpSender::getAsync()
{
    assert(static_cast<bool>(_internal));
    Guard const MUTEX_GUARD(_mutex);
    return WeakSafetyAsync(_internal->async);
}

UdpSender::Id UdpSender::id() const
{
    assert(static_cast<bool>(_internal));
    assert(static_cast<bool>(_internal->client));
    Guard const MUTEX_GUARD(_mutex);
    return _internal->client->id();
}

std::string UdpSender::dest() const
{
    assert(static_cast<bool>(_internal));
    Guard const MUTEX_GUARD(_mutex);
    return _internal->addr.getIpName();
}

int UdpSender::port() const
{
    assert(static_cast<bool>(_internal));
    Guard const MUTEX_GUARD(_mutex);
    return _internal->addr.getPortNumber();
}

void * UdpSender::udata()
{
    assert(static_cast<bool>(_internal));
    assert(static_cast<bool>(_internal->client));
    Guard const MUTEX_GUARD(_mutex);
    return _internal->client->getUserData();
}

Err UdpSender::init(char const * destination, int port)
{
    assert(static_cast<bool>(_internal));
    Guard const MUTEX_GUARD(_mutex);

    if (static_cast<bool>(_internal->client) == false) {
        return Err::E_EXPIRED;
    }

    if (_internal->addr.init(destination, port) != Err::E_SUCCESS) {
        tDLogE("UdpSender::init() Initialize fail.");
        return Err::E_UNKNOWN;
    }

    if (_internal->initHandles() == false) {
        tDLogE("UdpSender::init() Initialize fail (internal handles).");
        return Err::E_UNKNOWN;
    }
    return Err::E_SUCCESS;
}

Err UdpSender::start()
{
    return Err::E_UNSUPOP;
}

Err UdpSender::stop()
{
    return Err::E_UNSUPOP;
}

Err UdpSender::close()
{
    assert(static_cast<bool>(_internal));
    assert(static_cast<bool>(_internal->client));
    Guard const MUTEX_GUARD(_mutex);
    Loop & loop = _internal->getLoop();

    if (loop.isAliveAndThisThread() == false && static_cast<bool>(_internal->async)) {
        tDLogD("UdpSender::close() Async request.");
        _internal->async->newSendFunc([&](){
            Guard const MUTEX_GUARD_ASYNC(_mutex);
            _internal->closeAll();
        });
        return Err::E_ASYNCREQ;
    }

    Err code = Err::E_SUCCESS;
    if (loop.isAliveAndThisThread() == false && static_cast<bool>(_internal->async) == false) {
        tDLogW("UdpSender::close() Async is expired.");
        code = Err::E_WARNING;
    }

    tDLogD("UdpSender::close() Synced request.");
    _internal->closeAll();
    return code;
}

Err UdpSender::cancel()
{
    assert(static_cast<bool>(_internal));
    Guard const MUTEX_GUARD(_mutex);
    if (_internal->sender.state == SendState::SS_ASYNC) {
        _internal->sender.state = SendState::SS_ASYNC_CANCEL;
    }
    return Err::E_SUCCESS;
}

Err UdpSender::write(char const * buffer, std::size_t size)
{
    assert(static_cast<bool>(_internal));
    Guard const MUTEX_GUARD(_mutex);
    return _internal->autoSend(buffer, size);
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
    assert(_internal->sender.state == SendState::SS_SEND);
    _internal->sender.state = SendState::SS_READY;
    _mutex.unlock();

    assert(_interface != nullptr);
    _interface->onClientWrite(_interface->get(id()), code);
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
    _internal->sender.state = SendState::SS_END;
    _mutex.unlock();

    assert(_interface != nullptr);
    _interface->onClientClose(_interface->get(id()));
}

// ---------------
// Event callback.
// ---------------

void UdpSender::onConnect(Err code)
{
    TBAG_INACCESSIBLE_BLOCK_ASSERT();
}

void UdpSender::onShutdown(Err code)
{
    TBAG_INACCESSIBLE_BLOCK_ASSERT();
}

void UdpSender::onWrite(Err code)
{
    TBAG_INACCESSIBLE_BLOCK_ASSERT();
}

void UdpSender::onRead(Err code, ReadPacket const & packet)
{
    TBAG_INACCESSIBLE_BLOCK_ASSERT();
}

void UdpSender::onClose()
{
    TBAG_INACCESSIBLE_BLOCK_ASSERT();
}

} // namespace udp
} // namespace network

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

