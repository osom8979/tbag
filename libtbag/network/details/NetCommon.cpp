/**
 * @file   NetCommon.cpp
 * @brief  NetCommon class implementation.
 * @author zer0
 * @date   2017-05-03
 */

#include <libtbag/network/details/NetCommon.hpp>
#include <libtbag/string/StringUtils.hpp>
#include <libtbag/log/Log.hpp>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace network {
namespace details {

#ifndef IPV4SEG
#define IPV4SEG "(25[0-5]|(2[0-4]|1{0,1}[0-9]){0,1}[0-9])"
#endif

#ifndef IPV4REGEX
#define IPV4REGEX "(" IPV4SEG "\\.){3,3}" IPV4SEG
#endif

#ifndef IPV6SEG
#define IPV6SEG "[0-9a-fA-F]{1,4}"
#endif

#define IPV6REGEX01 "(" IPV6SEG ":){7,7}" IPV6SEG
#define IPV6REGEX02 "(" IPV6SEG ":){1,7}:"
#define IPV6REGEX03 "(" IPV6SEG ":){1,6}(:" IPV6SEG "){1,1}"
#define IPV6REGEX04 "(" IPV6SEG ":){1,5}(:" IPV6SEG "){1,2}"
#define IPV6REGEX05 "(" IPV6SEG ":){1,4}(:" IPV6SEG "){1,3}"
#define IPV6REGEX06 "(" IPV6SEG ":){1,3}(:" IPV6SEG "){1,4}"
#define IPV6REGEX07 "(" IPV6SEG ":){1,2}(:" IPV6SEG "){1,5}"
#define IPV6REGEX08 "(" IPV6SEG ":){1,1}(:" IPV6SEG "){1,6}"
#define IPV6REGEX09 ":((:" IPV6SEG "){1,7}|:)"

/**
 * @def IPV6REGEX10
 *
 * link-local IPv6 addresses with zone index.
 */
#define IPV6REGEX10 "fe80:(:[0-9a-fA-F]{0,4}){0,4}%[0-9a-zA-Z]{1,}"

/**
 * @def IPV6REGEX11
 *
 * IPv4-mapped IPv6 addresses and IPv4-translated addresses.
 */
#define IPV6REGEX11 "::(ffff(:0{1,4}){0,1}:){0,1}" IPV4REGEX

/**
 * @def IPV6REGEX12
 *
 * IPv4-Embedded IPv6 Address.
 */
#define IPV6REGEX12 "(" IPV6SEG ":){1,4}:" IPV4REGEX

bool isIpv4(std::string const & ip)
{
    return string::isMatch(ip, IPV4REGEX);
}

bool isIpv6(std::string const & ip)
{
    // @formatter:off
    if (string::isMatch(ip, IPV6REGEX01)) { return true; } // 1:2:3:4:5:6:7:8
    if (string::isMatch(ip, IPV6REGEX02)) { return true; } // 1::                                 1:2:3:4:5:6:7::
    if (string::isMatch(ip, IPV6REGEX03)) { return true; } // 1::8               1:2:3:4:5:6::8   1:2:3:4:5:6::8
    if (string::isMatch(ip, IPV6REGEX04)) { return true; } // 1::7:8             1:2:3:4:5::7:8   1:2:3:4:5::8
    if (string::isMatch(ip, IPV6REGEX05)) { return true; } // 1::6:7:8           1:2:3:4::6:7:8   1:2:3:4::8
    if (string::isMatch(ip, IPV6REGEX06)) { return true; } // 1::5:6:7:8         1:2:3::5:6:7:8   1:2:3::8
    if (string::isMatch(ip, IPV6REGEX07)) { return true; } // 1::4:5:6:7:8       1:2::4:5:6:7:8   1:2::8
    if (string::isMatch(ip, IPV6REGEX08)) { return true; } // 1::3:4:5:6:7:8     1::3:4:5:6:7:8   1::8
    if (string::isMatch(ip, IPV6REGEX09)) { return true; } // ::2:3:4:5:6:7:8    ::2:3:4:5:6:7:8  ::8
    if (string::isMatch(ip, IPV6REGEX10)) { return true; } // fe08::7:8%eth0     fe08::7:8%1
    if (string::isMatch(ip, IPV6REGEX11)) { return true; } // ::255.255.255.255  ::ffff:0:255.255.255.255
    if (string::isMatch(ip, IPV6REGEX12)) { return true; } // 2001:db8:3:4::192.0.2.33  64:ff9b::192.0.2.33
    // @formatter:on
    return false;
}

} // namespace details
} // namespace network

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

