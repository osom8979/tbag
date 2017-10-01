/**
 * @file   WsCommon.hpp
 * @brief  WsCommon class prototype.
 * @author zer0
 * @date   2017-10-01
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_NETWORK_HTTP_WS_WSCOMMON_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_NETWORK_HTTP_WS_WSCOMMON_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/predef.hpp>

#include <cstdint>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace network {
namespace http    {
namespace ws      {

enum class WsPayloadBit : uint8_t
{
    WSPL_BIT_7  =  7,
    WSPL_BIT_16 = 16,
    WSPL_BIT_64 = 64,
};

TBAG_CONSTEXPR uint8_t const WS_PAYLOAD_7BIT_TYPE_SIZE  = 125;
TBAG_CONSTEXPR uint8_t const WS_PAYLOAD_16BIT_TYPE_SIZE = 126;
TBAG_CONSTEXPR uint8_t const WS_PAYLOAD_64BIT_TYPE_SIZE = 127;

} // namespace ws
} // namespace http
} // namespace network

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_NETWORK_HTTP_WS_WSCOMMON_HPP__

