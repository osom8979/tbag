/**
 * @file   WsOpCode.hpp
 * @brief  WsOpCode class prototype.
 * @author zer0
 * @date   2017-10-01
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_NETWORK_HTTP_WS_WSOPCODE_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_NETWORK_HTTP_WS_WSOPCODE_HPP__

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

enum class WsOpCode : uint8_t
{
    WSOC_CONTINUATION_FRAME           = 0x0,
    WSOC_TEXT_FRAME                   = 0x1,
    WSOC_BINARY_FRAME                 = 0x2,
    WSOC_RESERVED_NON_CONTROL_FRAME_1 = 0x3,
    WSOC_RESERVED_NON_CONTROL_FRAME_2 = 0x4,
    WSOC_RESERVED_NON_CONTROL_FRAME_3 = 0x5,
    WSOC_RESERVED_NON_CONTROL_FRAME_4 = 0x6,
    WSOC_RESERVED_NON_CONTROL_FRAME_5 = 0x7,
    WSOC_CONNECTION_CLOSE             = 0x8,
    WSOC_DENOTES_PING                 = 0x9,
    WSOC_DENOTES_PONG                 = 0xA,
    WSOC_RESERVED_CONTROL_FRAME_1     = 0xB,
    WSOC_RESERVED_CONTROL_FRAME_2     = 0xC,
    WSOC_RESERVED_CONTROL_FRAME_3     = 0xD,
    WSOC_RESERVED_CONTROL_FRAME_4     = 0xE,
    WSOC_RESERVED_CONTROL_FRAME_5     = 0xF,
};

TBAG_API char const * getWsOpCodeName(WsOpCode code) TBAG_NOEXCEPT;
TBAG_API bool isWsControlFrame(WsOpCode code) TBAG_NOEXCEPT;

} // namespace ws
} // namespace http
} // namespace network

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_NETWORK_HTTP_WS_WSOPCODE_HPP__

