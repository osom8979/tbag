/**
 * @file   UdpNode.cpp
 * @brief  UdpNode class implementation.
 * @author zer0
 * @date   2017-08-12
 */

#include <libtbag/network/udp/UdpNode.hpp>
#include <libtbag/Noncopyable.hpp>
#include <libtbag/debug/Assert.hpp>
#include <libtbag/log/Log.hpp>
#include <libtbag/bitwise/BitFlags.hpp>

#include <libtbag/network/SocketAddress.hpp>
#include <libtbag/uvpp/Request.hpp>

#include <cassert>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace network {
namespace udp     {

/**
 * UdpNode internal helper class.
 *
 * @author zer0
 * @date   2017-06-17
 * @date   2017-08-12 (Rename: UdpSender::Internal -> UdpNode::Internal)
 *
 * @warning
 *  Don't mutex guard in this class. (However, callbacks are an exception)
 */
struct UdpNode::Internal : private Noncopyable
{
    using SocketAddress  = network::SocketAddress;
    using UdpSendRequest = uvpp::UdpSendRequest;

    struct AsyncSend : public SafetyAsync::JobInterface
    {
        UdpNode * parent;

        AsyncSend(UdpNode * p) : parent(p)
        { /* EMPTY. */ }
        virtual ~AsyncSend()
        { /* EMPTY. */ }

        virtual void run() override
        {
            assert(parent != nullptr);
            parent->onAsyncSend();
        }
    };

    UdpNode * parent;

    bool owner_async;
    unsigned int bind_flags;

    SharedClientBackend client;
    SharedSafetyAsync   async;

    struct {
        SocketAddress  addr;
        SendState      state;
        UdpSendRequest send_req;
        Buffer         buffer;
    } sender;

    Internal(UdpNode * p) : parent(p), owner_async(false), bind_flags(0)
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
        return client->send(sender.send_req, buffer, size, sender.addr.getCommon());
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
            tDLogE("UdpNode::Internal::autoSend() Illegal state error: {}",
                   getSendStateName(sender.state));
            return Err::E_ILLSTATE;
        }

        assert(static_cast<bool>(client));
        assert(static_cast<bool>(async));

        Loop & loop = getLoop();
        Err result_code = Err::E_UNKNOWN;

        if (loop.isAliveAndThisThread() == false && static_cast<bool>(async)) {
            assignSendBuffer(buffer, size);
            auto job = async->newSendJob<AsyncSend>(parent);
            if (static_cast<bool>(job) == false) {
                tDLogE("UdpNode::Internal::autoSend() New job error.");
                return Err::E_BADALLOC;
            }
            sender.state = SendState::SS_ASYNC;
            result_code = Err::E_ASYNCREQ;

        } else {
            Err const CODE = sendReal(buffer, size);
            if (CODE != Err::E_SUCCESS) {
                tDLogE("UdpNode::Internal::autoSend() Write {} error.", getErrName(CODE));
                return CODE;
            }

            sender.state = SendState::SS_SEND;
            if (loop.isAliveAndThisThread() == false && static_cast<bool>(async) == false) {
                tDLogW("UdpNode::Internal::autoSend() Async is expired.");
                result_code = Err::E_WARNING;
            } else {
                result_code = Err::E_SUCCESS;
            }
        }

