/**
 * @file   NetCommon.hpp
 * @brief  NetCommon class prototype.
 * @author zer0
 * @date   2017-05-03
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_NETWORK_DETAILS_NETCOMMON_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_NETWORK_DETAILS_NETCOMMON_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/predef.hpp>

#include <libtbag/uvpp/UvCommon.hpp>
#include <libtbag/uvpp/Loop.hpp>
#include <libtbag/uvpp/Tcp.hpp>
#include <libtbag/uvpp/Timer.hpp>
#include <libtbag/uvpp/ex/SafetyAsync.hpp>
#include <libtbag/uvpp/Request.hpp>

#include <memory>
#include <vector>
#include <string>

#include <string>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace network {
namespace details {

TBAG_CONSTEXPR char const * const ANY_IPV4 = "0.0.0.0";
TBAG_CONSTEXPR char const * const BROADCAST_SUBNET_IPV4 = "255.255.255.255";

TBAG_CONSTEXPR char const * const ANY_SUBNET_MASK_IPV4 = "0.0.0.0";

TBAG_CONSTEXPR char const * const LOOPBACK_NAME = "localhost";
TBAG_CONSTEXPR char const * const LOOPBACK_IPV4 = "127.0.0.1";
TBAG_CONSTEXPR char const * const LOOPBACK_IPV6 = "::1";

TBAG_CONSTEXPR int const OS_RANDOMLY_ASSIGNS_PORT = 0;

enum class NetType
{
    UNKNOWN,
    TCP,
    UDP,
    PIPE,
};

struct NetTypes
{
    using uerr = uvpp::uerr;
    using binf = uvpp::binf;

    using Type   = NetType;
    using Size   = std::size_t;
    using String = std::string;

    using Loop           = uvpp::Loop;
    using Timer          = uvpp::Timer;
    using SafetyAsync    = uvpp::ex::SafetyAsync;
    using SafetyAsyncJob = SafetyAsync::Job;

    using ConnectRequest = uvpp::ConnectRequest;
    using WriteRequest   = uvpp::WriteRequest;

    using Buffer = std::vector<char>;
};

struct NetInterface : public NetTypes
{
    // @formatter:off
    virtual Type getType() const { return Type::UNKNOWN; }

    virtual uerr init(String const & arg1, int arg2)
    { return uerr::UVPP_ENOSYS; }

    virtual uerr start() { return uerr::UVPP_ENOSYS; }
    virtual uerr  stop() { return uerr::UVPP_ENOSYS; }
    virtual uerr close() { return uerr::UVPP_ENOSYS; }
    // @formatter:on
};

struct ClientInterface : public NetInterface
{
    // @formatter:off
    virtual uerr  syncWrite(char const * buffer, Size * size) { return uerr::UVPP_ENOSYS; }
    virtual uerr asyncWrite(char const * buffer, Size * size) { return uerr::UVPP_ENOSYS; }
    virtual uerr   tryWrite(char const * buffer, Size * size) { return uerr::UVPP_ENOSYS; }

    virtual void onConnect(uerr code)                                 { /* EMPTY. */ }
    virtual binf onAlloc  (Size suggested_size)                       { return binf(); }
    virtual void onWrite  (uerr code)                                 { /* EMPTY. */ }
    virtual void onRead   (uerr code, char const * buffer, Size size) { /* EMPTY. */ }
    virtual void onClose  ()                                          { /* EMPTY. */ }
    virtual void onAsync  (int type, uerr code)                       { /* EMPTY. */ }
    // @formatter:on
};

struct ServerInterface : public NetInterface
{
    using NodeInter = details::ClientInterface;

    // @formatter:off
    virtual void onClose() { /* EMPTY. */ }

    virtual void onClientConnect(NodeInter * node, uerr code)                                 { /* EMPTY. */ }
    virtual binf onClientAlloc  (NodeInter * node, Size suggested_size)                       { return binf(); }
    virtual void onClientWrite  (NodeInter * node, uerr code)                                 { /* EMPTY. */ }
    virtual void onClientRead   (NodeInter * node, uerr code, char const * buffer, Size size) { /* EMPTY. */ }
    virtual void onClientClose  (NodeInter * node)                                            { /* EMPTY. */ }
    virtual void onClientAsync  (NodeInter * node, int type, uerr code)                       { /* EMPTY. */ }
    // @formatter:on
};

TBAG_API bool isIpv4(std::string const & ip);
TBAG_API bool isIpv6(std::string const & ip);

} // namespace details
} // namespace network

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_NETWORK_DETAILS_NETCOMMON_HPP__

