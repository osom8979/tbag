/**
 * @file   HttpCommon.hpp
 * @brief  HttpCommon class prototype.
 * @author zer0
 * @date   2017-09-30
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_NETWORK_HTTP_COMMON_HTTPCOMMON_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_NETWORK_HTTP_COMMON_HTTPCOMMON_HPP__

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
namespace common  {

TBAG_CONSTEXPR char const * const HTTP = "HTTP";
TBAG_CONSTEXPR char const * const   SP = " ";
TBAG_CONSTEXPR char const * const CRLF = "\r\n";
TBAG_CONSTEXPR char const * const   CR = "\r";
TBAG_CONSTEXPR char const * const   LF = "\n";
TBAG_CONSTEXPR char const * const   HT = "\t";

TBAG_CONSTEXPR int const DEFAULT_HTTP_VERSION_MAJOR = 1;
TBAG_CONSTEXPR int const DEFAULT_HTTP_VERSION_MINOR = 1;

/**
 * @defgroup __DOXYGEN_GROUP__HTTP_HEADER__ List of http headers.
 * @see <https://en.wikipedia.org/wiki/List_of_HTTP_header_fields>
 * @{
 */

TBAG_CONSTEXPR char const * const HEADER_HOST               = "Host";
TBAG_CONSTEXPR char const * const HEADER_UPGRADE            = "Upgrade";
TBAG_CONSTEXPR char const * const HEADER_SERVER             = "Server";
TBAG_CONSTEXPR char const * const HEADER_USER_AGENT         = "User-Agent";
TBAG_CONSTEXPR char const * const HEADER_ACCEPT             = "Accept";
TBAG_CONSTEXPR char const * const HEADER_TRANSFER_ENCODING  = "Transfer-Encoding";
TBAG_CONSTEXPR char const * const HEADER_CONTENT_TYPE       = "Content-Type";
TBAG_CONSTEXPR char const * const HEADER_CONTENT_LENGTH     = "Content-Length";
TBAG_CONSTEXPR char const * const HEADER_ORIGIN             = "Origin";
TBAG_CONSTEXPR char const * const HEADER_CONNECTION         = "Connection";

// WebSocket

TBAG_CONSTEXPR char const * const HEADER_SEC_WEBSOCKET_KEY      = "Sec-WebSocket-Key";
TBAG_CONSTEXPR char const * const HEADER_SEC_WEBSOCKET_ACCEPT   = "Sec-WebSocket-Accept";
TBAG_CONSTEXPR char const * const HEADER_SEC_WEBSOCKET_PROTOCOL = "Sec-WebSocket-Protocol";
TBAG_CONSTEXPR char const * const HEADER_SEC_WEBSOCKET_VERSION  = "Sec-WebSocket-Version";

/**
 * @}
 */

TBAG_CONSTEXPR char const * const VALUE_WEBSOCKET        = "WebSocket";
TBAG_CONSTEXPR char const * const VALUE_UPGRADE          = "Upgrade";
TBAG_CONSTEXPR char const * const VALUE_TBAG_PROTOCOL    = "Tbag";
TBAG_CONSTEXPR char const * const VALUE_TBAG_SERVER_INFO = LIBTBAG_TITLE_STRING "/" LIBTBAG_VERSION_STRING;

TBAG_CONSTEXPR char const * const DEFAULT_VALUE_OF_SERVER            = VALUE_TBAG_SERVER_INFO;
TBAG_CONSTEXPR char const * const DEFAULT_VALUE_OF_CONTENT_TYPE      = "text/html; charset=utf-8";
TBAG_CONSTEXPR char const * const DEFAULT_VALUE_OF_USER_AGENT        = VALUE_TBAG_SERVER_INFO;
TBAG_CONSTEXPR char const * const DEFAULT_VALUE_OF_ACCEPT            = "*/*";
TBAG_CONSTEXPR char const * const DEFAULT_VALUE_OF_TRANSFER_ENCODING = "identity";

TBAG_CONSTEXPR char const * const HEADER_DELIMITER = ":";
TBAG_CONSTEXPR char const * const DEFAULT_VALUE_DELIMITER = ",";

TBAG_CONSTEXPR uint64_t const DEFAULT_HTTP_TIMEOUT_MILLISEC = 15 * 1000;

} // namespace common
} // namespace http
} // namespace network

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_NETWORK_HTTP_COMMON_HTTPCOMMON_HPP__

