/**
 * @file   WsOpCode.cpp
 * @brief  WsOpCode class implementation.
 * @author zer0
 * @date   2017-10-01
 */

#include <libtbag/network/http/ws/WsOpCode.hpp>
#include <libtbag/log/Log.hpp>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace network {
namespace http    {
namespace ws      {

char const * getWsOpCodeName(WsOpCode code) TBAG_NOEXCEPT
{
    // @formatter:off
    switch (code) {
    case WsOpCode::WSOC_CONTINUATION_FRAME          : return "WSOC_CONTINUE";
    case WsOpCode::WSOC_TEXT_FRAME                  : return "WSOC_TEXT";
    case WsOpCode::WSOC_BINARY_FRAME                : return "WSOC_BINARY";
    case WsOpCode::WSOC_RESERVED_NON_CONTROL_FRAME_1: return "WSOC_NCF1";
    case WsOpCode::WSOC_RESERVED_NON_CONTROL_FRAME_2: return "WSOC_NCF2";
    case WsOpCode::WSOC_RESERVED_NON_CONTROL_FRAME_3: return "WSOC_NCF3";
    case WsOpCode::WSOC_RESERVED_NON_CONTROL_FRAME_4: return "WSOC_NCF4";
    case WsOpCode::WSOC_RESERVED_NON_CONTROL_FRAME_5: return "WSOC_NCF5";
    case WsOpCode::WSOC_CONNECTION_CLOSE            : return "WSOC_CLOSE";
    case WsOpCode::WSOC_DENOTES_PING                : return "WSOC_PING";
    case WsOpCode::WSOC_DENOTES_PONG                : return "WSOC_PONG";
    case WsOpCode::WSOC_RESERVED_CONTROL_FRAME_1    : return "WSOC_CF1";
    case WsOpCode::WSOC_RESERVED_CONTROL_FRAME_2    : return "WSOC_CF2";
    case WsOpCode::WSOC_RESERVED_CONTROL_FRAME_3    : return "WSOC_CF3";
    case WsOpCode::WSOC_RESERVED_CONTROL_FRAME_4    : return "WSOC_CF4";
    case WsOpCode::WSOC_RESERVED_CONTROL_FRAME_5    : return "WSOC_CF5";
    default: return "WSOC_UNKNOWN";
    }
    // @formatter:on
}

bool isWsControlFrame(WsOpCode code) TBAG_NOEXCEPT
{
    // @formatter:off
    return (WsOpCode::WSOC_RESERVED_NON_CONTROL_FRAME_1 <= COMPARE_AND(code) <= WsOpCode::WSOC_RESERVED_NON_CONTROL_FRAME_5) ||
           (WsOpCode::WSOC_RESERVED_CONTROL_FRAME_1     <= COMPARE_AND(code) <= WsOpCode::WSOC_RESERVED_CONTROL_FRAME_5);
    // @formatter:on
}

} // namespace ws
} // namespace http
} // namespace network

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

