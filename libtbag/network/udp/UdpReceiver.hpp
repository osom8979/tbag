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

    using SharedClientBackend = std::shared_ptr<UdpNodeBackend>;
    using   WeakClientBackend =   std::weak_ptr<UdpNodeBackend>;

    using       SafetyAsync = uvpp::ex::SafetyAsync;
    using SharedSafetyAsync = std::shared_ptr<SafetyAsync>;
    using   WeakSafetyAsync =   std::weak_ptr<SafetyAsync>;

    using Id     = id::Id;
    using Buffer = std::vector<char>;
    using Mutex  = std::mutex;
    using Guard  = std::lock_guard<Mutex>;

public:
    struct Internal;
    friend struct Internal;

    using UniqueInternal = std::unique_ptr<Internal>;

private:
    ServerInterface * _interface;

private:
    UniqueInternal _internal;
    mutable Mutex  _mutex;

public:
    UdpReceiver(Loop & loop, ServerInterface * parent);
    UdpReceiver(Loop & loop, SharedSafetyAsync async, ServerInterface * parent);
    virtual ~UdpReceiver();

public:
    void setBindFlags(unsigned int flags) TBAG_NOEXCEPT;
    unsigned int getBindFlags() const TBAG_NOEXCEPT;

    WeakClientBackend getClient();
    WeakSafetyAsync getAsync();

public:
    virtual Id          id   () const override;
    virtual std::string dest () const override;
    virtual int         port () const override;
    virtual void *      udata() override;

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
    virtual void onConnect (Err code) override;
    virtual void onShutdown(Err code) override;
    virtual void onWrite   (Err code) override;
    virtual void onRead    (Err code, ReadPacket const & packet) override;
    virtual void onClose   () override;
};

} // namespace udp
} // namespace network

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_NETWORK_UDP_UDPRECEIVER_HPP__

