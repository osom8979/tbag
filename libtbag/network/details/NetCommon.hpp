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

#include <libtbag/uvpp/Tcp.hpp>
#include <libtbag/uvpp/Pipe.hpp>

#include <type_traits>

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

enum class StreamType : int
{
    UNKNOWN,
    TCP,
    PIPE,
};

template <typename T> struct IsNetworkType : public std::false_type
{
    TBAG_CONSTEXPR static StreamType const STREAM_TYPE = StreamType::UNKNOWN;
};

template <> struct IsNetworkType<uvpp::Tcp> : public std::true_type
{
    TBAG_CONSTEXPR static StreamType const STREAM_TYPE = StreamType::TCP;
};

template <> struct IsNetworkType<uvpp::Pipe> : public std::true_type
{
    TBAG_CONSTEXPR static StreamType const STREAM_TYPE = StreamType::PIPE;
};

TBAG_API bool isIpv4(std::string const & ip);
TBAG_API bool isIpv6(std::string const & ip);

} // namespace details
} // namespace network

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_NETWORK_DETAILS_NETCOMMON_HPP__

