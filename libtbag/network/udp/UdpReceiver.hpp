/**
 * @file   UdpReceiver.hpp
 * @brief  UdpReceiver class prototype.
 * @author zer0
 * @date   2017-06-17
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_NETWORK_UDP_UDPRECEIVER_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_NETWORK_UDP_UDPRECEIVER_HPP__

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
#include <libtbag/uvpp/Request.hpp>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace network {
namespace udp     {

/**
 * UdpReceiver class prototype.
 *
 * @author zer0
 * @date   2017-06-17
 */
class TBAG_API UdpReceiver : public details::ClientInterface
{
public:
    using ServerInterface = details::ServerInterface;
    using ReadPacket      = details::ReadPacket;

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
    using Id      = id::Id;
    using Buffer  = std::vector<char>;
    using Buffers = std::vector<Buffer>;

    using Mutex = std::mutex;
    using Guard = std::lock_guard<Mutex>;

public:
    struct Internal;
    friend struct Internal;

public:
    using UniqueInternal = std::unique_ptr<Internal>;

private:
    ServerInterface * _parent;
    bool _owner_async;

private:
    UniqueInternal      _internal;
    SharedClientBackend _client;
    SharedSafetyAsync   _async;

private:
    mutable Mutex _mutex;

public:
    UdpReceiver(Loop & loop, SharedSafetyAsync async, ServerInterface * parent);
    virtual ~UdpReceiver();

public:
    WeakClientBackend getClient() { Guard g(_mutex); return WeakClientBackend(_client); }
    WeakSafetyAsync   getAsync () { Guard g(_mutex); return WeakSafetyAsync(_async); }

public:
    virtual Id          id   () const override;
    virtual std::string dest () const override;
    virtual int         port () const override;
    virtual void *      udata() override;

public:
    virtual Err  init  (char const * destination, int port = 0, uint64_t millisec = 0) override;
    virtual Err  start () override;
    virtual Err  stop  () override;
    virtual void close () override;
    virtual void cancel() override;
    virtual Err  write (binf const * buffer, std::size_t size, uint64_t millisec = 0) override;
    virtual Err  write (char const * buffer, std::size_t size, uint64_t millisec = 0) override;

// Event backend.
public:
    virtual void backConnect(Err code) override;
    virtual void backShutdown(Err code) override;
    virtual void backWrite(Err code) override;
    virtual void backRead(Err code, ReadPacket const & packet) override;
    virtual void backClose() override;
};

} // namespace udp
} // namespace network

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_NETWORK_UDP_UDPRECEIVER_HPP__

