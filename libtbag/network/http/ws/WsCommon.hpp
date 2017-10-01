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
#include <libtbag/debug/Assert.hpp>

#include <cstdint>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace network {
namespace http    {
namespace ws      {

TBAG_CONSTEXPR int const WEBSOCKET_VERSION_HYBI00 =  0; ///< Interim
TBAG_CONSTEXPR int const WEBSOCKET_VERSION_HYBI07 =  7; ///< Interim
TBAG_CONSTEXPR int const WEBSOCKET_VERSION_HYBI08 =  8; ///< Interim
TBAG_CONSTEXPR int const WEBSOCKET_VERSION_HYBI13 = 13; ///< RFC 6455 (Standard)

TBAG_CONSTEXPR char const * const WEBSOCKET_HANDSHAKE_GUID = "258EAFA5-E914-47DA-95CA-C5AB0DC85B11";

} // namespace ws
} // namespace http
} // namespace network

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_NETWORK_HTTP_WS_WSCOMMON_HPP__

