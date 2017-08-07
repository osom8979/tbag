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

    Err sendReal(char const * buffer, std::size_t size)
    {
        assert(_parent != nullptr);
        UdpSender & p = *_parent;

        assert(static_cast<bool>(p._client));
        return p._client->send(p._sender.send_req, buffer, size, p._addr.getCommon());
    }

    void assignSendBuffer(char const * buffer, std::size_t size)
    {
        assert(_parent != nullptr);
        UdpSender & p = *_parent;

        assert(buffer != nullptr);
        assert(size > 0);
        if (p._sender.buffer.size() < size) {
            p._sender.buffer.resize(size);
        }
        ::memcpy(p._sender.buffer.data(), buffer, size);
    }

    Err autoSend(char const * buffer, std::size_t size)
    {
        assert(_parent != nullptr);
        UdpSender & p = *_parent;

        if (p._sender.state != SendState::SS_READY) {
            tDLogE("UdpSender::Internal::autoSend() Illegal state error: {}",
                   getSendStateName(p._sender.state));
            return Err::E_ILLSTATE;
        }

        assert(static_cast<bool>(p._client));
        assert(static_cast<bool>(p._async));

        Loop * loop = p._client->getLoop();
        assert(loop != nullptr);

        if (loop->isAliveAndThisThread() || static_cast<bool>(p._async) == false) {
            Err const CODE = sendReal(buffer, size);
            if (CODE != Err::E_SUCCESS) {
                tDLogE("UdpSender::Internal::autoSend() Write {} error.", getErrName(CODE));
                return CODE;
            }
            p._sender.state = SendState::SS_SEND;

        } else {
            assignSendBuffer(buffer, size);
            auto job = p._async->newSendFunc(std::bind(&UdpSender::onAsyncSend, _parent));
            if (static_cast<bool>(job) == false) {
                tDLogE("UdpSender::Internal::autoSend() New job error.");
                return Err::E_BADALLOC;
            }
            p._sender.state = SendState::SS_ASYNC;
        }

        return Err::E_SUCCESS;
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

// -------------------------
// UdpSender implementation.
// -------------------------

UdpSender::UdpSender(Loop & loop, SharedSafetyAsync async, ServerInterface * parent)
        : _parent(parent), _internal(new Internal(this))
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

    _sender.state = SendState::SS_READY;
}

UdpSender::~UdpSender()
{
    // EMPTY.
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

UdpSender::Id UdpSender::id() const
{
    assert(static_cast<bool>(_client));
    Guard const MUTEX_GUARD(_mutex);
    return _client->id();
}

std::string UdpSender::dest() const
{
    Guard const MUTEX_GUARD(_mutex);
    return _addr.getIpName();
}

int UdpSender::port() const
{
    Guard const MUTEX_GUARD(_mutex);
    return _addr.getPortNumber();
}

void * UdpSender::udata()
{
    assert(static_cast<bool>(_client));
    Guard const MUTEX_GUARD(_mutex);
    return _client->getUserData();
}

Err UdpSender::init(char const * destination, int port)
{
    assert(static_cast<bool>(_client));
    assert(static_cast<bool>(_internal));

    Guard const MUTEX_GUARD(_mutex);
    if (static_cast<bool>(_client) == false) {
        return Err::E_EXPIRED;
    }

    if (_addr.init(destination, port) != Err::E_SUCCESS) {
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
    assert(static_cast<bool>(_client));
    assert(static_cast<bool>(_internal));
    Loop * loop = _client->getLoop();
    assert(loop != nullptr);

    Guard const MUTEX_GUARD(_mutex);
    if (loop->isAliveAndThisThread() == false && static_cast<bool>(_async)) {
        tDLogD("UdpSender::close() Async request.");
        _async->newSendFunc([&](){
            Guard const MUTEX_GUARD_ASYNC(_mutex);
            _internal->closeAll();
        });
        return Err::E_ASYNCREQ;
    }

    Err code = Err::E_SUCCESS;
    if (loop->isAliveAndThisThread() == false && static_cast<bool>(_async) == false) {
        tDLogW("UdpSender::close() Async is expired.");
        code = Err::E_WARNING;
    }

    tDLogD("UdpSender::close() Synced request.");
    _internal->closeAll();
    return code;
}

Err UdpSender::cancel()
{
    Guard const MUTEX_GUARD(_mutex);
    if (_sender.state == SendState::SS_ASYNC) {
        _sender.state = SendState::SS_ASYNC_CANCEL;
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
    _mutex.lock();
    _sender.state = SendState::SS_READY;
    _mutex.unlock();

    assert(_parent != nullptr);
    _parent->onClientWrite(_parent->get(id()), code);
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
    _sender.state = SendState::SS_END;
    _mutex.unlock();

    assert(_parent != nullptr);
    _parent->onClientClose(_parent->get(id()));
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

void UdpSender::onAsyncSend()
{
    assert(static_cast<bool>(_internal));
    Guard const MUTEX_GUARD(_mutex);

    if (_sender.state == SendState::SS_ASYNC_CANCEL) {
        tDLogN("UdpSender::onAsyncSend() Cancel async write.");
        _sender.state = SendState::SS_READY;
        return;
    }

    if (_sender.state != SendState::SS_ASYNC) {
        tDLogE("UdpSender::onAsyncSend() Error state: {}", getSendStateName(_sender.state));
        _sender.state = SendState::SS_READY;
        return;
    }

    assert(_sender.state == SendState::SS_ASYNC);

    if (_sender.buffer.empty()) {
        tDLogD("StreamClient::onAsyncWrite() Empty writer buffer.");
        _sender.state = SendState::SS_READY;
        return;
    }

    Err const CODE = _internal->sendReal(_sender.buffer.data(), _sender.buffer.size());
    if (CODE == Err::E_SUCCESS) {
        _sender.state = SendState::SS_SEND;
    } else {
        tDLogE("UdpSender::onAsyncSend() {} error.", getErrName(CODE));
        _sender.state = SendState::SS_READY;
    }
}

} // namespace udp
} // namespace network

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

