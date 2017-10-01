/**
 * @file   HttpMethod.hpp
 * @brief  HttpMethod enum class prototype.
 * @author zer0
 * @date   2017-09-29
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_NETWORK_HTTP_COMMON_HTTPMETHOD_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_NETWORK_HTTP_COMMON_HTTPMETHOD_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/predef.hpp>

#include <string>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace network {
namespace http    {
namespace common  {

#ifndef TBAG_HTTP_METHOD_MAP
#define TBAG_HTTP_METHOD_MAP(_TBAG_XX)      \
    _TBAG_XX( 0, DELETE,      DELETE)       \
    _TBAG_XX( 1, GET,         GET)          \
    _TBAG_XX( 2, HEAD,        HEAD)         \
    _TBAG_XX( 3, POST,        POST)         \
    _TBAG_XX( 4, PUT,         PUT)          \
    /* pathological */                      \
    _TBAG_XX( 5, CONNECT,     CONNECT)      \
    _TBAG_XX( 6, OPTIONS,     OPTIONS)      \
    _TBAG_XX( 7, TRACE,       TRACE)        \
    /* WebDAV */                            \
    _TBAG_XX( 8, COPY,        COPY)         \
    _TBAG_XX( 9, LOCK,        LOCK)         \
    _TBAG_XX(10, MKCOL,       MKCOL)        \
    _TBAG_XX(11, MOVE,        MOVE)         \
    _TBAG_XX(12, PROPFIND,    PROPFIND)     \
    _TBAG_XX(13, PROPPATCH,   PROPPATCH)    \
    _TBAG_XX(14, SEARCH,      SEARCH)       \
    _TBAG_XX(15, UNLOCK,      UNLOCK)       \
    _TBAG_XX(16, BIND,        BIND)         \
    _TBAG_XX(17, REBIND,      REBIND)       \
    _TBAG_XX(18, UNBIND,      UNBIND)       \
    _TBAG_XX(19, ACL,         ACL)          \
    /* subversion */                        \
    _TBAG_XX(20, REPORT,      REPORT)       \
    _TBAG_XX(21, MKACTIVITY,  MKACTIVITY)   \
    _TBAG_XX(22, CHECKOUT,    CHECKOUT)     \
    _TBAG_XX(23, MERGE,       MERGE)        \
    /* upnp */                              \
    _TBAG_XX(24, MSEARCH,     M-SEARCH)     \
    _TBAG_XX(25, NOTIFY,      NOTIFY)       \
    _TBAG_XX(26, SUBSCRIBE,   SUBSCRIBE)    \
    _TBAG_XX(27, UNSUBSCRIBE, UNSUBSCRIBE)  \
    /* RFC-5789 */                          \
    _TBAG_XX(28, PATCH,       PATCH)        \
    _TBAG_XX(29, PURGE,       PURGE)        \
    /* CalDAV */                            \
    _TBAG_XX(30, MKCALENDAR,  MKCALENDAR)   \
    /* RFC-2068, section 19.6.1.2 */        \
    _TBAG_XX(31, LINK,        LINK)         \
    _TBAG_XX(32, UNLINK,      UNLINK)       \
    /* EMD */
#endif

TBAG_CONSTEXPR int const TBAG_UNKNOWN_HTTP_METHOD = -1;

/**
 * List of http method.
 *
 * @author zer0
 * @date 2017-05-25
 */
enum class HttpMethod : int
{
    M_UNKNOWN = TBAG_UNKNOWN_HTTP_METHOD,
#define _TBAG_XX(num, name, str) M_##name = num,
    TBAG_HTTP_METHOD_MAP(_TBAG_XX)
#undef _TBAG_XX
};

#define _TBAG_XX(num, name, str)                                      \
    struct HttpMethod##name                                           \
    {                                                                 \
        TBAG_CONSTEXPR static char const * getMethod() TBAG_NOEXCEPT  \
        {                                                             \
            return #name;                                             \
        }                                                             \
    };                                                                \
    /* -- END -- */
TBAG_HTTP_METHOD_MAP(_TBAG_XX)
#undef _TBAG_XX

static_assert(static_cast<int>(HttpMethod::M_UNKNOWN) == TBAG_UNKNOWN_HTTP_METHOD,
              "Unknown HTTP Method number is not equals.");

// @formatter:off
TBAG_API char const * getHttpMethodName  (HttpMethod method) TBAG_NOEXCEPT;
TBAG_API int          getHttpMethodNumber(HttpMethod method) TBAG_NOEXCEPT;
TBAG_API HttpMethod   getHttpMethod      (std::string const & name);
// @formatter:on

} // namespace common
} // namespace http
} // namespace network

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_NETWORK_HTTP_COMMON_HTTPMETHOD_HPP__

