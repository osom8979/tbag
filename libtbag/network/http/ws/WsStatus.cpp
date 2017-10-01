/**
 * @file   WsStatus.cpp
 * @brief  WsStatus class implementation.
 * @author zer0
 * @date   2017-10-01
 */

#include <libtbag/network/http/ws/WsStatus.hpp>
#include <libtbag/log/Log.hpp>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace network {
namespace http    {
namespace ws      {

uint16_t getWsStatusCodeNumber(WsStatusCode code) TBAG_NOEXCEPT
{
    switch (code) {
#define _TBAG_XX(num, name, str) case WsStatusCode::WSSC_##name: return num;
    TBAG_WEB_SOCKET_STATUS_CODE_MAP(_TBAG_XX)
#undef _TBAG_XX
    default: return TBAG_UNKNOWN_WEBSOCKET_STATUS_CODE;
    }
}

char const * getWsStatusCodeName(WsStatusCode code) TBAG_NOEXCEPT
{
    switch (code) {
#define _TBAG_XX(num, name, str) case WsStatusCode::WSSC_##name: return #name;
    TBAG_WEB_SOCKET_STATUS_CODE_MAP(_TBAG_XX)
#undef _TBAG_XX
    default: return "WSSC_UNKNOWN";
    }
}

char const * getWsStatusCodeReason(WsStatusCode code) TBAG_NOEXCEPT
{
    switch (code) {
#define _TBAG_XX(num, name, str) case WsStatusCode::WSSC_##name: return str;
    TBAG_WEB_SOCKET_STATUS_CODE_MAP(_TBAG_XX)
#undef _TBAG_XX
    default: return "Unknown reason";
    }
}

WsStatusCode getWsStatusCode(uint16_t code) TBAG_NOEXCEPT
{
    switch (code) {
#define _TBAG_XX(num, name, str) case num: return WsStatusCode::WSSC_##name;
    TBAG_WEB_SOCKET_STATUS_CODE_MAP(_TBAG_XX)
#undef _TBAG_XX
    default: return WsStatusCode::WSSC_UNKNOWN;
    }
}

} // namespace ws
} // namespace http
} // namespace network

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

