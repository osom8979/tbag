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
#include <libtbag/uvpp/Request.hpp>

#include <libtbag/uvpp/ex/SafetyWriteAsync.hpp>
#include <libtbag/uvpp/ex/TimeoutClose.hpp>
#include <libtbag/uvpp/ex/TimeoutShutdown.hpp>
#include <libtbag/uvpp/ex/WriteJob.hpp>

#include <libtbag/id/Id.hpp>

#include <memory>
#include <vector>
#include <string>
#include <chrono>
#include <mutex>

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

struct NetCommon
{
    using Id = id::Id;

    using uerr = uvpp::uerr;
    using binf = uvpp::binf;

    using Type   = NetType;
    using Size   = std::size_t;
    using String = std::string;

    using Loop  = uvpp::Loop;
    using Timer = uvpp::Timer;

    using SafetyAsync      = uvpp::ex::SafetyAsync;
    using SafetyWriteAsync = uvpp::ex::SafetyWriteAsync;

    using AsyncJob = SafetyWriteAsync::Job;
    using WriteJob = uvpp::ex::WriteJob;

    using ShutdownRequest = uvpp::ShutdownRequest;
    using ConnectRequest  = uvpp::ConnectRequest;
    using WriteRequest    = uvpp::WriteRequest;

    using Buffer = std::vector<char>;

    using seconds      = std::chrono::seconds;
    using milliseconds = std::chrono::milliseconds;

    using Mutex = std::mutex;
    using Guard = std::lock_guard<Mutex>;

    using TimeoutClose    = uvpp::ex::TimeoutClose;
    using TimeoutShutdown = uvpp::ex::TimeoutShutdown;

    using SharedClose = std::shared_ptr<TimeoutClose>;
    using   WeakClose =   std::weak_ptr<TimeoutClose>;

    using SharedShutdown = std::shared_ptr<TimeoutShutdown>;
    using   WeakShutdown =   std::weak_ptr<TimeoutShutdown>;

    using SharedAsync = std::shared_ptr<SafetyWriteAsync>;
    using   WeakAsync =   std::weak_ptr<SafetyWriteAsync>;
};

TBAG_API bool isIpv4(std::string const & ip);
TBAG_API bool isIpv6(std::string const & ip);

} // namespace details
} // namespace network

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_NETWORK_DETAILS_NETCOMMON_HPP__

