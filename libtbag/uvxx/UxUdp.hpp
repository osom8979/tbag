/**
 * @file   UxUdp.hpp
 * @brief  UxUdp class prototype.
 * @author zer0
 * @date   2018-12-07
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_UVXX_UXUDP_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_UVXX_UXUDP_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/predef.hpp>
#include <libtbag/uvxx/UxHandle.hpp>
#include <libtbag/uvpp/func/FunctionalUdp.hpp>
#include <libtbag/uvpp/Request.hpp>

#include <memory>
#include <string>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace uvxx {

// Forward declaration.
class UxLoop;

/**
 * UxUdp class prototype.
 *
 * @author zer0
 * @date   2018-12-07
 */
class TBAG_API UxUdp : public libtbag::uvxx::UxHandle
{
public:
    using FuncUdp   = libtbag::uvpp::func::FuncUdp;
    using WeakUdp   = std::weak_ptr<FuncUdp>;
    using SharedUdp = std::shared_ptr<FuncUdp>;

public:
    using UdpSendRequest = libtbag::uvpp::UdpSendRequest;
    using Membership     = FuncUdp::Membership;

public:
    using usock = libtbag::uvpp::usock;
    using binf  = libtbag::uvpp::binf;

public:
    // clang-format off
    TBAG_CONSTEXPR static unsigned int const FLAG_IPV6ONLY  = libtbag::uvpp::UDP_FLAG_IPV6ONLY ;
    TBAG_CONSTEXPR static unsigned int const FLAG_PARTIAL   = libtbag::uvpp::UDP_FLAG_PARTIAL  ;
    TBAG_CONSTEXPR static unsigned int const FLAG_REUSEADDR = libtbag::uvpp::UDP_FLAG_REUSEADDR;
    // clang-format on

public:
    UxUdp();
    UxUdp(UxLoop & loop);
    UxUdp(UxUdp const & obj) TBAG_NOEXCEPT;
    UxUdp(UxUdp && obj) TBAG_NOEXCEPT;
    ~UxUdp();

public:
    UxUdp & operator =(UxUdp const & obj) TBAG_NOEXCEPT;
    UxUdp & operator =(UxUdp && obj) TBAG_NOEXCEPT;

public:
    void copy(UxUdp const & obj) TBAG_NOEXCEPT;
    void swap(UxUdp & obj) TBAG_NOEXCEPT;

public:
    inline friend void swap(UxUdp & lh, UxUdp & rh) TBAG_NOEXCEPT
    { lh.swap(rh); }

public:
    SharedUdp lock() const TBAG_NOEXCEPT_SP_OP(_handle.lock())
    { return std::static_pointer_cast<FuncUdp>(_handle.lock()); }

public:
    // clang-format off
    void setOnClose(FuncUdp::OnClose const & cb) { lock()->close_cb = cb; }
    void setOnWalk (FuncUdp::OnWalk  const & cb) { lock()->walk_cb  = cb; }
    void setOnSend (FuncUdp::OnSend  const & cb) { lock()->send_cb  = cb; }
    void setOnAlloc(FuncUdp::OnAlloc const & cb) { lock()->alloc_cb = cb; }
    void setOnRecv (FuncUdp::OnRecv  const & cb) { lock()->recv_cb  = cb; }
    // clang-format on

public:
    Err init(UxLoop & loop);

public:
    std::size_t getSendQueueSize() const;
    std::size_t getSendQueueCount() const;

public:
    Err open(usock sock);
    Err bind(sockaddr const * addr, unsigned int flags = 0);

public:
    // clang-format off
    inline Err bind(sockaddr_in  const * addr, unsigned int flags = 0)
    { return bind((struct sockaddr const *)addr, flags); }
    inline Err bind(sockaddr_in6 const * addr, unsigned int flags = 0)
    { return bind((struct sockaddr const *)addr, flags); }
    // clang-format on

public:
    Err getSockName(sockaddr * name, int * namelen);
    std::string getSockIp();
    int getSockPort();

public:
    Err setMembership(char const * multicast_addr, char const * interface_addr, Membership membership);
    Err setMulticastLoop(bool on = true);
    Err setMulticastTtl(int ttl);
    Err setMulticastInterface(char const * interface_addr);
    Err setBroadcast(bool on = true);
    Err setTtl(int ttl);

public:
    Err send(UdpSendRequest & request, binf const * infos, std::size_t infos_size, sockaddr const * addr);
    Err send(UdpSendRequest & request, char const * buffer, std::size_t size, sockaddr const * addr);

public:
    // clang-format off
    inline Err send(UdpSendRequest & request, binf const * infos, std::size_t infos_size, sockaddr_in const * addr)
    { return send(request, infos, infos_size, (struct sockaddr const *)addr); }
    inline Err send(UdpSendRequest & request, char const * buffer, std::size_t size, sockaddr_in const * addr)
    { return send(request, buffer, size, (struct sockaddr const *)addr); }
    inline Err send(UdpSendRequest & request, binf const * infos, std::size_t infos_size, sockaddr_in6 const * addr)
    { return send(request, infos, infos_size, (struct sockaddr const *)addr); }
    inline Err send(UdpSendRequest & request, char const * buffer, std::size_t size, sockaddr_in6 const * addr)
    { return send(request, buffer, size, (struct sockaddr const *)addr); }
    // clang-format on

public:
    std::size_t trySend(binf * infos, std::size_t infos_size, sockaddr const * addr, Err * result = nullptr);
    std::size_t trySend(char const * buffer, std::size_t size, sockaddr const * addr, Err * result = nullptr);

public:
    // clang-format off
    inline std::size_t trySend(binf * infos, std::size_t infos_size, sockaddr_in const * addr, Err * result = nullptr)
    { return trySend(infos, infos_size, (struct sockaddr const *)addr, result); }
    inline std::size_t trySend(char const * buffer, std::size_t size, sockaddr_in const * addr, Err * result = nullptr)
    { return trySend(buffer, size, (struct sockaddr const *)addr, result); }
    inline std::size_t trySend(binf * infos, std::size_t infos_size, sockaddr_in6 const * addr, Err * result = nullptr)
    { return trySend(infos, infos_size, (struct sockaddr const *)addr, result); }
    inline std::size_t trySend(char const * buffer, std::size_t size, sockaddr_in6 const * addr, Err * result = nullptr)
    { return trySend(buffer, size, (struct sockaddr const *)addr, result); }
    // clang-format on

public:
    Err startRecv();
    Err stopRecv();
};

} // namespace uvxx

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_UVXX_UXUDP_HPP__