        return result_code;
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

// -----------------------
// UdpNode implementation.
// -----------------------

UdpNode::UdpNode(Loop & loop, unsigned int flags) : UdpNode(loop, SharedSafetyAsync(), flags)
{
    // EMPTY.
}

UdpNode::UdpNode(Loop & loop, SharedSafetyAsync async, send_flags const & UNUSED_PARAM(send))
        : UdpNode(loop, SharedSafetyAsync(), UDP_NODE_FLAG_NOTING)
{
    // EMPTY.
}

UdpNode::UdpNode(Loop & loop, SharedSafetyAsync async, recv_flags const & UNUSED_PARAM(recv))
        : UdpNode(loop, SharedSafetyAsync(), UDP_NODE_FLAG_USE_BIND)
{
    // EMPTY.
}

UdpNode::UdpNode(Loop & loop, SharedSafetyAsync async, unsigned int flags) : _internal(new Internal(this)), _flags(flags)
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

UdpNode::~UdpNode()
{
    // EMPTY.
}

void UdpNode::onAsyncSend()
{
    assert(static_cast<bool>(_internal));
    Guard const MUTEX_GUARD(_mutex);

    if (_internal->sender.state == SendState::SS_ASYNC_CANCEL) {
        tDLogN("UdpNode::onAsyncSend() Cancel async write.");
        _internal->sender.state = SendState::SS_READY;
        return;
    }

    if (_internal->sender.state != SendState::SS_ASYNC) {
        tDLogE("UdpNode::onAsyncSend() Error state: {}", getSendStateName(_internal->sender.state));
        _internal->sender.state = SendState::SS_READY;
        return;
    }

    assert(_internal->sender.state == SendState::SS_ASYNC);

    if (_internal->sender.buffer.empty()) {
        tDLogD("UdpNode::onAsyncSend() Empty writer buffer.");
        _internal->sender.state = SendState::SS_READY;
        return;
    }

    Err const CODE = _internal->sendReal(_internal->sender.buffer.data(), _internal->sender.buffer.size());
    if (CODE == Err::E_SUCCESS) {
        _internal->sender.state = SendState::SS_SEND;
    } else {
        tDLogE("UdpNode::onAsyncSend() {} error.", getErrName(CODE));
        _internal->sender.state = SendState::SS_READY;
    }
}

char const * UdpNode::getSendStateName(SendState state) TBAG_NOEXCEPT
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

UdpNode::SendState UdpNode::getSendState() const TBAG_NOEXCEPT
{
    assert(static_cast<bool>(_internal));
    Guard const MUTEX_GUARD(_mutex);
    return _internal->sender.state;
}

char const * UdpNode::getSendStateName() const TBAG_NOEXCEPT
{
    assert(static_cast<bool>(_internal));
    Guard const MUTEX_GUARD(_mutex);
    return getSendStateName(_internal->sender.state);
}

void UdpNode::setBindFlags(unsigned int flags) TBAG_NOEXCEPT
{
    assert(static_cast<bool>(_internal));
    Guard const MUTEX_GUARD(_mutex);
    _internal->bind_flags = flags;
}

unsigned int UdpNode::getBindFlags() const TBAG_NOEXCEPT
{
    assert(static_cast<bool>(_internal));
    Guard const MUTEX_GUARD(_mutex);
    return _internal->bind_flags;
}

Err UdpNode::setDestination(char const * destination, int port)
{
    assert(static_cast<bool>(_internal));
    Guard const MUTEX_GUARD(_mutex);
    return _internal->sender.addr.init(destination, port);
}

Err UdpNode::setBroadcast(bool on)
{
    assert(static_cast<bool>(_internal));
    assert(static_cast<bool>(_internal->client));
    Guard const MUTEX_GUARD(_mutex);
    return _internal->client->setBroadcast(on);
}

UdpNode::WeakClientBackend UdpNode::getClient()
{
    assert(static_cast<bool>(_internal));
    Guard const MUTEX_GUARD(_mutex);
    return WeakClientBackend(_internal->client);
}

UdpNode::WeakSafetyAsync UdpNode::getAsync()
{
    assert(static_cast<bool>(_internal));
    Guard const MUTEX_GUARD(_mutex);
    return WeakSafetyAsync(_internal->async);
}

UdpNode::Id UdpNode::id() const
{
    assert(static_cast<bool>(_internal));
    assert(static_cast<bool>(_internal->client));
    Guard const MUTEX_GUARD(_mutex);
    return _internal->client->id();
}

std::string UdpNode::dest() const
{
    assert(static_cast<bool>(_internal));
    Guard const MUTEX_GUARD(_mutex);
    return _internal->client->getSockIp();
}

int UdpNode::port() const
{
    assert(static_cast<bool>(_internal));
    Guard const MUTEX_GUARD(_mutex);
    return _internal->client->getSockPort();
}

void * UdpNode::udata()
{
    assert(static_cast<bool>(_internal));
    assert(static_cast<bool>(_internal->client));
    Guard const MUTEX_GUARD(_mutex);
    return _internal->client->getUserData();
}

Err UdpNode::init(char const * destination, int port)
{
    assert(static_cast<bool>(_internal));
    assert(static_cast<bool>(_internal->client));
    Guard const MUTEX_GUARD(_mutex);

    if (_internal->client->isInit() == false) {
        tDLogE("UdpNode::init() Udp is not initialized.");
        return Err::E_EINIT;
    }

    if (bitwise::checkFlag(_flags, UDP_NODE_FLAG_USE_BIND)) {
        SocketAddress addr;
        Err const ADDRESS_INIT_CODE = addr.init(destination, port);
        if (ADDRESS_INIT_CODE != Err::E_SUCCESS) {
            tDLogE("UdpNode::init() Address initialize {} error.", getErrName(ADDRESS_INIT_CODE));
            return ADDRESS_INIT_CODE;
        }

        Err const BIND_CODE = _internal->client->bind(addr.getCommon(), _internal->bind_flags);
        if (BIND_CODE != Err::E_SUCCESS) {
            tDLogE("UdpNode::init() Udp bind {} error.", getErrName(BIND_CODE));
            return BIND_CODE;
        }
    }

    if (_internal->initHandles() == false) {
        tDLogE("UdpNode::init() Initialize fail (internal handles).");
        return Err::E_UNKNOWN;
    }
    return Err::E_SUCCESS;
}

Err UdpNode::start()
{
    assert(static_cast<bool>(_internal));
    assert(static_cast<bool>(_internal->client));
    Guard const MUTEX_GUARD(_mutex);

    Err const CODE = _internal->client->startRecv();
    if (CODE != Err::E_SUCCESS) {
        tDLogE("UdpNode::start() {} error", getErrName(CODE));
    }
    return CODE;
}

Err UdpNode::stop()
{
    assert(static_cast<bool>(_internal));
    assert(static_cast<bool>(_internal->client));
    Guard const MUTEX_GUARD(_mutex);

    Err const CODE = _internal->client->stopRecv();
    if (CODE != Err::E_SUCCESS) {
        tDLogE("UdpNode::stop() {} error", getErrName(CODE));
    }
    return CODE;
}

Err UdpNode::close()
{
    assert(static_cast<bool>(_internal));
    assert(static_cast<bool>(_internal->client));
    Guard const MUTEX_GUARD(_mutex);
    Loop & loop = _internal->getLoop();

    if (loop.isAliveAndThisThread() == false && static_cast<bool>(_internal->async)) {
        tDLogD("UdpNode::close() Async request.");
        _internal->async->newSendFunc([&](){
            Guard const MUTEX_GUARD_ASYNC(_mutex);
            _internal->closeAll();
        });
        return Err::E_ASYNCREQ;
    }

    Err code = Err::E_SUCCESS;
    if (loop.isAliveAndThisThread() == false && static_cast<bool>(_internal->async) == false) {
        tDLogW("UdpNode::close() Async is expired.");
        code = Err::E_WARNING;
    }

    tDLogD("UdpNode::close() Synced request.");
    _internal->closeAll();
    return code;
}

Err UdpNode::cancel()
{
    assert(static_cast<bool>(_internal));
    Guard const MUTEX_GUARD(_mutex);
    if (_internal->sender.state == SendState::SS_ASYNC) {
        _internal->sender.state = SendState::SS_ASYNC_CANCEL;
    }
    return Err::E_SUCCESS;
}

Err UdpNode::write(char const * buffer, std::size_t size)
{
    assert(static_cast<bool>(_internal));
    Guard const MUTEX_GUARD(_mutex);
    return _internal->autoSend(buffer, size);
}

// --------------
// Event backend.
// --------------

void UdpNode::backConnect(Err code)
{
    TBAG_INACCESSIBLE_BLOCK_ASSERT();
}

void UdpNode::backShutdown(Err code)
{
    TBAG_INACCESSIBLE_BLOCK_ASSERT();
}

void UdpNode::backWrite(Err code)
{
    assert(static_cast<bool>(_internal));
    _mutex.lock();
    assert(_internal->sender.state == SendState::SS_SEND);
    _internal->sender.state = SendState::SS_READY;
    _mutex.unlock();

    onWrite(code);
}

void UdpNode::backRead(Err code, ReadPacket const & packet)
{
    assert(static_cast<bool>(_internal));
    onRead(code, packet);
}

void UdpNode::backClose()
{
    assert(static_cast<bool>(_internal));
    _mutex.lock();
    _internal->closeAll();
    _internal->sender.state = SendState::SS_END;
    _mutex.unlock();

    onClose();
}

} // namespace udp
} // namespace network

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

