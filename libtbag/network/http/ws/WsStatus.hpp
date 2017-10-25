/**
 * @file   WsStatus.hpp
 * @brief  WsStatus class prototype.
 * @author zer0
 * @date   2017-10-01
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_NETWORK_HTTP_WS_WSSTATUS_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_NETWORK_HTTP_WS_WSSTATUS_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/predef.hpp>
#include <libtbag/Err.hpp>
#include <libtbag/util/BufferInfo.hpp>

#include <cstdint>
#include <string>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace network {
namespace http    {
namespace ws      {

/**
 * @def TBAG_WEB_SOCKET_STATUS_CODE_MAP
 *
 * WebSocket Status Codes
 * Reference: https://tools.ietf.org/html/rfc6455#section-7.4
 */
#ifndef TBAG_WEB_SOCKET_STATUS_CODE_MAP
#define TBAG_WEB_SOCKET_STATUS_CODE_MAP(_TBAG_XX) \
    _TBAG_XX(1000, NORMAL_CLOSURE         , "Normal closure"         ) \
    _TBAG_XX(1001, GOING_AWAY             , "Going away"             ) \
    _TBAG_XX(1002, PROTOCOL_ERROR         , "Protocol error"         ) \
    _TBAG_XX(1003, CANNOT_ACCEPT          , "Cannot accept"          ) \
    _TBAG_XX(1004, RESERVED               , "Reserved"               ) \
    _TBAG_XX(1005, NO_STATUS              , "No status"              ) \
    _TBAG_XX(1006, ABNORMAL_CLOSE         , "Abnormal close"         ) \
    _TBAG_XX(1007, INVALID_PAYLOAD        , "Invalid payload"        ) \
    _TBAG_XX(1008, POLICY_VIOLATION       , "Policy violation"       ) \
    _TBAG_XX(1009, MESSAGE_TOO_BIG        , "Message too big"        ) \
    _TBAG_XX(1010, EXTENSION_REQUIRED     , "Extension required"     ) \
    _TBAG_XX(1011, INTERNAL_ENDPOINT_ERROR, "Internal endpoint error") \
    _TBAG_XX(1015, TLS_HANDSHAKE          , "TLS handshake"          ) \
    _TBAG_XX(3000, UNKNOWN_ERROR          , "Unknown error"          ) \
    _TBAG_XX(3001, CLIENT_TIMER_ERROR     , "Client timer error"     ) \
    /* END */
#endif

TBAG_CONSTEXPR uint16_t const TBAG_UNKNOWN_WEBSOCKET_STATUS_CODE = 0;

/**
 * List of WebSocket status code.
 *
 * @author zer0
 * @date 2017-07-07
 * @date 2017-08-08 (Rename: WebSocketStatusCode -> WsStatusCode)
 */
enum class WsStatusCode : uint16_t
{
    WSSC_UNKNOWN = TBAG_UNKNOWN_WEBSOCKET_STATUS_CODE,
#define _TBAG_XX(num, name, str) WSSC_##name = num,
    TBAG_WEB_SOCKET_STATUS_CODE_MAP(_TBAG_XX)
#undef _TBAG_XX
};

TBAG_API uint16_t     getWsStatusCodeNumber(WsStatusCode code) TBAG_NOEXCEPT;
TBAG_API char const * getWsStatusCodeName  (WsStatusCode code) TBAG_NOEXCEPT;
TBAG_API char const * getWsStatusCodeReason(WsStatusCode code) TBAG_NOEXCEPT;
TBAG_API WsStatusCode getWsStatusCode      (uint16_t     code) TBAG_NOEXCEPT;

// WebSocket - Reserved Status Code Ranges
// Reference: https://tools.ietf.org/html/rfc6455#section-7.4.2

/** Status codes in the range 0-999 are not used. */
inline bool isWsStatusCodeNotUsed(uint16_t code) TBAG_NOEXCEPT
{
    return 0 <= COMPARE_AND(code) <= 999;
}

/**
 * Status codes in the range 1000-2999 are reserved for definition by
 * this protocol, its future revisions, and extensions specified in a
 * permanent and readily available public specification.
 */
inline bool isWsStatusCodePublicSpecification(uint16_t code) TBAG_NOEXCEPT
{
    return 1000 <= COMPARE_AND(code) <= 2999;
}

/**
 * Status codes in the range 3000-3999 are reserved for use by
 * libraries, frameworks, and applications. These status codes are
 * registered directly with IANA. The interpretation of these codes
 * is undefined by this protocol.
 */
inline bool isWsStatusCodeIANA(uint16_t code) TBAG_NOEXCEPT
{
    return 3000 <= COMPARE_AND(code) <= 3999;
}

/**
 * Status codes in the range 4000-4999 are reserved for private use
 * and thus can't be registered.  Such codes can be used by prior
 * agreements between WebSocket applications.  The interpretation of
 * these codes is undefined by this protocol.
 */
inline bool isWsStatusCodeUserDefined(uint16_t code) TBAG_NOEXCEPT
{
    return 4000 <= COMPARE_AND(code) <= 4999;
}

/**
 * WebSocket close result structure.
 *
 * @author zer0
 * @date 2017-07-07
 */
struct TBAG_API WsStatus
{
    uint16_t    code;
    std::string reason;

    WsStatus();
    WsStatus(uint16_t c);
    WsStatus(uint16_t c, std::string const & r);

    explicit WsStatus(char const * buffer, std::size_t size);
    explicit WsStatus(util::Buffer const & payload);

    explicit WsStatus(WsStatusCode s);
    ~WsStatus();

    void setWsStatusCode(WsStatusCode s);
    WsStatusCode getWsStatusCode() const TBAG_NOEXCEPT;

    Err parse(char const * buffer, std::size_t size);
    Err parse(util::Buffer const & payload);

    static Err getStatusCode(char const * payload_begin, std::size_t payload_length, uint16_t * result);
    static Err     getReason(char const * payload_begin, std::size_t payload_length, std::string * result);

    std::string toString() const;
};

} // namespace ws

using namespace libtbag::network::http::ws;

} // namespace http
} // namespace network

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_NETWORK_HTTP_WS_WSSTATUS_HPP__

