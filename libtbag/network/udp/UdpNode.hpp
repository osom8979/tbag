/**
 * @file   UdpNode.hpp
 * @brief  UdpNode class prototype.
 * @author zer0
 * @date   2017-06-15
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

#include <libtbag/network/details/NetCommon.hpp>
#include <libtbag/network/details/FunctionalNet.hpp>
#include <libtbag/network/udp/UdpNodeBackend.hpp>
#include <libtbag/uvpp/ex/SafetyAsync.hpp>
#include <libtbag/uvpp/func/FunctionalTimer.hpp>
#include <libtbag/uvpp/Request.hpp>

#include <string>
#include <vector>
#include <memory>
#include <mutex>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace network {
namespace udp     {

/**
 * UdpNode class prototype.
 *
 * @author zer0
 * @date   2017-06-15
 */
class TBAG_API UdpNode : public details::ClientInterface
{
public:
    using ReadPacket = details::ReadPacket;
    using ServerInterface = details::ServerInterface;

    using Loop = uvpp::Loop;
    using Udp  = uvpp::Udp;
    using binf = uvpp::binf;

    using UdpSendRequest = uvpp::UdpSendRequest;

public:
    using SharedClientBackend = std::shared_ptr<UdpNodeBackend>;
    using   WeakClientBackend =   std::weak_ptr<UdpNodeBackend>;

public:
    using       SafetyAsync = uvpp::ex::SafetyAsync;
    using SharedSafetyAsync = std::shared_ptr<SafetyAsync>;
    using   WeakSafetyAsync = std::shared_ptr<SafetyAsync>;

public:
    using       FuncTimer = uvpp::func::FuncTimer;
    using SharedFuncTimer = std::shared_ptr<FuncTimer>;
    using   WeakFuncTimer = std::shared_ptr<FuncTimer>;

public:
    using Id      = id::Id;
    using Buffer  = std::vector<char>;
    using Buffers = std::vector<Buffer>;

    using Mutex = std::mutex;
    using Guard = std::lock_guard<Mutex>;

public:
    enum class NodeType
    {
        NT_RECV,
        NT_SEND,
    };

    enum class SendStatus
    {
        SS_NOT_READY,
        SS_READY,           ///< Next: call send.
        SS_ASYNC,           ///< Next: onTimeout or onAsync.
        SS_ASYNC_CANCEL,    ///< Next: onAsync.
        SS_SEND,            ///< Next: onTimeout or onWrite.
        SS_END,
    };

public:
    struct Internal;
    friend struct Internal;

public:
    using UniqueInternal = std::unique_ptr<Internal>;

private:
    NodeType const NODE_TYPE;
    bool _owner_async;

private:
    UniqueInternal       _internal;
    SharedClientBackend  _client;
    SharedSafetyAsync    _async;
    SharedFuncTimer      _close_timer;

private:
    union {
        struct sockaddr_in  ipv4;
        struct sockaddr_in6 ipv6;
    } _addr;

private:
    mutable Mutex _mutex;

    struct {
        SendStatus     status;
        UdpSendRequest send_req;
        Buffers        buffers;
    } _sender;

public:
    UdpNode(Loop & loop, NodeType type, SharedSafetyAsync async, ServerInterface * parent);
    virtual ~UdpNode();

public:
    WeakClientBackend getClient() { Guard g(_mutex); return WeakClientBackend(_client); }
    WeakSafetyAsync   getAsync () { Guard g(_mutex); return WeakSafetyAsync(_async); }

private:
    static char const * getSendStatusName(SendStatus status) TBAG_NOEXCEPT;

public:
    inline SendStatus getSendStatus() const TBAG_NOEXCEPT
    { return _sender.status; }
    inline char const * getSendStatusName() const TBAG_NOEXCEPT
    { return getSendStatusName(_sender.status); }

//private:
//    void onAsyncSend();
//
//public:
//    virtual Id          id   () const override;
//    virtual std::string dest () const override;
//    virtual int         port () const override;
//    virtual void *      udata() override;
//
//public:
//    virtual Err  init  (char const * destination, int port = 0, uint64_t millisec = 0) override;
//    virtual Err  start () override;
//    virtual Err  stop  () override;
//    virtual void close () override;
//    virtual void cancel() override;
//    virtual Err  write (binf const * buffer, std::size_t size, uint64_t millisec = 0) override;
//    virtual Err  write (char const * buffer, std::size_t size, uint64_t millisec = 0) override;
//
//// Event backend.
//public:
//    virtual void backConnect(Err code) override;
//    virtual void backShutdown(Err code) override;
//    virtual void backWrite(Err code) override;
//    virtual void backRead(Err code, ReadPacket const & packet) override;
//    virtual void backClose() override;

public:
    template <typename Predicated>
    void updateUserData(Predicated predicated)
    {
        Guard guard(_mutex);
        predicated(_client->getUserData());
    }
};

using FunctionalUdpNode = details::FunctionalClient<UdpNode>;

} // namespace udp
} // namespace network

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_NETWORK_UDP_UDPNODE_HPP__

