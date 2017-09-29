/**
 * @file   UdpNode.hpp
 * @brief  UdpNode class prototype.
 * @author zer0
 * @date   2017-08-12
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_NETWORK_UDP_UDPNODE_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_NETWORK_UDP_UDPNODE_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/predef.hpp>
#include <libtbag/Err.hpp>
#include <libtbag/id/Id.hpp>

#include <libtbag/util/BufferInfo.hpp>
#include <libtbag/network/details/NetCommon.hpp>
#include <libtbag/network/details/FunctionalNet.hpp>
#include <libtbag/network/udp/UdpNodeBackend.hpp>
#include <libtbag/uvpp/ex/SafetyAsync.hpp>
#include <libtbag/uvpp/Udp.hpp>

#include <vector>
#include <mutex>
#include <memory>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace network {
namespace udp     {

/**
 * UdpNode class prototype.
 *
 * @author zer0
 * @date   2017-08-12
 */
class TBAG_API UdpNode : public details::ClientInterface
{
public:
    using ReadPacket = details::ReadPacket;

    using Loop = uvpp::Loop;
    using Udp  = uvpp::Udp;
    using binf = uvpp::binf;

    using SharedClientBackend = std::shared_ptr<UdpNodeBackend>;
    using   WeakClientBackend =   std::weak_ptr<UdpNodeBackend>;

    using       SafetyAsync = uvpp::ex::SafetyAsync;
    using SharedSafetyAsync = std::shared_ptr<SafetyAsync>;
    using   WeakSafetyAsync =   std::weak_ptr<SafetyAsync>;

    using Id     = id::Id;
    using Buffer = util::Buffer;
    using Mutex  = std::mutex;
    using Guard  = std::lock_guard<Mutex>;

public:
    enum class SendState
    {
        SS_READY,           ///< Next: call send.
        SS_ASYNC,           ///< Next: onTimeout or onAsync.
        SS_ASYNC_CANCEL,    ///< Next: onAsync.
        SS_SEND,            ///< Next: onTimeout or onWrite.
        SS_END,
    };

public:
    TBAG_CONSTEXPR static unsigned int const UDP_NODE_FLAG_NOTING   = 0x00;
    TBAG_CONSTEXPR static unsigned int const UDP_NODE_FLAG_USE_BIND = 0x01;

public:
    struct Internal;
    friend struct Internal;

    using UniqueInternal = std::unique_ptr<Internal>;

public:
    mutable Mutex _mutex;
    UniqueInternal _internal;

private:
    unsigned int _flags;

public:
    struct send_flags { /* EMPTY. */ };
    struct recv_flags { /* EMPTY. */ };

public:
    UdpNode(Loop & loop, unsigned int flags = UDP_NODE_FLAG_NOTING);
    UdpNode(Loop & loop, SharedSafetyAsync async, send_flags const & send);
    UdpNode(Loop & loop, SharedSafetyAsync async, recv_flags const & recv);
    UdpNode(Loop & loop, SharedSafetyAsync async, unsigned int flags = UDP_NODE_FLAG_NOTING);
    virtual ~UdpNode();

private:
    void onAsyncSend();

private:
    static char const * getSendStateName(SendState state) TBAG_NOEXCEPT;

public:
    SendState getSendState() const TBAG_NOEXCEPT;
    char const * getSendStateName() const TBAG_NOEXCEPT;

    inline void setFlags(unsigned int flags) TBAG_NOEXCEPT { _flags = flags; }
    inline unsigned int getFlags() const TBAG_NOEXCEPT { return _flags; }

    void setBindFlags(unsigned int flags) TBAG_NOEXCEPT;
    unsigned int getBindFlags() const TBAG_NOEXCEPT;

    Err setDestination(char const * destination, int port);
    Err setBroadcast(bool on = true);

public:
    WeakClientBackend  getClient();
    WeakSafetyAsync    getAsync();

public:
    virtual Id          id   () const override;
    virtual std::string dest () const override;
    virtual int         port () const override;

    virtual void * udata(void * data = nullptr) override;

public:
    virtual Err init  (char const * destination, int port = 0) override;
    virtual Err start () override;
    virtual Err stop  () override;
    virtual Err close () override;
    virtual Err cancel() override;
    virtual Err write (char const * buffer, std::size_t size) override;

// Event backend.
public:
    virtual void backConnect (Err code) override;
    virtual void backShutdown(Err code) override;
    virtual void backWrite   (Err code) override;
    virtual void backRead    (Err code, ReadPacket const & packet) override;
    virtual void backClose   () override;

// Event callback.
private:
    virtual void onConnect (Err code) override { /* EMPTY. */ }
    virtual void onShutdown(Err code) override { /* EMPTY. */ }
    virtual void onWrite   (Err code) override { /* EMPTY. */ }
    virtual void onRead    (Err code, ReadPacket const & packet) override { /* EMPTY. */ }
    virtual void onClose   () override { /* EMPTY. */ }
};

using FuncUdpNode = details::FunctionalClient<UdpNode>;

} // namespace udp
} // namespace network

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_NETWORK_UDP_UDPNODE_HPP__

