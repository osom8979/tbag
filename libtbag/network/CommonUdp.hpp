/**
 * @file   CommonUdp.hpp
 * @brief  CommonUdp class prototype.
 * @author zer0
 * @date   2017-01-12
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_NETWORK_COMMONUDP_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_NETWORK_COMMONUDP_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/predef.hpp>
#include <libtbag/Noncopyable.hpp>

#include <libtbag/debug/ErrorCode.hpp>
#include <libtbag/uv/ex/CallableUdp.hpp>
#include <libtbag/uv/ex/RequestQueue.hpp>

#include <string>
#include <vector>
#include <memory>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace network {

// Forward declaration.
class Loop;

/**
 * CommonUdp class prototype.
 *
 * @author zer0
 * @date   2017-01-12
 */
class TBAG_API CommonUdp : public uv::ex::CallableUdp::Callback
{
public:
    using CallableUdp = uv::ex::CallableUdp;
    using Callback    = CallableUdp::Callback;
    using SharedUdp   = std::shared_ptr<CallableUdp>;

public:
    using Parent = Callback;

public:
    using binf = uv::binf;
    using Loop = uv::Loop;

    using UdpSendRequest = uv::UdpSendRequest;
    using UdpSendQueue   = uv::ex::UdpSendQueue;

    using Buffer = std::vector<char>;

protected:
    SharedUdp _udp;

protected:
    Buffer _read_buffer;
    UdpSendQueue _senders;

public:
    CommonUdp();
    CommonUdp(SharedUdp udp);
    CommonUdp(CallableUdp * udp);
    CommonUdp(uv::Loop & loop);
    virtual ~CommonUdp();

public:
    // @formatter:off
    inline SharedUdp       & atUdp()       TBAG_NOEXCEPT { return _udp; }
    inline SharedUdp const & atUdp() const TBAG_NOEXCEPT { return _udp; }
    inline void setUdp(SharedUdp     udp) { _udp = udp;      }
    inline void setUdp(CallableUdp * udp) { _udp.reset(udp); }
    inline Buffer       & atReadBuffer()       TBAG_NOEXCEPT { return _read_buffer; }
    inline Buffer const & atReadBuffer() const TBAG_NOEXCEPT { return _read_buffer; }
    // @formatter:on

public:
    UdpSendRequest * obtainUdpSendRequest();
    void releaseUdpSendRequest(UdpSendRequest * request);

public:
    // @formatter:off
    inline void close    () { return _udp->close    (); }
    inline bool startRead() { return _udp->startRecv(); }
    inline bool stopRead () { return _udp->stopRecv (); }

    inline void setUserData(void * data) TBAG_NOEXCEPT { _udp->setUserData(data); }

    inline void       * getUserData()       TBAG_NOEXCEPT { return _udp->getUserData(); }
    inline void const * getUserData() const TBAG_NOEXCEPT { return _udp->getUserData(); }
    // @formatter:on

public:
    UdpSendRequest * asyncWrite(binf * infos, std::size_t infos_size, sockaddr const * addr);
    UdpSendRequest * asyncWrite(char const * buffer, std::size_t size, sockaddr const * addr);

    std::size_t tryWrite(binf * infos, std::size_t infos_size, sockaddr const * addr, Err * result = nullptr);
    std::size_t tryWrite(char const * buffer, std::size_t size, sockaddr const * addr, Err * result = nullptr);
};

} // namespace network

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_NETWORK_COMMONUDP_HPP__

